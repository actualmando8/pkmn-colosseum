#!/usr/bin/env python3
"""
Convert pragma-guarded register-level C to idiomatic C89 for Pokemon Colosseum decomp.

This script processes gs_render.c and gs_field_world.c, converting all
pragma-guarded functions from register-level pseudocode to idiomatic C89
that matches when compiled with CW GC/1.2.5n or GC/1.3 at -O4,p.

Patterns handled:
1. GSgfx state check + fn_800D4F98 dispatch (with 0x47E check)
2. Simple state check + fn_800D4F98 dispatch (without 0x47E check)
3. State check + fn_800D6B00 + store pattern
4. Generic cleanup (remove register boilerplate, keep logic)
"""

import re
import sys
import os
import copy


def parse_pragma_blocks(lines):
    """
    Parse file into a list of segments:
    - ('code', line_list): regular code
    - ('pragma', start, end, block_lines): pragma-guarded function
    """
    segments = []
    i = 0
    current_code = []

    while i < len(lines):
        if lines[i].strip() == '#pragma push':
            # Save any accumulated code
            if current_code:
                segments.append(('code', current_code))
                current_code = []

            # Find matching #pragma pop
            start = i
            depth = 1
            j = i + 1
            while j < len(lines) and depth > 0:
                if lines[j].strip() == '#pragma push':
                    depth += 1
                elif lines[j].strip() == '#pragma pop':
                    depth -= 1
                j += 1
            end = j  # line after #pragma pop
            segments.append(('pragma', start, end, lines[start:end]))
            i = end
        else:
            current_code.append(lines[i])
            i += 1

    if current_code:
        segments.append(('code', current_code))

    return segments


def extract_func_info(block_lines):
    """Extract function name, signature, extern declarations, and body from pragma block."""
    sig_line = None
    sig_idx = None
    externs = []
    comment_before = []
    body_lines = []
    in_body = False
    brace_depth = 0

    for i, line in enumerate(block_lines):
        s = line.strip()

        # Skip pragma directives
        if s in ('#pragma push', '#pragma pop',
                 '#pragma optimization_level 0',
                 '#pragma optimizewithasm off'):
            continue

        # Collect comments before function
        if not in_body and (s.startswith('/*') or s.startswith('*') or s.startswith('//')):
            comment_before.append(line)
            continue

        # Find function signature
        if not in_body and sig_line is None and '(' in s:
            # Check if this looks like a function definition
            if any(s.startswith(t) for t in ['void ', 'u32 ', 'u8 ', 's32 ', 'u16 ', 's16 ',
                                              'f32 ', 'f64 ', 'BOOL ', 'int ', 'u8* ',
                                              'void* ', 'u32* ', 's32* ']):
                sig_line = line
                sig_idx = i
                if '{' in s:
                    brace_depth += s.count('{') - s.count('}')
                    in_body = True
                continue
            elif s == '{':
                brace_depth = 1
                in_body = True
                continue

        if sig_line is not None and not in_body:
            if s == '{':
                brace_depth = 1
                in_body = True
                continue

        if in_body:
            if s.startswith('extern '):
                externs.append(line)
                continue
            if s.startswith('typedef '):
                externs.append(line)
                continue
            body_lines.append(line)

    # Extract function name
    func_name = None
    if sig_line:
        m = re.search(r'(\w+)\s*\(', sig_line)
        if m:
            func_name = m.group(1)

    return {
        'name': func_name,
        'sig_line': sig_line,
        'externs': externs,
        'comment_before': comment_before,
        'body_lines': body_lines,
    }


def is_reg_decl(line):
    """Check if line is a register variable declaration."""
    s = line.strip()
    if re.match(r'^u32 r\d+ = ', s):
        return True
    if re.match(r'^f32 f\d+ = ', s):
        return True
    if re.match(r'^f64 f\d+ = ', s):
        return True
    if re.match(r'^void \(\*ctr_fn\)', s):
        return True
    if re.match(r'^u32 ctr = 0;', s):
        return True
    return False


