# GC6E01 Porting Roadmap — Portable Library Recovery

**Scope:** portable, reference-backed library code only. Game-logic units (fight AI, colosseum_battle, gs_*, menu, people, people_field item-prefix, gbaCommunication) are excluded — they belong to the Fable type-recovery track.

**Portable universe (unmatched code bytes):**

| Track | Unmatched B | Compiler | Reference | Compiler confidence |
|---|---|---|---|---|
| Dolphin SDK (88 units) | 216,748 | GC/1.2.5n | GC SDK 2.3 / same-era dtk decomps | HIGH (SI/PAD/DVD verified) |
| HSD / sysdolphin (47 units) | 182,392 | GC/1.3 | doldecomp/melee `src/sysdolphin/baselib` | HIGH (hsd_class verified) |
| MusyX runtime (2 units) | 93,276 | GC/1.3 vs 1.3.2 (conflict) | AxioDL/musyx v2.0.0 | MEDIUM (unresolved) |
| MSL + MetroTRK (crt/*, trk/*) | 50,532 | GC/1.3 (+ mem 2.0, exit 1.3.2) | MSL fdlibm / zeldaret oot-gc | HIGH |
| **Total portable** | **542,948** | | | |

Total portable ≈ **24.3% of all unmatched code** (542,948 / 2,234,728 B), ≈ **21.8% of the whole code image**.

---

## 1. Ranked target table

Ordered by (unmatched code% × tractability). Tractability = compiler verified + exact reference + existing byte-match proof + concentration.

| Rank | Unit(s) | Unmatched B | Reference decomp | Compiler/version match confidence | Portability | Est. effort |
|---|---|---|---|---|---|---|
| 1 | **MusyX** — people_field runtime region (VA≥0x8014D000) + musyx_range_801652DC | **93,276** (93,052 + 224) | AxioDL/musyx v2.0.0 `src/musyx/runtime/*` | MEDIUM — GC/1.3 (archive, fn_80162118 verified) vs GC/1.3.2 (live). Resolve A/B first | HIGH | Med (concentrated; quick-wins at 97–99% fuzzy; one mcmd patch = 11 fns) |
| 2 | **HSD hsd_texp** | 41,516 | melee `baselib/texp.c` | HIGH — GC/1.3 family-verified | HIGH | High (GX/TEV register-packing) |
| 3 | **HSD hsd_jobj** | 27,396 | melee `baselib/jobj.c` | HIGH — 5.9% already matches under 1.3 | HIGH | Med-High |
| 4 | **Dolphin THP** — THP_range_801E1B54 | 46,220 | GC SDK 2.3 THP (Aug-2002) | HIGH-by-family, but 0% matched — calibrate 1 fn | MEDIUM | High (JPEG codec, thinner reference) |
| 5 | **Dolphin AI/AR/DSP** — sdk_range_800AC02C | 45,508 | GC SDK 2.3 AI/AR (Sep-2002) | HIGH-by-family, 0% matched | MEDIUM-HIGH | Med-High |
| 6 | **Dolphin OS** (aggregate) | 32,460 | any dtk GC decomp (OS 2.3, Mar-2003) | HIGH | HIGH | Med (universally shared, low drift) |
| 7 | **Dolphin GX** (aggregate) | 29,124 | GC decomps (GX 2.3, Feb-2003) | HIGH | HIGH | Med (needs matching GXStruct header) |
| 8 | **MSL fdlibm math** — crt/math_range_800CAA58 | 15,684 | MSL fdlibm (SUN), signature-DB matched | HIGH — GC/1.3 | HIGH | Low-Med (stable public source) |
| 9 | **HSD tobj/pobj/cobj/lobj/dobj** (aggregate) | 57,544 | melee baselib | HIGH | HIGH | Med (cobj already 24%, lobj 14%) |
| 10 | **Dolphin DVD/VI/EXI/SI/PAD/MTX** (aggregate) | 45,932 | any dtk GC decomp | HIGH — SI/PAD verified in-repo | HIGH | Low-Med |
| 11 | **MSL printf/stdio** — crt/printf, stdio_range | 11,052 | MSL (signature-matched) | HIGH | HIGH | Low-Med |
| 12 | **MetroTRK** (trk/*, 41 units) | 21,616 | zeldaret/oot-gc metrotrk | HIGH — many trk units already 100% | HIGH | Low-Med (many small) |
| 13 | **HSD remainder** (mobj/robj/aobj/fog/shadow/wobj/video/displayfunc/ByteCodeEval…) | ~55,900 | melee baselib | HIGH | HIGH→MED | Med (ByteCodeEval = single 5,732 B fn, schedule last) |

---

## 2. Recommended order

**First — MusyX (93,276 B).** Not the biggest track, but the best *pilot*: it is concentrated in essentially one object, has an exact matching-oriented reference (AxioDL/musyx v2.0.0), byte-matching is already *proven* (fn_80162118 matches), scout quick-wins sit at 97–99% fuzzy, and one macro fix (u64 flag field) lands the entire 11-fn / 2,860 B `fn_80153FEC` mcmd family from a single patch. This validates the whole vendor-source → recompile → objdiff pipeline on a small, high-yield surface before scaling. **Blocker to clear first:** the GC/1.3 vs GC/1.3.2 conflict — run an A/B on an anchor fn before committing.

**Second — HSD / sysdolphin (182,392 B).** Largest *compiler-ready* bucket. GC/1.3 is verified (hsd_class), the reference is a byte-identical 1:1 map to melee `baselib`, and multiple units already partially match cold (object 64%, class 64%, cobj 24%, fog 22%, wobj 19%, lobj 14%) — proof the toolchain works on sysdolphin without calibration. No splits.txt surgery needed. Bonus: HSD matches directly unblock gs_render residuals (game code consumes `_HSD_VtxDescList`).

**Third — Dolphin SDK (216,748 B).** Biggest absolute total and compiler+linker are verified (GC/1.2.5n; SI/PAD/DVD already byte-match). Sequenced third only because of version-sourcing overhead — the game is SDK **2.3** (2002–2003) while the obvious public reference (doldecomp/dolsdk2001) is the older 2001 SDK, so source must come from contemporaneous 2002–2003 game decomps and THP/AI blobs (0% matched) need a first-function calibration. Start with the low-drift cores (OS, GX, DVD/VI/EXI/SI/PAD/MTX), defer THP/AI-AR-DSP.

**Fourth — MSL + MetroTRK (50,532 B).** Clean matches, solid references, but a low ceiling (2.3% of unmatched). Do `math_range` (15,684 B fdlibm, single largest portable file) opportunistically alongside the bigger tracks; sweep TRK last.

**Reachable if all land: ~24.3% of unmatched code (542,948 B).**

---

## 3. First target — MusyX kickoff plan

**Reference repo:** `github.com/AxioDL/musyx` (MUSY_VERSION 2.0.0, MUSY_TARGET_DOLPHIN, actively maintained). Tree `src/musyx/runtime/`: synth.c, synthmacros.c, synthvoice.c, synth_adsr.c, seq.c, s_data.c, snd_midictrl.c, hardware.c, hw_dspctrl.c, hw_aramdma.c, hw_volconv.c, snd3d.c, and effect dirs StdReverb/reverb.c, CheapReverb, Delay, Chorus.

**Resolve the compiler conflict first (gating step).**
- Live `configure.py:558/905` sets **GC/1.3.2**; archive `compile_config.json` + campaign map say **GC/1.3** with a recorded byte-match of fn_80162118. Only 4.4% matched and no verification comment for 1.3.2.
- A/B both versions on a known anchor (e.g. `hwSetVolume` fn_80162A58, or `HandleReverb` fn_80164DD0) under the fixed flags `-O4,p -nodefaults -proc gekko -fp hard -Cpp_exceptions off -enum int -warn off -use_lmw_stmw off -sdata 8 -sdata2 8`. Cross-check against AxioDL's own MWCC target. **Commit the winner before porting.** `-use_lmw_stmw off` is confirmed (TU uses `_savegpr_/_restgpr_` helpers) and non-negotiable.

**Bringing source in (respect non-negotiables):**
- Vendor AxioDL headers + .c into the source tree; **no `.inc` asm includes** — port to C and iterate to byte-match via objdiff.
- **Preserve `fn_XXXX` traceability** — keep the address-named symbols as the identity anchors; do not rename to AxioDL names in a way that breaks the fn_ mapping (the 130 matched MusyX anchors + 113 already-named fns are the map).
- **Do not touch symbols.txt / splits.txt as part of the port.** The item-logic prefix split (below) is a *separate, build-verified* prerequisite change, not part of function porting.
- Set the MusyX region's `mw_version` in `configure.py` to the A/B winner; leave the flags as above.

**Prerequisite structural change (own commit, build-verified):** peel the 35,468 B item-use-on-Pokemon prefix (VA 0x80144574–0x8014D000: hpRecover, itemParamConvertOrigFormat, ITEMUSE2POKEMON_LOG) out of `people_field.c` into a game-logic TU so the MusyX region can align to AxioDL file boundaries (synth.c / synthmacros.c / reverb.c / snd_midictrl.c / hw_dspctrl.c …). This is *not* MusyX and must not inflate the track.

**First validation batch (lock the toolchain, highest bytes-per-effort):**
1. `fn_80153FEC` mcmd controller-macro family — 11 fns × 260 B = **2,860 B**, byte-identical modulo 3 constants; one shared macro fix (treat +0x114 as a u64 flag field with u64 mask) lands all 11. Maps to synthmacros.c. *Best single opening move.*
2. `fn_80162370` hw-init (184 B, 99.1% fuzzy) → hardware.c.
3. `fn_80162EB8` ARAM-flush (144 B, 98.1%) → hw_aramdma.c.
4. `fn_80162FB0` float-reorder (92 B, 98.7%) → hw_volconv.c.
5. `fn_801628C8` salActivateVoice wrapper (60 B, 98.3%); `fn_801631AC` 8-byte global copy (20 B, 97.6%); `fn_80162070` PRNG (28 B); `fn_80163490` spin-wait (24 B) — trivial confirmers.
6. Then `HandleReverb` fn_80164DD0 + `ReverbHICallback` (musyx_range_801652DC, 224 B) together from StdReverb/reverb.c.

Landing batch 1 alone proves the pipeline; the mcmd family + reverb pair are the highest-leverage first day.

---

## 4. Risks & unknowns

**Must-calibrate-before-porting:**
- **MusyX compiler version (TOP risk):** GC/1.3.2 (live) vs GC/1.3 (archive, verified). Wrong choice = zero byte-match. A/B on an anchor fn is a hard gate before any MusyX porting.
- **THP + AI/AR/DSP blobs (0% matched):** GC/1.2.5n is inherited from the dolphin family, never proven on these two 45K+ units. Calibrate one function each before a full port. Low absolute risk (SDK + linker are 1.2.5n and verified on SI/PAD/DVD) but not unit-verified.

**Compiler-version gaps (recompile + iterate, don't drop-in):**
- **HSD:** melee builds under GC/1.2.5(n); Colosseum HSD is GC/1.3 (confirmed real — hsd_class shows 1.2.5n emits a larger caller frame). Melee C is a template; recompile under 1.3 and diff per-function.
- **MusyX:** AxioDL is calibrated to the Prime-family toolchain (often 2.x/1.2.5), not GC/1.3. Register alloc/scheduling will differ; the 97–99% fuzzy scores show the gap is small but real.

**SDK/library-version drift (source is a template, not a guarantee):**
- **Dolphin = SDK 2.3 (2002–2003)**, but the obvious public reference (dolsdk2001) is the **2001** SDK. Port from contemporaneous 2002–2003 game decomps (Sunshine/Pikmin/Mario Party 4 era); expect small 2.1→2.3 per-function diffs. GX also needs the matching GXStruct header revision.
- **MusyX:** AxioDL v2.0.0 calibrated against Metroid Prime; Colosseum (Genius Sonority, 2003) may ship a slightly different MusyX 2.x build — a few fns may have diverged (extra params, reordered fields).
- **HSD:** melee = 2001 sysdolphin vs Colosseum 2003; struct layouts / added-removed fns may differ. Byte-identical symbol names mitigate but don't eliminate.
- **MSL/TRK:** byte-match depends on the exact Metrowerks MSL and MetroTRK *release* bundled with CodeWarrior — a separate versioning axis from the SDK. Verify against the crt/trk fragments already matched in-repo.

**Attribution / accounting:**
- Do **not** count people_field as 128,520 B of MusyX — ~35,468 B is item game-logic. Real MusyX budget is **93,276 B**. The `people` unit (61,128 B) is the real NPC system (zero audio symbols) — also excluded.
- Don't double-count THP/sdk_range across dolphin and "other-libs" dimensions.

**Hard blockers / dependencies:**
- MusyX per-file alignment requires the item-logic split out of people_field.c first (build-verified splits.txt change).
- Several MusyX quick-wins (sndBSearch twins, salActivateVoice wrapper) need correct prototypes + SDA key-struct layout; wrong `-sdata/-sdata2` placement blocks otherwise-trivial fns.

**Unclassified (needs disassembly):** dolphin `sdk_range_8009F77C` (880 B) and `sdk_range_800CE7DC` (608 B) have no named symbols — assign a component before porting.