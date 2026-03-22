#!/usr/bin/env python3
"""
Phase 2 goto eliminator for Pokemon Colosseum decompilation.

Handles the remaining patterns that convert_colosseum_gotos.py cannot:
1. Multi-ref forward merge points -> do{}while(0) + break
2. OR-condition chains -> if(a||b||c) {}
3. Consecutive conditional gotos to same target -> combined condition
4. Forward unconditional single-ref gotos (skip over dead code)
5. Label-only lines that are unreferenced -> removal
6. MWC label-before-while fix (add semicolons)
"""
import re
import sys


def invert_cond(cond):
    """Invert a C condition."""
    cond = cond.strip()
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
            label_sources.setdefault(lbl, []).append(i)
    label_pos = {}
    for i, line in enumerate(lines):
        m = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', line)
        if m:
            label_pos[m.group(1)] = i
    return label_refcount, label_pos, label_sources


def convert_or_chains(lines, label_refcount, label_pos, label_sources):
    """Convert consecutive conditional gotos to same target into OR conditions.

    Pattern:
        if (a) goto L;
        if (b) goto L;
        if (c) goto L;
        ...code...
    L:

    Into:
        if (a || b || c) goto L;
        ...code...
    L:

    This reduces goto count by combining, then other passes can convert to if-blocks.
    """
    changes = 0
    i = 0
    while i < len(lines):
        line = lines[i]
        m = re.match(r'^(\s+)if \((.+?)\) goto (L_[0-9A-Fa-f]+);$', line)
        if not m:
            i += 1
            continue

        indent, cond, target = m.group(1), m.group(2), m.group(3)

        # Look for consecutive conditional gotos to same target
        chain = [(i, cond)]
        j = i + 1
        while j < len(lines):
            m2 = re.match(r'^' + re.escape(indent) + r'if \((.+?)\) goto ' + re.escape(target) + r';$', lines[j])
            if m2:
                chain.append((j, m2.group(1)))
                j += 1
            elif not lines[j].strip():
                j += 1  # skip empty lines
            else:
                break

        if len(chain) < 2:
            i += 1
            continue

        # Combine into single OR condition
        combined = ' || '.join(c for _, c in chain)
        lines[chain[0][0]] = indent + 'if (' + combined + ') goto ' + target + ';'

        # Remove the other lines
        for idx, _ in chain[1:]:
            lines[idx] = ''
            label_refcount[target] = label_refcount.get(target, 1) - 1
            changes += 1

        i = j

    return changes


def convert_merge_to_do_while_break(lines, label_refcount, label_pos, label_sources):
    """Convert multi-ref forward merge points to do{}while(0)+break.

    Pattern: multiple gotos all targeting the same label further down.
    Convert goto MERGE -> break, wrap entire region in do{}while(0).

    Handles cases where inner labels exist as long as they are only
    referenced from within the merge range OR from gotos that also
    target our merge point.
    """
    changes = 0
    processed = set()

    for label in sorted(label_pos.keys(), key=lambda l: label_pos[l]):
        if label in processed:
            continue
        if label not in label_sources:
            continue

        pos = label_pos[label]
        refs = label_sources[label]

        # All refs must be forward
        if not all(r < pos for r in refs):
            continue

        # Need at least 2 refs
        if len(refs) < 2:
            continue

        first_ref = min(refs)

        # Don't wrap huge ranges (likely whole function)
        if pos - first_ref > 800:
            continue

        # Check brace depth: first_ref and label must be at same depth
        depth = 0
        for k in range(first_ref, pos + 1):
            depth += lines[k].count('{') - lines[k].count('}')
        if depth != 0:
            continue

        # Check: inner labels must only be referenced from within the range
        has_external_ref = False
        for k in range(first_ref, pos):
            lm = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
            if lm:
                inner_lbl = lm.group(1)
                for src in label_sources.get(inner_lbl, []):
                    if src < first_ref or src > pos:
                        has_external_ref = True
                        break
            if has_external_ref:
                break
        if has_external_ref:
            continue

        # Get indent from first ref
        indent_m = re.match(r'^(\s*)', lines[first_ref])
        indent = indent_m.group(1) if indent_m else '    '

        # Convert all gotos to breaks
        for r in refs:
            line = lines[r]
            if re.match(r'^\s+goto ' + re.escape(label) + r';$', line):
                lines[r] = re.sub(r'goto\s+' + re.escape(label) + r'\s*;', 'break;', line)
                changes += 1
            elif re.match(r'^\s+if \(.+\) goto ' + re.escape(label) + r';$', line):
                lines[r] = re.sub(r'goto\s+' + re.escape(label) + r'\s*;', 'break;', line)
                changes += 1

        # Indent all lines in range
        for k in range(first_ref, pos):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]

        # Insert do { before first_ref, } while (0); at label position
        lines[first_ref] = indent + 'do {\n' + lines[first_ref]
        lines[pos] = indent + '} while (0);'

        label_refcount[label] = 0
        processed.add(label)

    return changes


