# TU attribution from __FILE__ string xrefs

Ground truth: every function referencing a `*.c` string literal was compiled from that file (CodeWarrior `__FILE__`). Filename folding treats `cobj.c`≡`hsd_cobj.c`, `DVD.c`≡`dvd.c`, etc.

- ✅ **match** — string evidence agrees with the assigned source.
- 🔀 **relabel** — range assigned the wrong file; code is a different TU.
- ✂️ **split** — range spans 2+ source files; needs a split point.

## Range cross-check (existing splits vs. string evidence)

| range | status | assigned src | .c strings referenced | finding |
|---|---|---|---|---|
| `0x800055e0`-`0x80006630` | KNOWN | main.c | `error.c`×2 | 🔀 relabel (assigned main.c but code is error.c) |
| `0x80033278`-`0x80035e04` | GAP | gs_range_80033278.c | `menuCardE.c`×1 | 🔀 relabel (assigned gs_range_80033278.c but code is menuCardE.c) |
| `0x8005344c`-`0x80069c0c` | GAP | gs_range_8005344C.c | `menuCB_Battle.c`×1 | 🔀 relabel (assigned gs_range_8005344C.c but code is menuCB_Battle.c) |
| `0x8007109c`-`0x8007c260` | GAP | menu_range_8007109C.c | `menuCB_Common.c`×5, `menuCB_Rule.c`×4, `menuToolBattle.c`×1, `menuExDiscShrine.c`×1, `menuExDiscCoupon.c`×1 | ✅ match |
| `0x8007c260`-`0x8007c300` | KNOWN | menu_poke_coupon.c | `menuPokeCoupon.c`×1 | ✅ match |
| `0x8007c300`-`0x8007fd64` | KNOWN | menu_carde_matrix.c | `menuCardE_Matrix.c`×4 | ✅ match |
| `0x8007fd64`-`0x80088428` | GAP | gs_range_8007FD64.c | `cardesavedata.c`×11 | 🔀 relabel (assigned gs_range_8007FD64.c but code is cardesavedata.c) |
| `0x80089048`-`0x800896b8` | GAP | field_range_80089048.c | `pokeconv.c`×2 | 🔀 relabel (assigned field_range_80089048.c but code is pokeconv.c) |
| `0x8008c7b0`-`0x800980e0` | GAP | gs_range_8008C7B0.c | `gbaCommunication.c`×6 | 🔀 relabel (assigned gs_range_8008C7B0.c but code is gbaCommunication.c) |
| `0x800a4d28`-`0x800a5624` | GAP | sdk_range_800A4D28.c | `dvdfs.c`×4 | 🔀 relabel (assigned sdk_range_800A4D28.c but code is dvdfs.c) |
| `0x800a5624`-`0x800a7820` | KNOWN | DVD.c | `dvd.c`×2 | ✅ match |
| `0x800a8178`-`0x800aa280` | GAP | sdk_range_800A8178.c | `vi.c`×1 | 🔀 relabel (assigned sdk_range_800A8178.c but code is vi.c) |
| `0x800e202c`-`0x800ef098` | GAP | gs_range_800E202C.c | `parse.c`×1 | 🔀 relabel (assigned gs_range_800E202C.c but code is parse.c) |
| `0x80167040`-`0x80168c64` | KNOWN | gs_dvd.c | `GSdvd.c`×1 | ✅ match |
| `0x80168c64`-`0x8017572c` | GAP | ps_range_80168C64.c | `pslist.c`×2, `generator.c`×2, `psinterpret.c`×1 | ✂️ split (also holds: generator.c, psinterpret.c, pslist.c) |
| `0x801914f4`-`0x801920e4` | KNOWN | hsd_wobj.c | `wobj.c`×3 | ✅ match |
| `0x801938fc`-`0x80193c24` | KNOWN | hsd_class.c | `class.c`×1 | ✅ match |
| `0x80193c24`-`0x80197a64` | KNOWN | hsd_cobj.c | `cobj.c`×18 | ✅ match |
| `0x80197a64`-`0x80198f7c` | KNOWN | hsd_displayfunc.c | `displayfunc.c`×2 | ✅ match |
| `0x80198f7c`-`0x8019b7c0` | KNOWN | hsd_dobj.c | `fobj.c`×4, `dobj.c`×2 | ✂️ split (also holds: fobj.c) |
| `0x8019b7c0`-`0x8019c690` | KNOWN | hsd_fog.c | `fog.c`×2, `hash.c`×1, `id.c`×1 | ✂️ split (also holds: hash.c, id.c) |
| `0x8019c690`-`0x8019ce50` | KNOWN | hsd_initialize.c | `initialize.c`×4 | ✅ match |
| `0x8019ce50`-`0x801a4000` | KNOWN | hsd_jobj.c | `jobj.c`×13, `list.c`×2 | ✂️ split (also holds: list.c) |
| `0x801a4000`-`0x801a69c0` | KNOWN | hsd_lobj.c | `lobj.c`×7 | ✅ match |
| `0x801a69c0`-`0x801a8428` | GAP | hsd_range_801A69C0.c | `mobj.c`×6, `memory.c`×1 | ✂️ split (also holds: memory.c, mobj.c) |
| `0x801a84f0`-`0x801a85f0` | GAP | hsd_mobj_range_801A84F0.c | `mtx.c`×2 | 🔀 relabel (assigned hsd_mobj_range_801A84F0.c but code is mtx.c) |
| `0x801a86b4`-`0x801aa608` | GAP | hsd_mobj_range_801A86B4.c | `perf.c`×1 | 🔀 relabel (assigned hsd_mobj_range_801A86B4.c but code is perf.c) |
| `0x801aa608`-`0x801ae000` | GAP | hsd_pobj_range_801AA608.c | `pobj.c`×10, `robj.c`×3 | ✂️ split (also holds: robj.c) |
| `0x801ae008`-`0x801aebe0` | GAP | hsd_pobj_range_801AE008.c | `robj.c`×3 | 🔀 relabel (assigned hsd_pobj_range_801AE008.c but code is robj.c) |
| `0x801aebe4`-`0x801b009c` | GAP | hsd_pobj_range_801AEBE4.c | `robj.c`×6 | 🔀 relabel (assigned hsd_pobj_range_801AEBE4.c but code is robj.c) |
| `0x801b019c`-`0x801b1730` | KNOWN | hsd_shadow.c | `shadow.c`×10 | ✅ match |
| `0x801b1730`-`0x801bbac8` | KNOWN | hsd_texp.c | `texp.c`×15, `tev.c`×5 | ✂️ split (also holds: tev.c) |
| `0x801bbac8`-`0x801c01c8` | GAP | hsd_range_801BBAC8.c | `tobj.c`×14, `video.c`×3, `util.c`×1 | ✂️ split (also holds: tobj.c, util.c, video.c) |
| `0x801c01c8`-`0x801c0f20` | GAP | hsd_aobj_range_801C01C8.c | `aobj.c`×1 | ✅ match |
| `0x801c0f20`-`0x801c4cb8` | KNOWN | battle_grid.c | `aobj.c`×2 | 🔀 relabel (assigned battle_grid.c but code is aobj.c) |

