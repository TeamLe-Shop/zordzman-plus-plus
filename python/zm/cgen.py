"""Utility for templating C++ from message pipelines.

This runnable module implements a command-line utility which is used for
templating C++ with Jinja2 in order to automatically generate mappings
between Python-defined message types and C++ structures.

This modules takes two positional command-line arguments:

``pipeline``
    The name of the message pipeline subclass to use.

``template``
    The path to a Jinja2 template.

    The Jinja envrionment uses a file-system loader in order to load the
    template. The root path of the loader is the directory that contains the
    template, therefore it is possible to include files from the same
    directory (and subdirectories) as the template.

When rendering the template the following context is available:

``namespaces``
    This is a list of :class:`Namespace`s. Each namespace has a name, some
    accompanying documentation, a list of sub-namespaces and most importantly
    a list of :class:`Struct`s.

    It is the structures which represent each message type. Each struct has
    a name, it's own documentation and and a list of :class:`Field`s. And
    in-turn each field has a name and a type. The type is a Python primitive
    type, e.g. ``int`` or ``str``.

``int``
    The built-in ``int`` type.

``str``
    The built-in ``str`` type.

``float``
    The built-in ``float`` type.

``object``
    The built-in ``object`` type.

The Jinja envrionment used to render the template does not use the default
Jinja sytax, e.g. ``{{``, ``{%`` and ``{#``. As this module is intended to
be used to template C++, instead C-style comments are used. Hence the
aforementioned become ``/*{``, ``/*%`` and ``/*#`` respectively. This is done
so that the template plays nicer with syntax highlighting.

Finally, an additional filter is made available to the template on top of the
built-in Jinja ones:

``doxygen``
    Converts strings to Doxygen docstrings.

The rendered template is printed to stdout.
"""


import collections
import argparse
import pathlib
import sys

import formencode
import jinja2

import zm.pipeline


# Maps FormEncode validators to their to_python return type which should
# be a primitive, e.g. int, string or float.
FE_TYPES = {
    formencode.validators.Int: int,
    formencode.validators.String: str,
    formencode.validators.IPAddress: str,
}


class CGenError(Exception):
    """Base exception raised for C++ generation errors."""


Namespace = collections.namedtuple(
    "Namespace",
    (
        "name",
        "doc",
        "namespaces",
        "structs",
    )
)


Struct = collections.namedtuple(
    "Struct",
    (
        "name",
        "doc",
        "fields",
    )
)


Field = collections.namedtuple(
    "Field",
    (
        "name",
        "type",
    )
)


def _filter_doxygen(string):
    """Convert string to a Doxygen comment."""
    lines = []
    for line in string.splitlines():
        lines.append("/// " + line)
    return "\n".join(lines)



def _normalise_docstring(doc):
    """Convert a multi-line docstring to a single line string."""
    lines = []
    for line in doc.splitlines():
        lines.append(line.strip())
    return "".join(lines)


def _generate_fields(schema):
    """Generate :class:`Field`s from a FormEncode schema.

    Given a :class:`formencode.Schema` this will inspect the schema's field
    and generate a sorted list of corresponding :class:`Field`s. Each field
    will have the same name as it occurs in the schema and a type that deduced
    from the field's validator.

    The field type requires a look-up into :data:`FE_TYPES`. If the type for
    a validator is unknown then :exc:`CGenError` is raied.

    Alternately, if the validator is a :class:`formencode.Pipe` and the last
    validator in the pipe is a :class:`formencode.validators.ConfirmType`
    then that type will be used. If pipe validator doesn't end with a
    ``ConfirmType`` then a :exc:`CGenError` will be raised. :exc:`CGenError`
    is also raised if the type confirmation validator has specifies multiple
    types.

    The returned fields are sorted by name.

    :params schema: the :class:`formencode.Schema` to generate fields for.

    :returns: a list of :class:`Field`s.
    """
    fields = []
    for field_name, fe_field in schema.fields.items():
        if isinstance(fe_field, formencode.Pipe):
            confirm = fe_field.validators[-1]
            if not isinstance(confirm, formencode.validators.ConfirmType):
                raise CGenError(
                    "{}.{}'s validator is a Pipe but is missing "
                    "a terminating ConfirmType validator".format(
                        schema.__name__, field_name))
            if len(confirm.type) != 1:
                raise CGenError("Pipe-terminating ConfirmType validators "
                                "should only have 1 type, {} given: {}".format(
                                    len(confirm.type), confirm.type))
            type_ = confirm.type[0]
        else:
            try:
                type_ = FE_TYPES[fe_field.__class__]
            except KeyError as exc:
                raise CGenError(
                    "Cannot determine type of field {}.{} with validator "
                    "{!r}".format(schema.__name__,
                                  field_name,
                                  fe_field.__class__.__name__)) from exc
        fields.append(Field(name=field_name, type=type_))
    return sorted(fields, key=lambda f: f.name)


