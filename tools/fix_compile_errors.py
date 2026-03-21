#!/usr/bin/env python3
"""
fix_compile_errors.py - Fix remaining compile errors in colosseum_battle.c.

Issues to fix:
1. Remove "f32 f32 = 0.0f;" and "f32 f64 = 0.0f;" (variable name clashes with type)
2. Fix "*(f32*)lbl_XXXX" -> "*(f32*)(u32)lbl_XXXX" (explicit cast for array-to-ptr)
3. Fix "*(f32*)(rX + rY)" -> "*(f32*)(void*)(rX + rY)" (int-to-ptr cast)
4. Fix "*(f64*)(sp + 0xN)" -> "*(f64*)(void*)(sp + 0xN)"
5. Fix "void (*ctr_fn)(void) = 0;" placement after code
6. Fix missing r4, r5 in specific functions after f32/f64 declaration removals
"""

import re
import sys
from pathlib import Path

SRC = Path("src/game/colosseum_battle.c")


def main():
    with open(SRC, 'r') as f:
        lines = f.readlines()

    changes = 0
    new_lines = []

    for i, line in enumerate(lines):
        orig = line
        s = line.strip()

        # Fix 1: Remove "f32 f32 = 0.0f;" and "f32 f64 = 0.0f;"
        # These are invalid - variable name clashes with type name
        if re.match(r'\s+f32 f32\s*=\s*0\.0f;\s*$', line):
            changes += 1
            continue  # skip this line
        if re.match(r'\s+f32 f64\s*=\s*0\.0f;\s*$', line):
            changes += 1
            continue  # skip this line

        # Fix 2: *(f32*)lbl_XXXX -> *(f32*)(u32)lbl_XXXX
        # This handles SDA21 float loads from extern arrays
        line = re.sub(r'\*\(f32\*\)(lbl_[0-9A-Fa-f]+)', r'*(f32*)(u32)\1', line)

        # Fix 3: *(f32*)(rX + rY) -> *(f32*)(void*)(rX + rY)
        # CW C89 needs explicit int-to-pointer cast
        line = re.sub(r'\*\(f32\*\)\((\w+ [+\-] \w+)\)', r'*(f32*)(void*)(\1)', line)

        # Fix 4: *(f64*)(sp + 0xN) -> *(f64*)(void*)(sp + 0xN)
        line = re.sub(r'\*\(f64\*\)\((\w+ \+ 0x[0-9A-Fa-f]+)\)', r'*(f64*)(void*)(\1)', line)

        # Fix: (f64)(s32)f1 -> should be fine, but let's ensure parentheses
        # Actually the pattern f0 = (f64)(s32)f1 is a float-to-double conversion
        # via int. This needs to be: (f64)((s32)f1)
        # But actually in the original asm, fctiwz converts float to int,
        # stored to stack, then loaded. This is a float->int->double pattern.
        # Keep as is for now.

        if line != orig:
            changes += 1
        new_lines.append(line)

    print(f"Applied {changes} fixes")

    with open(SRC, 'w') as f:
        f.writelines(new_lines)

    print("Done!")


if __name__ == '__main__':
    main()