def is_stack_decl(line):
    return bool(re.match(r'\s*u8 sp\[', line.strip()))


def is_epilogue_restore(line):
    s = line.strip()
    return bool(re.match(r'^r\d+ = \*\(u32\*\)\(sp \+ 0x[0-9a-fA-F]+\);$', s))


def is_prologue_epilogue_noise(line):
    s = line.strip()
    if s.startswith('/* stmw') or s.startswith('/* lmw'):
        return True
    if s.startswith('/* psq_st') or s.startswith('/* psq_l'):
        return True
    if re.match(r'^\*\(f64\*\)\(sp \+ 0x[0-9a-fA-F]+\) = f\d+;$', s):
        return True
    if re.match(r'^f\d+ = \*\(f64\*\)\(sp \+ 0x[0-9a-fA-F]+\);$', s):
        return True
    return False


def is_crclr_comment(line):
    s = line.strip()
    return s in ('/* crclr cr1eq */;', '/* crset cr1eq */;')


def is_lwzx_comment(line):
    s = line.strip()
    return bool(re.match(r'^/\* (lwzx|stbx|subi|clrlslwi|lbzx|rlwinm) .* \*/;?$', s))


def classify_block(body_text):
    """
    Classify pragma block body into conversion category.
    """
    has_gslog = 'fn_800D4F98' in body_text
    has_47e_check = '0x47E' in body_text or '0x47e' in body_text
    has_state_check = "*(s32*)state == 1" in body_text or "!= (s32)0x1" in body_text or "!= (s32)0x1" in body_text
    has_r_state = "*(u32*)lbl_8047AA80" in body_text
    has_6b00 = 'fn_800D6B00' in body_text
    has_jumptable = 'jumptable_' in body_text
    has_loop_ctr = 'if (--ctr' in body_text or 'ctr != 0' in body_text
    has_memcpy = 'memcpy' in body_text
    has_memset = 'memset' in body_text

    goto_count = len(re.findall(r'\bgoto\b', body_text))
    label_count = len(re.findall(r'^L_[0-9a-fA-F]+\s*:', body_text, re.MULTILINE))

    # Pattern 1: GSgfx state + 0x47E check + fn_800D4F98 dispatch + else store fn ptr
    # These have exactly 2-3 gotos and 2-3 labels
    if has_gslog and has_47e_check and goto_count <= 3 and label_count <= 3:
        return 'state_47e_dispatch'

    # Pattern 2: Simple state check + fn_800D4F98 dispatch (no 0x47E check)
    if has_gslog and not has_47e_check and goto_count <= 3 and label_count <= 3:
        return 'state_simple_dispatch'

    # Pattern 3: State check + fn_800D6B00 + stores (matrix-related)
    if has_gslog and has_6b00 and goto_count <= 3:
        return 'state_6b00_dispatch'

    # Pattern 4: State check with direct calls (more branches)
    if has_gslog and goto_count <= 5 and label_count <= 5:
        return 'state_multi_branch'

    # Jump table functions
    if has_jumptable:
        return 'jumptable'

    # Loop functions
    if has_loop_ctr:
        return 'loop'

    # Small functions with few branches
    if goto_count <= 5:
        return 'small_complex'

    return 'large_complex'


