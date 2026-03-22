#!/usr/bin/env python3
"""
Specialized goto converter for colosseum_battle.c, colosseum_script.c, colosseum_event.c.

Handles patterns the generic convert_gotos.py doesn't:
1. Guard chains (if(cond) goto NEXT; r0=0; goto MERGE; NEXT:)
2. Do-while with inner labels (permissive)
3. Forward gotos with inner gotos (wrapping in if-block)
4. While loops with update code between cond label and back-jump
5. Break/continue detection post-loop-creation
6. Merge-point collapse (multiple goto MERGE -> if/else chains)
"""
import re
import sys


def invert_cond(cond):
    """Invert a C condition. Returns None if unable."""
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
    label_sources = {}
    for i, line in enumerate(lines):
        for m in re.finditer(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', line):
            lbl = m.group(1)
            label_refcount[lbl] = label_refcount.get(lbl, 0) + 1
            if lbl not in label_sources:
                label_sources[lbl] = []
            label_sources[lbl].append(i)
    label_pos = {}
    for i, line in enumerate(lines):
        m = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', line)
        if m:
            label_pos[m.group(1)] = i
    return label_refcount, label_pos, label_sources


def convert_guard_chain(lines, label_refcount, label_pos):
    """Convert: if(cond) goto NEXT; r0=val; goto MERGE; NEXT:
    Into: if(!(cond)) { r0=val; goto MERGE; }

    This is the most common remaining pattern - short forward skip
    where the skipped code contains a goto to a merge point.
    """
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
        # Only handle small gaps (1-4 non-empty statements between)
        gap = label_idx - i - 1
        if gap < 1 or gap > 5:
            continue
        # Don't allow inner labels
        has_inner_label = False
        for k in range(i+1, label_idx):
            if re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k]):
                has_inner_label = True
                break
        if has_inner_label:
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


def convert_forward_allow_inner_gotos(lines, label_refcount, label_pos, label_sources):
    """Convert forward single-ref conditional gotos even when body contains
    gotos to external targets, as long as no inner labels are referenced
    from outside."""
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
        # Skip huge gaps (these are often whole function restructurings)
        if label_idx - i > 200:
            continue
        # Check: no inner labels referenced from outside
        has_external_ref = False
        for k in range(i+1, label_idx):
            lm = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
            if lm:
                inner_lbl = lm.group(1)
                for src in label_sources.get(inner_lbl, []):
                    if src < i or src > label_idx:
                        has_external_ref = True
                        break
            if has_external_ref:
                break
        if has_external_ref:
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


def convert_do_while_permissive(lines, label_refcount, label_pos, label_sources):
    """Convert do-while even with inner labels/gotos, as long as inner labels
    are only referenced from within the loop and the label/back-jump are at
    the same brace depth."""
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
        # Check brace depth: label and back-jump must be at same depth
        depth = 0
        for k in range(label_idx, i + 1):
            depth += lines[k].count('{') - lines[k].count('}')
        if depth != 0:
            continue
        # Check inner labels
        has_external_ref = False
        for k in range(label_idx+1, i):
            lm = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
            if lm:
                inner_lbl = lm.group(1)
                for src in label_sources.get(inner_lbl, []):
                    if src < label_idx or src > i:
                        has_external_ref = True
                        break
            if has_external_ref:
                break
        if has_external_ref:
            continue
        lines[label_idx] = indent + 'do {'
        for k in range(label_idx+1, i):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]
        lines[i] = indent + '} while (' + cond + ');'
        changes += 1
        label_refcount[label] = 0
    return changes


