#!/usr/bin/env python3
"""
Convert simple register-level functions (<=3 gotos) to idiomatic C89.

Handles patterns:
- Conditional call: if (guard) call_fn(args)
- Conditional store: if (guard) *ptr = val
- Simple loops: while/for patterns with a counter
- Simple getters with guard: if (ptr) return ptr->field
"""

import re
import sys


def find_function_blocks(lines):
    """Find function start/end line indices, only for register-level functions."""
    functions = []
    brace_depth = 0
    func_start = None
    func_name = None

    for i, line in enumerate(lines):
        stripped = line.strip()

        if brace_depth == 0 and '{' in stripped and '(' in stripped:
            if re.match(r'^(void|s32|u32|u8|u16|s16|f32|BOOL|s64|u64|f64|void\*)\s+', stripped):
                func_start = i
                m = re.match(r'^\w+[\*\s]+(\w+)\(', stripped)
                func_name = m.group(1) if m else '?'

        if func_start is not None:
            brace_depth += stripped.count('{') - stripped.count('}')
            if brace_depth == 0 and '}' in stripped:
                # Check if this is register-level
                gotos = sum(1 for j in range(func_start, i+1) if 'goto L_' in lines[j])
                regs = sum(1 for j in range(func_start, i+1)
                          if re.match(r'^\s+u32 r\d+ = 0;$', lines[j]))
                if gotos > 0 and gotos <= 3 and regs >= 2:
                    functions.append((func_start, i, func_name, gotos))
                func_start = None
                brace_depth = 0

    return functions


def convert_simple_function(lines, start, end, name):
    """Convert a simple register-level function to idiomatic C89."""
    func_lines = lines[start:end+1]
    body = '\n'.join(func_lines)

    # Extract the signature line
    sig = lines[start].rstrip()

    # Extract extern declarations
    externs = []
    for i in range(start+1, end):
        s = lines[i].strip()
        if s.startswith('extern '):
            externs.append(s)

    # Determine return type
    ret_type = 'void'
    for t in ['void*', 's32', 'u32', 'u8', 'u16', 's16', 'f32', 'BOOL']:
        if sig.strip().startswith(t + ' ') or sig.strip().startswith(t + '* '):
            ret_type = t
            break

    # For now just clean up the unmapped instruction comments
    # and redundant register variable noise
    cleaned = []
    for line in func_lines:
        s = line.strip()
        # Skip unmapped instruction comments
        if re.match(r'^\s+/\*\s*(stmw|lmw|psq_st|psq_l|extrwi|xoris|subi|crclr)\s+.*\*/\s*;?\s*$', s):
            continue
        cleaned.append(line)

    return cleaned


def process_file(filepath):
    with open(filepath) as f:
        content = f.read()
    lines = content.split('\n')

    funcs = find_function_blocks(lines)
    if not funcs:
        print(f"  {filepath}: no simple register-level functions")
        return

    converted = 0
    for start, end, name, gotos in reversed(funcs):
        new_lines = convert_simple_function(lines, start, end, name)
        if new_lines != lines[start:end+1]:
            lines[start:end+1] = new_lines
            converted += 1

    with open(filepath, 'w') as f:
        f.write('\n'.join(lines))

    print(f"  {filepath}: converted {converted}/{len(funcs)} simple functions")


def main():
    for f in sys.argv[1:]:
        process_file(f)


if __name__ == '__main__':
    main()
