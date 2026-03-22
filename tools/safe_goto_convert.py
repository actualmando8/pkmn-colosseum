#!/usr/bin/env python3
"""
Ultra-conservative goto eliminator that verifies compilation after EACH transformation.
Only applies one transformation at a time, tests compilation, then continues or reverts.

Patterns handled (each verified individually):
1. if(cond) goto L; <code ending with return/break/continue> L:  (single-use)
   -> if(!cond) { <code> }
2. goto L; <only closing braces and whitespace> L:
   -> remove goto (redundant fallthrough)
3. if(A) goto L; if(B) goto L; -> if(A || B) goto L;
4. Backward goto to single-use label -> do { ... } while(cond);
5. if(cond) goto L; <code> L:  where code terminates (multi-use)
   -> if(!cond) { <code> }
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
    m = re.match(r'^(L_[0-9A-Fa-f]+|\w+)\s*:\s*;?\s*$', s)
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
    m = re.search(r'^(.*?)\s*(!=|==|>=|<=|>(?!=)|<(?!=))\s*(.*)$', cond)
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
    """Check if any label in lines[start:end] is goto-targeted from outside that range."""
    for i in range(start, end):
        lbl = is_label_line(lines[i])
        if lbl:
            for j, l in enumerate(lines):
                if (j < start or j >= end) and f'goto {lbl}' in l:
                    return True
    return False

def block_terminates(lines_block):
    """Check if a block always exits (return/break/continue/goto)."""
    ne = [l.strip() for l in lines_block if l.strip()]
    if not ne:
        return False
    last = ne[-1]
    return (last.startswith('return') or last == 'break;' or last == 'continue;'
            or re.match(r'^goto\s+\w+\s*;$', last) is not None)

def is_only_structural(lines_block):
    """Check if block contains only closing braces, whitespace, labels."""
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
    """Count net brace depth change in a block of lines."""
    depth = 0
    for l in lines_block:
        depth += l.count('{') - l.count('}')
    return depth


def try_one_transformation(lines):
    """Try to apply one goto elimination. Returns (new_lines, description) or None."""

    # Pattern 1: Redundant forward unconditional goto
    # goto L; <only braces/whitespace/labels> L:  ->  remove goto
    for i in range(len(lines)):
        g = extract_goto(lines[i])
        if g and g[0] is None:
            lbl = g[1]
            li = find_label(lines, lbl, i + 1)
            if li > i:
                between = lines[i + 1:li]
                if is_only_structural(between) and brace_depth_change(between) == 0:
                    new_lines = list(lines)
                    new_lines[i] = ''  # Remove goto
                    if label_use_count(new_lines, lbl) == 0:
                        new_lines[li] = ''
                    return new_lines, f'redundant goto {lbl} at line {i+1}'

    # Pattern 2: OR-chain merging
    # if(A) goto L; if(B) goto L; -> if(A || B) goto L;
    for i in range(len(lines)):
        g = extract_goto(lines[i])
        if g and g[0] is not None:
            cond1, lbl = g
            j = i + 1
            # skip blank lines
            while j < len(lines) and not lines[j].strip():
                j += 1
            if j < len(lines):
                g2 = extract_goto(lines[j])
                if g2 and g2[0] is not None and g2[1] == lbl:
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
                    merged = ' || '.join(f'({c})' for c in conds)
                    ind = get_indent(lines[i])
                    new_lines = list(lines)
                    new_lines[i] = f'{ind}if ({merged}) goto {lbl};\n'
                    # Remove the merged lines
                    for idx in range(i + 1, k):
                        gx = extract_goto(lines[idx])
                        if gx and gx[1] == lbl:
                            new_lines[idx] = ''
                    return new_lines, f'OR-chain {len(conds)} conditions -> goto {lbl} at line {i+1}'

    # Pattern 3: Forward conditional skip with single-use label
    # if(cond) goto L; <code> L:  where L is single-use and code has no external targets
    # -> if(!cond) { <code> }
    for i in range(len(lines)):
        g = extract_goto(lines[i])
        if g and g[0] is not None:
            cond, lbl = g
            li = find_label(lines, lbl, i + 1)
            if li > i and label_use_count(lines, lbl) == 1:
                code = lines[i + 1:li]
                if not has_labels_targeted_from_outside(lines, i + 1, li):
                    # Check brace depth: code must be at same nesting level
                    if brace_depth_change(code) == 0:
                        ind = get_indent(lines[i])
                        new_lines = list(lines)
                        replacement = [f'{ind}if ({negate(cond)}) {{\n']
                        for cl in code:
                            if cl.strip():
                                replacement.append(f'    {cl}')
                            else:
                                replacement.append(cl)
                        replacement.append(f'{ind}}}\n')
                        new_lines[i:li + 1] = replacement
                        return new_lines, f'if-skip single-use {lbl} at line {i+1}'

    # Pattern 4: Forward conditional skip where block terminates (multi-use label OK)
    # if(cond) goto L; <code ending with return/break> L:
    # -> if(!cond) { <code> }
    for i in range(len(lines)):
        g = extract_goto(lines[i])
        if g and g[0] is not None:
            cond, lbl = g
            li = find_label(lines, lbl, i + 1)
            if li > i:
                code = lines[i + 1:li]
                if (block_terminates(code)
                    and not has_labels_targeted_from_outside(lines, i + 1, li)
                    and brace_depth_change(code) == 0):
                    ind = get_indent(lines[i])
                    new_lines = list(lines)
                    replacement = [f'{ind}if ({negate(cond)}) {{\n']
                    for cl in code:
                        if cl.strip():
                            replacement.append(f'    {cl}')
                        else:
                            replacement.append(cl)
                    replacement.append(f'{ind}}}\n')
                    uses = label_use_count(lines, lbl)
                    if uses <= 1:
                        new_lines[i:li + 1] = replacement
                    else:
                        new_lines[i:li] = replacement
                    return new_lines, f'if-skip-term {lbl} at line {i+1}'

    # Pattern 5: Backward goto (single-use) -> do-while loop
    for i in range(len(lines)):
        g = extract_goto(lines[i])
        if g:
            cond, lbl = g
            li = find_label(lines, lbl, 0, i)
            if li >= 0 and label_use_count(lines, lbl) == 1:
                body = lines[li + 1:i]
                if brace_depth_change(body) == 0:
                    ind = get_indent(lines[li])
                    new_lines = list(lines)
                    if cond:
                        replacement = [f'{ind}do {{\n']
                        for bl in body:
                            if bl.strip():
                                replacement.append(f'    {bl}')
                            else:
                                replacement.append(bl)
                        replacement.append(f'{ind}}} while ({cond});\n')
                    else:
                        replacement = [f'{ind}while (1) {{\n']
                        for bl in body:
                            if bl.strip():
                                replacement.append(f'    {bl}')
                            else:
                                replacement.append(bl)
                        replacement.append(f'{ind}}}\n')
                    new_lines[li:i + 1] = replacement
                    return new_lines, f'backward loop {lbl} at line {i+1}'

    # Pattern 6: goto before closing brace to label after else block
    # ... goto L; } ... L: -> remove goto if it's fallthrough
    for i in range(len(lines)):
        g = extract_goto(lines[i])
        if g and g[0] is None:
            lbl = g[1]
            # Find next non-empty line
            j = i + 1
            while j < len(lines) and not lines[j].strip():
                j += 1
            if j < len(lines) and lines[j].strip().startswith('}'):
                li = find_label(lines, lbl, j)
                if li >= 0:
                    between = lines[j + 1:li]
                    all_structural = True
                    for b in between:
                        bs = b.strip()
                        if bs and bs != '}' and bs != '} else {' and not is_label_line(b):
                            all_structural = False
                            break
                    if all_structural and brace_depth_change(lines[i+1:li]) == 0:
                        new_lines = list(lines)
                        new_lines[i] = ''
                        if label_use_count(new_lines, lbl) == 0:
                            new_lines[li] = ''
                        return new_lines, f'goto-before-brace {lbl} at line {i+1}'

    return None


def process_file(filepath, ver):
    with open(filepath, 'r') as f:
        original = f.read()

    lines = original.split('\n')
    total_removed = 0
    orig_count = count_gotos_text(original)

    if orig_count == 0:
        return 0

    max_attempts = orig_count * 3  # Safety limit
    attempts = 0

    while attempts < max_attempts:
        attempts += 1
        result = try_one_transformation(lines)
        if result is None:
            break

        new_lines, desc = result
        # Write and test compilation
        new_text = '\n'.join(new_lines)

        # Quick sanity: did we actually reduce gotos?
        new_count = count_gotos_text(new_text)
        old_count = count_gotos_text('\n'.join(lines))
        if new_count >= old_count:
            # This transformation didn't help, skip it by marking the goto
            # Actually just break -- the pattern will keep matching
            break

        with open(filepath, 'w') as f:
            f.write(new_text)

        if test_compile(filepath, ver):
            removed = old_count - new_count
            total_removed += removed
            lines = new_text.split('\n')
        else:
            # Revert this transformation
            with open(filepath, 'w') as f:
                f.write('\n'.join(lines))
            # This particular transformation failed, but others might work
            # We need to skip this one. Mark it somehow.
            # For simplicity, break on first failure per pattern
            break

    # Final state
    final_text = '\n'.join(lines)
    with open(filepath, 'w') as f:
        f.write(final_text)

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

    # Get target files (optionally from command line)
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
                    gotos = count_gotos_text(open(fp, 'r', errors='ignore').read())
                    if gotos > 0 and fp in cmap:
                        targets.append(fp)
        targets.sort(key=lambda fp: -count_gotos_text(open(fp, 'r', errors='ignore').read()))

    grand_total = 0
    for fp in targets:
        ver = cmap.get(fp)
        if not ver:
            continue
        if not test_compile(fp, ver):
            print(f"SKIP (broken): {fp}", flush=True)
            continue

        before = count_gotos_text(open(fp, 'r', errors='ignore').read())
        removed = process_file(fp, ver)
        after = count_gotos_text(open(fp, 'r', errors='ignore').read())
        if removed > 0:
            grand_total += removed
            print(f"OK  {fp}: {before} -> {after} (-{removed})", flush=True)
        else:
            print(f"---  {fp}: {before} (no change)", flush=True)

    print(f"\nTotal removed: {grand_total}", flush=True)


if __name__ == '__main__':
    main()
