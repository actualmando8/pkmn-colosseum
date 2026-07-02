#!/usr/bin/env python3
"""gen_workunits.py — build decomp-permuter work units for the Windows farm.

For each row of a queue TSV (tier, pct, size, name, addr, unit) this produces
<outdir>/<fn>/ containing:
  base.c        preprocessed whole-TU source reduced to just <fn>
                (other function bodies stripped to declarations)
  target.o      single-function object assembled from the dtk asm .s
  compile.bat   Windows compile wrapper with the unit's exact MWCC flags
  settings.toml permuter settings (func_name, mwcc weights, objdump cmd)
  meta.json     provenance (fn, unit, addr, pct, mw_version, cflags, gate info)

The unit's exact compiler version + flags are read from build.ninja (ground
truth for the byte-matching build).  Preprocessing runs the unit's own
mwcceppc.exe -E via wibo, so the preprocessed source is exactly what the
matching build compiles.

Each unit is gated locally before being shipped:
  gate 1: base.c re-parses with decomp-permuter's pycparser fork
  gate 2: base.c compiles with the unit's exact mwcc + flags
  gate 3: the compiled object contains exactly one function: <fn>
  gate 4: permuter Scorer(base.o vs target.o) yields a finite base score > 0

Usage:
  python3 tools/decomp_work/permuter/gen_workunits.py \
      --queue build/permuter_queue_win.tsv \
      --outdir build/permuter_workunits/win \
      [--only fn_800FE38C] [--limit N] [--permuter <path>] [--binutils <dir>]

Only writes under --outdir (default under build/, which is gitignored).
"""

import argparse
import json
import os
import re
import shlex
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
DEFAULT_PERMUTER = Path(
    os.environ.get("DECOMP_PERMUTER", str(Path.home() / ".cache/pkmn-permuter-tools/decomp-permuter"))
)
DEFAULT_BINUTILS = Path(
    os.environ.get("PPC_BINUTILS", str(Path.home() / ".cache/pkmn-permuter-tools/ppc-binutils"))
)

# Windows-side layout (used inside generated compile.bat / settings.toml).
WIN_ROOT = "C:/Users/douglaswhittingham/gamecube-decomp/pkmn-permuter"

# ---------------------------------------------------------------------------
# build.ninja parsing
# ---------------------------------------------------------------------------

def parse_ninja(ninja_path: Path):
    """Return {obj_path: {"mw_version":…, "cflags":…}} for mwcc rules."""
    text = ninja_path.read_text()
    # Un-wrap ninja line continuations ("$\n" + indent).
    text = re.sub(r"\$\n\s*", "", text)
    units = {}
    cur = None
    for line in text.splitlines():
        m = re.match(r"^build (\S+?): (mwcc\S*) (\S+)", line)
        if m:
            cur = {"obj": m.group(1), "rule": m.group(2), "src": m.group(3)}
            units[m.group(1)] = cur
            continue
        if cur is not None:
            m = re.match(r"^\s+mw_version = (.+)$", line)
            if m:
                cur["mw_version"] = m.group(1).strip()
                continue
            m = re.match(r"^\s+cflags = (.+)$", line)
            if m:
                cur["cflags"] = m.group(1).strip()
                continue
            if not line.startswith(" "):
                cur = None
    return units


def unit_to_paths(unit: str):
    """Map queue unit name (e.g. main/game/gs_thread_hi) to project paths."""
    rel = unit.split("/", 1)[1]  # drop leading "main/"
    return {
        "src": REPO / "src" / (rel + ".c"),
        "obj": f"build/GC6E01/src/{rel}.o",
        "asm": REPO / "build/GC6E01/asm" / (rel + ".s"),
    }

# ---------------------------------------------------------------------------
# C pruning: strip all function bodies except the target's
# ---------------------------------------------------------------------------

