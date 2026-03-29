/**
 * @file scene_init.c
 * @brief Scene initialization, transition, and game-start thread management.
 *
 * This module handles the initial game boot sequence after movie playback,
 * including memory card detection, reset-code handling, thread creation for
 * the main game task, fade-in/fade-out timing, and model/floor resource
 * loading during scene transitions.
 *
 * Key behaviors:
 *   - Creates GS threads (via fn_800A19CC) with callback entry points
 *     fn_8003708C and fn_800370E0
 *   - Calls OSGetResetCode to detect warm vs cold boot
 *   - Checks memory card status (VIGetTvFormat area via fn_800AA498)
 *   - Coordinates fade effects through fn_801C41C8 / fn_801C40F0
 *   - Manages floor resource loading through fn_80106D3C / fn_80102568
 *   - Uses timing loops with float accumulators for fade transitions
 *   - Clears memory regions via memset for fresh game state
 *
 * BSS usage:
 *   - lbl_803A3E58 (0xE7 bytes): Scene init state structure
 *   - lbl_803A6498 (0x54 bytes): Sub-scene / card state
 *   - lbl_803A64EC (0x60 bytes): Transition parameter block A
 *   - lbl_803A654C (0x64 bytes): Transition parameter block B
 *   - lbl_803A65B0 (0x60 bytes): Transition parameter block C
 *
 * Address range: 0x8003686C - 0x80039998 (49 functions)
 */

#include "dolphin/types.h"

/* ===== Dolphin OS ===== */
extern u32  OSGetResetCode(void);

/* ===== CRT / libc ===== */
extern void* memset(void* dst, int val, u32 size);

/* ===== GS Engine ===== */
extern void  fn_800A19CC(void* ctx, void* callback, void* arg,
                         void* stack, u32 stackSize, u32 priority, u32 flag);
extern void  fn_800A1F94(void* ctx);    /* GS thread start */
extern void  fn_800F0308(void);          /* GSthread yield / step */
extern void  fn_801C41C8(f32 speed, u32 mode);  /* fade set */
extern void  fn_801C40F0(u32 enable);            /* fade enable */
extern u32   fn_80102568(u32 sceneId, u32 a, u32 b);  /* scene/model load */
extern u32   fn_8010264C(u32 sceneId, u32 a);         /* scene/model query */
extern u32   fn_80102510(u32 sceneId);                  /* scene/model unload */
extern void  fn_801026A4(u32 sceneId, u32 a, u32 b, u32 c,
                         u32 d, u32 e, ...);           /* scene setup */
extern void  fn_80102868(u32 a, u32 b, u32 c);        /* scene positioning */
extern void* fn_80104704(u32 a);         /* scene object query */
extern u8    fn_801045A8(u32 a, u32 b);  /* scene anim check */
extern s8    fn_801043A4(u32 a);         /* scene anim result */
extern void  fn_80106D3C(u32 slot, u32 floorId, u32 a, u32 b);  /* floor load */
extern void* fn_801046B8(void);          /* scene context get */
extern u32   fn_801022B8(u32 a);         /* scene message get */
extern void  fn_800F7F64(u32 pad);       /* input init */
extern u32   fn_800F7BC4(u32 pad);       /* input poll */
extern u32   fn_800AA498(void);          /* VIGetTvFormat / card detect */
extern void  fn_800A0F58(void);          /* card status */
extern void  fn_800A0FC8(u32 a);         /* card close */
extern void  fn_800D37CC(void);          /* GSmem tick */
extern u32   fn_800D3088(void);          /* GSmem query */
extern void  fn_800D37D4(u32 a, u32 b, u32 c, u32 d, u32 e, u16 size);
                                          /* GSmem transfer */
extern u16   fn_800C46B0(f32 a);         /* float-to-u16 convert */
extern void  fn_800E01D0(void* dst, void* src);  /* material copy */

/* ===== BSS data ===== */
extern u8    lbl_803A3E58[];   /* Scene init state (0xE7 bytes) */
extern u8    lbl_803A6498[];   /* Sub-scene / card state (0x54 bytes) */
extern u8    lbl_803A64EC[];   /* Transition params A (0x60 bytes) */
extern u8    lbl_803A654C[];   /* Transition params B (0x64 bytes) */
extern u8    lbl_803A65B0[];   /* Transition params C (0x60 bytes) */

/* ===== SDA data ===== */
extern u32   lbl_804788B8;     /* Memory card slot / state flag */
extern u32   lbl_8047A460;     /* Thread completion flag */
extern u32   lbl_8047A464;     /* Thread active flag */

/* ===== SDATA2 (float constants) ===== */
extern f32   lbl_8047BA30;     /* 0.0f */
extern f64   lbl_8047BA38;     /* 4503599627370496.0 (int-to-float bias) */
extern f64   lbl_8047BA40;     /* int-to-float bias B */
extern f32   lbl_8047BA48;     /* 1.0f */
extern f32   lbl_8047BA4C;     /* fade speed */
extern f32   lbl_8047BA50;     /* scale factor */

/* ===== Rodata ===== */
extern const u8 lbl_80267050[];  /* Shift-JIS: "読み出しエラー\n" (Read Error) */

/*
 * Functions in this translation unit (49 total):
 *
 * fn_8003686C  0x820  Scene main init (OSGetResetCode, thread setup, card detect)
 * fn_8003708C  0x054  Thread callback A (scene init step)
 * fn_800370E0  0x078  Thread callback B (scene init step)
 * fn_80037158  0x004  Stub (blr)
 * fn_8003715C  0x018  Small accessor
 * fn_80037174  0x00C  Small accessor
 * fn_80037180  0x170  BSS lbl_803A6498 state machine
 * fn_800372F0  0x004  Stub
 * fn_800372F4  0x004  Stub
 * fn_800372F8  0x004  Stub
 * fn_800372FC  0x004  Stub
 * fn_80037300  0x0C8  Transition parameter setup
 * fn_800373C8  0x0A0  Transition block A init (lbl_803A64EC, lbl_803A654C)
 * fn_80037468  0x0A0  Transition block A variant
 * fn_80037508  0x0A0  Transition block A variant
 * fn_800375A8  0x0A0  Transition block A variant
 * fn_80037648  0x080  Transition helper
 * fn_800376C8  0x030  Transition helper
 * fn_800376F8  0x0BC  Transition block B (lbl_803A654C)
 * fn_800377B4  0x0BC  Transition block B variant
 * fn_80037870  0x0BC  Transition block B variant
 * fn_8003792C  0x0BC  Transition block B variant
 * fn_800379E8  0x1C8  Transition timing loop
 * fn_80037BB0  0x1C8  Transition timing loop variant
 * fn_80037D78  0x1C8  Transition timing loop variant
 * fn_80037F40  0x1E4  Transition timing loop variant (extended)
 * fn_80038124  0x014  Small accessor
 * fn_80038138  0x038  Helper function
 * fn_80038170  0x0E0  Transition block B accessor (lbl_803A654C)
 * fn_80038250  0x098  Model positioning helper
 * fn_800382E8  0x098  Model positioning helper variant
 * fn_80038380  0x0AC  Model setup
 * fn_8003842C  0x498  Scene transition state machine (calls fn_80038A0C, fn_8003A520)
 * fn_800388C4  0x0CC  Scene completion check
 * fn_80038990  0x070  Scene cleanup
 * fn_80038A00  0x00C  Small accessor
 * fn_80038A0C  0x468  Scene sub-state machine (lbl_803A6498)
 * fn_80038E74  0x190  Transition parameter block C init (lbl_803A65B0)
 * fn_80039004  0x078  Transition block C accessor
 * fn_8003907C  0x0AC  Transition helper
 * fn_80039128  0x370  Transition block C state machine (lbl_803A65B0)
 * fn_80039498  0x0B0  Utility function
 * fn_80039548  0x024  Small accessor
 * fn_8003956C  0x098  Utility function
 * fn_80039604  0x040  Small accessor
 * fn_80039644  0x290  Utility state machine
 * fn_800398D4  0x058  Wrapper (calls fn_80039644)
 * fn_8003992C  0x044  Small function
 * fn_80039970  0x028  Small function
 */

/* ===================================================================
 * DECOMP STUBS
 * These are non-matching placeholder stubs.  Each will be replaced
 * with a matching reimplementation during Phase 2.
 * =================================================================== */


/* 0x8003686C | size: 0x820 */
/* Scene main init - OSGetResetCode handling, thread creation, card detection */
asm void fn_8003686C(void) { nofralloc
    #include "asm/GC6E01/nonmatching/scene_init/fn_8003686C.s"
}

/* 0x8003708C | size: 0x54 */
asm void fn_8003708C(void) { nofralloc
    #include "asm/GC6E01/nonmatching/scene_init/fn_8003708C.s"
}

/* 0x800370E0 | size: 0x78 */
asm void fn_800370E0(void) { nofralloc
    #include "asm/GC6E01/nonmatching/scene_init/fn_800370E0.s"
}


/* ===== Phase 2 recovery stubs ===== */

/* fn_80037158 - 0x80037158 | size: 0x4 */
#if 1
asm void fn_80037158(void) {
#include "src/game/scene_init_fn_80037158.inc"
}
#else
void fn_80037158(void) { /* TODO */ }
#endif

/* fn_8003715C - 0x8003715C | size: 0x18 */
extern u32 lbl_8047A470;
#if 1
asm void fn_8003715C(void) {
#include "src/game/scene_init_fn_8003715C.inc"
}
#else
void fn_8003715C(void) { /* TODO */ }
#endif

/* fn_80037174 - 0x80037174 | size: 0xc */
extern u32 lbl_8047A470;
#if 1
asm void fn_80037174(void) {
#include "src/game/scene_init_fn_80037174.inc"
}
#else
void fn_80037174(void) { /* TODO */ }
#endif

/* fn_80037180 - 0x80037180 | size: 0x170 */
extern void fn_80005748(void);
extern void fn_801EF214(void);
extern void fn_801EF274(void);
extern void fn_80132A38(void);
extern void fn_80109934(void);
extern void fn_800D88DC(void);
extern void fn_800D888C(void);
extern void fn_800D6A00(void);
extern void fn_800D7820(void);
extern void fn_800D85D4(void);
extern void fn_800D67BC(void);
extern void fn_800D61E4(void);
extern void fn_800D5CB8(void);
extern void fn_800D59B8(void);
extern void fn_800D6728(void);
extern u32 lbl_8047A498;
extern u32 lbl_8047A49C;
extern u32 lbl_8047A4A0;
extern u8 lbl_80314F98[];
extern u32 lbl_8047BA58;
extern u32 lbl_8047BA5C;
#if 1
asm void fn_80037180(void) {
#include "src/game/scene_init_fn_80037180.inc"
}
#else
void fn_80037180(void) { /* TODO */ }
#endif

/* fn_800372F0 - 0x800372F0 | size: 0x4 */
#if 1
asm void fn_800372F0(void) {
#include "src/game/scene_init_fn_800372F0.inc"
}
#else
void fn_800372F0(void) { /* TODO */ }
#endif

/* fn_800372F4 - 0x800372F4 | size: 0x4 */
#if 1
asm void fn_800372F4(void) {
#include "src/game/scene_init_fn_800372F4.inc"
}
#else
void fn_800372F4(void) { /* TODO */ }
#endif

/* fn_800372F8 - 0x800372F8 | size: 0x4 */
#if 1
asm void fn_800372F8(void) {
#include "src/game/scene_init_fn_800372F8.inc"
}
#else
void fn_800372F8(void) { /* TODO */ }
#endif

/* fn_800372FC - 0x800372FC | size: 0x4 */
#if 1
asm void fn_800372FC(void) {
#include "src/game/scene_init_fn_800372FC.inc"
}
#else
void fn_800372FC(void) { /* TODO */ }
#endif

/* fn_80037300 - 0x80037300 | size: 0xc8 */
extern void fn_800D5BA0(void);
extern u8 lbl_80314E08[];
#if 1
asm void fn_80037300(void) {
#include "src/game/scene_init_fn_80037300.inc"
}
#else
void fn_80037300(void) { /* TODO */ }
#endif

/* fn_800373C8 - 0x800373C8 | size: 0xa0 */
#if 1
asm void fn_800373C8(void) {
#include "src/game/scene_init_fn_800373C8.inc"
}
#else
void fn_800373C8(void) { /* TODO */ }
#endif

/* fn_80037468 - 0x80037468 | size: 0xa0 */
#if 1
asm void fn_80037468(void) {
#include "src/game/scene_init_fn_80037468.inc"
}
#else
void fn_80037468(void) { /* TODO */ }
#endif

/* fn_80037508 - 0x80037508 | size: 0xa0 */
#if 1
asm void fn_80037508(void) {
#include "src/game/scene_init_fn_80037508.inc"
}
#else
void fn_80037508(void) { /* TODO */ }
#endif

/* fn_800375A8 - 0x800375A8 | size: 0xa0 */
#if 1
asm void fn_800375A8(void) {
#include "src/game/scene_init_fn_800375A8.inc"
}
#else
void fn_800375A8(void) { /* TODO */ }
#endif

/* fn_80037648 - 0x80037648 | size: 0x80 */
extern void fn_800FB680(void);
extern u32 lbl_8047A490;
#if 1
asm void fn_80037648(void) {
#include "src/game/scene_init_fn_80037648.inc"
}
#else
void fn_80037648(void) { /* TODO */ }
#endif

/* fn_800376C8 - 0x800376C8 | size: 0x30 */
extern u32 lbl_8047A480;
#if 1
asm void fn_800376C8(void) {
#include "src/game/scene_init_fn_800376C8.inc"
}
#else
void fn_800376C8(void) { /* TODO */ }
#endif

/* fn_800376F8 - 0x800376F8 | size: 0xbc */
extern u32 lbl_8047BA58;
extern u32 lbl_8047A494;
#if 1
asm void fn_800376F8(void) {
#include "src/game/scene_init_fn_800376F8.inc"
}
#else
void fn_800376F8(void) { /* TODO */ }
#endif

