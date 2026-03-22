#!/usr/bin/env python3
"""
Ultra-conservative goto eliminator that verifies compilation after EACH transformation.
Only applies one transformation at a time, tests compilation, then continues or reverts.
Tracks which gotos failed to convert so it doesn't retry them infinitely.
"""
import re
import os
import subprocess
import sys

def count_gotos_text(text):
    return len(re.findall(r'\bgoto\s+\w+\s*;', text))

def test_compile(src, ver):
    os.makedirs('build/test', exist_ok=True)
    outf = os.path.join('build', 'test', os.path.basename(src).replace('.c', '.o'))
    exe = os.path.join('tools', 'mwcc_compiler', 'GC', ver, 'mwcceppc.exe')
    cmd = f'{exe} -c -O4,p -nodefaults -proc gekko -fp hard -Cpp_exceptions off -enum int -warn off -i include -o {outf} {src}'
    result = subprocess.run(cmd, capture_output=True, text=True, timeout=120, shell=True)
    return result.returncode == 0

def is_label_line(line):
    s = line.strip()
    m = re.match(r'^(\w+)\s*:\s*;?\s*$', s)
    if m and not s.startswith('default') and not s.startswith('case'):
        return m.group(1)
    return None

def extract_goto(line):
    s = line.strip()
    m = re.match(r'^if\s*\((.+?)\)\s+goto\s+(\w+)\s*;$', s)
    if m:
        return (m.group(1), m.group(2))
    m = re.match(r'^goto\s+(\w+)\s*;$', s)
    if m:
        return (None, m.group(1))
    return None

def negate(cond):
    cond = cond.strip()
    if cond.startswith('!(') and cond.endswith(')'):
        return cond[2:-1]
    ops = {'!=':'==','==':'!=','>=':'<','<=':'>','>':'<=','<':'>='}
    m = re.match(r'^(.*?)\s*(!=|==|>=|<=|>(?!=)|<(?!=))\s*(.*)$', cond)
    if m:
        l, op, r = m.groups()
        if op in ops:
            return f'{l} {ops[op]} {r}'
    if '&&' in cond or '||' in cond:
        return f'!({cond})'
    return f'!({cond})'

def get_indent(line):
    return line[:len(line) - len(line.lstrip())] if line.strip() else ''

def find_label(lines, label, start=0, end=None):
    if end is None:
        end = len(lines)
    t = label + ':'
    for i in range(start, end):
        s = lines[i].strip()
        if s == t or s == t + ' ;' or s == t + ';':
            return i
    return -1

def label_use_count(lines, label):
    pat = f'goto {label}'
    return sum(1 for l in lines if pat in l)

def has_labels_targeted_from_outside(lines, start, end):
    for i in range(start, end):
        lbl = is_label_line(lines[i])
        if lbl:
            for j, l in enumerate(lines):
                if (j < start or j >= end) and f'goto {lbl}' in l:
                    return True
    return False

def block_terminates(lines_block):
    ne = [l.strip() for l in lines_block if l.strip()]
    if not ne:
        return False
    last = ne[-1]
    return (last.startswith('return') or last == 'break;' or last == 'continue;'
            or re.match(r'^goto\s+\w+\s*;$', last) is not None)

def is_only_structural(lines_block):
    for l in lines_block:
        s = l.strip()
        if not s:
            continue
        if s == '}':
            continue
        if is_label_line(l):
            continue
        return False
    return True

def brace_depth_change(lines_block):
    depth = 0
    for l in lines_block:
        depth += l.count('{') - l.count('}')
    return depth

def indent_block(block, extra='    '):
    result = []
    for l in block:
        if l.strip():
            result.append(extra + l)
        else:
            result.append(l)
    return result