def _scan_functions(src: str):
    """Yield (name, header_start, body_open, body_close) for every top-level
    function definition.  Tokenizer-based: tracks strings/chars and nesting, so
    multi-line signatures are handled (unlike a line regex)."""
    i, n = 0, len(src)
    depth_brace = 0
    funcs = []
    last_semicolon = 0  # start of the current top-level "statement"
    while i < n:
        c = src[i]
        if c == '"' or c == "'":
            q = c
            i += 1
            while i < n:
                if src[i] == "\\":
                    i += 2
                    continue
                if src[i] == q:
                    break
                i += 1
        elif c == "{":
            if depth_brace == 0:
                # Function body iff the last non-ws char before '{' is ')'
                # (ANSI), or ';' preceded only by K&R parameter declarations
                # that trace back to a ')' (K&R style:  f(a, b) int a; int b; { ).
                j = i - 1
                while j >= 0 and src[j] in " \t\r\n":
                    j -= 1
                is_knr = False
                if j >= 0 and src[j] == ";":
                    k = j
                    declset = set(
                        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                        "0123456789_*,;[] \t\r\n"
                    )
                    while k >= 0 and src[k] in declset:
                        k -= 1
                    if k >= 0 and src[k] == ")":
                        is_knr = True
                        j = k  # continue as if ')' preceded the '{'
                if j >= 0 and src[j] == ")":
                    # find matching '(' for the parameter list
                    k, pd = j, 0
                    while k >= 0:
                        if src[k] == ")":
                            pd += 1
                        elif src[k] == "(":
                            pd -= 1
                            if pd == 0:
                                break
                        k -= 1
                    # identifier before '('
                    m = None
                    kk = k - 1
                    while kk >= 0 and src[kk] in " \t\r\n":
                        kk -= 1
                    end_id = kk + 1
                    while kk >= 0 and (src[kk].isalnum() or src[kk] == "_"):
                        kk -= 1
                    name = src[kk + 1 : end_id]
                    if name:
                        m = name
                    # brace-match the body
                    bd, ii = 0, i
                    while ii < n:
                        cc = src[ii]
                        if cc == '"' or cc == "'":
                            qq = cc
                            ii += 1
                            while ii < n:
                                if src[ii] == "\\":
                                    ii += 2
                                    continue
                                if src[ii] == qq:
                                    break
                                ii += 1
                        elif cc == "{":
                            bd += 1
                        elif cc == "}":
                            bd -= 1
                            if bd == 0:
                                break
                        ii += 1
                    if m:
                        # for K&R fns the declaration must end at the '(' —
                        # param names may not appear in a prototype.  The K&R
                        # param decls also contain top-level ';', which poisons
                        # last_semicolon, so recompute the header start by
                        # scanning back over the return-type tokens.
                        hs = last_semicolon
                        if is_knr:
                            t = kk
                            while t >= 0 and (src[t].isalnum() or src[t] in "_* \t\r\n"):
                                t -= 1
                            hs = t + 1
                        funcs.append((m, hs, i, ii, k if is_knr else None))
                    i = ii
                    last_semicolon = ii + 1
                else:
                    # top-level initializer block: skip it wholesale
                    bd, ii = 0, i
                    while ii < n:
                        cc = src[ii]
                        if cc == '"' or cc == "'":
                            qq = cc
                            ii += 1
                            while ii < n:
                                if src[ii] == "\\":
                                    ii += 2
                                    continue
                                if src[ii] == qq:
                                    break
                                ii += 1
                        elif cc == "{":
                            bd += 1
                        elif cc == "}":
                            bd -= 1
                            if bd == 0:
                                break
                        ii += 1
                    i = ii
                    last_semicolon = ii + 1
            else:
                depth_brace += 1
        elif c == "}":
            if depth_brace > 0:
                depth_brace -= 1
        elif c == ";" and depth_brace == 0:
            last_semicolon = i + 1
        i += 1
    return funcs