/* fn_800377B4 - 0x800377B4 | size: 0xbc */
extern u32 lbl_8047BA58;
extern u32 lbl_8047A494;
#if 1
asm void fn_800377B4(void) {
#include "src/game/scene_init_fn_800377B4.inc"
}
#else
void fn_800377B4(void) { /* TODO */ }
#endif

/* fn_80037870 - 0x80037870 | size: 0xbc */
extern u32 lbl_8047BA58;
extern u32 lbl_8047A494;
#if 1
asm void fn_80037870(void) {
#include "src/game/scene_init_fn_80037870.inc"
}
#else
void fn_80037870(void) { /* TODO */ }
#endif

/* fn_8003792C - 0x8003792C | size: 0xbc */
extern u32 lbl_8047BA58;
extern u32 lbl_8047A494;
#if 1
asm void fn_8003792C(void) {
#include "src/game/scene_init_fn_8003792C.inc"
}
#else
void fn_8003792C(void) { /* TODO */ }
#endif

/* fn_800379E8 - 0x800379E8 | size: 0x1c8 */
extern void fn_80105624(void);
extern void fn_800CE148(void);
extern void fn_800CDBE0(void);
extern u8 lbl_802E52B8[];
extern u32 lbl_8047A484;
extern u32 lbl_8047BA58;
extern u32 lbl_8047BA60;
extern u32 lbl_8047BA64;
extern u32 lbl_8047A494;
extern u32 lbl_8047A488;
extern u32 lbl_8047A48C;
extern u32 lbl_8047A47D;
extern u32 lbl_8047A47C;
extern u32 lbl_8047BA6C;
extern u32 lbl_8047BA68;
extern u32 lbl_8047BA70;
#if 1
asm void fn_800379E8(void) {
#include "src/game/scene_init_fn_800379E8.inc"
}
#else
void fn_800379E8(void) { /* TODO */ }
#endif

/* fn_80037BB0 - 0x80037BB0 | size: 0x1c8 */
extern u32 lbl_8047A484;
extern u32 lbl_8047BA58;
extern u32 lbl_8047BA60;
extern u32 lbl_8047BA64;
extern u32 lbl_8047A494;
extern u32 lbl_8047A488;
extern u32 lbl_8047A48C;
extern u32 lbl_8047A47D;
extern u32 lbl_8047A47C;
extern u32 lbl_8047BA6C;
extern u32 lbl_8047BA68;
extern u32 lbl_8047BA70;
#if 1
asm void fn_80037BB0(void) {
#include "src/game/scene_init_fn_80037BB0.inc"
}
#else
void fn_80037BB0(void) { /* TODO */ }
#endif

/* fn_80037D78 - 0x80037D78 | size: 0x1c8 */
extern u32 lbl_8047A484;
extern u32 lbl_8047BA58;
extern u32 lbl_8047BA60;
extern u32 lbl_8047BA64;
extern u32 lbl_8047A494;
extern u32 lbl_8047A488;
extern u32 lbl_8047A48C;
extern u32 lbl_8047A47D;
extern u32 lbl_8047A47C;
extern u32 lbl_8047BA6C;
extern u32 lbl_8047BA68;
extern u32 lbl_8047BA70;
#if 1
asm void fn_80037D78(void) {
#include "src/game/scene_init_fn_80037D78.inc"
}
#else
void fn_80037D78(void) { /* TODO */ }
#endif

/* fn_80037F40 - 0x80037F40 | size: 0x1e4 */
extern u32 lbl_8047A47C;
extern u32 lbl_8047A484;
extern u8 lbl_802E52A8[];
extern u32 lbl_8047BA58;
extern u32 lbl_8047A488;
extern u32 lbl_8047BA60;
extern u32 lbl_8047BA64;
extern u32 lbl_8047A494;
extern u32 lbl_8047A48C;
extern u32 lbl_8047A47D;
extern u32 lbl_8047BA6C;
extern u32 lbl_8047BA68;
extern u32 lbl_8047BA70;
#if 1
asm void fn_80037F40(void) {
#include "src/game/scene_init_fn_80037F40.inc"
}
#else
void fn_80037F40(void) { /* TODO */ }
#endif

/* fn_80038124 - 0x80038124 | size: 0x14 */
extern u32 lbl_8047BA74;
extern u32 lbl_8047A478;
#if 1
asm void fn_80038124(void) {
#include "src/game/scene_init_fn_80038124.inc"
}
#else
void fn_80038124(void) { /* TODO */ }
#endif

/* fn_80038138 - 0x80038138 | size: 0x38 */
extern u32 lbl_8047BA78;
extern u32 lbl_8047A494;
extern u32 lbl_8047A478;
extern u32 lbl_8047BA60;
#if 1
asm void fn_80038138(void) {
#include "src/game/scene_init_fn_80038138.inc"
}
#else
void fn_80038138(void) { /* TODO */ }
#endif

/* fn_80038170 - 0x80038170 | size: 0xe0 */
extern u32 lbl_8047BA58;
extern u8 lbl_802E5288[];
extern u32 lbl_8047A47C;
#if 1
asm void fn_80038170(void) {
#include "src/game/scene_init_fn_80038170.inc"
}
#else
void fn_80038170(void) { /* TODO */ }
#endif

/* fn_80038250 - 0x80038250 | size: 0x98 */
extern void fn_801080CC(void);
#if 1
asm void fn_80038250(void) {
#include "src/game/scene_init_fn_80038250.inc"
}
#else
void fn_80038250(void) { /* TODO */ }
#endif

/* fn_800382E8 - 0x800382E8 | size: 0x98 */
#if 1
asm void fn_800382E8(void) {
#include "src/game/scene_init_fn_800382E8.inc"
}
#else
void fn_800382E8(void) { /* TODO */ }
#endif

/* fn_80038380 - 0x80038380 | size: 0xac */
extern void fn_801020C0(void);
#if 1
asm void fn_80038380(void) {
#include "src/game/scene_init_fn_80038380.inc"
}
#else
void fn_80038380(void) { /* TODO */ }
#endif

/* fn_8003842C - 0x8003842C | size: 0x498 */
extern void fn_801661D0(void);
extern void fn_8010A5BC(void);
extern void fn_8010A010(void);
extern void fn_8018F6F4(void);
extern void fn_8018F4C8(void);
extern void fn_80109894(void);
extern void fn_8010A420(void);
extern void fn_801660D8(void);
extern void fn_800FF660(void);
extern void* memcpy(void* dst, const void* src, u32 n);
extern u8 lbl_802E51C8[];
extern u8 lbl_802EF0A8[];
extern u32 lbl_8047A480;
extern u32 lbl_8047A47C;
extern u32 lbl_8047A488;
extern u32 lbl_8047A484;
extern u32 lbl_8047A47D;
extern u32 lbl_8047A490;
#if 1
asm void fn_8003842C(void) {
#include "src/game/scene_init_fn_8003842C.inc"
}
#else
void fn_8003842C(void) { /* TODO */ }
#endif

/* fn_800388C4 - 0x800388C4 | size: 0xcc */
#if 1
asm void fn_800388C4(void) {
#include "src/game/scene_init_fn_800388C4.inc"
}
#else
void fn_800388C4(void) { /* TODO */ }
#endif

/* fn_80038990 - 0x80038990 | size: 0x70 */
extern u32 lbl_8047BA80;
extern u32 lbl_8047BA88;
extern u32 lbl_8047A494;
#if 1
asm void fn_80038990(void) {
#include "src/game/scene_init_fn_80038990.inc"
}
#else
void fn_80038990(void) { /* TODO */ }
#endif

/* fn_80038A00 - 0x80038A00 | size: 0xc */
extern u32 lbl_8047BA58;
extern u32 lbl_8047A484;
#if 1
asm void fn_80038A00(void) {
#include "src/game/scene_init_fn_80038A00.inc"
}
#else
void fn_80038A00(void) { /* TODO */ }
#endif

/* fn_80038A0C - 0x80038A0C | size: 0x468 */
extern void fn_8001E074(void);
extern void fn_801D036C(void);
extern void fn_80129280(void);
extern void fn_801D0748(void);
extern void fn_80135168(void);
extern void fn_8012A5B0(void);
extern void fn_800056EC(void);
extern void fn_801D0314(void);
extern void fn_8012F11C(void);
extern void fn_8012A450(void);
extern void fn_80135CD0(void);
extern void fn_80135C28(void);
extern void fn_80135BF8(void);
extern void fn_80135BE0(void);
extern void fn_8011418C(void);
extern void fn_80135B6C(void);
extern void fn_80135B4C(void);
extern void fn_80135B3C(void);
extern u32 lbl_8047A480;
extern u32 lbl_8047A498;
extern u32 lbl_8047A49C;
extern u32 lbl_8047A4A0;
extern u32 lbl_8047BA88;
extern u32 lbl_8047A47C;
extern u32 lbl_8047A488;
extern u32 lbl_8047A484;
extern u32 lbl_8047A47D;
#if 1
asm void fn_80038A0C(void) {
#include "src/game/scene_init_fn_80038A0C.inc"
}
#else
void fn_80038A0C(void) { /* TODO */ }
#endif

/* fn_80038E74 - 0x80038E74 | size: 0x190 */
extern u8 lbl_80267060[];
#if 1
asm void fn_80038E74(void) {
#include "src/game/scene_init_fn_80038E74.inc"
}
#else
void fn_80038E74(void) { /* TODO */ }
#endif

/* fn_80039004 - 0x80039004 | size: 0x78 */
#if 1
asm void fn_80039004(void) {
#include "src/game/scene_init_fn_80039004.inc"
}
#else
void fn_80039004(void) { /* TODO */ }
#endif

/* fn_8003907C - 0x8003907C | size: 0xac */
#if 1
asm void fn_8003907C(void) {
#include "src/game/scene_init_fn_8003907C.inc"
}
#else
void fn_8003907C(void) { /* TODO */ }
#endif

/* fn_80039128 - 0x80039128 | size: 0x370 */
extern u32 lbl_8047BAA8;
extern u32 lbl_8047BAA0;
extern u32 lbl_8047BA90;
extern u32 lbl_8047BA94;
extern u32 lbl_8047BA98;
#if 1
asm void fn_80039128(void) {
#include "src/game/scene_init_fn_80039128.inc"
}
#else
void fn_80039128(void) { /* TODO */ }
#endif

/* fn_80039498 - 0x80039498 | size: 0xb0 */
extern void fn_80102428(void);
extern u8 lbl_80267120[];
#if 1
asm void fn_80039498(void) {
#include "src/game/scene_init_fn_80039498.inc"
}
#else
void fn_80039498(void) { /* TODO */ }
#endif

/* fn_80039548 - 0x80039548 | size: 0x24 */
extern u32 lbl_8047A4B0;
#if 1
asm void fn_80039548(void) {
#include "src/game/scene_init_fn_80039548.inc"
}
#else
void fn_80039548(void) { /* TODO */ }
#endif

/* fn_8003956C - 0x8003956C | size: 0x98 */
extern void fn_801347D0(void);
extern void fn_80134768(void);
extern void fn_801429E8(void);
extern void fn_80109220(void);
extern u32 lbl_8047A4A8;
#if 1
asm void fn_8003956C(void) {
#include "src/game/scene_init_fn_8003956C.inc"
}
#else
void fn_8003956C(void) { /* TODO */ }
#endif

/* fn_80039604 - 0x80039604 | size: 0x40 */
extern u32 lbl_8047A4A8;
#if 1
asm void fn_80039604(void) {
#include "src/game/scene_init_fn_80039604.inc"
}
#else
void fn_80039604(void) { /* TODO */ }
#endif

/* fn_80039644 - 0x80039644 | size: 0x290 */
extern void fn_800FA444(void);
extern void fn_80143C68(void);
extern void fn_801440A0(void);
extern void fn_80144088(void);
extern void fn_80144014(void);
extern void fn_80143C50(void);
extern u32 lbl_8047A4A8;
extern u32 lbl_8047BAB0;
extern u32 lbl_8047A4C0;
extern u32 lbl_8047A4BC;
#if 1
asm void fn_80039644(void) {
#include "src/game/scene_init_fn_80039644.inc"
}
#else
void fn_80039644(void) { /* TODO */ }
#endif

/* fn_800398D4 - 0x800398D4 | size: 0x58 */
extern void fn_800FE38C(void);
extern void fn_800FE35C(void);
#if 1
asm void fn_800398D4(void) {
#include "src/game/scene_init_fn_800398D4.inc"
}
#else
void fn_800398D4(void) { /* TODO */ }
#endif

/* fn_8003992C - 0x8003992C | size: 0x44 */
extern u32 lbl_8047A4AC;
extern u32 lbl_8047A4BC;
extern u32 lbl_8047A4C0;
#if 1
asm void fn_8003992C(void) {
#include "src/game/scene_init_fn_8003992C.inc"
}
#else
void fn_8003992C(void) { /* TODO */ }
#endif

/* fn_80039970 - 0x80039970 | size: 0xe0 */
extern u32 lbl_8047A4B8;
extern u32 lbl_8047A4A8;
extern u32 lbl_8047A4BC;
extern u32 lbl_8047A4C0;
extern u32 lbl_8047A4AC;
#if 1
asm void fn_80039970(void) {
#include "src/game/scene_init_fn_80039970.inc"
}
#else
void fn_80039970(void) { /* TODO */ }
#endif

/* fn_80039F44 - 0x80039F44 | size: 0x2c */
extern void fn_80102ED4(void);
extern u32 lbl_8047A4B8;
#if 1
asm void fn_80039F44(void) {
#include "src/game/scene_init_fn_80039F44.inc"
}
#else
void fn_80039F44(void) { /* TODO */ }
#endif

/* fn_80039F70 - 0x80039F70 | size: 0x19c */
extern void fn_80143F84(void);
extern u32 lbl_8047A4A8;
extern u32 lbl_8047A4AC;
extern u32 lbl_8047BAB0;
extern u32 lbl_8047A4B4;
extern u32 lbl_8047A4C0;
extern u32 lbl_8047A4BC;
extern u32 lbl_8047A4B8;
extern u32 lbl_8047BABC;
#if 1
asm void fn_80039F70(void) {
#include "src/game/scene_init_fn_80039F70.inc"
}
#else
void fn_80039F70(void) { /* TODO */ }
#endif

