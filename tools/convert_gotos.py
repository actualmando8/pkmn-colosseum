#!/usr/bin/env python3
"""Convert goto-based patterns to structured control flow.

Enhanced version with additional pattern support:
- Forward single-ref gotos -> if blocks
- If/else patterns
- Do/while (backward single-ref)
- While-loop (goto cond; body; cond: if exit; if body)
- Multi-target OR conditions (if x==A goto L; if x!=B goto M; L:)
- Dead goto-to-next-label removal
- Unconditional goto + label on next line -> removal
- Two-condition while loop with init goto
"""
import re
import glob
import sys

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

def convert_while_loop(lines, label_refcount, label_pos):
    """Convert pattern:
        goto L_cond;
    L_body:
        ...body...
    L_cond:
        if (exitCond) return/goto_out;
        if (loopCond) goto L_body;

    Into:
        while (!exitCond && loopCond) {
            ...body...
        }

    Also handles two-condition while:
        goto L_cond;
    L_body:
        ...body...
    L_cond:
        if (cond1) goto L_exit;
        if (cond2) goto L_body;
    L_exit:
    """
    changes = 0
    for i in range(len(lines)):
        # Look for unconditional goto (the init jump)
        m_goto = re.match(r'^(\s+)goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m_goto:
            continue
        indent = m_goto.group(1)
        cond_label = m_goto.group(2)
        if cond_label not in label_pos:
            continue
        cond_idx = label_pos[cond_label]
        if cond_idx <= i:
            continue

        # Check next line is a label (body label)
        body_label = None
        body_idx = None
        for k in range(i+1, cond_idx):
            m_body = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
            if m_body:
                body_label = m_body.group(1)
                body_idx = k
                break
        if body_label is None or body_idx != i + 1:
            continue

        # At cond_idx, look for the condition pattern
        # Pattern 1: two lines - "if (exitCond) return;" + "if (loopCond) goto L_body;"
        # Pattern 2: two lines - "if (exitCond) goto L_exit;" + "if (loopCond) goto L_body;"
        # Pattern 3: one line - "if (loopCond) goto L_body;" (preceded by "if (exitCond) return;")

        # Look at lines after the cond label
        lines_after_cond = []
        k = cond_idx + 1
        while k < len(lines) and k < cond_idx + 5:
            stripped = lines[k].strip()
            if stripped:
                lines_after_cond.append((k, stripped))
            k += 1

        if len(lines_after_cond) < 1:
            continue

        # Find the backward goto to body
        back_goto_idx = None
        back_goto_cond = None
        exit_cond_lines = []

        for idx, (line_idx, stripped) in enumerate(lines_after_cond):
            m_back = re.match(r'^if \((.+)\) goto (L_[0-9A-Fa-f]+);$', stripped)
            if m_back and m_back.group(2) == body_label:
                back_goto_idx = line_idx
                back_goto_cond = m_back.group(1)
                break
            else:
                exit_cond_lines.append((line_idx, stripped))

        if back_goto_idx is None:
            continue

        # Check refcounts - cond_label should only be referenced by our goto
        if label_refcount.get(cond_label, 0) != 1:
            continue
        # body_label referenced by the back-goto (and possibly the init, but we check)
        if label_refcount.get(body_label, 0) != 1:
            continue

        # No other labels between body and cond
        has_other_labels = False
        for k in range(body_idx + 1, cond_idx):
            m_lbl = re.match(r'^\s*L_[0-9A-Fa-f]+\s*:\s*;?\s*$', lines[k])
            if m_lbl:
                has_other_labels = True
                break
        if has_other_labels:
            continue

        # No gotos in body that jump outside the loop
        has_bad = False
        for k in range(body_idx + 1, cond_idx):
            gm = re.search(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[k])
            if gm and gm.group(1) in label_pos:
                t = label_pos[gm.group(1)]
                if t < body_idx or t > back_goto_idx:
                    has_bad = True
                    break
        if has_bad:
            continue

        # Build the while condition
        # exit_cond_lines contains conditions that exit the loop
        cond_parts = []
        exit_lines_to_remove = []
        valid = True
        for line_idx, stripped in exit_cond_lines:
            # "if (cond) return;" -> while (!cond)
            m_ret = re.match(r'^if \((.+)\) return;$', stripped)
            if m_ret:
                inv = invert_cond(m_ret.group(1))
                if inv is None:
                    valid = False
                    break
                cond_parts.append(inv)
                exit_lines_to_remove.append(line_idx)
                continue
            # "if (cond) goto L_exit;" where L_exit is right after back_goto
            m_exit = re.match(r'^if \((.+)\) goto (L_[0-9A-Fa-f]+);$', stripped)
            if m_exit:
                exit_label = m_exit.group(2)
                if exit_label in label_pos:
                    exit_pos = label_pos[exit_label]
                    # exit label should be right after back_goto_idx (or close)
                    if exit_pos == back_goto_idx + 1 or exit_pos == back_goto_idx + 2:
                        inv = invert_cond(m_exit.group(1))
                        if inv is None:
                            valid = False
                            break
                        cond_parts.append(inv)
                        exit_lines_to_remove.append(line_idx)
                        continue
            valid = False
            break

        if not valid:
            # Try simple case: just a single back-jump, no exit conditions
            if len(exit_cond_lines) == 0:
                # while_cond is just back_goto_cond
                while_cond = back_goto_cond
                lines[i] = indent + 'while (' + while_cond + ') {'
                lines[body_idx] = ''
                for k in range(body_idx + 1, cond_idx):
                    if lines[k].strip():
                        lines[k] = '    ' + lines[k]
                lines[cond_idx] = ''
                lines[back_goto_idx] = indent + '}'
                label_refcount[cond_label] = 0
                label_refcount[body_label] = 0
                changes += 2
            continue

        # Add the back-goto condition
        cond_parts.append(back_goto_cond)

        # Build while condition
        while_cond = ' && '.join(cond_parts)

        # Apply transformation
        lines[i] = indent + 'while (' + while_cond + ') {'
        lines[body_idx] = ''  # remove body label
        for k in range(body_idx + 1, cond_idx):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]
        lines[cond_idx] = ''  # remove cond label
        for line_idx in exit_lines_to_remove:
            lines[line_idx] = ''
        lines[back_goto_idx] = indent + '}'

        label_refcount[cond_label] = 0
        label_refcount[body_label] = 0
        changes += 2  # removed at least the init goto + the back goto

    return changes

