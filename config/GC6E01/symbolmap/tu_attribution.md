# TU attribution from __FILE__ string xrefs

Ground truth: every function referencing a `*.c` string literal was compiled from that file (CodeWarrior `__FILE__`). Filename folding treats `cobj.c`≡`hsd_cobj.c`, `DVD.c`≡`dvd.c`, etc.

- ✅ **match** — string evidence agrees with the assigned source.
- 🔀 **relabel** — range assigned the wrong file; code is a different TU.
- ✂️ **split** — range spans 2+ source files; needs a split point.

## Range cross-check (existing splits vs. string evidence)

| range | status | assigned src | .c strings referenced | finding |
|---|---|---|---|---|
| `0x800055e0`-`0x80006630` | KNOWN | main.c | `error.c`×2 | 🔀 relabel (assigned main.c but code is error.c) |
| `0x80033278`-`0x80035e04` | GAP | gs_range_80033278.c | `menuCardE.c`×2 | 🔀 relabel (assigned gs_range_80033278.c but code is menuCardE.c) |
| `0x8005344c`-`0x80069c0c` | KNOWN | menuCB_Battle.c | `menuCB_Battle.c`×1 | ✅ match |
| `0x8007109c`-`0x8007c260` | GAP | menu_range_8007109C.c | `menuCB_Common.c`×7, `menuCB_Rule.c`×4, `menuToolBattle.c`×1, `menuExDiscShrine.c`×1, `menuExDiscCoupon.c`×1 | ✅ match |
| `0x8007c260`-`0x8007c300` | KNOWN | menu_poke_coupon.c | `menuPokeCoupon.c`×1 | ✅ match |
| `0x8007c300`-`0x8007fd64` | KNOWN | menu_carde_matrix.c | `menuCardE_Matrix.c`×4 | ✅ match |
| `0x8007fd64`-`0x80088428` | KNOWN | cardesavedata.c | `cardesavedata.c`×11 | ✅ match |
| `0x80089048`-`0x800896b8` | KNOWN | pokeconv.c | `pokeconv.c`×2 | ✅ match |
| `0x8008c7b0`-`0x800980e0` | KNOWN | gbaCommunication.c | `gbaCommunication.c`×6 | ✅ match |
| `0x800a4d28`-`0x800a5624` | GAP | dvdfs_range_800A4D28.c | `dvdfs.c`×8 | ✅ match |
| `0x800a5624`-`0x800a7820` | KNOWN | DVD.c | `dvd.c`×3 | ✅ match |
| `0x800a8178`-`0x800aa280` | GAP | VI_range_800A8178.c | `vi.c`×2 | 🔀 relabel (assigned VI_range_800A8178.c but code is vi.c) |
| `0x800e202c`-`0x800ef098` | GAP | gs_range_800E202C.c | `parse.c`×1 | 🔀 relabel (assigned gs_range_800E202C.c but code is parse.c) |
| `0x80167040`-`0x80168c64` | KNOWN | gs_dvd.c | `GSdvd.c`×1 | ✅ match |
| `0x80168c64`-`0x8017572c` | GAP | ps_range_80168C64.c | `pslist.c`×2, `generator.c`×2, `psinterpret.c`×1 | ✂️ split (also holds: generator.c, psinterpret.c, pslist.c) |
| `0x801914f4`-`0x801920e4` | KNOWN | hsd_wobj.c | `wobj.c`×3 | ✅ match |
| `0x801938fc`-`0x80193c24` | KNOWN | hsd_class.c | `class.c`×1 | ✅ match |
| `0x80193c24`-`0x80197a64` | KNOWN | hsd_cobj.c | `cobj.c`×29 | ✅ match |
| `0x80197a64`-`0x80198f7c` | KNOWN | hsd_displayfunc.c | `displayfunc.c`×4 | ✅ match |
| `0x80198f7c`-`0x8019b7c0` | KNOWN | hsd_dobj.c | `fobj.c`×7, `dobj.c`×3 | ✂️ split (also holds: fobj.c) |
| `0x8019b7c0`-`0x8019c690` | KNOWN | hsd_fog.c | `fog.c`×4, `hash.c`×1, `id.c`×1 | ✂️ split (also holds: hash.c, id.c) |
| `0x8019c690`-`0x8019ce50` | KNOWN | hsd_initialize.c | `initialize.c`×4 | ✅ match |
| `0x8019ce50`-`0x801a4000` | KNOWN | hsd_jobj.c | `jobj.c`×21, `list.c`×3 | ✂️ split (also holds: list.c) |
| `0x801a4000`-`0x801a69c0` | KNOWN | hsd_lobj.c | `lobj.c`×9 | ✅ match |
| `0x801a69c0`-`0x801a6ca4` | KNOWN | hsd_memory.c | `memory.c`×1 | ✅ match |
| `0x801a6ca4`-`0x801a8428` | KNOWN | hsd_mobj.c | `mobj.c`×11 | ✅ match |
| `0x801a84f0`-`0x801a85f0` | KNOWN | hsd_mtx.c | `mtx.c`×2 | ✅ match |
| `0x801a86b4`-`0x801aa608` | GAP | hsd_mobj_range_801A86B4.c | `perf.c`×1 | 🔀 relabel (assigned hsd_mobj_range_801A86B4.c but code is perf.c) |
| `0x801aa608`-`0x801add48` | GAP | hsd_pobj_range_801AA608.c | `pobj.c`×15 | ✅ match |
| `0x801add48`-`0x801ae000` | GAP | hsd_robj_range_801ADD48.c | `robj.c`×3 | ✅ match |
| `0x801ae008`-`0x801aebe0` | GAP | hsd_robj_range_801AE008.c | `robj.c`×4 | ✅ match |
| `0x801aebe4`-`0x801b009c` | GAP | hsd_robj_range_801AEBE4.c | `robj.c`×6 | ✅ match |
| `0x801b019c`-`0x801b1730` | KNOWN | hsd_shadow.c | `shadow.c`×10 | ✅ match |
| `0x801b1730`-`0x801bbac8` | KNOWN | hsd_texp.c | `texp.c`×19, `tev.c`×5 | ✂️ split (also holds: tev.c) |
| `0x801bbac8`-`0x801bf098` | KNOWN | hsd_tobj.c | `tobj.c`×22 | ✅ match |
| `0x801bf098`-`0x801bf6ac` | KNOWN | hsd_util.c | `util.c`×1 | ✅ match |
| `0x801bf6ac`-`0x801c01c8` | KNOWN | hsd_video.c | `video.c`×3 | ✅ match |
| `0x801c01c8`-`0x801c0f20` | GAP | hsd_aobj_range_801C01C8.c | `aobj.c`×1 | ✅ match |
| `0x801c0f20`-`0x801c4cb8` | KNOWN | battle_grid.c | `aobj.c`×2 | 🔀 relabel (assigned battle_grid.c but code is aobj.c) |

