#!/usr/bin/env python3
"""File-parameterized private-scratch band harness for parallel-agent decomp.

A generalization of cs_band.py (which was hard-wired to colosseum_script.c) so
the same private-scratch / parallel-agent workflow works on ANY big source file.

Each agent works on a PRIVATE scratch copy of the assigned source file
(tools/decomp_work/scratch/band_<tag>.c) so parallel agents never collide on
the shared base object. Compiler version + flags + the immutable target object
are resolved per file from config/GC6E01/compile_config.json (with the toolchain
default as fallback), exactly like the real matching build.

Usage (run from repo root; <tag> is any short agent-private label):
  band.py init <tag> <src/file.c>      create scratch + remember the canon/flags
  band.py check <tag> [fn ...]         compile scratch + per-fn match% (table)
  band.py json  <tag>                  compile scratch + per-fn match% (JSON)
  band.py diff  <tag> <fn>             aligned target-vs-ours asm diff
  band.py save  <tag> <fn> [fn ...]    persist ONLY >=100% defs to band_wins/<tag>.json
  band.py sections <src/file.c> [N]    split file's asm+near-miss fns into N bands

State per tag:
  tools/decomp_work/scratch/band_<tag>.c     private working copy
  tools/decomp_work/scratch/band_<tag>.src   JSON {src, target_o, cflags, compiler}
Wins:
  build/band_wins/<tag>.json                 {fn: verbatim def text} (>=100% only)

Trusted gate: `save` re-compiles + re-measures and refuses anything below 100%.
"""
import json
import os
import shutil
import subprocess
import sys
import collections
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent.parent
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(ROOT / "tools"))
import cs_splice  # noqa: E402  (def-span finder, reused verbatim)
import compile_check  # noqa: E402  (per-file flags/version/target resolution)
import measure_cache  # noqa: E402  (cached objdiff for banding)

# Optional fleet-lock integration: `init` takes a whole-file lock for <tag> so two
# sessions can never grab the same TU; `save` renews it (heartbeat). Best-effort —
# if locks.py is unavailable the harness behaves exactly as before.
try:
    sys.path.insert(0, str(HERE / "coordination"))
    import locks as _locks  # noqa: E402
except Exception:  # pragma: no cover - locks are advisory
    _locks = None


def _lock_acquire_file(tag, src_rel):
    """Take the file lock for <tag>. Warn (don't abort) if another agent owns it —
    re-running init on your own file just renews, but a foreign owner is a real
    collision the operator should see."""
    if _locks is None:
        return
    try:
        r = _locks.acquire(tag, src_rel, scope="file", note=f"band {tag}")
    except Exception as exc:  # never let an advisory lock break the harness
        print(f"  (lock: skipped — {exc})")
        return
    if r.get("ok"):
        lk = r.get("lock", {})
        ttl = lk.get("ttl_remaining")
        print(f"  lock: {r['action']} file '{src_rel}' for '{tag}'"
              + (f" (ttl {ttl}s)" if ttl is not None else ""))
    else:
        owner = r.get("owner")
        print(f"  !! LOCK DENIED: '{src_rel}' is owned by '{owner}', not '{tag}'.")
        print(f"  !! Another session owns this TU -- pick a different file or coordinate.")
        print(f"  !! (override only if you know it is stale: "
              f"python tools/decomp_work/coordination/locks.py release-file {owner} {src_rel} --force)")


def _lock_renew_file(tag, src_rel):
    if _locks is None:
        return
    try:
        _locks.acquire(tag, src_rel, scope="file", note=f"band {tag}")  # re-acquire == renew
    except Exception:
        pass

WINS = ROOT / "build" / "band_wins"
SCRATCH = ROOT / "tools" / "decomp_work" / "scratch"

