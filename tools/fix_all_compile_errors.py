#!/usr/bin/env python3
"""
fix_all_compile_errors.py - Fix compile errors across ALL failing source files.

Handles:
1. Missing r1 declaration - adds u32 r1 = 0; to functions that use r1
2. Redefined parameter variables - removes duplicate local declarations
3. void* parameters used as u32 - changes param type from void* to u32 in signature
"""

import re
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent
SRC_DIR = PROJECT_ROOT / "src"


def find_functions(lines):
    """Find all function definitions with their boundaries."""
    functions = []
    i = 0
    while i < len(lines):
        s = lines[i].rstrip()

        # Match function definition: rettype name(...) {
        m = re.match(r'^(\w[\w\s\*]*?)\s+(\w+)\s*\(([^)]*)\)\s*\{', s)
        if m:
            func_start = i
            depth = 1
            j = i + 1
            while j < len(lines) and depth > 0:
                ln = lines[j]
                depth += ln.count('{') - ln.count('}')
                j += 1
            func_end = j - 1

            functions.append({
                'start': func_start,
                'open_brace': func_start,
                'end': func_end,
                'name': m.group(2),
                'params_str': m.group(3).strip(),
                'ret_type': m.group(1).strip(),
            })
            i = func_end + 1
            continue

        i += 1

    return functions


def parse_params(params_str):
    """Parse parameter list into list of (type, name) tuples."""
    if not params_str or params_str.strip() == 'void' or params_str.strip() == '':
        return []

    params = []
    for param in params_str.split(','):
        param = param.strip()
        if not param:
            continue
        parts = param.rsplit(None, 1)
        if len(parts) == 2:
            ptype = parts[0].strip()
            pname = parts[1].strip().lstrip('*')
            if parts[1].startswith('*'):
                ptype += '*'
            params.append((ptype, pname))
        elif len(parts) == 1:
            params.append((parts[0], ''))

    return params


def fix_file(filepath):
    """Fix all compile errors in a single file. Returns number of fixes applied."""
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        content = f.read()

    lines = content.split('\n')
    total_fixes = 0

    # Process functions from bottom to top so line adjustments stay valid
    functions = find_functions(lines)
    functions.sort(key=lambda f: f['start'], reverse=True)

    for func in functions:
        start = func['start']
        open_brace = func['open_brace']
        end = func['end']
        params_str = func['params_str']

        body_start = open_brace + 1
        if body_start >= end:
            continue

        # Parse parameters
        params = parse_params(params_str)
        param_names = {p[1] for p in params if p[1]}
        param_type_map = {p[1]: p[0] for p in params if p[1]}

        # === Fix 3: void* parameter used as u32 ===
        # Must do this BEFORE removing redefined params, because the
        # redefined param removal depends on the param type being correct.
        for ptype, pname in params:
            if pname and ('void*' in ptype or 'void *' in ptype):
                body_after = '\n'.join(lines[body_start:end])
                used_as_int = bool(
                    re.search(rf'\b{re.escape(pname)}\s*=\s*0x[0-9A-Fa-f]+', body_after) or
                    re.search(rf'\b{re.escape(pname)}\s*=\s*\d+', body_after) or
                    re.search(rf'\b{re.escape(pname)}\s*&\s*0x', body_after) or
                    re.search(rf'\b{re.escape(pname)}\s*=\s*tmp\b', body_after) or
                    re.search(rf'\b{re.escape(pname)}\s*=\s*var_r', body_after) or
                    re.search(rf'r\d+\s*=\s*{re.escape(pname)}\s*;', body_after) or
                    re.search(rf'tmp\s*=\s*{re.escape(pname)}\s*[;&]', body_after) or
                    re.search(rf'tmp\s*=\s*{re.escape(pname)}\s*$', body_after, re.MULTILINE) or
                    re.search(rf'tmp\s*=\s*{re.escape(pname)}\s*\+', body_after) or
                    re.search(rf'\b{re.escape(pname)}\s*\+\s*{re.escape(pname)}\b', body_after)
                )

                if used_as_int:
                    old_sig = lines[start]
                    new_sig = re.sub(
                        rf'void\s*\*\s*{re.escape(pname)}',
                        f'u32 {pname}',
                        old_sig
                    )
                    if new_sig != old_sig:
                        lines[start] = new_sig
                        total_fixes += 1
                        # Update params_str tracking
                        param_type_map[pname] = 'u32'

                    # Fix forward declarations earlier in the file
                    for k in range(0, start):
                        if re.search(rf'void\s*\*\s*{re.escape(pname)}', lines[k]) and \
                           lines[k].strip().endswith(';') and func['name'] in lines[k]:
                            lines[k] = re.sub(
                                rf'void\s*\*\s*{re.escape(pname)}',
                                f'u32 {pname}',
                                lines[k]
                            )
                            total_fixes += 1

        # === Fix 2: Remove redefined parameter variables ===
        lines_to_remove = []
        for j in range(body_start, min(end, body_start + 120)):
            s = lines[j].strip()
            # Match local variable declarations
            m2 = re.match(r'^(u32|s32|u8|u16|s16|s8|f32|f64|int|char|BOOL|void)\s*\*?\s+(\w+)\s*=\s*0\s*;', s)
            if m2:
                var_name = m2.group(2)
                if var_name in param_names:
                    lines_to_remove.append(j)
                    total_fixes += 1

            # Stop at first non-declaration non-blank line
            if s and not s.startswith('extern ') and \
               not re.match(r'^(u32|s32|u8|u16|s16|s8|f32|f64|void|int|char|BOOL)\s+', s) and \
               not re.match(r'^u8\s+sp\[', s):
                break

        # Remove lines (reverse order)
        for j in sorted(lines_to_remove, reverse=True):
            lines.pop(j)
            end -= 1

        # === Fix 1: Add missing r1 declaration ===
        body_text = '\n'.join(lines[body_start:end])
        uses_r1 = bool(re.search(r'\br1\b', body_text))
        has_r1_decl = False
        last_decl_line = None
        decl_indent = "    "

        for j in range(body_start, min(end, body_start + 120)):
            s = lines[j].strip()

            if re.match(r'^u32\s+r1\s*=', s):
                has_r1_decl = True

            if re.match(r'^(u32|s32|u8|u16|s16|s8|f32|f64|void|int|char|BOOL)\s+', s) or \
               re.match(r'^extern\s+', s) or \
               re.match(r'^u8\s+sp\[', s) or \
               re.match(r'^(u32|s32|u8|u16|s16|s8|f32|f64|void|int|char|BOOL)\s+\(\*', s):
                last_decl_line = j
                indent = lines[j][:len(lines[j]) - len(lines[j].lstrip())]
                if indent:
                    decl_indent = indent
            elif s and not s.startswith('//'):
                break

        if uses_r1 and not has_r1_decl and last_decl_line is not None:
            has_sp = any(re.match(r'\s*u8\s+sp\[', lines[j])
                        for j in range(body_start, min(end, body_start + 120)))

            if has_sp:
                new_decl = f"{decl_indent}u32 r1 = (u32)sp;"
            else:
                new_decl = f"{decl_indent}u32 r1 = 0;"

            # Find insertion point - after u32 tmp = 0; if exists
            insert_after = None
            for j in range(body_start, min(end, body_start + 120)):
                s = lines[j].strip()
                if re.match(r'^u32\s+tmp\s*=', s):
                    insert_after = j
                    break

            if insert_after is None:
                insert_after = last_decl_line

            lines.insert(insert_after + 1, new_decl)
            end += 1
            total_fixes += 1

    new_content = '\n'.join(lines)
    if new_content != content:
        with open(filepath, 'w', encoding='utf-8', newline='\n') as f:
            f.write(new_content)

    return total_fixes


