# Structural-port salvage report

Source: `/Users/douglaswhittingham/pkmn-colosseum/archive/previous_campaign/config/GC6E01/symbolmap/structural_ports.json` (1145 archived mnemonic-fingerprint XD ports), revalidated against the CURRENT `/Users/douglaswhittingham/pkmn-colosseum/config/GC6E01/symbols.txt`.

No live XD asm is available in this tree, so this evidence was not re-mined - only re-checked address-by-address against current symbol names. See mine_xrefs.py/structural_port.py porting notes for how to wire up a live XD split.

| status | count | meaning |
|---|---|---|
| still-open | 49 | live REVIEW-band lead - function is still `fn_`/`lbl_` today |
| already-applied | 1079 | independently confirmed - already renamed to match (or `name_ADDR` variant) |
| already-conflict | 4 | already renamed, but to a DIFFERENT name (see samples) |
| address-not-found | 0 | no function symbol at that address anymore |
| skip (non-address fn) | 13 | archived `fn` was already a real name at archive time |

`structural_ports.json` in this directory now holds 595 entries (0 newly merged from salvage) - the still-open band only.

## Sample: still-open (actionable REVIEW leads)

| fn | addr | -> xd_name | n | confidence | shared strings |
|---|---|---|---|---|---|
| `fn_8009EFE4` | 0x8009EFE4 | **OSUnlink** | 117 | MED | `` |
| `fn_800A5268` | 0x800A5268 | **DVDGetCurrentDir** | 49 | MED | `` |
| `fn_8001BD80` | 0x8001BD80 | **menuPokemonOpenFight** | 29 | MED | `` |
| `fn_80255C18` | 0x80255C18 | **fightTrainerAiWazaHit160** | 29 | MED | `` |
| `fn_8020FC04` | 0x8020FC04 | **fightWazaWzxVariationFuncWeatherHP** | 27 | MED | `` |
| `fn_802400D8` | 0x802400D8 | **fightTrainerAiWazaValueOomugaesi** | 27 | MED | `` |
| `fn_80258A64` | 0x80258A64 | **fightTrainerAiWazaHit079** | 27 | MED | `` |
| `fn_80255AE4` | 0x80255AE4 | **fightTrainerAiWazaHit162** | 26 | MED | `` |
| `fn_80259A84` | 0x80259A84 | **fightTrainerAiWazaHit057** | 26 | MED | `` |
| `fn_801F00D0` | 0x801F00D0 | **fightTargetGetRelativeHostSideFightTargetIdToTragetPtr** | 25 | MED | `` |

## Sample: already-applied (QA confirmation, no action needed)

| fn | addr | archived xd_name | current name |
|---|---|---|---|
| `fn_8017424C` | 0x8017424C | generateParticle_801947D4 | **generateParticle_8017424C** |
| `fn_800AA8D4` | 0x800AA8D4 | PADTypeAndStatusCallback | **PADTypeAndStatusCallback** |
| `fn_800CF764` | 0x800CF764 | CompleteTransfer | **CompleteTransfer** |
| `fn_800A4D28` | 0x800A4D28 | DVDConvertPathToEntrynum | **DVDConvertPathToEntrynum** |
| `fn_801402AC` | 0x801402AC | _pachiruEffectCreateTexture__FP9GStextureP9GStextureUl | **_pachiruEffectCreateTexture__FP9GStextureP9GStextureUl** |
| `fn_80173F98` | 0x80173F98 | psExecGenerator | **psExecGenerator** |
| `fn_80172D00` | 0x80172D00 | setVelToJObj | **setVelToJObj** |
| `fn_800D00B0` | 0x800D00B0 | __SITransfer | **__SITransfer** |
| `fn_80190C90` | 0x80190C90 | GSflagInitBitPos | **GSflagInitBitPos** |
| `fn_8016A17C` | 0x8016A17C | psInitDataBank | **psInitDataBank** |

## already-conflict (informational only)

| fn | addr | archived xd_name | current name |
|---|---|---|---|
| `fn_80007364` | 0x80007364 | _dbgMenuFightFightPokemonSelectSub | menuFightPokemonSelectSub |
| `fn_80006FAC` | 0x80006FAC | dbmMenuFightButtonNormal | menuFightButtonNormal |
| `fn_800D1860` | 0x800D1860 | GSlightSetAnimFrame | GScameraSetAnimFrame |
| `fn_8018FD88` | 0x8018FD88 | peopleBiosGetWorkPtr | peopleGetEntry |