def convert_pair_merge_aggressive(lines, label_refcount, label_pos, label_sources):
    """Aggressively convert pairs of forward gotos to same label.

    For labels with exactly 2 forward refs, try to convert them
    even when inner labels exist that are referenced from within the pair,
    by using nested do{}while(0) blocks.

    Specific pattern:
        if (cond) goto L_merge;  (or uncond goto)
        ...code...
        goto L_merge;
    L_merge:

    Where inner gotos go to inner labels only within the range.
    """
    changes = 0
    processed = set()

    for label in sorted(label_pos.keys(), key=lambda l: label_pos[l]):
        if label in processed:
            continue
        if label not in label_sources:
            continue

        pos = label_pos[label]
        refs = label_sources[label]

        if not all(r < pos for r in refs):
            continue
        if len(refs) != 2:
            continue

        first_ref = min(refs)
        last_ref = max(refs)

        # Skip huge gaps
        if pos - first_ref > 400:
            continue

        # Check brace depth
        depth = 0
        for k in range(first_ref, pos + 1):
            depth += lines[k].count('{') - lines[k].count('}')
        if depth != 0:
            continue

        # Collect inner labels
        inner_labels = set()
        for k in range(first_ref, pos):
            lm = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
            if lm:
                inner_labels.add(lm.group(1))

        # Check inner labels: all references must be from within range
        has_ext = False
        for il in inner_labels:
            for src in label_sources.get(il, []):
                if src < first_ref or src >= pos:
                    has_ext = True
                    break
            if has_ext:
                break
        if has_ext:
            continue

        # Get indent
        indent_m = re.match(r'^(\s*)', lines[first_ref])
        indent = indent_m.group(1) if indent_m else '    '

        # Convert gotos to breaks
        for r in refs:
            line = lines[r]
            if re.search(r'\bgoto\s+' + re.escape(label) + r'\s*;', line):
                lines[r] = re.sub(r'goto\s+' + re.escape(label) + r'\s*;', 'break;', line)
                changes += 1

        # Indent range
        for k in range(first_ref, pos):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]

        lines[first_ref] = indent + 'do {\n' + lines[first_ref]
        lines[pos] = indent + '} while (0);'

        label_refcount[label] = 0
        processed.add(label)

    return changes


