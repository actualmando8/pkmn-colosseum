#!/usr/bin/env python3
"""
Goto eliminator for decompiled C code (PowerPC decomp).
Converts goto-based control flow to structured C.

Operates iteratively, applying safe transformations in passes until
no more gotos can be eliminated. Handles:
  - Forward conditional skip  (if-goto -> if-else)
  - Forward unconditional skip (goto past block)
  - If-goto-else patterns
  - Backward goto -> do-while / while loops
  - Multi-goto to same label (continue/break in loops)
  - goto into loop condition (-> while loop conversion)
  - Chain of if-goto to same label -> compound || condition
  - Early return guard patterns
  - Orphan label cleanup
"""

import re
import sys
import os

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def count_gotos(lines):
    return sum(len(re.findall(r'\bgoto\s+\w+\s*;', l)) for l in lines)

def find_label(lines, label, start=0, end=None):
    """Return index of label definition, or -1"""
    if end is None:
        end = len(lines)
    t1 = label + ':'
    for i in range(start, end):
        s = lines[i].strip()
        if s == t1 or s == t1 + ' ;' or s.startswith(t1 + ' '):
            return i
    return -1

def is_label_line(line):
    """Return label name if line is a bare label, else None"""
    s = line.strip()
    m = re.match(r'^(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', s)
    return m.group(1) if m else None

def label_use_count(lines, label):
    """How many goto statements reference this label?"""
    pat = f'goto {label}'
    return sum(1 for l in lines if pat in l)

def get_indent(line):
    return line[:len(line) - len(line.lstrip())] if line.strip() else ''

def indent_block(block, extra='    '):
    """Add indentation to a block of lines"""
    return [extra + l if l.strip() else l for l in block]

def extract_goto(line):
    """Return (condition_or_None, label) or None"""
    s = line.strip()
    m = re.match(r'if\s*\((.+?)\)\s+goto\s+(\w+)\s*;$', s)
    if m:
        return (m.group(1), m.group(2))
    m = re.match(r'goto\s+(\w+)\s*;$', s)
    if m:
        return (None, m.group(1))
    return None

def negate(cond):
    """Negate a C boolean condition string"""
    cond = cond.strip()
    if cond.startswith('!(') and cond.endswith(')'):
        return cond[2:-1]
    ops = {'!=':'==', '==':'!=', '>=':'<', '<=':'>', '>':'<=', '<':'>='}
    m = re.search(r'(.*?)\s*(!=|==|>=|<=|>(?!=)|<(?!=))\s*(.*)', cond)
    if m:
        l, op, r = m.groups()
        if op in ops:
            return f'{l} {ops[op]} {r}'
    return f'!({cond})'

def block_has_external_label_targets(lines, block_start, block_end, full_lines):
    """Check if any label inside block[block_start:block_end] is targeted by a goto
    outside the block."""
    for i in range(block_start, block_end):
        lbl = is_label_line(full_lines[i])
        if lbl:
            for j, l in enumerate(full_lines):
                if (j < block_start or j >= block_end) and f'goto {lbl}' in l:
                    return True
    return False

def block_gotos_stay_inside(lines, block_start, block_end):
    """Check that gotos inside block only target labels inside block or after block_end"""
    for i in range(block_start, block_end):
        g = extract_goto(lines[i])
        if g:
            _, lbl = g
            idx = find_label(lines, lbl)
            if idx >= 0 and idx < block_start:
                return False  # backward goto outside block
    return True

# ---------------------------------------------------------------------------
# Pass 1: Simple forward conditional skip
#   if (cond) goto L;  ...code...  L: ;
#   -> if (negcond) { ...code... }
# Only when L is targeted once.
# ---------------------------------------------------------------------------
def pass_forward_cond_skip(lines):
    changed = True
    while changed:
        changed = False
        i = 0
        while i < len(lines):
            g = extract_goto(lines[i])
            if g and g[0] is not None:
                cond, lbl = g
                li = find_label(lines, lbl, i+1)
                if li > i and label_use_count(lines, lbl) == 1:
                    block = lines[i+1:li]
                    if not block_has_external_label_targets(lines, i+1, li, lines):
                        ind = get_indent(lines[i])
                        new = [f'{ind}if ({negate(cond)}) {{']
                        new += indent_block(block)
                        new += [f'{ind}}}']
                        lines[i:li+1] = new
                        changed = True
                        continue
            i += 1
    return lines

