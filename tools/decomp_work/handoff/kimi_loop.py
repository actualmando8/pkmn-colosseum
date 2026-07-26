#!/usr/bin/env python3
"""Drive Kimi (Moonshot API) through the local match loop for one function.

The economics: Kimi generates candidates cheaply; local ground truth (mwcc +
objdiff) scores every attempt; nothing is trusted until the DOL gate. This is
the sol-pattern serial grind with an external model doing the candidate
generation -- no subagent fleets, no shared-tree races.

    set MOONSHOT_API_KEY=sk-...          (or $env:MOONSHOT_API_KEY)
    python tools/decomp_work/handoff/kimi_loop.py \
        --source src/game/people/people.c --symbol fn_8018BC88 --rounds 6

Each round: send the brief (+ previous diff feedback) -> receive a full
replacement function body -> splice byte-safely -> rebuild object -> objdiff.
Stops at 100.00% or after --rounds. On success, prints the verify.py command
that runs the real gate. Restores the source unless --keep and 100% reached.
"""
import argparse
import json
import os
import re
import subprocess
import sys
import tempfile
import urllib.request
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import c89_lint  # noqa: E402  (local sibling module)

ROOT = Path(__file__).resolve().parents[3]
CLI = ROOT / "build" / "tools" / ("objdiff-cli.exe" if os.name == "nt" else "objdiff-cli")
API = "https://api.moonshot.ai/v1/chat/completions"
MODEL = os.environ.get("KIMI_MODEL", "kimi-k2-0711-preview")


def chat(messages, key):
    import time
    import urllib.error
    body = json.dumps({"model": MODEL, "messages": messages,
                       "max_tokens": 8192}).encode()  # kimi-k3: temperature must be omitted
    for attempt in range(3):
        req = urllib.request.Request(
            API, data=body,
            headers={"Content-Type": "application/json",
                     "Authorization": f"Bearer {key}"})
        try:
            with urllib.request.urlopen(req, timeout=420) as r:
                msg = json.loads(r.read())["choices"][0]["message"]
                # kimi-k3 puts thinking in reasoning_content and may leave
                # content EMPTY; an empty assistant message poisons the next
                # request (Moonshot 400s on it). Fall back, then floor it.
                return (msg.get("content") or
                        msg.get("reasoning_content") or "(empty reply)")
        except urllib.error.HTTPError as e:
            detail = e.read().decode("utf-8", "replace")[:300]
            print(f"API {e.code} (attempt {attempt + 1}/3): {detail}", flush=True)
            if attempt < 2:
                time.sleep(15 * (attempt + 1))
                continue
            raise
    raise RuntimeError("API failed after 3 attempts")


def unit_for(source):
    cfg = json.loads((ROOT / "objdiff.json").read_text())
    tail = Path(source).with_suffix(".o").as_posix()
    for u in cfg["units"]:
        if (u.get("base_path") or "").replace("\\", "/").endswith(tail):
            return u["name"]
    sys.exit(f"no unit for {source}")


def measure(unit, symbol):
    with tempfile.TemporaryDirectory() as td:
        out = Path(td) / "d.json"
        subprocess.run([str(CLI), "diff", "-p", str(ROOT), "-u", unit,
                        "-o", str(out), "--format", "json", symbol],
                       cwd=ROOT, capture_output=True)
        if not out.exists():
            return None, []
        d = json.loads(out.read_text())
        s = next((x for x in d["left"].get("symbols", []) if x["name"] == symbol), None)
        if not s:
            return None, []
        L = s.get("instructions") or []
        R = next(((x.get("instructions") or []) for x in d["right"]["symbols"]
                  if x["name"] == symbol), [])
        f = lambda i: (i.get("instruction") or {}).get("formatted", "") if i else ""
        rows = []
        for k, (a, b) in enumerate(zip(L, R)):
            if "diff_kind" in a or "diff_kind" in b:
                rows.append(f"{k:>4}  want: {f(a):<36} have: {f(b)}")
        return s.get("match_percent"), rows


