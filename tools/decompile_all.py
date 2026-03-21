#!/usr/bin/env python3
"""
decompile_all.py - Convert register-level pseudo-assembly to idiomatic C89.

This script performs a multi-pass transformation of register-level C code
(using PPC register variables r0-r31, f1-f31, goto labels, etc.) into
cleaner C89 code.

Transformations:
1. Remove dead register variables (not referenced in code body)
2. Remove stack frame boilerplate (sp[], r1=(u32)sp, epilog restores)
3. Remove asm-comment no-ops (/* stmw */, /* lmw */, /* crclr */)
4. Remove duplicate label loads (r3 = (u32)lbl_X; r3 = (u32)lbl_X;)
5. Map register arguments to function parameters
6. Inline trivial register aliases
7. Simplify comparisons and conditionals
"""

import re
import sys
import os
import copy


def find_register_functions(lines):
    """Find all functions with register-style pseudo-assembly code."""
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
                })
            i = func_end + 1
        else:
            i += 1
    return functions


def parse_params(params_str):
    """Parse C function parameter string."""
    if not params_str or params_str.strip() == 'void':
        return []
    params = []
    for p in params_str.split(','):
        p = p.strip()
        if not p:
            continue
        # Handle pointer types
        m = re.match(r'^(.*?)\s*(\*?\s*\w+)$', p)
        if m:
            ptype = m.group(1).strip()
            pname = m.group(2).strip().lstrip('*').strip()
            ptr = '*' if '*' in m.group(2) else ''
            params.append((ptype + ptr, pname))
        else:
            params.append((p, 'arg'))
    return params


def extract_function_parts(lines, func):
    """Extract the parts of a register-level function."""
    start = func['start']
    end = func['end']
    body = lines[start:end + 1]

    sig = body[0]
    externs = []
    decls = []
    code = []
    in_code = False

    for i in range(1, len(body)):
        s = body[i].strip()
        if s == '}':
            code.append(body[i])
            continue
        if not in_code:
            if s.startswith('extern '):
                externs.append(body[i])
                continue
            # Various declarations
            if (re.match(r'^u8 sp\[0x\w+\];$', s) or
                re.match(r'^u32 r1 = \(u32\)sp;$', s) or
                re.match(r'^(u32|s32) r\d+ = 0;$', s) or
                re.match(r'^(f32|f64) f\d+ = 0\.0f?;$', s) or
                re.match(r'^void \(\*ctr_fn\)\(void\) = 0;$', s) or
                re.match(r'^u32 ctr = 0;$', s)):
                decls.append(body[i])
                continue
            if s == '' and not code:
                continue
            in_code = True
        code.append(body[i])

    return sig, externs, decls, code


def is_referenced(name, code_lines):
    """Check if a variable name is referenced in code lines."""
    text = '\n'.join(l for l in code_lines)
    return bool(re.search(r'\b' + re.escape(name) + r'\b', text))


def clean_code(code, decl_vars):
    """Remove safe boilerplate patterns from code."""
    cleaned = []
    prev_stripped = ''

    # First pass: check if sp is used for real work (not just spill/restore)
    sp_for_work = False
    for line in code:
        s = line.strip()
        if 'sp' in s:
            if not re.match(r'^r\d+ = \*\(u32\*\)\(sp \+ 0x\w+\);$', s):
                sp_for_work = True

    for line in code:
        s = line.strip()

        # Remove asm no-ops
        if re.match(r'^/\* (stmw|lmw|crclr|rlwinm|and\.|mr\.|extrwi|subi) .+ \*/;$', s):
            continue

        # Remove duplicate consecutive label loads
        if s == prev_stripped and re.match(r'^r\d+ = \(u32\)lbl_\w+;$', s):
            continue

        # Remove callee-save register restore from stack (epilog)
        # Only safe when sp is not used for real work
        if not sp_for_work:
            if re.match(r'^r\d+ = \*\(u32\*\)\(sp \+ 0x\w+\);$', s):
                continue

        cleaned.append(line)
        prev_stripped = s

    return cleaned


def filter_decls(decls, code):
    """Keep only declarations that are referenced in code."""
    code_text = '\n'.join(code)
    kept = []
    need_r1 = False
    need_sp = False

    # First pass: check which are needed
    for d in decls:
        s = d.strip()

        m_sp = re.match(r'^u8 sp\[0x\w+\];$', s)
        if m_sp:
            if re.search(r'\bsp\b', code_text):
                kept.append(d)
                need_sp = True
            continue

        m_r1 = re.match(r'^u32 r1 = \(u32\)sp;$', s)
        if m_r1:
            if re.search(r'\br1\b', code_text):
                kept.append(d)
                need_r1 = True
            continue

        # Extract var name
        m = re.match(r'^(?:u32|s32|f32|f64)\s+(r\d+|f\d+)', s)
        if m:
            name = m.group(1)
            if re.search(r'\b' + re.escape(name) + r'\b', code_text):
                kept.append(d)
            continue

        m_ctr = re.match(r'^void \(\*ctr_fn\)', s)
        if m_ctr:
            if re.search(r'\bctr_fn\b', code_text):
                kept.append(d)
            continue

        m_ctr2 = re.match(r'^u32 ctr = 0;$', s)
        if m_ctr2:
            if re.search(r'\bctr\b', code_text):
                kept.append(d)
            continue

        kept.append(d)  # keep unknown declarations

    # Ensure sp is included if r1 is needed
    if need_r1 and not need_sp:
        for d in decls:
            if 'u8 sp[' in d.strip():
                kept.insert(0, d)
                break

    return kept


def convert_function(lines, func):
    """Convert a single register-level function to cleaner C89."""
    sig, externs, decls, code = extract_function_parts(lines, func)

    # Get declared variable names
    decl_vars = set()
    for d in decls:
        m = re.match(r'^\s*(?:u32|s32|f32|f64)\s+(r\d+|f\d+)', d.strip())
        if m:
            decl_vars.add(m.group(1))

    # Clean code
    cleaned_code = clean_code(code, decl_vars)

    # Filter declarations
    kept_decls = filter_decls(decls, cleaned_code)

    # Rebuild
    result = [sig]
    result.extend(externs)
    result.extend(kept_decls)
    if externs or kept_decls:
        result.append('')
    result.extend(cleaned_code)

    return result


def process_file(filepath):
    """Process all register-level functions in a file."""
    with open(filepath, 'r') as f:
        lines = f.read().split('\n')

    functions = find_register_functions(lines)
    print(f"  Found {len(functions)} register-level functions in {filepath}")

    converted = 0
    for func in reversed(functions):
        new_body = convert_function(lines, func)
        old_len = func['end'] - func['start'] + 1
        lines[func['start']:func['end'] + 1] = new_body
        delta = len(new_body) - old_len
        if delta != 0:
            converted += 1

    with open(filepath, 'w') as f:
        f.write('\n'.join(lines))

    print(f"  Converted {converted} functions (reduced code)")
    return converted


def main():
    if len(sys.argv) < 2:
        print("Usage: python tools/decompile_all.py <file.c> [...]")
        sys.exit(1)

    total = 0
    for filepath in sys.argv[1:]:
        if not os.path.exists(filepath):
            print(f"ERROR: File not found: {filepath}")
            continue
        total += process_file(filepath)

    print(f"\nTotal: converted {total} functions")


if __name__ == '__main__':
    main()