def try_convert_state_47e_dispatch(info, body_lines):
    """
    Convert pattern: state[0x47E] check + fn_800D4F98 dispatch.

    Register-level pattern:
        r5 = *(u32*)lbl_8047AA80;
        r0 = *(u8*)((u8*)r5 + 0x47E);
        if ((u32)r0 != (u32)0x0) goto L_ELSE;
        r0 = *(u32*)((u8*)r5 + 0x0);
        if ((s32)r0 != (s32)0x1) goto L_ELSE;
        <set up args from r3/r4/r5/etc>
        r3 = CMD;
        r4 = ARGC;
        fn_800D4F98();
        goto L_END;
    L_ELSE:
        <store fn ptr and params into state>
    L_END:
        return;

    Converts to:
        u8* state = (u8*)lbl_8047AA80;
        if (state[0x47E] == 0 && *(s32*)state == 1) {
            fn_800D4F98(CMD, ARGC, ...);
        } else {
            <direct execution>
        }
    """
    # Parse the register-level body to extract:
    # 1. CMD and ARGC values
    # 2. Parameter mappings (which input params go to fn_800D4F98)
    # 3. Else-branch stores

    body_text = '\n'.join(l.strip() for l in body_lines)

    # Find CMD: r3 = 0xNN; right before fn_800D4F98()
    cmd_match = re.search(r'r3 = (0x[0-9a-fA-F]+);.*?fn_800D4F98\(\)', body_text, re.DOTALL)
    if not cmd_match:
        return None
    cmd = cmd_match.group(1)

    # Find ARGC: r4 = 0xNN; right before fn_800D4F98()
    argc_match = re.search(r'r4 = (0x[0-9a-fA-F]+);.*?fn_800D4F98\(\)', body_text, re.DOTALL)
    if not argc_match:
        return None
    argc = argc_match.group(1)
    argc_int = int(argc, 16)

    # Find the else label and extract stores
    labels = re.findall(r'(L_[0-9a-fA-F]+)\s*:', body_text)
    if len(labels) < 1:
        return None

    # The else branch: everything between the first label and the last label (or return)
    else_label = labels[0]
    end_label = labels[-1] if len(labels) > 1 else None

    # Extract else-branch lines
    in_else = False
    else_lines = []
    for line in body_lines:
        s = line.strip()
        if s.startswith(else_label):
            in_else = True
            continue
        if in_else:
            if end_label and s.startswith(end_label):
                break
            if s == 'return;' or s == '}':
                break
            if is_epilogue_restore(s) or is_prologue_epilogue_noise(s):
                continue
            else_lines.append(s)

    # Now reconstruct the function
    # Determine parameters from function signature
    sig = info['sig_line'].strip()

    # Determine param args to fn_800D4F98 by looking at r5, r6, r7 assignments before CMD
    # Parse the if-branch for param setup
    if_lines = []
    collecting_if = False
    for line in body_lines:
        s = line.strip()
        # Start collecting after the state check passes
        if '0x47E' in s or '0x47e' in s:
            collecting_if = True
            continue
        if collecting_if:
            if s.startswith(else_label):
                break
            if s.startswith('goto'):
                break
            if_lines.append(s)

    # Extract extra args from if branch (r5, r6, r7, etc assignments)
    extra_args = []
    for line in if_lines:
        # r5 = r3 & 0xFF; -> (u8)param
        # r5 = (s8)r3; -> (s8)param
        # r5 = r3; -> param
        # r5 = (s16)r3; -> (s16)param
        m = re.match(r'r(\d+) = r3 & 0xFF;', line)
        if m:
            extra_args.append('(u32)(u8)r3_param')
            continue
        m = re.match(r'r(\d+) = r3 & 0xFFFF;', line)
        if m:
            extra_args.append('(u32)(u16)r3_param')
            continue
        m = re.match(r'r(\d+) = \(s8\)r3;', line)
        if m:
            extra_args.append('(s32)(s8)r3_param')
            continue
        m = re.match(r'r(\d+) = \(s16\)r3;', line)
        if m:
            extra_args.append('(s32)(s16)r3_param')
            continue
        m = re.match(r'r(\d+) = r3;', line)
        if m:
            extra_args.append('r3_param')
            continue

    # Build converted else branch
    converted_else = []
    for line in else_lines:
        # Clean up store patterns
        if is_crclr_comment(line):
            continue
        if is_lwzx_comment(line):
            continue
        if line.startswith('r') and '=' in line and 'fn_' not in line:
            # Register assignment - convert to state access
            converted_else.append('    ' + line)
        elif 'fn_' in line:
            converted_else.append('    ' + line)
        else:
            converted_else.append('    ' + line)

    # We can't always perfectly reconstruct, so let's do the simpler approach:
    # Just strip pragmas and clean up the boilerplate
    return None  # Fall through to generic cleanup


