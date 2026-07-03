#!/usr/bin/env python3

###
# Pokémon Colosseum (GC6E01) — dtk-template build configuration.
#
# Generates build.ninja and objdiff.json from the project configuration via the
# canonical decomp-toolkit pipeline (tools/project.py). The build splits the DOL
# into relocatable objects with dtk, links them back (substituting matching C
# objects where declared), and verifies the result against config/.../build.sha1.
#
# Usage:
#   python configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import sys
from pathlib import Path
from typing import Any, Dict, List

from tools.project import (
    Object,
    ProgressCategory,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
DEFAULT_VERSION = 0
VERSIONS = [
    "GC6E01",  # 0 — NTSC-U Rev 0
]

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "-v",
    "--version",
    choices=VERSIONS,
    type=str.upper,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="DIR",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (default: tools/dtk.exe)",
)
parser.add_argument(
    "--objdiff",
    metavar="BINARY | DIR",
    type=Path,
    help="path to objdiff-cli binary or source (default: tools/objdiff-cli.exe)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--ninja",
    metavar="BINARY",
    type=Path,
    help="path to ninja binary (optional)",
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
parser.add_argument(
    "--non-matching",
    dest="non_matching",
    action="store_true",
    help="builds equivalent (but non-matching) or modded objects",
)
parser.add_argument(
    "--no-progress",
    dest="progress",
    action="store_false",
    help="disable progress calculation",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = str(args.version)
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.generate_map = args.map
config.non_matching = args.non_matching
config.ninja_path = args.ninja
config.progress = args.progress
if not is_windows():
    config.wrapper = args.wrapper
# Don't build asm unless we're --non-matching
if not config.non_matching:
    config.asm_dir = None

# Tool versions (used as download fallbacks when the local path is absent).
config.binutils_tag = "2.42-2"
config.dtk_tag = "v1.8.3"
config.compilers_tag = "20251118"
config.objdiff_tag = "v3.6.1"
config.sjiswrap_tag = "v1.2.2"
config.wibo_tag = "1.0.3"

# dtk, objdiff-cli and the Metrowerks compilers are all left unset so project.py
# downloads the pinned, PLATFORM-APPROPRIATE binaries (dtk_tag / objdiff_tag /
# compilers_tag) into build/ — the canonical dtk-template behavior, and what makes
# the Linux CI work from the same config as Windows. --dtk/--objdiff/--compilers
# override with a local copy.
config.binutils_path = args.binutils
config.dtk_path = args.dtk
config.objdiff_path = args.objdiff
config.compilers_path = args.compilers
config.sjiswrap_path = args.sjiswrap

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I build/{config.version}/include",
    f"--defsym BUILD_VERSION={version_num}",
]
# The retail DOL matches when linked with the project linker-script override and
# no extra mwldeppc flags.
config.ldflags = []
if args.map:
    config.ldflags.append("-mapunused")

# Use for any additional files that should cause a re-configure when modified
config.reconfig_deps = []

# Base CodeWarrior flags, common to most GC games. Per-object overrides live in
# the libs/objects below. The byte-match linker is GC/1.2.5n (see ra/mwldeppc.exe).
config.linker_version = "GC/1.2.5n"

cflags_base = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hard",
    "-Cpp_exceptions off",
    "-O4,p",
    "-inline auto",
    '-pragma "cats off"',
    "-nosyspath",
    "-RTTI off",
    "-fp_contract on",
    "-str reuse",
    "-multibyte",
    "-i include",
    f"-i build/{config.version}/include",
    f"-DBUILD_VERSION={version_num}",
    f"-DVERSION_{config.version}",
]

if args.debug:
    cflags_base.extend(["-sym on", "-DDEBUG=1"])
else:
    cflags_base.append("-DNDEBUG=1")

Matching = True                   # Object matches and should be linked
NonMatching = False               # Object does not match and should not be linked
Equivalent = config.non_matching  # Linked only when configured with --non-matching
DataCandidate = NonMatching       # Compared by objdiff, but not linked yet
CodeCandidate = NonMatching       # Compared by objdiff, but not linked yet


def GameLib(lib_name: str, mw_version: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": mw_version,
        "cflags": cflags_base,
        "progress_category": "game",
        "objects": objects,
    }