/* fn_8003A10C - 0x8003A10C | size: 0x414 */
extern void fn_801298B8(void);
extern void fn_801069FC(void);
extern void fn_80129A78(void);
extern void fn_80134584(void);
extern u32 lbl_8047A4B0;
extern u32 lbl_8047A4A8;
extern u32 lbl_8047A4AC;
extern u32 lbl_8047A4B4;
#if 1
asm void fn_8003A10C(void) {
#include "src/game/scene_init_fn_8003A10C.inc"
}
#else
void fn_8003A10C(void) { /* TODO */ }
#endif

/* fn_8003A520 - 0x8003A520 | size: 0x1a0 */
extern void fn_8017B3E4(void);
extern void fn_8017B2CC(void);
extern void fn_80018F54(void);
extern void fn_8017B1CC(void);
extern void fn_800F915C(void);
#if 1
asm void fn_8003A520(void) {
#include "src/game/scene_init_fn_8003A520.inc"
}
#else
void fn_8003A520(void) { /* TODO */ }
#endif

/* fn_8003A6C0 - 0x8003A6C0 | size: 0x130 */
extern void fn_800FB8C8(void);
extern u8 lbl_80267130[];
extern u32 lbl_8047A4C8;
#if 1
asm void fn_8003A6C0(void) {
#include "src/game/scene_init_fn_8003A6C0.inc"
}
#else
void fn_8003A6C0(void) { /* TODO */ }
#endif

/* fn_8003A7F0 - 0x8003A7F0 | size: 0x460 */
extern void fn_80166A50(void);
extern u32 lbl_8047A4C8;
#if 1
asm void fn_8003A7F0(void) {
#include "src/game/scene_init_fn_8003A7F0.inc"
}
#else
void fn_8003A7F0(void) { /* TODO */ }
#endif

/* fn_8003AC50 - 0x8003AC50 | size: 0x98 */
#if 1
asm void fn_8003AC50(void) {
#include "src/game/scene_init_fn_8003AC50.inc"
}
#else
void fn_8003AC50(void) { /* TODO */ }
#endif

/* fn_8003ACE8 - 0x8003ACE8 | size: 0x84 */
extern u32 lbl_8047A4C8;
#if 1
asm void fn_8003ACE8(void) {
#include "src/game/scene_init_fn_8003ACE8.inc"
}
#else
void fn_8003ACE8(void) { /* TODO */ }
#endif

/* fn_8003AE84 - 0x8003AE84 | size: 0x6c */
#if 1
asm void fn_8003AE84(void) {
#include "src/game/scene_init_fn_8003AE84.inc"
}
#else
void fn_8003AE84(void) { /* TODO */ }
#endif

/* fn_8003AFDC - 0x8003AFDC | size: 0x2fc */
extern void fn_801EE8F4(void);
extern void fn_80109B90(void);
extern void fn_801EED88(void);
extern void fn_801EE614(void);
extern void fn_801EEAD0(void);
extern void fn_801EEC74(void);
extern void fn_801040F0(void);
extern u8 lbl_803A6748[];
extern u32 lbl_8047A4D4;
extern u32 lbl_8047BAC4;
extern u32 lbl_8047BAC8;
#if 1
asm void fn_8003AFDC(void) {
#include "src/game/scene_init_fn_8003AFDC.inc"
}
#else
void fn_8003AFDC(void) { /* TODO */ }
#endif

/* fn_8003B2D8 - 0x8003B2D8 | size: 0x1a0 */
extern void fn_801EE544(void);
extern void fn_801EEFAC(void);
extern void fn_801EE328(void);
extern void fn_801FCCC4(void);
extern void fn_801FCC7C(void);
extern void fn_800FA280(void);
extern u32 lbl_8047BAC0;
extern u32 lbl_8047A4D0;
extern u32 lbl_8047A4D4;
#if 1
asm void fn_8003B2D8(void) {
#include "src/game/scene_init_fn_8003B2D8.inc"
}
#else
void fn_8003B2D8(void) { /* TODO */ }
#endif

/* fn_8003B478 - 0x8003B478 | size: 0x258 */
extern void fn_80135938(void);
extern void fn_801240C4(void);
extern void fn_801EE750(void);
extern void fn_8011DFE0(void);
extern void fn_8011D8F4(void);
extern void fn_8011D8D8(void);
extern void fn_801231A4(void);
extern u32 lbl_8047BAC0;
extern u32 lbl_8047A4D4;
extern u32 lbl_8047A4D0;
#if 1
asm void fn_8003B478(void) {
#include "src/game/scene_init_fn_8003B478.inc"
}
#else
void fn_8003B478(void) { /* TODO */ }
#endif

/* fn_8003B6D0 - 0x8003B6D0 | size: 0x144 */
extern void fn_801EE248(void);
extern void fn_8011E778(void);
extern void fn_8011E760(void);
extern void fn_801FCC3C(void);
extern void fn_801FCA2C(void);
extern void fn_801FCA14(void);
extern void fn_801FC964(void);
extern u32 lbl_8047BAC0;
extern u32 lbl_8047A4D4;
#if 1
asm void fn_8003B6D0(void) {
#include "src/game/scene_init_fn_8003B6D0.inc"
}
#else
void fn_8003B6D0(void) { /* TODO */ }
#endif

/* fn_8003B814 - 0x8003B814 | size: 0x24 */
extern void fn_8003B85C(void);
#if 1
asm void fn_8003B814(void) {
#include "src/game/scene_init_fn_8003B814.inc"
}
#else
void fn_8003B814(void) { /* TODO */ }
#endif

/* fn_8003B838 - 0x8003B838 | size: 0x24 */
#if 1
asm void fn_8003B838(void) {
#include "src/game/scene_init_fn_8003B838.inc"
}
#else
void fn_8003B838(void) { /* TODO */ }
#endif

/* fn_8003BF54 - 0x8003BF54 | size: 0xe8 */
extern u32 lbl_8047BAC0;
extern u32 lbl_8047BAE0;
extern u32 lbl_8047BAD8;
#if 1
asm void fn_8003BF54(void) {
#include "src/game/scene_init_fn_8003BF54.inc"
}
#else
void fn_8003BF54(void) { /* TODO */ }
#endif

/* fn_8003C03C - 0x8003C03C | size: 0x100 */
extern u32 lbl_8047BAC0;
extern u32 lbl_8047BAF4;
extern u32 lbl_8047BAF0;
extern u32 lbl_8047BAE0;
#if 1
asm void fn_8003C03C(void) {
#include "src/game/scene_init_fn_8003C03C.inc"
}
#else
void fn_8003C03C(void) { /* TODO */ }
#endif

/* fn_8003C13C - 0x8003C13C | size: 0xe0 */
extern u32 lbl_8047BAC0;
extern u32 lbl_8047BAF4;
extern u32 lbl_8047BAF0;
extern u32 lbl_8047BAE0;
#if 1
asm void fn_8003C13C(void) {
#include "src/game/scene_init_fn_8003C13C.inc"
}
#else
void fn_8003C13C(void) { /* TODO */ }
#endif

/* fn_8003C21C - 0x8003C21C | size: 0x30 */
extern u32 lbl_8047BAC0;
#if 1
asm void fn_8003C21C(void) {
#include "src/game/scene_init_fn_8003C21C.inc"
}
#else
void fn_8003C21C(void) { /* TODO */ }
#endif

/* fn_8003C24C - 0x8003C24C | size: 0x6c */
extern u32 lbl_8047BAC0;
#if 1
asm void fn_8003C24C(void) {
#include "src/game/scene_init_fn_8003C24C.inc"
}
#else
void fn_8003C24C(void) { /* TODO */ }
#endif

/* fn_8003C2B8 - 0x8003C2B8 | size: 0x470 */
extern u32 lbl_8047BAC0;
extern u32 lbl_8047BAF8;
extern u32 lbl_8047BAFC;
extern u32 lbl_8047BAC8;
extern u32 lbl_8047BAD8;
extern u32 lbl_8047A4D4;
extern u32 lbl_8047A4D0;
extern u32 lbl_8047BB00;
extern u32 lbl_8047A4DC;
#if 1
asm void fn_8003C2B8(void) {
#include "src/game/scene_init_fn_8003C2B8.inc"
}
#else
void fn_8003C2B8(void) { /* TODO */ }
#endif

/* fn_8003C728 - 0x8003C728 | size: 0x98 */
extern u32 lbl_8047BAC0;
extern u32 lbl_8047BAE0;
#if 1
asm void fn_8003C728(void) {
#include "src/game/scene_init_fn_8003C728.inc"
}
#else
void fn_8003C728(void) { /* TODO */ }
#endif

/* fn_8003C7C0 - 0x8003C7C0 | size: 0x65c */
extern void fn_80124410(void);
extern void fn_80166AB8(void);
extern void fn_80109C88(void);
extern void fn_801EEDEC(void);
extern void fn_800E202C(void);
extern void fn_800E24B0(void);
extern void fn_800E209C(void);
extern u32 lbl_8047BAC4;
extern u32 lbl_8047BAC8;
extern u8 lbl_803A6610[];
extern u32 lbl_8047A4DC;
extern u32 lbl_8047BAE0;
extern u32 lbl_8047A4D4;
extern u32 lbl_8047A4D0;
extern u32 lbl_8047BAD4;
#if 1
asm void fn_8003C7C0(void) {
#include "src/game/scene_init_fn_8003C7C0.inc"
}
#else
void fn_8003C7C0(void) { /* TODO */ }
#endif

/* fn_8003CE1C - 0x8003CE1C | size: 0x11c */
extern void fn_801EE174(void);
extern void fn_801FBD58(void);
extern void fn_801EEF40(void);
extern void fn_801FCC64(void);
extern void fn_801FBD28(void);
extern u32 lbl_8047A4D0;
extern u32 lbl_8047A4D4;
#if 1
asm void fn_8003CE1C(void) {
#include "src/game/scene_init_fn_8003CE1C.inc"
}
#else
void fn_8003CE1C(void) { /* TODO */ }
#endif

/* fn_8003CF38 - 0x8003CF38 | size: 0x2c4 */
extern void fn_800E2C04(void);
extern void fn_800E27B0(void);
extern void fn_801EE0BC(void);
extern u32 lbl_8047A4DC;
extern u32 lbl_8047A4D0;
extern u32 lbl_8047A4D4;
extern u32 lbl_8047A4D8;
#if 1
asm void fn_8003CF38(void) {
#include "src/game/scene_init_fn_8003CF38.inc"
}
#else
void fn_8003CF38(void) { /* TODO */ }
#endif

/* fn_8003D4C8 - 0x8003D4C8 | size: 0x350 */
extern void fn_8025FE84(void);
extern void fn_8025FEE4(void);
extern void fn_8025FDDC(void);
extern void fn_8025FD34(void);
extern void fn_8011DF90(void);
extern void fn_8011F5C8(void);
extern void fn_8011E1D4(void);
extern u32 lbl_8047A4E4;
extern u32 lbl_8047A4E8;
extern u8 lbl_803A6818[];
extern u32 lbl_8047A4E0;
#if 1
asm void fn_8003D4C8(void) {
#include "src/game/scene_init_fn_8003D4C8.inc"
}
#else
void fn_8003D4C8(void) { /* TODO */ }
#endif

/* fn_8003D818 - 0x8003D818 | size: 0xb4 */
extern u32 lbl_8047A4E4;
extern u32 lbl_8047A4E8;
#if 1
asm void fn_8003D818(void) {
#include "src/game/scene_init_fn_8003D818.inc"
}
#else
void fn_8003D818(void) { /* TODO */ }
#endif

/* fn_8003D8CC - 0x8003D8CC | size: 0x388 */
extern void fn_800F9D04(void);
extern void fn_800F96E4(void);
extern u8 lbl_802E60B0[];
extern u32 lbl_8047A4E0;
extern u32 lbl_8047A4E4;
extern u32 lbl_8047A4E8;
#if 1
asm void fn_8003D8CC(void) {
#include "src/game/scene_init_fn_8003D8CC.inc"
}
#else
void fn_8003D8CC(void) { /* TODO */ }
#endif

/* fn_8003DC54 - 0x8003DC54 | size: 0x740 */
extern void fn_8012640C(void);
extern u32 lbl_8047A4E4;
extern u32 lbl_8047A4E0;
extern u32 lbl_8047A4E8;
#if 1
asm void fn_8003DC54(void) {
#include "src/game/scene_init_fn_8003DC54.inc"
}
#else
void fn_8003DC54(void) { /* TODO */ }
#endif

/* fn_8003E394 - 0x8003E394 | size: 0xcac */
extern void fn_8011E18C(void);
extern void fn_8011E1A4(void);
extern u32 lbl_8047A4E4;
extern u32 lbl_8047A4E0;
extern u32 lbl_8047A4E8;
#if 1
asm void fn_8003E394(void) {
#include "src/game/scene_init_fn_8003E394.inc"
}
#else
void fn_8003E394(void) { /* TODO */ }
#endif

/* fn_8003F040 - 0x8003F040 | size: 0x29c */
extern void fn_800F9EE4(void);
extern u32 lbl_8047A4E4;
extern u32 lbl_8047A4E0;
extern u32 lbl_8047A4E8;
#if 1
asm void fn_8003F040(void) {
#include "src/game/scene_init_fn_8003F040.inc"
}
#else
void fn_8003F040(void) { /* TODO */ }
#endif

/* fn_8003F2DC - 0x8003F2DC | size: 0x188 */
#if 1
asm void fn_8003F2DC(void) {
#include "src/game/scene_init_fn_8003F2DC.inc"
}
#else
void fn_8003F2DC(void) { /* TODO */ }
#endif