# Tool resolution is platform-aware so the SAME harness runs on the Windows
# workstation AND a Linux cloud/CI env. On Linux, `configure.py`+`ninja` download
# the platform-appropriate toolchain into build/ (objdiff-cli, wibo, and the mwcc
# compilers), and the Windows mwcceppc.exe is run through the wibo PE loader.
# Windows behaviour is unchanged.
if os.name == "nt":
    MWCC_BASE = ROOT / "tools" / "mwcc_compiler" / "GC"
    OBJDIFF = ROOT / "tools" / "objdiff-cli.exe"
    WIBO = None
else:
    MWCC_BASE = ROOT / "build" / "compilers" / "GC"     # configure.py: build/compilers/GC/<ver>/mwcceppc.exe
    OBJDIFF = ROOT / "build" / "tools" / "objdiff-cli"
    _wibo = ROOT / "build" / "tools" / "wibo"
    WIBO = _wibo if _wibo.exists() else None


# --------------------------------------------------------------------------- #
# Per-tag state                                                                #
# --------------------------------------------------------------------------- #
def scratch_c(tag):
    return SCRATCH / f"band_{tag}.c"


def scratch_o(tag):
    return SCRATCH / f"band_{tag}.o"


def state_path(tag):
    return SCRATCH / f"band_{tag}.src"


def resolve_source(name):
    """Resolve a file stem or path to a tracked src/**.c (mirrors decomp.sh)."""
    p = Path(name)
    if not p.is_absolute():
        cand = ROOT / name
        if cand.exists():
            p = cand
    if p.exists():
        return p.resolve()
    stem = Path(name).stem
    matches = sorted((ROOT / "src").rglob(f"{stem}.c"))
    if matches:
        return matches[0].resolve()
    sys.exit(f"ERROR: cannot resolve source file from {name!r}")


def file_config(src_path):
    """Return (compiler_version, cflags, target_o) for a source file, resolved
    from compile_config.json (with toolchain default fallback)."""
    version = compile_check.get_file_compiler_version(src_path)
    cflags = [str(x) for x in compile_check.get_file_cflags(src_path)]
    target_o = compile_check.find_target_obj(src_path)
    return version, cflags, target_o


def load_state(tag):
    sp = state_path(tag)
    if not sp.exists():
        sys.exit(f"no state for tag {tag!r}; run: band.py init {tag} <src/file.c>")
    return json.loads(sp.read_text(encoding="utf-8"))


# --------------------------------------------------------------------------- #
# Compile + measure                                                            #
# --------------------------------------------------------------------------- #
def compile_band(tag):
    st = load_state(tag)
    src = scratch_c(tag)
    if not src.exists():
        sys.exit(f"scratch file missing; run: band.py init {tag} {st['src']}")
    mwcc_dir = MWCC_BASE / st["compiler"]
    mwcc = mwcc_dir / "mwcceppc.exe"
    if not mwcc.exists():
        sys.exit(f"compiler not found: {mwcc}")
    env = dict(os.environ)
    env["PATH"] = str(mwcc_dir) + os.pathsep + env.get("PATH", "")
    # On Linux the Windows mwcceppc.exe runs through wibo (the PE loader ninja uses).
    cmd = ([str(WIBO)] if WIBO else []) + \
        [str(mwcc), *st["cflags"], "-c", "-o", str(scratch_o(tag)), str(src)]
    r = subprocess.run(cmd, capture_output=True, text=True, env=env, cwd=str(ROOT))
    if r.returncode != 0 or not scratch_o(tag).exists():
        print("COMPILE FAILED")
        print(r.stdout[-4000:])
        print(r.stderr[-4000:])
        sys.exit(1)
    return st


def objdiff_json(tag, st):
    r = subprocess.run(
        [str(OBJDIFF), "diff", "-1", st["target_o"], "-2", str(scratch_o(tag)),
         "-o", "-", "--format", "json",
         "-c", "ppc.calculatePoolRelocations=false"],
        capture_output=True, text=True, cwd=str(ROOT))
    if r.returncode != 0:
        sys.exit("objdiff failed: " + r.stderr[:500])
    return json.loads(r.stdout)


