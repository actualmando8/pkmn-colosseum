#!/usr/bin/env python3
"""autorewrite.py - zero-token mechanical C-transform sweeper.

Stage 2.5 of the pipeline (see docs/decomp_notes/WORKFLOW.md). Consumes
diffclass's AUTO classifications and applies the *targeted* C-source
transform for that class to the single function, recompiles, and measures.
Keeps the transform only if that function's match% strictly improves;
otherwise reverts. No LLM.

Where automatch sweeps pragmas, autorewrite sweeps source rewrites that
this session proved repeatedly land matches but which an LLM otherwise
rediscovers by trial-and-error (~5-30K tokens each):

  signed-compare    flip *(u32*)<->*(s32*) (and u16/u8) at the deref site
  redundant-extend  collapse (s16)(s32)x -> (s16)x  (and s8 / short/int)

Each transform is applied only within the target function's line span, so
neighbouring functions are never touched. Whole-function application is
tried first; if that regresses or no-ops, a per-occurrence search flips
one site at a time.

Usage:
    python tools/autorewrite.py src/game/gs_render.c
    python tools/autorewrite.py src/game/gs_render.c --band 90 99.99
    python tools/autorewrite.py src/game/gs_render.c --symbol fn_800D56C0
    python tools/autorewrite.py src/game/gs_render.c --apply --report r.md
"""

import argparse
import re
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import automatch  # noqa: E402  (find_fn_def, measure, matched_count)
import diffclass  # noqa: E402  (fetch, classify)

# Each transform: (label, function(body_text) -> new_body_text). They are
# pure text substitutions scoped to one function's span.
PTR_FLIPS = [
    ("*(u32*)", "*(s32*)"), ("*(s32*)", "*(u32*)"),
    ("*(u16*)", "*(s16*)"), ("*(s16*)", "*(u16*)"),
    ("*(u8*)", "*(s8*)"), ("*(s8*)", "*(u8*)"),
]

EXTEND_COLLAPSE = [
    (re.compile(r"\(s16\)\s*\(s32\)"), "(s16)"),
    (re.compile(r"\(s16\)\s*\(u32\)"), "(s16)"),
    (re.compile(r"\(s8\)\s*\(s32\)"), "(s8)"),
    (re.compile(r"\(s8\)\s*\(u32\)"), "(s8)"),
    (re.compile(r"\(short\)\s*\(int\)"), "(short)"),
    (re.compile(r"\(char\)\s*\(int\)"), "(char)"),
]


def variants_for(category, body):
    """Yield (label, new_body) candidates for a diffclass category."""
    if category == "signed-compare":
        # whole-function flips first
        for a, b in PTR_FLIPS:
            if a in body:
                yield (f"all {a}->{b}", body.replace(a, b))
        # then one-site-at-a-time (handles mixed signedness in one fn)
        for a, b in PTR_FLIPS:
            n = body.count(a)
            if n > 1:
                for i in range(n):
                    yield (f"{a}->{b} #site{i+1}",
                           _replace_nth(body, a, b, i))
    elif category == "redundant-extend":
        for rx, repl in EXTEND_COLLAPSE:
            if rx.search(body):
                yield (f"collapse {rx.pattern}", rx.sub(repl, body))


def _replace_nth(text, old, new, n):
    """Replace only the n-th (0-based) occurrence of old with new."""
    idx = -1
    for _ in range(n + 1):
        idx = text.find(old, idx + 1)
        if idx == -1:
            return text
    return text[:idx] + new + text[idx + len(old):]


def span_text(lines, sig_idx, close_idx):
    return "".join(lines[sig_idx:close_idx + 1])


