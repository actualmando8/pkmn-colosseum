# Audit Pass 3: Fight-AI Engine — Structure, Ledger, and the 191KB Gap

Date: 2026-07-01. Read-only audit (sonnet evidence pass + haiku vocabulary
mining); no source/symbol/split edits. Follow-up to passes 1-2
(docs/fable5_audit_pass1_musyx_discovery.md, _pass2_symbol_name_audit.md).

## Headline findings

1. **A 191KB / 470-function block (0x80211A00-0x802405C0) is completely
   unassigned in splits.txt** — invisible to report.json. It contains
   fightSeqGetItemType, fightTrainerAiGetValueAryMaxBanme, and dozens of
   callees used by colosseum_battle.c's WazaHit family. It is roughly as much
   code as colosseum_battle.c itself and is almost certainly the real bulk of
   the AI-evaluation support layer. Zero attempted matches. Fixing splits here
   is target #8 below and costs no decompilation effort.
2. **fn_801F54A4 (pokemon.c, 3352B, already 90.08% fuzzy) is the campaign's
   synthInit-equivalent**: a 94-entry jumptable master getter
   (jumptable_803754AC) whose entries are trivial per-field accessors — the
   single richest struct-offset evidence source, nearly matched already.
3. Attribution nuance vs pass 2: pokemon.c/trainer.c genuinely hold the
   fight-floor infrastructure (88-100% on named anchors); the real fictions
   are (a) counting colosseum_event.c as part of this engine (zero anchors,
   though it does host the FightActionWork setter cluster) and (b) the gap
   blindness.

## Struct ledgers (confidence per campaign scale)

### FightSideEntry (stride 0x18)
Base *(lbl_80478F4C), count *(lbl_80478F48); resolver fn_801F6738(id) with
bound check. Exposed via gs_task.c fn_8000879C debug hook.

| Offset | Type | Conf | Evidence |
| ---: | --- | --- | --- |
| 0x00 | u8 | B | fn_801F66D4 |
| 0x02 | u16 | B | fn_801F6720 |
| 0x04 | ptr/u32 | B | fn_801F6600 |
| 0x08 | ptr/u32 | B | fn_801F66BC |
| 0x0C | ptr/u32 | B | fn_801F66A4 |
| 0x10 | ptr/u32 | B | fn_801F668C |
| 0x14 | u16[2] | B | fn_801F66EC (idx<2) |

B not A: proven by repeated jumptable-getter callsites; no allocator located.

