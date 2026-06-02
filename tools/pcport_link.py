#!/usr/bin/env python3
"""pcport_link.py — drive the PC-port bootstrap to LINK and RUN.

Compiles the bootstrap + HSD sources (generated copies where asm-blocked) with
clang -m32, links against the CMake-built shim/glfw/glad libs, then auto-generates
stub definitions for every unresolved symbol (fn_* -> no-op function, lbl_* -> data
bytes, others best-effort) and re-links. This gets a launching exe before every TU
is ported; stubs are replaced by real code incrementally.

Run:  python tools/pcport_link.py            # build + link + (on success) run --window-smoke
"""
import subprocess, re, sys, os
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CLANG = r"C:\Program Files\LLVM\bin\clang.exe"
OBJ = ROOT / "build_pc" / "obj"
GEN = ROOT / "build_pc" / "gen"
OBJ.mkdir(parents=True, exist_ok=True)
INC = ["-Iinclude", "-Iinclude/dolphin", "-Iinclude/game", "-Iinclude/hsd",
       "-Isrc/pcport", "-Ithird_party", "-Ithird_party/glad/include",
       "-Ibuild_pc/_deps/glfw-src/include"]   # GLFW headers (FetchContent) + stb_image (third_party)
COMPAT = ["-include", "src/pcport/pcport_compat.h"]
# -fms-compatibility/-fms-extensions: emulate MSVC leniency (e.g. u32 = long-vs-int
# typedef redefinitions the decomp+SDK headers disagree on) so the same TUs that
# the CMake/MSVC build compiles also compile here.
CFLAGS = ["-m32", "-c", "-DPCPORT=1", "-w", "-O1",
          "-fms-compatibility", "-fms-extensions"] + COMPAT + INC
LIBS = [
    # project libs by full path (not in standard search dirs)
    str(ROOT / "build_pc/Debug/pcport_shim.lib"),
    str(ROOT / "build_pc/Debug/glad.lib"),
    str(ROOT / "build_pc/_deps/glfw-build/src/Debug/glfw3.lib"),
    # system libs via -l (clang/lld-link resolves against the auto-detected SDK paths)
    "-lopengl32", "-luser32", "-lgdi32", "-lshell32", "-lkernel32",
]

# bootstrap sources (== CMake add_executable list)
BOOT = ["src/pcport/pcport_main.c", "src/pcport/gs_gfx_host_support.c",
        "src/pcport/real_content_host.c", "src/pcport/hsd_host.c",
        "src/pcport/thp_player.c",
        "src/pcport/os_thread_host.c", "src/pcport/engine_host.c",
        "src/pcport/engine_spike.c", "src/pcport/gs_sched_host.c",
        "src/pcport/engine_boot.c",
        "src/dolphin/vi/VI.c",
        "src/dolphin/os/OSStateFlags.c", "src/trk/TRKUtil.c",
        "src/hsd/hsd_pobj_disp.c", "src/game/gs_gfx.c", "src/game/gs_render.c"]
HSD_CLEAN = ["hsd_fobj", "hsd_mobj_ext", "hsd_mtx", "hsd_pobj", "hsd_pobj_ext",
             "hsd_robj", "hsd_shadow", "hsd_state", "hsd_tev", "hsd_texp",
             "hsd_tobj", "hsd_tobj_ext"]
HSD_GEN = ["hsd_dobj", "hsd_jobj", "hsd_mobj", "hsd_aobj", "hsd_lobj", "hsd_object",
           "hsd_class", "hsd_initialize", "hsd_wobj", "hsd_displayfunc",
           "hsd_render", "hsd_memory_ext", "hsd_util", "hsd_jobj_display", "hsd_cobj"]


def compile_one(src, name):
    o = OBJ / f"{name}.o"
    r = subprocess.run([CLANG, *CFLAGS, str(src), "-o", str(o)],
                       cwd=ROOT, capture_output=True, text=True)
    return (o if r.returncode == 0 else None), r.stderr