def convert_while_loop_with_update(lines, label_refcount, label_pos):
    """Convert while-loop where update code sits between cond label and back-jump:
        goto L_cond;
    L_body:
        ...body...
    L_cond:
        ...update code...
        if (loopCond) goto L_body;
    Into:
        while (1) {
            ...update code...
            if (!(loopCond)) break;
            ...body...
        }
    """
    changes = 0
    i = 0
    while i < len(lines):
        m_goto = re.match(r'^(\s+)goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m_goto:
            i += 1
            continue
        indent = m_goto.group(1)
        cond_label = m_goto.group(2)
        if cond_label not in label_pos:
            i += 1
            continue
        cond_idx = label_pos[cond_label]
        if cond_idx <= i:
            i += 1
            continue

        # Find body label after the goto
        body_label = body_idx = None
        for k in range(i+1, min(i+3, cond_idx)):
            m_body = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
            if m_body:
                body_label = m_body.group(1)
                body_idx = k
                break
        if body_label is None:
            i += 1
            continue

        # Find the back-jump
        back_goto_idx = back_goto_cond = None
        for k in range(cond_idx + 1, min(cond_idx + 15, len(lines))):
            stripped = lines[k].strip()
            if not stripped:
                continue
            m_back = re.match(r'^if \((.+)\) goto (L_[0-9A-Fa-f]+);$', stripped)
            if m_back and m_back.group(2) == body_label:
                back_goto_idx = k
                back_goto_cond = m_back.group(1)
                break
            if re.match(r'^L_[0-9A-Fa-f]+\s*:', stripped):
                break
            if re.match(r'^goto\s+', stripped):
                break

        if back_goto_idx is None:
            i += 1
            continue

        if label_refcount.get(body_label, 0) != 1:
            i += 1
            continue
        if label_refcount.get(cond_label, 0) != 1:
            i += 1
            continue

        # Check no gotos in cond eval code
        has_bad = False
        for k in range(cond_idx + 1, back_goto_idx):
            if re.search(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[k]):
                has_bad = True
                break
        if has_bad:
            i += 1
            continue

        inv_cond = invert_cond(back_goto_cond)
        if inv_cond is None:
            i += 1
            continue

        # Collect cond eval lines
        cond_eval_content = []
        for k in range(cond_idx + 1, back_goto_idx):
            if lines[k].strip():
                cond_eval_content.append(indent + '    ' + lines[k].strip())

        # Apply
        lines[i] = indent + 'while (1) {'
        insert_code = cond_eval_content + [indent + '    if (' + inv_cond + ') break;']
        lines[body_idx] = '\n'.join(insert_code)

        for k in range(body_idx + 1, cond_idx):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]

        lines[cond_idx] = ''
        for k in range(cond_idx + 1, back_goto_idx):
            lines[k] = ''
        lines[back_goto_idx] = indent + '}'

        label_refcount[cond_label] = 0
        label_refcount[body_label] = 0
        changes += 2
        i += 1

    return changes


def convert_goto_after_loop_to_break(lines, label_refcount, label_pos):
    """Convert gotos inside loops to break when target is right after the loop end."""
    changes = 0
    # Find loop structures
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
        elif re.match(r'^\s*while\s*\(.+\)\s*\{', line):
            depth = 1
            for j in range(i + 1, len(lines)):
                depth += lines[j].count('{') - lines[j].count('}')
                if depth <= 0:
                    loop_ranges.append((i, j))
                    break

    for loop_start, loop_end in loop_ranges:
        # Check label right after loop
        for k in range(loop_end + 1, min(loop_end + 3, len(lines))):
            stripped = lines[k].strip()
            if not stripped:
                continue
            m_lbl = re.match(r'^(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', stripped)
            if m_lbl:
                lbl = m_lbl.group(1)
                # Convert gotos to this label within the loop to break
                for j in range(loop_start, loop_end + 1):
                    mg = re.match(r'^(\s+)goto ' + re.escape(lbl) + r';$', lines[j])
                    if mg:
                        lines[j] = mg.group(1) + 'break;'
                        label_refcount[lbl] = label_refcount.get(lbl, 0) - 1
                        changes += 1
                        continue
                    mg = re.match(r'^(\s+)if \((.+)\) goto ' + re.escape(lbl) + r';$', lines[j])
                    if mg:
                        lines[j] = mg.group(1) + 'if (' + mg.group(2) + ') break;'
                        label_refcount[lbl] = label_refcount.get(lbl, 0) - 1
                        changes += 1
            break

    return changes