### FightActionWork (per-battler action state, size >=0x2C) — confidence A
Init: fightActionInit via 9 NULL-guarded setters fn_8020D78C..fn_8020D8C8
(colosseum_event.c, all matched): 0x00 u16=0, 0x04..0x18 u32=0, 0x1C u32=-1
sentinel (selected action), 0x20-0x2C u32[4]=0 (4 slots = doubles: 2 sides x
2 active). Getter side not yet located (target #9).

### FightFloorPtrAryCtx (transient collector passed to _fightFloor*Sub callbacks)
0x00 filter value; 0x08 output index accum; 0x0C output array base; 0x10 loop
bound; 0x14 extra-filter flag (==1 -> fn_80206608). Verified in 2 of 5 Subs
only — do not generalize without re-audit.

### Accessor architecture
fn_801F54A4 (94-entry getter, 90%), fn_801FB1C0 (trainer.c 1848B, 0.22% —
likely the setter mirror; biggest remaining type unlock), fn_80239058
(gap block, smaller id-keyed flag check). Shared-storage proof pending.

## Real module map

| Range | Split file | Actual content | State |
| --- | --- | --- | --- |
| 0x801F1460-0x801F7798 | pokemon.c | floor iteration + accessor core (fightActionInit, 5 Floor Subs, fn_801F2B5C AI-dispatch orchestrator, fn_801F54A4, fn_801F6738) | 88-100% anchors |
| 0x801F7F80-0x80201764 | trainer.c | fightOutPokemonCheckNoAttackFlag + fn_801FB1C0 wall | unit 47% |
| 0x80201764-0x80202810 | UNASSIGNED | 14 fns, 4268B, unaudited | 0% |
| 0x80202810-0x80211A00 | colosseum_event.c | no fight anchors; hosts FightActionWork setter cluster | 84% fuzzy |
| 0x80211A00-0x802405C0 | **UNASSIGNED** | 470 fns / 191KB AI support layer | 0%, invisible |
| 0x802405C0-0x80265EC4 | colosseum_battle.c | 22 WazaHitNNN + WazaDamage104 + AI-veto predicates (Horobinouta=Perish Song) — names real; all 26 AI anchors unattempted | unit 41% |

## Vocabulary (haiku mining)

# Fight vocabulary mining (haiku agent, 2026-07-01)
- 37-term vocabulary: Waza=move, Tokusei=ability, Seikaku=nature, Huuin=sealed(Imprison), Kouka=effect, Mamoru=protect, Yokodori=steal(Snatch), Horobinouta=Perish Song, Oumugaesi=counter(Mirror Move?), Bosou=rampage, Irekaeru=switch, Dakeki=strike, Negoto=Sleep Talk, Nekonote?, Yubiwohuru=Metronome, Monomane=Mimic, Floor=battle side/field, Side, FightTrainer/FightPokemon/FightOutPokemon/FightAction/FightSeq.
- Struct hierarchy: FightFloor -> FightTrainer[] -> {FightPokemon[], FightOutPokemon[]} + TrainerAI per-move evaluators.
- Per-move AI handler table: fightTrainerAiWaza<Type><MoveID>, 23 evaluators seen covering move IDs 7-205 (table-driven).
- 6 mangled sigs demangled, all __FPvUsPv (floor*, u16 idx, callback*) -> key ones: _fightFloorGetFightTrainerFightPokemonPtrArySub@0x801F1B14, ...FightOutPokemonPtrArySub@0x801F1C98, ...CheckHuuinWaza...@0x801F1F7C, Tokusei lookups @0x801F34EC/0x801F3678, _fightTrainerAiCheckHorobinoutaSub@0x8025C6BC.
- XD vocabulary: 385 battle/floor/waza names total; only 9 applied; 376 portable candidates (battle:24, floor:213, waza:139).


## Ten ranked targets (from the audit)
1. fn_801F2B5C (992B, 88.1%) — AI-dispatch orchestrator, near-complete.
2. fn_801F54A4 (3352B, 90.1%) — 94-field master getter; highest type-unlock/effort.
3. fightTrainerAiGetValueAryMaxBanme (300B, fully decoded: argmax w/ RNG tiebreak via fn_800E0C54).
4. _fightTrainerAiCheckHorobinoutaSub (180B, fully decoded: Perish Song counter callback).
5. fightSeqGetItemType (380B, fully decoded; reuses the item-data accessors from pass 1 — cross-campaign synergy).
6. fightTrainerAiWazaHit007/008 (smallest of the 22-family; cracking one likely templates the rest, cf. mcmd family).
7. fn_801FB1C0 (1848B, 0.22%) — master setter mirror; pins FightActionWork to full A.
8. **Splits fix: assign 0x80211A00-0x802405C0** (zero decomp effort, unblocks metrics).
9. Locate FightActionWork getters (research-only).
10. _fightFloorCheckHuuinWazaFightOutPokemonSub (164B, 96.3%) — near done.

## Open questions
- fn_801F8424 / fn_80206608 filter semantics (alive? on-field?).
- FightSideEntry capacity (2?) and allocator location (needed for A).
- Layouts of the 3 unaudited _fightFloor*Sub contexts.
- The WazaHitNNN family's dispatch table (007..205 non-sequential — move-effect ID switch elsewhere).
- Shared storage proof across the 3 dispatchers.
- The 14-fn gap 0x80201764-0x80202810.
- colosseum_battle.c's pre-existing header claims (CheckTrainerPokemonFlag etc.) — hints only, unverified.

## Provenance
Sonnet read-only evidence audit (~198k tok) + haiku vocabulary mining
(~53k tok), 2026-07-01. XD vocabulary: 385 battle/floor/waza names, 376 not
yet applied (battle:24, floor:213, waza:139) — the naming reservoir for this
campaign. No files modified.
