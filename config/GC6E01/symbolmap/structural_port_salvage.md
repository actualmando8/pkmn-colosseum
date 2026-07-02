# Structural-port salvage report

Source: `/Users/douglaswhittingham/pkmn-colosseum/archive/previous_campaign/config/GC6E01/symbolmap/structural_ports.json` (1145 archived mnemonic-fingerprint XD ports), revalidated against the CURRENT `/Users/douglaswhittingham/pkmn-colosseum/config/GC6E01/symbols.txt`.

No live XD asm is available in this tree, so this evidence was not re-mined - only re-checked address-by-address against current symbol names. See mine_xrefs.py/structural_port.py porting notes for how to wire up a live XD split.

| status | count | meaning |
|---|---|---|
| still-open | 595 | live REVIEW-band lead - function is still `fn_`/`lbl_` today |
| already-applied | 533 | independently confirmed - already renamed to match (or `name_ADDR` variant) |
| already-conflict | 4 | already renamed, but to a DIFFERENT name (see samples) |
| address-not-found | 0 | no function symbol at that address anymore |
| skip (non-address fn) | 13 | archived `fn` was already a real name at archive time |

`structural_ports.json` in this directory now holds 595 entries (595 newly merged from salvage) - the still-open band only.

## Sample: still-open (actionable REVIEW leads)

| fn | addr | -> xd_name | n | confidence | shared strings |
|---|---|---|---|---|---|
| `fn_800A4D28` | 0x800A4D28 | **DVDConvertPathToEntrynum** | 189 | HIGH | `DVDConvertEntrynumToPath(pos; dvdfs.c` |
| `fn_800D00B0` | 0x800D00B0 | **__SITransfer** | 131 | HIGH | `<< Dolphin SDK - SI\trelease` |
| `fn_800A501C` | 0x800A501C | **DVDOpen** | 50 | HIGH | `Warning: DVDOpen(): file '%s` |
| `fn_800BEF44` | 0x800BEF44 | **TRKGetFreeBuffer** | 50 | HIGH | `ERROR : No buffer available\` |
| `fn_800C0504` | 0x800C0504 | **OutputData** | 42 | HIGH | `%02x ; \n` |
| `fn_800C5A58` | 0x800C5A58 | **__two_exp_800DCCB8** | 1505 | MED | `` |
| `fn_800C1FB0` | 0x800C1FB0 | **TRKTargetAccessFP** | 323 | MED | `` |
| `fn_800C1A08` | 0x800C1A08 | **TRKTargetAccessExtended2** | 270 | MED | `` |
| `fn_800C56A4` | 0x800C56A4 | **__num2dec_internal** | 237 | MED | `` |
| `fn_8009E414` | 0x8009E414 | **__OSDispatchInterrupt** | 209 | MED | `` |

## Sample: already-applied (QA confirmation, no action needed)

| fn | addr | archived xd_name | current name |
|---|---|---|---|
| `fn_8017424C` | 0x8017424C | generateParticle_801947D4 | **generateParticle_8017424C** |
| `fn_800AA8D4` | 0x800AA8D4 | PADTypeAndStatusCallback | **PADTypeAndStatusCallback** |
| `fn_800CF764` | 0x800CF764 | CompleteTransfer | **CompleteTransfer** |
| `fn_801402AC` | 0x801402AC | _pachiruEffectCreateTexture__FP9GStextureP9GStextureUl | **_pachiruEffectCreateTexture__FP9GStextureP9GStextureUl** |
| `fn_80173F98` | 0x80173F98 | psExecGenerator | **psExecGenerator** |
| `fn_80172D00` | 0x80172D00 | setVelToJObj | **setVelToJObj** |
| `fn_80190C90` | 0x80190C90 | GSflagInitBitPos | **GSflagInitBitPos** |
| `fn_8016A17C` | 0x8016A17C | psInitDataBank | **psInitDataBank** |
| `fn_80167BE8` | 0x80167BE8 | _gsdvdErrorTask_801879AC | **_gsdvdErrorTask_801879AC** |
| `fn_80172BBC` | 0x80172BBC | applyForceJObj | **applyForceJObj** |

## already-conflict (informational only)

| fn | addr | archived xd_name | current name |
|---|---|---|---|
| `fn_80007364` | 0x80007364 | _dbgMenuFightFightPokemonSelectSub | menuFightPokemonSelectSub |
| `fn_80006FAC` | 0x80006FAC | dbmMenuFightButtonNormal | menuFightButtonNormal |
| `fn_800D1860` | 0x800D1860 | GSlightSetAnimFrame | GScameraSetAnimFrame |
| `fn_8018FD88` | 0x8018FD88 | peopleBiosGetWorkPtr | peopleGetEntry |
