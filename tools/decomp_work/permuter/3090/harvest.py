#!/usr/bin/env python3
"""harvest.py — score & harvest a finished permuter run for one function.

usage: harvest.py <fn> <unit> [worker]

Reads $FARM/dirs/<fn>/ (permuter output-<score>-* dirs + logs/run_<fn>.log).
On a score-0 isolated candidate:
  - recompiles it with the unit's exact compile.sh
  - re-scores with objdiff-cli (independent scorer; must report 100% match)
  - writes the full source/diff plus focused function.c/function.diff and a
    summary explicitly marked live_tree_validated=false
  - records WIN_UNCONFIRMED until a current-master full-TU replay verifies
    relocation identity, sibling fidelity, and a real report improvement
Non-zero best candidates are stashed under results/_partials/<fn>/ so partial
progress is preserved. Terminal state written to $FARM/state/<fn>.status.
"""
import difflib
import glob
import json
import os
import re
import subprocess
import sys
import time

BASE = os.environ.get("FARM_BASE", "/storage/finetune/pkmn-colosseum-2026")
FARM = os.path.join(BASE, "farm")
OBJDIFF = os.path.join(BASE, "tools", "objdiff-cli")


def extract_function(text, fn):
    """Return one complete function definition from preprocessed C.

    Permuter output contains a large normalized translation unit. Saving only
    that file makes a win easy to misapply: a small but essential second edit
    can be buried thousands of lines away in the target body. This scanner
    emits the complete body while ignoring braces in comments and strings.
    """
    for match in re.finditer(r"\b" + re.escape(fn) + r"\s*\(", text):
        i = match.end() - 1
        depth = 0
        quote = None
        escaped = False
        while i < len(text):
            ch = text[i]
            if quote:
                if escaped:
                    escaped = False
                elif ch == "\\":
                    escaped = True
                elif ch == quote:
                    quote = None
            elif ch in ('"', "'"):
                quote = ch
            elif ch == "(":
                depth += 1
            elif ch == ")":
                depth -= 1
                if depth == 0:
                    break
            i += 1
        if depth != 0:
            continue
        i += 1
        while i < len(text) and text[i].isspace():
            i += 1
        if i >= len(text) or text[i] != "{":
            continue

        start = text.rfind("\n", 0, match.start()) + 1
        brace_depth = 0
        quote = None
        escaped = False
        line_comment = False
        block_comment = False
        j = i
        while j < len(text):
            ch = text[j]
            nxt = text[j + 1] if j + 1 < len(text) else ""
            if line_comment:
                if ch == "\n":
                    line_comment = False
            elif block_comment:
                if ch == "*" and nxt == "/":
                    block_comment = False
                    j += 1
            elif quote:
                if escaped:
                    escaped = False
                elif ch == "\\":
                    escaped = True
                elif ch == quote:
                    quote = None
            elif ch == "/" and nxt == "/":
                line_comment = True
                j += 1
            elif ch == "/" and nxt == "*":
                block_comment = True
                j += 1
            elif ch in ('"', "'"):
                quote = ch
            elif ch == "{":
                brace_depth += 1
            elif ch == "}":
                brace_depth -= 1
                if brace_depth == 0:
                    end = j + 1
                    if end < len(text) and text[end] == "\n":
                        end += 1
                    return text[start:end]
            j += 1
    return None


def objdiff_pct(target_o, cand_o, fn):
    """Return objdiff match percent for fn, or None."""
    try:
        r = subprocess.run(
            [OBJDIFF, "diff", "-1", target_o, "-2", cand_o, "-o", "-",
             "--format", "json", fn],
            capture_output=True, text=True, timeout=120)
        data = json.loads(r.stdout)
    except Exception:
        return None

    # walk the report for the symbol's match_percent
    def walk(o):
        if isinstance(o, dict):
            name = o.get("name") or o.get("symbol_name") or o.get("demangled_name")
            if name == fn:
                for k in ("match_percent", "matchPercent", "fuzzy_match_percent"):
                    if k in o:
                        return float(o[k])
            for v in o.values():
                got = walk(v)
                if got is not None:
                    return got
        elif isinstance(o, list):
            for v in o:
                got = walk(v)
                if got is not None:
                    return got
        return None

    return walk(data)


def existing_partial_score(partial_dir):
    try:
        with open(os.path.join(partial_dir, "info.json")) as f:
            data = json.load(f)
        score = data.get("best_score")
        return int(score) if score is not None else None
    except (OSError, TypeError, ValueError, json.JSONDecodeError):
        return None


