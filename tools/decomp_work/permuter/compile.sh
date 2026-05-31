#!/bin/bash
# compile.sh — decomp-permuter compile wrapper for Pokemon Colosseum (GPXE01).
#
# decomp-permuter invokes: ./compile.sh <C_FILE> -o <OUT_O>
#   $1 = input .c    $2 = "-o"    $3 = output .o
#
# Runs the vendored MetroWerks compiler mwcceppc.exe (a native Windows PE)
# through WSL<->Windows interop with the EXACT project flags. Paths handed to
# the .exe are converted to Windows form with `wslpath -w`.
#
# Requires: run under WSL (so powerpc-linux-gnu-objdump, the scorer's
# disassembler, is available natively).
set -euo pipefail

REPO="/mnt/c/Users/douglaswhittingham/pkmn-colosseum"
MWCC="$REPO/tools/mwcc_compiler/GC/1.3/mwcceppc.exe"
INCLUDE="$REPO/include"

IN_C="$1"
OUT_O="$3"

# Project's exact compile flags (must match compile_config.json default for
# gs_field_world.c, which uses GC/1.3).
FLAGS=(-O4,p -nodefaults -proc gekko -fp hard -Cpp_exceptions off \
       -enum int -warn off -use_lmw_stmw on -sdata 8 -sdata2 8)

# mwcceppc.exe is a Windows binary: feed it Windows paths.
WIN_IN="$(wslpath -w "$IN_C")"
WIN_OUT="$(wslpath -w "$OUT_O")"
WIN_INC="$(wslpath -w "$INCLUDE")"

# -c compile only; emit object directly (no separate assembler step needed).
"$MWCC" "${FLAGS[@]}" -i "$WIN_INC" -c -o "$WIN_OUT" "$WIN_IN"