def find_all_transformations(lines, skip_set):
    """Find all possible transformations, return list of (priority, line_idx, transform_func, desc)."""
    transforms = []

    for i in range(len(lines)):
        if i in skip_set:
            continue
        g = extract_goto(lines[i])
        if not g:
            continue

        cond, lbl = g

        # Pattern 1: Redundant forward unconditional goto
        if cond is None:
            li = find_label(lines, lbl, i + 1)
            if li > i:
                between = lines[i + 1:li]
                if is_only_structural(between) and brace_depth_change(between) == 0:
                    def make_t1(i=i, lbl=lbl, li=li):
                        new_lines = list(lines)
                        new_lines[i] = ''
                        if label_use_count(new_lines, lbl) == 0:
                            new_lines[li] = ''
                        return new_lines
                    transforms.append((0, i, make_t1, f'redundant goto {lbl} at line {i+1}'))
                    continue

        # Pattern 2: OR-chain
        if cond is not None:
            j = i + 1
            while j < len(lines) and not lines[j].strip():
                j += 1
            if j < len(lines):
                g2 = extract_goto(lines[j])
                if g2 and g2[0] is not None and g2[1] == lbl:
                    conds = [cond, g2[0]]
                    k = j + 1
                    while k < len(lines):
                        if not lines[k].strip():
                            k += 1
                            continue
                        gk = extract_goto(lines[k])
                        if gk and gk[0] is not None and gk[1] == lbl:
                            conds.append(gk[0])
                            k += 1
                        else:
                            break
                    if len(conds) > 1:
                        def make_t2(i=i, lbl=lbl, conds=conds[:], k=k):
                            new_lines = list(lines)
                            merged = ' || '.join(f'({c})' for c in conds)
                            ind = get_indent(lines[i])
                            new_lines[i] = f'{ind}if ({merged}) goto {lbl};'
                            for idx in range(i + 1, k):
                                gx = extract_goto(lines[idx])
                                if gx and gx[1] == lbl:
                                    new_lines[idx] = ''
                            return new_lines
                        transforms.append((1, i, make_t2, f'OR-chain {len(conds)}x -> goto {lbl} at line {i+1}'))
                        continue

        # Pattern 3: Forward conditional skip (single-use label)
        if cond is not None:
            li = find_label(lines, lbl, i + 1)
            if li > i and label_use_count(lines, lbl) == 1:
                code = lines[i + 1:li]
                if (not has_labels_targeted_from_outside(lines, i + 1, li)
                    and brace_depth_change(code) == 0):
                    def make_t3(i=i, lbl=lbl, li=li, cond=cond, code=code[:]):
                        new_lines = list(lines)
                        ind = get_indent(lines[i])
                        replacement = [f'{ind}if ({negate(cond)}) {{']
                        replacement += indent_block(code)
                        replacement.append(f'{ind}}}')
                        new_lines[i:li + 1] = replacement
                        return new_lines
                    transforms.append((2, i, make_t3, f'if-skip single {lbl} at line {i+1}'))
                    continue

        # Pattern 4: Forward conditional skip (terminates, multi-use OK)
        if cond is not None:
            li = find_label(lines, lbl, i + 1)
            if li > i:
                code = lines[i + 1:li]
                if (block_terminates(code)
                    and not has_labels_targeted_from_outside(lines, i + 1, li)
                    and brace_depth_change(code) == 0):
                    def make_t4(i=i, lbl=lbl, li=li, cond=cond, code=code[:]):
                        new_lines = list(lines)
                        ind = get_indent(lines[i])
                        replacement = [f'{ind}if ({negate(cond)}) {{']
                        replacement += indent_block(code)
                        replacement.append(f'{ind}}}')
                        uses = label_use_count(lines, lbl)
                        if uses <= 1:
                            new_lines[i:li + 1] = replacement
                        else:
                            new_lines[i:li] = replacement
                        return new_lines
                    transforms.append((3, i, make_t4, f'if-skip-term {lbl} at line {i+1}'))
                    continue

        # Pattern 5: Backward goto (single-use) -> do-while
        li = find_label(lines, lbl, 0, i)
        if li >= 0 and label_use_count(lines, lbl) == 1:
            body = lines[li + 1:i]
            if brace_depth_change(body) == 0:
                def make_t5(i=i, lbl=lbl, li=li, cond=cond, body=body[:]):
                    new_lines = list(lines)
                    ind = get_indent(lines[li])
                    if cond:
                        replacement = [f'{ind}do {{']
                        replacement += indent_block(body)
                        replacement.append(f'{ind}}} while ({cond});')
                    else:
                        replacement = [f'{ind}while (1) {{']
                        replacement += indent_block(body)
                        replacement.append(f'{ind}}}')
                    new_lines[li:i + 1] = replacement
                    return new_lines
                transforms.append((4, i, make_t5, f'backward loop {lbl} at line {i+1}'))
                continue

        # Pattern 6: goto before closing brace -> fallthrough
        if cond is None:
            j = i + 1
            while j < len(lines) and not lines[j].strip():
                j += 1
            if j < len(lines) and lines[j].strip().startswith('}'):
                li = find_label(lines, lbl, j)
                if li >= 0:
                    between = lines[j + 1:li]
                    all_struct = all(
                        not b.strip() or b.strip() == '}' or b.strip() == '} else {' or is_label_line(b)
                        for b in between
                    )
                    if all_struct and brace_depth_change(lines[i+1:li]) == 0:
                        def make_t6(i=i, lbl=lbl, li=li):
                            new_lines = list(lines)
                            new_lines[i] = ''
                            if label_use_count(new_lines, lbl) == 0:
                                new_lines[li] = ''
                            return new_lines
                        transforms.append((5, i, make_t6, f'goto-brace {lbl} at line {i+1}'))
                        continue

    transforms.sort(key=lambda x: x[0])
    return transforms


