#!/usr/bin/env python3
"""fdiff.py <fn> <unit> -- instruction diff of one function, target | ours.

Branch targets are absolute and differ between the two objects, so they are
normalised to <t>; without that every branch reads as a mismatch.
Equal instruction counts with only register-name diffs means the structure is
right and what is left is register allocation.
"""
import json,sys,subprocess,difflib,os,re
# NORMBR: branch targets are absolute and differ between the two objects
BR=re.compile(r'^(b|beq|bne|blt|bge|ble|bgt|bdnz|bdz|bso|bns)\s+0x[0-9a-f]+$')
def nb(t): return BR.sub(r'\1 <t>', t)
os.chdir(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..'))
FN=sys.argv[1]; U=sys.argv[2]
OBJ='build/GC6E01/src/'+U.replace('main/','',1)+'.o'
r=subprocess.run(['ninja',OBJ],capture_output=True,text=True)
if r.returncode!=0:
    print(r.stdout[-4000:]); print(r.stderr[-4000:]); sys.exit(1)
out=subprocess.run(['build/tools/objdiff-cli','diff','-p','.','-u',U,'-o','-','--format','json'],capture_output=True,text=True).stdout
d=json.loads(out)
def get(side):
    for x in d[side]['symbols']:
        if x['name']==FN: return [nb(i.get('instruction',{}).get('formatted','<data>')) for i in x['instructions']]
L=get('left'); R=get('right')
if R is None: print('OUR sym missing'); sys.exit(0)
sm=difflib.SequenceMatcher(None,L,R,autojunk=False)
same=sum(b.size for b in sm.get_matching_blocks())
print(f'{FN}: target {len(L)} ours {len(R)} aligned-same {same} ({100.0*same/len(L):.1f}%)')
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=='equal':
        if i2-i1>6:
            for k in range(i1,i1+2): print(f'  {k:4d}   {L[k]}')
            print(f'         ... {i2-i1-4} same ...')
            for k in range(i2-2,i2): print(f'  {k:4d}   {L[k]}')
        else:
            for k in range(i1,i2): print(f'  {k:4d}   {L[k]}')
    else:
        for k in range(max(i2-i1,j2-j1)):
            a=L[i1+k] if i1+k<i2 else ''; b=R[j1+k] if j1+k<j2 else ''
            print(f'  {i1+k:4d} | {a:<42} {b}')
