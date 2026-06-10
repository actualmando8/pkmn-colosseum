# decomp notes: people/people_field.c

## Solved

### 2026-06-10: `_GetInputValue` symbol-name lever — 11-function family to 100%

The `fn_8016161C`..`fn_801618EC` family (11 siblings, stride 0x48) was stuck at
**99.72%**. The C bodies were already byte-identical to the target — the ONLY
diff was the relocation symbol name on the tail call:

- target `.o`: `bl _GetInputValue`
- our base `.o`: `bl fn_80161134`

Address 0x80161134 is the same on both sides; the target object was built from
`config/GC6E01/symbols.build.txt` which names it `_GetInputValue`, while our
build resolves the C identifier verbatim. Renaming the call (and its `extern`
decl) `fn_80161134` -> `_GetInputValue` in the TU makes CW emit a reloc to the
matching symbol name. **Byte-neutral, authoritative** (the target's own reloc
literally reads `_GetInputValue`, not a structural-port guess).

Result: all 11 family members -> **100.0%** with no code change.

| fn | before | after |
|----|--------|-------|
| fn_8016161C | 99.72 | 100.0 |
| fn_80161664 | 99.72 | 100.0 |
| fn_801616AC | 99.72 | 100.0 |
| fn_801616F4 | 99.72 | 100.0 |
| fn_8016173C | 99.72 | 100.0 |
| fn_80161784 | 99.72 | 100.0 |
| fn_801617CC | 99.72 | 100.0 |
| fn_80161814 | 99.72 | 100.0 |
| fn_8016185C | 99.72 | 100.0 |
| fn_801618A4 | 99.72 | 100.0 |
| fn_801618EC | 99.72 | 100.0 |

LESSON: a 99.x C-active near-miss whose only objdiff DIFF is a `bl <name>`
reloc is a symbol-naming artifact, not reg-alloc. Check the target's reloc name
(`.inc` `bl _Name` or `symbols.build.txt`) and rename the C identifier to match.
