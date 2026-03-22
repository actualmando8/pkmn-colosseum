#!/usr/bin/env python3
"""Inject new goto conversion functions into convert_gotos.py."""
import re

NEW_FUNCS = r'''

def convert_multi_condition_or(lines, label_refcount, label_pos):
    changes = 0
    i = 0
    while i < len(lines):
        m = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m:
            i += 1
            continue
        indent, first_cond, label = m.group(1), m.group(2), m.group(3)
        conditions = [first_cond]
        j = i + 1
        while j < len(lines):
            mj = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', lines[j])
            if mj and mj.group(3) == label:
                conditions.append(mj.group(2))
                j += 1
            else:
                break
        if len(conditions) > 1:
            combined = ' || '.join(conditions)
            lines[i] = indent + 'if (' + combined + ') goto ' + label + ';'
            for k in range(i + 1, i + len(conditions)):
                lines[k] = ''
                label_refcount[label] = label_refcount.get(label, 0) - 1
                changes += 1
            i = i + len(conditions)
        else:
            i += 1
    return changes


def convert_goto_to_return(lines, label_refcount, label_pos):
    changes = 0
    return_labels = set()
    for lbl, pos in label_pos.items():
        for k in range(pos + 1, min(pos + 3, len(lines))):
            stripped = lines[k].strip()
            if stripped:
                if stripped == 'return;':
                    return_labels.add(lbl)
                break
    if not return_labels:
        return 0
    for i in range(len(lines)):
        line = lines[i]
        m = re.match(r'^(\s+)goto (L_[0-9A-Fa-f]+);$', line)
        if m and m.group(2) in return_labels:
            lines[i] = m.group(1) + 'return;'
            label_refcount[m.group(2)] = label_refcount.get(m.group(2), 0) - 1
            changes += 1
            continue
        m = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', line)
        if m and m.group(3) in return_labels:
            lines[i] = m.group(1) + 'if (' + m.group(2) + ') return;'
            label_refcount[m.group(3)] = label_refcount.get(m.group(3), 0) - 1
            changes += 1
    return changes


def convert_return_value(lines, label_refcount, label_pos):
    changes = 0
    return_var_labels = {}
    for lbl, pos in label_pos.items():
        for k in range(pos + 1, min(pos + 3, len(lines))):
            stripped = lines[k].strip()
            if stripped:
                m = re.match(r'^return\s+(\w+)\s*;$', stripped)
                if m:
                    return_var_labels[lbl] = m.group(1)
                break
    if not return_var_labels:
        return 0
    for i in range(len(lines) - 1, -1, -1):
        m_assign = re.match(r'^(\s+)(\w+)\s*=\s*(.+);\s*$', lines[i])
        if not m_assign:
            continue
        indent, var, value = m_assign.group(1), m_assign.group(2), m_assign.group(3)
        for j in range(i + 1, min(i + 3, len(lines))):
            stripped = lines[j].strip()
            if stripped:
                m_goto = re.match(r'^goto (L_[0-9A-Fa-f]+);$', stripped)
                if m_goto:
                    lbl = m_goto.group(1)
                    if lbl in return_var_labels and return_var_labels[lbl] == var:
                        lines[i] = indent + 'return ' + value + ';'
                        lines[j] = ''
                        label_refcount[lbl] = label_refcount.get(lbl, 0) - 1
                        changes += 1
                break
    return changes


def convert_forward_singles_nested(lines, label_refcount, label_pos, label_sources):
    changes = 0
    for i in range(len(lines) - 1, -1, -1):
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
        body_labels = []
        for k in range(i + 1, label_idx):
            lm = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
            if lm:
                body_labels.append(lm.group(1))
        if not body_labels:
            continue
        all_internal = True
        for bl in body_labels:
            for src in label_sources.get(bl, []):
                if src < i or src > label_idx:
                    all_internal = False
                    break
            if not all_internal:
                break
        if not all_internal:
            continue
        has_bad = False
        for k in range(i + 1, label_idx):
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
        for k in range(i + 1, label_idx):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]
        lines[label_idx] = indent + '}'
        changes += 1
        label_refcount[label] = 0
    return changes


def convert_cascaded_if_else(lines, label_refcount, label_pos):
    changes = 0
    processed = set()
    for i in range(len(lines)):
        if i in processed:
            continue
        m1 = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m1:
            continue
        indent, first_cond, first_label = m1.group(1), m1.group(2), m1.group(3)
        if first_label not in label_pos or label_refcount.get(first_label, 0) != 1:
            continue
        first_label_idx = label_pos[first_label]
        if first_label_idx <= i:
            continue
        end_goto_idx = end_label = None
        for k in range(first_label_idx - 1, i, -1):
            s = lines[k].strip()
            if not s or s.startswith('//'):
                continue
            mg = re.match(r'^goto (L_[0-9A-Fa-f]+);$', s)
            if mg:
                end_goto_idx = k
                end_label = mg.group(1)
            break
        if end_goto_idx is None or end_label is None or end_label not in label_pos:
            continue
        end_label_idx = label_pos[end_label]
        if end_label_idx <= first_label_idx:
            continue
        if any(re.match(r'^\s*L_[0-9A-Fa-f]+\s*:\s*;?\s*$', lines[k]) for k in range(i + 1, end_goto_idx)):
            continue
        chain = [{'cond': first_cond, 'body_start': i + 1, 'body_end': end_goto_idx, 'skip_goto_idx': end_goto_idx, 'label': first_label, 'label_idx': first_label_idx}]
        cur_label_idx = first_label_idx
        chain_end_label = end_label
        while True:
            next_if_idx = next_cond_val = next_label_val = None
            for k in range(cur_label_idx + 1, min(cur_label_idx + 3, len(lines))):
                s = lines[k].strip()
                if not s:
                    continue
                mn = re.match(r'^if \((.+)\) goto (L_[0-9A-Fa-f]+);$', s)
                if mn:
                    next_if_idx, next_cond_val, next_label_val = k, mn.group(1), mn.group(2)
                break
            if next_if_idx is None:
                break
            if next_label_val not in label_pos or label_refcount.get(next_label_val, 0) != 1:
                break
            next_label_idx = label_pos[next_label_val]
            if next_label_idx <= next_if_idx:
                break
            next_end_goto_idx = None
            for k in range(next_label_idx - 1, next_if_idx, -1):
                s = lines[k].strip()
                if not s or s.startswith('//'):
                    continue
                mg = re.match(r'^goto (L_[0-9A-Fa-f]+);$', s)
                if mg and mg.group(1) == chain_end_label:
                    next_end_goto_idx = k
                break
            if next_end_goto_idx is None:
                break
            if any(re.match(r'^\s*L_[0-9A-Fa-f]+\s*:\s*;?\s*$', lines[k]) for k in range(next_if_idx + 1, next_end_goto_idx)):
                break
            chain.append({'cond': next_cond_val, 'body_start': next_if_idx + 1, 'body_end': next_end_goto_idx, 'skip_goto_idx': next_end_goto_idx, 'label': next_label_val, 'label_idx': next_label_idx})
            cur_label_idx = next_label_idx
        if len(chain) < 2:
            continue
        if not all(invert_cond(c['cond']) is not None for c in chain):
            continue
        inv = invert_cond(chain[0]['cond'])
        lines[i] = indent + 'if (' + inv + ') {'
        for k in range(chain[0]['body_start'], chain[0]['body_end']):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]
        lines[chain[0]['skip_goto_idx']] = ''
        label_refcount[chain[0]['label']] = 0
        label_refcount[chain_end_label] = label_refcount.get(chain_end_label, 0) - 1
        processed.add(i)
        for ci in range(1, len(chain)):
            c = chain[ci]
            inv = invert_cond(c['cond'])
            lines[c['label_idx']] = ''
            lines[chain[ci - 1]['label_idx']] = indent + '} else if (' + inv + ') {'
            for k in range(c['body_start'], c['body_end']):
                if lines[k].strip():
                    lines[k] = '    ' + lines[k]
            lines[c['skip_goto_idx']] = ''
            label_refcount[c['label']] = 0
            label_refcount[chain_end_label] = label_refcount.get(chain_end_label, 0) - 1
        last_label_idx = chain[-1]['label_idx']
        else_start, else_end = last_label_idx + 1, end_label_idx
        has_else_body = any(lines[k].strip() for k in range(else_start, else_end))
        has_else_label = any(re.match(r'^\s*L_[0-9A-Fa-f]+\s*:\s*;?\s*$', lines[k]) for k in range(else_start, else_end))
        if has_else_body and not has_else_label:
            lines[last_label_idx] = indent + '} else {'
            for k in range(else_start, else_end):
                if lines[k].strip():
                    lines[k] = '    ' + lines[k]
            lines[else_end] = indent + '}'
            label_refcount[chain_end_label] = max(label_refcount.get(chain_end_label, 0) - 1, 0)
        else:
            lines[last_label_idx] = indent + '}'
        changes += len(chain) * 2
    return changes


def convert_break_from_loop(lines, label_refcount, label_pos):
    changes = 0
    loop_ranges = []
    for i, line in enumerate(lines):
        if re.match(r'^\s*do\s*\{', line):
            depth = 1
            for j in range(i + 1, len(lines)):
                depth += lines[j].count('{') - lines[j].count('}')
                if depth <= 0:
                    if re.match(r'^\s*\}\s*while\s*\(', lines[j]):
                        loop_ranges.append((i, j))
                    break
    for i, line in enumerate(lines):
        if re.match(r'^\s*while\s*\(.+\)\s*\{', line):
            depth = 1
            for j in range(i + 1, len(lines)):
                depth += lines[j].count('{') - lines[j].count('}')
                if depth <= 0:
                    loop_ranges.append((i, j))
                    break
    if not loop_ranges:
        return 0
    for i in range(len(lines)):
        line = lines[i]
        m = re.match(r'^(\s+)goto (L_[0-9A-Fa-f]+);$', line)
        is_conditional = False
        cond = None
        if m:
            indent, lbl = m.group(1), m.group(2)
        else:
            m = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', line)
            if m:
                indent, cond, lbl = m.group(1), m.group(2), m.group(3)
                is_conditional = True
            else:
                continue
        if lbl not in label_pos:
            continue
        target = label_pos[lbl]
        best_loop = None
        for (start, end) in loop_ranges:
            if start < i < end:
                if best_loop is None or start > best_loop[0]:
                    best_loop = (start, end)
        if best_loop is None:
            continue
        loop_start, loop_end = best_loop
        valid = (target == loop_end + 1) or (target == loop_end + 2 and not lines[loop_end + 1].strip())
        if valid:
            if is_conditional:
                lines[i] = indent + 'if (' + cond + ') break;'
            else:
                lines[i] = indent + 'break;'
            label_refcount[lbl] = label_refcount.get(lbl, 0) - 1
            changes += 1
    return changes


def convert_continue_in_loop(lines, label_refcount, label_pos):
    changes = 0
    loop_ranges = []
    for i, line in enumerate(lines):
        if re.match(r'^\s*do\s*\{', line):
            depth = 1
            for j in range(i + 1, len(lines)):
                depth += lines[j].count('{') - lines[j].count('}')
                if depth <= 0:
                    if re.match(r'^\s*\}\s*while\s*\(', lines[j]):
                        loop_ranges.append((i, j, 'do-while'))
                    break
    for i, line in enumerate(lines):
        if re.match(r'^\s*while\s*\(.+\)\s*\{', line):
            depth = 1
            for j in range(i + 1, len(lines)):
                depth += lines[j].count('{') - lines[j].count('}')
                if depth <= 0:
                    loop_ranges.append((i, j, 'while'))
                    break
    if not loop_ranges:
        return 0
    for i in range(len(lines)):
        line = lines[i]
        m = re.match(r'^(\s+)goto (L_[0-9A-Fa-f]+);$', line)
        is_conditional = False
        cond = None
        if m:
            indent, lbl = m.group(1), m.group(2)
        else:
            m = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', line)
            if m:
                indent, cond, lbl = m.group(1), m.group(2), m.group(3)
                is_conditional = True
            else:
                continue
        if lbl not in label_pos:
            continue
        target = label_pos[lbl]
        best_loop = None
        for (start, end, ltype) in loop_ranges:
            if start < i < end:
                if best_loop is None or start > best_loop[0]:
                    best_loop = (start, end, ltype)
        if best_loop is None:
            continue
        loop_start, loop_end, loop_type = best_loop
        is_continue = (target == loop_start) or (loop_type == 'do-while' and target == loop_end)
        if is_continue:
            if is_conditional:
                lines[i] = indent + 'if (' + cond + ') continue;'
            else:
                lines[i] = indent + 'continue;'
            label_refcount[lbl] = label_refcount.get(lbl, 0) - 1
            changes += 1
    return changes


def convert_switch(lines, label_refcount, label_pos):
    changes = 0
    i = 0
    while i < len(lines):
        m = re.match(r'^(\s+)if \((.+?) == (.+?)\) goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m:
            i += 1
            continue
        indent, switch_var = m.group(1), m.group(2)
        first_val, first_label = m.group(3), m.group(4)
        cases = [(first_val, first_label)]
        j = i + 1
        while j < len(lines):
            mj = re.match(r'^\s+if \(' + re.escape(switch_var) + r' == (.+?)\) goto (L_[0-9A-Fa-f]+);$', lines[j])
            if mj:
                cases.append((mj.group(1), mj.group(2)))
                j += 1
            else:
                break
        if len(cases) < 3:
            i += 1
            continue
        default_label = default_goto_idx = None
        if j < len(lines):
            md = re.match(r'^\s+goto (L_[0-9A-Fa-f]+);$', lines[j])
            if md:
                default_label = md.group(1)
                default_goto_idx = j
        all_valid = all(lbl in label_pos and label_pos[lbl] > i and label_refcount.get(lbl, 0) == 1 for _, lbl in cases)
        if not all_valid:
            i += 1
            continue
        case_positions = sorted([(val, lbl, label_pos[lbl]) for val, lbl in cases], key=lambda x: x[2])
        end_boundary = label_pos.get(default_label, case_positions[-1][2]) if default_label else case_positions[-1][2]
        end_label = end_label_idx = None
        search_start = end_boundary + 1
        for k in range(search_start, min(search_start + 100, len(lines))):
            ml = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
            if ml:
                end_label, end_label_idx = ml.group(1), k
                break
        valid = True
        for ci in range(len(case_positions)):
            _, _, cpos = case_positions[ci]
            nxt = case_positions[ci+1][2] if ci+1 < len(case_positions) else (label_pos.get(default_label, end_label_idx or cpos+20) if default_label else (end_label_idx or cpos+20))
            for k in range(cpos + 1, min(nxt, len(lines))):
                gm = re.search(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[k])
                if gm and gm.group(1) in label_pos and label_pos[gm.group(1)] < i:
                    valid = False
                    break
            if not valid:
                break
        if not valid:
            i += 1
            continue
        lines[i] = indent + 'switch (' + switch_var + ') {'
        for k in range(i + 1, i + len(cases)):
            lines[k] = ''
        if default_goto_idx is not None:
            lines[default_goto_idx] = ''
        for ci in range(len(case_positions)):
            val, lbl, cpos = case_positions[ci]
            nxt = case_positions[ci+1][2] if ci+1 < len(case_positions) else (label_pos.get(default_label, end_label_idx or cpos+1) if default_label else (end_label_idx or cpos+1))
            lines[cpos] = indent + 'case ' + val + ':'
            for k in range(cpos + 1, nxt):
                if lines[k].strip():
                    lines[k] = '    ' + lines[k]
            last_nonempty = None
            for k in range(nxt - 1, cpos, -1):
                if lines[k].strip():
                    last_nonempty = k
                    break
            if last_nonempty is not None:
                s = lines[last_nonempty].strip()
                if not (s.startswith('goto ') or s.startswith('return') or s.startswith('break') or s.startswith('continue')):
                    lines[last_nonempty] += '\n' + indent + '    break;'
            label_refcount[lbl] = 0
        if default_label and default_label in label_pos:
            dpos = label_pos[default_label]
            lines[dpos] = indent + 'default:'
            if end_label_idx and end_label_idx > dpos:
                for k in range(dpos + 1, end_label_idx):
                    if lines[k].strip():
                        lines[k] = '    ' + lines[k]
                lines[end_label_idx] = indent + '}'
            else:
                lines[dpos] += '\n' + indent + '}'
            label_refcount[default_label] = label_refcount.get(default_label, 0) - 1
        elif end_label_idx:
            lines[end_label_idx] = indent + '}'
        else:
            lines[case_positions[-1][2]] += '\n' + indent + '}'
        changes += len(cases) + (1 if default_label else 0)
        i = j + 1 if default_goto_idx is not None else j
    return changes

'''

with open('tools/convert_gotos.py', 'r') as f:
    content = f.read()

idx = content.find('\ndef main():')
if idx == -1:
    print('ERROR: could not find main()')
    exit(1)

content = content[:idx] + NEW_FUNCS + content[idx:]

with open('tools/convert_gotos.py', 'w') as f:
    f.write(content)
print('Functions injected successfully')
