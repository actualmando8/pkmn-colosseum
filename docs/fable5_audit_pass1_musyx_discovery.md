# Audit Pass 1: people_field "Type Recovery" — Premise Falsified (MusyX Discovery)

Date: 2026-07-01. Audit-only pass per `docs/fable5_type_recovery_campaign.md`
("Current First Task For Fable"). No source, header, symbol, or split changes.

## Verdict

The campaign premise for the people-first audit is **falsified by the assembly**.
The audited address range is not NPC/field-people code:

- The 0x404-stride array at `lbl_8047AF48` ("PeopleFieldWork",
  "gPeopleFieldWorkArray") is the **MusyX audio runtime voice/macro work
  struct** (SYNTH_VOICE-family), allocated by the genuine MusyX `synthInit`.
- `include/game/people/people_field.h` is fiction layered over audio code. Its
  "field behavior API" (0x80144574 - 0x801652DC) covers item-use game logic and
  the MusyX runtime; none of it is NPC field behavior.
- symbols.txt names previously flagged as "bogus audio names" (`synthInit`,
  `hwSetVolume`, `HandleReverb`, `salInitAi`, ...) are **correct** MusyX
  symbols. The people-flavored names in this range (`peopleFieldCompare*TableKey`,
  `peopleFieldMotionResolveInput*`) are the mislabeled ones.

Independent confirmation: 130 non-fn function symbols in
0x80144574 - 0x80165400 are already-matched MusyX names covering every
subsystem of the library: `seq*` (seqPause/seqStop/seqVolume/seqCrossFade),
`synth*` (synthInit/synthAddJob/synthHandle/synthVolume), `snd*`
(sndStreamActivate/sndOutputMode), `data*` (dataInsertKeymap/dataInsertLayer),
the entire `mcmd*` macro-command interpreter, `voice*`/`adsr*`/`vs*`
(voiceFree/voiceKill/adsrHandle/vsSampleUpdates), `sal*`
(salInitDspCtrl/salActivateVoice/salCalcVolume/salApplyMatrix), `inp*` MIDI
controller handling (inpSetMidiCtrl14/inpGetMidiCtrl/inpGetExCtrl), `hw*`
hardware layer (hwSetADSR/hwKeyOff/hwSetVolume/hwInitIrq), `aram*` upload, and
the complete `ReverbHI*` effect set.

## Evidence-Function Identification

| Address | Header claim (people_field.h) | Actual identity | Evidence |
| --- | --- | --- | --- |
| fn_8014D000 | peopleFieldSystemInit | **synthInit** (symbols.txt correct). Allocates count*0x404 voice array into lbl_8047AF48, memsets it, per-slot defaults loop. `mulli r14, r15, 0x404` at 0x8014D024 proves sizeof == 0x404. | init loop 0x8014D078-0x8014D258 |
| fn_801557EC | peopleFieldResetState | **MusyX macro interpreter main** (synthmacros, macHandle-family). Conditional reset block, then jumptable dispatch over ~0x72 8-byte macro commands; calls mcmdWait/mcmdLoop/mcmdVibrato/DoSetPitch/voiceFree/voiceSetPriority/macSetExternalKeyoff. | dispatch loop from 0x80155A44 |
| fn_8015B250 | peopleFieldScriptMain | **sal DSP command-list builder** (salBuildCommandList-family). Emits 16-bit DSP opcodes (0x1/0xD/0xE/0xF) into buffer chain at lbl_8047B000-10; walks 0xBC-stride studio array at lbl_80447E60; zero contact with the 0x404 array. | 0x8015B264-0x8015D3A4 |
| fn_801603C0 | peopleFieldMotionUpdate | **inpSetMidiCtrl** (MIDI control-change handler). Args are 4 bytes: (ctrl, chan-id, id, value&0x7F). Handles CC 0x06/0x26 (data entry MSB/LSB) and 0x60/0x61 (data inc/dec); clamps RPN pitch-bend range to 0x18 (24 semitones). Sits directly before inpSetMidiCtrl14 (0x801609C8). Scans voice array in 8 loops at stride 0x404. | switch at 0x801603F4+ |
| fn_80144574 | peopleFieldSpawnMain | **Item-use-on-Pokemon logic** (game code, not audio). Calls `hpRecover__FP20ITEMUSE2POKEMON_LOG1PsP7PokemonUcbUsP12FightPokemon` and `itemParamConvertOrigFormat`; builds 8-byte ITEMUSE2POKEMON_LOG records, cap 0x20. Zero contact with lbl_8047AF48. | hpRecover call 0x801457E8 |
| fn_80162A58 | peopleFieldMoveApply | **hwSetVolume** (symbols.txt correct). Operates on the separate 0xF4-stride HW voice array at lbl_8047B024; quantizes L/R/S volume triplets; calls salCalcVolume. | 0x80162A68-0x80162CF8 |
| fn_80164DD0 | peopleFieldAnimInterp | **HandleReverb** (symbols.txt correct; genuine MusyX reverb_hi). 3-channel comb/allpass delay lines; 160-sample blocks; int<->float via 0x43300000 bias. Caller ReverbHICallback passes ch 0/1/2. | 0x80164E44-0x80165290 |

