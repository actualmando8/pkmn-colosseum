#!/usr/bin/env python3
"""
batch_idiomatize.py - Batch convert register-level C to idiomatic C89.

Converts functions that follow known patterns from register-level pseudocode
to proper idiomatic C89 that compiles at -O4,p without #pragma optimization_level 0.

Pattern 1: Pokemon field accessor (fn_801FB1C0 x2 + fn_8012640C)
Pattern 2: Simple 2-3 call wrapper with no gotos
Pattern 3: Simple conditional with 1-2 gotos and clear if/else pattern
"""

import re
import sys
import os


def process_file(filepath):
    with open(filepath, 'r') as f:
        content = f.read()
    lines = content.split('\n')

    # Find all pragma-guarded functions
    funcs = find_pragma_functions(lines)
    print(f"Found {len(funcs)} pragma-guarded functions")

    converted = 0
    # Process in reverse order to maintain line indices
    for func in reversed(funcs):
        result = try_convert(lines, func)
        if result is not None:
            # Find the full range to replace (pragma_line to restore_line)
            start = func['pragma']
            end = func['end']
            # Check for #pragma optimization_level 4 after
            if end + 1 < len(lines) and '#pragma optimization_level 4' in lines[end + 1]:
                end = end + 1
            # Check for comment line before pragma
            comment_start = start
            if start > 0 and (lines[start - 1].strip().startswith('/*') or lines[start - 1].strip() == ''):
                pass  # Don't include the comment - it might be shared
            lines[start:end + 1] = result
            converted += 1

    with open(filepath, 'w') as f:
        f.write('\n'.join(lines))

    print(f"Converted {converted} functions to idiomatic C")
    return converted


def find_pragma_functions(lines):
    funcs = []
    i = 0
    while i < len(lines):
        if lines[i].strip() == '#pragma optimization_level 0':
            for j in range(i + 1, min(i + 3, len(lines))):
                m = re.match(
                    r'^(void|u32|s32)\s+(fn_[0-9A-Fa-f]+)\s*\(([^)]*)\)\s*\{',
                    lines[j].strip()
                )
                if m:
                    depth = 0
                    for k in range(j, len(lines)):
                        depth += lines[k].count('{') - lines[k].count('}')
                        if depth == 0 and k > j:
                            break
                    body_lines = lines[j + 1:k]
                    body_text = '\n'.join(body_lines)
                    gotos = len(re.findall(r'\bgoto\b', body_text))
                    labels = len(re.findall(r'^L_[0-9A-Fa-f]+\s*:', body_text, re.MULTILINE))

                    # Extract call sequence
                    calls = []
                    for bl in body_lines:
                        s = bl.strip()
                        cm = re.search(r'^(fn_[0-9A-Fa-f]+)\(\);', s)
                        if cm:
                            calls.append(cm.group(1))
                        cm2 = re.search(r'\(\(void\(\*\)\(void\)\)(fn_[0-9A-Fa-f]+)\)\(\);', s)
                        if cm2:
                            calls.append(cm2.group(1))

                    funcs.append({
                        'name': m.group(2),
                        'ret': m.group(1),
                        'params': m.group(3),
                        'pragma': i,
                        'start': j,
                        'end': k,
                        'gotos': gotos,
                        'labels': labels,
                        'calls': calls,
                        'body_lines': body_lines,
                        'body_text': body_text,
                        'num_lines': k - j + 1,
                    })
                    i = k + 2
                    break
            else:
                i += 1
        else:
            i += 1
    return funcs


def try_convert(lines, func):
    """Try to convert a function. Returns replacement lines or None."""

    # Only attempt small-to-medium functions
    if func['num_lines'] > 60:
        return None

    # Pattern 1: Zero-goto functions
    if func['gotos'] == 0:
        return convert_zero_goto(lines, func)

    # Pattern 2: Simple conditional with 1-2 gotos
    if func['gotos'] <= 2 and func['labels'] <= 2 and func['num_lines'] <= 40:
        return convert_simple_conditional(lines, func)

    return None