def _generate_struct(message_type):
    """Generate a structure from a :class:`zm.pipeline.MessageType`.

    The name of the structure is derived from the message type name. The type
    name is split on dots and made camel-case. For example, ``foo.bar`` would
    become ``FooBar``.

    Optionally, the message type name can be namespaced by separating the
    namespaces by colons. For example, ``baz:foo.bar`` is said to be in the
    namespace ``baz`` with the structure name of ``FooBar``. Multiple
    namespaces are allowed, such as ``baz:qux:foo.bar``. In such a case
    the ``FooBar`` structure exists in the ``baz:qux`` namespace.

    The namespaces are returned as a list of the namespaces, so that
    ``baz:qux`` becomes ``["baz", "qux"]``.

    The doc of the structure is taken from the ``__doc__`` of the message
    type.

    :params message_type: a :class:`zm.pipeline.MessageType` object to be
        converted to a :class:`Struct`.

    :returns: a tuple containg the list of namespaces and the :class:`Struct`.
    """
    name_split = message_type.name.split(":")
    namespaces = name_split[:-1]
    name = "".join(part.capitalize() for part in name_split[-1].split("."))
    return namespaces, Struct(
        name=name,
        doc=_normalise_docstring(message_type.__doc__),
        fields=_generate_fields(message_type._schema),
    )


def _find_namespace(namespace, names):
    """Find or created a nested namespace.

    Given a top-level namespace, this will search for a sub-namespace which
    matches the given list of namespace names. If a namespace is missing it
    will be created and added to its parent namespace.

    :params Namespace namespace: the top-level namespace to begin searching
        from.
    :params names: a list of namespace names.

    :returns: the :class:`Namespace` which matches the list of namespace
        names.
    """
    for name in names:
        try:
            namespace = [ns for ns
                         in namespace.namespaces if ns.name == name][0]
        except IndexError:
            new_ns = Namespace(
                name=name,
                doc="",
                namespaces=[],
                structs=[],
            )
            namespace.namespaces.append(new_ns)
            namespace = new_ns
    return namespace


def _generate_namespaces(pipeline):
    """Generate a namespace for a pipline.

    This will produce a :class:`Namespace` for a given message pipeline that
    includes :class:`Struct`s for all the message types registered to the
    pipeline.

    The namespace may include nested namespaces if the message types are
    namespaced using the ``namespace:struct`` notation.

    The namespace will inherit its documentation from the pipeline's
    docstring. The namespace's name is just the lowercase form of the
    pipeline's name.

    :param pipeline: a :class:`zm.pipeline.Pipline`-dervied class to build
        a namespace for.

    :returns: a :class:`Namespace`.
    """
    top = Namespace(name=pipeline.__name__.lower(),
                    doc=_normalise_docstring(pipeline.__doc__),
                    namespaces=[], structs=[])
    for message_type in pipeline.types:
        namespaces, struct = _generate_struct(message_type)
        _find_namespace(top, namespaces).structs.append(struct)
    return top


def _pipeline(class_name):
    """Convert a pipeline name to an actual pipeline class.

    This searches :class:`zm.pipeline.Pipeline`'s subclasses to find a
    matching subclass.

    :raises argparse.ArgumentTypeError: if the requested class does not exist.

    :returns: a :class:`zm.pipeline.Pipeline` subclass.
    """
    for cls in zm.pipeline.Pipeline.__subclasses__():
        if cls.__name__ == class_name:
            return cls
    raise argparse.ArgumentTypeError(
        "Unknown pipeline {!r}".format(class_name))


def _parse_args(argv=None):
    """Parse command line arguments."""
    parser = argparse.ArgumentParser(prog="cgen")
    parser.add_argument(
        "pipeline",
        type=_pipeline,
        help="name of the message pipeline to use",
    )
    parser.add_argument(
        "template",
        type=pathlib.Path,
        help="path to a Jinja2 template",
    )
    return parser.parse_args(argv)


def main(argv=None):
    """Application entry point.

    :returns: an exit status code.
    """
    args = _parse_args(argv)
    env = jinja2.Environment(
        loader=jinja2.FileSystemLoader(str(args.template.parent)),
        block_start_string="/*%",
        block_end_string="%*/",
        variable_start_string="/*{",
        variable_end_string="}*/",
        comment_start_string="/*#",
        comment_end_string="#*/",
    )
    env.filters["doxygen"] = _filter_doxygen
    template = env.get_template(args.template.name)
    print(template.render(
        namespaces=[_generate_namespaces(args.pipeline)],
        int=int,
        str=str,
        float=float,
        object=object,
    ))
    return 0


if __name__ == "__main__":
    sys.exit(main())