def _rows(tag, st):
    j = objdiff_json(tag, st)
    rows = {}
    for s in j.get("right", {}).get("symbols", []):
        if s.get("kind") == "SYMBOL_FUNCTION" and s.get("name", "").startswith("fn_"):
            rows[s["name"]] = float(s.get("match_percent") or 0.0)
    return rows


# --------------------------------------------------------------------------- #
# Commands                                                                     #
# --------------------------------------------------------------------------- #
def cmd_init(tag, srcname, config_from=None):
    src_path = resolve_source(srcname)
    # Flags/version/target come from `config_from` when given (e.g. integrating a
    # build/band_*_integrated.c temp file whose stem isn't in compile_config.json —
    # resolve its -O4,s/target from the REAL canonical source instead of the
    # default -O4,p fallback). The scratch CONTENT still comes from srcname.
    cfg_path = resolve_source(config_from) if config_from else src_path
    version, cflags, target_o = file_config(cfg_path)
    if not target_o.exists():
        sys.exit(f"target object missing: {target_o}")
    SCRATCH.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(src_path, scratch_c(tag))
    state = {
        "src": str(src_path.relative_to(ROOT)).replace("\\", "/"),
        "src_abs": str(src_path),
        "target_o": str(target_o),
        "cflags": cflags,
        "compiler": version,
    }
    state_path(tag).write_text(json.dumps(state, indent=1), encoding="utf-8")
    print(f"created {scratch_c(tag).relative_to(ROOT)} from {state['src']}")
    print(f"  compiler GC/{version}  target {Path(target_o).name}")
    print(f"  cflags: {' '.join(cflags)}")
    _lock_acquire_file(tag, state["src"])


def cmd_check(tag, fns):
    st = compile_band(tag)
    rows = _rows(tag, st)
    exact = sum(1 for v in rows.values() if v >= 100.0 - 1e-6)
    print(f"compile OK; {exact}/{len(rows)} byte-exact in scratch object")
    for fn in (fns or sorted(rows)):
        if fn in rows:
            mark = "MATCH" if rows[fn] >= 100.0 - 1e-6 else ""
            print(f"  {fn}  {rows[fn]:.2f}%  {mark}")
        else:
            print(f"  {fn}  NOT FOUND in object")


def cmd_json(tag):
    st = compile_band(tag)
    print(json.dumps(_rows(tag, st)))


def _extract(tag, fn):
    """Return the verbatim top-level definition text for fn from the scratch."""
    raw = scratch_c(tag).read_bytes().decode("utf-8", errors="replace")
    nl = "\r\n" if "\r\n" in raw else "\n"
    lines = raw.split(nl)
    span = cs_splice.find_def_span(lines, fn)
    if span is None or isinstance(span, list):
        return None
    return nl.join(lines[span[0]:span[1] + 1])


def cmd_save(tag, fns):
    """Re-measure, then persist verbatim definitions of fns confirmed >=100%
    into build/band_wins/<tag>.json. Rejects anything below 100% (trusted
    ground-truth gate: only true byte-matches are saved for integration)."""
    if not fns:
        sys.exit("usage: band.py save <tag> <fn> [<fn> ...]")
    st = compile_band(tag)
    rows = _rows(tag, st)
    WINS.mkdir(parents=True, exist_ok=True)
    out = WINS / f"{tag}.json"
    data = {}
    if out.exists():
        try:
            data = json.loads(out.read_text(encoding="utf-8"))
        except ValueError:
            data = {}
    saved, rejected = [], []
    for fn in fns:
        pct = rows.get(fn)
        if pct is None:
            rejected.append(f"{fn} (NOT FOUND)")
            continue
        if pct < 100.0 - 1e-6:
            rejected.append(f"{fn} ({pct:.2f}%)")
            continue
        body = _extract(tag, fn)
        if not body:
            rejected.append(f"{fn} (EXTRACT FAILED)")
            continue
        data[fn] = body
        # record THIS fn's source so one tag can hold wins from many files
        # without losing them (band_integrate groups by per-fn _srcs).
        data.setdefault("_srcs", {})[fn] = st["src"]
        saved.append(fn)
    # _src kept for backward-compat (last source); band_integrate prefers _srcs.
    data["_src"] = st["src"]
    out.write_bytes(json.dumps(data, indent=1).encode("utf-8"))
    _lock_renew_file(tag, st["src"])  # heartbeat: keep the file lock alive during long grinds
    print(f"SAVED {len(saved)}: {' '.join(saved) if saved else '-'}")
    if rejected:
        print(f"REJECTED (not 100%): {'; '.join(rejected)}")
    nfn = len([k for k in data if not k.startswith("_")])
    print(f"wins file now holds {nfn} fn(s): {out.relative_to(ROOT)}")