def convert_uncond_backward_to_loop(lines, label_refcount, label_pos, label_sources):
    """Convert unconditional backward goto with single ref into do-while(1):
        L_BODY: ;
        ...code...
        goto L_BODY;
    Into:
        do {
            ...code...
        } while (1);
    """
    changes = 0
    for i in range(len(lines)-1, -1, -1):
        line = lines[i]
        m = re.match(r'^(\s+)goto (L_[0-9A-Fa-f]+);$', line)
        if not m:
            continue
        indent, label = m.group(1), m.group(2)
        if label not in label_pos:
            continue
        label_idx = label_pos[label]
        if label_idx >= i:
            continue
        if label_refcount.get(label, 0) != 1:
            continue
        # Check brace depth
        depth = 0
        for k in range(label_idx, i + 1):
            depth += lines[k].count('{') - lines[k].count('}')
        if depth != 0:
            continue
        # Check inner labels not referenced from outside
        has_external_ref = False
        for k in range(label_idx+1, i):
            lm = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
            if lm:
                inner_lbl = lm.group(1)
                for src in label_sources.get(inner_lbl, []):
                    if src < label_idx or src > i:
                        has_external_ref = True
                        break
            if has_external_ref:
                break
        if has_external_ref:
            continue
        lines[label_idx] = indent + 'do {'
        for k in range(label_idx+1, i):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]
        lines[i] = indent + '} while (1);'
        changes += 1
        label_refcount[label] = 0
    return changes


def convert_goto_to_continue(lines, label_refcount, label_pos):
    """Convert gotos to loop headers into continue statements."""
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
        elif re.match(r'^\s*while\s*\(.+\)\s*\{', line):
            depth = 1
            for j in range(i + 1, len(lines)):
                depth += lines[j].count('{') - lines[j].count('}')
                if depth <= 0:
                    loop_ranges.append((i, j, 'while'))
                    break

    for loop_start, loop_end, loop_type in loop_ranges:
        for i in range(loop_start + 1, loop_end):
            line = lines[i]
            m = re.match(r'^(\s+)goto (L_[0-9A-Fa-f]+);$', line)
            cond = None
            if not m:
                m = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', line)
                if m:
                    cond = m.group(2)
                    lbl = m.group(3)
                    indent = m.group(1)
                else:
                    continue
            else:
                lbl = m.group(2)
                indent = m.group(1)

            if lbl not in label_pos:
                continue
            target = label_pos[lbl]

            is_continue = False
            if target == loop_start:
                is_continue = True
            elif loop_type == 'do-while' and target == loop_end:
                is_continue = True

            if is_continue:
                if cond:
                    lines[i] = indent + 'if (' + cond + ') continue;'
                else:
                    lines[i] = indent + 'continue;'
                label_refcount[lbl] = label_refcount.get(lbl, 0) - 1
                changes += 1

    return changes