FAILING_FILES = [
    "crt/extras.c",
    "crt/printf.c",
    "crt/stdio.c",
    "crt/strtoul.c",
    "dolphin/dvd/DVDFs.c",
    "dolphin/exi/EXI.c",
    "dolphin/os/OSContext.c",
    "dolphin/os/OSReboot.c",
    "dolphin/os/OSSram.c",
    "dolphin/vi/VIFull.c",
    "game/battle/battle_logic.c",
    "game/battle/battle_main.c",
    "game/colosseum_script.c",
    "game/colosseum_ui.c",
    "game/effect/tracefx.c",
    "game/gba/gba_conv.c",
    "game/gba/gba_misc.c",
    "game/gs_colsys.c",
    "game/gs_dvd.c",
    "game/gs_floor.c",
    "game/gs_npc_event.c",
    "game/gs_npc_interact.c",
    "game/gs_pcbox.c",
    "game/gs_scene.c",
    "game/gs_task.c",
    "game/gs_title.c",
    "game/late_game.c",
    "game/menu/menu_carde_main.c",
    "game/menu/menu_carde_matrix.c",
    "game/menu/menu_common_ext.c",
    "game/menu/menu_exdisc2.c",
    "game/menu/menu_precine.c",
    "game/menu/menu_tool.c",
    "game/menu/menu_tool2.c",
    "game/save/save_carde.c",
    "hsd/hsd_cobj.c",
    "hsd/hsd_displayfunc.c",
    "hsd/hsd_fog.c",
    "hsd/hsd_initialize.c",
    "hsd/hsd_jobj.c",
    "hsd/hsd_jobj_display.c",
    "hsd/hsd_lobj.c",
    "hsd/hsd_mobj.c",
    "hsd/hsd_object.c",
    "hsd/hsd_render.c",
    "hsd/hsd_state.c",
    "hsd/hsd_util.c",
    "hsd/hsd_wobj.c",
    "trk/TRKComm.c",
    "trk/TRKInit.c",
    "trk/TRKNub.c",
    "trk/TRKSerial.c",
    "trk/TRKTarget.c",
]


def main():
    total = 0
    for rel in FAILING_FILES:
        filepath = SRC_DIR / rel
        if not filepath.exists():
            print(f"SKIP (not found): {rel}")
            continue
        fixes = fix_file(filepath)
        if fixes > 0:
            print(f"FIXED ({fixes} changes): {rel}")
            total += fixes
        else:
            print(f"NO CHANGES: {rel}")

    print(f"\nTotal fixes: {total}")


if __name__ == '__main__':
    main()