Header-contamination note: `people_field.h` fields `interpFactorA` (0x1A0),
`interpPointCount` (0x1A4), `field_1B8`, `motionStateB` (0x16C), `motionTypeA`
(0x190) line up with the **reverb work struct** offsets used by HandleReverb
(damping/preDelayTime/preDelayPtr/combCoef/lpLastout) — they appear to have
been invented from reverb code and projected onto the 0x404 voice struct. All
such names are demoted (see below).

## Voice/Macro Work Struct Ledger (0x404 bytes, array base lbl_8047AF48)

Merged from fn_8014D000 (init defaults; zeroed baseline via memset),
fn_801557EC (macro interpreter; work ptr in r3), fn_801603C0 (voice-array
scans). Semantic names deliberately conservative pending cross-reference
against MusyX reference source; MusyX-informed hypotheses are marked (H).

Key proven facts:

- Struct size 0x404: `mulli` allocation and 8 independent `addi ..., 0x404`
  scan loops (A).
- 0xF4: s32 voice/entity ID, -1 or 0xFF-style sentinel = free; walked by
  stride-0x404 scans; slot ptr passed to macSetExternalKeyoff (A).
- 0x34: ptr, macro/command data base = fn_8015211C(id) result (B).
- 0x38: ptr, macro program counter — dereferenced {u32,u32} and advanced +8
  per interpreted command (B). (H: MusyX macro PC; commands are 8 bytes.)
- 0x114/0x118: paired u32 flag words read/written together — u64 flags field
  (B). Derived ptr base+0x114 with disp 0/4 in init.
- 0x104: u8, explicit default 1 at init (B).
- 0x120-0x123, 0x12F-0x130: u8 cluster zeroed/defaulted at init and rewritten
  by interpreter (C — widths proven, semantics not).
- 0x1C0-0x1D3: six real fields inside the header's `pad_1BC[0x4C]`, including
  two 0x7FFF sentinels at init (B). Header padding is wrong.
- 0x1D6/0x1D7: u8 pair written with identical clamped 0..0x18 counter by
  inpSetMidiCtrl — pitch-bend-range MSB/LSB pair (H) (B).
- 0x214: u8 written by inpSetMidiCtrl slot scans (C).
- Width conflicts vs header: 0x10C is byte-stored at init (header: u32);
  0x1B8/0x1B9 are independent byte stores (header: u32 field_1B8); 0x1A0 has
  no FPR evidence (header: f32).

Full merged offset table (93 rows):

