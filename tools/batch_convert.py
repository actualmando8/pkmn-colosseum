#!/usr/bin/env python3
"""
batch_convert.py - Pattern-based conversion of register-level C to idiomatic C89.

Detects and converts common patterns found in the Colosseum decomp:

Pattern A: "Queue or flag-set" - the most common pattern
  Load state -> check state[0] == 1 -> queue via fn_800D4F98
  else -> set flag -> call SDK function

Pattern B: "Simple forwarder" - call a function with register args

Pattern C: State check with early return

This converts function bodies in-place, preserving the file structure.
"""

import re
import sys
import os


def find_register_functions(lines):
    """Find all functions with register-style code."""
    pattern = re.compile(
        r'^((?:static\s+)?'
        r'(?:void|u32|s32|u16|s16|u8|s8|int|f32|f64|BOOL)'
        r'[\s\*]*\s+'
        r'(\w+)\s*'
        r'\(([^)]*)\))\s*\{'
    )
    functions = []
    i = 0
    while i < len(lines):
        m = pattern.match(lines[i])
        if m:
            fname = m.group(2)
            params_str = m.group(3)
            func_start = i
            sig_rest = lines[i][lines[i].index('{') + 1:]
            depth = 1 + sig_rest.count('{') - sig_rest.count('}')
            if depth == 0:
                i += 1
                continue
            has_regs = False
            j = i + 1
            while j < len(lines) and depth > 0:
                s = lines[j].strip()
                depth += s.count('{') - s.count('}')
                if re.match(r'^u32 r\d+ = 0;$', s):
                    has_regs = True
                j += 1
            func_end = j - 1
            if has_regs:
                functions.append({
                    'name': fname,
                    'start': func_start,
                    'end': func_end,
                    'params_str': params_str,
                    'ret_type': lines[i].split()[0],
                })
            i = func_end + 1
        else:
            i += 1
    return functions


def get_body_lines(lines, func):
    """Get the function body lines (between { and })."""
    body = []
    for i in range(func['start'] + 1, func['end']):
        body.append(lines[i])
    return body


def try_convert_queue_or_flag(lines, func):
    """Try to convert the 'queue or flag-set' pattern.

    Pattern:
        state = lbl_8047AA80
        if state[0] == 1: fn_800D4F98(cmd, nargs, args...)
        else: set flag, call SDK
    """
    body = get_body_lines(lines, func)
    body_text = '\n'.join(l.strip() for l in body)

    # Must reference lbl_8047AA80 and fn_800D4F98
    if 'lbl_8047AA80' not in body_text:
        return None
    if 'fn_800D4F98' not in body_text:
        return None

    # Count labels - simple patterns have few
    labels = sum(1 for l in body if re.match(r'\s*L_[0-9A-F]+\s*:', l.strip()))
    if labels > 6:
        return None  # too complex

    # Count code lines (excluding decls and blanks)
    code_lines = [l for l in body if l.strip() and
                  not l.strip().startswith('extern') and
                  not re.match(r'^\s*(u32|s32|f32|f64|u8|void|u16)\s', l.strip()) and
                  l.strip() != '}']
    if len(code_lines) > 30:
        return None  # too complex for this pattern

    return None  # For now, skip auto-conversion of complex patterns


def process_file(filepath):
    """Process a file."""
    with open(filepath, 'r') as f:
        lines = f.read().split('\n')

    functions = find_register_functions(lines)
    print(f"  Found {len(functions)} register-level functions in {filepath}")

    converted = 0
    for func in reversed(functions):
        result = try_convert_queue_or_flag(lines, func)
        if result is not None:
            lines[func['start']:func['end'] + 1] = result
            converted += 1

    if converted > 0:
        with open(filepath, 'w') as f:
            f.write('\n'.join(lines))

    print(f"  Auto-converted {converted} functions")
    return converted


def main():
    if len(sys.argv) < 2:
        print("Usage: python tools/batch_convert.py <file.c> [...]")
        sys.exit(1)

    for filepath in sys.argv[1:]:
        process_file(filepath)


if __name__ == '__main__':
    main()
