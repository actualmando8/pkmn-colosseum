#!/usr/bin/env python3
import re, sys

def go(lines):
    gotos, labels = [], {}
    for i, l in enumerate(lines):
        if l is None: continue
        m = re.search(r"goto\s+(L_[0-9A-Fa-f]+)\s*;", l)
        if m: gotos.append((i, m.group(1)))
        m = re.match(r"^(\s*)(L_[0-9A-Fa-f]+)\s*:\s*(.*)", l)
        if m: labels[m.group(2)] = i
    return gotos, labels

def refs(lines, n):
    return sum(1 for l in lines if l and ("goto " + n) in l)

def rm(lines, n):
    if refs(lines, n) > 0: return
    for i, l in enumerate(lines):
        if l is None: continue
        m = re.match(r"^(\s*)" + re.escape(n) + r"\s*:\s*(.*)", l)
        if m:
            c = m.group(2).strip()
            lines[i] = (m.group(1) + c + "
") if c and c != ";" else None
            break

def neg(c):
    c = c.strip()
    for o, n in [("!=","=="),("==","!="),(">=" ,"<"),("<=",">")]  :
        d = 0
        for i in range(len(c)):
            if c[i]=="(": d+=1
            elif c[i]==")": d-=1
            elif d==0 and c[i:i+len(o)]==o: return c[:i]+n+c[i+len(o):]
    for o, n in [(">","<="),("<",">=")]:
        d = 0
        for i in range(len(c)):
            if c[i]=="(": d+=1
            elif c[i]==")": d-=1
            elif d==0 and c[i]==o:
                if i>0 and c[i-1] in "!=<>": continue
                if i+1<len(c) and c[i+1] in "=": continue
                return c[:i]+n+c[i+1:]
    return "!(" + c + ")"

def bd(lines, s, e):
    d = 0
    for i in range(s+1, e):
        if lines[i] is None: continue
        for ch in lines[i]:
            if ch=="{": d+=1
            elif ch=="}": d-=1
    return d

def gi(l): return 0 if l is None else len(l)-len(l.lstrip())