## GAP-range attribution proposals

Functions in unattributed GAP regions that reference a `*.c` literal — these regions can be split to that TU.

- **`tobj.c`** — 14 GAP function(s), `0x801bbd84`..`0x801beedc` (e.g. HSD_ImageDescAlloc, fn_801BBDDC, HSD_Index2TexMtx, HSD_TObjSetup, …)
- **`robj.c`** — 12 GAP function(s), `0x801add48`..`0x801afcac` (e.g. fn_801ADD48, fn_801ADE50, fn_801ADF54, fn_801AE008, …)
- **`cardesavedata.c`** — 11 GAP function(s), `0x80082650`..`0x800836ac` (e.g. fn_80082650, fn_80082738, fn_80082960, fn_80082A88, …)
- **`pobj.c`** — 10 GAP function(s), `0x801aa8bc`..`0x801ad354` (e.g. fn_801AA8BC, fn_801AAEA8, fn_801AB538, fn_801AB67C, …)
- **`gbaCommunication.c`** — 6 GAP function(s), `0x80092c90`..`0x80093698` (e.g. fn_80092C90, fn_80092E38, fn_80092FC8, fn_80093160, …)
- **`mobj.c`** — 6 GAP function(s), `0x801a6ca4`..`0x801a7d58` (e.g. fn_801A6CA4, MObjSetupTev, fn_801A7128, fn_801A7B24, …)
- **`menuCB_Common.c`** — 5 GAP function(s), `0x8007109c`..`0x800715bc` (e.g. fn_8007109C, fn_80071104, fn_80071398, fn_800714C8, …)
- **`menuCB_Rule.c`** — 4 GAP function(s), `0x800767b8`..`0x800776e4` (e.g. fn_800767B8, fn_800772AC, fn_800774D4, fn_800776E4)
- **`dvdfs.c`** — 4 GAP function(s), `0x800a4d28`..`0x800a5558` (e.g. fn_800A4D28, fn_800A532C, fn_800A541C, fn_800A5558)
- **`video.c`** — 3 GAP function(s), `0x801bf6ac`..`0x801bff18` (e.g. fn_801BF6AC, fn_801BFA1C, fn_801BFF18)
- **`pokeconv.c`** — 2 GAP function(s), `0x80089048`..`0x80089380` (e.g. fn_80089048, fn_80089380)
- **`pslist.c`** — 2 GAP function(s), `0x80168c64`..`0x80168cd0` (e.g. _psListGetFirst, _psListDelete)
- **`generator.c`** — 2 GAP function(s), `0x8017424c`..`0x8017424c` (e.g. generateParticle_8017424C, generateParticle_801947D4)
- **`mtx.c`** — 2 GAP function(s), `0x801a8524`..`0x801a85a4` (e.g. HSD_MtxAlloc, HSD_VecAlloc)
- **`menuCardE.c`** — 1 GAP function(s), `0x80033278`..`0x80033278` (e.g. fn_80033278)
- **`menuCB_Battle.c`** — 1 GAP function(s), `0x80069a60`..`0x80069a60` (e.g. fn_80069A60)
- **`menuToolBattle.c`** — 1 GAP function(s), `0x8007581c`..`0x8007581c` (e.g. fn_8007581C)
- **`menuExDiscShrine.c`** — 1 GAP function(s), `0x80077ed4`..`0x80077ed4` (e.g. fn_80077ED4)
- **`menuExDiscCoupon.c`** — 1 GAP function(s), `0x800792d8`..`0x800792d8` (e.g. fn_800792D8)
- **`vi.c`** — 1 GAP function(s), `0x800a94ac`..`0x800a94ac` (e.g. fn_800A94AC)
- **`parse.c`** — 1 GAP function(s), `0x800eafe4`..`0x800eafe4` (e.g. fn_800EAFE4)
- **`psinterpret.c`** — 1 GAP function(s), `0x8016f430`..`0x8016f430` (e.g. psInterpretParticles)
- **`memory.c`** — 1 GAP function(s), `0x801a69c0`..`0x801a69c0` (e.g. _HSD_MemSetCallbacks)
- **`perf.c`** — 1 GAP function(s), `0x801aa5ac`..`0x801aa5ac` (e.g. fn_801AA5AC)
- **`util.c`** — 1 GAP function(s), `0x801bf098`..`0x801bf098` (e.g. HSD_Index2PosNrmMtx)
- **`aobj.c`** — 1 GAP function(s), `0x801c028c`..`0x801c028c` (e.g. HSD_ForeachAnim)
