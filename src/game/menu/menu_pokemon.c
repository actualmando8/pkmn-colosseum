/**
 * @file menu_pokemon.c
 * @brief Pokemon summary screen and party management UI.
 *
 * Implements the Pokemon summary screen, party list view, and all sub-screens
 * for viewing and managing Pokemon in the player's party. This is one of the
 * largest modules in the unattributed gap, containing 36 functions spanning
 * nearly 50KB of code.
 *
 * Key behaviors:
 *   - Uses BSS lbl_803A6818 (0x248 bytes) as the primary summary screen state.
 *     29 of 36 functions reference this structure directly.
 *   - The state machine at offset +0x1C in the BSS struct controls which
 *     sub-screen is active (values 0-13 observed in fn_8003D1FC's switch)
 *   - fn_80044630 (0x1B38 = 6,968 bytes) is the largest function -- the main
 *     summary screen update loop, dispatching to stat pages, move lists, etc.
 *   - fn_80042658 (0x10D0 = 4,304 bytes) handles party list selection
 *   - fn_80040308 (0xE0C = 3,596 bytes) handles summary page rendering
 *   - Heavily calls Pokemon data accessors fn_801FAA58 (129x) and
 *     fn_801FB1C0 (112x) from the pokemon.c / trainer.c area
 *   - Also references fn_8001E224 / fn_8001E200 (91x each) for string/text
 *     message lookups
 *   - Uses internal helpers fn_800478B4 and fn_80047CC0 for party cursor
 *     navigation and selection UI
 *   - Material/render setup via fn_800E01D0 using data from lbl_802E5448
 *   - Also uses lbl_803A67E8 (0x14 bytes) and lbl_803A67FC (0x1C bytes)
 *     for party selection sub-state
 *
 * BSS usage:
 *   - lbl_803A6818 (0x248 bytes): Summary screen state structure
 *   - lbl_803A67E8 (0x14 bytes): Party sub-state A
 *   - lbl_803A67FC (0x1C bytes): Party selection cursor state
 *
 * Address range: 0x8003D1FC - 0x800495C8 (36 functions)
 */

#include "dolphin/types.h"

/* ===== GS Engine ===== */
extern void  fn_800F0308(void);           /* GSthread yield */
extern void  fn_800E01D0(void* dst, void* src); /* material copy */
extern u32   fn_80102568(u32 a, u32 b, u32 c); /* scene load */
extern u32   fn_8010264C(u32 a, u32 b);        /* scene query */
extern u32   fn_80102510(u32 a);                /* scene unload */
extern void  fn_801026A4(u32 sceneId, u32 a, u32 b, u32 c,
                         u32 d, u32 e, ...);
extern u32   fn_80102428(u32 a);                /* scene property get */
extern void  fn_80109220(u32 obj, u8 visible);  /* model visibility set */
extern void  fn_800FB680(u32 a, u32 b, s32 c, u32 d); /* sound trigger */
extern void  fn_80132A38(u32 effectId, u32 param);     /* effect trigger */
extern void  fn_80135938(u32 a, u32 b);         /* effect alloc */
extern void  fn_801240C4(void* obj, u16 species, u32 form); /* people set species */
extern void* fn_80123FBC(void* obj);            /* people get */
extern void  fn_8012640C(void* obj);            /* people field update */
extern void  fn_8011DFE0(void* obj, u32 pos);  /* people position set */
extern void  fn_8011DF90(void* obj, u32 rot);  /* people rotation set */
extern void  fn_80177A44(u32 sceneType);        /* scene type set */
extern u32   fn_80105624(u32 a);                /* model state query */
extern u32   fn_801080CC(u32 a);                /* model anim check */
extern void  fn_800FA280(void);                 /* field utility */
extern void  fn_800FA444(void* obj);            /* field model update */
extern void  fn_800D61E4(void* obj);            /* render setup */
extern void  fn_800D5CB8(void* obj);            /* render cleanup */
extern void  fn_800D20CC(void* obj);            /* render matrix */
extern void  fn_8011F5C8(u32 a);               /* field world query */
extern void  fn_8011E778(u32 a);               /* field world accessor */

/* ===== Pokemon / Trainer data ===== */
extern void* fn_801FAA58(u32 slot);       /* Get Pokemon from party slot */
extern u32   fn_801FB1C0(void* pkmn, u32 field);  /* Get Pokemon field data */
extern u32   fn_8025FDDC(u32 a, u16 species);     /* Species data accessor A */
extern u32   fn_8025FD34(u32 a, u16 species);     /* Species data accessor B */
extern void* fn_801FBFBC(u16 species);             /* Species base data get */

/* ===== Text / Messages ===== */
extern void* fn_8001E224(u32 msgBank, u32 msgId);  /* Message string get A */
extern u32   fn_8001E200(u32 msgBank, u32 msgId);  /* Message string get B */
extern s8    fn_8001E074(u32 a, u32 b, u32 c, u32 d); /* Input wait / text input */

/* ===== Battle system ===== */
extern void  fn_801D1F7C(u32 a);           /* Battle data accessor */
extern void  fn_801D16F0(u32 a);           /* Battle type query */

/* ===== Internal helpers ===== */
extern void  fn_800478B4(void* party, void* cursor);  /* Party cursor update */
extern void  fn_80047CC0(void* party);                 /* Party cursor navigate */
extern void  fn_800492CC(void* state);                 /* Party selection finalize */

/* ===== BSS data ===== */
extern u8    lbl_803A67E8[];   /* Party sub-state A (0x14 bytes) */
extern u8    lbl_803A67FC[];   /* Party selection cursor (0x1C bytes) */
extern u8    lbl_803A6818[];   /* Summary screen state (0x248 bytes) */

