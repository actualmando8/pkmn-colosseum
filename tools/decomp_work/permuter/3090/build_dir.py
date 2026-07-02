#!/usr/bin/env python3
"""build_dir.py — build a decomp-permuter work dir for ONE function on the 3090.

usage: build_dir.py <fn_name> <unit>          (unit e.g. main/game/pokemon)

Produces $FARM/dirs/<fn>/ with:
  base.c        — preprocessed TU (exact per-unit mwcc flags), reduced to <fn>
  target.o      — the dtk split (expected) object for the unit
  compile.sh    — wibo + [sjiswrap] + exact per-unit mwcceppc invocation
  settings.toml — func_name / compiler_type=mwcc / ppc objdump / weights

Pipeline is a port of the previously-proven archive build_dir.sh:
  select_c_branch -> mwcc -E -> drop line markers -> strip_other_fns ->
  scrub_asm_bodies -> asm-qualifier fixup -> smoke compile + symbol check.

Exit codes: 0 ok, 2 preprocess failed, 3 fn missing from compiled base,
            4 fn missing from target.o, 5 smoke compile failed,
            6 fidelity check failed (isolated compile != Mac full-TU compile).

Fidelity gate: the isolated base.c compile must produce the SAME instructions
for <fn> as the Mac's real full-TU build object (build/GC6E01/src/...o, synced
into the repo copy). If stripping siblings broke it (e.g. a same-TU callee that
MWCC inlines), we retry keeping the whole TU; if it still diverges we refuse,
so the farm never anneals against a wrong baseline.
"""
import json
import os
import re
import shlex
import subprocess
import sys

BASE = os.environ.get("FARM_BASE", "/storage/finetune/pkmn-colosseum-2026")
REPO = os.path.join(BASE, "repo")
PERM = os.path.join(BASE, "decomp-permuter")
FARM = os.path.join(BASE, "farm")
WIBO = os.environ.get("WIBO", os.path.expanduser("~/.local/bin/wibo"))
OBJDUMP = "powerpc-linux-gnu-objdump"


def run(cmd, **kw):
    return subprocess.run(cmd, capture_output=True, text=True, **kw)


def norm_disasm(obj, fn):
    """Normalized per-instruction disassembly of fn: ignores addresses, section
    headers, anonymous local reloc numbering (@N) and symbol-target rendering —
    mirrors what the permuter scorer considers significant."""
    r = run([OBJDUMP, "-dr", "-EB", "-mpowerpc", "-M", "broadway",
             f"--disassemble={fn}", obj])
    out = []
    for ln in r.stdout.split("\n"):
        ln = ln.strip()
        if (not ln or ln.startswith("Disassembly") or "file format" in ln
                or ln.endswith(">:")):
            continue
        m = re.match(r"^[0-9a-f]+:\s+(R_PPC\S+)\s+(.*)$", ln)
        if m:
            sym = re.sub(r"@\d+", "@N", m.group(2).strip())
            out.append(f"RELOC {m.group(1)} {sym}")
            continue
        m = re.match(r"^[0-9a-f]+:\s+((?:[0-9a-f]{2} ){4})\s*(.*)$", ln)
        if m:
            insn = re.sub(r"\b[0-9a-f]+ <[^>]*>", "<T>", m.group(2))
            out.append(m.group(1).strip() + " " + insn.strip())
            continue
        out.append(ln)
    return "\n".join(out)


