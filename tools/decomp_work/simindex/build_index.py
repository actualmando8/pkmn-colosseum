#!/usr/bin/env python3
"""Build/refresh the asm-similarity index into build/simindex/index.pkl.

Usage: python3 tools/decomp_work/simindex/build_index.py
"""

import os
import sys
import time

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import simindex_lib as lib


def _pref(name, rel):
    """Preference score for dedup: real names beat fn_XXXXXXXX/lbl_ auto
    names; per-unit asm files beat auto_* split files."""
    score = 0
    if not (name.startswith("fn_") or name.startswith("lbl_")):
        score += 2
    if not os.path.basename(rel).startswith("auto_"):
        score += 1
    return score


def ingest(corpus, asm_root, report, functions):
    """The asm tree contains overlapping coverage (auto_* whole-range split
    files AND per-unit files); dedup by address keeping the best variant."""
    best = {}  # addr -> record
    for path in lib.iter_asm_files(asm_root):
        rel = os.path.relpath(path, asm_root)
        for fn in lib.parse_dtk_asm(path):
            if not fn["insns"]:
                continue
            rec = (fn, rel)
            prev = best.get(fn["addr"])
            if prev is None or _pref(fn["name"], rel) > _pref(prev[0]["name"], prev[1]):
                best[fn["addr"]] = rec
    for addr in sorted(best):
        fn, rel = best[addr]
        norm = lib.normalize(fn["insns"], fn["labels"])
        info = report.get(fn["addr"]) if report else None
        functions.append({
            "id": len(functions),
            "corpus": corpus,
            "name": fn["name"],
            "addr": fn["addr"],
            "size": fn["size"],
            "n_insns": len(fn["insns"]),
            "asm_file": rel,
            "unit": info["unit"] if info else None,
            "src_path": info["src_path"] if info else None,
            "fuzzy": info["fuzzy"] if info else None,
            "norm": norm,
        })
    return len(best)


def main():
    t0 = time.time()
    report = lib.load_report()
    print("report.json: %d functions with match status" % len(report))

    functions = []
    n_colo = ingest("colo", lib.COLO_ASM_DIR, report, functions)
    print("colo: %d functions parsed (%.1fs)" % (n_colo, time.time() - t0))
    n_xd = ingest("xd", lib.XD_ASM_DIR, None, functions)
    print("xd:   %d functions parsed (%.1fs)" % (n_xd, time.time() - t0))

    # minhash signatures
    sigs = np.empty((len(functions), lib.N_PERMS), dtype=np.uint64)
    for f in functions:
        sigs[f["id"]] = lib.minhash_sig(lib.shingles(f["norm"]))
    print("minhash: %d sigs (%.1fs)" % (len(functions), time.time() - t0))

    # LSH buckets
    lsh = {}
    for f in functions:
        for key in lib.lsh_keys(sigs[f["id"]]):
            lsh.setdefault(key, []).append(f["id"])
    print("lsh: %d buckets (%.1fs)" % (len(lsh), time.time() - t0))

    by_name, by_addr = {}, {}
    for f in functions:
        by_name.setdefault(f["name"], []).append(f["id"])
        if f["addr"] is not None:
            by_addr.setdefault(f["addr"], []).append(f["id"])

    matched = sum(1 for f in functions
                  if f["corpus"] == "colo" and (f["fuzzy"] or 0) >= 100.0)
    no_status = sum(1 for f in functions
                    if f["corpus"] == "colo" and f["fuzzy"] is None)
    index = {
        "version": 1,
        "built_at": time.time(),
        "functions": functions,
        "sig_matrix": sigs,
        "lsh": lsh,
        "by_name": by_name,
        "by_addr": by_addr,
        "stats": {"colo": n_colo, "xd": n_xd, "colo_matched": matched,
                  "colo_no_report_status": no_status},
    }
    lib.save_index(index)
    print("saved %s (%.1f MB, %.1fs total)" % (
        lib.INDEX_PATH, os.path.getsize(lib.INDEX_PATH) / 1e6, time.time() - t0))
    print("stats:", index["stats"])


if __name__ == "__main__":
    main()
