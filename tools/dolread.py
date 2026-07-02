#!/usr/bin/env python3
"""Read the retail DOL directly: strings, words, hexdumps, pointer tables.

The DOL header is 18 section descriptors (7 text + 11 data): file
offsets at 0x00, load addresses at 0x48, sizes at 0x90, big-endian u32
each. Mapping a virtual address to its file offset lets us read any
byte the retail image loads, without symbols or objdiff.

Validated evidence workflow (floorRead family, commit 345de554):
  1. `findptr` known member addresses -> data-section hits reveal a
     registration/dispatch table and its stride,
  2. `table` decodes the entries with symbols.txt annotation,
  3. the table's pairing/order is naming proof stronger than strings
     or XD structural ports (it overturned a string-only assignment).

Usage:
  tools/dolread.py str  0x80272200            # C string at address
  tools/dolread.py hex  0x8036C2A0 64         # hexdump N bytes
  tools/dolread.py u32  0x8036C2A0 16         # N big-endian words
  tools/dolread.py table 0x8036C2A0 --stride 16 --count 20
  tools/dolread.py findptr 0x8011432C 0x80114948 ...   # hunt tables
  tools/dolread.py sweep                      # find ALL candidate tables
  tools/dolread.py sections                   # list DOL sections
Options: --dol PATH (default orig/GC6E01/sys/main.dol),
         --symbols PATH (default config/GC6E01/symbols.txt).
"""
import argparse
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


class Dol:
    def __init__(self, path):
        self.raw = Path(path).read_bytes()
        offs = struct.unpack(">18I", self.raw[0x00:0x48])
        addrs = struct.unpack(">18I", self.raw[0x48:0x90])
        sizes = struct.unpack(">18I", self.raw[0x90:0xD8])
        self.sections = [
            (o, a, s, "text" if i < 7 else "data")
            for i, (o, a, s) in enumerate(zip(offs, addrs, sizes))
            if s > 0
        ]

    def vmap(self, addr):
        for off, base, size, _ in self.sections:
            if base <= addr < base + size:
                return off + (addr - base)
        return None

    def read(self, addr, n):
        off = self.vmap(addr)
        if off is None:
            raise SystemExit(f"0x{addr:08X} is not in any DOL section")
        return self.raw[off:off + n]

    def cstring(self, addr, limit=256):
        b = self.read(addr, limit).split(b"\0")[0]
        try:
            return b.decode("ascii")
        except UnicodeDecodeError:
            return b.decode("shift_jis", errors="backslashreplace")

    def find_words(self, values, kinds=("data",)):
        """Every (address, matched-value) where a section holds one of
        `values` as an aligned big-endian u32."""
        targets = {struct.pack(">I", v): v for v in values}
        hits = []
        for off, base, size, kind in self.sections:
            if kind not in kinds:
                continue
            blob = self.raw[off:off + size]
            for pat, val in targets.items():
                start = 0
                while True:
                    i = blob.find(pat, start)
                    if i < 0:
                        break
                    if i % 4 == 0:
                        hits.append((base + i, val))
                    start = i + 1
        return sorted(hits)


def load_symbols(path):
    by_addr = {}
    pat = re.compile(r"(\w+) = \.(\w+):(0x[0-9A-Fa-f]+);")
    for line in open(path):
        m = pat.match(line)
        if m:
            by_addr[int(m.group(3), 16)] = m.group(1)
    return by_addr


