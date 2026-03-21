#!/usr/bin/env python3
"""
fix_all_errors.py - Comprehensive fix for all compile errors in register-level
C functions across the Pokemon Colosseum decomp.

Strategy: within register-level functions (identified by 'u32 r0 = 0;' or
'u32 tmp = 0;'), fix all type mismatches and undefined identifiers by:

1. Replacing all standalone 'r1' with '(u32)sp' (stack pointer)
2. For void* params used as u32: revert to raw register names
3. Fix variable redefinitions by prefixing with underscore
4. Fix broken identifier names (fv51 etc.)
"""

import re
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent
SRC_DIR = PROJECT_ROOT / "src"


def find_functions(lines):
    """Find all register-level functions and their boundaries."""
    functions = []
    i = 0
    while i < len(lines):
        s = lines[i].strip()
        # Function signature
        m = re.match(
            r'^((?:static\s+)?(?:void|u32|s32|u16|s16|u8|s8|int|f32|f64|BOOL)\s*\**)\s*'
            r'(\w+)\s*\(([^)]*)\)\s*\{', s)
        if not m:
            i += 1
            continue

        ret_type = m.group(1).strip()
        func_name = m.group(2)
        params_str = m.group(3).strip()
        sig_line = i

        # Find function end
        brace_depth = s.count('{') - s.count('}')
        j = i + 1
        while j < len(lines) and brace_depth > 0:
            brace_depth += lines[j].count('{') - lines[j].count('}')
            j += 1
        func_end = j - 1

        # Check if register-level
        body_text = '\n'.join(lines[sig_line:func_end+1])
        if 'u32 r0 = 0;' in body_text or 'u32 tmp = 0;' in body_text:
            # Parse params
            params = []
            if params_str and params_str != 'void':
                for p in params_str.split(','):
                    p = p.strip()
                    parts = p.rsplit(None, 1)
                    if len(parts) == 2:
                        ptype = parts[0].strip()
                        pname = parts[1].strip()
                        # Handle pointer in name
                        if pname.startswith('*'):
                            pname = pname.lstrip('*')
                            if not ptype.endswith('*'):
                                ptype += '*'
                        params.append((ptype, pname))

            functions.append({
                'sig_line': sig_line,
                'func_end': func_end,
                'ret_type': ret_type,
                'func_name': func_name,
                'params': params,
                'params_str': params_str,
            })
        i = func_end + 1

    return functions


