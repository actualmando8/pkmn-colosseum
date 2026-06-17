#!/usr/bin/env python3
"""Deterministic integration + RE-VERIFICATION of band_wins back into canon.

Generalization of cs_integrate.py. Every band wins file
(build/band_wins/<tag>.json) records the source file it belongs to (the `_src`
key, written by band.py save). This merges all wins per source file, splices
them into a fresh copy of that canonical source, recompiles the integrated
file via a private band, and RE-MEASURES every spliced function via objdiff.

This is the GROUND-TRUTH parent gate: it trusts no agent claim. Only functions
that still measure >=100% in the freshly-compiled integrated file are kept; any
that dropped below 100% after integration are reported and DROPPED (the canonical
source is left untouched for them).

Usage:
  band.py band_integrate.py            integrate every tag in build/band_wins/
  band_integrate.py <tag> [<tag> ...]  integrate only these tags
  band_integrate.py --apply            also overwrite the canonical src in place
                                       (default: write build/band_<stem>_integrated.c
                                        only; never edits src/ without --apply)

Outputs (per source file):
  build/band_<stem>_patch.json         merged {fn: body} that re-verified 100%
  build/band_<stem>_integrated.c       canonical with verified wins spliced in
Exit 0 if every saved win re-verified (or was cleanly dropped); 1 on splice error.
"""
import json
import re
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent.parent
WINS = ROOT / "build" / "band_wins"
BUILD = ROOT / "build"
PY = sys.executable
M = 99.9999


def _scratch_json(tag, integrated_c, config_from):
    """init a private band from the integrated file, compile + measure it,
    return {fn: pct}. Resolves flags/version/target from the REAL canon source
    (config_from) — the integrated temp file's stem isn't in compile_config.json,
    so without this it would fall back to default -O4,p and falsely drop wins on
    -O4,s TUs (trainer.c, pokemon.c, ...)."""
    inttag = f"_int_{tag}"
    subprocess.run([PY, str(HERE / "band.py"), "init", inttag, str(integrated_c),
                    "--config-from", str(config_from)],
                   capture_output=True, text=True)
    r = subprocess.run([PY, str(HERE / "band.py"), "json", inttag],
                       capture_output=True, text=True)
    line = [l for l in r.stdout.splitlines() if l.strip().startswith("{")]
    if not line:
        return None, (r.stdout + r.stderr)
    return json.loads(line[-1]), None


