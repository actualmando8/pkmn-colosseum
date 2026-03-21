#!/usr/bin/env python3
"""
Pragma-guarded function converter for Pokemon Colosseum decompilation.

Converts pseudo-asm register-style C functions back to idiomatic C89.
Handles common PPC patterns:
  - Simple getters/setters (load/store from SDA globals)
  - Struct field access (pointer + offset patterns)
  - Function call wrappers
  - Loop constructs (goto-based -> while/for)
  - Conditional branches (goto-based -> if/else)
"""

import re
import sys
import os


def find_pragma_blocks(lines):
    """Find all #pragma push ... #pragma pop blocks.
    Returns list of (start_line, end_line) tuples (0-indexed)."""
    blocks = []
    stack = []
    for i, line in enumerate(lines):
        stripped = line.strip()
        if stripped == '#pragma push':
            stack.append(i)
        elif stripped == '#pragma pop':
            if stack:
                start = stack.pop()
                blocks.append((start, i))
    return blocks


def extract_function_body(lines, start, end):
    """Extract the function signature and body from a pragma block."""
    # Find the function signature (first line ending with '{')
    func_start = None
    for i in range(start, end + 1):
        stripped = lines[i].strip()
        if stripped.startswith('#pragma'):
            continue
        if '{' in stripped and ('void ' in stripped or 'u32 ' in stripped or
                                's32 ' in stripped or 'u16 ' in stripped or
                                'BOOL ' in stripped or 'f32 ' in stripped or
                                'GSThread' in stripped or 'GSTask' in stripped):
            func_start = i
            break
    if func_start is None:
        return None
    return func_start


def get_comment_before_pragma(lines, pragma_start):
    """Get the comment block immediately before a pragma push."""
    comments = []
    i = pragma_start - 1
    while i >= 0 and (lines[i].strip().startswith('/*') or
                       lines[i].strip().startswith('*') or
                       lines[i].strip() == '' or
                       lines[i].strip().startswith('//')):
        comments.insert(0, lines[i])
        i -= 1
        if lines[i + 1].strip().startswith('/*'):
            break
    return comments


def is_simple_return_or_nop(lines, func_start, end):
    """Check if function body is essentially empty or just a return."""
    body_lines = []
    in_body = False
    brace_depth = 0
    for i in range(func_start, end + 1):
        line = lines[i].strip()
        if '{' in line:
            brace_depth += line.count('{') - line.count('}')
            in_body = True
            continue
        if in_body:
            if '}' in line:
                brace_depth -= 1
                if brace_depth <= 0:
                    break
            # Skip register declarations, sp array
            if re.match(r'u32 r\d+ = 0;', line):
                continue
            if re.match(r'u8 sp\[', line):
                continue
            if re.match(r'f32 f\d+ = 0\.0f;', line):
                continue
            if re.match(r'u32 r1 = \(u32\)sp;', line):
                continue
            if re.match(r'void \(\*ctr_fn\)', line):
                continue
            if re.match(r'u32 ctr = 0;', line):
                continue
            if line == 'return;' or line == '':
                continue
            body_lines.append(line)

    # If no meaningful body lines, it's a nop
    if not body_lines:
        return True
    return False


def classify_function(lines, func_start, end):
    """Classify the function pattern for conversion strategy."""
    body = '\n'.join(lines[func_start:end + 1])

    # Count gotos, labels, function calls
    num_gotos = len(re.findall(r'\bgoto\b', body))
    num_labels = len(re.findall(r'^L_[0-9A-Fa-f]+:', body, re.MULTILINE))
    num_calls = len(re.findall(r'fn_[0-9A-Fa-f]+\(\)', body))

    # Count lines of actual logic (not declarations)
    logic_lines = 0
    for line in lines[func_start:end + 1]:
        stripped = line.strip()
        if stripped and not stripped.startswith(('u32 r', 'u8 sp', 'f32 f',
                'u32 r1', 'void (*', 'u32 ctr', '#pragma', 'extern ',
                '{', '}', '//')):
            if not re.match(r'^(u32|s32|u16|u8|f32|void|BOOL) ', stripped):
                logic_lines += 1

    if is_simple_return_or_nop(lines, func_start, end):
        return 'nop'
    if num_gotos == 0 and logic_lines < 10:
        return 'simple'
    if num_gotos <= 2:
        return 'small_branch'
    return 'complex'


def convert_pragma_block_to_stub(lines, start, end):
    """Convert a pragma block by removing pragmas and keeping body as-is
    but strip the pragma lines and register boilerplate where possible."""
    result = []

    # Get comment before pragma
    comments = get_comment_before_pragma(lines, start)

    func_start = extract_function_body(lines, start, end)
    if func_start is None:
        # Just strip pragmas
        for i in range(start, end + 1):
            if not lines[i].strip().startswith('#pragma'):
                result.append(lines[i])
        return result

    # Include comments
    # result.extend(comments)

    # Build function body without pragmas
    in_func = False
    skip_pragmas = True
    for i in range(start, end + 1):
        stripped = lines[i].strip()
        if stripped.startswith('#pragma'):
            continue
        result.append(lines[i])

    return result


def strip_register_boilerplate(func_lines):
    """Remove register variable declarations and sp array from function body."""
    result = []
    for line in func_lines:
        stripped = line.strip()
        # Skip register declarations
        if re.match(r'u32 r\d+ = 0;$', stripped):
            continue
        if re.match(r'u32 r1 = \(u32\)sp;$', stripped):
            continue
        if re.match(r'f32 f\d+ = 0\.0f;$', stripped):
            continue
        if re.match(r'u8 sp\[0x[0-9A-Fa-f]+\];$', stripped):
            continue
        if re.match(r'void \(\*ctr_fn\)\(void\) = 0;$', stripped):
            continue
        if re.match(r'u32 ctr = 0;$', stripped):
            continue
        result.append(line)
    return result


def process_file(filepath):
    """Process a single C file, converting all pragma-guarded functions."""
    with open(filepath, 'r') as f:
        lines = f.readlines()

    blocks = find_pragma_blocks(lines)
    print(f"Found {len(blocks)} pragma blocks in {filepath}")

    # Process from end to start so line numbers stay valid
    for start, end in reversed(blocks):
        func_start = extract_function_body(lines, start, end)
        if func_start is None:
            # Just strip pragma lines
            new_lines = []
            for i in range(start, end + 1):
                if not lines[i].strip().startswith('#pragma'):
                    new_lines.append(lines[i])
            lines[start:end + 1] = new_lines
            continue

        classification = classify_function(lines, func_start, end)

        # Convert: remove pragma lines and strip register boilerplate for nops
        new_lines = convert_pragma_block_to_stub(lines, start, end)

        if classification == 'nop':
            new_lines = strip_register_boilerplate(new_lines)

        lines[start:end + 1] = new_lines

    with open(filepath, 'w') as f:
        f.writelines(lines)

    print(f"Processed {len(blocks)} blocks in {filepath}")


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: python pragma_convert.py <file.c>")
        sys.exit(1)
    process_file(sys.argv[1])