/* ===== Data section ===== */
extern const u8 lbl_802E5448[];  /* Material preset data */

/* ===== Rodata tables ===== */
extern const u32 lbl_80267060[];  /* Summary screen layout rects (8 entries, 0x18 per entry) */
extern const u32 lbl_80267120[];  /* Page index table [0,1,2,3] */
extern const u32 lbl_80267130[];  /* Message IDs for stat labels [0x395,0x396,0x397,0] */
extern const u32 lbl_80267140[];  /* Message IDs for stat pages [0x39A,0x39C,0x39B,0x39D] */
extern const f32 lbl_80267150[];  /* Summary screen float positions (12 floats) */
extern const f32 lbl_80267180[];  /* Color values [255.0, 255.0, 255.0, 0.0] */
extern const f32 lbl_80267190[];  /* Camera offset [14.0, -12.0, 10.0] */
extern const u16 lbl_8026719C[];  /* Stat bar positions (12 halfwords) */
extern const u16 lbl_802671B4[];  /* Stat bar positions alt (14 halfwords) */
extern const u32 lbl_802671D0[];  /* Type icon message IDs (12 entries: 0x462-0x472) */
extern const u32 lbl_80267200[];  /* Level bar segments descending (0x47C-0x473) */
extern const u32 lbl_80267228[];  /* Level bar segments ascending (0x47D-0x486) */
extern const u32 lbl_80267250[];  /* HP bar segments descending (0x490-0x487) */
extern const u32 lbl_80267278[];  /* EXP bar segments (0x49E-0x495) */
extern const u32 lbl_802672A0[];  /* Status bar segments (10 entries) */
extern const u32 lbl_802672C8[];  /* Move slot message IDs [0x36BC-0x36BF] */
extern const u32 lbl_802672D8[];  /* Move detail positions (6 entries) */
extern const u32 lbl_802672F0[];  /* Contest stat table (12 entries) */
extern const u32 lbl_80267320[];  /* Ribbon display table (6 entries) */
extern const u32 lbl_80267338[];  /* Ribbon detail table (6 entries) */
extern const u32 lbl_80267350[];  /* Marking/icon table (18 entries) */

/*
 * Functions in this translation unit (36 total):
 *
 * fn_8003D1FC  0x2CC  Summary screen top-level state machine
 * fn_8003D4C8  0x350  Summary sub-state dispatcher (calls fn_8003D8CC, fn_8003DC54, fn_8003E394)
 * fn_8003D818  0x0B4  Summary page transition (calls fn_8003E394)
 * fn_8003D8CC  0x388  Summary data loader
 * fn_8003DC54  0x740  Stat page renderer (large)
 * fn_8003E394  0xCAC  Move list / detail page renderer (very large, 3244 bytes)
 * fn_8003F040  0x29C  Utility sub-handler A
 * fn_8003F2DC  0x188  Utility sub-handler B
 * fn_8003F464  0xBB4  Contest/ribbon page renderer (2996 bytes)
 * fn_80040018  0x2F0  Info page renderer
 * fn_80040308  0xE0C  Summary page rendering core (3596 bytes)
 * fn_80041114  0x048  Party select entry A (calls fn_80041E48, fn_800439BC)
 * fn_8004115C  0x048  Party select entry B (calls fn_80042658, fn_800439BC)
 * fn_800411A4  0x048  Party select entry C (calls fn_80042658, fn_800439BC)
 * fn_800411EC  0x010  Party get current selection (lbl_803A6818+0x28)
 * fn_800411FC  0x960  Party list navigation handler (lbl_803A67E8, lbl_803A6818)
 * fn_80041B5C  0x074  Party list sub-handler (calls fn_800411FC)
 * fn_80041BD0  0x278  Party list display
 * fn_80041E48  0x810  Party select state machine (lbl_803A67FC, lbl_803A6818)
 * fn_80042658  0x10D0 Party list main handler (4304 bytes, calls fn_80043728, fn_800478B4, fn_80047CC0)
 * fn_80043728  0x294  Party swap handler
 * fn_800439BC  0x338  Party data refresh
 * fn_80043CD8  0x0E8  Party status icon updater
 * fn_80043DC0  0x108  Party HP bar updater
 * fn_80043EC8  0x0E0  Party EXP bar updater
 * fn_80043FA8  0x3D0  Party item display
 * fn_80044378  0x2B8  Party action handler (calls fn_800492CC, fn_8004BDEC, fn_8004BDFC)
 * fn_80044630  0x1B38 Summary screen main update (6968 bytes, calls fn_80046168, fn_80048918)
 * fn_80046168  0x1164 Summary input handler (4452 bytes)
 * fn_800472CC  0x114  Summary page navigator (calls fn_800473E0)
 * fn_800473E0  0x4D4  Summary page state machine
 * fn_800478B4  0x40C  Party cursor update
 * fn_80047CC0  0x7E4  Party cursor navigation (2020 bytes)
 * fn_800484A4  0x474  Party cursor visual
 * fn_80048918  0x9B4  Summary detail panel (2484 bytes)
 * fn_800492CC  0x304  Party selection finalize
 */

#pragma push
#pragma force_active on

/* 0x8003D1FC | size: 0x2CC */
asm void fn_8003D1FC(void) { nofralloc
    #include "asm/GC6E01/nonmatching/menu_pokemon/fn_8003D1FC.s"
}

/* 0x80044630 | size: 0x1B38 */
asm void fn_80044630(void) { nofralloc
    #include "asm/GC6E01/nonmatching/menu_pokemon/fn_80044630.s"
}

#pragma pop