| Offset | Type | Conf | Evidence fns | Notes |
| ---: | --- | --- | --- | --- |
| 0x008 | u8 | B | fn_8014D000 | vals: i (0..count-1). role: self-referential slot index (default = own slot number i); first byte of an apparent {u8 idx, u8 0xFF} pair repeating at 0x8/0x14/0x20 (stride 0xC) |
| 0x009 | u8 | B | fn_8014D000 | vals: 0xFF. role: id/link-like byte, 0xFF = none sentinel |
| 0x014 | u8 | B | fn_8014D000 | vals: i. role: self-referential slot index (second record of 0xC-stride pattern) |
| 0x015 | u8 | B | fn_8014D000 | vals: 0xFF. role: id/link-like byte, 0xFF = none sentinel |
| 0x020 | u8 | B | fn_8014D000 | vals: i. role: self-referential slot index (third record of 0xC-stride pattern) |
| 0x021 | u8 | B | fn_8014D000 | vals: 0xFF. role: id/link-like byte, 0xFF = none sentinel |
| 0x034 | ptr | B | fn_801557EC | role: base pointer of current command/script data; set from fn_8015211C(id) result after NULL check; restored from 0x6c+i*8 stack on return-case |
| 0x038 | ptr | B | fn_801557EC | vals: advanced by 8 per command. role: current command read pointer; dereferenced (lwz 0x0/0x4 of it) and advanced +8 every interpreter iteration; re-targeted to 0x34-base + idx*8 on jump cases |
| 0x050 | ptr | A | fn_801557EC | vals: 0 at reset. role: u32 ptr array[3] (with 0x54/0x58): zeroed at reset, indexed store of fn_8015211C result, indexed load/NULL-test in clear loop bounded i<3 |
| 0x054 | u32 | C | fn_801557EC | vals: 0 at reset. role: element [1] of ptr array at 0x50 |
| 0x058 | u32 | C | fn_801557EC | vals: 0 at reset. role: element [2] of ptr array at 0x50 |
| 0x05C | ptr | B | fn_801557EC | role: u32 ptr array[3]? parallel to 0x50 array; stores fn_8015211C result + cmdIdx*8 (same pointer arithmetic as 0x38) |
| 0x068 | u8 | A | fn_801557EC | vals: 0, 1. role: flag: set 1 immediately after storing a pointer into 0x50[i], cleared 0 at reset and after loop verifies all 3 entries of 0x50[] are NULL |
| 0x06C | ptr | B | fn_801557EC | role: u32 array[4] stride 8 (pairs with 0x70): saved copy of 0x34 popped into 0x34 on return-case; indexed by 0x8d (kept mod 4) |
| 0x070 | ptr | B | fn_801557EC | role: u32 array[4] stride 8 (pairs with 0x6c): saved copy of 0x38 popped into 0x38 on return-case |
| 0x08C | u8 | B | fn_801557EC | role: u8 counter-like: tested nonzero to gate the 0x6c/0x70 pop, then decremented (call-stack depth-like) |
| 0x08D | u8 | B | fn_801557EC | vals: kept mod 4. role: u8 index into 0x6c/0x70 pair array, decremented and masked mod 4 (clrlwi ...,30) |
| 0x090 | u32 | C | fn_801557EC | vals: copy of lbl_8047AFB8. role: receives global lbl_8047AFB8 at reset (first half of a global word pair; possibly u64 with 0x94) |
| 0x094 | u32 | C | fn_801557EC | vals: copy of lbl_8047AFBC. role: receives global lbl_8047AFBC at reset (second half of pair) |
| 0x0A0 | u32 | C | fn_801557EC | vals: copy of lbl_8047AFB8. role: receives same global lbl_8047AFB8 at reset (duplicate of 0x90 pair) |
| 0x0A4 | u32 | C | fn_801557EC | vals: copy of lbl_8047AFBC. role: receives global lbl_8047AFBC at reset |
| 0x0AA | u16 | C | fn_801557EC | vals: 0 at reset.  |
| 0x0AC | u32 | A | fn_801557EC | vals: zeroed at reset. role: u32 array[16] (0xac-0xeb): memset to 0 at reset (dst r31+0xac, len 0x40); two indexed store sites bounded by cmplwi idx,0x10 with overflow redirected to global lbl_804356B |
| 0x0F4 | u32 | B | fn_8014D000, fn_801557EC | vals: 0xFFFFFFFF (-1); low byte = slot index. role: 32-bit id/handle-like, -1 = unassigned sentinel (signedness not proven by any op here); u32 identity value: low byte extracted (clrlwi 24) as first  |
| 0x104 | u8 | C | fn_8014D000, fn_801557EC | vals: 0; 0 at reset.  |
| 0x10C | u8 | B | fn_8014D000, fn_801557EC | vals: 0. role: byte-wide here; header claims u32 field_10C -- width conflict; u8 read-only here: packed into bits 24-31 (rlwimi ...,24,0,7) of the second arg to fn_80162494 alongside 0x110>>15; also u |
| 0x10E | u16 | C | fn_801557EC | vals: 0 when divisor 0. role: u16 derived quotient: (0x110>>8)/u16-divisor from command, or 0 when divisor is 0 |
| 0x110 | u32 | B | fn_8014D000, fn_801557EC | vals: 0; max 0x7FFF8000; 0xEA60 cap. role: u32 value stored as n<<15 (15-bit fixed shift); n read back via srwi 15; clamped to 0 / 0x7FFF8000 (n<=0xFFFF) in relative case and n<=0xEA60 (60000) in abso |
| 0x114 | u32 | A | fn_8014D000, fn_801557EC | vals: 0; masks 0x3000, 0x1000, 0x800, 0x400, 0x100. role: u32 flag word: read with 0x118 at entry; cleared to 0 then /=0x3000 during reset; /=0x1000, /=0x800 in handlers; bit 0x100 tested with /=0x400 |
| 0x118 | u32 | A | fn_8014D000, fn_801557EC | vals: 0; masks 0x3, 0x1, 0xFFFFFFFE, 0x8, 0x80, 0x10000. role: u32 flag word: bits 0-1 gate whole reset block; bit0 cleared (and -2) with fn_8016265C notify; masked to bit3 only (and 8) at reset end;  |
| 0x11C | u8 | C | fn_8014D000 | vals: 0.  |
| 0x11E | u8 | B | fn_8014D000, fn_801557EC | vals: 0x17. role: receives copy of default byte 0x20e at reset; u8 with distinctive non-zero default 0x17 (23); inside header's pad_11C -- header wrong, semantics unknown |
| 0x11F | u8 | C | fn_8014D000, fn_801557EC | vals: 0. role: receives copy of default byte 0x20f at reset |
| 0x120 | u8 | C | fn_801557EC | role: receives copy of default byte 0x20d at reset |
| 0x121 | u8 | A | fn_8014D000, fn_801557EC, fn_801603C0 | vals: 0xFF; 0xFF sentinel checked 3x; compared to arg r4 (0xFF is a caller-side early-out, never reaches the compare). role: id-like byte, 0xFF = none sentinel; u8 id with 0xFF = none/invalid sentinel |
| 0x122 | u8 | A | fn_801557EC, fn_801603C0 | vals: compared to arg r5, including literal 0xFF in the wildcard path. role: u8 id passed as arg2 alongside 0x121 to the same callees; initialized from default byte 0x20b; u8 slot-identity byte, unsig |
| 0x123 | u8 | C | fn_801557EC | role: receives copy of default byte 0x20c at reset |
| 0x124 | u32 | A | fn_801557EC | vals: -1 at reset. role: u32 with -1 = disabled sentinel: set to 0xFFFFFFFF at reset; later checked ==-1 (addis +0x10000; cmplwi 0xffff) to gate DoSetPitch call |
| 0x128 | u32 | A | fn_801557EC | vals: -1 at reset. role: u32 id: set to -1 at reset; rebuilt in handler as (cmd>>8) OR'd with low byte of global command arg word |
| 0x12C | s16 | A | fn_801557EC | vals: clamped 0..0x7F. role: s16 key/step number: compared against command byte; written as 7-bit value or as 0x130-base + s8 delta; sign-tested via extsh. and clamped to 0..0x7F; low byte passed as a |
| 0x12E | u8 | B | fn_801557EC | role: u8 stored from command bits 16-23 in both handlers that write 0x12c (velocity-like second parameter, generic) |
| 0x12F | u8 | B | fn_801557EC | role: u8 base value, read-only here: fallback copied to 0x130 when fn_801610F8 returns 0xFF; passed as arg r5 to fn_801610BC; added to command byte to form target id |
| 0x130 | u8 | A | fn_801557EC | role: u8 cached result of fn_801610F8(0x121,0x122) at reset (fallback = 0x12f when result==0xFF); later read as additive base for new 0x12c value |
| 0x131 | u8 | C | fn_8014D000, fn_801557EC | vals: 0; 0 at reset. role: explicitly zeroed despite whole-struct memset, so likely a real field (header calls it pad_131) |
| 0x132 | u16 | B | fn_801557EC | vals: 0 or ctrl-0x41 value. role: u16 cached inpGetMidiCtrl(0x41, 0x121, 0x122) result at reset, 0 when 0x121==0xFF |
| 0x134 | u32 | C | fn_801557EC | vals: 0x6400 at reset.  |
| 0x13C | u32 | B | fn_8014D000, fn_801557EC | vals: 0 at reset; 0x6400. role: u32 with distinctive default 0x6400 (25600; could be 100<<8 fixed-point or a rate/limit); inside header's pad_131 region -- header wrong |
| 0x150 | s16 | A | fn_801557EC | vals: 0 at reset; s8<<8. role: s16 signed adjust value: 0 at reset; set to s8<<8 from command; read back with lha, sign-compared (cmpwi 0), then +/- (value*0x51EB851F magic = /100 style scale) applied |
| 0x154 | u32 | A | fn_8014D000, fn_801557EC | vals: 0; init (u8)<<16; clamp 0x7F0000. role: u32 16.16-style level: initialized (default byte 0x208)<<16; copied to 0x158; rescaled ((v>>5)*f)>>7 from either itself or 0x158; clamped to max 0x7F0000; |
| 0x158 | u32 | A | fn_801557EC | vals: copy of 0x154. role: u32 backup of initial 0x154 value; used as scale base for absolute variant of the 0x154 rescale command |
| 0x168 | f32 | C | fn_801557EC | vals: 1.0f. role: f32, written 1.0f (lfs from sdata2 const lbl_8047D3EC) in the handler that also writes 0x16c/0x16e |
| 0x16C | u16 | B | fn_801557EC | vals: 0 at reset. role: u16: 0 at reset; set to command bits 8-23 in same handler as 0x16e/0x168 |
| 0x16E | u16 | C | fn_801557EC | role: u16 from global command arg word (0x4(r28)), truncated by sth |
| 0x170 | u32 | B | fn_8014D000, fn_801557EC | vals: 0x00400000; init (u8)<<16; clamp 0..0x7F0000. role: u32 default 0x00400000; mirrors 0x180 exactly (paired records 0x170/0x174 vs 0x180/0x184); could be 12.20 fixed-point 4.0 or a flag bit -- unk |
| 0x174 | u32 | C | fn_8014D000, fn_801557EC | vals: 0; 0 at reset. role: paired with 0x170 (next word), zeroed at reset like 0x184 |
| 0x180 | u32 | B | fn_8014D000, fn_801557EC | vals: 0x00400000; init (u8)<<16; clamp 0..0x7F0000. role: u32 default 0x00400000; mirrors 0x170; u32 level-like twin of 0x170: same (u8 0x209)<<16 init and same clamped re-store |
| 0x184 | u32 | C | fn_8014D000, fn_801557EC | vals: 0; 0 at reset. role: paired with 0x180, zeroed at reset |
| 0x190 | u8 | B | fn_8014D000, fn_801557EC | vals: 0x80; 0x80 default. role: u8 default 0x80 (midpoint of 0..0xFF -- level/volume/speed-like, semantics unproven); u8, default 0x80 at reset (center-like), re-stored from command byte in a handler  |
| 0x191 | u8 | B | fn_8014D000, fn_801557EC | vals: 0; 0 default. role: u8 paired with 0x190: 0 at reset, re-stored from command byte |
| 0x192 | u8 | B | fn_8014D000, fn_801557EC | vals: 0; 0/1. role: u8 bool: 0 at reset; set to (cmdByte != 0) via neg/or/srwi-31 idiom |
| 0x193 | u8 | B | fn_8014D000, fn_801557EC | vals: 0/1; 1. role: u8 bool-like: init from default byte 0x210; set to (cmdByte == 0) via cntlzw>>5 idiom; u8 flag/scale-like, default 1 (explicit non-zero default after memset) |
| 0x1A0 | u32 | C | fn_8014D000, fn_801557EC | vals: 0. role: 4-byte, zeroed via GPR stwx; header claims f32 interpFactorA but NO float op touches it here -- f32 unproven (0 bit pattern is compatible with 0.0f); zeroed at reset and again in the ha |
| 0x1A4 | u32 | C | fn_8014D000, fn_801557EC | vals: 0. role: zeroed at reset and in the parallel handler programming 0x1b9/0x1b4/0x1ac |
| 0x1A8 | s32 | C | fn_801557EC | vals: (+/-x)<<16. role: s32: signed fn_80162FB0 result <<16 (negative input handled via neg/call/neg) |
| 0x1AC | s32 | C | fn_801557EC | vals: (+/-x)<<16. role: s32 twin of 0x1a8 in the parallel handler |
| 0x1B0 | u32 | C | fn_801557EC | vals: (u8)<<16. role: u32 = (u8 at 0x1b8)<<16, written right after 0x1b8 store |
| 0x1B4 | u32 | C | fn_801557EC | vals: (u8)<<16. role: u32 = (u8 at 0x1b9)<<16, parallel handler |
| 0x1B8 | u8 | B | fn_8014D000, fn_801557EC | vals: 0; 0 at reset. role: byte-wide here (adjacent independent byte at 0x1B9); header claims u32 field_1B8 -- width conflict; u8: 0 at reset; set from command byte then immediately read back to build |
| 0x1B9 | u8 | B | fn_8014D000, fn_801557EC | vals: 0; 0 at reset. role: u8 twin of 0x1b8 for the 0x1a4/0x1b4/0x1ac group |
| 0x1BC | u32 | C | fn_801557EC | role: u32 written via r25 = base + cmdByte*0xc with fn_801621BC-transformed u16, only when 0x1c0(+n*0xc) already nonzero |
| 0x1C0 | u32 | B | fn_8014D000, fn_801557EC | vals: 0; 0 at reset. role: u32: 0 at reset; element of 0xc-stride block accessed via r25 = base + cmdByte*0xc (tested nonzero, then overwritten with fn_801621BC result) |
| 0x1C4 | u16 | C | fn_8014D000, fn_801557EC | vals: 0; 0 at reset. role: member of 0xc-stride block starting 0x1c0; u16; forms 0 / 0x7FFF init pair with 0x1C6 (current-vs-max or min/max-like) |
| 0x1C6 | u16 | B | fn_8014D000, fn_801557EC | vals: 0x7FFF; 0x7FFF at reset. role: member of 0xc-stride block starting 0x1c0; u16 default 0x7FFF = INT16_MAX sentinel-like (suggests s16 semantics but no signed op proves it) |
| 0x1CC | u32 | C | fn_8014D000, fn_801557EC | vals: 0; 0 at reset. role: second 0xc-stride block (mirrors 0x1c0) |
| 0x1D0 | u16 | C | fn_8014D000, fn_801557EC | vals: 0; 0 at reset. role: second block, mirrors 0x1c4; u16; forms 0 / 0x7FFF init pair with 0x1D2, mirroring the 0x1C4/0x1C6 pair |
| 0x1D2 | u16 | B | fn_8014D000, fn_801557EC | vals: 0x7FFF; 0x7FFF at reset. role: second block, mirrors 0x1c6; u16 default 0x7FFF = INT16_MAX sentinel-like |
| 0x1D6 | u8 | B | fn_801557EC, fn_801603C0 | vals: 0..0x18 (clamp constants 0x18 at 80160458/8016074C/801605D0/801608C0; floor 0 at 8016051C/8016080C). role: u8 counter-like value in range 0..0x18, always written together with 0x1D7 with the ide |
| 0x1D7 | u8 | B | fn_801557EC, fn_801603C0 | vals: 0..0x18 (same clamped value as 0x1D6). role: u8 counter-like value in range 0..0x18, same value r11 as stored to 0x1D6 in every path (pair written together, e.g. current/target of the same quant |
| 0x1D8 | u16 | C | fn_801557EC | vals: 0x2000 at reset (center-like constant).  |
| 0x208 | u8 | C | fn_801557EC | role: reset-default template byte: value<<16 seeds 0x154/0x158 |
| 0x209 | u8 | C | fn_801557EC | role: reset-default template byte: value<<16 seeds 0x180 and 0x170 |
| 0x20A | u8 | C | fn_801557EC | role: reset-default template byte copied to 0x121 |
| 0x20B | u8 | C | fn_801557EC | role: reset-default template byte copied to 0x122 |
| 0x20C | u8 | C | fn_801557EC | role: reset-default template byte copied to 0x123 |
| 0x20D | u8 | C | fn_801557EC | role: reset-default template byte copied to 0x120 |
| 0x20E | u8 | C | fn_801557EC | role: reset-default template byte copied to 0x11e |
| 0x20F | u8 | C | fn_801557EC | role: reset-default template byte copied to 0x11f |
| 0x210 | u8 | C | fn_801557EC | role: reset-default template byte copied to 0x193 (note: the lbz 0x210(r29) at 80155C60 is global lbl_80434C50, a different object) |
| 0x214 | u32 | B | fn_801603C0 | vals: 0x1FFF (li at 80160678 into r27, at 80160964 into r28). role: 32-bit field set to sentinel constant 0x1FFF on every slot matching [0x121]==r4 && [0x122]==r5, immediately followed by fn_8014C07C( |
| 0x3EC | u8 | B | fn_801557EC | role: u8 pending-count for the 4-entry ring at 0x3f0: tested nonzero, decremented after consuming an entry |
| 0x3ED | u8 | B | fn_801557EC | vals: kept mod 4. role: u8 read-index of the 0x3f0 ring, incremented mod 4 (clrlwi ...,30) |
| 0x3F0 | u32 | B | fn_801557EC | role: u32 ring buffer[4] (0x3f0-0x3ff): consumed entry stored into 0xac-var array or global var array (message/mailbox-like) |
| 0x400 | u16 | C | fn_801557EC | vals: 0 at reset. role: last field before 0x404 end (consistent with struct size) |

## Demotions (header names with no supporting evidence)

Every semantic name in the `PeopleFieldWork` definition is demoted to
fiction pending MusyX cross-reference: `entityID` (offset right, meaning
recoverable as voice ID), `resetFlag`, `flagsLo/flagsHi` (shape right — u64
flags — name keep-able), `defaultAnimBank`, `animBankA/B/C`, `prevAnimBank`,
`currentAnimBank`, `motionStateA/B`, `motionConfigA/B`, `blendTargetA/B`,
`blendSourceA/B`, `motionTypeA/B`, `motionBlendFlag`, `motionSpeed`,
`interpFactorA`, `interpPointCount`, `initAnimBank*`, `initDefaultAnim`,
`initMotionType`, `initMotionSpeed`. The `PeopleFieldEntry`/"gPeopleFieldArray"
claims in the same header are untested by this pass and now suspect.

Function prototypes in the header for this range are all wrong in kind
(audio/item functions typed as NPC functions); do not propagate them.

## Recommended Campaign Pivot

1. **MusyX porting campaign (highest leverage).** This unit is a known
   vendor library with matched open-source reference decompilations (e.g.
   PrimeDecomp's `musyx/runtime`: synth.c, synthmacros.c, synthdata.c,
   synthvoice.c, seq.c, sal.c, hardware.c, snd3d.c, reverb_hi.c, s_data.c,
   dsp*). Port per-file, adapt to this build's MWCC flags. The 130 matched
   symbol anchors make file-boundary recovery straightforward. Potential:
   most of the ~267 unmatched functions in the "people_field" unit.
   Check `archive/previous_campaign/config/GC6E01/compile_config.json` for a
   dedicated audio-lib MWCC version (memory: this unit already needs
   `-use_lmw_stmw off`, consistent with a separately-built library).
2. **Re-locate the real people/NPC system.** `people.h` claims 0x80180C78+
   for the floor-level system; the actual field-NPC code must be found there
   or elsewhere — a fresh evidence-first audit, not reusing people_field.h.
3. **Header cleanup pass (needs build reason + review).** Retire/rename the
   fictional people_field.h types; rename `peopleFieldCompare*TableKey` and
   `peopleFieldMotionResolveInput*` to MusyX-consistent names. Do only with
   report-verified builds, preserving fn_ address traceability.
4. **people_data audit.** fn_80144574's callees (fn_801440A0 "48 callers",
   fn_80143DFC, fn_80143A94) are item-data accessors, so "people_data"
   (0x80140588-0x80144574) is likely item/pokemon-data code, not people data.

## Immediate Small Matching Targets (scout-verified recipes)

| Target | Size | Fuzzy | Recipe |
| --- | ---: | ---: | --- |
| fn_80162FB0 | 92 | 98.7% | Reorder float expr: `constf * (f32)arg / (f32)global`; 0x4330 idiom falls out. Trivial. |
| fn_80162070 | 28 | 10.0% | PRNG step: `seed = seed * 0xA8351D63u; return (seed >> 6) & 0xFFFF;` on lbl_80478BF0. Trivial. |
| fn_80163490 | 24 | 31.7% | Spin-wait on `volatile u8` at lbl_8044FB90+0x281 (far data, lis/addi). Trivial. |
| fn_801631AC | 20 | 97.6% | 8-byte copy idiom: assign {u32,u32} struct (or u64) global pair lbl_8047B054/58 in one statement. |
| fn_801628C8 | 60 | 98.3% | Add unused u8 second param to fn and to salActivateVoice prototype so r4 is reserved (scratch regs shift). |
| fn_80162EB8 | 144 | 98.1% | ARAM flush: mutate params in place (`size += offset & 0x1F; size = (size+0x1F) & ~0x1F`), drop register hints. |
| fn_80162370 | 184 | 99.1% | hw init: widen params 2/3 and let implicit narrowing at u8-prototyped salInitDspCtrl emit the clrlwi's. |
| fn_801522F0 / fn_8015234C | 92x2 | 70.9% | Twin sndBSearch wrappers; need sndBSearch prototype + SDA key struct; note li rX,sym@sda21 address-taken idiom. |
| fn_80153FEC family | 260x11 | 43.4% | Byte-identical modulo 3 constants (flag bit, sub-struct offset 0x23C..0x3A4, ori mask). Fix shared macro: +0x114 must be u64 flag field with u64 mask; then all 11 match from one patch. These are the mcmd controller-macro family; offsets 0x23C..0x3A4 match the `peopleFieldMotionResolveInput23C..3C8` resolver names. |

## Provenance

Multi-agent audit (7 independent per-function auditors + 1 target scout),
run 2026-07-01. Synthesis/verification phases were interrupted; per-function
results were recovered from the workflow journal and the load-bearing claim
(MusyX identity) was independently re-verified against symbols.txt by direct
range grep (130 MusyX names). Raw audit JSON: workflow run wf_f34bf20e-193,
session 6d847024. Validation for this docs-only pass: `git diff --check`
clean; baseline `configure.py --no-progress` + `ninja all_source
build/GC6E01/report.json` ran clean before audit (3313/8603 matched,
people_field 83/350 @ 6.38% fuzzy).