def remove_dead_gotos(lines, label_refcount, label_pos):
    """Remove goto L; L: (goto to immediately following label)."""
    changes = 0
    for i in range(len(lines) - 1):
        m_goto = re.match(r'^(\s+)goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m_goto:
            continue
        label = m_goto.group(2)
        for k in range(i + 1, min(i + 5, len(lines))):
            if lines[k].strip():
                m_lbl = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
                if m_lbl and m_lbl.group(1) == label:
                    lines[i] = ''
                    label_refcount[label] = label_refcount.get(label, 0) - 1
                    changes += 1
                break
    return changes


def remove_unreferenced_labels(lines, label_refcount, label_pos):
    """Remove labels with zero references."""
    changes = 0
    for label, pos in label_pos.items():
        if label_refcount.get(label, 0) <= 0:
            m = re.match(r'^(\s*)(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[pos])
            if m:
                lines[pos] = ''
                changes += 1
    return changes


def convert_goto_to_return(lines, label_refcount, label_pos):
    """Convert gotos to labels followed by return into direct returns."""
    changes = 0
    return_labels = set()
    for lbl, pos in label_pos.items():
        for k in range(pos + 1, min(pos + 3, len(lines))):
            stripped = lines[k].strip()
            if stripped:
                if stripped == 'return;':
                    return_labels.add(lbl)
                break
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


def convert_multi_condition_or(lines, label_refcount, label_pos):
    """Collapse consecutive gotos to the same target into OR conditions."""
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


def convert_multi_ref_forward_2(lines, label_refcount, label_pos, label_sources):
    """Convert labels with exactly 2 adjacent conditional forward refs into
    combined if-block."""
    changes = 0
    processed = set()
    for label, pos in label_pos.items():
        if label in processed:
            continue
        if label_refcount.get(label, 0) != 2:
            continue
        refs = label_sources.get(label, [])
        if len(refs) != 2:
            continue
        idx1, idx2 = sorted(refs)
        if idx1 >= pos or idx2 >= pos:
            continue
        if idx2 != idx1 + 1:
            continue
        m1 = re.match(r'^(\s+)if \((.+)\) goto ' + re.escape(label) + r';$', lines[idx1])
        m2 = re.match(r'^(\s+)if \((.+)\) goto ' + re.escape(label) + r';$', lines[idx2])
        if not m1 or not m2:
            continue
        indent = m1.group(1)
        cond1, cond2 = m1.group(2), m2.group(2)
        # No labels between idx2 and target
        has_other = False
        for k in range(idx2 + 1, pos):
            if re.match(r'^\s*L_[0-9A-Fa-f]+\s*:\s*;?\s*$', lines[k]):
                has_other = True
                break
        if has_other:
            continue
        # No gotos between
        has_bad = False
        for k in range(idx2 + 1, pos):
            if re.search(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[k]):
                has_bad = True
                break
        if has_bad:
            continue
        inv1 = invert_cond(cond1)
        inv2 = invert_cond(cond2)
        if not inv1 or not inv2:
            continue
        lines[idx1] = indent + 'if (' + inv1 + ' && ' + inv2 + ') {'
        lines[idx2] = ''
        for k in range(idx2 + 1, pos):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]
        lines[pos] = indent + '}'
        label_refcount[label] = 0
        processed.add(label)
        changes += 2
    return changes


def convert_chain_checks_to_body(lines, label_refcount, label_pos, label_sources):
    """Convert pattern where multiple forward gotos to same label form an OR:
        if (check1) goto L_body;
        ...code1...
        if (check2) goto L_body;
        ...code2...
        if (!check3) goto L_skip;
    L_body:
        ...body...
    L_skip:

    This pattern is: if (check1 || check2 || check3) { body }.
    Only handle the case where the last ref is immediately before the label
    AND the second-to-last ref's intervening code is simple (no labels, no gotos).
    """
    changes = 0
    processed = set()
    for label, pos in label_pos.items():
        if label in processed:
            continue
        refs = label_sources.get(label, [])
        refcount = label_refcount.get(label, 0)
        if refcount < 2 or refcount > 10:
            continue
        # All refs must be conditional forward gotos
        all_cond = True
        all_forward = True
        ref_lines = sorted(refs)
        for r in ref_lines:
            if r >= pos:
                all_forward = False
                break
            if not re.match(r'^\s+if \((.+)\) goto ' + re.escape(label) + r';$', lines[r]):
                all_cond = False
                break
        if not all_cond or not all_forward:
            continue
        # The last ref should be close to the label (within 5 lines)
        if pos - ref_lines[-1] > 15:
            continue
        # Check: between last ref and label, should be simple code or empty
        # (it could be the "else" branch - if check3 fails, goto L_skip)
        between_last = []
        has_label_between = False
        for k in range(ref_lines[-1] + 1, pos):
            s = lines[k].strip()
            if not s:
                continue
            if re.match(r'^L_[0-9A-Fa-f]+\s*:\s*;?\s*$', s):
                has_label_between = True
                break
            between_last.append(k)
        if has_label_between:
            continue

        # Don't handle if between-last has gotos
        if any(re.search(r'\bgoto\b', lines[k]) for k in between_last):
            continue

        # Check that between consecutive refs, there are no labels or gotos from outside
        valid = True
        for ri in range(len(ref_lines) - 1):
            for k in range(ref_lines[ri] + 1, ref_lines[ri + 1]):
                s = lines[k].strip()
                if re.match(r'^L_[0-9A-Fa-f]+\s*:\s*;?\s*$', s):
                    # Check if referenced from outside
                    lm = re.match(r'^(L_[0-9A-Fa-f]+)', s)
                    inner_lbl = lm.group(1)
                    for src in label_sources.get(inner_lbl, []):
                        if src < ref_lines[0] or src > pos:
                            valid = False
                            break
                if not valid:
                    break
            if not valid:
                break
        if not valid:
            continue

        # All checks pass - now determine if this is a "skip" or "body" pattern
        # If any ref goes to a label and the code after the label is the body,
        # this is: if (check1 || check2 || ...) { body_after_label }
        # We can collapse the multiple gotos into a single OR condition

        # First: collapse the gotos into a single combined goto
        conditions = []
        for r in ref_lines:
            m = re.match(r'^\s+if \((.+)\) goto ' + re.escape(label) + r';$', lines[r])
            if m:
                conditions.append(m.group(1))

        if len(conditions) < 2:
            continue

        # Combine into one goto
        indent = re.match(r'^(\s+)', lines[ref_lines[0]]).group(1)
        combined = ' || '.join(conditions)
        lines[ref_lines[0]] = indent + 'if (' + combined + ') goto ' + label + ';'
        for r in ref_lines[1:]:
            lines[r] = ''
            label_refcount[label] = label_refcount.get(label, 0) - 1
            changes += 1
        processed.add(label)

    return changes


def convert_goto_merge_to_nested_if(lines, label_refcount, label_pos, label_sources):
    """Convert the guard chain pattern where a forward conditional goto's else branch
    contains 'r0 = 0; goto MERGE;' into a nested if:

    Pattern:
        if (!(check)) {
            r0 = 0x0;
            goto L_MERGE;
        }
        ...more_checks...

    Where L_MERGE is a multi-ref merge point. This can be converted to:
        if (check) {
            ...more_checks...
        } else {
            r0 = 0x0;
        }
        L_MERGE: ;

    But since L_MERGE is multi-ref, we can't easily remove it.
    Instead, we can wrap the whole chain in a do { ... } while(0); and convert
    the goto MERGE to break.
    """
    changes = 0
    # Find merge labels (multi-ref forward targets)
    for label, pos in label_pos.items():
        refcount = label_refcount.get(label, 0)
        if refcount < 3:
            continue
        refs = sorted(label_sources.get(label, []))
        if not refs:
            continue
        # All refs must be forward
        if any(r >= pos for r in refs):
            continue
        # All refs must be unconditional gotos
        all_uncond = all(re.match(r'^\s+goto ' + re.escape(label) + r';$', lines[r]) for r in refs)
        if not all_uncond:
            continue
        # All refs should be from "inside if blocks" - check if prev line is an assignment
        # Pattern: r0 = val; goto MERGE;
        all_assignment_before = True
        for r in refs:
            if r == 0:
                all_assignment_before = False
                break
            prev = lines[r - 1].strip()
            if not re.match(r'^r0\s*=\s*0x[0-9a-fA-F]+\s*;$', prev):
                all_assignment_before = False
                break
        if not all_assignment_before:
            continue
        # All refs are: r0 = val; goto MERGE;
        # The first ref tells us the start of the chain
        chain_start = refs[0]
        # Find what's before the chain - should be right after a function or block start

        # Wrap everything from chain_start-1 to pos in do { } while(0);
        # and convert gotos to break

        # Find the actual start of the chain (first ref's enclosing if-block)
        # This is complex - let's just convert the gotos to break and wrap in do-while(0)

        # Find the beginning of the chain region
        # It should be after some initial code
        first_ref_line = refs[0]
        # Look backwards for where the chain starts (should be before the first if-block)
        region_start = first_ref_line - 1  # r0 = val
        # Go further back to find the if statement that wraps it
        if region_start > 0:
            prev = lines[region_start - 1].strip()
            if re.match(r'^if\s*\(', prev) and '{' in prev:
                region_start = region_start - 1

        # Check if we're inside a do { } while already
        # For safety, skip if the region is too large or complex
        region_size = pos - region_start
        if region_size > 200:
            continue

        # Convert: wrap in do { } while (0); and replace gotos with break
        indent = re.match(r'^(\s*)', lines[region_start]).group(1)

        # Don't wrap if already in a loop
        in_loop = False
        for k in range(max(0, region_start - 5), region_start):
            if re.match(r'^\s*(do|while|for)\s*[\({]', lines[k]):
                in_loop = True
                break
        if in_loop:
            continue

        # Apply: wrap in do-while(0) and convert gotos to break
        lines[region_start] = indent + 'do {\n' + lines[region_start]
        for r in refs:
            lines[r] = re.sub(r'goto\s+' + re.escape(label) + r'\s*;', 'break;', lines[r])
            label_refcount[label] = label_refcount.get(label, 0) - 1
            changes += 1
        # Add closing after the last ref's enclosing if-block or before the label
        # Insert } while (0); right before the label
        lines[pos] = indent + '} while (0);\n' + lines[pos]
        # Indent the content
        for k in range(region_start + 1, pos):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]

    return changes


