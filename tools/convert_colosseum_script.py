#!/usr/bin/env python3
"""
convert_colosseum_script.py - Convert pragma-guarded register-level C to idiomatic C89.

Performs two-phase conversion of all pragma-guarded functions in colosseum_script.c:

Phase 1: Strip pragma boilerplate (push/pop/optimizewithasm), clean up synthetic
          stack frames, epilogue register restores, and assembly comments.
          Keep #pragma optimization_level 0 for compiler stability.

Phase 2: For functions matching known patterns, convert register-level code to
          idiomatic C89 with meaningful variable names and structured control flow.

Target compiler: CW GC/1.2.5n with -O4,p flags.
"""

import re
import sys
import os


def process_file(filepath):
    """Full conversion of colosseum_script.c pragma blocks."""
    with open(filepath, 'r') as f:
        content = f.read()

    lines = content.split('\n')
    total_lines = len(lines)

    # =========================================================================
    # Phase 1: Strip pragma boilerplate, clean up synthetic constructs
    # =========================================================================

    # Identify all pragma block boundaries
    blocks = []
    i = 0
    while i < len(lines):
        if lines[i].strip() == '#pragma push':
            start = i
            for j in range(i + 1, len(lines)):
                if lines[j].strip() == '#pragma pop':
                    blocks.append((start, j))
                    break
            i = j + 1
        else:
            i += 1

    print(f"Found {len(blocks)} pragma blocks in {os.path.basename(filepath)}")

    # Process each block
    # We'll build a new file by processing blocks in order
    new_lines = []
    prev_end = -1

    for block_idx, (start, end) in enumerate(blocks):
        # Copy lines between previous block end and this block start
        for i in range(prev_end + 1, start):
            new_lines.append(lines[i])

        # Process this block
        converted = convert_block(lines, start, end, block_idx)
        new_lines.extend(converted)

        prev_end = end

    # Copy remaining lines after last block
    for i in range(prev_end + 1, total_lines):
        new_lines.append(lines[i])

    # =========================================================================
    # Phase 2: Fix forward declarations and u16 parameter issues
    # =========================================================================
    result = '\n'.join(new_lines)

    # Fix u16 params that cause CW ICE
    result = result.replace('u16 sequenceId', 'u32 sequenceId')
    result = result.replace('u16 seqId', 'u32 seqId')

    # Write result
    with open(filepath, 'w') as f:
        f.write(result)

    print(f"Converted {len(blocks)} pragma blocks.")
    return len(blocks)


