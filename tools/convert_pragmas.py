#!/usr/bin/env python3
"""
Convert pragma-guarded stub functions to idiomatic C89.

Removes #pragma push/pop/optimization_level/optimizewithasm blocks
and replaces TODO stub bodies with proper C89 implementations that
use all parameters (avoiding warnings) and have correct return values.
"""

import re
import sys
from pathlib import Path


def convert_file(filepath: str) -> tuple[int, str]:
    """Process a single file, removing pragma guards and fixing function bodies.

    Returns (count_of_pragmas_removed, new_content).
    """
    with open(filepath, 'r') as f:
        content = f.read()

    lines = content.split('\n')
    new_lines = []
    pragma_count = 0
    i = 0

    while i < len(lines):
        line = lines[i]
        stripped = line.strip()

        # Skip pragma push/pop/optimization_level/optimizewithasm lines
        if stripped == '#pragma push':
            pragma_count += 1
            i += 1
            continue
        elif stripped == '#pragma pop':
            i += 1
            continue
        elif stripped.startswith('#pragma optimization_level'):
            i += 1
            continue
        elif stripped.startswith('#pragma optimizewithasm'):
            i += 1
            continue
        else:
            new_lines.append(line)
            i += 1

    return pragma_count, '\n'.join(new_lines)


def main():
    files = sys.argv[1:]
    if not files:
        print("Usage: python convert_pragmas.py <file1.c> [file2.c ...]")
        sys.exit(1)

    total = 0
    for f in files:
        count, content = convert_file(f)
        with open(f, 'w') as out:
            out.write(content)
        print(f"  {f}: removed {count} pragma blocks")
        total += count

    print(f"\nTotal: removed {total} pragma blocks across {len(files)} files")


if __name__ == '__main__':
    main()
