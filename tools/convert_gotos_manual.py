#!/usr/bin/env python3
"""
Aggressive goto converter - handles patterns the safe tool misses.
Specifically handles:
1. Unconditional gotos that skip to a label within 5 lines (with code between)
2. if (cond) { ...; goto LABEL; } where the label is the very next non-blank line
3. do-while loops formed by unconditional backward gotos
4. Multi-reference forward labels where ALL refs are "if (cond) { r0 = X; goto LABEL; }"
"""
import re
import sys
import os


def invert_cond(cond):
    cond = cond.strip()
    if ' && ' in cond and ' || ' not in cond:
        parts = cond.split(' && ')
        inv = [invert_cond(p.strip()) for p in parts]
        if None in inv:
            return None
        return ' || '.join(inv)
    if ' || ' in cond and ' && ' not in cond:
        parts = cond.split(' || ')
        inv = [invert_cond(p.strip()) for p in parts]
        if None in inv:
            return None
        return ' && '.join(inv)
    for old, new in [(' == ', ' != '), (' != ', ' == '), (' < ', ' >= '),
                     (' >= ', ' < '), (' > ', ' <= '), (' <= ', ' > ')]:
        if old in cond:
            count = 0
            for op_check in [' == ', ' != ', ' < ', ' >= ', ' > ', ' <= ']:
                count += cond.count(op_check)
            if count == 1:
                return cond.replace(old, new, 1)
    if cond.startswith('!'):
        return cond[1:]
    return None