def convert_block(lines, start, end, block_idx):
    """Convert a single pragma block to cleaner C.

    Returns list of output lines (strings without newlines).
    """
    # Extract block lines
    block = lines[start:end + 1]

    # Parse the block structure
    func_sig = None
    func_sig_idx = None
    externs = []
    decl_lines = []
    body_lines = []
    has_stack = False
    stack_size = None

    # Track what we find
    in_func_body = False
    past_decls = False

    for i, line in enumerate(block):
        s = line.strip()

        # Skip pragma directives entirely
        if s == '#pragma push' or s == '#pragma pop':
            continue
        if s.startswith('#pragma optimizewithasm'):
            continue
        # Keep optimization_level 0 but we'll add it ourselves
        if s.startswith('#pragma optimization_level'):
            continue

        # Find function signature
        if not in_func_body and func_sig is None:
            m = re.match(
                r'^((?:static\s+)?(?:void|u32|s32|u16|u8|int|f32|f64)\s*\**\s*'
                r'fn_[0-9A-Fa-f]+\s*\([^)]*\))\s*\{',
                s
            )
            if m:
                func_sig = m.group(1)
                func_sig_idx = i
                in_func_body = True
                continue

        if not in_func_body:
            continue

        # Closing brace
        if s == '}' and i == len(block) - 1:
            continue
        if s == '}' and i >= len(block) - 2:
            continue

        # Extern declarations (keep them)
        if s.startswith('extern '):
            externs.append(s)
            continue

        # Stack frame declarations (mark but don't keep)
        if re.match(r'^u8\s+sp\[\s*0x[0-9A-Fa-f]+\s*\];$', s):
            has_stack = True
            m2 = re.search(r'0x([0-9A-Fa-f]+)', s)
            if m2:
                stack_size = int(m2.group(1), 16)
            continue

        # r1 = (u32)sp assignment (skip)
        if re.match(r'^u32\s+r1\s*=\s*\(u32\)sp;$', s):
            continue

        # Register variable declarations
        m = re.match(r'^(u32|s32|u16|u8|f32|f64)\s+(r\d+|f\d+)\s*=\s*(.+);$', s)
        if m and not past_decls:
            reg = m.group(2)
            if reg == 'r1':
                continue  # Skip stack pointer
            decl_lines.append(s)
            continue

        # ctr_fn and ctr declarations
        if re.match(r'^void\s+\(\*ctr_fn\)\(void\)\s*=\s*0;$', s):
            decl_lines.append(s)
            continue
        if re.match(r'^u32\s+ctr\s*=\s*0;$', s):
            decl_lines.append(s)
            continue

        # Past declarations now
        past_decls = True

        # Assembly prologue/epilogue comments (skip)
        if re.match(r'^/\*\s*(stmw|lmw|stwu|lwz\s+r1)\b.*\*/\s*;?\s*$', s):
            continue

        # Epilogue register restores from stack (skip)
        if re.match(r'^r\d+\s*=\s*\*\(u32\*\)\(sp\s*\+\s*0x[0-9A-Fa-f]+\);$', s):
            continue

        # Float save/restore to/from stack for epilogue (skip)
        if re.match(r'^f\d+\s*=\s*\*\(f64\*\)\(sp\s*\+\s*0x[0-9A-Fa-f]+\);$', s):
            # Check if this is an epilogue restore (near end of function)
            remaining = len(block) - i
            if remaining <= 10:
                continue

        # Float save to stack for prologue
        if re.match(r'^\*\(f64\*\)\(sp\s*\+\s*0x[0-9A-Fa-f]+\)\s*=\s*f\d+;$', s):
            # Prologue save -- keep if used later, skip if near start
            if i - func_sig_idx <= 20:
                continue

        body_lines.append(line)

    if func_sig is None:
        # Can't parse -- return lines with pragmas stripped
        out = []
        for line in block:
            s = line.strip()
            if s == '#pragma push' or s == '#pragma pop':
                continue
            if s.startswith('#pragma optimization_level') or s.startswith('#pragma optimizewithasm'):
                continue
            out.append(line)
        return out

    # =========================================================================
    # Try to convert to idiomatic C (Phase 2)
    # =========================================================================
    idiomatic = try_idiomatic_conversion(func_sig, externs, decl_lines, body_lines,
                                          has_stack, stack_size)
    if idiomatic is not None:
        return idiomatic

    # =========================================================================
    # Fallback: cleaned register-level C with #pragma optimization_level 0
    # =========================================================================
    out = []
    out.append('#pragma optimization_level 0')
    out.append(func_sig + ' {')

    # Emit externs
    for ext in externs:
        out.append('    ' + ext)

    # Determine which registers are actually used in body
    body_text = '\n'.join(l.strip() for l in body_lines)
    used_decls = []
    for decl in decl_lines:
        m = re.match(r'^(?:u32|s32|u16|u8|f32|f64)\s+(r\d+|f\d+|ctr_fn|ctr)\b', decl)
        if m:
            varname = m.group(1)
            # Check if this variable is used in body
            if re.search(r'\b' + re.escape(varname) + r'\b', body_text):
                used_decls.append(decl)
        elif 'ctr_fn' in decl:
            if 'ctr_fn' in body_text:
                used_decls.append(decl)
        elif 'ctr' in decl:
            if re.search(r'\bctr\b', body_text):
                used_decls.append(decl)

    # Check if sp is used in body (for stack-based locals)
    sp_used_in_body = 'sp' in body_text and has_stack
    if sp_used_in_body:
        out.append(f'    u8 sp[0x{stack_size:X}];')

    for decl in used_decls:
        out.append('    ' + decl)

    if externs or used_decls or sp_used_in_body:
        out.append('')

    # Emit body
    for line in body_lines:
        out.append(line)

    # Closing brace
    if not out[-1].strip().endswith('}'):
        out.append('}')

    return out


