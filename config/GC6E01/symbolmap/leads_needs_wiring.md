# Confirmed-name leads needing signature/typing work

11 remaining (of the original proposals). These names are confirmed but the asm-wrapper `(void)` signature / untyped `lbl_` globals / a name collision block a clean bulk rename — each needs per-function decomp work (match the real prototype, type its globals). Wired leads have been moved to applied_symbols.txt.

| addr (fn_) | confirmed name | provenance | header proto in |
|---|---|---|---|
| `fn_801993A4` | DObjLoad | XD port (score 2.0) | ? |
| `fn_8013111C` | GSeffect | string self-name (leading) | G:/decomp-worktrees/symbolmap/include/game/world/gs_field.h |
| `fn_800EF5FC` | GStextureCreate | XD port (score 2.0) | G:/decomp-worktrees/symbolmap/include/game/gs_texture.h |
| `fn_800F07A8` | GSthreadCreate | string self-name (leading) | G:/decomp-worktrees/symbolmap/include/game/gs_thread.h |
| `fn_80195794` | HSD_CObjGetEyeDistance | XD port (score 2.0) | G:/decomp-worktrees/symbolmap/include/hsd/hsd_cobj.h |
| `fn_801BBFE4` | HSD_TObjSetup | XD port (score 2.25) | G:/decomp-worktrees/symbolmap/include/hsd/hsd_tobj.h |
| `fn_80114948` | floorReadCameraPreFunc | string self-name (leading) | ? |
| `fn_80114CA8` | floorReadMapPreFunc | string self-name (leading) | ? |
| `fn_80117514` | floorUpdateFieldCamera | string self-name (leading) | G:/decomp-worktrees/symbolmap/include/game/world/gs_field.h |
| `fn_80193B30` | hsdInitClassInfo | XD port (score 3.0) | G:/decomp-worktrees/symbolmap/include/hsd/hsd_class.h |
| `fn_80137AA4` | tracefxStartEffect | string self-name (leading) | G:/decomp-worktrees/symbolmap/include/game/effect/gs_effect.h |
