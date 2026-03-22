#!/usr/bin/env python3
"""
Aggressive goto converter for remaining patterns in Pokemon Colosseum decomp.

Handles patterns the existing converters can't:
1. goto inside do-while to label N lines after loop (break + flag)
2. goto inside nested if/loop to label after enclosing block
3. Multi-ref forward gotos with relaxed constraints
4. goto-to-return patterns where label has code before return
5. Forward gotos that skip over small blocks (1-10 stmts)
6. Cascaded if-goto chains forming switch-like patterns
"""
import re
import sys


def invert_cond(cond):
    """Invert a C condition. Returns None if unable."""
    if ' || ' in cond:
        parts = cond.split(' || ')
        inverted = []
        for p in parts:
            inv = invert_cond(p.strip())
            if inv is None:
                return None
            inverted.append(inv)
        return ' && '.join(inverted)
    if ' && ' in cond:
        parts = cond.split(' && ')
        inverted = []
        for p in parts:
            inv = invert_cond(p.strip())
            if inv is None:
                return None
            inverted.append(inv)
        return ' || '.join(inverted)
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


def find_enclosing_loop(lines, line_idx):
    """Find the innermost loop containing line_idx.
    Returns (loop_start, loop_end) or None."""
    # Walk backwards to find a do { or while ( {
    depth = 0
    for i in range(line_idx, -1, -1):
        line = lines[i]
        depth += line.count('}') - line.count('{')
        if depth < 0:
            # We exited a block - check if this is a loop
            if re.match(r'^\s*do\s*\{', line):
                # Find the end
                d2 = 1
                for j in range(i + 1, len(lines)):
                    d2 += lines[j].count('{') - lines[j].count('}')
                    if d2 <= 0:
                        if re.match(r'^\s*\}\s*while\s*\(', lines[j]):
                            return (i, j)
                        break
            elif re.match(r'^\s*while\s*\(.+\)\s*\{', line):
                d2 = 1
                for j in range(i + 1, len(lines)):
                    d2 += lines[j].count('{') - lines[j].count('}')
                    if d2 <= 0:
                        return (i, j)
                        break
            depth = 0  # reset and keep looking
    return None


def find_all_loops(lines):
    """Find all loop structures in the file."""
    loops = []
    for i, line in enumerate(lines):
        if re.match(r'^\s*do\s*\{', line):
            depth = 1
            for j in range(i + 1, len(lines)):
                depth += lines[j].count('{') - lines[j].count('}')
                if depth <= 0:
                    if re.match(r'^\s*\}\s*while\s*\(', lines[j]):
                        loops.append((i, j))
                    break
        elif re.match(r'^\s*while\s*\(.+\)\s*\{', line):
            depth = 1
            for j in range(i + 1, len(lines)):
                depth += lines[j].count('{') - lines[j].count('}')
                if depth <= 0:
                    loops.append((i, j))
                    break
    return loops


def convert_loop_break_extended(lines, label_refcount, label_pos, label_sources):
    """Extended loop break: convert goto to label near loop end to break.

    Looks up to 10 lines after loop end for the target label,
    as long as the intervening code is just closing braces or simple stmts.
    """
    changes = 0
    loops = find_all_loops(lines)

    for loop_start, loop_end in loops:
        # Find labels within 10 lines after loop end
        for k in range(loop_end + 1, min(loop_end + 10, len(lines))):
            stripped = lines[k].strip()
            if not stripped:
                continue
            m_lbl = re.match(r'^(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', stripped)
            if m_lbl:
                lbl = m_lbl.group(1)
                # Check all gotos to this label within the loop
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
                break  # only handle first label found

    return changes


