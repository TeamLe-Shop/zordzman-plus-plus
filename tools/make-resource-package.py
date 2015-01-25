#!/usr/bin/env python3

"""Script to build resource packages from multiple sources."""


import argparse
import hashlib
import io
import json
import logging
import pathlib
import tarfile


log = logging.getLogger("make-resource-package")
RESOURCE_TYPE_SCHEMAS = {
    "sprite": {
        "path": pathlib.Path,
        "x": int,
        "y": int,
        "width": int,
        "height": int,
    },
    "music": {
        "path": pathlib.Path,
    },
}


def validate_resource(resource, path):
    if "type" not in resource or not isinstance(resource["type"], str):
        raise ValueError("Resource missing 'type' field or wrong type")
    if "name" not in resource or not isinstance(resource["name"], str):
        raise ValueError("Resource missing 'name' field or wrong type")
    resource_name = resource["name"]
    resource_type = resource["type"]
    if resource_type not in RESOURCE_TYPE_SCHEMAS:
        raise ValueError("Unknown resource type {!r}".format(resource_type))
    schema = RESOURCE_TYPE_SCHEMAS[resource_type]
    resource = {field: schema[field](value)
                for field, value in resource.items() if field in schema}
    # Resolve paths
    for field, value in resource.items():
        if isinstance(value, pathlib.Path):
            resource[field] = (path / value).resolve()
    resource["name"] = resource_name
    resource["type"] = resource_type
    return resource


def load_fragments(fragment_paths):
    resources = []
    for path in fragment_paths:
        log.info("Loading fragment {}".format(path))
        with path.open() as fp:
            for i, resource in enumerate(json.load(fp)):
                log.info("Validating {} {}".format(path, i))
                resources.append(validate_resource(resource, path.parent))
    return resources


def build(output, fragments):
    names = {}
    paths = set()
    resources = load_fragments(fragments)
    with tarfile.open(str(output), "w") as package:
        log.info("Writing resource package to {}".format(output))
        for resource in resources:
            if resource["type"] not in names:
                names[resource["type"]] = set()
            if resource["name"] in names[resource["type"]]:
                raise ValueError("Multiple resources of type {type!r} "
                                 "with name {name!r}".format(**resource))
            names[resource["type"]].add(resource["name"])
            with resource["path"].open("rb") as fp:
                # TODO: Don't read everything into memory
                package_name = (hashlib.sha1(fp.read()).hexdigest()
                                + "_" + resource["path"].stem)
            if resource["path"] not in paths:
                log.info("Add {path} as {0}".format(package_name, **resource))
                package.add(str(resource["path"]), package_name)
                paths.add(resource["path"])
            resource["path"] = package_name
        log.info("Writing {} ({} files) resources "
                 "to manifest".format(len(resources), len(paths)))
        manifest_file = io.BytesIO(json.dumps(resources, indent=2).encode())
        manifest = tarfile.TarInfo(name="manifest")
        manifest.size = len(manifest_file.getvalue())
        package.addfile(manifest, manifest_file)


def main():
    logging.basicConfig(level=logging.DEBUG)
    parser = argparse.ArgumentParser(
        description="Build a resource package from multiple sources.")
    parser.add_argument(
        "fragments",
        metavar="FRAGMENT",
        nargs="+",
        type=pathlib.Path,
        help="A manifest fragment.",
    )
    parser.add_argument(
        "--output",
        default="resources.tar",
        type=pathlib.Path,
        help="Destination for the generated resource package.",
    )
    build(**vars(parser.parse_args()))
    return 0


if __name__ == "__main__":
    exit(main())