def integrate_source(src_rel, fn_bodies, apply):
    """Splice fn_bodies into a fresh copy of src_rel, recompile, re-verify each
    fn is still >=100%. Returns (held, dropped) lists of (fn, pct)."""
    canon = ROOT / src_rel
    if not canon.exists():
        print(f"  SKIP: canonical source missing: {src_rel}")
        return [], []
    stem = Path(src_rel).stem
    patch = BUILD / f"band_{stem}_patch.json"
    integrated = BUILD / f"band_{stem}_integrated.c"

    patch.write_bytes(json.dumps(fn_bodies, indent=1).encode("utf-8"))
    r = subprocess.run([PY, str(HERE / "cs_splice.py"), str(canon), str(patch),
                        str(integrated)], capture_output=True, text=True)
    print("  " + (r.stdout.strip() or r.stderr.strip()))
    if r.returncode != 0:
        print("  SPLICE FAILED:\n" + r.stderr)
        return None, None

    newm, err = _scratch_json(stem, integrated, canon)
    if newm is None:
        print("  MEASURE FAILED:\n" + (err or ""))
        return None, None

    # FRAUD GUARD: a byte-match win must be DECOMPILED C, not inline assembly.
    # Agents have gamed the metric by hand-transcribing the .inc disassembly into
    # `asm void fn(){ stwu...; lwz...; }` blocks (or keeping the `#include .inc`
    # wrapper) to force a 100% match without producing any C. Reject any saved body
    # that is an `asm`-storage-class function or contains an inline asm{} / __asm
    # block — real C never does. (The legitimate un-decompiled baseline is also an
    # `asm` wrapper, and must never be re-saved as a "win" either.)
    ASM_FN = re.compile(r"\basm\b\s+[\w*]+\s+" + r"\w+\s*\(")   # `asm <type> fn(`
    ASM_BLOCK = re.compile(r"\basm\b\s*\{|__asm\b|#include\s+\"[^\"]*\.inc\"")
    held, dropped, rejected = [], [], []
    for fn in fn_bodies:
        body = fn_bodies[fn] or ""
        if ASM_FN.search(body) or ASM_BLOCK.search(body):
            rejected.append((fn, newm.get(fn, 0.0)))
            continue
        pct = newm.get(fn, 0.0)
        (held if pct >= M else dropped).append((fn, pct))

    if rejected:
        print(f"  !! REJECTED {len(rejected)} inline-asm/wrapper 'wins' (NOT real C — fraud):")
        for fn, pct in sorted(rejected):
            print(f"    REJECT-ASM  {fn}  {pct:.2f}%  (inline assembly, not decompilation)")
    print(f"  re-verified: {len(held)} held, {len(dropped)} dropped")
    for fn, pct in sorted(held):
        print(f"    HELD  {fn}  {pct:.2f}%")
    for fn, pct in sorted(dropped):
        print(f"    DROP  {fn}  {pct:.2f}%  (kept out of canon)")

    if dropped or rejected:
        # Re-splice with only the verified wins so the integrated file is clean
        # (excludes both sub-100 drops and rejected inline-asm/wrapper fraud).
        kept = {fn: b for fn, b in fn_bodies.items() if fn in dict(held)}
        if kept:
            patch.write_bytes(json.dumps(kept, indent=1).encode("utf-8"))
            subprocess.run([PY, str(HERE / "cs_splice.py"), str(canon),
                            str(patch), str(integrated)],
                           capture_output=True, text=True)
        else:
            integrated.write_bytes(canon.read_bytes())

    print(f"  integrated -> {integrated.relative_to(ROOT)}")
    if apply and held:
        canon.write_bytes(integrated.read_bytes())
        print(f"  APPLIED to {src_rel}")
    return held, dropped


def main():
    args = [a for a in sys.argv[1:] if a != "--apply"]
    apply = "--apply" in sys.argv[1:]
    if not WINS.exists():
        print("no build/band_wins/ — nothing to integrate.")
        return 0

    files = sorted(WINS.glob("*.json"))
    if args:
        wanted = {a if a.endswith(".json") else f"{a}.json" for a in args}
        files = [f for f in files if f.name in wanted]
    if not files:
        print("no matching band wins files.")
        return 0

    # Group wins by source file (each wins file carries its own _src key).
    by_src = {}
    for wf in files:
        try:
            d = json.loads(wf.read_text(encoding="utf-8"))
        except ValueError:
            print(f"WARN: unparseable {wf.name}; skipping")
            continue
        src_rel = d.get("_src")
        if not src_rel:
            print(f"WARN: {wf.name} has no _src key (old format?); skipping")
            continue
        bucket = by_src.setdefault(src_rel, {})
        for fn, body in d.items():
            if fn == "_src":
                continue
            bucket[fn] = body  # bands are disjoint by design; last writer wins

    rc = 0
    total_held = total_dropped = 0
    for src_rel, fn_bodies in sorted(by_src.items()):
        print(f"\n=== {src_rel}  ({len(fn_bodies)} saved win(s)) ===")
        held, dropped = integrate_source(src_rel, fn_bodies, apply)
        if held is None:
            rc = 1
            continue
        total_held += len(held)
        total_dropped += len(dropped)

    print(f"\n=== TOTAL: {total_held} held, {total_dropped} dropped "
          f"across {len(by_src)} file(s) ===")
    if not apply:
        print("(dry run — pass --apply to overwrite canonical src files)")
    return rc


if __name__ == "__main__":
    sys.exit(main())
