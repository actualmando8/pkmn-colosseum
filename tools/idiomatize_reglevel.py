#!/usr/bin/env python3
"""
idiomatize_reglevel.py - Strip pragmas + clean up register-level C functions.

Two-step approach:
1. Strip all #pragma push/pop/optimization_level/optimizewithasm lines
2. For each register-level function, remove only:
   - Truly dead register declarations (unused in function body)
   - No-op ASM comments (/* stmw ... */; etc.)
   - Consecutive duplicate label loads
   - Stack frame spill/restore ONLY if sp is not otherwise used

All declarations that are referenced ANYWHERE in the code body are kept.
"""

import re
import sys
import os


def strip_pragmas(content):
    """Remove all pragma push/pop/optimization/optimizewithasm lines."""
    lines = content.split('\n')
    out = []
    removed = 0
    for line in lines:
        s = line.strip()
        if s in ('#pragma push', '#pragma pop'):
            removed += 1
            continue
        if s.startswith('#pragma optimization_level') or \
           s.startswith('#pragma optimizewithasm'):
            removed += 1
            continue
        out.append(line)
    return '\n'.join(out), removed


def find_register_functions(lines):
    """Find functions containing register-style code (u32 rN = 0;)."""
    functions = []
    i = 0
    while i < len(lines):
        m = re.match(
            r'^((?:static\s+)?(?:void|u32|s32|u16|s16|u8|s8|int|f32|f64|BOOL)'
            r'[\s\*]*\s+(\w+)\s*\([^)]*\))\s*\{',
            lines[i]
        )
        if m:
            fname = m.group(2)
            func_start = i
            has_regs = False
            # Count extra braces on the signature line itself (after the first {)
            sig_rest = lines[i][lines[i].index('{') + 1:]
            depth = 1 + sig_rest.count('{') - sig_rest.count('}')
            if depth == 0:
                # One-liner function like: void f(void) { return 0; }
                i += 1
                continue
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
                })
            i = func_end + 1
        else:
            i += 1
    return functions


def clean_function(lines, func):
    """Clean up a register-level function, preserving all used identifiers."""
    start = func['start']
    end = func['end']
    body = lines[start:end + 1]

    # Separate signature, externs, declarations, and code
    sig = body[0]
    externs = []
    all_decls = []  # (original_line, var_name, is_sp, is_r1)
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

            # Stack frame decl
            m_sp = re.match(r'^u8 sp\[0x\w+\];$', s)
            if m_sp:
                all_decls.append((body[i], 'sp', True, False))
                continue

            m_r1 = re.match(r'^u32 r1 = \(u32\)sp;$', s)
            if m_r1:
                all_decls.append((body[i], 'r1', False, True))
                continue

            # Register declarations
            m_reg = re.match(r'^(u32|s32) (r\d+) = 0;$', s)
            if m_reg:
                all_decls.append((body[i], m_reg.group(2), False, False))
                continue

            m_freg = re.match(r'^(f32|f64) (f\d+) = 0\.0f?;$', s)
            if m_freg:
                all_decls.append((body[i], m_freg.group(2), False, False))
                continue

            m_ctr = re.match(r'^void \(\*ctr_fn\)\(void\) = 0;$', s)
            if m_ctr:
                all_decls.append((body[i], 'ctr_fn', False, False))
                continue

            m_ctr2 = re.match(r'^u32 ctr = 0;$', s)
            if m_ctr2:
                all_decls.append((body[i], 'ctr', False, False))
                continue

            if s == '' and not code:
                continue  # skip blank lines between decls and code

            in_code = True
        code.append(body[i])

    # Build code text for reference checking
    code_text = '\n'.join(c.strip() for c in code)

    # Clean code: remove safe patterns
    cleaned = []
    prev_stripped = ''
    for line in code:
        s = line.strip()

        # Remove no-op asm comments (stmw/lmw save/restore, crclr)
        if re.match(r'^/\* (stmw|lmw|crclr) .+ \*/;$', s):
            continue

        # Remove consecutive duplicate label loads
        if s == prev_stripped and re.match(r'^r\d+ = \(u32\)lbl_\w+;$', s):
            continue

        # Remove stack epilog spill/restore ONLY if sp not otherwise referenced
        # Check: is sp used anywhere else besides spill/restore patterns?
        if re.match(r'^r\d+ = \*\(u32\*\)\(sp \+ 0x\w+\);$', s):
            # This is a callee-save register restore. Only safe to remove
            # if sp is not used for anything else
            sp_uses_in_code = 0
            for cl in code:
                cs = cl.strip()
                if 'sp' in cs and not re.match(r'^r\d+ = \*\(u32\*\)\(sp \+ 0x\w+\);$', cs):
                    sp_uses_in_code += 1
            if sp_uses_in_code == 0:
                continue

        cleaned.append(line)
        prev_stripped = s

    # Determine which declarations are used in cleaned code
    cleaned_text = '\n'.join(c for c in cleaned)
    kept_decls = []
    need_sp = False
    need_r1 = False

    for orig_line, var_name, is_sp, is_r1 in all_decls:
        if is_sp:
            # Keep sp if referenced in cleaned code
            if re.search(r'\bsp\b', cleaned_text):
                kept_decls.append(orig_line)
                need_sp = True
            continue
        if is_r1:
            # Keep r1 = (u32)sp if r1 is referenced in cleaned code
            if re.search(r'\br1\b', cleaned_text):
                kept_decls.append(orig_line)
                need_r1 = True
                # Also need sp for r1
                need_sp = True
            continue
        # Regular register: keep if used
        if re.search(r'\b' + re.escape(var_name) + r'\b', cleaned_text):
            kept_decls.append(orig_line)

    # If r1 is kept, sp must be too since r1 = (u32)sp references sp
    if need_r1:
        sp_already_kept = any('u8 sp[' in d for d in kept_decls)
        if not sp_already_kept:
            for orig_line, var_name, is_sp, _ in all_decls:
                if is_sp:
                    kept_decls.insert(0, orig_line)
                    break

    # Rebuild function
    result = [sig]
    result.extend(externs)
    result.extend(kept_decls)
    if kept_decls or externs:
        result.append('')
    result.extend(cleaned)

    return result


def process_file(filepath):
    """Process a file: strip pragmas + clean register functions."""
    with open(filepath, 'r') as f:
        content = f.read()

    # Step 1: Strip pragmas
    content, pragma_count = strip_pragmas(content)
    print(f"  Stripped {pragma_count} pragma lines from {filepath}")

    lines = content.split('\n')

    # Step 2: Clean register-level functions
    functions = find_register_functions(lines)
    print(f"  Found {len(functions)} register-level functions")

    converted = 0
    for func in reversed(functions):
        new_body = clean_function(lines, func)
        lines[func['start']:func['end'] + 1] = new_body
        converted += 1

    with open(filepath, 'w') as f:
        f.write('\n'.join(lines))

    print(f"  Cleaned {converted} functions")
    return converted


def main():
    if len(sys.argv) < 2:
        print("Usage: python tools/idiomatize_reglevel.py <file.c> [...]")
        sys.exit(1)

    total = 0
    for filepath in sys.argv[1:]:
        if not os.path.exists(filepath):
            print(f"ERROR: File not found: {filepath}")
            continue
        total += process_file(filepath)
    print(f"\nTotal: cleaned {total} functions")


if __name__ == '__main__':
    main()