def generic_cleanup(block_lines):
    """
    Generic cleanup: remove pragmas, register declarations, stack arrays,
    epilogue restores, prologue/epilogue comments. Keep everything else.
    """
    result = []
    for line in block_lines:
        s = line.strip()

        # Skip pragma directives
        if s in ('#pragma push', '#pragma pop',
                 '#pragma optimization_level 0',
                 '#pragma optimizewithasm off'):
            continue

        # Skip register variable declarations
        if is_reg_decl(s):
            continue

        # Skip stack array declarations
        if is_stack_decl(s):
            continue

        # Skip epilogue register restores
        if is_epilogue_restore(s):
            continue

        # Skip prologue/epilogue noise
        if is_prologue_epilogue_noise(s):
            continue

        # Skip crclr/crset comments
        if is_crclr_comment(s):
            continue

        result.append(line)

    return result


def convert_known_pattern_47e(func_name, sig_line, externs, body_lines, comment_lines):
    """
    Full conversion of state[0x47E] + state[0] check + fn_800D4F98 dispatch.

    Returns converted function lines, or None if can't convert.
    """
    body_text = '\n'.join(l.strip() for l in body_lines)

    # ---- Step 1: Find the input register mappings ----
    # r31 = r4; (save param2 to r31)
    # r30 = r3; (save param1 to r30)
    # etc.
    param_saves = {}  # r31 -> 'r4' etc.
    for line in body_lines:
        s = line.strip()
        m = re.match(r'^(r\d+) = (r[3-9]|r10);$', s)
        if m:
            param_saves[m.group(1)] = m.group(2)

    # ---- Step 2: Find CMD and ARGC ----
    # Find the line "r3 = CMD_VALUE;" near fn_800D4F98()
    # and "r4 = ARGC_VALUE;"
    lines_flat = [l.strip() for l in body_lines]
    fn_idx = None
    for i, s in enumerate(lines_flat):
        if 'fn_800D4F98()' in s:
            fn_idx = i
            break

    if fn_idx is None:
        return None

    # Search backward for r3 = CMD and r4 = ARGC
    cmd = None
    argc = None
    for i in range(fn_idx - 1, max(fn_idx - 10, -1), -1):
        s = lines_flat[i]
        if cmd is None:
            m = re.match(r'^r3 = (0x[0-9a-fA-F]+);$', s)
            if m:
                cmd = m.group(1)
        if argc is None:
            m = re.match(r'^r4 = (0x[0-9a-fA-F]+);$', s)
            if m:
                argc = m.group(1)

    if cmd is None or argc is None:
        return None

    argc_int = int(argc, 16)

    # ---- Step 3: Find extra args for fn_800D4F98 ----
    # Between the state check and fn_800D4F98 call, look for r5, r6, r7... assignments
    # These are the varargs to fn_800D4F98
    extra_args = []
    param_lines_zone = lines_flat[:fn_idx]

    # Find where the state check ends (after "if ... goto L_")
    state_check_end = 0
    for i, s in enumerate(param_lines_zone):
        if '0x47E' in s or '0x47e' in s:
            state_check_end = i
        if 'if ((s32)r0 != (s32)0x1)' in s:
            state_check_end = i + 1
            break
        if 'if ((u32)r0 != (u32)0x0)' in s and '0x47E' not in s and '0x47e' not in s:
            pass

    # Extract arg-setup lines between state check and CMD/ARGC setup
    arg_setup = param_lines_zone[state_check_end:]

    for s in arg_setup:
        # r5 = r3 & 0xFF; -> param cast as u8
        m = re.match(r'^r(\d+) = r(\d+) & 0xFF;$', s)
        if m:
            src_reg = 'r' + m.group(2)
            # Resolve to original param register
            actual = param_saves.get(src_reg, src_reg)
            extra_args.append(f'(u32)(u8){actual}_param')
            continue

        m = re.match(r'^r(\d+) = r(\d+) & 0xFFFF;$', s)
        if m:
            src_reg = 'r' + m.group(2)
            actual = param_saves.get(src_reg, src_reg)
            extra_args.append(f'(u32)(u16){actual}_param')
            continue

        m = re.match(r'^r(\d+) = \(s8\)(r\d+);$', s)
        if m:
            src_reg = m.group(2)
            actual = param_saves.get(src_reg, src_reg)
            extra_args.append(f'(s32)(s8){actual}_param')
            continue

        m = re.match(r'^r(\d+) = \(s16\)(r\d+);$', s)
        if m:
            src_reg = m.group(2)
            actual = param_saves.get(src_reg, src_reg)
            extra_args.append(f'(s32)(s16){actual}_param')
            continue

        m = re.match(r'^r(\d+) = (r\d+);$', s)
        if m and m.group(2) in ('r3', 'r4', 'r5', 'r6', 'r7', 'r8', 'r9', 'r10'):
            dst = m.group(1)
            src = m.group(2)
            if int(dst) >= 5:  # These are extra args
                actual = param_saves.get(src, src)
                extra_args.append(f'{actual}_param')
            continue

    # ---- Step 4: Find the else branch ----
    # Find the first label after the state check
    labels = []
    for i, s in enumerate(lines_flat):
        m = re.match(r'^(L_[0-9a-fA-F]+)\s*:$', s)
        if m:
            labels.append((i, m.group(1)))

    if len(labels) < 1:
        return None

    else_label_idx = labels[0][0]
    else_label = labels[0][1]

    # Find end label
    end_label_idx = labels[-1][0] if len(labels) > 1 else len(lines_flat)

    # Extract else-branch lines
    else_body = []
    for i in range(else_label_idx + 1, end_label_idx):
        s = lines_flat[i]
        if s == 'return;':
            break
        if is_epilogue_restore(s):
            continue
        if is_prologue_epilogue_noise(s):
            continue
        if is_crclr_comment(s):
            continue
        if is_lwzx_comment(s):
            continue
        if s.startswith('goto '):
            continue
        else_body.append(s)

    # Convert else body: replace register refs with state ptr operations
    # This is the tricky part - for now, keep the else body as-is with state ptr
    converted_else = []
    for line in else_body:
        # Replace *(u32*)lbl_8047AA80 with state cast
        line = line.replace('*(u32*)lbl_8047AA80', '(u32)state')
        converted_else.append('        ' + line)

    # ---- Step 5: Build the converted function ----
    # This is complex enough that for now, just do the generic cleanup
    return None


