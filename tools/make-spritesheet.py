#!/usr/bin/env python3

"""Script for combining multiple sprites into a single spritesheet."""


import argparse
import json
import logging
import os.path
import pathlib
import sys

import PIL.Image as image


log = logging.getLogger("make-spritesheet")


def load_sprite(path):
    """Load a sprite from a metadata file path.

    The sprite metadata file should be a JSON object with a 'name' and 'path'
    fields. The 'name' should be a string that uniquely identifies the sprite.
    The 'path' should be the path to the sprite's image file relative to the
    metadata file.

    This metadata dictionary is modified to have the following additional
    fields:

        * `image` -- a Pillow image as loaded from `path`.
        * `width` -- the width of the sprite image.
        * `height` -- the height of the sprite image.

    :returns: the sprite metadata dictionary.
    """
    log.info("Loading sprite from {}".format(path))
    sprite = {}
    with path.open() as meta_fp:
        sprite = json.load(meta_fp)
    if not isinstance(sprite.get("name"), str):
        raise ValueError(
            "Sprite missing 'name' field or the name is not a string")
    sprite["path"] = path.parent / pathlib.Path(sprite["path"])
    log.info("Loading sprite image from {}".format(sprite["path"]))
    sprite["image"] = image.open(str(sprite["path"]))
    sprite["width"] = sprite["image"].size[0]
    sprite["height"] = sprite["image"].size[1]
    return sprite


def layout_sprites(sprites):
    """Arrange the sprites on the spritesheet.

    This adds the 'x' and 'y' to each sprite dictionary in `sprites` which
    signifies the position of the sprite on the spritesheet.

    The layout algorithm is not very sophisticated. It sorts sprites with
    the same height into rows, where the first (top) row has the shortest
    sprites. Then, the sprites are further arranged so that the widest sprites
    on a particular row are placed rightmost.

    As well as determining the position of each sprite, the required size of
    the sheet is also calculated.

    :returns: the width and height of the spritesheet.
    """
    rows = {}
    for sprite in sprites:
        if sprite["height"] not in rows:
            rows[sprite["height"]] = []
        rows[sprite["height"]].append(sprite)
    sheet_width = 0
    sheet_height = sum(rows.keys())
    for row, sprites in rows.items():
        sheet_width = max(sheet_width, sum(s["width"] for s in sprites))
        rows[row] = sorted(sprites, key=lambda s: s["width"])
    rows = [row for height, row in sorted(rows.items(), key=lambda r: r[0])]
    # Set the `x` and `y` fields for each sprite
    x, y = 0, 0
    for row in rows:
        x = 0
        for sprite in row:
            sprite["x"], sprite["y"] = x, y
            log.info("Placed {path} at {x}, {y}".format(**sprite))
            x += sprite["width"]
        y += sprite["height"]
    return sheet_width, sheet_height


def render_sheet(size, sprites):
    """Render sprites to a sheet in-memory.

    This creates a new image of the given `size` in RGBA mode. Each sprite
    image is then copied onto the sheet based on the setting of the 'x' and
    'y' fields of the sprite dictionary.

    Sprite images are converted to RGBA if they are not already.

    :returns: the Pillow image of the spritesheet.
    """
    log.info("Creating sheet {}x{}, RGBA".format(*size))
    sheet = image.new("RGBA", size)
    for sprite in sprites:
        dest = (
            sprite["x"],
            sprite["y"],
            sprite["x"] + sprite["width"],
            sprite["y"] + sprite["height"],
        )
        log.info("Copying {path} "
                 "({image.mode}) to sheet {0}".format(dest, **sprite))
        sprite_source = sprite["image"].copy()
        sprite_source.convert("RGBA")
        sheet.paste(sprite_source, dest)
    return sheet


def write_manifest_fragment(manifest, sheet, sprites):
    """Write a resource package manifest for the sprites.

    This writes a JSON resource manifest to the given destination. As its
    a manifest, it is an array of objects. Each object describes the sprite
    based on the sprite dictionary state.

    Each object has the following fields:

     * `type` -- always 'sprite'.
     * `name` -- the name of the sprite taken from the sprite metadata file.
     * `path` -- the path to the spritesheet relative to the manifest.
     * `x` -- X coordinate of the sprite on the sheet as an integer.
     * `y` -- Y coordinate of the sprite on the sheet as an integer.
     * `width` -- the width of the sprite as an integer.
     * `height` -- the height of the sprite as an integer.
    """
    fragment = []
    for sprite in sprites:
        fragment.append({
            "type": "sprite",
            "name": sprite["name"],
            "path": os.path.relpath(str(sheet), str(manifest.parent)),
            "x": sprite["x"],
            "y": sprite["y"],
            "width": sprite["width"],
            "height": sprite["height"],
        })
    with manifest.open("w") as fp:
        json.dump(fragment, fp, indent=2)
    log.info("Wrote manifest fragment to {}".format(manifest))


def build(sprites, manifest, output):
    sprites = [load_sprite(sprite) for sprite in sprites]
    size = layout_sprites(sprites)
    render_sheet(size, sprites).save(str(output))
    write_manifest_fragment(manifest, output, sprites)


def main(argv):
    logging.basicConfig(level=logging.INFO)
    parser = argparse.ArgumentParser(
        description="Build a sprite sheet from multiple, smaller images.")
    parser.add_argument(
        "sprites",
        metavar="SPRITE",
        nargs="+",
        type=pathlib.Path,
        help="A sprite descriptor file.",
    )
    parser.add_argument(
        "--output",
        default="spritesheet.png",
        type=pathlib.Path,
        help="Destination for the generated spritesheet.",
    )
    parser.add_argument(
        "--manifest",
        default="sprites.manifest",
        type=pathlib.Path,
        help="Destination for the resource manifest fragment.",
    )
    build(**vars(parser.parse_args()))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
