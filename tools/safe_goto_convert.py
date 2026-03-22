#!/usr/bin/env python3
"""
Ultra-conservative goto eliminator.
Tests compilation after EACH transformation. Never produces broken code.

Strategy: apply one transform, compile-check, keep or revert. Repeat.
Tracks failed attempts to avoid infinite loops.
"""
import re
import os
import subprocess
import sys

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
        lbl = is_label(lines[i])
        if lbl:
            for j, l in enumerate(lines):
                if (j < start or j >= end) and f'goto {lbl}' in l:
                    return True
    return False

def block_terminates(block):
    ne = [l.strip() for l in block if l.strip()]
    if not ne:
        return False
    last = ne[-1]
    return (last.startswith('return') or last == 'break;' or last == 'continue;'
            or re.match(r'^goto\s+\w+\s*;$', last) is not None)

def brace_depth_change(block):
    d = 0
    for l in block:
        d += l.count('{') - l.count('}')
    return d

def indent_block(block, extra='    '):
    return [extra + l if l.strip() else l for l in block]


def try_transforms(lines, skip_ids):
    """Try one transformation. Returns (new_lines, id, desc) or None."""

    # === PASS 1: Redundant goto removal ===
    # goto L; where everything between goto and L: is just } and whitespace
    # AND brace depth is 0 (natural fallthrough)
    for i in range(len(lines)):
        g = extract_goto(lines[i])
        if not g or g[0] is not None:
            continue
        lbl = g[1]
        tid = f'redund:{i}:{lbl}'
        if tid in skip_ids:
            continue

        li = find_label(lines, lbl, i + 1)
        if li < 0:
            continue
        between = lines[i + 1:li]
        # Check: only closing braces and whitespace between
        all_close = all(not b.strip() or b.strip() == '}' for b in between)
        if all_close and brace_depth_change(between) == 0:
            new = list(lines)
            new[i] = ''
            if label_use_count(new, lbl) == 0:
                new[li] = ''
            return new, tid, f'remove redundant goto {lbl} (line {i+1})'

    # === PASS 2: OR-chain merging ===
    for i in range(len(lines)):
        g = extract_goto(lines[i])
        if not g or g[0] is None:
            continue
        cond1, lbl = g
        tid = f'orchain:{i}:{lbl}'
        if tid in skip_ids:
            continue

        j = i + 1
        while j < len(lines) and not lines[j].strip():
            j += 1
        if j >= len(lines):
            continue
        g2 = extract_goto(lines[j])
        if not g2 or g2[0] is None or g2[1] != lbl:
            continue

        conds = [cond1, g2[0]]
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

        new = list(lines)
        merged = ' || '.join(f'({c})' for c in conds)
        ind = get_indent(lines[i])
        new[i] = f'{ind}if ({merged}) goto {lbl};'
        for idx in range(i + 1, k):
            gx = extract_goto(lines[idx])
            if gx and gx[1] == lbl:
                new[idx] = ''
        return new, tid, f'OR-chain {len(conds)}x goto {lbl} (line {i+1})'

    # === PASS 3: if(cond) goto L; <code> L: where single-use, same depth, code has no external targets ===
    for i in range(len(lines)):
        g = extract_goto(lines[i])
        if not g or g[0] is None:
            continue
        cond, lbl = g
        tid = f'ifskip:{i}:{lbl}'
        if tid in skip_ids:
            continue

        li = find_label(lines, lbl, i + 1)
        if li < 0 or li <= i:
            continue
        if label_use_count(lines, lbl) != 1:
            continue
        code = lines[i + 1:li]
        if brace_depth_change(code) != 0:
            continue
        if has_labels_targeted_from_outside(lines, i + 1, li):
            continue

        ind = get_indent(lines[i])
        new = list(lines)
        replacement = [f'{ind}if ({negate(cond)}) {{']
        replacement += indent_block(code)
        replacement.append(f'{ind}}}')
        new[i:li + 1] = replacement
        return new, tid, f'if-skip single {lbl} (line {i+1})'

    # === PASS 4: if(cond) goto L; <code terminating> L: multi-use OK ===
    for i in range(len(lines)):
        g = extract_goto(lines[i])
        if not g or g[0] is None:
            continue
        cond, lbl = g
        tid = f'ifterm:{i}:{lbl}'
        if tid in skip_ids:
            continue

        li = find_label(lines, lbl, i + 1)
        if li < 0 or li <= i:
            continue
        code = lines[i + 1:li]
        if brace_depth_change(code) != 0:
            continue
        if not block_terminates(code):
            continue
        if has_labels_targeted_from_outside(lines, i + 1, li):
            continue

        ind = get_indent(lines[i])
        new = list(lines)
        replacement = [f'{ind}if ({negate(cond)}) {{']
        replacement += indent_block(code)
        replacement.append(f'{ind}}}')
        uses = label_use_count(lines, lbl)
        if uses <= 1:
            new[i:li + 1] = replacement
        else:
            new[i:li] = replacement
        return new, tid, f'if-skip-term {lbl} (line {i+1})'

    # === PASS 5: Backward goto (single-use) -> do-while/while ===
    for i in range(len(lines)):
        g = extract_goto(lines[i])
        if not g:
            continue
        cond, lbl = g
        tid = f'backloop:{i}:{lbl}'
        if tid in skip_ids:
            continue

        li = find_label(lines, lbl, 0, i)
        if li < 0:
            continue
        if label_use_count(lines, lbl) != 1:
            continue
        body = lines[li + 1:i]
        if brace_depth_change(body) != 0:
            continue

        ind = get_indent(lines[li])
        new = list(lines)
        if cond:
            replacement = [f'{ind}do {{']
            replacement += indent_block(body)
            replacement.append(f'{ind}}} while ({cond});')
        else:
            replacement = [f'{ind}while (1) {{']
            replacement += indent_block(body)
            replacement.append(f'{ind}}}')
        new[li:i + 1] = replacement
        return new, tid, f'backward loop {lbl} (line {i+1})'

    # === PASS 6: goto L; } <close-braces> L: where depth < 0 ===
    # The goto is redundant if removing it still reaches L via fallthrough
    # This is safe when between is ONLY closing braces (no else)
    for i in range(len(lines)):
        g = extract_goto(lines[i])
        if not g or g[0] is not None:
            continue
        lbl = g[1]
        tid = f'gotobrace:{i}:{lbl}'
        if tid in skip_ids:
            continue

        li = find_label(lines, lbl, i + 1)
        if li < 0:
            continue
        between = lines[i + 1:li]
        # ONLY allow pure closing braces and whitespace (NO else)
        all_close = all(
            not b.strip() or b.strip() == '}'
            for b in between
        )
        if not all_close:
            continue
        # Any depth change is OK as long as it's only }
        new = list(lines)
        new[i] = ''
        if label_use_count(new, lbl) == 0:
            new[li] = ''
        return new, tid, f'goto-close-brace {lbl} (line {i+1})'

    # === PASS 7: if-goto-else pattern ===
    # if(cond) goto A; <else-code> goto B; A: <then-code> B:
    for i in range(len(lines)):
        g = extract_goto(lines[i])
        if not g or g[0] is None:
            continue
        cond, lbl_a = g
        tid = f'ifelse:{i}:{lbl_a}'
        if tid in skip_ids:
            continue

        la = find_label(lines, lbl_a, i + 1)
        if la < 0 or la <= i + 1:
            continue
        if label_use_count(lines, lbl_a) != 1:
            continue

        # Find the goto B before label A
        pre = la - 1
        while pre > i and not lines[pre].strip():
            pre -= 1
        g2 = extract_goto(lines[pre])
        if not g2 or g2[0] is not None or pre <= i:
            continue
        lbl_b = g2[1]

        lb = find_label(lines, lbl_b, la + 1)
        if lb < 0:
            continue

        else_code = lines[i + 1:pre]
        then_code = lines[la + 1:lb]

        if brace_depth_change(else_code) != 0 or brace_depth_change(then_code) != 0:
            continue
        if has_labels_targeted_from_outside(lines, i + 1, pre):
            continue
        if has_labels_targeted_from_outside(lines, la + 1, lb):
            continue

        ind = get_indent(lines[i])
        new = list(lines)
        replacement = [f'{ind}if ({cond}) {{']
        replacement += indent_block(then_code)
        replacement.append(f'{ind}}} else {{')
        replacement += indent_block(else_code)
        replacement.append(f'{ind}}}')
        end = lb + 1 if label_use_count(lines, lbl_b) <= 1 else lb
        new[i:end] = replacement
        return new, tid, f'if-else {lbl_a}/{lbl_b} (line {i+1})'

    return None


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