# ---------------------------------------------------------------------------
# Pass 2: if-goto / else-block / goto-merge / label / then-block / merge-label
#   if (cond) goto A;  <else>  goto B;  A:  <then>  B:
#   -> if (cond) { then } else { else }
# ---------------------------------------------------------------------------
def pass_if_goto_else(lines):
    changed = True
    while changed:
        changed = False
        i = 0
        while i < len(lines):
            g = extract_goto(lines[i])
            if g and g[0] is not None:
                cond, lbl_a = g
                la = find_label(lines, lbl_a, i+1)
                if la > i + 1:
                    # find unconditional goto just before label A
                    pre = la - 1
                    while pre > i and not lines[pre].strip():
                        pre -= 1
                    g2 = extract_goto(lines[pre])
                    if g2 and g2[0] is None and pre > i:
                        lbl_b = g2[1]
                        lb = find_label(lines, lbl_b, la+1)
                        if lb > la and label_use_count(lines, lbl_a) == 1:
                            else_block = lines[i+1:pre]
                            then_block = lines[la+1:lb]
                            if (not block_has_external_label_targets(lines, i+1, pre, lines)
                                and not block_has_external_label_targets(lines, la+1, lb, lines)):
                                ind = get_indent(lines[i])
                                new = [f'{ind}if ({cond}) {{']
                                new += indent_block(then_block)
                                new += [f'{ind}}} else {{']
                                new += indent_block(else_block)
                                new += [f'{ind}}}']
                                # keep label B if still used (minus the one we removed)
                                end = lb + 1 if label_use_count(lines, lbl_b) <= 1 else lb
                                lines[i:end] = new
                                changed = True
                                continue
            i += 1
    return lines

# ---------------------------------------------------------------------------
# Pass 3: Backward goto -> do-while
#   L:  <body>  if (cond) goto L;   ->  do { body } while (cond);
#   L:  <body>  goto L;             ->  while (1) { body }
# Only when L has exactly one goto reference.
# ---------------------------------------------------------------------------
def pass_backward_loop(lines):
    changed = True
    while changed:
        changed = False
        i = 0
        while i < len(lines):
            g = extract_goto(lines[i])
            if g:
                cond, lbl = g
                li = find_label(lines, lbl, 0, i)
                if li >= 0 and label_use_count(lines, lbl) == 1:
                    body = lines[li+1:i]
                    ind = get_indent(lines[li])
                    if cond:
                        new = [f'{ind}do {{'] + indent_block(body) + [f'{ind}}} while ({cond});']
                    else:
                        new = [f'{ind}while (1) {{'] + indent_block(body) + [f'{ind}}}']
                    lines[li:i+1] = new
                    changed = True
                    continue
            i += 1
    return lines

# ---------------------------------------------------------------------------
# Pass 4: goto to label right after loop end -> break
#   Inside do{}/while{}: if (cond) goto L;  ... } while(); L: ;
# ---------------------------------------------------------------------------
def pass_break_conversion(lines):
    changed = True
    while changed:
        changed = False
        for i, line in enumerate(lines):
            s = line.strip()
            if s.startswith('do {') or (s.startswith('while (') and s.endswith('{')):
                depth = 0
                end = -1
                for j in range(i, len(lines)):
                    depth += lines[j].count('{') - lines[j].count('}')
                    if depth == 0:
                        end = j
                        break
                if end < 0:
                    continue
                # Check lines after loop end for a label
                after = end + 1
                while after < len(lines) and not lines[after].strip():
                    after += 1
                if after < len(lines):
                    after_lbl = is_label_line(lines[after])
                    if after_lbl:
                        # Convert gotos inside loop to break
                        for k in range(i+1, end):
                            gk = extract_goto(lines[k])
                            if gk and gk[1] == after_lbl:
                                ck, _ = gk
                                ind_k = get_indent(lines[k])
                                if ck:
                                    lines[k] = f'{ind_k}if ({ck}) break;'
                                else:
                                    lines[k] = f'{ind_k}break;'
                                changed = True
                        # Remove label if no longer used
                        if label_use_count(lines, after_lbl) == 0:
                            lines[after] = ''
    return lines

