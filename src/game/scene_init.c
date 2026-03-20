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

#pragma push
#pragma force_active on

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

#pragma pop