def main():
    fn, unit = sys.argv[1], sys.argv[2]
    units = json.load(open(os.path.join(REPO, "build", "permuter_units_3090.json")))
    u = units[unit]
    mwcc = os.path.join(REPO, "build", "compilers", u["mw_version"], "mwcceppc.exe")
    sjiswrap = os.path.join(REPO, "build", "tools", "sjiswrap.exe")
    outdir = os.path.join(FARM, "dirs", fn)
    os.makedirs(outdir, exist_ok=True)

    # absolutize the -i include paths in the exact ninja cflags
    flags = shlex.split(u["cflags"])
    for i, tok in enumerate(flags):
        if tok == "-i" and i + 1 < len(flags) and not flags[i + 1].startswith("/"):
            flags[i + 1] = os.path.join(REPO, flags[i + 1])

    wrapper = [WIBO] + ([sjiswrap] if u["sjis"] else []) + [mwcc]

    # 1. select the target's C branch into a temp copy (no-op if already C)
    tmpsrc = os.path.join(outdir, "_src.c")
    r = run(["python3", os.path.join(FARM, "select_c_branch.py"), u["src"], fn],
            cwd=REPO)
    if r.returncode != 0 or not r.stdout:
        # fall back to the raw source
        r_stdout = open(os.path.join(REPO, u["src"]), encoding="utf-8", errors="replace").read()
    else:
        r_stdout = r.stdout
    # protect #pragma lines through mwcc -E (which strips them): encode each as
    # a hex-named extern marker that survives preprocessing, function stripping
    # and pycparser round-trips. compile.sh decodes markers back to #pragma at
    # every candidate compile, so codegen pragmas (scheduling/peephole/push/pop)
    # keep their exact original position relative to the target function.
    def protect_pragma(m):
        text = m.group(1).strip()
        return f"extern char __PRGMA_{text.encode('utf-8').hex()}__;"

    r_stdout = re.sub(r"^[ \t]*#[ \t]*pragma[ \t]+(.*)$", protect_pragma,
                      r_stdout, flags=re.M)
    with open(tmpsrc, "w", encoding="utf-8") as f:
        f.write(r_stdout)

    # 2. preprocess with the unit's EXACT flags
    pp = os.path.join(outdir, "_pp.c")
    r = run(wrapper + flags + ["-E", "-o", pp, tmpsrc], cwd=REPO)
    if not (os.path.exists(pp) and os.path.getsize(pp) > 0):
        sys.stderr.write(f"preprocess failed:\n{r.stdout}\n{r.stderr}\n")
        sys.exit(2)

    # 3. drop mwcc line markers
    txt = open(pp, encoding="utf-8", errors="replace").read()
    txt = "\n".join(ln for ln in txt.split("\n") if not ln.startswith("/* #line "))
    open(pp, "w", encoding="utf-8").write(txt)

    # keep a pristine preprocessed copy; isolation attempts start from it
    pp_orig = os.path.join(outdir, "_pp_orig.c")
    subprocess.run(["cp", "-f", pp, pp_orig], check=True)

    # 4. target.o = the dtk split expected object
    target_o = os.path.join(outdir, "target.o")
    subprocess.run(["cp", "-f", os.path.join(REPO, u["target_o"]), target_o], check=True)
    r = run([OBJDUMP, "-t", target_o])
    if not re.search(r"\b" + re.escape(fn) + r"\b", r.stdout):
        sys.stderr.write(f"{fn} not found in target.o symbols\n")
        sys.exit(4)

    # ground truth for the fidelity gate: the Mac's real full-TU build object
    mac_o = os.path.join(REPO, u["base_o"])
    mac_norm = norm_disasm(mac_o, fn) if os.path.exists(mac_o) else None
    if not mac_norm:
        sys.stderr.write(f"warning: no Mac base object for fidelity check ({mac_o})\n")

    base_c = os.path.join(outdir, "base.c")

    def make_base(strip):
        subprocess.run(["cp", "-f", pp_orig, pp], check=True)
        if strip:
            run(["python3", os.path.join(PERM, "strip_other_fns.py"), pp, fn])
        run(["python3", os.path.join(FARM, "scrub_asm_bodies.py"), pp, fn])
        txt = open(pp, encoding="utf-8", errors="replace").read()
        txt = re.sub(r"^[ \t]*asm[ \t]+", "    ", txt, flags=re.M)
        open(base_c, "w", encoding="utf-8").write(txt)

    # 6. compile.sh — permuter invokes: ./compile.sh <in.c> -o <out.o>
    #    Decodes __PRGMA_<hex>__ markers back to #pragma before compiling.
    cmd = " ".join(shlex.quote(c) for c in wrapper + flags)
    decode = ("import re,sys;t=open(sys.argv[1],encoding='utf-8',errors='replace').read();"
              "sys.stdout.write(re.sub(r'extern\\s+char\\s+__PRGMA_([0-9a-f]+)__\\s*;',"
              "lambda m: '#pragma '+bytes.fromhex(m.group(1)).decode(), t))")
    with open(os.path.join(outdir, "compile.sh"), "w") as f:
        f.write("#!/bin/bash\nset -euo pipefail\n"
                'IN="$1"; OUT_O="$3"\n'
                'TMPC="$(mktemp "${TMPDIR:-/tmp}/permcc.XXXXXX.c")"\n'
                "trap 'rm -f \"$TMPC\"' EXIT\n"
                f"python3 -c {shlex.quote(decode)} \"$IN\" > \"$TMPC\"\n"
                f'{cmd} -c -o "$OUT_O" "$TMPC"\n')
    os.chmod(os.path.join(outdir, "compile.sh"), 0o755)

    # 7. settings.toml (weights per archive-proven config)
    with open(os.path.join(outdir, "settings.toml"), "w") as f:
        f.write(f'''func_name = "{fn}"
compiler_type = "mwcc"
objdump_command = "{OBJDUMP} -dr -EB -mpowerpc -M broadway --disassemble={fn}"

[weight_overrides]
perm_reorder_decls = 40
perm_reorder_stmts = 40
perm_dummy_comma_expr = 30
perm_commutative = 20
perm_pad_var_decl = 10
perm_split_assignment = 20
perm_duplicate_assignment = 15
perm_add_self_assignment = 15
perm_refer_to_var = 15
''')

    # 8. smoke + fidelity gate, with fallback: stripped TU -> whole TU
    base_o = os.path.join(outdir, "base_smoke.o")
    last_err = ""
    for strip in (True, False):
        make_base(strip)
        if os.path.exists(base_o):
            os.remove(base_o)
        r = run(["bash", os.path.join(outdir, "compile.sh"), base_c, "-o", base_o])
        if r.returncode != 0 or not os.path.exists(base_o):
            last_err = f"smoke compile failed (strip={strip}):\n{r.stdout[-2000:]}\n{r.stderr[-2000:]}"
            continue
        r = run([OBJDUMP, "-t", base_o])
        if not re.search(r"\b" + re.escape(fn) + r"\b", r.stdout):
            last_err = f"{fn} not in compiled base.c symbols (strip={strip})"
            continue
        if mac_norm is not None and norm_disasm(base_o, fn) != mac_norm:
            last_err = (f"fidelity FAIL (strip={strip}): isolated compile of {fn} "
                        f"differs from Mac full-TU object")
            continue
        os.remove(base_o)
        print(f"built {outdir} (strip={strip}, fidelity={'ok' if mac_norm else 'unchecked'})")
        return
    sys.stderr.write(last_err + "\n")
    sys.exit(6 if "fidelity" in last_err else 5)


if __name__ == "__main__":
    main()