/* fn_8003F464 - 0x8003F464 | size: 0xbb4 */
extern void fn_800FE6D0(void);
extern void fn_800FE4D4(void);
extern void jumptable_802E60C4();
extern u8 lbl_802E52C8[];
extern u32 lbl_8047BCA0;
extern u32 lbl_8047BCA8;
extern u32 lbl_8047BCA4;
extern u32 lbl_8047BCB0;
extern u32 lbl_8047BCAC;
#if 1
asm void fn_8003F464(void) {
#include "src/game/scene_init_fn_8003F464.inc"
}
#else
void fn_8003F464(void) { /* TODO */ }
#endif

/* fn_80040018 - 0x80040018 | size: 0x2f0 */
extern void fn_8005DA18(void);
extern u32 lbl_8047BCB8;
extern u32 lbl_8047BCBC;
extern u32 lbl_8047BC94;
extern u32 lbl_8047BCC0;
extern u32 lbl_8047BCC4;
extern u32 lbl_8047BC98;
extern u32 lbl_8047BCC8;
extern u32 lbl_804788C0;
#if 1
asm void fn_80040018(void) {
#include "src/game/scene_init_fn_80040018.inc"
}
#else
void fn_80040018(void) { /* TODO */ }
#endif

/* fn_80040308 - 0x80040308 | size: 0xe0c */
extern void fn_8010A210(void);
extern void fn_80109BFC(void);
extern void fn_800E3C5C(void);
extern void fn_80190E34(void);
extern void fn_80177A44(void);
extern void fn_800D1FDC(void);
extern void fn_800E01F4(void);
extern void fn_800D207C(void);
extern void fn_800D20CC(void);
extern void fn_800D1F04(void);
extern void fn_800DCCF0(void);
extern void fn_800DCC84(void);
extern void fn_800DCC60(void);
extern void fn_800DCC3C(void);
extern void fn_800DCC34(void);
extern void fn_800D203C(void);
extern void fn_80103484(void);
extern u32 lbl_8047BCB8;
extern u32 lbl_8047BCBC;
extern u32 lbl_8047BC94;
extern u32 lbl_8047BCC0;
extern u32 lbl_8047BCC4;
extern u32 lbl_8047BC98;
extern u32 lbl_8047BCC8;
extern u32 lbl_8047A4E8;
extern u32 lbl_804788C0;
extern u32 lbl_8047A4E0;
extern u32 lbl_8047A4E4;
extern u8 lbl_802E5448[];
extern u8 lbl_802E543C[];
extern u8 lbl_80267180[];
extern u32 lbl_8047BC9C;
extern u32 lbl_8047BCCC;
extern u8 lbl_804788D4[];
extern u32 lbl_8047BCB0;
#if 1
asm void fn_80040308(void) {
#include "src/game/scene_init_fn_80040308.inc"
}
#else
void fn_80040308(void) { /* TODO */ }
#endif

/* fn_80041114 - 0x80041114 | size: 0x48 */
extern u8 lbl_803A67FC[];
#if 1
asm void fn_80041114(void) {
#include "src/game/scene_init_fn_80041114.inc"
}
#else
void fn_80041114(void) { /* TODO */ }
#endif

/* fn_8004115C - 0x8004115C | size: 0x48 */
#if 1
asm void fn_8004115C(void) {
#include "src/game/scene_init_fn_8004115C.inc"
}
#else
void fn_8004115C(void) { /* TODO */ }
#endif

/* fn_800411A4 - 0x800411A4 | size: 0x48 */
#if 1
asm void fn_800411A4(void) {
#include "src/game/scene_init_fn_800411A4.inc"
}
#else
void fn_800411A4(void) { /* TODO */ }
#endif

/* fn_800411EC - 0x800411EC | size: 0x10 */
#if 1
asm void fn_800411EC(void) {
#include "src/game/scene_init_fn_800411EC.inc"
}
#else
void fn_800411EC(void) { /* TODO */ }
#endif

/* fn_800411FC - 0x800411FC | size: 0x960 */
extern void fn_8011E444(void);
extern void fn_8011CB6C(void);
extern void fn_8011CB54(void);
extern void fn_800FBB34(void);
extern void fn_8011E1BC(void);
extern u32 lbl_8047A4E0;
extern u32 lbl_8047A4E4;
extern u8 lbl_803A67E8[];
extern u32 lbl_8047A4F0;
#if 1
asm void fn_800411FC(void) {
#include "src/game/scene_init_fn_800411FC.inc"
}
#else
void fn_800411FC(void) { /* TODO */ }
#endif

/* fn_80041B5C - 0x80041B5C | size: 0x74 */
extern u32 lbl_8047BCA0;
#if 1
asm void fn_80041B5C(void) {
#include "src/game/scene_init_fn_80041B5C.inc"
}
#else
void fn_80041B5C(void) { /* TODO */ }
#endif

/* fn_80041BD0 - 0x80041BD0 | size: 0x278 */
extern u32 lbl_8047A4E0;
extern u32 lbl_8047A4E4;
extern u32 lbl_8047BCA0;
extern u8 lbl_802E554C[];
#if 1
asm void fn_80041BD0(void) {
#include "src/game/scene_init_fn_80041BD0.inc"
}
#else
void fn_80041BD0(void) { /* TODO */ }
#endif

/* fn_80041E48 - 0x80041E48 | size: 0x810 */
extern u32 lbl_8047BCB0;
extern u32 lbl_8047BCD8;
extern u32 lbl_8047BCBC;
extern u32 lbl_8047BC94;
extern u32 lbl_8047BCC4;
extern u32 lbl_8047BCD0;
extern u32 lbl_804788C4;
extern u32 lbl_8047BCD4;
extern u32 lbl_8047BCB8;
extern u32 lbl_8047BCC0;
extern u32 lbl_8047BC98;
extern u32 lbl_8047BCC8;
extern u32 lbl_8047BCAC;
#if 1
asm void fn_80041E48(void) {
#include "src/game/scene_init_fn_80041E48.inc"
}
#else
void fn_80041E48(void) { /* TODO */ }
#endif

/* fn_80042658 - 0x80042658 | size: 0x10d0 */
extern void fn_801DAC3C(void);
extern void fn_800F7994(void);
extern void fn_800F7920(void);
extern void fn_800E019C(void);
extern void fn_800E3D98(void);
extern void fn_800E43A4(void);
extern void fn_800E3BC0(void);
extern u32 lbl_804788C4;
extern u32 lbl_8047BCB0;
extern u32 lbl_8047BCD8;
extern u32 lbl_8047BCBC;
extern u32 lbl_8047BC94;
extern u32 lbl_8047BCC4;
extern u32 lbl_8047BCD0;
extern u32 lbl_8047BCD4;
extern u32 lbl_8047BCB8;
extern u32 lbl_8047BCC0;
extern u32 lbl_8047BC98;
extern u32 lbl_8047BCC8;
extern u32 lbl_8047BCA4;
extern u32 lbl_8047BCE0;
extern u32 lbl_8047BCE4;
extern u32 lbl_8047BCE8;
extern u32 lbl_8047BCEC;
extern u32 lbl_8047BC9C;
extern u32 lbl_8047BCA8;
extern u32 lbl_8047BCF0;
extern u32 lbl_8047A4E4;
extern u32 lbl_804788C0;
extern u32 lbl_8047A4E0;
#if 1
asm void fn_80042658(void) {
#include "src/game/scene_init_fn_80042658.inc"
}
#else
void fn_80042658(void) { /* TODO */ }
#endif

/* fn_80043728 - 0x80043728 | size: 0x294 */
extern u32 lbl_8047A4E8;
extern u32 lbl_8047BCF4;
extern u32 lbl_8047BCB0;
extern u32 lbl_8047BCF8;
extern u32 lbl_8047BC94;
#if 1
asm void fn_80043728(void) {
#include "src/game/scene_init_fn_80043728.inc"
}
#else
void fn_80043728(void) { /* TODO */ }
#endif

/* fn_800439BC - 0x800439BC | size: 0x31c */
extern void fn_800F7A08(void);
extern void fn_800F7A7C(void);
extern void fn_800CE2D8(void);
extern u32 lbl_8047BCB0;
extern u32 lbl_8047BC94;
extern u32 lbl_8047BCFC;
extern u32 lbl_8047BD00;
extern u32 lbl_8047BD04;
extern u32 lbl_8047BD08;
extern u32 lbl_804788C4;
#if 1
asm void fn_800439BC(void) {
#include "src/game/scene_init_fn_800439BC.inc"
}
#else
void fn_800439BC(void) { /* TODO */ }
#endif

/* fn_80043CD8 - 0x80043CD8 | size: 0xe8 */
extern u32 lbl_8047BCA0;
extern u32 lbl_8047BCB0;
extern u32 lbl_8047BCF4;
#if 1
asm void fn_80043CD8(void) {
#include "src/game/scene_init_fn_80043CD8.inc"
}
#else
void fn_80043CD8(void) { /* TODO */ }
#endif

/* fn_80043DC0 - 0x80043DC0 | size: 0x108 */
extern u32 lbl_8047BCA0;
extern u32 lbl_804788C0;
extern u32 lbl_8047BCA8;
extern u32 lbl_8047BCA4;
extern u32 lbl_8047BCB0;
extern u32 lbl_8047BD0C;
#if 1
asm void fn_80043DC0(void) {
#include "src/game/scene_init_fn_80043DC0.inc"
}
#else
void fn_80043DC0(void) { /* TODO */ }
#endif

/* fn_80043EC8 - 0x80043EC8 | size: 0xe0 */
extern u32 lbl_8047BCA0;
extern u32 lbl_8047BCA8;
extern u32 lbl_8047BCA4;
extern u32 lbl_8047BCB0;
extern u32 lbl_8047BD0C;
#if 1
asm void fn_80043EC8(void) {
#include "src/game/scene_init_fn_80043EC8.inc"
}
#else
void fn_80043EC8(void) { /* TODO */ }
#endif

/* fn_80043FA8 - 0x80043FA8 | size: 0x3d0 */
extern u32 lbl_8047BCA0;
extern u32 lbl_8047BCA8;
extern u32 lbl_8047BCA4;
extern u32 lbl_8047BD14;
extern u32 lbl_8047BD10;
extern u32 lbl_8047BC94;
#if 1
asm void fn_80043FA8(void) {
#include "src/game/scene_init_fn_80043FA8.inc"
}
#else
void fn_80043FA8(void) { /* TODO */ }
#endif

/* fn_80044378 - 0x80044378 | size: 0x2b8 */
extern u32 lbl_8047BCA0;
extern u32 lbl_8047BCF4;
extern u32 lbl_8047A4E0;
extern u32 lbl_8047A4E4;
extern u32 lbl_8047A4E8;
#if 1
asm void fn_80044378(void) {
#include "src/game/scene_init_fn_80044378.inc"
}
#else
void fn_80044378(void) { /* TODO */ }
#endif

/* fn_80046168 - 0x80046168 | size: 0x1164 */
extern void fn_8011E15C(void);
extern void fn_8011CA60(void);
extern void fn_801666BC(void);
extern void fn_800F7AF0(void);
extern u32 lbl_8047BCB0;
extern u32 lbl_8047BD1C;
extern u32 lbl_8047BC94;
extern u32 lbl_8047BCA0;
extern u32 lbl_8047BD18;
extern u32 lbl_8047A4EC;
extern u32 lbl_8047A4E4;
extern u32 lbl_8047A4E0;
extern u32 lbl_8047BCBC;
extern u32 lbl_8047BCFC;
extern u32 lbl_8047BD00;
#if 1
asm void fn_80046168(void) {
#include "src/game/scene_init_fn_80046168.inc"
}
#else
void fn_80046168(void) { /* TODO */ }
#endif

/* fn_800472CC - 0x800472CC | size: 0x114 */
extern u32 lbl_8047BCA0;
extern u32 lbl_8047A4EC;
extern u32 lbl_8047BCC4;
extern u32 lbl_8047BC94;
#if 1
asm void fn_800472CC(void) {
#include "src/game/scene_init_fn_800472CC.inc"
}
#else
void fn_800472CC(void) { /* TODO */ }
#endif

/* fn_800473E0 - 0x800473E0 | size: 0x4d4 */
extern void fn_80176228(void);
extern void fn_801760C4(void);
extern void fn_800D5648(void);
extern void fn_80176068(void);
extern u32 lbl_8047BCD8;
extern u32 lbl_8047BC94;
extern u32 lbl_8047BCBC;
extern u32 lbl_8047BD20;
extern u32 lbl_8047BCB0;
extern u32 lbl_8047BD24;
extern u32 lbl_8047BD2C;
extern u32 lbl_8047BD28;
extern u32 lbl_8047BCA8;
#if 1
asm void fn_800473E0(void) {
#include "src/game/scene_init_fn_800473E0.inc"
}
#else
void fn_800473E0(void) { /* TODO */ }
#endif

/* fn_800478B4 - 0x800478B4 | size: 0x40c */
extern void fn_801DAC24(void);
extern void fn_800CE220(void);
extern void fn_800E064C(void);
extern void fn_800E03B4(void);
extern void fn_800E4598(void);
extern u32 lbl_8047BD30;
extern u32 lbl_8047BCB0;
extern u32 lbl_8047BD34;
extern u32 lbl_8047BC94;
extern u8 lbl_802E5430[];
extern u32 lbl_8047BD18;
extern u8 lbl_802E5424[];
extern u32 lbl_8047BC98;
extern u32 lbl_8047BC9C;
#if 1
asm void fn_800478B4(void) {
#include "src/game/scene_init_fn_800478B4.inc"
}
#else
void fn_800478B4(void) { /* TODO */ }
#endif