def convert_forward_single_ref(lines, label_refcount, label_pos, label_sources):
    """Convert remaining single-ref forward conditional gotos.
    Allows larger gaps and inner gotos/labels as long as inner labels
    aren't referenced from outside.
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
        # Skip huge gaps
        if label_idx - i > 500:
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


def convert_backward_single_ref(lines, label_refcount, label_pos, label_sources):
    """Convert backward single-ref conditional gotos to do-while."""
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
        # Check brace depth
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


def convert_if_else(lines, label_refcount, label_pos, label_sources):
    """Convert if/goto/else patterns.

    Pattern:
        if (cond) goto L_else;
        ...then code...
        goto L_end;
    L_else:
        ...else code...
    L_end:

    Into:
        if (!cond) {
            ...then code...
        } else {
            ...else code...
        }
    """
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

        # Find unconditional goto just before label1
        uncond_idx = None
        for k in range(label1_idx - 1, i, -1):
            stripped = lines[k].strip()
            if not stripped:
                continue
            m_u = re.match(r'^\s+goto (L_[0-9A-Fa-f]+);$', stripped)
            if m_u:
                uncond_idx = k
                uncond_label = m_u.group(1)
                break
            break

        if uncond_idx is None:
            continue
        if uncond_label not in label_pos or label_refcount.get(uncond_label, 0) != 1:
            continue

        label2_idx = label_pos[uncond_label]
        if label2_idx <= label1_idx:
            continue

        # Check no inner labels referenced from outside in then-block
        has_bad = False
        for k in range(i+1, uncond_idx):
            lm = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
            if lm:
                for src in label_sources.get(lm.group(1), []):
                    if src < i or src > label2_idx:
                        has_bad = True
                        break
            if has_bad:
                break

        # Check no inner labels in else-block
        for k in range(label1_idx+1, label2_idx):
            lm = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
            if lm:
                for src in label_sources.get(lm.group(1), []):
                    if src < i or src > label2_idx:
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
        changes += 1
        label_refcount[label1] = 0
        label_refcount[uncond_label] = 0

    return changes


def convert_while_loops(lines, label_refcount, label_pos, label_sources):
    """Convert goto L_cond; L_body: ...; L_cond: if(x) goto L_body; patterns."""
    changes = 0
    for i in range(len(lines)):
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
        if label_refcount.get(cond_label, 0) != 1:
            continue

        # Find body label right after goto
        body_label = body_idx = None
        for k in range(i+1, min(i+3, cond_idx)):
            m_body = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
            if m_body:
                body_label = m_body.group(1)
                body_idx = k
                break
        if body_label is None:
            continue

        # Find back-jump after cond label
        back_idx = back_cond = None
        for k in range(cond_idx + 1, min(cond_idx + 20, len(lines))):
            stripped = lines[k].strip()
            if not stripped:
                continue
            m_back = re.match(r'^if \((.+)\) goto (L_[0-9A-Fa-f]+);$', stripped)
            if m_back and m_back.group(2) == body_label:
                back_idx = k
                back_cond = m_back.group(1)
                break
            if re.match(r'^(L_[0-9A-Fa-f]+)\s*:', stripped):
                break
            if re.match(r'^goto\s+', stripped):
                break

        if back_idx is None:
            continue
        if label_refcount.get(body_label, 0) != 1:
            continue

        # Check inner labels not referenced from outside
        has_ext = False
        for k in range(body_idx+1, back_idx):
            lm = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
            if lm:
                for src in label_sources.get(lm.group(1), []):
                    if src < i or src > back_idx:
                        has_ext = True
                        break
            if has_ext:
                break
        if has_ext:
            continue

        # Collect update code between cond label and back-jump
        update_lines = []
        for k in range(cond_idx + 1, back_idx):
            if lines[k].strip():
                update_lines.append(k)

        # Transform: while(cond) { update; body; }
        lines[i] = indent + 'while (' + back_cond + ') {'
        lines[body_idx] = ''  # remove body label
        lines[cond_idx] = ''  # remove cond label

        # Move update code to top of loop (conceptually, but keep in place for now)
        # Actually the update is the loop condition check, keep it as break
        inv = invert_cond(back_cond)
        if inv:
            # while(1) { update; if (!cond) break; body; }
            lines[i] = indent + 'while (1) {'
            lines[back_idx] = indent + '    if (' + inv + ') break;'
        else:
            lines[back_idx] = ''

        # Indent body
        for k in range(body_idx + 1, back_idx + 1):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]

        # Also indent update code
        for k in range(cond_idx + 1, back_idx):
            if lines[k].strip() and k not in update_lines:
                pass  # already indented above

        # Close while
        if back_idx + 1 < len(lines):
            lines[back_idx] = lines[back_idx] + '\n' + indent + '}'

        changes += 1
        label_refcount[cond_label] = 0
        label_refcount[body_label] = 0

    return changes


def remove_orphan_labels(lines, label_refcount, label_pos):
    """Remove labels no longer referenced by any goto."""
    changes = 0
    # Rebuild to be safe
    goto_targets = set()
    for line in lines:
        for m in re.finditer(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', line):
            goto_targets.add(m.group(1))

    for i, line in enumerate(lines):
        m = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', line)
        if m and m.group(1) not in goto_targets:
            lines[i] = ''
            changes += 1

    return changes


def fix_mwc_label_while(lines):
    """Add semicolons after labels that appear right before } while."""
    changes = 0
    for i in range(len(lines) - 1):
        if re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*$', lines[i]):
            next_stripped = lines[i+1].strip() if i+1 < len(lines) else ''
            if next_stripped.startswith('} while'):
                lines[i] = lines[i].rstrip() + '\n' + '        ;'
                changes += 1
    return changes


def clean_empty_lines(lines):
    """Collapse more than 2 consecutive empty lines."""
    result = []
    empty = 0
    for line in lines:
        if not line.strip():
            empty += 1
            if empty <= 2:
                result.append(line)
        else:
            empty = 0
            result.append(line)
    return result


def process_file(filepath):
    with open(filepath, 'r') as f:
        content = f.read()

    initial = len(re.findall(r'\bgoto\s+L_[0-9A-Fa-f]+\s*;', content))
    print(f"Processing {filepath}: {initial} gotos")

    lines = content.split('\n')
    total_changes = 0

    for pass_num in range(1, 20):
        label_refcount, label_pos, label_sources = build_indices(lines)

        c = 0
        c += convert_or_chains(lines, label_refcount, label_pos, label_sources)

        label_refcount, label_pos, label_sources = build_indices(lines)
        c += convert_if_else(lines, label_refcount, label_pos, label_sources)

        label_refcount, label_pos, label_sources = build_indices(lines)
        c += convert_forward_single_ref(lines, label_refcount, label_pos, label_sources)

        label_refcount, label_pos, label_sources = build_indices(lines)
        c += convert_backward_single_ref(lines, label_refcount, label_pos, label_sources)

        label_refcount, label_pos, label_sources = build_indices(lines)
        c += convert_while_loops(lines, label_refcount, label_pos, label_sources)

        label_refcount, label_pos, label_sources = build_indices(lines)
        c += convert_merge_to_do_while_break(lines, label_refcount, label_pos, label_sources)

        label_refcount, label_pos, label_sources = build_indices(lines)
        c += convert_pair_merge_aggressive(lines, label_refcount, label_pos, label_sources)

        label_refcount, label_pos, label_sources = build_indices(lines)
        c += remove_orphan_labels(lines, label_refcount, label_pos)

        total_changes += c
        current = len([l for l in lines if re.search(r'\bgoto\s+L_[0-9A-Fa-f]+\s*;', l)])
        print(f"  Pass {pass_num}: {c} changes, {current} gotos remaining")

        if c == 0:
            break

    # Fix MWC label-before-while issue
    fix_mwc_label_while(lines)

    lines = clean_empty_lines(lines)

    final = len([l for l in lines if re.search(r'\bgoto\s+L_[0-9A-Fa-f]+\s*;', l)])
    removed = initial - final
    print(f"  DONE: {removed} gotos removed ({final} remaining)")

    with open(filepath, 'w', newline='\n') as f:
        f.write('\n'.join(lines))

    return initial, final


def main():
    if len(sys.argv) < 2:
        print("Usage: goto_phase2.py <file.c> [<file2.c> ...]")
        sys.exit(1)

    total_initial = 0
    total_final = 0

    for filepath in sys.argv[1:]:
        initial, final = process_file(filepath)
        total_initial += initial
        total_final += final

    total_removed = total_initial - total_final
    print(f"\n=== TOTAL: {total_removed} gotos removed ({total_final} remaining from {total_initial}) ===")


if __name__ == '__main__':
    main()