def cleanup_empty(lines):
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
    skip_ids = set()

    max_rounds = 2000
    for round_num in range(max_rounds):
        result = try_transforms(lines, skip_ids)
        if result is None:
            break

        new_lines, tid, desc = result
        new_text = '\n'.join(new_lines)
        old_count = count_gotos('\n'.join(lines))
        new_count = count_gotos(new_text)

        if new_count >= old_count:
            skip_ids.add(tid)
            continue

        with open(filepath, 'w') as f:
            f.write(new_text)

        if test_compile(filepath, ver):
            removed = old_count - new_count
            total_removed += removed
            lines = new_lines
            skip_ids.clear()  # Line numbers shifted, clear skip set
        else:
            # Revert and skip this transform
            with open(filepath, 'w') as f:
                f.write('\n'.join(lines))
            skip_ids.add(tid)

    # Cleanup
    lines = cleanup_labels(lines)
    lines = cleanup_empty(lines)
    final = '\n'.join(lines)
    with open(filepath, 'w') as f:
        f.write(final)

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
        'src/game/colosseum_script.c',
        'src/game/gba/gba_conv.c',
        'src/game/gs_field_world.c',
        'src/game/gs_model.c',
        'src/game/menu/menu_common_ext.c',
        'src/game/menu/menu_tool.c',
        'src/game/ui/ui_core.c',
        'src/hsd/hsd_cobj.c',
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
        # Sort smallest first for faster initial results
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
            print(f"---  {fp}: {before} (no safe transforms)", flush=True)

    print(f"\nGrand total removed: {grand_total}", flush=True)


if __name__ == '__main__':
    main()