def splice(lines, sig_idx, close_idx, new_text):
    out = list(lines[:sig_idx])
    out += new_text.splitlines(keepends=True)
    out += lines[close_idx + 1:]
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("source")
    ap.add_argument("--band", nargs=2, type=float, default=[85.0, 99.99],
                    metavar=("LO", "HI"))
    ap.add_argument("--symbol")
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("--report")
    args = ap.parse_args()

    src = Path(args.source)
    if not src.is_absolute():
        src = ROOT / src
    original = automatch.read_src(src)

    print(f"[autorewrite] baseline compile of {src.name} ...")
    base = automatch.measure(src, None)
    if base is None:
        sys.exit("baseline compile failed")
    base_matched = automatch.matched_count(base)
    print(f"[autorewrite] baseline {base_matched}/{len(base)} @ 100%")

    # diffclass on the freshly-built baseline .o
    j = diffclass.fetch(src, compile_first=False)
    left = {s["name"]: s for s in j["left"]["symbols"]
            if s.get("kind") == "SYMBOL_FUNCTION"}
    right = {s["name"]: s for s in j["right"]["symbols"]
             if s.get("kind") == "SYMBOL_FUNCTION"}

    lo, hi = args.band
    targets = []
    for name, rs in right.items():
        if not name.startswith("fn_"):
            continue
        pct = rs.get("match_percent", 0.0)
        if args.symbol:
            if name != args.symbol:
                continue
        elif not (lo <= pct < hi):
            continue
        cat, act, _ = diffclass.classify(
            left.get(name, {}).get("instructions", []),
            rs.get("instructions", []))
        if cat in ("signed-compare", "redundant-extend"):
            targets.append((pct, name, cat))
    targets.sort(key=lambda t: -t[0])
    print(f"[autorewrite] {len(targets)} rewrite-class targets "
          f"(signed-compare / redundant-extend)")

    lines = original.splitlines(keepends=True)
    results = []
    t0 = time.time()

    for idx, (b0, name, cat) in enumerate(targets, 1):
        loc = automatch.find_fn_def(lines, name)
        if loc is None:
            print(f"  [{idx}/{len(targets)}] {name} -- no C body, skip")
            continue
        si, ci = loc
        body = span_text(lines, si, ci)
        best_pct, best_label, best_text = b0, None, None

        for label, new_body in variants_for(cat, body):
            if new_body == body:
                continue
            trial = splice(lines, si, ci, new_body)
            automatch.write_src(src, "".join(trial))
            m = automatch.measure(src, [name])
            if m is None:
                continue
            pct = m.get(name, 0.0)
            if pct > best_pct + 1e-6:
                best_pct, best_label, best_text = pct, label, new_body
            if best_pct >= 100.0:
                break

        automatch.write_src(src, original)  # restore pristine
        tag = ("=100" if best_pct >= 100 else f"+{best_pct - b0:.2f}") \
            if best_label else "no change"
        print(f"  [{idx}/{len(targets)}] {name} [{cat}] {b0:.2f}% -> "
              f"{best_pct:.2f}%  ({tag})"
              + (f"  via {best_label}" if best_label else ""))
        results.append((name, cat, b0, best_pct, best_label, best_text,
                        si, ci))

    if args.apply:
        wins = [r for r in results if r[4] and r[3] > r[2] + 1e-6]
        if wins:
            cur = original
            for name, cat, b0, bp, lbl, txt, si, ci in wins:
                ls = cur.splitlines(keepends=True)
                loc = automatch.find_fn_def(ls, name)
                if loc is None:
                    continue
                s2, c2 = loc
                cur = "".join(splice(ls, s2, c2, txt))
            automatch.write_src(src, cur)
            final = automatch.measure(src, None)
            if final is None or automatch.matched_count(final) < base_matched:
                automatch.write_src(src, original)
                print("[autorewrite] APPLY REVERTED -- net regression")
            else:
                print(f"[autorewrite] APPLIED {len(wins)} wins. "
                      f"matched {base_matched} -> "
                      f"{automatch.matched_count(final)}")
        else:
            print("[autorewrite] no improvements to apply")

    dt = time.time() - t0
    solved = [r for r in results if r[3] >= 100.0]
    improved = [r for r in results if r[4] and r[3] > r[2] + 1e-6]
    print(f"\n[autorewrite] done in {dt:.0f}s -- {len(solved)} reached "
          f"100%, {len(improved)} improved, "
          f"{len(targets) - len(improved)} unchanged")

    if args.report:
        with open(args.report, "w", encoding="utf-8") as f:
            f.write(f"# autorewrite report: {src.name}\n\n")
            f.write(f"baseline {base_matched}/{len(base)}, "
                    f"{len(targets)} targets, {dt:.0f}s\n\n")
            f.write("## Solved (zero-token mechanical rewrite)\n\n")
            for n, c, b, p, l, _t, _s, _e in solved:
                f.write(f"- **{n}** [{c}] {b:.2f}% -> 100% via `{l}`\n")
            f.write("\n## Improved but not 100%\n\n")
            for n, c, b, p, l, _t, _s, _e in improved:
                if p < 100:
                    f.write(f"- {n} [{c}] {b:.2f}% -> {p:.2f}% "
                            f"via `{l}`\n")
        print(f"[autorewrite] report -> {args.report}")


if __name__ == "__main__":
    main()
