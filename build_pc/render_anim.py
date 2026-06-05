#!/usr/bin/env python3
import argparse
import json
import math
import os
from PIL import Image, ImageDraw, ImageFont


def load_json(path):
    with open(path, "r", encoding="utf-8") as f:
        return json.load(f)


def bounds(data):
    min_x = min_y = float("inf")
    max_x = max_y = float("-inf")
    for frame in data["frames"]:
        for joint in frame["joints"]:
            x = float(joint["x"])
            y = float(joint["y"])
            min_x = min(min_x, x)
            max_x = max(max_x, x)
            min_y = min(min_y, y)
            max_y = max(max_y, y)
    if not math.isfinite(min_x):
        min_x = min_y = -1.0
        max_x = max_y = 1.0
    return min_x, min_y, max_x, max_y


def project(x, y, center_x, center_y, scale, cell_w, cell_h):
    px = (x - center_x) * scale + cell_w * 0.5
    py = cell_h * 0.5 - (y - center_y) * scale
    return px, py


def render_sheet(data, output_path, cell_w=256, cell_h=256):
    frames = data["frames"]
    min_x, min_y, max_x, max_y = bounds(data)
    span_x = max(max_x - min_x, 1.0)
    span_y = max(max_y - min_y, 1.0)
    margin = 22.0
    scale = min((cell_w - 2.0 * margin) / span_x,
                (cell_h - 2.0 * margin) / span_y)
    center_x = (min_x + max_x) * 0.5
    center_y = (min_y + max_y) * 0.5

    sheet = Image.new("RGBA", (cell_w * len(frames), cell_h), (250, 248, 244, 255))
    draw = ImageDraw.Draw(sheet)
    font = ImageFont.load_default()

    for idx, frame in enumerate(frames):
        ox = idx * cell_w
        draw.rectangle([ox, 0, ox + cell_w - 1, cell_h - 1], outline=(200, 194, 184, 255))
        joints = frame["joints"]
        pts = []
        for joint in joints:
            px, py = project(float(joint["x"]), float(joint["y"]),
                             center_x, center_y, scale, cell_w, cell_h)
            pts.append((ox + px, py))

        for j, joint in enumerate(joints):
            parent = int(joint["parent"])
            if parent >= 0 and parent < len(joints):
                draw.line([pts[parent], pts[j]], fill=(36, 36, 36, 255), width=3)
        for x, y in pts:
            r = 3
            draw.ellipse([x - r, y - r, x + r, y + r], fill=(190, 44, 44, 255))

        label = f"{idx:02d}"
        if "motionId" in data:
            label = f"{label}  m{data['motionId']}"
        draw.text((ox + 6, 6), label, fill=(92, 84, 72, 255), font=font)

    sheet.save(output_path)


def main():
    parser = argparse.ArgumentParser(description="Render a joint-animation JSON dump to a sprite sheet.")
    parser.add_argument("input_json")
    parser.add_argument("output_png", nargs="?")
    args = parser.parse_args()

    data = load_json(args.input_json)
    output = args.output_png
    if not output:
        base, _ = os.path.splitext(args.input_json)
        output = base + ".png"
    render_sheet(data, output)
    print(output)


if __name__ == "__main__":
    main()