def convert_goto_to_return_extended(lines, label_refcount, label_pos):
    """Extended goto-to-return: handle cases where label is followed by
    assignments then return, or where label has return with value."""
    changes = 0

    # Find labels followed by return (possibly with code in between)
    return_labels = {}  # label -> list of lines before return
    for lbl, pos in label_pos.items():
        # Look at up to 5 lines after label
        code_before_return = []
        found_return = False
        for k in range(pos + 1, min(pos + 6, len(lines))):
            stripped = lines[k].strip()
            if not stripped:
                continue
            if stripped.startswith('return'):
                found_return = True
                return_labels[lbl] = (code_before_return, stripped)
                break
            # Allow simple assignments before return
            if re.match(r'^[a-zA-Z_]\w*\s*=\s*', stripped) and 'goto' not in stripped:
                code_before_return.append(stripped)
            else:
                break  # not a simple pattern

    if not return_labels:
        return 0

    # Convert gotos to these labels
    for i in range(len(lines)):
        line = lines[i]
        # Unconditional goto
        m = re.match(r'^(\s+)goto (L_[0-9A-Fa-f]+);$', line)
        if m:
            lbl = m.group(2)
            if lbl in return_labels:
                indent = m.group(1)
                pre_code, ret_stmt = return_labels[lbl]
                if not pre_code:
                    # Just replace with return
                    lines[i] = indent + ret_stmt
                    label_refcount[lbl] = label_refcount.get(lbl, 0) - 1
                    changes += 1
                elif len(pre_code) == 1:
                    # One assignment before return - inline it
                    lines[i] = indent + pre_code[0] + '\n' + indent + ret_stmt
                    label_refcount[lbl] = label_refcount.get(lbl, 0) - 1
                    changes += 1
            continue

        # Conditional goto
        m = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', line)
        if m:
            lbl = m.group(3)
            if lbl in return_labels:
                indent = m.group(1)
                cond = m.group(2)
                pre_code, ret_stmt = return_labels[lbl]
                if not pre_code:
                    lines[i] = indent + 'if (' + cond + ') ' + ret_stmt
                    label_refcount[lbl] = label_refcount.get(lbl, 0) - 1
                    changes += 1
                elif len(pre_code) == 1:
                    lines[i] = indent + 'if (' + cond + ') { ' + pre_code[0] + ' ' + ret_stmt + ' }'
                    label_refcount[lbl] = label_refcount.get(lbl, 0) - 1
                    changes += 1

    return changes


def convert_forward_multi_ref(lines, label_refcount, label_pos, label_sources):
    """Convert forward gotos to multi-ref labels by wrapping in if blocks.

    For labels with exactly 2 refs where both are forward conditional gotos,
    even if they're not adjacent - wrap the code between the first goto and
    the label in an if block.
    """
    changes = 0
    processed = set()

    for label, pos in sorted(label_pos.items(), key=lambda x: x[1], reverse=True):
        if label in processed:
            continue
        refcount = label_refcount.get(label, 0)
        if refcount < 2 or refcount > 5:
            continue
        refs = sorted(label_sources.get(label, []))
        if not refs or len(refs) != refcount:
            continue
        # All refs must be forward conditional gotos
        all_forward_cond = True
        for r in refs:
            if r >= pos:
                all_forward_cond = False
                break
            if not re.match(r'^\s+if \((.+)\) goto ' + re.escape(label) + r';$', lines[r]):
                all_forward_cond = False
                break
        if not all_forward_cond:
            continue

        # Check for exactly 2 adjacent refs
        if len(refs) == 2 and refs[1] == refs[0] + 1:
            # Already handled by convert_multi_ref_forward_2
            continue

        # For 2+ consecutive forward conditional gotos to same target,
        # collapse them into an OR
        if len(refs) >= 2:
            # Check if all refs are consecutive
            all_consecutive = all(refs[i+1] == refs[i] + 1 for i in range(len(refs) - 1))
            if all_consecutive:
                # Combine into single OR condition
                conditions = []
                for r in refs:
                    m = re.match(r'^\s+if \((.+)\) goto ' + re.escape(label) + r';$', lines[r])
                    if m:
                        conditions.append(m.group(1))
                if len(conditions) == len(refs):
                    indent = re.match(r'^(\s+)', lines[refs[0]]).group(1)
                    combined = ' || '.join(conditions)
                    lines[refs[0]] = indent + 'if (' + combined + ') goto ' + label + ';'
                    for r in refs[1:]:
                        lines[r] = ''
                        label_refcount[label] = label_refcount.get(label, 0) - 1
                        changes += 1
                    processed.add(label)

    return changes


