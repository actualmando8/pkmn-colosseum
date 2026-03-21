#!/usr/bin/env python3
"""Standalone pragma converter -- no external dependencies."""
import re

def read_file(path):
    with open(path, 'r', encoding='utf-8') as f:
        return f.readlines()

def write_file(path, lines):
    with open(path, 'w', encoding='utf-8', newline='\n') as f:
        f.writelines(lines)

def find_blocks(lines):
    blocks = []
    i = 0
    n = len(lines)
    while i < n:
        s = lines[i].strip()
        if s == '#pragma optimization_level 0':
            opt_line = i
            j = i + 1
            while j < n and lines[j].strip() == '':
                j += 1
            if j < n and lines[j].strip() == '#pragma optimizewithasm off':
                asm_line = j
                push_line = None
                k = opt_line - 1
                while k >= 0:
                    ks = lines[k].strip()
                    if ks == '#pragma push':
                        push_line = k
                        break
                    elif ks == '' or ks.startswith('#pragma'):
                        k -= 1
                    else:
                        break
                func_line = None
                j2 = asm_line + 1
                while j2 < n:
                    fs = lines[j2].strip()
                    if fs == '' or fs.startswith('//') or fs.startswith('/*'):
                        j2 += 1
                        continue
                    pat = r'^(void|u32|s32|u16|u8|BOOL|f32|GSThread\*|GSTask\*)\s+fn_[0-9A-Fa-f]+\s*\('
                    if re.match(pat, fs):
                        func_line = j2
                    break
                if func_line is not None:
                    bd = 0
                    func_end = func_line
                    found_open = False
                    for j3 in range(func_line, n):
                        bd += lines[j3].count('{') - lines[j3].count('}')
                        if '{' in lines[j3]:
                            found_open = True
                        if found_open and bd == 0:
                            func_end = j3
                            break
                    pop_line = None
                    j4 = func_end + 1
                    while j4 < n:
                        ps = lines[j4].strip()
                        if ps == '#pragma pop':
                            pop_line = j4
                            break
                        elif ps == '' or ps.startswith('//'):
                            j4 += 1
                        else:
                            break
                    start = push_line if push_line is not None else opt_line
                    end = pop_line if pop_line is not None else func_end
                    blocks.append((start, end, func_line, func_end))
                    i = end + 1
                    continue
        i += 1
    return blocks

def extract_body(lines, fl, fe):
    body = []
    in_body = False
    bd = 0
    for i in range(fl, fe + 1):
        line = lines[i]
        if not in_body:
            if '{' in line:
                in_body = True
                bd = line.count('{') - line.count('}')
                idx = line.index('{')
                rest = line[idx+1:].strip()
                if rest and rest != '}':
                    body.append(rest)
                continue
        else:
            bc = line.count('{') - line.count('}')
            bd += bc
            if bd <= 0:
                idx = line.index('}')
                rest = line[:idx].strip()
                if rest:
                    body.append(rest)
                break
            body.append(line.strip())
    return body

def parse_body(body_lines):
    externs = []
    regs = set()
    fregs = set()
    sp_size = 0
    has_ctr_fn = False
    has_ctr = False
    logic = []
    for line in body_lines:
        m = re.match(r'extern\s+(\w+(?:\s*\*)?)\s+(\w+).*?;', line)
        if m:
            externs.append(line)
            continue
        m = re.match(r'u8\s+sp\[0x([0-9A-Fa-f]+)\];', line)
        if m:
            sp_size = int(m.group(1), 16)
            continue
        m = re.match(r'u32\s+(r\d+)\s*=\s*0;$', line)
        if m:
            regs.add(m.group(1))
            continue
        # r1 stack pointer alias
        if re.match(r'u32\s+r1\s*=\s*\(u32\)sp;', line):
            regs.add('r1')
            continue
        m = re.match(r'f32\s+(f\d+)\s*=\s*0\.0f;$', line)
        if m:
            fregs.add(m.group(1))
            continue
        if re.match(r'void\s+\(\*ctr_fn\)', line):
            has_ctr_fn = True
            continue
        if re.match(r'u32\s+ctr\s*=\s*0;$', line):
            has_ctr = True
            continue
        logic.append(line)
    return externs, regs, fregs, sp_size, has_ctr_fn, has_ctr, logic