def remove_dead_gotos(lines, label_refcount, label_pos):
    """Remove patterns like:
        goto L_X;
    L_X:
    Where goto jumps to the very next label (effectively a no-op).
    Also handles: goto L_X; L_X: goto L_Y; -> goto L_Y; (chain collapse)
    """
    changes = 0
    for i in range(len(lines) - 1):
        m_goto = re.match(r'^(\s+)goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m_goto:
            continue
        label = m_goto.group(2)
        # Find the next non-empty line
        next_idx = None
        for k in range(i + 1, min(i + 5, len(lines))):
            if lines[k].strip():
                next_idx = k
                break
        if next_idx is None:
            continue
        # Check if next non-empty line is the target label
        m_lbl = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[next_idx])
        if m_lbl and m_lbl.group(1) == label:
            lines[i] = ''
            label_refcount[label] = label_refcount.get(label, 0) - 1
            changes += 1
    return changes

def remove_unreferenced_labels(lines, label_refcount, label_pos):
    """Remove labels that have zero references."""
    changes = 0
    for label, pos in label_pos.items():
        if label_refcount.get(label, 0) <= 0:
            # Make sure the line is just a label
            m = re.match(r'^(\s*)(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[pos])
            if m:
                lines[pos] = ''
                changes += 1
    return changes

def convert_multi_target_or(lines, label_refcount, label_pos):
    """Convert pattern:
        if (x == A) goto L_target;
        if (x != B) goto L_other;
    L_target: ;
        ...code...

    Into:
        if (x == A || x == B) {
            ...code...
        }

    This handles the common multi-target branch where two conditions
    share the same target label, forming an OR condition.
    """
    changes = 0
    for i in range(len(lines) - 1):
        # Match first: if (x == A) goto L_target;
        m1 = re.match(r'^(\s+)if \((.+) == (.+)\) goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m1:
            continue
        indent = m1.group(1)
        var1 = m1.group(2)
        val1 = m1.group(3)
        target = m1.group(4)

        if target not in label_pos:
            continue
        target_idx = label_pos[target]

        # Match second: if (x != B) goto L_other; on the next line
        next_line = i + 1
        if next_line >= len(lines):
            continue
        m2 = re.match(r'^(\s+)if \((.+) != (.+)\) goto (L_[0-9A-Fa-f]+);$', lines[next_line])
        if not m2:
            continue
        var2 = m2.group(2)
        val2 = m2.group(3)
        other_label = m2.group(4)

        # Variables must match
        if var1 != var2:
            continue

        # Target label must be right after the second if (line i+2 or thereabouts)
        if target_idx != next_line + 1 and target_idx != next_line + 2:
            continue

        # This pattern: if (x == A) goto target; if (x != B) goto other; target:
        # Means: if x == A OR x == B, fall through to target code
        # The "goto other" is the else branch

        # Only handle if label_refcount for target is exactly 1 (from our goto)
        if label_refcount.get(target, 0) != 1:
            continue

        # Replace with combined condition
        # if (x == A) goto target; if (x != B) goto other;
        # becomes: r0 = 0; if (x == A || x == B) r0 = 1;
        # But that changes semantics... let's be careful.

        # Actually, the common pattern in this codebase is:
        #   if (x == 0x32) goto L; if (x != 0x1e) goto M; L: ; r0 = 1; M: ;
        # This means: if (x == 0x32 || x == 0x1e) { r0 = 1; }

        # For now, just combine the two conditions and the label
        lines[i] = ''  # remove first goto
        # Change second line to combined condition
        # if (x != B) goto other -> if (x != A && x != B) goto other
        lines[next_line] = indent + 'if (' + var1 + ' != ' + val1 + ' && ' + var2 + ' != ' + val2 + ') goto ' + other_label + ';'
        # Remove the target label since it's now unreferenced
        if target_idx < len(lines):
            m_lbl = re.match(r'^\s*L_[0-9A-Fa-f]+\s*:\s*;?\s*$', lines[target_idx])
            if m_lbl:
                lines[target_idx] = ''
        label_refcount[target] = 0
        changes += 1

    return changes

def convert_flag_set_skip(lines, label_refcount, label_pos):
    """Convert pattern:
        var = init_val;
        if (cond) goto L_skip;
        var = other_val;
    L_skip: ;

    Into:
        if (!(cond)) {
            var = other_val;
        } else {
            var = init_val;  (already set above)
        }

    Or more simply, into:
        var = init_val;
        if (!(cond)) {
            var = other_val;
        }
    """
    changes = 0
    for i in range(len(lines)):
        m = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', lines[i])
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
        # Check that between goto and label there are only 1-3 simple assignment lines
        body_lines = []
        skip = False
        for k in range(i+1, label_idx):
            s = lines[k].strip()
            if not s:
                continue
            if re.match(r'^\s*L_[0-9A-Fa-f]+\s*:\s*;?\s*$', lines[k]):
                skip = True
                break
            if re.search(r'\bgoto\s+', s):
                skip = True
                break
            body_lines.append(k)
        if skip or len(body_lines) == 0 or len(body_lines) > 5:
            continue
        # All body lines should be simple assignments or statements
        inv = invert_cond(cond)
        if inv is None:
            continue
        lines[i] = indent + 'if (' + inv + ') {'
        for k in body_lines:
            if lines[k].strip():
                lines[k] = '    ' + lines[k]
        lines[label_idx] = indent + '}'
        label_refcount[label] = 0
        changes += 1
    return changes

def convert_multi_ref_forward(lines, label_refcount, label_pos):
    """Convert forward gotos where a label is referenced by exactly 2 adjacent
    conditional gotos, forming an OR condition.

    Pattern:
        if (condA) goto L_target;
        if (condB) goto L_target;
        ...code between...
    L_target:

    Convert to:
        if (!(condA) && !(condB)) {
            ...code between...
        }
    """
    changes = 0
    processed = set()

    for label, pos in label_pos.items():
        if label in processed:
            continue
        if label_refcount.get(label, 0) != 2:
            continue

        # Find both references
        refs = []
        for k in range(len(lines)):
            m = re.match(r'^(\s+)if \((.+)\) goto ' + re.escape(label) + r';$', lines[k])
            if m:
                refs.append((k, m.group(1), m.group(2)))

        if len(refs) != 2:
            continue

        idx1, indent1, cond1 = refs[0]
        idx2, indent2, cond2 = refs[1]

        # Both must be before the label
        if idx1 >= pos or idx2 >= pos:
            continue

        # They must be adjacent (idx2 == idx1 + 1)
        if idx2 != idx1 + 1:
            continue

        # No other labels between second goto and target
        has_other = False
        for k in range(idx2 + 1, pos):
            if re.match(r'^\s*L_[0-9A-Fa-f]+\s*:\s*;?\s*$', lines[k]):
                has_other = True
                break
        if has_other:
            continue

        # No gotos between second ref and label
        has_bad = False
        for k in range(idx2 + 1, pos):
            if re.search(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[k]):
                has_bad = True
                break
        if has_bad:
            continue

        # Invert both conditions
        inv1 = invert_cond(cond1)
        inv2 = invert_cond(cond2)
        if inv1 is None or inv2 is None:
            continue

        # Apply: combine into if (inv1 && inv2) { body }
        lines[idx1] = indent1 + 'if (' + inv1 + ' && ' + inv2 + ') {'
        lines[idx2] = ''
        for k in range(idx2 + 1, pos):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]
        lines[pos] = indent1 + '}'
        label_refcount[label] = 0
        processed.add(label)
        changes += 2

    return changes

def main():
    c_files = glob.glob('src/**/*.c', recursive=True)
    total_changes = 0

    for iteration in range(15):
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

            # Pass 1: Remove dead gotos (goto L; L:)
            label_refcount, label_pos = build_indices(lines)
            changes += remove_dead_gotos(lines, label_refcount, label_pos)

            # Pass 2: Forward single-ref gotos -> if blocks
            label_refcount, label_pos = build_indices(lines)
            changes += convert_forward_singles(lines, label_refcount, label_pos)

            # Pass 3: If/else patterns
            label_refcount, label_pos = build_indices(lines)
            changes += convert_if_else(lines, label_refcount, label_pos)

            # Pass 4: Do/while (backward single-ref)
            label_refcount, label_pos = build_indices(lines)
            changes += convert_do_while(lines, label_refcount, label_pos)

            # Pass 5: While loops (goto cond; body; cond: exit_check; back_goto)
            label_refcount, label_pos = build_indices(lines)
            changes += convert_while_loop(lines, label_refcount, label_pos)

            # Pass 6: Multi-target OR conditions
            label_refcount, label_pos = build_indices(lines)
            changes += convert_multi_target_or(lines, label_refcount, label_pos)

            # Pass 7: Flag-set skip patterns
            label_refcount, label_pos = build_indices(lines)
            changes += convert_flag_set_skip(lines, label_refcount, label_pos)

            # Pass 8: Multi-ref forward gotos (2 adjacent conditions -> OR)
            label_refcount, label_pos = build_indices(lines)
            changes += convert_multi_ref_forward(lines, label_refcount, label_pos)

            # Pass 9: Remove unreferenced labels
            label_refcount, label_pos = build_indices(lines)
            changes += remove_unreferenced_labels(lines, label_refcount, label_pos)

            if changes > 0:
                with open(filepath, 'w', encoding='utf-8', errors='replace') as f:
                    f.write('\n'.join(lines))
                iter_changes += changes
                total_changes += changes

        if iter_changes == 0:
            break
        print(f'Iteration {iteration+1}: {iter_changes} gotos/labels converted')

    print(f'Total: {total_changes} gotos/labels converted')

if __name__ == '__main__':
    main()
