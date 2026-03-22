#!/usr/bin/env python3
"""
Convert goto patterns using else-conversion and if-neg-depth patterns.
Verifies compilation after each transform.
"""
import re
import os
import subprocess
import sys
import time

def count_gotos(text):
    return len(re.findall(r'\bgoto\s+\w+\s*;', text))

def test_compile(src, ver):
    os.makedirs('build/test', exist_ok=True)
    outf = os.path.join('build', 'test', os.path.basename(src).replace('.c', '.o'))
    exe = os.path.join('tools', 'mwcc_compiler', 'GC', ver, 'mwcceppc.exe')
    cmd = f'{exe} -c -O4,p -nodefaults -proc gekko -fp hard -Cpp_exceptions off -enum int -warn off -i include -o {outf} {src}'
    result = subprocess.run(cmd, capture_output=True, text=True, timeout=120, shell=True)
    return result.returncode == 0

def is_label(line):
    s = line.strip()
    m = re.match(r'^(\w+)\s*:\s*;?\s*$', s)
    if m and not s.startswith('default') and not s.startswith('case'):
        return m.group(1)
    return None

def extract_goto(line):
    s = line.strip()
    m = re.match(r'^goto\s+(\w+)\s*;$', s)
    if m:
        return (None, m.group(1))
    m = re.match(r'^if\s*\((.+?)\)\s+goto\s+(\w+)\s*;$', s)
    if m:
        return (m.group(1), m.group(2))
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
    return f'!({cond})'

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
        lbl = is_label(lines[i])
        if lbl:
            for j, l in enumerate(lines):
                if (j < start or j >= end) and f'goto {lbl}' in l:
                    return True
    return False

def write_file(filepath, text):
    for attempt in range(3):
        try:
            with open(filepath, 'w', newline='') as f:
                f.write(text)
            return
        except PermissionError:
            if attempt < 2:
                time.sleep(1)
            else:
                raise
        except Exception:
            try:
                with open(filepath, 'w') as f:
                    f.write(text)
                return
            except:
                if attempt < 2:
                    time.sleep(1)
                else:
                    raise

def try_else_conversion(lines, skip_set):
    for i in range(len(lines)):
        g = extract_goto(lines[i])
        if not g or g[0] is not None:
            continue
        lbl = g[1]
        tid = f'else:{i}:{lbl}'
        if tid in skip_set:
            continue
        li = find_label(lines, lbl, i + 1)
        if li < 0 or li <= i + 1:
            continue
        j = i + 1
        while j < li and not lines[j].strip():
            j += 1
        if j >= li or lines[j].strip() != '}':
            continue
        else_code = lines[j + 1:li]
        if has_labels_targeted_from_outside(lines, j + 1, li):
            continue
        depth = sum(l.count('{') - l.count('}') for l in else_code)
        if depth != 0:
            continue
        ind = get_indent(lines[j])
        new = list(lines)
        new[i] = ''
        new[j] = f'{ind}}} else {{'
        for k in range(j + 1, li):
            if lines[k].strip():
                new[k] = '    ' + lines[k]
        uses = label_use_count(new, lbl)
        if uses == 0:
            new[li] = f'{ind}}}'
        else:
            new.insert(li, f'{ind}}}')
        return new, tid, f'else-pattern {lbl} (line {i+1})'
    return None