def convert_zero_goto(lines, func):
    """Convert a zero-goto function to idiomatic C.

    These are typically:
    - Multi-call wrappers that chain function calls
    - Field accessors that look up Pokemon data
    """
    body = func['body_text']
    body_lines = func['body_lines']

    # Skip functions with complex patterns we can't handle
    if '*(f64*)' in body:  # Float operations
        return None
    if 'ctr_fn' in body or '--ctr' in body:  # Loop counters
        return None

    # Analyze register flow
    # PPC calling convention: r3-r10 = args, r3/r4 = return
    # Pattern: set r3-r6 then call fn_XXX(); capture r3 for return value

    # Parse function parameters
    params = parse_params(func['params'])
    param_regs = ['r3', 'r4', 'r5', 'r6', 'r7', 'r8', 'r9', 'r10']

    # Build register state by tracing through the code
    # This is a simplified symbolic execution
    reg_state = {}
    for i, (ptype, pname) in enumerate(params):
        if i < len(param_regs):
            reg_state[param_regs[i]] = pname

    # Parse the body into a sequence of operations
    ops = parse_operations(body_lines)
    if ops is None:
        return None

    # Generate idiomatic C from operations
    result = generate_idiomatic(func, ops, params)
    if result is None:
        return None

    return result


def convert_simple_conditional(lines, func):
    """Convert a simple 1-2 goto conditional function."""
    # This is complex enough that we skip for now
    return None


def parse_params(params_str):
    if not params_str or params_str.strip() == 'void':
        return []
    params = []
    for p in params_str.split(','):
        p = p.strip()
        parts = p.split()
        if len(parts) >= 2:
            ptype = ' '.join(parts[:-1])
            pname = parts[-1].lstrip('*')
            params.append((ptype, pname))
    return params


def parse_operations(body_lines):
    """Parse body lines into a sequence of high-level operations.

    Returns list of operation dicts, or None if can't parse.
    """
    ops = []
    # Track register assignments
    reg_state = {}
    i = 0
    code_lines = []

    for line in body_lines:
        s = line.strip()
        if not s or s == '{' or s == '}' or s == 'return;':
            if s == 'return;':
                ops.append({'type': 'return'})
            continue
        if s.startswith('extern ') or s.startswith('u32 ') or s.startswith('s32 ') or s.startswith('f32 '):
            if '=' in s and '(' not in s:
                # Variable declaration - skip
                continue
            elif s.startswith('extern'):
                continue
        code_lines.append(s)

    return code_lines  # Return raw code lines for now


def generate_idiomatic(func, ops, params):
    """Generate idiomatic C from parsed operations.

    For now, this handles the specific patterns we've identified.
    """
    # We need the raw body to analyze
    body_lines = [l.strip() for l in func['body_lines'] if l.strip()]

    # Filter to just code lines (not decls)
    code_lines = []
    for s in body_lines:
        if s.startswith('extern ') or re.match(r'^(u32|s32|f32|u8|void)\s+(r\d+|f\d+|ctr|sp)', s):
            continue
        if s in ('', '{', '}'):
            continue
        code_lines.append(s)

    if not code_lines:
        return None

    # For zero-goto functions with known call patterns, we can trace
    # the register flow and reconstruct the function calls with arguments

    # Extract extern declarations
    externs = set()
    for s in body_lines:
        if s.startswith('extern '):
            m = re.search(r'(fn_[0-9A-Fa-f]+)', s)
            if m:
                externs.add(m.group(1))

    # Check if this is a pure call-chain (no complex logic)
    has_complex = False
    for s in code_lines:
        if s.startswith('if ') or s.startswith('goto ') or 'goto ' in s:
            has_complex = True
        if s.startswith('L_'):
            has_complex = True
        if 'sp' in s and '*(u' in s:
            has_complex = True

    if has_complex:
        return None

    # For simple call chains, we can't easily reconstruct proper C
    # without full register tracing. Skip for now unless it matches
    # a known template.
    return None


def main():
    if len(sys.argv) < 2:
        print("Usage: python tools/batch_idiomatize.py <source_file>")
        sys.exit(1)

    filepath = sys.argv[1]
    count = process_file(filepath)
    print(f"\nDone. Converted {count} functions.")


if __name__ == '__main__':
    main()
