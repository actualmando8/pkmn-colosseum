# Structural decomp workflow (gs_model.c pivot) — shared by all lanes

We are pivoting from byte-exact near-miss polishing (saturated on reg-alloc walls)
to STRUCTURAL decomp: turning still-asm functions into faithful real C. gs_model.c
is the target — every fn shares the model / joint / TObj / DObj / PObj structs, so a
type you reverse-engineer helps every other lane. THIS is where the KG cooperative
sharing actually pays off.

## RULE 0 — PRESERVE CANON SIGNATURES (do this or your win won't integrate)
Before writing any C, read the fn's CURRENT signature in canon:
  grep -nE 'fn_XXXXXXXX *\(' src/<file>.c
KEEP that EXACT signature — return type, parameter types AND names. Do NOT invent a
new one (e.g. canon `f32 fn_801D14C0(void* entry)` must NOT become `s32 fn(s32 idx)`).
The integrator splices your BODY against canon's signature; a changed signature leaves
your locals undefined and the win is rejected. If you believe canon's signature is wrong,
REPORT it (one line) — do not change it. We ground every lane to canon so the wins merge.

## Per-function loop
1. m2c first draft:
     python3 tools/decomp_work/m2c_draft.py <fn> src/game/gs_model.c
   -> writes tools/decomp_work/m2c_out/<fn>.c (CodeWarrior-shaped C draft)
2. Understand the fn — use the KG, do NOT guess blindly:
     kg="python tools/decomp_work/kg/kg.py"
     $kg q call-neighborhood <fn>     # callers/callees + TU context
     $kg q name-evidence <fn>         # name candidates + calltag evidence
3. Rewrite the m2c draft into FAITHFUL real C in your band scratch — use the TU's
   REAL struct types from the headers (HSD_*, GSModel*, etc.), not m2c's raw
   `*(u32*)(x+0x4)` casts. The goal is C a human would have written.
4. Measure:
     tools/decomp_work/decomp.sh band check <tag> <fn>
     tools/decomp_work/decomp.sh band diff  <tag> <fn>
5. Refine toward 100%. If you reach high-% faithful C but hit a reg-alloc/scheduling
   wall, LEAVE THE DRAFT IN scratch/band_<tag>.c (do not revert) and report the %
   so the next round can finish it. `band save` still only writes canon at 100.00.

## Cooperative struct sharing (the point of this round)
- If a fn needs a struct FIELD that the header doesn't define yet, add a LOCAL cast
  for now and REPORT the discovery (offset + inferred type + which struct) in your
  pane. DO NOT concurrently edit shared headers — the orchestrator consolidates
  struct field discoveries across lanes to avoid merge conflicts.
- After any byte-exact 100% win: $kg record-crack <fn> <lever-slug> --delta "asm -> 100% real C"

## Hard rules (enforced by the gate)
- Real C only. NEVER asm{} storage / inline-asm / #include "*_fn_*.inc". NEVER flip #if 0 -> #if 1.
- Edit ONLY your own scratch/band_<tag>.c. `band save` refuses < 100%.
- Report per fn:  REALC <fn> <best%> (struct notes: ...)  |  SAVED <fn> 100.00  |  WALL <fn> <best%> <residual>