def fix_function(lines, func):
    """Fix all compile errors in a single register-level function."""
    start = func['sig_line']
    end = func['func_end']
    params = func['params']
    changed = False

    # Build set of void* param names
    void_ptr_params = {}
    all_param_names = set()
    for ptype, pname in params:
        all_param_names.add(pname)
        if '*' in ptype:
            void_ptr_params[pname] = ptype

    # Fix 1: r1 -> (u32)sp in the body (not in declarations)
    for k in range(start + 1, end + 1):
        if 'u32 r1 ' in lines[k]:
            continue  # Don't touch r1 declaration
        old = lines[k]
        # Replace standalone r1 that's not part of another register name
        lines[k] = re.sub(r'(?<!\w)r1(?!\d)', '(u32)sp', lines[k])
        if lines[k] != old:
            changed = True

    # Fix 2: Variable redefinition - rename conflicting local vars
    for k in range(start + 1, end + 1):
        for pname in all_param_names:
            # Match: u32 PNAME = ...;  (local variable shadowing param)
            m = re.match(r'^(\s*)(u32|s32)\s+' + re.escape(pname) + r'\s*=\s*(.+?)\s*;$',
                        lines[k])
            if m:
                indent, vtype, vinit = m.group(1), m.group(2), m.group(3)
                new_name = f'_{pname}'
                lines[k] = f'{indent}{vtype} {new_name} = {vinit};'
                # Rename all uses in subsequent lines
                for l in range(k + 1, end + 1):
                    lines[l] = re.sub(r'\b' + re.escape(pname) + r'\b', new_name, lines[l])
                changed = True
                break  # One fix per line

    # Fix 3: void* params used as integers
    # These need (u32) casts when used as integers or assigned integer values
    for pname, ptype in void_ptr_params.items():
        for k in range(start + 1, end + 1):
            s = lines[k].strip()

            # PNAME = integer_literal;
            m = re.match(r'^(\s*)' + re.escape(pname) + r'\s*=\s*(0x[0-9A-Fa-f]+|-?\d+)\s*;$', s)
            if m:
                indent = m.group(1)
                val = m.group(2)
                lines[k] = f'{indent}{pname} = ({ptype})(u32){val};'
                changed = True
                continue

            # PNAME = (u32)something;
            m = re.match(r'^(\s*)' + re.escape(pname) + r'\s*=\s*\(u32\)(.+?)\s*;$', s)
            if m:
                indent = m.group(1)
                rhs = m.group(2)
                lines[k] = f'{indent}{pname} = ({ptype})(u32){rhs};'
                changed = True
                continue

            # PNAME = *(uXX*)...;  (load from memory)
            m = re.match(r'^(\s*)' + re.escape(pname) + r'\s*=\s*(\*\((u32|u16|u8|s32|s16|s8)\*\).+?)\s*;$', s)
            if m:
                indent = m.group(1)
                rhs = m.group(2)
                lines[k] = f'{indent}{pname} = ({ptype}){rhs};'
                changed = True
                continue

            # PNAME = u32_var; (assign from u32 variable)
            m = re.match(r'^(\s*)' + re.escape(pname) + r'\s*=\s*(\w+)\s*;$', s)
            if m:
                indent = m.group(1)
                rhs = m.group(2)
                if rhs not in void_ptr_params:
                    lines[k] = f'{indent}{pname} = ({ptype})(u32){rhs};'
                    changed = True
                    continue

            # PNAME = u32_var & mask;
            m = re.match(r'^(\s*)' + re.escape(pname) + r'\s*=\s*(.+?\s*&\s*0x[0-9A-Fa-f]+)\s*;$', s)
            if m:
                indent = m.group(1)
                rhs = m.group(2)
                lines[k] = f'{indent}{pname} = ({ptype})({rhs});'
                changed = True
                continue

            # PNAME = expr + expr;
            m = re.match(r'^(\s*)' + re.escape(pname) + r'\s*=\s*(.+?\s*[\+\-\*]\s*.+?)\s*;$', s)
            if m:
                indent = m.group(1)
                rhs = m.group(2)
                # Check if the rhs involves void* which needs cast
                if pname in rhs or any(vp in rhs for vp in void_ptr_params):
                    # Cast the whole expression
                    rhs_casted = rhs
                    for vp in void_ptr_params:
                        rhs_casted = re.sub(r'\b' + re.escape(vp) + r'\b', f'(u32){vp}', rhs_casted)
                    lines[k] = f'{indent}{pname} = ({ptype})({rhs_casted});'
                    changed = True
                    continue

            # u32_var = PNAME;
            m = re.match(r'^(\s*)(\w+)\s*=\s*' + re.escape(pname) + r'\s*;$', s)
            if m:
                indent = m.group(1)
                lhs = m.group(2)
                if lhs not in void_ptr_params:
                    lines[k] = f'{indent}{lhs} = (u32){pname};'
                    changed = True
                    continue

            # u32_var = PNAME & mask;
            m = re.match(r'^(\s*)(\w+)\s*=\s*' + re.escape(pname) + r'\s*&\s*(0x[0-9A-Fa-f]+)\s*;$', s)
            if m:
                indent = m.group(1)
                lhs = m.group(2)
                mask = m.group(3)
                lines[k] = f'{indent}{lhs} = (u32){pname} & {mask};'
                changed = True
                continue

            # u32_var = PNAME + something;
            m = re.match(r'^(\s*)(\w+)\s*=\s*' + re.escape(pname) + r'\s*\+\s*(.+?)\s*;$', s)
            if m:
                indent = m.group(1)
                lhs = m.group(2)
                rhs = m.group(3)
                if lhs not in void_ptr_params:
                    lines[k] = f'{indent}{lhs} = (u32){pname} + {rhs};'
                    changed = True
                    continue

            # *(type*)(PNAME + offset) = val; or val = *(type*)(PNAME + offset);
            # These work fine since void* can be cast

            # if (PNAME OP u32_val) -> if ((u32)PNAME OP u32_val)
            m = re.match(r'^(\s*)if\s+\(' + re.escape(pname) + r'\s*(==|!=|<|>|<=|>=)\s*(\w+)\)', s)
            if m:
                indent = m.group(1)
                op = m.group(2)
                rhs = m.group(3)
                rest = s[m.end():]
                lines[k] = f'{indent}if ((u32){pname} {op} {rhs}){rest}'
                changed = True
                continue

            # if (u32_var OP PNAME) -> if (u32_var OP (u32)PNAME)
            m = re.match(r'^(\s*)if\s+\((\w+)\s*(==|!=|<|>|<=|>=)\s*' + re.escape(pname) + r'\)', s)
            if m:
                indent = m.group(1)
                lhs = m.group(2)
                op = m.group(3)
                rest = s[m.end():]
                lines[k] = f'{indent}if ({lhs} {op} (u32){pname}){rest}'
                changed = True
                continue

            # *(u32*)PNAME = val;  -> *(u32*)(u32)PNAME = val;  (not needed, void* works)

    # Fix 4: broken identifier names (fvNN from bad rename)
    for k in range(start + 1, end + 1):
        # fv51 -> f64 (was f64 type, got renamed badly)
        lines[k] = re.sub(r'\bfv(\d+)\b', lambda m: f'f{m.group(1)}', lines[k])

    return changed


def process_file(filepath):
    """Process a single file."""
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        lines = f.read().split('\n')

    functions = find_functions(lines)
    if not functions:
        return False

    changed = False
    for func in functions:
        if fix_function(lines, func):
            changed = True

    if changed:
        with open(filepath, 'w', encoding='utf-8', newline='\n') as f:
            f.write('\n'.join(lines))

    return changed


def main():
    src_files = sorted(SRC_DIR.rglob("*.c"))
    fixed = 0

    for src_file in src_files:
        with open(src_file, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()
        if 'u32 r0 = 0;' not in content and 'u32 tmp = 0;' not in content:
            continue

        rel = src_file.relative_to(PROJECT_ROOT)
        if process_file(str(src_file)):
            fixed += 1
            print(f"  Fixed: {rel}")

    print(f"\nFixed {fixed} files")


if __name__ == '__main__':
    sys.exit(main() or 0)
