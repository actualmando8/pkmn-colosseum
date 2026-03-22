#!/usr/bin/env python3
"""Test compilation of all files and report broken ones."""
import re
import os
import subprocess
import sys

def main():
    with open('build.ninja', 'r') as f:
        content = f.read()

    rules = re.findall(r'build\s+(\S+\.o)\s*:\s*(cc_GC_\w+)\s+(\S+\.c)', content)
    compiler_map = {}
    for obj, rule, src in rules:
        if '1_2_5n' in rule:
            compiler_map[src] = '1.2.5n'
        elif '1_3' in rule:
            compiler_map[src] = '1.3'

    os.makedirs('build/test', exist_ok=True)

    broken = []
    ok = []
    for src in sorted(compiler_map.keys()):
        ver = compiler_map[src]
        outf = os.path.join('build', 'test', os.path.basename(src).replace('.c', '.o'))
        exe = os.path.join('tools', 'mwcc_compiler', 'GC', ver, 'mwcceppc.exe')
        cmd = f'{exe} -c -O4,p -nodefaults -proc gekko -fp hard -Cpp_exceptions off -enum int -warn off -i include -o {outf} {src}'
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=60, shell=True)
        if result.returncode != 0:
            broken.append(src)
        else:
            ok.append(src)

    print(f'{len(ok)} OK, {len(broken)} BROKEN out of {len(compiler_map)} total')

    if '--only-broken' in sys.argv:
        for f in sorted(broken):
            print(f)
    else:
        for f in sorted(broken):
            print(f'BROKEN: {f}')
        for f in sorted(ok):
            print(f'OK: {f}')

if __name__ == '__main__':
    main()