/* fn_80047CC0 - 0x80047CC0 | size: 0x7e4 */
extern void fn_8025FA20(void);
extern void fn_800CD85C(void);
extern void fn_800E032C(void);
extern void fn_800E6BC8(void);
extern void fn_800EE0E8(void);
extern void fn_800EE150(void);
extern void fn_800EE3BC(void);
extern void fn_800EE828(void);
extern void fn_800E6B20(void);
extern u32 lbl_8047A4E0;
extern u32 lbl_8047BD38;
extern u32 lbl_8047BCBC;
extern u32 lbl_8047BCC0;
extern u32 lbl_8047A4E4;
extern u32 lbl_8047BD3C;
extern u32 lbl_8047BD40;
extern u32 lbl_8047BD44;
extern u32 lbl_8047BD48;
extern u32 lbl_8047BD4C;
extern u32 lbl_8047BD30;
extern u32 lbl_8047BCB0;
extern u32 lbl_8047BC94;
extern u32 lbl_8047BD50;
extern u32 lbl_8047BD58;
extern u32 lbl_8047BD60;
extern u8 lbl_80478AC0[];
extern u32 lbl_8047BD68;
extern u32 lbl_8047BD18;
extern u8 lbl_802E5418[];
extern u8 lbl_802E540C[];
extern u32 lbl_8047BC98;
extern u32 lbl_8047BC9C;
#if 1
asm void fn_80047CC0(void) {
#include "src/game/scene_init_fn_80047CC0.inc"
}
#else
void fn_80047CC0(void) { /* TODO */ }
#endif

/* fn_800484A4 - 0x800484A4 | size: 0x474 */
extern void fn_800E0518(void);
extern void fn_800E0370(void);
extern void fn_800DFF98(void);
extern void fn_800E0560(void);
extern u32 lbl_8047A4E0;
extern u32 lbl_8047BCBC;
extern u32 lbl_8047BCC0;
extern u32 lbl_8047A4E4;
extern u32 lbl_8047BD3C;
extern u32 lbl_8047BD40;
extern u32 lbl_8047BD44;
extern u32 lbl_8047BD48;
extern u32 lbl_8047BD4C;
extern u32 lbl_8047BD30;
extern u32 lbl_8047BCB0;
extern u32 lbl_8047BD34;
extern u32 lbl_8047BC94;
extern u32 lbl_8047BD18;
extern u8 lbl_802E53F4[];
extern u8 lbl_802E5400[];
extern u32 lbl_8047BC98;
extern u32 lbl_8047BC9C;
#if 1
asm void fn_800484A4(void) {
#include "src/game/scene_init_fn_800484A4.inc"
}
#else
void fn_800484A4(void) { /* TODO */ }
#endif

/* fn_80048918 - 0x80048918 | size: 0x9b4 */
extern void fn_801096F8(void);
extern void fn_801CB954(void);
extern void fn_80113F48(void);
extern void fn_800F9318(void);
extern void fn_800E4014(void);
extern void fn_801CB9D8(void);
extern void fn_801CBA0C(void);
extern void fn_800EC9DC(void);
extern void fn_800D1F84(void);
extern void fn_800D1F58(void);
extern void fn_80176E0C(void);
extern u8 lbl_80267150[];
extern u32 lbl_8047BC94;
extern u32 lbl_8047BCF0;
extern u32 lbl_8047BD18;
extern u32 lbl_8047A4E0;
extern u32 lbl_804788C0;
extern u32 lbl_8047BCBC;
extern u32 lbl_8047A4E4;
extern u32 lbl_804788C4;
extern u32 lbl_8047BCCC;
extern u32 lbl_8047BC98;
extern u32 lbl_8047BC9C;
extern u32 lbl_8047A4E8;
#if 1
asm void fn_80048918(void) {
#include "src/game/scene_init_fn_80048918.inc"
}
#else
void fn_80048918(void) { /* TODO */ }
#endif

/* fn_800492CC - 0x800492CC | size: 0x2fc */
extern u32 lbl_804788C0;
extern u8 lbl_804788CC[];
extern u8 lbl_804788C8[];
extern u8 lbl_804788D0[];
extern u32 lbl_8047BCA0;
extern u32 lbl_8047BC94;
extern u32 lbl_8047BCBC;
#if 1
asm void fn_800492CC(void) {
#include "src/game/scene_init_fn_800492CC.inc"
}
#else
void fn_800492CC(void) { /* TODO */ }
#endif

/* fn_8004A47C - 0x8004A47C | size: 0x32c */
extern void fn_801902E0(void);
extern u32 lbl_8047BD78;
extern u32 lbl_8047BD7C;
extern u32 lbl_8047BD80;
extern u32 lbl_8047BD84;
extern u8 lbl_8026719C[];
extern u8 lbl_802671B4[];
extern u32 lbl_8047BD88;
extern u32 lbl_8047BD8C;
extern u32 lbl_8047BD90;
extern u32 lbl_8047BD94;
#if 1
asm void fn_8004A47C(void) {
#include "src/game/scene_init_fn_8004A47C.inc"
}
#else
void fn_8004A47C(void) { /* TODO */ }
#endif

/* fn_8004A7A8 - 0x8004A7A8 | size: 0xad0 */
extern u8 lbl_803A6A60[];
extern u32 lbl_8047BDB0;
extern u32 lbl_8047BDCC;
extern u32 lbl_8047BDAC;
extern u32 lbl_804788D8;
extern u32 lbl_8047BDC8;
extern u32 lbl_8047BDA0;
extern u32 lbl_8047BDA8;
extern u32 lbl_8047BDD0;
extern u32 lbl_8047BDD4;
extern u32 lbl_8047BDB8;
#if 1
asm void fn_8004A7A8(void) {
#include "src/game/scene_init_fn_8004A7A8.inc"
}
#else
void fn_8004A7A8(void) { /* TODO */ }
#endif

/* fn_8004B278 - 0x8004B278 | size: 0x320 */
extern void fn_8005D8F8(void);
extern u32 lbl_8047BDC0;
extern u32 lbl_8047BDE8;
extern u32 lbl_8047BDA8;
extern u32 lbl_8047BDA0;
extern u32 lbl_8047BDAC;
extern u32 lbl_8047BDD8;
extern u32 lbl_8047BDDC;
extern u32 lbl_8047BDE0;
extern u32 lbl_8047BDE4;
#if 1
asm void fn_8004B278(void) {
#include "src/game/scene_init_fn_8004B278.inc"
}
#else
void fn_8004B278(void) { /* TODO */ }
#endif

/* fn_8004B598 - 0x8004B598 | size: 0x254 */
extern u8 lbl_80267190[];
extern u32 lbl_8047BDF0;
extern u32 lbl_8047BDAC;
#if 1
asm void fn_8004B598(void) {
#include "src/game/scene_init_fn_8004B598.inc"
}
#else
void fn_8004B598(void) { /* TODO */ }
#endif

/* fn_8004B7EC - 0x8004B7EC | size: 0x5cc */
extern void fn_801096E8(void);
extern void fn_800F92D4(void);
extern void fn_800E3CC8(void);
extern void fn_800ECCA8(void);
extern void fn_800ECA78(void);
extern void fn_800ECB74(void);
extern void fn_800DCAF0(void);
extern void fn_800DCADC(void);
extern void fn_800DCAD4(void);
extern void fn_800DCAB0(void);
extern void fn_800EC990(void);
extern void fn_800EC960(void);
extern void fn_800EC96C(void);
extern void fn_80176B48(void);
extern void fn_80044630(void);
extern void fn_8004C120(void);
extern void fn_8011288C(void);
extern u32 lbl_8047BDAC;
extern u32 lbl_8047BDA0;
extern u32 lbl_8047BDF8;
extern u32 lbl_8047BDF4;
extern u32 lbl_8047BDA8;
extern u8 lbl_8047A4FC[];
extern u8 lbl_8047A4F8[];
extern u32 lbl_8047BDFC;
extern u32 lbl_8047BE00;
#if 1
asm void fn_8004B7EC(void) {
#include "src/game/scene_init_fn_8004B7EC.inc"
}
#else
void fn_8004B7EC(void) { /* TODO */ }
#endif

/* fn_8004BDB8 - 0x8004BDB8 | size: 0x34 */
#if 1
asm void fn_8004BDB8(void) {
#include "src/game/scene_init_fn_8004BDB8.inc"
}
#else
void fn_8004BDB8(void) { /* TODO */ }
#endif

/* fn_8004BDEC - 0x8004BDEC | size: 0x10 */
#if 1
asm void fn_8004BDEC(void) {
#include "src/game/scene_init_fn_8004BDEC.inc"
}
#else
void fn_8004BDEC(void) { /* TODO */ }
#endif

/* fn_8004BDFC - 0x8004BDFC | size: 0x10 */
#if 1
asm void fn_8004BDFC(void) {
#include "src/game/scene_init_fn_8004BDFC.inc"
}
#else
void fn_8004BDFC(void) { /* TODO */ }
#endif

/* fn_8004BE40 - 0x8004BE40 | size: 0x50 */
extern void fn_801D1F7C(void);
extern u32 lbl_8047A500;
#if 1
asm void fn_8004BE40(void) {
#include "src/game/scene_init_fn_8004BE40.inc"
}
#else
void fn_8004BE40(void) { /* TODO */ }
#endif

/* fn_8004BE90 - 0x8004BE90 | size: 0x90 */
extern void fn_801D1ACC(void);
extern void fn_801D1E50(void);
#if 1
asm void fn_8004BE90(void) {
#include "src/game/scene_init_fn_8004BE90.inc"
}
#else
void fn_8004BE90(void) { /* TODO */ }
#endif

/* fn_8004BF20 - 0x8004BF20 | size: 0x90 */
extern void fn_801D1A88(void);
#if 1
asm void fn_8004BF20(void) {
#include "src/game/scene_init_fn_8004BF20.inc"
}
#else
void fn_8004BF20(void) { /* TODO */ }
#endif

/* fn_8004BFB0 - 0x8004BFB0 | size: 0x170 */
extern void fn_801D1B4C(void);
extern void fn_801D1F0C(void);
extern void fn_800CA620(void);
extern u32 lbl_8047A500;
#if 1
asm void fn_8004BFB0(void) {
#include "src/game/scene_init_fn_8004BFB0.inc"
}
#else
void fn_8004BFB0(void) { /* TODO */ }
#endif

/* fn_8004C2D8 - 0x8004C2D8 | size: 0x94 */
#if 1
asm void fn_8004C2D8(void) {
#include "src/game/scene_init_fn_8004C2D8.inc"
}
#else
void fn_8004C2D8(void) { /* TODO */ }
#endif

/* fn_8004C36C - 0x8004C36C | size: 0x78 */
#if 1
asm void fn_8004C36C(void) {
#include "src/game/scene_init_fn_8004C36C.inc"
}
#else
void fn_8004C36C(void) { /* TODO */ }
#endif

/* fn_8004C3E4 - 0x8004C3E4 | size: 0xc0 */
extern u8 lbl_802672C8[];
#if 1
asm void fn_8004C3E4(void) {
#include "src/game/scene_init_fn_8004C3E4.inc"
}
#else
void fn_8004C3E4(void) { /* TODO */ }
#endif

/* fn_8004C4A4 - 0x8004C4A4 | size: 0x10c */
extern u32 lbl_8047BE10;
extern u32 lbl_8047BE08;
extern u32 lbl_8047BE0C;
#if 1
asm void fn_8004C4A4(void) {
#include "src/game/scene_init_fn_8004C4A4.inc"
}
#else
void fn_8004C4A4(void) { /* TODO */ }
#endif

/* fn_8004C5B0 - 0x8004C5B0 | size: 0x110 */
extern void fn_80103E68(void);
extern u8 lbl_802671D0[];
#if 1
asm void fn_8004C5B0(void) {
#include "src/game/scene_init_fn_8004C5B0.inc"
}
#else
void fn_8004C5B0(void) { /* TODO */ }
#endif

/* fn_8004C6C0 - 0x8004C6C0 | size: 0x1ec */
extern void fn_801D1B78(void);
extern u8 lbl_802672A0[];
#if 1
asm void fn_8004C6C0(void) {
#include "src/game/scene_init_fn_8004C6C0.inc"
}
#else
void fn_8004C6C0(void) { /* TODO */ }
#endif

/* fn_8004C8AC - 0x8004C8AC | size: 0x1ec */
extern u8 lbl_80267278[];
#if 1
asm void fn_8004C8AC(void) {
#include "src/game/scene_init_fn_8004C8AC.inc"
}
#else
void fn_8004C8AC(void) { /* TODO */ }
#endif

/* fn_8004CA98 - 0x8004CA98 | size: 0x1a0 */
extern void fn_801D16F0(void);
extern u8 lbl_80267250[];
#if 1
asm void fn_8004CA98(void) {
#include "src/game/scene_init_fn_8004CA98.inc"
}
#else
void fn_8004CA98(void) { /* TODO */ }
#endif

/* fn_8004CC38 - 0x8004CC38 | size: 0x1a0 */
extern u8 lbl_80267228[];
#if 1
asm void fn_8004CC38(void) {
#include "src/game/scene_init_fn_8004CC38.inc"
}
#else
void fn_8004CC38(void) { /* TODO */ }
#endif

/* fn_8004CDD8 - 0x8004CDD8 | size: 0x1a0 */
extern u8 lbl_80267200[];
#if 1
asm void fn_8004CDD8(void) {
#include "src/game/scene_init_fn_8004CDD8.inc"
}
#else
void fn_8004CDD8(void) { /* TODO */ }
#endif

/* fn_8004CF78 - 0x8004CF78 | size: 0x2f4 */
extern void fn_80103EAC(void);
#if 1
asm void fn_8004CF78(void) {
#include "src/game/scene_init_fn_8004CF78.inc"
}
#else
void fn_8004CF78(void) { /* TODO */ }
#endif

/* fn_8004D26C - 0x8004D26C | size: 0xe0 */
extern u32 lbl_8047BE18;
extern u32 lbl_8047BE1C;
#if 1
asm void fn_8004D26C(void) {
#include "src/game/scene_init_fn_8004D26C.inc"
}
#else
void fn_8004D26C(void) { /* TODO */ }
#endif

/* fn_8004D34C - 0x8004D34C | size: 0x244 */
extern void fn_801D1B10(void);
extern void fn_801D16C4(void);
extern void fn_801D167C(void);
extern u32 lbl_804788E0;
extern u32 lbl_8047A50C;
extern u32 lbl_8047A508;
extern u32 lbl_8047BE20;
extern u8 lbl_8047A510[];
#if 1
asm void fn_8004D34C(void) {
#include "src/game/scene_init_fn_8004D34C.inc"
}
#else
void fn_8004D34C(void) { /* TODO */ }
#endif