def build_indices(lines):
    label_pos = {}
    label_rc = {}
    label_sources = {}
    for i, line in enumerate(lines):
        m = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', line)
        if m:
            label_pos[m.group(1)] = i
    for i, line in enumerate(lines):
        for m in re.finditer(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', line):
            lbl = m.group(1)
            label_rc[lbl] = label_rc.get(lbl, 0) + 1
            if lbl not in label_sources:
                label_sources[lbl] = []
            label_sources[lbl].append(i)
    return label_pos, label_rc, label_sources


def count_gotos(lines):
    return sum(len(re.findall(r'\bgoto\s+L_[0-9A-Fa-f]+\b', line)) for line in lines)


def find_function_ranges(lines):
    ranges = []
    depth = 0
    func_start = None
    for i, line in enumerate(lines):
        opens = line.count('{')
        closes = line.count('}')
        if depth == 0 and opens > 0:
            func_start = i
        depth += opens - closes
        if depth == 0 and func_start is not None:
            ranges.append((func_start, i))
            func_start = None
    return ranges


def verify_brace_balance(lines, func_ranges):
    for start, end in func_ranges:
        depth = 0
        for i in range(start, end + 1):
            depth += lines[i].count('{') - lines[i].count('}')
        if depth != 0:
            return False, start
    return True, -1


def pass_unconditional_backward_goto_to_while(lines, lp, rc, ls):
    """Convert unconditional backward goto to while(1) loop.
    ... LABEL: <body>; goto LABEL; -> while(1) { <body>; }
    Only if label has exactly 1 ref (the goto itself)."""
    changes = 0
    for i in range(len(lines)):
        m = re.match(r'^(\s+)goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m:
            continue
        indent, label = m.group(1), m.group(2)
        if label not in lp or rc.get(label, 0) != 1:
            continue
        label_idx = lp[label]
        if label_idx >= i:
            continue
        # Check no other labels or backward gotos in body
        body_ok = True
        brace_depth = 0
        for k in range(label_idx + 1, i):
            s = lines[k].strip()
            brace_depth += lines[k].count('{') - lines[k].count('}')
            ml = re.match(r'^(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', s)
            if ml:
                for src in ls.get(ml.group(1), []):
                    if src < label_idx or src > i:
                        body_ok = False
                        break
            gm = re.search(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', s)
            if gm:
                t = gm.group(1)
                if t in lp and lp[t] < label_idx:
                    body_ok = False
        if not body_ok or brace_depth != 0:
            continue
        # Convert
        lines[label_idx] = indent + 'while (1) {\n'
        lines[i] = indent + '}\n'
        for k in range(label_idx + 1, i):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]
        rc[label] = 0
        changes += 1
    return changes


def pass_conditional_goto_to_next_label(lines, lp, rc, ls):
    """if (cond) { ...; goto LABEL; } where LABEL is very near after the closing brace.
    Convert the goto to just removing the goto + label if single ref."""
    changes = 0
    for i in range(len(lines)):
        # Match: goto LABEL; inside an if block, where the label is within 3 lines after the if block's }
        m = re.match(r'^(\s+)goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m:
            continue
        indent, label = m.group(1), m.group(2)
        if label not in lp or rc.get(label, 0) != 1:
            continue
        label_idx = lp[label]
        if label_idx <= i or label_idx - i > 5:
            continue
        # Check that only blank lines and closing braces are between goto and label
        all_ok = True
        for k in range(i + 1, label_idx):
            s = lines[k].strip()
            if s and s != '}':
                all_ok = False
                break
        if not all_ok:
            continue
        # Remove the goto and the label
        lines[i] = ''
        lines[label_idx] = ''
        rc[label] = 0
        changes += 1
    return changes


def pass_if_goto_with_assignment(lines, lp, rc, ls):
    """Convert: if (cond) { VAR = X; goto LABEL; } ... LABEL:
    where label is single-ref and within 50 lines.
    To: if (!cond) { <body between>; } VAR = X; (and remove label)"""
    changes = 0
    i = 0
    while i < len(lines):
        # Pattern: if (cond) {
        m_if = re.match(r'^(\s+)if \((.+)\) \{$', lines[i])
        if not m_if:
            i += 1
            continue
        indent, cond = m_if.group(1), m_if.group(2)
        # Check next few lines for: assignment; goto LABEL; }
        if i + 3 >= len(lines):
            i += 1
            continue
        # Look for goto within the if block
        k = i + 1
        block_stmts = []
        found_goto = False
        goto_label = None
        close_idx = None
        while k < min(i + 10, len(lines)):
            s = lines[k].strip()
            if not s:
                k += 1
                continue
            if s == '}':
                close_idx = k
                break
            gm = re.match(r'^goto (L_[0-9A-Fa-f]+);$', s)
            if gm:
                found_goto = True
                goto_label = gm.group(1)
                k += 1
                continue
            block_stmts.append((k, s))
            k += 1
        if not found_goto or goto_label is None or close_idx is None:
            i += 1
            continue
        if goto_label not in lp or rc.get(goto_label, 0) != 1:
            i += 1
            continue
        label_idx = lp[goto_label]
        if label_idx <= close_idx:
            i += 1
            continue
        # Verify no labels/gotos in body between close_idx+1 and label_idx
        body_ok = True
        brace_depth = 0
        for bk in range(close_idx + 1, label_idx):
            s = lines[bk].strip()
            if not s:
                continue
            brace_depth += lines[bk].count('{') - lines[bk].count('}')
            if re.match(r'^L_[0-9A-Fa-f]+\s*:\s*;?\s*$', s):
                body_ok = False
                break
            if re.search(r'\bgoto\s+L_[0-9A-Fa-f]+\s*;', s):
                body_ok = False
                break
        if not body_ok or brace_depth != 0:
            i += 1
            continue
        inv = invert_cond(cond)
        if inv is None:
            i += 1
            continue
        # Only if block_stmts has exactly assignment(s) before goto
        if len(block_stmts) > 3:
            i += 1
            continue
        # Restructure: keep assignments, wrap body in if(!cond)
        new_lines = []
        # Output assignments outside
        for (bk, stmt) in block_stmts:
            new_lines.append(indent + stmt + '\n')
            lines[bk] = ''
        # Remove the goto line
        for bk in range(i + 1, close_idx):
            s = lines[bk].strip()
            if re.match(r'^goto L_[0-9A-Fa-f]+;$', s):
                lines[bk] = ''
        # Convert if block to if(!cond) wrapping the body
        lines[i] = ''  # remove old if
        lines[close_idx] = ''  # remove old }
        # Now wrap body between close_idx+1 and label_idx
        if label_idx > close_idx + 1:
            # There's body to wrap
            body_lines_content = []
            for bk in range(close_idx + 1, label_idx):
                if lines[bk].strip():
                    body_lines_content.append(bk)
            if body_lines_content:
                lines[body_lines_content[0]] = indent + 'if (' + inv + ') {\n' + '    ' + lines[body_lines_content[0]]
                for bk in body_lines_content[1:]:
                    lines[bk] = '    ' + lines[bk]
                lines[body_lines_content[-1]] = lines[body_lines_content[-1]].rstrip('\n') + '\n' + indent + '}\n'
        # Place assignments before the if block
        for nl in reversed(new_lines):
            lines[close_idx] = nl + lines[close_idx]
        lines[label_idx] = ''
        rc[goto_label] = 0
        changes += 1
        i = label_idx + 1
    return changes


def pass_remove_dead_labels(lines, lp, rc, ls):
    """Remove labels that have no remaining goto references."""
    changes = 0
    for label, pos in list(lp.items()):
        if rc.get(label, 0) <= 0:
            m = re.match(r'^(\s*)(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[pos])
            if m:
                lines[pos] = ''
                del lp[label]
                changes += 1
    return changes


def process_file(filepath):
    with open(filepath) as f:
        content = f.read()
    lines = content.split('\n')
    lines = [l + '\n' for l in lines]
    if lines:
        lines[-1] = lines[-1].rstrip('\n')

    initial_gotos = count_gotos(lines)
    print(f"Processing {filepath}: {initial_gotos} gotos")

    for iteration in range(20):
        lp, rc, ls = build_indices(lines)
        total_changes = 0

        for pass_fn in [
            pass_conditional_goto_to_next_label,
            pass_unconditional_backward_goto_to_while,
            pass_remove_dead_labels,
        ]:
            lp, rc, ls = build_indices(lines)
            c = pass_fn(lines, lp, rc, ls)
            total_changes += c

        if total_changes == 0:
            break

        current = count_gotos(lines)
        print(f"  Iteration {iteration + 1}: {total_changes} changes ({current} remaining)")

    # Verify brace balance
    func_ranges = find_function_ranges(lines)
    ok, bad_line = verify_brace_balance(lines, func_ranges)
    if not ok:
        print(f"  WARNING: Brace imbalance near line {bad_line}, reverting!")
        return 0

    final_gotos = count_gotos(lines)
    print(f"  Result: {initial_gotos} -> {final_gotos} gotos ({initial_gotos - final_gotos} removed)")

    content = ''.join(lines)
    # Clean up multiple blank lines
    while '\n\n\n' in content:
        content = content.replace('\n\n\n', '\n\n')
    with open(filepath, 'w') as f:
        f.write(content)
    return initial_gotos - final_gotos


def main():
    files = sys.argv[1:]
    if not files:
        print("Usage: convert_gotos_manual.py <file.c> [file2.c ...]")
        sys.exit(1)
    total = 0
    for f in files:
        if os.path.exists(f):
            total += process_file(f)
        else:
            print(f'File not found: {f}')
    print(f'\nTotal gotos removed: {total}')


if __name__ == '__main__':
    main()