def try_if_wrap_neg_depth(lines, skip_set):
    for i in range(len(lines)):
        g = extract_goto(lines[i])
        if not g or g[0] is None:
            continue
        cond, lbl = g
        tid = f'ifneg:{i}:{lbl}'
        if tid in skip_set:
            continue
        li = find_label(lines, lbl, i + 1)
        if li < 0 or li <= i + 1:
            continue
        depth = 0
        close_brace = -1
        for k in range(i + 1, li):
            depth += lines[k].count('{') - lines[k].count('}')
            if depth == -1 and lines[k].strip() == '}':
                close_brace = k
                break
        if close_brace < 0:
            continue
        code = lines[i + 1:close_brace]
        if has_labels_targeted_from_outside(lines, i + 1, close_brace):
            continue
        code_depth = sum(l.count('{') - l.count('}') for l in code)
        if code_depth != 0:
            continue
        rest = lines[close_brace + 1:li]
        rest_depth = sum(l.count('{') - l.count('}') for l in rest)
        if rest_depth != 0:
            continue
        if has_labels_targeted_from_outside(lines, close_brace + 1, li):
            continue
        ind = get_indent(lines[i])
        new = list(lines)
        replacement = [f'{ind}if ({negate(cond)}) {{']
        for cl in code:
            if cl.strip():
                replacement.append('    ' + cl)
            else:
                replacement.append(cl)
        replacement.append(f'{ind}}}')
        uses = label_use_count(lines, lbl)
        if uses <= 1 and not rest:
            new[i:li + 1] = replacement
        else:
            new[i:close_brace + 1] = replacement
        return new, tid, f'if-neg-depth {lbl} (line {i+1})'
    return None

def get_indent(line):
    return line[:len(line) - len(line.lstrip())] if line.strip() else ''

def cleanup_labels(lines):
    changed = True
    while changed:
        changed = False
        new = []
        for line in lines:
            lbl = is_label(line)
            if lbl and label_use_count(lines, lbl) == 0:
                changed = True
                continue
            new.append(line)
        lines = new
    return lines

def process_file(filepath, ver):
    with open(filepath, 'r', newline='') as f:
        original = f.read()
    line_ending = '\r\n' if '\r\n' in original else '\n'
    normalized = original.replace('\r\n', '\n')
    lines = normalized.split('\n')
    total_removed = 0
    skip_set = set()
    transforms = [try_else_conversion, try_if_wrap_neg_depth]

    for round_num in range(3000):
        applied = False
        for transform_fn in transforms:
            result = transform_fn(lines, skip_set)
            if result is None:
                continue
            new_lines, tid, desc = result
            new_text = '\n'.join(new_lines)
            old_count = count_gotos('\n'.join(lines))
            new_count = count_gotos(new_text)
            if new_count >= old_count:
                skip_set.add(tid)
                continue
            write_file(filepath, new_text.replace('\n', line_ending))
            if test_compile(filepath, ver):
                removed = old_count - new_count
                total_removed += removed
                lines = new_lines
                skip_set.clear()
                applied = True
                break
            else:
                write_file(filepath, '\n'.join(lines).replace('\n', line_ending))
                skip_set.add(tid)
        if not applied:
            break

    lines = cleanup_labels(lines)
    final = '\n'.join(lines).replace('\n', line_ending)
    write_file(filepath, final)
    if not test_compile(filepath, ver):
        write_file(filepath, original)
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
        'src/game/menu/menu_common_ext.c',
        'src/game/menu/menu_middle.c',
        'src/game/menu/menu_tool.c',
        'src/game/ui/ui_core.c',
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
                        gotos = count_gotos(f.read())
                    if gotos > 0 and fp in cmap:
                        targets.append(fp)
        targets.sort(key=lambda fp: count_gotos(open(fp, 'r', errors='ignore').read()))

    grand_total = 0
    for fp in targets:
        ver = cmap.get(fp)
        if not ver:
            continue
        if not test_compile(fp, ver):
            print(f"SKIP (broken): {fp}", flush=True)
            continue
        with open(fp, 'r', errors='ignore') as f:
            before = count_gotos(f.read())
        removed = process_file(fp, ver)
        with open(fp, 'r', errors='ignore') as f:
            after = count_gotos(f.read())
        if removed > 0:
            grand_total += removed
            print(f"OK  {fp}: {before} -> {after} (-{removed}) [total: {grand_total}]", flush=True)
        else:
            print(f"---  {fp}: {before}", flush=True)
    print(f"\nGrand total removed: {grand_total}", flush=True)

if __name__ == '__main__':
    main()