def splice(text, symbol, body):
    # Match the function's definition line. Skip the dead `#if 0 asm void X(){
    # #include ...inc }` reference stubs that wrap most functions -- those start
    # with `asm ` and splicing into them changes nothing (the real #else C is
    # what compiles). Pick the first NON-asm definition.
    pat = re.compile(r'^([A-Za-z_][^\n;]*?\b' + re.escape(symbol)
                     + r'\s*\([^;]*?\)\s*)\{', re.M)
    m = None
    for cand in pat.finditer(text):
        head = cand.group(1)
        if re.match(r'^\s*asm\b', head):
            continue           # dead #if 0 reference stub
        m = cand
        break
    if not m:
        return None
    i = m.start()
    j = text.index("{", m.start())
    depth = 0
    while j < len(text):
        if text[j] == "{":
            depth += 1
        elif text[j] == "}":
            depth -= 1
            if depth == 0:
                break
        j += 1
    return text[:i] + body.strip() + text[j + 1:]


def extract_code(reply):
    m = re.findall(r"```(?:c)?\n(.*?)```", reply, re.S)
    return m[-1].strip() if m else None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--source", required=True)
    ap.add_argument("--symbol", required=True)
    ap.add_argument("--rounds", type=int, default=6)
    ap.add_argument("--keep", action="store_true")
    a = ap.parse_args()

    key = os.environ.get("MOONSHOT_API_KEY")
    if not key:
        sys.exit("MOONSHOT_API_KEY not set")

    src = ROOT / a.source
    unit = unit_for(a.source)
    obj = ROOT / "build" / "GC6E01" / Path(a.source).with_suffix(".o")
    backup = src.read_bytes()

    # decode as utf-8/replace (NOT the Windows cp1252 locale) -- gen_brief
    # embeds the source, and many game TUs contain Shift-JIS bytes that cp1252
    # cannot decode; force utf-8 both directions.
    env = dict(os.environ, PYTHONIOENCODING="utf-8")
    br = subprocess.run(
        [sys.executable, str(ROOT / "tools/decomp_work/handoff/gen_brief.py"),
         "--source", a.source, "--symbol", a.symbol],
        cwd=ROOT, capture_output=True, encoding="utf-8", errors="replace", env=env)
    brief = br.stdout or ""
    if not brief.strip():
        sys.exit("gen_brief produced no output -- likely missing "
                 "build/GC6E01/report.json (run: ninja all_source "
                 "build/GC6E01/report.json). stderr: " + br.stderr[:300])

    msgs = [
        {"role": "system", "content":
         "You are matching CodeWarrior GC/1.3 PowerPC output byte-exactly. "
         "Reply with ONLY the complete replacement C function in one ```c fence. "
         "C89 (declarations before statements). No inline asm ever. Types "
         "u8/s8/u16/s16/u32/s32/f32/f64. Named extern float labels, no literals."},
        {"role": "user", "content": brief[:24000]},
    ]

    best = None
    ok = False
    try:
        for rnd in range(1, a.rounds + 1):
            reply = chat(msgs, key)
            body = extract_code(reply)
            # reject only real asm constructs, not the word "asm" in prose --
            # comment mentions have false-positived three times in this repo
            bad_asm = body and re.search(
                r'\basm\s*(?:\w+\s*)?[({]|__asm\b|#\s*include\s+"[^"]+\.inc"',
                body)
            if not body or bad_asm:
                print(f"round {rnd}: rejected "
                      f"({'asm construct' if bad_asm else 'no code fence'})",
                      flush=True)
                msgs.append({"role": "assistant", "content": reply or "(empty)"})
                msgs.append({"role": "user", "content":
                             "Reply with only one ```c fence containing pure "
                             "C89, no asm constructs."})
                continue
            # C89 pre-lint: catch mid-block declarations BEFORE compiling.
            # kimi-k3 emits C99-style decls repeatedly; the compiler's caret-
            # only "declaration syntax error" taught it nothing (0/8 on two
            # lanes). Feeding back the exact offending lines fixes the round.
            viols = c89_lint.lint(body)
            if viols:
                fb = "\n".join(f"  line {ln}: {txt}  ({why})"
                               for ln, txt, why in viols[:8])
                print(f"round {rnd}: C89 pre-lint rejected "
                      f"({len(viols)} mid-block declaration(s))", flush=True)
                msgs.append({"role": "assistant", "content": reply})
                msgs.append({"role": "user", "content":
                             "Your C is not C89. These lines declare variables "
                             "AFTER a statement:\n" + fb +
                             "\nMove EVERY declaration to the top of its block, "
                             "before any statement. Return the full function."})
                continue
            # model output may carry unicode (arrows in comments); the source
            # round-trips latin-1 to preserve Shift-JIS bytes, so force the
            # candidate itself to ASCII before splicing.
            body = body.encode("ascii", "replace").decode("ascii")
            text = splice(backup.decode("latin-1"), a.symbol, body)
            if not text:
                print("splice failed"); break
            src.write_bytes(text.encode("latin-1"))
            if obj.exists():
                obj.unlink()
            cc = subprocess.run(["ninja", str(obj.relative_to(ROOT)).replace("\\", "/")],
                                cwd=ROOT, capture_output=True,
                                encoding="utf-8", errors="replace")
            if not obj.exists():
                # mwcc prints the offending SOURCE LINE above the "Error: ^"
                # caret; grepping only "Error" lines drops the actionable
                # context. Capture a window around each error/caret instead,
                # and skip the non-fatal blanket Shift-JIS warning.
                out = ((cc.stdout or "") + (cc.stderr or "")).splitlines()
                keep, seen = [], set()
                for idx, l in enumerate(out):
                    low = l.lower()
                    if ("error" in low or "syntax" in low) and "shift jis" not in low:
                        for j in range(max(0, idx - 2), min(len(out), idx + 2)):
                            if j not in seen and out[j].strip():
                                seen.add(j); keep.append(out[j].rstrip())
                pct, rows = None, ["COMPILE FAILED (fix the C, keep it C89):"] + keep[:20]
            else:
                pct, rows = measure(unit, a.symbol)
            print(f"round {rnd}: {pct if pct is not None else 'compile-fail'}",
                  flush=True)
            if pct is None:
                for ln in rows[:6]:
                    print(f"    {ln}", flush=True)
            if pct is not None and (best is None or pct > best):
                best = pct
                # persist the best body -- the finally-restore otherwise
                # destroys it and the strongest candidate is lost
                (ROOT / f"kimi_best_{a.symbol}.c").write_text(
                    body, encoding="utf-8", newline="\n")
            if pct == 100.0:
                ok = True
                print(f"MATCHED. Real gate:\n  python tools/decomp_work/handoff/"
                      f"verify.py --source {a.source} --symbol {a.symbol} "
                      f"--candidate <file> --link")
                break
            fb = "\n".join(rows[:20]) or "did not compile"
            msgs.append({"role": "assistant", "content": reply})
            msgs.append({"role": "user", "content":
                         f"Score: {pct}. Remaining instruction diffs "
                         f"(want=target, have=yours):\n{fb}\n"
                         "Adjust the C to close these. Full function again."})
    finally:
        if not (ok and a.keep):
            src.write_bytes(backup)
            if obj.exists():
                obj.unlink()
            subprocess.run(["ninja", str(obj.relative_to(ROOT)).replace("\\", "/")],
                           cwd=ROOT, capture_output=True)
            print(f"restored (best this session: {best})")
        else:
            print("kept in tree; run the verify.py gate before committing.")


if __name__ == "__main__":
    main()
