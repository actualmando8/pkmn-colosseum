#!/usr/bin/env python3
"""Normalize source-built objects so they can replace dtk split objects."""

import argparse
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path


def run(cmd):
    proc = subprocess.run(cmd, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if proc.returncode != 0:
        if proc.stdout:
            print(proc.stdout, file=sys.stderr, end="")
        raise SystemExit(proc.returncode)
    return proc


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--objcopy", required=True)
    parser.add_argument("--rename-section", action="append", default=[])
    parser.add_argument("--metadata-from")
    parser.add_argument("--copy-section", action="append", default=[])
    parser.add_argument("input")
    parser.add_argument("output")
    args = parser.parse_args()

    input_path = Path(args.input)
    output_path = Path(args.output)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(input_path, output_path)

    objcopy = Path(args.objcopy)
    for rename in args.rename_section:
        run([objcopy, "--rename-section", rename, output_path])

    if args.metadata_from:
        metadata_from = Path(args.metadata_from)
        with tempfile.TemporaryDirectory(prefix="split-object-") as tmp:
            tmpdir = Path(tmp)
            for section in args.copy_section:
                dump_path = tmpdir / (section.replace(".", "_") + ".bin")
                run([objcopy, "--dump-section", f"{section}={dump_path}", metadata_from])

                update = subprocess.run(
                    [objcopy, "--update-section", f"{section}={dump_path}", output_path],
                    text=True,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.STDOUT,
                )
                if update.returncode != 0:
                    run([objcopy, "--add-section", f"{section}={dump_path}", output_path])

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
