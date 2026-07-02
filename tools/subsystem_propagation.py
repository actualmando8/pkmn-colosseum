#!/usr/bin/env python3
"""Propagate subsystem labels across all .text functions.

Seeds labels from verified named-symbol prefixes, then propagates over
two evidence channels:
  1. address locality (TUs are contiguous, so address neighbors usually
     share a subsystem), and
  2. the direct call graph (bl edges, both directions).

Output: config/GC6E01/subsystem_map.json with one record per function
{name, addr, size, label, confidence, evidence} plus contiguous label
runs usable for assigning auto blocks in splits.txt.

Labels are ATTRIBUTION hints (which library/subsystem owns the code),
not semantic names. Current source, symbols, splits, and asm outrank
this map (audit 2026-07-01, docs/fable5_audit_pass2_symbol_name_audit.md).
"""
import json
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ASM = ROOT / "build" / "GC6E01" / "asm"
SYMBOLS = ROOT / "config" / "GC6E01" / "symbols.txt"
OUT = ROOT / "config" / "GC6E01" / "subsystem_map.json"

# Verified name-prefix seeds (longest prefix wins). From the pass-2
# region strata + pass-1/3 discoveries; all prefixes below were
# asm-verified at least once this campaign.
PREFIX_SEEDS = [
    ("PPC", "dolphin-os"), ("OS", "dolphin-os"), ("__OS", "dolphin-os"),
    ("DC", "dolphin-os"), ("IC", "dolphin-os"), ("LC", "dolphin-os"),
    ("L2", "dolphin-os"), ("SystemCallVector", "dolphin-os"),
    ("EXI", "dolphin-exi"), ("__EXI", "dolphin-exi"),
    ("DVD", "dolphin-dvd"), ("__DVD", "dolphin-dvd"), ("__fst", "dolphin-dvd"),
    ("AI", "dolphin-ai"), ("__AI", "dolphin-ai"),
    ("AR", "dolphin-ar"), ("__AR", "dolphin-ar"), ("ARQ", "dolphin-ar"),
    ("DSP", "dolphin-dsp"), ("__DSP", "dolphin-dsp"),
    ("CARD", "dolphin-card"), ("__CARD", "dolphin-card"),
    ("GX", "dolphin-gx"), ("__GX", "dolphin-gx"),
    ("SI", "dolphin-si"), ("__SI", "dolphin-si"),
    ("VI", "dolphin-vi"), ("__VI", "dolphin-vi"),
    ("PAD", "dolphin-pad"), ("__PAD", "dolphin-pad"), ("SPEC", "dolphin-pad"),
    ("UpdateOrigin", "dolphin-pad"),
    ("MTX", "dolphin-mtx"), ("PSMTX", "dolphin-mtx"), ("C_MTX", "dolphin-mtx"),
    ("VEC", "dolphin-mtx"), ("QUAT", "dolphin-mtx"),
    ("THP", "thp"), ("__THP", "thp"),
    ("TRK", "trk"), ("udp_cc", "trk"), ("ddh_cc", "trk"), ("gdev_cc", "trk"),
    ("__ieee754", "libm"), ("__kernel", "libm"), ("frexp", "libm"),
    ("ldexp", "libm"), ("scalbn", "libm"), ("fabs", "libm"),
    # MusyX runtime (verified pass 1)
    ("seq", "musyx"), ("synth", "musyx"), ("snd", "musyx"), ("data", "musyx"),
    ("mcmd", "musyx"), ("voice", "musyx"), ("adsr", "musyx"),
    ("salI", "musyx"), ("salA", "musyx"), ("salC", "musyx"),
    ("inpGet", "musyx"), ("inpSet", "musyx"), ("inpReset", "musyx"),
    ("aram", "musyx"), ("Reverb", "musyx"),
    ("curvecmp", "musyx"), ("layercmp", "musyx"), ("maccmp", "musyx"),
    ("fxcmp", "musyx"), ("_GetInputValue", "musyx"),
    # sysdolphin
    ("HSD_", "hsd"), ("hsd", "hsd"), ("DObj", "hsd"), ("LObj", "hsd"),
    ("WObj", "hsd"), ("CObj", "hsd"), ("PObj", "hsd"), ("RObj", "hsd"),
    ("MObj", "hsd"), ("FObj", "hsd"), ("TObj", "hsd"), ("Obj", "hsd"),
    ("__assert", "hsd"),
    # GS engine + game
    ("GS", "gs-engine"), ("_matGS", "gs-engine"),
    ("getCp", "gs-engine"), ("cameraSet", "gs-engine"),
    ("threadLoad", "gs-engine"), ("threadSave", "gs-engine"),
    ("threadExecute", "gs-engine"), ("_threadSwitch", "gs-engine"),
    ("ps", "particle"), ("_ps", "particle"),
    ("generateParticle", "particle"),
    ("menu", "menu"), ("dbm", "menu"), ("dbgMenu", "menu"), ("_menu", "menu"),
    ("_dbgMenu", "menu"), ("_AGB", "menu"), ("gba", "menu"),
    ("fight", "fight"), ("_fight", "fight"),
    ("people", "people"), ("item", "item"), ("hpRecover", "item"),
    ("itemParam", "item"), ("testEvolution", "menu"),
    ("floor", "field"), ("hero", "field"), ("gamedata", "field"),
    ("waza", "waza"), ("fade", "fade"),
    ("kouka", "effect"), ("_kouka", "effect"), ("leaffx", "effect"),
    ("electronStart", "effect"), ("filterStart", "effect"),
    ("surfEffect", "effect"), ("seaEffect", "effect"), ("tracefx", "effect"),
    ("_lightning", "effect"), ("_leaffx", "effect"), ("patchiru", "effect"),
    ("pcbox", "savedata"), ("charName", "savedata"), ("tableRes", "savedata"),
    ("menuCardE", "menu"),
]

