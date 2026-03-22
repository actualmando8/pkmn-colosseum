#!/usr/bin/env python3
"""Convert goto patterns using else-conversion and if-neg-depth patterns."""
import re, os, subprocess, sys, time

def count_gotos(text): return len(re.findall(r'\bgoto\s+\w+\s*;', text))
def test_compile(src, ver):
    os.makedirs('build/test', exist_ok=True)
    outf = os.path.join('build', 'test', os.path.basename(src).replace('.c', '.o'))
    exe = os.path.join('tools', 'mwcc_compiler', 'GC', ver, 'mwcceppc.exe')
    cmd = f'{exe} -c -O4,p -nodefaults -proc gekko -fp hard -Cpp_exceptions off -enum int -warn off -i include -o {outf} {src}'
    return subprocess.run(cmd, capture_output=True, text=True, timeout=120, shell=True).returncode == 0
def is_label(line):
    s = line.strip()
    m = re.match(r'^(\w+)\s*:\s*;?\s*$', s)
    return m.group(1) if m and not s.startswith('default') and not s.startswith('case') else None
def extract_goto(line):
    s = line.strip()
    m = re.match(r'^goto\s+(\w+)\s*;$', s)
    if m: return (None, m.group(1))
    m = re.match(r'^if\s*\((.+?)\)\s+goto\s+(\w+)\s*;$', s)
    if m: return (m.group(1), m.group(2))
    return None
def negate(cond):
    cond = cond.strip()
    if cond.startswith('!(') and cond.endswith(')'): return cond[2:-1]
    ops = {'!=':'==','==':'!=','>=':'<','<=':'>','>':'<=','<':'>='}
    m = re.match(r'^(.*?)\s*(!=|==|>=|<=|>(?!=)|<(?!=))\s*(.*)$', cond)
    if m:
        l, op, r = m.groups()
        if op in ops: return f'{l} {ops[op]} {r}'
    return f'!({cond})'
def find_label(lines, label, start=0, end=None):
    if end is None: end = len(lines)
    t = label + ':'
    for i in range(start, end):
        s = lines[i].strip()
        if s == t or s == t + ' ;' or s == t + ';': return i
    return -1
def label_use_count(lines, label): return sum(1 for l in lines if f'goto {label}' in l)
def has_ext_labels(lines, start, end):
    for i in range(start, end):
        lbl = is_label(lines[i])
        if lbl:
            for j, l in enumerate(lines):
                if (j < start or j >= end) and f'goto {lbl}' in l: return True
    return False
def get_indent(line): return line[:len(line) - len(line.lstrip())] if line.strip() else ''
def write_file(fp, text):
    for a in range(3):
        try:
            with open(fp, 'w', newline='') as f: f.write(text); return
        except:
            if a < 2: time.sleep(1)
            else: raise

def try_else(lines, skip):
    for i in range(len(lines)):
        g = extract_goto(lines[i])
        if not g or g[0] is not None: continue
        lbl = g[1]; tid = f'e:{i}:{lbl}'
        if tid in skip: continue
        li = find_label(lines, lbl, i+1)
        if li < 0 or li <= i+1: continue
        j = i+1
        while j < li and not lines[j].strip(): j += 1
        if j >= li or lines[j].strip() != '}': continue
        ec = lines[j+1:li]
        if has_ext_labels(lines, j+1, li): continue
        if sum(l.count('{')-l.count('}') for l in ec) != 0: continue
        ind = get_indent(lines[j]); new = list(lines); new[i] = ''
        new[j] = f'{ind}}} else {{'
        for k in range(j+1, li):
            if lines[k].strip(): new[k] = '    ' + lines[k]
        if label_use_count(new, lbl) == 0: new[li] = f'{ind}}}'
        else: new.insert(li, f'{ind}}}')
        return new, tid
    return None