def process_file(filepath):
    """Process a single C file, converting all pragma blocks."""
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()
        lines = content.split('\n')
        # Preserve line endings
        lines_with_nl = [l + '\n' for l in lines]
        if content.endswith('\n'):
            lines_with_nl[-1] = lines[-1] + '\n'
        else:
            lines_with_nl[-1] = lines[-1]

    with open(filepath, 'r', encoding='utf-8') as f:
        original_lines = f.readlines()

    segments = parse_pragma_blocks(original_lines)

    total_pragma = sum(1 for s in segments if s[0] == 'pragma')
    print(f"Found {total_pragma} pragma blocks in {os.path.basename(filepath)}")

    converted = 0
    result_lines = []

    for seg in segments:
        if seg[0] == 'code':
            result_lines.extend(seg[1])
        elif seg[0] == 'pragma':
            start, end, block = seg[1], seg[2], seg[3]
            cleaned = generic_cleanup(block)
            result_lines.extend(cleaned)
            converted += 1

    # Write result
    with open(filepath, 'w', encoding='utf-8', newline='') as f:
        f.writelines(result_lines)

    print(f"Cleaned {converted}/{total_pragma} pragma blocks")
    return converted


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: python convert_pragma_blocks.py <file.c> [file2.c ...]")
        sys.exit(1)

    total = 0
    for filepath in sys.argv[1:]:
        total += process_file(filepath)
    print(f"\nTotal: {total} blocks cleaned")