def annotate(word, dol, syms):
    if word in syms:
        return syms[word]
    for _, base, size, kind in dol.sections:
        if base <= word < base + size:
            return f"0x{word:08X}<{kind}>"
    return f"0x{word:08X}"


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("cmd", choices=["str", "hex", "u32", "table", "findptr",
                                    "sweep", "sections"])
    ap.add_argument("--min-run", type=int, default=5,
                    help="sweep: minimum members per table")
    ap.add_argument("args", nargs="*")
    ap.add_argument("--dol", default=str(ROOT / "orig/GC6E01/sys/main.dol"))
    ap.add_argument("--symbols", default=str(ROOT / "config/GC6E01/symbols.txt"))
    ap.add_argument("--stride", type=int, default=16)
    ap.add_argument("--count", type=int, default=16)
    a = ap.parse_args()

    dol = Dol(a.dol)
    if a.cmd == "sections":
        for off, base, size, kind in dol.sections:
            print(f"{kind:>4} vaddr 0x{base:08X}..0x{base + size:08X} "
                  f"file 0x{off:06X} size 0x{size:X}")
        return
    syms = load_symbols(a.symbols) if Path(a.symbols).exists() else {}
    addrs = [int(x, 16) for x in a.args if x.lower().startswith("0x")]

    if a.cmd == "str":
        for addr in addrs:
            print(f"0x{addr:08X}: {dol.cstring(addr)!r}")
    elif a.cmd == "hex":
        n = int(a.args[1], 0) if len(a.args) > 1 else 64
        b = dol.read(addrs[0], n)
        for i in range(0, len(b), 16):
            row = b[i:i + 16]
            hexs = " ".join(f"{c:02X}" for c in row)
            print(f"0x{addrs[0] + i:08X}: {hexs}")
    elif a.cmd == "u32":
        n = int(a.args[1]) if len(a.args) > 1 else 8
        b = dol.read(addrs[0], n * 4)
        for i in range(n):
            w = struct.unpack(">I", b[i * 4:i * 4 + 4])[0]
            print(f"0x{addrs[0] + i * 4:08X}: {annotate(w, dol, syms)}")
    elif a.cmd == "table":
        words = a.stride // 4
        for i in range(a.count):
            base = addrs[0] + i * a.stride
            b = dol.read(base, a.stride)
            ws = struct.unpack(f">{words}I", b)
            cells = ", ".join(annotate(w, dol, syms) for w in ws)
            print(f"{i:>3} @0x{base:08X}: [{cells}]")
    elif a.cmd == "sweep":
        # Find every strided run of function-start pointers in the data
        # sections: dispatch tables, handler registries, vtables. Runs
        # are claimed smallest-stride-first so a dense array is not also
        # reported at every multiple of its stride.
        fn_pat = re.compile(
            r"(\w+) = \.text:(0x[0-9A-Fa-f]+); // type:function")
        fn_starts = {}
        for line in open(a.symbols):
            m = fn_pat.match(line)
            if m:
                fn_starts[int(m.group(2), 16)] = m.group(1)
        hits = {}
        for off, base, size, kind in dol.sections:
            if kind != "data":
                continue
            for i in range(0, size - 3, 4):
                w = struct.unpack_from(">I", dol.raw[off + i:off + i + 4])[0]
                if w in fn_starts:
                    hits[base + i] = w
        posset = set(hits)
        consumed = set()
        tables = []
        for stride in (4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 48, 64):
            for p in sorted(hits):
                if p in consumed:
                    continue
                if (p - stride) in posset and (p - stride) not in consumed:
                    continue  # not the start of a run at this stride
                n, q = 0, p
                while q in posset and q not in consumed:
                    n += 1
                    q += stride
                if n < a.min_run:
                    continue
                members = [fn_starts[hits[p + i * stride]] for i in range(n)]
                named = [m for m in members if not m.startswith(("fn_", "lbl_"))]
                for i in range(n):
                    consumed.add(p + i * stride)
                tables.append((p, stride, n, len(named), named[:3],
                               [m for m in members if m.startswith("fn_")][:3]))
        tables.sort(key=lambda t: (-(t[3] > 0 and t[3] < t[2]), t[3] == 0,
                                   -(t[2] - t[3])))
        print(f"{len(tables)} candidate tables (>= {a.min_run} members); "
              f"anchored-and-incomplete first")
        for p, stride, n, nn, sn, su in tables:
            print(f"0x{p:08X} stride {stride:>2} count {n:>4} named {nn:>3} "
                  f"| {','.join(sn)} | {','.join(su)}")
    elif a.cmd == "findptr":
        hits = dol.find_words(addrs)
        for addr, val in hits:
            print(f"0x{addr:08X} -> {annotate(val, dol, syms)}")
        # stride hint: spacing between consecutive hits on distinct values
        if len(hits) > 1:
            gaps = [b[0] - a_[0] for a_, b in zip(hits, hits[1:])
                    if a_[1] != b[1]]
            if gaps:
                from collections import Counter
                top = Counter(gaps).most_common(3)
                print("stride hint (gap: occurrences):",
                      ", ".join(f"0x{g:X}: {c}" for g, c in top))


if __name__ == "__main__":
    main()