# Undeclared units link the dtk-extracted object as-is, reproducing the original
# DOL. Matching source objects are declared here and substituted in.
config.warn_missing_config = False
config.warn_missing_source = False
config.libs = [
    GameLib(
        "Runtime.PPCEABI.H",
        "GC/1.2.5n",
        [
            Object(CodeCandidate, "trk/ddh_cc_range_800C3E90.c", mw_version="GC/1.3", progress_category="runtime"),  # CALIB_TRK
            Object(CodeCandidate, "hsd/hsd_mobj_range_801A8478.c", mw_version="GC/1.3", progress_category="hsd"),  # CALIB_HSD1
            Object(CodeCandidate, "hsd/hsd_mobj_range_801A84B4.c", mw_version="GC/1.3", progress_category="hsd"),  # BANK_HSD_VECINIT
            Object(CodeCandidate, "hsd/hsd_mtx.c", mw_version="GC/1.3", progress_category="hsd"),  # CALIB_HSD2
            Object(CodeCandidate, "crt/stdio_range_800C7558.c", mw_version="GC/1.3", progress_category="runtime"),  # CALIB_CRT
            Object(CodeCandidate, "trk/ddh_cc_range_800C3C00.c", mw_version="GC/1.3", progress_category="runtime"),  # BANK_TRK
            Object(CodeCandidate, "trk/gdev_cc_range_800C41AC.c", mw_version="GC/1.3", progress_category="runtime"),  # BANK_TRK
            Object(CodeCandidate, "trk/gdev_cc_range_800C4444.c", mw_version="GC/1.3", progress_category="runtime"),  # BANK_TRK
            Object(CodeCandidate, "hsd/hsd_mobj_range_801A86B4.c", mw_version="GC/1.3", progress_category="hsd"),  # BANK_HSD
            Object(CodeCandidate, "hsd/hsd_pobj_range_801AA608.c", mw_version="GC/1.3", progress_category="hsd"),  # BANK_HSD_POBJ
            Object(CodeCandidate, "trk/TRKTarget_range_800C1310.c", mw_version="GC/1.3", progress_category="runtime"),  # BANK_TRK2
            Object(CodeCandidate, "trk/TRKComm_range_800C3678.c", mw_version="GC/1.3", progress_category="runtime"),  # BANK_TRK3
            Object(CodeCandidate, "trk/TRKNub_range_800BE47C.c", mw_version="GC/1.3", progress_category="runtime"),  # BANK_TRK3
            Object(CodeCandidate, "trk/TRKNub_range_800BEE74.c", mw_version="GC/1.3", progress_category="runtime"),  # BANK_TRK3
            Object(CodeCandidate, "trk/TRKDispatch_range_800C0CD8.c", mw_version="GC/1.3", progress_category="runtime"),  # BANK_TRK3
            Object(CodeCandidate, "trk/TRKTarget_range_800C1348.c", mw_version="GC/1.3", progress_category="runtime"),  # BANK_TRK3
            Object(CodeCandidate, "trk/TRKInit.c", mw_version="GC/1.3", progress_category="runtime"),  # BANK_TRK3
            Object(CodeCandidate, "trk/TRKBoard_range_800C33BC.c", mw_version="GC/1.3", progress_category="runtime"),  # BANK_TRK3
            Object(CodeCandidate, "crt/printf.c", mw_version="GC/1.3", progress_category="runtime"),  # BANK_CRT_PRINTF
            Object(
                NonMatching,
                "__init_cpp_exceptions.cpp",
                source="crt_data/__init_cpp_exceptions.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "crt/mem.c",
                mw_version="GC/2.0",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "crt/string.c",
                mw_version="GC/1.3",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "crt/wchar.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "crt/mwtrace_helpers.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "crt/critical_regions.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "crt/stdio_atexit.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "dolphin/dvd/DVDFsExtras.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/dvd/DVDFs.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/dvd/DVDQueue.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/dvd/DVDError.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/dvd/DVDLowInitWA.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/dvd/DVDLowSetWAType.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/exi/EXI2Stubs.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/vi/VI_fn_800AA280.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/vi/VI_fn_800AA498.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/pad/PAD.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/exi/EXI2_range_800CEA3C.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/gx/GX_fn_800B71F0.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/gx/GX_fn_800B770C.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/gx/GX_fn_800B7714.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/gx/GX_fn_800B856C.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/gx/GX_fn_800BA198.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/gx/GX_fn_800BA414.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/gx/GX_fn_800BA424.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/gx/GX_fn_800BA440.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/gx/GX_fn_800BAE5C.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/gx/GX_fn_800BB2E4.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/gx/GX_fn_800BB2F8.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/os/PPCArch.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/os/OSGetExceptionHandler.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/os/OSAlarmCreate.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/os/OSArena.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/os/OSContextCurrent.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/os/OSContextClear.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/os/OSInterruptHandlers.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/os/OSEXI_fn_8009E7A8.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/os/OSEXI_fn_8009E7AC.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/os/OSState_fn_8009FAEC.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/os/OSReboot_fn_800A064C.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/os/OSReboot_WriteSram.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/os/OSThreadQueue.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/os/OSTime.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/db/DBInit.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/db/DBGetFirstCallback.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/db/DBIsExceptionMarked.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/db/DBPrintf.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/si/SI_fn_800CF708.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/si/SI_fn_800CF728.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/si/SITypeDecode.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/si/SI_fn_800D0F44.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/si/SI_fn_800D104C.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "game/menu/menu_bag.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/sound/sound.c",
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/effect/gs_effect.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/effect/effect_util.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/effect/effect_visual.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/item_range_80144574.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "musyx/runtime/seq.c",
                mw_version="GC/1.3.2",
                extra_cflags=["-use_lmw_stmw off", "-sdata 8", "-sdata2 8"],
                progress_category="musyx",
            ),
            Object(
                CodeCandidate,
                "musyx/runtime/synth.c",
                mw_version="GC/1.3.2",
                extra_cflags=["-use_lmw_stmw off", "-sdata 8", "-sdata2 8"],
                progress_category="musyx",
            ),
            Object(
                CodeCandidate,
                "musyx/runtime/seq_api.c",
                mw_version="GC/1.3.2",
                extra_cflags=["-use_lmw_stmw off", "-sdata 8", "-sdata2 8"],
                progress_category="musyx",
            ),
            Object(
                CodeCandidate,
                "musyx/runtime/snd_synthapi.c",
                mw_version="GC/1.3.2",
                extra_cflags=["-use_lmw_stmw off", "-sdata 8", "-sdata2 8"],
                progress_category="musyx",
            ),
            Object(
                CodeCandidate,
                "musyx/runtime/stream.c",
                mw_version="GC/1.3.2",
                extra_cflags=["-use_lmw_stmw off", "-sdata 8", "-sdata2 8"],
                progress_category="musyx",
            ),
            Object(
                CodeCandidate,
                "musyx/runtime/synthdata.c",
                mw_version="GC/1.3.2",
                extra_cflags=["-use_lmw_stmw off", "-sdata 8", "-sdata2 8"],
                progress_category="musyx",
            ),
            Object(
                CodeCandidate,
                "musyx/runtime/synthmacros.c",
                mw_version="GC/1.3.2",
                extra_cflags=["-use_lmw_stmw off", "-sdata 8", "-sdata2 8"],
                progress_category="musyx",
            ),
            Object(
                CodeCandidate,
                "musyx/musyx_range_80157280.c",
                mw_version="GC/1.3.2",
                extra_cflags=["-use_lmw_stmw off", "-sdata 8", "-sdata2 8"],
                progress_category="musyx",
            ),
            Object(
                CodeCandidate,
                "game/people/people_data.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/trainer.c",
                mw_version="GC/1.3",
                extra_cflags=["-O4,s", "-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/pokemon.c",
                mw_version="GC/1.3",
                extra_cflags=["-O4,s", "-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/colosseum_event.c",
                mw_version="GC/1.3",
                extra_cflags=["-O4,s", "-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/battle/battle_waza.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_model.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_field_world.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_range_8000D290.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_range_80011EA4.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_range_80033278.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_range_8003686C.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/menu/cardesavedata.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/pokeconv.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gbaCommunication.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_80098108.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_8009A0F4.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_8009A2D8.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_8009AFD0.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_8009BD84.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_8009E7B0.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_8009F77C.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_800A03B4.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_800A07C4.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_800A2B9C.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_800A2D38.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/dvd/dvdfs_range_800A4D28.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_800A7820.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/vi/VI_range_800A8178.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_800AA288.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_800AC02C.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_800B71FC.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_800B771C.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_800B857C.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_800BA1B4.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_800BA44C.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_800BAE64.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_800BB30C.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_800BF33C.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "trk/trk_range_800C3EBC.c",
                mw_version="GC/1.3",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "trk/trk_range_800C4470.c",
                mw_version="GC/1.3",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "game/gs_range_800C45A0.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_800C470C.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_800C5458.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "crt/math_range_800CAA58.c",
                mw_version="GC/1.3",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_800CE7DC.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/sdk_range_800CEB64.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "game/gs_range_800D1070.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_range_800E202C.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_range_80101910.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_range_80109C88.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_range_8010CBD0.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/field_range_801140DC.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/field_range_80114AE0.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/musyx_range_801652DC.c",
                mw_version="GC/1.3.2",
                extra_cflags=["-use_lmw_stmw off", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_range_801653CC.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/ps_range_80168C64.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_range_8017A5FC.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_range_8017F2C4.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_range_801920E4.c",
                mw_version="GC/1.3",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_memory.c",
                mw_version="GC/1.3",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_range_801B0158.c",
                mw_version="GC/1.3",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_tobj.c",
                mw_version="GC/1.3",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "game/fight_range_80201764.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_range_80265EC4.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_range_80009178.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_range_800096B4.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_range_8000BE74.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/fight_range_80051710.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/menu/menuCB_Battle.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/menu/menu_range_8007109C.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/menu/menu_poke_coupon.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "dolphin/os/OSContext_range_8009B914.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/si/SI_range_800CF764.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "game/gs_range_8018FE30.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/effect/fade_range_801C4CB8.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_range_801C766C.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/field_range_801CA7EC.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/field_range_801CB180.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/battle/battle_range_801D0AA0.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_range_801DE698.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/field_range_801DF790.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_range_801E09E0.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "dolphin/thp/THP_range_801E1B54.c",
                mw_version="GC/1.2.5n",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "game/field_range_801ECFE0.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/battle/battle_range_801ED640.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "crt/__start.c",
                mw_version="GC/1.3.2",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "game/fight_range_80211A00.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/colosseum_battle.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/battle/battle_main.c",
                mw_version="GC/1.3",
                extra_cflags=["-O4,s", "-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/battle/battle_grid.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_party_access.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gba/gba_misc.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gba/gba_conv.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_field_colquery.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_colsys.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_field_resource.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_render_util.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_event_exec.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_npc_interact.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/menu/menu_middle.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_thread.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_thread_hi.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_task.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_npc_event.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_pokemon_summary.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/main.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_texture.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/input/input.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/movie.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_gfx.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_dobj.c",
                mw_version="GC/1.3",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_wobj.c",
                mw_version="GC/1.3",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_fog.c",
                mw_version="GC/1.3",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_initialize.c",
                mw_version="GC/1.3",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_class.c",
                mw_version="GC/1.3",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_object.c",
                mw_version="GC/1.3",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_displayfunc.c",
                mw_version="GC/1.3",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "dolphin/os/OSMemory.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "game/gs_render.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_worldmap.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_title.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8", "-opt nopeephole"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_scene.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/fsys/fsys_file.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/people/people.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_pcbox.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_cobj.c",
                mw_version="GC/1.3",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_jobj.c",
                mw_version="GC/1.3",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_lobj.c",
                mw_version="GC/1.3",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "game/gs_dvd.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_floor_data.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/menu/menu_carde_matrix.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/gs_floor.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "game/effect/tracefx.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="game",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_shadow.c",
                mw_version="GC/1.3",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_util.c",
                mw_version="GC/1.3",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_video.c",
                mw_version="GC/1.3",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_mobj.c",
                mw_version="GC/1.3",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_robj_range_801ADD48.c",
                mw_version="GC/1.3",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_texp.c",
                mw_version="GC/1.3",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_aobj_range_801C01C8.c",
                mw_version="GC/1.3",
                extra_cflags=["-use_lmw_stmw on", "-sdata 8", "-sdata2 8"],
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "dolphin/os/OSCache.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/dvd/DVD.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "dolphin/os/OSReset.c",
                progress_category="sdk",
            ),
            Object(
                CodeCandidate,
                "crt/exit.c",
                mw_version="GC/1.3.2",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "crt/global_destructor_chain.c",
                mw_version="GC/1.3",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_pobj.c",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_mobj_clear_flags.c",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_mobj_set_flags.c",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_mobj_get_flags.c",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_mobj_set_current.c",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_mtx_get_alloc_data.c",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_vec_get_alloc_data.c",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_mtx_scaled_add.c",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_pobj_empty.c",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_robj_update_func.c",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_robj_find_by_type.c",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_robj_get_alloc_data.c",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "hsd/hsd_robj_get_alloc_data2.c",
                progress_category="hsd",
            ),
            Object(
                CodeCandidate,
                "trk/TRKConstructEvent.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "trk/TRKBufferReset.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "trk/TRKGetBuffer.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "trk/TRKTerminateSerialHandler.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "trk/TRKSerial.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "trk/usr_put_initialize.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "trk/TRKDispatchInit.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "trk/TRKDispatchConnected.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "trk/TRKDispatchMutex.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "trk/TRKTargetState.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "trk/TRKTargetStopped.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "trk/TRKBoard.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "trk/TRKCommState.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "trk/TRKComm.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "trk/udp_cc.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "trk/ddh_cc_close.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "trk/ddh_cc_shutdown.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "trk/circle_buffer_count.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "trk/gdev_cc_close.c",
                progress_category="runtime",
            ),
            Object(
                CodeCandidate,
                "trk/gdev_cc_shutdown.c",
                progress_category="runtime",
            ),
            Object(
                DataCandidate,
                "crt/sdata2_math.c",
                source="crt_data/sdata2_math.c",
                progress_category="runtime",
            ),
            Object(
                DataCandidate,
                "game/data/rodata_80267060.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/rodata_802663A0.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/rodata_80266BD8.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/rodata_80266C7C.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/rodata_80266D78.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/rodata_80267250.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/rodata_80267350.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/rodata_80268424.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047B6A0.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047B7A0.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047B8A0.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047B9A0.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047BAA0.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047BBA0.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047BCA0.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047BDA0.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047BEA0.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047BFA0.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047C0A0.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047C1A0.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047C2A0.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047C3A0.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "crt/sdata2_math_8047C8A0.c",
                source="crt_data/sdata2_math_8047C8A0.c",
                progress_category="runtime",
            ),
            Object(
                DataCandidate,
                "game/gs_render_util_sdata2.c",
                source="game/gs_render_util_sdata2.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047C9A0.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047CAA0.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047CBE0.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047CC98.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/gs_model_sdata2_8047CD98.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047CE98.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047CF98.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047D098.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/effect/effect_visual_sdata2_8047D198.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/effect/effect_visual_sdata2_8047D298.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047D398.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047D498.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047D690.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/people/people_sdata2_8047D790.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047D890.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "hsd/hsd_sdata2_8047D990.c",
                progress_category="hsd",
            ),
            Object(
                DataCandidate,
                "hsd/hsd_sdata2_8047DA90.c",
                progress_category="hsd",
            ),
            Object(
                DataCandidate,
                "hsd/hsd_sdata2_8047DB90.c",
                progress_category="hsd",
            ),
            Object(
                DataCandidate,
                "hsd/hsd_sdata2_8047DC90.c",
                progress_category="hsd",
            ),
            Object(
                DataCandidate,
                "hsd/hsd_sdata2_8047DD90.c",
                progress_category="hsd",
            ),
            Object(
                DataCandidate,
                "hsd/hsd_sdata2_8047DE90.c",
                progress_category="hsd",
            ),
            Object(
                DataCandidate,
                "game/battle_sdata2_8047DF90.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/battle_sdata2_8047E090.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/battle_sdata2_8047E190.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/battle_waza_sdata2_8047E290.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047E390.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047E490.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/data/sdata2_8047E538.c",
                progress_category="game",
            ),
            Object(
                DataCandidate,
                "game/colosseum_battle_sdata2.c",
                progress_category="game",
            ),
        ],
    ),
]

config.progress_categories = [
    ProgressCategory("game", "Game Code"),
    ProgressCategory("sdk", "Dolphin SDK Code"),
    ProgressCategory("hsd", "HSD/sysdolphin (Third Party)"),
    ProgressCategory("musyx", "MusyX (Third Party)"),
    ProgressCategory("runtime", "Gekko Runtime Code"),
]
config.progress_each_module = args.verbose

if args.mode == "configure":
    generate_build(config)
elif args.mode == "progress":
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