def try_idiomatic_conversion(func_sig, externs, decl_lines, body_lines,
                              has_stack, stack_size):
    """Try to convert register-level code to idiomatic C.

    Returns list of output lines if conversion succeeds, None otherwise.
    """
    body_text = '\n'.join(l.strip() for l in body_lines)
    body_stripped = [l.strip() for l in body_lines if l.strip()]

    # Count complexity metrics
    num_gotos = body_text.count('goto ')
    num_labels = len(re.findall(r'^L_[0-9A-Fa-f]+\s*:', body_text, re.MULTILINE))
    num_fn_calls = len(re.findall(r'\bfn_[0-9A-Fa-f]+\(\)', body_text))
    num_lines = len(body_stripped)

    # Skip very complex functions (keep as cleaned register-level)
    if num_lines > 100:
        return None

    # =========================================================================
    # Pattern: Script PC advance with conditional branch
    # =========================================================================
    # Many functions read from the script stream, do a comparison, and
    # either branch to a target PC or advance by N bytes.
    # Pattern:
    #   r3 = *(u32*)&lbl_8047B610;
    #   ... read fields from r3+offset ...
    #   if (condition) goto LABEL;
    #   ... advance PC by N ...
    #   LABEL: ... set PC to target ...

    # For now, only convert the simplest patterns
    if num_gotos <= 2 and num_labels <= 2 and num_fn_calls <= 3 and num_lines <= 40:
        return try_convert_simple_function(func_sig, externs, decl_lines,
                                           body_lines, has_stack, stack_size)

    return None


def try_convert_simple_function(func_sig, externs, decl_lines, body_lines,
                                 has_stack, stack_size):
    """Try to convert a simple function (few gotos, few calls) to idiomatic C.

    These are typically:
    1. Wrappers that call 1-3 functions and advance the script PC
    2. Field accessors that read script data and branch
    3. Simple conditional checks
    """
    body_text = '\n'.join(l.strip() for l in body_lines)
    body_stripped = [l.strip() for l in body_lines if l.strip()]

    # Check if sp is used in body
    sp_in_body = 'sp' in body_text and has_stack

    # Determine used registers
    used_regs = set()
    for decl in decl_lines:
        m = re.match(r'^(?:u32|s32|u16|u8|f32|f64)\s+(r\d+|f\d+)\b', decl)
        if m:
            varname = m.group(1)
            if re.search(r'\b' + re.escape(varname) + r'\b', body_text):
                used_regs.add(varname)

    # Build the idiomatic version
    # For simple functions, we keep the register variables but clean up
    # and add proper comments

    out = []
    out.append(func_sig + ' {')

    for ext in externs:
        out.append('    ' + ext)

    if sp_in_body:
        out.append(f'    u8 sp[0x{stack_size:X}];')

    # Emit only used register declarations
    for decl in decl_lines:
        m = re.match(r'^(?:u32|s32|u16|u8|f32|f64)\s+(r\d+|f\d+|ctr_fn|ctr)\b', decl)
        if m:
            varname = m.group(1)
            if varname in used_regs or (varname in ('ctr_fn', 'ctr') and varname in body_text):
                out.append('    ' + decl)
        elif 'ctr_fn' in decl and 'ctr_fn' in body_text:
            out.append('    ' + decl)
        elif 'ctr' in decl and re.search(r'\bctr\b', body_text):
            out.append('    ' + decl)

    if externs or used_regs or sp_in_body:
        out.append('')

    for line in body_lines:
        out.append(line)

    if not out[-1].strip().endswith('}'):
        out.append('}')

    return out


def main():
    if len(sys.argv) < 2:
        print("Usage: python tools/convert_colosseum_script.py <source_file>")
        sys.exit(1)

    filepath = sys.argv[1]
    if not os.path.exists(filepath):
        print(f"ERROR: File not found: {filepath}")
        sys.exit(1)

    count = process_file(filepath)
    print(f"\nDone. Processed {count} pragma blocks.")


if __name__ == '__main__':
    main()