def try_ifneg(lines, skip):
    for i in range(len(lines)):
        g = extract_goto(lines[i])
        if not g or g[0] is None: continue
        cond, lbl = g; tid = f'n:{i}:{lbl}'
        if tid in skip: continue
        li = find_label(lines, lbl, i+1)
        if li < 0 or li <= i+1: continue
        d = 0; cb = -1
        for k in range(i+1, li):
            d += lines[k].count('{')-lines[k].count('}')
            if d == -1 and lines[k].strip() == '}': cb = k; break
        if cb < 0: continue
        code = lines[i+1:cb]
        if has_ext_labels(lines, i+1, cb): continue
        if sum(l.count('{')-l.count('}') for l in code) != 0: continue
        rest = lines[cb+1:li]
        if sum(l.count('{')-l.count('}') for l in rest) != 0: continue
        if has_ext_labels(lines, cb+1, li): continue
        ind = get_indent(lines[i]); new = list(lines)
        repl = [f'{ind}if ({negate(cond)}) {{']
        for cl in code: repl.append(('    '+cl) if cl.strip() else cl)
        repl.append(f'{ind}}}')
        if label_use_count(lines, lbl) <= 1 and not rest: new[i:li+1] = repl
        else: new[i:cb+1] = repl
        return new, tid
    return None

def cleanup_labels(lines):
    ch = True
    while ch:
        ch = False; new = []
        for l in lines:
            lb = is_label(l)
            if lb and label_use_count(lines, lb) == 0: ch = True; continue
            new.append(l)
        lines = new
    return lines

def process_file(fp, ver):
    with open(fp, 'r', newline='') as f: orig = f.read()
    le = '\r\n' if '\r\n' in orig else '\n'
    lines = orig.replace('\r\n', '\n').split('\n')
    tot = 0; skip = set()
    for _ in range(3000):
        applied = False
        for tfn in [try_else, try_ifneg]:
            r = tfn(lines, skip)
            if not r: continue
            nl, tid = r; nt = '\n'.join(nl)
            oc = count_gotos('\n'.join(lines)); nc = count_gotos(nt)
            if nc >= oc: skip.add(tid); continue
            write_file(fp, nt.replace('\n', le))
            if test_compile(fp, ver):
                tot += oc-nc; lines = nl; skip.clear(); applied = True; break
            else:
                write_file(fp, '\n'.join(lines).replace('\n', le)); skip.add(tid)
        if not applied: break
    lines = cleanup_labels(lines)
    final = '\n'.join(lines).replace('\n', le)
    write_file(fp, final)
    if not test_compile(fp, ver): write_file(fp, orig); return 0
    return tot

def main():
    with open('build.ninja') as f: content = f.read()
    cmap = {}
    for obj, rule, src in re.findall(r'build\s+(\S+\.o)\s*:\s*(cc_GC_\w+)\s+(\S+\.c)', content):
        cmap[src] = '1.2.5n' if '1_2_5n' in rule else '1.3'
    BROKEN = {'src/game/battle/battle_logic.c','src/game/gba/gba_conv.c','src/game/gs_field_world.c',
              'src/game/menu/menu_common_ext.c','src/game/menu/menu_middle.c','src/game/menu/menu_tool.c','src/game/ui/ui_core.c'}
    targets = sys.argv[1:] if len(sys.argv) > 1 else sorted(
        [os.path.join(r,f).replace(os.sep,'/') for r,d,fs in os.walk('src') for f in fs
         if f.endswith('.c') and os.path.join(r,f).replace(os.sep,'/') not in BROKEN
         and count_gotos(open(os.path.join(r,f),errors='ignore').read()) > 0
         and os.path.join(r,f).replace(os.sep,'/') in cmap],
        key=lambda fp: count_gotos(open(fp,errors='ignore').read()))
    gt = 0
    for fp in targets:
        ver = cmap.get(fp)
        if not ver or not test_compile(fp, ver): continue
        with open(fp,errors='ignore') as f: b = count_gotos(f.read())
        rm = process_file(fp, ver)
        with open(fp,errors='ignore') as f: a = count_gotos(f.read())
        if rm > 0: gt += rm; print(f"OK  {fp}: {b} -> {a} (-{rm}) [total: {gt}]", flush=True)
        else: print(f"---  {fp}: {b}", flush=True)
    print(f"\nGrand total removed: {gt}", flush=True)

if __name__ == '__main__': main()