/* fn_8004D590 - 0x8004D590 | size: 0x5c */
#if 1
asm void fn_8004D590(void) {
#include "src/game/scene_init_fn_8004D590.inc"
}
#else
void fn_8004D590(void) { /* TODO */ }
#endif

/* fn_8004D5EC - 0x8004D5EC | size: 0x60 */
#if 1
asm void fn_8004D5EC(void) {
#include "src/game/scene_init_fn_8004D5EC.inc"
}
#else
void fn_8004D5EC(void) { /* TODO */ }
#endif

/* fn_8004D64C - 0x8004D64C | size: 0x60 */
#if 1
asm void fn_8004D64C(void) {
#include "src/game/scene_init_fn_8004D64C.inc"
}
#else
void fn_8004D64C(void) { /* TODO */ }
#endif

/* fn_8004D6AC - 0x8004D6AC | size: 0x44 */
extern void fn_801D1A44(void);
#if 1
asm void fn_8004D6AC(void) {
#include "src/game/scene_init_fn_8004D6AC.inc"
}
#else
void fn_8004D6AC(void) { /* TODO */ }
#endif

/* fn_8004D6F0 - 0x8004D6F0 | size: 0x70 */
#if 1
asm void fn_8004D6F0(void) {
#include "src/game/scene_init_fn_8004D6F0.inc"
}
#else
void fn_8004D6F0(void) { /* TODO */ }
#endif

/* fn_8004D760 - 0x8004D760 | size: 0x70 */
#if 1
asm void fn_8004D760(void) {
#include "src/game/scene_init_fn_8004D760.inc"
}
#else
void fn_8004D760(void) { /* TODO */ }
#endif

/* fn_8004D7D0 - 0x8004D7D0 | size: 0xec */
extern void fn_801D1C20(void);
extern void fn_801D228C(void);
#if 1
asm void fn_8004D7D0(void) {
#include "src/game/scene_init_fn_8004D7D0.inc"
}
#else
void fn_8004D7D0(void) { /* TODO */ }
#endif

/* fn_8004D8BC - 0x8004D8BC | size: 0x6c */
#if 1
asm void fn_8004D8BC(void) {
#include "src/game/scene_init_fn_8004D8BC.inc"
}
#else
void fn_8004D8BC(void) { /* TODO */ }
#endif

/* fn_8004D928 - 0x8004D928 | size: 0x98 */
#if 1
asm void fn_8004D928(void) {
#include "src/game/scene_init_fn_8004D928.inc"
}
#else
void fn_8004D928(void) { /* TODO */ }
#endif

/* fn_8004D9C0 - 0x8004D9C0 | size: 0xa4 */
extern u8 lbl_8047A518[];
#if 1
asm void fn_8004D9C0(void) {
#include "src/game/scene_init_fn_8004D9C0.inc"
}
#else
void fn_8004D9C0(void) { /* TODO */ }
#endif

/* fn_8004DA64 - 0x8004DA64 | size: 0xd0 */
extern u8 lbl_802672D8[];
#if 1
asm void fn_8004DA64(void) {
#include "src/game/scene_init_fn_8004DA64.inc"
}
#else
void fn_8004DA64(void) { /* TODO */ }
#endif

/* fn_8004DB34 - 0x8004DB34 | size: 0x4c */
#if 1
asm void fn_8004DB34(void) {
#include "src/game/scene_init_fn_8004DB34.inc"
}
#else
void fn_8004DB34(void) { /* TODO */ }
#endif

/* fn_8004DB80 - 0x8004DB80 | size: 0x98 */
#if 1
asm void fn_8004DB80(void) {
#include "src/game/scene_init_fn_8004DB80.inc"
}
#else
void fn_8004DB80(void) { /* TODO */ }
#endif

/* fn_8004DC18 - 0x8004DC18 | size: 0xa8 */
#if 1
asm void fn_8004DC18(void) {
#include "src/game/scene_init_fn_8004DC18.inc"
}
#else
void fn_8004DC18(void) { /* TODO */ }
#endif

/* fn_8004DCC0 - 0x8004DCC0 | size: 0x100 */
extern void fn_801D1620(void);
extern u8 lbl_802672F0[];
#if 1
asm void fn_8004DCC0(void) {
#include "src/game/scene_init_fn_8004DCC0.inc"
}
#else
void fn_8004DCC0(void) { /* TODO */ }
#endif

/* fn_8004DDC0 - 0x8004DDC0 | size: 0x174 */
extern void fn_801D1650(void);
extern void fn_80166B18(void);
extern void fn_801654E0(void);
extern void fn_80166B3C(void);
extern void fn_801D1618(void);
extern void fn_801669E4(void);
extern u32 lbl_8047A520;
extern u32 lbl_8047A524;
extern u32 lbl_8047A52C;
extern u32 lbl_8047A528;
#if 1
asm void fn_8004DDC0(void) {
#include "src/game/scene_init_fn_8004DDC0.inc"
}
#else
void fn_8004DDC0(void) { /* TODO */ }
#endif

/* fn_8004DF34 - 0x8004DF34 | size: 0x98 */
#if 1
asm void fn_8004DF34(void) {
#include "src/game/scene_init_fn_8004DF34.inc"
}
#else
void fn_8004DF34(void) { /* TODO */ }
#endif

/* fn_8004DFCC - 0x8004DFCC | size: 0x178 */
extern void fn_8016557C(void);
extern void fn_800F9418(void);
extern void fn_800F9378(void);
extern void fn_800F9210(void);
extern void fn_80165548(void);
extern u32 lbl_8047A52C;
extern u32 lbl_8047A524;
extern u32 lbl_8047A528;
#if 1
asm void fn_8004DFCC(void) {
#include "src/game/scene_init_fn_8004DFCC.inc"
}
#else
void fn_8004DFCC(void) { /* TODO */ }
#endif

/* fn_8004E144 - 0x8004E144 | size: 0x3c */
extern u32 lbl_8047BE28;
extern u32 lbl_8047BE30;
#if 1
asm void fn_8004E144(void) {
#include "src/game/scene_init_fn_8004E144.inc"
}
#else
void fn_8004E144(void) { /* TODO */ }
#endif

/* fn_8004E180 - 0x8004E180 | size: 0x160 */
extern void fn_8017B448(void);
extern void fn_8017B4BC(void);
extern void fn_8017B5A4(void);
#if 1
asm void fn_8004E180(void) {
#include "src/game/scene_init_fn_8004E180.inc"
}
#else
void fn_8004E180(void) { /* TODO */ }
#endif

/* fn_8004E2E0 - 0x8004E2E0 | size: 0x160 */
extern u32 lbl_8047BE40;
extern u32 lbl_8047BE38;
extern u32 lbl_8047BE3C;
#if 1
asm void fn_8004E2E0(void) {
#include "src/game/scene_init_fn_8004E2E0.inc"
}
#else
void fn_8004E2E0(void) { /* TODO */ }
#endif

/* fn_8004E440 - 0x8004E440 | size: 0xd0 */
#if 1
asm void fn_8004E440(void) {
#include "src/game/scene_init_fn_8004E440.inc"
}
#else
void fn_8004E440(void) { /* TODO */ }
#endif

/* fn_8004E510 - 0x8004E510 | size: 0x280 */
extern void fn_8017B07C(void);
extern u32 lbl_8047BE48;
extern u32 lbl_8047BE4C;
#if 1
asm void fn_8004E510(void) {
#include "src/game/scene_init_fn_8004E510.inc"
}
#else
void fn_8004E510(void) { /* TODO */ }
#endif

/* fn_8004E790 - 0x8004E790 | size: 0x10c */
#if 1
asm void fn_8004E790(void) {
#include "src/game/scene_init_fn_8004E790.inc"
}
#else
void fn_8004E790(void) { /* TODO */ }
#endif

/* fn_8004E89C - 0x8004E89C | size: 0x44 */
#if 1
asm void fn_8004E89C(void) {
#include "src/game/scene_init_fn_8004E89C.inc"
}
#else
void fn_8004E89C(void) { /* TODO */ }
#endif

/* fn_8004E8E0 - 0x8004E8E0 | size: 0xe0 */
extern u32 lbl_8047BE50;
extern u32 lbl_8047BE4C;
#if 1
asm void fn_8004E8E0(void) {
#include "src/game/scene_init_fn_8004E8E0.inc"
}
#else
void fn_8004E8E0(void) { /* TODO */ }
#endif

/* fn_8004E9C0 - 0x8004E9C0 | size: 0x11c */
extern u32 lbl_804788E8;
extern u32 lbl_8047A534;
extern u32 lbl_8047A530;
extern u8 lbl_8047A538[];
extern u8 lbl_8047A53C[];
extern u32 lbl_8047BE48;
#if 1
asm void fn_8004E9C0(void) {
#include "src/game/scene_init_fn_8004E9C0.inc"
}
#else
void fn_8004E9C0(void) { /* TODO */ }
#endif

/* fn_8004EC54 - 0x8004EC54 | size: 0x178 */
extern void fn_801FBFBC(void);
extern u8 lbl_803A6AB0[];
#if 1
asm void fn_8004EC54(void) {
#include "src/game/scene_init_fn_8004EC54.inc"
}
#else
void fn_8004EC54(void) { /* TODO */ }
#endif

/* fn_8004EDCC - 0x8004EDCC | size: 0xa94 */
extern void fn_801FB1C0(void);
extern void fn_8001E224(void);
extern void fn_801FAA58(void);
extern void fn_8001E200(void);
#if 1
asm void fn_8004EDCC(void) {
#include "src/game/scene_init_fn_8004EDCC.inc"
}
#else
void fn_8004EDCC(void) { /* TODO */ }
#endif

/* fn_8004F860 - 0x8004F860 | size: 0x5dc */
#if 1
asm void fn_8004F860(void) {
#include "src/game/scene_init_fn_8004F860.inc"
}
#else
void fn_8004F860(void) { /* TODO */ }
#endif

/* fn_8004FE3C - 0x8004FE3C | size: 0xa08 */
#if 1
asm void fn_8004FE3C(void) {
#include "src/game/scene_init_fn_8004FE3C.inc"
}
#else
void fn_8004FE3C(void) { /* TODO */ }
#endif

/* fn_80051710 - 0x80051710 | size: 0x728 */
extern void fn_801FCAD0(void);
#if 1
asm void fn_80051710(void) {
#include "src/game/scene_init_fn_80051710.inc"
}
#else
void fn_80051710(void) { /* TODO */ }
#endif

/* fn_80051E38 - 0x80051E38 | size: 0x122c */
extern void fn_801FC658(void);
extern void fn_80008518(void);
extern void fn_80008460(void);
#if 1
asm void fn_80051E38(void) {
#include "src/game/scene_init_fn_80051E38.inc"
}
#else
void fn_80051E38(void) { /* TODO */ }
#endif

/* fn_80053064 - 0x80053064 | size: 0xac */
#if 1
asm void fn_80053064(void) {
#include "src/game/scene_init_fn_80053064.inc"
}
#else
void fn_80053064(void) { /* TODO */ }
#endif

/* fn_8005344C - 0x8005344C | size: 0xd0 */
#if 1
asm void fn_8005344C(void) {
#include "src/game/scene_init_fn_8005344C.inc"
}
#else
void fn_8005344C(void) { /* TODO */ }
#endif

/* fn_8005351C - 0x8005351C | size: 0x20c */
extern u8 lbl_80267338[];
extern u32 lbl_8047BE58;
extern u32 lbl_8047A540;
#if 1
asm void fn_8005351C(void) {
#include "src/game/scene_init_fn_8005351C.inc"
}
#else
void fn_8005351C(void) { /* TODO */ }
#endif

/* fn_80053728 - 0x80053728 | size: 0x50 */
#if 1
asm void fn_80053728(void) {
#include "src/game/scene_init_fn_80053728.inc"
}
#else
void fn_80053728(void) { /* TODO */ }
#endif

/* fn_80053A60 - 0x80053A60 | size: 0x68 */
extern void fn_80057270(void);
extern void fn_8011E820(void);
#if 1
asm void fn_80053A60(void) {
#include "src/game/scene_init_fn_80053A60.inc"
}
#else
void fn_80053A60(void) { /* TODO */ }
#endif

/* fn_80053AC8 - 0x80053AC8 | size: 0x68 */
#if 1
asm void fn_80053AC8(void) {
#include "src/game/scene_init_fn_80053AC8.inc"
}
#else
void fn_80053AC8(void) { /* TODO */ }
#endif

/* fn_80053B30 - 0x80053B30 | size: 0x68 */
#if 1
asm void fn_80053B30(void) {
#include "src/game/scene_init_fn_80053B30.inc"
}
#else
void fn_80053B30(void) { /* TODO */ }
#endif

/* fn_80053B98 - 0x80053B98 | size: 0x68 */
#if 1
asm void fn_80053B98(void) {
#include "src/game/scene_init_fn_80053B98.inc"
}
#else
void fn_80053B98(void) { /* TODO */ }
#endif

/* fn_80053C00 - 0x80053C00 | size: 0x84 */
extern void fn_801230E0(void);
#if 1
asm void fn_80053C00(void) {
#include "src/game/scene_init_fn_80053C00.inc"
}
#else
void fn_80053C00(void) { /* TODO */ }
#endif

/* fn_80053C84 - 0x80053C84 | size: 0x64 */
#if 1
asm void fn_80053C84(void) {
#include "src/game/scene_init_fn_80053C84.inc"
}
#else
void fn_80053C84(void) { /* TODO */ }
#endif

/* fn_80053CE8 - 0x80053CE8 | size: 0x7c */
extern void fn_8001DA60(void);
#if 1
asm void fn_80053CE8(void) {
#include "src/game/scene_init_fn_80053CE8.inc"
}
#else
void fn_80053CE8(void) { /* TODO */ }
#endif