def cmd_diff(tag, fn):
    st = compile_band(tag)
    j = objdiff_json(tag, st)

    def get(side):
        for s in j[side]["symbols"]:
            if s.get("name") == fn:
                return s.get("instructions", [])
        return []

    L, R = get("left"), get("right")

    def fmt(ins):
        i = ins.get("instruction")
        return i.get("formatted", "?") if i else "---"

    print(f"{'TARGET (aim for)':<44} | OURS")
    print("-" * 92)
    for idx in range(max(len(L), len(R))):
        lf = fmt(L[idx]) if idx < len(L) else "---"
        rf = fmt(R[idx]) if idx < len(R) else "---"
        lk = (L[idx].get("diff_kind") or "DIFF_NONE") if idx < len(L) else "X"
        rk = (R[idx].get("diff_kind") or "DIFF_NONE") if idx < len(R) else "X"
        mark = "   " if lk == "DIFF_NONE" and rk == "DIFF_NONE" else ">>>"
        print(f"{mark} {lf:<42} | {rf}")


def _tier(m):
    return "A" if m >= 90 else "B" if m >= 75 else "C" if m >= 50 else "D"


def active_asm_fns(src_path, fn_names):
    """Return the subset of fn_names whose `#include "..._<fn>.inc"` is ACTIVE
    (compiled) in the canon — i.e. an `#if 1` asm-wrapper. These are byte-exact
    in the real ROM; their sub-100% objdiff scores are pure reloc-name/address
    artifacts (jumptable@ha, lbl@sda21 vs raw addr). They are NOT real-C targets,
    so agents must NOT spend attempts on them. Detected by tracking #if 1/#if 0
    branch state around each .inc include (the generated wrappers use literal
    1/0 exclusively)."""
    try:
        lines = src_path.read_text(errors="ignore").splitlines()
    except OSError:
        return set()
    active, stack = set(), []
    for raw in lines:
        s = raw.strip()
        if s.startswith("#if"):
            stack.append(s[3:].strip() == "1")          # #if 1 -> True, else False
        elif s.startswith("#else"):
            if stack:
                stack[-1] = not stack[-1]
        elif s.startswith("#endif"):
            if stack:
                stack.pop()
        elif "#include" in s and s.rstrip().endswith('.inc"'):
            if stack and all(stack):                      # include is in a live branch
                stem = s.split('"')[1].rsplit("/", 1)[-1][:-4]   # filename minus .inc
                for fn in fn_names:
                    if stem == fn or stem.endswith("_" + fn):
                        active.add(fn)
                        break
    return active