# ---------------------------------------------------------------------------
# Pass 5: goto to loop condition label -> continue
#   Inside do{}: ... goto LOOP_CONTINUE; ... LOOP_CONTINUE: ; <advance> } while();
# Where LOOP_CONTINUE is a label just before the advance + loop-check.
# ---------------------------------------------------------------------------
def pass_continue_conversion(lines):
    changed = True
    while changed:
        changed = False
        for i, line in enumerate(lines):
            s = line.strip()
            if s.startswith('do {'):
                depth = 0
                end = -1
                for j in range(i, len(lines)):
                    depth += lines[j].count('{') - lines[j].count('}')
                    if depth == 0:
                        end = j
                        break
                if end < 0:
                    continue
                # Find labels inside the loop that are only targeted from within loop
                for k in range(i+1, end):
                    lbl = is_label_line(lines[k])
                    if lbl:
                        # Check if all gotos to this label are inside the loop
                        all_inside = True
                        any_goto = False
                        for m_idx in range(len(lines)):
                            if f'goto {lbl}' in lines[m_idx]:
                                any_goto = True
                                if m_idx <= i or m_idx >= end:
                                    all_inside = False
                                    break
                        if any_goto and all_inside:
                            # Check if this label is near the end of loop (within last 3 non-empty lines before end)
                            # This would be a continue target
                            non_empty_after = [idx for idx in range(k+1, end) if lines[idx].strip()]
                            if len(non_empty_after) <= 2:
                                # Convert all gotos to this label within the loop to continue
                                for m_idx in range(i+1, end):
                                    gm = extract_goto(lines[m_idx])
                                    if gm and gm[1] == lbl:
                                        cm, _ = gm
                                        ind_m = get_indent(lines[m_idx])
                                        if cm:
                                            lines[m_idx] = f'{ind_m}if ({cm}) continue;'
                                        else:
                                            lines[m_idx] = f'{ind_m}continue;'
                                        changed = True
                                if changed and label_use_count(lines, lbl) == 0:
                                    lines[k] = ''
    return lines

# ---------------------------------------------------------------------------
# Pass 6: Chain of if-goto to same forward label -> compound OR conditions
#   if (A) goto L;  if (B) goto L;  if (C) goto L;  <code>  L:
#   -> if (A || B || C) goto L;  <code>  L:
#   (Then pass 1 can convert the single goto)
# ---------------------------------------------------------------------------
def pass_or_chain(lines):
    changed = True
    while changed:
        changed = False
        i = 0
        while i < len(lines):
            g = extract_goto(lines[i])
            if g and g[0] is not None:
                cond1, lbl = g
                # Look ahead for more if-gotos to same label
                j = i + 1
                conds = [cond1]
                while j < len(lines):
                    gj = extract_goto(lines[j])
                    if gj and gj[0] is not None and gj[1] == lbl:
                        conds.append(gj[0])
                        j += 1
                    else:
                        break
                if len(conds) > 1:
                    # Merge into one conditional goto
                    merged = ' || '.join(f'({c})' for c in conds)
                    ind = get_indent(lines[i])
                    lines[i:j] = [f'{ind}if ({merged}) goto {lbl};']
                    changed = True
                    continue
            i += 1
    return lines