/* fn_80053D64 - 0x80053D64 | size: 0x70 */
#if 1
asm void fn_80053D64(void) {
#include "src/game/scene_init_fn_80053D64.inc"
}
#else
void fn_80053D64(void) { /* TODO */ }
#endif

/* fn_80053DD4 - 0x80053DD4 | size: 0xa8 */
extern void fn_8011F4A8(void);
#if 1
asm void fn_80053DD4(void) {
#include "src/game/scene_init_fn_80053DD4.inc"
}
#else
void fn_80053DD4(void) { /* TODO */ }
#endif

/* fn_80053E7C - 0x80053E7C | size: 0x5c */
extern void fn_8011F4F0(void);
#if 1
asm void fn_80053E7C(void) {
#include "src/game/scene_init_fn_80053E7C.inc"
}
#else
void fn_80053E7C(void) { /* TODO */ }
#endif

/* fn_80053ED8 - 0x80053ED8 | size: 0x548 */
extern void fn_80123FBC(void);
extern void fn_800DC0D4(void);
extern void fn_800DC14C(void);
extern void fn_800DBFD4(void);
extern void fn_800DC04C(void);
extern void fn_800EF4FC(void);
extern void fn_800EF4F4(void);
extern void fn_8005D858(void);
extern void fn_80104160(void);
extern u8 lbl_802E61E8[];
extern u32 lbl_8047BE80;
extern u32 lbl_8047A558;
extern u8 lbl_80267320[];
extern u32 lbl_8047A550;
extern u32 lbl_8047BE6C;
extern u32 lbl_8047BE64;
extern u32 lbl_8047BE60;
extern u32 lbl_8047BE84;
extern u32 lbl_8047A54C;
extern u32 lbl_8047A548;
extern u32 lbl_8047BE58;
extern u32 lbl_8047BE78;
extern u32 lbl_8047BE68;
#if 1
asm void fn_80053ED8(void) {
#include "src/game/scene_init_fn_80053ED8.inc"
}
#else
void fn_80053ED8(void) { /* TODO */ }
#endif

/* fn_80054420 - 0x80054420 | size: 0x88 */
extern u32 lbl_8047BE80;
extern u32 lbl_8047A558;
#if 1
asm void fn_80054420(void) {
#include "src/game/scene_init_fn_80054420.inc"
}
#else
void fn_80054420(void) { /* TODO */ }
#endif

/* fn_800544A8 - 0x800544A8 | size: 0x1a4 */
extern u32 lbl_8047A544;
extern u32 lbl_8047BE68;
extern u32 lbl_8047BE60;
extern u32 lbl_8047A550;
extern u32 lbl_8047A54C;
extern u32 lbl_8047A540;
extern u32 lbl_8047A554;
extern u32 lbl_8047A558;
extern u32 lbl_8047BE88;
#if 1
asm void fn_800544A8(void) {
#include "src/game/scene_init_fn_800544A8.inc"
}
#else
void fn_800544A8(void) { /* TODO */ }
#endif

/* fn_8005464C - 0x8005464C | size: 0x24 */
extern u32 lbl_8047A54C;
extern u32 lbl_8047BE8C;
#if 1
asm void fn_8005464C(void) {
#include "src/game/scene_init_fn_8005464C.inc"
}
#else
void fn_8005464C(void) { /* TODO */ }
#endif

/* fn_80054670 - 0x80054670 | size: 0x10 */
extern u32 lbl_8047BE68;
extern u32 lbl_8047A548;
extern u32 lbl_8047A54C;
#if 1
asm void fn_80054670(void) {
#include "src/game/scene_init_fn_80054670.inc"
}
#else
void fn_80054670(void) { /* TODO */ }
#endif

/* fn_80054680 - 0x80054680 | size: 0x40 */
extern u32 lbl_8047A554;
extern u32 lbl_8047BE68;
extern u32 lbl_8047A558;
extern u32 lbl_8047BE60;
#if 1
asm void fn_80054680(void) {
#include "src/game/scene_init_fn_80054680.inc"
}
#else
void fn_80054680(void) { /* TODO */ }
#endif

/* fn_800546C0 - 0x800546C0 | size: 0x30 */
extern u32 lbl_8047BE60;
extern u32 lbl_8047BE68;
extern u32 lbl_8047A558;
extern u32 lbl_8047A554;
extern u32 lbl_8047BE90;
#if 1
asm void fn_800546C0(void) {
#include "src/game/scene_init_fn_800546C0.inc"
}
#else
void fn_800546C0(void) { /* TODO */ }
#endif

/* fn_800546F0 - 0x800546F0 | size: 0x2c */
extern u32 lbl_8047BE68;
extern u32 lbl_8047A558;
extern u32 lbl_8047A554;
extern u32 lbl_8047BE60;
extern u32 lbl_8047BE94;
#if 1
asm void fn_800546F0(void) {
#include "src/game/scene_init_fn_800546F0.inc"
}
#else
void fn_800546F0(void) { /* TODO */ }
#endif

/* fn_8005471C - 0x8005471C | size: 0x44 */
extern u8 lbl_803A9720[];
#if 1
asm void fn_8005471C(void) {
#include "src/game/scene_init_fn_8005471C.inc"
}
#else
void fn_8005471C(void) { /* TODO */ }
#endif

/* fn_80054760 - 0x80054760 | size: 0x1b4 */
extern void fn_8005D934(void);
extern void fn_80124A60(void);
extern u32 lbl_804788F8;
extern u32 lbl_8047A544;
extern u32 lbl_8047BE60;
extern u32 lbl_8047BE68;
extern u32 lbl_8047A558;
extern u32 lbl_8047A554;
extern u8 lbl_803A95E8[];
#if 1
asm void fn_80054760(void) {
#include "src/game/scene_init_fn_80054760.inc"
}
#else
void fn_80054760(void) { /* TODO */ }
#endif

/* fn_800549F0 - 0x800549F0 | size: 0x3c */
#if 1
asm void fn_800549F0(void) {
#include "src/game/scene_init_fn_800549F0.inc"
}
#else
void fn_800549F0(void) { /* TODO */ }
#endif

/* fn_80054A2C - 0x80054A2C | size: 0x3c */
#if 1
asm void fn_80054A2C(void) {
#include "src/game/scene_init_fn_80054A2C.inc"
}
#else
void fn_80054A2C(void) { /* TODO */ }
#endif

/* fn_80054A68 - 0x80054A68 | size: 0x3c */
#if 1
asm void fn_80054A68(void) {
#include "src/game/scene_init_fn_80054A68.inc"
}
#else
void fn_80054A68(void) { /* TODO */ }
#endif

/* fn_80054AA4 - 0x80054AA4 | size: 0x3c */
#if 1
asm void fn_80054AA4(void) {
#include "src/game/scene_init_fn_80054AA4.inc"
}
#else
void fn_80054AA4(void) { /* TODO */ }
#endif

/* fn_80054AE0 - 0x80054AE0 | size: 0x3c */
#if 1
asm void fn_80054AE0(void) {
#include "src/game/scene_init_fn_80054AE0.inc"
}
#else
void fn_80054AE0(void) { /* TODO */ }
#endif

/* fn_80054B1C - 0x80054B1C | size: 0x128 */
#if 1
asm void fn_80054B1C(void) {
#include "src/game/scene_init_fn_80054B1C.inc"
}
#else
void fn_80054B1C(void) { /* TODO */ }
#endif

/* fn_80054C44 - 0x80054C44 | size: 0x238 */
extern void fn_80102F38(void);
extern void fn_80104318(void);
extern u8 lbl_80267398[];
#if 1
asm void fn_80054C44(void) {
#include "src/game/scene_init_fn_80054C44.inc"
}
#else
void fn_80054C44(void) { /* TODO */ }
#endif

/* fn_80054E7C - 0x80054E7C | size: 0x4c */
#if 1
asm void fn_80054E7C(void) {
#include "src/game/scene_init_fn_80054E7C.inc"
}
#else
void fn_80054E7C(void) { /* TODO */ }
#endif

/* fn_80054EC8 - 0x80054EC8 | size: 0x1ec */
extern void fn_80056A78(void);
extern void fn_80134EF0(void);
extern void fn_80134AF8(void);
extern void fn_80134E10(void);
extern u32 lbl_8047A560;
#if 1
asm void fn_80054EC8(void) {
#include "src/game/scene_init_fn_80054EC8.inc"
}
#else
void fn_80054EC8(void) { /* TODO */ }
#endif

/* fn_800550B4 - 0x800550B4 | size: 0xe0 */
#if 1
asm void fn_800550B4(void) {
#include "src/game/scene_init_fn_800550B4.inc"
}
#else
void fn_800550B4(void) { /* TODO */ }
#endif

/* fn_80055194 - 0x80055194 | size: 0x38 */
#if 1
asm void fn_80055194(void) {
#include "src/game/scene_init_fn_80055194.inc"
}
#else
void fn_80055194(void) { /* TODO */ }
#endif

/* fn_800551CC - 0x800551CC | size: 0x108 */
extern void fn_801347D8(void);
#if 1
asm void fn_800551CC(void) {
#include "src/game/scene_init_fn_800551CC.inc"
}
#else
void fn_800551CC(void) { /* TODO */ }
#endif

/* fn_800558B8 - 0x800558B8 | size: 0x2e0 */
extern void fn_8005D3D0(void);
extern void fn_800552D4(void);
extern void fn_8005D26C(void);
extern void fn_80057B34(void);
extern u32 lbl_8047A560;
extern u32 lbl_8047BE98;
#if 1
asm void fn_800558B8(void) {
#include "src/game/scene_init_fn_800558B8.inc"
}
#else
void fn_800558B8(void) { /* TODO */ }
#endif

/* fn_80055B98 - 0x80055B98 | size: 0x94 */
#if 1
asm void fn_80055B98(void) {
#include "src/game/scene_init_fn_80055B98.inc"
}
#else
void fn_80055B98(void) { /* TODO */ }
#endif

/* fn_80055C2C - 0x80055C2C | size: 0xa8 */
extern void fn_801347E0(void);
extern void fn_801348EC(void);
#if 1
asm void fn_80055C2C(void) {
#include "src/game/scene_init_fn_80055C2C.inc"
}
#else
void fn_80055C2C(void) { /* TODO */ }
#endif

/* fn_80055CD4 - 0x80055CD4 | size: 0x60 */
extern void fn_80134A98(void);
#if 1
asm void fn_80055CD4(void) {
#include "src/game/scene_init_fn_80055CD4.inc"
}
#else
void fn_80055CD4(void) { /* TODO */ }
#endif

/* fn_80055D34 - 0x80055D34 | size: 0xac */
#if 1
asm void fn_80055D34(void) {
#include "src/game/scene_init_fn_80055D34.inc"
}
#else
void fn_80055D34(void) { /* TODO */ }
#endif

/* fn_80055DE0 - 0x80055DE0 | size: 0x30 */
#if 1
asm void fn_80055DE0(void) {
#include "src/game/scene_init_fn_80055DE0.inc"
}
#else
void fn_80055DE0(void) { /* TODO */ }
#endif

/* fn_80055E10 - 0x80055E10 | size: 0x28 */
#if 1
asm void fn_80055E10(void) {
#include "src/game/scene_init_fn_80055E10.inc"
}
#else
void fn_80055E10(void) { /* TODO */ }
#endif

/* fn_80055E38 - 0x80055E38 | size: 0x80 */
extern void fn_801070F4(void);
extern u8 lbl_8026768C[];
#if 1
asm void fn_80055E38(void) {
#include "src/game/scene_init_fn_80055E38.inc"
}
#else
void fn_80055E38(void) { /* TODO */ }
#endif

/* fn_80055EB8 - 0x80055EB8 | size: 0xd0 */
#if 1
asm void fn_80055EB8(void) {
#include "src/game/scene_init_fn_80055EB8.inc"
}
#else
void fn_80055EB8(void) { /* TODO */ }
#endif

/* fn_80055F88 - 0x80055F88 | size: 0xfc */
extern u8 lbl_80267680[];
#if 1
asm void fn_80055F88(void) {
#include "src/game/scene_init_fn_80055F88.inc"
}
#else
void fn_80055F88(void) { /* TODO */ }
#endif

/* fn_80056084 - 0x80056084 | size: 0x58c */
extern u8 lbl_80267518[];
extern u32 lbl_8047A574;
extern u32 lbl_8047BEB8;
extern u32 lbl_8047BEB0;
extern u32 lbl_8047BEB4;
extern u32 lbl_8047BEBC;
extern u32 lbl_8047A570;
extern u32 lbl_8047A56C;
extern u32 lbl_8047BEA8;
extern u32 lbl_8047BEA0;
extern u32 lbl_8047BEC0;
#if 1
asm void fn_80056084(void) {
#include "src/game/scene_init_fn_80056084.inc"
}
#else
void fn_80056084(void) { /* TODO */ }
#endif

/* fn_80056610 - 0x80056610 | size: 0xa4 */
extern u32 lbl_8047A568;
#if 1
asm void fn_80056610(void) {
#include "src/game/scene_init_fn_80056610.inc"
}
#else
void fn_80056610(void) { /* TODO */ }
#endif

/* fn_800566B4 - 0x800566B4 | size: 0x24 */
extern u32 lbl_8047A570;
extern u32 lbl_8047BEC4;
#if 1
asm void fn_800566B4(void) {
#include "src/game/scene_init_fn_800566B4.inc"
}
#else
void fn_800566B4(void) { /* TODO */ }
#endif

/* fn_800566D8 - 0x800566D8 | size: 0x10 */
extern u32 lbl_8047BEC0;
extern u32 lbl_8047A56C;
extern u32 lbl_8047A570;
#if 1
asm void fn_800566D8(void) {
#include "src/game/scene_init_fn_800566D8.inc"
}
#else
void fn_800566D8(void) { /* TODO */ }
#endif

/* fn_800566E8 - 0x800566E8 | size: 0x1c */
extern u32 lbl_8047BEC0;
extern u32 lbl_8047A578;
#if 1
asm void fn_800566E8(void) {
#include "src/game/scene_init_fn_800566E8.inc"
}
#else
void fn_800566E8(void) { /* TODO */ }
#endif

