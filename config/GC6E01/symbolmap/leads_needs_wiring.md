# Confirmed-name leads needing signature/typing work

These functions' names are doubly-confirmed — the XD decomp AND a typed prototype already in this project's headers agree — but the asm-wrapper's `(void)` signature and untyped `lbl_` globals at the call sites conflict with the real prototype. Wiring each is per-function decomp work (match the header prototype, type its globals), so they are left as `fn_` for now rather than bulk-renamed.

| addr (fn_) | confirmed name | provenance | header proto in |
|---|---|---|---|
| `fn_800EF5FC` | GStextureCreate | XD port (score 2.0) | G:/decomp-worktrees/symbolmap/include/game/gs_texture.h |
| `fn_800F07A8` | GSthreadCreate | string self-name (leading) | G:/decomp-worktrees/symbolmap/include/game/gs_thread.h |
| `fn_80114948` | floorReadCameraPreFunc | string self-name (leading) | ? |
| `fn_80114CA8` | floorReadMapPreFunc | string self-name (leading) | ? |
| `fn_80117514` | floorUpdateFieldCamera | string self-name (leading) | G:/decomp-worktrees/symbolmap/include/game/world/gs_field.h |
| `fn_8013111C` | GSeffect | string self-name (leading) | G:/decomp-worktrees/symbolmap/include/game/world/gs_field.h |
| `fn_80137AA4` | tracefxStartEffect | string self-name (leading) | G:/decomp-worktrees/symbolmap/include/game/effect/gs_effect.h |
| `fn_8013B158` | filterStart | string self-name (leading) | G:/decomp-worktrees/symbolmap/include/game/effect/effect_visual.h |
| `fn_8013B5E4` | surfEffectStart | string self-name (leading) | G:/decomp-worktrees/symbolmap/include/game/effect/effect_visual.h |
| `fn_8013C718` | seaEffectStart | string self-name (leading) | G:/decomp-worktrees/symbolmap/include/game/effect/effect_visual.h |
| `fn_8013D804` | envMapEffectInit | string self-name (leading) | G:/decomp-worktrees/symbolmap/include/game/effect/effect_visual.h |
| `fn_8013DDCC` | blurEffectStart | string self-name (leading) | G:/decomp-worktrees/symbolmap/include/game/effect/effect_visual.h |
| `fn_8013E658` | auraEffectStart | string self-name (leading) | G:/decomp-worktrees/symbolmap/include/game/effect/effect_visual.h |
| `fn_8013F344` | distortionEffectStart | string self-name (leading) | G:/decomp-worktrees/symbolmap/include/game/effect/effect_visual.h |
| `fn_8013FDD0` | billboardEffectStart | string self-name (leading) | G:/decomp-worktrees/symbolmap/include/game/effect/effect_visual.h |
| `fn_80167318` | _sndCheckSndWorkALL | XD port (score 2.0) | G:/decomp-worktrees/symbolmap/include/game/gs_dvd.h |
| `fn_80193B30` | hsdInitClassInfo | XD port (score 3.0) | G:/decomp-worktrees/symbolmap/include/hsd/hsd_class.h |
| `fn_80195794` | HSD_CObjGetEyeDistance | XD port (score 2.0) | G:/decomp-worktrees/symbolmap/include/hsd/hsd_cobj.h |
| `fn_801993A4` | DObjLoad | XD port (score 2.0) | ? |
| `fn_801BBFE4` | HSD_TObjSetup | XD port (score 2.25) | G:/decomp-worktrees/symbolmap/include/hsd/hsd_tobj.h |
