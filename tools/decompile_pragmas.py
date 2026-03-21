#!/usr/bin/env python3
"""
Decompile pragma-guarded pseudo-asm functions into idiomatic C89.

Handles the nested pragma pattern:
  #pragma push
  #pragma force_active on     <-- outer wrapper (keep)

  #pragma optimization_level 0   <-- per-function guard (remove)
  #pragma optimizewithasm off    <-- per-function guard (remove)
  void fn_XXXXXXXX(void) { ... }

  #pragma push                   <-- nested per-function (remove)
  #pragma optimization_level 0
  #pragma optimizewithasm off
  void fn_YYYYYYYY(void) { ... }
  #pragma pop                    <-- nested per-function (remove)

  #pragma pop                 <-- outer wrapper (keep)
"""

import re
import sys
import os


def read_file(path):
    with open(path, 'r', encoding='utf-8') as f:
        return f.readlines()


def write_file(path, lines):
    with open(path, 'w', encoding='utf-8', newline='\n') as f:
        f.writelines(lines)


def find_function_pragma_blocks(lines):
    """Find pragma-guarded function blocks. These are either:
    1. Standalone: #pragma push / optimization_level 0 / optimizewithasm off ... #pragma pop
    2. Inline within outer force_active block: optimization_level 0 / optimizewithasm off ... (next pragma or end)

    Returns list of (start_pragma, end_pragma, func_line, func_end_brace) tuples.
    """
    blocks = []
    i = 0
    while i < len(lines):
        s = lines[i].strip()

        # Look for #pragma optimization_level 0 followed by #pragma optimizewithasm off
        if s == '#pragma optimization_level 0':
            opt_line = i
            # Check for optimizewithasm off on next non-empty line
            j = i + 1
            while j < len(lines) and lines[j].strip() == '':
                j += 1
            if j < len(lines) and lines[j].strip() == '#pragma optimizewithasm off':
                asm_line = j

                # Find the pragma push before (if any)
                push_line = None
                k = opt_line - 1
                while k >= 0:
                    ks = lines[k].strip()
                    if ks == '#pragma push':
                        push_line = k
                        break
                    elif ks == '' or ks.startswith('#pragma'):
                        k -= 1
                        continue
                    else:
                        break

                # Find the function definition
                func_line = None
                j2 = asm_line + 1
                while j2 < len(lines):
                    fs = lines[j2].strip()
                    if fs == '' or fs.startswith('//') or fs.startswith('/*'):
                        j2 += 1
                        continue
                    # Match function definition
                    if re.match(r'^(void|u32|s32|u16|u8|BOOL|f32|GSThread\*|GSTask\*)\s+fn_[0-9A-Fa-f]+\s*\(', fs):
                        func_line = j2
                        break
                    j2 += 1
                    break  # Only skip one non-function line

                if func_line is not None:
                    # Find the closing brace of the function
                    brace_depth = 0
                    func_end = func_line
                    for j3 in range(func_line, len(lines)):
                        brace_depth += lines[j3].count('{') - lines[j3].count('}')
                        if brace_depth == 0 and '{' in ''.join(lines[func_line:j3+1]):
                            func_end = j3
                            break

                    # Find #pragma pop after
                    pop_line = None
                    j4 = func_end + 1
                    while j4 < len(lines):
                        ps = lines[j4].strip()
                        if ps == '#pragma pop':
                            pop_line = j4
                            break
                        elif ps == '' or ps.startswith('//'):
                            j4 += 1
                            continue
                        else:
                            break

                    start = push_line if push_line is not None else opt_line
                    end = pop_line if pop_line is not None else func_end

                    blocks.append((start, end, func_line, func_end))
                    i = end + 1
                    continue
        i += 1

    return blocks


def extract_body(lines, func_line, func_end):
    """Extract function body lines between { and }."""
    body = []
    in_body = False
    brace_depth = 0
    for i in range(func_line, func_end + 1):
        line = lines[i]
        if not in_body:
            if '{' in line:
                in_body = True
                brace_depth = line.count('{') - line.count('}')
                # Get text after first {
                idx = line.index('{')
                rest = line[idx+1:].strip()
                if rest and rest != '}':
                    body.append(rest)
                continue
        else:
            bc = line.count('{') - line.count('}')
            brace_depth += bc
            if brace_depth <= 0:
                # closing brace
                idx = line.index('}')
                rest = line[:idx].strip()
                if rest:
                    body.append(rest)
                break
            body.append(line.strip())
    return body


def parse_body(body_lines):
    """Parse body into externs, register decls, and logic lines."""
    externs = []
    regs = set()
    fregs = set()
    sp_size = 0
    logic = []
    has_ctr_fn = False
    has_ctr = False

    for line in body_lines:
        # extern
        m = re.match(r'extern\s+(\w+(?:\s*\*)?)\s+(\w+).*?;', line)
        if m:
            externs.append(line)
            continue
        # sp array
        m = re.match(r'u8\s+sp\[0x([0-9A-Fa-f]+)\];', line)
        if m:
            sp_size = int(m.group(1), 16)
            continue
        # u32 rN = 0;
        m = re.match(r'u32\s+(r\d+)\s*=\s*0;$', line)
        if m:
            regs.add(m.group(1))
            continue
        # u32 r1 = (u32)sp;
        if re.match(r'u32\s+r1\s*=\s*\(u32\)sp;', line):
            continue
        # f32 fN = 0.0f;
        m = re.match(r'f32\s+(f\d+)\s*=\s*0\.0f;$', line)
        if m:
            fregs.add(m.group(1))
            continue
        # void (*ctr_fn)(void) = 0;
        if re.match(r'void\s+\(\*ctr_fn\)', line):
            has_ctr_fn = True
            continue
        # u32 ctr = 0;
        if re.match(r'u32\s+ctr\s*=\s*0;$', line):
            has_ctr = True
            continue
        logic.append(line)

    return externs, regs, fregs, sp_size, has_ctr_fn, has_ctr, logic