def cmd_sections(srcname, nbands):
    """List the file's active-asm + near-miss (<100%) fns split into N disjoint
    bands (tiered A/B/C/D, closest-to-100% first), one band per agent. Reuses
    cs_make_wave banding logic: rank near-misses by descending match%, group by
    tier, chunk into N bands. Measured the same way the matching build measures:
    compile the canon to its base .o, then objdiff vs the immutable target."""
    src_path = resolve_source(srcname)
    version, cflags, target_o = file_config(src_path)
    if not target_o.exists():
        sys.exit(f"target object missing: {target_o}")

    # Compile the canonical file to its base .o (compile_check handles flags +
    # version from compile_config.json), then diff vs the immutable target.
    base_o = compile_check.compile_source(src_path, verbose=False)
    funcs = measure_cache.diff_funcs(target_o, base_o)
    measure_cache.flush()
    if funcs is None:
        sys.exit("objdiff failed measuring the canonical file")

    pct = {x["name"]: x["match"] for x in funcs}
    sz = {x["name"]: x["size"] for x in funcs}
    near = sorted([x for x in funcs if x["match"] < 99.9999],
                  key=lambda x: -x["match"])
    rel = str(src_path.relative_to(ROOT)).replace("\\", "/")

    # Drop active asm-wrappers: they are byte-exact in ROM (sub-100% = reloc-name
    # artifacts only). Agents must not spend attempts on them.
    asm = active_asm_fns(src_path, {x["name"] for x in near})
    pool = [x["name"] for x in near if x["name"] not in asm]

    if not pool:
        print(f"{rel}: all {len(funcs)} fn(s) byte-exact or active asm-wrapper "
              f"({len(asm)} wrapper artifact(s)) — no real-C targets to band.")
        return

    # Tier then chunk into nbands disjoint bands. Distribute tier chunks
    # round-robin so every band gets roughly equal work across tiers.
    byT = collections.defaultdict(list)
    for fn in pool:
        byT[_tier(pct[fn])].append(fn)
    ordered = []
    for T in ("A", "B", "C", "D"):
        ordered.extend(byT[T])
    nbands = max(1, int(nbands))
    buckets = [[] for _ in range(nbands)]
    for i, fn in enumerate(ordered):
        buckets[i % nbands].append(fn)

    byte_exact = sum(1 for v in pct.values() if v >= 99.9999) + (len(funcs) - len(pct))
    print(f"file: {rel}")
    print(f"compiler GC/{version}   target {Path(target_o).name}")
    print(f"{len(funcs)} fn(s); {byte_exact} byte-exact; {len(asm)} asm-wrapper "
          f"artifact(s) [skipped]; {len(pool)} real-C near-miss across "
          f"{nbands} band(s)\n")
    for bi, names in enumerate(buckets):
        names = sorted(names, key=lambda fn: -pct[fn])
        tag = f"b{bi}"
        print(f"=== band {tag}  ({len(names)} fns) ===")
        for fn in names:
            print(f"  {fn}  {pct[fn]:6.2f}%  sz={sz.get(fn,0)}")
    print(f"\nEach agent: band.py init <tag> {rel}  then work ONLY its band's fns.")


# --------------------------------------------------------------------------- #
def main():
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    cmd = sys.argv[1]
    if cmd == "sections":
        if len(sys.argv) < 3:
            sys.exit("usage: band.py sections <src/file.c> [N]")
        nbands = sys.argv[3] if len(sys.argv) > 3 else 4
        cmd_sections(sys.argv[2], nbands)
        return
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    tag = sys.argv[2]
    if cmd == "init":
        if len(sys.argv) < 4:
            sys.exit("usage: band.py init <tag> <src/file.c> [--config-from <src>]")
        config_from = None
        if "--config-from" in sys.argv:
            i = sys.argv.index("--config-from")
            config_from = sys.argv[i + 1] if i + 1 < len(sys.argv) else None
        cmd_init(tag, sys.argv[3], config_from)
    elif cmd == "check":
        cmd_check(tag, sys.argv[3:])
    elif cmd == "json":
        cmd_json(tag)
    elif cmd == "save":
        cmd_save(tag, sys.argv[3:])
    elif cmd == "diff":
        if len(sys.argv) < 4:
            sys.exit("usage: band.py diff <tag> <fn>")
        cmd_diff(tag, sys.argv[3])
    else:
        sys.exit(__doc__)


if __name__ == "__main__":
    main()
