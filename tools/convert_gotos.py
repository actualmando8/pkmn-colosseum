#!/usr/bin/env python3
"""Convert goto-based patterns to structured control flow."""
import re
import glob

def invert_cond(cond):
    if ' == ' in cond and cond.count('==') == 1:
        return cond.replace(' == ', ' != ')
    elif ' != ' in cond and cond.count('!=') == 1:
        return cond.replace(' != ', ' == ')
    elif ' < ' in cond and cond.count('<') == 1 and '<=' not in cond:
        return cond.replace(' < ', ' >= ')
    elif ' > ' in cond and cond.count('>') == 1 and '>=' not in cond:
        return cond.replace(' > ', ' <= ')
    elif ' <= ' in cond and cond.count('<=') == 1:
        return cond.replace(' <= ', ' > ')
    elif ' >= ' in cond and cond.count('>=') == 1:
        return cond.replace(' >= ', ' < ')
    return None

def build_indices(lines):
    label_refcount = {}
    for i, line in enumerate(lines):
        for m in re.finditer(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', line):
            lbl = m.group(1)
            label_refcount[lbl] = label_refcount.get(lbl, 0) + 1
    label_pos = {}
    for i, line in enumerate(lines):
        m = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', line)
        if m:
            label_pos[m.group(1)] = i
    return label_refcount, label_pos

def convert_forward_singles(lines, label_refcount, label_pos):
    changes = 0
    for i in range(len(lines)-1, -1, -1):
        line = lines[i]
        m = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', line)
        if not m:
            continue
        indent, cond, label = m.group(1), m.group(2), m.group(3)
        if label_refcount.get(label, 0) != 1:
            continue
        if label not in label_pos:
            continue
        label_idx = label_pos[label]
        if label_idx <= i:
            continue
        has_other = any(re.match(r'^\s*L_[0-9A-Fa-f]+\s*:\s*;?\s*$', lines[k]) for k in range(i+1, label_idx))
        if has_other:
            continue
        has_bad = False
        for k in range(i+1, label_idx):
            gm = re.search(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[k])
            if gm and gm.group(1) in label_pos:
                t = label_pos[gm.group(1)]
                if t < i or t > label_idx:
                    has_bad = True
                    break
        if has_bad:
            continue
        inv = invert_cond(cond)
        if inv is None:
            continue
        lines[i] = indent + 'if (' + inv + ') {'
        for k in range(i+1, label_idx):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]
        lines[label_idx] = indent + '}'
        changes += 1
        label_refcount[label] = 0
    return changes

def convert_if_else(lines, label_refcount, label_pos):
    changes = 0
    for i in range(len(lines)-1, -1, -1):
        line = lines[i]
        m_cond = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', line)
        if not m_cond:
            continue
        indent, cond, label1 = m_cond.group(1), m_cond.group(2), m_cond.group(3)
        if label1 not in label_pos or label_refcount.get(label1, 0) != 1:
            continue
        label1_idx = label_pos[label1]
        if label1_idx <= i:
            continue
        uncond_idx = uncond_label = None
        for k in range(i+1, label1_idx):
            m_u = re.match(r'^\s+goto (L_[0-9A-Fa-f]+);$', lines[k])
            if m_u:
                uncond_idx = k
                uncond_label = m_u.group(1)
                break
        if not uncond_idx or not uncond_label:
            continue
        if uncond_label not in label_pos or label_refcount.get(uncond_label, 0) != 1:
            continue
        label2_idx = label_pos[uncond_label]
        if label2_idx <= label1_idx:
            continue
        if any(lines[k].strip() and not lines[k].strip().startswith('//') for k in range(uncond_idx+1, label1_idx)):
            continue
        if any(re.match(r'^\s*L_[0-9A-Fa-f]+\s*:\s*;?\s*$', lines[k]) for k in range(i+1, uncond_idx)):
            continue
        if any(re.match(r'^\s*L_[0-9A-Fa-f]+\s*:\s*;?\s*$', lines[k]) for k in range(label1_idx+1, label2_idx)):
            continue
        has_bad = False
        for rng in [(i+1, uncond_idx), (label1_idx+1, label2_idx)]:
            for k in range(*rng):
                gm = re.search(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[k])
                if gm and gm.group(1) in label_pos:
                    t = label_pos[gm.group(1)]
                    if t < i or t > label2_idx:
                        has_bad = True
                        break
            if has_bad:
                break
        if has_bad:
            continue
        inv = invert_cond(cond)
        if inv is None:
            continue
        lines[i] = indent + 'if (' + inv + ') {'
        for k in range(i+1, uncond_idx):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]
        lines[uncond_idx] = indent + '} else {'
        lines[label1_idx] = ''
        for k in range(label1_idx+1, label2_idx):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]
        lines[label2_idx] = indent + '}'
        label_refcount[label1] = 0
        label_refcount[uncond_label] = 0
        changes += 2
    return changes

def convert_do_while(lines, label_refcount, label_pos):
    changes = 0
    for i in range(len(lines)-1, -1, -1):
        line = lines[i]
        m = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', line)
        if not m:
            continue
        indent, cond, label = m.group(1), m.group(2), m.group(3)
        if label not in label_pos:
            continue
        label_idx = label_pos[label]
        if label_idx >= i:
            continue
        if label_refcount.get(label, 0) != 1:
            continue
        has_other = any(re.match(r'^\s*L_[0-9A-Fa-f]+\s*:\s*;?\s*$', lines[k]) for k in range(label_idx+1, i))
        if has_other:
            continue
        has_bad = False
        for k in range(label_idx+1, i):
            gm = re.search(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[k])
            if gm and gm.group(1) in label_pos:
                t = label_pos[gm.group(1)]
                if t < label_idx or t > i:
                    has_bad = True
                    break
        if has_bad:
            continue
        lines[label_idx] = indent + 'do {'
        for k in range(label_idx+1, i):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]
        lines[i] = indent + '} while (' + cond + ');'
        changes += 1
        label_refcount[label] = 0
    return changes

def main():
    c_files = glob.glob('src/**/*.c', recursive=True)
    total_changes = 0

    for iteration in range(10):
        iter_changes = 0
        for filepath in c_files:
            try:
                with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
                    content = f.read()
            except:
                continue
            if 'goto' not in content:
                continue
            lines = content.split('\n')
            changes = 0

            label_refcount, label_pos = build_indices(lines)
            changes += convert_forward_singles(lines, label_refcount, label_pos)

            label_refcount, label_pos = build_indices(lines)
            changes += convert_if_else(lines, label_refcount, label_pos)

            label_refcount, label_pos = build_indices(lines)
            changes += convert_do_while(lines, label_refcount, label_pos)

            if changes > 0:
                with open(filepath, 'w', encoding='utf-8', errors='replace') as f:
                    f.write('\n'.join(lines))
                iter_changes += changes
                total_changes += changes

        if iter_changes == 0:
            break
        print(f'Iteration {iteration+1}: {iter_changes} gotos converted')

    print(f'Total: {total_changes} gotos converted')

if __name__ == '__main__':
    main()