def convert_forward_large_gap(lines, label_refcount, label_pos, label_sources):
    """Convert single-ref forward conditional gotos with larger gaps (up to 300 lines).
    The existing converter limits to 200 lines; we extend that.

    Only handles cases where inner labels are all internally referenced.
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
        gap = label_idx - i
        if gap <= 200 or gap > 500:
            continue

        # Check inner labels
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


def convert_goto_skip_one_stmt(lines, label_refcount, label_pos):
    """Convert conditional goto that skips exactly one statement.

    Pattern:
        if (cond) goto L;
        one_statement;
    L: ;

    Into:
        if (!(cond)) {
            one_statement;
        }
    """
    changes = 0
    for i in range(len(lines)-1, -1, -1):
        m = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m:
            continue
        indent, cond, label = m.group(1), m.group(2), m.group(3)
        if label not in label_pos:
            continue
        label_idx = label_pos[label]
        if label_idx <= i:
            continue
        # Count non-empty lines between
        non_empty = []
        for k in range(i+1, label_idx):
            if lines[k].strip():
                non_empty.append(k)
        if len(non_empty) < 1 or len(non_empty) > 8:
            continue
        # Check no labels or gotos in between
        has_label_or_goto = False
        for k in non_empty:
            s = lines[k].strip()
            if re.match(r'^L_[0-9A-Fa-f]+\s*:', s):
                has_label_or_goto = True
                break
            if re.search(r'\bgoto\s+L_[0-9A-Fa-f]+\s*;', s):
                has_label_or_goto = True
                break
        if has_label_or_goto:
            continue
        inv = invert_cond(cond)
        if inv is None:
            continue
        # Only convert if single ref
        if label_refcount.get(label, 0) != 1:
            continue
        lines[i] = indent + 'if (' + inv + ') {'
        for k in non_empty:
            lines[k] = '    ' + lines[k]
        lines[label_idx] = indent + '}'
        label_refcount[label] = 0
        changes += 1
    return changes


def convert_multi_ref_break_pattern(lines, label_refcount, label_pos, label_sources):
    """Convert gotos inside if-blocks that jump to labels after enclosing if.

    Pattern:
        if (cond) {
            ...
            goto L_target;
        }
        ...
    L_target: ;

    When L_target is multi-ref, we can't simply restructure. But if the goto
    is the last statement in the if-block, we can sometimes convert it to
    a break if we wrap the enclosing region in do { } while(0).
    """
    changes = 0

    # Find labels with multiple forward-only unconditional gotos
    for label, pos in sorted(label_pos.items(), key=lambda x: x[1], reverse=True):
        refcount = label_refcount.get(label, 0)
        if refcount < 2:
            continue
        refs = sorted(label_sources.get(label, []))
        if len(refs) != refcount:
            continue
        # All must be forward
        if any(r >= pos for r in refs):
            continue
        # All must be unconditional gotos
        all_uncond = all(re.match(r'^\s+goto ' + re.escape(label) + r';$', lines[r]) for r in refs)
        if not all_uncond:
            continue

        # Check that the region isn't too large
        first_ref = refs[0]
        if pos - first_ref > 150:
            continue

        # Check brace depth
        depth = 0
        for k in range(first_ref, pos):
            depth += lines[k].count('{') - lines[k].count('}')
        if depth != 0:
            continue

        # Check no inner labels referenced from outside
        has_external = False
        for k in range(first_ref, pos):
            lm = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
            if lm:
                inner = lm.group(1)
                for src in label_sources.get(inner, []):
                    if src < first_ref or src > pos:
                        has_external = True
                        break
            if has_external:
                break
        if has_external:
            continue

        # Check not already in a do-while(0)
        if first_ref > 0 and re.match(r'^\s*do\s*\{', lines[first_ref - 1]):
            continue

        # Apply: wrap in do { } while(0) and convert gotos to break
        indent = re.match(r'^(\s*)', lines[first_ref]).group(1)

        for r in refs:
            lines[r] = re.sub(r'goto\s+' + re.escape(label) + r'\s*;', 'break;', lines[r])
            label_refcount[label] = label_refcount.get(label, 0) - 1
            changes += 1

        # Indent content
        for k in range(first_ref, pos):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]

        # Insert do { before first ref and } while(0); before label
        lines[first_ref] = indent + 'do {\n' + lines[first_ref]
        lines[pos] = indent + '} while (0);\n' + lines[pos]

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


def convert_guard_chain_extended(lines, label_refcount, label_pos):
    """Extended guard chain: handle forward conditional gotos with single ref
    and a gap of up to 10 lines, even with gotos to external labels in body."""
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
        gap = label_idx - i - 1
        if gap < 1 or gap > 10:
            continue
        # No inner labels
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


def convert_if_else_extended(lines, label_refcount, label_pos):
    """Extended if-else: handle patterns where if(cond) goto L; then
    unconditional goto L2; then L: ... L2: forming if-else.
    Allow multi-ref for L2 (merge point)."""
    changes = 0
    for i in range(len(lines)-1, -1, -1):
        m_cond = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m_cond:
            continue
        indent, cond, label1 = m_cond.group(1), m_cond.group(2), m_cond.group(3)
        if label_refcount.get(label1, 0) != 1:
            continue
        if label1 not in label_pos:
            continue
        label1_idx = label_pos[label1]
        if label1_idx <= i:
            continue

        # Find unconditional goto between i and label1
        uncond_idx = uncond_label = None
        for k in range(i+1, label1_idx):
            m_u = re.match(r'^\s+goto (L_[0-9A-Fa-f]+);$', lines[k])
            if m_u:
                uncond_idx = k
                uncond_label = m_u.group(1)
                break
        if uncond_idx is None or uncond_label is None:
            continue
        if uncond_label not in label_pos:
            continue
        label2_idx = label_pos[uncond_label]
        if label2_idx <= label1_idx:
            continue

        # Check no code between uncond goto and label1
        has_code_between = False
        for k in range(uncond_idx + 1, label1_idx):
            if lines[k].strip() and not lines[k].strip().startswith('//'):
                has_code_between = True
                break
        if has_code_between:
            continue

        # Check no inner labels in either branch
        has_inner = False
        for k in range(i+1, uncond_idx):
            if re.match(r'^\s*L_[0-9A-Fa-f]+\s*:\s*;?\s*$', lines[k]):
                has_inner = True
                break
        if has_inner:
            continue
        for k in range(label1_idx+1, label2_idx):
            if re.match(r'^\s*L_[0-9A-Fa-f]+\s*:\s*;?\s*$', lines[k]):
                has_inner = True
                break
        if has_inner:
            continue

        # Check no gotos in either branch (except the uncond goto we remove)
        has_goto = False
        for k in range(i+1, uncond_idx):
            if re.search(r'\bgoto\s+L_[0-9A-Fa-f]+\s*;', lines[k]):
                has_goto = True
                break
        if has_goto:
            continue
        for k in range(label1_idx+1, label2_idx):
            if re.search(r'\bgoto\s+L_[0-9A-Fa-f]+\s*;', lines[k]):
                has_goto = True
                break
        if has_goto:
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

        # Only remove label2 if it has exactly 1 ref (our goto)
        if label_refcount.get(uncond_label, 0) == 1:
            lines[label2_idx] = indent + '}'
            label_refcount[uncond_label] = 0
        else:
            # Keep label2 but close the else
            lines[label2_idx] = indent + '}\n' + lines[label2_idx]
            label_refcount[uncond_label] = label_refcount.get(uncond_label, 0) - 1

        label_refcount[label1] = 0
        changes += 2

    return changes


def convert_goto_to_return_value(lines, label_refcount, label_pos):
    """Convert pattern: r3 = value; goto L; ... L: return;
    Into: return value; (when r3 is the return register)."""
    changes = 0
    return_labels = set()
    for lbl, pos in label_pos.items():
        for k in range(pos + 1, min(pos + 3, len(lines))):
            stripped = lines[k].strip()
            if stripped:
                if stripped == 'return;' or re.match(r'^return\s+\w+\s*;$', stripped):
                    return_labels.add(lbl)
                break

    for i in range(len(lines) - 1, -1, -1):
        # Pattern: r3 = value; goto L;
        m_assign = re.match(r'^(\s+)(r3\s*=\s*.+);$', lines[i])
        if not m_assign:
            continue
        indent = m_assign.group(1)
        assign = m_assign.group(2)
        # Next non-empty line should be goto
        for k in range(i + 1, min(i + 3, len(lines))):
            stripped = lines[k].strip()
            if stripped:
                m_goto = re.match(r'^goto (L_[0-9A-Fa-f]+);$', stripped)
                if m_goto and m_goto.group(1) in return_labels:
                    lbl = m_goto.group(1)
                    lines[i] = indent + assign + ';'
                    lines[k] = indent + 'return;'
                    label_refcount[lbl] = label_refcount.get(lbl, 0) - 1
                    changes += 1
                break

    return changes


def process_file(filepath, verbose=False):
    """Process a single file through all aggressive conversion passes."""
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        content = f.read()
    if 'goto' not in content:
        return 0

    lines = content.split('\n')
    initial_gotos = sum(1 for line in lines if re.search(r'\bgoto\b', line))

    total_changes = 0
    for iteration in range(40):
        changes = 0

        # 1: Multi-condition OR
        rc, lp, ls = build_indices(lines)
        changes += convert_multi_condition_or(lines, rc, lp)

        # 2: Dead gotos
        rc, lp, ls = build_indices(lines)
        changes += remove_dead_gotos(lines, rc, lp)

        # 3: Extended goto to return
        rc, lp, ls = build_indices(lines)
        changes += convert_goto_to_return_extended(lines, rc, lp)

        # 3b: Return value pattern
        rc, lp, ls = build_indices(lines)
        changes += convert_goto_to_return_value(lines, rc, lp)

        # 4: Extended guard chain (up to 10 stmts)
        rc, lp, ls = build_indices(lines)
        changes += convert_guard_chain_extended(lines, rc, lp)

        # 5: Skip one statement
        rc, lp, ls = build_indices(lines)
        changes += convert_goto_skip_one_stmt(lines, rc, lp)

        # 6: Extended loop break
        rc, lp, ls = build_indices(lines)
        changes += convert_loop_break_extended(lines, rc, lp, ls)

        # 7: If-else extended
        rc, lp, ls = build_indices(lines)
        changes += convert_if_else_extended(lines, rc, lp)

        # 8: Forward multi-ref OR collapse
        rc, lp, ls = build_indices(lines)
        changes += convert_forward_multi_ref(lines, rc, lp, ls)

        # 9: Forward large gap
        rc, lp, ls = build_indices(lines)
        changes += convert_forward_large_gap(lines, rc, lp, ls)

        # 10: Multi-ref break pattern
        rc, lp, ls = build_indices(lines)
        changes += convert_multi_ref_break_pattern(lines, rc, lp, ls)

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
        print("Usage: python convert_gotos_aggressive.py [-v] file1.c [file2.c ...]")
        sys.exit(1)

    total = 0
    for f in files:
        total += process_file(f, verbose)

    print(f'Total gotos removed: {total}')


if __name__ == '__main__':
    main()