# ---------------------------------------------------------------------------
# Pass 7: goto-into-do-while -> while conversion
#   goto COND;  do { <body>  COND: ; } while (cond);
#   -> while (cond) { <body> }
#   (COND label is at the while-check position)
# ---------------------------------------------------------------------------
def pass_goto_into_dowhile(lines):
    changed = True
    while changed:
        changed = False
        i = 0
        while i < len(lines):
            g = extract_goto(lines[i])
            if g and g[0] is None:
                lbl = g[1]
                # Look for do { after this goto
                next_i = i + 1
                while next_i < len(lines) and not lines[next_i].strip():
                    next_i += 1
                if next_i < len(lines) and lines[next_i].strip().startswith('do {'):
                    # Find matching end
                    depth = 0
                    end = -1
                    for j in range(next_i, len(lines)):
                        depth += lines[j].count('{') - lines[j].count('}')
                        if depth == 0:
                            end = j
                            break
                    if end > 0:
                        # Check if label is inside the do-while near the end
                        li = find_label(lines, lbl, next_i, end+1)
                        if li >= 0 and label_use_count(lines, lbl) == 1:
                            # Extract while condition
                            end_s = lines[end].strip()
                            m = re.match(r'\}\s*while\s*\((.+)\)\s*;', end_s)
                            if m:
                                while_cond = m.group(1)
                                ind = get_indent(lines[i])
                                body = lines[next_i+1:li]
                                after_label = lines[li+1:end]
                                # Construct while loop
                                new = [f'{ind}while ({while_cond}) {{']
                                # after_label goes first (it's the advance/check before loop body)
                                new += indent_block(after_label)
                                new += indent_block(body)
                                new += [f'{ind}}}']
                                lines[i:end+1] = new
                                changed = True
                                continue
            i += 1
    return lines

# ---------------------------------------------------------------------------
# Pass 8: if (cond) goto L; <return-block> L: (return guard)
#   where block ends with return -> if (!cond) { block }
#   Handles multiple gotos to same label
# ---------------------------------------------------------------------------
def pass_return_guard(lines):
    changed = True
    while changed:
        changed = False
        i = 0
        while i < len(lines):
            g = extract_goto(lines[i])
            if g and g[0] is not None:
                cond, lbl = g
                li = find_label(lines, lbl, i+1)
                if li > i:
                    block = lines[i+1:li]
                    non_empty = [l for l in block if l.strip()]
                    # Check block ends with return
                    if non_empty and ('return' in non_empty[-1] or non_empty[-1].strip().startswith('return')):
                        # Check no external labels targeted
                        if not block_has_external_label_targets(lines, i+1, li, lines):
                            ind = get_indent(lines[i])
                            new = [f'{ind}if ({negate(cond)}) {{']
                            new += indent_block(block)
                            new += [f'{ind}}}']
                            # Decrement label usage
                            remaining_uses = label_use_count(lines, lbl) - 1
                            if remaining_uses <= 0:
                                lines[i:li+1] = new
                            else:
                                lines[i:li] = new
                            changed = True
                            continue
            i += 1
    return lines

# ---------------------------------------------------------------------------
# Pass 9: Unconditional goto to forward label with single use
#   goto L;  <unreachable?>  L:
#   -> remove goto and label if no code between, or wrap code before label
# ---------------------------------------------------------------------------
def pass_uncond_forward(lines):
    changed = True
    while changed:
        changed = False
        i = 0
        while i < len(lines):
            g = extract_goto(lines[i])
            if g and g[0] is None:
                lbl = g[1]
                li = find_label(lines, lbl, i+1)
                if li > i and label_use_count(lines, lbl) == 1:
                    block = lines[i+1:li]
                    # If block is empty or all empty/label lines, just remove goto+label
                    non_empty = [l for l in block if l.strip() and not is_label_line(l)]
                    if not non_empty:
                        lines[i:li+1] = block  # keep any labels in between, remove goto and target label
                        changed = True
                        continue
            i += 1
    return lines

