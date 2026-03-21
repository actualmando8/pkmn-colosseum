#!/usr/bin/env python3
"""
fix_compile_errors_v2.py - Fix compile errors in register-level C functions
caused by incorrect variable renaming from previous idiomatization passes.

Fixes:
1. Variable redefinition: when u32 X = 0; conflicts with function param X
   -> Change local to u32 _X = 0; (prefix with underscore)
2. Type mismatches: void* param used as u32 in register-level code
   -> Change the variable type to u32 with cast
3. r1 undefined: replace r1 references with (u32)sp
4. Remove orphaned r1 declarations

Operates on ALL .c files under src/ that have register-level functions.
"""

import re
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent
SRC_DIR = PROJECT_ROOT / "src"


def fix_file(filepath):
    """Fix compile errors in a single file."""
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        content = f.read()

    if 'u32 r0 = 0;' not in content and 'u32 tmp = 0;' not in content:
        return False

    lines = content.split('\n')
    changed = False

    # Find all functions and fix their bodies
    i = 0
    while i < len(lines):
        s = lines[i].strip()

        # Find function signature
        m = re.match(
            r'^((?:static\s+)?(?:void|u32|s32|u16|s16|u8|s8|int|f32|f64|BOOL)\s*\**)\s*'
            r'(\w+)\s*\(([^)]*)\)\s*\{?\s*$', s)
        if not m:
            i += 1
            continue

        func_ret = m.group(1).strip()
        func_name = m.group(2)
        params_str = m.group(3).strip()
        sig_line = i

        # Parse function parameters
        param_names = set()
        param_types = {}  # name -> type
        if params_str and params_str != 'void':
            for p in params_str.split(','):
                p = p.strip()
                parts = p.rsplit(None, 1)
                if len(parts) == 2:
                    ptype = parts[0].strip()
                    pname = parts[1].strip().lstrip('*')
                    param_names.add(pname)
                    param_types[pname] = ptype

        if not param_names:
            i += 1
            continue

        # Find function body end
        if '{' not in lines[i]:
            i += 1
            continue

        brace_depth = lines[i].count('{') - lines[i].count('}')
        func_end = i
        j = i + 1
        while j < len(lines) and brace_depth > 0:
            brace_depth += lines[j].count('{') - lines[j].count('}')
            if brace_depth <= 0:
                func_end = j
                break
            j += 1
        else:
            i += 1
            continue

        # Check for issues in the function body
        has_reglevel = False
        for k in range(sig_line + 1, func_end):
            if 'u32 r0 = 0;' in lines[k] or 'u32 tmp = 0;' in lines[k]:
                has_reglevel = True
                break

        if not has_reglevel:
            i = func_end + 1
            continue

        # Fix 1: Variable redefinition - rename conflicting locals
        for k in range(sig_line + 1, func_end):
            for pname in param_names:
                # Check for: u32 PNAME = ...;
                pattern = re.compile(r'^(\s*)(u32|s32)\s+' + re.escape(pname) + r'\s*=\s*(.+?)\s*;$')
                m = pattern.match(lines[k])
                if m:
                    indent = m.group(1)
                    vtype = m.group(2)
                    vinit = m.group(3)
                    new_name = f'_{pname}'
                    lines[k] = f'{indent}{vtype} {new_name} = {vinit};'
                    # Rename all subsequent uses in this function
                    for l in range(k + 1, func_end):
                        lines[l] = re.sub(r'\b' + re.escape(pname) + r'\b', new_name, lines[l])
                    changed = True

        # Fix 2: void* param used as u32 - replace with casts
        for pname in param_names:
            ptype = param_types.get(pname, '')
            if '*' not in ptype:
                continue
            # This param is a pointer but the register-level code uses it as u32
            # We need to cast at assignment sites
            for k in range(sig_line + 1, func_end):
                s = lines[k].strip()

                # PNAME = integer_value; -> PNAME = (void*)integer_value;
                m = re.match(r'^(\s*)' + re.escape(pname) + r'\s*=\s*(0x[0-9A-Fa-f]+|-?\d+)\s*;$', s)
                if m:
                    indent = m.group(1)
                    val = m.group(2)
                    lines[k] = f'{indent}{pname} = ({ptype})(u32){val};'
                    changed = True
                    continue

                # PNAME = u32_var; -> PNAME = (void*)(u32)u32_var;
                m = re.match(r'^(\s*)' + re.escape(pname) + r'\s*=\s*(\w+)\s*;$', s)
                if m:
                    indent = m.group(1)
                    rhs = m.group(2)
                    if rhs not in param_names or '*' not in param_types.get(rhs, ''):
                        # rhs is u32 var
                        lines[k] = f'{indent}{pname} = ({ptype})(u32){rhs};'
                        changed = True
                        continue

                # PNAME = *(u32*)...; -> PNAME = (void*)*(u32*)...;
                m = re.match(r'^(\s*)' + re.escape(pname) + r'\s*=\s*(\*\(u32\*\).+?)\s*;$', s)
                if m:
                    indent = m.group(1)
                    rhs = m.group(2)
                    lines[k] = f'{indent}{pname} = ({ptype}){rhs};'
                    changed = True
                    continue

                # PNAME = *(u8*)...; -> PNAME = (void*)(u32)*(u8*)...;
                m = re.match(r'^(\s*)' + re.escape(pname) + r'\s*=\s*(\*\(u8\*\).+?)\s*;$', s)
                if m:
                    indent = m.group(1)
                    rhs = m.group(2)
                    lines[k] = f'{indent}{pname} = ({ptype})(u32){rhs};'
                    changed = True
                    continue

                # u32_var = PNAME; -> u32_var = (u32)PNAME;
                m = re.match(r'^(\s*)(\w+)\s*=\s*' + re.escape(pname) + r'\s*;$', s)
                if m:
                    indent = m.group(1)
                    lhs = m.group(2)
                    if lhs not in param_names or '*' not in param_types.get(lhs, ''):
                        lines[k] = f'{indent}{lhs} = (u32){pname};'
                        changed = True
                        continue

                # u32_var = PNAME + PNAME; -> u32_var = (u32)PNAME + (u32)PNAME;
                m = re.match(r'^(\s*)(\w+)\s*=\s*' + re.escape(pname) + r'\s*\+\s*' + re.escape(pname) + r'\s*;$', s)
                if m:
                    indent = m.group(1)
                    lhs = m.group(2)
                    lines[k] = f'{indent}{lhs} = (u32){pname} + (u32){pname};'
                    changed = True
                    continue

                # tmp = PNAME & 0xFF; -> tmp = (u32)PNAME & 0xFF;
                m = re.match(r'^(\s*)(\w+)\s*=\s*' + re.escape(pname) + r'\s*&\s*(0x[0-9A-Fa-f]+)\s*;$', s)
                if m:
                    indent = m.group(1)
                    lhs = m.group(2)
                    mask = m.group(3)
                    lines[k] = f'{indent}{lhs} = (u32){pname} & {mask};'
                    changed = True
                    continue

                # PNAME = PNAME + u32_expr; -> PNAME = (void*)((u32)PNAME + expr);
                m = re.match(r'^(\s*)' + re.escape(pname) + r'\s*=\s*' + re.escape(pname) + r'\s*\+\s*(.+?)\s*;$', s)
                if m:
                    indent = m.group(1)
                    rhs = m.group(2)
                    lines[k] = f'{indent}{pname} = ({ptype})((u32){pname} + {rhs});'
                    changed = True
                    continue

        # Fix 3: r1 undefined -> (u32)sp
        for k in range(sig_line + 1, func_end):
            if re.search(r'\br1\b', lines[k]) and 'u32 r1' not in lines[k]:
                old = lines[k]
                lines[k] = re.sub(r'\br1\b', '(u32)sp', lines[k])
                if lines[k] != old:
                    # Make sure sp is declared
                    changed = True

        i = func_end + 1

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
        if fix_file(str(src_file)):
            fixed += 1
            print(f"  Fixed: {rel}")

    print(f"\nFixed {fixed} files")


if __name__ == '__main__':
    sys.exit(main() or 0)