# Range seeds: verified library extents (stronger than prefixes inside them).
RANGE_SEEDS = [
    (0x80146E88, 0x80165400, "musyx"),       # pass-1 discovery
    (0x80140588, 0x80146E88, "item"),        # people_data + item-use logic
    (0x80180C78, 0x8018FE30, "people"),      # verified real NPC system
    (0x801F1460, 0x80265EC4, "fight"),       # fight engine span (pass 3)
    (0x80097FFC, 0x800C1960, "dolphin-sdk"), # PPC..TRK strata
    (0x800C3BB8, 0x800C4500, "trk"),         # udp/ddh/gdev drivers
    (0x800CABB0, 0x800CD85C, "libm"),
    (0x800CFA60, 0x800D0900, "dolphin-si"),
    (0x800D172C, 0x8010FAF4, "gs-engine"),   # GS camera/gfx/model strata
    (0x80113778, 0x80130CE0, "field"),       # floor/hero field-world
    (0x80168C64, 0x80177830, "particle"),    # ps* strata
    (0x80177830, 0x80180C78, "gs-engine"),   # GSscene + gsdvd region
    (0x80191628, 0x801C0F20, "hsd"),
    (0x801C6274, 0x801C71B0, "fade"),
    (0x801D7E58, 0x801DBB10, "waza"),
    (0x801E57D0, 0x801EB644, "thp"),
]


def parse_symbols():
    fns = []
    pat = re.compile(
        r"(\w+) = \.text:(0x[0-9A-Fa-f]+); // type:function size:(0x[0-9A-Fa-f]+)")
    for line in open(SYMBOLS):
        m = pat.match(line)
        if m:
            fns.append({"name": m.group(1), "addr": int(m.group(2), 16),
                        "size": int(m.group(3), 16)})
    fns.sort(key=lambda f: f["addr"])
    return fns


def seed_label(name, addr):
    for lo, hi, label in RANGE_SEEDS:
        if lo <= addr < hi:
            return label, "range"
    if name.startswith(("fn_", "lbl_")):
        return None, None
    best = None
    for prefix, label in PREFIX_SEEDS:
        if name.lstrip("_").startswith(prefix.lstrip("_")) or name.startswith(prefix):
            if best is None or len(prefix) > len(best[0]):
                best = (prefix, label)
    return (best[1], "prefix") if best else (None, None)


def parse_call_graph():
    calls = defaultdict(set)
    cur = None
    fn_re = re.compile(r"^\.fn (\w+)")
    bl_re = re.compile(r"\tbl (\w+)")
    for path in ASM.rglob("*.s"):
        for line in open(path, errors="replace"):
            m = fn_re.match(line)
            if m:
                cur = m.group(1)
                continue
            if cur and "\tbl " in line:
                m = bl_re.search(line)
                if m:
                    calls[cur].add(m.group(1))
    return calls


def main():
    fns = parse_symbols()
    byname = {f["name"]: f for f in fns}
    labels, sources = {}, {}
    for f in fns:
        lab, src = seed_label(f["name"], f["addr"])
        if lab:
            labels[f["name"]] = lab
            sources[f["name"]] = src
    print(f"{len(fns)} functions, {len(labels)} seeded", file=sys.stderr)

    calls = parse_call_graph()
    callers = defaultdict(set)
    for src_fn, tgts in calls.items():
        for t in tgts:
            callers[t].add(src_fn)

    # Iterative propagation: address neighbors (weight 2) + call edges (1).
    order = [f["name"] for f in fns]
    idx = {n: i for i, n in enumerate(order)}
    conf = {n: (3.0 if sources.get(n) == "range" else 2.0)
            for n in labels}
    for it in range(6):
        changed = 0
        for f in fns:
            n = f["name"]
            if sources.get(n) in ("range", "prefix"):
                continue
            votes = Counter()
            i = idx[n]
            for j in (i - 1, i + 1):
                if 0 <= j < len(order):
                    nb = order[j]
                    if nb in labels:
                        votes[labels[nb]] += 2.0 * min(conf.get(nb, 1), 2)
            for nb in calls.get(n, set()) | callers.get(n, set()):
                if nb in labels:
                    votes[labels[nb]] += 1.0 * min(conf.get(nb, 1), 2)
            if votes:
                top, w = votes.most_common(1)[0]
                if labels.get(n) != top:
                    changed += 1
                labels[n] = top
                conf[n] = min(w / 4.0, 1.9)
        print(f"iter {it}: {changed} changed, {len(labels)} labeled",
              file=sys.stderr)
        if changed == 0:
            break

    records = []
    for f in fns:
        n = f["name"]
        records.append({
            "name": n, "addr": f"0x{f['addr']:08X}", "size": f["size"],
            "label": labels.get(n), "confidence": round(conf.get(n, 0), 2),
            "seed": sources.get(n),
        })
    # contiguous label runs (for auto-block assignment)
    runs = []
    for r in records:
        if runs and runs[-1]["label"] == r["label"]:
            runs[-1]["end"] = hex(int(r["addr"], 16) + r["size"])
            runs[-1]["count"] += 1
        else:
            runs.append({"label": r["label"], "start": r["addr"],
                         "end": hex(int(r["addr"], 16) + r["size"]),
                         "count": 1})
    json.dump({"functions": records, "runs": runs},
              open(OUT, "w"), indent=0)
    labeled = sum(1 for r in records if r["label"])
    print(f"labeled {labeled}/{len(records)} "
          f"({labeled/len(records)*100:.1f}%); {len(runs)} runs -> {OUT}",
          file=sys.stderr)


if __name__ == "__main__":
    main()