def select_partial_score(existing_score, candidate_score):
    """Return (record_score, replace) without regressing a banked partial."""
    if candidate_score is None:
        return existing_score, False
    if existing_score is None or candidate_score < existing_score:
        return candidate_score, True
    return existing_score, False


def main():
    fn, unit = sys.argv[1], sys.argv[2]
    worker = sys.argv[3] if len(sys.argv) > 3 else "?"
    d = os.path.join(FARM, "dirs", fn)
    runlog = os.path.join(FARM, "logs", f"run_{fn}.log")
    state = os.path.join(FARM, "state", fn + ".status")
    os.makedirs(os.path.join(FARM, "state"), exist_ok=True)
    log = ""
    if os.path.exists(runlog):
        log = open(runlog, errors="replace").read()
    mbase = re.search(r"base score = (-?\d+)", log)
    base_score = int(mbase.group(1)) if mbase else None

    outs = []
    for od in glob.glob(os.path.join(d, "output-*")):
        m = re.match(r"output-(-?\d+)", os.path.basename(od))
        sp = os.path.join(od, "source.c")
        if m and os.path.exists(sp):
            outs.append((int(m.group(1)), sp))
    outs.sort()
    best_score, best_src = outs[0] if outs else (None, None)
    if best_score is None and base_score == 0:
        best_score, best_src = 0, os.path.join(d, "base.c")

    if best_score == 0 and best_src:
        # independent verification: recompile + objdiff score
        cand_o = os.path.join(d, "cand_verify.o")
        r = subprocess.run(["bash", os.path.join(d, "compile.sh"),
                            best_src, "-o", cand_o], capture_output=True, text=True)
        pct = None
        if r.returncode == 0 and os.path.exists(cand_o):
            pct = objdiff_pct(os.path.join(d, "target.o"), cand_o, fn)
        rd = os.path.join(FARM, "results", fn)
        os.makedirs(rd, exist_ok=True)
        subprocess.run(["cp", "-f", best_src, os.path.join(rd, "source.c")])
        base_src = os.path.join(d, "base.c")
        diff = subprocess.run(["diff", "-u", base_src, best_src],
                              capture_output=True, text=True).stdout
        open(os.path.join(rd, "diff.txt"), "w").write(diff)
        candidate_text = open(best_src, errors="replace").read()
        base_text = open(base_src, errors="replace").read()
        candidate_fn = extract_function(candidate_text, fn)
        base_fn = extract_function(base_text, fn)
        function_diff = ""
        if candidate_fn:
            open(os.path.join(rd, "function.c"), "w").write(candidate_fn)
        if candidate_fn and base_fn:
            function_diff = "".join(difflib.unified_diff(
                base_fn.splitlines(keepends=True),
                candidate_fn.splitlines(keepends=True),
                fromfile=f"base/{fn}.c", tofile=f"candidate/{fn}.c"))
            open(os.path.join(rd, "function.diff"), "w").write(function_diff)
        summary = {
            "fn": fn, "unit": unit, "worker": worker,
            "permuter_score": 0, "base_score": base_score,
            "achieved_pct": pct,
            "objdiff_confirmed": pct is not None and pct >= 100.0,
            "recompile_ok": r.returncode == 0,
            "verification_scope": "isolated_preprocessed_function",
            "live_tree_validated": False,
            "function_extract_ok": candidate_fn is not None,
            "created": time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime()),
            "source_diff": diff[:20000],
            "function_diff": function_diff[:20000],
        }
        with open(os.path.join(rd, "summary.json"), "w") as f:
            json.dump(summary, f, indent=1)
        # Isolated score-zero is only a candidate. It can hide relocation-name
        # substitutions or stale-snapshot/no-op results, so only an external
        # current-master full-TU replay may promote this state to WIN.
        status = "WIN_UNCONFIRMED"
        with open(state, "w") as f:
            f.write(f"{status} {worker} {int(time.time())} pct={pct}\n")
        print(f"{status} {fn} pct={pct}")
    else:
        pd = os.path.join(FARM, "results", "_partials", fn)
        recorded_score, replace_partial = select_partial_score(
            existing_partial_score(pd), best_score)
        if best_src and replace_partial:
            os.makedirs(pd, exist_ok=True)
            subprocess.run(["cp", "-f", best_src, os.path.join(pd, "best.c")])
            with open(os.path.join(pd, "info.json"), "w") as f:
                json.dump({"fn": fn, "unit": unit, "best_score": best_score,
                           "base_score": base_score}, f)
        with open(state, "w") as f:
            f.write(f"NOWIN {worker} {int(time.time())} best={recorded_score} base={base_score}\n")
        print(f"NOWIN {fn} run_best={best_score} best={recorded_score} base={base_score}")


if __name__ == "__main__":
    main()
