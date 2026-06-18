# why_diff re-triage of the worker near-miss queue — 2026-06-17

The blind near-miss queue the worker (%5) was grinding was routed by why_diff.py.
**0 of 5 were correctly-routed worker reg-alloc cracks.** This is the case the
ORCHESTRATOR_BRIEFING predicted: workers burn hours on functions that are walls,
stubs, or permuter cases. Route by why_diff BEFORE assigning a worker.

| fn | TU | objdiff% | why_diff verdict | correct route |
|---|---|---|---|---|
| fn_80034DC0 | gs_npc_event.c | 95.27 | SCHEDULE — `li r6,15560` (0x3CC8 arg) hoisted; 5 same-insn blocks | **permuter** anneal_pragma (running; peephole_off reached score 10) |
| fn_801839A0 | people/people.c | "98.86" | **STUB** — ours = 1 insn / frame 0x0 / 0 calls vs target 105 insn / frame 0x40 / 8 calls. The % was an asm-wrapper artifact. | **full Phase-1 decomp** (m2c_draft + Ghidra), NOT a crack |
| fn_80209960 | colosseum_event.c | 98.95 | why_diff MATCH + objdiff<100 ⇒ reloc artifact; already filed **W3 stmw** wall | leave walled |
| fn_80211A78 | colosseum_event.c | 98.45 | DIFFERENT-INSN = `r13 @sda21` vs absolute `@ha` (SDA reloc); already filed **W2/W3** wall | leave walled |
| fn_8020E7AC | colosseum_event.c | 96.44 | DIFFERENT-INSN — `clrlwi. r0,r0,24;bne` (target) vs `cmplwi r0,0;beq` (ours): byte-test + branch-sense. Filed as **W1 reg-alloc tie-break** — why_diff CONTRADICTS that cause. | worker re-attempt on the **bool-test expression shape** (NOT reg-alloc) |

## Lesson encoded
- **why_diff MATCH but objdiff < 100% ⇒ a reloc/representation WALL** (@sda21 vs
  absolute, stmw vs individual stw). The instruction stream matches; the residual
  is non-C-fixable section/reloc placement. Do not grind these.
- **ours = ~1 insn ⇒ STUB, not a near-miss.** The headline % came from the inactive
  asm wrapper, not the active C. Route to full decomp.
- **why_diff can re-open a misfiled wall.** fn_8020E7AC was filed as a saved-band
  reg-alloc tie-break on 2026-06-10; why_diff shows the FIRST divergence is an
  expression (byte-test + branch-sense), which a worker can attack — the filed
  cause is wrong.

## fn_8020E7AC worker packet (the one genuine worker-actionable item)
- Target insn 36-38: `clrlwi. r0,r0,24` then `bne` (positive sense, branch when
  `(u8)X != 0`). Ours: `cmplwi r0,0` then `beq` (branch when `X == 0`).
- Tried-and-FAILED lever: typing `bVar1` as `u8` → 95.97% (regressed from 96.44).
  So the `clrlwi.` value is NOT `bVar1`.
- Next levers to try: invert the `if (bVar1 == 0) goto LAB` (line ~8468, the
  matchVal<0 branch) and the `if ((bVar1) && ...)` (line ~8483) to the positive
  truthy sense so CW emits `bne`; check whether the tested value is a `(u8)` cast
  of a callee return (`fn_801FBF04`) or the `(uVar5 & 0xff) < 4` loop guard.
- 4 reg-renames + 4 structural blocks remain past the first divergence; the
  expression fix may not reach 100% alone, but it is the correct first move and
  it re-opens the wall.
