import pathlib

import formencode
import jinja2

import zm.client


def filter_type(type_):
    return "".join(s.capitalize() for s in type_.split("."))


def filter_callbacks(type_):
    return "m_{}_callbacks".format(filter_type(type_))


def filter_entity(type_):
    return filter_type(type_)


def filter_ctype(type_):
    return TYPE_CONVERSIONS[type_]


def filter_converter(struct_or_field):
    if isinstance(struct_or_field, Struct):
        return "_to_" + struct.name
    else:
        field = struct_or_field
        if field.type is not dict:
            return PRIMITIVE_CONVERTERS[field.type]
        else:
            return "HALP"


if __name__ == "__main__":
    import collections

    PYOBJECT = object()

    TYPE_CONVERSIONS = {
        PYOBJECT: "PyObject*",
        int: "long",
        float: "double",
        str: "std::string",
    }

    PRIMITIVE_CONVERTERS = {
        PYOBJECT: "_pyobject_as_native",
        int: "_int_as_native",
        float: "_float_as_native",
        str: "_str_as_native",
    }

    Struct = collections.namedtuple(
        "Struct",
        (
            "name",
            "fields",
        ),
    )

    StructField = collections.namedtuple(
        "StructField",
        (
            "name",
            "type",
            "struct",
        )
    )

    MessageType = collections.namedtuple(
        "MessageType",
        (
            "type",
            "struct",
        )
    )

    structs = []
    types = []

    for mtype, schema in zm.client.MESSAGE_TYPES.items():
        schema = schema()
        name = schema.__class__.__name__
        if name.endswith("Schema"):
            name = name[:-len("Schema")]
        # print(name)
        # print(schema.fields)
        fields = []
        for field, validator in schema.fields.items():
            type_ = None
            if isinstance(validator, formencode.validators.String):
                type_ = str
            elif isinstance(validator, formencode.validators.Int):
                type_ = int
            elif isinstance(validator, formencode.Any):
                type_ = PYOBJECT
            if type_:
                fields.append(StructField(field, type_, None))
        fields.sort(key=lambda f: len(f.name))
        struct = Struct(name, tuple(fields))
        structs.append(struct)
        types.append(MessageType(mtype, struct))
        # print(struct)

    src_dir = pathlib.Path(__file__).parent.parent
    template_path = src_dir / "python_i.hpp.in"
    env = jinja2.Environment(loader=jinja2.FileSystemLoader(str(src_dir)))
    env.filters["type_identifier"] = filter_type
    env.filters["type_name"] = filter_type
    env.filters["callbacks"] = filter_callbacks
    env.filters["converter"] = filter_converter
    env.filters["entity"] = filter_entity
    env.filters["ctype"] = filter_ctype
    template = env.get_template(str(template_path.relative_to(src_dir)))
    out = template.render(types=types, structs=structs)
    print(out)