# ---------------------------------------------------------------------------
# Cleanup: remove unused labels
# ---------------------------------------------------------------------------
def cleanup_labels(lines):
    changed = True
    while changed:
        changed = False
        new_lines = []
        for line in lines:
            lbl = is_label_line(line)
            if lbl and label_use_count(lines, lbl) == 0:
                changed = True
                continue
            new_lines.append(line)
        lines = new_lines
    return lines

# ---------------------------------------------------------------------------
# Cleanup: remove consecutive blank lines (max 1)
# ---------------------------------------------------------------------------
def cleanup_blanks(lines):
    new = []
    prev_blank = False
    for line in lines:
        if not line.strip():
            if prev_blank:
                continue
            prev_blank = True
        else:
            prev_blank = False
        new.append(line)
    return new

# ---------------------------------------------------------------------------
# Main processing
# ---------------------------------------------------------------------------

def process_function(func_lines):
    """Apply all goto-elimination passes to a function. Returns (new_lines, eliminated_count)."""
    original = count_gotos(func_lines)
    if original == 0:
        return func_lines, 0

    lines = list(func_lines)
    prev = original + 1
    rounds = 0
    max_rounds = 30

    while count_gotos(lines) > 0 and count_gotos(lines) < prev and rounds < max_rounds:
        prev = count_gotos(lines)
        rounds += 1

        lines = pass_or_chain(lines)
        lines = pass_forward_cond_skip(lines)
        lines = pass_if_goto_else(lines)
        lines = pass_return_guard(lines)
        lines = pass_backward_loop(lines)
        lines = pass_goto_into_dowhile(lines)
        lines = pass_break_conversion(lines)
        lines = pass_continue_conversion(lines)
        lines = pass_uncond_forward(lines)
        lines = cleanup_labels(lines)

    lines = cleanup_blanks(lines)
    final = count_gotos(lines)
    return lines, original - final


def find_functions(lines):
    """Return list of (start_line, end_line) for top-level function definitions."""
    funcs = []
    i = 0
    while i < len(lines):
        if re.match(r'^(void|u32|s32|u16|s16|u8|s8|f32|f64|int|void\s*\*)\s+\w+\s*\(', lines[i]):
            # Find opening brace
            j = i
            while j < len(lines) and '{' not in lines[j]:
                j += 1
            if j >= len(lines):
                i += 1
                continue
            depth = 0
            for k in range(j, len(lines)):
                depth += lines[k].count('{') - lines[k].count('}')
                if depth == 0:
                    funcs.append((i, k))
                    i = k + 1
                    break
            else:
                i += 1
        else:
            i += 1
    return funcs


def process_file(filename):
    with open(filename, 'r') as f:
        all_lines = f.read().split('\n')

    functions = find_functions(all_lines)
    total_elim = 0
    total_remain = 0
    funcs_done = 0

    # Process in reverse so indices don't shift
    for start, end in reversed(functions):
        func_lines = all_lines[start:end+1]
        gc = count_gotos(func_lines)
        if gc == 0:
            continue

        new_lines, elim = process_function(func_lines)
        if elim > 0:
            all_lines[start:end+1] = new_lines
            total_elim += elim
            remain = gc - elim
            total_remain += remain
            funcs_done += 1
            name = func_lines[0].strip()[:70]
            print(f'  {name}: -{elim}/{gc} ({remain} left)')
        else:
            total_remain += gc

    with open(filename, 'w') as f:
        f.write('\n'.join(all_lines))

    print(f'\n  TOTAL: {total_elim} eliminated, {total_remain} remaining ({funcs_done} functions touched)')
    return total_elim, total_remain


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: python goto_eliminator.py <file.c> [file2.c ...]")
        sys.exit(1)

    grand_elim = 0
    grand_remain = 0
    for fn in sys.argv[1:]:
        print(f'\n=== {fn} ===')
        e, r = process_file(fn)
        grand_elim += e
        grand_remain += r

    print(f'\n=== GRAND TOTAL: {grand_elim} gotos eliminated, {grand_remain} remaining ===')