def gen_hsd():
    srcs = [f"src/hsd/{n}.c" for n in HSD_GEN]
    subprocess.run([sys.executable, "tools/pcport_gen.py", "--out-dir", "build_pc/gen", *srcs],
                   cwd=ROOT, capture_output=True)


def link(objs, exe, extra=None):
    cmd = [CLANG, "-m32", *[str(o) for o in objs]]
    if extra: cmd.append(str(extra))
    # /FORCE:MULTIPLE: gs_gfx_host_support.c had placeholder defs that the real HSD
    # TUs now provide — allow the duplicates (linker takes the first) so we get a
    # launching exe; the redundant host stubs get cleaned up next.
    cmd += LIBS + ["-o", str(exe), "-Xlinker", "/SUBSYSTEM:CONSOLE",
                   "-Xlinker", "/FORCE:MULTIPLE", "-Xlinker", "/IGNORE:4006,4088",
                   "-Xlinker", "/ERRORLIMIT:0"]   # report ALL undefined, not just 20
    return subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)


UNDEF_RE = re.compile(r'undefined symbol:\s+_?([A-Za-z_]\w*)')


def parse_undef(stderr):
    return sorted(set(UNDEF_RE.findall(stderr)))


def make_stubs(symbols):
    lines = ["/* AUTO-GENERATED stubs for not-yet-ported symbols (pcport_link.py). */",
             "typedef unsigned int u32;"]
    for s in symbols:
        if s.startswith("fn_"):
            lines.append(f"int {s}(){{return 0;}}")
        elif s.startswith("lbl_"):
            lines.append(f"char {s}[0x4000];")
        else:
            # unknown: emit BOTH would conflict; guess data for known-data names else fn
            lines.append(f"int {s}(){{return 0;}}")
    (ROOT / "build_pc/gen/pcport_stubs.c").write_text("\n".join(lines) + "\n")
    return ROOT / "build_pc/gen/pcport_stubs.c"


def main():
    gen_hsd()
    objs, failed = [], []
    for src in BOOT:
        o, err = compile_one(ROOT / src, Path(src).stem)
        (objs if o else failed).append(o or (src, err))
    for n in HSD_CLEAN:
        o, err = compile_one(ROOT / f"src/hsd/{n}.c", n)
        (objs.append(o) if o else failed.append((n, err)))
    for n in HSD_GEN:
        o, err = compile_one(GEN / f"hsd/{n}.c", n)
        (objs.append(o) if o else failed.append((n, err)))
    objs = [o for o in objs if o]
    print(f"compiled {len(objs)} objects; {len(failed)} failed to compile: "
          f"{[f[0] if isinstance(f, tuple) else f for f in failed]}")

    exe = ROOT / "build_pc/pcport_bootstrap.exe"
    all_undef, so = set(), None
    for rnd in range(1, 9):
        if exe.exists():
            exe.unlink()
        r = link(objs + ([so] if so else []), exe)
        if r.returncode == 0 and exe.exists():
            print(f"round {rnd}: LINKED OK ({len(all_undef)} symbols stubbed)")
            print(f"\n=== BUILT {exe} ===")
            return
        undef = parse_undef(r.stderr)
        if not undef:
            print(f"round {rnd}: link failed with NO undefined symbols (other error):\n"
                  + (r.stderr or r.stdout)[-1500:])
            return
        new = set(undef) - all_undef
        print(f"round {rnd}: rc={r.returncode}, {len(undef)} undefined ({len(new)} new) -> stubbing")
        if not new:
            print("no new undefined but still failing; residual:", sorted(undef)[:20]); return
        all_undef |= set(undef)
        stub_src = make_stubs(sorted(all_undef))
        so, serr = compile_one(stub_src, "pcport_stubs")
        if not so:
            print("STUB COMPILE FAILED:\n" + serr[:1500]); return
    print("did not converge in 8 rounds; stubbed", len(all_undef))


if __name__ == "__main__":
    main()