def prune_source(src: str, keep_fn: str):
    """Replace every top-level function body except keep_fn's with ';'.
    'static'/'asm' qualifiers on stripped definitions are rewritten so the
    result still compiles (a declared-but-undefined static is an error)."""
    funcs = _scan_functions(src)
    kept = [f for f in funcs if f[0] == keep_fn]
    if not kept:
        return None, funcs
    out = []
    pos = 0
    for name, hdr_start, body_open, body_close, knr_paren in funcs:
        if name == keep_fn:
            continue
        out.append(src[pos:hdr_start])
        if knr_paren is not None:
            # K&R: declaration is "<ret> <name>();"
            header = src[hdr_start:knr_paren].rstrip() + "()"
        else:
            header = src[hdr_start:body_open].rstrip()
        # drop 'asm' qualifier and turn 'static' into 'extern'
        header = re.sub(r"(^|\s)asm(\s)", r"\1\2", header)
        header = re.sub(r"(^|\s)static(\s)", r"\1extern\2", header)
        out.append(header + ";\n")
        pos = body_close + 1
    out.append(src[pos:])
    return "".join(out), funcs

# ---------------------------------------------------------------------------
# target.o extraction
# ---------------------------------------------------------------------------

def extract_fn_asm(asm_path: Path, fn: str):
    lines = asm_path.read_text().splitlines()
    out = None
    for ln in lines:
        if re.match(rf"^\.fn {re.escape(fn)},", ln):
            out = [".text", f".globl {fn}", f"{fn}:"]
            continue
        if out is not None:
            if ln.startswith(f".endfn {fn}"):
                return "\n".join(out) + "\n"
            # GNU as (gc-wii-binutils) wants a bare GQR number in psq_l/psq_st
            # operands where dtk writes qr0..qr7.
            ln = re.sub(r",\s*qr(\d)\b", r", \1", ln)
            # ... and bare CR-bit numbers where dtk writes cr1eq etc.
            ln = re.sub(
                r"\bcr([0-7])(lt|gt|eq|so|un)\b",
                lambda m: str(int(m.group(1)) * 4
                              + {"lt": 0, "gt": 1, "eq": 2, "so": 3, "un": 3}[m.group(2)]),
                ln,
            )
            out.append(ln)
    return None