def process_file(filepath, verbose=False):
    """Process a single file through all conversion passes."""
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        content = f.read()
    if 'goto' not in content:
        return 0

    lines = content.split('\n')
    initial_gotos = sum(1 for line in lines if re.search(r'\bgoto\b', line))

    total_changes = 0
    for iteration in range(30):
        changes = 0

        # Rebuild indices each sub-pass
        rc, lp, ls = build_indices(lines)

        # 1: Multi-condition OR
        changes += convert_multi_condition_or(lines, rc, lp)

        # 2: Dead gotos
        rc, lp, ls = build_indices(lines)
        changes += remove_dead_gotos(lines, rc, lp)

        # 3: Goto to return
        rc, lp, ls = build_indices(lines)
        changes += convert_goto_to_return(lines, rc, lp)

        # 4: Guard chain (small forward skips with inner gotos)
        rc, lp, ls = build_indices(lines)
        changes += convert_guard_chain(lines, rc, lp)

        # 5: Forward singles allowing inner gotos
        rc, lp, ls = build_indices(lines)
        changes += convert_forward_allow_inner_gotos(lines, rc, lp, ls)

        # 6: Multi-ref forward (2 adjacent -> AND)
        rc, lp, ls = build_indices(lines)
        changes += convert_multi_ref_forward_2(lines, rc, lp, ls)

        # 7: Do-while permissive
        rc, lp, ls = build_indices(lines)
        changes += convert_do_while_permissive(lines, rc, lp, ls)

        # 7b: Unconditional backward -> do-while(1)
        rc, lp, ls = build_indices(lines)
        changes += convert_uncond_backward_to_loop(lines, rc, lp, ls)

        # 8: While with update
        rc, lp, ls = build_indices(lines)
        changes += convert_while_loop_with_update(lines, rc, lp)

        # 9: Break from loop
        rc, lp, ls = build_indices(lines)
        changes += convert_goto_after_loop_to_break(lines, rc, lp)

        # 9b: Chain checks to body (OR collapse)
        rc, lp, ls = build_indices(lines)
        changes += convert_chain_checks_to_body(lines, rc, lp, ls)

        # 10: Continue in loop
        rc, lp, ls = build_indices(lines)
        changes += convert_goto_to_continue(lines, rc, lp)

        # 11: Remove unreferenced labels
        rc, lp, ls = build_indices(lines)
        changes += remove_unreferenced_labels(lines, rc, lp)

        total_changes += changes
        if changes == 0:
            break

    final_gotos = sum(1 for line in lines if re.search(r'\bgoto\b', line))

    with open(filepath, 'w', encoding='utf-8', errors='replace') as f:
        f.write('\n'.join(lines))

    if verbose:
        print(f'  {filepath}: {initial_gotos} -> {final_gotos} gotos ({initial_gotos - final_gotos} removed)')

    return initial_gotos - final_gotos


def main():
    verbose = '--verbose' in sys.argv or '-v' in sys.argv
    files = [a for a in sys.argv[1:] if not a.startswith('-')]
    if not files:
        files = [
            'src/game/colosseum_event.c',
            'src/game/colosseum_battle.c',
            'src/game/colosseum_script.c',
        ]

    total = 0
    for f in files:
        total += process_file(f, verbose)

    print(f'Total gotos removed: {total}')


if __name__ == '__main__':
    main()