def get_used_regs(logic_lines, all_regs):
    """Find which registers are actually referenced in logic lines."""
    text = '\n'.join(logic_lines)
    used = set()
    for reg in all_regs:
        if re.search(r'\b' + re.escape(reg) + r'\b', text):
            used.add(reg)
    return used


def is_nop_function(logic_lines):
    """Check if function body is essentially empty."""
    for line in logic_lines:
        s = line.strip()
        if not s or s == 'return;':
            continue
        if s.startswith('/*') and s.endswith('*/'):
            continue
        if re.match(r'^L_[0-9A-Fa-f]+:\s*;$', s):
            continue
        return False
    return True


def rebuild_function(lines, start, end, func_line, func_end):
    """Rebuild a pragma-guarded function as idiomatic C89."""
    # Extract function signature
    sig_line = lines[func_line].strip()
    m = re.match(r'^((?:void|u32|s32|u16|u8|BOOL|f32|GSThread\*|GSTask\*)\s+fn_[0-9A-Fa-f]+\s*\([^)]*\))\s*\{', sig_line)
    if not m:
        return None
    signature = m.group(1)

    body = extract_body(lines, func_line, func_end)
    externs, regs, fregs, sp_size, has_ctr_fn, has_ctr, logic = parse_body(body)

    # Determine which regs/fregs are used in logic
    used_regs = get_used_regs(logic, regs)
    used_fregs = get_used_regs(logic, fregs)

    # Check if logic uses sp or r1
    text = '\n'.join(logic)
    uses_sp = 'sp' in text
    uses_r1 = bool(re.search(r'\br1\b', text))
    uses_ctr_fn = 'ctr_fn' in text
    uses_ctr = bool(re.search(r'\bctr\b', text))

    # Check for nop
    if is_nop_function(logic):
        if 'void' in signature.split('(')[0]:
            return f'{signature} {{\n}}\n'
        else:
            return f'{signature} {{\n    return 0;\n}}\n'

    # Build the replacement
    result = []
    result.append(f'{signature} {{\n')

    # Externs
    for ext in externs:
        result.append(f'    {ext}\n')

    # sp array -- needed if sp is referenced directly OR r1 is used (r1 = (u32)sp)
    if sp_size > 0 and (uses_sp or uses_r1):
        result.append(f'    u8 sp[0x{sp_size:X}];\n')

    # Register declarations (only used ones)
    for reg in sorted(used_regs, key=lambda x: int(x[1:])):
        if reg != 'r1':
            result.append(f'    u32 {reg} = 0;\n')

    # r1 is special -- it's the stack pointer, declared as (u32)sp
    if uses_r1:
        result.append('    u32 r1 = (u32)sp;\n')

    # Float register declarations
    for freg in sorted(used_fregs, key=lambda x: int(x[1:])):
        result.append(f'    f32 {freg} = 0.0f;\n')

    # ctr_fn / ctr
    if uses_ctr_fn:
        result.append('    void (*ctr_fn)(void) = 0;\n')
    if uses_ctr:
        result.append('    u32 ctr = 0;\n')

    if used_regs or used_fregs or uses_ctr_fn or uses_ctr:
        result.append('\n')

    # Logic lines - filter out epilogue boilerplate
    for line in logic:
        s = line.strip()
        # Skip stmw/lmw asm comments
        if s.startswith('/*') and ('stmw' in s or 'lmw' in s):
            continue
        # Skip register restores from stack at epilogue
        if re.match(r'r\d+ = \*\(u32\*\)\(sp \+ 0x[0-9A-Fa-f]+\);$', s):
            continue
        result.append(f'    {s}\n')

    result.append('}\n')
    return ''.join(result)


def process_file(filepath):
    """Process a C file, converting all pragma-guarded functions."""
    lines = read_file(filepath)
    blocks = find_function_pragma_blocks(lines)

    print(f"Found {len(blocks)} pragma-guarded functions in {filepath}")

    converted = 0
    # Process from end to start
    for start, end, func_line, func_end in reversed(blocks):
        # Get comment before block
        comment_lines = []
        k = start - 1
        while k >= 0:
            ks = lines[k].strip()
            if ks.startswith('/*') or ks.startswith('*') or ks.startswith('//'):
                comment_lines.insert(0, lines[k])
                if ks.startswith('/*'):
                    break
                k -= 1
            elif ks == '':
                comment_lines.insert(0, lines[k])
                k -= 1
            else:
                break

        new_func = rebuild_function(lines, start, end, func_line, func_end)
        if new_func is None:
            # Just strip pragma lines
            new_lines = []
            for i in range(start, end + 1):
                if not lines[i].strip().startswith('#pragma'):
                    new_lines.append(lines[i])
            lines[start:end+1] = new_lines
            continue

        # Replace the block
        # Keep lines before start that are comments
        replacement = comment_lines + [new_func, '\n']

        # Calculate how far back comments go
        comment_start = start - len(comment_lines)
        if comment_start < 0:
            comment_start = 0
            comment_lines = comment_lines[-start:] if start > 0 else []

        lines[start:end+1] = [new_func, '\n']
        converted += 1

    write_file(filepath, lines)
    print(f"Converted {converted}/{len(blocks)} functions")
    return converted


def main():
    if len(sys.argv) < 2:
        print("Usage: python decompile_pragmas.py <file1.c> [file2.c ...]")
        sys.exit(1)

    total = 0
    for filepath in sys.argv[1:]:
        total += process_file(filepath)

    print(f"\nTotal: {total} functions converted")


if __name__ == '__main__':
    main()