# ---------------------------------------------------------------------------
# main
# ---------------------------------------------------------------------------

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--queue", default="build/permuter_queue_win.tsv")
    ap.add_argument("--outdir", default="build/permuter_workunits/win")
    ap.add_argument("--only", action="append", default=None, help="only these fn names")
    ap.add_argument("--limit", type=int, default=0)
    ap.add_argument("--permuter", default=str(DEFAULT_PERMUTER))
    ap.add_argument("--binutils", default=str(DEFAULT_BINUTILS))
    ap.add_argument("--force", action="store_true", help="regenerate existing units")
    args = ap.parse_args()

    permuter = Path(args.permuter)
    binutils = Path(args.binutils)
    ppc_as = binutils / "powerpc-eabi-as"
    ppc_objdump = binutils / "powerpc-eabi-objdump"
    for p in (permuter / "strip_other_fns.py", ppc_as, ppc_objdump):
        if not p.exists():
            sys.exit(f"missing prerequisite: {p}")
    sys.path.insert(0, str(permuter))

    wibo = REPO / "build/tools/wibo"
    sjiswrap = REPO / "build/tools/sjiswrap.exe"

    ninja_units = parse_ninja(REPO / "build.ninja")

    outdir = REPO / args.outdir
    outdir.mkdir(parents=True, exist_ok=True)

    rows = []
    with open(REPO / args.queue) as f:
        for ln in f:
            if ln.startswith("#") or not ln.strip():
                continue
            tier, pct, size, name, addr, unit = ln.rstrip("\n").split("\t")
            rows.append(dict(tier=int(tier), pct=float(pct), size=int(size),
                             fn=name, addr=addr, unit=unit))
    if args.only:
        rows = [r for r in rows if r["fn"] in set(args.only)]
    if args.limit:
        rows = rows[: args.limit]

    # local scorer (gate 4)
    from src.scorer import Scorer  # decomp-permuter

    manifest = []
    n_ok = n_skip = 0
    for row in rows:
        fn, unit = row["fn"], row["unit"]
        udir = outdir / fn
        meta = dict(row)
        meta["status"] = "unknown"

        def fail(reason, detail=""):
            nonlocal n_skip
            meta["status"] = "skip"
            meta["reason"] = reason
            if detail:
                meta["detail"] = detail[-1500:]
            manifest.append(meta)
            n_skip += 1
            print(f"SKIP {fn} ({unit}): {reason}")

        if udir.exists() and (udir / "meta.json").exists() and not args.force:
            old = json.loads((udir / "meta.json").read_text())
            if old.get("status") == "ok":
                manifest.append(old)
                n_ok += 1
                print(f"KEEP {fn} (already generated)")
                continue

        paths = unit_to_paths(unit)
        nu = ninja_units.get(paths["obj"])
        if nu is None:
            fail("unit not in build.ninja")
            continue
        if not paths["src"].exists() or not paths["asm"].exists():
            fail("missing src or asm", f"{paths['src']} {paths['asm']}")
            continue

        mwcc = REPO / "build/compilers" / nu["mw_version"] / "mwcceppc.exe"
        cflags = shlex.split(nu["cflags"])

        udir.mkdir(parents=True, exist_ok=True)

        # 1. preprocess with the unit's own mwcc
        pp = udir / "_pp.c"
        r = subprocess.run(
            [str(wibo), str(mwcc), *cflags, "-E", str(paths["src"]), "-o", str(pp)],
            cwd=REPO, capture_output=True, text=True)
        if r.returncode != 0 or not pp.exists():
            fail("mwcc -E failed", r.stderr or r.stdout)
            continue
        src_pp = pp.read_text(encoding="utf-8", errors="replace")
        src_pp = re.sub(r"^/\* #line .*\n?", "", src_pp, flags=re.M)

        # 2. prune to the single target function
        pruned, funcs = prune_source(src_pp, fn)
        if pruned is None:
            fail("target fn not found in preprocessed TU (asm-only or name mismatch)")
            continue
        base_c = udir / "base.c"
        base_c.write_text(pruned, encoding="utf-8")

        # gate 1: pycparser parse (what the permuter itself will do)
        gate = subprocess.run(
            [sys.executable, "-c",
             "import sys; sys.path.insert(0, sys.argv[1]);"
             "from perm_pycparser.c_parser import CParser;"
             "CParser().parse(open(sys.argv[2]).read(), 'base.c')",
             str(permuter), str(base_c)],
            capture_output=True, text=True)
        if gate.returncode != 0:
            fail("pycparser parse failed", gate.stderr)
            continue

        # 3. target.o from dtk asm
        fn_asm = extract_fn_asm(paths["asm"], fn)
        if fn_asm is None:
            fail("fn not found in dtk asm .s")
            continue
        tgt_s = udir / "_target.s"
        tgt_s.write_text(fn_asm)
        r = subprocess.run(
            [str(ppc_as), "-mgekko", "-mregnames", "-be",
             "-o", str(udir / "target.o"), str(tgt_s)],
            capture_output=True, text=True)
        if r.returncode != 0:
            fail("powerpc-eabi-as failed", r.stderr)
            continue

        # 4. compile gate with exact flags (drop -i/include & -MMD-ish flags:
        # base.c is fully preprocessed).  Keep everything else identical.
        cf = []
        skip_next = False
        for a in cflags:
            if skip_next:
                skip_next = False
                continue
            if a == "-i":
                skip_next = True
                continue
            cf.append(a)
        base_o = udir / "_base.o"
        r = subprocess.run(
            [str(wibo), str(sjiswrap), str(mwcc), *cf, "-c",
             str(base_c), "-o", str(base_o)],
            cwd=REPO, capture_output=True, text=True)
        if r.returncode != 0 or not base_o.exists():
            fail("base.c does not compile", r.stderr or r.stdout)
            continue

        # gate 3: candidate object contains exactly one function: fn
        r = subprocess.run([str(ppc_objdump), "-d", str(base_o)],
                           capture_output=True, text=True)
        symbols = re.findall(r"^[0-9a-f]+ <([^>]+)>:$", r.stdout, flags=re.M)
        if symbols != [fn]:
            fail("stripped object has wrong functions", ",".join(symbols))
            continue

        # gate 4: finite base score
        try:
            scorer = Scorer(str(udir / "target.o"), stack_differences=True,
                            algorithm="difflib", debug_mode=False,
                            ign_branch_targets=False,
                            objdump_command=f"{ppc_objdump} -dr -EB -mpowerpc -M broadway")
            score, _ = scorer.score(str(base_o))
        except Exception as e:  # noqa
            fail("scorer failed", str(e))
            continue
        if score >= Scorer.PENALTY_INF:
            fail("base score infinite")
            continue
        meta["base_score"] = score
        if score == 0:
            fail("base score already 0 (function already matches?)")
            continue

        # 5. Windows-side files
        mwver = nu["mw_version"]
        flags_str = subprocess.list2cmdline(cf)
        compile_bat = (
            "@echo off\r\n"
            f"\"{WIN_ROOT}/tools/sjiswrap.exe\" \"{WIN_ROOT}/tools/mwcc/{mwver}/mwcceppc.exe\" "
            f"{flags_str} -c %1 -o %3\r\n"
        )
        (udir / "compile.bat").write_text(compile_bat)
        # POSIX wrapper kept for local (mac) permuter runs / debugging:
        compile_sh = (
            "#!/bin/bash\n"
            f"exec \"{REPO}/build/tools/wibo\" \"{REPO}/build/tools/sjiswrap.exe\" "
            f"\"{REPO}/build/compilers/{mwver}/mwcceppc.exe\" {flags_str} -c \"$1\" -o \"$3\"\n"
        )
        (udir / "compile.sh").write_text(compile_sh)
        os.chmod(udir / "compile.sh", 0o755)

        # Direct-argv compile command (avoids cmd.exe per candidate; sjiswrap
        # only when the preprocessed source actually contains non-ASCII).
        needs_sjis = any(b > 0x7F for b in base_c.read_bytes())
        argv = []
        if needs_sjis:
            argv.append(f"{WIN_ROOT}/tools/sjiswrap.exe")
        argv.append(f"{WIN_ROOT}/tools/mwcc/{mwver}/mwcceppc.exe")
        argv += cf
        (udir / "compile_cmd.json").write_text(json.dumps({"argv": argv}, indent=1))

        settings = (
            f'func_name = "{fn}"\n'
            'compiler_type = "mwcc"\n'
            f'objdump_command = "{WIN_ROOT}/tools/binutils/powerpc-eabi-objdump.exe -dr -EB -mpowerpc -M broadway"\n'
            "\n"
            "[weight_overrides]\n"
            "perm_reorder_decls = 40\n"
            "perm_reorder_stmts = 40\n"
            "perm_dummy_comma_expr = 30\n"
            "perm_commutative = 20\n"
            "perm_pad_var_decl = 10\n"
            "perm_split_assignment = 20\n"
            "perm_duplicate_assignment = 15\n"
            "perm_add_self_assignment = 15\n"
            "perm_refer_to_var = 15\n"
        )
        (udir / "settings.toml").write_text(settings)

        meta["status"] = "ok"
        meta["mw_version"] = mwver
        meta["cflags"] = flags_str
        meta["n_fns_in_tu"] = len(funcs)
        (udir / "meta.json").write_text(json.dumps(meta, indent=1))
        manifest.append(meta)
        n_ok += 1
        print(f"OK   {fn} ({unit}) base_score={score}")

        # tidy intermediates
        for tmp in (pp, tgt_s, base_o):
            try:
                tmp.unlink()
            except OSError:
                pass

    (outdir / "manifest.json").write_text(json.dumps(manifest, indent=1))
    print(f"\ndone: {n_ok} ok, {n_skip} skipped -> {outdir}/manifest.json")


if __name__ == "__main__":
    main()