/* fn_80056704 - 0x80056704 | size: 0xa8 */
extern void fn_80102254(void);
extern u32 lbl_8047A584;
extern u32 lbl_8047A580;
extern u32 lbl_8047BEC0;
extern u32 lbl_8047BEC8;
extern u32 lbl_8047A57C;
extern u32 lbl_8047A578;
#if 1
asm void fn_80056704(void) {
#include "src/game/scene_init_fn_80056704.inc"
}
#else
void fn_80056704(void) { /* TODO */ }
#endif

/* fn_800567AC - 0x800567AC | size: 0xa8 */
extern u32 lbl_8047A584;
extern u32 lbl_8047A580;
extern u32 lbl_8047BEC0;
extern u32 lbl_8047BECC;
extern u32 lbl_8047A57C;
extern u32 lbl_8047A578;
#if 1
asm void fn_800567AC(void) {
#include "src/game/scene_init_fn_800567AC.inc"
}
#else
void fn_800567AC(void) { /* TODO */ }
#endif

/* fn_80056854 - 0x80056854 | size: 0x224 */
extern u32 lbl_8047A578;
extern u32 lbl_8047BEC0;
extern u32 lbl_8047A57C;
extern u32 lbl_8047BEB4;
extern u32 lbl_8047BED0;
extern u32 lbl_8047A580;
extern u32 lbl_8047BED4;
extern u32 lbl_8047A584;
extern u32 lbl_8047A574;
extern u32 lbl_8047BED8;
extern u32 lbl_8047A570;
#if 1
asm void fn_80056854(void) {
#include "src/game/scene_init_fn_80056854.inc"
}
#else
void fn_80056854(void) { /* TODO */ }
#endif

/* fn_80056A80 - 0x80056A80 | size: 0xf4 */
extern u32 lbl_8047A584;
#if 1
asm void fn_80056A80(void) {
#include "src/game/scene_init_fn_80056A80.inc"
}
#else
void fn_80056A80(void) { /* TODO */ }
#endif

/* fn_80056B74 - 0x80056B74 | size: 0xe0 */
extern u32 lbl_8047A568;
extern u32 lbl_8047BEC0;
extern u32 lbl_8047BEB4;
extern u32 lbl_8047A584;
extern u32 lbl_8047A580;
extern u32 lbl_8047A57C;
extern u32 lbl_8047A578;
extern u32 lbl_8047A574;
extern u32 lbl_8047A570;
#if 1
asm void fn_80056B74(void) {
#include "src/game/scene_init_fn_80056B74.inc"
}
#else
void fn_80056B74(void) { /* TODO */ }
#endif

/* fn_80057094 - 0x80057094 | size: 0x3c */
extern u8 lbl_803A9768[];
#if 1
asm void fn_80057094(void) {
#include "src/game/scene_init_fn_80057094.inc"
}
#else
void fn_80057094(void) { /* TODO */ }
#endif

/* fn_800570D0 - 0x800570D0 | size: 0x44 */
extern void fn_80056C54(void);
#if 1
asm void fn_800570D0(void) {
#include "src/game/scene_init_fn_800570D0.inc"
}
#else
void fn_800570D0(void) { /* TODO */ }
#endif

/* fn_80057114 - 0x80057114 | size: 0x30 */
#if 1
asm void fn_80057114(void) {
#include "src/game/scene_init_fn_80057114.inc"
}
#else
void fn_80057114(void) { /* TODO */ }
#endif

/* fn_80057144 - 0x80057144 | size: 0x12c */
extern u8 lbl_802676B4[];
#if 1
asm void fn_80057144(void) {
#include "src/game/scene_init_fn_80057144.inc"
}
#else
void fn_80057144(void) { /* TODO */ }
#endif

/* fn_800573C0 - 0x800573C0 | size: 0x40 */
extern u32 lbl_8047BF00;
#if 1
asm void fn_800573C0(void) {
#include "src/game/scene_init_fn_800573C0.inc"
}
#else
void fn_800573C0(void) { /* TODO */ }
#endif

/* fn_80057400 - 0x80057400 | size: 0x28 */
#if 1
asm void fn_80057400(void) {
#include "src/game/scene_init_fn_80057400.inc"
}
#else
void fn_80057400(void) { /* TODO */ }
#endif

/* fn_80057428 - 0x80057428 | size: 0x24 */
extern u32 lbl_8047A588;
extern u32 lbl_8047BF04;
#if 1
asm void fn_80057428(void) {
#include "src/game/scene_init_fn_80057428.inc"
}
#else
void fn_80057428(void) { /* TODO */ }
#endif

/* fn_8005744C - 0x8005744C | size: 0xc */
extern u32 lbl_8047BF00;
extern u32 lbl_8047A588;
#if 1
asm void fn_8005744C(void) {
#include "src/game/scene_init_fn_8005744C.inc"
}
#else
void fn_8005744C(void) { /* TODO */ }
#endif

/* fn_80057458 - 0x80057458 | size: 0x50 */
#if 1
asm void fn_80057458(void) {
#include "src/game/scene_init_fn_80057458.inc"
}
#else
void fn_80057458(void) { /* TODO */ }
#endif

/* fn_800574A8 - 0x800574A8 | size: 0x38 */
#if 1
asm void fn_800574A8(void) {
#include "src/game/scene_init_fn_800574A8.inc"
}
#else
void fn_800574A8(void) { /* TODO */ }
#endif

/* fn_800574E0 - 0x800574E0 | size: 0x1c */
#if 1
asm void fn_800574E0(void) {
#include "src/game/scene_init_fn_800574E0.inc"
}
#else
void fn_800574E0(void) { /* TODO */ }
#endif

/* fn_800574FC - 0x800574FC | size: 0x3c */
#if 1
asm void fn_800574FC(void) {
#include "src/game/scene_init_fn_800574FC.inc"
}
#else
void fn_800574FC(void) { /* TODO */ }
#endif

/* fn_80057538 - 0x80057538 | size: 0x15c */
extern void fn_80107170(void);
extern u8 lbl_80267698[];
#if 1
asm void fn_80057538(void) {
#include "src/game/scene_init_fn_80057538.inc"
}
#else
void fn_80057538(void) { /* TODO */ }
#endif

/* fn_80057694 - 0x80057694 | size: 0x10 */
#if 1
asm void fn_80057694(void) {
#include "src/game/scene_init_fn_80057694.inc"
}
#else
void fn_80057694(void) { /* TODO */ }
#endif

/* fn_800576A4 - 0x800576A4 | size: 0x10 */
#if 1
asm void fn_800576A4(void) {
#include "src/game/scene_init_fn_800576A4.inc"
}
#else
void fn_800576A4(void) { /* TODO */ }
#endif

/* fn_800576B4 - 0x800576B4 | size: 0x10 */
#if 1
asm void fn_800576B4(void) {
#include "src/game/scene_init_fn_800576B4.inc"
}
#else
void fn_800576B4(void) { /* TODO */ }
#endif

/* fn_800576C4 - 0x800576C4 | size: 0x16c */
extern void fn_801081F8(void);
#if 1
asm void fn_800576C4(void) {
#include "src/game/scene_init_fn_800576C4.inc"
}
#else
void fn_800576C4(void) { /* TODO */ }
#endif

/* fn_80057830 - 0x80057830 | size: 0x118 */
extern u32 lbl_8047BEE8;
extern u32 lbl_8047BEF4;
extern u32 lbl_8047BF00;
extern u32 lbl_8047BF08;
#if 1
asm void fn_80057830(void) {
#include "src/game/scene_init_fn_80057830.inc"
}
#else
void fn_80057830(void) { /* TODO */ }
#endif

/* fn_80057948 - 0x80057948 | size: 0xc0 */
extern u32 lbl_8047BF00;
extern u32 lbl_8047BEF4;
extern u32 lbl_8047A58C;
extern u32 lbl_8047BF0C;
extern u32 lbl_8047A588;
#if 1
asm void fn_80057948(void) {
#include "src/game/scene_init_fn_80057948.inc"
}
#else
void fn_80057948(void) { /* TODO */ }
#endif

/* fn_80057A08 - 0x80057A08 | size: 0x30 */
#if 1
asm void fn_80057A08(void) {
#include "src/game/scene_init_fn_80057A08.inc"
}
#else
void fn_80057A08(void) { /* TODO */ }
#endif

/* fn_80057A38 - 0x80057A38 | size: 0x2c */
#if 1
asm void fn_80057A38(void) {
#include "src/game/scene_init_fn_80057A38.inc"
}
#else
void fn_80057A38(void) { /* TODO */ }
#endif

/* fn_80057A64 - 0x80057A64 | size: 0xd0 */
extern u32 lbl_8047BF00;
extern u32 lbl_8047BEF4;
extern u32 lbl_8047A58C;
extern u32 lbl_8047A588;
#if 1
asm void fn_80057A64(void) {
#include "src/game/scene_init_fn_80057A64.inc"
}
#else
void fn_80057A64(void) { /* TODO */ }
#endif

/* fn_80057C9C - 0x80057C9C | size: 0x14c */
extern void fn_80097D94(void);
extern u8 lbl_803A9A08[];
#if 1
asm void fn_80057C9C(void) {
#include "src/game/scene_init_fn_80057C9C.inc"
}
#else
void fn_80057C9C(void) { /* TODO */ }
#endif

/* fn_80057DE8 - 0x80057DE8 | size: 0x58 */
extern void fn_8011FC74(void);
#if 1
asm void fn_80057DE8(void) {
#include "src/game/scene_init_fn_80057DE8.inc"
}
#else
void fn_80057DE8(void) { /* TODO */ }
#endif

/* fn_80057E40 - 0x80057E40 | size: 0x30 */
#if 1
asm void fn_80057E40(void) {
#include "src/game/scene_init_fn_80057E40.inc"
}
#else
void fn_80057E40(void) { /* TODO */ }
#endif

/* fn_80057E70 - 0x80057E70 | size: 0x124 */
extern void fn_8017B13C(void);
extern u32 lbl_8047A590;
#if 1
asm void fn_80057E70(void) {
#include "src/game/scene_init_fn_80057E70.inc"
}
#else
void fn_80057E70(void) { /* TODO */ }
#endif

/* fn_80057F94 - 0x80057F94 | size: 0x1bc */
extern void fn_80122334(void);
extern u8 lbl_802676F0[];
#if 1
asm void fn_80057F94(void) {
#include "src/game/scene_init_fn_80057F94.inc"
}
#else
void fn_80057F94(void) { /* TODO */ }
#endif

/* fn_80058150 - 0x80058150 | size: 0x604 */
extern void fn_801CAC6C(void);
extern void fn_800E3534(void);
extern void fn_800FF560(void);
extern void fn_800F07A8(void);
extern void fn_801CAAF4(void);
extern void fn_80190528(void);
extern u32 lbl_8047A590;
#if 1
asm void fn_80058150(void) {
#include "src/game/scene_init_fn_80058150.inc"
}
#else
void fn_80058150(void) { /* TODO */ }
#endif

/* fn_80058754 - 0x80058754 | size: 0x44 */
extern u8 lbl_803A9A18[];
#if 1
asm void fn_80058754(void) {
#include "src/game/scene_init_fn_80058754.inc"
}
#else
void fn_80058754(void) { /* TODO */ }
#endif

/* fn_80058798 - 0x80058798 | size: 0x40 */
#if 1
asm void fn_80058798(void) {
#include "src/game/scene_init_fn_80058798.inc"
}
#else
void fn_80058798(void) { /* TODO */ }
#endif

/* fn_800587D8 - 0x800587D8 | size: 0x2c */
#if 1
asm void fn_800587D8(void) {
#include "src/game/scene_init_fn_800587D8.inc"
}
#else
void fn_800587D8(void) { /* TODO */ }
#endif

/* fn_80058804 - 0x80058804 | size: 0x7c */
#if 1
asm void fn_80058804(void) {
#include "src/game/scene_init_fn_80058804.inc"
}
#else
void fn_80058804(void) { /* TODO */ }
#endif

/* fn_80058880 - 0x80058880 | size: 0x230 */
extern u8 lbl_802677D0[];
extern u32 lbl_8047A598;
extern u32 lbl_8047A59C;
#if 1
asm void fn_80058880(void) {
#include "src/game/scene_init_fn_80058880.inc"
}
#else
void fn_80058880(void) { /* TODO */ }
#endif

/* fn_80058AB0 - 0x80058AB0 | size: 0x40 */
#if 1
asm void fn_80058AB0(void) {
#include "src/game/scene_init_fn_80058AB0.inc"
}
#else
void fn_80058AB0(void) { /* TODO */ }
#endif

/* fn_80058AF0 - 0x80058AF0 | size: 0x2dc */
#if 1
asm void fn_80058AF0(void) {
#include "src/game/scene_init_fn_80058AF0.inc"
}
#else
void fn_80058AF0(void) { /* TODO */ }
#endif

/* fn_80058DCC - 0x80058DCC | size: 0x13c */
#if 1
asm void fn_80058DCC(void) {
#include "src/game/scene_init_fn_80058DCC.inc"
}
#else
void fn_80058DCC(void) { /* TODO */ }
#endif

/* fn_80058F08 - 0x80058F08 | size: 0x38 */
#if 1
asm void fn_80058F08(void) {
#include "src/game/scene_init_fn_80058F08.inc"
}
#else
void fn_80058F08(void) { /* TODO */ }
#endif

/* fn_80058F40 - 0x80058F40 | size: 0xf4 */
#if 1
asm void fn_80058F40(void) {
#include "src/game/scene_init_fn_80058F40.inc"
}
#else
void fn_80058F40(void) { /* TODO */ }
#endif

/* fn_800599AC - 0x800599AC | size: 0x230 */
extern void fn_80059034(void);
extern u32 lbl_8047A59C;
extern u32 lbl_8047A598;
#if 1
asm void fn_800599AC(void) {
#include "src/game/scene_init_fn_800599AC.inc"
}
#else
void fn_800599AC(void) { /* TODO */ }
#endif