def cleanup_labels(lines):
    """Remove unused labels."""
    changed = True
    while changed:
        changed = False
        new = []
        for line in lines:
            lbl = is_label_line(line)
            if lbl and label_use_count(lines, lbl) == 0:
                changed = True
                continue
            new.append(line)
        lines = new
    return lines


def cleanup_empty_lines(lines):
    """Remove consecutive blank lines."""
    new = []
    for line in lines:
        if not line.strip() and new and not new[-1].strip():
            continue
        new.append(line)
    return new


def process_file(filepath, ver):
    with open(filepath, 'r') as f:
        original = f.read()

    lines = original.split('\n')
    total_removed = 0
    skip_set = set()  # Line indices that failed compilation after transform

    max_rounds = 500
    for round_num in range(max_rounds):
        transforms = find_all_transformations(lines, skip_set)
        if not transforms:
            break

        applied = False
        for priority, line_idx, make_func, desc in transforms:
            new_lines = make_func()

            old_count = count_gotos_text('\n'.join(lines))
            new_count = count_gotos_text('\n'.join(new_lines))

            if new_count >= old_count:
                skip_set.add(line_idx)
                continue

            # Test compilation
            new_text = '\n'.join(new_lines)
            with open(filepath, 'w') as f:
                f.write(new_text)

            if test_compile(filepath, ver):
                removed = old_count - new_count
                total_removed += removed
                lines = new_lines
                # Clear skip_set since line numbers changed
                skip_set.clear()
                applied = True
                break
            else:
                # Revert
                with open(filepath, 'w') as f:
                    f.write('\n'.join(lines))
                skip_set.add(line_idx)
                continue

        if not applied:
            break

    # Final cleanup: remove unused labels, empty lines
    lines = cleanup_labels(lines)
    lines = cleanup_empty_lines(lines)
    final_text = '\n'.join(lines)
    with open(filepath, 'w') as f:
        f.write(final_text)

    # Verify final state compiles
    if not test_compile(filepath, ver):
        with open(filepath, 'w') as f:
            f.write(original)
        return 0

    return total_removed


def main():
    with open('build.ninja', 'r') as f:
        content = f.read()
    rules = re.findall(r'build\s+(\S+\.o)\s*:\s*(cc_GC_\w+)\s+(\S+\.c)', content)
    cmap = {}
    for obj, rule, src in rules:
        if '1_2_5n' in rule:
            cmap[src] = '1.2.5n'
        elif '1_3' in rule:
            cmap[src] = '1.3'

    BROKEN = {
        'src/game/battle/battle_logic.c',
        'src/game/gba/gba_conv.c',
        'src/game/gs_field_world.c',
        'src/game/gs_thread.c',
        'src/game/menu/menu_common_ext.c',
        'src/game/menu/menu_tool.c',
        'src/game/ui/ui_core.c',
        'src/hsd/hsd_cobj.c',
        'src/hsd/hsd_lobj.c',
    }

    if len(sys.argv) > 1:
        targets = sys.argv[1:]
    else:
        targets = []
        for root, dirs, filenames in os.walk('src'):
            for fn in filenames:
                if fn.endswith('.c'):
                    fp = os.path.join(root, fn).replace(os.sep, '/')
                    if fp in BROKEN:
                        continue
                    with open(fp, 'r', errors='ignore') as f:
                        gotos = count_gotos_text(f.read())
                    if gotos > 0 and fp in cmap:
                        targets.append(fp)
        targets.sort(key=lambda fp: count_gotos_text(open(fp, 'r', errors='ignore').read()))

    grand_total = 0
    for fp in targets:
        ver = cmap.get(fp)
        if not ver:
            continue
        if not test_compile(fp, ver):
            with open(fp, 'r', errors='ignore') as f:
                before = count_gotos_text(f.read())
            print(f"SKIP (broken): {fp} ({before} gotos)", flush=True)
            continue

        with open(fp, 'r', errors='ignore') as f:
            before = count_gotos_text(f.read())
        removed = process_file(fp, ver)
        with open(fp, 'r', errors='ignore') as f:
            after = count_gotos_text(f.read())
        if removed > 0:
            grand_total += removed
            print(f"OK  {fp}: {before} -> {after} (-{removed})", flush=True)
        else:
            print(f"---  {fp}: {before} (no safe transforms)", flush=True)

    print(f"\nTotal removed: {grand_total}", flush=True)


if __name__ == '__main__':
    main()