def get_used(logic_lines, all_set):
    text = '\n'.join(logic_lines)
    return {r for r in all_set if re.search(r'\b' + re.escape(r) + r'\b', text)}

def is_nop(logic_lines):
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

def rebuild(lines, start, end, fl, fe):
    sig_line = lines[fl].strip()
    m = re.match(r'^((?:void|u32|s32|u16|u8|BOOL|f32|GSThread\*|GSTask\*)\s+fn_[0-9A-Fa-f]+\s*\([^)]*\))\s*\{', sig_line)
    if not m:
        return None
    sig = m.group(1)
    body = extract_body(lines, fl, fe)
    externs, regs, fregs, sp_size, has_ctr_fn, has_ctr, logic = parse_body(body)
    ur = get_used(logic, regs)
    uf = get_used(logic, fregs)
    text = '\n'.join(logic)
    uses_sp = 'sp' in text
    uses_r1 = bool(re.search(r'\br1\b', text))
    uses_ctr_fn = 'ctr_fn' in text
    uses_ctr = bool(re.search(r'\bctr\b', text))

    if is_nop(logic):
        if 'void' in sig.split('(')[0]:
            return sig + ' {\n}\n'
        else:
            return sig + ' {\n    return 0;\n}\n'

    out = [sig + ' {\n']
    for ext in externs:
        out.append('    ' + ext + '\n')
    if sp_size > 0 and (uses_sp or uses_r1):
        out.append('    u8 sp[0x%X];\n' % sp_size)
    for reg in sorted(ur, key=lambda x: int(x[1:])):
        if reg == 'r1':
            if sp_size > 0:
                out.append('    u32 r1 = (u32)sp;\n')
            else:
                out.append('    u32 r1 = 0;\n')
        else:
            out.append('    u32 %s = 0;\n' % reg)
    for freg in sorted(uf, key=lambda x: int(x[1:])):
        out.append('    f32 %s = 0.0f;\n' % freg)
    if uses_ctr_fn:
        out.append('    void (*ctr_fn)(void) = 0;\n')
    if uses_ctr:
        out.append('    u32 ctr = 0;\n')
    if ur or uf or uses_ctr_fn or uses_ctr:
        out.append('\n')
    for line in logic:
        s = line.strip()
        if s.startswith('/*') and ('stmw' in s or 'lmw' in s):
            continue
        if re.match(r'r\d+ = \*\(u32\*\)\(sp \+ 0x[0-9A-Fa-f]+\);$', s):
            continue
        out.append('    ' + s + '\n')
    out.append('}\n')
    return ''.join(out)

def process(filepath):
    lines = read_file(filepath)
    blocks = find_blocks(lines)
    print("Found %d blocks in %s" % (len(blocks), filepath))
    c = 0
    for start, end, fl, fe in reversed(blocks):
        nf = rebuild(lines, start, end, fl, fe)
        if nf is None:
            nl = [lines[i] for i in range(start, end + 1) if not lines[i].strip().startswith('#pragma')]
            lines[start:end+1] = nl
        else:
            lines[start:end+1] = [nf, '\n']
            c += 1
    write_file(filepath, lines)
    # Verify
    remaining = sum(1 for l in lines if '#pragma optimization_level' in l)
    print("  Converted %d, remaining opt pragmas: %d" % (c, remaining))
    return c

files = [
    'src/game/colosseum_event.c',
    'src/game/gs_thread.c',
    'src/game/pokemon.c',
    'src/game/trainer.c',
    'src/game/menu/menu_middle.c',
    'src/game/ui/ui_core.c',
]
total = 0
for f in files:
    total += process(f)
print("\nTotal: %d functions converted" % total)