## GAP-range attribution proposals

Functions in unattributed GAP regions that reference a `*.c` literal — these regions can be split to that TU.

- **`pobj.c`** — 15 GAP function(s), `0x801aa8bc`..`0x801ad354` (e.g. HSD_PObjDisp, fn_801AA8BC, SetupEnvelopeModelMtx, fn_801AAEA8, …)
- **`robj.c`** — 13 GAP function(s), `0x801add48`..`0x801afcac` (e.g. fn_801ADD48, fn_801ADE50, fn_801ADF54, fn_801AE008, …)
- **`dvdfs.c`** — 8 GAP function(s), `0x800a4d28`..`0x800a5558` (e.g. DVDConvertPathToEntrynum, fn_800A4D28, DVDReadAsync, fn_800A532C, …)
- **`menuCB_Common.c`** — 7 GAP function(s), `0x8007109c`..`0x800715bc` (e.g. fn_8007109C, fn_80071104, fn_80071398, _menuPop, …)
- **`menuCB_Rule.c`** — 4 GAP function(s), `0x800767b8`..`0x800776e4` (e.g. fn_800767B8, fn_800772AC, fn_800774D4, fn_800776E4)
- **`menuCardE.c`** — 2 GAP function(s), `0x80033278`..`0x80033278` (e.g. _sysvarsProcessData__FP16sysvarsFuncEntryPc, fn_80033278)
- **`vi.c`** — 2 GAP function(s), `0x800a94ac`..`0x800a94ac` (e.g. VIConfigure, fn_800A94AC)
- **`pslist.c`** — 2 GAP function(s), `0x80168c64`..`0x80168cd0` (e.g. _psListGetFirst, _psListDelete)
- **`generator.c`** — 2 GAP function(s), `0x8017424c`..`0x8017424c` (e.g. generateParticle_8017424C, generateParticle_801947D4)
- **`menuToolBattle.c`** — 1 GAP function(s), `0x8007581c`..`0x8007581c` (e.g. fn_8007581C)
- **`menuExDiscShrine.c`** — 1 GAP function(s), `0x80077ed4`..`0x80077ed4` (e.g. fn_80077ED4)
- **`menuExDiscCoupon.c`** — 1 GAP function(s), `0x800792d8`..`0x800792d8` (e.g. fn_800792D8)
- **`parse.c`** — 1 GAP function(s), `0x800eafe4`..`0x800eafe4` (e.g. fn_800EAFE4)
- **`psinterpret.c`** — 1 GAP function(s), `0x8016f430`..`0x8016f430` (e.g. psInterpretParticles)
- **`perf.c`** — 1 GAP function(s), `0x801aa5ac`..`0x801aa5ac` (e.g. fn_801AA5AC)
- **`aobj.c`** — 1 GAP function(s), `0x801c028c`..`0x801c028c` (e.g. HSD_ForeachAnim)
