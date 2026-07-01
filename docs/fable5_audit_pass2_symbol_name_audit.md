# Audit Pass 2: Symbol-Name Trust Audit (all 1013 named functions)

Date: 2026-07-01. Audit-only; no source/symbol/split edits. Follow-up to
`docs/fable5_audit_pass1_musyx_discovery.md`. Method: deterministic provenance
tracing (old-campaign symbolmap + git blame + prefix-neighborhood analysis)
followed by a 17-agent audit (8× haiku trust-triage over all 1013 names, 9×
sonnet asm-evidence audits of every conflicted region).

## Name provenance (deterministic)

Every current named function symbol traces to one of three sources; zero names
moved addresses between the old campaign and the current tree (the transplant
itself was address-faithful — the errors are in the old campaign's own naming):

| Provenance | Count | Trust |
| --- | ---: | --- |
| post-reset (dtk signature analysis, XD-map mangled names, new imports) | 445 | HIGH for SDK/MusyX/HSD signatures and mangled `__F` names |
| old-campaign func_tu_map, status=KNOWN (means "assigned to a TU", NOT verified) | 344 | MIXED — this class produced the peopleField* fiction |
| old-campaign verified (applied_symbols.txt with per-name evidence) | 224 | MEDIUM-HIGH |

Triage buckets over all names: **REFERENCE 528** (known public Dolphin
SDK/MusyX/HSD/TRK/libm symbols or mangled map names), **PLAUSIBLE_STATIC 319**
(credible library statics matching neighborhood), **GAME_UNVERIFIED 133**
(game-code names with no external reference), **SUSPECT 20**.

## Confirmed wrong names (asm-verified)

| Symbol | Addr | Real identity (evidence in asm) |
| --- | --- | --- |
| peopleFieldCompare8ByteTableKey | 0x801522E0 | MusyX bsearch key comparator (u16 key at +4, 8-byte stride), passed to sndBSearch |
| peopleFieldCompare12ByteTableKey | 0x801523A8 | Same, 12-byte stride |
| peopleFieldMotionResolveInput23C..3C8 (11 fns) | 0x8016161C-0x801618EC | MusyX lazy cached-controller-value getters: dirty-bit in voice struct +0x214 gates cache vs recompute via `_GetInputValue` on ctrl-table entries +0x23C..+0x3C8. Same table offsets as the fn_80153FEC mcmd family (pass 1). |
| dbmMenuFightButtonNormal | 0x80006FAC | In-battle Fight-command task code in gs_task.c, not a debug menu (dbg/dbm prefix wrong) |
| _dbgMenuFightFightPokemonSelectSub | 0x80007364 | Same — battle party-switch task code, prefix wrong |
| generateParticle_801947D4 | 0x8017424C | Address-suffix does not match own address or any symbol — stale/foreign-address (likely XD-address) auto-name; body unverified |
| ObjInfoInit_802596A4 | 0x801AA568 | Same address-suffix anomaly inside HSD region |

All 6 additional triage suspects were asm-verified CORRECT (2026-07-01
follow-up): `menuCardE_CompareEntryPtrs` (matches menu.h field docs),
`applyForceJObj`/`setVelToJObj` (literal "jobj"/"jobj.h" assert strings),
`cb` (real DVD boot-sequence async callback, scope:local), `__fstLoad`
(reads the 0x80000000 disc header), `PADTypeAndStatusCallback` (genuine PAD
code in the unattributed 0x800AA4D4-0x800AB5B4 gap — future split should be
dolphin/pad/PAD.c, not VI.c).

CORRECTION (same follow-up): the earlier `HSD_ForeachAnim` "wrong" verdict is
OVERTURNED — the asm at 0x801C028C has the classic PPC varargs prologue
(cr1-gated f1-f8 spills + r3-r10 save area) and sysdolphin's HSD_ForeachAnim
is variadic; callers in gs_render*.c fit. The name is CORRECT; the actual
defect is battle_grid.c's split start (HSD code extends past 0x801C0270).
`_menuCBBattleStartDispTrainerTexCallBack__FlPvl` is fine as a name but is not
located in any of the six gs_* units that cite it.

## Unit attribution verdicts (splits-level fiction)

The old campaign's TU story is wrong for much of the game region. Asm-verified:

| Unit (splits.txt) | Actual content |
| --- | --- |
| game/people/people_data.c | **Item data module** (item-param tables + accessors; `itemGetStatus` 0x80142CF4 jumptable dispatcher; fn_801440A0 = item-record accessor with 48 callers). The people_field.h "data management API" block collides with proven item functions. |
| game/people/people_field.c | MusyX audio runtime + item-use logic (pass 1) |
| people.h claim "people.c at 0x80180C78+" | **No such unit exists**; range is unassigned in splits.txt. Real NPC system location: unknown, must be found fresh. |
| game/gs_task.c | In-battle Fight-command / party-switch task code (not generic task system, not debug menu) |
| game/gs_party_access.c | Party accessors + genuinely embedded debug/test menu (dbgMenu* names credible here) |
| game/gs_npc_interact.c | Battle Fight-menu UI (move usability vs MENU_WAZA_STATUS, panel close, HP-gauge tween) — mislabeled |
| game/gs_event_exec.c | Numeric-input menu widgets (decimal cursor, level-up panel) — mislabeled |
| game/gs_pokemon_summary.c | Plausibly summary screen (zero named anchors; unverified) |
| game/gs_pcbox.c | Largely plausible PC-Box screen; largest fn is a shared multi-screen panel dispatcher |
| game/gs_title.c | **Mislabeled** — largest fn calls itemParamConvertOrigFormat/itemParamGetRecoverType (item screens, not title) |
| game/gs_worldmap.c | Mixed: worldmap data loading + Name-Entry screen code (menuNameEntryCtrl) |
| game/effect/effect_util.c | Grab-bag: kouka (効果=effect) dispatcher + unrelated savedata/PC-box subsystem |
| game/effect/effect_visual.c | Coherent, credible (leaffx/lightning/electron/surf visual effects) |
| game/battle/battle_grid.c | Genuine game code that *uses* HSD (not HSD library); unit name plausible |
| game/pokemon.c | Arbitrary boundary; content = fight-engine fragment (all 7 named fns are fight* family) |
| game/trainer.c | Arbitrary boundary; 387/388 fns unnamed, single anchor is fight* |
| game/colosseum_event.c | **Zero evidence** for the name: 385 unnamed fns, no strings |
| game/colosseum_battle.c | Best-supported: 25-name fightTrainerAiWaza* battle-AI cluster (coherent Japanese battle vocabulary: Waza/Horobinouta/Tokusei), but not proven a single TU |

The 35-anchor `fight*` family (0x801F1460–0x8025C6BC) is one coherent
cross-unit **trainer-AI battle-evaluation engine**; the four unit names
partitioning it are old-campaign fiction. Verified-correct region strata for
future contextualization: Dolphin SDK (PPC/OS/EXI/DVD/AI/AR/DSP/CARD/GX/SI/VI/PAD),
MetroWerks TRK + udp/ddh/gdev drivers, MSL libm (`__ieee754_*`/`__kernel_*`),
MusyX (0x80146E88–0x80165400), GS engine (Genius Sonority: GScamera/GSgfx/
GSmodel/GSthread/GSmsg/GSscene...), ps* particle system, sysdolphin HSD_
(0x80191628–0x801C0F20 area), THP video, fade, waza, fight/AI.

## Recommended actions (for a follow-up implementation pass — none applied)

1. **Rename batch A (safe, evidence-attached):** demote the 13 peopleField*
   MusyX names to MusyX-consistent names (or neutral `snd*`/`fn_`-restoring
   names) keeping `fn_XXXXXXXX` traceability comments; fix the dbm/dbgMenu
   prefixes in gs_task.c; retag HSD_ForeachAnim.
2. **Header retirement:** `include/game/people/people_field.h` (fiction, pass 1)
   and the people.h unit claims. The item-data reality of people_data.c should
   eventually rename that unit + its API docs.
3. **Suspect follow-ups:** one-function asm checks for the 6 triage suspects +
   the 2 address-suffix anomalies.
4. **Contextualization lanes for the 7576 fn_ autos:** the verified region
   strata above make cheap-model naming tractable — per-region lanes with
   reference vocabularies (MusyX/HSD/SDK from public decomps; fight*/menu*
   from XD map vocabulary in `archive/.../symbolmap/xd_vocabulary.json`).
   Highest-value first: MusyX unit (reference source exists → names + matches
   together), then the fight AI engine (self-consistent vocabulary, 35 anchors,
   ~1600 unnamed fns in its span).
5. **Splits hygiene:** unit renames require build/report verification and are
   lower priority than function naming (unit names are cosmetic until splits
   change; function names propagate into decomp work products).

## Applied fixes (2026-07-01, same session)

Rename batch A was implemented after the user waived the no-rename campaign
rule. All renames verified lossless: matched_functions 3318 and matched_code
250760 identical before/after; git diff --check clean.

- 11 MusyX cached-controller getters renamed by dirty-bit mask decoded from
  asm, 1:1 against AxioDL/musyx snd_midictrl.c: peopleFieldMotionResolveInput
  {23C,260,284,2A8,2CC,2F0,338,35C,380,3A4,3C8} -> inpGet{Panning,
  SurroundPanning,PitchBend,Doppler,Modulation,Pedal,PreAuxA,Reverb,PreAuxB,
  PostAuxB,Tremolo}. (Volume 0x1 and Portamento 0x80 getters absent, matching
  the CTRL_DEST offset gap.)
- peopleFieldCompare8ByteTableKey -> curvecmp (uniquely shared by the two
  sndBSearch wrapper callers, as dataGetCurve/dataGetKeymap share curvecmp in
  reference synthdata.c); peopleFieldCompare12ByteTableKey -> layercmp.
- dbmMenuFightButtonNormal -> menuFightButtonNormal;
  _dbgMenuFightFightPokemonSelectSub -> menuFightPokemonSelectSub (evidence
  supports the fight-menu semantics; only the debug prefixes were fiction).
- Stale foreign-address suffixes corrected in place:
  generateParticle_801947D4 -> generateParticle_8017424C;
  ObjInfoInit_802596A4 -> ObjInfoInit_801AA568.
- include/game/people/people_field.h DELETED (pure fiction; nothing included
  it). people_field.c's 489-line fictional header comment replaced with an
  honest MusyX/item-code description.
- The five 16-byte comparator-shaped fns around 0x8015210C-0x80152434 map by
  reference order to maccmp/smpcmp/curvecmp/layercmp/fxcmp and their callers
  to dataGetMacro/dataGetSample/dataGetCurve/dataGetKeymap/dataGetLayer/
  dataGetFX — only the two proven ones were renamed; the rest awaits the
  MusyX port pass.
- NEW EVIDENCE on the real NPC system: orphaned src/game/people/
  people_fn_*.inc extracts cover 0x80181478-0x8018FC50 — the old campaign had
  a people.c TU there and the range is currently unassigned in splits.txt.
  That is the likely true location of the people system (matches people.h).

## Provenance

Workflow run wf_f3b7bc51-663 (17 agents, ~756K tokens, haiku triage + sonnet
region audits), 2026-07-01. Deterministic inputs: `archive/previous_campaign/
config/GC6E01/symbolmap/*` (applied_symbols, name_proposals, xd_port,
func_tu_map), git blame of symbols.txt, prefix-run region map. Raw structured
results in the session workflow journal. Validation: docs-only pass,
`git diff --check` clean.
