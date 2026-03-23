/**
 * @file gs_field_world.c
 * @brief GSfield world logic -- field camera, object management, world updates.
 *
 * This is the largest sub-module of GSfield, containing the field camera
 * system, world object management, and per-frame field update logic.
 *
 * Decompiled from (selected major functions):
 *   fn_80114CA8 - fn_80117514: Floor data parsing, scene setup
 *   fn_80117514 (floorUpdateFieldCamera)
 *   fn_80117514 - fn_80130CE0: World update, object management,
 *                               field rendering, transition logic
 *
 * The field camera (fn_80117514) is the most identifiable function via
 * its debug string: "floorUpdateFieldCamera: error updating field camera
 * - divide by zero!"
 *
 * This file covers approximately 600+ functions in the range
 * 0x80114CA8 - 0x80130CE0. The functions include:
 *   - Scene data loading and relocation
 *   - Field camera interpolation and cutscene cameras
 *   - Field object spawn/despawn/update
 *   - NPC placement and region triggers
 *   - Weather and ambient effect control
 *   - Door/warp transition logic
 *   - Field rendering pass management
 *   - Minimap integration
 *
 * Key patterns observed:
 *   - Many small accessor functions (0x18-0x34 bytes) that read
 *     struct fields at fixed offsets (0x8011E4A4-0x8011E800 area)
 *   - Large state machine functions (fn_8012640C at 5456 bytes,
 *     fn_801254B4 at 3928 bytes) for field transition logic
 *   - Iteration over object lists with 0x28 or 0x34 byte strides
 *   - Frequent calls to GSmem (fn_800E27B0, fn_800E3534)
 *   - Matrix operations for camera (fn_800A37CC, fn_800A3A9C)
 *
 * Debug strings:
 *   "floorUpdateFieldCamera: error updating field camera - divide by zero!"
 *
 * Address range: 0x80114CA8 - 0x80130CE0
 */
#include "dolphin/types.h"
#include "game/world/gs_field.h"
/* ===== External SDK / engine functions ===== */
extern void  fn_800DD970(const char* fmt, ...);         /* OSReport / GSlog */
extern void* memcpy(void* dst, const void* src, u32 n);
extern void* memset(void* dst, int val, u32 size);
/* GSmem */
extern u16   fn_800E3534(u32 size);                     /* GSmemAllocRaw */
extern void* fn_800E27B0(u16 handle);                   /* GSmemGetPtr */
extern void* fn_800E24B0(u16 handle);                   /* GSmemLock */
extern void  fn_800E209C(u16 handle);                   /* GSmemFree */
/* Matrix / vector */
extern void  fn_800A37CC(void* mtx, void* vec, void* out); /* MTXMultVec3 */
extern void  fn_800A3A9C(void* out, void* in, f32 s);     /* VEC normalize */
/* GSgfx renderer */
extern void  fn_800D7868(void* handle, u32 a, u32 b, u32 c,
                          u32 d, u32 e, u32 f, u32 g);     /* GSgfx draw setup */
extern void* fn_800D7894(void);                             /* GSgfx create render obj */
/* GSfloor / GScolsys */
extern void* fn_800FF56C(void);                             /* GSfloor get active */
extern void  fn_8010C7BC(u32 triIdx, void* outFlag);        /* GScolsys query */
/* ===== Index lookup globals ===== */
extern u8 lbl_8035BBA8[];  /* NPC table (BSS) */
extern u8 lbl_8035C430[];  /* field obj table (BSS) */
extern u32 lbl_80478B48;  /* NPC count (SDA) */
extern u32 lbl_80478B50;  /* field obj count (SDA) */
extern u32 lbl_80478F90;  /* obj header ptr (SDA) */
extern u32 lbl_80478F94;  /* obj data base (SDA) */
/* Field subsystems -- forward declarations (defined below) */
u32  fn_80115094(void);
void fn_80115A38(u32 entry);
void fn_80117C84(void);
void fn_8012546C(void* ptr);
/* ===== String constants (rodata) ===== */
extern const char lbl_80272770[]; /* "floorUpdateFieldCamera: error updating..." */
extern const char lbl_802724E8[]; /* "floorReadMapPreFunc: can't alloc..." */
extern const char lbl_80272520[]; /* "floorReadScriptPreFunc(): can't alloc..." */
extern const char lbl_8027255C[]; /* "floorReadFontPreFunc(): can't alloc..." */
extern const char lbl_80272594[]; /* "floorReadMsgPreFunc(): can't alloc..." */
extern const char lbl_802725CC[]; /* "floorReadNormalPreFunc(): can't alloc..." */
/* ==================================================================
 * fn_80117514 -- floorUpdateFieldCamera
 *
 * Update the field camera each frame. Interpolates position, target,
 * and FOV toward their destination values. Includes safety check for
 * divide-by-zero when computing distance-based interpolation.
 *
 * This function is 0x1B4 bytes (436 bytes) and uses extensive float
 * math for smooth camera transitions.
 *
 * From disassembly:
 *   - Loads camera state from a BSS pointer
 *   - Computes direction vector from current to destination
 *   - Normalizes and scales by interpSpeed
 *   - If distance is near-zero, snaps to destination (avoids /0)
 *   - On divide-by-zero path: logs lbl_80272770
 * ================================================================== */
void floorUpdateFieldCamera(void) {
    extern u8 lbl_8047CFD0[];
    extern u8 lbl_8047CFDC[];
    extern u8 lbl_8047CFE0[];
    extern void fn_800E0020();
    extern void fn_800E01F4();
    extern u8 lbl_8047AD68;
    extern u8 lbl_8047AD6C;
    u8 sp[0xA0];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f26 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    *(f64*)(sp + 0x90) = f31;
    /* psq_st f31, 0x98((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x80) = f30;
    /* psq_st f30, 0x88((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x70) = f29;
    /* psq_st f29, 0x78((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x60) = f28;
    /* psq_st f28, 0x68((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x50) = f27;
    /* psq_st f27, 0x58((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x40) = f26;
    /* psq_st f26, 0x48((u32)sp), 0, qr0 */;
    r25 = r3;
    r26 = r4;
    r27 = r5;
    r28 = r6;
    r0 = *(u32*)&lbl_8047AD68;
    if ((u32)r0 == (u32)0x1) {
        r4 = *(u32*)&lbl_8047AD6C;
        r3 = 0x1;
        f0 = *(f32*)((u8*)r4 + 0x10);
        *(f32*)((u8*)r26 + 0x0) = f0;
        r4 = *(u32*)&lbl_8047AD6C;
        f0 = *(f32*)((u8*)r4 + 0xC);
        *(f32*)((u8*)r27 + 0x0) = f0;
        r4 = *(u32*)&lbl_8047AD6C;
        f0 = *(f32*)((u8*)r4 + 0x14);
        *(f32*)((u8*)r28 + 0x0) = f0;
    } else {
    f29 = *(f32*)lbl_8047CFD0;
    r30 = 0x0;
    r31 = 0x0;
    f28 = f29;
    f27 = f29;
    f26 = f29;
    f30 = *(f32*)lbl_8047CFDC;
    f31 = *(f32*)lbl_8047CFE0;
    while (r0 = *(u32*)&lbl_8047AD68, (u32)r30 < (u32)r0) {

    r0 = *(u32*)&lbl_8047AD6C;
    r3 = (u32)sp + 0x8;
    r29 = r0 + r31;
    f1 = *(f32*)((u8*)r29 + 0x0);
    f2 = *(f32*)((u8*)r29 + 0x4);
    f3 = *(f32*)((u8*)r29 + 0x8);
    fn_800E01F4();
    r4 = r25;
    r3 = (u32)sp + 0x8;
    fn_800E0020();
    if (f1 > f30) {
        f3 = f31 / f1;
        f2 = *(f32*)((u8*)r29 + 0xC);
        f1 = *(f32*)((u8*)r29 + 0x10);
        f0 = *(f32*)((u8*)r29 + 0x14);
        f29 = f29 + f3;
        f28 = f2 * f3 + f28;
        f27 = f1 * f3 + f27;
        f26 = f0 * f3 + f26;
    r31 = r31 + 0x18;
    r30 = r30 + 0x1;
        continue;
    }
    f29 = f30;
    f27 = *(f32*)((u8*)r29 + 0x10);
    f28 = *(f32*)((u8*)r29 + 0xC);
    f26 = *(f32*)((u8*)r29 + 0x14);
    break;
    }

    f0 = *(f32*)lbl_8047CFD0;
    if (f0 == f29) {
        r3 = (u32)&lbl_80272770;
        r3 = (u32)&lbl_80272770;
        ((void(*)(void))fn_800DD970)();
        r3 = 0x0;
    } else {
    f0 = *(f32*)lbl_8047CFDC;
    r3 = 0x1;
    f0 = f0 / f29;
    f2 = f27 * f0;
    f1 = f28 * f0;
    f0 = f26 * f0;
    *(f32*)((u8*)r26 + 0x0) = f2;
    *(f32*)((u8*)r27 + 0x0) = f1;
    *(f32*)((u8*)r28 + 0x0) = f0;
    }
    }
    /* psq_l f31, 0x98((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x90);
    /* psq_l f30, 0x88((u32)sp), 0, qr0 */;
    f30 = *(f64*)(sp + 0x80);
    /* psq_l f29, 0x78((u32)sp), 0, qr0 */;
    f29 = *(f64*)(sp + 0x70);
    /* psq_l f28, 0x68((u32)sp), 0, qr0 */;
    f28 = *(f64*)(sp + 0x60);
    /* psq_l f27, 0x58((u32)sp), 0, qr0 */;
    f27 = *(f64*)(sp + 0x50);
    /* psq_l f26, 0x48((u32)sp), 0, qr0 */;
    f26 = *(f64*)(sp + 0x40);
    return;
}
/* ==================================================================
 * fn_8012640C -- Field transition state machine
 *
 * At 5456 bytes, this is the largest function in the field system.
 * It manages the multi-step process of transitioning between floors:
 *   1. Fade out current floor
 *   2. Unload current resources
 *   3. Load new floor data
 *   4. Set up collision, camera, NPCs
 *   5. Fade in new floor
 *
 * Uses a switch/case state machine (detected by large jump table in
 * the disassembly).
 * ================================================================== */
void GSfield_TransitionStateMachine(void) {
    extern void fn_8011BEB4();
    extern void fn_8011CE44();
    extern void fn_8011CE74();
    extern void fn_8011E000();
    extern void fn_8011E018();
    extern void fn_8011E030();
    extern void fn_8011E048();
    extern void fn_8011E078();
    extern void fn_8011E0AC();
    extern void fn_8011E0C4();
    extern void fn_8011E0DC();
    extern void fn_8011E0F4();
    extern void fn_8011E128();
    extern void fn_8011E15C();
    extern void fn_8011E174();
    extern void fn_8011E18C();
    extern void fn_8011E1A4();
    extern void fn_8011E1BC();
    extern void fn_8011E1D4();
    extern void fn_8011E21C();
    extern void fn_8011E264();
    extern void fn_8011E2AC();
    extern void fn_8011E2DC();
    extern void fn_8011E324();
    extern void fn_8011E36C();
    extern void fn_8011E3B4();
    extern void fn_8011E3FC();
    extern void fn_8011E444();
    extern void fn_8011E474();
    extern void fn_8011E4A4();
    extern void fn_8011E4D8();
    extern void fn_8011E4F0();
    extern void fn_8011E508();
    extern void fn_8011E520();
    extern void fn_8011E538();
    extern void fn_8011E550();
    extern void fn_8011E57C();
    extern void fn_8011E5A8();
    extern void fn_8011E5D4();
    extern void fn_8011E600();
    extern void fn_8011E62C();
    extern void fn_8011E658();
    extern void fn_8011E684();
    extern void fn_8011E6B0();
    extern void fn_8011E6DC();
    extern void fn_8011E708();
    extern void fn_8011E734();
    extern void fn_8011E760();
    extern void fn_8011E778();
    extern void fn_8011E7C0();
    extern void fn_8011E7D8();
    extern void fn_8011E7F0();
    extern void fn_8011E808();
    extern void fn_8011E820();
    extern void fn_8011E838();
    extern void fn_8011E850();
    extern void fn_8011E868();
    extern void fn_8011E8DC();
    extern void fn_8011E8F4();
    extern void fn_8011E90C();
    extern void fn_8011E938();
    extern void fn_8011E964();
    extern void fn_8011E990();
    extern void fn_8011E9BC();
    extern void fn_8011E9E8();
    extern void fn_8011EA14();
    extern void fn_8011EA40();
    extern void fn_8011EA6C();
    extern void fn_8011EA98();
    extern void fn_8011EAC4();
    extern void fn_8011EAF0();
    extern void fn_8011EB1C();
    extern void fn_8011EB48();
    extern void fn_8011EB60();
    extern void fn_8011EB8C();
    extern void fn_8011EBB8();
    extern void fn_8011EBE4();
    extern void fn_8011EC10();
    extern void fn_8011EC3C();
    extern void fn_8011EC68();
    extern void fn_8011EC94();
    extern void fn_8011ECC0();
    extern void fn_8011ECEC();
    extern void fn_8011ED18();
    extern void fn_8011ED68();
    extern void fn_8011EDC4();
    extern void fn_8011EDF8();
    extern void fn_8011EE10();
    extern void fn_8011EE28();
    extern void fn_8011EE40();
    extern void fn_8011EE58();
    extern void fn_8011EE70();
    extern void fn_8011EE9C();
    extern void fn_8011EEC8();
    extern void fn_8011EEF4();
    extern void fn_8011EF20();
    extern void fn_8011EF4C();
    extern void fn_8011EF78();
    extern void fn_8011EFA4();
    extern void fn_8011EFD0();
    extern void fn_8011EFFC();
    extern void fn_8011F028();
    extern void fn_8011F054();
    extern void fn_8011F080();
    extern void fn_8011F0AC();
    extern void fn_8011F0D8();
    extern void fn_8011F104();
    extern void fn_8011F130();
    extern void fn_8011F15C();
    extern void fn_8011F188();
    extern void fn_8011F1A0();
    extern void fn_8011F1B8();
    extern void fn_8011F1F0();
    extern void fn_8011F228();
    extern void fn_8011F45C();
    extern void fn_8011F474();
    extern void fn_8011F4A8();
    extern void fn_8011F4C0();
    extern void fn_8011F4D8();
    extern void fn_8011F4F0();
    extern void fn_8011F508();
    extern void fn_8011F520();
    extern void fn_8011F538();
    extern void fn_8011F550();
    extern void fn_8011F568();
    extern void fn_8011F580();
    extern void fn_8011F598();
    extern void fn_8011F5B0();
    extern void fn_8011F5C8();
    extern void fn_8012640C();
    extern void fn_80131574();
    extern void fn_801FCF8C();
    extern void fn_801FCFA4();
    extern void fn_801FCFBC();
    extern void fn_801FCFD4();
    extern void fn_801FCFEC();
    extern void fn_801FD004();
    extern void fn_801FD01C();
    extern void fn_801FD034();
    extern void fn_801FD04C();
    extern void fn_801FD064();
    extern void fn_801FD11C();
    extern void fn_801FD160();
    extern void fn_801FD188();
    extern void fn_801FD340();
    extern void fn_801FD358();
    extern void fn_801FD370();
    extern void fn_801FD388();
    extern void fn_801FD3A0();
    extern void fn_801FD3B8();
    extern void fn_801FD3D0();
    extern void fn_801FD3E8();
    extern void fn_801FD400();
    extern void fn_801FD418();
    extern void fn_801FD430();
    extern void fn_801FD448();
    extern void fn_801FD460();
    extern void fn_801FD478();
    extern void fn_801FD490();
    extern void fn_801FD4A8();
    extern void fn_801FD4C0();
    extern void fn_801FD4D8();
    extern void fn_801FD4F0();
    extern void fn_801FD508();
    extern void fn_801FD520();
    extern void fn_801FD538();
    extern void fn_801FD550();
    extern void fn_801FD568();
    extern void fn_801FD580();
    extern void fn_801FD598();
    extern void fn_801FD5B0();
    extern void fn_801FD5D8();
    extern void fn_801FD614();
    extern void fn_801FD648();
    extern void fn_801FD684();
    extern void fn_801FD738();
    extern void fn_801FD750();
    extern void fn_801FD768();
    extern void fn_801FD780();
    extern void fn_801FD798();
    extern void fn_801FD7B0();
    extern void fn_801FD7C8();
    extern void fn_801FD7E0();
    extern void fn_801FD808();
    extern void fn_801FD8E0();
    extern void fn_801FD948();
    extern void fn_801FD960();
    extern void fn_801FD978();
    extern void fn_801FD990();
    extern void fn_801FD9A8();
    extern void fn_801FD9C0();
    extern void fn_801FD9D8();
    extern void fn_801FD9F0();
    extern void fn_801FDA08();
    extern void fn_801FDA20();
    extern void fn_801FDA38();
    extern void fn_801FDA6C();
    extern void fn_801FDA84();
    extern void fn_801FDA9C();
    extern void fn_801FDAB4();
    extern void fn_801FDACC();
    extern void fn_801FDAE4();
    extern void fn_801FDAFC();
    extern void fn_801FDB14();
    extern void fn_801FDB48();
    extern void fn_801FDB60();
    extern void fn_80205184();
    extern void fn_80205224();
    extern void fn_802062FC();
    extern void fn_802096E8();
    extern void fn_8020990C();
    extern u8 jumptable_8035E4B0[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    r0 = r5 & 0xFFFF;
    r30 = r5;
    r29 = r6;
    r28 = r3;
    if ((s32)r0 == (s32)0) { r3 = 0x0; return; }
    if ((u32)r0 >= (u32)0x124) {
        r3 = 0x0;
        return;
    }
    if ((u32)r0 < (u32)0x6d) {
        r3 = r4;
        fn_8011E778();
        if ((u32)r0 != (u32)0x6d) goto L_8012647C;
        r3 = 0x0;
        return;
    }
    if ((u32)r28 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
L_8012647C: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 > (u32)0x123) { r3 = 0x0; return; }
    r3 = (u32)jumptable_8035E4B0;
    r0 = r0 << 2;
    r3 = (u32)jumptable_8035E4B0;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r3 = r28;
    fn_8011E760();
    return;
    r3 = r28;
    fn_8011E734();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011E708();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011E6DC();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011E6B0();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011E684();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011E658();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011E62C();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011E600();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011E5D4();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011E5A8();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011E57C();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011E550();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011E538();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011E520();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011E508();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011E4F0();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011E4D8();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    r4 = r29;
    fn_8011E4A4();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    r4 = r29;
    fn_8011E474();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = r29;
    fn_8011E444();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = r29;
    fn_8011E3FC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = r29;
    fn_8011E3B4();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    r4 = r29;
    fn_8011E36C();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    r4 = r29;
    fn_8011E324();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = r29;
    fn_8011E2DC();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    r4 = 0x0;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x1;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x2;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x3;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x4;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x5;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x6;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x7;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x8;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x9;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0xa;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0xb;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0xc;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0xd;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0xe;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0xf;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x10;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x11;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x12;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x13;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x14;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x15;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x16;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x17;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x18;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x1a;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x1b;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x1c;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x1d;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x1e;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x1f;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x20;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x21;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x22;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x23;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x24;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x25;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x26;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x27;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x28;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x29;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x2a;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x2b;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x2c;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x2d;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x2e;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x2f;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x30;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x31;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x32;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x33;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x34;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x35;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x36;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x37;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x38;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x39;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x3a;
    fn_8011E2AC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = r29;
    fn_8011E264();
    return;
    r3 = r28;
    r4 = r29;
    fn_8011E21C();
    return;
    r3 = r28;
    r4 = r29;
    fn_8011E1D4();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011E1BC();
    return;
    r3 = r28;
    fn_8011E1A4();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011E18C();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011E174();
    return;
    r3 = r28;
    fn_8011E15C();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011E128();
    return;
    r3 = r28;
    fn_8011E0F4();
    return;
    r3 = r28;
    fn_8011E0DC();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011E0C4();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011E0AC();
    return;
    r3 = r28;
    r4 = r29;
    fn_8011E078();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    r4 = r29;
    fn_8011E048();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011E030();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011E018();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011E000();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011F5C8();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011F5B0();
    return;
    r3 = r28;
    fn_8011F598();
    return;
    r3 = r28;
    fn_8011F580();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011F568();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011F550();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011F538();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011F520();
    return;
    r3 = r28;
    fn_8011F508();
    return;
    r3 = r28;
    fn_8011F4F0();
    return;
    r3 = r28;
    fn_8011F4D8();
    return;
    r3 = r28;
    fn_8011F4C0();
    return;
    r3 = r28;
    fn_8011F4A8();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x83;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFFFF;
    r0 = __cntlzw(r0);
    return;
    r3 = r28;
    r4 = r29;
    fn_8011F474();
    return;
    r3 = r28;
    fn_8011F45C();
    return;
    r3 = r28;
    r4 = r29;
    fn_8011F228();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    r4 = r29;
    fn_8011F1F0();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = r29;
    fn_8011F1B8();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011F1A0();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011F188();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011F15C();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011F130();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011F104();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011F0D8();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011F0AC();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011F080();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011F054();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011F028();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011EFFC();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011EFD0();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011EFA4();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011EF78();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011EF4C();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011EF20();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011EEF4();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011EEC8();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011EE9C();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011EE70();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011EE58();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011ECEC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011ECC0();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011EC94();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011EC68();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011EC3C();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011EB48();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011EB1C();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011EAF0();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011EAC4();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011EA98();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011EA6C();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011EA40();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011EA14();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011E9E8();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011E9BC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011E990();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011E964();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011E938();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011E90C();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011EC10();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011EBE4();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011EBB8();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011EB8C();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011EB60();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011E8F4();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011E8DC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011E868();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011E850();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011E838();
    r3 = r3 & 0xFF;
    return;
    if ((u32)r28 == (u32)0x0) {
        r0 = 0x2;
        r3 = r0 & 0xFF;
        return;
    }
    r3 = r28;
    r4 = 0x0;
    r5 = 0x6f;
    r6 = 0x0;
    fn_8012640C();
    r31 = r3;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x13;
    r6 = 0x0;
    fn_8012640C();
    r30 = r3 & 0xFFFF;
    if ((u32)r28 == (u32)0x0) {
        r3 = 0x2;
        goto L_80127134;
    }
    r3 = r28;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x13;
    r6 = 0x0;
    fn_8012640C();
    r29 = r3 & 0xFFFF;
    r3 = 0x0;
    fn_80131574();
    r0 = r3 & 0xFF;
    if ((s32)r29 == (s32)r0) {
        r3 = 0x0;
        goto L_80127134;
    }
    r3 = 0x1;
    fn_80131574();
    r0 = r3 & 0xFF;
    if ((s32)r29 == (s32)r0) {
        r3 = 0x1;
        goto L_80127134;
    }
    r3 = 0x2;
    fn_80131574();
    r0 = r3 & 0xFF;
    if ((s32)r29 == (s32)r0) {
        r3 = 0x2;
        goto L_80127134;
    }
    r3 = -0x1;
L_80127134: ;
    r0 = (s8)r3;
    if ((s32)r29 < (s32)r0) {
        r0 = r31 & 0xFF;
        if ((u32)r30 <= (u32)r0) goto L_80127150;
        r3 = 0x1;
    }
    goto L_80127154;
L_80127150: ;
    r3 = 0x0;
L_80127154: ;
    r0 = r3 & 0xFF;
    r3 = r0 & 0xFF;
    return;
    r3 = r28;
    fn_8011E820();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011E808();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_8011E7D8();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011E7F0();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x6f;
    r6 = 0x0;
    fn_8012640C();
    r4 = (0x51ec << 16);
    r0 = (u32)((u64)r0 * (u64)r3 >> 32);
    r0 = (u32)r0 >> 3;
    r0 = r0 * 0x19;
    r0 = r3 - r0;
    r3 = r0 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x11;
    r6 = 0x0;
    fn_8012640C();
    r30 = r3 & 0xFF;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x79;
    r6 = 0x0;
    fn_8012640C();
    r31 = r3;
    r3 = r30;
    fn_8011CE74();
    if ((u32)r30 == (u32)r0) {
        r0 = 0x0;
        r3 = r0 & 0xFF;
        return;
    }
    r29 = 0x1;
L_80127234: ;
    r3 = r30;
    r4 = r29 & 0xFF;
    fn_8011CE44();
    if ((u32)r3 > (u32)r31) goto L_80127254;
    r29 = r29 + 0x1;
    if ((s32)r29 < (s32)0x65) goto L_80127234;
L_80127254: ;
    r0 = r0 & 0xFF;
    r3 = r0 & 0xFF;
    return;
    if ((u32)r28 == (u32)0x0) {
        r0 = 0x0;
    } else {
        r3 = r28;
        r4 = 0x0;
        r5 = 0x75;
        r6 = 0x0;
        fn_8012640C();
        r31 = r3;
        r3 = r28;
        r4 = 0x0;
        r5 = 0x6f;
        r6 = 0x0;
        fn_8012640C();
        r4 = (u32)r31 >> 16;
        r0 = r31 & 0xFFFF;
        r5 = (u32)r3 >> 16;
        r6 = r3 & 0xFFFF;
        r0 = r4 ^ r0;
        r3 = 0x8;
        r0 = r5 ^ r0;
        r0 = r6 ^ r0;
        r0 = r3 ^ r0;
        r0 = __cntlzw(r0);
        r0 = r3 << r0;
        r0 = (u32)r0 >> 31;
    }
    r3 = r0 & 0xFF;
    return;
    r3 = r28;
    fn_8011E7C0();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011EE40();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011ED18();
    return;
    r3 = r28;
    fn_8011EE28();
    return;
    r3 = r28;
    fn_8011EE10();
    return;
    r3 = r28;
    fn_8011EDF8();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_8011ED68();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = r29;
    fn_8011EDC4();
    return;
    r3 = r28;
    fn_801FDB60();
    return;
    r3 = r28;
    fn_801FDB48();
    return;
    r3 = r28;
    r4 = r29;
    fn_801FDB14();
    return;
    r3 = r28;
    fn_801FDAFC();
    r3 = (s16)r3;
    return;
    r3 = r28;
    fn_801FDAE4();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FDACC();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FDAB4();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD8E0();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FDA9C();
    return;
    r3 = r28;
    fn_801FDA84();
    return;
    r3 = r28;
    fn_801FDA6C();
    return;
    r3 = r28;
    r4 = r29;
    fn_801FDA38();
    return;
    r3 = r28;
    fn_801FDA20();
    return;
    r3 = r28;
    fn_80205184();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_80205224();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    fn_8012640C();
    r4 = 0x0;
    r5 = 0x2c;
    r6 = 0x0;
    fn_8011BEB4();
    return;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    fn_8012640C();
    r4 = 0x0;
    r5 = 0x2b;
    r6 = 0x0;
    fn_8011BEB4();
    return;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    fn_8012640C();
    r4 = r29;
    fn_8020990C();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    fn_8012640C();
    fn_802096E8();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    fn_8012640C();
    r4 = 0x0;
    r5 = 0x2d;
    r6 = 0x0;
    fn_8011BEB4();
    return;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    fn_8012640C();
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BEB4();
    return;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    fn_8012640C();
    r4 = 0x0;
    r5 = 0x29;
    r6 = 0x0;
    fn_8011BEB4();
    return;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    fn_8012640C();
    r4 = 0x0;
    r5 = 0x2e;
    r6 = 0x0;
    fn_8011BEB4();
    return;
    r3 = r28;
    fn_801FDA08();
    return;
    r3 = r28;
    fn_801FD9F0();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD9D8();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD9C0();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD9A8();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD990();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD978();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD960();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD948();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_801FD808();
    return;
    r3 = r28;
    fn_801FD064();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_801FD04C();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_801FD034();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_801FD01C();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_801FD004();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_801FCFEC();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_801FCFD4();
    r3 = (s16)r3;
    return;
    r3 = r28;
    fn_801FCFBC();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_801FCFA4();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_801FCF8C();
    return;
    r3 = r28;
    fn_801FD7E0();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD7C8();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_801FD7B0();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_801FD798();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    r4 = r29 & 0xFF;
    fn_801FD684();
    r3 = (s16)r3;
    return;
    r3 = r28;
    fn_801FD648();
    return;
    r3 = r28;
    r4 = r29 & 0xFF;
    fn_801FD614();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_801FD5D8();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_801FD5B0();
    return;
    r3 = r28;
    fn_801FD780();
    r3 = (s16)r3;
    return;
    r3 = r28;
    fn_801FD768();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_801FD750();
    r3 = (s16)r3;
    return;
    r3 = r28;
    fn_801FD738();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_801FD598();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD580();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD568();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD550();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD538();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD520();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD508();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD4F0();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD4D8();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD4C0();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD4A8();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD490();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD478();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD460();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD448();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD430();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD418();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD400();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD3E8();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD3D0();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD3B8();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD3A0();
    return;
    r3 = r28;
    fn_801FD388();
    r3 = (s16)r3;
    return;
    r3 = r28;
    fn_801FD370();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_801FD358();
    r3 = (s16)r3;
    return;
    r3 = r28;
    fn_801FD340();
    r3 = r3 & 0xFFFF;
    return;
    r3 = r28;
    fn_801FD188();
    r3 = r3 & 0xFF;
    return;
    r3 = r28;
    fn_801FD160();
    r3 = (s16)r3;
    return;
    r3 = r28;
    r4 = r29;
    fn_801FD11C();
    return;
    r3 = r28;
    fn_802062FC();
    r3 = r3 & 0xFF;
    return;
    r3 = 0x0;
    return;
}
/* ==================================================================
 * fn_801254B4 -- Field rendering pass
 *
 * Second-largest function in the field system at 3928 bytes.
 * Manages the per-frame rendering of the field scene, including:
 *   - Setting up the view matrix from camera state
 *   - Drawing the ground mesh
 *   - Drawing field objects and NPCs
 *   - Applying post-processing effects
 * ================================================================== */
void GSfield_RenderPass(void) {
    extern void fn_8011BBD8();
    extern void fn_8011CEA0();
    extern void fn_8011CEB0();
    extern void fn_8011CEC0();
    extern void fn_8011CED0();
    extern void fn_8011CEF0();
    extern void fn_8011CF14();
    extern void fn_8011CF24();
    extern void fn_8011CF34();
    extern void fn_8011CF44();
    extern void fn_8011CF70();
    extern void fn_8011CF9C();
    extern void fn_8011CFAC();
    extern void fn_8011CFBC();
    extern void fn_8011CFCC();
    extern void fn_8011CFDC();
    extern void fn_8011CFEC();
    extern void fn_8011D02C();
    extern void fn_8011D06C();
    extern void fn_8011D0AC();
    extern void fn_8011D0CC();
    extern void fn_8011D10C();
    extern void fn_8011D14C();
    extern void fn_8011D18C();
    extern void fn_8011D1CC();
    extern void fn_8011D20C();
    extern void fn_8011D22C();
    extern void fn_8011D24C();
    extern void fn_8011D270();
    extern void fn_8011D280();
    extern void fn_8011D290();
    extern void fn_8011D2A0();
    extern void fn_8011D2B0();
    extern void fn_8011D2C0();
    extern void fn_8011D2E4();
    extern void fn_8011D308();
    extern void fn_8011D32C();
    extern void fn_8011D350();
    extern void fn_8011D374();
    extern void fn_8011D398();
    extern void fn_8011D3BC();
    extern void fn_8011D3E0();
    extern void fn_8011D404();
    extern void fn_8011D428();
    extern void fn_8011D44C();
    extern void fn_8011D470();
    extern void fn_8011D494();
    extern void fn_8011D4A4();
    extern void fn_8011D4B4();
    extern void fn_8011D4C4();
    extern void fn_8011D4D4();
    extern void fn_8011D4E4();
    extern void fn_8011D4F4();
    extern void fn_8011D504();
    extern void fn_8011D56C();
    extern void fn_8011D57C();
    extern void fn_8011D58C();
    extern void fn_8011D5B0();
    extern void fn_8011D5D4();
    extern void fn_8011D5F8();
    extern void fn_8011D61C();
    extern void fn_8011D640();
    extern void fn_8011D664();
    extern void fn_8011D688();
    extern void fn_8011D6AC();
    extern void fn_8011D6D0();
    extern void fn_8011D6F4();
    extern void fn_8011D718();
    extern void fn_8011D73C();
    extern void fn_8011D760();
    extern void fn_8011D770();
    extern void fn_8011D794();
    extern void fn_8011D7B8();
    extern void fn_8011D7DC();
    extern void fn_8011D800();
    extern void fn_8011D824();
    extern void fn_8011D848();
    extern void fn_8011D86C();
    extern void fn_8011D890();
    extern void fn_8011D8B4();
    extern void fn_8011D8D8();
    extern void fn_8011D8F4();
    extern void fn_8011D904();
    extern void fn_8011D924();
    extern void fn_8011D958();
    extern void fn_8011D98C();
    extern void fn_8011D9C0();
    extern void fn_8011D9F4();
    extern void fn_8011DA28();
    extern void fn_8011DA5C();
    extern void fn_8011DA90();
    extern void fn_8011DAC4();
    extern void fn_8011DAF8();
    extern void fn_8011DB2C();
    extern void fn_8011DB60();
    extern void fn_8011DB94();
    extern void fn_8011DBB8();
    extern void fn_8011DBDC();
    extern void fn_8011DC00();
    extern void fn_8011DC24();
    extern void fn_8011DC48();
    extern void fn_8011DC6C();
    extern void fn_8011DCB4();
    extern void fn_8011DCC4();
    extern void fn_8011DD80();
    extern void fn_8011DDFC();
    extern void fn_8011DE38();
    extern void fn_8011DE48();
    extern void fn_8011DE68();
    extern void fn_8011DE88();
    extern void fn_8011DE98();
    extern void fn_8011DEA8();
    extern void fn_8011DEE4();
    extern void fn_8011DF54();
    extern void fn_8011DF90();
    extern void fn_8011DFA0();
    extern void fn_8011DFB0();
    extern void fn_8011DFC0();
    extern void fn_8011DFD0();
    extern void fn_8011DFE0();
    extern void fn_8011DFF0();
    extern void fn_8011E778();
    extern void fn_8012640C();
    extern void fn_801FCEFC();
    extern void fn_801FCF0C();
    extern void fn_801FCF1C();
    extern void fn_801FCF2C();
    extern void fn_801FCF3C();
    extern void fn_801FCF4C();
    extern void fn_801FCF5C();
    extern void fn_801FCF6C();
    extern void fn_801FCF7C();
    extern void fn_801FD150();
    extern void fn_801FD178();
    extern void fn_801FD1A0();
    extern void fn_801FD1B0();
    extern void fn_801FD1C0();
    extern void fn_801FD1D0();
    extern void fn_801FD1E0();
    extern void fn_801FD1F0();
    extern void fn_801FD200();
    extern void fn_801FD210();
    extern void fn_801FD220();
    extern void fn_801FD230();
    extern void fn_801FD240();
    extern void fn_801FD250();
    extern void fn_801FD260();
    extern void fn_801FD270();
    extern void fn_801FD280();
    extern void fn_801FD290();
    extern void fn_801FD2A0();
    extern void fn_801FD2B0();
    extern void fn_801FD2C0();
    extern void fn_801FD2D0();
    extern void fn_801FD2E0();
    extern void fn_801FD2F0();
    extern void fn_801FD300();
    extern void fn_801FD310();
    extern void fn_801FD320();
    extern void fn_801FD330();
    extern void fn_801FD5C8();
    extern void fn_801FD5F0();
    extern void fn_801FD660();
    extern void fn_801FD6B8();
    extern void fn_801FD6C8();
    extern void fn_801FD6D8();
    extern void fn_801FD6E8();
    extern void fn_801FD6F8();
    extern void fn_801FD708();
    extern void fn_801FD718();
    extern void fn_801FD728();
    extern void fn_801FD7F8();
    extern void fn_801FD820();
    extern void fn_801FD840();
    extern void fn_801FD850();
    extern void fn_801FD860();
    extern void fn_801FD870();
    extern void fn_801FD880();
    extern void fn_801FD890();
    extern void fn_801FD8A0();
    extern void fn_801FD8B0();
    extern void fn_801FD8C0();
    extern void fn_801FD8D0();
    extern void fn_801FD8F8();
    extern void fn_801FD908();
    extern void fn_801FD918();
    extern void fn_801FD928();
    extern void fn_801FD938();
    extern void fn_802097C8();
    extern void fn_8020981C();
    extern void fn_8020990C();
    extern void fn_80209960();
    extern void fn_80209FAC();
    extern u8 jumptable_8035E028[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    r0 = r5 & 0xFFFF;
    r31 = r7;
    r30 = r6;
    r29 = r5;
    if ((s32)r0 == (s32)0) return;
    if ((u32)r0 >= (u32)0x124) {
        return;
    }
    if ((u32)r0 < (u32)0x6d) {
        r3 = r4;
        fn_8011E778();
        if ((u32)r3 == (u32)0x0) return;
    }
    if ((u32)r3 == (u32)0x0) return;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 > (u32)0x121) return;
    r4 = (u32)jumptable_8035E028;
    r0 = r0 << 2;
    r4 = (u32)jumptable_8035E028;
    r0 = *(u32*)(r4 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r4 = r31;
    fn_8011D470();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D44C();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D428();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D404();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D3E0();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D3BC();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D398();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D374();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D350();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D32C();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D308();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D2E4();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D2C0();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D2B0();
    return;
    r4 = r31 & 0xFF;
    fn_8011D2A0();
    return;
    r4 = r31 & 0xFF;
    fn_8011D290();
    return;
    r4 = r31 & 0xFF;
    fn_8011D280();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D270();
    return;
    r4 = r30;
    r5 = r31 & 0xFFFF;
    fn_8011D24C();
    return;
    r4 = r30;
    r5 = r31 & 0xFF;
    fn_8011D22C();
    return;
    r4 = r30;
    r5 = r31 & 0xFF;
    fn_8011D20C();
    return;
    r4 = r30;
    r5 = r31 & 0xFF;
    fn_8011D1CC();
    return;
    r4 = r30;
    r5 = r31 & 0xFFFF;
    fn_8011D18C();
    return;
    r4 = r30;
    r5 = r31 & 0xFFFF;
    fn_8011D14C();
    return;
    r4 = r30;
    r5 = r31 & 0xFF;
    fn_8011D10C();
    return;
    r4 = r30;
    r5 = r31 & 0xFFFF;
    fn_8011D0CC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x0;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x1;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x2;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x3;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x4;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x5;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x6;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x7;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x8;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x9;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0xa;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0xb;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0xc;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0xd;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0xe;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0xf;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x10;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x11;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x12;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x13;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x14;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x15;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x16;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x17;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x18;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x1a;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x1b;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x1c;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x1d;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x1e;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x1f;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x20;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x21;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x22;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x23;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x24;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x25;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x26;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x27;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x28;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x29;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x2a;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x2b;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x2c;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x2d;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x2e;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x2f;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x30;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x31;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x32;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x33;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x34;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x35;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x36;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x37;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x38;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x39;
    fn_8011D0AC();
    return;
    r5 = r31 & 0xFF;
    r4 = 0x3a;
    fn_8011D0AC();
    return;
    r4 = r30;
    r5 = r31;
    fn_8011D06C();
    return;
    r4 = r30;
    r5 = r31 & 0xFFFF;
    fn_8011D02C();
    return;
    r4 = r30;
    r5 = r31 & 0xFF;
    fn_8011CFEC();
    return;
    r4 = r31;
    fn_8011CFDC();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011CFCC();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011CFBC();
    return;
    r4 = r31;
    fn_8011CFAC();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011CF9C();
    return;
    r4 = r31;
    fn_8011CF70();
    return;
    r4 = r31;
    fn_8011CF44();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011CF34();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011CF24();
    return;
    r4 = r31;
    fn_8011CF14();
    return;
    r4 = r30;
    r5 = r31 & 0xFFFF;
    fn_8011CEF0();
    return;
    r4 = r30;
    r5 = r31 & 0xFF;
    fn_8011CED0();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011CEC0();
    return;
    r4 = r31 & 0xFF;
    fn_8011CEB0();
    return;
    r4 = r31 & 0xFF;
    fn_8011CEA0();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011DFF0();
    return;
    r4 = r31;
    fn_8011DFE0();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011DFD0();
    return;
    r4 = r31 & 0xFF;
    fn_8011DFC0();
    return;
    r4 = r31 & 0xFF;
    fn_8011DFB0();
    return;
    r4 = r31 & 0xFF;
    fn_8011DFA0();
    return;
    r4 = r31;
    fn_8011DF90();
    return;
    r4 = r31;
    fn_8011DF54();
    return;
    r4 = r31;
    fn_8011DEE4();
    return;
    r4 = r31;
    fn_8011DEA8();
    return;
    r4 = r31;
    fn_8011DE98();
    return;
    r4 = r31 & 0xFF;
    fn_8011DE48();
    return;
    r4 = 0x0;
    r5 = 0x7c;
    r6 = 0x0;
    fn_8012640C();
    if ((u32)r3 == (u32)0x0) return;
    r4 = *(u32*)((u8*)r31 + 0x0);
    r0 = *(u32*)((u8*)r31 + 0x4);
    *(u32*)((u8*)r3 + 0x0) = r4;
    *(u32*)((u8*)r3 + 0x4) = r0;
    r4 = *(u32*)((u8*)r31 + 0x8);
    r0 = *(u32*)((u8*)r31 + 0xC);
    *(u32*)((u8*)r3 + 0x8) = r4;
    *(u32*)((u8*)r3 + 0xC) = r0;
    return;
    r4 = r31;
    fn_8011DE38();
    return;
    r4 = r30;
    r5 = r31 & 0xFFFF;
    fn_8011DDFC();
    return;
    r4 = r30;
    r5 = r31 & 0xFF;
    fn_8011DD80();
    return;
    r4 = r30;
    r5 = r31 & 0xFF;
    fn_8011DCC4();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011DCB4();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011DC6C();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011DC48();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011DC24();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011DC00();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011DBDC();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011DBB8();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011DB94();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011DB60();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011DB2C();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011DAF8();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011DAC4();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011DA90();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011DA5C();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011DA28();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D9F4();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D9C0();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D98C();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D958();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D924();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D904();
    return;
    r4 = r31 & 0xFF;
    fn_8011D8B4();
    return;
    r4 = r31 & 0xFF;
    fn_8011D890();
    return;
    r4 = r31 & 0xFF;
    fn_8011D86C();
    return;
    r4 = r31 & 0xFF;
    fn_8011D848();
    return;
    r4 = r31 & 0xFF;
    fn_8011D824();
    return;
    r4 = r31 & 0xFF;
    fn_8011D760();
    return;
    r4 = r31 & 0xFF;
    fn_8011D73C();
    return;
    r4 = r31 & 0xFF;
    fn_8011D718();
    return;
    r4 = r31 & 0xFF;
    fn_8011D6F4();
    return;
    r4 = r31 & 0xFF;
    fn_8011D6D0();
    return;
    r4 = r31 & 0xFF;
    fn_8011D6AC();
    return;
    r4 = r31 & 0xFF;
    fn_8011D688();
    return;
    r4 = r31 & 0xFF;
    fn_8011D664();
    return;
    r4 = r31 & 0xFF;
    fn_8011D640();
    return;
    r4 = r31 & 0xFF;
    fn_8011D61C();
    return;
    r4 = r31 & 0xFF;
    fn_8011D5F8();
    return;
    r4 = r31 & 0xFF;
    fn_8011D5D4();
    return;
    r4 = r31 & 0xFF;
    fn_8011D5B0();
    return;
    r4 = r31 & 0xFF;
    fn_8011D58C();
    return;
    r4 = r31 & 0xFF;
    fn_8011D800();
    return;
    r4 = r31 & 0xFF;
    fn_8011D7DC();
    return;
    r4 = r31 & 0xFF;
    fn_8011D7B8();
    return;
    r4 = r31 & 0xFF;
    fn_8011D794();
    return;
    r4 = r31 & 0xFF;
    fn_8011D770();
    return;
    r4 = r31 & 0xFF;
    fn_8011D57C();
    return;
    r4 = r31 & 0xFF;
    fn_8011D56C();
    return;
    r4 = r31 & 0xFF;
    fn_8011D504();
    return;
    r4 = r31 & 0xFF;
    fn_8011D4F4();
    return;
    r4 = r31 & 0xFF;
    fn_8011D4E4();
    return;
    r4 = r31 & 0xFF;
    fn_8011D4D4();
    return;
    r4 = r31 & 0xFF;
    fn_8011D4C4();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D4A4();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D4B4();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D494();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011D8F4();
    return;
    r4 = r31;
    fn_8011D8D8();
    return;
    r4 = r31;
    fn_8011DE88();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011DE68();
    return;
    r4 = r31;
    fn_801FD938();
    return;
    r4 = 0x0;
    r5 = 0xcd;
    r6 = 0x0;
    fn_8012640C();
    if ((u32)r3 == (u32)0x0) return;
    r4 = *(u32*)((u8*)r31 + 0x0);
    r0 = *(u32*)((u8*)r31 + 0x4);
    *(u32*)((u8*)r3 + 0x0) = r4;
    *(u32*)((u8*)r3 + 0x4) = r0;
    r4 = *(u32*)((u8*)r31 + 0x8);
    r0 = *(u32*)((u8*)r31 + 0xC);
    *(u32*)((u8*)r3 + 0x8) = r4;
    *(u32*)((u8*)r3 + 0xC) = r0;
    return;
    r4 = (s16)r31;
    fn_801FD928();
    return;
    r4 = r31 & 0xFF;
    fn_801FD918();
    return;
    r4 = r31 & 0xFF;
    fn_801FD908();
    return;
    r4 = r31 & 0xFF;
    fn_801FD8F8();
    return;
    r4 = r31 & 0xFF;
    fn_801FD8D0();
    return;
    r4 = r31;
    fn_801FD8C0();
    return;
    r4 = r31;
    fn_801FD8B0();
    return;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    fn_8012640C();
    r7 = r31;
    r4 = 0x0;
    r5 = 0x28;
    r6 = 0x0;
    fn_8011BBD8();
    return;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    fn_8012640C();
    r7 = r31;
    r4 = 0x0;
    r5 = 0x27;
    r6 = 0x0;
    fn_8011BBD8();
    return;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    fn_8012640C();
    r7 = r31;
    r4 = 0x0;
    r5 = 0x2c;
    r6 = 0x0;
    fn_8011BBD8();
    return;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    fn_8012640C();
    r7 = r31;
    r4 = 0x0;
    r5 = 0x2b;
    r6 = 0x0;
    fn_8011BBD8();
    return;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    fn_8012640C();
    r4 = r31 & 0xFFFF;
    r30 = r3;
    fn_8020981C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x2) return;
    r3 = r30;
    r4 = r31 & 0xFFFF;
    r5 = 0x0;
    fn_802097C8();
    return;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    fn_8012640C();
    r4 = r31 & 0xFFFF;
    r30 = r3;
    if ((u32)r0 != (u32)0x2) {
        fn_8020990C();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) return;
        r3 = r30;
        r4 = r31 & 0xFFFF;
        fn_80209960();
        return;
    }
    fn_80209FAC();
    return;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    fn_8012640C();
    r7 = r31;
    r4 = 0x0;
    r5 = 0x2d;
    r6 = 0x0;
    fn_8011BBD8();
    return;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    fn_8012640C();
    r7 = r31 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BBD8();
    return;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    fn_8012640C();
    r7 = r31;
    r4 = 0x0;
    r5 = 0x29;
    r6 = 0x0;
    fn_8011BBD8();
    return;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    fn_8012640C();
    r7 = r31;
    r4 = 0x0;
    r5 = 0x2e;
    r6 = 0x0;
    fn_8011BBD8();
    return;
    r4 = r31 & 0xFF;
    fn_801FD8A0();
    return;
    r4 = r31 & 0xFF;
    fn_801FD890();
    return;
    r4 = r31 & 0xFF;
    fn_801FD880();
    return;
    r4 = r31 & 0xFF;
    fn_801FD870();
    return;
    r4 = r31 & 0xFF;
    fn_801FD860();
    return;
    r4 = r31 & 0xFF;
    fn_801FD850();
    return;
    r4 = r31 & 0xFF;
    fn_801FD840();
    return;
    r4 = r31 & 0xFFFF;
    fn_801FD820();
    return;
    r4 = r31;
    fn_801FD7F8();
    return;
    r4 = r31 & 0xFFFF;
    fn_801FCF7C();
    return;
    r4 = r31 & 0xFFFF;
    fn_801FCF6C();
    return;
    r4 = r31 & 0xFFFF;
    fn_801FCF5C();
    return;
    r4 = r31 & 0xFFFF;
    fn_801FCF4C();
    return;
    r4 = r31 & 0xFFFF;
    fn_801FCF3C();
    return;
    r4 = r31 & 0xFFFF;
    fn_801FCF2C();
    return;
    r4 = (s16)r31;
    fn_801FCF1C();
    return;
    r4 = r31 & 0xFFFF;
    fn_801FCF0C();
    return;
    r4 = r31 & 0xFFFF;
    fn_801FCEFC();
    return;
    r4 = r31 & 0xFF;
    fn_801FD728();
    return;
    r4 = r31 & 0xFFFF;
    fn_801FD718();
    return;
    r4 = r31 & 0xFFFF;
    fn_801FD708();
    return;
    r4 = r31 & 0xFF;
    fn_801FD6F8();
    return;
    r5 = (s8)r31;
    r4 = r30 & 0xFF;
    fn_801FD660();
    return;
    r4 = r30 & 0xFF;
    r5 = r31 & 0xFFFF;
    fn_801FD5F0();
    return;
    r4 = r31 & 0xFFFF;
    fn_801FD5C8();
    return;
    r4 = (s16)r31;
    fn_801FD6E8();
    return;
    r4 = r31 & 0xFFFF;
    fn_801FD6D8();
    return;
    r4 = (s16)r31;
    fn_801FD6C8();
    return;
    r4 = r31 & 0xFFFF;
    fn_801FD6B8();
    return;
    r4 = r31 & 0xFF;
    fn_801FD330();
    return;
    r4 = r31 & 0xFF;
    fn_801FD320();
    return;
    r4 = r31 & 0xFF;
    fn_801FD310();
    return;
    r4 = r31 & 0xFF;
    fn_801FD300();
    return;
    r4 = r31 & 0xFF;
    fn_801FD2F0();
    return;
    r4 = r31 & 0xFF;
    fn_801FD2E0();
    return;
    r4 = r31 & 0xFF;
    fn_801FD2D0();
    return;
    r4 = r31 & 0xFF;
    fn_801FD2C0();
    return;
    r4 = r31 & 0xFF;
    fn_801FD2B0();
    return;
    r4 = r31 & 0xFF;
    fn_801FD2A0();
    return;
    r4 = r31 & 0xFF;
    fn_801FD290();
    return;
    r4 = r31 & 0xFF;
    fn_801FD280();
    return;
    r4 = r31 & 0xFF;
    fn_801FD270();
    return;
    r4 = r31 & 0xFF;
    fn_801FD260();
    return;
    r4 = r31 & 0xFF;
    fn_801FD250();
    return;
    r4 = r31 & 0xFF;
    fn_801FD240();
    return;
    r4 = r31 & 0xFF;
    fn_801FD230();
    return;
    r4 = r31 & 0xFF;
    fn_801FD220();
    return;
    r4 = r31 & 0xFF;
    fn_801FD210();
    return;
    r4 = r31 & 0xFF;
    fn_801FD200();
    return;
    r4 = r31 & 0xFF;
    fn_801FD1F0();
    return;
    r4 = r31;
    fn_801FD1E0();
    return;
    r4 = (s16)r31;
    fn_801FD1D0();
    return;
    r4 = r31 & 0xFFFF;
    fn_801FD1C0();
    return;
    r4 = (s16)r31;
    fn_801FD1B0();
    return;
    r4 = r31 & 0xFFFF;
    fn_801FD1A0();
    return;
    r4 = r31 & 0xFF;
    fn_801FD178();
    return;
    r4 = (s16)r31;
    fn_801FD150();
    return;
}
/* ==================================================================
 * fn_80124A60 -- Field object batch update
 *
 * 2008 bytes. Updates all active field objects in a single pass.
 * ================================================================== */
void GSfield_ObjectBatchUpdate(void) {
    extern void fn_8011B950();
    extern void fn_8011D480();
    extern void fn_801254B4();
    extern void fn_8012640C();
    extern void fn_80135708();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = 0x0;
    *(u16*)(sp + 0x8) = r0;
    if ((s32)r0 != (s32)0) {
        r4 = 0x0;
        r5 = 0xc9;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xc3;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        if ((u32)r30 != (u32)0x0) {
            r3 = r30;
            r4 = 0x0;
            r5 = 0xc5;
            r6 = 0x0;
            r7 = -0x64;
            fn_801254B4();
        }
        r3 = r30;
        r4 = 0x0;
        r5 = 0xc6;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xc7;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xc8;
        r6 = 0x0;
        fn_8012640C();
        r4 = 0x1;
        fn_8011B950();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x6e;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x6f;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x70;
        r6 = 0x0;
        fn_8012640C();
        fn_80135708();
        if ((u32)r30 != (u32)0x0) {
            r3 = r30;
            r4 = 0x0;
            r5 = 0x71;
            r6 = 0x0;
            r7 = 0x0;
            fn_801254B4();
            r3 = r30;
            r4 = 0x0;
            r5 = 0x72;
            r6 = 0x0;
            r7 = 0x0;
            fn_801254B4();
            r3 = r30;
            r4 = 0x0;
            r5 = 0x73;
            r6 = 0x0;
            r7 = 0x0;
            fn_801254B4();
            r3 = r30;
            r4 = 0x0;
            r5 = 0x74;
            r6 = 0x0;
            r7 = 0x2;
            fn_801254B4();
            r3 = r30;
            r4 = 0x0;
            r5 = 0x75;
            r6 = 0x0;
            r7 = 0x0;
            fn_801254B4();
            r3 = r30;
            r7 = (u32)sp + 0x8;
            r4 = 0x0;
            r5 = 0x76;
            r6 = 0x0;
            fn_801254B4();
        }
        r3 = r30;
        r7 = (u32)sp + 0x8;
        r4 = 0x0;
        r5 = 0x77;
        r6 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x79;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x7a;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x7c;
        r6 = 0x0;
        fn_8012640C();
        r4 = 0x1;
        fn_8011B950();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x7d;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        if ((u32)r30 != (u32)0x0) {
            r31 = 0x0;
            while (r0 = r31 & 0xFFFF, (u32)r0 < (u32)0x4) {

            if ((u32)r30 != (u32)0x0) {
                r3 = r30;
                r6 = r31;
                r4 = 0x0;
                r5 = 0x7f;
                r7 = 0x0;
                fn_801254B4();
                r3 = r30;
                r6 = r31;
                r4 = 0x0;
                r5 = 0x80;
                r7 = 0x0;
                fn_801254B4();
                r3 = r30;
                r6 = r31;
                r4 = 0x0;
                r5 = 0x81;
                r7 = 0x0;
                fn_801254B4();
            }
            r31 = r31 + 0x1;
            }

        }
        if ((u32)r30 != (u32)0x0) {
            r3 = r30;
            r4 = 0x0;
            r5 = 0x82;
            r6 = 0x0;
            fn_8012640C();
            r3 = r30;
            r4 = 0x0;
            r5 = 0x82;
            r6 = 0x0;
            r7 = 0x0;
            fn_801254B4();
        }
        r3 = r30;
        r4 = 0x0;
        r5 = 0x83;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x87;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x88;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x89;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x8a;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x8b;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x8c;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x8d;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x8e;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x8f;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x90;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x91;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x92;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x93;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x94;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x95;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x96;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x97;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x98;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x99;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x9c;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x9d;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x9e;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x9f;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xa0;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xa1;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xa3;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xa4;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xa5;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xa6;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xa7;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xa8;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xa9;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xaa;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xab;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xac;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xad;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xae;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xaf;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xb0;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xb1;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xb2;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xb3;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xb4;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xb5;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xb6;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xb7;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xb8;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xb9;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xbb;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xbc;
        r6 = 0x0;
        r7 = 0xff;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xbd;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xbe;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        fn_8011D480();
    }
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    return;
}
/* ==================================================================
 * fn_8012CA84 -- Field event/trigger processing
 *
 * 2104 bytes. Processes field event triggers (door warps, NPC
 * interaction zones, item pickups, etc.).
 * ================================================================== */
void GSfield_ProcessTriggers(void) {
    extern u8 lbl_80478AC0[];
    extern u8 lbl_8047D030[];
    extern u8 lbl_8047D034[];
    extern u8 lbl_8047D038[];
    extern u8 lbl_8047D048[];
    extern u8 lbl_8047D050[];
    extern u8 lbl_8047D058[];
    extern u8 lbl_8047D060[];
    extern u8 lbl_8047D068[];
    extern u8 lbl_8047D080[];
    extern u8 lbl_8047D094[];
    extern u8 lbl_8047D098[];
    extern u8 lbl_8047D09C[];
    extern u8 lbl_8047D0A0[];
    extern u8 lbl_8047D0A4[];
    extern void fn_800A3AC0();
    extern void fn_800A3B7C();
    extern void fn_800CE2D8();
    extern void fn_800D3088();
    extern void fn_800E3D6C();
    extern void fn_800E3D98();
    extern void fn_800F9318();
    extern void fn_8012C660();
    extern void fn_8018790C();
    extern void fn_8018805C();
    extern void fn_801885C4();
    extern void fn_801887D8();
    extern void fn_8018D928();
    extern void fn_8018D998();
    extern void fn_8018F658();
    extern void fn_8018F678();
    extern void fn_8018F6F4();
    extern u8 lbl_80426BD0;
    u8 sp[0x120];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    *(f64*)(sp + 0x110) = f31;
    /* psq_st f31, 0x118((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x100) = f30;
    /* psq_st f30, 0x108((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0xF0) = f29;
    /* psq_st f29, 0xf8((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0xE0) = f28;
    /* psq_st f28, 0xe8((u32)sp), 0, qr0 */;
    r23 = r3;
    r24 = r4;
    r25 = r5;
    fn_800D3088();
    r5 = (0x4330 << 16);
    r4 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    f1 = *(f64*)lbl_8047D068;
    f0 = *(f64*)(sp + 0xA8);
    f29 = f0 - f1;
    *(u32*)(sp + 0x4C) = r0;
    if (((s32)r23 >= (s32)0x0) && ((s32)r23 < (s32)0x2)) {
        r0 = r23 << 2;
        r3 = (u32)sp + 0x48;
        r26 = *(u32*)(r3 + r0);
    }
    f0 = *(f32*)((u8*)r24 + 0x0);
    f1 = *(f32*)((u8*)r24 + 0x8);
    f2 = f0 * f0;
    f0 = *(f32*)lbl_8047D038;
    f1 = f1 * f1;
    f31 = f2 + f1;
    if (f31 > f0) {
        /* frsqrte f1, f31 */;
        f3 = *(f64*)lbl_8047D048;
        f2 = *(f64*)lbl_8047D050;
        f0 = f1 * f1;
        f1 = f3 * f1;
        f0 = -(f31 * f0 - f2);
        f1 = f1 * f0;
        f0 = f1 * f1;
        f1 = f3 * f1;
        f0 = -(f31 * f0 - f2);
        f1 = f1 * f0;
        f0 = f1 * f1;
        f1 = f3 * f1;
        f0 = -(f31 * f0 - f2);
        f0 = f1 * f0;
        f31 = f31 * f0;
        f31 = (f32)f31;
        goto L_8012CBF4;
    }
    f0 = *(f64*)lbl_8047D058;
    if (f31 < f0) {
        r3 = (u32)lbl_80478AC0;
        f31 = *(f32*)lbl_80478AC0;
        goto L_8012CBF4;
    }
    *(f32*)(sp + 0xC) = f31;
    r0 = (0x7f80 << 16);
    r4 = *(u32*)(sp + 0xC);
    r3 = r4 & 0x7F800000;
    if ((s32)r3 != (s32)r0) {
        if ((s32)r3 < (s32)r0) {
            if ((s32)r3 == (s32)0x0) goto L_8012CBC8;
        }
        goto L_8012CBE0;
    }
    r0 = r4 & 0x7FFFFF;
    if ((s32)r3 != (s32)0x0) {
        r0 = 0x1;
        goto L_8012CBE4;
    }
    r0 = 0x2;
    goto L_8012CBE4;
L_8012CBC8: ;
    r0 = r4 & 0x7FFFFF;
    if ((s32)r3 != (s32)0x0) {
        r0 = 0x5;
        goto L_8012CBE4;
    }
    r0 = 0x3;
    goto L_8012CBE4;
L_8012CBE0: ;
    r0 = 0x4;
L_8012CBE4: ;
    if ((s32)r0 == (s32)0x1) {
        r3 = (u32)lbl_80478AC0;
        f31 = *(f32*)lbl_80478AC0;
    }
L_8012CBF4: ;
    f0 = *(f32*)lbl_8047D038;
    if (f31 > f0) {
        r3 = *(u32*)lbl_8047D030;
        r0 = *(u32*)lbl_8047D034;
        *(u32*)(sp + 0x3C) = r0;
        if (((s32)r23 >= (s32)0x0) && ((s32)r23 < (s32)0x2)) {
            r0 = r23 << 2;
            r3 = (u32)sp + 0x38;
            r31 = *(u32*)(r3 + r0);
        }
        r4 = r31;
        r3 = 0x0;
        fn_800F9318();
        r4 = (u32)sp + 0x5c;
        fn_800E3D98();
        f0 = *(f32*)lbl_8047D038;
        r4 = r26;
        r5 = r24;
        r3 = 0x0;
        *(f32*)(sp + 0x60) = f0;
        r6 = 0x0;
        fn_801885C4();
        r3 = *(u32*)lbl_8047D030;
        r0 = *(u32*)lbl_8047D034;
        *(u32*)(sp + 0x34) = r0;
        if (((s32)r23 >= (s32)0x0) && ((s32)r23 < (s32)0x2)) {
            r0 = r23 << 2;
            r3 = (u32)sp + 0x30;
            r30 = *(u32*)(r3 + r0);
        }
        r4 = r30;
        r3 = 0x0;
        fn_800F9318();
        r4 = (u32)sp + 0x50;
        fn_800E3D98();
        f0 = *(f32*)lbl_8047D038;
        r3 = (u32)sp + 0x50;
        r4 = (u32)sp + 0x5c;
        r5 = (u32)sp + 0x68;
        *(f32*)(sp + 0x54) = f0;
        ((void(*)(void))fn_800A3A9C)();
        f30 = f31 / f29;
        f0 = *(f32*)lbl_8047D080;
        if (f30 > f0) {
            f30 = f0;
        }
        f1 = f30 / f31;
        r3 = (u32)sp + 0x68;
        r4 = (u32)sp + 0x74;
        fn_800A3AC0();
        f0 = *(f32*)((u8*)r25 + 0x0);
        f1 = *(f32*)((u8*)r25 + 0x8);
        f2 = f0 * f0;
        f0 = *(f32*)lbl_8047D038;
        f1 = f1 * f1;
        f4 = f2 + f1;
        if (f4 > f0) {
            /* frsqrte f1, f4 */;
            f3 = *(f64*)lbl_8047D048;
            f2 = *(f64*)lbl_8047D050;
            f0 = f1 * f1;
            f1 = f3 * f1;
            f0 = -(f4 * f0 - f2);
            f1 = f1 * f0;
            f0 = f1 * f1;
            f1 = f3 * f1;
            f0 = -(f4 * f0 - f2);
            f1 = f1 * f0;
            f0 = f1 * f1;
            f1 = f3 * f1;
            f0 = -(f4 * f0 - f2);
            f0 = f1 * f0;
            f4 = f4 * f0;
            f4 = (f32)f4;
            goto L_8012CDC4;
        }
        f0 = *(f64*)lbl_8047D058;
        if (f4 < f0) {
            r3 = (u32)lbl_80478AC0;
            f4 = *(f32*)lbl_80478AC0;
            goto L_8012CDC4;
        }
        *(f32*)(sp + 0x8) = f4;
        r0 = (0x7f80 << 16);
        r4 = *(u32*)(sp + 0x8);
        r3 = r4 & 0x7F800000;
        if ((s32)r3 != (s32)r0) {
            if ((s32)r3 < (s32)r0) {
                if ((s32)r3 == (s32)0x0) goto L_8012CD98;
            }
            goto L_8012CDB0;
        }
        r0 = r4 & 0x7FFFFF;
        if ((s32)r3 != (s32)0x0) {
            r0 = 0x1;
            goto L_8012CDB4;
        }
        r0 = 0x2;
        goto L_8012CDB4;
    L_8012CD98: ;
        r0 = r4 & 0x7FFFFF;
        if ((s32)r3 != (s32)0x0) {
            r0 = 0x5;
            goto L_8012CDB4;
        }
        r0 = 0x3;
        goto L_8012CDB4;
    L_8012CDB0: ;
        r0 = 0x4;
    L_8012CDB4: ;
        if ((s32)r0 == (s32)0x1) {
            r3 = (u32)lbl_80478AC0;
            f4 = *(f32*)lbl_80478AC0;
        }
    L_8012CDC4: ;
        f0 = *(f32*)lbl_8047D060;
        if (f4 > f0) {
            f1 = *(f32*)((u8*)r25 + 0x0);
            f2 = *(f32*)((u8*)r25 + 0x8);
            fn_800CE2D8();
            f1 = (f32)f1;
            r4 = r26;
            f2 = f30;
            r3 = 0x0;
            fn_8018805C();
        }
        r4 = r26;
        r5 = (u32)sp + 0x74;
        r3 = 0x0;
        fn_801887D8();
        f28 = f1;
        r4 = r25;
        r3 = (u32)sp + 0x68;
        fn_800A3B7C();
        f0 = *(f32*)lbl_8047D038;
        if (f1 < f0) {
            f28 = -f28;
        }
        goto L_8012D244;
    }
    if ((s32)r23 >= (s32)0x0) {
        if ((s32)r23 < (s32)0x2) goto L_8012CE3C;
    }
    r0 = 0x0;
    goto L_8012CE64;
L_8012CE3C: ;
    r3 = (u32)&lbl_80426BD0;
    r0 = r23 << 5;
    r3 = (u32)&lbl_80426BD0;
    r3 = r3 + r0;
    r0 = *(u16*)((u8*)r3 + 0x4);
    r0 = r0 & 0x1;
    if ((s32)r23 == (s32)0x2) {
        r0 = 0x0;
        goto L_8012CE64;
    }
    r0 = 0x1;
L_8012CE64: ;
    r0 = r0 & 0xFF;
    if ((s32)r23 != (s32)0x2) {
        r3 = (u32)&lbl_80426BD0;
        r0 = r23 << 5;
        r3 = (u32)&lbl_80426BD0;
        r3 = r3 + r0;
        r0 = *(u32*)((u8*)r3 + 0xC);
    } else {
        r0 = 0x2;
    }
    if ((s32)r0 != (s32)0x1) {
        r4 = r26;
        r3 = 0x0;
        fn_8018790C();
        f28 = *(f32*)lbl_8047D038;
        goto L_8012D244;
    }
    r4 = (u32)&lbl_80426BD0;
    r3 = *(u32*)lbl_8047D030;
    r4 = (u32)&lbl_80426BD0;
    r0 = *(u32*)lbl_8047D034;
    r4 = *(u32*)((u8*)r4 + 0x0);
    *(u32*)(sp + 0x2C) = r0;
    if (((s32)r4 >= (s32)0x0) && ((s32)r4 < (s32)0x2)) {
        r0 = r4 << 2;
        r3 = (u32)sp + 0x28;
        r30 = *(u32*)(r3 + r0);
    }
    r4 = r30;
    r3 = 0x0;
    fn_800F9318();
    r4 = (u32)sp + 0x8c;
    fn_800E3D98();
    r3 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0x24) = r0;
    if (((s32)r23 >= (s32)0x0) && ((s32)r23 < (s32)0x2)) {
        r0 = r23 << 2;
        r3 = (u32)sp + 0x20;
        r29 = *(u32*)(r3 + r0);
    }
    r4 = r29;
    r3 = 0x0;
    fn_800F9318();
    r4 = (u32)sp + 0x80;
    fn_800E3D98();
    r3 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0x1C) = r0;
    if (((s32)r23 >= (s32)0x0) && ((s32)r23 < (s32)0x2)) {
        r0 = r23 << 2;
        r3 = (u32)sp + 0x18;
        r28 = *(u32*)(r3 + r0);
    }
    r4 = r28;
    r3 = 0x0;
    fn_800F9318();
    r4 = (u32)sp + 0x98;
    fn_800E3D6C();
    f4 = *(f32*)(sp + 0x9C);
    f0 = *(f32*)lbl_8047D094;
    goto L_8012CF80;
L_8012CF7C: ;
    f4 = f4 - f0;
L_8012CF80: ;
    /* cror eq, gt, eq */;
    if (f4 == f0) goto L_8012CF7C;
    f1 = *(f32*)lbl_8047D094;
    f0 = *(f32*)lbl_8047D098;
    goto L_8012CF9C;
L_8012CF98: ;
    f4 = f4 + f1;
L_8012CF9C: ;
    /* cror eq, lt, eq */;
    if (f4 == f0) goto L_8012CF98;
    f3 = *(f32*)(sp + 0x8C);
    f1 = *(f32*)(sp + 0x80);
    f2 = *(f32*)(sp + 0x94);
    f0 = *(f32*)(sp + 0x88);
    f1 = f3 - f1;
    *(f32*)(sp + 0x9C) = f4;
    f2 = f2 - f0;
    fn_800CE2D8();
    f28 = (f32)f1;
    f1 = *(f32*)(sp + 0x9C);
    f0 = *(f32*)lbl_8047D09C;
    f31 = f28 - f1;
    if (f31 < f0) {
        f0 = *(f32*)lbl_8047D094;
        f31 = f31 + f0;
        goto L_8012D000;
    }
    f0 = *(f32*)lbl_8047D0A0;
    if (f31 > f0) {
        f0 = *(f32*)lbl_8047D094;
        f31 = f31 - f0;
    }
L_8012D000: ;
    r3 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0x44) = r0;
    if ((s32)r23 >= (s32)0x0) {
        if ((s32)r23 < (s32)0x2) goto L_8012D028;
    }
    r0 = 0x0;
    goto L_8012D038;
L_8012D028: ;
    r0 = r23 << 2;
    r3 = (u32)sp + 0x40;
    r31 = *(u32*)(r3 + r0);
    r0 = 0x1;
L_8012D038: ;
    r0 = r0 & 0xFF;
    if ((s32)r23 == (s32)0x2) {
        r3 = -0x1;
        goto L_8012D06C;
    }
    r4 = r31;
    r3 = 0x0;
    fn_8018D998();
    fn_8018D928();
    if ((u32)r3 == (u32)0x0) {
        r3 = -0x1;
        goto L_8012D06C;
    }
    r3 = *(u32*)((u8*)r3 + 0x30);
L_8012D06C: ;
    fn_8018F6F4();
    r24 = r3;
    fn_8018F678();
    f0 = *(f32*)lbl_8047D038;
    if (f1 > f0) {
        r3 = r24;
        fn_8018F678();
        f29 = f1;
    } else {
        r3 = r24;
        fn_8018F678();
        f29 = -f1;
    }
    r3 = r24;
    fn_8018F658();
    f0 = *(f32*)lbl_8047D038;
    if (f1 > f0) {
        r3 = r24;
        fn_8018F658();
    } else {
        r3 = r24;
        fn_8018F658();
        f1 = -f1;
    }
    f0 = *(f32*)lbl_8047D038;
    r3 = 0x0;
    if (f31 < f0) {
        if (f31 > f0) {
        } else {
            f31 = -f31;
        }
        if (f31 > f1) {
            f30 = f28 + f1;
            f0 = *(f32*)lbl_8047D094;
            /* cror eq, gt, eq */;
            if (f30 == f0) {
                f30 = f30 - f0;
            }
            r3 = 0x1;
        }
        goto L_8012D150;
    }
    if (f31 > f29) {
        f1 = f28 - f29;
        f0 = *(f32*)lbl_8047D09C;
        if (f1 < f0) {
            f0 = *(f32*)lbl_8047D094;
            f1 = f1 + f0;
            goto L_8012D148;
        }
        f0 = *(f32*)lbl_8047D0A0;
        if (f1 > f0) {
            f0 = *(f32*)lbl_8047D094;
            f1 = f1 - f0;
    }
    L_8012D148: ;
        f30 = f1;
        r3 = 0x1;
    }
L_8012D150: ;
    r0 = r3 & 0xFF;
    if (f1 != f0) {
        f3 = *(f32*)(sp + 0x9C);
        f0 = *(f32*)lbl_8047D09C;
        f2 = f30 - f3;
        f1 = f2;
        if (f2 < f0) {
            f0 = *(f32*)lbl_8047D094;
            f1 = f2 + f0;
            goto L_8012D190;
        }
        f0 = *(f32*)lbl_8047D0A0;
        if (f2 > f0) {
            f0 = *(f32*)lbl_8047D094;
            f1 = f2 - f0;
        }
    L_8012D190: ;
        f0 = *(f32*)lbl_8047D038;
        if (f1 > f0) {
            f0 = *(f32*)lbl_8047D09C;
            if (f2 < f0) {
                f0 = *(f32*)lbl_8047D094;
                f2 = f2 + f0;
                goto L_8012D1FC;
            }
            f0 = *(f32*)lbl_8047D0A0;
            if (f2 > f0) {
                f0 = *(f32*)lbl_8047D094;
                f2 = f2 - f0;
            }
            goto L_8012D1FC;
        }
        f0 = *(f32*)lbl_8047D09C;
        if (f2 < f0) {
            f0 = *(f32*)lbl_8047D094;
            f2 = f2 + f0;
            goto L_8012D1F8;
        }
        f0 = *(f32*)lbl_8047D0A0;
        if (f2 > f0) {
            f0 = *(f32*)lbl_8047D094;
            f2 = f2 - f0;
        }
    L_8012D1F8: ;
        f2 = -f2;
    L_8012D1FC: ;
        f0 = *(f32*)lbl_8047D0A4;
        if (f2 < f0) {
            f30 = f3;
            r3 = 0x0;
        }
    }
    r0 = r3 & 0xFF;
    if (f2 != f0) {
        f1 = f30;
        f2 = *(f32*)lbl_8047D080;
        r4 = r26;
        r3 = 0x0;
        fn_8018805C();
        f28 = *(f32*)lbl_8047D080;
        goto L_8012D244;
    }
    r4 = r26;
    r3 = 0x0;
    fn_8018790C();
    f28 = *(f32*)lbl_8047D038;
L_8012D244: ;
    r3 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0x14) = r0;
    if (((s32)r23 >= (s32)0x0) && ((s32)r23 < (s32)0x2)) {
        r0 = r23 << 2;
        r3 = (u32)sp + 0x10;
        r27 = *(u32*)(r3 + r0);
    }
    r4 = r27;
    r3 = 0x0;
    fn_800F9318();
    f1 = f28;
    r4 = r23;
    fn_8012C660();
    /* psq_l f31, 0x118((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x110);
    /* psq_l f30, 0x108((u32)sp), 0, qr0 */;
    f30 = *(f64*)(sp + 0x100);
    /* psq_l f29, 0xf8((u32)sp), 0, qr0 */;
    f29 = *(f64*)(sp + 0xF0);
    /* psq_l f28, 0xe8((u32)sp), 0, qr0 */;
    f28 = *(f64*)(sp + 0xE0);
    return;
}
/* ==================================================================
 * Small accessor functions -- field object getters
 *
 * The 0x8011E4A4-0x8011E800 range contains many tiny (0x18-0x34 byte)
 * functions that read specific fields from a field object struct.
 * Pattern: null check, then load from fixed offset and return.
 *
 * Example (fn_8011E4D8, 0x18 bytes):
 *   cmplwi r3, 0x0
 *   bne .ok
 *   li r3, 0x0
 *   blr
 *   .ok:
 *   lhz r3, 0x8(r3)    ; read u16 at offset 0x8
 *   blr
 * ================================================================== */
/* fn_8011E4A4 */ u16 GSfield_GetObjAttrU16(void* obj, u16 slot) {
    if (obj == NULL) { return 0; }
    if (slot >= 2) { return 0; }
    return *(u16*)((u8*)obj + slot * 2 + 0x70);
}
/* fn_8011E4D8 */ u16 GSfield_GetObjType(void* obj) {
    if (obj == NULL) { return 0; }
    return *(u16*)((u8*)obj + 0x8);
}
/* fn_8011E4F0 */ u8 GSfield_GetObjSubtype(void* obj) {
    if (obj == NULL) { return 0; }
    return *(u8*)((u8*)obj + 0x2);
}
/* fn_8011E508 */ u8 GSfield_GetObjFlags(void* obj) {
    if (obj == NULL) { return 0; }
    return *(u8*)((u8*)obj + 0x1);
}
/* fn_8011E520 */ u8 GSfield_GetObjState(void* obj) {
    if (obj == NULL) { return 0; }
    return *(u8*)((u8*)obj + 0x0);
}
/* fn_8011E538 */ u16 GSfield_GetObjGroupId(void* obj) {
    if (obj == NULL) { return 0; }
    return *(u16*)((u8*)obj + 0x6);
}
/* fn_8011E550 */ u16 GSfield_GetObjRegionId(void* obj) {
    u8* sub;
    if (obj == NULL) { return 0; }
    sub = (u8*)obj + 0x90;
    if (sub == NULL) { return 0; }
    return *(u16*)(sub + 0xA);
}
/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 208 functions matched
 * =================================================================== */
extern u8 lbl_8047AD71;
extern u32 lbl_8047ADC0;
/* Address: 0x801174EC | Size: 0x8 | Pattern: sda_getter */
u8 fn_801174EC(void) {
    return lbl_8047AD71;
}
/* Address: 0x80118068 | Size: 0x8 | Pattern: return_constant */
u32 fn_80118068(void) { return 0; }
/* Address: 0x80119F90 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_80119F90(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xC]) = val;
}
/* Address: 0x80119FA0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_80119FA0(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x8]) = val;
}
/* Address: 0x80119FB0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_80119FB0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x6]) = val;
}
/* Address: 0x80119FC0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_80119FC0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x5]) = val;
}
/* Address: 0x80119FD0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_80119FD0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x4]) = val;
}
/* Address: 0x80119FE0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_80119FE0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x2]) = val;
}
/* Address: 0x80119FF0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_80119FF0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x0]) = val;
}
/* Address: 0x8011A000 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011A000(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xC]);
}
/* Address: 0x8011A018 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011A018(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x8]);
}
/* Address: 0x8011A030 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011A030(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x6]);
}
/* Address: 0x8011A048 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011A048(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x5]);
}
/* Address: 0x8011A060 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011A060(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x4]);
}
/* Address: 0x8011A078 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011A078(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x2]);
}
/* Address: 0x8011A090 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011A090(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}
/* Address: 0x8011C420 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C420(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x13]) = val;
}
/* Address: 0x8011C570 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C570(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x13]);
}
/* Address: 0x8011C5B8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C5B8(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x12]) = val;
}
/* Address: 0x8011C5C8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C5C8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x12]);
}
/* Address: 0x8011C5E0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C5E0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x15]) = val;
}
/* Address: 0x8011C5F0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C5F0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x14]) = val;
}
/* Address: 0x8011C600 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C600(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x2C]) = val;
}
/* Address: 0x8011C610 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C610(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x30]) = val;
}
/* Address: 0x8011C620 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C620(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x11]) = val;
}
/* Address: 0x8011C630 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C630(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x10]) = val;
}
/* Address: 0x8011C640 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C640(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xF]) = val;
}
/* Address: 0x8011C650 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C650(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xE]) = val;
}
/* Address: 0x8011C660 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C660(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xD]) = val;
}
/* Address: 0x8011C670 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C670(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xC]) = val;
}
/* Address: 0x8011C680 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C680(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xB]) = val;
}
/* Address: 0x8011C690 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C690(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA]) = val;
}
/* Address: 0x8011C6A0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C6A0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x9]) = val;
}
/* Address: 0x8011C6B0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C6B0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x8]) = val;
}
/* Address: 0x8011C6C0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C6C0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x7]) = val;
}
/* Address: 0x8011C6D0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C6D0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x6]) = val;
}
/* Address: 0x8011C6E0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C6E0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x5]) = val;
}
/* Address: 0x8011C6F0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C6F0(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x28]) = val;
}
/* Address: 0x8011C700 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C700(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x1A]) = val;
}
/* Address: 0x8011C710 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C710(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x18]) = val;
}
/* Address: 0x8011C720 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C720(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x16]) = val;
}
/* Address: 0x8011C730 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C730(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x4]) = val;
}
/* Address: 0x8011C740 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C740(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x3]) = val;
}
/* Address: 0x8011C750 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C750(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x0]) = val;
}
/* Address: 0x8011C760 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C760(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x2]) = val;
}
/* Address: 0x8011C770 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C770(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x1]) = val;
}
/* Address: 0x8011C780 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C780(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x20]) = val;
}
/* Address: 0x8011C790 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C790(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x15]);
}
/* Address: 0x8011C7A8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C7A8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x14]);
}
/* Address: 0x8011C7C0 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011C7C0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x2C]);
}
/* Address: 0x8011C7D8 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011C7D8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x30]);
}
/* Address: 0x8011C7F0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C7F0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x11]);
}
/* Address: 0x8011C808 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C808(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x10]);
}
/* Address: 0x8011C820 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C820(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xF]);
}
/* Address: 0x8011C838 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C838(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xE]);
}
/* Address: 0x8011C850 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C850(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xD]);
}
/* Address: 0x8011C868 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C868(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xC]);
}
/* Address: 0x8011C880 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C880(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xB]);
}
/* Address: 0x8011C898 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C898(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA]);
}
/* Address: 0x8011C8B0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C8B0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x9]);
}
/* Address: 0x8011C8C8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C8C8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x8]);
}
/* Address: 0x8011C8E0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C8E0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x7]);
}
/* Address: 0x8011C8F8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C8F8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x6]);
}
/* Address: 0x8011C910 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C910(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x5]);
}
/* Address: 0x8011C928 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011C928(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x28]);
}
/* Address: 0x8011C940 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011C940(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x24]);
}
/* Address: 0x8011C958 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011C958(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x1A]);
}
/* Address: 0x8011C970 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011C970(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x18]);
}
/* Address: 0x8011C988 | Size: 0x18 | Pattern: nullcheck_getter */
s16 fn_8011C988(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(s16*)(&ptr[0x16]);
}
/* Address: 0x8011C9A0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C9A0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x4]);
}
/* Address: 0x8011C9B8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C9B8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x3]);
}
/* Address: 0x8011C9D0 | Size: 0x1C | Pattern: nullcheck_getter_s8 */
s32 fn_8011C9D0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x0];
}
/* Address: 0x8011C9EC | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C9EC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x2]);
}
/* Address: 0x8011CA04 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CA04(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x1]);
}
/* Address: 0x8011CA1C | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011CA1C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x20]);
}
/* Address: 0x8011CA9C | Size: 0x1C | Pattern: nullcheck_getter_s8 */
s32 fn_8011CA9C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x0];
}
/* Address: 0x8011CB3C | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011CB3C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x8]);
}
/* Address: 0x8011CB54 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011CB54(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}
/* Address: 0x8011CB98 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CB98(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x1]);
}
/* Address: 0x8011CBB0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CBB0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}
/* Address: 0x8011CC54 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CC54(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x11]);
}
/* Address: 0x8011CC6C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CC6C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x10]);
}
/* Address: 0x8011CC84 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CC84(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xF]);
}
/* Address: 0x8011CC9C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CC9C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xE]);
}
/* Address: 0x8011CCB4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CCB4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xD]);
}
/* Address: 0x8011CCCC | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CCCC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xC]);
}
/* Address: 0x8011CCE4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CCE4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xB]);
}
/* Address: 0x8011CCFC | Size: 0x1C | Pattern: nullcheck_getter_s8 */
s32 fn_8011CCFC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x4];
}
/* Address: 0x8011CD18 | Size: 0x1C | Pattern: nullcheck_getter_s8 */
s32 fn_8011CD18(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x3];
}
/* Address: 0x8011CD34 | Size: 0x1C | Pattern: nullcheck_getter_s8 */
s32 fn_8011CD34(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x2];
}
/* Address: 0x8011CD50 | Size: 0x1C | Pattern: nullcheck_getter_s8 */
s32 fn_8011CD50(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x1];
}
/* Address: 0x8011CD6C | Size: 0x1C | Pattern: nullcheck_getter_s8 */
s32 fn_8011CD6C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x0];
}
/* Address: 0x8011CD88 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CD88(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x9]);
}
/* Address: 0x8011CDA0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CDA0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x8]);
}
/* Address: 0x8011CDB8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CDB8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x7]);
}
/* Address: 0x8011CDD0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CDD0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x6]);
}
/* Address: 0x8011CDE8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CDE8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x5]);
}
/* Address: 0x8011CE00 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011CE00(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x14]);
}
/* Address: 0x8011CEA0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CEA0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x4]) = val;
}
/* Address: 0x8011CEB0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CEB0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x3]) = val;
}
/* Address: 0x8011CEC0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CEC0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x16]) = val;
}
/* Address: 0x8011CF14 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CF14(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x2C]) = val;
}
/* Address: 0x8011CF24 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CF24(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x12]) = val;
}
/* Address: 0x8011CF34 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CF34(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x14]) = val;
}
/* Address: 0x8011CF9C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CF9C(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xE]) = val;
}
/* Address: 0x8011CFAC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CFAC(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x20]) = val;
}
/* Address: 0x8011CFBC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CFBC(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xC]) = val;
}
/* Address: 0x8011CFCC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CFCC(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xA]) = val;
}
/* Address: 0x8011CFDC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CFDC(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x1C]) = val;
}
/* Address: 0x8011D270 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D270(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x8]) = val;
}
/* Address: 0x8011D280 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D280(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x2]) = val;
}
/* Address: 0x8011D290 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D290(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x1]) = val;
}
/* Address: 0x8011D2A0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D2A0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x0]) = val;
}
/* Address: 0x8011D2B0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D2B0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x6]) = val;
}
/* Address: 0x8011D470 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D470(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x18]) = val;
}
/* Address: 0x8011D494 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D494(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xD6]) = val;
}
/* Address: 0x8011D4A4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D4A4(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xD2]) = val;
}
/* Address: 0x8011D4B4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D4B4(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xD4]) = val;
}
/* Address: 0x8011D4C4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D4C4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xD0]) = val;
}
/* Address: 0x8011D4D4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D4D4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xCF]) = val;
}
/* Address: 0x8011D4E4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D4E4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xCE]) = val;
}
/* Address: 0x8011D4F4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D4F4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xCD]) = val;
}
/* Address: 0x8011D56C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D56C(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xCB]) = val;
}
/* Address: 0x8011D57C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D57C(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xCA]) = val;
}
/* Address: 0x8011D760 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D760(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xBC]) = val;
}
/* Address: 0x8011D8F4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D8F4(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xD8]) = val;
}
/* Address: 0x8011DCB4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DCB4(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x88]) = val;
}
/* Address: 0x8011DE38 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DE38(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x74]) = val;
}
/* Address: 0x8011DE88 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DE88(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0xE0]) = val;
}
/* Address: 0x8011DE98 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DE98(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x5C]) = val;
}
/* Address: 0x8011DF90 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DF90(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x14]) = val;
}
/* Address: 0x8011DFA0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DFA0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x10]) = val;
}
/* Address: 0x8011DFB0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DFB0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xF]) = val;
}
/* Address: 0x8011DFC0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DFC0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xE]) = val;
}
/* Address: 0x8011DFD0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DFD0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xC]) = val;
}
/* Address: 0x8011DFE0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DFE0(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x4]) = val;
}
/* Address: 0x8011DFF0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DFF0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x0]) = val;
}
/* Address: 0x8011E000 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E000(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x4]);
}
/* Address: 0x8011E018 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E018(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x3]);
}
/* Address: 0x8011E030 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E030(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x16]);
}
/* Address: 0x8011E0AC | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011E0AC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x2C]);
}
/* Address: 0x8011E0C4 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E0C4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x12]);
}
/* Address: 0x8011E0DC | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E0DC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x14]);
}
/* Address: 0x8011E15C | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E15C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xE]);
}
/* Address: 0x8011E174 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011E174(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x20]);
}
/* Address: 0x8011E18C | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E18C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xC]);
}
/* Address: 0x8011E1A4 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E1A4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xA]);
}
/* Address: 0x8011E1BC | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011E1BC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x1C]);
}
/* Address: 0x8011E4D8 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E4D8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x8]);
}
/* Address: 0x8011E4F0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E4F0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x2]);
}
/* Address: 0x8011E508 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E508(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x1]);
}
/* Address: 0x8011E520 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E520(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}
/* Address: 0x8011E538 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E538(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x6]);
}
/* Address: 0x8011E760 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011E760(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x18]);
}
/* Address: 0x8011E7C0 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E7C0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xD6]);
}
/* Address: 0x8011E7D8 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E7D8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xD2]);
}
/* Address: 0x8011E7F0 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E7F0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xD4]);
}
/* Address: 0x8011E808 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E808(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xD0]);
}
/* Address: 0x8011E820 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E820(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xCF]);
}
/* Address: 0x8011E838 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E838(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xCE]);
}
/* Address: 0x8011E850 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E850(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xCD]);
}
/* Address: 0x8011E8DC | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E8DC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xCB]);
}
/* Address: 0x8011E8F4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E8F4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xCA]);
}
/* Address: 0x8011EB48 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011EB48(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xBC]);
}
/* Address: 0x8011EDF8 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011EDF8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xE4]);
}
/* Address: 0x8011EE10 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011EE10(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xE0]);
}
/* Address: 0x8011EE28 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011EE28(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xDC]);
}
/* Address: 0x8011EE40 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011EE40(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xD8]);
}
/* Address: 0x8011EE58 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011EE58(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xB0]);
}
/* Address: 0x8011F188 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011F188(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x8A]);
}
/* Address: 0x8011F1A0 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011F1A0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x88]);
}
/* Address: 0x8011F45C | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011F45C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x74]);
}
/* Address: 0x8011F4A8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011F4A8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x60]);
}
/* Address: 0x8011F4C0 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011F4C0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x5C]);
}
/* Address: 0x8011F520 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011F520(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x14]);
}
/* Address: 0x8011F538 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011F538(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x10]);
}
/* Address: 0x8011F550 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011F550(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xF]);
}
/* Address: 0x8011F568 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011F568(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xE]);
}
/* Address: 0x8011F580 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011F580(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xC]);
}
/* Address: 0x8011F5B0 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011F5B0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}
/* Address: 0x8012A774 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012A774(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA81]) = val;
}
/* Address: 0x8012A784 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012A784(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA81]);
}
/* Address: 0x8012A79C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012A79C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA98]);
}
/* Address: 0x8012A7B4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012A7B4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA98]) = val;
}
/* Address: 0x8012A7C4 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8012A7C4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xA8C]);
}
/* Address: 0x8012A80C | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8012A80C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xA88]);
}
/* Address: 0x8012A854 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8012A854(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xA84]);
}
/* Address: 0x8012A8EC | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012A8EC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA97]);
}
/* Address: 0x8012A904 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012A904(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA96]);
}
/* Address: 0x8012A91C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012A91C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA95]);
}
/* Address: 0x8012A934 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012A934(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA94]);
}
/* Address: 0x8012A94C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012A94C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA93]);
}
/* Address: 0x8012A964 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012A964(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA92]);
}
/* Address: 0x8012A97C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012A97C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA91]);
}
/* Address: 0x8012A994 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012A994(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA90]);
}
/* Address: 0x8012A9AC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012A9AC(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA97]) = val;
}
/* Address: 0x8012A9BC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012A9BC(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA96]) = val;
}
/* Address: 0x8012A9CC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012A9CC(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA95]) = val;
}
/* Address: 0x8012A9DC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012A9DC(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA94]) = val;
}
/* Address: 0x8012A9EC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012A9EC(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA93]) = val;
}
/* Address: 0x8012A9FC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012A9FC(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA92]) = val;
}
/* Address: 0x8012AA0C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012AA0C(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA91]) = val;
}
/* Address: 0x8012AA1C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012AA1C(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA90]) = val;
}
/* Address: 0x8012AA2C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012AA2C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA80]);
}
/* Address: 0x8012AA44 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012AA44(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA80]) = val;
}
/* Address: 0x8012AA54 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012AA54(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x2C]) = val;
}
/* Address: 0x8012AC3C | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8012AC3C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x2C]);
}
/* Address: 0x80130CD8 | Size: 0x8 | Pattern: sda_getter */
u32 fn_80130CD8(void) {
    return lbl_8047ADC0;
}
/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 2 functions matched
 * =================================================================== */
extern u32 lbl_8047ADB8;
/* Address: 0x8011F5C8 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011F5C8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}
/* Address: 0x80128E24 | Size: 0x8 | Pattern: sda_getter */
u32 fn_80128E24(void) {
    return lbl_8047ADB8;
}
/* ===================================================================
 * Generated: 1 pattern-matched + 515 stubs
 * Range: 0x80114CA8 - 0x80130CD8
 * =================================================================== */
extern u32 lbl_804083D0;
extern u32 lbl_8047AD68;
extern u32 lbl_8047AD6C;
extern void fn_801ED674(void);
/* Forward declarations for functions called before definition */
void fn_801193BC(void);
/* Forward declarations for converted functions */
u32 fn_80123E70(void);
void fn_8011BEB4(void);
void fn_8011F260(void);
void fn_8012546C(void*);
void fn_8012795C(void);
void fn_80129BC8(void);
void fn_8012A450(void);
void fn_8012A5B0(void);
/* 0x70 | fn_80114CA8 | alloc_wrapper */
void* fn_80114CA8(void* owner, u32 param, u32 alloc_size) {
    u32 aligned = (alloc_size + 0x1F) & ~0x1F;
    void* mem = (void*)fn_800F9418(aligned + 0x60, 0x20, (u32)owner, (u32)param, 0);
    if (mem == NULL) {
        fn_800DD970("");
        return NULL;
    }
    return (u8*)mem + 0x60;
}
/* 0x54 | fn_80114D18 | generic */
void fn_80114D18(void) {
    fn_800F9318();
    fn_800FF548();
    fn_800F76E4();
    fn_80112700();
}
/* 0x80114D6C | 0xA0 */
extern u32 fn_80115124(void*);
extern u32 fn_80115170(void*);
extern u32 fn_801151BC(void*);
void* fn_80114D6C(void* owner, u32 param, u32 alloc_size) {
    u32 aligned;
    void* mem;
    if ((u8)fn_800FF548() != 0) { return NULL; }
    aligned = (alloc_size + 0x1F) & ~0x1F;
    mem = (void*)fn_800F9418(aligned, 0x20, (u32)owner, param, (u32)fn_80115124);
    if (mem == NULL) {
        fn_800DD970(lbl_80272520, aligned);
    }
    return mem;
}
/* 0x6C | fn_80114E0C | nullcheck_call_flag */
u32 fn_80114E0C(void* obj) {
    if (fn_800FF548() == 0) { return 0; }
    fn_800FC39C(obj);
    return 1;
}
/* 0x80114E78 | 0xA0 */
void* fn_80114E78(void* owner, u32 param, u32 alloc_size) {
    u32 aligned;
    void* mem;
    if ((u8)fn_800FF548() != 0) { return NULL; }
    aligned = (alloc_size + 0x1F) & ~0x1F;
    mem = (void*)fn_800F9418(aligned, 0x20, (u32)owner, param, (u32)fn_80115170);
    if (mem == NULL) {
        fn_800DD970(lbl_8027255C, aligned);
    }
    return mem;
}
/* 0x6C | fn_80114F18 | nullcheck_call_flag */
u32 fn_80114F18(void* obj) {
    if (fn_800FF548() == 0) { return 0; }
    fn_800FC244(obj);
    return 1;
}
/* 0x80114F84 | 0xA0 */
void* fn_80114F84(void* owner, u32 param, u32 alloc_size) {
    u32 aligned;
    void* mem;
    if ((u8)fn_800FF548() != 0) { return NULL; }
    aligned = (alloc_size + 0x1F) & ~0x1F;
    mem = (void*)fn_800F9418(aligned, 0x20, (u32)owner, param, (u32)fn_801151BC);
    if (mem == NULL) {
        fn_800DD970(lbl_80272594, aligned);
    }
    return mem;
}
/* 0x70 | fn_80115024 | alloc_wrapper */
void* fn_80115024(void* owner, u32 param, u32 alloc_size) {
    u32 aligned = (alloc_size + 0x1F) & ~0x1F;
    void* mem = (void*)fn_800F9418(aligned, 0x20, (u32)owner, (u32)param, 0);
    if (mem == NULL) {
        fn_800DD970("");
        return NULL;
    }
    return mem;
}
/* 0x80115094 | 0x24 | call_return_const */
#pragma push
#pragma scheduling off
u32 fn_80115094(void) {
    fn_801ED674();
    return 1;
}
#pragma pop
/* 0x801150B8 | 36 bytes | call_return_const */
#pragma push
#pragma scheduling off
u32 fn_801150B8(void) {
    fn_801193BC();
    return 1;
}
#pragma pop
/* 0x801150DC | 36 bytes | call_return_const */
#pragma push
#pragma scheduling off
u32 fn_801150DC(void) {
    fn_800D2738();
    return 1;
}
#pragma pop
/* 0x80115100 | 36 bytes | call_return_const */
#pragma push
#pragma scheduling off
u32 fn_80115100(void) {
    fn_800DCD98();
    return 1;
}
#pragma pop
/* 0x4C | fn_80115124 | nullcheck_call_flag */
u32 fn_80115124(void* obj) {
    if (fn_800FF554() == 0) { return 0; }
    fn_800F760C(obj);
    return 1;
}
/* 0x4C | fn_80115170 | nullcheck_call_flag */
u32 fn_80115170(void* obj) {
    if (fn_800FF554() == 0) { return 0; }
    fn_800FC2A8(obj);
    return 1;
}
/* 0x4C | fn_801151BC | nullcheck_call_flag */
u32 fn_801151BC(void* obj) {
    if (fn_800FF554() == 0) { return 0; }
    fn_800FC1D0(obj);
    return 1;
}
/* 0x80115208 | 36 bytes | call_return_const */
#pragma push
#pragma scheduling off
u32 fn_80115208(void) {
    fn_8010CC04();
    return 1;
}
#pragma pop
/* 0x8011522C | 36 bytes | call_return_const */
#pragma push
#pragma scheduling off
u32 fn_8011522C(void) {
    fn_800EF5A4();
    return 1;
}
#pragma pop
/* 0x80115250 | 0xC */
u32 fn_80115250(u32 val) {
    return (val & 0x7FFF0000U) | 0x1A00;
}
/* 0x8011525C | 0xC */
u32 fn_8011525C(u32 val) {
    return (val & 0x7FFF0000U) | 0x1800;
}
/* 0x80115268 | 0xC */
u32 fn_80115268(u32 val) {
    return (val & 0x7FFF0000U) | 0x1600;
}
/* 0x80115274 | 0xC */
u32 fn_80115274(u32 val) {
    return (val & 0x7FFF0000U) | 0x1000;
}
/* 0x80115280 | 0x10C */
u32 fn_80115280(void* ptr) {
    extern char lbl_80272608;
    extern char lbl_8027262C;
    extern char lbl_8035BB50;
    u32* data;
    u32 count;
    u32 i;
    if (ptr == NULL) {
        fn_800DD970(&lbl_80272608, &lbl_8035BB50);
        return 0;
    }
    data = *(u32**)((u8*)ptr + 0x10);
    if (data == NULL) {
        return 0;
    }
    data = *(u32**)data;
    if (data == NULL) {
        fn_800DD970(&lbl_8027262C, &lbl_8035BB50);
        return 0;
    }
    count = 0;
    for (i = 2; i < 10; i++) {
        if (data[i] != 0) {
            count++;
        }
    }
    return count;
}
/* 0x8011538C | 0xA0 */
extern const char lbl_80272608[];
extern const char lbl_8027262C[];
extern u8 lbl_8035BB30[];
extern u8 lbl_8035BB50[];
void* fn_8011538C(void* ptr) {
    void* p1;
    void* p2;
    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035BB30);
        return NULL;
    }
    p1 = *(void**)((u8*)ptr + 0x10);
    if (p1 == NULL) { return NULL; }
    p2 = *(void**)((u8*)p1 + 0x4);
    if (p2 == NULL) {
        fn_800DD970(lbl_8027262C, lbl_8035BB30);
        return NULL;
    }
    return p2;
}
/* 0x8011542C | 0x88 */
extern const char lbl_80272608[];
extern const char lbl_8027262C[];
extern u8 lbl_8035BB10[];
void* fn_8011542C(void* ptr) {
    void* p1;
    void* p2;
    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035BB10);
        return NULL;
    }
    p1 = *(void**)((u8*)ptr + 0x10);
    if (p1 == NULL) { return NULL; }
    p2 = *(void**)p1;
    if (p2 == NULL) {
        fn_800DD970(lbl_8027262C, lbl_8035BB10);
        return NULL;
    }
    return *(void**)((u8*)p2 + 0x4);
}
/* 0x801154B4 | 0x88 */
extern u8 lbl_8035BAF4[];
void* fn_801154B4(void* ptr) {
    void* p1;
    void* p2;
    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035BAF4);
        return NULL;
    }
    p1 = *(void**)((u8*)ptr + 0x10);
    if (p1 == NULL) { return NULL; }
    p2 = *(void**)p1;
    if (p2 == NULL) {
        fn_800DD970(lbl_8027262C, lbl_8035BAF4);
        return NULL;
    }
    return *(void**)p2;
}
/* 0x48 | fn_8011553C | nullcheck_store */
void fn_8011553C(void* obj, u32 val) {
    if (obj == NULL) {
        fn_800DD970("");
        return;
    }
    *(u32*)((u8*)obj + 0x34) = val;
}
/* 0x48 | fn_80115584 | nullcheck_store */
void fn_80115584(void* obj, u32 val) {
    if (obj == NULL) {
        fn_800DD970("");
        return;
    }
    *(u32*)((u8*)obj + 0x8) = val;
}
/* 0x5C | fn_801155CC | guarded_call */
u32 fn_801155CC(void) {
    if (1 /* guard r3 != 0 */) { return 0; }
    fn_800DD970("");
    return 0;
}
/* 0x5C | fn_80115628 | guarded_call */
u32 fn_80115628(void) {
    if (1 /* guard r3 != 0 */) { return 0; }
    fn_800DD970("");
    return 0;
}
/* 0x80 | fn_80115684 | guarded_call */
u32 fn_80115684(void) {
    if (1 /* guard r3 != 0 */) { return 0; }
    fn_800DD970("");
    return 0;
}
/* 0x64 | fn_80115704 | guarded_call */
u32 fn_80115704(void) {
    if (1 /* guard r3 != 0 */) { return 0; }
    fn_800DD970("");
    return 0;
}
/* 0x48 | fn_80115768 | global_cond_call */
u32 fn_80115768(void) {
    /* uses lbl_80272608 */
    if (1 /* field check */) { return 0; }
    fn_800DD970("");
    return 0;
}
/* 0x48 | fn_801157B0 | global_cond_call */
u32 fn_801157B0(void) {
    /* uses lbl_80272608 */
    if (1 /* field check */) { return 0; }
    fn_800DD970("");
    return 0;
}
/* 0x48 | fn_801157F8 | global_cond_call */
u32 fn_801157F8(void) {
    /* uses lbl_80272608 */
    if (1 /* field check */) { return 0; }
    fn_800DD970("");
    return 0;
}
/* 0x48 | fn_80115840 | global_cond_call */
u32 fn_80115840(void) {
    /* uses lbl_80272608 */
    if (1 /* field check */) { return 0; }
    fn_800DD970("");
    return 0;
}
/* 0x48 | fn_80115888 | global_cond_call */
u32 fn_80115888(void) {
    /* uses lbl_80272608 */
    if (1 /* field check */) { return 0; }
    fn_800DD970("");
    return 0;
}
/* 0x48 | fn_801158D0 | global_cond_call */
u32 fn_801158D0(void) {
    /* uses lbl_80272608 */
    if (1 /* field check */) { return 0; }
    fn_800DD970("");
    return 0;
}
/* 0x48 | fn_80115918 | global_cond_call */
u32 fn_80115918(void) {
    /* uses lbl_80272608 */
    if (1 /* field check */) { return 0; }
    fn_800DD970("");
    return 0;
}
/* 0x48 | fn_80115960 | global_cond_call */
u32 fn_80115960(void) {
    /* uses lbl_80272608 */
    if (1 /* field check */) { return 0; }
    fn_800DD970("");
    return 0;
}
/* 0x48 | fn_801159A8 | global_cond_call */
u32 fn_801159A8(void) {
    /* uses lbl_80272608 */
    if (1 /* field check */) { return 0; }
    fn_800DD970("");
    return 0;
}
/* 0x48 | fn_801159F0 | global_cond_call */
u32 fn_801159F0(void) {
    /* uses lbl_80272608 */
    if (1 /* field check */) { return 0; }
    fn_800DD970("");
    return 0;
}
/* 0xfn_80115A38 | global_cond_call */
void fn_80115A38(u32 entry) {
    extern u8 lbl_8035B91C[];
    if (entry == 0) {
        fn_800DD970((const char*)&lbl_80272608, (const char*)lbl_8035B91C);
    }
}
/* 0x48 | fn_80115A80 | global_cond_call */
u32 fn_80115A80(void) {
    /* uses lbl_80272608 */
    if (1 /* field check */) { return 0; }
    fn_800DD970("");
    return 0;
}
/* 0x80 | fn_80115AC8 | generic */
u32 fn_80115AC8(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    /* refs: lbl_80272608, lbl_8035B8E8 */
    fn_800DD970("");
    fn_801EF624();
    return 1;
}
/* 0x48 | fn_80115B48 | global_cond_call */
u32 fn_80115B48(void) {
    /* uses lbl_802726AC */
    if (1 /* field check */) { return 0; }
    fn_800DD970("");
    return 0;
}
/* 0x48 | fn_80115B90 | global_cond_call */
u32 fn_80115B90(void) {
    /* uses lbl_80272608 */
    if (1 /* field check */) { return 0; }
    fn_800DD970("");
    return 0;
}
/* 0x70 | fn_80115BD8 | generic */
void fn_80115BD8(void) {
    /* refs: lbl_802726D4, lbl_8035B8A0, lbl_80478FB8, lbl_80478FBC */
    fn_800FF56C();
    fn_800DD970("");
}
/* 0x6C | fn_80115C48 | guarded_call */
u32 fn_80115C48(void) {
    if (0 /* guard r0 == 0 */) { return 0; }
    fn_800DD970("");
    return 0;
}
/* 0x80115CB4 | 0xB0 */
void fn_80115CB4(void) {
    extern u8 lbl_80478EB8[];
    extern u8 lbl_80478EBC[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r4 = r3 & 0x7FFF0000;
    r30 = 0x0;
    /* subis r0, r4, 0x7fff */;
    r28 = 0x0;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r27 = r3 & 0x1FF;
    r29 = 0x0;
    r31 = 0x0;
    while (r3 = *(u32*)lbl_80478EB8, r0 = *(u32*)((u8*)r3 + 0x0), (u32)r29 < (u32)r0) {
        r0 = *(u32*)lbl_80478EBC;
        r30 = r0 + r31;
        ((void(*)(void))fn_800FF56C)();
        r0 = *(u16*)((u8*)r30 + 0x4);
        if ((u32)r0 == (u32)r3) {
            r0 = r28;
            r28 = r28 + 0x1;
            if ((u32)r27 == (u32)r0) break;
        }
        r31 = r31 + 0x1c;
        r29 = r29 + 0x1;
    }
    r3 = *(u32*)lbl_80478EB8;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r29 == (u32)r0) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    return;
}
/* 0x80115D64 | 0xA0 */
void fn_80115D64(void) {
    extern u8 lbl_80478EB8[];
    extern u8 lbl_80478EBC[];
    extern void fn_80113F48();
    extern void fn_8018C1E8();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r25 = r3;
    r26 = r4;
    r28 = 0x0;
    r29 = 0x0;
    r31 = 0x0;
    while (r3 = *(u32*)lbl_80478EB8, r0 = *(u32*)((u8*)r3 + 0x0), (u32)r29 < (u32)r0) {
        r0 = *(u32*)lbl_80478EBC;
        r30 = r0 + r31;
        ((void(*)(void))fn_800FF56C)();
        r0 = *(u16*)((u8*)r30 + 0x4);
        if ((u32)r0 == (u32)r3) {
            if ((u32)r29 == (u32)r25) {
                r27 = r28 | (0x7fff << 16);
                break;
            }
            r28 = r28 + 0x1;
        }
        r31 = r31 + 0x1c;
        r29 = r29 + 0x1;
    }
    r3 = *(u32*)lbl_80478EB8;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r29 != (u32)r0) {
        fn_80113F48();
        r4 = r27;
        r5 = r26;
        fn_8018C1E8();
    }
    return;
}
/* 0x68 | fn_80115E04 | global_cond_call */
u32 fn_80115E04(void) {
    /* uses lbl_80272708 */
    if (1 /* field check */) { return -1; }
    fn_800DD970("");
    return 0;
}
/* 0x80115E6C | 0x2F8 */
void fn_80115E6C(void) {
    extern void fn_8001E184();
    extern void fn_801069FC();
    extern void fn_80106D3C();
    extern void fn_801294C4();
    extern void fn_80129A78();
    extern void fn_80132A38();
    extern void fn_8013467C();
    extern void fn_801653CC();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = 0x0;
    r3 = 0x3ca;
    r4 = 0x0;
    r5 = 0xff;
    fn_801653CC();
    r0 = r27 & 0xFF;
    if ((s32)r0 != (s32)0x3) {
        if ((s32)r0 < (s32)0x3) {
            if ((s32)r0 == (s32)0x1) goto L_80115F1C;
            if ((s32)r0 >= (s32)0x1) goto L_80115EC8;
            r3 = r31;
            return;
        }
        if ((s32)r0 >= (s32)0x5) { r3 = r31; return; }
        goto L_80115F1C;
    L_80115EC8: ;
        r4 = r28;
        r3 = 0x4b;
        fn_80132A38();
        r3 = 0x3;
        r4 = 0x3cb5;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        r3 = 0x1;
        fn_801069FC();
        r4 = r28;
        r3 = 0x0;
        fn_801294C4();
        r3 = 0x3;
        r4 = 0x3cb7;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        r3 = 0x1;
        fn_801069FC();
        r3 = r31;
        return;
    L_80115F1C: ;
        if ((s32)r29 <= (s32)0x0) { r3 = r31; return; }
        r4 = r28;
        r3 = 0x2d;
        fn_80132A38();
        r4 = r29;
        r3 = 0x2f;
        fn_80132A38();
        r0 = r27 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            if ((s32)r29 == (s32)0x1) {
                r30 = 0x3cb4;
            } else {
                r30 = 0x3cb9;
            }
            r4 = r30;
            r3 = 0x3;
            r5 = 0x1;
            r6 = 0x0;
            fn_80106D3C();
            r3 = 0x1;
            fn_801069FC();
        }
        r4 = r28 & 0xFFFF;
        r5 = r29 & 0xFFFF;
        r3 = 0x0;
        r6 = -0x1;
        fn_80129A78();
        r31 = r3;
        if ((s32)r31 == (s32)0x0) {
            if ((s32)r29 == (s32)0x1) {
                r30 = 0x3cb8;
                goto L_80115FDC;
            }
            r30 = 0x3cbd;
            goto L_80115FDC;
        }
        if ((s32)r29 > (s32)0x1) {
            r4 = r28 & 0xFFFF;
            r5 = r31 & 0xFFFF;
            r3 = 0x0;
            fn_8013467C();
            r31 = r3 & 0xFFFF;
            if ((s32)r29 != (s32)0x1) goto L_80115FD8;
            r30 = 0x3cba;
        }
        goto L_80115FDC;
    L_80115FD8: ;
        r30 = 0x3cbb;
    L_80115FDC: ;
        r4 = r30;
        r3 = 0x3;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        r3 = 0x1;
        fn_801069FC();
        r3 = r31;
        return;
    }
    r4 = r28;
    r3 = 0x2d;
    fn_80132A38();
    r3 = 0x3;
    r4 = 0x3cbc;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    r3 = 0x1;
    fn_801069FC();
    r4 = r28 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x1;
    r6 = -0x1;
    fn_80129A78();
    r3 = 0x3;
    r4 = 0x3cb6;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    r3 = 0x1;
    fn_801069FC();
    if ((s32)r28 != (s32)0x21d) {
        if ((s32)r28 < (s32)0x21d) {
            if ((s32)r28 == (s32)0x21b) goto L_80116094;
            if ((s32)r28 >= (s32)0x21b) goto L_8011609C;
            if ((s32)r28 >= (s32)0x21a) goto L_80116084;
            goto L_801160A8;
        }
        if ((s32)r28 == (s32)0x223) goto L_801160A4;
        goto L_801160A8;
    L_80116084: ;
        r30 = 0x3b33;
        goto L_801160A8;
    }
    r30 = 0x3b35;
    goto L_801160A8;
L_80116094: ;
    r30 = 0x3b39;
    goto L_801160A8;
L_8011609C: ;
    r30 = 0x3b37;
    goto L_801160A8;
L_801160A4: ;
    r30 = 0x44c4;
L_801160A8: ;
    r4 = r30;
    r3 = 0x3;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    fn_8001E184();
    r31 = (s8)r3;
    r3 = 0x1;
    fn_801069FC();
    if ((s32)r31 != (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r28 != (s32)0x21d) {
        if ((s32)r28 < (s32)0x21d) {
            if ((s32)r28 == (s32)0x21b) goto L_8011611C;
            if ((s32)r28 >= (s32)0x21b) goto L_80116124;
            if ((s32)r28 >= (s32)0x21a) goto L_8011610C;
            goto L_80116130;
        }
        if ((s32)r28 == (s32)0x223) goto L_8011612C;
        goto L_80116130;
    L_8011610C: ;
        r30 = 0x3b34;
        goto L_80116130;
    }
    r30 = 0x3b36;
    goto L_80116130;
L_8011611C: ;
    r30 = 0x3b30;
    goto L_80116130;
L_80116124: ;
    r30 = 0x3b38;
    goto L_80116130;
L_8011612C: ;
    r30 = 0x44c5;
L_80116130: ;
    r4 = r30;
    r3 = 0x3;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    r3 = 0x1;
    fn_801069FC();
    r3 = r31;
    return;
}
/* 0x80116164 | 0x30C */
void fn_80116164(void) {
    extern u8 lbl_80478EB8[];
    extern u8 lbl_80478EBC[];
    extern void fn_80115E6C();
    extern void fn_80166A28();
    extern void fn_8018B07C();
    extern void fn_8018B76C();
    extern void fn_8018C1E8();
    extern void fn_8018C7C8();
    extern void fn_801902E0();
    extern void fn_80190528();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r28 = r3;
    r29 = r4;
    r23 = r5;
    r30 = r29 & 0x7FFF0000;
    r31 = 0x0;
    /* subis r0, r30, 0x7fff */;
    r26 = r31;
    if ((u32)r0 != (u32)0x0) {
        goto L_80116200;
    }
    r25 = r31;
    r24 = r31;
    r27 = r29 & 0x1FF;
    while (r3 = *(u32*)lbl_80478EB8, r0 = *(u32*)((u8*)r3 + 0x0), (u32)r25 < (u32)r0) {

    r0 = *(u32*)lbl_80478EBC;
    r31 = r0 + r24;
    ((void(*)(void))fn_800FF56C)();
    r0 = *(u16*)((u8*)r31 + 0x4);
    if ((u32)r0 == (u32)r3) {
        r0 = r26;
        r26 = r26 + 0x1;
        if ((u32)r27 == (u32)r0) goto L_801161EC;
    }
    r24 = r24 + 0x1c;
    r25 = r25 + 0x1;
    }

L_801161EC: ;
    r3 = *(u32*)lbl_80478EB8;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r25 == (u32)r0) {
        r31 = 0x0;
    }
L_80116200: ;
    if ((u32)r31 == (u32)0x0) {
        r3 = -0x1;
        return;
    }
    r0 = r23 & 0xFF;
    if ((s32)r0 != (s32)0x1) {
        if ((s32)r0 < (s32)0x1) {
            if ((s32)r0 >= (s32)0x0) goto L_80116238;
            r3 = 0x0;
            return;
        }
        if ((s32)r0 >= (s32)0x3) { r3 = 0x0; return; }
        goto L_801162EC;
    L_80116238: ;
        r0 = *(u8*)((u8*)r31 + 0x0);
        if ((s32)r0 != (s32)0x1) {
            if ((s32)r0 < (s32)0x1) { r3 = 0x0; return; }
            if ((s32)r0 >= (s32)0x4) { r3 = 0x0; return; }
        } else {
            r3 = r28;
            r4 = r29;
            r5 = 0x2;
            r6 = 0x0;
            r7 = 0x1;
            fn_8018B76C();
            r3 = r28;
            r4 = r29;
            r5 = 0x1;
            fn_8018C7C8();
            r3 = 0x0;
            return;
        }
        r3 = r28;
        r4 = r29;
        r5 = 0x0;
        fn_8018C1E8();
        r3 = 0x0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x0);
    if ((s32)r0 != (s32)0x1) {
        if ((s32)r0 < (s32)0x1) { r3 = 0x0; return; }
        if ((s32)r0 >= (s32)0x4) { r3 = 0x0; return; }
    } else {
        r3 = r28;
        r4 = r29;
        r5 = 0x0;
        r6 = 0x0;
        r7 = 0x1;
        fn_8018B76C();
    }
    r3 = r28;
    r4 = r29;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_8018B76C();
    r3 = 0x0;
    return;
L_801162EC: ;
    r3 = *(u16*)((u8*)r31 + 0x6);
    if ((u32)r3 != (u32)0x0) {
        fn_801902E0();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x0) {
            r3 = 0x0;
            return;
    }
    }
    r0 = *(u8*)((u8*)r31 + 0x0);
    if ((s32)r0 != (s32)0x1) {
        if ((s32)r0 < (s32)0x1) goto L_8011635C;
        goto L_8011635C;
    }
    r3 = r28;
    r4 = r29;
    r5 = 0x1;
    r6 = 0x0;
    r7 = 0x0;
    fn_8018B76C();
    r3 = 0x3c2;
    fn_80166A28();
    r3 = r28;
    r4 = r29;
    r5 = 0x1;
    fn_8018B07C();
L_8011635C: ;
    /* subis r0, r30, 0x7fff */;
    r30 = 0x0;
    r26 = r30;
    if ((u32)r0 != (u32)0x0) {
        goto L_801163D8;
    }
    r27 = r30;
    r24 = r30;
    r25 = r29 & 0x1FF;
    while (r3 = *(u32*)lbl_80478EB8, r0 = *(u32*)((u8*)r3 + 0x0), (u32)r27 < (u32)r0) {

    r0 = *(u32*)lbl_80478EBC;
    r30 = r0 + r24;
    ((void(*)(void))fn_800FF56C)();
    r0 = *(u16*)((u8*)r30 + 0x4);
    if ((u32)r0 == (u32)r3) {
        r0 = r26;
        r26 = r26 + 0x1;
        if ((u32)r25 == (u32)r0) goto L_801163C4;
    }
    r24 = r24 + 0x1c;
    r27 = r27 + 0x1;
    }

L_801163C4: ;
    r3 = *(u32*)lbl_80478EB8;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r27 == (u32)r0) {
        r30 = 0x0;
    }
L_801163D8: ;
    if ((u32)r30 != (u32)0x0) {
        r0 = *(u8*)((u8*)r30 + 0x0);
        if ((s32)r0 == (s32)0x1) goto L_80116400;
    }
    if ((s32)r0 < (s32)0x1 || (s32)r0 >= (s32)0x4) goto L_8011643C;
    goto L_8011642C;
L_80116400: ;
    r3 = r28;
    r4 = r29;
    r5 = 0x2;
    r6 = 0x0;
    r7 = 0x1;
    fn_8018B76C();
    r3 = r28;
    r4 = r29;
    r5 = 0x1;
    fn_8018C7C8();
    goto L_8011643C;
L_8011642C: ;
    r3 = r28;
    r4 = r29;
    r5 = 0x0;
    fn_8018C1E8();
L_8011643C: ;
    r3 = *(u16*)((u8*)r31 + 0x6);
    fn_80190528();
    r0 = *(u8*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0xC);
    r5 = *(u8*)((u8*)r31 + 0x1);
    fn_80115E6C();
    r3 = 0x0;
    return;
}
/* 0x80116470 | 0x4E8 */
void fn_80116470(void) {
    extern u8 lbl_80478EC8[];
    extern u8 lbl_80478ECC[];
    extern u8 lbl_8047CFA0[];
    extern u8 lbl_8047CFA4[];
    extern u8 lbl_8047CFA8[];
    extern u8 lbl_8047CFAC[];
    extern u8 lbl_8047CFB0[];
    extern u8 lbl_8047CFB4[];
    extern u8 lbl_8047CFB8[];
    extern void fn_800D37CC();
    extern void fn_800EC1BC();
    extern void fn_800EC4D0();
    extern void fn_800EC960();
    extern void fn_800EC990();
    extern void fn_800EC9DC();
    extern void fn_800ECA78();
    extern void fn_800ECB74();
    extern void fn_800ECCA8();
    extern void fn_800EE150();
    extern void fn_800EE3BC();
    extern void fn_800EE828();
    extern void fn_800F0308();
    extern void fn_80113F6C();
    extern void fn_80115A80();
    extern void fn_80115C48();
    extern void fn_80116470();
    extern void fn_801669BC();
    extern void fn_80166A28();
    extern void fn_80184470();
    extern void fn_801845E4();
    extern void fn_801860F8();
    extern void fn_8018805C();
    extern void fn_8018A280();
    extern void fn_8018AACC();
    extern void fn_8018C0A8();
    u8 sp[0x60];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f31 = 0.0f;
    *(f64*)(sp + 0x50) = f31;
    /* psq_st f31, 0x58((u32)sp), 0, qr0 */;
    r24 = r3;
    r25 = r4;
    r23 = r5;
    r26 = r6;
    r27 = r7;
    f0 = *(f32*)lbl_8047CFA0;
    r28 = -0x1;
    r4 = *(u32*)lbl_80478EC8;
    *(f32*)(sp + 0x8) = f0;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r25 >= (u32)r0) {
        r3 = -0x1;
        goto L_8011693C;
    }
    r0 = r25 * 0x18;
    r4 = *(u32*)lbl_80478ECC;
    r30 = r4 + r0;
    r4 = *(u32*)((u8*)r30 + 0x14);
    if ((u32)r4 == (u32)0x0) {
        r3 = -0x1;
        goto L_8011693C;
    }
    fn_80113F6C();
    r29 = r3;
    if ((u32)r29 == (u32)0x0) {
        r3 = -0x1;
        goto L_8011693C;
    }
    r3 = r24;
    fn_80115C48();
    fn_80115A80();
    r31 = r23 & 0xFFFF;
    r23 = r3;
    if ((s32)r31 < (s32)0x83) {
        if ((s32)r31 < (s32)0x3) {
            if ((s32)r31 >= (s32)0x1) goto L_80116540;
            goto L_80116798;
        }
        if ((s32)r31 >= (s32)0x81) goto L_80116540;
        goto L_80116798;
    }
    if ((s32)r31 == (s32)0xc0) goto L_80116750;
    goto L_80116798;
L_80116540: ;
    r0 = *(u8*)((u8*)r30 + 0x0);
    r4 = (s8)r0;
    if ((s32)r4 >= (s32)0x0) {
        f31 = *(f32*)(sp + 0x8);
        if ((u32)r29 != (u32)0x0) {
            r3 = r29;
            fn_800ECCA8();
            f1 = f31;
            r3 = r29;
            fn_800ECA78();
            f1 = *(f32*)lbl_8047CFA4;
            r3 = r29;
            fn_800EC9DC();
            r3 = r29;
            r4 = 0x0;
            fn_800ECB74();
            r3 = r29;
            fn_800EC990();
    }
    }
    r24 = r31 & 0x00000080;
    if ((s32)r24 != (s32)0x0) {
        r3 = 0x44;
        fn_80166A28();
    }
    if ((u32)r29 == (u32)0x0) {
        goto L_801165CC;
    }
    while (r3 = r29, fn_800EC960(), r0 = r3 & 0xFF, (u32)r0 != (u32)0x0) {

    fn_800F0308();
    }

L_801165CC: ;
    r4 = *(u8*)((u8*)r30 + 0x8);
    r3 = r29;
    fn_800EE150();
    r4 = (u32)sp + 0xc;
    r25 = r3;
    r5 = 0x0;
    r6 = 0x0;
    fn_800EE3BC();
    r3 = r25;
    fn_800EE828();
    r0 = r31 & 0x1;
    if ((s32)r0 != (s32)0x0) {
        r3 = r26;
        r4 = r27;
        r6 = (u32)sp + 0xc;
        r5 = 0x1;
        fn_8018AACC();
        r3 = r26;
        r4 = r27;
        r5 = 0x1;
        fn_8018A280();
        f1 = *(f32*)lbl_8047CFA0;
        r3 = r26;
        f2 = *(f32*)lbl_8047CFA4;
        r4 = r27;
        fn_8018805C();
        r3 = r26;
        r4 = r27;
        r5 = 0x1;
        fn_8018A280();
    } else {
        r3 = r26;
        r4 = r27;
        fn_80184470();
        r4 = *(u8*)((u8*)r30 + 0x8);
        r3 = r29;
        fn_800EE150();
        r4 = (u32)sp + 0xc;
        r25 = r3;
        r5 = 0x0;
        r6 = 0x0;
        fn_800EE3BC();
        r3 = r25;
        fn_800EE828();
        r3 = r26;
        r4 = r27;
        r5 = (u32)sp + 0xc;
        fn_8018C0A8();
        f1 = *(f32*)(sp + 0x14);
        r3 = r26;
        f0 = *(f32*)lbl_8047CFA8;
        r4 = r27;
        r6 = (u32)sp + 0xc;
        r5 = 0x1;
        f0 = f1 + f0;
        *(f32*)(sp + 0x14) = f0;
        fn_8018AACC();
        r3 = r26;
        r4 = r27;
        r5 = 0x1;
        fn_8018A280();
    }
    r0 = *(u8*)((u8*)r30 + 0x1);
    r4 = (s8)r0;
    if ((s32)r4 >= (s32)0x0) {
        f31 = *(f32*)(sp + 0x8);
        if ((u32)r29 != (u32)0x0) {
            r3 = r29;
            fn_800ECCA8();
            f1 = f31;
            r3 = r29;
            fn_800ECA78();
            f1 = *(f32*)lbl_8047CFA4;
            r3 = r29;
            fn_800EC9DC();
            r3 = r29;
            r4 = 0x0;
            fn_800ECB74();
            r3 = r29;
            fn_800EC990();
    }
    }
    if ((s32)r24 != (s32)0x0) {
        r3 = 0x44;
        fn_80166A28();
    }
    if ((u32)r29 == (u32)0x0) {
        goto L_801168E8;
    }
    while (r3 = r29, fn_800EC960(), r0 = r3 & 0xFF, (u32)r0 != (u32)0x0) {

    fn_800F0308();
    }

    goto L_801168E8;
L_80116750: ;
    if ((u32)r29 == (u32)0x0) {
        goto L_80116778;
    }
    while (r3 = r29, fn_800EC960(), r0 = r3 & 0xFF, (u32)r0 != (u32)0x0) {

    fn_800F0308();
    }

L_80116778: ;
    r0 = r31 & 0x00000080;
    if ((s32)r0 != (s32)0x0) {
        r3 = 0x45;
        fn_801669BC();
        r3 = 0x46;
        fn_80166A28();
    }
    goto L_801168E8;
L_80116798: ;
    r0 = r31 & 0x00000004;
    if ((s32)r0 != (s32)0x0) {
        r28 = *(u8*)((u8*)r30 + 0x5);
        r28 = (s8)r28;
        goto L_801167C4;
    }
    r0 = r31 & 0x00000008;
    if ((s32)r0 != (s32)0x0) {
        r28 = *(u8*)((u8*)r30 + 0x6);
        r28 = (s8)r28;
    }
L_801167C4: ;
    r0 = (s16)r28;
    if ((s32)r0 < (s32)0x0) {
        r3 = -0x1;
        goto L_8011693C;
    }
    r3 = r29;
    fn_800EC1BC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r3 = -0x1;
        goto L_8011693C;
    }
    r0 = r31 & 0x00000020;
    if ((s32)r0 != (s32)0x0) {
        r6 = *(u32*)((u8*)r30 + 0x14);
        r3 = r26;
        r7 = *(u8*)((u8*)r30 + 0x8);
        r4 = r27;
        r5 = r23;
        fn_801845E4();
        f1 = *(f32*)lbl_8047CFA0;
        r3 = r26;
        r4 = r27;
        f2 = f1;
        f3 = f1;
        fn_801860F8();
    }
    r0 = r31 & 0x00000010;
    if ((s32)r0 != (s32)0x0) {
        r3 = r29;
        r4 = (s16)r28;
        fn_800ECCA8();
        r3 = r29;
        r4 = (u32)sp + 0x8;
        r5 = 0x0;
        fn_800EC4D0();
        f1 = *(f32*)(sp + 0x8);
        f0 = *(f32*)lbl_8047CFAC;
        f0 = f1 - f0;
        *(f32*)(sp + 0x8) = f0;
    }
    f31 = *(f32*)(sp + 0x8);
    if ((u32)r29 != (u32)0x0) {
        r3 = r29;
        r4 = (s16)r28;
        fn_800ECCA8();
        f1 = f31;
        r3 = r29;
        fn_800ECA78();
        f1 = *(f32*)lbl_8047CFA4;
        r3 = r29;
        fn_800EC9DC();
        r3 = r29;
        r4 = 0x0;
        fn_800ECB74();
        r3 = r29;
        fn_800EC990();
    }
    r0 = r31 & 0x00000040;
    if ((s32)r0 != (s32)0x0) {
        r3 = r24;
        r4 = r25;
        r6 = r26;
        r7 = r27;
        r5 = 0xc0;
        fn_80116470();
        goto L_801168E8;
    }
    r0 = r31 & 0x00000080;
    if ((s32)r0 != (s32)0x0) {
        r3 = 0x45;
        fn_80166A28();
    }
L_801168E8: ;
    r3 = r29;
    r4 = (u32)sp + 0x8;
    r5 = 0x0;
    fn_800EC4D0();
    fn_800D37CC();
    /* xoris r3, r3, 0x8000 */;
    r0 = (0x4330 << 16);
    f3 = *(f64*)lbl_8047CFB8;
    *(u32*)(sp + 0x18) = r0;
    f2 = *(f32*)lbl_8047CFB4;
    f0 = *(f64*)(sp + 0x18);
    f1 = *(f32*)(sp + 0x8);
    f3 = f0 - f3;
    f0 = *(f32*)lbl_8047CFB0;
    f1 = f2 * f1;
    f1 = f1 / f3;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x20) = f0;
    r3 = *(u32*)(sp + 0x24);
L_8011693C: ;
    /* psq_l f31, 0x58((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x50);
    return;
}
/* 0x80116958 | 0x3D8 */
void fn_80116958(void) {
    extern u8 lbl_80478EC8[];
    extern u8 lbl_80478ECC[];
    extern u8 lbl_8047CFA0[];
    extern u8 lbl_8047CFA4[];
    extern u8 lbl_8047CFAC[];
    extern void fn_800EC1BC();
    extern void fn_800EC4D0();
    extern void fn_800EC960();
    extern void fn_800EC990();
    extern void fn_800EC9DC();
    extern void fn_800ECA78();
    extern void fn_800ECB74();
    extern void fn_800ECCA8();
    extern void fn_800F0308();
    extern void fn_80113F6C();
    extern void fn_8012BBA8();
    extern void fn_8012BCA4();
    extern void fn_80166A28();
    extern void fn_8018C558();
    extern void fn_8018C8F4();
    extern void fn_801902E0();
    extern void fn_801903B0();
    extern void fn_80190528();
    extern void fn_801CAA08();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f31 = 0.0f;
    *(f64*)(sp + 0x30) = f31;
    /* psq_st f31, 0x38((u32)sp), 0, qr0 */;
    r28 = r3;
    f0 = *(f32*)lbl_8047CFA0;
    r30 = -0x1;
    r3 = *(u32*)lbl_80478EC8;
    r29 = 0x0;
    *(f32*)(sp + 0x8) = f0;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r4 >= (u32)r0) {
        r3 = -0x1;
        goto L_80116D14;
    }
    r31 = r5 & 0xFF;
    r3 = *(u32*)lbl_80478ECC;
    r0 = r4 * 0x18;
    r27 = r3 + r0;
    if ((s32)r31 != (s32)0x2) {
        if ((s32)r31 < (s32)0x2) {
            if ((s32)r31 == (s32)0x0) goto L_801169D4;
            if ((s32)r31 >= (s32)0x0) goto L_80116A1C;
            goto L_80116A7C;
        }
        if ((s32)r31 >= (s32)0x4) goto L_80116A7C;
        goto L_80116A60;
    L_801169D4: ;
        r3 = *(u16*)((u8*)r27 + 0xE);
        if ((u32)r3 != (u32)0x0) {
            fn_801902E0();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x0) {
                r3 = 0x0;
                goto L_80116D14;
            }
        }
        r3 = *(u16*)((u8*)r27 + 0x10);
        if ((u32)r3 != (u32)0x0) {
            fn_801902E0();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x0) {
                r3 = 0x1;
                goto L_80116D14;
        }
        }
    L_80116A1C: ;
        r3 = *(u16*)((u8*)r27 + 0xE);
        if ((u32)r3 != (u32)0x0) {
            fn_80190528();
        }
        r3 = *(s16*)((u8*)r27 + 0xA);
        r4 = 0x0;
        fn_801CAA08();
        goto L_80116A7C;
    }
    r3 = *(u16*)((u8*)r27 + 0xE);
    if ((u32)r3 != (u32)0x0) {
        fn_801902E0();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x0) {
            r3 = 0x0;
            goto L_80116D14;
    }
    }
L_80116A60: ;
    r3 = *(u16*)((u8*)r27 + 0xE);
    if ((u32)r3 != (u32)0x0) {
        fn_801903B0();
    }
    r3 = *(s16*)((u8*)r27 + 0xA);
    r4 = 0x1;
    fn_801CAA08();
L_80116A7C: ;
    r4 = *(u32*)((u8*)r27 + 0x14);
    if ((u32)r4 == (u32)0x0) {
        r3 = -0x1;
        goto L_80116D14;
    }
    r3 = r28;
    fn_80113F6C();
    r28 = r3;
    if ((u32)r28 == (u32)0x0) {
        r3 = -0x1;
        goto L_80116D14;
    }
    if ((s32)r31 != (s32)0x2) {
        if ((s32)r31 < (s32)0x2) {
            if ((s32)r31 == (s32)0x0) goto L_80116AD4;
            if ((s32)r31 >= (s32)0x0) goto L_80116B1C;
            goto L_80116BF8;
        }
        if ((s32)r31 >= (s32)0x4) goto L_80116BF8;
        goto L_80116BB0;
    L_80116AD4: ;
        r0 = *(u8*)((u8*)r27 + 0x7);
        r30 = *(u8*)((u8*)r27 + 0x0);
        r30 = (s8)r30;
        if ((s32)r0 != (s32)0x2) {
            if ((s32)r0 < (s32)0x2) {
                if ((s32)r0 >= (s32)0x1) goto L_80116B04;
                goto L_80116BF8;
            }
            if ((s32)r0 >= (s32)0x4) goto L_80116BF8;
            goto L_80116B14;
        L_80116B04: ;
            r29 = 0x44;
            goto L_80116BF8;
        }
        r29 = 0x44;
        goto L_80116BF8;
    L_80116B14: ;
        r29 = 0x4be;
        goto L_80116BF8;
    L_80116B1C: ;
        r30 = *(u8*)((u8*)r27 + 0x2);
        r30 = (s8)r30;
        if ((s32)r30 < (s32)0x0) {
            r30 = *(u8*)((u8*)r27 + 0x0);
            r30 = (s8)r30;
            if ((s32)r30 >= (s32)0x0) {
                r4 = r30;
                fn_800ECCA8();
                r3 = r28;
                r4 = (u32)sp + 0x8;
                r5 = 0x0;
                fn_800EC4D0();
                f1 = *(f32*)(sp + 0x8);
                f0 = *(f32*)lbl_8047CFAC;
                f0 = f1 - f0;
                *(f32*)(sp + 0x8) = f0;
        }
        }
        goto L_80116BF8;
    }
    r0 = *(u8*)((u8*)r27 + 0x7);
    r30 = *(u8*)((u8*)r27 + 0x1);
    r30 = (s8)r30;
    if ((s32)r0 != (s32)0x2) {
        if ((s32)r0 < (s32)0x2) {
            if ((s32)r0 >= (s32)0x1) goto L_80116B98;
            goto L_80116BF8;
        }
        if ((s32)r0 >= (s32)0x4) goto L_80116BF8;
        goto L_80116BA8;
    L_80116B98: ;
        r29 = 0x44;
        goto L_80116BF8;
    }
    r29 = 0x44;
    goto L_80116BF8;
L_80116BA8: ;
    r29 = 0x4be;
    goto L_80116BF8;
L_80116BB0: ;
    r30 = *(u8*)((u8*)r27 + 0x3);
    r30 = (s8)r30;
    if ((s32)r30 < (s32)0x0) {
        r30 = *(u8*)((u8*)r27 + 0x1);
        r30 = (s8)r30;
        if ((s32)r30 >= (s32)0x0) {
            r4 = r30;
            fn_800ECCA8();
            r3 = r28;
            r4 = (u32)sp + 0x8;
            r5 = 0x0;
            fn_800EC4D0();
            f1 = *(f32*)(sp + 0x8);
            f0 = *(f32*)lbl_8047CFAC;
            f0 = f1 - f0;
            *(f32*)(sp + 0x8) = f0;
    }
    }
L_80116BF8: ;
    r0 = (s16)r30;
    if ((s32)r0 < (s32)0x0) {
        r3 = -0x1;
        goto L_80116D14;
    }
    r3 = r28;
    fn_800EC1BC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r3 = -0x1;
        goto L_80116D14;
    }
    f31 = *(f32*)(sp + 0x8);
    if ((u32)r28 != (u32)0x0) {
        r3 = r28;
        r4 = (s16)r30;
        fn_800ECCA8();
        f1 = f31;
        r3 = r28;
        fn_800ECA78();
        f1 = *(f32*)lbl_8047CFA4;
        r3 = r28;
        fn_800EC9DC();
        r3 = r28;
        r4 = 0x0;
        fn_800ECB74();
        r3 = r28;
        fn_800EC990();
    }
    if ((u32)r29 != (u32)0x0) {
        r3 = r29;
        fn_80166A28();
    }
    if ((s32)r31 != (s32)0x1) {
        if ((s32)r31 >= (s32)0x1) goto L_80116C94;
        if ((s32)r31 >= (s32)0x0) goto L_80116C9C;
    }
    goto L_80116D10;
L_80116C94: ;
    if ((s32)r31 >= (s32)0x3) goto L_80116D10;
L_80116C9C: ;
    r3 = 0x0;
    r4 = 0x64;
    fn_8018C558();
    r0 = r3;
    r3 = 0x0;
    r27 = r0;
    r4 = 0x65;
    fn_8018C558();
    r29 = r3;
    fn_8012BCA4();
    if ((u32)r28 == (u32)0x0) {
        goto L_80116CEC;
    }
    while (r3 = r28, fn_800EC960(), r0 = r3 & 0xFF, (u32)r0 != (u32)0x0) {

    fn_800F0308();
    }

L_80116CEC: ;
    fn_8012BBA8();
    r5 = r27;
    r3 = 0x0;
    r4 = 0x64;
    fn_8018C8F4();
    r5 = r29;
    r3 = 0x0;
    r4 = 0x65;
    fn_8018C8F4();
L_80116D10: ;
    r3 = 0x0;
L_80116D14: ;
    /* psq_l f31, 0x38((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x30);
    return;
}
/* 0x80116D30 | 0x13C */
void fn_80116D30(void) {
    extern u8 lbl_80272708[];
    extern u8 lbl_80478EC0[];
    extern u8 lbl_80478EC4[];
    extern void fn_800F7434();
    extern void fn_801141F8();
    extern void fn_801CA9F8();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r26 = r3;
    r29 = r4;
    r5 = (u32)lbl_80272708;
    r31 = (u32)lbl_80272708;
    fn_801141F8();
    r28 = (s8)r26;
    switch ((s32)r28) {
    case 0x1:
        r4 = r29;
        r3 = r31 + 0x14;
        ((void(*)(void))fn_800DD970)();
        break;
    case 0x2:
        r4 = r29;
        r3 = r31 + 0x28;
        ((void(*)(void))fn_800DD970)();
        break;
    case 0x3:
        r4 = r29;
        r3 = r31 + 0x3c;
        ((void(*)(void))fn_800DD970)();
        break;
    case 0x4:
        r4 = r29;
        r3 = r31 + 0x50;
        ((void(*)(void))fn_800DD970)();
        break;
    default:
        break;
    }
    ((void(*)(void))fn_800FF56C)();
    r31 = r3;
    r26 = 0x0;
    r30 = 0x0;
    while (1) {
        r3 = *(u32*)lbl_80478EC0;
        r0 = *(u32*)((u8*)r3 + 0x0);
        if ((u32)r26 >= (u32)r0) break;
        r0 = *(u32*)lbl_80478EC4;
        r27 = r0 + r30;
        r0 = *(u16*)((u8*)r27 + 0x2);
        if ((u32)r0 == (u32)r31) {
            r0 = *(u8*)((u8*)r27 + 0x0);
            if ((s32)r0 == (s32)r28) {
                r0 = *(u32*)((u8*)r27 + 0x4);
                if ((u32)r0 == (u32)r29) {
                    r0 = *(u32*)((u8*)r27 + 0x8);
                    if ((u32)r0 != (u32)0x0) {
                        r3 = r29;
                        fn_801CA9F8();
                        r3 = *(u32*)((u8*)r27 + 0x8);
                        r4 = 0x4;
                        r5 = *(u32*)((u8*)r27 + 0xC);
                        r6 = *(u32*)((u8*)r27 + 0x10);
                        r7 = *(u32*)((u8*)r27 + 0x14);
                        r8 = *(u32*)((u8*)r27 + 0x18);
                        fn_800F7434();
                    }
                }
            }
        }
        r30 = r30 + 0x1c;
        r26 = r26 + 0x1;
    }
    return;
}
/* 0x80116E6C | 0x18 */
void fn_80116E6C(u8* ptr, u8 val) {
    u8 tmp;
    if (ptr == NULL) { return; }
    tmp = ptr[0];
    tmp = (u8)((tmp & ~0x80) | ((val & 1) << 7));
    ptr[0] = tmp;
}
/* 0x80116E84 | 0x2C */
void fn_80116E84(u8* dst, f32* src) {
    if (dst == NULL) { return; }
    if (src == NULL) { return; }
    *(f32*)(&dst[0x18]) = src[0];
    *(f32*)(&dst[0x1C]) = src[1];
    *(f32*)(&dst[0x20]) = src[2];
}
/* 0x80116EB0 | 24 bytes | beq_default_getter */
u32 fn_80116EB0(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0x14);
}
/* 0x80116EC8 | 24 bytes | beq_default_getter */
u32 fn_80116EC8(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0x10);
}
/* 0x80116EE0 | 24 bytes | beq_default_getter */
u16 fn_80116EE0(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)((u8*)ptr + 0x8);
}
/* 0x80116EF8 | 0x1C */
u32 fn_80116EF8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (u32)((ptr[0] >> 4) & 1);
}
/* 0x80116F14 | 0x1C */
u32 fn_80116F14(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (u32)((ptr[1] >> 4) & 3);
}
/* 0x80116F30 | 0x1C */
u32 fn_80116F30(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (u32)((ptr[1] >> 6) & 3);
}
/* 0x80116F4C | 0x1C */
u32 fn_80116F4C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (u32)(ptr[0] & 7);
}
/* 0x78 | fn_80116F68 | guarded_call */
u32 fn_80116F68(void) {
    if (1 /* guard r3 != 0 */) { return 0; }
    if (1 /* guard r4 != 0 */) { return 0; }
    fn_800E01F4();
    return 1;
}
/* 0x58 | fn_80116FE0 | guarded_call */
u32 fn_80116FE0(void) {
    if (1 /* guard r5 != 0 */) { return 0; }
    if (1 /* guard r4 != 0 */) { return 0; }
    fn_800E01F4();
    return 1;
}
/* 0x80117038 | 0x1C */
u32 fn_80117038(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (u32)((ptr[0] >> 6) & 1);
}
/* 0x80117054 | 0x1C */
u32 fn_80117054(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (u32)((ptr[0] >> 7) & 1);
}
/* 0x80117070 | 0x34 */
extern void* fn_8018F6CC(u16);
void* fn_80117070(u8* ptr) {
    if (ptr == NULL) { return NULL; }
    return fn_8018F6CC(*(u16*)(&ptr[0x6]));
}
/* 0x78 | fn_801170A4 | generic */
u32 fn_801170A4(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_800FF56C();
    fn_80115C48();
    fn_80115A80();
    fn_80115684();
    return 0;
}
/* 0x8011711C | 0x38 */
void fn_8011711C(u32 arg) {
    /* depends on fn_80115C48/fn_80115684 signatures */
}
/* 0x80117154 | 16 bytes | multi_sda_store */
void fn_80117154(void) {
    lbl_8047AD68 = 0;
    lbl_8047AD6C = 0;
}
/* 0x64 | fn_80117164 | generic */
void fn_80117164(void) {
    /* refs: lbl_8047AD68, lbl_8047AD6C, lbl_8047AD70, lbl_8047AD71, lbl_8047AD74, lbl_8047AD78, lbl_8047AD7C, lbl_8047CFD0 */
    fn_801155CC();
}
/* 0x801171C8 | 0x168 */
void fn_801171C8(void) {
    extern u8 lbl_8047AD74[];
    extern u8 lbl_8047AD78[];
    extern u8 lbl_8047AD7C[];
    extern u8 lbl_8047CFD4[];
    extern u8 lbl_8047CFD8[];
    extern void fn_80117514();
    extern void fn_80176758();
    extern void fn_801767E0();
    extern void fn_80176868();
    extern void fn_80177908();
    extern void fn_80177A38();
    extern u8 lbl_8047AD70;
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    *(f64*)(sp + 0x30) = f31;
    /* psq_st f31, 0x38((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x20) = f30;
    /* psq_st f30, 0x28((u32)sp), 0, qr0 */;
    r0 = *(u8*)&lbl_8047AD71;
    if ((u32)r0 != (u32)0x0) {
        fn_80177A38();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x0) {
            r0 = *(u32*)&lbl_8047AD68;
            if ((u32)r0 != (u32)0x0) {
                if ((u32)r0 == (u32)0x1) {
                r3 = *(u32*)&lbl_8047AD6C;
                f30 = *(f32*)((u8*)r3 + 0x14);
                f31 = *(f32*)((u8*)r3 + 0xC);
                f1 = *(f32*)((u8*)r3 + 0x10);
                fn_80176868();
                f1 = f31;
                fn_801767E0();
                f1 = f30;
                fn_80176758();
                } else {
    r3 = (u32)sp + 0x14;
    fn_80177908();
    f2 = *(f32*)lbl_8047AD74;
    r3 = (u32)sp + 0x14;
    f1 = *(f32*)lbl_8047AD78;
    r4 = (u32)sp + 0xc;
    f0 = *(f32*)lbl_8047AD7C;
    r5 = (u32)sp + 0x10;
    *(f32*)(sp + 0xC) = f2;
    r6 = (u32)sp + 0x8;
    *(f32*)(sp + 0x10) = f1;
    *(f32*)(sp + 0x8) = f0;
    fn_80117514();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r0 = *(u8*)&lbl_8047AD70;
        if ((u32)r0 != (u32)0x0) {
            f3 = *(f32*)lbl_8047CFD8;
            f2 = *(f32*)(sp + 0xC);
            f1 = *(f32*)(sp + 0x10);
            f0 = *(f32*)(sp + 0x8);
            f4 = f3 * f2;
            f2 = f3 * f1;
            f6 = *(f32*)lbl_8047CFD4;
            f5 = *(f32*)lbl_8047AD74;
            f0 = f3 * f0;
            f3 = *(f32*)lbl_8047AD78;
            f1 = *(f32*)lbl_8047AD7C;
            f4 = f6 * f5 + f4;
            f2 = f6 * f3 + f2;
            f0 = f6 * f1 + f0;
            *(f32*)lbl_8047AD74 = f4;
            *(f32*)lbl_8047AD78 = f2;
            *(f32*)lbl_8047AD7C = f0;
        } else {
            f2 = *(f32*)(sp + 0xC);
            r0 = 0x1;
            f1 = *(f32*)(sp + 0x10);
            f0 = *(f32*)(sp + 0x8);
            *(f32*)lbl_8047AD74 = f2;
            *(f32*)lbl_8047AD78 = f1;
            *(f32*)lbl_8047AD7C = f0;
            *(u8*)&lbl_8047AD70 = r0;
        }
        f31 = *(f32*)lbl_8047AD7C;
        f30 = *(f32*)lbl_8047AD78;
        f1 = *(f32*)lbl_8047AD74;
        fn_80176868();
        f1 = f30;
        fn_801767E0();
        f1 = f31;
        fn_80176758();
    }
                }
    }
    }
    }
    /* psq_l f31, 0x38((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x30);
    /* psq_l f30, 0x28((u32)sp), 0, qr0 */;
    f30 = *(f64*)(sp + 0x20);
    return;
}
/* 0x80117330 | 0x194 */
void fn_80117330(void) {
    extern u8 lbl_8047CFD0[];
    extern void fn_800CE2D8();
    extern void fn_800DFF98();
    extern void fn_800E019C();
    extern void fn_800E01F4();
    extern void fn_800E0518();
    extern void fn_800E3D98();
    extern void fn_800F9318();
    extern void fn_80117514();
    extern void fn_80176684();
    extern void fn_80176690();
    extern void fn_8017669C();
    extern void fn_80177478();
    extern void fn_80177574();
    extern void fn_801776E8();
    extern void fn_80177908();
    extern void fn_8017795C();
    extern void fn_80177A38();
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f31 = 0.0f;
    *(f64*)(sp + 0x80) = f31;
    /* psq_st f31, 0x88((u32)sp), 0, qr0 */;
    f31 = f1;
    r0 = *(u8*)&lbl_8047AD71;
    if ((u32)r0 != (u32)0x0) {
        fn_80177A38();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x0) {
            r0 = *(u32*)&lbl_8047AD68;
            if ((u32)r0 != (u32)0x0) {
                r3 = 0x0;
                r4 = 0x64;
                fn_800F9318();
                if ((u32)r3 != (u32)0x0) {
                    r4 = (u32)sp + 0x44;
                    fn_800E3D98();
                } else {
                    r3 = (u32)sp + 0x44;
                    fn_80177908();
                }
                r3 = (u32)sp + 0x38;
                fn_8017795C();
                r0 = *(u32*)&lbl_8047AD68;
                if ((u32)r0 == (u32)0x1) {
                    r3 = *(u32*)&lbl_8047AD6C;
                    f0 = *(f32*)((u8*)r3 + 0x10);
                    *(f32*)(sp + 0xC) = f0;
                    f0 = *(f32*)((u8*)r3 + 0xC);
                    *(f32*)(sp + 0x10) = f0;
                    f0 = *(f32*)((u8*)r3 + 0x14);
                    *(f32*)(sp + 0x8) = f0;
                    r0 = 0x1;
                } else {
                fn_8017669C();
                *(f32*)(sp + 0xC) = f1;
                fn_80176690();
                *(f32*)(sp + 0x10) = f1;
                fn_80176684();
                *(f32*)(sp + 0x8) = f1;
                r3 = (u32)sp + 0x44;
                r4 = (u32)sp + 0xc;
                r5 = (u32)sp + 0x10;
                r6 = (u32)sp + 0x8;
                fn_80117514();
                r0 = r3 & 0xFF;
                }
    }
    }
    }
    if ((u32)r0 != (u32)0x0) {
    f1 = *(f32*)lbl_8047CFD0;
    r3 = (u32)sp + 0x14;
    f2 = *(f32*)(sp + 0xC);
    f3 = *(f32*)(sp + 0x10);
    fn_800E01F4();
    f1 = *(f32*)(sp + 0x8);
    r3 = (u32)sp + 0x50;
    fn_800E0518();
    r3 = (u32)sp + 0x14;
    r4 = (u32)sp + 0x50;
    r5 = r3;
    fn_800DFF98();
    r3 = (u32)sp + 0x2c;
    r4 = (u32)sp + 0x44;
    r5 = (u32)sp + 0x38;
    fn_800E019C();
    r3 = (u32)sp + 0x2c;
    r5 = (u32)sp + 0x14;
    r4 = r3;
    fn_800E019C();
    f0 = *(f32*)(sp + 0x8);
    f1 = *(f32*)(sp + 0xC);
    *(f32*)(sp + 0x24) = f0;
    f2 = *(f32*)(sp + 0x10);
    fn_800CE2D8();
    f2 = (f32)f1;
    f0 = *(f32*)lbl_8047CFD0;
    f1 = f31;
    r4 = (u32)sp + 0x44;
    *(f32*)(sp + 0x28) = f0;
    r3 = 0x0;
    f0 = -f2;
    *(f32*)(sp + 0x20) = f0;
    fn_801776E8();
    f1 = f31;
    r4 = (u32)sp + 0x2c;
    r3 = 0x0;
    fn_80177574();
    f1 = f31;
    r4 = (u32)sp + 0x20;
    r3 = 0x0;
    fn_80177478();
    }
    /* psq_l f31, 0x88((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x80);
    return;
}
/* 0x801174C4 | 0x28 */
u8 fn_801174C4(void) {
    u8 result = 0;
    if (lbl_8047AD68 != 0 && lbl_8047AD6C != 0) {
        result = 1;
    }
    return result;
}
/* 0x801174F4 | 0xC */
void fn_801174F4(void) {
    lbl_8047AD71 = 0;
}
/* 0x80117500 | 0x14 */
extern u8 lbl_8047AD70;
void fn_80117500(void) {
    lbl_8047AD71 = 1;
    lbl_8047AD70 = 0;
}
/* 0x801176C8 | 0x254 */
void fn_801176C8(void) {
    extern void fn_800FE714();
    extern void fn_800FE834();
    extern void fn_801155CC();
    extern void fn_80115C48();
    extern void fn_80117164();
    extern void fn_8011791C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r26 = r3;
    fn_80115C48();
    r4 = (u32)&lbl_804083D0;
    r0 = 0x0;
    r28 = (u32)&lbl_804083D0;
    r27 = r3;
    r4 = *(u32*)((u8*)r28 + 0x10);
    *(u8*)((u8*)r28 + 0x1C) = r0;
    if ((u32)r4 != (u32)0x0) {
        r3 = r4;
        fn_80115C48();
        if ((u32)r3 != (u32)0x0) {
            r4 = (u32)&lbl_804083D0;
            r4 = (u32)&lbl_804083D0;
            r0 = *(u32*)((u8*)r4 + 0x14);
            *(u32*)((u8*)r3 + 0x1C) = r0;
            fn_80117164();
    }
    }
    r3 = (u32)&lbl_804083D0;
    r29 = (u32)&lbl_804083D0;
    r3 = *(u32*)((u8*)r29 + 0x18);
    if ((u32)r3 != (u32)0x0) {
        fn_800FE714();
    }
    r3 = (u32)&lbl_804083D0;
    r30 = (u32)&lbl_804083D0;
    r25 = *(u16*)((u8*)r30 + 0xE);
    if ((u32)r25 != (u32)0x0) {
        r3 = r25;
        ((void(*)(void))fn_800E24B0)();
        r3 = r25;
        ((void(*)(void))fn_800E209C)();
    }
    r3 = (u32)&lbl_804083D0;
    r31 = (u32)&lbl_804083D0;
    r24 = *(u16*)((u8*)r31 + 0xC);
    if ((u32)r24 != (u32)0x0) {
        r3 = r24;
        ((void(*)(void))fn_800E24B0)();
        r3 = r24;
        ((void(*)(void))fn_800E209C)();
    }
    r3 = (u32)&lbl_804083D0;
    r4 = 0x0;
    r3 = (u32)&lbl_804083D0;
    r5 = 0x20;
    memset((void*)r3, (int)r4, (u32)r5);
    if ((u32)r27 != (u32)0x0) {
        r4 = (u32)fn_8011791C;
        r3 = 0x1;
        r6 = (u32)fn_8011791C;
        r5 = 0x0;
        r4 = 0x7f;
        fn_800FE834();
        *(u32*)((u8*)r29 + 0x18) = r3;
        if ((u32)r3 != (u32)0x0) {
            r3 = 0x8;
            ((void(*)(void))fn_800E3534)();
            r25 = r3;
            r0 = r25 & 0xFFFF;
            if ((u32)r0 != (u32)0x0) {
                ((void(*)(void))fn_800E27B0)();
                r4 = (u32)&lbl_804083D0;
                *(u16*)((u8*)r31 + 0xC) = r25;
                r25 = (u32)&lbl_804083D0;
                *(u32*)((u8*)r25 + 0x4) = r3;
                r3 = r27;
                fn_801155CC();
                *(u32*)((u8*)r28 + 0x10) = r26;
                r4 = (u32)&lbl_804083D0;
                r4 = (u32)&lbl_804083D0;
                r26 = r3;
                r0 = *(u32*)((u8*)r27 + 0x1C);
                r3 = *(u32*)((u8*)r25 + 0x4);
                r27 = r4 + 0x8;
                *(u32*)((u8*)r4 + 0x14) = r0;
                *(u32*)((u8*)r3 + 0x0) = r27;
                if ((u32)r26 == (u32)0x0) return;
                r3 = *(u32*)((u8*)r26 + 0x0);
                r23 = *(u32*)((u8*)r3 + 0x0);
                if ((u32)r23 == (u32)0x0) return;
                r24 = r23 * 0x18;
                r3 = r24;
                ((void(*)(void))fn_800E3534)();
                r0 = r3 & 0xFFFF;
                if ((u32)r0 != (u32)0x0) {
                    *(u16*)((u8*)r30 + 0xE) = r3;
                    *(u32*)((u8*)r27 + 0x0) = r23;
                    ((void(*)(void))fn_800E27B0)();
                    r4 = *(u32*)((u8*)r25 + 0x4);
                    r5 = r24;
                    *(u32*)((u8*)r4 + 0x4) = r3;
                    r3 = *(u32*)((u8*)r25 + 0x4);
                    r4 = *(u32*)((u8*)r26 + 0x4);
                    r3 = *(u32*)((u8*)r3 + 0x4);
                    memcpy((void*)r3, (const void*)r4, (u32)r5);
                    return;
    }
    }
    }
    }
    r3 = *(u32*)((u8*)r28 + 0x10);
    r0 = 0x0;
    *(u8*)((u8*)r28 + 0x1C) = r0;
    if ((u32)r3 != (u32)0x0) {
        fn_80115C48();
        if ((u32)r3 != (u32)0x0) {
            r4 = (u32)&lbl_804083D0;
            r4 = (u32)&lbl_804083D0;
            r0 = *(u32*)((u8*)r4 + 0x14);
            *(u32*)((u8*)r3 + 0x1C) = r0;
            fn_80117164();
    }
    }
    r3 = *(u32*)((u8*)r29 + 0x18);
    if ((u32)r3 != (u32)0x0) {
        fn_800FE714();
    }
    r25 = *(u16*)((u8*)r30 + 0xE);
    if ((u32)r25 != (u32)0x0) {
        r3 = r25;
        ((void(*)(void))fn_800E24B0)();
        r3 = r25;
        ((void(*)(void))fn_800E209C)();
    }
    r24 = *(u16*)((u8*)r31 + 0xC);
    if ((u32)r24 != (u32)0x0) {
        r3 = r24;
        ((void(*)(void))fn_800E24B0)();
        r3 = r24;
        ((void(*)(void))fn_800E209C)();
    }
    r3 = (u32)&lbl_804083D0;
    r4 = 0x0;
    r3 = (u32)&lbl_804083D0;
    r5 = 0x20;
    memset((void*)r3, (int)r4, (u32)r5);
    return;
}
/* 0x8011791C | 0x1B8 */
void fn_8011791C(void) {
    extern u8 lbl_802727B8[];
    extern void fn_800D2584();
    extern void fn_800E01D0();
    extern void fn_800FE714();
    extern void fn_80115C48();
    extern void fn_80117164();
    extern void fn_80176684();
    extern void fn_80176690();
    extern void fn_8017669C();
    extern void fn_80177A38();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    r3 = (u32)&lbl_804083D0;
    r3 = (u32)&lbl_804083D0;
    r29 = *(u32*)((u8*)r3 + 0x0);
    ((void(*)(void))fn_800FF56C)();
    fn_80115C48();
    r4 = (u32)&lbl_804083D0;
    r30 = r3;
    r31 = (u32)&lbl_804083D0;
    r3 = *(u32*)((u8*)r31 + 0x10);
    fn_80115C48();
    if ((u32)r30 != (u32)0x0 && (u32)r30 == (u32)r3) {
    r3 = (u32)&lbl_804083D0;
    r3 = (u32)&lbl_804083D0;
    r0 = *(u8*)((u8*)r3 + 0x1C);
    if ((u32)r0 == (u32)0x0 && (u32)r29 != (u32)0x0) {
        r3 = (u32)lbl_802727B8;
        r5 = (u32)lbl_802727B8;
        r4 = *(u32*)((u8*)r5 + 0x0);
        r3 = *(u32*)((u8*)r5 + 0x4);
        r0 = *(u32*)((u8*)r5 + 0x8);
        *(u32*)(sp + 0x10) = r0;
        fn_80177A38();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x5) {
            fn_800D2584();
            r4 = r3;
            if ((u32)r4 != (u32)0x0) {
                r3 = (u32)sp + 0x8;
                r4 = r4 + 0x100;
                fn_800E01D0();
                f0 = *(f32*)(sp + 0x8);
                *(f32*)((u8*)r29 + 0x0) = f0;
                f0 = *(f32*)(sp + 0xC);
                *(f32*)((u8*)r29 + 0x4) = f0;
                f0 = *(f32*)(sp + 0x10);
                *(f32*)((u8*)r29 + 0x8) = f0;
                fn_80176690();
                *(f32*)((u8*)r29 + 0xC) = f1;
                fn_8017669C();
                *(f32*)((u8*)r29 + 0x10) = f1;
                fn_80176684();
                *(f32*)((u8*)r29 + 0x14) = f1;
    }
    }
    }
    } else {
    r3 = *(u32*)((u8*)r31 + 0x10);
    r4 = (u32)&lbl_804083D0;
    r4 = (u32)&lbl_804083D0;
    r0 = 0x0;
    *(u8*)((u8*)r4 + 0x1C) = r0;
    if ((u32)r3 != (u32)0x0) {
        fn_80115C48();
        if ((u32)r3 != (u32)0x0) {
            r4 = (u32)&lbl_804083D0;
            r4 = (u32)&lbl_804083D0;
            r0 = *(u32*)((u8*)r4 + 0x14);
            *(u32*)((u8*)r3 + 0x1C) = r0;
            fn_80117164();
    }
    }
    r3 = (u32)&lbl_804083D0;
    r3 = (u32)&lbl_804083D0;
    r3 = *(u32*)((u8*)r3 + 0x18);
    if ((u32)r3 != (u32)0x0) {
        fn_800FE714();
    }
    r3 = (u32)&lbl_804083D0;
    r3 = (u32)&lbl_804083D0;
    r30 = *(u16*)((u8*)r3 + 0xE);
    if ((u32)r30 != (u32)0x0) {
        r3 = r30;
        ((void(*)(void))fn_800E24B0)();
        r3 = r30;
        ((void(*)(void))fn_800E209C)();
    }
    r3 = (u32)&lbl_804083D0;
    r3 = (u32)&lbl_804083D0;
    r30 = *(u16*)((u8*)r3 + 0xC);
    if ((u32)r30 != (u32)0x0) {
        r3 = r30;
        ((void(*)(void))fn_800E24B0)();
        r3 = r30;
        ((void(*)(void))fn_800E209C)();
    }
    r3 = (u32)&lbl_804083D0;
    r4 = 0x0;
    r3 = (u32)&lbl_804083D0;
    r5 = 0x20;
    memset((void*)r3, (int)r4, (u32)r5);
    }
    r31 = *(u32*)(sp + 0x2C);
    r30 = *(u32*)(sp + 0x28);
    r29 = *(u32*)(sp + 0x24);
    return;
}
/* 0x80117AD4 | 16 bytes | global_getter */
u32 fn_80117AD4(void) {
    return *(u32*)((u8*)lbl_804083D0 + 0x10);
}
/* 0x80117AE4 | 0x1A0 */
void fn_80117AE4(void) {
    extern u8 lbl_80478B40[];
    extern u8 lbl_8047AD80[];
    extern u8 lbl_8047AD84[];
    extern u8 lbl_8047AD88[];
    extern u8 lbl_8047AD8C[];
    extern u8 lbl_8047AD90[];
    extern u8 lbl_8047AD94[];
    extern void fn_800E4014();
    extern void fn_800E4BF4();
    extern void fn_800E5550();
    extern void fn_800E563C();
    extern void fn_800EC188();
    extern void fn_800EC990();
    extern void fn_800ECCA8();
    extern void fn_800EF5A4();
    extern void fn_800EF5FC();
    extern void fn_800F9318();
    extern void fn_80113D34();
    extern void fn_80113F48();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;
    r31 = r3;
    r0 = *(u32*)lbl_80478B40;
    if ((s32)r0 == (s32)r31) return;
    r30 = *(u32*)lbl_8047AD88;
    if ((u32)r30 != (u32)0x0) {
        fn_80113F48();
        r4 = *(u32*)((u8*)r30 + 0x8);
        fn_800F9318();
        fn_800E5550();
        r3 = *(u32*)lbl_8047AD8C;
        if ((u32)r3 != (u32)0x0) {
            fn_800EF5A4();
            r0 = 0x0;
            *(u32*)lbl_8047AD8C = r0;
        }
        r3 = *(u32*)lbl_8047AD90;
        if ((u32)r3 != (u32)0x0) {
            fn_800E4BF4();
            r0 = 0x0;
            *(u32*)lbl_8047AD90 = r0;
        }
        r3 = 0x0;
        r0 = -0x1;
        *(u32*)lbl_8047AD94 = r3;
        *(u32*)lbl_80478B40 = r0;
    }
    r0 = *(u32*)lbl_8047AD80;
    r4 = 0x0;
    r3 = *(u32*)lbl_8047AD84;
    *(u32*)lbl_8047AD88 = r0;
    ctr_fn = (void(*)(void))r3;
    if ((u32)r3 != (u32)0x0) {
    do {
    r3 = *(u32*)lbl_8047AD88;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((u32)r0 == (u32)r31) {
        r4 = 0x1;
        break;
    }
    r0 = r3 + 0x18;
    *(u32*)lbl_8047AD88 = r0;
    } while (--ctr != 0);
    }
    r0 = r4 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r0 = 0x0;
        r3 = 0x0;
        *(u32*)lbl_8047AD88 = r0;
        return;
    }
    r4 = *(u32*)lbl_8047AD88;
    r5 = 0x44;
    r6 = 0x0;
    r7 = 0x0;
    r3 = *(u16*)((u8*)r4 + 0x0);
    r4 = *(u16*)((u8*)r4 + 0x2);
    fn_800EF5FC();
    *(u32*)lbl_8047AD8C = r3;
    if ((u32)r3 == (u32)0x0) {
        r0 = 0x0;
        r3 = 0x0;
        *(u32*)lbl_8047AD88 = r0;
        return;
    }
    fn_80113F48();
    r4 = *(u32*)lbl_8047AD88;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_80113D34();
    *(u32*)lbl_8047AD90 = r3;
    r4 = 0x0;
    fn_800E4014();
    r3 = *(u32*)lbl_8047AD90;
    r4 = 0x1;
    fn_800EC188();
    r4 = *(u32*)lbl_8047AD88;
    r3 = *(u32*)lbl_8047AD90;
    r4 = *(u32*)((u8*)r4 + 0x10);
    fn_800ECCA8();
    r3 = *(u32*)lbl_8047AD90;
    fn_800EC990();
    fn_80113F48();
    r4 = *(u32*)lbl_8047AD88;
    r4 = *(u32*)((u8*)r4 + 0x14);
    fn_800F9318();
    *(u32*)lbl_8047AD94 = r3;
    fn_80113F48();
    r4 = *(u32*)lbl_8047AD88;
    r4 = *(u32*)((u8*)r4 + 0x8);
    fn_800F9318();
    r4 = *(u32*)lbl_8047AD8C;
    fn_800E563C();
    *(u32*)lbl_80478B40 = r31;
    r3 = 0x1;
    return;
}
/* 0x80117C84 | 0x90 */
void fn_80117C84(void) {
    extern u8 lbl_80478B40[];
    extern u8 lbl_8047AD80[];
    extern u8 lbl_8047AD84[];
    extern u8 lbl_8047AD88[];
    extern u8 lbl_8047AD8C[];
    extern u8 lbl_8047AD90[];
    extern u8 lbl_8047AD94[];
    extern void fn_800E4BF4();
    extern void fn_800E5550();
    extern void fn_800EF5A4();
    extern void fn_800F9318();
    extern void fn_80113F48();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;
    r31 = *(u32*)lbl_8047AD88;
    if ((u32)r31 != (u32)0x0) {
        fn_80113F48();
        r4 = *(u32*)((u8*)r31 + 0x8);
        fn_800F9318();
        fn_800E5550();
        r3 = *(u32*)lbl_8047AD8C;
        if ((u32)r3 != (u32)0x0) {
            fn_800EF5A4();
            r0 = 0x0;
            *(u32*)lbl_8047AD8C = r0;
        }
        r3 = *(u32*)lbl_8047AD90;
        if ((u32)r3 != (u32)0x0) {
            fn_800E4BF4();
            r0 = 0x0;
            *(u32*)lbl_8047AD90 = r0;
        }
        r3 = 0x0;
        r0 = -0x1;
        *(u32*)lbl_8047AD94 = r3;
        *(u32*)lbl_8047AD88 = r3;
        *(u32*)lbl_80478B40 = r0;
    }
    r0 = 0x0;
    *(u32*)lbl_8047AD80 = r0;
    *(u32*)lbl_8047AD84 = r0;
    return;
}
/* 0x80117D14 | 0x144 */
void fn_80117D14(void) {
    extern u8 lbl_8047AD88[];
    extern u8 lbl_8047AD8C[];
    extern u8 lbl_8047AD90[];
    extern u8 lbl_8047AD94[];
    extern void fn_800D2248();
    extern void fn_800D2584();
    extern void fn_800D258C();
    extern void fn_800D3190();
    extern void fn_800D3410();
    extern void fn_800D377C();
    extern void fn_800D4604();
    extern void fn_800D9AF0();
    extern void fn_800D9B24();
    extern void fn_800D9C24();
    extern void fn_800D9D68();
    extern void fn_800E3760();
    extern void fn_800EC134();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;
    r0 = *(u32*)lbl_8047AD88;
    if ((u32)r0 == (u32)0x0) { r31 = *(u32*)(sp + 0x1C); return; }
    r3 = *(u32*)lbl_8047AD90;
    if ((u32)r3 == (u32)0x0) {
        r31 = *(u32*)(sp + 0x1C);
        return;
    }
    fn_800EC134();
    fn_800D2584();
    r0 = r3;
    r3 = 0x2;
    r31 = r0;
    fn_800D4604();
    r3 = 0x1;
    fn_800D377C();
    r3 = *(u32*)lbl_8047AD8C;
    r4 = 0x0;
    fn_800D3410();
    r3 = (u32)sp + 0x16;
    r4 = (u32)sp + 0x14;
    r5 = (u32)sp + 0x12;
    r6 = (u32)sp + 0x10;
    fn_800D9B24();
    r3 = (u32)sp + 0xe;
    r4 = (u32)sp + 0xc;
    r5 = (u32)sp + 0xa;
    r6 = (u32)sp + 0x8;
    fn_800D9AF0();
    r3 = *(u32*)lbl_8047AD94;
    fn_800D258C();
    r5 = *(u32*)lbl_8047AD88;
    r3 = 0x0;
    r4 = 0x0;
    r6 = *(u16*)((u8*)r5 + 0x0);
    r5 = *(u16*)((u8*)r5 + 0x2);
    r5 = r6 & 0xFFFF;
    r6 = r0 & 0xFFFF;
    fn_800D9D68();
    r5 = *(u32*)lbl_8047AD88;
    r3 = 0x0;
    r4 = 0x0;
    r6 = *(u16*)((u8*)r5 + 0x0);
    r5 = *(u16*)((u8*)r5 + 0x2);
    r5 = r6 & 0xFFFF;
    r6 = r0 & 0xFFFF;
    fn_800D9C24();
    fn_800D2248();
    r3 = *(u32*)lbl_8047AD90;
    r4 = 0x3010;
    fn_800E3760();
    fn_800D3190();
    r3 = 0x1;
    fn_800D377C();
    r3 = r31;
    fn_800D258C();
    r3 = *(u16*)(sp + 0x16);
    r4 = *(u16*)(sp + 0x14);
    r5 = *(u16*)(sp + 0x12);
    r6 = *(u16*)(sp + 0x10);
    fn_800D9D68();
    r3 = *(u16*)(sp + 0xE);
    r4 = *(u16*)(sp + 0xC);
    r5 = *(u16*)(sp + 0xA);
    r6 = *(u16*)(sp + 0x8);
    fn_800D9C24();
    fn_800D2248();
    r3 = 0x1;
    fn_800D4604();
    r31 = *(u32*)(sp + 0x1C);
    return;
}
/* 0x80117E58 | 0x1C8 */
void fn_80117E58(void) {
    extern u8 lbl_80478B40[];
    extern u8 lbl_8047AD80[];
    extern u8 lbl_8047AD84[];
    extern u8 lbl_8047AD88[];
    extern u8 lbl_8047AD8C[];
    extern u8 lbl_8047AD90[];
    extern u8 lbl_8047AD94[];
    extern void fn_800E4014();
    extern void fn_800E4BF4();
    extern void fn_800E5550();
    extern void fn_800E563C();
    extern void fn_800EC188();
    extern void fn_800EC990();
    extern void fn_800ECCA8();
    extern void fn_800EF5A4();
    extern void fn_800EF5FC();
    extern void fn_800F9318();
    extern void fn_80113D34();
    extern void fn_80113F48();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;
    r4 = *(u32*)((u8*)r3 + 0x48);
    if ((u32)r4 == (u32)0x0) return;
    r4 = *(u32*)((u8*)r4 + 0x0);
    r0 = *(u32*)((u8*)r4 + 0x4);
    *(u32*)lbl_8047AD80 = r0;
    if ((u32)r0 == (u32)0x0) return;
    r3 = *(u32*)((u8*)r3 + 0x48);
    r3 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r3 + 0x0);
    r0 = *(u32*)((u8*)r3 + 0x0);
    *(u32*)lbl_8047AD84 = r0;
    if ((u32)r0 == (u32)0x0) return;
    r0 = *(u32*)lbl_80478B40;
    if ((s32)r0 == (s32)0x0) return;
    r31 = *(u32*)lbl_8047AD88;
    if ((u32)r31 != (u32)0x0) {
        fn_80113F48();
        r4 = *(u32*)((u8*)r31 + 0x8);
        fn_800F9318();
        fn_800E5550();
        r3 = *(u32*)lbl_8047AD8C;
        if ((u32)r3 != (u32)0x0) {
            fn_800EF5A4();
            r0 = 0x0;
            *(u32*)lbl_8047AD8C = r0;
        }
        r3 = *(u32*)lbl_8047AD90;
        if ((u32)r3 != (u32)0x0) {
            fn_800E4BF4();
            r0 = 0x0;
            *(u32*)lbl_8047AD90 = r0;
        }
        r3 = 0x0;
        r0 = -0x1;
        *(u32*)lbl_8047AD94 = r3;
        *(u32*)lbl_80478B40 = r0;
    }
    r0 = *(u32*)lbl_8047AD80;
    r4 = 0x0;
    r3 = *(u32*)lbl_8047AD84;
    *(u32*)lbl_8047AD88 = r0;
    ctr_fn = (void(*)(void))r3;
    if ((u32)r3 != (u32)0x0) {
    do {
    r3 = *(u32*)lbl_8047AD88;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((u32)r0 == (u32)0x0) {
        r4 = 0x1;
        break;
    }
    r0 = r3 + 0x18;
    *(u32*)lbl_8047AD88 = r0;
    } while (--ctr != 0);
    }
    r0 = r4 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r0 = 0x0;
        *(u32*)lbl_8047AD88 = r0;
        return;
    }
    r4 = *(u32*)lbl_8047AD88;
    r5 = 0x44;
    r6 = 0x0;
    r7 = 0x0;
    r3 = *(u16*)((u8*)r4 + 0x0);
    r4 = *(u16*)((u8*)r4 + 0x2);
    fn_800EF5FC();
    *(u32*)lbl_8047AD8C = r3;
    if ((u32)r3 == (u32)0x0) {
        r0 = 0x0;
        *(u32*)lbl_8047AD88 = r0;
        return;
    }
    fn_80113F48();
    r4 = *(u32*)lbl_8047AD88;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_80113D34();
    *(u32*)lbl_8047AD90 = r3;
    r4 = 0x0;
    fn_800E4014();
    r3 = *(u32*)lbl_8047AD90;
    r4 = 0x1;
    fn_800EC188();
    r4 = *(u32*)lbl_8047AD88;
    r3 = *(u32*)lbl_8047AD90;
    r4 = *(u32*)((u8*)r4 + 0x10);
    fn_800ECCA8();
    r3 = *(u32*)lbl_8047AD90;
    fn_800EC990();
    fn_80113F48();
    r4 = *(u32*)lbl_8047AD88;
    r4 = *(u32*)((u8*)r4 + 0x14);
    fn_800F9318();
    *(u32*)lbl_8047AD94 = r3;
    fn_80113F48();
    r4 = *(u32*)lbl_8047AD88;
    r4 = *(u32*)((u8*)r4 + 0x8);
    fn_800F9318();
    r4 = *(u32*)lbl_8047AD8C;
    fn_800E563C();
    r0 = 0x0;
    *(u32*)lbl_80478B40 = r0;
    return;
}
/* 0x48 | fn_80118020 | single_call_straight */
void fn_80118020(void) {
    fn_800FF4D4();
}
/* 0x80118070 | 0x90 */
void fn_80118070(void) {
    extern u8 lbl_80478B40[];
    extern u8 lbl_8047AD80[];
    extern u8 lbl_8047AD84[];
    extern u8 lbl_8047AD88[];
    extern u8 lbl_8047AD8C[];
    extern u8 lbl_8047AD90[];
    extern u8 lbl_8047AD94[];
    extern void fn_800E4BF4();
    extern void fn_800E5550();
    extern void fn_800EF5A4();
    extern void fn_800F9318();
    extern void fn_80113F48();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;
    r31 = *(u32*)lbl_8047AD88;
    if ((u32)r31 != (u32)0x0) {
        fn_80113F48();
        r4 = *(u32*)((u8*)r31 + 0x8);
        fn_800F9318();
        fn_800E5550();
        r3 = *(u32*)lbl_8047AD8C;
        if ((u32)r3 != (u32)0x0) {
            fn_800EF5A4();
            r0 = 0x0;
            *(u32*)lbl_8047AD8C = r0;
        }
        r3 = *(u32*)lbl_8047AD90;
        if ((u32)r3 != (u32)0x0) {
            fn_800E4BF4();
            r0 = 0x0;
            *(u32*)lbl_8047AD90 = r0;
        }
        r3 = 0x0;
        r0 = -0x1;
        *(u32*)lbl_8047AD94 = r3;
        *(u32*)lbl_8047AD88 = r3;
        *(u32*)lbl_80478B40 = r0;
    }
    r0 = 0x0;
    *(u32*)lbl_8047AD80 = r0;
    *(u32*)lbl_8047AD84 = r0;
    return;
}
/* 0x80118100 | 0x4 | void_stub */
void fn_80118100(void) {
}
/* 0x80118104 | 0xAC */
void fn_80118104(void) {
    extern void fn_800D2584();
    extern void fn_8016AB94();
    extern void fn_80173624();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r30 = r4;
    r29 = r3;
    fn_800D2584();
    if ((u32)r3 == (u32)0x0) return;
    r3 = *(u32*)((u8*)r3 + 0xC);
    fn_80173624();
    switch ((s32)r29) {
    case 0x10:
        r31 = 0x0;
        break;
    case 0x1000:
        r31 = 0x1;
        break;
    case 0x2000:
        r31 = 0x2;
        break;
    default:
        break;
    }
    r0 = r30 & 0xFF;
    if ((s32)r29 == (s32)0x2000) {
        r4 = r31;
        r3 = 0x1;
        fn_8016AB94();
        return;
    }
    r4 = r31;
    r3 = 0x2;
    fn_8016AB94();
    return;
}
/* 0x801181B0 | 0x23C */
void fn_801181B0(void) {
    extern u8 lbl_8047AD9C[];
    extern u8 lbl_8047ADA0[];
    extern void fn_800E01D0();
    extern void fn_800EC160();
    extern void fn_80169484();
    extern void fn_801694A8();
    extern void fn_801695FC();
    extern void fn_80175A1C();
    extern void fn_801694E0();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;
    r29 = 0x0;
    r30 = 0x0;
    while (r0 = *(u32*)lbl_8047ADA0, (u32)r29 < (u32)r0) {

    r0 = *(u32*)lbl_8047AD9C;
    r3 = r0 + r30;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x1) {
        r28 = 0x0;
        r31 = r3;
        do {
            r27 = *(u32*)((u8*)r31 + 0x8);
            if ((u32)r27 != (u32)0x0) {
                r0 = *(u8*)((u8*)r27 + 0x1);
                if ((u32)r0 == (u32)0x0) {
                    if ((u32)r0 == (u32)0x1) {
                        r3 = -0x1;
                    } else {
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        ((void(*)(void))fn_801694E0)();
                    }
                    if ((u32)r3 == (u32)0x0) {
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        fn_801694A8();
                        if ((u32)r3 == (u32)0x0) {
                            r4 = *(u32*)((u8*)r27 + 0x10);
                            r3 = *(u16*)((u8*)r4 + 0x18);
                            r4 = *(u8*)((u8*)r4 + 0x15);
                            fn_801695FC();
                            r0 = *(u32*)((u8*)r27 + 0x44);
                            if ((s32)r0 != (s32)0x0 && (s32)r0 != (s32)0x0) {
                                r3 = *(u32*)((u8*)r27 + 0x48);
                                r4 = 0x0;
                                fn_800EC160();
                                r0 = 0x0;
                                *(u32*)((u8*)r27 + 0x48) = r0;
                                *(u32*)((u8*)r27 + 0x4C) = r0;
                                *(u8*)((u8*)r27 + 0x6) = r0;
                                *(u8*)((u8*)r27 + 0x5) = r0;
                                r3 = *(u32*)((u8*)r27 + 0x10);
                                fn_80169484();
                                r0 = 0x0;
                                *(u32*)((u8*)r27 + 0x44) = r0;
                                r0 = *(u32*)((u8*)r27 + 0x44);
                                if ((s32)r0 == (s32)0x0) {
                                    r3 = r27 + 0x14;
                                    r4 = r27 + 0x50;
                                    fn_800E01D0();
                                    f0 = *(f32*)((u8*)r27 + 0x50);
                                    r3 = *(u32*)((u8*)r27 + 0x10);
                                    *(f32*)((u8*)r3 + 0x20) = f0;
                                    f0 = *(f32*)((u8*)r27 + 0x54);
                                    r3 = *(u32*)((u8*)r27 + 0x10);
                                    *(f32*)((u8*)r3 + 0x24) = f0;
                                    f0 = *(f32*)((u8*)r27 + 0x58);
                                    r3 = *(u32*)((u8*)r27 + 0x10);
                                    *(f32*)((u8*)r3 + 0x28) = f0;
                                } else {
                                    r3 = r27 + 0x50;
                                    r4 = r3;
                                    fn_800E01D0();
                                }
                                r0 = *(u32*)((u8*)r27 + 0x44);
                                if ((s32)r0 == (s32)0x0) {
                                    r3 = r27 + 0x20;
                                    r4 = r27 + 0x5c;
                                    fn_800E01D0();
                                    f0 = *(f32*)((u8*)r27 + 0x5C);
                                    r3 = *(u32*)((u8*)r27 + 0x10);
                                    *(f32*)((u8*)r3 + 0x8C) = f0;
                                    f0 = *(f32*)((u8*)r27 + 0x60);
                                    r3 = *(u32*)((u8*)r27 + 0x10);
                                    *(f32*)((u8*)r3 + 0x90) = f0;
                                    f0 = *(f32*)((u8*)r27 + 0x64);
                                    r3 = *(u32*)((u8*)r27 + 0x10);
                                    *(f32*)((u8*)r3 + 0x94) = f0;
                                } else {
                                    r3 = r27 + 0x5c;
                                    r4 = r3;
                                    fn_800E01D0();
                                }
                                r0 = *(u32*)((u8*)r27 + 0x44);
                                if ((s32)r0 == (s32)0x0) {
                                r3 = r27 + 0x2c;
                                r4 = r27 + 0x68;
                                fn_800E01D0();
                                f0 = *(f32*)((u8*)r27 + 0x68);
                                r3 = *(u32*)((u8*)r27 + 0x10);
                                *(f32*)((u8*)r3 + 0x98) = f0;
                                f0 = *(f32*)((u8*)r27 + 0x6C);
                                r3 = *(u32*)((u8*)r27 + 0x10);
                                *(f32*)((u8*)r3 + 0x9C) = f0;
                                f0 = *(f32*)((u8*)r27 + 0x70);
                                r3 = *(u32*)((u8*)r27 + 0x10);
                                *(f32*)((u8*)r3 + 0xA0) = f0;
                } else {
                            r3 = r27 + 0x68;
                            r4 = r3;
                            fn_800E01D0();
                }
                            r3 = *(u32*)((u8*)r27 + 0x10);
                            fn_80175A1C();
                            r5 = *(u32*)((u8*)r27 + 0xC);
                            r0 = 0x40;
                            r3 = 0x0;
                            r4 = r5;
                            ctr_fn = (void(*)(void))r0;
                        do {
                            r0 = *(u32*)((u8*)r4 + 0x8);
                            if ((u32)r0 == (u32)r27) {
                                r3 = r3 << 2;
                                r4 = 0x0;
                                r0 = r3 + 0x8;
                                *(u32*)(r5 + r0) = r4;
                                break;
                            }
                            r4 = r4 + 0x4;
                            r3 = r3 + 0x1;
                        } while (--ctr != 0);
                            r0 = 0x0;
                            *(u8*)((u8*)r27 + 0x0) = r0;
                }
                }
                }
                }
            }
            r28 = r28 + 0x1;
            r31 = r31 + 0x4;
        } while ((u32)r28 < (u32)0x40);
    }
    r30 = r30 + 0x108;
    r29 = r29 + 0x1;
    }

    return;
}
/* 0x801183EC | 0x488 */
void fn_801183EC(void) {
    extern u8 lbl_8047AD9C[];
    extern u8 lbl_8047ADA0[];
    extern u8 lbl_8047ADB0[];
    extern void fn_800057A0();
    extern void fn_800DFEEC();
    extern void fn_800E0108();
    extern void fn_800E019C();
    extern void fn_800E01D0();
    extern void fn_800E06EC();
    extern void fn_800EE150();
    extern void fn_800EE3BC();
    extern void fn_800EE828();
    extern void fn_8016F430();
    extern void fn_80173F98();
    extern u8 jumptable_8035BB88[];
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    void (*ctr_fn)(void) = 0;
    r26 = r3;
    r29 = 0x0;
    r30 = 0x0;
    while (r0 = *(u32*)lbl_8047ADA0, (u32)r29 < (u32)r0) {

    r0 = *(u32*)lbl_8047AD9C;
    r3 = r0 + r30;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x1) {
        r28 = 0x0;
        r31 = r3;
        do {
            r27 = *(u32*)((u8*)r31 + 0x8);
            if ((u32)r27 != (u32)0x0) {
                r0 = *(u32*)((u8*)r27 + 0x44);
                if ((s32)r0 != (s32)0x0) {
                    r0 = *(u8*)((u8*)r27 + 0x6);
                    if ((u32)r0 != (u32)0x0) {
                        r3 = *(u32*)((u8*)r27 + 0x48);
                        r4 = *(u32*)((u8*)r27 + 0x4C);
                        fn_800EE150();
                        r4 = (u32)sp + 0x3c;
                        r25 = r3;
                        r5 = (u32)sp + 0x30;
                        r6 = (u32)sp + 0x24;
                        fn_800EE3BC();
                        r3 = r25;
                        fn_800EE828();
                        r3 = (u32)sp + 0x14;
                        r4 = (u32)sp + 0x30;
                        fn_800E06EC();
                        r3 = (u32)sp + 0x8;
                        r4 = (u32)sp + 0x14;
                        r5 = r27 + 0x50;
                        fn_800DFEEC();
                        r3 = (u32)sp + 0x3c;
                        r5 = (u32)sp + 0x8;
                        r4 = r3;
                        fn_800E019C();
                        r3 = (u32)sp + 0x30;
                        r5 = r27 + 0x5c;
                        r4 = r3;
                        fn_800E019C();
                        r3 = (u32)sp + 0x24;
                        r5 = r27 + 0x68;
                        r4 = r3;
                        fn_800E0108();
                        r3 = r27 + 0x14;
                        r4 = r27 + 0x50;
                        fn_800E01D0();
                        f0 = *(f32*)((u8*)r27 + 0x50);
                        r3 = r27 + 0x20;
                        r5 = *(u32*)((u8*)r27 + 0x10);
                        r4 = r27 + 0x5c;
                        *(f32*)((u8*)r5 + 0x20) = f0;
                        f0 = *(f32*)((u8*)r27 + 0x54);
                        r5 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r5 + 0x24) = f0;
                        f0 = *(f32*)((u8*)r27 + 0x58);
                        r5 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r5 + 0x28) = f0;
                        fn_800E01D0();
                        f0 = *(f32*)((u8*)r27 + 0x5C);
                        r3 = r27 + 0x2c;
                        r5 = *(u32*)((u8*)r27 + 0x10);
                        r4 = r27 + 0x68;
                        *(f32*)((u8*)r5 + 0x8C) = f0;
                        f0 = *(f32*)((u8*)r27 + 0x60);
                        r5 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r5 + 0x90) = f0;
                        f0 = *(f32*)((u8*)r27 + 0x64);
                        r5 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r5 + 0x94) = f0;
                        fn_800E01D0();
                        f0 = *(f32*)((u8*)r27 + 0x68);
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r3 + 0x98) = f0;
                        f0 = *(f32*)((u8*)r27 + 0x6C);
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r3 + 0x9C) = f0;
                        f0 = *(f32*)((u8*)r27 + 0x70);
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r3 + 0xA0) = f0;
                        r0 = *(u32*)((u8*)r27 + 0x44);
                        if ((u32)r0 <= (u32)0x7) {
                            r3 = (u32)jumptable_8035BB88;
                            r0 = r0 << 2;
                            r3 = (u32)jumptable_8035BB88;
                            r0 = *(u32*)(r3 + r0);
                            ctr_fn = (void(*)(void))r0;
                            /* indirect jump via ctr */;
                            r3 = r27 + 0x14;
                            r4 = (u32)sp + 0x3c;
                            fn_800E01D0();
                            f0 = *(f32*)(sp + 0x3C);
                            r3 = *(u32*)((u8*)r27 + 0x10);
                            *(f32*)((u8*)r3 + 0x20) = f0;
                            f0 = *(f32*)(sp + 0x40);
                            r3 = *(u32*)((u8*)r27 + 0x10);
                            *(f32*)((u8*)r3 + 0x24) = f0;
                            f0 = *(f32*)(sp + 0x44);
                            r3 = *(u32*)((u8*)r27 + 0x10);
                            *(f32*)((u8*)r3 + 0x28) = f0;
                }
                        goto L_801187CC;
                        r3 = r27 + 0x20;
                        r4 = (u32)sp + 0x30;
                        fn_800E01D0();
                        f0 = *(f32*)(sp + 0x30);
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r3 + 0x8C) = f0;
                        f0 = *(f32*)(sp + 0x34);
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r3 + 0x90) = f0;
                        f0 = *(f32*)(sp + 0x38);
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r3 + 0x94) = f0;
                        goto L_801187CC;
                        r3 = r27 + 0x2c;
                        r4 = (u32)sp + 0x24;
                        fn_800E01D0();
                        f0 = *(f32*)(sp + 0x24);
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r3 + 0x98) = f0;
                        f0 = *(f32*)(sp + 0x28);
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r3 + 0x9C) = f0;
                        f0 = *(f32*)(sp + 0x2C);
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r3 + 0xA0) = f0;
                        goto L_801187CC;
                        r3 = r27 + 0x14;
                        r4 = (u32)sp + 0x3c;
                        fn_800E01D0();
                        f0 = *(f32*)(sp + 0x3C);
                        r3 = r27 + 0x20;
                        r5 = *(u32*)((u8*)r27 + 0x10);
                        r4 = (u32)sp + 0x30;
                        *(f32*)((u8*)r5 + 0x20) = f0;
                        f0 = *(f32*)(sp + 0x40);
                        r5 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r5 + 0x24) = f0;
                        f0 = *(f32*)(sp + 0x44);
                        r5 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r5 + 0x28) = f0;
                        fn_800E01D0();
                        f0 = *(f32*)(sp + 0x30);
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r3 + 0x8C) = f0;
                        f0 = *(f32*)(sp + 0x34);
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r3 + 0x90) = f0;
                        f0 = *(f32*)(sp + 0x38);
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r3 + 0x94) = f0;
                        goto L_801187CC;
                        r3 = r27 + 0x20;
                        r4 = (u32)sp + 0x30;
                        fn_800E01D0();
                        f0 = *(f32*)(sp + 0x30);
                        r3 = r27 + 0x2c;
                        r5 = *(u32*)((u8*)r27 + 0x10);
                        r4 = (u32)sp + 0x24;
                        *(f32*)((u8*)r5 + 0x8C) = f0;
                        f0 = *(f32*)(sp + 0x34);
                        r5 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r5 + 0x90) = f0;
                        f0 = *(f32*)(sp + 0x38);
                        r5 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r5 + 0x94) = f0;
                        fn_800E01D0();
                        f0 = *(f32*)(sp + 0x24);
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r3 + 0x98) = f0;
                        f0 = *(f32*)(sp + 0x28);
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r3 + 0x9C) = f0;
                        f0 = *(f32*)(sp + 0x2C);
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r3 + 0xA0) = f0;
                        goto L_801187CC;
                        r3 = r27 + 0x14;
                        r4 = (u32)sp + 0x3c;
                        fn_800E01D0();
                        f0 = *(f32*)(sp + 0x3C);
                        r3 = r27 + 0x2c;
                        r5 = *(u32*)((u8*)r27 + 0x10);
                        r4 = (u32)sp + 0x24;
                        *(f32*)((u8*)r5 + 0x20) = f0;
                        f0 = *(f32*)(sp + 0x40);
                        r5 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r5 + 0x24) = f0;
                        f0 = *(f32*)(sp + 0x44);
                        r5 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r5 + 0x28) = f0;
                        fn_800E01D0();
                        f0 = *(f32*)(sp + 0x24);
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r3 + 0x98) = f0;
                        f0 = *(f32*)(sp + 0x28);
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r3 + 0x9C) = f0;
                        f0 = *(f32*)(sp + 0x2C);
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r3 + 0xA0) = f0;
                        goto L_801187CC;
                        r3 = r27 + 0x14;
                        r4 = (u32)sp + 0x3c;
                        fn_800E01D0();
                        f0 = *(f32*)(sp + 0x3C);
                        r3 = r27 + 0x20;
                        r5 = *(u32*)((u8*)r27 + 0x10);
                        r4 = (u32)sp + 0x30;
                        *(f32*)((u8*)r5 + 0x20) = f0;
                        f0 = *(f32*)(sp + 0x40);
                        r5 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r5 + 0x24) = f0;
                        f0 = *(f32*)(sp + 0x44);
                        r5 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r5 + 0x28) = f0;
                        fn_800E01D0();
                        f0 = *(f32*)(sp + 0x30);
                        r3 = r27 + 0x2c;
                        r5 = *(u32*)((u8*)r27 + 0x10);
                        r4 = (u32)sp + 0x24;
                        *(f32*)((u8*)r5 + 0x8C) = f0;
                        f0 = *(f32*)(sp + 0x34);
                        r5 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r5 + 0x90) = f0;
                        f0 = *(f32*)(sp + 0x38);
                        r5 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r5 + 0x94) = f0;
                        fn_800E01D0();
                        f0 = *(f32*)(sp + 0x24);
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r3 + 0x98) = f0;
                        f0 = *(f32*)(sp + 0x28);
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r3 + 0x9C) = f0;
                        f0 = *(f32*)(sp + 0x2C);
                        r3 = *(u32*)((u8*)r27 + 0x10);
                        *(f32*)((u8*)r3 + 0xA0) = f0;
                    L_801187CC: ;
                        r0 = *(u8*)((u8*)r27 + 0x5);
                        r0 = __cntlzw(r0);
                        *(u8*)((u8*)r27 + 0x6) = r0;
                }
                }
            }
            r28 = r28 + 0x1;
            r31 = r31 + 0x4;
        } while ((u32)r28 < (u32)0x40);
    }
    r30 = r30 + 0x108;
    r29 = r29 + 0x1;
    }

    r27 = 0x0;
    while ((u32)r27 < (u32)r26) {
        r3 = 0x0;
        fn_8016F430();
        r3 = 0x0;
        fn_80173F98();
        r27 = r27 + 0x1;
    }
    fn_800057A0();
    if ((s32)r3 == (s32)0x2) {
        r3 = *(u8*)lbl_8047ADB0;
        r3 = r3 + 0x1;
        r0 = r3 & 0xFF;
        *(u8*)lbl_8047ADB0 = r3;
        if ((u32)r0 >= (u32)0x5) {
            r3 = 0x0;
            fn_8016F430();
            r3 = 0x0;
            fn_80173F98();
            r0 = 0x0;
            *(u8*)lbl_8047ADB0 = r0;
    }
    }
    return;
}
/* 0x80118874 | 0x1F4 */
void fn_80118874(void) {
    extern void fn_800E01D0();
    extern void fn_800EC160();
    extern void fn_80169484();
    extern void fn_801695FC();
    extern void fn_80175A1C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;
    r31 = r3;
    r29 = 0x0;
    r28 = r4;
    do {
        r30 = *(u32*)((u8*)r31 + 0x8);
        if ((u32)r30 != (u32)0x0) {
            r0 = r28 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r4 = *(u32*)((u8*)r30 + 0x10);
                r3 = *(u16*)((u8*)r4 + 0x18);
                r4 = *(u8*)((u8*)r4 + 0x15);
                fn_801695FC();
            }
            r0 = *(u32*)((u8*)r30 + 0x44);
            if ((s32)r0 != (s32)0x0 && (s32)r0 != (s32)0x0) {
                r3 = *(u32*)((u8*)r30 + 0x48);
                r4 = 0x0;
                fn_800EC160();
                r0 = 0x0;
                *(u32*)((u8*)r30 + 0x48) = r0;
                *(u32*)((u8*)r30 + 0x4C) = r0;
                *(u8*)((u8*)r30 + 0x6) = r0;
                *(u8*)((u8*)r30 + 0x5) = r0;
                r3 = *(u32*)((u8*)r30 + 0x10);
                fn_80169484();
                r0 = 0x0;
                *(u32*)((u8*)r30 + 0x44) = r0;
                r0 = *(u32*)((u8*)r30 + 0x44);
                if ((s32)r0 == (s32)0x0) {
                    r3 = r30 + 0x14;
                    r4 = r30 + 0x50;
                    fn_800E01D0();
                    f0 = *(f32*)((u8*)r30 + 0x50);
                    r3 = *(u32*)((u8*)r30 + 0x10);
                    *(f32*)((u8*)r3 + 0x20) = f0;
                    f0 = *(f32*)((u8*)r30 + 0x54);
                    r3 = *(u32*)((u8*)r30 + 0x10);
                    *(f32*)((u8*)r3 + 0x24) = f0;
                    f0 = *(f32*)((u8*)r30 + 0x58);
                    r3 = *(u32*)((u8*)r30 + 0x10);
                    *(f32*)((u8*)r3 + 0x28) = f0;
                } else {
                    r3 = r30 + 0x50;
                    r4 = r3;
                    fn_800E01D0();
                }
                r0 = *(u32*)((u8*)r30 + 0x44);
                if ((s32)r0 == (s32)0x0) {
                    r3 = r30 + 0x20;
                    r4 = r30 + 0x5c;
                    fn_800E01D0();
                    f0 = *(f32*)((u8*)r30 + 0x5C);
                    r3 = *(u32*)((u8*)r30 + 0x10);
                    *(f32*)((u8*)r3 + 0x8C) = f0;
                    f0 = *(f32*)((u8*)r30 + 0x60);
                    r3 = *(u32*)((u8*)r30 + 0x10);
                    *(f32*)((u8*)r3 + 0x90) = f0;
                    f0 = *(f32*)((u8*)r30 + 0x64);
                    r3 = *(u32*)((u8*)r30 + 0x10);
                    *(f32*)((u8*)r3 + 0x94) = f0;
                } else {
                    r3 = r30 + 0x5c;
                    r4 = r3;
                    fn_800E01D0();
                }
                r0 = *(u32*)((u8*)r30 + 0x44);
                if ((s32)r0 == (s32)0x0) {
                r3 = r30 + 0x2c;
                r4 = r30 + 0x68;
                fn_800E01D0();
                f0 = *(f32*)((u8*)r30 + 0x68);
                r3 = *(u32*)((u8*)r30 + 0x10);
                *(f32*)((u8*)r3 + 0x98) = f0;
                f0 = *(f32*)((u8*)r30 + 0x6C);
                r3 = *(u32*)((u8*)r30 + 0x10);
                *(f32*)((u8*)r3 + 0x9C) = f0;
                f0 = *(f32*)((u8*)r30 + 0x70);
                r3 = *(u32*)((u8*)r30 + 0x10);
                *(f32*)((u8*)r3 + 0xA0) = f0;
            } else {
            r3 = r30 + 0x68;
            r4 = r3;
            fn_800E01D0();
            }
            r3 = *(u32*)((u8*)r30 + 0x10);
            fn_80175A1C();
            r5 = *(u32*)((u8*)r30 + 0xC);
            r0 = 0x40;
            r3 = 0x0;
            r4 = r5;
            ctr_fn = (void(*)(void))r0;
        do {
            r0 = *(u32*)((u8*)r4 + 0x8);
            if ((u32)r0 == (u32)r30) {
                r3 = r3 << 2;
                r4 = 0x0;
                r0 = r3 + 0x8;
                *(u32*)(r5 + r0) = r4;
                break;
            }
            r4 = r4 + 0x4;
            r3 = r3 + 0x1;
        } while (--ctr != 0);
            r0 = 0x0;
            *(u8*)((u8*)r30 + 0x0) = r0;
            }
        }
        r29 = r29 + 0x1;
        r31 = r31 + 0x4;
    } while ((u32)r29 < (u32)0x40);
    return;
}
/* 0x80118A68 | 0x1B8 */
void fn_80118A68(void) {
    extern void fn_800E01D0();
    extern void fn_800EC160();
    extern void fn_80169484();
    extern void fn_801695FC();
    extern void fn_80175A1C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;
    r0 = r4 & 0xFF;
    r31 = r3;
    if ((u32)r0 == (u32)0x1) {
        r4 = *(u32*)((u8*)r31 + 0x10);
        r3 = *(u16*)((u8*)r4 + 0x18);
        r4 = *(u8*)((u8*)r4 + 0x15);
        fn_801695FC();
    }
    r0 = *(u32*)((u8*)r31 + 0x44);
    if ((s32)r0 != (s32)0x0 && (s32)r0 != (s32)0x0) {
        r3 = *(u32*)((u8*)r31 + 0x48);
        r4 = 0x0;
        fn_800EC160();
        r0 = 0x0;
        *(u32*)((u8*)r31 + 0x48) = r0;
        *(u32*)((u8*)r31 + 0x4C) = r0;
        *(u8*)((u8*)r31 + 0x6) = r0;
        *(u8*)((u8*)r31 + 0x5) = r0;
        r3 = *(u32*)((u8*)r31 + 0x10);
        fn_80169484();
        r0 = 0x0;
        *(u32*)((u8*)r31 + 0x44) = r0;
        r0 = *(u32*)((u8*)r31 + 0x44);
        if ((s32)r0 == (s32)0x0) {
            r3 = r31 + 0x14;
            r4 = r31 + 0x50;
            fn_800E01D0();
            f0 = *(f32*)((u8*)r31 + 0x50);
            r3 = *(u32*)((u8*)r31 + 0x10);
            *(f32*)((u8*)r3 + 0x20) = f0;
            f0 = *(f32*)((u8*)r31 + 0x54);
            r3 = *(u32*)((u8*)r31 + 0x10);
            *(f32*)((u8*)r3 + 0x24) = f0;
            f0 = *(f32*)((u8*)r31 + 0x58);
            r3 = *(u32*)((u8*)r31 + 0x10);
            *(f32*)((u8*)r3 + 0x28) = f0;
        } else {
            r3 = r31 + 0x50;
            r4 = r3;
            fn_800E01D0();
        }
        r0 = *(u32*)((u8*)r31 + 0x44);
        if ((s32)r0 == (s32)0x0) {
            r3 = r31 + 0x20;
            r4 = r31 + 0x5c;
            fn_800E01D0();
            f0 = *(f32*)((u8*)r31 + 0x5C);
            r3 = *(u32*)((u8*)r31 + 0x10);
            *(f32*)((u8*)r3 + 0x8C) = f0;
            f0 = *(f32*)((u8*)r31 + 0x60);
            r3 = *(u32*)((u8*)r31 + 0x10);
            *(f32*)((u8*)r3 + 0x90) = f0;
            f0 = *(f32*)((u8*)r31 + 0x64);
            r3 = *(u32*)((u8*)r31 + 0x10);
            *(f32*)((u8*)r3 + 0x94) = f0;
        } else {
            r3 = r31 + 0x5c;
            r4 = r3;
            fn_800E01D0();
        }
        r0 = *(u32*)((u8*)r31 + 0x44);
        if ((s32)r0 == (s32)0x0) {
        r3 = r31 + 0x2c;
        r4 = r31 + 0x68;
        fn_800E01D0();
        f0 = *(f32*)((u8*)r31 + 0x68);
        r3 = *(u32*)((u8*)r31 + 0x10);
        *(f32*)((u8*)r3 + 0x98) = f0;
        f0 = *(f32*)((u8*)r31 + 0x6C);
        r3 = *(u32*)((u8*)r31 + 0x10);
        *(f32*)((u8*)r3 + 0x9C) = f0;
        f0 = *(f32*)((u8*)r31 + 0x70);
        r3 = *(u32*)((u8*)r31 + 0x10);
        *(f32*)((u8*)r3 + 0xA0) = f0;
    } else {
    r3 = r31 + 0x68;
    r4 = r3;
    fn_800E01D0();
    }
    r3 = *(u32*)((u8*)r31 + 0x10);
    fn_80175A1C();
    r5 = *(u32*)((u8*)r31 + 0xC);
    r0 = 0x40;
    r3 = 0x0;
    r4 = r5;
    ctr_fn = (void(*)(void))r0;
    do {
    r0 = *(u32*)((u8*)r4 + 0x8);
    if ((u32)r0 == (u32)r31) {
        r0 = r3 << 2;
        r4 = 0x0;
        r3 = r5 + r0;
        *(u32*)((u8*)r3 + 0x8) = r4;
        break;
    }
    r4 = r4 + 0x4;
    r3 = r3 + 0x1;
    } while (--ctr != 0);
    r0 = 0x0;
    *(u8*)((u8*)r31 + 0x0) = r0;
    }
    return;
}
/* 0x68 | fn_80118C20 | guarded_call */
void fn_80118C20(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    if (0 /* guard r3 == 0 */) { return; }
    fn_80169034();
}
/* 0x80118C88 | 36 bytes | load_then_call */
void fn_80118C88(void* obj) {
    fn_80169034(*(void**)((u8*)obj + 0x10));
}
/* 0x80118CAC | 36 bytes | load_then_call */
void fn_80118CAC(void* obj) {
    fn_80169340(*(void**)((u8*)obj + 0x10));
}
/* 0x80118CD0 | 36 bytes | load_then_call */
void fn_80118CD0(void* obj) {
    fn_80169368(*(void**)((u8*)obj + 0x10));
}
/* 0x80118CF4 | 36 bytes | load_then_call */
void fn_80118CF4(void* obj) {
    fn_80169390(*(void**)((u8*)obj + 0x10));
}
/* 0x80118D18 | 36 bytes | load_then_call */
void fn_80118D18(void* obj) {
    fn_801693DC(*(void**)((u8*)obj + 0x10));
}
/* 0x80118D3C | 36 bytes | load_then_call */
void fn_80118D3C(void* obj) {
    fn_80169404(*(void**)((u8*)obj + 0x10));
}
/* 0x80118D60 | 36 bytes | load_then_call */
void fn_80118D60(void* obj) {
    fn_8016945C(*(void**)((u8*)obj + 0x10));
}
/* 0x80118D84 | 36 bytes | load_then_call */
void fn_80118D84(void* obj) {
    fn_801694A8(*(void**)((u8*)obj + 0x10));
}
/* 0x80118DA8 | 0x38 */
extern s32 fn_801694E0(u32);
s32 fn_80118DA8(u8* ptr) {
    if (ptr[1] == 1) { return -1; }
    return fn_801694E0(*(u32*)(&ptr[0x10]));
}
/* 0x80118DE0 | 0xAC */
void fn_80118DE0(void) {
    extern void fn_800E01D0();
    extern void fn_80169104();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    r31 = r6;
    r30 = r5;
    r29 = r4;
    r28 = r3;
    r0 = *(u32*)((u8*)r3 + 0x44);
    if ((s32)r0 == (s32)0x0) {
        r3 = r28 + 0x2c;
        fn_800E01D0();
        f0 = *(f32*)((u8*)r29 + 0x0);
        r3 = *(u32*)((u8*)r28 + 0x10);
        *(f32*)((u8*)r3 + 0x98) = f0;
        f0 = *(f32*)((u8*)r29 + 0x4);
        r3 = *(u32*)((u8*)r28 + 0x10);
        *(f32*)((u8*)r3 + 0x9C) = f0;
        f0 = *(f32*)((u8*)r29 + 0x8);
        r3 = *(u32*)((u8*)r28 + 0x10);
        *(f32*)((u8*)r3 + 0xA0) = f0;
    } else {
        r3 = r28 + 0x68;
        fn_800E01D0();
    }
    r0 = r30 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = *(u32*)((u8*)r28 + 0x10);
        r4 = r29;
        r5 = r31;
        fn_80169104();
    }
    return;
}
/* 0x78 | fn_80118E8C | two_call_arg_check */
void fn_80118E8C(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    if (arg1 != 0) { return; }
    fn_800E01D0();
    fn_800E01D0();
}
/* 0x78 | fn_80118F04 | two_call_arg_check */
void fn_80118F04(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    if (arg1 != 0) { return; }
    fn_800E01D0();
    fn_800E01D0();
}
/* 0x80118F7C | 0x34 */
void fn_80118F7C(u8* obj, void* arg) {
    f32 f1 = *(f32*)(&obj[0x38]);
    f32 f2 = *(f32*)(&obj[0x3C]);
    f32 f3 = *(f32*)(&obj[0x40]);
    fn_800E01F4(arg, f1, f2, f3);
}
/* 0x80118FB0 | 0x12C */
void fn_80118FB0(void) {
    extern u8 lbl_8047CFE8[];
    extern u8 lbl_8047CFEC[];
    extern void fn_800E01D0();
    extern void fn_800E01F4();
    extern void fn_80169494();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r31 = r8;
    r0 = *(u32*)((u8*)r3 + 0x44);
    if (((s32)r0 == (s32)0x0) && ((s32)r29 != (s32)0x0)) {
        r3 = *(u32*)((u8*)r28 + 0x4);
        r0 = r7 & 0xFF;
        *(u32*)((u8*)r27 + 0x48) = r3;
        r0 = *(u16*)((u8*)r28 + 0x2);
        *(u32*)((u8*)r27 + 0x4C) = r0;
        if ((u32)r0 == (u32)0x1) {
            f1 = *(f32*)lbl_8047CFE8;
            r3 = r27 + 0x50;
            f2 = f1;
            f3 = f1;
            fn_800E01F4();
            f1 = *(f32*)lbl_8047CFE8;
            r3 = r27 + 0x5c;
            f2 = f1;
            f3 = f1;
            fn_800E01F4();
            f1 = *(f32*)lbl_8047CFEC;
            r3 = r27 + 0x68;
            f2 = f1;
            f3 = f1;
            fn_800E01F4();
        } else {
            r3 = r27 + 0x50;
            r4 = r27 + 0x14;
            fn_800E01D0();
            r3 = r27 + 0x5c;
            r4 = r27 + 0x20;
            fn_800E01D0();
            r3 = r27 + 0x68;
            r4 = r27 + 0x2c;
            fn_800E01D0();
        }
        f1 = *(f32*)lbl_8047CFE8;
        r3 = r27 + 0x14;
        f2 = f1;
        f3 = f1;
        fn_800E01F4();
        f1 = *(f32*)lbl_8047CFE8;
        r3 = r27 + 0x20;
        f2 = f1;
        f3 = f1;
        fn_800E01F4();
        f1 = *(f32*)lbl_8047CFEC;
        r3 = r27 + 0x2c;
        f2 = f1;
        f3 = f1;
        fn_800E01F4();
        r0 = r31 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = *(u32*)((u8*)r27 + 0x10);
            r4 = *(u32*)((u8*)r28 + 0x8);
            fn_80169494();
        }
        *(u32*)((u8*)r27 + 0x44) = r29;
        r0 = 0x1;
        *(u8*)((u8*)r27 + 0x5) = r30;
        *(u8*)((u8*)r27 + 0x6) = r0;
    }
    return;
}
/* 0x801190DC | 0x2E0 */
void fn_801190DC(void) {
    extern u8 lbl_8047ADA8[];
    extern u8 lbl_8047ADAC[];
    extern void fn_800D3094();
    extern void fn_800E01F4();
    extern void fn_80119930();
    extern void fn_80173718();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r0 = *(u32*)lbl_8047ADAC;
    r31 = *(u32*)lbl_8047ADA8;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r0 > (u32)0x0) {
    L_80119120: ;
        r0 = *(u8*)((u8*)r31 + 0x0);
        if ((u32)r0 == (u32)0x0) {
            goto L_8011913C;
        }
        r31 = r31 + 0x74;
        if (--ctr != 0) goto L_80119120;
    }
    r31 = 0x0;
L_8011913C: ;
    if ((u32)r31 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r0 = 0x8;
    r4 = r28;
    r3 = 0x0;
    ctr_fn = (void(*)(void))r0;
L_8011915C: ;
    r0 = *(u32*)((u8*)r4 + 0x8);
    if ((u32)r0 == (u32)0x0) {
        goto L_80119208;
    }
    r0 = *(u32*)((u8*)r4 + 0xC);
    r3 = r3 + 0x1;
    if ((u32)r0 == (u32)0x0) {
        goto L_80119208;
    }
    r0 = *(u32*)((u8*)r4 + 0x10);
    r3 = r3 + 0x1;
    if ((u32)r0 == (u32)0x0) {
        goto L_80119208;
    }
    r0 = *(u32*)((u8*)r4 + 0x14);
    r3 = r3 + 0x1;
    if ((u32)r0 == (u32)0x0) {
        goto L_80119208;
    }
    r0 = *(u32*)((u8*)r4 + 0x18);
    r3 = r3 + 0x1;
    if ((u32)r0 == (u32)0x0) {
        goto L_80119208;
    }
    r0 = *(u32*)((u8*)r4 + 0x1C);
    r3 = r3 + 0x1;
    if ((u32)r0 == (u32)0x0) {
        goto L_80119208;
    }
    r0 = *(u32*)((u8*)r4 + 0x20);
    r3 = r3 + 0x1;
    if ((u32)r0 == (u32)0x0) {
        goto L_80119208;
    }
    r0 = *(u32*)((u8*)r4 + 0x24);
    r3 = r3 + 0x1;
    if ((u32)r0 == (u32)0x0) {
        goto L_80119208;
    }
    r4 = r4 + 0x20;
    r3 = r3 + 0x1;
    if (--ctr != 0) goto L_8011915C;
    r3 = -0x1;
L_80119208: ;
    r27 = r3;
    if ((s32)r3 == (s32)-0x1) {
        r3 = r28;
        fn_80119930();
        r0 = 0x8;
        r4 = r28;
        r3 = 0x0;
        ctr_fn = (void(*)(void))r0;
    L_8011922C: ;
        r0 = *(u32*)((u8*)r4 + 0x8);
        if ((u32)r0 == (u32)0x0) {
            goto L_801192D8;
        }
        r0 = *(u32*)((u8*)r4 + 0xC);
        r3 = r3 + 0x1;
        if ((u32)r0 == (u32)0x0) {
            goto L_801192D8;
        }
        r0 = *(u32*)((u8*)r4 + 0x10);
        r3 = r3 + 0x1;
        if ((u32)r0 == (u32)0x0) {
            goto L_801192D8;
        }
        r0 = *(u32*)((u8*)r4 + 0x14);
        r3 = r3 + 0x1;
        if ((u32)r0 == (u32)0x0) {
            goto L_801192D8;
        }
        r0 = *(u32*)((u8*)r4 + 0x18);
        r3 = r3 + 0x1;
        if ((u32)r0 == (u32)0x0) {
            goto L_801192D8;
        }
        r0 = *(u32*)((u8*)r4 + 0x1C);
        r3 = r3 + 0x1;
        if ((u32)r0 == (u32)0x0) {
            goto L_801192D8;
        }
        r0 = *(u32*)((u8*)r4 + 0x20);
        r3 = r3 + 0x1;
        if ((u32)r0 == (u32)0x0) {
            goto L_801192D8;
        }
        r0 = *(u32*)((u8*)r4 + 0x24);
        r3 = r3 + 0x1;
        if ((u32)r0 == (u32)0x0) {
            goto L_801192D8;
        }
        r4 = r4 + 0x20;
        r3 = r3 + 0x1;
        if (--ctr != 0) goto L_8011922C;
        r3 = -0x1;
    L_801192D8: ;
        r27 = r3;
        if ((s32)r3 == (s32)-0x1) {
            r3 = 0x0;
            return;
        }
    }
    r0 = r30 & 0xFF;
    r3 = 0x0;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x1;
    }
    r4 = *(u8*)((u8*)r28 + 0x1);
    r5 = r29;
    fn_80173718();
    *(u32*)((u8*)r31 + 0x10) = r3;
    r3 = *(u32*)((u8*)r31 + 0x10);
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r4 = 0x1;
    r0 = 0x0;
    *(u8*)((u8*)r3 + 0x17) = r4;
    r3 = r31 + 0x38;
    *(u8*)((u8*)r31 + 0x0) = r4;
    *(u32*)((u8*)r31 + 0xC) = r28;
    *(u16*)((u8*)r31 + 0x2) = r29;
    *(u32*)((u8*)r31 + 0x44) = r0;
    *(u32*)((u8*)r31 + 0x48) = r0;
    *(u32*)((u8*)r31 + 0x4C) = r0;
    *(u8*)((u8*)r31 + 0x5) = r0;
    *(u8*)((u8*)r31 + 0x6) = r0;
    r4 = *(u32*)((u8*)r31 + 0x10);
    f1 = *(f32*)((u8*)r4 + 0x20);
    f2 = *(f32*)((u8*)r4 + 0x24);
    f3 = *(f32*)((u8*)r4 + 0x28);
    fn_800E01F4();
    *(u8*)((u8*)r31 + 0x4) = r30;
    fn_800D3094();
    r0 = r27 << 2;
    *(u32*)((u8*)r31 + 0x8) = r3;
    r3 = r28 + r0;
    *(u32*)((u8*)r3 + 0x8) = r31;
    r3 = *(u32*)((u8*)r31 + 0x10);
    r0 = *(u16*)((u8*)r3 + 0x10);
    if ((u32)r0 == (u32)0x0) {
        r0 = 0x1;
        *(u8*)((u8*)r31 + 0x1) = r0;
    } else {
        r0 = 0x0;
        *(u8*)((u8*)r31 + 0x1) = r0;
    }
    r3 = r31;
    return;
}
/* 0x801193BC | 0x1F0 */
void fn_801193BC(void) {
    extern void fn_800E01D0();
    extern void fn_800EC160();
    extern void fn_80169484();
    extern void fn_801695FC();
    extern void fn_80175A1C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;
    r31 = 0x0;
    r28 = r3;
    r30 = r28;
    do {
        r29 = *(u32*)((u8*)r30 + 0x8);
        if ((u32)r29 != (u32)0x0) {
            r4 = *(u32*)((u8*)r29 + 0x10);
            r3 = *(u16*)((u8*)r4 + 0x18);
            r4 = *(u8*)((u8*)r4 + 0x15);
            fn_801695FC();
            r0 = *(u32*)((u8*)r29 + 0x44);
            if ((s32)r0 != (s32)0x0 && (s32)r0 != (s32)0x0) {
                r3 = *(u32*)((u8*)r29 + 0x48);
                r4 = 0x0;
                fn_800EC160();
                r0 = 0x0;
                *(u32*)((u8*)r29 + 0x48) = r0;
                *(u32*)((u8*)r29 + 0x4C) = r0;
                *(u8*)((u8*)r29 + 0x6) = r0;
                *(u8*)((u8*)r29 + 0x5) = r0;
                r3 = *(u32*)((u8*)r29 + 0x10);
                fn_80169484();
                r0 = 0x0;
                *(u32*)((u8*)r29 + 0x44) = r0;
                r0 = *(u32*)((u8*)r29 + 0x44);
                if ((s32)r0 == (s32)0x0) {
                    r3 = r29 + 0x14;
                    r4 = r29 + 0x50;
                    fn_800E01D0();
                    f0 = *(f32*)((u8*)r29 + 0x50);
                    r3 = *(u32*)((u8*)r29 + 0x10);
                    *(f32*)((u8*)r3 + 0x20) = f0;
                    f0 = *(f32*)((u8*)r29 + 0x54);
                    r3 = *(u32*)((u8*)r29 + 0x10);
                    *(f32*)((u8*)r3 + 0x24) = f0;
                    f0 = *(f32*)((u8*)r29 + 0x58);
                    r3 = *(u32*)((u8*)r29 + 0x10);
                    *(f32*)((u8*)r3 + 0x28) = f0;
                } else {
                    r3 = r29 + 0x50;
                    r4 = r3;
                    fn_800E01D0();
                }
                r0 = *(u32*)((u8*)r29 + 0x44);
                if ((s32)r0 == (s32)0x0) {
                    r3 = r29 + 0x20;
                    r4 = r29 + 0x5c;
                    fn_800E01D0();
                    f0 = *(f32*)((u8*)r29 + 0x5C);
                    r3 = *(u32*)((u8*)r29 + 0x10);
                    *(f32*)((u8*)r3 + 0x8C) = f0;
                    f0 = *(f32*)((u8*)r29 + 0x60);
                    r3 = *(u32*)((u8*)r29 + 0x10);
                    *(f32*)((u8*)r3 + 0x90) = f0;
                    f0 = *(f32*)((u8*)r29 + 0x64);
                    r3 = *(u32*)((u8*)r29 + 0x10);
                    *(f32*)((u8*)r3 + 0x94) = f0;
                } else {
                    r3 = r29 + 0x5c;
                    r4 = r3;
                    fn_800E01D0();
                }
                r0 = *(u32*)((u8*)r29 + 0x44);
                if ((s32)r0 == (s32)0x0) {
                r3 = r29 + 0x2c;
                r4 = r29 + 0x68;
                fn_800E01D0();
                f0 = *(f32*)((u8*)r29 + 0x68);
                r3 = *(u32*)((u8*)r29 + 0x10);
                *(f32*)((u8*)r3 + 0x98) = f0;
                f0 = *(f32*)((u8*)r29 + 0x6C);
                r3 = *(u32*)((u8*)r29 + 0x10);
                *(f32*)((u8*)r3 + 0x9C) = f0;
                f0 = *(f32*)((u8*)r29 + 0x70);
                r3 = *(u32*)((u8*)r29 + 0x10);
                *(f32*)((u8*)r3 + 0xA0) = f0;
            } else {
            r3 = r29 + 0x68;
            r4 = r3;
            fn_800E01D0();
            }
            r3 = *(u32*)((u8*)r29 + 0x10);
            fn_80175A1C();
            r5 = *(u32*)((u8*)r29 + 0xC);
            r0 = 0x40;
            r3 = 0x0;
            r4 = r5;
            ctr_fn = (void(*)(void))r0;
        do {
            r0 = *(u32*)((u8*)r4 + 0x8);
            if ((u32)r0 == (u32)r29) {
                r3 = r3 << 2;
                r4 = 0x0;
                r0 = r3 + 0x8;
                *(u32*)(r5 + r0) = r4;
                break;
            }
            r4 = r4 + 0x4;
            r3 = r3 + 0x1;
        } while (--ctr != 0);
            r0 = 0x0;
            *(u8*)((u8*)r29 + 0x0) = r0;
            }
        }
        r31 = r31 + 0x1;
        r30 = r30 + 0x4;
    } while ((u32)r31 < (u32)0x40);
    r0 = 0x0;
    *(u8*)((u8*)r28 + 0x0) = r0;
    return;
}
/* 0x801195AC | 0x278 */
void fn_801195AC(void) {
    extern u8 lbl_802727D8[];
    extern u8 lbl_8047AD9C[];
    extern u8 lbl_8047ADA0[];
    extern void fn_8016A17C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;
    r5 = (u32)lbl_802727D8;
    r31 = r3;
    r3 = (0x4750 << 16);
    r0 = r3 + 0x5431;
    r30 = (u32)lbl_802727D8;
    r4 = *(u32*)((u8*)r31 + 0x0);
    if ((s32)r4 != (s32)r0) {
        if ((s32)r4 < (s32)r0) {
            r0 = r3 + 0x5430;
            if ((s32)r4 >= (s32)r0) goto L_801195F4;
        }
        goto L_80119620;
    L_801195F4: ;
        r3 = r30 + 0x50;
        ((void(*)(void))fn_800DD970)();
        r3 = r30 + 0xac;
        ((void(*)(void))fn_800DD970)();
        r3 = r30 + 0x50;
        ((void(*)(void))fn_800DD970)();
        r3 = 0x0;
        return;
    L_80119620: ;
        r3 = r30 + 0x108;
        ((void(*)(void))fn_800DD970)();
        r3 = 0x0;
        return;
    }
    r0 = *(u32*)((u8*)r31 + 0x4);
    r0 = r0 + r31;
    *(u32*)((u8*)r31 + 0x4) = r0;
    r0 = *(u32*)((u8*)r31 + 0x8);
    r0 = r0 + r31;
    *(u32*)((u8*)r31 + 0x8) = r0;
    r0 = *(u32*)((u8*)r31 + 0x10);
    r0 = r0 + r31;
    *(u32*)((u8*)r31 + 0x10) = r0;
    r3 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u16*)((u8*)r3 + 0x0);
    if ((s32)r5 != (s32)0x43) {
        r3 = r30 + 0x128;
        r4 = 0x43;
        ((void(*)(void))fn_800DD970)();
        r3 = 0x0;
        return;
    }
    r4 = *(u32*)((u8*)r31 + 0xC);
    r3 = *(u32*)((u8*)r31 + 0x8);
    r0 = r4 + 0x1f;
    /* clrrwi r4, r0, 5 */;
    DCFlushRange();
    r6 = *(u32*)lbl_8047AD9C;
    r7 = *(u32*)lbl_8047ADA0;
    r30 = r6;
    ctr_fn = (void(*)(void))r7;
    if ((u32)r7 > (u32)0x0) {
    L_801196AC: ;
        r0 = *(u8*)((u8*)r30 + 0x0);
        if ((u32)r0 == (u32)0x0) {
            goto L_801196C8;
        }
        r30 = r30 + 0x108;
        if (--ctr != 0) goto L_801196AC;
    }
    r30 = 0x0;
L_801196C8: ;
    if ((u32)r30 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r4 = 0x0;
    while (r0 = r4 & 0xFF, (u32)r0 < (u32)0x40) {

    r5 = r6;
    r0 = r4 & 0xFF;
    ctr_fn = (void(*)(void))r7;
    if ((u32)r7 > (u32)0x0) {
    L_801196F4: ;
        r3 = *(u8*)((u8*)r5 + 0x1);
        if ((u32)r3 == (u32)r0) {
            r0 = 0x1;
            goto L_80119714;
        }
        r5 = r5 + 0x108;
        if (--ctr != 0) goto L_801196F4;
    }
    r0 = 0x0;
L_80119714: ;
    r0 = r0 & 0xFF;
    if ((u32)r3 == (u32)r0) {
        goto L_80119734;
    }
    r4 = r4 + 0x1;
    }

    r4 = 0xff;
L_80119734: ;
    r0 = r4 & 0xFF;
    if ((u32)r0 == (u32)0xff) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;
    r0 = 0x2;
    *(u8*)((u8*)r30 + 0x0) = r3;
    r5 = r30;
    r3 = 0x0;
    *(u8*)((u8*)r30 + 0x1) = r4;
    *(u32*)((u8*)r30 + 0x4) = r31;
    ctr_fn = (void(*)(void))r0;
    do {
        *(u32*)((u8*)r5 + 0x8) = r3;
        *(u32*)((u8*)r5 + 0xC) = r3;
        *(u32*)((u8*)r5 + 0x10) = r3;
        *(u32*)((u8*)r5 + 0x14) = r3;
        *(u32*)((u8*)r5 + 0x18) = r3;
        *(u32*)((u8*)r5 + 0x1C) = r3;
        *(u32*)((u8*)r5 + 0x20) = r3;
        *(u32*)((u8*)r5 + 0x24) = r3;
        *(u32*)((u8*)r5 + 0x28) = r3;
        *(u32*)((u8*)r5 + 0x2C) = r3;
        *(u32*)((u8*)r5 + 0x30) = r3;
        *(u32*)((u8*)r5 + 0x34) = r3;
        *(u32*)((u8*)r5 + 0x38) = r3;
        *(u32*)((u8*)r5 + 0x3C) = r3;
        *(u32*)((u8*)r5 + 0x40) = r3;
        *(u32*)((u8*)r5 + 0x44) = r3;
        *(u32*)((u8*)r5 + 0x48) = r3;
        *(u32*)((u8*)r5 + 0x4C) = r3;
        *(u32*)((u8*)r5 + 0x50) = r3;
        *(u32*)((u8*)r5 + 0x54) = r3;
        *(u32*)((u8*)r5 + 0x58) = r3;
        *(u32*)((u8*)r5 + 0x5C) = r3;
        *(u32*)((u8*)r5 + 0x60) = r3;
        *(u32*)((u8*)r5 + 0x64) = r3;
        *(u32*)((u8*)r5 + 0x68) = r3;
        *(u32*)((u8*)r5 + 0x6C) = r3;
        *(u32*)((u8*)r5 + 0x70) = r3;
        *(u32*)((u8*)r5 + 0x74) = r3;
        *(u32*)((u8*)r5 + 0x78) = r3;
        *(u32*)((u8*)r5 + 0x7C) = r3;
        *(u32*)((u8*)r5 + 0x80) = r3;
        *(u32*)((u8*)r5 + 0x84) = r3;
        r5 = r5 + 0x80;
    } while (--ctr != 0);
    r3 = *(u8*)((u8*)r30 + 0x1);
    r7 = 0x0;
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r6 = *(u32*)((u8*)r31 + 0x10);
    fn_8016A17C();
    r3 = r30;
    return;
}
/* 0x80119824 | 0x10C */
void fn_80119824(void) {
    extern u8 lbl_8047AD98[];
    extern u8 lbl_8047AD9C[];
    extern u8 lbl_8047ADA0[];
    extern u8 lbl_8047ADA4[];
    extern u8 lbl_8047ADA8[];
    extern u8 lbl_8047ADAC[];
    extern void fn_8016A01C();
    extern void fn_8016AAF4();
    extern void fn_80175DF0();
    extern void fn_8019733C();
    extern void fn_8019D610();
    extern void fn_8019D618();
    extern void fn_80119BD0();
    extern void fn_8016972C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;
    r0 = r3;
    r3 = r0 * 0x108;
    r31 = r4;
    *(u32*)lbl_8047ADA0 = r0;
    ((void(*)(void))fn_800E3534)();
    r0 = r3 & 0xFFFF;
    *(u16*)lbl_8047AD98 = r3;
    if ((s32)r0 == (s32)0) return;
    r3 = r0;
    ((void(*)(void))fn_800E27B0)();
    r5 = 0x0;
    *(u32*)lbl_8047AD9C = r3;
    r4 = r5;
    r6 = 0x0;
    while (r0 = *(u32*)lbl_8047ADA0, (u32)r6 < (u32)r0) {

    r3 = *(u32*)lbl_8047AD9C;
    r6 = r6 + 0x1;
    *(u8*)(r3 + r5) = r4;
    r5 = r5 + 0x108;
    }

    r3 = r31 * 0x74;
    *(u32*)lbl_8047ADAC = r31;
    ((void(*)(void))fn_800E3534)();
    r0 = r3 & 0xFFFF;
    *(u16*)lbl_8047ADA4 = r3;
    if ((u32)r6 == (u32)r0) return;
    r3 = r0;
    ((void(*)(void))fn_800E27B0)();
    r6 = 0x0;
    *(u32*)lbl_8047ADA8 = r3;
    r5 = r6;
    r4 = r6;
    while (r0 = *(u32*)lbl_8047ADAC, (u32)r6 < (u32)r0) {

    r3 = *(u32*)lbl_8047ADA8;
    r6 = r6 + 0x1;
    *(u8*)(r3 + r5) = r4;
    r5 = r5 + 0x74;
    }

    r3 = 0x0;
    fn_8016A01C();
    r3 = 0x0;
    fn_80175DF0();
    r3 = 0x0;
    r4 = 0x74;
    fn_8016AAF4();
    r3 = (u32)fn_80119BD0;
    r3 = (u32)fn_80119BD0;
    fn_8019733C();
    r3 = (u32)fn_80119BD0;
    r3 = (u32)fn_80119BD0;
    fn_8019D618();
    r3 = (u32)fn_8016972C;
    r3 = (u32)fn_8016972C;
    fn_8019D610();
    return;
}
/* 0x80119930 | 0x2A0 */
void fn_80119930(void) {
    extern void fn_800E01D0();
    extern void fn_800EC160();
    extern void fn_80169484();
    extern void fn_801695FC();
    extern void fn_80175A1C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;
    r5 = r3;
    r8 = -0x1;
    r0 = 0x10;
    r9 = -0x1;
    r6 = 0x0;
    r4 = 0x0;
    ctr_fn = (void(*)(void))r0;
    do {
        r7 = *(u32*)((u8*)r5 + 0x8);
        r0 = *(u8*)((u8*)r7 + 0x0);
        if ((u32)r0 == (u32)0x0) {
            *(u32*)((u8*)r5 + 0x8) = r4;
            goto L_80119988;
        }
        r0 = *(u32*)((u8*)r7 + 0x8);
        if ((u32)r0 < (u32)r8) {
            r8 = r0;
            r9 = r6;
        }
    L_80119988: ;
        r7 = *(u32*)((u8*)r5 + 0xC);
        r6 = r6 + 0x1;
        r0 = *(u8*)((u8*)r7 + 0x0);
        if ((u32)r0 == (u32)0x0) {
            *(u32*)((u8*)r5 + 0xC) = r4;
            goto L_801199B8;
        }
        r0 = *(u32*)((u8*)r7 + 0x8);
        if ((u32)r0 < (u32)r8) {
            r8 = r0;
            r9 = r6;
        }
    L_801199B8: ;
        r7 = *(u32*)((u8*)r5 + 0x10);
        r6 = r6 + 0x1;
        r0 = *(u8*)((u8*)r7 + 0x0);
        if ((u32)r0 == (u32)0x0) {
            *(u32*)((u8*)r5 + 0x10) = r4;
            goto L_801199E8;
        }
        r0 = *(u32*)((u8*)r7 + 0x8);
        if ((u32)r0 < (u32)r8) {
            r8 = r0;
            r9 = r6;
        }
    L_801199E8: ;
        r7 = *(u32*)((u8*)r5 + 0x14);
        r6 = r6 + 0x1;
        r0 = *(u8*)((u8*)r7 + 0x0);
        if ((u32)r0 == (u32)0x0) {
            *(u32*)((u8*)r5 + 0x14) = r4;
            goto L_80119A18;
        }
        r0 = *(u32*)((u8*)r7 + 0x8);
        if ((u32)r0 < (u32)r8) {
            r8 = r0;
            r9 = r6;
        }
    L_80119A18: ;
        r5 = r5 + 0x10;
        r6 = r6 + 0x1;
    } while (--ctr != 0);
    if ((s32)r9 == (s32)-0x1) return;
    r0 = r9 << 2;
    r3 = r3 + r0;
    r31 = *(u32*)((u8*)r3 + 0x8);
    r4 = *(u32*)((u8*)r31 + 0x10);
    r3 = *(u16*)((u8*)r4 + 0x18);
    r4 = *(u8*)((u8*)r4 + 0x15);
    fn_801695FC();
    r0 = *(u32*)((u8*)r31 + 0x44);
    if ((s32)r0 != (s32)0x0 && (s32)r0 != (s32)0x0) {
        r3 = *(u32*)((u8*)r31 + 0x48);
        r4 = 0x0;
        fn_800EC160();
        r0 = 0x0;
        *(u32*)((u8*)r31 + 0x48) = r0;
        *(u32*)((u8*)r31 + 0x4C) = r0;
        *(u8*)((u8*)r31 + 0x6) = r0;
        *(u8*)((u8*)r31 + 0x5) = r0;
        r3 = *(u32*)((u8*)r31 + 0x10);
        fn_80169484();
        r0 = 0x0;
        *(u32*)((u8*)r31 + 0x44) = r0;
        r0 = *(u32*)((u8*)r31 + 0x44);
        if ((s32)r0 == (s32)0x0) {
            r3 = r31 + 0x14;
            r4 = r31 + 0x50;
            fn_800E01D0();
            f0 = *(f32*)((u8*)r31 + 0x50);
            r3 = *(u32*)((u8*)r31 + 0x10);
            *(f32*)((u8*)r3 + 0x20) = f0;
            f0 = *(f32*)((u8*)r31 + 0x54);
            r3 = *(u32*)((u8*)r31 + 0x10);
            *(f32*)((u8*)r3 + 0x24) = f0;
            f0 = *(f32*)((u8*)r31 + 0x58);
            r3 = *(u32*)((u8*)r31 + 0x10);
            *(f32*)((u8*)r3 + 0x28) = f0;
        } else {
            r3 = r31 + 0x50;
            r4 = r3;
            fn_800E01D0();
        }
        r0 = *(u32*)((u8*)r31 + 0x44);
        if ((s32)r0 == (s32)0x0) {
            r3 = r31 + 0x20;
            r4 = r31 + 0x5c;
            fn_800E01D0();
            f0 = *(f32*)((u8*)r31 + 0x5C);
            r3 = *(u32*)((u8*)r31 + 0x10);
            *(f32*)((u8*)r3 + 0x8C) = f0;
            f0 = *(f32*)((u8*)r31 + 0x60);
            r3 = *(u32*)((u8*)r31 + 0x10);
            *(f32*)((u8*)r3 + 0x90) = f0;
            f0 = *(f32*)((u8*)r31 + 0x64);
            r3 = *(u32*)((u8*)r31 + 0x10);
            *(f32*)((u8*)r3 + 0x94) = f0;
        } else {
            r3 = r31 + 0x5c;
            r4 = r3;
            fn_800E01D0();
        }
        r0 = *(u32*)((u8*)r31 + 0x44);
        if ((s32)r0 != (s32)0x0) goto L_80119B60;
        r3 = r31 + 0x2c;
        r4 = r31 + 0x68;
        fn_800E01D0();
        f0 = *(f32*)((u8*)r31 + 0x68);
        r3 = *(u32*)((u8*)r31 + 0x10);
        *(f32*)((u8*)r3 + 0x98) = f0;
        f0 = *(f32*)((u8*)r31 + 0x6C);
        r3 = *(u32*)((u8*)r31 + 0x10);
        *(f32*)((u8*)r3 + 0x9C) = f0;
        f0 = *(f32*)((u8*)r31 + 0x70);
        r3 = *(u32*)((u8*)r31 + 0x10);
        *(f32*)((u8*)r3 + 0xA0) = f0;
    }
    goto L_80119B6C;
L_80119B60: ;
    r3 = r31 + 0x68;
    r4 = r3;
    fn_800E01D0();
L_80119B6C: ;
    r3 = *(u32*)((u8*)r31 + 0x10);
    fn_80175A1C();
    r5 = *(u32*)((u8*)r31 + 0xC);
    r0 = 0x40;
    r3 = 0x0;
    r4 = r5;
    ctr_fn = (void(*)(void))r0;
L_80119B88: ;
    r0 = *(u32*)((u8*)r4 + 0x8);
    if ((u32)r0 == (u32)r31) {
        r0 = r3 << 2;
        r4 = 0x0;
        r3 = r5 + r0;
        *(u32*)((u8*)r3 + 0x8) = r4;
        goto L_80119BB4;
    }
    r4 = r4 + 0x4;
    r3 = r3 + 0x1;
    if (--ctr != 0) goto L_80119B88;
L_80119BB4: ;
    r0 = 0x0;
    *(u8*)((u8*)r31 + 0x0) = r0;
    return;
}
/* 0x80119BD0 | 0x1C0 */
void fn_80119BD0(void) {
    extern u8 lbl_8047CFE8[];
    extern u8 lbl_8047CFEC[];
    extern void fn_800E01F4();
    extern void fn_800E3B6C();
    extern void fn_800E3BF0();
    extern void fn_800E3BF8();
    extern void fn_800E3CBC();
    extern void fn_800E3D08();
    extern void fn_800E6DC0();
    extern void fn_800EE150();
    extern void fn_800EE22C();
    extern void fn_800EE828();
    extern void fn_801190DC();
    extern void fn_80169034();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    r28 = r6;
    r27 = r5;
    r30 = r28;
    while (1) {
    if ((u32)r30 == (u32)0x0) {
        r0 = 0x0;
    } else {
        r0 = *(u32*)((u8*)r30 + 0xC);
    }
    if ((u32)r0 == (u32)0x0) break;
    if ((u32)r30 == (u32)0x0) {
        r30 = 0x0;
    } else {
    r30 = *(u32*)((u8*)r30 + 0xC);
    }
    }
    r3 = r30;
    fn_800E3B6C();
    if ((u32)r0 == (u32)0x0) return;
    fn_800E3BF8();
    if ((u32)r0 == (u32)0x0) return;
    r3 = r29;
    fn_800E6DC0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r30 = *(u32*)((u8*)r30 + 0x10);
    }
    r3 = r30;
    r4 = r28;
    fn_800EE22C();
    r4 = r3;
    r0 = r4 + (0x1 << 16);
    if ((u32)r0 == (u32)0xffff) return;
    r3 = r29;
    fn_800EE150();
    if ((u32)r0 == (u32)0xffff) return;
    r3 = r29;
    fn_800E3CBC();
    r5 = r3;
    r3 = r31;
    r4 = r27;
    fn_801190DC();
    if ((u32)r0 != (u32)0xffff) {
        r3 = r29;
        fn_800E3BF0();
        r0 = *(u32*)((u8*)r30 + 0x44);
        r31 = r3;
        if ((s32)r0 == (s32)0x0) {
            if ((s32)r31 != (s32)0x0) {
                r0 = *(u32*)((u8*)r28 + 0x4);
                r3 = r30 + 0x50;
                f1 = *(f32*)lbl_8047CFE8;
                *(u32*)((u8*)r30 + 0x48) = r0;
                f2 = f1;
                r0 = *(u16*)((u8*)r28 + 0x2);
                f3 = f1;
                *(u32*)((u8*)r30 + 0x4C) = r0;
                fn_800E01F4();
                f1 = *(f32*)lbl_8047CFE8;
                r3 = r30 + 0x5c;
                f2 = f1;
                f3 = f1;
                fn_800E01F4();
                f1 = *(f32*)lbl_8047CFEC;
                r3 = r30 + 0x68;
                f2 = f1;
                f3 = f1;
                fn_800E01F4();
                f1 = *(f32*)lbl_8047CFE8;
                r3 = r30 + 0x14;
                f2 = f1;
                f3 = f1;
                fn_800E01F4();
                f1 = *(f32*)lbl_8047CFE8;
                r3 = r30 + 0x20;
                f2 = f1;
                f3 = f1;
                fn_800E01F4();
                f1 = *(f32*)lbl_8047CFEC;
                r3 = r30 + 0x2c;
                f2 = f1;
                f3 = f1;
                fn_800E01F4();
                *(u32*)((u8*)r30 + 0x44) = r31;
                r3 = 0x0;
                r0 = 0x1;
                *(u8*)((u8*)r30 + 0x5) = r3;
                *(u8*)((u8*)r30 + 0x6) = r0;
    }
    }
    }
    r3 = r28;
    fn_800EE828();
    r3 = r29;
    fn_800E3D08();
    r0 = r3 & 0xFF;
    if ((s32)r31 != (s32)0x0) return;
    r3 = *(u32*)((u8*)r30 + 0x10);
    r4 = 0x0;
    fn_80169034();
    return;
}
/* 0x40 | fn_80119D90 | index_lookup */
u8 fn_80119D90(u16 idx) {
    u8* entry;
    if (idx >= lbl_80478B48) { return 0; }
    entry = (u8*)lbl_8035BBA8 + (u32)idx * 0x14;
    if (entry == NULL) { return 0; }
    return entry[0x4];
}
/* 0x40 | fn_80119DD0 | index_lookup */
u8 fn_80119DD0(u16 idx) {
    void* entry;
    if (idx >= lbl_80478B48) { return 0; }
    entry = (u8*)lbl_8035BBA8 + idx * 0x14;
    if (entry == NULL) { return 0; }
    return *(u8*)((u8*)entry + 0x3);
}
/* 0x40 | fn_80119E10 | index_lookup */
u16 fn_80119E10(u16 idx) {
    void* entry;
    if (idx >= lbl_80478B48) { return 0; }
    entry = (u8*)lbl_8035BBA8 + idx * 0x14;
    if (entry == NULL) { return 0; }
    return *(u16*)((u8*)entry + 0xA);
}
/* 0x40 | fn_80119E50 | index_lookup */
u8 fn_80119E50(u16 idx) {
    void* entry;
    if (idx >= lbl_80478B48) { return 0; }
    entry = (u8*)lbl_8035BBA8 + idx * 0x14;
    if (entry == NULL) { return 0; }
    return *(u8*)((u8*)entry + 0x2);
}
/* 0x40 | fn_80119E90 | index_lookup */
u8 fn_80119E90(u16 idx) {
    void* entry;
    if (idx >= lbl_80478B48) { return 0; }
    entry = (u8*)lbl_8035BBA8 + idx * 0x14;
    if (entry == NULL) { return 0; }
    return *(u8*)((u8*)entry + 0x1);
}
/* 0x40 | fn_80119ED0 | index_lookup */
u16 fn_80119ED0(u16 idx) {
    void* entry;
    if (idx >= lbl_80478B48) { return 0; }
    entry = (u8*)lbl_8035BBA8 + idx * 0x14;
    if (entry == NULL) { return 0; }
    return *(u16*)((u8*)entry + 0x8);
}
/* 0x40 | fn_80119F10 | index_lookup */
u8 fn_80119F10(u16 idx) {
    void* entry;
    if (idx >= lbl_80478B48) { return 0; }
    entry = (u8*)lbl_8035BBA8 + idx * 0x14;
    if (entry == NULL) { return 0; }
    return *(u8*)((u8*)entry + 0x0);
}
/* 0x40 | fn_80119F50 | index_lookup */
u32 fn_80119F50(u16 idx) {
    void* entry;
    if (idx >= lbl_80478B48) { return 0; }
    entry = (u8*)lbl_8035BBA8 + idx * 0x14;
    if (entry == NULL) { return 0; }
    return *(u32*)((u8*)entry + 0x10);
}
/* 0x8011A0A8 | 0x1D8 */
void fn_8011A0A8(void) {
    extern void fn_80119E90();
    extern void fn_80119ED0();
    extern void fn_80119F10();
    extern void fn_8011A090();
    extern void fn_80135E44();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = r5 & 0xFFFF;
    r30 = r5;
    r27 = r3;
    r29 = r4;
    if ((s32)r0 == (s32)0) return;
    r3 = r30;
    fn_80119E90();
    r28 = r3 & 0xFF;
    r3 = r30;
    fn_80119F10();
    r31 = r3;
    r3 = r30;
    fn_80119ED0();
    r6 = r3;
    r3 = r31;
    r4 = r27;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r31 = 0x0;
    } else {
        /* clrlslwi r0, r28, 16, 4 */;
        r31 = r3 + r0;
    }
    if ((u32)r31 == (u32)0x0) return;
    r0 = r30 & 0xFFFF;
    do {
        if ((u32)r31 == (u32)0x0) {
            r0 = 0x0;
            break;
        }
        r3 = r30;
        fn_80119E90();
        r28 = r3 & 0xFF;
        r3 = r30;
        fn_80119F10();
        r27 = r3;
        r3 = r30;
        fn_80119ED0();
        r6 = r3;
        r3 = r27;
        r4 = r29;
        r5 = 0x0;
        r7 = 0x0;
        fn_80135E44();
        if ((u32)r3 == (u32)0x0) {
            r27 = 0x0;
        } else {
            /* clrlslwi r0, r28, 16, 4 */;
            r27 = r3 + r0;
        }
        if ((u32)r27 == (u32)0x0) {
            r0 = 0x0;
            break;
        }
        { u32 _flag;
        if ((u32)r27 == (u32)0x0) {
            _flag = 0x0;
        } else {
            r3 = r27;
            fn_8011A090();
            r0 = r3 & 0xFFFF;
            if ((u32)r27 == (u32)0x0) {
                _flag = 0x0;
            } else {
                _flag = 0x1;
            }
        }
        _flag = _flag & 0xFF;
        if ((u32)_flag == (u32)0x1) {
            r3 = r27;
            fn_8011A090();
            r3 = r3 & 0xFFFF;
            r0 = r30 & 0xFFFF;
            if ((u32)r0 == (u32)r3) {
                r0 = 0x1;
                break;
            }
        }
        }
        r0 = 0x0;
    } while (0);
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)r3) return;
    r3 = r30;
    fn_80119E90();
    r27 = r3 & 0xFF;
    r3 = r30;
    fn_80119F10();
    r28 = r3;
    r3 = r30;
    fn_80119ED0();
    r6 = r3;
    r3 = r28;
    r4 = r29;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r4 = 0x0;
    } else {
        /* clrlslwi r0, r27, 16, 4 */;
        r4 = r3 + r0;
    }
    if ((u32)r4 == (u32)0x0) return;
    r3 = *(u32*)((u8*)r4 + 0x0);
    r0 = *(u32*)((u8*)r4 + 0x4);
    *(u32*)((u8*)r31 + 0x0) = r3;
    *(u32*)((u8*)r31 + 0x4) = r0;
    r3 = *(u32*)((u8*)r4 + 0x8);
    r0 = *(u32*)((u8*)r4 + 0xC);
    *(u32*)((u8*)r31 + 0x8) = r3;
    *(u32*)((u8*)r31 + 0xC) = r0;
    return;
}
/* 0x8011A280 | 0x164 */
void fn_8011A280(void) {
    extern void fn_80119E90();
    extern void fn_80119ED0();
    extern void fn_80119F10();
    extern void fn_80119FA0();
    extern void fn_8011A090();
    extern void fn_80135E44();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = r4 & 0xFFFF;
    r30 = r4;
    r29 = r3;
    r31 = r5;
    if ((s32)r0 == (s32)0) return;
    do {
        if ((s32)r0 == (s32)0) {
            r0 = 0x0;
            break;
        }
        r3 = r30;
        fn_80119E90();
        r28 = r3 & 0xFF;
        r3 = r30;
        fn_80119F10();
        r27 = r3;
        r3 = r30;
        fn_80119ED0();
        r6 = r3;
        r3 = r27;
        r4 = r29;
        r5 = 0x0;
        r7 = 0x0;
        fn_80135E44();
        if ((u32)r3 == (u32)0x0) {
            r27 = 0x0;
        } else {
            /* clrlslwi r0, r28, 16, 4 */;
            r27 = r3 + r0;
        }
        if ((u32)r27 == (u32)0x0) {
            r0 = 0x0;
            break;
        }
        { u32 _flag;
        if ((u32)r27 == (u32)0x0) {
            _flag = 0x0;
        } else {
            r3 = r27;
            fn_8011A090();
            r0 = r3 & 0xFFFF;
            if ((u32)r27 == (u32)0x0) {
                _flag = 0x0;
            } else {
                _flag = 0x1;
            }
        }
        _flag = _flag & 0xFF;
        if ((u32)_flag == (u32)0x1) {
            r3 = r27;
            fn_8011A090();
            r3 = r3 & 0xFFFF;
            r0 = r30 & 0xFFFF;
            if ((u32)r0 == (u32)r3) {
                r0 = 0x1;
                break;
            }
        }
        }
        r0 = 0x0;
    } while (0);
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)r3) return;
    r3 = r30;
    fn_80119E90();
    r27 = r3 & 0xFF;
    r3 = r30;
    fn_80119F10();
    r28 = r3;
    r3 = r30;
    fn_80119ED0();
    r6 = r3;
    r3 = r28;
    r4 = r29;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
    } else {
        /* clrlslwi r0, r27, 16, 4 */;
        r3 = r3 + r0;
    }
    if ((u32)r3 == (u32)0x0) return;
    r4 = r31;
    fn_80119FA0();
    return;
}
/* 0x8011A3E4 | 0x18C */
void fn_8011A3E4(void* obj) {
    extern void fn_80119E90();
    extern void fn_80119ED0();
    extern void fn_80119F10();
    extern void fn_8011A018();
    extern void fn_8011A090();
    extern void fn_80135E44();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = r4 & 0xFFFF;
    r31 = r4;
    r30 = r3;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    do {
        if ((s32)r0 == (s32)0) {
            r0 = 0x0;
            break;
        }
        r3 = r31;
        fn_80119E90();
        r29 = r3 & 0xFF;
        r3 = r31;
        fn_80119F10();
        r28 = r3;
        r3 = r31;
        fn_80119ED0();
        r6 = r3;
        r3 = r28;
        r4 = r30;
        r5 = 0x0;
        r7 = 0x0;
        fn_80135E44();
        if ((u32)r3 == (u32)0x0) {
            r28 = 0x0;
        } else {
            /* clrlslwi r0, r29, 16, 4 */;
            r28 = r3 + r0;
        }
        if ((u32)r28 == (u32)0x0) {
            r0 = 0x0;
            break;
        }
        { u32 _flag;
        if ((u32)r28 == (u32)0x0) {
            _flag = 0x0;
        } else {
            r3 = r28;
            fn_8011A090();
            r0 = r3 & 0xFFFF;
            if ((u32)r28 == (u32)0x0) {
                _flag = 0x0;
            } else {
                _flag = 0x1;
            }
        }
        _flag = _flag & 0xFF;
        if ((u32)_flag == (u32)0x1) {
            r3 = r28;
            fn_8011A090();
            r3 = r3 & 0xFFFF;
            r0 = r31 & 0xFFFF;
            if ((u32)r0 == (u32)r3) {
                r0 = 0x1;
                break;
            }
        }
        }
        r0 = 0x0;
    } while (0);
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)r3) {
        r3 = 0x0;
        return;
    }
    r3 = r31;
    fn_80119E90();
    r28 = r3 & 0xFF;
    r3 = r31;
    fn_80119F10();
    r29 = r3;
    r3 = r31;
    fn_80119ED0();
    r6 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
    } else {
        /* clrlslwi r0, r28, 16, 4 */;
        r3 = r3 + r0;
    }
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    fn_8011A018();
    return;
}
/* 0x8011A570 | 0x164 */
void fn_8011A570(void) {
    extern void fn_80119E90();
    extern void fn_80119ED0();
    extern void fn_80119F10();
    extern void fn_80119F90();
    extern void fn_8011A090();
    extern void fn_80135E44();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = r4 & 0xFFFF;
    r30 = r4;
    r29 = r3;
    r31 = r5;
    if ((s32)r0 == (s32)0) return;
    do {
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
        break;
    }
    r3 = r30;
    fn_80119E90();
    r28 = r3 & 0xFF;
    r3 = r30;
    fn_80119F10();
    r27 = r3;
    r3 = r30;
    fn_80119ED0();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r27 = 0x0;
    } else {
        /* clrlslwi r0, r28, 16, 4 */;
        r27 = r3 + r0;
    }
    if ((u32)r27 == (u32)0x0) {
        r0 = 0x0;
        break;
    }
    { u32 _flag;
    if ((u32)r27 == (u32)0x0) {
        r0 = 0x0;
        _flag = 0x0;
    }
    r3 = r27;
    fn_8011A090();
    r0 = r3 & 0xFFFF;
    if ((u32)r27 == (u32)0x0) {
        r0 = 0x0;
        _flag = 0x0;
    }
    _flag = 0x1;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        fn_8011A090();
        r3 = r3 & 0xFFFF;
        r0 = r30 & 0xFFFF;
        if ((u32)r0 == (u32)r3) {
            r0 = 0x1;
            break;
        }
    }
    r0 = 0x0;
    }
} while (0);
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)r3) return;
    r3 = r30;
    fn_80119E90();
    r27 = r3 & 0xFF;
    r3 = r30;
    fn_80119F10();
    r28 = r3;
    r3 = r30;
    fn_80119ED0();
    r6 = r3;
    r3 = r28;
    r4 = r29;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
    } else {
        /* clrlslwi r0, r27, 16, 4 */;
        r3 = r3 + r0;
    }
    if ((u32)r3 == (u32)0x0) return;
    r4 = r31;
    fn_80119F90();
    return;
}
/* 0x8011A6D4 | 0x18C */
void fn_8011A6D4(void* obj) {
    extern void fn_80119E90();
    extern void fn_80119ED0();
    extern void fn_80119F10();
    extern void fn_8011A000();
    extern void fn_8011A090();
    extern void fn_80135E44();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = r4 & 0xFFFF;
    r31 = r4;
    r30 = r3;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    do {
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
        break;
    }
    r3 = r31;
    fn_80119E90();
    r29 = r3 & 0xFF;
    r3 = r31;
    fn_80119F10();
    r28 = r3;
    r3 = r31;
    fn_80119ED0();
    r6 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r28 = 0x0;
    } else {
        /* clrlslwi r0, r29, 16, 4 */;
        r28 = r3 + r0;
    }
    if ((u32)r28 == (u32)0x0) {
        r0 = 0x0;
        break;
    }
    { u32 _flag;
    if ((u32)r28 == (u32)0x0) {
        r0 = 0x0;
        _flag = 0x0;
    }
    r3 = r28;
    fn_8011A090();
    r0 = r3 & 0xFFFF;
    if ((u32)r28 == (u32)0x0) {
        r0 = 0x0;
        _flag = 0x0;
    }
    _flag = 0x1;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r28;
        fn_8011A090();
        r3 = r3 & 0xFFFF;
        r0 = r31 & 0xFFFF;
        if ((u32)r0 == (u32)r3) {
            r0 = 0x1;
            break;
        }
    }
    r0 = 0x0;
    }
} while (0);
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)r3) {
        r3 = 0x0;
        return;
    }
    r3 = r31;
    fn_80119E90();
    r28 = r3 & 0xFF;
    r3 = r31;
    fn_80119F10();
    r29 = r3;
    r3 = r31;
    fn_80119ED0();
    r6 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
    } else {
        /* clrlslwi r0, r28, 16, 4 */;
        r3 = r3 + r0;
    }
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    fn_8011A000();
    return;
}
/* 0x8011A860 | 0x18C */
void fn_8011A860(void* obj) {
    extern void fn_80119E90();
    extern void fn_80119ED0();
    extern void fn_80119F10();
    extern void fn_8011A078();
    extern void fn_8011A090();
    extern void fn_80135E44();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = r4 & 0xFFFF;
    r31 = r4;
    r30 = r3;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    do {
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
        break;
    }
    r3 = r31;
    fn_80119E90();
    r29 = r3 & 0xFF;
    r3 = r31;
    fn_80119F10();
    r28 = r3;
    r3 = r31;
    fn_80119ED0();
    r6 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r28 = 0x0;
    } else {
        /* clrlslwi r0, r29, 16, 4 */;
        r28 = r3 + r0;
    }
    if ((u32)r28 == (u32)0x0) {
        r0 = 0x0;
        break;
    }
    { u32 _flag;
    if ((u32)r28 == (u32)0x0) {
        r0 = 0x0;
        _flag = 0x0;
    }
    r3 = r28;
    fn_8011A090();
    r0 = r3 & 0xFFFF;
    if ((u32)r28 == (u32)0x0) {
        r0 = 0x0;
        _flag = 0x0;
    }
    _flag = 0x1;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r28;
        fn_8011A090();
        r3 = r3 & 0xFFFF;
        r0 = r31 & 0xFFFF;
        if ((u32)r0 == (u32)r3) {
            r0 = 0x1;
            break;
        }
    }
    r0 = 0x0;
    }
} while (0);
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)r3) {
        r3 = 0x0;
        return;
    }
    r3 = r31;
    fn_80119E90();
    r28 = r3 & 0xFF;
    r3 = r31;
    fn_80119F10();
    r29 = r3;
    r3 = r31;
    fn_80119ED0();
    r6 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
    } else {
        /* clrlslwi r0, r28, 16, 4 */;
        r3 = r3 + r0;
    }
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    fn_8011A078();
    return;
}
/* 0x8011A9EC | 0x164 */
void fn_8011A9EC(void) {
    extern void fn_80119E90();
    extern void fn_80119ED0();
    extern void fn_80119F10();
    extern void fn_80119FD0();
    extern void fn_8011A090();
    extern void fn_80135E44();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = r4 & 0xFFFF;
    r30 = r4;
    r29 = r3;
    r31 = r5;
    if ((s32)r0 == (s32)0) return;
    do {
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
        break;
    }
    r3 = r30;
    fn_80119E90();
    r28 = r3 & 0xFF;
    r3 = r30;
    fn_80119F10();
    r27 = r3;
    r3 = r30;
    fn_80119ED0();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r27 = 0x0;
    } else {
        /* clrlslwi r0, r28, 16, 4 */;
        r27 = r3 + r0;
    }
    if ((u32)r27 == (u32)0x0) {
        r0 = 0x0;
        break;
    }
    { u32 _flag;
    if ((u32)r27 == (u32)0x0) {
        r0 = 0x0;
        _flag = 0x0;
    }
    r3 = r27;
    fn_8011A090();
    r0 = r3 & 0xFFFF;
    if ((u32)r27 == (u32)0x0) {
        r0 = 0x0;
        _flag = 0x0;
    }
    _flag = 0x1;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        fn_8011A090();
        r3 = r3 & 0xFFFF;
        r0 = r30 & 0xFFFF;
        if ((u32)r0 == (u32)r3) {
            r0 = 0x1;
            break;
        }
    }
    r0 = 0x0;
    }
} while (0);
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)r3) return;
    r3 = r30;
    fn_80119E90();
    r27 = r3 & 0xFF;
    r3 = r30;
    fn_80119F10();
    r28 = r3;
    r3 = r30;
    fn_80119ED0();
    r6 = r3;
    r3 = r28;
    r4 = r29;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
    } else {
        /* clrlslwi r0, r27, 16, 4 */;
        r3 = r3 + r0;
    }
    if ((u32)r3 == (u32)0x0) return;
    r4 = r31;
    fn_80119FD0();
    return;
}
/* 0x8011AB50 | 0x164 */
void fn_8011AB50(void) {
    extern void fn_80119E90();
    extern void fn_80119ED0();
    extern void fn_80119F10();
    extern void fn_80119FC0();
    extern void fn_8011A090();
    extern void fn_80135E44();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = r4 & 0xFFFF;
    r30 = r4;
    r29 = r3;
    r31 = r5;
    if ((s32)r0 == (s32)0) return;
    do {
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
        break;
    }
    r3 = r30;
    fn_80119E90();
    r28 = r3 & 0xFF;
    r3 = r30;
    fn_80119F10();
    r27 = r3;
    r3 = r30;
    fn_80119ED0();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r27 = 0x0;
    } else {
        /* clrlslwi r0, r28, 16, 4 */;
        r27 = r3 + r0;
    }
    if ((u32)r27 == (u32)0x0) {
        r0 = 0x0;
        break;
    }
    { u32 _flag;
    if ((u32)r27 == (u32)0x0) {
        r0 = 0x0;
        _flag = 0x0;
    }
    r3 = r27;
    fn_8011A090();
    r0 = r3 & 0xFFFF;
    if ((u32)r27 == (u32)0x0) {
        r0 = 0x0;
        _flag = 0x0;
    }
    _flag = 0x1;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        fn_8011A090();
        r3 = r3 & 0xFFFF;
        r0 = r30 & 0xFFFF;
        if ((u32)r0 == (u32)r3) {
            r0 = 0x1;
            break;
        }
    }
    r0 = 0x0;
    }
} while (0);
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)r3) return;
    r3 = r30;
    fn_80119E90();
    r27 = r3 & 0xFF;
    r3 = r30;
    fn_80119F10();
    r28 = r3;
    r3 = r30;
    fn_80119ED0();
    r6 = r3;
    r3 = r28;
    r4 = r29;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
    } else {
        /* clrlslwi r0, r27, 16, 4 */;
        r3 = r3 + r0;
    }
    if ((u32)r3 == (u32)0x0) return;
    r4 = r31;
    fn_80119FC0();
    return;
}
/* 0x8011ACB4 | 0x18C */
void fn_8011ACB4(void* obj) {
    extern void fn_80119E90();
    extern void fn_80119ED0();
    extern void fn_80119F10();
    extern void fn_8011A048();
    extern void fn_8011A090();
    extern void fn_80135E44();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = r4 & 0xFFFF;
    r31 = r4;
    r30 = r3;
    if ((s32)r0 == (s32)0) {
        r3 = -0x1;
        return;
    }
    do {
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
        break;
    }
    r3 = r31;
    fn_80119E90();
    r29 = r3 & 0xFF;
    r3 = r31;
    fn_80119F10();
    r28 = r3;
    r3 = r31;
    fn_80119ED0();
    r6 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r28 = 0x0;
    } else {
        /* clrlslwi r0, r29, 16, 4 */;
        r28 = r3 + r0;
    }
    if ((u32)r28 == (u32)0x0) {
        r0 = 0x0;
        break;
    }
    { u32 _flag;
    if ((u32)r28 == (u32)0x0) {
        r0 = 0x0;
        _flag = 0x0;
    }
    r3 = r28;
    fn_8011A090();
    r0 = r3 & 0xFFFF;
    if ((u32)r28 == (u32)0x0) {
        r0 = 0x0;
        _flag = 0x0;
    }
    _flag = 0x1;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r28;
        fn_8011A090();
        r3 = r3 & 0xFFFF;
        r0 = r31 & 0xFFFF;
        if ((u32)r0 == (u32)r3) {
            r0 = 0x1;
            break;
        }
    }
    r0 = 0x0;
    }
} while (0);
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)r3) {
        r3 = -0x1;
        return;
    }
    r3 = r31;
    fn_80119E90();
    r28 = r3 & 0xFF;
    r3 = r31;
    fn_80119F10();
    r29 = r3;
    r3 = r31;
    fn_80119ED0();
    r6 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
    } else {
        /* clrlslwi r0, r28, 16, 4 */;
        r3 = r3 + r0;
    }
    if ((u32)r3 == (u32)0x0) {
        r3 = -0x1;
        return;
    }
    fn_8011A048();
    return;
}
/* 0x8011AE40 | 0x18C */
void fn_8011AE40(void* obj) {
    extern void fn_80119E90();
    extern void fn_80119ED0();
    extern void fn_80119F10();
    extern void fn_8011A060();
    extern void fn_8011A090();
    extern void fn_80135E44();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = r4 & 0xFFFF;
    r31 = r4;
    r30 = r3;
    if ((s32)r0 == (s32)0) {
        r3 = -0x1;
        return;
    }
    do {
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
        break;
    }
    r3 = r31;
    fn_80119E90();
    r29 = r3 & 0xFF;
    r3 = r31;
    fn_80119F10();
    r28 = r3;
    r3 = r31;
    fn_80119ED0();
    r6 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r28 = 0x0;
    } else {
        /* clrlslwi r0, r29, 16, 4 */;
        r28 = r3 + r0;
    }
    if ((u32)r28 == (u32)0x0) {
        r0 = 0x0;
        break;
    }
    { u32 _flag;
    if ((u32)r28 == (u32)0x0) {
        r0 = 0x0;
        _flag = 0x0;
    }
    r3 = r28;
    fn_8011A090();
    r0 = r3 & 0xFFFF;
    if ((u32)r28 == (u32)0x0) {
        r0 = 0x0;
        _flag = 0x0;
    }
    _flag = 0x1;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r28;
        fn_8011A090();
        r3 = r3 & 0xFFFF;
        r0 = r31 & 0xFFFF;
        if ((u32)r0 == (u32)r3) {
            r0 = 0x1;
            break;
        }
    }
    r0 = 0x0;
    }
} while (0);
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)r3) {
        r3 = -0x1;
        return;
    }
    r3 = r31;
    fn_80119E90();
    r28 = r3 & 0xFF;
    r3 = r31;
    fn_80119F10();
    r29 = r3;
    r3 = r31;
    fn_80119ED0();
    r6 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
    } else {
        /* clrlslwi r0, r28, 16, 4 */;
        r3 = r3 + r0;
    }
    if ((u32)r3 == (u32)0x0) {
        r3 = -0x1;
        return;
    }
    fn_8011A060();
    return;
}
/* 0x8011AFCC | 0x164 */
void fn_8011AFCC(void) {
    extern void fn_80119E90();
    extern void fn_80119ED0();
    extern void fn_80119F10();
    extern void fn_80119FB0();
    extern void fn_8011A090();
    extern void fn_80135E44();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = r4 & 0xFFFF;
    r30 = r4;
    r29 = r3;
    r31 = r5;
    if ((s32)r0 == (s32)0) return;
    do {
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
        break;
    }
    r3 = r30;
    fn_80119E90();
    r28 = r3 & 0xFF;
    r3 = r30;
    fn_80119F10();
    r27 = r3;
    r3 = r30;
    fn_80119ED0();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r27 = 0x0;
    } else {
        /* clrlslwi r0, r28, 16, 4 */;
        r27 = r3 + r0;
    }
    if ((u32)r27 == (u32)0x0) {
        r0 = 0x0;
        break;
    }
    { u32 _flag;
    if ((u32)r27 == (u32)0x0) {
        r0 = 0x0;
        _flag = 0x0;
    }
    r3 = r27;
    fn_8011A090();
    r0 = r3 & 0xFFFF;
    if ((u32)r27 == (u32)0x0) {
        r0 = 0x0;
        _flag = 0x0;
    }
    _flag = 0x1;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        fn_8011A090();
        r3 = r3 & 0xFFFF;
        r0 = r30 & 0xFFFF;
        if ((u32)r0 == (u32)r3) {
            r0 = 0x1;
            break;
        }
    }
    r0 = 0x0;
    }
} while (0);
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)r3) return;
    r3 = r30;
    fn_80119E90();
    r27 = r3 & 0xFF;
    r3 = r30;
    fn_80119F10();
    r28 = r3;
    r3 = r30;
    fn_80119ED0();
    r6 = r3;
    r3 = r28;
    r4 = r29;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
    } else {
        /* clrlslwi r0, r27, 16, 4 */;
        r3 = r3 + r0;
    }
    if ((u32)r3 == (u32)0x0) return;
    r4 = r31 & 0xFF;
    fn_80119FB0();
    return;
}
/* 0x8011B130 | 0x190 */
void fn_8011B130(void* obj) {
    extern void fn_80119E90();
    extern void fn_80119ED0();
    extern void fn_80119F10();
    extern void fn_8011A030();
    extern void fn_8011A090();
    extern void fn_80135E44();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = r4 & 0xFFFF;
    r31 = r4;
    r30 = r3;
    if ((s32)r0 == (s32)0) {
        r3 = -0x1;
        return;
    }
    do {
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
        break;
    }
    r3 = r31;
    fn_80119E90();
    r29 = r3 & 0xFF;
    r3 = r31;
    fn_80119F10();
    r28 = r3;
    r3 = r31;
    fn_80119ED0();
    r6 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r28 = 0x0;
    } else {
        /* clrlslwi r0, r29, 16, 4 */;
        r28 = r3 + r0;
    }
    if ((u32)r28 == (u32)0x0) {
        r0 = 0x0;
        break;
    }
    { u32 _flag;
    if ((u32)r28 == (u32)0x0) {
        r0 = 0x0;
        _flag = 0x0;
    }
    r3 = r28;
    fn_8011A090();
    r0 = r3 & 0xFFFF;
    if ((u32)r28 == (u32)0x0) {
        r0 = 0x0;
        _flag = 0x0;
    }
    _flag = 0x1;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r28;
        fn_8011A090();
        r3 = r3 & 0xFFFF;
        r0 = r31 & 0xFFFF;
        if ((u32)r0 == (u32)r3) {
            r0 = 0x1;
            break;
        }
    }
    r0 = 0x0;
    }
} while (0);
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)r3) {
        r3 = -0x1;
        return;
    }
    r3 = r31;
    fn_80119E90();
    r28 = r3 & 0xFF;
    r3 = r31;
    fn_80119F10();
    r29 = r3;
    r3 = r31;
    fn_80119ED0();
    r6 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
    } else {
        /* clrlslwi r0, r28, 16, 4 */;
        r3 = r3 + r0;
    }
    if ((u32)r3 == (u32)0x0) {
        r3 = -0x1;
        return;
    }
    fn_8011A030();
    r3 = r3 & 0xFF;
    return;
}
/* 0x8011B2C0 | 0x184 */
void fn_8011B2C0(void) {
    extern void fn_80101AC4();
    extern void fn_80119D90();
    extern void fn_80119DD0();
    extern void fn_80119E50();
    extern void fn_80119E90();
    extern void fn_80119ED0();
    extern void fn_80119F10();
    extern void fn_80119F90();
    extern void fn_80119FA0();
    extern void fn_80119FB0();
    extern void fn_80119FC0();
    extern void fn_80119FD0();
    extern void fn_80119FE0();
    extern void fn_80119FF0();
    extern void fn_8011A030();
    extern void fn_80135E44();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = r4 & 0xFFFF;
    r27 = r4;
    r31 = r3;
    r28 = r5;
    if ((s32)r0 != (s32)0) {
        r3 = r27;
        fn_80119E90();
        r30 = r3 & 0xFF;
        r3 = r27;
        fn_80119F10();
        r29 = r3;
        r3 = r27;
        fn_80119ED0();
        r6 = r3;
        r3 = r29;
        r4 = r31;
        r5 = 0x0;
        r7 = 0x0;
        fn_80135E44();
        if ((u32)r3 == (u32)0x0) {
            r29 = 0x0;
        } else {
            /* clrlslwi r0, r30, 16, 4 */;
            r29 = r3 + r0;
        }
        if ((u32)r29 != (u32)0x0) {
            r3 = r27;
            fn_80119E50();
            r30 = r3;
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x4) {
                r3 = r29;
                fn_8011A030();
                r31 = r3;
            } else {
                r31 = 0x0;
            }
            if ((u32)r29 != (u32)0x0) {
                r3 = r29;
                r4 = 0x0;
                fn_80119FF0();
                r3 = r29;
                r4 = 0x0;
                fn_80119FE0();
                r3 = r29;
                r4 = 0x0;
                fn_80119FD0();
                r3 = r29;
                r4 = 0x0;
                fn_80119FC0();
                r3 = r29;
                r4 = 0x0;
                fn_80119FB0();
                r3 = r29;
                r4 = 0x0;
                fn_80119FA0();
                r3 = r29;
                r4 = 0x0;
                fn_80119F90();
            }
            r3 = r29;
            r4 = r27;
            fn_80119FF0();
            r3 = r29;
            r4 = r28;
            fn_80119FE0();
            r3 = r27;
            fn_80119D90();
            r3 = r3 & 0xFF;
            fn_80101AC4();
            r4 = (s8)r3;
            r3 = r29;
            fn_80119FD0();
            r0 = r30 & 0xFF;
            if ((u32)r0 == (u32)0x4) {
                r3 = r27;
                r30 = r31 + 0x1;
                fn_80119DD0();
                r0 = r30 & 0xFF;
                r4 = r3 & 0xFF;
                if ((u32)r4 < (u32)r0) {
                    r30 = r3;
                }
                r3 = r29;
                r4 = r30;
                fn_80119FB0();
    }
    }
    }
    return;
}
/* 0x8011B444 | 0x238 */
void fn_8011B444(void* obj) {
    extern void fn_80119E10();
    extern void fn_80119E50();
    extern void fn_80119E90();
    extern void fn_80119ED0();
    extern void fn_80119F10();
    extern void fn_8011A090();
    extern void fn_80135E44();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = r4 & 0xFFFF;
    r31 = r4;
    r28 = r3;
    if ((s32)r0 == (s32)0) {
        r3 = 0x1;
        return;
    }
    r3 = r31;
    fn_80119E90();
    r29 = r3 & 0xFF;
    r3 = r31;
    fn_80119F10();
    r30 = r3;
    r3 = r31;
    fn_80119ED0();
    r6 = r3;
    r3 = r30;
    r4 = r28;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r30 = 0x0;
    } else {
        /* clrlslwi r0, r29, 16, 4 */;
        r30 = r3 + r0;
    }
    if ((u32)r30 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    fn_8011A090();
    r0 = r3;
    r3 = r31;
    r29 = r0;
    fn_80119E50();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0x2) {
        if ((s32)r0 < (s32)0x2) {
            if ((s32)r0 == (s32)0x0) { r3 = 0x2; return; }
            if ((s32)r0 >= (s32)0x0) { r3 = 0x2; return; }
            r3 = 0x0;
            return;
        }
        if ((s32)r0 == (s32)0x4) goto L_8011B5E4;
        if ((s32)r0 >= (s32)0x4) { r3 = 0x0; return; }
        goto L_8011B570;
        r3 = 0x2;
        return;
        r3 = 0x2;
        return;
    }
    if ((u32)r30 == (u32)0x0) {
        r0 = 0x0;
        goto L_8011B55C;
    }
    r3 = r30;
    fn_8011A090();
    r0 = r3 & 0xFFFF;
    if ((u32)r30 == (u32)0x0) {
        r0 = 0x0;
        goto L_8011B55C;
    }
    r0 = 0x1;
L_8011B55C: ;
    r0 = r0 & 0xFF;
    r0 = __cntlzw(r0);
    r3 = (u32)r0 >> 5;
    r3 = r3 + 0x1;
    return;
L_8011B570: ;
    if ((u32)r30 == (u32)0x0) {
        r0 = 0x0;
        goto L_8011B59C;
    }
    r3 = r30;
    fn_8011A090();
    r0 = r3 & 0xFFFF;
    if ((u32)r30 == (u32)0x0) {
        r0 = 0x0;
        goto L_8011B59C;
    }
    r0 = 0x1;
L_8011B59C: ;
    r0 = r0 & 0xFF;
    if ((u32)r30 == (u32)0x0) {
        r3 = 0x2;
        return;
    }
    r3 = r29 & 0xFFFF;
    r0 = r31 & 0xFFFF;
    if ((u32)r3 != (u32)r0) {
        r3 = r29;
        fn_80119E10();
        r3 = r3 & 0xFFFF;
        r0 = r31 & 0xFFFF;
        if ((u32)r0 != (u32)r3) {
            r3 = 0x2;
            return;
    }
    }
    r3 = 0x1;
    return;
L_8011B5E4: ;
    if ((u32)r30 == (u32)0x0) {
        r0 = 0x0;
        goto L_8011B610;
    }
    r3 = r30;
    fn_8011A090();
    r0 = r3 & 0xFFFF;
    if ((u32)r30 == (u32)0x0) {
        r0 = 0x0;
        goto L_8011B610;
    }
    r0 = 0x1;
L_8011B610: ;
    r0 = r0 & 0xFF;
    if ((u32)r30 == (u32)0x0) {
        r3 = 0x2;
        return;
    }
    r3 = r29 & 0xFFFF;
    r0 = r31 & 0xFFFF;
    if ((u32)r3 == (u32)r0) { r3 = 0x2; return; }
    r3 = r29;
    fn_80119E10();
    r3 = r3 & 0xFFFF;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 != (u32)r3) { r3 = 0x1; return; }
    r3 = 0x2;
    return;
    r3 = 0x1;
    return;
    r3 = 0x0;
    return;
}
/* 0x8011B67C | 0x10C */
void fn_8011B67C(void* obj) {
    extern void fn_80119E90();
    extern void fn_80119ED0();
    extern void fn_80119F10();
    extern void fn_8011A090();
    extern void fn_80135E44();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = r4 & 0xFFFF;
    r29 = r4;
    r28 = r3;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r3 = r29;
    fn_80119E90();
    r30 = r3 & 0xFF;
    r3 = r29;
    fn_80119F10();
    r31 = r3;
    r3 = r29;
    fn_80119ED0();
    r6 = r3;
    r3 = r31;
    r4 = r28;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r31 = 0x0;
    } else {
        /* clrlslwi r0, r30, 16, 4 */;
        r31 = r3 + r0;
    }
    if ((u32)r31 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((u32)r31 == (u32)0x0) {
        r0 = 0x0;
    } else {
    r3 = r31;
    fn_8011A090();
    r0 = r3 & 0xFFFF;
    if ((u32)r31 == (u32)0x0) {
        r0 = 0x0;
    } else {
    r0 = 0x1;
    }
    }
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r31;
        fn_8011A090();
        r3 = r3 & 0xFFFF;
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)r3) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x0;
    return;
}
/* 0x8011B788 | 0x1C8 */
void fn_8011B788(void) {
    extern void fn_80119E90();
    extern void fn_80119ED0();
    extern void fn_80119F10();
    extern void fn_80119F90();
    extern void fn_80119FA0();
    extern void fn_80119FB0();
    extern void fn_80119FC0();
    extern void fn_80119FD0();
    extern void fn_80119FE0();
    extern void fn_80119FF0();
    extern void fn_8011A090();
    extern void fn_80135E44();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = r4 & 0xFFFF;
    r31 = r4;
    r30 = r3;
    if ((s32)r0 == (s32)0) return;
    if ((s32)r0 != (s32)0) {
    r3 = r31;
    fn_80119E90();
    r29 = r3 & 0xFF;
    r3 = r31;
    fn_80119F10();
    r28 = r3;
    r3 = r31;
    fn_80119ED0();
    r6 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r28 = 0x0;
    } else {
        /* clrlslwi r0, r29, 16, 4 */;
        r28 = r3 + r0;
    }
    if ((u32)r28 == (u32)0x0) {
        r0 = 0x0;
    } else {
    if ((u32)r28 == (u32)0x0) {
        r0 = 0x0;
    } else {
    r3 = r28;
    fn_8011A090();
    r0 = r3 & 0xFFFF;
    if ((u32)r28 == (u32)0x0) {
        r0 = 0x0;
    } else {
    r0 = 0x1;
    }
    }
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r28;
        fn_8011A090();
        r3 = r3 & 0xFFFF;
        r0 = r31 & 0xFFFF;
        if ((u32)r0 == (u32)r3) {
            r0 = 0x1;
        } else {
    r0 = 0x0;
        }
    } else {
    r0 = 0x0;
    }
    }
    } else {
    r0 = 0x0;
    }
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)r3) return;
    r3 = r31;
    fn_80119E90();
    r28 = r3 & 0xFF;
    r3 = r31;
    fn_80119F10();
    r29 = r3;
    r3 = r31;
    fn_80119ED0();
    r6 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    if ((u32)r3 == (u32)0x0) {
        r30 = 0x0;
    } else {
        /* clrlslwi r0, r28, 16, 4 */;
        r30 = r3 + r0;
    }
    if ((u32)r30 == (u32)0x0) return;
    if ((u32)r30 == (u32)0x0) return;
    r3 = r30;
    r4 = 0x0;
    fn_80119FF0();
    r3 = r30;
    r4 = 0x0;
    fn_80119FE0();
    r3 = r30;
    r4 = 0x0;
    fn_80119FD0();
    r3 = r30;
    r4 = 0x0;
    fn_80119FC0();
    r3 = r30;
    r4 = 0x0;
    fn_80119FB0();
    r3 = r30;
    r4 = 0x0;
    fn_80119FA0();
    r3 = r30;
    r4 = 0x0;
    fn_80119F90();
    return;
}
/* 0x8011B950 | 0xBC */
void fn_8011B950(void) {
    extern void fn_80119F90();
    extern void fn_80119FA0();
    extern void fn_80119FB0();
    extern void fn_80119FC0();
    extern void fn_80119FD0();
    extern void fn_80119FE0();
    extern void fn_80119FF0();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r31 = r4 & 0xFFFF;
    r29 = 0x0;
    r28 = r3;
    while (r0 = r29 & 0xFFFF, (u32)r0 < (u32)r31) {
        /* clrlslwi r0, r29, 16, 4 */;
        /* add. r30, r28, r0 */;
        if ((s32)r0 != (s32)0) {
            r3 = r30;
            r4 = 0x0;
            fn_80119FF0();
            r3 = r30;
            r4 = 0x0;
            fn_80119FE0();
            r3 = r30;
            r4 = 0x0;
            fn_80119FD0();
            r3 = r30;
            r4 = 0x0;
            fn_80119FC0();
            r3 = r30;
            r4 = 0x0;
            fn_80119FB0();
            r3 = r30;
            r4 = 0x0;
            fn_80119FA0();
            r3 = r30;
            r4 = 0x0;
            fn_80119F90();
        }
        r29 = r29 + 0x1;
    }
    return;
}
/* 0x8011BA0C | 0xB4 */
void fn_8011BA0C(void) {
    extern u8 jumptable_8035C260[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    void (*ctr_fn)(void) = 0;
    r0 = r3 & 0xFF;
    r3 = 0x0;
    if ((u32)r0 > (u32)0xb) return;
    r4 = (u32)jumptable_8035C260;
    r0 = r0 << 2;
    r4 = (u32)jumptable_8035C260;
    r0 = *(u32*)(r4 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r3 = 0x0;
    return;
    r3 = (0x1 << 16);
    return;
    r3 = (0x1 << 16);
    return;
    r3 = (0x1 << 16);
    return;
    r3 = (0x1 << 16);
    return;
    r3 = (0x1 << 16);
    return;
    r3 = (0x1 << 16);
    return;
    r3 = (0x1 << 16);
    return;
    r3 = (0x1 << 16);
    return;
    r3 = (0x1 << 16);
    return;
    r3 = (0x1 << 16);
    return;
    r3 = (0x1 << 16);
    return;
}
/* 0x8011BAC0 | 0xAC */
void fn_8011BAC0(void) {
    extern void fn_8011BEB4();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = r3 & 0xFFFF;
    r29 = r3;
    if ((s32)r0 == (s32)0) {
        if ((u32)r0 == (u32)0x163) {
            if ((u32)r0 == (u32)0x165) {
                r3 = 0x0;
                return;
    }
    }
    }
    r31 = r4 & 0xFF;
    if ((u32)r0 == (u32)0x165) {
        r3 = 0x0;
        return;
    }
    r30 = 0x0;
    while (r0 = r30 & 0xFF, (u32)r0 < (u32)0x3) {

    r4 = r29;
    r6 = r30 & 0xFF;
    r3 = 0x0;
    r5 = 0x1a;
    fn_8011BEB4();
    r0 = r3 & 0xFF;
    if ((u32)r31 == (u32)r0) {
        r3 = 0x1;
        return;
    }
    r30 = r30 + 0x1;
    }

    r3 = 0x0;
    return;
}
/* 0x6C | fn_8011BB6C | single_call_straight */
void fn_8011BB6C(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_8011BEB4();
}
/* 0x8011BBD8 | 0x2DC */
void fn_8011BBD8(void) {
    extern void fn_8011C1D0();
    extern void fn_8011C220();
    extern void fn_8011C330();
    extern void fn_8011C380();
    extern void fn_8011C3D0();
    extern void fn_8011C420();
    extern void fn_8011C430();
    extern void fn_8011C5B8();
    extern void fn_8011C5E0();
    extern void fn_8011C5F0();
    extern void fn_8011C600();
    extern void fn_8011C610();
    extern void fn_8011C620();
    extern void fn_8011C630();
    extern void fn_8011C640();
    extern void fn_8011C650();
    extern void fn_8011C660();
    extern void fn_8011C670();
    extern void fn_8011C680();
    extern void fn_8011C690();
    extern void fn_8011C6A0();
    extern void fn_8011C6B0();
    extern void fn_8011C6C0();
    extern void fn_8011C6D0();
    extern void fn_8011C6E0();
    extern void fn_8011C6F0();
    extern void fn_8011C700();
    extern void fn_8011C710();
    extern void fn_8011C720();
    extern void fn_8011C730();
    extern void fn_8011C740();
    extern void fn_8011C750();
    extern void fn_8011C760();
    extern void fn_8011C770();
    extern void fn_8011C780();
    extern void fn_8011CA34();
    extern void fn_8020A0A4();
    extern void fn_8020A0B4();
    extern void fn_8020A0C4();
    extern void fn_8020A0D4();
    extern void fn_8020A0E4();
    extern void fn_8020A0F4();
    extern void fn_8020A104();
    extern void fn_8020A114();
    extern void fn_8020A124();
    extern void fn_8020A134();
    extern void fn_8020A144();
    extern void fn_8020A154();
    extern u8 jumptable_8035C290[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    r0 = r5 & 0xFFFF;
    r31 = r7;
    r30 = r6;
    r29 = r5;
    if ((s32)r0 == (s32)0) return;
    if ((u32)r0 >= (u32)0x35) {
        return;
    }
    if ((u32)r0 < (u32)0x25) {
        r3 = r4;
        fn_8011CA34();
        if ((u32)r3 == (u32)0x0) return;
    }
    if ((u32)r3 == (u32)0x0) return;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 > (u32)0x32) return;
    r4 = (u32)jumptable_8035C290;
    r0 = r0 << 2;
    r4 = (u32)jumptable_8035C290;
    r0 = *(u32*)(r4 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r4 = r31;
    fn_8011C780();
    return;
    r4 = r31 & 0xFF;
    fn_8011C770();
    return;
    r4 = r31 & 0xFF;
    fn_8011C760();
    return;
    r4 = (s8)r31;
    fn_8011C750();
    return;
    r4 = r31 & 0xFF;
    fn_8011C740();
    return;
    r4 = r31 & 0xFF;
    fn_8011C730();
    return;
    r4 = (s16)r31;
    fn_8011C720();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011C710();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011C700();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011C6F0();
    return;
    r4 = r31 & 0xFFFF;
    fn_8011C6F0();
    return;
    r4 = r31 & 0xFF;
    fn_8011C6E0();
    return;
    r4 = r31 & 0xFF;
    fn_8011C6D0();
    return;
    r4 = r31 & 0xFF;
    fn_8011C6C0();
    return;
    r4 = r31 & 0xFF;
    fn_8011C6B0();
    return;
    r4 = r31 & 0xFF;
    fn_8011C6A0();
    return;
    r4 = r31 & 0xFF;
    fn_8011C690();
    return;
    r4 = r31 & 0xFF;
    fn_8011C680();
    return;
    r4 = r31 & 0xFF;
    fn_8011C670();
    return;
    r4 = r31 & 0xFF;
    fn_8011C660();
    return;
    r4 = r31 & 0xFF;
    fn_8011C650();
    return;
    r4 = r31 & 0xFF;
    fn_8011C640();
    return;
    r4 = r31 & 0xFF;
    fn_8011C630();
    return;
    r4 = r31 & 0xFF;
    fn_8011C620();
    return;
    r4 = r31 & 0xFF;
    fn_8011C5B8();
    return;
    r4 = r30;
    r5 = r31 & 0xFF;
    fn_8011C430();
    return;
    r4 = r31 & 0xFF;
    fn_8011C420();
    return;
    r4 = r31;
    fn_8011C3D0();
    return;
    r4 = r31;
    fn_8011C380();
    return;
    r4 = r31;
    fn_8011C330();
    return;
    r4 = r31;
    fn_8011C220();
    return;
    r4 = r31;
    fn_8011C1D0();
    return;
    r4 = r31;
    fn_8011C610();
    return;
    r4 = r31;
    fn_8011C600();
    return;
    r4 = r31 & 0xFF;
    fn_8011C5F0();
    return;
    r4 = r31 & 0xFF;
    fn_8011C5E0();
    return;
    r4 = (s8)r31;
    fn_8020A154();
    return;
    r4 = r31 & 0xFFFF;
    fn_8020A144();
    return;
    r4 = r31 & 0xFFFF;
    fn_8020A134();
    return;
    r4 = r31 & 0xFFFF;
    fn_8020A124();
    return;
    r4 = r31 & 0xFF;
    fn_8020A114();
    return;
    r4 = r31 & 0xFF;
    fn_8020A104();
    return;
    r4 = r31;
    fn_8020A0F4();
    return;
    r4 = r31;
    fn_8020A0E4();
    return;
    r4 = r31 & 0xFFFF;
    fn_8020A0D4();
    return;
    r4 = r31 & 0xFFFF;
    fn_8020A0C4();
    return;
    r4 = r31 & 0xFF;
    fn_8020A0B4();
    return;
    r4 = r31 & 0xFF;
    fn_8020A0A4();
    return;
}
/* 0x8011BEB4 | 0x31C */
void fn_8011BEB4(void) {
    extern void fn_8011BEB4();
    extern void fn_8011C270();
    extern void fn_8011C2D0();
    extern void fn_8011C450();
    extern void fn_8011C4B0();
    extern void fn_8011C510();
    extern void fn_8011C570();
    extern void fn_8011C588();
    extern void fn_8011C5C8();
    extern void fn_8011C790();
    extern void fn_8011C7A8();
    extern void fn_8011C7C0();
    extern void fn_8011C7D8();
    extern void fn_8011C7F0();
    extern void fn_8011C808();
    extern void fn_8011C820();
    extern void fn_8011C838();
    extern void fn_8011C850();
    extern void fn_8011C868();
    extern void fn_8011C880();
    extern void fn_8011C898();
    extern void fn_8011C8B0();
    extern void fn_8011C8C8();
    extern void fn_8011C8E0();
    extern void fn_8011C8F8();
    extern void fn_8011C910();
    extern void fn_8011C928();
    extern void fn_8011C940();
    extern void fn_8011C958();
    extern void fn_8011C970();
    extern void fn_8011C988();
    extern void fn_8011C9A0();
    extern void fn_8011C9B8();
    extern void fn_8011C9D0();
    extern void fn_8011C9EC();
    extern void fn_8011CA04();
    extern void fn_8011CA1C();
    extern void fn_8011CA34();
    extern void fn_8020A164();
    extern void fn_8020A17C();
    extern void fn_8020A194();
    extern void fn_8020A1AC();
    extern void fn_8020A1C4();
    extern void fn_8020A1DC();
    extern void fn_8020A1F4();
    extern void fn_8020A20C();
    extern void fn_8020A224();
    extern void fn_8020A258();
    extern void fn_8020A270();
    extern void fn_8020A288();
    extern void fn_8020A2A0();
    extern u8 jumptable_8035C35C[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    r0 = r5 & 0xFFFF;
    r31 = r6;
    r30 = r5;
    if ((s32)r0 == (s32)0) { r3 = 0x0; return; }
    if ((u32)r0 >= (u32)0x35) {
        r3 = 0x0;
        return;
    }
    if ((u32)r0 < (u32)0x25) {
        r3 = r4;
        fn_8011CA34();
        if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        return;
        }
    } else {
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    }
    r0 = r30 & 0xFFFF;
    if ((u32)r0 <= (u32)0x34) {
        r4 = (u32)jumptable_8035C35C;
        r0 = r0 << 2;
        r4 = (u32)jumptable_8035C35C;
        r0 = *(u32*)(r4 + r0);
        ctr_fn = (void(*)(void))r0;
        /* indirect jump via ctr */;
        fn_8011CA1C();
        return;
        fn_8011CA04();
        r3 = r3 & 0xFF;
        return;
        fn_8011C9EC();
        r3 = r3 & 0xFF;
        return;
        fn_8011C9D0();
        r3 = (s8)r3;
        return;
        fn_8011C9B8();
        r3 = r3 & 0xFF;
        return;
        fn_8011C9A0();
        r3 = r3 & 0xFF;
        return;
        fn_8011C988();
        r3 = (s16)r3;
        return;
        fn_8011C970();
        r3 = r3 & 0xFFFF;
        return;
        fn_8011C958();
        r3 = r3 & 0xFFFF;
        return;
        fn_8011C940();
        return;
        fn_8011C928();
        return;
        fn_8011C910();
        r3 = r3 & 0xFF;
        return;
        fn_8011C8F8();
        r3 = r3 & 0xFF;
        return;
        fn_8011C8E0();
        r3 = r3 & 0xFF;
        return;
        fn_8011C8C8();
        r3 = r3 & 0xFF;
        return;
        fn_8011C8B0();
        r3 = r3 & 0xFF;
        return;
        fn_8011C898();
        r3 = r3 & 0xFF;
        return;
        fn_8011C880();
        r3 = r3 & 0xFF;
        return;
        fn_8011C868();
        r3 = r3 & 0xFF;
        return;
        fn_8011C850();
        r3 = r3 & 0xFF;
        return;
        fn_8011C838();
        r3 = r3 & 0xFF;
        return;
        fn_8011C820();
        r3 = r3 & 0xFF;
        return;
        fn_8011C808();
        r3 = r3 & 0xFF;
        return;
        fn_8011C7F0();
        r3 = r3 & 0xFF;
        return;
        fn_8011C5C8();
        r3 = r3 & 0xFF;
        return;
        r4 = r31;
        fn_8011C588();
        r3 = r3 & 0xFF;
        return;
        fn_8011C570();
        r3 = r3 & 0xFF;
        return;
        fn_8011C510();
        return;
        fn_8011C4B0();
        return;
        fn_8011C450();
        return;
        fn_8011C2D0();
        return;
        fn_8011C270();
        return;
        fn_8011C7D8();
        return;
        fn_8011C7C0();
        return;
        fn_8011C7A8();
        r3 = r3 & 0xFF;
        return;
        fn_8011C790();
        r3 = r3 & 0xFF;
        return;
        fn_8020A2A0();
        r3 = (s8)r3;
        return;
        fn_8020A288();
        r3 = r3 & 0xFFFF;
        return;
        fn_8020A270();
        r3 = r3 & 0xFFFF;
        return;
        fn_8020A258();
        r3 = r3 & 0xFFFF;
        return;
        r4 = r31;
        fn_8020A224();
        return;
        fn_8020A20C();
        r3 = r3 & 0xFF;
        return;
        fn_8020A1F4();
        r3 = r3 & 0xFF;
        return;
        fn_8020A1DC();
        return;
        fn_8020A1C4();
        return;
        fn_8020A1AC();
        r3 = r3 & 0xFFFF;
        return;
        fn_8020A194();
        r3 = r3 & 0xFFFF;
        return;
        fn_8020A17C();
        r3 = r3 & 0xFF;
        return;
        fn_8020A164();
        r3 = r3 & 0xFF;
        return;
        r4 = 0x0;
        r5 = 0x27;
        r6 = 0x0;
        fn_8011BEB4();
        r0 = r3;
        r3 = 0x0;
        r4 = r0 & 0xFFFF;
        r5 = 0x3;
        r6 = 0x0;
        fn_8011BEB4();
        return;
        r4 = 0x0;
        r5 = 0x28;
        r6 = 0x0;
        fn_8011BEB4();
        r0 = r3;
        r3 = 0x0;
        r4 = r0 & 0xFFFF;
        r5 = 0x3;
        r6 = 0x0;
        fn_8011BEB4();
        return;
    }
    r3 = 0x0;
    return;
}
/* 0x50 | fn_8011C1D0 | index_lookup */
void* fn_8011C1D0(u16 idx) {
    if (idx >= lbl_80478B50) { return NULL; }
    return (u8*)lbl_8035C430 + idx * 0x14;
}
/* 0x50 | fn_8011C220 | index_lookup */
void* fn_8011C220(u16 idx) {
    if (idx >= lbl_80478B50) { return NULL; }
    return (u8*)lbl_8035C430 + idx * 0x14;
}
/* 0x60 | fn_8011C270 | index_lookup */
u32 fn_8011C270(u16 idx) {
    void* entry;
    if (idx >= lbl_80478B50) { return 0; }
    entry = (u8*)lbl_8035C430 + idx * 0x14;
    if (entry == NULL) { return 0; }
    return *(u32*)((u8*)entry + 0x10);
}
/* 0x60 | fn_8011C2D0 | index_lookup */
u32 fn_8011C2D0(u16 idx) {
    void* entry;
    if (idx >= lbl_80478B50) { return 0; }
    entry = (u8*)lbl_8035C430 + idx * 0x14;
    if (entry == NULL) { return 0; }
    return *(u32*)((u8*)entry + 0xC);
}
/* 0x50 | fn_8011C330 | index_lookup */
void* fn_8011C330(u16 idx) {
    if (idx >= lbl_80478B50) { return NULL; }
    return (u8*)lbl_8035C430 + idx * 0x14;
}
/* 0x50 | fn_8011C380 | index_lookup */
void* fn_8011C380(u16 idx) {
    if (idx >= lbl_80478B50) { return NULL; }
    return (u8*)lbl_8035C430 + idx * 0x14;
}
/* 0x50 | fn_8011C3D0 | index_lookup */
void* fn_8011C3D0(u16 idx) {
    if (idx >= lbl_80478B50) { return NULL; }
    return (u8*)lbl_8035C430 + idx * 0x14;
}
/* 0x8011C430 | 0x20 */
void fn_8011C430(u8* ptr, u16 idx, u8 val) {
    if (ptr == NULL) { return; }
    if (idx >= 3) { return; }
    ptr[(u16)idx + 0x34] = val;
}
/* 0x60 | fn_8011C450 | index_lookup */
u32 fn_8011C450(u16 idx) {
    void* entry;
    if (idx >= lbl_80478B50) { return 0; }
    entry = (u8*)lbl_8035C430 + idx * 0x14;
    if (entry == NULL) { return 0; }
    return *(u32*)((u8*)entry + 0x8);
}
/* 0x60 | fn_8011C4B0 | index_lookup */
u32 fn_8011C4B0(u16 idx) {
    void* entry;
    if (idx >= lbl_80478B50) { return 0; }
    entry = (u8*)lbl_8035C430 + idx * 0x14;
    if (entry == NULL) { return 0; }
    return *(u32*)((u8*)entry + 0x4);
}
/* 0x60 | fn_8011C510 | index_lookup */
u32 fn_8011C510(u16 idx) {
    void* entry;
    if (idx >= lbl_80478B50) { return 0; }
    entry = (u8*)lbl_8035C430 + idx * 0x14;
    if (entry == NULL) { return 0; }
    return *(u32*)((u8*)entry + 0x0);
}
/* 0x8011C588 | 0x30 */
u8 fn_8011C588(u8* ptr, u16 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 3) { return 0; }
    return ptr[(u16)idx + 0x34];
}
/* 0x8011CA34 | 0x2C */
extern u32 lbl_80478DF8;
extern u32 lbl_80478DFC;
void* fn_8011CA34(u16 idx) {
    u32* hdr = (u32*)lbl_80478DF8;
    if ((u16)idx >= hdr[0]) {
        return (void*)lbl_80478DFC;
    }
    return (u8*)lbl_80478DFC + (u32)idx * 0x38;
}
/* 0x8011CA60 | 0x3C */
extern u32 lbl_80478B78;
extern u32 lbl_8035F9A8[];
u32 fn_8011CA60(u8* ptr) {
    u16 idx;
    if (ptr == NULL) { return 0; }
    idx = *(u16*)(&ptr[0x10]);
    if (idx >= lbl_80478B78) { return 0; }
    return lbl_8035F9A8[(u16)idx];
}
/* 0x8011CAB8 | 0x28 */
extern u32 lbl_80478E68;
extern u32 lbl_80478E6C;
void* fn_8011CAB8(u16 idx) {
    u32* hdr = (u32*)lbl_80478E68;
    if ((u16)idx >= hdr[0]) { return NULL; }
    return (u8*)lbl_80478E6C + (u16)idx;
}
/* 0x8011CAE0 | 0x30 */
s8 fn_8011CAE0(u8* ptr, u8 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 3) { return 0; }
    return (s8)ptr[(u8)idx];
}
/* 0x8011CB10 | 0x2C */
extern u32 lbl_80478B70;
extern u8 lbl_8035F988[];
void* fn_8011CB10(u16 idx) {
    if ((u16)idx >= lbl_80478B70) { return NULL; }
    return (u8*)lbl_8035F988 + (u32)idx * 3;
}
/* 0x8011CB6C | 0x2C */
extern u32 lbl_80478B68;
extern u8 lbl_8035F5E0[];
void* fn_8011CB6C(u16 idx) {
    if ((u16)idx >= lbl_80478B68) { return NULL; }
    return (u8*)lbl_8035F5E0 + (u32)idx * 0xC;
}
/* 0x8011CBC8 | 0x2C */
extern u32 lbl_80478E58;
extern u32 lbl_80478E5C;
void* fn_8011CBC8(u8 idx) {
    u32* hdr = (u32*)lbl_80478E58;
    if ((u8)idx >= hdr[0]) { return NULL; }
    return (u8*)lbl_80478E5C + (u32)idx * 2;
}
/* 0x8011CBF4 | 0x30 */
u8 fn_8011CBF4(u8* ptr, u8 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 7) { return 0; }
    return ptr[(u8)idx + 0x1F];
}
/* 0x8011CC24 | 0x30 */
u8 fn_8011CC24(u8* ptr, u8 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 7) { return 0; }
    return ptr[(u8)idx + 0x18];
}
/* 0x8011CE18 | 0x2C */
extern u32 lbl_80478E60;
extern u32 lbl_80478E64;
void* fn_8011CE18(u8 idx) {
    u32* hdr = (u32*)lbl_80478E60;
    if ((u8)idx >= hdr[0]) { return NULL; }
    return (u8*)lbl_80478E64 + (u32)idx * 0x28;
}
/* 0x8011CE44 | 0x30 */
u32 fn_8011CE44(u8* ptr, u8 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 0x65) { return 0; }
    return *(u32*)(&ptr[(u32)idx * 4]);
}
/* 0x8011CE74 | 0x2C */
extern u32 lbl_80478B60;
extern u8 lbl_8035E940[];
void* fn_8011CE74(u8 idx) {
    if ((u8)idx >= lbl_80478B60) { return NULL; }
    return (u8*)lbl_8035E940 + (u32)idx * 0x194;
}
/* 0x8011CED0 | 0x20 */
void fn_8011CED0(u8* ptr, u16 idx, u8 val) {
    if (ptr == NULL) { return; }
    if (idx >= 2) { return; }
    ptr[(u16)idx + 0x6E] = val;
}
/* 0x8011CEF0 | 0x24 */
void fn_8011CEF0(u8* ptr, u16 idx, u16 val) {
    if (ptr == NULL) { return; }
    if (idx >= 8) { return; }
    *(u16*)(&ptr[idx * 2 + 0x74]) = val;
}
/* 0x8011CF44 | 0x2C */
extern void fn_8019075C(u32);
void fn_8011CF44(u8* ptr) {
    if (ptr == NULL) { return; }
    fn_8019075C(*(u32*)(&ptr[0x28]));
}
/* 0x8011CF70 | 0x2C */
void fn_8011CF70(u8* ptr) {
    if (ptr == NULL) { return; }
    fn_8019075C(*(u32*)(&ptr[0x24]));
}
/* 0x40 | fn_8011CFEC | compound_indexed_setter */
void fn_8011CFEC(u8* ptr, u16 idx, u8 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 2) {
        sub = NULL;
    } else {
        sub = ptr + (u16)idx * 8 + 0x10C;
    }
    if (sub == NULL) { return; }
    *(u8*)(sub + 0x0) = val;
}
/* 0x40 | fn_8011D02C | compound_indexed_setter */
void fn_8011D02C(u8* ptr, u16 idx, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 2) {
        sub = NULL;
    } else {
        sub = ptr + (u16)idx * 8 + 0x10C;
    }
    if (sub == NULL) { return; }
    *(u16*)(sub + 0x2) = val;
}
/* 0x40 | fn_8011D06C | compound_indexed_setter */
void fn_8011D06C(u8* ptr, u16 idx, u32 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 2) {
        sub = NULL;
    } else {
        sub = ptr + (u16)idx * 8 + 0x10C;
    }
    if (sub == NULL) { return; }
    *(u32*)(sub + 0x4) = val;
}
/* 0x8011D0AC | 0x20 */
void fn_8011D0AC(u8* ptr, u16 idx, u8 val) {
    if (ptr == NULL) { return; }
    if (idx >= 0x3A) { return; }
    ptr[(u16)idx + 0x34] = val;
}
/* 0x40 | fn_8011D0CC | compound_indexed_setter */
void fn_8011D0CC(u8* ptr, u16 idx, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 0x14) {
        sub = NULL;
    } else {
        sub = ptr + (u16)idx * 4 + 0xBA;
    }
    if (sub == NULL) { return; }
    *(u16*)(sub + 0x2) = val;
}
/* 0x40 | fn_8011D10C | compound_indexed_setter */
void fn_8011D10C(u8* ptr, u16 idx, u8 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 0x14) {
        sub = NULL;
    } else {
        sub = ptr + (u16)idx * 4 + 0xBA;
    }
    if (sub == NULL) { return; }
    *(u8*)(sub + 0x0) = val;
}
/* 0x40 | fn_8011D14C | compound_indexed_setter */
void fn_8011D14C(u8* ptr, u16 idx, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 5) {
        sub = NULL;
    } else {
        sub = ptr + (u16)idx * 6 + 0x9C;
    }
    if (sub == NULL) { return; }
    *(u16*)(sub + 0x4) = val;
}
/* 0x40 | fn_8011D18C | compound_indexed_setter */
void fn_8011D18C(u8* ptr, u16 idx, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 5) {
        sub = NULL;
    } else {
        sub = ptr + (u16)idx * 6 + 0x9C;
    }
    if (sub == NULL) { return; }
    *(u16*)(sub + 0x2) = val;
}
/* 0x40 | fn_8011D1CC | compound_indexed_setter */
void fn_8011D1CC(u8* ptr, u16 idx, u8 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 5) {
        sub = NULL;
    } else {
        sub = ptr + (u16)idx * 6 + 0x9C;
    }
    if (sub == NULL) { return; }
    *(u8*)(sub + 0x0) = val;
}
/* 0x8011D20C | 0x20 */
void fn_8011D20C(u8* ptr, u16 idx, u8 val) {
    if (ptr == NULL) { return; }
    if (idx >= 2) { return; }
    ptr[(u16)idx + 0x32] = val;
}
/* 0x8011D22C | 0x20 */
void fn_8011D22C(u8* ptr, u16 idx, u8 val) {
    if (ptr == NULL) { return; }
    if (idx >= 2) { return; }
    ptr[(u16)idx + 0x30] = val;
}
/* 0x8011D24C | 0x24 */
void fn_8011D24C(u8* ptr, u16 idx, u16 val) {
    if (ptr == NULL) { return; }
    if (idx >= 2) { return; }
    *(u16*)(&ptr[idx * 2 + 0x70]) = val;
}
/* 0x8011D2C0 | 36 bytes | compound_setter */
void fn_8011D2C0(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0x90;
    if (sub == NULL) { return; }
    *(u16*)((u8*)sub + 0xA) = val;
}
/* 0x8011D2E4 | 36 bytes | compound_setter */
void fn_8011D2E4(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0x90;
    if (sub == NULL) { return; }
    *(u16*)((u8*)sub + 0x8) = val;
}
/* 0x8011D308 | 36 bytes | compound_setter */
void fn_8011D308(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0x90;
    if (sub == NULL) { return; }
    *(u16*)((u8*)sub + 0x6) = val;
}
/* 0x8011D32C | 36 bytes | compound_setter */
void fn_8011D32C(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0x90;
    if (sub == NULL) { return; }
    *(u16*)((u8*)sub + 0x4) = val;
}
/* 0x8011D350 | 36 bytes | compound_setter */
void fn_8011D350(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0x90;
    if (sub == NULL) { return; }
    *(u16*)((u8*)sub + 0x2) = val;
}
/* 0x8011D374 | 36 bytes | compound_setter */
void fn_8011D374(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0x90;
    if (sub == NULL) { return; }
    *(u16*)((u8*)sub + 0x0) = val;
}
/* 0x8011D398 | 36 bytes | compound_setter */
void fn_8011D398(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0x84;
    if (sub == NULL) { return; }
    *(u16*)((u8*)sub + 0xA) = val;
}
/* 0x8011D3BC | 36 bytes | compound_setter */
void fn_8011D3BC(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0x84;
    if (sub == NULL) { return; }
    *(u16*)((u8*)sub + 0x8) = val;
}
/* 0x8011D3E0 | 36 bytes | compound_setter */
void fn_8011D3E0(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0x84;
    if (sub == NULL) { return; }
    *(u16*)((u8*)sub + 0x6) = val;
}
/* 0x8011D404 | 36 bytes | compound_setter */
void fn_8011D404(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0x84;
    if (sub == NULL) { return; }
    *(u16*)((u8*)sub + 0x4) = val;
}
/* 0x8011D428 | 36 bytes | compound_setter */
void fn_8011D428(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0x84;
    if (sub == NULL) { return; }
    *(u16*)((u8*)sub + 0x2) = val;
}
/* 0x8011D44C | 36 bytes | compound_setter */
void fn_8011D44C(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0x84;
    if (sub == NULL) { return; }
    *(u16*)((u8*)sub + 0x0) = val;
}
/* 0x8011D480 | 0x14 */
void fn_8011D480(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0xF8]) = (u32)val;
}
/* 0x68 | fn_8011D504 | compound_chained_setter */
void fn_8011D504(u8* ptr, u8 val) {
    u16 idx;
    void* entry;
    u8 check;
    if (ptr == NULL) { return; }
    if (ptr == NULL) {
        idx = 0;
    } else {
        idx = *(u16*)(ptr + 0x0);
    }
    if ((u16)idx >= *(u32*)lbl_80478F90) {
        entry = NULL;
    } else {
        entry = (u8*)lbl_80478F94 + (u32)(u16)idx * 0x11C;
    }
    if (entry == NULL) {
        check = 0;
    } else {
        check = *(u8*)((u8*)entry + 0x33);
    }
    if (check == 0) {
        val = 0;
    }
    *(u8*)(ptr + 0xCC) = val;
}
/* 0x8011D58C | 36 bytes | compound_setter */
void fn_8011D58C(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0xC) = val;
}
/* 0x8011D5B0 | 36 bytes | compound_setter */
void fn_8011D5B0(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0xB) = val;
}
/* 0x8011D5D4 | 36 bytes | compound_setter */
void fn_8011D5D4(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0xA) = val;
}
/* 0x8011D5F8 | 36 bytes | compound_setter */
void fn_8011D5F8(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0x9) = val;
}
/* 0x8011D61C | 36 bytes | compound_setter */
void fn_8011D61C(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0x8) = val;
}
/* 0x8011D640 | 36 bytes | compound_setter */
void fn_8011D640(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0x7) = val;
}
/* 0x8011D664 | 36 bytes | compound_setter */
void fn_8011D664(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0x6) = val;
}
/* 0x8011D688 | 36 bytes | compound_setter */
void fn_8011D688(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0x5) = val;
}
/* 0x8011D6AC | 36 bytes | compound_setter */
void fn_8011D6AC(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0x4) = val;
}
/* 0x8011D6D0 | 36 bytes | compound_setter */
void fn_8011D6D0(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0x3) = val;
}
/* 0x8011D6F4 | 36 bytes | compound_setter */
void fn_8011D6F4(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0x2) = val;
}
/* 0x8011D718 | 36 bytes | compound_setter */
void fn_8011D718(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0x1) = val;
}
/* 0x8011D73C | 36 bytes | compound_setter */
void fn_8011D73C(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0x0) = val;
}
/* 0x8011D770 | 36 bytes | compound_setter */
void fn_8011D770(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xB7;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0x4) = val;
}
/* 0x8011D794 | 36 bytes | compound_setter */
void fn_8011D794(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xB7;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0x3) = val;
}
/* 0x8011D7B8 | 36 bytes | compound_setter */
void fn_8011D7B8(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xB7;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0x2) = val;
}
/* 0x8011D7DC | 36 bytes | compound_setter */
void fn_8011D7DC(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xB7;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0x1) = val;
}
/* 0x8011D800 | 36 bytes | compound_setter */
void fn_8011D800(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xB7;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0x0) = val;
}
/* 0x8011D824 | 36 bytes | compound_setter */
void fn_8011D824(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xB2;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0x4) = val;
}
/* 0x8011D848 | 36 bytes | compound_setter */
void fn_8011D848(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xB2;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0x3) = val;
}
/* 0x8011D86C | 36 bytes | compound_setter */
void fn_8011D86C(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xB2;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0x2) = val;
}
/* 0x8011D890 | 36 bytes | compound_setter */
void fn_8011D890(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xB2;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0x1) = val;
}
/* 0x8011D8B4 | 36 bytes | compound_setter */
void fn_8011D8B4(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0xB2;
    if (sub == NULL) { return; }
    *(u8*)((u8*)sub + 0x0) = val;
}
/* 0x8011D8D8 | 0x1C */
void fn_8011D8D8(u8* ptr, s32 val) {
    if (ptr == NULL) { return; }
    if (val >= 0x639C) { val = 0x639C; }
    *(u32*)(&ptr[0xDC]) = (u32)val;
}
/* 0x8011D904 | 0x20 */
void fn_8011D904(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    if ((u16)val > 0xFF) { val = 0xFF; }
    *(u16*)(&ptr[0xB0]) = val;
}
/* 0x8011D924 | 0x34 */
void fn_8011D924(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) { return; }
    sub = ptr + 0xA4;
    if (sub == NULL) { return; }
    if ((u16)val > 0x1F) { val = 0x1F; }
    *(u16*)(sub + 0xA) = val;
}
/* 0x8011D958 | 0x34 */
void fn_8011D958(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) { return; }
    sub = ptr + 0xA4;
    if (sub == NULL) { return; }
    if ((u16)val > 0x1F) { val = 0x1F; }
    *(u16*)(sub + 0x8) = val;
}
/* 0x8011D98C | 0x34 */
void fn_8011D98C(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) { return; }
    sub = ptr + 0xA4;
    if (sub == NULL) { return; }
    if ((u16)val > 0x1F) { val = 0x1F; }
    *(u16*)(sub + 0x6) = val;
}
/* 0x8011D9C0 | 0x34 */
void fn_8011D9C0(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) { return; }
    sub = ptr + 0xA4;
    if (sub == NULL) { return; }
    if ((u16)val > 0x1F) { val = 0x1F; }
    *(u16*)(sub + 0x4) = val;
}
/* 0x8011D9F4 | 0x34 */
void fn_8011D9F4(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) { return; }
    sub = ptr + 0xA4;
    if (sub == NULL) { return; }
    if ((u16)val > 0x1F) { val = 0x1F; }
    *(u16*)(sub + 0x2) = val;
}
/* 0x8011DA28 | 0x34 */
void fn_8011DA28(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) { return; }
    sub = ptr + 0xA4;
    if (sub == NULL) { return; }
    if ((u16)val > 0x1F) { val = 0x1F; }
    *(u16*)(sub + 0x0) = val;
}
/* 0x8011DA5C | 0x34 */
void fn_8011DA5C(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) { return; }
    sub = ptr + 0x98;
    if (sub == NULL) { return; }
    if ((u16)val > 0xFF) { val = 0xFF; }
    *(u16*)(sub + 0xA) = val;
}
/* 0x8011DA90 | 0x34 */
void fn_8011DA90(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) { return; }
    sub = ptr + 0x98;
    if (sub == NULL) { return; }
    if ((u16)val > 0xFF) { val = 0xFF; }
    *(u16*)(sub + 0x8) = val;
}
/* 0x8011DAC4 | 0x34 */
void fn_8011DAC4(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) { return; }
    sub = ptr + 0x98;
    if (sub == NULL) { return; }
    if ((u16)val > 0xFF) { val = 0xFF; }
    *(u16*)(sub + 0x6) = val;
}
/* 0x8011DAF8 | 0x34 */
void fn_8011DAF8(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) { return; }
    sub = ptr + 0x98;
    if (sub == NULL) { return; }
    if ((u16)val > 0xFF) { val = 0xFF; }
    *(u16*)(sub + 0x4) = val;
}
/* 0x8011DB2C | 0x34 */
void fn_8011DB2C(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) { return; }
    sub = ptr + 0x98;
    if (sub == NULL) { return; }
    if ((u16)val > 0xFF) { val = 0xFF; }
    *(u16*)(sub + 0x2) = val;
}
/* 0x8011DB60 | 0x34 */
void fn_8011DB60(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) { return; }
    sub = ptr + 0x98;
    if (sub == NULL) { return; }
    if ((u16)val > 0xFF) { val = 0xFF; }
    *(u16*)(sub + 0x0) = val;
}
/* 0x8011DB94 | 36 bytes | compound_setter */
void fn_8011DB94(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0x8C;
    if (sub == NULL) { return; }
    *(u16*)((u8*)sub + 0xA) = val;
}
/* 0x8011DBB8 | 36 bytes | compound_setter */
void fn_8011DBB8(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0x8C;
    if (sub == NULL) { return; }
    *(u16*)((u8*)sub + 0x8) = val;
}
/* 0x8011DBDC | 36 bytes | compound_setter */
void fn_8011DBDC(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0x8C;
    if (sub == NULL) { return; }
    *(u16*)((u8*)sub + 0x6) = val;
}
/* 0x8011DC00 | 36 bytes | compound_setter */
void fn_8011DC00(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0x8C;
    if (sub == NULL) { return; }
    *(u16*)((u8*)sub + 0x4) = val;
}
/* 0x8011DC24 | 36 bytes | compound_setter */
void fn_8011DC24(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0x8C;
    if (sub == NULL) { return; }
    *(u16*)((u8*)sub + 0x2) = val;
}
/* 0x8011DC48 | 36 bytes | compound_setter */
void fn_8011DC48(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) { return; }
    sub = (u8*)ptr + 0x8C;
    if (sub == NULL) { return; }
    *(u16*)((u8*)sub + 0x0) = val;
}
/* 0x48 | fn_8011DC6C | compound_clamp_setter */
void fn_8011DC6C(u8* ptr, u16 val) {
    void* sub;
    u16 subVal;
    if (ptr == NULL) { return; }
    *(u16*)(ptr + 0x8A) = val;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) {
        subVal = 0;
    } else {
        subVal = *(u16*)((u8*)sub + 0x0);
    }
    if ((u16)subVal >= *(u16*)(ptr + 0x8A)) { return; }
    *(u16*)(ptr + 0x8A) = subVal;
}
/* 0x8011DCC4 | 0xBC */
void fn_8011DCC4(void) {
    extern void fn_8011CA04();
    extern void fn_8011CA34();
    extern void fn_8011F260();
    extern void fn_80123CD4();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r30 = r5;
    r5 = 0x0;
    r29 = r4;
    r28 = r3;
    fn_8011F260();
    if ((s32)r0 == (s32)0) return;
    r0 = r30 & 0xFF;
    if ((u32)r0 > (u32)0x3) {
        r30 = 0x3;
    }
    r3 = r28;
    r4 = r29;
    fn_80123CD4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r28;
        r4 = r29;
        r5 = 0x1;
        fn_8011F260();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
        } else {
            r3 = *(u16*)((u8*)r3 + 0x0);
        }
        fn_8011CA34();
        fn_8011CA04();
        r0 = r3 & 0xFF;
        if ((u32)r0 <= (u32)0x4) return;
    }
    *(u8*)((u8*)r31 + 0x3) = r30;
    return;
}
/* 0x7C | fn_8011DD80 | call_clamp_store */
void fn_8011DD80(u32 arg1, u32 arg2, u8 maxVal) {
    extern void* fn_8011F260();
    extern u32 fn_80123E70();
    u8* result;
    u8 val;
    result = fn_8011F260(arg1, arg2, 0);
    if (result == NULL) { return; }
    val = fn_80123E70(arg1, arg2);
    if ((u8)val < (u8)maxVal) {
        maxVal = val;
    }
    *(u8*)(result + 0x2) = maxVal;
}
/* 0x8011DDFC | 0x3C */
void fn_8011DDFC(void* ctx, u32 p1, u32 value) {
    extern void* fn_8011F260();
    void* result = fn_8011F260(ctx, p1, 0);
    if (result != 0) {
        *(u16*)result = (u16)value;
    }
}
/* 0x8011DE48 | 0x20 */
void fn_8011DE48(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    if ((u8)val > 0x64) { val = 0x64; }
    ptr[0x60] = val;
}
/* 0x8011DE68 | 0x20 */
void fn_8011DE68(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    if ((u16)val > 0xFF) { val = 0xFF; }
    *(u16*)(&ptr[0xE4]) = val;
}
/* 0x8011DEA8 | 0x3C */
void fn_8011DEA8(u8* ptr, void* src) {
    if (ptr == NULL) { return; }
    if (src == NULL) { return; }
    fn_800F9D24(ptr + 0x44, src, 0xB);
}
/* 0x70 | fn_8011DEE4 | dual_memcpy_setter */
void fn_8011DEE4(u8* ptr, void* src) {
    extern void fn_800F9D24();
    if (ptr == NULL) { return; }
    if (src == NULL) { return; }
    fn_800F9D24(ptr + 0x2E, src, 0xB);
    if (ptr == NULL) { return; }
    if (src == NULL) { return; }
    fn_800F9D24(ptr + 0x44, src, 0xB);
}
/* 0x8011DF54 | 0x3C */
void fn_8011DF54(u8* ptr, void* src) {
    if (ptr == NULL) { return; }
    if (src == NULL) { return; }
    fn_800F9D24(ptr + 0x18, src, 0xB);
}
/* 0x8011E048 | 0x30 */
u8 fn_8011E048(u8* ptr, u16 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 2) { return 0; }
    return ptr[(u16)idx + 0x6E];
}
/* 0x8011E078 | 0x34 */
u16 fn_8011E078(u8* ptr, u16 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 8) { return 0; }
    return *(u16*)(&ptr[idx * 2 + 0x74]);
}
/* 0x8011E0F4 | 0x34 */
extern void* fn_801906A0(u32);
void* fn_8011E0F4(u8* ptr) {
    if (ptr == NULL) { return NULL; }
    return fn_801906A0(*(u32*)(&ptr[0x28]));
}
/* 0x8011E128 | 0x34 */
void* fn_8011E128(u8* ptr) {
    if (ptr == NULL) { return NULL; }
    return fn_801906A0(*(u32*)(&ptr[0x24]));
}
/* 0x48 | fn_8011E1D4 | compound_indexed_getter */
u8 fn_8011E1D4(u8* ptr, u16 idx) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 2) {
        sub = NULL;
    } else {
        sub = ptr + (u16)idx * 8 + 0x10C;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)(sub + 0x0);
}
/* 0x48 | fn_8011E21C | compound_indexed_getter */
u16 fn_8011E21C(u8* ptr, u16 idx) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 2) {
        sub = NULL;
    } else {
        sub = ptr + (u16)idx * 8 + 0x10C;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)(sub + 0x2);
}
/* 0x48 | fn_8011E264 | compound_indexed_getter */
u32 fn_8011E264(u8* ptr, u16 idx) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 2) {
        sub = NULL;
    } else {
        sub = ptr + (u16)idx * 8 + 0x10C;
    }
    if (sub == NULL) { return 0; }
    return *(u32*)(sub + 0x4);
}
/* 0x8011E2AC | 0x30 */
u8 fn_8011E2AC(u8* ptr, u16 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 0x3A) { return 0; }
    return ptr[(u16)idx + 0x34];
}
/* 0x48 | fn_8011E2DC | compound_indexed_getter */
u16 fn_8011E2DC(u8* ptr, u16 idx) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 0x14) {
        sub = NULL;
    } else {
        sub = ptr + (u16)idx * 4 + 0xBA;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)(sub + 0x2);
}
/* 0x48 | fn_8011E324 | compound_indexed_getter */
u8 fn_8011E324(u8* ptr, u16 idx) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 0x14) {
        sub = NULL;
    } else {
        sub = ptr + (u16)idx * 4 + 0xBA;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)(sub + 0x0);
}
/* 0x48 | fn_8011E36C | compound_indexed_getter */
u16 fn_8011E36C(u8* ptr, u16 idx) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 5) {
        sub = NULL;
    } else {
        sub = ptr + (u16)idx * 6 + 0x9C;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)(sub + 0x4);
}
/* 0x48 | fn_8011E3B4 | compound_indexed_getter */
u16 fn_8011E3B4(u8* ptr, u16 idx) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 5) {
        sub = NULL;
    } else {
        sub = ptr + (u16)idx * 6 + 0x9C;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)(sub + 0x2);
}
/* 0x48 | fn_8011E3FC | compound_indexed_getter */
u8 fn_8011E3FC(u8* ptr, u16 idx) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 5) {
        sub = NULL;
    } else {
        sub = ptr + (u16)idx * 6 + 0x9C;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)(sub + 0x0);
}
/* 0x8011E444 | 0x30 */
u8 fn_8011E444(u8* ptr, u16 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 2) { return 0; }
    return ptr[(u16)idx + 0x32];
}
/* 0x8011E474 | 0x30 */
u8 fn_8011E474(u8* ptr, u16 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 2) { return 0; }
    return ptr[(u16)idx + 0x30];
}
/* 0x8011E57C | 44 bytes | compound_getter */
u16 fn_8011E57C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x8);
}
/* 0x8011E5A8 | 44 bytes | compound_getter */
u16 fn_8011E5A8(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x6);
}
/* 0x8011E5D4 | 44 bytes | compound_getter */
u16 fn_8011E5D4(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x4);
}
/* 0x8011E600 | 44 bytes | compound_getter */
u16 fn_8011E600(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x2);
}
/* 0x8011E62C | 44 bytes | compound_getter */
u16 fn_8011E62C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x0);
}
/* 0x8011E658 | 44 bytes | compound_getter */
u16 fn_8011E658(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0xA);
}
/* 0x8011E684 | 44 bytes | compound_getter */
u16 fn_8011E684(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x8);
}
/* 0x8011E6B0 | 44 bytes | compound_getter */
u16 fn_8011E6B0(void* ptr) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x6);
}
/* 0x8011E6DC | 44 bytes | compound_getter */
u16 fn_8011E6DC(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x4);
}
/* 0x8011E708 | 44 bytes | compound_getter */
u16 fn_8011E708(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x2);
}
/* 0x8011E734 | 44 bytes | compound_getter */
u16 fn_8011E734(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x0);
}
/* 0x8011E778 | 0x2C */
extern u32 lbl_80478F90;
extern u32 lbl_80478F94;
void* fn_8011E778(u16 idx) {
    u32* hdr = (u32*)lbl_80478F90;
    if ((u16)idx >= hdr[0]) { return NULL; }
    return (u8*)lbl_80478F94 + (u32)idx * 0x11C;
}
/* 0x8011E7A4 | 0x1C */
u8 fn_8011E7A4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (u8)(*(u32*)(&ptr[0xF8]));
}
/* 0x74 | fn_8011E868 | compound_chained_getter */
u8 fn_8011E868(u8* ptr) {
    u16 idx;
    void* entry;
    u8 val;
    if (ptr == NULL) { return 0; }
    if (ptr == NULL) {
        idx = 0;
    } else {
        idx = *(u16*)(ptr + 0x0);
    }
    if ((u16)idx >= *(u32*)lbl_80478F90) {
        entry = NULL;
    } else {
        entry = (u8*)lbl_80478F94 + (u32)(u16)idx * 0x11C;
    }
    if (entry == NULL) {
        val = 0;
    } else {
        val = *(u8*)((u8*)entry + 0x33);
    }
    if (val == 0) { return 0; }
    return *(u8*)(ptr + 0xCC);
}
/* 0x8011E90C | 44 bytes | compound_getter */
u8 fn_8011E90C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0xC);
}
/* 0x8011E938 | 44 bytes | compound_getter */
u8 fn_8011E938(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0xB);
}
/* 0x8011E964 | 44 bytes | compound_getter */
u8 fn_8011E964(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0xA);
}
/* 0x8011E990 | 44 bytes | compound_getter */
u8 fn_8011E990(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x9);
}
/* 0x8011E9BC | 44 bytes | compound_getter */
u8 fn_8011E9BC(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x8);
}
/* 0x8011E9E8 | 44 bytes | compound_getter */
u8 fn_8011E9E8(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x7);
}
/* 0x8011EA14 | 44 bytes | compound_getter */
u8 fn_8011EA14(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x6);
}
/* 0x8011EA40 | 44 bytes | compound_getter */
u8 fn_8011EA40(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x5);
}
/* 0x8011EA6C | 44 bytes | compound_getter */
u8 fn_8011EA6C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x4);
}
/* 0x8011EA98 | 44 bytes | compound_getter */
u8 fn_8011EA98(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x3);
}
/* 0x8011EAC4 | 44 bytes | compound_getter */
u8 fn_8011EAC4(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x2);
}
/* 0x8011EAF0 | 44 bytes | compound_getter */
u8 fn_8011EAF0(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x1);
}
/* 0x8011EB1C | 44 bytes | compound_getter */
u8 fn_8011EB1C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x0);
}
/* 0x8011EB60 | 44 bytes | compound_getter */
u8 fn_8011EB60(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x4);
}
/* 0x8011EB8C | 44 bytes | compound_getter */
u8 fn_8011EB8C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x3);
}
/* 0x8011EBB8 | 44 bytes | compound_getter */
u8 fn_8011EBB8(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x2);
}
/* 0x8011EBE4 | 44 bytes | compound_getter */
u8 fn_8011EBE4(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x1);
}
/* 0x8011EC10 | 44 bytes | compound_getter */
u8 fn_8011EC10(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x0);
}
/* 0x8011EC3C | 44 bytes | compound_getter */
u8 fn_8011EC3C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x4);
}
/* 0x8011EC68 | 44 bytes | compound_getter */
u8 fn_8011EC68(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x3);
}
/* 0x8011EC94 | 44 bytes | compound_getter */
u8 fn_8011EC94(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x2);
}
/* 0x8011ECC0 | 44 bytes | compound_getter */
u8 fn_8011ECC0(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x1);
}
/* 0x8011ECEC | 44 bytes | compound_getter */
u8 fn_8011ECEC(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x0);
}
/* 0x50 | fn_8011ED18 | guarded_call */
u32 fn_8011ED18(void) {
    if (1 /* guard r3 != 0 */) { return 0; }
    fn_801EEEB8();
    return 0;
}
/* 0x5C | fn_8011ED68 | compound_chained_getter */
u32 fn_8011ED68(u8* ptr) {
    u16 val;
    s32 val2;
    if (ptr == NULL) { return 0; }
    if (ptr == NULL) {
        val = 0;
    } else {
        val = *(u16*)(ptr + 0xD8);
    }
    if ((u16)val == 0) { return 0; }
    if (ptr == NULL) {
        val2 = 0;
    } else {
        val2 = *(s32*)(ptr + 0xDC);
    }
    if (val2 < 0) { return 0; }
    return 1;
}
/* 0x8011EDC4 | 0x34 */
void* fn_8011EDC4(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 1) { return NULL; }
    return ptr + (u32)idx * 16 + 0xE8;
}
/* 0x8011EE70 | 44 bytes | compound_getter */
u16 fn_8011EE70(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xA4;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0xA);
}
/* 0x8011EE9C | 44 bytes | compound_getter */
u16 fn_8011EE9C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xA4;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x8);
}
/* 0x8011EEC8 | 44 bytes | compound_getter */
u16 fn_8011EEC8(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xA4;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x6);
}
/* 0x8011EEF4 | 44 bytes | compound_getter */
u16 fn_8011EEF4(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xA4;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x4);
}
/* 0x8011EF20 | 44 bytes | compound_getter */
u16 fn_8011EF20(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xA4;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x2);
}
/* 0x8011EF4C | 44 bytes | compound_getter */
u16 fn_8011EF4C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xA4;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x0);
}
/* 0x8011EF78 | 44 bytes | compound_getter */
u16 fn_8011EF78(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x98;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0xA);
}
/* 0x8011EFA4 | 44 bytes | compound_getter */
u16 fn_8011EFA4(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x98;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x8);
}
/* 0x8011EFD0 | 44 bytes | compound_getter */
u16 fn_8011EFD0(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x98;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x6);
}
/* 0x8011EFFC | 44 bytes | compound_getter */
u16 fn_8011EFFC(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x98;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x4);
}
/* 0x8011F028 | 44 bytes | compound_getter */
u16 fn_8011F028(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x98;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x2);
}
/* 0x8011F054 | 44 bytes | compound_getter */
u16 fn_8011F054(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x98;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x0);
}
/* 0x8011F080 | 44 bytes | compound_getter */
u16 fn_8011F080(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0xA);
}
/* 0x8011F0AC | 44 bytes | compound_getter */
u16 fn_8011F0AC(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x8);
}
/* 0x8011F0D8 | 44 bytes | compound_getter */
u16 fn_8011F0D8(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x6);
}
/* 0x8011F104 | 44 bytes | compound_getter */
u16 fn_8011F104(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x4);
}
/* 0x8011F130 | 44 bytes | compound_getter */
u16 fn_8011F130(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x2);
}
/* 0x8011F15C | 44 bytes | compound_getter */
u16 fn_8011F15C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x0);
}
/* 0x8011F1B8 | 0x38 */
u8 fn_8011F1B8(void* ctx, u32 p1) {
    extern void* fn_8011F260();
    void* result = fn_8011F260(ctx, p1, 1);
    if (result == 0) return 0;
    return *((u8*)result + 0x3);
}
/* 0x8011F1F0 | 0x38 */
u8 fn_8011F1F0(void* ctx, u32 p1) {
    extern void* fn_8011F260();
    void* result = fn_8011F260(ctx, p1, 1);
    if (result == 0) return 0;
    return *((u8*)result + 0x2);
}
/* 0x8011F228 | 0x38 */
u16 fn_8011F228(void* ctx, u32 p1) {
    extern void* fn_8011F260();
    void* result = fn_8011F260(ctx, p1, 1);
    if (result == 0) return 0;
    return *(u16*)result;
}
/* 0x8011F260 | 0x1FC */
void fn_8011F260(void) {
    extern u8 lbl_80478B58[];
    extern u8 lbl_80478B5C[];
    extern void fn_8011F77C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r31 = r4;
    r29 = 0x0;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
    } else {
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
    } else {
        r0 = *(u16*)((u8*)r30 + 0xD8);
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
    } else {
    if ((u32)r30 == (u32)0x0) {
        r0 = 0x0;
    } else {
        r0 = *(u32*)((u8*)r30 + 0xDC);
    }
    if ((s32)r0 < (s32)0x0) {
        r0 = 0x0;
    } else {
    r0 = 0x1;
    }
    }
    }
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        if ((u32)r30 == (u32)0x0) {
            r0 = 0x0;
        } else {
            r0 = *(u16*)((u8*)r30 + 0xD6);
        }
        r0 = r0 & 0xFFFF;
        if ((u32)r30 == (u32)0x0) {
            r0 = r5 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r29 = 0x1;
    }
    }
    }
    if ((u32)r30 == (u32)0x0) {
        r0 = 0x0;
    } else {
    if ((u32)r30 == (u32)0x0) {
        r0 = 0x0;
    } else {
        r0 = *(u16*)((u8*)r30 + 0xD8);
    }
    r0 = r0 & 0xFFFF;
    if ((u32)r30 == (u32)0x0) {
        r0 = 0x0;
    } else {
    if ((u32)r30 == (u32)0x0) {
        r0 = 0x0;
    } else {
        r0 = *(u32*)((u8*)r30 + 0xDC);
    }
    if ((s32)r0 < (s32)0x0) {
        r0 = 0x0;
    } else {
    r0 = 0x1;
    }
    }
    }
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = r31 & 0xFFFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = r5 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r29 = 0x1;
    }
    }
    }
    r3 = r30;
    fn_8011F77C();
    r0 = r29 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = r31 & 0xFFFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = (u32)lbl_80478B58;
            return;
        }
        if ((u32)r0 == (u32)0x1) {
        r0 = r3 & 0xFF;
        }
    }
    if ((u32)r0 < (u32)0x2) {
    r3 = (u32)lbl_80478B5C;
    return;
    }
    if ((u32)r0 == (u32)0x2) {
        r0 = r3 & 0xFF;
        if ((u32)r0 < (u32)0x4) {
        r3 = (u32)lbl_80478B5C;
        return;
        }
    } else
    if ((u32)r0 == (u32)0x3) {
        r0 = r3 & 0xFF;
        if ((u32)r0 < (u32)0x5) {
            r3 = (u32)lbl_80478B5C;
            return;
    }
    }
    r0 = r31 & 0xFFFF;
    if ((u32)r0 >= (u32)0x4) {
        r0 = r31 & 0xFFFF;
        if ((u32)r0 >= (u32)0x4) {
            r3 = 0x0;
            return;
    }
    }
    /* clrlslwi r3, r31, 16, 2 */;
    r3 = r3 + 0x78;
    r3 = r30 + r3;
    return;
}
/* 0x8011F474 | 0x34 */
void* fn_8011F474(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 1) { return NULL; }
    return ptr + (u32)idx * 16 + 0x64;
}
/* 0x8011F4D8 | 24 bytes | nc_addi_ptr */
void* fn_8011F4D8(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x44;
}
/* 0x8011F4F0 | 24 bytes | nc_addi_ptr */
void* fn_8011F4F0(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x2E;
}
/* 0x8011F508 | 24 bytes | nc_addi_ptr */
void* fn_8011F508(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x18;
}
/* 0x8011F598 | 24 bytes | nc_addi_ptr */
void* fn_8011F598(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x8;
}
/* 0x8011F5E0 | 0x1C */
void fn_8011F5E0(u32* dst, u32* src) {
    if (dst == NULL) { return; }
    if (src == NULL) { return; }
    *dst = *src;
}
/* 0x8011F5FC | 0x38 */
void fn_8011F5FC(u32* dst, u32* src) {
    s32 i;
    if (dst == NULL) { return; }
    if (src == NULL) { return; }
    for (i = 0; i < 0x27; i++) {
        dst[i * 2] = src[i * 2];
        dst[i * 2 + 1] = src[i * 2 + 1];
    }
}
/* 0x8011F634 | 0xA4 */
void fn_8011F634(void) {
    extern void fn_8011CBF4();
    extern void fn_8011CE18();
    extern void fn_8011F77C();
    extern void fn_8012640C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r4 = 0x0;
    r5 = 0xc2;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r4 = 0x0;
    r5 = 0xbf;
    r6 = 0x0;
    fn_8012640C();
    r31 = r3 & 0xFF;
    r3 = r30;
    fn_8011F77C();
    r0 = r3 & 0xFF;
    r30 = r3;
    if ((u32)r0 <= (u32)0x7) {
        r3 = r31;
        fn_8011CE18();
        r4 = r30;
        fn_8011CBF4();
        return;
    }
    r3 = 0x0;
    return;
}
/* 0x8011F6D8 | 0xA4 */
void fn_8011F6D8(void) {
    extern void fn_8011CC24();
    extern void fn_8011CE18();
    extern void fn_8011F77C();
    extern void fn_8012640C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r4 = 0x0;
    r5 = 0xc2;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r4 = 0x0;
    r5 = 0xbf;
    r6 = 0x0;
    fn_8012640C();
    r31 = r3 & 0xFF;
    r3 = r30;
    fn_8011F77C();
    r0 = r3 & 0xFF;
    r30 = r3;
    if ((u32)r0 <= (u32)0x7) {
        r3 = r31;
        fn_8011CE18();
        r4 = r30;
        fn_8011CC24();
        return;
    }
    r3 = 0x0;
    return;
}
/* 0x8011F77C | 0x194 */
void fn_8011F77C(void) {
    extern u8 lbl_8047CFF0[];
    extern u8 lbl_8047CFF4[];
    extern u8 lbl_8047CFF8[];
    extern u8 lbl_8047CFFC[];
    extern u8 lbl_8047D000[];
    extern u8 lbl_8047D004[];
    extern u8 lbl_8047D008[];
    extern u8 lbl_8047D010[];
    extern void fn_8012640C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    if ((s32)r0 == (s32)0) {
        r3 = 0x7;
    } else {
        r4 = 0x0;
        r5 = 0xc2;
        r6 = 0x0;
        fn_8012640C();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = r30;
            r4 = 0x0;
            r5 = 0xc4;
            r6 = 0x0;
            fn_8012640C();
            r31 = r3 & 0xFFFF;
            if ((u32)r0 == (u32)0x1) {
                r31 = 0x1;
            }
            if ((u32)r30 == (u32)0x0) {
                f2 = *(f32*)lbl_8047CFF0;
            } else {
                r3 = r30;
                r4 = 0x0;
                r5 = 0xc5;
                r6 = 0x0;
                fn_8012640C();
                /* xoris r3, r3, 0x8000 */;
                r0 = (0x4330 << 16);
                f2 = *(f64*)lbl_8047D008;
                *(u32*)(sp + 0x8) = r0;
                f0 = *(f32*)lbl_8047CFF4;
                f1 = *(f64*)(sp + 0x8);
                f1 = f1 - f2;
                f2 = f1 / f0;
            }
            f0 = *(f32*)lbl_8047CFF0;
            if (f2 < f0) {
                f1 = f0;
            } else {
                r3 = r31 & 0xFFFF;
                r0 = (0x4330 << 16);
                f0 = *(f32*)lbl_8047CFF4;
                *(u32*)(sp + 0x8) = r0;
                f1 = *(f64*)lbl_8047D010;
                f2 = f0 * f2;
                f0 = *(f64*)(sp + 0x8);
                f0 = f0 - f1;
                f1 = f2 / f0;
            }
            f0 = *(f32*)lbl_8047CFF4;
            /* cror eq, gt, eq */;
            if (f1 == f0) {
                r3 = 0x0;
            } else if (f0 = *(f32*)lbl_8047CFF8, /* cror eq, gt, eq */ f1 == f0) {
                r3 = 0x1;
            } else if (f0 = *(f32*)lbl_8047CFFC, /* cror eq, gt, eq */ f1 == f0) {
                r3 = 0x2;
            } else if (f0 = *(f32*)lbl_8047D000, /* cror eq, gt, eq */ f1 == f0) {
                r3 = 0x3;
            } else if (f0 = *(f32*)lbl_8047D004, /* cror eq, gt, eq */ f1 == f0) {
                r3 = 0x4;
            } else if (f0 = *(f32*)lbl_8047CFF0, f1 > f0) {
                r3 = 0x5;
            } else {
                r3 = 0x6;
            }
        } else {
            r3 = 0x7;
        }
    }
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    return;
}
/* 0x8011F910 | 0x2BC */
void fn_8011F910(void) {
    extern u8 lbl_8047CFF0[];
    extern u8 lbl_8047CFF4[];
    extern u8 lbl_8047D008[];
    extern u8 lbl_8047D010[];
    extern u8 lbl_8047D018[];
    extern void fn_8011CA9C();
    extern void fn_8011CAB8();
    extern void fn_8011CB98();
    extern void fn_8011CBB0();
    extern void fn_8011CBC8();
    extern void fn_8011CCFC();
    extern void fn_8011CD18();
    extern void fn_8011CD34();
    extern void fn_8011CD50();
    extern void fn_8011CD6C();
    extern void fn_8011CE18();
    extern void fn_801254B4();
    extern void fn_8012640C();
    extern void fn_80143E14();
    extern void fn_80144014();
    extern void fn_801440A0();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f31 = 0.0f;
    *(f64*)(sp + 0x30) = f31;
    /* psq_st f31, 0x38((u32)sp), 0, qr0 */;
    r27 = r4;
    r30 = r5;
    r29 = r3;
    r4 = 0x0;
    r5 = 0xc2;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFF;
    { u32 _doWork = 0;
    if ((s32)r0 != (s32)0) {
        r3 = r29;
        r4 = 0x0;
        r5 = 0xbf;
        r6 = 0x0;
        fn_8012640C();
        r3 = r3 & 0xFF;
        fn_8011CE18();
        if ((s32)r0 != (s32)0) {
            r3 = r30;
            fn_8011CAB8();
            fn_8011CA9C();
            r0 = (s8)r3;
            r3 = (0x4330 << 16);
            /* xoris r4, r0, 0x8000 */;
            r0 = r30 & 0xFFFF;
            f1 = *(f64*)lbl_8047D008;
            f0 = *(f64*)(sp + 0x8);
            f31 = f0 - f1;
            if ((u32)r0 != (u32)0x4) {
                _doWork = 1;
            } else {
                r3 = r27;
                fn_801440A0();
                if ((u32)r0 != (u32)0x4) {
                    fn_80144014();
                    r0 = r3 & 0xFF;
                }
                if ((u32)r0 == (u32)0x6) {
                    r3 = r27;
                    fn_80143E14();
                    r0 = (0x4330 << 16);
                    f1 = *(f64*)lbl_8047D010;
                    *(u32*)(sp + 0x8) = r0;
                    f0 = *(f64*)(sp + 0x8);
                    f0 = f0 - f1;
                    f31 = f31 * f0;
                    _doWork = 1;
                }
            }
        } else {
            if ((u32)r0 == (u32)0x6) {
                r3 = r27;
                fn_80143E14();
                r0 = (0x4330 << 16);
                f1 = *(f64*)lbl_8047D010;
                *(u32*)(sp + 0x8) = r0;
                f0 = *(f64*)(sp + 0x8);
                f0 = f0 - f1;
                f31 = f31 * f0;
                _doWork = 1;
            }
        }
    } else {
        if ((u32)r0 == (u32)0x6) {
            r3 = r27;
            fn_80143E14();
            r0 = (0x4330 << 16);
            f1 = *(f64*)lbl_8047D010;
            *(u32*)(sp + 0x8) = r0;
            f0 = *(f64*)(sp + 0x8);
            f0 = f0 - f1;
            f31 = f31 * f0;
            _doWork = 1;
        }
    }
    if (_doWork) {
    r0 = r30 & 0xFFFF;
    if ((u32)r0 == (u32)0x5) {
        if ((u32)r29 == (u32)0x0) {
            f1 = *(f32*)lbl_8047CFF0;
        } else {
            r3 = r29;
            r4 = 0x0;
            r5 = 0xc5;
            r6 = 0x0;
            fn_8012640C();
            /* xoris r3, r3, 0x8000 */;
            r0 = (0x4330 << 16);
            f2 = *(f64*)lbl_8047D008;
            *(u32*)(sp + 0x8) = r0;
            f0 = *(f32*)lbl_8047CFF4;
            f1 = *(f64*)(sp + 0x8);
            f1 = f1 - f2;
            f1 = f1 / f0;
        }
        f0 = *(f32*)lbl_8047D018;
        f31 = f0 * f1;
    }
    r0 = r30 & 0xFFFF;
    if ((u32)r29 == (u32)0x0) {
        r3 = r28;
        fn_8011CD6C();
        r31 = r3;
    } else if ((u32)r0 == (u32)0x1) {
        r3 = r28;
        fn_8011CD50();
        r31 = r3;
    } else if ((u32)r0 == (u32)0x2) {
        r3 = r28;
        fn_8011CD34();
        r31 = r3;
    } else if ((u32)r0 == (u32)0x3) {
        r3 = r28;
        fn_8011CD18();
        r31 = r3;
    } else if ((u32)r0 == (u32)0x4) {
        r3 = r28;
        fn_8011CCFC();
        r31 = r3;
    }
    r3 = r31 & 0xFF;
    fn_8011CBC8();
    if ((u32)r0 != (u32)0x4) {
        fn_8011CBB0();
        r30 = r3;
        r3 = r28;
        fn_8011CB98();
        r4 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x4) {
            r0 = (0x4330 << 16);
            r3 = r30 & 0xFF;
            f2 = *(f64*)lbl_8047D010;
            *(u32*)(sp + 0x8) = r0;
            f0 = *(f64*)(sp + 0x8);
            f1 = f0 - f2;
            *(u32*)(sp + 0x10) = r0;
            f0 = *(f64*)(sp + 0x10);
            f31 = f31 * f1;
            f0 = f0 - f2;
            f31 = f31 / f0;
            if ((u32)r29 != (u32)0x0) {
                if ((u32)r29 == (u32)0x0) {
                    f0 = *(f32*)lbl_8047CFF0;
                } else {
                    r3 = r29;
                    r4 = 0x0;
                    r5 = 0xc5;
                    r6 = 0x0;
                    fn_8012640C();
                    /* xoris r3, r3, 0x8000 */;
                    r0 = (0x4330 << 16);
                    f2 = *(f64*)lbl_8047D008;
                    *(u32*)(sp + 0x10) = r0;
                    f0 = *(f32*)lbl_8047CFF4;
                    f1 = *(f64*)(sp + 0x10);
                    f1 = f1 - f2;
                    f0 = f1 / f0;
                }
                f1 = f0 + f31;
                f0 = *(f32*)lbl_8047CFF0;
                if (f1 < f0) {
                    f1 = f0;
                }
                if ((u32)r29 != (u32)0x0) {
                    f0 = *(f32*)lbl_8047CFF4;
                    r3 = r29;
                    r4 = 0x0;
                    r5 = 0xc5;
                    f0 = f0 * f1;
                    r6 = 0x0;
                    f0 = (f64)(s32)f0;
                    *(f64*)(sp + 0x10) = f0;
                    r7 = *(u32*)(sp + 0x14);
                    fn_801254B4();
                }
            }
        }
    }
    } /* end if (_doWork) */
    } /* end _doWork scope */
    /* psq_l f31, 0x38((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x30);
    return;
}
/* 0x48 | fn_8011FBCC | generic */
void fn_8011FBCC(void) {
    /* refs: lbl_8047CFF4 */
    fn_801254B4();
}
/* 0x60 | fn_8011FC14 | generic */
void fn_8011FC14(void) {
    /* refs: lbl_8047CFF0, lbl_8047CFF4, lbl_8047D008 */
    fn_8012640C();
}
/* 0x8011FC74 | 0x30 */
u8 fn_8011FC74(u32 arg) {
    /* depends on fn_8012640C signature */
    return 0;
}
/* 0x8011FCA4 | 0x124 */
void fn_8011FCA4(void) {
    extern u8 lbl_8047CFF4[];
    extern u8 lbl_8047D010[];
    extern void fn_8011B950();
    extern void fn_801254B4();
    extern void fn_8012640C();
    extern void fn_801EEEB8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    r31 = r4;
    if ((s32)r0 != (s32)0) {
        r4 = 0x0;
        r5 = 0xc3;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        if ((u32)r30 != (u32)0x0) {
            r3 = r30;
            r4 = 0x0;
            r5 = 0xc5;
            r6 = 0x0;
            r7 = -0x64;
            fn_801254B4();
        }
        r3 = r30;
        r4 = 0x0;
        r5 = 0xc6;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xc7;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r3 = r30;
        r4 = 0x0;
        r5 = 0xc8;
        r6 = 0x0;
        fn_8012640C();
        r4 = 0x1;
        fn_8011B950();
        r3 = r30;
        r7 = r31 & 0xFFFF;
        r4 = 0x0;
        r5 = 0xc3;
        r6 = 0x0;
        fn_801254B4();
        r3 = r31;
        fn_801EEEB8();
        r3 = r3 & 0xFFFF;
        r0 = (0x4330 << 16);
        f1 = *(f64*)lbl_8047D010;
        *(u32*)(sp + 0x8) = r0;
        f0 = *(f64*)(sp + 0x8);
        f1 = f0 - f1;
        if ((u32)r30 != (u32)0x0) {
            f0 = *(f32*)lbl_8047CFF4;
            r3 = r30;
            r4 = 0x0;
            r5 = 0xc5;
            f0 = f0 * f1;
            r6 = 0x0;
            f0 = (f64)(s32)f0;
            *(f64*)(sp + 0x8) = f0;
            r7 = *(u32*)(sp + 0xC);
            fn_801254B4();
    }
    }
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    return;
}
/* 0x8011FDC8 | 0x504 */
void fn_8011FDC8(void) {
    extern u8 lbl_8047CFF0[];
    extern u8 lbl_8047CFF4[];
    extern u8 lbl_8047D008[];
    extern u8 lbl_8047D010[];
    extern void fn_8001D994();
    extern void fn_800F9E70();
    extern void fn_8010BBB8();
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    extern void fn_8011CE44();
    extern void fn_8011CE74();
    extern void fn_8012640C();
    extern void fn_801EEEB8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    r5 = 0x6e;
    r6 = 0x0;
    r31 = r3;
    r30 = r4;
    r4 = 0x0;
    fn_8012640C();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x77;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3;
    r3 = r30;
    fn_800F9E70();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x7a;
    r6 = 0x0;
    fn_8012640C();
    *(u8*)((u8*)r30 + 0x17) = r3;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x87;
    r6 = 0x0;
    fn_8012640C();
    *(u16*)((u8*)r30 + 0x18) = r3;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x83;
    r6 = 0x0;
    fn_8012640C();
    *(u16*)((u8*)r30 + 0x1A) = r3;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x7a;
    r6 = 0x0;
    fn_8012640C();
    r29 = r3 & 0xFF;
    if ((u32)r31 == (u32)0x0) {
        r28 = 0x0;
    } else {
        r3 = r31;
        r4 = 0x0;
        r5 = 0x6e;
        r6 = 0x0;
        fn_8012640C();
        r4 = r3 & 0xFFFF;
        r3 = 0x0;
        r5 = 0x11;
        r6 = 0x0;
        fn_8012640C();
        r3 = r3 & 0xFF;
        fn_8011CE74();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
        } else {
            r4 = r29;
            fn_8011CE44();
        }
        r28 = r3;
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0xc2;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFF;
    if ((u32)r3 == (u32)0x0) {
        r0 = r29 + 0x1;
        r29 = r0 & 0xFF;
        if ((u32)r31 == (u32)0x0) {
            r3 = 0x0;
            goto L_8011FF54;
        }
        r3 = r31;
        r4 = 0x0;
        r5 = 0x6e;
        r6 = 0x0;
        fn_8012640C();
        r4 = r3 & 0xFFFF;
        r3 = 0x0;
        r5 = 0x11;
        r6 = 0x0;
        fn_8012640C();
        r3 = r3 & 0xFF;
        fn_8011CE74();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            goto L_8011FF54;
        }
        r4 = r29;
        fn_8011CE44();
    L_8011FF54: ;
        r0 = r3 - r28;
        r3 = r31;
        *(u32*)((u8*)r30 + 0x1C) = r0;
        r4 = 0x0;
        r5 = 0x79;
        r6 = 0x0;
        fn_8012640C();
        r0 = r3 - r28;
        *(u32*)((u8*)r30 + 0x20) = r0;
        goto L_80120004;
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0xc3;
    r6 = 0x0;
    fn_8012640C();
    r3 = r3 & 0xFFFF;
    fn_801EEEB8();
    r3 = r3 & 0xFFFF;
    r0 = (0x4330 << 16);
    f1 = *(f64*)lbl_8047D010;
    *(u32*)(sp + 0x8) = r0;
    f0 = *(f64*)(sp + 0x8);
    f0 = f0 - f1;
    *(f32*)((u8*)r30 + 0x1C) = f0;
    if ((u32)r31 == (u32)0x0) {
        f0 = *(f32*)lbl_8047CFF0;
    } else {
        r3 = r31;
        r4 = 0x0;
        r5 = 0xc5;
        r6 = 0x0;
        fn_8012640C();
        /* xoris r3, r3, 0x8000 */;
        r0 = (0x4330 << 16);
        f2 = *(f64*)lbl_8047D008;
        *(u32*)(sp + 0x8) = r0;
        f0 = *(f32*)lbl_8047CFF4;
        f1 = *(f64*)(sp + 0x8);
        f1 = f1 - f2;
        f0 = f1 / f0;
    }
    *(f32*)((u8*)r30 + 0x20) = f0;
L_80120004: ;
    if ((u32)r31 == (u32)0x0) {
        r0 = 0x0;
        goto L_801201F8;
    }
    r3 = 0x3;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x3, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x3;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x3a;
        goto L_801201F8;
    }
    r3 = 0x4;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x4, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x4;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x3a;
        goto L_801201F8;
    }
    r3 = 0x5;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x5, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x5;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x3b;
        goto L_801201F8;
    }
    r3 = 0x6;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x6, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x6;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x3c;
        goto L_801201F8;
    }
    r3 = 0x7;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x7, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x7;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x3d;
        goto L_801201F8;
    }
    r3 = 0x8;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x8, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x8;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x3e;
        goto L_801201F8;
    }
    r0 = 0x0;
L_801201F8: ;
    *(u16*)((u8*)r30 + 0x24) = r0;
    r3 = r31;
    fn_8010BBB8();
    *(u16*)((u8*)r30 + 0x26) = r3;
    r3 = r31;
    fn_8001D994();
    *(u8*)((u8*)r30 + 0x28) = r3;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xc2;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        *(u8*)((u8*)r30 + 0x29) = r0;
        goto L_80120294;
    }
    r3 = 0x3e;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x3e, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x3e;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0xc8) {
        r0 = 0x1;
        *(u8*)((u8*)r30 + 0x29) = r0;
        goto L_80120294;
    }
    r0 = 0x2;
    *(u8*)((u8*)r30 + 0x29) = r0;
L_80120294: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x82;
    r6 = 0x0;
    fn_8012640C();
    *(u16*)((u8*)r30 + 0x2A) = r3;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
/* 0x801202CC | 0x1DC */
void fn_801202CC(void) {
    extern u8 lbl_8047CFF0[];
    extern u8 lbl_8047CFF4[];
    extern u8 lbl_8047D008[];
    extern u8 lbl_8047D010[];
    extern void fn_8011CE44();
    extern void fn_8011CE74();
    extern void fn_8012640C();
    extern void fn_801EEEB8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    r5 = 0x7a;
    r6 = 0x0;
    r31 = r4;
    r4 = 0x0;
    r30 = r3;
    fn_8012640C();
    r28 = r3 & 0xFF;
    if ((u32)r30 == (u32)0x0) {
        r29 = 0x0;
    } else {
        r3 = r30;
        r4 = 0x0;
        r5 = 0x6e;
        r6 = 0x0;
        fn_8012640C();
        r4 = r3 & 0xFFFF;
        r3 = 0x0;
        r5 = 0x11;
        r6 = 0x0;
        fn_8012640C();
        r3 = r3 & 0xFF;
        fn_8011CE74();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
        } else {
            r4 = r28;
            fn_8011CE44();
        }
        r29 = r3;
    }
    r3 = r30;
    r4 = 0x0;
    r5 = 0xc2;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFF;
    if ((u32)r3 == (u32)0x0) {
        if ((u32)r30 != (u32)0x0) {
        r3 = r30;
        r4 = 0x0;
        r5 = 0x6e;
        r6 = 0x0;
        fn_8012640C();
        r4 = r3 & 0xFFFF;
        r3 = 0x0;
        r5 = 0x11;
        r6 = 0x0;
        fn_8012640C();
        r3 = r3 & 0xFF;
        fn_8011CE74();
        if ((u32)r3 != (u32)0x0) {
        r0 = r28 + 0x1;
        r4 = r0 & 0xFF;
        fn_8011CE44();
        }
        }
        r0 = r3 - r29;
        r3 = r30;
        *(u32*)((u8*)r31 + 0x1C) = r0;
        r4 = 0x0;
        r5 = 0x79;
        r6 = 0x0;
        fn_8012640C();
        r0 = r3 - r29;
        *(u32*)((u8*)r31 + 0x20) = r0;
    } else {
    r3 = r30;
    r4 = 0x0;
    r5 = 0xc3;
    r6 = 0x0;
    fn_8012640C();
    r3 = r3 & 0xFFFF;
    fn_801EEEB8();
    r3 = r3 & 0xFFFF;
    r0 = (0x4330 << 16);
    f1 = *(f64*)lbl_8047D010;
    *(u32*)(sp + 0x8) = r0;
    f0 = *(f64*)(sp + 0x8);
    f0 = f0 - f1;
    *(f32*)((u8*)r31 + 0x1C) = f0;
    if ((u32)r30 == (u32)0x0) {
        f0 = *(f32*)lbl_8047CFF0;
    } else {
        r3 = r30;
        r4 = 0x0;
        r5 = 0xc5;
        r6 = 0x0;
        fn_8012640C();
        /* xoris r3, r3, 0x8000 */;
        r0 = (0x4330 << 16);
        f2 = *(f64*)lbl_8047D008;
        *(u32*)(sp + 0x8) = r0;
        f0 = *(f32*)lbl_8047CFF4;
        f1 = *(f64*)(sp + 0x8);
        f1 = f1 - f2;
        f0 = f1 / f0;
    }
    *(f32*)((u8*)r31 + 0x20) = f0;
    }
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
/* 0x801204A8 | 0x1CC */
void fn_801204A8(void) {
    extern void fn_800FA280();
    extern void fn_8010C46C();
    extern void fn_8010C4D4();
    extern void fn_8011BB6C();
    extern void fn_8011BEB4();
    extern void fn_8012640C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r5 = 0x48;
    r31 = r4;
    r30 = r3;
    r4 = 0x0;
    r3 = r31;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = r30;
    r4 = 0x0;
    r5 = 0x77;
    r6 = 0x0;
    fn_8012640C();
    *(u32*)((u8*)r31 + 0x0) = r3;
    r26 = 0x0;
    while (r0 = r26 & 0xFF, (u32)r0 < (u32)0x4) {

    r29 = r26 & 0xFF;
    r3 = r29 * 0xc;
    r25 = r3 + 0x4;
    r25 = r31 + r25;
    if ((u32)r30 == (u32)0x0) {
        r0 = 0x0;
    } else {
    r3 = r30;
    r6 = r29;
    r4 = 0x0;
    r5 = 0x7f;
    fn_8012640C();
    if ((s32)r3 == (s32)0x0) {
        r0 = 0x0;
    } else {
    r3 = r30;
    r6 = r29;
    r4 = 0x0;
    r5 = 0x7f;
    fn_8012640C();
    if ((s32)r3 == (s32)0x163) {
        r0 = 0x0;
    } else {
    r0 = 0x1;
    }
    }
    }
    r0 = r0 & 0xFF;
    if ((s32)r3 == (s32)0x163) {
        r0 = 0x0;
        *(u32*)((u8*)r25 + 0x0) = r0;
        *(u32*)((u8*)r25 + 0x4) = r0;
        *(u8*)((u8*)r25 + 0xA) = r0;
        *(u8*)((u8*)r25 + 0xB) = r0;
    } else {
        r28 = r26 & 0xFF;
        r3 = r30;
        r6 = r28;
        r4 = 0x0;
        r5 = 0x7f;
        fn_8012640C();
        r27 = r3 & 0xFFFF;
        r3 = 0x0;
        r4 = r27;
        r5 = 0x3;
        r6 = 0x0;
        fn_8011BEB4();
        r29 = r3 & 0xFFFF;
        r4 = r27;
        r3 = 0x0;
        r5 = 0x1;
        r6 = 0x0;
        fn_8011BEB4();
        fn_800FA280();
        *(u32*)((u8*)r25 + 0x0) = r3;
        r3 = r29;
        fn_8010C4D4();
        fn_800FA280();
        *(u32*)((u8*)r25 + 0x4) = r3;
        r3 = r29;
        fn_8010C46C();
        *(u16*)((u8*)r25 + 0x8) = r3;
        if ((u32)r30 == (u32)0x0) {
            r3 = 0x0;
        } else {
            r27 = r28 + 0x4;
            r3 = r30;
            r6 = r27 & 0xFFFF;
            r4 = 0x0;
            r5 = 0x7f;
            fn_8012640C();
            r29 = r3 & 0xFFFF;
            r3 = r30;
            r6 = r27 & 0xFFFF;
            r4 = 0x0;
            r5 = 0x81;
            fn_8012640C();
            r4 = r3 & 0xFF;
            r3 = r29;
            fn_8011BB6C();
        }
        *(u8*)((u8*)r25 + 0xA) = r3;
        r3 = r30;
        r6 = r28;
        r4 = 0x0;
        r5 = 0x80;
        fn_8012640C();
        *(u8*)((u8*)r25 + 0xB) = r3;
    }
    r26 = r26 + 0x1;
    }

    return;
}
/* 0x80120674 | 0x1F8 */
void fn_80120674(void) {
    extern u8 lbl_8027296C[];
    extern void fn_800E0C54();
    extern void fn_801254B4();
    extern void fn_8012640C();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r4 = (u32)lbl_8027296C;
    r30 = (u32)lbl_8027296C;
    r12 = *(u32*)((u8*)r30 + 0x0);
    r11 = *(u32*)((u8*)r30 + 0x4);
    r10 = *(u32*)((u8*)r30 + 0x8);
    r9 = *(u32*)((u8*)r30 + 0xC);
    r8 = *(u32*)((u8*)r30 + 0x10);
    r7 = *(u32*)((u8*)r30 + 0x14);
    r6 = *(u32*)((u8*)r30 + 0x18);
    r5 = *(u32*)((u8*)r30 + 0x1C);
    r4 = *(u32*)((u8*)r30 + 0x20);
    r0 = *(u32*)((u8*)r30 + 0x24);
    *(u32*)(sp + 0x2C) = r0;
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
    } else {
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r30 = r3 & 0xFFFF;
    r3 = 0x0;
    r4 = r30;
    r5 = 0x17;
    r6 = 0x1;
    fn_8012640C();
    if ((s32)r3 == (s32)0x0) {
        r4 = r30;
        r3 = 0x0;
        r5 = 0x17;
        r6 = 0x0;
        fn_8012640C();
        r0 = r3 & 0xFFFF;
    } else {
    r3 = r31;
    r4 = 0x0;
    r5 = 0xb7;
    r6 = 0x0;
    fn_8012640C();
    r6 = r3 & 0xFF;
    r4 = r30;
    r3 = 0x0;
    r5 = 0x17;
    fn_8012640C();
    r0 = r3 & 0xFFFF;
    }
    }
    r0 = r0 & 0xFFFF;
    if ((u32)r0 == (u32)0x35) {
        r3 = r31;
        r4 = 0x0;
        r5 = 0x82;
        r6 = 0x0;
        fn_8012640C();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 == (u32)0x35) {
            fn_800E0C54();
            r4 = (0x6666 << 16);
            r5 = r3 & 0xFFFF;
            r0 = r4 + 0x6667;
            r0 = (s32)((s64)r0 * (s64)r5 >> 32);
            r0 = (s32)r0 >> 2;
            r3 = (u32)r0 >> 31;
            r0 = r0 + r3;
            r0 = r0 * 0xa;
            /* subf. r0, r0, r5 */;
            if ((u32)r0 == (u32)0x35) {
                fn_800E0C54();
                r4 = (0x51ec << 16);
                r5 = r3 & 0xFFFF;
                r3 = (u32)sp + 0x8;
                r0 = (s32)((s64)r0 * (s64)r5 >> 32);
                r6 = 0x0;
                r7 = 0x0;
                r0 = (s32)r0 >> 5;
                r4 = (u32)r0 >> 31;
                r0 = r0 + r4;
                r0 = r0 * 0x64;
                r5 = r5 - r0;
                while (r0 = r7 & 0xFFFF, (s32)r0 < (s32)0x14) {

                r4 = r0 << 1;
                r0 = r4 + 0x2;
                r0 = *(u16*)(r3 + r0);
                if ((u32)r0 > (u32)r5) {
                    /* clrlslwi r0, r7, 16, 1 */;
                    r6 = *(u16*)(r3 + r0);
                    break;
                }
                r7 = r7 + 0x2;
                }

                r7 = r6 & 0xFFFF;
                if ((s32)r0 != (s32)0x14) {
                    if ((u32)r31 != (u32)0x0) {
                        r3 = r31;
                        r4 = 0x0;
                        r5 = 0x82;
                        r6 = 0x0;
                        fn_801254B4();
                    }
                    r3 = 0x1;
        } else {
    r3 = 0x0;
        }
        } else {
    r3 = 0x0;
        }
        } else {
    r3 = 0x0;
        }
    } else {
    r3 = 0x0;
    }
    r31 = *(u32*)(sp + 0x3C);
    r30 = *(u32*)(sp + 0x38);
    return;
}
/* 0x8012086C | 0x294 */
void fn_8012086C(void) {
    extern void fn_80119ED0();
    extern void fn_8011B788();
    extern void fn_8011BB6C();
    extern void fn_801254B4();
    extern void fn_8012640C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    if ((s32)r0 == (s32)0) return;
    r4 = 0x0;
    r5 = 0x87;
    r6 = 0x0;
    fn_8012640C();
    r7 = r3 & 0xFFFF;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x83;
    r6 = 0x0;
    fn_801254B4();
    r28 = 0x0;
    while ((r28 & 0xFF) < (u32)0x4) {
        if ((u32)r31 == (u32)0x0) {
            r0 = 0x0;
        } else {
            r30 = r28 & 0xFF;
            r3 = r31;
            r6 = r30;
            r4 = 0x0;
            r5 = 0x7f;
            fn_8012640C();
            if ((s32)r3 == (s32)0x0) {
                r0 = 0x0;
            } else {
                r3 = r31;
                r6 = r30;
                r4 = 0x0;
                r5 = 0x7f;
                fn_8012640C();
                if ((s32)r3 == (s32)0x163) {
                    r0 = 0x0;
                } else {
                    r0 = 0x1;
                }
            }
        }
        r0 = r0 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            if ((u32)r31 == (u32)0x0) {
                r0 = 0x0;
            } else {
                r4 = r28 & 0xFF;
                r3 = r31;
                r29 = r4 + 0x4;
                r4 = 0x0;
                r6 = r29 & 0xFFFF;
                r5 = 0x7f;
                fn_8012640C();
                r30 = r3 & 0xFFFF;
                r3 = r31;
                r6 = r29 & 0xFFFF;
                r4 = 0x0;
                r5 = 0x81;
                fn_8012640C();
                r4 = r3 & 0xFF;
                r3 = r30;
                fn_8011BB6C();
                r0 = r3;
            }
            r3 = r31;
            r6 = r28 & 0xFF;
            r7 = r0 & 0xFF;
            r4 = 0x0;
            r5 = 0x80;
            fn_801254B4();
        }
        r28 = r28 + 0x1;
    }
    r3 = 0x3;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x3, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x3;
        fn_8011B788();
    }
    r3 = 0x4;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x4, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x4;
        fn_8011B788();
    }
    r3 = 0x5;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x5, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x5;
        fn_8011B788();
    }
    r3 = 0x6;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x6, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x6;
        fn_8011B788();
    }
    r3 = 0x7;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x7, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x7;
        fn_8011B788();
    }
    r3 = 0x8;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x8, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x8;
        fn_8011B788();
    }
    return;
}
/* 0x80120B00 | 0x16C */
void fn_80120B00(void) {
    extern void fn_8012640C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r6 = 0x0;
    r28 = r4;
    r29 = r5;
    r24 = r3;
    r4 = 0x0;
    r5 = 0x93;
    fn_8012640C();
    r27 = r3 & 0xFFFF;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x94;
    r6 = 0x0;
    fn_8012640C();
    r26 = r3 & 0xFFFF;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x95;
    r6 = 0x0;
    fn_8012640C();
    r25 = r3 & 0xFFFF;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x96;
    r6 = 0x0;
    fn_8012640C();
    r31 = r3 & 0xFFFF;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x97;
    r6 = 0x0;
    fn_8012640C();
    r30 = r3 & 0xFFFF;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x98;
    r6 = 0x0;
    fn_8012640C();
    r4 = r27 & 0x00000002;
    r3 = r3 & 0xFFFF;
    r5 = (s32)r4 >> 1;
    /* clrlslwi r0, r26, 31, 1 */;
    r5 = (r5 & ~0x00000002) | (((r26 << 0) | ((u32)r26 >> 32)) & 0x00000002);
    r4 = (0x8208 << 16);
    r5 = (r5 & ~0x00000004) | (((r25 << 1) | ((u32)r25 >> 31)) & 0x00000004);
    r0 = (r0 & ~0x00000001) | (((r27 << 0) | ((u32)r27 >> 32)) & 0x00000001);
    r5 = (r5 & ~0x00000008) | (((r3 << 2) | ((u32)r3 >> 30)) & 0x00000008);
    r6 = r4 + 0x2083;
    r5 = (r5 & ~0x00000010) | (((r31 << 3) | ((u32)r31 >> 29)) & 0x00000010);
    r0 = (r0 & ~0x00000004) | (((r25 << 2) | ((u32)r25 >> 30)) & 0x00000004);
    r0 = (r0 & ~0x00000008) | (((r3 << 3) | ((u32)r3 >> 29)) & 0x00000008);
    r5 = (r5 & ~0x00000020) | (((r30 << 4) | ((u32)r30 >> 28)) & 0x00000020);
    r0 = (r0 & ~0x00000010) | (((r31 << 4) | ((u32)r31 >> 28)) & 0x00000010);
    r3 = r5 & 0xFF;
    r0 = (r0 & ~0x00000020) | (((r30 << 5) | ((u32)r30 >> 27)) & 0x00000020);
    r4 = r3 * 0x28;
    r0 = r0 & 0xFF;
    r0 = r0 * 0xf;
    r5 = (s32)((s64)r6 * (s64)r4 >> 32);
    r3 = (s32)((s64)r6 * (s64)r0 >> 32);
    r4 = r5 + r4;
    r4 = (s32)r4 >> 5;
    r0 = r3 + r0;
    r5 = (u32)r4 >> 31;
    r0 = (s32)r0 >> 5;
    r3 = (u32)r0 >> 31;
    r4 = r4 + r5;
    r3 = r0 + r3;
    r0 = r3 + 0x1;
    r4 = r4 + 0x1e;
    r3 = r0 & 0xFF;
    r4 = r4 & 0xFFFF;
    r0 = r3;
    if ((u32)r3 >= (u32)0x9) {
        r0 = r3 + 0x1;
        r0 = r0 & 0xFFFF;
    }
    if ((u32)r28 != (u32)0x0) {
        *(u16*)((u8*)r28 + 0x0) = r4;
    }
    if ((u32)r29 != (u32)0x0) {
        *(u16*)((u8*)r29 + 0x0) = r0;
    }
    return;
}
/* 0x70 | fn_80120C6C | generic */
u32 fn_80120C6C(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    /* refs: lbl_80478F90 */
    fn_8012640C();
    return 0;
}
/* 0x80120CDC | 0x90 */
void fn_80120CDC(void) {
    extern void fn_8011CE44();
    extern void fn_8011CE74();
    extern void fn_8012640C();
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    r30 = r3;
    fn_8012640C();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x11;
    r6 = 0x0;
    fn_8012640C();
    r31 = r3 & 0xFF;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x7a;
    r6 = 0x0;
    fn_8012640C();
    r30 = r3 & 0xFF;
    r3 = r31;
    fn_8011CE74();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
    } else {
        r4 = r30;
        fn_8011CE44();
    }
    return;
}
/* 0x64 | fn_80120D6C | generic */
u32 fn_80120D6C(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_8012640C();
    fn_80143B08();
    fn_80143ABC();
    return -1;
}
/* 0x80120DD0 | 0x210 */
void fn_80120DD0(void) {
    extern void fn_80119ED0();
    extern void fn_80119F50();
    extern void fn_8011B67C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r31 = 0x0;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x3;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x3, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r30;
        r4 = 0x3;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r31 = 0x3;
    }
    r3 = 0x4;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x4, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r30;
        r4 = 0x4;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r31 = 0x4;
    }
    r3 = 0x8;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x8, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r30;
        r4 = 0x8;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r31 = 0x8;
    }
    r3 = 0x5;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x5, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r30;
        r4 = 0x5;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r31 = 0x5;
    }
    r3 = 0x6;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x6, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r30;
        r4 = 0x6;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r31 = 0x6;
    }
    r3 = 0x7;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x7, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r30;
        r4 = 0x7;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r31 = 0x7;
    }
    r3 = r31;
    fn_80119F50();
    return;
}
/* 0x80120FE0 | 0x218 */
void fn_80120FE0(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x3;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x3, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x3;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x3a;
        return;
    }
    r3 = 0x4;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x4, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x4;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x3a;
        return;
    }
    r3 = 0x5;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x5, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x5;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x3b;
        return;
    }
    r3 = 0x6;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x6, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x6;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x3c;
        return;
    }
    r3 = 0x7;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x7, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x7;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x3d;
        return;
    }
    r3 = 0x8;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x8, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x8;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x3e;
        return;
    }
    r3 = 0x0;
    return;
}
/* 0x801211F8 | 0x218 */
void fn_801211F8(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x3;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x3, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x3;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x3;
        return;
    }
    r3 = 0x4;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x4, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x4;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x4;
        return;
    }
    r3 = 0x5;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x5, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x5;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x5;
        return;
    }
    r3 = 0x6;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x6, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x6;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x6;
        return;
    }
    r3 = 0x7;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x7, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x7;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x7;
        return;
    }
    r3 = 0x8;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x8, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x8;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x8;
        return;
    }
    r3 = 0x0;
    return;
}
/* 0x74 | fn_80121410 | generic */
u32 fn_80121410(void) {
    fn_8012640C();
    fn_8012640C();
    return 0;
}
/* 0x78 | fn_80121484 | generic */
void fn_80121484(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6) {
    fn_80119ED0(0);
    fn_80119ED0(0);
    fn_8011A0A8();
}
/* 0x78 | fn_801214FC | generic */
void fn_801214FC(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6) {
    fn_80119ED0(0);
    fn_80119ED0(0);
    fn_8011A280();
}
/* 0x70 | fn_80121574 | nullcheck_call_flag */
u32 fn_80121574(void* obj) {
    if (fn_80119ED0(0) == 0) { return 0; }
    fn_8011A3E4(obj);
    return 1;
}
/* 0x78 | fn_801215E4 | generic */
void fn_801215E4(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6) {
    fn_80119ED0(0);
    fn_80119ED0(0);
    fn_8011A570();
}
/* 0x70 | fn_8012165C | nullcheck_call_flag */
u32 fn_8012165C(void* obj) {
    if (fn_80119ED0(0) == 0) { return 0; }
    fn_8011A6D4(obj);
    return 1;
}
/* 0x70 | fn_801216CC | nullcheck_call_flag */
u32 fn_801216CC(void* obj) {
    if (fn_80119ED0(0) == 0) { return 0; }
    fn_8011A860(obj);
    return 1;
}
/* 0x78 | fn_8012173C | generic */
void fn_8012173C(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6) {
    fn_80119ED0(0);
    fn_80119ED0(0);
    fn_8011A9EC();
}
/* 0x78 | fn_801217B4 | generic */
void fn_801217B4(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6) {
    fn_80119ED0(0);
    fn_80119ED0(0);
    fn_8011AB50();
}
/* 0x70 | fn_8012182C | nullcheck_call_flag */
u32 fn_8012182C(void* obj) {
    if (fn_80119ED0(0) == 0) { return 0; }
    fn_8011ACB4(obj);
    return 1;
}
/* 0x70 | fn_8012189C | nullcheck_call_flag */
u32 fn_8012189C(void* obj) {
    if (fn_80119ED0(0) == 0) { return 0; }
    fn_8011AE40(obj);
    return 1;
}
/* 0x78 | fn_8012190C | generic */
void fn_8012190C(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6) {
    fn_80119ED0(0);
    fn_80119ED0(0);
    fn_8011AFCC();
}
/* 0x70 | fn_80121984 | nullcheck_call_flag */
u32 fn_80121984(void* obj) {
    if (fn_80119ED0(0) == 0) { return 0; }
    fn_8011B130(obj);
    return 1;
}
/* 0x78 | fn_801219F4 | generic */
void fn_801219F4(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6) {
    fn_80119ED0(0);
    fn_80119ED0(0);
    fn_8011B2C0();
}
/* 0x70 | fn_80121A6C | nullcheck_call_flag */
u32 fn_80121A6C(void* obj) {
    if (fn_80119ED0(0) == 0) { return 0; }
    fn_8011B444(obj);
    return 1;
}
/* 0x70 | fn_80121ADC | nullcheck_call_flag */
u32 fn_80121ADC(void* obj) {
    if (fn_80119ED0(0) == 0) { return 0; }
    fn_8011B67C(obj);
    return 1;
}
/* 0x68 | fn_80121B4C | generic */
void fn_80121B4C(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_80119ED0(0);
    fn_80119ED0(0);
    fn_8011B788();
}
/* 0x64 | fn_80121BB4 | generic -- depends on fn_8011F5FC signature */
void fn_80121BB4(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
}
/* 0x80121C18 | 0x428 */
void fn_80121C18(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    extern void fn_8012640C();
    extern void fn_801DA36C();
    extern void fn_801DA3CC();
    extern void fn_801DE190();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r4 = 0x0;
    r5 = 0x6f;
    r6 = 0x0;
    r31 = r3;
    fn_8012640C();
    r0 = r3;
    r3 = r31;
    r30 = r0;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r28 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
        goto L_80121CB4;
    }
    r4 = r28;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x0;
    fn_8012640C();
    if ((u32)r3 == (u32)0x0) {
        r0 = 0x0;
        goto L_80121CB4;
    }
    r3 = *(u32*)&lbl_80478F90;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r28 >= (u32)r0) {
        r0 = 0x0;
        goto L_80121CB4;
    }
    r0 = 0x1;
L_80121CB4: ;
    r0 = r0 & 0xFF;
    if ((u32)r28 == (u32)r0) {
        r3 = 0x0;
        return;
    }
    r4 = r28;
    r3 = 0x0;
    r5 = 0x66;
    r6 = 0x0;
    fn_8012640C();
    r28 = r3;
    if ((u32)r31 == (u32)0x0) {
        r5 = 0x0;
    } else {
        r3 = r31;
        r4 = 0x0;
        r5 = 0x75;
        r6 = 0x0;
        fn_8012640C();
        r29 = r3;
        r3 = r31;
        r4 = 0x0;
        r5 = 0x6f;
        r6 = 0x0;
        fn_8012640C();
        r4 = (u32)r29 >> 16;
        r0 = r29 & 0xFFFF;
        r5 = (u32)r3 >> 16;
        r6 = r3 & 0xFFFF;
        r0 = r4 ^ r0;
        r3 = 0x8;
        r0 = r5 ^ r0;
        r0 = r6 ^ r0;
        r0 = r3 ^ r0;
        r0 = __cntlzw(r0);
        r0 = r3 << r0;
        r5 = (u32)r0 >> 31;
    }
    r4 = r30;
    r3 = r28 & 0xFFFF;
    fn_801DE190();
    r0 = r3;
    r3 = 0x8;
    r30 = r0;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x8, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x8;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r30;
        r4 = 0x1;
        fn_801DA3CC();
    }
    r3 = 0x7;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x7, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x7;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r30;
        r4 = 0x2;
        fn_801DA3CC();
    }
    if ((u32)r31 == (u32)0x0) {
        r0 = 0x0;
        goto L_80121FF8;
    }
    r3 = 0x3;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x3, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x3;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_80121FF8;
    }
    r3 = 0x4;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x4, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x4;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_80121FF8;
    }
    r3 = 0x5;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x5, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x5;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_80121FF8;
    }
    r3 = 0x6;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x6, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x6;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_80121FF8;
    }
    r3 = 0x7;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x7, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x7;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_80121FF8;
    }
    r3 = 0x8;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x8, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x8;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_80121FF8;
    }
    r0 = 0x1;
L_80121FF8: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r30;
        r4 = 0x1;
        fn_801DA36C();
        r3 = r30;
        r4 = 0x2;
        fn_801DA36C();
    }
    r3 = r30;
    return;
}
/* 0x80122040 | 0x2F4 */
void fn_80122040(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    extern void fn_801DA36C();
    extern void fn_801DA3CC();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r31 = r4;
    r30 = r3;
    r3 = 0x8;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x8, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r30;
        r4 = 0x8;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r31;
        r4 = 0x1;
        fn_801DA3CC();
    }
    r3 = 0x7;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x7, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r30;
        r4 = 0x7;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r31;
        r4 = 0x2;
        fn_801DA3CC();
    }
    if ((u32)r30 == (u32)0x0) {
        r0 = 0x0;
        goto L_801222F8;
    }
    r3 = 0x3;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x3, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r30;
        r4 = 0x3;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_801222F8;
    }
    r3 = 0x4;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x4, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r30;
        r4 = 0x4;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_801222F8;
    }
    r3 = 0x5;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x5, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r30;
        r4 = 0x5;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_801222F8;
    }
    r3 = 0x6;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x6, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r30;
        r4 = 0x6;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_801222F8;
    }
    r3 = 0x7;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x7, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r30;
        r4 = 0x7;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_801222F8;
    }
    r3 = 0x8;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x8, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r30;
        r4 = 0x8;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_801222F8;
    }
    r0 = 0x1;
L_801222F8: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r31;
        r4 = 0x1;
        fn_801DA36C();
        r3 = r31;
        r4 = 0x2;
        fn_801DA36C();
    }
    return;
}
/* fn_80122334 -- bit permutation hash mod 28 | Size: 0x3C */
u8 fn_80122334(u32 val) {
    u32 shuffled;
    /* Bit-permute: extract pairs of bits from different positions */
    shuffled = (val << 20 | val >> 12) & 0x30;
    shuffled = (shuffled & ~0xC0) | ((val << 14 | val >> 18) & 0xC0);
    shuffled = (shuffled & ~0x0C) | ((val << 26 | val >> 6) & 0x0C);
    shuffled = (shuffled & ~0x03) | (val & 0x03);
    return (u8)(shuffled % 28);
}
/* 0x80122370 | 0x360 */
void fn_80122370(void) {
    extern void fn_800E0C54();
    extern void fn_8011CAE0();
    extern void fn_8011CB10();
    extern void fn_8011F77C();
    extern void fn_801254B4();
    extern void fn_8012640C();
    extern void fn_80135530();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r30 = r4;
    r31 = r5;
    if ((s32)r0 == (s32)0) return;
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
        goto L_8012246C;
    }
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r28 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
        goto L_8012246C;
    }
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
        goto L_80122408;
    }
    r4 = r28;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x0;
    fn_8012640C();
    if ((u32)r3 == (u32)0x0) {
        r0 = 0x0;
        goto L_80122408;
    }
    r3 = *(u32*)&lbl_80478F90;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r28 >= (u32)r0) {
        r0 = 0x0;
        goto L_80122408;
    }
    r0 = 0x1;
L_80122408: ;
    r0 = r0 & 0xFF;
    if ((u32)r28 == (u32)r0) {
        r0 = 0x0;
        goto L_8012246C;
    }
    r3 = r29;
    r4 = 0x0;
    r5 = 0x70;
    r6 = 0x0;
    fn_8012640C();
    fn_80135530();
    r0 = r3 & 0xFF;
    if ((u32)r28 == (u32)r0) {
        r0 = 0x0;
        goto L_8012246C;
    }
    r3 = r29;
    r4 = 0x0;
    r5 = 0xb8;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_8012246C;
    }
    r0 = 0x1;
L_8012246C: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_80122510;
    }
    r3 = r29;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    if ((s32)r3 == (s32)0x19c) {
        r0 = 0x0;
        goto L_80122510;
    }
    r3 = r29;
    r4 = 0x0;
    r5 = 0xb6;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_80122510;
    }
    r0 = r31 & 0xFFFF;
    if ((u32)r0 != (u32)0x6 && (u32)r0 != (u32)0x7 && (u32)r0 != (u32)0x8) {
        r3 = r29;
        r4 = 0x0;
        r5 = 0x7b;
        r6 = 0x0;
        fn_8012640C();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = 0x0;
            goto L_80122510;
        }
    }
    r0 = 0x1;
L_80122510: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) return;
    r3 = r29;
    r4 = 0x0;
    r5 = 0x99;
    r6 = 0x0;
    fn_8012640C();
    r28 = r3;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xc2;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r29;
        r4 = 0x0;
        r5 = 0xc7;
        r6 = 0x0;
        fn_8012640C();
        r28 = r28 + r3;
    }
    r27 = 0x0;
    if ((s32)r28 >= (s32)0x64) {
        r27 = 0x1;
    }
    if ((s32)r28 >= (s32)0xc8) {
        r3 = r27 & 0xFF;
        r0 = r3 + 0x1;
        r27 = r0 & 0xFF;
    }
    r0 = r31 & 0xFFFF;
    if ((u32)r0 == (u32)0x5) {
        fn_800E0C54();
        r0 = r3 & 0x1;
        if ((u32)r0 != (u32)0x5) return;
    }
    r3 = r31;
    fn_8011CB10();
    r4 = r27;
    fn_8011CAE0();
    r31 = (s8)r3;
    if ((u32)r0 > (u32)0x5) {
        r0 = r30 & 0xFFFF;
        if ((u32)r0 == (u32)0x1b) {
            r0 = r31 * 0x96;
            r3 = (0x51ec << 16);
            r0 = (s32)((s64)r3 * (s64)r0 >> 32);
            r0 = (s32)r0 >> 5;
            r3 = (u32)r0 >> 31;
            r31 = r0 + r3;
    }
    }
    r30 = r31;
    if ((s32)r31 > (s32)0x0) {
        r3 = r29;
        r4 = 0x0;
        r5 = 0x73;
        r6 = 0x0;
        fn_8012640C();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 == (u32)0xb) {
            r30 = r31 + 0x1;
    }
    }
    r3 = r29;
    r4 = 0x0;
    r5 = 0xc2;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r29;
        fn_8011F77C();
        r0 = r3 & 0xFF;
        if ((u32)r0 < (u32)0x3) return;
        r3 = r29;
        r4 = 0x0;
        r5 = 0xc7;
        r6 = 0x0;
        fn_8012640C();
        /* add. r7, r3, r30 */;
        if ((u32)r0 < (u32)0x3) {
            r7 = 0x0;
        }
        if ((s32)r7 > (s32)0xff) {
            r7 = 0xff;
        }
        r3 = r29;
        r4 = 0x0;
        r5 = 0xc7;
        r6 = 0x0;
        fn_801254B4();
        return;
    }
    /* add. r28, r28, r30 */;
    if ((s32)r7 < (s32)0xff) {
        r28 = 0x0;
    }
    if ((s32)r28 > (s32)0xff) {
        r28 = 0xff;
    }
    r3 = r29;
    r7 = r28;
    r4 = 0x0;
    r5 = 0x99;
    r6 = 0x0;
    fn_801254B4();
    return;
}
/* 0x801226D0 | 0x324 */
void fn_801226D0(void) {
    extern u8 lbl_80272948[];
    extern void fn_801254B4();
    extern void fn_8012640C();
    extern void fn_80135530();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r7 = (u32)lbl_80272948;
    r12 = (u32)lbl_80272948;
    r29 = r4;
    r28 = r5;
    r26 = r6;
    r11 = *(u32*)((u8*)r12 + 0x0);
    r10 = *(u32*)((u8*)r12 + 0x4);
    r9 = *(u32*)((u8*)r12 + 0x8);
    r8 = *(u32*)((u8*)r12 + 0xC);
    r7 = *(u32*)((u8*)r12 + 0x10);
    r6 = *(u32*)((u8*)r12 + 0x14);
    r5 = *(u32*)((u8*)r12 + 0x18);
    r4 = *(u32*)((u8*)r12 + 0x1C);
    r0 = *(u32*)((u8*)r12 + 0x20);
    *(u32*)(sp + 0x28) = r0;
    if ((s32)r0 == (s32)0) return;
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
        goto L_80122820;
    }
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r27 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
        goto L_80122820;
    }
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
        goto L_801227BC;
    }
    r4 = r27;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x0;
    fn_8012640C();
    if ((u32)r3 == (u32)0x0) {
        r0 = 0x0;
        goto L_801227BC;
    }
    r3 = *(u32*)&lbl_80478F90;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r27 >= (u32)r0) {
        r0 = 0x0;
        goto L_801227BC;
    }
    r0 = 0x1;
L_801227BC: ;
    r0 = r0 & 0xFF;
    if ((u32)r27 == (u32)r0) {
        r0 = 0x0;
        goto L_80122820;
    }
    r3 = r25;
    r4 = 0x0;
    r5 = 0x70;
    r6 = 0x0;
    fn_8012640C();
    fn_80135530();
    r0 = r3 & 0xFF;
    if ((u32)r27 == (u32)r0) {
        r0 = 0x0;
        goto L_80122820;
    }
    r3 = r25;
    r4 = 0x0;
    r5 = 0xb8;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_80122820;
    }
    r0 = 0x1;
L_80122820: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_801228A8;
    }
    r3 = r25;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    if ((s32)r3 == (s32)0x19c) {
        r0 = 0x0;
        goto L_801228A8;
    }
    r3 = r25;
    r4 = 0x0;
    r5 = 0xb6;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_801228A8;
    }
    r3 = r25;
    r4 = 0x0;
    r5 = 0x7b;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_801228A8;
    }
    r0 = 0x1;
L_801228A8: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) return;
    r24 = 0x1;
    r27 = 0x0;
    r31 = 0x0;
    while (r0 = r31 & 0xFF, (u32)r0 < (u32)0x6) {

    r0 = r31 & 0xFF;
    r30 = (u32)sp + 0x8;
    r0 = r0 * 0x6;
    r3 = r25;
    r4 = 0x0;
    r6 = 0x0;
    r30 = r30 + r0;
    r5 = *(u16*)((u8*)r30 + 0x0);
    fn_8012640C();
    *(u16*)((u8*)r30 + 0x4) = r3;
    r31 = r31 + 0x1;
    r0 = *(u16*)((u8*)r30 + 0x4);
    r27 = r27 + r0;
    }

    r0 = r28 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r24 = 0x2;
    }
    r30 = r24 & 0xFF;
    r31 = r29 & 0xFFFF;
    r28 = 0x0;
    while (r0 = r28 & 0xFF, (u32)r0 < (u32)0x6) {

    r0 = r27 & 0xFFFF;
    if ((u32)r0 >= (u32)0x1fe) return;
    r0 = r28 & 0xFF;
    r29 = (u32)sp + 0x8;
    r0 = r0 * 0x6;
    r4 = r26;
    r3 = 0x0;
    r6 = 0x0;
    r29 = r29 + r0;
    r5 = *(u16*)((u8*)r29 + 0x2);
    fn_8012640C();
    r0 = r3 & 0xFFFF;
    r0 = r30 * r0;
    r5 = r0 & 0xFFFF;
    if ((u32)r31 == (u32)0x18) {
        /* clrlslwi r5, r5, 17, 1 */;
    }
    r3 = r27 & 0xFFFF;
    r0 = r5 & 0xFFFF;
    r3 = r3 + r0;
    if ((s32)r3 > (s32)0x1fe) {
        r0 = r5 - r0;
        r5 = r0 & 0xFFFF;
    }
    r0 = *(u16*)((u8*)r29 + 0x4);
    r3 = r5 & 0xFFFF;
    r3 = r3 + r0;
    if ((s32)r3 > (s32)0xff) {
        r0 = r5 - r0;
        r5 = r0 & 0xFFFF;
    }
    r0 = *(u16*)((u8*)r29 + 0x4);
    r3 = r25;
    r27 = r27 + r5;
    r4 = 0x0;
    r0 = r0 + r5;
    r6 = 0x0;
    *(u16*)((u8*)r29 + 0x4) = r0;
    r5 = *(u16*)((u8*)r29 + 0x0);
    r7 = *(u16*)((u8*)r29 + 0x4);
    fn_801254B4();
    r28 = r28 + 0x1;
    }

    return;
}
/* 0x7C | fn_801229F4 | generic -- placeholder, depends on fn_8011CE74/fn_8011CE44 */
u32 fn_801229F4(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    return 0;
}
/* 0x70 | fn_80122A70 | generic */
u32 fn_80122A70(void) {
    fn_8012640C();
    fn_8012640C();
    return 0;
}
/* 0x70 | fn_80122AE0 | generic */
u32 fn_80122AE0(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_8012640C();
    return 1;
}
/* 0x70 | fn_80122B50 | generic */
u32 fn_80122B50(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_8012640C();
    return 1;
}
/* 0x80122BC0 | 0xA4 */
void fn_80122BC0(void) {
    extern void fn_8012640C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = r4 & 0xFFFF;
    r30 = r4;
    r29 = r3;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    if ((u32)r29 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r4 = 0x0;
    r5 = 0x83;
    r6 = 0x0;
    fn_8012640C();
    r31 = r3 & 0xFFFF;
    r3 = r29;
    r4 = 0x0;
    r5 = 0x87;
    r6 = 0x0;
    fn_8012640C();
    r3 = r3 & 0xFFFF;
    r0 = r30 & 0xFFFF;
    r0 = (s32)r3 / (s32)r0;
    r3 = (u32)r31 >> 31;
    r4 = (s32)r0 >> 31;
    r0 = r0 - r31;
    r0 = r4 + r3; /* +carry */;
    r3 = r0 & 0xFF;
    return;
}
/* 0x80122C64 | 0x178 */
void fn_80122C64(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x3;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x3, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x3;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x5;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x5, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x5;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x6;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x6, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x6;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x4;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x4, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x4;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x0;
    return;
}
/* 0x80122DDC | 0x218 */
void fn_80122DDC(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x3;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x3, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x3;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x4;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x4, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x4;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x5;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x5, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x5;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x6;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x6, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x6;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x7;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x7, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x7;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x8;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c || ((r3 = 0x8, fn_80119ED0(), r0 = r3 & 0xFFFF), (u32)r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x8;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;
    return;
}
/* 0x80122FF4 | 0x9C */
void fn_80122FF4(void) {
    extern void fn_80008154();
    extern void fn_8012640C();
    extern void fn_80142CF4();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;
    r4 = 0x0;
    r5 = 0x82;
    r6 = 0x0;
    fn_8012640C();
    r31 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    fn_80008154();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r4 = r31;
        r3 = 0x0;
        r5 = 0x7;
        r6 = 0x0;
        fn_80142CF4();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 == (u32)0x1a) { r3 = 0x63; return; }
        if ((u32)r0 == (u32)0x1e) {
            r3 = 0x63;
            return;
        }
    }
    r4 = r31;
    r3 = 0x0;
    r5 = 0xa;
    r6 = 0x0;
    fn_80142CF4();
    r3 = r3 & 0xFFFF;
    return;
}
/* 0x50 | fn_80123090 | multi_call_cond */
u32 fn_80123090(void) {
    if (fn_8012640C() != 0) { return 0; }
    fn_80142CF4();
    return 0;
}
/* 0x801230E0 | 0x30 */
u16 fn_801230E0(u32 arg) {
    /* depends on fn_8012640C signature */
    return 0;
}
/* 0x80123110 | 0x94 */
void fn_80123110(void) {
    extern void fn_801254B4();
    extern void fn_8012640C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r31 = r4;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
    } else {
        r0 = r5 & 0xFF;
        if ((s32)r0 == (s32)0) {
            r4 = 0x0;
            r5 = 0x82;
            r6 = 0x0;
            fn_8012640C();
            r0 = r3 & 0xFFFF;
            r3 = r30;
            r31 = r0;
            r4 = 0x0;
            r5 = 0x82;
            r6 = 0x0;
            r7 = 0x0;
            fn_801254B4();
        } else {
            r7 = r31 & 0xFFFF;
            r4 = 0x0;
            r5 = 0x82;
            r6 = 0x0;
            fn_801254B4();
        }
        r3 = r31;
    }
    return;
}
/* 0x801231A4 | 0x13C */
void fn_801231A4(void) {
    extern void fn_8012640C();
    extern void fn_80131574();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    if ((s32)r0 == (s32)0) {
        r3 = 0x2;
        return;
    }
    r4 = 0x0;
    r5 = 0x6f;
    r6 = 0x0;
    fn_8012640C();
    r31 = r3;
    r3 = r29;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x13;
    r6 = 0x0;
    fn_8012640C();
    r30 = r3 & 0xFFFF;
    if ((u32)r29 == (u32)0x0) {
        r3 = 0x2;
    } else {
    r3 = r29;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x13;
    r6 = 0x0;
    fn_8012640C();
    r29 = r3 & 0xFFFF;
    r3 = 0x0;
    fn_80131574();
    r0 = r3 & 0xFF;
    if ((s32)r29 == (s32)r0) {
        r3 = 0x0;
    } else {
    r3 = 0x1;
    fn_80131574();
    r0 = r3 & 0xFF;
    if ((s32)r29 == (s32)r0) {
        r3 = 0x1;
    } else {
    r3 = 0x2;
    fn_80131574();
    r0 = r3 & 0xFF;
    if ((s32)r29 == (s32)r0) {
        r3 = 0x2;
    } else {
    r3 = -0x1;
    }
    }
    }
    }
    r0 = (s8)r3;
    if ((s32)r29 >= (s32)r0) { r3 = r3 & 0xFF; return; }
    r0 = r31 & 0xFF;
    if ((u32)r30 > (u32)r0) {
        r3 = 0x1;
        r3 = r3 & 0xFF;
        return;
    }
    r3 = 0x0;
    r3 = r3 & 0xFF;
    return;
}
/* 0x801232E0 | 0x88 */
void fn_801232E0(void) {
    extern void fn_8012640C();
    extern void fn_801EE958();
    extern void fn_801EEB34();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r30 = r4;
    if ((s32)r0 != (s32)0) {
        r4 = 0x0;
        r5 = 0xc2;
        r6 = 0x0;
        fn_8012640C();
        r0 = r3 & 0xFF;
        if ((s32)r0 != (s32)0) {
            r3 = r31;
            r4 = 0x0;
            r5 = 0xc3;
            r6 = 0x0;
            fn_8012640C();
            r31 = r3 & 0xFFFF;
            r4 = 0x1;
            r3 = r31;
            fn_801EE958();
            r0 = r30 & 0xFF;
            if ((s32)r0 != (s32)0) {
                r3 = r31;
                r4 = 0x1;
                fn_801EEB34();
    }
    }
    }
    return;
}
/* 0x80123368 | 0x8C */
void fn_80123368(void) {
    extern void fn_801254B4();
    extern void fn_8012640C();
    extern void fn_8025FF9C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r31 = r4;
    if ((s32)r0 != (s32)0) {
        r4 = 0x0;
        r5 = 0x6e;
        r6 = 0x0;
        fn_8012640C();
        r4 = r30;
        r3 = 0x0;
        fn_8025FF9C();
        r3 = r30;
        r4 = 0x0;
        r5 = 0x62;
        r6 = 0x0;
        r7 = 0x1;
        fn_801254B4();
        r0 = r31 & 0xFF;
        if ((s32)r0 != (s32)0) {
            r3 = r30;
            r4 = 0x0;
            r5 = 0x63;
            r6 = 0x0;
            r7 = 0x1;
            fn_801254B4();
        }
    }
    return;
}
/* 0x801233F4 | 0x190 */
void fn_801233F4(void) {
    extern void fn_8012640C();
    extern void fn_80135530();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    do {
        if ((s32)r0 == (s32)0) {
            r0 = 0x0;
            break;
        }
        r4 = 0x0;
        r5 = 0x6e;
        r6 = 0x0;
        fn_8012640C();
        r31 = r3 & 0xFFFF;
        if ((s32)r0 == (s32)0) {
            r0 = 0x0;
            break;
        }
        if ((s32)r0 == (s32)0) {
            r0 = 0x0;
        } else {
            r4 = r31;
            r3 = 0x0;
            r5 = 0x1;
            r6 = 0x0;
            fn_8012640C();
            if ((u32)r3 == (u32)0x0) {
                r0 = 0x0;
            } else {
                r3 = *(u32*)&lbl_80478F90;
                r0 = *(u32*)((u8*)r3 + 0x0);
                if ((u32)r31 >= (u32)r0) {
                    r0 = 0x0;
                } else {
                    r0 = 0x1;
                }
            }
        }
        r0 = r0 & 0xFF;
        if ((u32)r31 == (u32)r0) {
            r0 = 0x0;
            break;
        }
        r3 = r30;
        r4 = 0x0;
        r5 = 0x70;
        r6 = 0x0;
        fn_8012640C();
        fn_80135530();
        r0 = r3 & 0xFF;
        if ((u32)r31 == (u32)r0) {
            r0 = 0x0;
            break;
        }
        r3 = r30;
        r4 = 0x0;
        r5 = 0xb8;
        r6 = 0x0;
        fn_8012640C();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = 0x0;
            break;
        }
        r0 = 0x1;
    } while (0);
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    if ((s32)r3 == (s32)0x19c) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r4 = 0x0;
    r5 = 0xb6;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r4 = 0x0;
    r5 = 0x7b;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3 & 0xFF;
    r3 = 0x1 - r4;
    r0 = r3 | r0;
    r3 = (u32)r0 >> 31;
    return;
}
/* 0x80123584 | 0x98 */
void fn_80123584(void) {
    extern void fn_8012640C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r29 = r4;
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r30 = r3 & 0xFFFF;
    r31 = r29 & 0xFF;
    r29 = 0x0;
    while (r0 = r29 & 0xFF, (u32)r0 < (u32)0x14) {
        r4 = r30;
        r6 = r29 & 0xFF;
        r3 = 0x0;
        r5 = 0x1d;
        fn_8012640C();
        if ((s32)r31 == (s32)r3) { r3 = r29; return; }
        r29 = r29 + 0x1;
    }
    r3 = r29;
    return;
}
/* 0x8012361C | 0xDC */
void fn_8012361C(void) {
    extern void fn_801237B8();
    extern void fn_8012640C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r30 = r4;
    r28 = r5;
    r29 = r6;
    if ((s32)r0 == (s32)0) {
        r3 = -0x2;
        return;
    }
    if ((s32)r0 != (s32)0 && (u32)r29 != (u32)0x0) {
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r31 = r3 & 0xFFFF;
    r30 = r30 & 0xFF;
    while (r6 = *(u8*)((u8*)r29 + 0x0), (u32)r6 < (u32)0x14) {

    r4 = r31;
    r3 = 0x0;
    r5 = 0x1d;
    fn_8012640C();
    if ((s32)r30 == (s32)r3) {
        r6 = *(u8*)((u8*)r29 + 0x0);
        r4 = r31;
        r3 = 0x0;
        r5 = 0x1e;
        fn_8012640C();
        r4 = r3 & 0xFFFF;
        break;
    }
    r3 = *(u8*)((u8*)r29 + 0x0);
    r0 = r3 + 0x1;
    *(u8*)((u8*)r29 + 0x0) = r0;
    }

    if ((u32)r6 >= (u32)0x14) { r4 = 0x0; }
    } else {
    r4 = 0x0;
    }
    r0 = r4 & 0xFFFF;
    if ((u32)r6 == (u32)0x14) {
        r3 = -0x3;
        return;
    }
    r3 = r27;
    r5 = r28;
    fn_801237B8();
    return;
}
/* 0x801236F8 | 0xC0 */
void fn_801236F8(void) {
    extern void fn_8012640C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r31 = r4;
    r29 = r5;
    if ((u32)r3 == (u32)0x0) { r3 = 0x0; return; }
    if ((u32)r29 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r30 = r3 & 0xFFFF;
    r31 = r31 & 0xFF;
    while (r6 = *(u8*)((u8*)r29 + 0x0), (u32)r6 < (u32)0x14) {
        r4 = r30;
        r3 = 0x0;
        r5 = 0x1d;
        fn_8012640C();
        if ((s32)r31 == (s32)r3) {
            r6 = *(u8*)((u8*)r29 + 0x0);
            r4 = r30;
            r3 = 0x0;
            r5 = 0x1e;
            fn_8012640C();
            r3 = r3 & 0xFFFF;
            return;
        }
        r3 = *(u8*)((u8*)r29 + 0x0);
        r0 = r3 + 0x1;
        *(u8*)((u8*)r29 + 0x0) = r0;
    }
    r3 = 0x0;
    return;
}
/* 0x801237B8 | 0x3A4 */
void fn_801237B8(void) {
    extern void fn_8011BB6C();
    extern void fn_8011F260();
    extern void fn_8011F5E0();
    extern void fn_801254B4();
    extern void fn_8012640C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r31 = r4;
    r25 = r5;
    if ((s32)r0 == (s32)0) {
        r3 = -0x2;
        return;
    }
    if ((s32)r0 == (s32)0) {
        r29 = -0x1;
        goto L_80123894;
    }
    r28 = r31 & 0xFFFF;
    r29 = 0x0;
    while (r0 = (s8)r29, (s32)r0 < (s32)0x4) {

    r27 = (s8)r29;
    if ((u32)r30 == (u32)0x0) {
        r0 = 0x0;
        goto L_80123858;
    }
    r3 = r30;
    r6 = r27;
    r4 = 0x0;
    r5 = 0x7f;
    fn_8012640C();
    if ((s32)r3 == (s32)0x0) {
        r0 = 0x0;
        goto L_80123858;
    }
    r3 = r30;
    r6 = r27;
    r4 = 0x0;
    r5 = 0x7f;
    fn_8012640C();
    if ((s32)r3 == (s32)0x163) {
        r0 = 0x0;
        goto L_80123858;
    }
    r0 = 0x1;
L_80123858: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 != (s32)0x163) {
        r3 = r30;
        r6 = r27;
        r4 = 0x0;
        r5 = 0x7f;
        fn_8012640C();
        if ((s32)r28 == (s32)r3) {
            goto L_80123894;
        }
    }
    r29 = r29 + 0x1;
    }

    r29 = -0x1;
L_80123894: ;
    r0 = (s8)r29;
    if ((s32)r0 >= (s32)0x4) {
        r3 = -0x2;
        return;
    }
    r26 = 0x0;
    while (r0 = (s8)r26, (s32)r0 < (s32)0x4) {

    if ((u32)r30 == (u32)0x0) {
        r0 = 0x0;
        goto L_8012390C;
    }
    r27 = (s8)r26;
    r3 = r30;
    r6 = r27;
    r4 = 0x0;
    r5 = 0x7f;
    fn_8012640C();
    if ((s32)r3 == (s32)0x0) {
        r0 = 0x0;
        goto L_8012390C;
    }
    r3 = r30;
    r6 = r27;
    r4 = 0x0;
    r5 = 0x7f;
    fn_8012640C();
    if ((s32)r3 == (s32)0x163) {
        r0 = 0x0;
        goto L_8012390C;
    }
    r0 = 0x1;
L_8012390C: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 == (s32)0x163) {
        if ((u32)r30 != (u32)0x0) {
            if ((u32)r30 != (u32)0x0) {
                r27 = (s8)r26;
                r3 = r30;
                r6 = r27;
                r4 = 0x0;
                r5 = 0x7f;
                r7 = 0x0;
                fn_801254B4();
                r3 = r30;
                r6 = r27;
                r4 = 0x0;
                r5 = 0x80;
                r7 = 0x0;
                fn_801254B4();
                r3 = r30;
                r6 = r27;
                r4 = 0x0;
                r5 = 0x81;
                r7 = 0x0;
                fn_801254B4();
            }
            r27 = (s8)r26;
            r3 = r30;
            r6 = r27;
            r7 = r31 & 0xFFFF;
            r4 = 0x0;
            r5 = 0x7f;
            fn_801254B4();
            if ((u32)r30 == (u32)0x0) {
                r0 = 0x0;
            } else {
                r4 = r27 & 0xFFFF;
                r3 = r30;
                r28 = r4 + 0x4;
                r4 = 0x0;
                r6 = r28 & 0xFFFF;
                r5 = 0x7f;
                fn_8012640C();
                r29 = r3 & 0xFFFF;
                r3 = r30;
                r6 = r28 & 0xFFFF;
                r4 = 0x0;
                r5 = 0x81;
                fn_8012640C();
                r4 = r3 & 0xFF;
                r3 = r29;
                fn_8011BB6C();
                r0 = r3;
            }
            r3 = r30;
            r6 = r27;
            r7 = r0 & 0xFF;
            r4 = 0x0;
            r5 = 0x80;
            fn_801254B4();
        }
        r3 = r26;
        return;
    }
    r26 = r26 + 0x1;
    }

    r0 = r25 & 0xFF;
    if ((s32)r0 == (s32)0x4) {
        r3 = -0x1;
        return;
    }
    r27 = 0x1;
    while (r0 = (s8)r27, (s32)r0 < (s32)0x4) {

    if ((u32)r30 != (u32)0x0) {
        r4 = (s8)r27;
        r3 = r30;
        r5 = 0x0;
        r4 = r0 & 0xFFFF;
        fn_8011F260();
        r29 = r3;
        r3 = r30;
        r4 = (s8)r27;
        r5 = 0x0;
        fn_8011F260();
        r4 = r3;
        r3 = r29;
        fn_8011F5E0();
    }
    r27 = r27 + 0x1;
    }

    if ((u32)r30 != (u32)0x0) {
        if ((u32)r30 != (u32)0x0) {
            r3 = r30;
            r4 = 0x0;
            r5 = 0x7f;
            r6 = 0x3;
            r7 = 0x0;
            fn_801254B4();
            r3 = r30;
            r4 = 0x0;
            r5 = 0x80;
            r6 = 0x3;
            r7 = 0x0;
            fn_801254B4();
            r3 = r30;
            r4 = 0x0;
            r5 = 0x81;
            r6 = 0x3;
            r7 = 0x0;
            fn_801254B4();
        }
        r3 = r30;
        r7 = r31 & 0xFFFF;
        r4 = 0x0;
        r5 = 0x7f;
        r6 = 0x3;
        fn_801254B4();
        if ((u32)r30 == (u32)0x0) {
            r0 = 0x0;
        } else {
            r3 = r30;
            r4 = 0x0;
            r5 = 0x7f;
            r6 = 0x7;
            fn_8012640C();
            r29 = r3 & 0xFFFF;
            r3 = r30;
            r4 = 0x0;
            r5 = 0x81;
            r6 = 0x7;
            fn_8012640C();
            r4 = r3 & 0xFF;
            r3 = r29;
            fn_8011BB6C();
            r0 = r3;
        }
        r3 = r30;
        r7 = r0 & 0xFF;
        r4 = 0x0;
        r5 = 0x80;
        r6 = 0x3;
        fn_801254B4();
    }
    r3 = 0x3;
    return;
}
/* 0x80123B5C | 0xF8 */
void fn_80123B5C(void) {
    extern void fn_8012640C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    if ((s32)r0 == (s32)0) {
        r3 = -0x1;
        return;
    }
    r31 = r4 & 0xFFFF;
    r29 = 0x0;
    while (r0 = (s8)r29, (s32)r0 < (s32)0x4) {

    if ((u32)r28 == (u32)0x0) {
        r0 = 0x0;
    } else {
    r30 = (s8)r29;
    r3 = r28;
    r6 = r30;
    r4 = 0x0;
    r5 = 0x7f;
    fn_8012640C();
    if ((s32)r3 == (s32)0x0) {
        r0 = 0x0;
    } else {
    r3 = r28;
    r6 = r30;
    r4 = 0x0;
    r5 = 0x7f;
    fn_8012640C();
    if ((s32)r3 == (s32)0x163) {
        r0 = 0x0;
    } else {
    r0 = 0x1;
    }
    }
    }
    r0 = r0 & 0xFF;
    if ((s32)r3 != (s32)0x163) {
        r3 = r28;
        r6 = (s8)r29;
        r4 = 0x0;
        r5 = 0x7f;
        fn_8012640C();
        if ((s32)r31 == (s32)r3) {
            r3 = r29;
            return;
    }
    }
    r29 = r29 + 0x1;
    }

    r3 = -0x1;
    return;
}
/* 0x80 | fn_80123C54 | generic */
void fn_80123C54(void* ptr, u32 idx, u32 arg) {
    /* depends on fn_8011F260 signature -- will convert with fn_8011F260 */
}
/* 0x80123CD4 | 0x84 */
void fn_80123CD4(void) {
    extern void fn_8012640C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r31 = r4;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r6 = r31;
    r4 = 0x0;
    r5 = 0x7f;
    fn_8012640C();
    if ((s32)r3 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r6 = r31;
    r4 = 0x0;
    r5 = 0x7f;
    fn_8012640C();
    r4 = 0x163 - r3;
    r0 = r4 | r0;
    r3 = (u32)r0 >> 31;
    return;
}
/* 0x80123D58 | 0x118 */
void fn_80123D58(void) {
    extern void fn_8011BB6C();
    extern void fn_801254B4();
    extern void fn_8012640C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r30 = r5;
    r29 = r4;
    if ((s32)r0 != (s32)0) {
        if ((s32)r0 != (s32)0) {
            r6 = r29;
            r4 = 0x0;
            r5 = 0x7f;
            r7 = 0x0;
            fn_801254B4();
            r3 = r28;
            r6 = r29;
            r4 = 0x0;
            r5 = 0x80;
            r7 = 0x0;
            fn_801254B4();
            r3 = r28;
            r6 = r29;
            r4 = 0x0;
            r5 = 0x81;
            r7 = 0x0;
            fn_801254B4();
        }
        r3 = r28;
        r6 = r29;
        r7 = r30 & 0xFFFF;
        r4 = 0x0;
        r5 = 0x7f;
        fn_801254B4();
        if ((u32)r28 == (u32)0x0) {
            r0 = 0x0;
        } else {
            r4 = r29 & 0xFFFF;
            r3 = r28;
            r30 = r4 + 0x4;
            r4 = 0x0;
            r6 = r30 & 0xFFFF;
            r5 = 0x7f;
            fn_8012640C();
            r31 = r3 & 0xFFFF;
            r3 = r28;
            r6 = r30 & 0xFFFF;
            r4 = 0x0;
            r5 = 0x81;
            fn_8012640C();
            r4 = r3 & 0xFF;
            r3 = r31;
            fn_8011BB6C();
            r0 = r3;
        }
        r3 = r28;
        r6 = r29;
        r7 = r0 & 0xFF;
        r4 = 0x0;
        r5 = 0x80;
        fn_801254B4();
    }
    return;
}
/* 0x80 | fn_80123E70 | generic */
u32 fn_80123E70(void) {
    fn_8012640C();
    fn_8012640C();
    fn_8011BB6C(0, 0, 0, 0, 0);
    return 0;
}
/* 0x80123EF0 | 0xCC */
void fn_80123EF0(void) {
    extern void fn_801254B4();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r27 = r5;
    r28 = r6;
    r29 = r7;
    r30 = r8;
    r31 = r9;
    if ((s32)r0 != (s32)0) {
        r7 = r4;
        r4 = 0x0;
        r5 = 0x71;
        r6 = 0x0;
        fn_801254B4();
        r3 = r26;
        r7 = r27 & 0xFF;
        r4 = 0x0;
        r5 = 0x72;
        r6 = 0x0;
        fn_801254B4();
        r3 = r26;
        r7 = r28 & 0xFFFF;
        r4 = 0x0;
        r5 = 0x73;
        r6 = 0x0;
        fn_801254B4();
        r3 = r26;
        r7 = r29 & 0xFF;
        r4 = 0x0;
        r5 = 0x74;
        r6 = 0x0;
        fn_801254B4();
        r3 = r26;
        r7 = r30;
        r4 = 0x0;
        r5 = 0x75;
        r6 = 0x0;
        fn_801254B4();
        r3 = r26;
        r7 = r31;
        r4 = 0x0;
        r5 = 0x76;
        r6 = 0x0;
        fn_801254B4();
    }
    return;
}
/* 0x80123FBC | 0x108 */
void fn_80123FBC(void) {
    extern void fn_8012640C();
    extern void fn_80135530();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r31 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
    } else {
    r4 = r31;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x0;
    fn_8012640C();
    if ((u32)r3 == (u32)0x0) {
        r0 = 0x0;
    } else {
    r3 = *(u32*)&lbl_80478F90;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r31 >= (u32)r0) {
        r0 = 0x0;
    } else {
    r0 = 0x1;
    }
    }
    }
    r0 = r0 & 0xFF;
    if ((u32)r31 == (u32)r0) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r4 = 0x0;
    r5 = 0x70;
    r6 = 0x0;
    fn_8012640C();
    fn_80135530();
    r0 = r3 & 0xFF;
    if ((u32)r31 == (u32)r0) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r4 = 0x0;
    r5 = 0xb8;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3 & 0xFF;
    r3 = 0x1 - r4;
    r0 = r3 | r0;
    r3 = (u32)r0 >> 31;
    return;
}
/* 0x801240C4 | 0x34C */
void fn_801240C4(void) {
    extern void fn_800E0C54();
    extern void fn_800FA280();
    extern void fn_8011CE44();
    extern void fn_8011CE74();
    extern void fn_8012361C();
    extern void fn_80124A60();
    extern void fn_8012546C();
    extern void fn_801254B4();
    extern void fn_8012640C();
    extern void fn_80135AD0();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r30 = r4;
    r31 = r5;
    r27 = r6;
    if ((s32)r0 == (s32)0) return;
    r0 = r30 & 0xFFFF;
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
        goto L_80124138;
    }
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x0;
    fn_8012640C();
    if ((u32)r3 == (u32)0x0) {
        r0 = 0x0;
        goto L_80124138;
    }
    r3 = *(u32*)&lbl_80478F90;
    r4 = r30 & 0xFFFF;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r4 >= (u32)r0) {
        r0 = 0x0;
        goto L_80124138;
    }
    r0 = 0x1;
L_80124138: ;
    r0 = r0 & 0xFF;
    if ((u32)r4 == (u32)r0) return;
    if ((u32)r27 == (u32)0x0) return;
    r3 = r29;
    fn_80124A60();
    r3 = r29;
    r7 = r30 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_801254B4();
    fn_800E0C54();
    r28 = r3 << 16;
    fn_800E0C54();
    r0 = r3 & 0xFFFF;
    r3 = r29;
    r7 = r0 | r28;
    r4 = 0x0;
    r5 = 0x6f;
    r6 = 0x0;
    fn_801254B4();
    r3 = r29;
    r4 = 0x0;
    r5 = 0x70;
    r6 = 0x0;
    fn_8012640C();
    r4 = r27;
    fn_80135AD0();
    r4 = r30;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x0;
    fn_8012640C();
    fn_800FA280();
    r7 = r3;
    r3 = r29;
    r4 = 0x0;
    r5 = 0x77;
    r6 = 0x0;
    fn_801254B4();
    r3 = r29;
    r7 = r31 & 0xFF;
    r4 = 0x0;
    r5 = 0x7a;
    r6 = 0x0;
    fn_801254B4();
    fn_800E0C54();
    r7 = r3 & 0x1F;
    r27 = r3 & 0xFFFF;
    r3 = r29;
    r4 = 0x0;
    r5 = 0x93;
    r6 = 0x0;
    fn_801254B4();
    r3 = r29;
    r4 = 0x0;
    r5 = 0x94;
    r6 = 0x0;
    fn_801254B4();
    r3 = r29;
    r4 = 0x0;
    r5 = 0x95;
    r6 = 0x0;
    fn_801254B4();
    fn_800E0C54();
    r7 = r3 & 0x1F;
    r27 = r3 & 0xFFFF;
    r3 = r29;
    r4 = 0x0;
    r5 = 0x98;
    r6 = 0x0;
    fn_801254B4();
    r3 = r29;
    r4 = 0x0;
    r5 = 0x96;
    r6 = 0x0;
    fn_801254B4();
    r3 = r29;
    r4 = 0x0;
    r5 = 0x97;
    r6 = 0x0;
    fn_801254B4();
    r3 = r29;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x11;
    r6 = 0x0;
    fn_8012640C();
    r27 = r3 & 0xFF;
    r3 = r29;
    r4 = 0x0;
    r5 = 0x7a;
    r6 = 0x0;
    fn_8012640C();
    r28 = r3 & 0xFF;
    r3 = r27;
    fn_8011CE74();
    if ((u32)r3 == (u32)0x0) {
        r7 = 0x0;
    } else {
        r4 = r28;
        fn_8011CE44();
        r7 = r3;
    }
    r3 = r29;
    r4 = 0x0;
    r5 = 0x79;
    r6 = 0x0;
    fn_801254B4();
    r4 = r30;
    r3 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3;
    r3 = r29;
    r7 = r0;
    r4 = 0x0;
    r5 = 0x99;
    r6 = 0x0;
    fn_801254B4();
    fn_800E0C54();
    r0 = r3 & 0x1;
    if ((u32)r3 != (u32)0x0) {
        r27 = 0x1;
        if ((u32)r29 != (u32)0x0) {
            r3 = r29;
            r4 = 0x0;
            r5 = 0x6e;
            r6 = 0x0;
            fn_8012640C();
            r4 = r3 & 0xFFFF;
            r3 = 0x0;
            r5 = 0x17;
            r6 = 0x1;
            fn_8012640C();
            if ((s32)r3 == (s32)0x0) {
                r27 = 0x0;
            }
            r3 = r29;
            r7 = r27 & 0xFF;
            r4 = 0x0;
            r5 = 0xb7;
            r6 = 0x0;
            fn_801254B4();
    }
    }
    r27 = r31 & 0xFF;
    r28 = 0x1;
    r30 = 0x0;
    while ((u32)r28 <= (u32)r27) {
        *(u8*)(sp + 0x8) = r30;
    L_801243B8: ;
        r3 = r29;
        r4 = r28 & 0xFF;
        r6 = (u32)sp + 0x8;
        r5 = 0x1;
        fn_8012361C();
        r0 = (s8)r3;
        if ((s32)r0 == (s32)-0x3) goto L_801243E8;
        r3 = *(u8*)(sp + 0x8);
        r0 = r3 + 0x1;
        *(u8*)(sp + 0x8) = r0;
        goto L_801243B8;
    L_801243E8: ;
        r28 = r28 + 0x1;
    }
    r3 = r29;
    fn_8012546C();
    return;
}
/* 0x80124410 | 0x4B4 */
void fn_80124410(void) {
    extern void fn_800E0C54();
    extern void fn_8012640C();
    extern void fn_80131574();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r25 = r4;
    r26 = r5;
    r27 = r6;
    if ((s32)r0 != (s32)0) goto L_80124898;
    fn_800E0C54();
    r23 = r3 << 16;
    fn_800E0C54();
    r0 = r3 & 0xFFFF;
    r3 = r0 | r23;
    return;
L_8012444C: ;
    fn_800E0C54();
    r23 = r3 << 16;
    fn_800E0C54();
    r0 = (s8)r25;
    r0 = r3 & 0xFFFF;
    r31 = r0 | r23;
    if ((s32)r0 >= (s32)0) {
        if ((u32)r24 == (u32)0x0) {
            r0 = 0x2;
            goto L_801244FC;
        }
        r3 = r24;
        r4 = 0x0;
        r5 = 0x6e;
        r6 = 0x0;
        fn_8012640C();
        r4 = r3 & 0xFFFF;
        r3 = 0x0;
        r5 = 0x13;
        r6 = 0x0;
        fn_8012640C();
        r23 = r3 & 0xFFFF;
        r3 = 0x0;
        fn_80131574();
        r0 = r3 & 0xFF;
        if ((s32)r23 == (s32)r0) {
            r0 = 0x0;
            goto L_801244FC;
        }
        r3 = 0x1;
        fn_80131574();
        r0 = r3 & 0xFF;
        if ((s32)r23 == (s32)r0) {
            r0 = 0x1;
            goto L_801244FC;
        }
        r3 = 0x2;
        fn_80131574();
        r0 = r3 & 0xFF;
        if ((s32)r23 == (s32)r0) {
            r0 = 0x2;
            goto L_801244FC;
        }
        r0 = -0x1;
    L_801244FC: ;
        r0 = (s8)r0;
    }
    if ((s32)r23 >= (s32)r0) goto L_801247FC;
    r0 = (s8)r25;
    if ((s32)r0 == (s32)0x2) goto L_80124600;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x13;
    r6 = 0x0;
    fn_8012640C();
    r23 = r3 & 0xFFFF;
    if ((u32)r24 == (u32)0x0) {
        r3 = 0x2;
        goto L_801245D0;
    }
    r3 = r24;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x13;
    r6 = 0x0;
    fn_8012640C();
    r22 = r3 & 0xFFFF;
    r3 = 0x0;
    fn_80131574();
    r0 = r3 & 0xFF;
    if ((s32)r22 == (s32)r0) {
        r3 = 0x0;
        goto L_801245D0;
    }
    r3 = 0x1;
    fn_80131574();
    r0 = r3 & 0xFF;
    if ((s32)r22 == (s32)r0) {
        r3 = 0x1;
        goto L_801245D0;
    }
    r3 = 0x2;
    fn_80131574();
    r0 = r3 & 0xFF;
    if ((s32)r22 == (s32)r0) {
        r3 = 0x2;
        goto L_801245D0;
    }
    r3 = -0x1;
L_801245D0: ;
    r0 = (s8)r3;
    if ((s32)r22 < (s32)r0) {
        r0 = r31 & 0xFF;
        if ((u32)r23 <= (u32)r0) goto L_801245EC;
        r3 = 0x1;
    }
    goto L_801245F0;
L_801245EC: ;
    r3 = 0x0;
L_801245F0: ;
    r0 = r3 & 0xFF;
    if ((s32)r30 != (s32)r0) goto L_8012444C;
    goto L_801247FC;
L_80124600: ;
    if ((u32)r24 == (u32)0x0) {
        r23 = 0x2;
        goto L_8012470C;
    }
    r3 = r24;
    r4 = 0x0;
    r5 = 0x6f;
    r6 = 0x0;
    fn_8012640C();
    r23 = r3;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x13;
    r6 = 0x0;
    fn_8012640C();
    r22 = r3 & 0xFFFF;
    if ((u32)r24 == (u32)0x0) {
        r3 = 0x2;
        goto L_801246E8;
    }
    r3 = r24;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x13;
    r6 = 0x0;
    fn_8012640C();
    r21 = r3 & 0xFFFF;
    r3 = 0x0;
    fn_80131574();
    r0 = r3 & 0xFF;
    if ((s32)r21 == (s32)r0) {
        r3 = 0x0;
        goto L_801246E8;
    }
    r3 = 0x1;
    fn_80131574();
    r0 = r3 & 0xFF;
    if ((s32)r21 == (s32)r0) {
        r3 = 0x1;
        goto L_801246E8;
    }
    r3 = 0x2;
    fn_80131574();
    r0 = r3 & 0xFF;
    if ((s32)r21 == (s32)r0) {
        r3 = 0x2;
        goto L_801246E8;
    }
    r3 = -0x1;
L_801246E8: ;
    r0 = (s8)r3;
    if ((s32)r21 < (s32)r0) {
        r0 = r23 & 0xFF;
        if ((u32)r22 <= (u32)r0) goto L_80124704;
        r3 = 0x1;
    }
    goto L_80124708;
L_80124704: ;
    r3 = 0x0;
L_80124708: ;
    r23 = r3 & 0xFF;
L_8012470C: ;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x13;
    r6 = 0x0;
    fn_8012640C();
    r22 = r3 & 0xFFFF;
    if ((u32)r24 == (u32)0x0) {
        r4 = 0x2;
        goto L_801247CC;
    }
    r3 = r24;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x13;
    r6 = 0x0;
    fn_8012640C();
    r21 = r3 & 0xFFFF;
    r3 = 0x0;
    fn_80131574();
    r0 = r3 & 0xFF;
    if ((s32)r21 == (s32)r0) {
        r4 = 0x0;
        goto L_801247CC;
    }
    r3 = 0x1;
    fn_80131574();
    r0 = r3 & 0xFF;
    if ((s32)r21 == (s32)r0) {
        r4 = 0x1;
        goto L_801247CC;
    }
    r3 = 0x2;
    fn_80131574();
    r0 = r3 & 0xFF;
    if ((s32)r21 == (s32)r0) {
        r4 = 0x2;
        goto L_801247CC;
    }
    r4 = -0x1;
L_801247CC: ;
    r0 = (s8)r4;
    if ((s32)r21 < (s32)r0) {
        r0 = r31 & 0xFF;
        if ((u32)r22 <= (u32)r0) goto L_801247E8;
        r4 = 0x1;
    }
    goto L_801247EC;
L_801247E8: ;
    r4 = 0x0;
L_801247EC: ;
    r3 = r23 & 0xFF;
    r0 = r4 & 0xFF;
    if ((u32)r3 != (u32)r0) goto L_8012444C;
L_801247FC: ;
    r0 = (s8)r26;
    if ((u32)r3 < (u32)r0) goto L_80124828;
    r3 = (0x51ec << 16);
    r0 = (u32)((u64)r0 * (u64)r31 >> 32);
    r0 = (u32)r0 >> 3;
    r0 = r0 * 0x19;
    r0 = r31 - r0;
    r0 = r0 & 0xFF;
    if ((s32)r29 != (s32)r0) goto L_8012444C;
L_80124828: ;
    r0 = (s8)r27;
    if ((s32)r29 < (s32)r0) { r3 = r31; return; }
    r0 = (s8)r27;
    if ((s32)r29 == (s32)r0) goto L_80124864;
    r0 = (u32)r31 >> 16;
    r4 = r31 & 0xFFFF;
    r0 = r0 ^ r28;
    r3 = 0x8;
    r0 = r4 ^ r0;
    r0 = r3 ^ r0;
    r0 = __cntlzw(r0);
    r0 = r3 << r0;
    /* srwi. r0, r0, 31 */;
    if ((s32)r29 == (s32)r0) goto L_8012444C;
    r3 = r31;
    return;
L_80124864: ;
    r0 = (u32)r31 >> 16;
    r4 = r31 & 0xFFFF;
    r0 = r0 ^ r28;
    r3 = 0x8;
    r0 = r4 ^ r0;
    r0 = r3 ^ r0;
    r0 = __cntlzw(r0);
    r0 = r3 << r0;
    r0 = (u32)r0 >> 31;
    if ((u32)r0 == (u32)0x1) goto L_8012444C;
    r3 = r31;
    return;
L_80124898: ;
    r3 = (u32)r7 >> 16;
    r0 = r7 & 0xFFFF;
    r30 = (s8)r25;
    r29 = (s8)r26;
    r28 = r3 ^ r0;
    goto L_8012444C;
    return;
}
/* 0x801248C4 | 0xB4 */
void fn_801248C4(void) {
    extern void fn_8012640C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r31 = r3 & 0xFFFF;
    r3 = 0x0;
    r4 = r31;
    r5 = 0x17;
    r6 = 0x1;
    fn_8012640C();
    if ((s32)r3 == (s32)0x0) {
        r4 = r31;
        r3 = 0x0;
        r5 = 0x17;
        r6 = 0x0;
        fn_8012640C();
        r3 = r3 & 0xFFFF;
        return;
    }
    r3 = r30;
    r4 = 0x0;
    r5 = 0xb7;
    r6 = 0x0;
    fn_8012640C();
    r6 = r3 & 0xFF;
    r4 = r31;
    r3 = 0x0;
    r5 = 0x17;
    fn_8012640C();
    r3 = r3 & 0xFFFF;
    return;
}
/* 0x80 | fn_80124978 | generic */
u32 fn_80124978(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_8012640C();
    fn_8012640C();
    fn_801254B4();
    return 0;
}
/* 0x68 | fn_801249F8 | generic */
void fn_801249F8(void) {
    fn_80124A60();
}
/* 0x80125238 | 0xA8 */
void fn_80125238(void) {
    extern void fn_8011B950();
    extern void fn_801254B4();
    extern void fn_8012640C();
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;
    r4 = 0x0;
    r5 = 0xc3;
    r6 = 0x0;
    r7 = 0x0;
    r31 = r3;
    fn_801254B4();
    if ((u32)r31 != (u32)0x0) {
        r3 = r31;
        r4 = 0x0;
        r5 = 0xc5;
        r6 = 0x0;
        r7 = -0x64;
        fn_801254B4();
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0xc6;
    r6 = 0x0;
    r7 = 0x0;
    fn_801254B4();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xc7;
    r6 = 0x0;
    r7 = 0x0;
    fn_801254B4();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xc8;
    r6 = 0x0;
    fn_8012640C();
    r4 = 0x1;
    fn_8011B950();
    return;
}
/* 0x801252E0 | 0x34 */
void fn_801252E0(u32 arg) {
    /* depends on fn_8012640C + fn_8011B950 signatures */
}
/* 0x7C | fn_80125314 | generic */
void fn_80125314(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_801254B4();
    fn_801254B4();
    fn_801254B4();
}
/* 0x80125390 | 0x94 */
void fn_80125390(void) {
    extern void fn_8012640C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
    } else {
        r4 = 0x0;
        r5 = 0x75;
        r6 = 0x0;
        fn_8012640C();
        r31 = r3;
        r3 = r30;
        r4 = 0x0;
        r5 = 0x6f;
        r6 = 0x0;
        fn_8012640C();
        r4 = (u32)r31 >> 16;
        r0 = r31 & 0xFFFF;
        r5 = (u32)r3 >> 16;
        r6 = r3 & 0xFFFF;
        r0 = r4 ^ r0;
        r3 = 0x8;
        r0 = r5 ^ r0;
        r0 = r6 ^ r0;
        r0 = r3 ^ r0;
        r0 = __cntlzw(r0);
        r0 = r3 << r0;
        r3 = (u32)r0 >> 31;
    }
    return;
}
/* 0x48 | fn_80125424 | null_guard_chain */
void fn_80125424(void* ptr, u32 arg2) {
    extern void fn_801254B4();
    if (ptr == NULL) { return; }
    fn_801254B4(ptr, 0, 0x79, 0, arg2);
    fn_8012546C(ptr);
}
/* 0x48 | fn_8012546C | null_guard_chain */
void fn_8012546C(void* ptr) {
    extern u32 fn_8012640C();
    extern void fn_8012795C();
    u8 val;
    if (ptr == NULL) { return; }
    val = fn_8012640C(ptr, 0, 0xC0, 0);
    fn_8012795C(ptr, val);
}
/* 0x8012795C | 0x700 */
void fn_8012795C(void) {
    extern void fn_8011CB98();
    extern void fn_8011CBB0();
    extern void fn_8011CBC8();
    extern void fn_8011CD88();
    extern void fn_8011CDA0();
    extern void fn_8011CDB8();
    extern void fn_8011CDD0();
    extern void fn_8011CDE8();
    extern void fn_8011CE18();
    extern void fn_801254B4();
    extern void fn_8012640C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r5 = 0x6e;
    r6 = 0x0;
    r27 = r4;
    r30 = r3;
    r4 = 0x0;
    fn_8012640C();
    r29 = r3 & 0xFFFF;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x87;
    r6 = 0x0;
    fn_8012640C();
    r31 = r3 & 0xFFFF;
    r3 = r30;
    r7 = r27 & 0xFF;
    r4 = 0x0;
    r5 = 0x7a;
    r6 = 0x0;
    fn_801254B4();
    if ((u32)r29 == (u32)0x12f) {
        r3 = r30;
        r28 = 0x1;
        r4 = 0x0;
        r5 = 0x87;
        r6 = 0x0;
        r7 = 0x1;
        fn_801254B4();
    } else {
        r3 = r30;
        r4 = 0x0;
        r5 = 0x6e;
        r6 = 0x0;
        fn_8012640C();
        r25 = r3 & 0xFFFF;
        r3 = r30;
        r4 = 0x0;
        r5 = 0xbf;
        r6 = 0x0;
        fn_8012640C();
        r26 = r3 & 0xFF;
        r4 = r25;
        r3 = 0x0;
        r5 = 0x3;
        r6 = 0x0;
        fn_8012640C();
        r25 = r3 & 0xFFFF;
        r3 = r30;
        r4 = 0x0;
        r5 = 0x93;
        r6 = 0x0;
        fn_8012640C();
        r28 = r3 & 0xFFFF;
        r3 = r30;
        r4 = 0x0;
        r5 = 0x8d;
        r6 = 0x0;
        fn_8012640C();
        /* clrlslwi r0, r25, 16, 1 */;
        r0 = r0 + r3;
        r5 = r27 & 0xFF;
        r0 = r28 + r0;
        r3 = (0x51ec << 16);
        r0 = r5 * r0;
        r3 = r26;
        r0 = (s32)((s64)r4 * (s64)r0 >> 32);
        r0 = (s32)r0 >> 5;
        r4 = (u32)r0 >> 31;
        r0 = r0 + r4;
        r25 = r5 + r0;
        r25 = r25 + 0xa;
        fn_8011CE18();
        r3 = r30;
        r7 = r25;
        r4 = 0x0;
        r5 = 0x87;
        r6 = 0x0;
        fn_801254B4();
        r28 = r25 & 0xFFFF;
    }
    r3 = r30;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r24 = r3 & 0xFFFF;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xbf;
    r6 = 0x0;
    fn_8012640C();
    r25 = r3 & 0xFF;
    r4 = r24;
    r3 = 0x0;
    r5 = 0x4;
    r6 = 0x0;
    fn_8012640C();
    r24 = r3 & 0xFFFF;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x94;
    r6 = 0x0;
    fn_8012640C();
    r26 = r3 & 0xFFFF;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x8e;
    r6 = 0x0;
    fn_8012640C();
    /* clrlslwi r0, r24, 16, 1 */;
    r0 = r0 + r3;
    r27 = r27 & 0xFF;
    r0 = r26 + r0;
    r3 = (0x51ec << 16);
    r0 = r27 * r0;
    r3 = r25;
    r0 = (s32)((s64)r4 * (s64)r0 >> 32);
    r0 = (s32)r0 >> 5;
    r4 = (u32)r0 >> 31;
    r4 = r0 + r4;
    r24 = r4 + 0x5;
    fn_8011CE18();
    if ((u32)r3 == (u32)0x0) {
        goto L_80127BAC;
    }
    fn_8011CDE8();
    fn_8011CBC8();
    if ((u32)r3 == (u32)0x0) {
        r24 = 0x0;
        goto L_80127BAC;
    }
    fn_8011CBB0();
    r26 = r3;
    r3 = r25;
    fn_8011CB98();
    r4 = r26 & 0xFF;
    r0 = r3 & 0xFF;
    r24 = r24 * r4;
    if ((u32)r3 != (u32)0x0) {
        r24 = (s32)r24 / (s32)r0;
    }
L_80127BAC: ;
    r3 = r30;
    r7 = r24;
    r4 = 0x0;
    r5 = 0x88;
    r6 = 0x0;
    fn_801254B4();
    r3 = r30;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r25 = r3 & 0xFFFF;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xbf;
    r6 = 0x0;
    fn_8012640C();
    r24 = r3 & 0xFF;
    r4 = r25;
    r3 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_8012640C();
    r26 = r3 & 0xFFFF;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x95;
    r6 = 0x0;
    fn_8012640C();
    r25 = r3 & 0xFFFF;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x8f;
    r6 = 0x0;
    fn_8012640C();
    /* clrlslwi r0, r26, 16, 1 */;
    r0 = r0 + r3;
    r0 = r25 + r0;
    r3 = (0x51ec << 16);
    r0 = r27 * r0;
    r3 = r24;
    r0 = (s32)((s64)r4 * (s64)r0 >> 32);
    r0 = (s32)r0 >> 5;
    r4 = (u32)r0 >> 31;
    r4 = r0 + r4;
    r24 = r4 + 0x5;
    fn_8011CE18();
    if ((u32)r3 == (u32)0x0) {
        goto L_80127CB8;
    }
    fn_8011CDD0();
    fn_8011CBC8();
    if ((u32)r3 == (u32)0x0) {
        r24 = 0x0;
        goto L_80127CB8;
    }
    fn_8011CBB0();
    r26 = r3;
    r3 = r25;
    fn_8011CB98();
    r4 = r26 & 0xFF;
    r0 = r3 & 0xFF;
    r24 = r24 * r4;
    if ((u32)r3 != (u32)0x0) {
        r24 = (s32)r24 / (s32)r0;
    }
L_80127CB8: ;
    r3 = r30;
    r7 = r24;
    r4 = 0x0;
    r5 = 0x89;
    r6 = 0x0;
    fn_801254B4();
    r3 = r30;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r25 = r3 & 0xFFFF;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xbf;
    r6 = 0x0;
    fn_8012640C();
    r24 = r3 & 0xFF;
    r4 = r25;
    r3 = 0x0;
    r5 = 0x8;
    r6 = 0x0;
    fn_8012640C();
    r26 = r3 & 0xFFFF;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x98;
    r6 = 0x0;
    fn_8012640C();
    r25 = r3 & 0xFFFF;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x92;
    r6 = 0x0;
    fn_8012640C();
    /* clrlslwi r0, r26, 16, 1 */;
    r0 = r0 + r3;
    r0 = r25 + r0;
    r3 = (0x51ec << 16);
    r0 = r27 * r0;
    r3 = r24;
    r0 = (s32)((s64)r4 * (s64)r0 >> 32);
    r0 = (s32)r0 >> 5;
    r4 = (u32)r0 >> 31;
    r4 = r0 + r4;
    r24 = r4 + 0x5;
    fn_8011CE18();
    if ((u32)r3 == (u32)0x0) {
        goto L_80127DC4;
    }
    fn_8011CD88();
    fn_8011CBC8();
    if ((u32)r3 == (u32)0x0) {
        r24 = 0x0;
        goto L_80127DC4;
    }
    fn_8011CBB0();
    r26 = r3;
    r3 = r25;
    fn_8011CB98();
    r4 = r26 & 0xFF;
    r0 = r3 & 0xFF;
    r24 = r24 * r4;
    if ((u32)r3 != (u32)0x0) {
        r24 = (s32)r24 / (s32)r0;
    }
L_80127DC4: ;
    r3 = r30;
    r7 = r24;
    r4 = 0x0;
    r5 = 0x8c;
    r6 = 0x0;
    fn_801254B4();
    r3 = r30;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r25 = r3 & 0xFFFF;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xbf;
    r6 = 0x0;
    fn_8012640C();
    r24 = r3 & 0xFF;
    r4 = r25;
    r3 = 0x0;
    r5 = 0x6;
    r6 = 0x0;
    fn_8012640C();
    r26 = r3 & 0xFFFF;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x96;
    r6 = 0x0;
    fn_8012640C();
    r25 = r3 & 0xFFFF;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x90;
    r6 = 0x0;
    fn_8012640C();
    /* clrlslwi r0, r26, 16, 1 */;
    r0 = r0 + r3;
    r0 = r25 + r0;
    r3 = (0x51ec << 16);
    r0 = r27 * r0;
    r3 = r24;
    r0 = (s32)((s64)r4 * (s64)r0 >> 32);
    r0 = (s32)r0 >> 5;
    r4 = (u32)r0 >> 31;
    r4 = r0 + r4;
    r24 = r4 + 0x5;
    fn_8011CE18();
    if ((u32)r3 == (u32)0x0) {
        goto L_80127ED0;
    }
    fn_8011CDB8();
    fn_8011CBC8();
    if ((u32)r3 == (u32)0x0) {
        r24 = 0x0;
        goto L_80127ED0;
    }
    fn_8011CBB0();
    r26 = r3;
    r3 = r25;
    fn_8011CB98();
    r4 = r26 & 0xFF;
    r0 = r3 & 0xFF;
    r24 = r24 * r4;
    if ((u32)r3 != (u32)0x0) {
        r24 = (s32)r24 / (s32)r0;
    }
L_80127ED0: ;
    r3 = r30;
    r7 = r24;
    r4 = 0x0;
    r5 = 0x8a;
    r6 = 0x0;
    fn_801254B4();
    r3 = r30;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r25 = r3 & 0xFFFF;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xbf;
    r6 = 0x0;
    fn_8012640C();
    r24 = r3 & 0xFF;
    r4 = r25;
    r3 = 0x0;
    r5 = 0x7;
    r6 = 0x0;
    fn_8012640C();
    r26 = r3 & 0xFFFF;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x97;
    r6 = 0x0;
    fn_8012640C();
    r25 = r3 & 0xFFFF;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x91;
    r6 = 0x0;
    fn_8012640C();
    /* clrlslwi r0, r26, 16, 1 */;
    r0 = r0 + r3;
    r0 = r25 + r0;
    r3 = (0x51ec << 16);
    r0 = r27 * r0;
    r3 = r24;
    r0 = (s32)((s64)r4 * (s64)r0 >> 32);
    r0 = (s32)r0 >> 5;
    r4 = (u32)r0 >> 31;
    r4 = r0 + r4;
    r24 = r4 + 0x5;
    fn_8011CE18();
    if ((u32)r3 == (u32)0x0) {
        goto L_80127FDC;
    }
    fn_8011CDA0();
    fn_8011CBC8();
    if ((u32)r3 == (u32)0x0) {
        r24 = 0x0;
        goto L_80127FDC;
    }
    fn_8011CBB0();
    r27 = r3;
    r3 = r25;
    fn_8011CB98();
    r4 = r27 & 0xFF;
    r0 = r3 & 0xFF;
    r24 = r24 * r4;
    if ((u32)r3 != (u32)0x0) {
        r24 = (s32)r24 / (s32)r0;
    }
L_80127FDC: ;
    r3 = r30;
    r7 = r24;
    r4 = 0x0;
    r5 = 0x8b;
    r6 = 0x0;
    fn_801254B4();
    r3 = r30;
    r4 = 0x0;
    r5 = 0x83;
    r6 = 0x0;
    fn_8012640C();
    r3 = r3 & 0xFFFF;
    if ((u32)r3 == (u32)0x0) {
        if ((u32)r31 != (u32)0x0) return;
    }
    r0 = 0x1;
    if ((u32)r29 != (u32)0x12f) {
        r0 = r28 - r31;
        r0 = r3 + r0;
        r0 = r0 & 0xFFFF;
    }
    r3 = r30;
    r7 = r0 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x83;
    r6 = 0x0;
    fn_801254B4();
    return;
}
/* 0x8012805C | 0x2A4 */
void fn_8012805C(void) {
    extern u8 lbl_8047D020[];
    extern void fn_8011F5FC();
    extern void fn_80123D58();
    extern void fn_80123FBC();
    extern void fn_80128300();
    extern void fn_80128524();
    extern void fn_8012A08C();
    extern void fn_8012A5B0();
    extern void fn_801C40F0();
    extern void fn_801C41C8();
    extern void fn_8025FF9C();
    extern void fn_8026132C();
    u8 sp[0x300];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;
    r25 = r3;
    r31 = r4;
    r26 = r5;
    r24 = r6;
    r27 = r7;
    r28 = r8;
    r29 = r9;
    r30 = r10;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x2;
        return;
    }
    r0 = r31 & 0xFFFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x2;
        return;
    }
    r4 = r25;
    r5 = r31;
    r6 = r24;
    r3 = (u32)sp + 0x1a4;
    r7 = (u32)sp + 0x44;
    fn_80128524();
    if ((s32)r0 < (s32)0) {
        r3 = 0x2;
        return;
    }
    if ((s32)r30 != (s32)0x0) {
        f1 = *(f32*)lbl_8047D020;
        r3 = 0x3;
        fn_801C41C8();
        r3 = 0x1;
        fn_801C40F0();
    }
    r6 = 0x0;
    if ((s32)r6 < (s32)r31) {
        if ((s32)r31 > (s32)0x8) {
            r0 = r3 + 0x7;
            r4 = (u32)sp + 0x44;
            r0 = (u32)r0 >> 3;
            r5 = (u32)sp + 0x1c;
            ctr_fn = (void(*)(void))r0;
            if ((s32)r3 > (s32)0x0) {
                do {
                    r3 = *(u16*)((u8*)r4 + 0x0);
                    r6 = r6 + 0x8;
                    r0 = *(u16*)((u8*)r4 + 0x2);
                    *(u16*)((u8*)r5 + 0x0) = r3;
                    r3 = *(u16*)((u8*)r4 + 0x4);
                    *(u16*)((u8*)r5 + 0x2) = r0;
                    r0 = *(u16*)((u8*)r4 + 0x6);
                    *(u16*)((u8*)r5 + 0x4) = r3;
                    r3 = *(u16*)((u8*)r4 + 0x8);
                    *(u16*)((u8*)r5 + 0x6) = r0;
                    r0 = *(u16*)((u8*)r4 + 0xA);
                    *(u16*)((u8*)r5 + 0x8) = r3;
                    r3 = *(u16*)((u8*)r4 + 0xC);
                    *(u16*)((u8*)r5 + 0xA) = r0;
                    r0 = *(u16*)((u8*)r4 + 0xE);
                    r4 = r4 + 0x10;
                    *(u16*)((u8*)r5 + 0xC) = r3;
                    *(u16*)((u8*)r5 + 0xE) = r0;
                    r5 = r5 + 0x10;
                } while (--ctr != 0);
        }
        }
        r5 = r6 << 1;
        r3 = (u32)sp + 0x44;
        r4 = (u32)sp + 0x1c;
        r0 = r31 - r6;
        r3 = r3 + r5;
        r4 = r4 + r5;
        ctr_fn = (void(*)(void))r0;
        if ((s32)r6 < (s32)r31) {
            do {
                r0 = *(u16*)((u8*)r3 + 0x0);
                r3 = r3 + 0x2;
                *(u16*)((u8*)r4 + 0x0) = r0;
                r4 = r4 + 0x2;
            } while (--ctr != 0);
    }
    }
    r3 = r25;
    r5 = r29;
    r7 = r31;
    r4 = (u32)sp + 0x1a4;
    r6 = (u32)sp + 0x1c;
    r8 = (u32)sp + 0x8;
    fn_8026132C();
    r29 = r3;
    if ((s32)r30 != (s32)0x0) {
        f1 = *(f32*)lbl_8047D020;
        r3 = 0x2;
        fn_801C41C8();
        r3 = 0x1;
        fn_801C40F0();
    }
    if ((s32)r29 == (s32)0x2) {
        r3 = 0x2;
        return;
    }
    if ((s32)r29 == (s32)0x1) {
        r3 = 0x1;
        return;
    }
    if ((s32)r28 != (s32)0x0) {
        r4 = (u32)sp + 0x1a4;
        r3 = 0x0;
        fn_8025FF9C();
    }
    r3 = r25;
    r4 = (u32)sp + 0x1a4;
    fn_8011F5FC();
    r29 = (u32)sp + 0x8;
    r30 = (u32)sp + 0x44;
    r24 = 0x0;
    while ((s32)r24 < (s32)r31) {
        r4 = *(u8*)((u8*)r29 + 0x0);
        if ((u32)r4 != (u32)0xff) {
            r5 = *(u16*)((u8*)r30 + 0x0);
            r3 = r25;
            fn_80123D58();
        }
        r29 = r29 + 0x1;
        r30 = r30 + 0x2;
        r24 = r24 + 0x1;
    }
    r0 = r26 & 0xFFFF;
    if ((s32)r24 == (s32)r31) { r3 = 0x0; return; }
    if ((s32)r24 == (s32)r31) { r3 = 0x0; return; }
    r29 = 0x0;
    while (1) {
    r3 = r27;
    r5 = r29 & 0xFFFF;
    r4 = 0x3;
    fn_8012A5B0();
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((s32)r24 == (s32)r31) break;
    r29 = r29 + 0x1;
    if ((s32)r29 >= (s32)0x6) break;
    }
    r0 = -0x1;
    if ((s32)r29 < (s32)0x6) {
        r0 = (s8)r29;
    }
    r0 = (s8)r0;
    if ((s32)r29 < (s32)0x6) { r3 = 0x0; return; }
    r4 = r25;
    r5 = r26;
    r3 = (u32)sp + 0x6c;
    fn_80128300();
    r3 = r27;
    r4 = (u32)sp + 0x6c;
    fn_8012A08C();
    if ((s32)r28 == (s32)0x0) { r3 = 0x0; return; }
    r4 = (u32)sp + 0x6c;
    r3 = 0x0;
    fn_8025FF9C();
    r3 = 0x0;
    return;
}
/* 0x80128300 | 0x224 */
void fn_80128300(void) {
    extern u8 lbl_80272998[];
    extern u8 lbl_802729A4[];
    extern void fn_800FA280();
    extern void fn_8011DCB4();
    extern void fn_8011DEE4();
    extern void fn_8011DFF0();
    extern void fn_8011E760();
    extern void fn_8011E778();
    extern void fn_8011F5C8();
    extern void fn_8011F5FC();
    extern void fn_80120C6C();
    extern void fn_801252E0();
    extern void fn_8012546C();
    extern void fn_801254B4();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r6 = (u32)lbl_802729A4;
    r7 = (u32)lbl_80272998;
    r10 = (u32)lbl_802729A4;
    r31 = r5;
    r30 = r4;
    r4 = r31;
    r29 = r3;
    r3 = (u32)lbl_80272998;
    r9 = *(u32*)((u8*)r10 + 0x0);
    r28 = *(u32*)((u8*)r3 + 0x0);
    r12 = *(u32*)((u8*)r3 + 0x4);
    r11 = *(u16*)((u8*)r3 + 0x8);
    r3 = 0x0;
    r8 = *(u32*)((u8*)r10 + 0x4);
    r7 = *(u32*)((u8*)r10 + 0x8);
    r6 = *(u32*)((u8*)r10 + 0xC);
    r5 = *(u32*)((u8*)r10 + 0x10);
    r0 = *(u32*)((u8*)r10 + 0x14);
    *(u16*)(sp + 0x10) = r11;
    *(u32*)(sp + 0x28) = r0;
    fn_80120C6C();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
    } else {
    r3 = r30;
    fn_8011F5C8();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
    } else {
        fn_8011E778();
    }
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
    } else {
    r3 = r29;
    r4 = r30;
    fn_8011F5FC();
    r3 = r29;
    r4 = r31;
    fn_8011DFF0();
    r3 = r29;
    fn_8011F5C8();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
    } else {
        fn_8011E778();
    }
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
    } else {
    fn_8011E760();
    fn_800FA280();
    r0 = r3;
    r3 = r29;
    r4 = r0;
    fn_8011DEE4();
    r3 = r29;
    r4 = 0x0;
    fn_8011DCB4();
    r3 = r29;
    r4 = 0x0;
    r5 = 0xbb;
    r6 = 0x0;
    r7 = 0x0;
    fn_801254B4();
    r3 = r29;
    r4 = 0x0;
    r5 = 0xbe;
    r6 = 0x0;
    r7 = 0x0;
    fn_801254B4();
    r30 = 0x0;
    r28 = (u32)sp + 0x8;
    do {
        r5 = *(u16*)((u8*)r28 + 0x0);
        r3 = r29;
        r4 = 0x0;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r30 = r30 + 0x1;
        r28 = r28 + 0x2;
    } while ((s32)r30 < (s32)0x5);
    r30 = 0x0;
    r28 = (u32)sp + 0x14;
    do {
        r5 = *(u16*)((u8*)r28 + 0x0);
        r3 = r29;
        r4 = 0x0;
        r6 = 0x0;
        r7 = 0x0;
        fn_801254B4();
        r30 = r30 + 0x1;
        r28 = r28 + 0x2;
    } while ((s32)r30 < (s32)0xc);
    r3 = r29;
    r4 = 0x0;
    r5 = 0xaf;
    r6 = 0x0;
    r7 = 0x0;
    fn_801254B4();
    r3 = r29;
    fn_801252E0();
    r3 = r29;
    r4 = 0x0;
    r5 = 0xbc;
    r6 = 0x0;
    r7 = 0x0;
    fn_801254B4();
    r3 = r29;
    fn_8012546C();
    r3 = 0x1;
    }
    }
    }
    r31 = *(u32*)(sp + 0x3C);
    r30 = *(u32*)(sp + 0x38);
    r29 = *(u32*)(sp + 0x34);
    r28 = *(u32*)(sp + 0x30);
    return;
}
/* 0x80128524 | 0x1A4 */
void fn_80128524(void) {
    extern void fn_800F9EE4();
    extern void fn_800FA280();
    extern void fn_8011DEE4();
    extern void fn_8011DFF0();
    extern void fn_8011E760();
    extern void fn_8011E778();
    extern void fn_8011F4A8();
    extern void fn_8011F4F0();
    extern void fn_8011F5C8();
    extern void fn_8011F5FC();
    extern void fn_80120C6C();
    extern void fn_80123110();
    extern void fn_801236F8();
    extern void fn_80123B5C();
    extern void fn_80123FBC();
    extern void fn_8012546C();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r24 = r4;
    r27 = r3;
    r28 = r5;
    r29 = r6;
    r30 = r7;
    r3 = r24;
    r31 = 0x0;
    r26 = 0x0;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = -0x1;
        return;
    }
    r4 = r28;
    r3 = 0x0;
    fn_80120C6C();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = -0x1;
        return;
    }
    r3 = r24;
    fn_8011F5C8();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) {
        r25 = 0x0;
    } else {
        fn_8011E778();
        r25 = r3;
    }
    if ((u32)r25 == (u32)0x0) {
        r3 = -0x1;
        return;
    }
    r3 = r27;
    r4 = r24;
    fn_8011F5FC();
    r3 = r27;
    fn_8011F4F0();
    r0 = r3;
    r3 = r25;
    r25 = r0;
    fn_8011E760();
    fn_800FA280();
    r0 = r3;
    r3 = r25;
    r4 = r0;
    fn_800F9EE4();
    if ((s32)r3 == (s32)0x0) {
        r3 = r28;
        fn_8011E778();
        fn_8011E760();
        fn_800FA280();
        r0 = r3;
        r3 = r27;
        r4 = r0;
        fn_8011DEE4();
    }
    r3 = r27;
    r4 = r28;
    fn_8011DFF0();
    r3 = r27;
    fn_8012546C();
    r0 = *(u8*)((u8*)r29 + 0x0);
    if ((u32)r0 == (u32)0x6) {
        r3 = r27;
        r4 = 0x0;
        r5 = 0x0;
        fn_80123110();
    }
    r3 = r27;
    fn_8011F4A8();
    r0 = 0x0;
    r28 = r3;
    *(u8*)(sp + 0x8) = r0;
    while (1) {
    r3 = r27;
    r4 = r28;
    r5 = (u32)sp + 0x8;
    fn_801236F8();
    r0 = r3 & 0xFFFF;
    r25 = r3;
    if ((s32)r31 == (s32)0x14) break;
    r3 = r27;
    r4 = r25;
    fn_80123B5C();
    r0 = (s8)r3;
    if ((s32)r0 == (s32)-0x1) {
        r31 = r31 + 0x1;
        *(u16*)(r30 + r26) = r25;
        r26 = r26 + 0x2;
        if ((s32)r31 >= (s32)0x14) { r3 = r31; return; }
    }
    r3 = *(u8*)(sp + 0x8);
    r0 = r3 + 0x1;
    *(u8*)(sp + 0x8) = r0;
    }
    r3 = r31;
    return;
}
/* 0x801286C8 | 0x39C */
void fn_801286C8(void) {
    extern void fn_8011E36C();
    extern void fn_8011E3B4();
    extern void fn_8011E3FC();
    extern void fn_8011E778();
    extern void fn_8011ECC0();
    extern void fn_8011EE58();
    extern void fn_8011F104();
    extern void fn_8011F130();
    extern void fn_8011F4A8();
    extern void fn_8011F5B0();
    extern void fn_8011F5C8();
    extern void fn_80123FBC();
    extern void fn_8012A5B0();
    extern u8 jumptable_80363468[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r23 = 0x0;
    r31 = 0x0;
    fn_8011F5C8();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) {
        r25 = 0x0;
    } else {
        fn_8011E778();
        r25 = r3;
    }
    if ((u32)r25 == (u32)0x0) {
        r3 = (0x1 << 16);
        return;
    }
    r24 = 0x0;
    do {
        r3 = r25;
        r4 = r24 & 0xFFFF;
        fn_8011E3FC();
        r26 = r3;
        r3 = r25;
        r4 = r24 & 0xFFFF;
        fn_8011E3B4();
        r0 = r26 & 0xFF;
        r27 = r3;
        if ((u32)r0 <= (u32)0xf) {
            r3 = (u32)jumptable_80363468;
            r0 = r0 << 2;
            r3 = (u32)jumptable_80363468;
            r0 = *(u32*)(r3 + r0);
            ctr_fn = (void(*)(void))r0;
            /* indirect jump via ctr */;
            r3 = r28;
            fn_8011EE58();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 >= (u32)0xdc) {
                r3 = r25;
                r4 = r24 & 0xFFFF;
                fn_8011E36C();
                *(u8*)((u8*)r30 + 0x0) = r26;
                r23 = r3;
                *(u16*)((u8*)r30 + 0x2) = r27;
            }
        }
        goto L_80128A3C;
        r3 = r28;
        fn_8011EE58();
        goto L_80128A3C;
        r3 = r28;
        fn_8011EE58();
        goto L_80128A3C;
        r3 = r28;
        fn_8011F4A8();
        r3 = r3 & 0xFF;
        r0 = r27 & 0xFFFF;
        if ((s32)r3 >= (s32)r0) {
            r3 = r25;
            r4 = r24 & 0xFFFF;
            fn_8011E36C();
            *(u8*)((u8*)r30 + 0x0) = r26;
            r23 = r3;
            *(u16*)((u8*)r30 + 0x2) = r27;
        }
        goto L_80128A3C;
        r3 = r28;
        fn_8011F4A8();
        r3 = r3 & 0xFF;
        r0 = r27 & 0xFFFF;
        if ((s32)r3 >= (s32)r0) {
            r3 = r28;
            fn_8011F130();
            r22 = r3 & 0xFFFF;
            r3 = r28;
            fn_8011F104();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 < (u32)r22) {
                r3 = r25;
                r4 = r24 & 0xFFFF;
                fn_8011E36C();
                *(u8*)((u8*)r30 + 0x0) = r26;
                r23 = r3;
                *(u16*)((u8*)r30 + 0x2) = r27;
        }
        }
        goto L_80128A3C;
        r3 = r28;
        fn_8011F4A8();
        r3 = r3 & 0xFF;
        r0 = r27 & 0xFFFF;
        if ((s32)r3 >= (s32)r0) {
            r3 = r28;
            fn_8011F130();
            r22 = r3 & 0xFFFF;
            r3 = r28;
            fn_8011F104();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 == (u32)r22) {
                r3 = r25;
                r4 = r24 & 0xFFFF;
                fn_8011E36C();
                *(u8*)((u8*)r30 + 0x0) = r26;
                r23 = r3;
                *(u16*)((u8*)r30 + 0x2) = r27;
        }
        }
        goto L_80128A3C;
        r3 = r28;
        fn_8011F4A8();
        r3 = r3 & 0xFF;
        r0 = r27 & 0xFFFF;
        if ((s32)r3 >= (s32)r0) {
            r3 = r28;
            fn_8011F130();
            r22 = r3 & 0xFFFF;
            r3 = r28;
            fn_8011F104();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 > (u32)r22) {
                r3 = r25;
                r4 = r24 & 0xFFFF;
                fn_8011E36C();
                *(u8*)((u8*)r30 + 0x0) = r26;
                r23 = r3;
                *(u16*)((u8*)r30 + 0x2) = r27;
        }
        }
        goto L_80128A3C;
        r3 = r28;
        fn_8011F4A8();
        r3 = r3 & 0xFF;
        r0 = r27 & 0xFFFF;
        if ((s32)r3 >= (s32)r0) {
            r3 = r28;
            fn_8011F5B0();
            r4 = (0xcccd << 16);
            r3 = (u32)r3 >> 16;
            r0 = (u32)((u64)r0 * (u64)r3 >> 32);
            r0 = (u32)r0 >> 3;
            r0 = r0 * 0xa;
            r0 = r3 - r0;
            if ((u32)r0 < (u32)0x5) {
                r3 = r25;
                r4 = r24 & 0xFFFF;
                fn_8011E36C();
                *(u8*)((u8*)r30 + 0x0) = r26;
                r23 = r3;
                *(u16*)((u8*)r30 + 0x2) = r27;
        }
        }
        goto L_80128A3C;
        r3 = r28;
        fn_8011F4A8();
        r3 = r3 & 0xFF;
        r0 = r27 & 0xFFFF;
        if ((s32)r3 >= (s32)r0) {
            r3 = r28;
            fn_8011F5B0();
            r4 = (0xcccd << 16);
            r3 = (u32)r3 >> 16;
            r0 = (u32)((u64)r0 * (u64)r3 >> 32);
            r0 = (u32)r0 >> 3;
            r0 = r0 * 0xa;
            r0 = r3 - r0;
            if ((u32)r0 >= (u32)0x5) {
                r3 = r25;
                r4 = r24 & 0xFFFF;
                fn_8011E36C();
                *(u8*)((u8*)r30 + 0x0) = r26;
                r23 = r3;
                *(u16*)((u8*)r30 + 0x2) = r27;
        }
        }
        goto L_80128A3C;
        r3 = r28;
        fn_8011F4A8();
        r3 = r3 & 0xFF;
        r0 = r27 & 0xFFFF;
        if ((s32)r3 >= (s32)r0) {
            r22 = 0x0;
        L_801289B8: ;
            r5 = r22 & 0xFFFF;
            r3 = 0x0;
            r4 = 0x3;
            fn_8012A5B0();
            fn_80123FBC();
            r0 = r3 & 0xFF;
            if ((s32)r3 == (s32)r0) goto L_801289E0;
            r22 = r22 + 0x1;
            if ((s32)r22 < (s32)0x6) goto L_801289B8;
        L_801289E0: ;
            r0 = -0x1;
            if ((s32)r22 < (s32)0x6) {
                r0 = (s8)r22;
            }
            r0 = (s8)r0;
            if ((s32)r22 >= (s32)0x6) {
                r3 = r25;
                r4 = r24 & 0xFFFF;
                fn_8011E36C();
                r31 = r3;
        }
        }
        goto L_80128A3C;
        r3 = r28;
        fn_8011ECC0();
        r3 = r3 & 0xFF;
        r0 = r27 & 0xFFFF;
        if ((s32)r3 >= (s32)r0) {
            r3 = r25;
            r4 = r24 & 0xFFFF;
            fn_8011E36C();
            *(u8*)((u8*)r30 + 0x0) = r26;
            r23 = r3;
            *(u16*)((u8*)r30 + 0x2) = r27;
        }
    L_80128A3C: ;
        r24 = r24 + 0x1;
    } while ((s32)r24 < (s32)0x5);
    *(u16*)((u8*)r29 + 0x0) = r31;
    r3 = r23;
    return;
}
/* 0x80128A64 | 0x25C */
void fn_80128A64(void) {
    extern void fn_8011E36C();
    extern void fn_8011E3B4();
    extern void fn_8011E3FC();
    extern void fn_8011E778();
    extern void fn_8011F1A0();
    extern void fn_8011F5C8();
    extern void fn_801230E0();
    extern void fn_80123FBC();
    extern void fn_801286C8();
    extern void fn_80143F6C();
    extern void fn_801440A0();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = 0x0;
    r28 = r3;
    r24 = r4;
    r29 = r5;
    r30 = r6;
    r31 = r7;
    *(u16*)(sp + 0x8) = r0;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = (0x1 << 16);
        return;
    }
    r3 = r28;
    fn_801230E0();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
    } else {
        fn_801440A0();
        fn_80143F6C();
    }
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x26) {
        r0 = *(u16*)(sp + 0x8);
        r3 = 0x0;
        *(u16*)((u8*)r30 + 0x0) = r0;
        return;
    }
    if ((s32)r24 != (s32)0x1) {
        if ((s32)r24 < (s32)0x1) {
            if ((s32)r24 >= (s32)0x0) goto L_80128B08;
            goto L_80128C9C;
        }
        if ((s32)r24 >= (s32)0x3) goto L_80128C9C;
        goto L_80128BC4;
    L_80128B08: ;
        r3 = r28;
        r5 = r31;
        r4 = (u32)sp + 0x8;
        fn_801286C8();
        goto L_80128CA4;
    }
    r3 = r28;
    r26 = 0x0;
    fn_8011F5C8();
    r0 = r3 & 0xFFFF;
    if ((s32)r24 == (s32)0x3) {
        r24 = 0x0;
    } else {
        fn_8011E778();
        r24 = r3;
    }
    if ((u32)r24 == (u32)0x0) {
        r3 = (0x1 << 16);
        goto L_80128BBC;
    }
    r25 = 0x0;
    do {
        r3 = r24;
        r4 = r25 & 0xFFFF;
        fn_8011E3FC();
        r28 = r3;
        r3 = r24;
        r4 = r25 & 0xFFFF;
        fn_8011E3B4();
        r0 = r28 & 0xFF;
        r27 = r3;
        if ((s32)r0 != (s32)0x7) {
            goto L_80128BB0;
        }
        r3 = r27 & 0xFFFF;
        r0 = r29 & 0xFFFF;
        if ((u32)r3 == (u32)r0) {
            r3 = r24;
            r4 = r25 & 0xFFFF;
            fn_8011E36C();
            *(u8*)((u8*)r31 + 0x0) = r28;
            r26 = r3;
            *(u16*)((u8*)r31 + 0x2) = r27;
        }
    L_80128BB0: ;
        r25 = r25 + 0x1;
    } while ((s32)r25 < (s32)0x5);
L_80128BBC: ;
    r3 = r26;
    goto L_80128CA4;
L_80128BC4: ;
    r3 = r28;
    r25 = 0x0;
    fn_8011F5C8();
    r0 = r3 & 0xFFFF;
    if ((s32)r25 == (s32)0x5) {
        r24 = 0x0;
    } else {
        fn_8011E778();
        r24 = r3;
    }
    if ((u32)r24 == (u32)0x0) {
        r3 = (0x1 << 16);
        goto L_80128C94;
    }
    r26 = 0x0;
    do {
        r3 = r24;
        r4 = r26 & 0xFFFF;
        fn_8011E3FC();
        r27 = r3;
        r3 = r24;
        r4 = r26 & 0xFFFF;
        fn_8011E3B4();
        r0 = r27 & 0xFF;
        r29 = r3;
        if ((s32)r0 != (s32)0x6) {
            if ((s32)r0 < (s32)0x6) {
                if ((s32)r0 >= (s32)0x5) goto L_80128C3C;
            }
            goto L_80128C88;
        L_80128C3C: ;
            r3 = r24;
            r4 = r26 & 0xFFFF;
            fn_8011E36C();
            *(u8*)((u8*)r31 + 0x0) = r27;
            r25 = r3;
            *(u16*)((u8*)r31 + 0x2) = r29;
            goto L_80128C88;
        }
        r3 = r28;
        fn_8011F1A0();
        r3 = r3 & 0xFFFF;
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)r3) {
            r3 = r24;
            r4 = r26 & 0xFFFF;
            fn_8011E36C();
            *(u8*)((u8*)r31 + 0x0) = r27;
            r25 = r3;
            *(u16*)((u8*)r31 + 0x2) = r29;
        }
    L_80128C88: ;
        r26 = r26 + 0x1;
    } while ((s32)r26 < (s32)0x5);
L_80128C94: ;
    r3 = r25;
    goto L_80128CA4;
L_80128C9C: ;
    r3 = (0x1 << 16);
L_80128CA4: ;
    r0 = *(u16*)(sp + 0x8);
    *(u16*)((u8*)r30 + 0x0) = r0;
    return;
}
/* 0x80128CC0 | 0x1C */
void* fn_80128CC0(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x1C45C;
}
/* 0x80128CDC | 0x1C */
void* fn_80128CDC(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x1B014;
}
/* 0x80128CF8 | 0x1C */
void* fn_80128CF8(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0xE3E8;
}
/* 0x80128D14 | 0x1C */
void* fn_80128D14(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x9A1C;
}
/* 0x80128D30 | 0x1C */
void* fn_80128D30(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x82A8;
}
/* 0x80128D4C | 0x1C */
void* fn_80128D4C(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x8168;
}
/* 0x80128D68 | 24 bytes | nc_addi_ptr */
void* fn_80128D68(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x7D20;
}
/* 0x80128D80 | 0x1C */
void* fn_80128D80(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x1BE5C;
}
/* 0x80128D9C | 0x1C */
void* fn_80128D9C(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x1BDDC;
}
/* 0x80128DB8 | 0x1C */
void* fn_80128DB8(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x1BDBC;
}
/* 0x80128DD4 | 24 bytes | nc_addi_ptr */
void* fn_80128DD4(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0xB88;
}
/* 0x80128DEC | 24 bytes | nc_addi_ptr */
void* fn_80128DEC(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x70;
}
/* 0x80128E04 | 16 bytes | nc_bnelr */
u32 fn_80128E04(void* ptr) {
    if (ptr != NULL) { return (u32)ptr; }
    return 0;
}
/* 0x80128E14 | 0x10 */
void fn_80128E14(void* ptr) {
    if (ptr == NULL) { return; }
    lbl_8047ADB8 = (u32)ptr;
}
/* 0x80128E2C | 0xC */
extern u8 lbl_80408400[];
void* fn_80128E2C(void) {
    return (void*)lbl_80408400;
}
/* 0x80128E38 | 0x25C */
void fn_80128E38(void) {
    extern u8 lbl_8047D028[];
    extern void fn_800057A0();
    extern void fn_800F9D04();
    extern void fn_80128DEC();
    extern void fn_80128E04();
    extern void fn_80128E24();
    extern void fn_80129094();
    extern void fn_80129A78();
    extern void fn_8012A1A4();
    extern void fn_8012A450();
    extern void fn_8012AC08();
    extern void fn_80130A88();
    extern void fn_80130BB0();
    extern void fn_80135030();
    extern void fn_8013528C();
    extern void fn_8025FF9C();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r30 = r3;
    r29 = r4;
    fn_80129094();
    r3 = r30;
    if ((u32)r30 == (u32)0x0) {
        fn_80128E24();
        if ((u32)r3 == (u32)0x0) {
            r31 = 0x0;
            goto L_80128E84;
        }
    }
    fn_80128E04();
    r31 = r3;
L_80128E84: ;
    fn_800057A0();
    if ((s32)r3 != (s32)0x1) {
        if ((s32)r3 < (s32)0x1) {
            if ((s32)r3 >= (s32)0x0) goto L_80128EAC;
            goto L_80128EFC;
        }
        if ((s32)r3 >= (s32)0x3) goto L_80128EFC;
        goto L_80128EE4;
    L_80128EAC: ;
        r3 = r31;
        r4 = 0xb;
        r5 = 0x3;
        r6 = 0x1;
        r7 = 0x1;
        fn_8013528C();
        goto L_80128EFC;
    }
    r3 = r31;
    r4 = 0xb;
    r5 = 0x3;
    r6 = 0x2;
    r7 = 0x2;
    fn_8013528C();
    goto L_80128EFC;
L_80128EE4: ;
    r3 = r31;
    r4 = 0xb;
    r5 = 0x3;
    r6 = 0x3;
    r7 = 0x8;
    fn_8013528C();
L_80128EFC: ;
    r3 = r30;
    if ((u32)r30 == (u32)0x0) {
        fn_80128E24();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            goto L_80128F20;
        }
    }
    fn_80128DEC();
L_80128F20: ;
    r30 = r3;
    if ((u32)r29 == (u32)0x0) {
        r3 = (u32)sp + 0x8;
        r4 = (u32)lbl_8047D028;
        fn_800F9D04();
        r29 = (u32)sp + 0x8;
    }
    r3 = r30;
    r4 = r29;
    r5 = 0x0;
    fn_8012A1A4();
    r3 = r31;
    r4 = 0x5;
    r5 = 0x2;
    fn_80135030();
    r3 = r31;
    r4 = 0x7;
    r5 = 0x1;
    fn_80135030();
    r3 = r31;
    r4 = 0x8;
    r5 = 0x1;
    fn_80135030();
    r3 = r30;
    r4 = 0x0;
    fn_80130BB0();
    r3 = r30;
    r4 = 0x0;
    fn_80130A88();
    r3 = r30;
    r4 = 0x0;
    fn_8012AC08();
    r0 = r3;
    r3 = 0x0;
    r4 = r0;
    fn_8025FF9C();
    r3 = r30;
    r4 = 0x1;
    fn_8012AC08();
    r4 = r3;
    r3 = 0x0;
    fn_8025FF9C();
    r3 = r30;
    r4 = 0xc;
    r5 = 0x2710;
    fn_8012A450();
    r3 = r30;
    r4 = 0x16;
    r5 = 0x2;
    r6 = -0x1;
    fn_80129A78();
    r3 = r30;
    r4 = 0xd;
    r5 = 0x5;
    r6 = -0x1;
    fn_80129A78();
    r3 = r30;
    r4 = 0xe;
    r5 = 0x2;
    r6 = -0x1;
    fn_80129A78();
    r3 = r30;
    r4 = 0xf;
    r5 = 0x2;
    r6 = -0x1;
    fn_80129A78();
    r3 = r30;
    r4 = 0x10;
    r5 = 0x2;
    r6 = -0x1;
    fn_80129A78();
    r3 = r30;
    r4 = 0x12;
    r5 = 0x2;
    r6 = -0x1;
    fn_80129A78();
    r3 = r30;
    r4 = 0x11;
    r5 = 0x2;
    r6 = -0x1;
    fn_80129A78();
    r3 = r30;
    r4 = 0x17;
    r5 = 0x2;
    r6 = -0x1;
    fn_80129A78();
    return;
}
/* 0x80129094 | 0x1EC */
void fn_80129094(void) {
    extern void fn_8006B6B4();
    extern void fn_80083CBC();
    extern void fn_80128CC0();
    extern void fn_80128CDC();
    extern void fn_80128CF8();
    extern void fn_80128D14();
    extern void fn_80128D30();
    extern void fn_80128D4C();
    extern void fn_80128D68();
    extern void fn_80128DD4();
    extern void fn_80128DEC();
    extern void fn_80128E04();
    extern void fn_80128E24();
    extern void fn_8012A248();
    extern void fn_80134F88();
    extern void fn_80135338();
    extern void fn_801908D4();
    extern void fn_801D1FA8();
    extern void fn_801ED310();
    extern void fn_801EF128();
    extern void fn_80260070();
    extern void fn_80265F4C();
    u32 r3 = 0;
    u32 r31 = 0;
    r31 = r3;
    if ((u32)r3 == (u32)0x0) {
        fn_80128E24();
        if ((u32)r3 != (u32)0x0) {
            fn_80128E04();
        }
    } else {
        fn_80128E04();
    }
    fn_80135338();
    r3 = r31;
    if ((u32)r31 == (u32)0x0) {
        fn_80128E24();
        if ((u32)r3 != (u32)0x0) {
            fn_80128DEC();
        }
    } else {
        fn_80128DEC();
    }
    fn_8012A248();
    r3 = r31;
    if ((u32)r31 == (u32)0x0) {
        fn_80128E24();
        if ((u32)r3 != (u32)0x0) {
            fn_80128DD4();
        }
    } else {
        fn_80128DD4();
    }
    fn_80134F88();
    if ((u32)r31 != (u32)0x0) {
        fn_80128E24();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
        }
        if ((u32)r31 == (u32)r3) {
            r3 = 0x1;
            fn_801908D4();
            r3 = 0x2;
            fn_801908D4();
            r3 = 0x3;
            fn_801908D4();
        }
    } else {
        r3 = 0x1;
        fn_801908D4();
        r3 = 0x2;
        fn_801908D4();
        r3 = 0x3;
        fn_801908D4();
    }
    r3 = r31;
    if ((u32)r31 == (u32)0x0) {
        fn_80128E24();
        if ((u32)r3 != (u32)0x0) {
            fn_80128D68();
        }
    } else {
        fn_80128D68();
    }
    fn_801D1FA8();
    r3 = r31;
    if ((u32)r31 == (u32)0x0) {
        fn_80128E24();
        if ((u32)r3 != (u32)0x0) {
            fn_80128D4C();
        }
    } else {
        fn_80128D4C();
    }
    fn_801ED310();
    r3 = r31;
    if ((u32)r31 == (u32)0x0) {
        fn_80128E24();
        if ((u32)r3 != (u32)0x0) {
            fn_80128CF8();
        }
    } else {
        fn_80128CF8();
    }
    fn_8006B6B4();
    r3 = r31;
    if ((u32)r31 == (u32)0x0) {
        fn_80128E24();
        if ((u32)r3 != (u32)0x0) {
            fn_80128D30();
        }
    } else {
        fn_80128D30();
    }
    fn_80260070();
    r3 = r31;
    if ((u32)r31 == (u32)0x0) {
        fn_80128E24();
        if ((u32)r3 != (u32)0x0) {
            fn_80128D14();
        }
    } else {
        fn_80128D14();
    }
    fn_80083CBC();
    r3 = r31;
    if ((u32)r31 == (u32)0x0) {
        fn_80128E24();
        if ((u32)r3 != (u32)0x0) {
            fn_80128CDC();
        }
    } else {
        fn_80128CDC();
    }
    fn_801EF128();
    r3 = r31;
    if ((u32)r31 == (u32)0x0) {
        fn_80128E24();
        if ((u32)r3 != (u32)0x0) {
            fn_80128CC0();
        }
    } else {
        fn_80128CC0();
    }
    fn_80265F4C();
    return;
}
/* 0x80129280 | 0x104 */
void fn_80129280(void) {
    extern void fn_80128CC0();
    extern void fn_80128CDC();
    extern void fn_80128CF8();
    extern void fn_80128D14();
    extern void fn_80128D30();
    extern void fn_80128D4C();
    extern void fn_80128D68();
    extern void fn_80128D80();
    extern void fn_80128D9C();
    extern void fn_80128DB8();
    extern void fn_80128DD4();
    extern void fn_80128DEC();
    extern void fn_80128E04();
    extern void fn_80128E24();
    extern u8 jumptable_803634A8[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    r0 = r4 & 0xFFFF;
    r31 = r4;
    if ((u32)r0 >= (u32)0x11) {
        r3 = 0x0;
        return;
    }
    if ((u32)r3 == (u32)0x0) {
        fn_80128E24();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            return;
    }
    }
    r0 = r31 & 0xFFFF;
    if ((u32)r0 <= (u32)0x10) {
        r4 = (u32)jumptable_803634A8;
        r0 = r0 << 2;
        r4 = (u32)jumptable_803634A8;
        r0 = *(u32*)(r4 + r0);
        ctr_fn = (void(*)(void))r0;
        /* indirect jump via ctr */;
        return;
        fn_80128E04();
        return;
        fn_80128DEC();
        return;
        fn_80128DD4();
        return;
        fn_80128DB8();
        return;
        fn_80128D9C();
        return;
        fn_80128D80();
        return;
        r3 = 0x8;
        return;
        r3 = 0x20;
        return;
        r3 = 0x180;
        return;
        fn_80128D68();
        return;
        fn_80128D4C();
        return;
        fn_80128D30();
        return;
        fn_80128D14();
        return;
        fn_80128CF8();
        return;
        fn_80128CDC();
        return;
        fn_80128CC0();
        return;
    }
    r3 = 0x0;
    return;
}
/* 0x78 | fn_80129384 | multi_call_guarded */
void fn_80129384(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_8012A5B0();
    /* fn_8012A450() */
    fn_8012A5B0();
    fn_8012A450();
}
/* 0x78 | fn_801293FC | multi_call_guarded */
void fn_801293FC(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_8012A5B0();
    /* fn_8012A450() */
    fn_8012A5B0();
    fn_8012A450();
}
/* 0x50 | fn_80129474 | call_sequence */
void fn_80129474(void) {
    fn_8012A5B0();
    fn_8012A450();
}
/* 0x50 | fn_801294C4 | call_sequence */
void fn_801294C4(void) {
    fn_8012A5B0();
    fn_8012A450();
}
/* 0x80129514 | 0x88 */
void fn_80129514(void) {
    extern void fn_8012A5B0();
    extern void fn_80140A9C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    /* addic. r0, (u32)sp, 0x8 */;
    r31 = r5;
    r30 = r4;
    if ((s32)r0 != (s32)0) {
        r0 = 0xa;
        *(u16*)(sp + 0x8) = r0;
    }
    r4 = 0xa;
    r5 = 0x0;
    fn_8012A5B0();
    if ((s32)r0 != (s32)0) {
        r3 = *(u16*)(sp + 0x8);
        r0 = r30 & 0xFFFF;
        if ((u32)r0 < (u32)r3) {
            r0 = r31 & 0xFFFF;
            if ((u32)r0 < (u32)r3) {
                /* clrlslwi r3, r30, 16, 2 */;
                /* clrlslwi r0, r31, 16, 2 */;
                r3 = r4 + r3;
                r4 = r4 + r0;
                fn_80140A9C();
    }
    }
    }
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    return;
}
/* 0x8012959C | 0xB4 */
void fn_8012959C(void) {
    extern void fn_8012A5B0();
    extern void fn_80140ACC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r7 = 0x0;
    /* addic. r0, (u32)sp, 0xc */;
    r31 = r6;
    r30 = r5;
    r29 = r4;
    *(u16*)(sp + 0xC) = r7;
    *(u16*)(sp + 0xA) = r7;
    if ((s32)r0 != (s32)0) {
        r0 = 0xa;
        *(u16*)(sp + 0xC) = r0;
    }
    /* addic. r0, (u32)sp, 0xa */;
    if ((s32)r0 != (s32)0) {
        r0 = 0x1;
        *(u16*)(sp + 0xA) = r0;
    }
    /* addic. r0, (u32)sp, 0x8 */;
    if ((s32)r0 != (s32)0) {
        r0 = 0x0;
        *(u8*)(sp + 0x8) = r0;
    }
    r4 = 0xa;
    r5 = 0x0;
    fn_8012A5B0();
    if ((u32)r3 == (u32)0x0) {
        r3 = -0x1;
    } else {
        r4 = *(u16*)(sp + 0xC);
        r5 = r29;
        r8 = *(u16*)(sp + 0xA);
        r6 = r30;
        r9 = *(u8*)(sp + 0x8);
        r7 = r31;
        fn_80140ACC();
    }
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}
/* 0x80129650 | 0xC8 */
void fn_80129650(void) {
    extern void fn_8012A5B0();
    extern void fn_80141308();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r7 = 0x0;
    /* addic. r0, (u32)sp, 0xc */;
    r31 = r6;
    r30 = r5;
    r29 = r4;
    *(u16*)(sp + 0xC) = r7;
    *(u16*)(sp + 0xA) = r7;
    if ((s32)r0 != (s32)0) {
        r0 = 0xa;
        *(u16*)(sp + 0xC) = r0;
    }
    /* addic. r0, (u32)sp, 0xa */;
    if ((s32)r0 != (s32)0) {
        r0 = 0x1;
        *(u16*)(sp + 0xA) = r0;
    }
    /* addic. r0, (u32)sp, 0x9 */;
    if ((s32)r0 != (s32)0) {
        r0 = 0x0;
        *(u8*)(sp + 0x9) = r0;
    }
    /* addic. r0, (u32)sp, 0x8 */;
    if ((s32)r0 != (s32)0) {
        r0 = 0x1;
        *(u8*)(sp + 0x8) = r0;
    }
    r4 = 0xa;
    r5 = 0x0;
    fn_8012A5B0();
    if ((u32)r3 == (u32)0x0) {
        r3 = -0x1;
    } else {
        r4 = *(u16*)(sp + 0xC);
        r5 = r29;
        r8 = *(u16*)(sp + 0xA);
        r6 = r30;
        r9 = *(u8*)(sp + 0x9);
        r7 = r31;
        r10 = *(u8*)(sp + 0x8);
        fn_80141308();
    }
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}
/* 0x80129718 | 0xC0 */
void fn_80129718(void) {
    extern void fn_8012A5B0();
    extern void fn_80142368();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r5 = 0x0;
    /* addic. r0, (u32)sp, 0xa */;
    r30 = r4;
    *(u16*)(sp + 0xA) = r5;
    *(u16*)(sp + 0x8) = r5;
    if ((s32)r0 != (s32)0) {
        r0 = 0xa;
        *(u16*)(sp + 0xA) = r0;
    }
    /* addic. r0, (u32)sp, 0x8 */;
    if ((s32)r0 != (s32)0) {
        r0 = 0x1;
        *(u16*)(sp + 0x8) = r0;
    }
    r4 = 0xa;
    r5 = 0x0;
    fn_8012A5B0();
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
    } else {
        r4 = *(u16*)(sp + 0xA);
        r5 = r30;
        r7 = *(u16*)(sp + 0x8);
        r6 = 0x1;
        fn_80142368();
        if ((u32)r3 != (u32)0x0) {
            r3 = 0x1;
        } else {
            r4 = *(u16*)(sp + 0xA);
            r3 = r31;
            r7 = *(u16*)(sp + 0x8);
            r5 = r30;
            r6 = 0x2;
            fn_80142368();
            r0 = -r3;
            r0 = r0 | r3;
            r3 = (u32)r0 >> 31;
        }
    }
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    return;
}
/* 0x68 | fn_801297D8 | guarded_call */
void fn_801297D8(void) {
    if (0 /* guard r4 == 0 */) { return; }
    if (0 /* guard r5 == 0 */) { return; }
    if (0 /* guard r6 == 0 */) { return; }
    if (0 /* guard r7 == 0 */) { return; }
    fn_8012A5B0();
}
/* 0x78 | fn_80129840 | generic */
void fn_80129840(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    fn_8012A5B0();
    fn_80123FBC();
    fn_80120674();
}
/* 0x801298B8 | 0x90 */
void fn_801298B8(void) {
    extern void fn_80129BC8();
    extern void fn_80140588();
    extern void fn_80142CF4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r5 = 0x2;
    r6 = 0x0;
    r0 = 0x0;
    r31 = r4;
    r30 = r3;
    r3 = 0x0;
    *(u16*)(sp + 0xC) = r0;
    *(u16*)(sp + 0xA) = r0;
    fn_80142CF4();
    r4 = r3 & 0xFF;
    r3 = r30;
    r5 = (u32)sp + 0xc;
    r6 = (u32)sp + 0xa;
    r8 = (u32)sp + 0x8;
    r7 = 0x0;
    fn_80129BC8();
    if ((u32)r3 == (u32)0x0) {
        r3 = -0x1;
    } else {
        r4 = *(u16*)(sp + 0xC);
        r5 = r31;
        r6 = *(u16*)(sp + 0xA);
        r7 = *(u8*)(sp + 0x8);
        fn_80140588();
    }
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    return;
}
/* 0x80 | fn_80129948 | generic */
void fn_80129948(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, u32 arg7) {
    fn_80129BC8();
    fn_80140A9C();
}
/* 0x801299C8 | 0xB0 */
void fn_801299C8(void) {
    extern void fn_80129BC8();
    extern void fn_80140ACC();
    extern void fn_80142CF4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = 0x0;
    r31 = r6;
    r6 = 0x0;
    r30 = r5;
    r5 = 0x2;
    r29 = r4;
    r28 = r3;
    r3 = 0x0;
    *(u16*)(sp + 0xC) = r0;
    *(u16*)(sp + 0xA) = r0;
    fn_80142CF4();
    r4 = r3 & 0xFF;
    r3 = r28;
    r5 = (u32)sp + 0xc;
    r6 = (u32)sp + 0xa;
    r7 = (u32)sp + 0x8;
    r8 = 0x0;
    fn_80129BC8();
    if ((u32)r3 == (u32)0x0) {
        r3 = -0x1;
    } else {
        r4 = *(u16*)(sp + 0xC);
        r5 = r29;
        r8 = *(u16*)(sp + 0xA);
        r6 = r30;
        r9 = *(u8*)(sp + 0x8);
        r7 = r31;
        fn_80140ACC();
    }
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
/* 0x80129A78 | 0xB4 */
void fn_80129A78(void) {
    extern void fn_80129BC8();
    extern void fn_80141308();
    extern void fn_80142CF4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = 0x0;
    r31 = r6;
    r6 = 0x0;
    r30 = r5;
    r5 = 0x2;
    r29 = r4;
    r28 = r3;
    r3 = 0x0;
    *(u16*)(sp + 0xC) = r0;
    *(u16*)(sp + 0xA) = r0;
    fn_80142CF4();
    r4 = r3 & 0xFF;
    r3 = r28;
    r5 = (u32)sp + 0xc;
    r6 = (u32)sp + 0xa;
    r7 = (u32)sp + 0x9;
    r8 = (u32)sp + 0x8;
    fn_80129BC8();
    if ((u32)r3 == (u32)0x0) {
        r3 = -0x1;
    } else {
        r4 = *(u16*)(sp + 0xC);
        r5 = r29;
        r8 = *(u16*)(sp + 0xA);
        r6 = r30;
        r9 = *(u8*)(sp + 0x9);
        r7 = r31;
        r10 = *(u8*)(sp + 0x8);
        fn_80141308();
    }
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
/* 0x80129B2C | 0x9C */
void fn_80129B2C(void) {
    extern void fn_80129BC8();
    extern void fn_80142368();
    extern void fn_80142CF4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r5 = 0x2;
    r6 = 0x0;
    r0 = 0x0;
    r31 = r4;
    r30 = r3;
    r3 = 0x0;
    *(u16*)(sp + 0xA) = r0;
    *(u16*)(sp + 0x8) = r0;
    fn_80142CF4();
    r4 = r3 & 0xFF;
    r3 = r30;
    r5 = (u32)sp + 0xa;
    r6 = (u32)sp + 0x8;
    r7 = 0x0;
    r8 = 0x0;
    fn_80129BC8();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
    } else {
        r4 = *(u16*)(sp + 0xA);
        r5 = r31;
        r7 = *(u16*)(sp + 0x8);
        r6 = 0x0;
        fn_80142368();
        r0 = -r3;
        r0 = r0 | r3;
        r3 = (u32)r0 >> 31;
    }
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    return;
}
/* 0x80129BC8 | 0x19C */
void fn_80129BC8(void) {
    extern void fn_8012A5B0();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r0 = r4 & 0xFF;
    r4 = 0x0;
    r31 = r8;
    r30 = r7;
    r7 = 0x0;
    r29 = r6;
    r6 = 0x0;
    r28 = r5;
    r5 = 0x0;
    switch ((s32)r0) {
    case 0x1:
        r4 = 0x6;
        r5 = 0x0;
        fn_8012A5B0();
        r4 = r3;
        r5 = 0x10;
        r6 = 0x63;
        r7 = 0x0;
        r3 = 0x1;
        break;
    case 0x2:
        r4 = 0x4;
        r5 = 0x0;
        fn_8012A5B0();
        r4 = r3;
        r5 = 0x14;
        r6 = 0x63;
        r7 = 0x0;
        r3 = 0x1;
        break;
    case 0x3:
        r4 = 0x8;
        r5 = 0x0;
        fn_8012A5B0();
        r4 = r3;
        r5 = 0x2e;
        r6 = 0x3e7;
        r7 = 0x1;
        r3 = 0x1;
        break;
    case 0x4:
        r4 = 0x7;
        r5 = 0x0;
        fn_8012A5B0();
        r4 = r3;
        r5 = 0x40;
        r6 = 0x63;
        r7 = 0x1;
        r3 = 0x0;
        break;
    case 0x5:
        r4 = 0x5;
        r5 = 0x0;
        fn_8012A5B0();
        r4 = r3;
        r5 = 0x2b;
        r6 = 0x63;
        r7 = 0x0;
        r3 = 0x0;
        break;
    case 0x6:
        r4 = 0x9;
        r5 = 0x0;
        fn_8012A5B0();
        r4 = r3;
        r5 = 0x3;
        r6 = 0x63;
        r7 = 0x1;
        r3 = 0x0;
        break;
    default:
        break;
    }
    if ((u32)r28 != (u32)0x0) {
        *(u16*)((u8*)r28 + 0x0) = r5;
    }
    if ((u32)r29 != (u32)0x0) {
        *(u16*)((u8*)r29 + 0x0) = r6;
    }
    if ((u32)r30 != (u32)0x0) {
        *(u8*)((u8*)r30 + 0x0) = r7;
    }
    if ((u32)r31 != (u32)0x0) {
        *(u8*)((u8*)r31 + 0x0) = r3;
    }
    r3 = r4;
    return;
}
/* 0x80129D64 | 0xBC */
void fn_80129D64(void) {
    extern void fn_800F9EE4();
    extern void fn_8012640C();
    extern void fn_8012A5B0();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r5 = 0x0;
    r29 = r3;
    r28 = r4;
    r4 = 0x2;
    fn_8012A5B0();
    r30 = r3;
    r3 = r29;
    r4 = 0x1;
    r5 = 0x0;
    fn_8012A5B0();
    r0 = r3;
    r3 = r28;
    r29 = r0;
    r4 = 0x0;
    r5 = 0x75;
    r6 = 0x0;
    fn_8012640C();
    r31 = r3;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x76;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3;
    if ((u32)r30 != (u32)r31) {
        r3 = 0x0;
    } else {
        r3 = r29;
        fn_800F9EE4();
        r0 = __cntlzw(r3);
        r3 = (u32)r0 >> 5;
    }
    return;
}
/* 0x80129E20 | 0x100 */
void fn_80129E20(void) {
    extern void fn_8011F5FC();
    extern void fn_80123FBC();
    extern void fn_8012A5B0();
    extern void fn_80134BC0();
    u8 sp[0x150];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r29 = r5;
    r28 = r3;
    if ((u32)r4 == (u32)0x0) {
        r3 = 0x6;
        return;
    }
    r3 = (u32)sp + 0x8;
    fn_8011F5FC();
    /* addic. r0, (u32)sp, 0x8 */;
    if ((u32)r4 == (u32)0x0) {
        r31 = 0x6;
    } else {
    r31 = 0x0;
    while (r0 = r31 & 0xFF, (u32)r0 < (u32)0x6) {

    r3 = r28;
    r5 = r31 & 0xFF;
    r4 = 0x3;
    fn_8012A5B0();
    r30 = r3;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r3 = r30;
        r4 = (u32)sp + 0x8;
        fn_8011F5FC();
        break;
    }
    r31 = r31 + 0x1;
    }

    if (r0 = r31 & 0xFF, (u32)r0 >= (u32)0x6) { r31 = 0x6; }
    }
    r0 = r31 & 0xFF;
    if ((u32)r0 >= (u32)0x6) {
        r0 = r29 & 0xFF;
        if ((u32)r0 == (u32)0x6) {
            r3 = -0x2;
            return;
        }
        r4 = (u32)sp + 0x8;
        r3 = 0x0;
        r5 = -0x1;
        fn_80134BC0();
        r0 = 0x1 - r3;
        r0 = __cntlzw(r0);
        r3 = (u32)r0 >> 5;
        return;
    }
    r3 = (s16)r0;
    return;
}
/* 0x80129F20 | 0x16C */
void fn_80129F20(void) {
    extern void fn_8011F5FC();
    extern void fn_80123EF0();
    extern void fn_80123FBC();
    extern void fn_8012640C();
    extern void fn_8012A5B0();
    extern void fn_80134BC0();
    u8 sp[0x160];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r24 = r3;
    r25 = r5;
    r26 = r6;
    r27 = r7;
    if ((s32)r0 == (s32)0) {
        r3 = 0x6;
        return;
    }
    r3 = r28;
    r4 = 0x0;
    r5 = 0x7a;
    r6 = 0x0;
    fn_8012640C();
    r30 = r3 & 0xFF;
    r3 = r24;
    r4 = 0xb;
    r5 = 0x0;
    fn_8012A5B0();
    r31 = r3 & 0xFF;
    r3 = r24;
    r4 = 0x2;
    r5 = 0x0;
    fn_8012A5B0();
    r0 = r3;
    r3 = r24;
    r29 = r0;
    r4 = 0x1;
    r5 = 0x0;
    fn_8012A5B0();
    r4 = r28;
    r28 = r3;
    r3 = (u32)sp + 0x8;
    fn_8011F5FC();
    r4 = r25;
    r5 = r30;
    r6 = r26;
    r7 = r31;
    r8 = r29;
    r9 = r28;
    r3 = (u32)sp + 0x8;
    fn_80123EF0();
    /* addic. r0, (u32)sp, 0x8 */;
    if ((s32)r0 == (s32)0) {
        r31 = 0x6;
    } else {
        r31 = 0x0;
        while ((r31 & 0xFF) < (u32)0x6) {
            r3 = r24;
            r5 = r31 & 0xFF;
            r4 = 0x3;
            fn_8012A5B0();
            r30 = r3;
            fn_80123FBC();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) {
                r3 = r30;
                r4 = (u32)sp + 0x8;
                fn_8011F5FC();
                break;
            }
            r31 = r31 + 0x1;
        }
        if ((r31 & 0xFF) >= (u32)0x6) {
            r31 = 0x6;
        }
    }
    r0 = r31 & 0xFF;
    if ((u32)r0 >= (u32)0x6) {
        r0 = r27 & 0xFF;
        if ((u32)r0 == (u32)0x6) {
            r3 = -0x2;
            return;
        }
        r4 = (u32)sp + 0x8;
        r3 = 0x0;
        r5 = -0x1;
        fn_80134BC0();
        r0 = 0x1 - r3;
        r0 = __cntlzw(r0);
        r3 = (u32)r0 >> 5;
        return;
    }
    r3 = (s16)r0;
    return;
}
/* 0x8012A08C | 0xA4 */
void fn_8012A08C(void) {
    extern void fn_8011F5FC();
    extern void fn_80123FBC();
    extern void fn_8012A5B0();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r28 = r3;
    if ((s32)r0 == (s32)0) {
        r3 = 0x6;
        return;
    }
    r30 = 0x0;
    while ((r30 & 0xFF) < (u32)0x6) {
        r3 = r28;
        r5 = r30 & 0xFF;
        r4 = 0x3;
        fn_8012A5B0();
        r31 = r3;
        fn_80123FBC();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = r31;
            r4 = r29;
            fn_8011F5FC();
            r3 = r30;
            return;
        }
        r30 = r30 + 0x1;
    }
    r3 = 0x6;
    return;
}
/* 0x74 | fn_8012A130 | multi_call_cond */
u32 fn_8012A130(void) {
    /* fn_8012A5B0() */
    fn_800F9EE4();
    fn_8012A5B0();
    return 0;
}
/* 0x8012A1A4 | 0xA4 */
void fn_8012A1A4(void) {
    extern void fn_800E0C54();
    extern void fn_800FA280();
    extern void fn_8012A248();
    extern void fn_8012A450();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r30 = r5;
    r29 = r4;
    r28 = r3;
    fn_8012A248();
    fn_800E0C54();
    r31 = r3 & 0xFFFF;
    fn_800E0C54();
    r0 = r3 << 16;
    r3 = r28;
    r5 = r0 | r31;
    r4 = 0x2;
    fn_8012A450();
    r3 = r28;
    r5 = r29;
    r4 = 0x1;
    fn_8012A450();
    r3 = r28;
    r5 = r30 & 0xFF;
    r4 = 0xb;
    fn_8012A450();
    r3 = 0xfa2;
    fn_800FA280();
    r5 = r3;
    r3 = r28;
    r4 = 0x17;
    fn_8012A450();
    return;
}
/* 0x8012A248 | 0x208 */
void fn_8012A248(void) {
    extern void fn_801249F8();
    extern void fn_8012A450();
    extern void fn_8012A5B0();
    extern void fn_80142A88();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r4 = 0x1;
    r0 = 0x0;
    r31 = (u32)sp + 0x8;
    r5 = r31;
    r30 = r3;
    *(u16*)(sp + 0x8) = r0;
    fn_8012A450();
    r3 = r30;
    r4 = 0x2;
    r5 = 0x0;
    fn_8012A450();
    r3 = r30;
    r4 = 0x3;
    r5 = 0x0;
    fn_8012A5B0();
    r4 = 0x6;
    fn_801249F8();
    r3 = r30;
    r4 = 0x4;
    r5 = 0x0;
    fn_8012A5B0();
    r4 = 0x14;
    fn_80142A88();
    r3 = r30;
    r4 = 0x5;
    r5 = 0x0;
    fn_8012A5B0();
    r4 = 0x2b;
    fn_80142A88();
    r3 = r30;
    r4 = 0x6;
    r5 = 0x0;
    fn_8012A5B0();
    r4 = 0x10;
    fn_80142A88();
    r3 = r30;
    r4 = 0x7;
    r5 = 0x0;
    fn_8012A5B0();
    r4 = 0x40;
    fn_80142A88();
    r3 = r30;
    r4 = 0x8;
    r5 = 0x0;
    fn_8012A5B0();
    r4 = 0x2e;
    fn_80142A88();
    r3 = r30;
    r4 = 0x9;
    r5 = 0x0;
    fn_8012A5B0();
    r4 = 0x3;
    fn_80142A88();
    r3 = r30;
    r4 = 0xb;
    r5 = 0x2;
    fn_8012A450();
    r3 = r30;
    r4 = 0xc;
    r5 = 0x0;
    fn_8012A450();
    r3 = r30;
    r4 = 0xd;
    r5 = 0x0;
    fn_8012A450();
    r3 = r30;
    r4 = 0xe;
    r5 = 0x0;
    fn_8012A450();
    r3 = r30;
    r4 = 0xf;
    r5 = 0x1;
    fn_8012A450();
    r3 = r30;
    r4 = 0x10;
    r5 = 0x1;
    fn_8012A450();
    r3 = r30;
    r4 = 0x11;
    r5 = 0x1;
    fn_8012A450();
    r3 = r30;
    r4 = 0x12;
    r5 = 0x1;
    fn_8012A450();
    r3 = r30;
    r4 = 0x13;
    r5 = 0x1;
    fn_8012A450();
    r3 = r30;
    r4 = 0x14;
    r5 = 0x1;
    fn_8012A450();
    r3 = r30;
    r4 = 0x15;
    r5 = 0x1;
    fn_8012A450();
    r3 = r30;
    r4 = 0x16;
    r5 = 0x1;
    fn_8012A450();
    r3 = r30;
    r5 = r31;
    r4 = 0x17;
    fn_8012A450();
    r3 = r30;
    r4 = 0x18;
    r5 = 0x0;
    fn_8012A450();
    r3 = r30;
    r4 = 0xa;
    r5 = 0x0;
    fn_8012A5B0();
    r4 = 0xa;
    fn_80142A88();
    r3 = r30;
    r4 = 0x19;
    r5 = 0x0;
    fn_8012A450();
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    return;
}
/* 0x8012A450 | 0x160 */
void fn_8012A450(void) {
    extern void fn_80129280();
    extern void fn_8012A774();
    extern void fn_8012A7B4();
    extern void fn_8012A7DC();
    extern void fn_8012A824();
    extern void fn_8012A86C();
    extern void fn_8012A89C();
    extern void fn_8012A9AC();
    extern void fn_8012A9BC();
    extern void fn_8012A9CC();
    extern void fn_8012A9DC();
    extern void fn_8012A9EC();
    extern void fn_8012A9FC();
    extern void fn_8012AA0C();
    extern void fn_8012AA1C();
    extern void fn_8012AA44();
    extern void fn_8012AA54();
    extern void fn_8012AA64();
    extern u8 jumptable_803634F0[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    r0 = r4 & 0xFFFF;
    r31 = r5;
    r30 = r4;
    if ((s32)r0 == (s32)0) return;
    if ((u32)r0 >= (u32)0x1a) {
        return;
    }
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        r4 = 0x0;
        fn_80129280();
        if ((u32)r3 == (u32)0x0) return;
        r4 = 0x2;
        fn_80129280();
        if ((u32)r3 == (u32)0x0) return;
    }
    r0 = r30 & 0xFFFF;
    if ((u32)r0 > (u32)0x19) return;
    r4 = (u32)jumptable_803634F0;
    r0 = r0 << 2;
    r4 = (u32)jumptable_803634F0;
    r0 = *(u32*)(r4 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r4 = r31;
    fn_8012AA64();
    return;
    r4 = r31;
    fn_8012AA54();
    return;
    r4 = r31 & 0xFF;
    fn_8012AA44();
    return;
    r4 = r31;
    fn_8012A86C();
    return;
    r4 = r31;
    fn_8012A824();
    return;
    r4 = r31;
    fn_8012A7DC();
    return;
    r4 = r31 & 0xFF;
    fn_8012AA1C();
    return;
    r4 = r31 & 0xFF;
    fn_8012AA0C();
    return;
    r4 = r31 & 0xFF;
    fn_8012A9FC();
    return;
    r4 = r31 & 0xFF;
    fn_8012A9EC();
    return;
    r4 = r31 & 0xFF;
    fn_8012A9DC();
    return;
    r4 = r31 & 0xFF;
    fn_8012A9CC();
    return;
    r4 = r31 & 0xFF;
    fn_8012A9BC();
    return;
    r4 = r31 & 0xFF;
    fn_8012A9AC();
    return;
    r4 = r31;
    fn_8012A89C();
    return;
    r4 = r31 & 0xFF;
    fn_8012A7B4();
    return;
    r4 = r31 & 0xFF;
    fn_8012A774();
    return;
}
/* 0x8012A5B0 | 0x1C4 */
void fn_8012A5B0(void) {
    extern void fn_80129280();
    extern void fn_8012A784();
    extern void fn_8012A79C();
    extern void fn_8012A7C4();
    extern void fn_8012A80C();
    extern void fn_8012A854();
    extern void fn_8012A8D4();
    extern void fn_8012A8EC();
    extern void fn_8012A904();
    extern void fn_8012A91C();
    extern void fn_8012A934();
    extern void fn_8012A94C();
    extern void fn_8012A964();
    extern void fn_8012A97C();
    extern void fn_8012A994();
    extern void fn_8012AA2C();
    extern void fn_8012AA9C();
    extern void fn_8012AAD0();
    extern void fn_8012AB04();
    extern void fn_8012AB38();
    extern void fn_8012AB6C();
    extern void fn_8012ABA0();
    extern void fn_8012ABD4();
    extern void fn_8012AC08();
    extern void fn_8012AC3C();
    extern void fn_8012AC54();
    extern u8 jumptable_80363558[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    r0 = r4 & 0xFFFF;
    r31 = r5;
    r30 = r4;
    if ((s32)r0 == (s32)0) { r3 = 0x0; return; }
    if ((u32)r0 >= (u32)0x1a) {
        r3 = 0x0;
        return;
    }
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        r4 = 0x0;
        fn_80129280();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            return;
        }
        r4 = 0x2;
        fn_80129280();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            return;
    }
    }
    r0 = r30 & 0xFFFF;
    if ((u32)r0 <= (u32)0x19) {
        r4 = (u32)jumptable_80363558;
        r0 = r0 << 2;
        r4 = (u32)jumptable_80363558;
        r0 = *(u32*)(r4 + r0);
        ctr_fn = (void(*)(void))r0;
        /* indirect jump via ctr */;
        fn_8012AC54();
        return;
        fn_8012AC3C();
        return;
        r4 = r31;
        fn_8012AC08();
        return;
        r4 = r31;
        fn_8012ABD4();
        return;
        r4 = r31;
        fn_8012ABA0();
        return;
        r4 = r31;
        fn_8012AB6C();
        return;
        r4 = r31;
        fn_8012AB38();
        return;
        r4 = r31;
        fn_8012AB04();
        return;
        r4 = r31;
        fn_8012AAD0();
        return;
        r4 = r31;
        fn_8012AA9C();
        return;
        fn_8012AA2C();
        r3 = r3 & 0xFF;
        return;
        fn_8012A854();
        return;
        fn_8012A80C();
        return;
        fn_8012A7C4();
        return;
        fn_8012A994();
        r3 = r3 & 0xFF;
        return;
        fn_8012A97C();
        r3 = r3 & 0xFF;
        return;
        fn_8012A964();
        r3 = r3 & 0xFF;
        return;
        fn_8012A94C();
        r3 = r3 & 0xFF;
        return;
        fn_8012A934();
        r3 = r3 & 0xFF;
        return;
        fn_8012A91C();
        r3 = r3 & 0xFF;
        return;
        fn_8012A904();
        r3 = r3 & 0xFF;
        return;
        fn_8012A8EC();
        r3 = r3 & 0xFF;
        return;
        fn_8012A8D4();
        return;
        fn_8012A79C();
        r3 = r3 & 0xFF;
        return;
        fn_8012A784();
        r3 = r3 & 0xFF;
        return;
    }
    r3 = 0x0;
    return;
}
/* 0x8012A7DC | 0x30 */
void fn_8012A7DC(u8* ptr, s32 val) {
    if (ptr == NULL) { return; }
    if (val < 0) { val = 0; }
    if (val > 0x98967F) { val = 0x98967F; }
    *(s32*)(&ptr[0xA8C]) = val;
}
/* 0x8012A824 | 0x30 */
void fn_8012A824(u8* ptr, s32 val) {
    if (ptr == NULL) { return; }
    if (val < 0) { val = 0; }
    if (val > 0x98967F) { val = 0x98967F; }
    *(s32*)(&ptr[0xA88]) = val;
}
/* 0x8012A86C | 0x30 */
void fn_8012A86C(u8* ptr, s32 val) {
    if (ptr == NULL) { return; }
    if (val < 0) { val = 0; }
    if (val > 0x98967F) { val = 0x98967F; }
    *(s32*)(&ptr[0xA84]) = val;
}
/* 0x8012A89C | 0x38 */
void fn_8012A89C(u8* ptr, void* src) {
    if (ptr == NULL) { return; }
    if (src == NULL) { return; }
    fn_800F9D24(ptr + 0xAC2, src, 0xB);
}
/* 0x8012A8D4 | 24 bytes | nc_addi_ptr */
void* fn_8012A8D4(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0xAC2;
}
/* 0x8012AA64 | 0x38 */
void fn_8012AA64(void* dst, void* src) {
    if (dst == NULL) { return; }
    if (src == NULL) { return; }
    fn_800F9D24(dst, src, 0xB);
}
/* 0x8012AA9C | 0x34 */
void* fn_8012AA9C(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 0xA) { return NULL; }
    return ptr + (u32)idx * 4 + 0xA9A;
}
/* 0x8012AAD0 | 0x34 */
void* fn_8012AAD0(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 0x3) { return NULL; }
    return ptr + (u32)idx * 4 + 0xA74;
}
/* 0x8012AB04 | 0x34 */
void* fn_8012AB04(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 0x2E) { return NULL; }
    return ptr + (u32)idx * 4 + 0x9BC;
}
/* 0x8012AB38 | 0x34 */
void* fn_8012AB38(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 0x40) { return NULL; }
    return ptr + (u32)idx * 4 + 0x8BC;
}
/* 0x8012AB6C | 0x34 */
void* fn_8012AB6C(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 0x10) { return NULL; }
    return ptr + (u32)idx * 4 + 0x87C;
}
/* 0x8012ABA0 | 0x34 */
void* fn_8012ABA0(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 0x2B) { return NULL; }
    return ptr + (u32)idx * 4 + 0x7D0;
}
/* 0x8012ABD4 | 0x34 */
void* fn_8012ABD4(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 0x14) { return NULL; }
    return ptr + (u32)idx * 4 + 0x780;
}
/* 0x8012AC08 | 0x34 */
void* fn_8012AC08(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 6) { return NULL; }
    return ptr + (u32)idx * 0x138 + 0x30;
}
/* 0x8012AC54 | 16 bytes | nc_bnelr */
u32 fn_8012AC54(void* ptr) {
    if (ptr != NULL) { return (u32)ptr; }
    return 0;
}
/* 0x8012AC64 | 0x38 */
void fn_8012AC64(u32* dst, u32* src) {
    s32 i;
    if (dst == NULL) { return; }
    if (src == NULL) { return; }
    for (i = 0; i < 0x163; i++) {
        dst[i * 2] = src[i * 2];
        dst[i * 2 + 1] = src[i * 2 + 1];
    }
}
/* 0x8012AC9C | 0xB4 */
void fn_8012AC9C(void) {
    extern void fn_8011F1A0();
    extern void fn_80122370();
    extern void fn_80123FBC();
    extern void fn_8012A5B0();
    extern void fn_80143F6C();
    extern void fn_801440A0();
    extern u8 lbl_80426BD0;
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r3 = (u32)&lbl_80426BD0;
    r4 = (u32)&lbl_80426BD0;
    r3 = *(u32*)((u8*)r4 + 0x184);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x184) = r0;
    if ((s32)r0 >= (s32)0x100) {
        r0 = 0x0;
        r31 = 0x0;
        *(u32*)((u8*)r4 + 0x184) = r0;
        do {
            r5 = r31 & 0xFFFF;
            r3 = 0x0;
            r4 = 0x3;
            fn_8012A5B0();
            if ((s32)r0 != (s32)0x100) {
                fn_80123FBC();
                r0 = r3 & 0xFF;
                if ((s32)r0 != (s32)0x100) {
                    r3 = r30;
                    fn_8011F1A0();
                    fn_801440A0();
                    if ((u32)r3 == (u32)0x0) {
                        r4 = 0x0;
                    } else {
                        fn_80143F6C();
                        r4 = r3;
                    }
                    r3 = r30;
                    r5 = 0x5;
                    fn_80122370();
            }
            }
            r31 = r31 + 0x1;
        } while ((s32)r31 < (s32)0x6);
    }
    return;
}
/* 0x8012AD50 | 0x434 */
void fn_8012AD50(void) {
    extern u8 lbl_8047D030[];
    extern u8 lbl_8047D034[];
    extern u8 lbl_8047D038[];
    extern void fn_800F9318();
    extern void fn_801065B8();
    extern void fn_801067E8();
    extern void fn_80113FE8();
    extern void fn_8011F1A0();
    extern void fn_8011F4F0();
    extern void fn_80121ADC();
    extern void fn_80121B4C();
    extern void fn_80122370();
    extern void fn_80123FBC();
    extern void fn_801254B4();
    extern void fn_8012640C();
    extern void fn_80129474();
    extern void fn_8012A5B0();
    extern void fn_8012C660();
    extern void fn_80132A38();
    extern void fn_80143F6C();
    extern void fn_801440A0();
    extern void fn_8018C69C();
    extern void fn_8018C7C8();
    extern void fn_8018CA20();
    extern void fn_801C409C();
    extern void fn_801D0AFC();
    extern u8 lbl_80426BD0;
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    r3 = (u32)&lbl_80426BD0;
    r4 = (u32)&lbl_80426BD0;
    r26 = 0x0;
    r25 = 0x0;
    r30 = 0x0;
    r3 = *(u32*)((u8*)r4 + 0x180);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x180) = r0;
    if ((s32)r0 < (s32)0x4) return;
    r0 = 0x0;
    r27 = (u32)sp + 0x28;
    *(u32*)((u8*)r4 + 0x180) = r0;
    r23 = r27;
    r24 = 0x0;
    do {
        r5 = r24 & 0xFFFF;
        r3 = 0x0;
        r4 = 0x3;
        fn_8012A5B0();
        if ((s32)r0 != (s32)0x4) {
            fn_80123FBC();
            r0 = r3 & 0xFF;
            if ((s32)r0 != (s32)0x4) {
                r3 = r22;
                r4 = 0x0;
                r5 = 0x83;
                r6 = 0x0;
                fn_8012640C();
                r21 = r3 & 0xFFFF;
                if ((s32)r0 != (s32)0x4) {
                    r3 = r22;
                    r4 = 0x3;
                    fn_80121ADC();
                    r0 = r3 & 0xFF;
                    if ((s32)r0 == (s32)0x4) {
                        r3 = r22;
                        r4 = 0x4;
                        fn_80121ADC();
                        r0 = r3 & 0xFF;
                        if ((s32)r0 == (s32)0x4) goto L_8012AE38;
                    }
                    r3 = r22;
                    r7 = r21 & 0xFFFF;
                    r4 = 0x0;
                    r5 = 0x83;
                    r6 = 0x0;
                    fn_801254B4();
                    r0 = r21 & 0xFFFF;
                    r30 = 0x1;
                    if ((s32)r0 == (s32)0x4) {
                        *(u32*)((u8*)r23 + 0x0) = r24;
                        r23 = r23 + 0x4;
                        r25 = r25 + 0x1;
            }
                L_8012AE38: ;
                    r0 = r21 & 0xFFFF;
                    if ((s32)r0 != (s32)0x4) {
                        r26 = r26 + 0x1;
            }
            }
            }
        }
        r24 = r24 + 0x1;
    } while ((s32)r24 < (s32)0x6);
    r0 = r30 & 0xFF;
    if ((s32)r24 != (s32)0x6) {
        fn_801C409C();
    }
    if ((s32)r25 <= (s32)0x0) return;
    r30 = 0x0;
    r24 = 0x0;
    r23 = r30;
    do {
        if ((s32)r30 >= (s32)0x0) {
            if ((s32)r30 < (s32)0x2) goto L_8012AE88;
        }
        r0 = 0x0;
        goto L_8012AEAC;
    L_8012AE88: ;
        r3 = (u32)&lbl_80426BD0;
        r0 = (u32)&lbl_80426BD0;
        r3 = r0 + r23;
        r0 = *(u16*)((u8*)r3 + 0x4);
        r0 = r0 & 0x1;
        if ((s32)r30 == (s32)0x2) {
            r0 = 0x0;
            goto L_8012AEAC;
        }
        r0 = 0x1;
    L_8012AEAC: ;
        r0 = r0 & 0xFF;
        if ((s32)r30 != (s32)0x2) {
            r3 = *(u32*)lbl_8047D030;
            r0 = *(u32*)lbl_8047D034;
            *(u32*)(sp + 0xC) = r0;
            *(u32*)(sp + 0x24) = r0;
            if (((s32)r30 >= (s32)0x0) && ((s32)r30 < (s32)0x2)) {
                r3 = (u32)sp + 0x20;
                r31 = *(u32*)(r3 + r24);
            }
            r3 = *(u32*)(sp + 0x8);
            r0 = *(u32*)(sp + 0xC);
            *(u32*)(sp + 0x14) = r0;
            if (((s32)r30 >= (s32)0x0) && ((s32)r30 < (s32)0x2)) {
                r3 = (u32)sp + 0x10;
                r28 = *(u32*)(r3 + r24);
            }
            r4 = r28;
            r3 = 0x0;
            fn_800F9318();
            f1 = *(f32*)lbl_8047D038;
            r4 = r30;
            fn_8012C660();
            r3 = (0x8000 << 16);
            r4 = r31;
            r5 = r3 + 0x8;
            r3 = 0x0;
            fn_8018C7C8();
            r4 = r31;
            r3 = 0x0;
            r5 = 0x100;
            fn_8018C69C();
            r4 = r31;
            r3 = 0x0;
            r5 = 0x400;
            fn_8018C69C();
            r4 = r31;
            r3 = 0x0;
            r5 = 0x0;
            fn_8018CA20();
        }
        r30 = r30 + 0x1;
        r23 = r23 + 0x20;
        r24 = r24 + 0x4;
    } while ((s32)r30 < (s32)0x2);
    r23 = 0x0;
    while ((s32)r23 < (s32)r25) {
        r0 = *(u32*)((u8*)r27 + 0x0);
        r3 = 0x0;
        r4 = 0x3;
        r5 = r0 & 0xFFFF;
        fn_8012A5B0();
        r24 = r3;
        fn_8011F4F0();
        r0 = r3;
        r3 = 0x32;
        r4 = r0;
        fn_80132A38();
        r3 = 0x444e;
        r4 = 0x1;
        r5 = 0x0;
        fn_801067E8();
        r3 = 0x1;
        fn_801065B8();
        r3 = r24;
        fn_8011F1A0();
        fn_801440A0();
        if ((u32)r3 == (u32)0x0) {
            r4 = 0x0;
        } else {
            fn_80143F6C();
            r4 = r3;
        }
        r3 = r24;
        r5 = 0x7;
        fn_80122370();
        r27 = r27 + 0x4;
        r23 = r23 + 0x1;
    }
    if ((s32)r26 <= (s32)0x0) {
        r3 = 0x444f;
        r4 = 0x1;
        r5 = 0x0;
        fn_801067E8();
        r3 = 0x1;
        fn_801065B8();
        r3 = 0x0;
        r4 = 0xc;
        r5 = 0x0;
        fn_8012A5B0();
        r4 = r3;
        r3 = 0x0;
        r0 = (u32)r4 >> 31;
        r0 = r0 + r4;
        r4 = (s32)r0 >> 1;
        fn_80129474();
        r3 = 0x1;
        fn_801D0AFC();
        r23 = 0x0;
        do {
            r5 = r23 & 0xFFFF;
            r3 = 0x0;
            r4 = 0x3;
            fn_8012A5B0();
            if ((s32)r26 != (s32)0x0) {
                fn_80123FBC();
                r0 = r3 & 0xFF;
                if ((s32)r26 != (s32)0x0) {
                    r3 = r24;
                    r4 = 0x3e;
                    fn_80121ADC();
                    r0 = r3 & 0xFF;
                    if ((s32)r26 != (s32)0x0) {
                        r3 = r24;
                        r4 = 0x3e;
                        fn_80121B4C();
            }
            }
            }
            r23 = r23 + 0x1;
        } while ((s32)r23 < (s32)0x6);
        fn_80113FE8();
    }
    r23 = 0x0;
    r25 = 0x0;
    r26 = 0x0;
    do {
        if ((s32)r23 >= (s32)0x0) {
            if ((s32)r23 < (s32)0x2) goto L_8012B0D4;
        }
        r0 = 0x0;
        goto L_8012B0F8;
    L_8012B0D4: ;
        r3 = (u32)&lbl_80426BD0;
        r0 = (u32)&lbl_80426BD0;
        r3 = r0 + r26;
        r0 = *(u16*)((u8*)r3 + 0x4);
        r0 = r0 & 0x1;
        if ((s32)r23 == (s32)0x2) {
            r0 = 0x0;
            goto L_8012B0F8;
        }
        r0 = 0x1;
    L_8012B0F8: ;
        r0 = r0 & 0xFF;
        if ((s32)r23 != (s32)0x2) {
            r3 = *(u32*)lbl_8047D030;
            r0 = *(u32*)lbl_8047D034;
            *(u32*)(sp + 0x1C) = r0;
            if (((s32)r23 >= (s32)0x0) && ((s32)r23 < (s32)0x2)) {
                r3 = (u32)sp + 0x18;
                r29 = *(u32*)(r3 + r25);
            }
            r4 = r29;
            r3 = 0x0;
            r5 = 0x1;
            fn_8018CA20();
            r4 = r29;
            r3 = 0x0;
            r5 = 0x700;
            fn_8018C7C8();
            r3 = (0x8000 << 16);
            r4 = r29;
            r5 = r3 + 0x8;
            r3 = 0x0;
            fn_8018C69C();
        }
        r23 = r23 + 0x1;
        r26 = r26 + 0x20;
        r25 = r25 + 0x4;
    } while ((s32)r23 < (s32)0x2);
    return;
}
/* 0x8012B184 | 0x18 */
extern u8 lbl_80426BD0[];
void fn_8012B184(s32 val) {
    if (val < 0) { return; }
    *(u32*)(lbl_80426BD0 + 0x188) = (u32)val;
}
/* 0x8012B19C | 0x448 */
void fn_8012B19C(void) {
    extern u8 lbl_80478AC0[];
    extern u8 lbl_8047D030[];
    extern u8 lbl_8047D034[];
    extern u8 lbl_8047D038[];
    extern u8 lbl_8047D03C[];
    extern u8 lbl_8047D040[];
    extern u8 lbl_8047D048[];
    extern u8 lbl_8047D050[];
    extern u8 lbl_8047D058[];
    extern u8 lbl_8047D060[];
    extern void fn_800A3A78();
    extern void fn_800A3AC0();
    extern void fn_800E3D98();
    extern void fn_800F9318();
    extern void fn_8010F320();
    extern void fn_8010FDF8();
    extern void fn_8018D928();
    extern void fn_8018D998();
    extern void fn_8018F5E4();
    extern void fn_8018F6F4();
    u8 sp[0xA0];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    *(f64*)(sp + 0x90) = f31;
    /* psq_st f31, 0x98((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x80) = f30;
    /* psq_st f30, 0x88((u32)sp), 0, qr0 */;
    f31 = f1;
    r27 = r4;
    r28 = r5;
    if ((s32)r0 >= (s32)0) {
        if ((s32)r26 < (s32)0x2) goto L_8012B1E0;
    }
    r0 = 0x0;
    goto L_8012B208;
L_8012B1E0: ;
    r3 = (u32)&lbl_80426BD0;
    r0 = r26 << 5;
    r3 = (u32)&lbl_80426BD0;
    r3 = r3 + r0;
    r0 = *(u16*)((u8*)r3 + 0x4);
    r0 = r0 & 0x1;
    if ((s32)r26 == (s32)0x2) {
        r0 = 0x0;
        goto L_8012B208;
    }
    r0 = 0x1;
L_8012B208: ;
    r0 = r0 & 0xFF;
    if ((s32)r26 == (s32)0x2) {
        r3 = 0x0;
        goto L_8012B5C0;
    }
    if ((u32)r27 != (u32)0x0) {
        f0 = *(f32*)((u8*)r27 + 0x0);
        *(f32*)(sp + 0x2C) = f0;
        f0 = *(f32*)((u8*)r27 + 0x4);
        *(f32*)(sp + 0x30) = f0;
        f0 = *(f32*)((u8*)r27 + 0x8);
        *(f32*)(sp + 0x34) = f0;
    } else {
        r3 = *(u32*)lbl_8047D030;
        r0 = *(u32*)lbl_8047D034;
        *(u32*)(sp + 0x18) = r0;
        if (((s32)r26 >= (s32)0x0) && ((s32)r26 < (s32)0x2)) {
            r0 = r26 << 2;
            r3 = (u32)sp + 0x14;
            r4 = *(u32*)(r3 + r0);
        }
        r3 = 0x0;
        fn_800F9318();
        r4 = (u32)sp + 0x2c;
        fn_800E3D98();
    }
    f0 = *(f32*)(sp + 0x30);
    f1 = *(f32*)lbl_8047D03C;
    r3 = *(u32*)lbl_8047D030;
    f0 = f0 + f1;
    r0 = *(u32*)lbl_8047D034;
    *(f32*)(sp + 0x30) = f0;
    f0 = *(f32*)((u8*)r28 + 0x0);
    *(u32*)(sp + 0x20) = r0;
    *(f32*)(sp + 0x38) = f0;
    f0 = *(f32*)((u8*)r28 + 0x4);
    f0 = f1 + f0;
    *(f32*)(sp + 0x3C) = f0;
    f0 = *(f32*)((u8*)r28 + 0x8);
    *(f32*)(sp + 0x40) = f0;
    if ((s32)r26 >= (s32)0x0) {
        if ((s32)r26 < (s32)0x2) goto L_8012B2CC;
    }
    r0 = 0x0;
    goto L_8012B2DC;
L_8012B2CC: ;
    r0 = r26 << 2;
    r3 = (u32)sp + 0x1c;
    r31 = *(u32*)(r3 + r0);
    r0 = 0x1;
L_8012B2DC: ;
    r0 = r0 & 0xFF;
    if ((s32)r26 == (s32)0x2) {
        r3 = -0x1;
        goto L_8012B310;
    }
    r4 = r31;
    r3 = 0x0;
    fn_8018D998();
    fn_8018D928();
    if ((u32)r3 == (u32)0x0) {
        r3 = -0x1;
        goto L_8012B310;
    }
    r3 = *(u32*)((u8*)r3 + 0x30);
L_8012B310: ;
    r0 = r3 + (0x1 << 16);
    if ((u32)r0 == (u32)0xffff) {
        r3 = 0x0;
        goto L_8012B5C0;
    }
    fn_8018F6F4();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        goto L_8012B5C0;
    }
    fn_8018F5E4();
    f0 = *(f32*)lbl_8047D040;
    r3 = (u32)sp + 0x2c;
    r4 = (u32)sp + 0x38;
    r5 = 0x0;
    f30 = f0 * f1;
    f1 = f30;
    fn_8010F320();
    if ((s32)r3 != (s32)0x0) {
        r3 = 0x0;
        goto L_8012B5C0;
    }
    r3 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0x28) = r0;
    if (((s32)r26 >= (s32)0x0) && ((s32)r26 < (s32)0x2)) {
        r0 = r26 << 2;
        r3 = (u32)sp + 0x24;
        r29 = *(u32*)(r3 + r0);
    }
    r4 = r29;
    r3 = 0x0;
    fn_8018D998();
    fn_8018D928();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        goto L_8012B5C0;
    }
    r31 = *(u32*)((u8*)r3 + 0x50);
    if ((u32)r27 != (u32)0x0) {
        f0 = *(f32*)((u8*)r27 + 0x0);
        *(f32*)(sp + 0x2C) = f0;
        f0 = *(f32*)((u8*)r27 + 0x4);
        *(f32*)(sp + 0x30) = f0;
        f0 = *(f32*)((u8*)r27 + 0x8);
        *(f32*)(sp + 0x34) = f0;
    } else {
        r3 = *(u32*)lbl_8047D030;
        r0 = *(u32*)lbl_8047D034;
        *(u32*)(sp + 0x10) = r0;
        if (((s32)r26 >= (s32)0x0) && ((s32)r26 < (s32)0x2)) {
            r0 = r26 << 2;
            r3 = (u32)sp + 0xc;
            r30 = *(u32*)(r3 + r0);
        }
        r4 = r30;
        r3 = 0x0;
        fn_800F9318();
        r4 = (u32)sp + 0x2c;
        fn_800E3D98();
    }
    f0 = *(f32*)((u8*)r28 + 0x0);
    r3 = (u32)sp + 0x38;
    r4 = (u32)sp + 0x2c;
    r5 = (u32)sp + 0x50;
    *(f32*)(sp + 0x38) = f0;
    f0 = *(f32*)((u8*)r28 + 0x4);
    *(f32*)(sp + 0x3C) = f0;
    f0 = *(f32*)((u8*)r28 + 0x8);
    *(f32*)(sp + 0x40) = f0;
    ((void(*)(void))fn_800A3A9C)();
    f3 = *(f32*)(sp + 0x2C);
    f5 = f30 + f31;
    f0 = *(f32*)(sp + 0x38);
    f2 = *(f32*)(sp + 0x34);
    f1 = *(f32*)(sp + 0x40);
    f3 = f3 - f0;
    f0 = *(f32*)lbl_8047D038;
    f1 = f2 - f1;
    f2 = f3 * f3;
    f1 = f1 * f1;
    f4 = f2 + f1;
    if (f4 > f0) {
        /* frsqrte f1, f4 */;
        f3 = *(f64*)lbl_8047D048;
        f2 = *(f64*)lbl_8047D050;
        f0 = f1 * f1;
        f1 = f3 * f1;
        f0 = -(f4 * f0 - f2);
        f1 = f1 * f0;
        f0 = f1 * f1;
        f1 = f3 * f1;
        f0 = -(f4 * f0 - f2);
        f1 = f1 * f0;
        f0 = f1 * f1;
        f1 = f3 * f1;
        f0 = -(f4 * f0 - f2);
        f0 = f1 * f0;
        f4 = f4 * f0;
        f4 = (f32)f4;
        goto L_8012B544;
    }
    f0 = *(f64*)lbl_8047D058;
    if (f4 < f0) {
        r3 = (u32)lbl_80478AC0;
        f4 = *(f32*)lbl_80478AC0;
        goto L_8012B544;
    }
    *(f32*)(sp + 0x8) = f4;
    r0 = (0x7f80 << 16);
    r4 = *(u32*)(sp + 0x8);
    r3 = r4 & 0x7F800000;
    if ((s32)r3 != (s32)r0) {
        if ((s32)r3 < (s32)r0) {
            if ((s32)r3 == (s32)0x0) goto L_8012B518;
        }
        goto L_8012B530;
    }
    r0 = r4 & 0x7FFFFF;
    if ((s32)r3 != (s32)0x0) {
        r0 = 0x1;
        goto L_8012B534;
    }
    r0 = 0x2;
    goto L_8012B534;
L_8012B518: ;
    r0 = r4 & 0x7FFFFF;
    if ((s32)r3 != (s32)0x0) {
        r0 = 0x5;
        goto L_8012B534;
    }
    r0 = 0x3;
    goto L_8012B534;
L_8012B530: ;
    r0 = 0x4;
L_8012B534: ;
    if ((s32)r0 == (s32)0x1) {
        r3 = (u32)lbl_80478AC0;
        f4 = *(f32*)lbl_80478AC0;
    }
L_8012B544: ;
    f0 = *(f32*)lbl_8047D038;
    /* cror eq, lt, eq */;
    if (f4 == f0) {
        r3 = 0x1;
        goto L_8012B5C0;
    }
    /* cror eq, lt, eq */;
    if (f4 == f5) {
        r3 = 0x1;
        goto L_8012B5C0;
    }
    f1 = f4 - f5;
    f0 = *(f32*)lbl_8047D060;
    r3 = (u32)sp + 0x50;
    r4 = (u32)sp + 0x44;
    f0 = f1 - f0;
    f1 = f0 / f4;
    fn_800A3AC0();
    r3 = (u32)sp + 0x2c;
    r4 = (u32)sp + 0x44;
    r5 = (u32)sp + 0x38;
    fn_800A3A78();
    r3 = r31;
    r4 = (u32)sp + 0x2c;
    r5 = (u32)sp + 0x38;
    r6 = 0x0;
    fn_8010FDF8();
    r4 = 0x6 - r3;
    r0 = r4 | r0;
    r3 = (u32)r0 >> 31;
L_8012B5C0: ;
    /* psq_l f31, 0x98((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x90);
    /* psq_l f30, 0x88((u32)sp), 0, qr0 */;
    f30 = *(f64*)(sp + 0x80);
    return;
}
/* 0x8012B5E4 | 0x4EC */
void fn_8012B5E4(void) {
    extern u8 lbl_80478AC0[];
    extern u8 lbl_8047D030[];
    extern u8 lbl_8047D034[];
    extern u8 lbl_8047D038[];
    extern u8 lbl_8047D03C[];
    extern u8 lbl_8047D048[];
    extern u8 lbl_8047D050[];
    extern u8 lbl_8047D058[];
    extern u8 lbl_8047D060[];
    extern void fn_800A3A78();
    extern void fn_800A3AC0();
    extern void fn_800E3D98();
    extern void fn_800F9318();
    extern void fn_8010F320();
    extern void fn_8010FDF8();
    extern void fn_8018D928();
    extern void fn_8018D998();
    extern void fn_8018F5E4();
    extern void fn_8018F6F4();
    u8 sp[0xC0];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    *(f64*)(sp + 0xB0) = f31;
    /* psq_st f31, 0xb8((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0xA0) = f30;
    /* psq_st f30, 0xa8((u32)sp), 0, qr0 */;
    if ((s32)r0 >= (s32)0) {
        if ((s32)r24 < (s32)0x2) goto L_8012B61C;
    }
    r0 = 0x0;
    goto L_8012B644;
L_8012B61C: ;
    r3 = (u32)&lbl_80426BD0;
    r0 = r24 << 5;
    r3 = (u32)&lbl_80426BD0;
    r3 = r3 + r0;
    r0 = *(u16*)((u8*)r3 + 0x4);
    r0 = r0 & 0x1;
    if ((s32)r24 == (s32)0x2) {
        r0 = 0x0;
        goto L_8012B644;
    }
    r0 = 0x1;
L_8012B644: ;
    r0 = r0 & 0xFF;
    if ((s32)r24 == (s32)0x2) {
        r3 = 0x0;
        goto L_8012BAAC;
    }
    r3 = *(u32*)lbl_8047D030;
    r5 = (u32)&lbl_80426BD0;
    r0 = *(u32*)lbl_8047D034;
    r26 = *(u32*)&lbl_80426BD0;
    *(u32*)(sp + 0x28) = r0;
    if (((s32)r24 >= (s32)0x0) && ((s32)r24 < (s32)0x2)) {
        r0 = r24 << 2;
        r3 = (u32)sp + 0x24;
        r4 = *(u32*)(r3 + r0);
    }
    r3 = 0x0;
    fn_800F9318();
    r4 = (u32)sp + 0x68;
    fn_800E3D98();
    f1 = *(f32*)(sp + 0x6C);
    f0 = *(f32*)lbl_8047D03C;
    r3 = *(u32*)lbl_8047D030;
    f0 = f1 + f0;
    r0 = *(u32*)lbl_8047D034;
    *(f32*)(sp + 0x6C) = f0;
    *(u32*)(sp + 0x20) = r0;
    if (((s32)r26 >= (s32)0x0) && ((s32)r26 < (s32)0x2)) {
        r0 = r26 << 2;
        r3 = (u32)sp + 0x1c;
        r29 = *(u32*)(r3 + r0);
    }
    r4 = r29;
    r3 = 0x0;
    fn_800F9318();
    r4 = (u32)sp + 0x5c;
    fn_800E3D98();
    f1 = *(f32*)(sp + 0x60);
    f0 = *(f32*)lbl_8047D03C;
    r3 = *(u32*)lbl_8047D030;
    f0 = f1 + f0;
    r0 = *(u32*)lbl_8047D034;
    *(f32*)(sp + 0x60) = f0;
    *(u32*)(sp + 0x38) = r0;
    if ((s32)r24 >= (s32)0x0) {
        if ((s32)r24 < (s32)0x2) goto L_8012B720;
    }
    r0 = 0x0;
    goto L_8012B730;
L_8012B720: ;
    r0 = r24 << 2;
    r3 = (u32)sp + 0x34;
    r31 = *(u32*)(r3 + r0);
    r0 = 0x1;
L_8012B730: ;
    r0 = r0 & 0xFF;
    if ((s32)r24 == (s32)0x2) {
        r3 = -0x1;
        goto L_8012B764;
    }
    r4 = r31;
    r3 = 0x0;
    fn_8018D998();
    fn_8018D928();
    if ((u32)r3 == (u32)0x0) {
        r3 = -0x1;
        goto L_8012B764;
    }
    r3 = *(u32*)((u8*)r3 + 0x30);
L_8012B764: ;
    r0 = r3 + (0x1 << 16);
    if ((u32)r0 == (u32)0xffff) {
        r3 = 0x0;
        goto L_8012BAAC;
    }
    fn_8018F6F4();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        goto L_8012BAAC;
    }
    fn_8018F5E4();
    f30 = f1;
    r3 = (u32)sp + 0x68;
    r4 = (u32)sp + 0x5c;
    r5 = 0x0;
    fn_8010F320();
    if ((s32)r3 != (s32)0x0) {
        r3 = 0x0;
        goto L_8012BAAC;
    }
    r3 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0x30) = r0;
    if ((s32)r26 >= (s32)0x0) {
        if ((s32)r26 < (s32)0x2) goto L_8012B7DC;
    }
    r0 = 0x0;
    goto L_8012B7EC;
L_8012B7DC: ;
    r0 = r26 << 2;
    r3 = (u32)sp + 0x2c;
    r30 = *(u32*)(r3 + r0);
    r0 = 0x1;
L_8012B7EC: ;
    r0 = r0 & 0xFF;
    if ((s32)r26 == (s32)0x2) {
        r3 = -0x1;
        goto L_8012B820;
    }
    r4 = r30;
    r3 = 0x0;
    fn_8018D998();
    fn_8018D928();
    if ((u32)r3 == (u32)0x0) {
        r3 = -0x1;
        goto L_8012B820;
    }
    r3 = *(u32*)((u8*)r3 + 0x30);
L_8012B820: ;
    r0 = r3 + (0x1 << 16);
    if ((u32)r0 == (u32)0xffff) {
        r3 = 0x0;
        goto L_8012BAAC;
    }
    fn_8018F6F4();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        goto L_8012BAAC;
    }
    fn_8018F5E4();
    r3 = *(u32*)lbl_8047D030;
    f31 = f1;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0x40) = r0;
    if (((s32)r24 >= (s32)0x0) && ((s32)r24 < (s32)0x2)) {
        r0 = r24 << 2;
        r3 = (u32)sp + 0x3c;
        r25 = *(u32*)(r3 + r0);
    }
    r4 = r25;
    r3 = 0x0;
    fn_8018D998();
    fn_8018D928();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        goto L_8012BAAC;
    }
    r4 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    r29 = *(u32*)((u8*)r3 + 0x50);
    *(u32*)(sp + 0x18) = r0;
    if (((s32)r24 >= (s32)0x0) && ((s32)r24 < (s32)0x2)) {
        r0 = r24 << 2;
        r3 = (u32)sp + 0x14;
        r28 = *(u32*)(r3 + r0);
    }
    r4 = r28;
    r3 = 0x0;
    fn_800F9318();
    r4 = (u32)sp + 0x68;
    fn_800E3D98();
    r3 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0x10) = r0;
    if (((s32)r26 >= (s32)0x0) && ((s32)r26 < (s32)0x2)) {
        r0 = r26 << 2;
        r3 = (u32)sp + 0xc;
        r27 = *(u32*)(r3 + r0);
    }
    r4 = r27;
    r3 = 0x0;
    fn_800F9318();
    r4 = (u32)sp + 0x5c;
    fn_800E3D98();
    r3 = (u32)sp + 0x5c;
    r4 = (u32)sp + 0x68;
    r5 = (u32)sp + 0x50;
    ((void(*)(void))fn_800A3A9C)();
    f3 = *(f32*)(sp + 0x68);
    f5 = f30 + f31;
    f0 = *(f32*)(sp + 0x5C);
    f2 = *(f32*)(sp + 0x70);
    f1 = *(f32*)(sp + 0x64);
    f3 = f3 - f0;
    f0 = *(f32*)lbl_8047D038;
    f1 = f2 - f1;
    f2 = f3 * f3;
    f1 = f1 * f1;
    f4 = f2 + f1;
    if (f4 > f0) {
        /* frsqrte f1, f4 */;
        f3 = *(f64*)lbl_8047D048;
        f2 = *(f64*)lbl_8047D050;
        f0 = f1 * f1;
        f1 = f3 * f1;
        f0 = -(f4 * f0 - f2);
        f1 = f1 * f0;
        f0 = f1 * f1;
        f1 = f3 * f1;
        f0 = -(f4 * f0 - f2);
        f1 = f1 * f0;
        f0 = f1 * f1;
        f1 = f3 * f1;
        f0 = -(f4 * f0 - f2);
        f0 = f1 * f0;
        f4 = f4 * f0;
        f4 = (f32)f4;
        goto L_8012BA30;
    }
    f0 = *(f64*)lbl_8047D058;
    if (f4 < f0) {
        r3 = (u32)lbl_80478AC0;
        f4 = *(f32*)lbl_80478AC0;
        goto L_8012BA30;
    }
    *(f32*)(sp + 0x8) = f4;
    r0 = (0x7f80 << 16);
    r4 = *(u32*)(sp + 0x8);
    r3 = r4 & 0x7F800000;
    if ((s32)r3 != (s32)r0) {
        if ((s32)r3 < (s32)r0) {
            if ((s32)r3 == (s32)0x0) goto L_8012BA04;
        }
        goto L_8012BA1C;
    }
    r0 = r4 & 0x7FFFFF;
    if ((s32)r3 != (s32)0x0) {
        r0 = 0x1;
        goto L_8012BA20;
    }
    r0 = 0x2;
    goto L_8012BA20;
L_8012BA04: ;
    r0 = r4 & 0x7FFFFF;
    if ((s32)r3 != (s32)0x0) {
        r0 = 0x5;
        goto L_8012BA20;
    }
    r0 = 0x3;
    goto L_8012BA20;
L_8012BA1C: ;
    r0 = 0x4;
L_8012BA20: ;
    if ((s32)r0 == (s32)0x1) {
        r3 = (u32)lbl_80478AC0;
        f4 = *(f32*)lbl_80478AC0;
    }
L_8012BA30: ;
    f0 = *(f32*)lbl_8047D038;
    /* cror eq, lt, eq */;
    if (f4 == f0) {
        r3 = 0x1;
        goto L_8012BAAC;
    }
    /* cror eq, lt, eq */;
    if (f4 == f5) {
        r3 = 0x1;
        goto L_8012BAAC;
    }
    f1 = f4 - f5;
    f0 = *(f32*)lbl_8047D060;
    r3 = (u32)sp + 0x50;
    r4 = (u32)sp + 0x44;
    f0 = f1 - f0;
    f1 = f0 / f4;
    fn_800A3AC0();
    r3 = (u32)sp + 0x68;
    r4 = (u32)sp + 0x44;
    r5 = (u32)sp + 0x5c;
    fn_800A3A78();
    r3 = r29;
    r4 = (u32)sp + 0x68;
    r5 = (u32)sp + 0x5c;
    r6 = 0x0;
    fn_8010FDF8();
    r4 = 0x6 - r3;
    r0 = r4 | r0;
    r3 = (u32)r0 >> 31;
L_8012BAAC: ;
    /* psq_l f31, 0xb8((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0xB0);
    /* psq_l f30, 0xa8((u32)sp), 0, qr0 */;
    f30 = *(f64*)(sp + 0xA0);
    return;
}
/* 0x8012BAD0 | 0x20 */
void fn_8012BAD0(u32 a, u32 b, u32 c, u32 d, u32 e) {
    u8* base = lbl_80426BD0;
    *(u32*)(base + 0x18C) = a;
    *(u32*)(base + 0x190) = b;
    *(u32*)(base + 0x194) = c;
    *(u32*)(base + 0x198) = d;
    *(u32*)(base + 0x19C) = e;
}
/* 0x8012BAF0 | 0xB8 */
void fn_8012BAF0(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;
    r0 = r3 & 0xFF;
    r31 = r5;
    if ((s32)r0 != (s32)0x2) {
        if ((s32)r0 < (s32)0x2) {
            if ((s32)r0 < (s32)0x1) return;
        }
        if ((s32)r0 >= (s32)0x4) return;
        r3 = (u32)&lbl_80426BD0;
        r5 = 0xd0;
        r3 = (u32)&lbl_80426BD0;
        r3 = r3 + 0x1a0;
        memcpy((void*)r3, (const void*)r4, (u32)r5);
        r3 = (u32)&lbl_80426BD0;
        r3 = (u32)&lbl_80426BD0;
        *(u32*)((u8*)r3 + 0x410) = r31;
        return;
    }
    r3 = (u32)&lbl_80426BD0;
    r5 = 0xd0;
    r3 = (u32)&lbl_80426BD0;
    r3 = r3 + 0x340;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r3 = (u32)&lbl_80426BD0;
    r3 = (u32)&lbl_80426BD0;
    *(u32*)((u8*)r3 + 0x418) = r31;
    return;
}
/* 0x8012BBA8 | 0xFC */
void fn_8012BBA8(void) {
    extern u8 lbl_8047D030[];
    extern u8 lbl_8047D034[];
    extern void fn_8018C69C();
    extern void fn_8018C7C8();
    extern void fn_8018CA20();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r31 = 0x0;
    r30 = 0x0;
    r29 = 0x0;
    do {
        if ((s32)r29 >= (s32)0x0 && (s32)r29 < (s32)0x2) {
        r3 = (u32)&lbl_80426BD0;
        r0 = (u32)&lbl_80426BD0;
        r3 = r0 + r30;
        r0 = *(u16*)((u8*)r3 + 0x4);
        r0 = r0 & 0x1;
        if ((s32)r29 == (s32)0x2) {
            r0 = 0x0;
        } else {
        r0 = 0x1;
        }
        } else {
        r0 = 0x0;
        }
        r0 = r0 & 0xFF;
        if ((s32)r29 != (s32)0x2) {
            r3 = *(u32*)lbl_8047D030;
            r0 = *(u32*)lbl_8047D034;
            *(u32*)(sp + 0xC) = r0;
            if (((s32)r29 >= (s32)0x0) && ((s32)r29 < (s32)0x2)) {
                r3 = (u32)sp + 0x8;
                r28 = *(u32*)(r3 + r31);
            }
            r4 = r28;
            r3 = 0x0;
            r5 = 0x1;
            fn_8018CA20();
            r4 = r28;
            r3 = 0x0;
            r5 = 0x700;
            fn_8018C7C8();
            r3 = (0x8000 << 16);
            r4 = r28;
            r5 = r3 + 0x8;
            r3 = 0x0;
            fn_8018C69C();
        }
        r29 = r29 + 0x1;
        r30 = r30 + 0x20;
        r31 = r31 + 0x4;
    } while ((s32)r29 < (s32)0x2);
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
/* 0x8012BCA4 | 0x13C */
void fn_8012BCA4(void) {
    extern u8 lbl_8047D030[];
    extern u8 lbl_8047D034[];
    extern u8 lbl_8047D038[];
    extern void fn_800F9318();
    extern void fn_8012C660();
    extern void fn_8018C69C();
    extern void fn_8018C7C8();
    extern void fn_8018CA20();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    r30 = 0x0;
    r28 = 0x0;
    r27 = 0x0;
    do {
        if ((s32)r30 >= (s32)0x0 && (s32)r30 < (s32)0x2) {
        r3 = (u32)&lbl_80426BD0;
        r0 = (u32)&lbl_80426BD0;
        r3 = r0 + r27;
        r0 = *(u16*)((u8*)r3 + 0x4);
        r0 = r0 & 0x1;
        if ((s32)r30 == (s32)0x2) {
            r0 = 0x0;
        } else {
        r0 = 0x1;
        }
        } else {
        r0 = 0x0;
        }
        r0 = r0 & 0xFF;
        if ((s32)r30 != (s32)0x2) {
            r3 = *(u32*)lbl_8047D030;
            r0 = *(u32*)lbl_8047D034;
            *(u32*)(sp + 0xC) = r0;
            *(u32*)(sp + 0x1C) = r0;
            if (((s32)r30 >= (s32)0x0) && ((s32)r30 < (s32)0x2)) {
                r3 = (u32)sp + 0x18;
                r29 = *(u32*)(r3 + r28);
            }
            r3 = *(u32*)(sp + 0x8);
            r0 = *(u32*)(sp + 0xC);
            *(u32*)(sp + 0x14) = r0;
            if (((s32)r30 >= (s32)0x0) && ((s32)r30 < (s32)0x2)) {
                r3 = (u32)sp + 0x10;
                r31 = *(u32*)(r3 + r28);
            }
            r4 = r31;
            r3 = 0x0;
            fn_800F9318();
            f1 = *(f32*)lbl_8047D038;
            r4 = r30;
            fn_8012C660();
            r3 = (0x8000 << 16);
            r4 = r29;
            r5 = r3 + 0x8;
            r3 = 0x0;
            fn_8018C7C8();
            r4 = r29;
            r3 = 0x0;
            r5 = 0x100;
            fn_8018C69C();
            r4 = r29;
            r3 = 0x0;
            r5 = 0x400;
            fn_8018C69C();
            r4 = r29;
            r3 = 0x0;
            r5 = 0x0;
            fn_8018CA20();
        }
        r30 = r30 + 0x1;
        r27 = r27 + 0x20;
        r28 = r28 + 0x4;
    } while ((s32)r30 < (s32)0x2);
    return;
}
/* 0x8012BDE0 | 0xD4 */
void fn_8012BDE0(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    r5 = (u32)&lbl_80426BD0;
    r7 = 0x0;
    r5 = (u32)&lbl_80426BD0;
    r0 = *(u32*)((u8*)r5 + 0x140);
    if ((u32)r0 != (u32)0x0) {
        r5 = r5 + 0x8;
        r7 = 0x1;
        r0 = *(u32*)((u8*)r5 + 0x140);
        if ((u32)r0 != (u32)0x0) {
            r0 = *(u32*)((u8*)r5 + 0x148);
            r7 = 0x2;
            r5 = r5 + 0x8;
            if ((u32)r0 != (u32)0x0) {
                r0 = *(u32*)((u8*)r5 + 0x148);
                r7 = 0x3;
                r5 = r5 + 0x8;
                if ((u32)r0 != (u32)0x0) {
                    r0 = *(u32*)((u8*)r5 + 0x148);
                    r7 = 0x4;
                    r5 = r5 + 0x8;
                    if ((u32)r0 != (u32)0x0) {
                        r0 = *(u32*)((u8*)r5 + 0x148);
                        r7 = 0x5;
                        r5 = r5 + 0x8;
                        if ((u32)r0 != (u32)0x0) {
                            r0 = *(u32*)((u8*)r5 + 0x148);
                            r7 = 0x6;
                            r5 = r5 + 0x8;
                            if ((u32)r0 != (u32)0x0) {
                                r0 = *(u32*)((u8*)r5 + 0x148);
                                r7 = 0x7;
                                if ((u32)r0 != (u32)0x0) {
                                    r7 = 0x8;
    }
    }
    }
    }
    }
    }
    }
    }
    if ((s32)r7 >= (s32)0x8) {
        r3 = -0x1;
        return;
    }
    r5 = (u32)&lbl_80426BD0;
    r6 = r7 << 3;
    r0 = (u32)&lbl_80426BD0;
    r5 = r0 + r6;
    *(u32*)((u8*)r5 + 0x140) = r3;
    r3 = r7;
    *(u32*)((u8*)r5 + 0x144) = r4;
    return;
}
/* 0x8012BEB4 | 0x200 */
void fn_8012BEB4(void) {
    extern u8 lbl_8047D038[];
    extern u8 lbl_8047D040[];
    extern u8 lbl_8047D068[];
    extern void fn_800D3088();
    extern void fn_800E3D98();
    extern void fn_800EE150();
    extern void fn_800EE3BC();
    extern void fn_800EE828();
    u8 sp[0x80];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    *(f64*)(sp + 0x70) = f31;
    /* psq_st f31, 0x78((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x60) = f30;
    /* psq_st f30, 0x68((u32)sp), 0, qr0 */;
    r23 = r3;
    r24 = r4;
    r26 = r5;
    r25 = r6;
    fn_800D3088();
    r0 = (0x4330 << 16);
    f1 = *(f64*)lbl_8047D068;
    r3 = r24;
    *(u32*)(sp + 0x30) = r0;
    r4 = (u32)sp + 0x24;
    f0 = *(f64*)(sp + 0x30);
    f30 = f0 - f1;
    fn_800E3D98();
    r28 = (u32)sp + 0x8;
    f31 = *(f32*)lbl_8047D038;
    r31 = r28;
    r30 = r23;
    r29 = r26;
    r26 = 0x0;
    do {
        *(f32*)((u8*)r30 + 0x10) = f31;
        *(f32*)((u8*)r30 + 0x14) = f31;
        *(f32*)((u8*)r30 + 0x18) = f31;
        r4 = *(u32*)((u8*)r29 + 0x0);
        *(f32*)((u8*)r31 + 0x0) = f31;
        if ((s32)r4 >= (s32)0x0) {
            r3 = r24;
            fn_800EE150();
            if ((s32)r4 != (s32)0x0) {
                r4 = (u32)sp + 0x18;
                r5 = 0x0;
                r6 = 0x0;
                fn_800EE3BC();
                r3 = r27;
                fn_800EE828();
                f1 = *(f32*)(sp + 0x1C);
                f0 = *(f32*)(sp + 0x28);
                r3 = *(u32*)(sp + 0x18);
                r0 = *(u32*)(sp + 0x1C);
                f0 = f1 - f0;
                *(u32*)((u8*)r30 + 0x10) = r3;
                *(u32*)((u8*)r30 + 0x14) = r0;
                r0 = *(u32*)(sp + 0x20);
                *(f32*)((u8*)r31 + 0x0) = f0;
                *(u32*)((u8*)r30 + 0x18) = r0;
        }
        }
        r26 = r26 + 0x1;
        r30 = r30 + 0xc;
        r29 = r29 + 0x4;
        r31 = r31 + 0x4;
    } while ((s32)r26 < (s32)0x4);
    f2 = *(f32*)lbl_8047D040;
    f3 = *(f32*)((u8*)r25 + 0x0);
    f0 = *(f32*)lbl_8047D038;
    /* cror eq, gt, eq */;
    if (f3 == f2 && f1 < f2) {
            f1 = f3 - f1;
            f1 = f1 / f30;
            *(f32*)((u8*)r23 + 0x0) = f1;
    } else {
    *(f32*)((u8*)r23 + 0x0) = f0;
    }
    f3 = *(f32*)((u8*)r25 + 0x4);
    /* cror eq, gt, eq */;
    f1 = *(f32*)(sp + 0xC);
    if (f3 == f2 && f1 < f2) {
            f1 = f3 - f1;
            f1 = f1 / f30;
            *(f32*)((u8*)r23 + 0x4) = f1;
    } else {
    *(f32*)((u8*)r23 + 0x4) = f0;
    }
    f3 = *(f32*)((u8*)r25 + 0x8);
    /* cror eq, gt, eq */;
    f1 = *(f32*)((u8*)r28 + 0x8);
    if (f3 == f2 && f1 < f2) {
            f1 = f3 - f1;
            f1 = f1 / f30;
            *(f32*)((u8*)r23 + 0x8) = f1;
    } else {
    *(f32*)((u8*)r23 + 0x8) = f0;
    }
    f3 = *(f32*)((u8*)r25 + 0xC);
    /* cror eq, gt, eq */;
    f1 = *(f32*)((u8*)r28 + 0xC);
    if (f3 == f2 && f1 < f2) {
            f1 = f3 - f1;
            f1 = f1 / f30;
            *(f32*)((u8*)r23 + 0xC) = f1;
    } else {
    *(f32*)((u8*)r23 + 0xC) = f0;
    }
    f1 = *(f32*)(sp + 0x8);
    f0 = *(f32*)(sp + 0xC);
    *(f32*)((u8*)r25 + 0x0) = f1;
    f1 = *(f32*)(sp + 0x10);
    *(f32*)((u8*)r25 + 0x4) = f0;
    f0 = *(f32*)(sp + 0x14);
    *(f32*)((u8*)r25 + 0x8) = f1;
    *(f32*)((u8*)r25 + 0xC) = f0;
    /* psq_l f31, 0x78((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x70);
    /* psq_l f30, 0x68((u32)sp), 0, qr0 */;
    f30 = *(f64*)(sp + 0x60);
    return;
}
/* 0x8012C0B4 | 0x48C */
void fn_8012C0B4(void) {
    extern u8 lbl_8047D030[];
    extern u8 lbl_8047D034[];
    extern u8 lbl_8047D038[];
    extern u8 lbl_8047D070[];
    extern u8 lbl_8047D074[];
    extern u8 lbl_8047D078[];
    extern u8 lbl_8047D07C[];
    extern u8 lbl_8047D080[];
    extern void fn_800A3A78();
    extern void fn_800CDBE0();
    extern void fn_800CE148();
    extern void fn_800F7434();
    extern void fn_800F7C8C();
    extern void fn_800F7D38();
    extern void fn_800F9318();
    extern void fn_80111B9C();
    extern void fn_80116164();
    extern void fn_8012C660();
    extern void fn_801812E8();
    extern void fn_80183688();
    extern void fn_80183730();
    extern void fn_8018397C();
    extern void fn_8018790C();
    extern void fn_80187D48();
    extern void fn_80189490();
    extern void fn_8018BA04();
    extern void fn_8018C69C();
    extern void fn_8018C7C8();
    extern void fn_8018CA20();
    extern void fn_8018CD08();
    extern void fn_8018D7D0();
    extern void fn_8018D928();
    extern void fn_8018D998();
    extern void fn_8018FC50();
    extern void fn_8018FCBC();
    u8 sp[0x150];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    r4 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0x34) = r0;
    if (((s32)r3 >= (s32)0x0) && ((s32)r3 < (s32)0x2)) {
        r0 = r3 << 2;
        r3 = (u32)sp + 0x30;
        r26 = *(u32*)(r3 + r0);
    }
    f1 = *(f32*)lbl_8047D070;
    r4 = r26;
    f2 = *(f32*)lbl_8047D074;
    r3 = 0x0;
    fn_8018CD08();
    if ((s32)r3 == (s32)0x2) {
        r4 = (u32)&lbl_80426BD0;
        r3 = *(u32*)lbl_8047D030;
        r4 = (u32)&lbl_80426BD0;
        r0 = *(u32*)lbl_8047D034;
        r4 = *(u32*)((u8*)r4 + 0x0);
        *(u32*)(sp + 0x2C) = r0;
        if (((s32)r4 >= (s32)0x0) && ((s32)r4 < (s32)0x2)) {
            r0 = r4 << 2;
            r3 = (u32)sp + 0x28;
            r24 = *(u32*)(r3 + r0);
        }
        r4 = r24;
        r3 = 0x0;
        fn_8018D998();
        fn_8018D928();
        if ((s32)r4 != (s32)0x2) {
            fn_8018FCBC();
            r25 = r3;
            r3 = r24;
            fn_8018FC50();
            r4 = *(u32*)((u8*)r25 + 0x0);
            r24 = r3;
            r0 = *(u32*)((u8*)r25 + 0x4);
            f0 = *(f32*)lbl_8047D078;
            *(u32*)(sp + 0x3C) = r0;
            f1 = *(f32*)(sp + 0x3C);
            f0 = f1 + f0;
            r0 = *(u32*)((u8*)r25 + 0x8);
            *(u32*)(sp + 0x40) = r0;
            *(f32*)(sp + 0x3C) = f0;
            f1 = *(f32*)((u8*)r3 + 0x4);
            fn_800CE148();
            f2 = (f32)f1;
            f1 = *(f32*)lbl_8047D07C;
            f0 = *(f32*)lbl_8047D038;
            f1 = f1 * f2;
            *(f32*)(sp + 0x48) = f0;
            *(f32*)(sp + 0x44) = f1;
            f1 = *(f32*)((u8*)r24 + 0x4);
            fn_800CDBE0();
            f1 = (f32)f1;
            f0 = *(f32*)lbl_8047D07C;
            r4 = (u32)sp + 0x44;
            r3 = (u32)sp + 0x38;
            r5 = r4;
            f0 = f0 * f1;
            *(f32*)(sp + 0x4C) = f0;
            fn_800A3A78();
            r3 = (u32)sp + 0x38;
            r4 = (u32)sp + 0x44;
            r5 = (u32)sp + 0x5c;
            fn_80111B9C();
            r30 = r3;
        }
        r3 = (u32)&lbl_80426BD0;
        r4 = (u32)sp + 0x5c;
        r3 = (u32)&lbl_80426BD0;
        r5 = 0xd0;
        r3 = r3 + 0x1a0;
        memcpy((void*)r3, (const void*)r4, (u32)r5);
        r3 = (u32)&lbl_80426BD0;
        r3 = (u32)&lbl_80426BD0;
        *(u32*)((u8*)r3 + 0x410) = r30;
        if ((s32)r30 != (s32)0x0) {
            r3 = 0x0;
            r4 = 0x64;
            fn_8018790C();
            r3 = 0x1;
            return;
        }
        r3 = 0x0;
        return;
    }
    r3 = 0x1;
    r4 = 0x0;
    r5 = 0x0;
    fn_800F7D38();
    r3 = 0x1;
    r4 = 0x0;
    r5 = 0x0;
    fn_800F7C8C();
    r29 = 0x0;
    r25 = 0x0;
    r24 = r29;
    do {
        if ((s32)r29 >= (s32)0x0) {
            if ((s32)r29 < (s32)0x2) goto L_8012C280;
        }
        r0 = 0x0;
        goto L_8012C2A4;
    L_8012C280: ;
        r3 = (u32)&lbl_80426BD0;
        r0 = (u32)&lbl_80426BD0;
        r3 = r0 + r24;
        r0 = *(u16*)((u8*)r3 + 0x4);
        r0 = r0 & 0x1;
        if ((s32)r29 == (s32)0x2) {
            r0 = 0x0;
            goto L_8012C2A4;
        }
        r0 = 0x1;
    L_8012C2A4: ;
        r0 = r0 & 0xFF;
        if ((s32)r29 != (s32)0x2) {
            r3 = *(u32*)lbl_8047D030;
            r0 = *(u32*)lbl_8047D034;
            *(u32*)(sp + 0xC) = r0;
            *(u32*)(sp + 0x24) = r0;
            if (((s32)r29 >= (s32)0x0) && ((s32)r29 < (s32)0x2)) {
                r3 = (u32)sp + 0x20;
                r30 = *(u32*)(r3 + r25);
            }
            r3 = *(u32*)(sp + 0x8);
            r0 = *(u32*)(sp + 0xC);
            *(u32*)(sp + 0x14) = r0;
            if (((s32)r29 >= (s32)0x0) && ((s32)r29 < (s32)0x2)) {
                r3 = (u32)sp + 0x10;
                r27 = *(u32*)(r3 + r25);
            }
            r4 = r27;
            r3 = 0x0;
            fn_800F9318();
            f1 = *(f32*)lbl_8047D038;
            r4 = r29;
            fn_8012C660();
            r3 = (0x8000 << 16);
            r4 = r30;
            r5 = r3 + 0x8;
            r3 = 0x0;
            fn_8018C7C8();
            r4 = r30;
            r3 = 0x0;
            r5 = 0x100;
            fn_8018C69C();
            r4 = r30;
            r3 = 0x0;
            r5 = 0x400;
            fn_8018C69C();
            r4 = r30;
            r3 = 0x0;
            r5 = 0x0;
            fn_8018CA20();
        }
        r29 = r29 + 0x1;
        r24 = r24 + 0x20;
        r25 = r25 + 0x4;
    } while ((s32)r29 < (s32)0x2);
    r3 = *(u32*)((u8*)r31 + 0x28);
    r5 = (u32)sp + 0x50;
    r4 = *(u32*)((u8*)r31 + 0x2C);
    fn_8018BA04();
    f1 = *(f32*)(sp + 0x50);
    r4 = r26;
    f2 = *(f32*)(sp + 0x54);
    r3 = 0x0;
    f3 = *(f32*)(sp + 0x58);
    f4 = *(f32*)lbl_8047D080;
    fn_80187D48();
    r3 = *(u32*)((u8*)r31 + 0x28);
    r4 = *(u32*)((u8*)r31 + 0x2C);
    fn_8018D7D0();
    r0 = r3 & 0xFF;
    if ((s32)r29 != (s32)0x2) {
        r3 = *(u32*)((u8*)r31 + 0x28);
        r5 = 0x2;
        r4 = *(u32*)((u8*)r31 + 0x2C);
        fn_80116164();
    } else {
        r3 = *(u32*)((u8*)r31 + 0x28);
        r4 = *(u32*)((u8*)r31 + 0x2C);
        fn_8018D998();
        fn_80183730();
        r3 = *(u32*)((u8*)r31 + 0x28);
        r4 = *(u32*)((u8*)r31 + 0x2C);
        fn_8018397C();
        if ((s32)r29 != (s32)0x2) {
            r3 = *(u32*)((u8*)r31 + 0x28);
            r5 = 0x1;
            r4 = *(u32*)((u8*)r31 + 0x2C);
            fn_801812E8();
            r5 = *(u32*)((u8*)r31 + 0x28);
            r3 = r24;
            r6 = *(u32*)((u8*)r31 + 0x2C);
            r4 = 0x4;
            r7 = 0x0;
            r8 = 0x0;
            fn_800F7434();
            r3 = *(u32*)((u8*)r31 + 0x28);
            r5 = 0x0;
            r4 = *(u32*)((u8*)r31 + 0x2C);
            fn_801812E8();
            r3 = *(u32*)((u8*)r31 + 0x28);
            r4 = *(u32*)((u8*)r31 + 0x2C);
            fn_80189490();
        }
        r3 = *(u32*)((u8*)r31 + 0x28);
        r4 = *(u32*)((u8*)r31 + 0x2C);
        fn_8018D998();
        fn_80183688();
    }
    r24 = 0x0;
    r26 = 0x0;
    r27 = 0x0;
    do {
        if ((s32)r24 >= (s32)0x0) {
            if ((s32)r24 < (s32)0x2) goto L_8012C46C;
        }
        r0 = 0x0;
        goto L_8012C490;
    L_8012C46C: ;
        r3 = (u32)&lbl_80426BD0;
        r0 = (u32)&lbl_80426BD0;
        r3 = r0 + r27;
        r0 = *(u16*)((u8*)r3 + 0x4);
        r0 = r0 & 0x1;
        if ((s32)r24 == (s32)0x2) {
            r0 = 0x0;
            goto L_8012C490;
        }
        r0 = 0x1;
    L_8012C490: ;
        r0 = r0 & 0xFF;
        if ((s32)r24 != (s32)0x2) {
            r3 = *(u32*)lbl_8047D030;
            r0 = *(u32*)lbl_8047D034;
            *(u32*)(sp + 0x1C) = r0;
            if (((s32)r24 >= (s32)0x0) && ((s32)r24 < (s32)0x2)) {
                r3 = (u32)sp + 0x18;
                r28 = *(u32*)(r3 + r26);
            }
            r4 = r28;
            r3 = 0x0;
            r5 = 0x1;
            fn_8018CA20();
            r4 = r28;
            r3 = 0x0;
            r5 = 0x700;
            fn_8018C7C8();
            r3 = (0x8000 << 16);
            r4 = r28;
            r5 = r3 + 0x8;
            r3 = 0x0;
            fn_8018C69C();
        }
        r24 = r24 + 0x1;
        r27 = r27 + 0x20;
        r26 = r26 + 0x4;
    } while ((s32)r24 < (s32)0x2);
    r3 = 0x1;
    r4 = 0x0;
    r5 = 0x0;
    fn_800F7D38();
    r3 = 0x1;
    r4 = 0x0;
    r5 = 0x0;
    fn_800F7C8C();
    r3 = 0x1;
    return;
}
/* 0x8012C540 | 0x120 */
void fn_8012C540(void) {
    extern u8 lbl_8047D030[];
    extern u8 lbl_8047D034[];
    extern u8 lbl_8047D038[];
    extern u8 lbl_8047D078[];
    extern u8 lbl_8047D07C[];
    extern void fn_800A3A78();
    extern void fn_800CDBE0();
    extern void fn_800CE148();
    extern void fn_80111B9C();
    extern void fn_8018D928();
    extern void fn_8018D998();
    extern void fn_8018FC50();
    extern void fn_8018FCBC();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    r6 = (u32)&lbl_80426BD0;
    r6 = (u32)&lbl_80426BD0;
    r29 = r3;
    r6 = *(u32*)((u8*)r6 + 0x0);
    r5 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0xC) = r0;
    if (((s32)r6 >= (s32)0x0) && ((s32)r6 < (s32)0x2)) {
        r0 = r6 << 2;
        r3 = (u32)sp + 0x8;
        r4 = *(u32*)(r3 + r0);
    }
    r3 = 0x0;
    fn_8018D998();
    fn_8018D928();
    if ((s32)r6 != (s32)0x2) {
        fn_8018FCBC();
        r30 = r3;
        r3 = r31;
        fn_8018FC50();
        r4 = *(u32*)((u8*)r30 + 0x0);
        r31 = r3;
        r0 = *(u32*)((u8*)r30 + 0x4);
        f0 = *(f32*)lbl_8047D078;
        *(u32*)(sp + 0x20) = r0;
        f1 = *(f32*)(sp + 0x20);
        f0 = f1 + f0;
        r0 = *(u32*)((u8*)r30 + 0x8);
        *(u32*)(sp + 0x24) = r0;
        *(f32*)(sp + 0x20) = f0;
        f1 = *(f32*)((u8*)r3 + 0x4);
        fn_800CE148();
        f2 = (f32)f1;
        f1 = *(f32*)lbl_8047D07C;
        f0 = *(f32*)lbl_8047D038;
        f1 = f1 * f2;
        *(f32*)(sp + 0x14) = f0;
        *(f32*)(sp + 0x10) = f1;
        f1 = *(f32*)((u8*)r31 + 0x4);
        fn_800CDBE0();
        f1 = (f32)f1;
        f0 = *(f32*)lbl_8047D07C;
        r4 = (u32)sp + 0x10;
        r3 = (u32)sp + 0x1c;
        r5 = r4;
        f0 = f0 * f1;
        *(f32*)(sp + 0x18) = f0;
        fn_800A3A78();
        r5 = r29;
        r3 = (u32)sp + 0x1c;
        r4 = (u32)sp + 0x10;
        fn_80111B9C();
        r30 = r3;
    }
    r3 = r30;
    r31 = *(u32*)(sp + 0x3C);
    r30 = *(u32*)(sp + 0x38);
    r29 = *(u32*)(sp + 0x34);
    return;
}
/* 0x8012C660 | 0x424 */
void fn_8012C660(void) {
    extern u8 lbl_8047D030[];
    extern u8 lbl_8047D034[];
    extern u8 lbl_8047D038[];
    extern u8 lbl_8047D040[];
    extern u8 lbl_8047D080[];
    extern u8 lbl_8047D084[];
    extern u8 lbl_8047D088[];
    extern u8 lbl_8047D08C[];
    extern u8 lbl_8047D090[];
    extern void fn_800EC4D0();
    extern void fn_800EC53C();
    extern void fn_800EC578();
    extern void fn_800EC5B8();
    extern void fn_800EC5FC();
    extern void fn_800EC9DC();
    extern void fn_800ECA78();
    extern void fn_800ECCA8();
    extern void fn_8018D928();
    extern void fn_8018D998();
    extern void fn_8018F4C8();
    extern void fn_8018F6F4();
    u8 sp[0x60];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    *(f64*)(sp + 0x50) = f31;
    /* psq_st f31, 0x58((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x40) = f30;
    /* psq_st f30, 0x48((u32)sp), 0, qr0 */;
    r5 = *(u32*)lbl_8047D030;
    f31 = f1;
    r0 = *(u32*)lbl_8047D034;
    r31 = r3;
    *(u32*)(sp + 0x30) = r0;
    if ((s32)r4 >= (s32)0x0) {
        if ((s32)r4 < (s32)0x2) goto L_8012C6B4;
    }
    r0 = 0x0;
    goto L_8012C6C4;
L_8012C6B4: ;
    r0 = r4 << 2;
    r3 = (u32)sp + 0x2c;
    r4 = *(u32*)(r3 + r0);
    r0 = 0x1;
L_8012C6C4: ;
    r0 = r0 & 0xFF;
    if ((s32)r4 == (s32)0x2) {
        r3 = -0x1;
        goto L_8012C6F4;
    }
    r3 = 0x0;
    fn_8018D998();
    fn_8018D928();
    if ((u32)r3 == (u32)0x0) {
        r3 = -0x1;
        goto L_8012C6F4;
    }
    r3 = *(u32*)((u8*)r3 + 0x30);
L_8012C6F4: ;
    fn_8018F6F4();
    r5 = (u32)sp + 0x20;
    r30 = r3;
    r6 = (u32)sp + 0x8;
    r4 = 0x1;
    fn_8018F4C8();
    r3 = r30;
    r5 = (u32)sp + 0x1c;
    r6 = (u32)sp + 0x8;
    r4 = 0x2;
    fn_8018F4C8();
    r3 = r30;
    r5 = (u32)sp + 0x18;
    r6 = (u32)sp + 0x8;
    r4 = 0x3;
    fn_8018F4C8();
    r3 = r30;
    r5 = (u32)sp + 0x14;
    r6 = (u32)sp + 0x8;
    r4 = 0x4;
    fn_8018F4C8();
    f0 = *(f32*)lbl_8047D084;
    if (f31 > f0) {
        f31 = f0;
    }
    r3 = r31;
    r4 = (u32)sp + 0x28;
    r5 = (u32)sp + 0x24;
    fn_800EC578();
    f0 = *(f32*)lbl_8047D088;
    if (f31 < f0) {
        r3 = *(u32*)(sp + 0x28);
        r0 = *(u32*)(sp + 0x14);
        if ((s32)r3 == (s32)r0) {
            r0 = *(u32*)(sp + 0x24);
            if ((s32)r0 == (s32)-0x1) goto L_8012C7E0;
        }
        r0 = *(u32*)(sp + 0x24);
        f30 = *(f32*)lbl_8047D038;
        if ((s32)r0 != (s32)-0x1) {
            r3 = r31;
            r4 = (u32)sp + 0x10;
            r5 = (u32)sp + 0xc;
            fn_800EC4D0();
            r3 = r31;
            fn_800EC53C();
            f2 = *(f32*)(sp + 0x10);
            f0 = *(f32*)(sp + 0xC);
            f0 = f2 / f0;
            f30 = f0 * f1;
        }
        r4 = *(u32*)(sp + 0x14);
        r3 = r31;
        fn_800ECCA8();
        f1 = f30;
        r3 = r31;
        fn_800ECA78();
    L_8012C7E0: ;
        f1 = *(f32*)lbl_8047D080;
        r3 = r31;
        fn_800EC9DC();
        goto L_8012CA5C;
    }
    f0 = *(f32*)lbl_8047D038;
    if (f31 < f0) {
        r3 = *(u32*)(sp + 0x28);
        r0 = *(u32*)(sp + 0x14);
        if ((s32)r3 == (s32)r0) {
            r3 = *(u32*)(sp + 0x24);
            r0 = *(u32*)(sp + 0x20);
            if ((s32)r3 == (s32)r0) goto L_8012C870;
        }
        r0 = *(u32*)(sp + 0x24);
        f30 = *(f32*)lbl_8047D038;
        if ((s32)r0 == (s32)-0x1) {
            r3 = r31;
            fn_800EC53C();
            f30 = f1;
        }
        r4 = *(u32*)(sp + 0x14);
        r3 = r31;
        r5 = *(u32*)(sp + 0x20);
        fn_800EC5FC();
        r3 = r31;
        r4 = (u32)sp + 0x10;
        r5 = (u32)sp + 0xc;
        fn_800EC4D0();
        f1 = *(f32*)(sp + 0xC);
        r3 = r31;
        f0 = *(f32*)(sp + 0x10);
        f0 = f1 / f0;
        f1 = f30 * f0;
        fn_800ECA78();
    L_8012C870: ;
        f1 = *(f32*)lbl_8047D088;
        r3 = r31;
        f0 = *(f32*)lbl_8047D08C;
        f1 = f31 - f1;
        f31 = f1 / f0;
        f1 = f31;
        fn_800EC5B8();
        f1 = *(f32*)lbl_8047D040;
        r3 = r31;
        fn_800EC9DC();
        goto L_8012CA5C;
    }
    f0 = *(f32*)lbl_8047D08C;
    if (f31 < f0) {
        r3 = *(u32*)(sp + 0x28);
        r0 = *(u32*)(sp + 0x1C);
        if ((s32)r3 == (s32)r0) {
            r3 = *(u32*)(sp + 0x24);
            r0 = *(u32*)(sp + 0x20);
            if ((s32)r3 == (s32)r0) goto L_8012C91C;
        }
        r0 = *(u32*)(sp + 0x24);
        f30 = *(f32*)lbl_8047D038;
        if ((s32)r0 == (s32)-0x1) {
            r3 = r31;
            fn_800EC53C();
            f30 = f1;
        }
        r4 = *(u32*)(sp + 0x1C);
        r3 = r31;
        r5 = *(u32*)(sp + 0x20);
        fn_800EC5FC();
        r3 = r31;
        r4 = (u32)sp + 0x10;
        r5 = (u32)sp + 0xc;
        fn_800EC4D0();
        f1 = *(f32*)(sp + 0xC);
        r3 = r31;
        f0 = *(f32*)(sp + 0x10);
        f0 = f1 / f0;
        f1 = f30 * f0;
        fn_800ECA78();
    L_8012C91C: ;
        f1 = *(f32*)lbl_8047D090;
        r3 = r31;
        f0 = *(f32*)lbl_8047D080;
        f1 = -(f1 * f31 - f0);
        fn_800EC5B8();
        f1 = *(f32*)lbl_8047D040;
        r3 = r31;
        fn_800EC9DC();
        goto L_8012CA5C;
    }
    f0 = *(f32*)lbl_8047D080;
    if (f31 < f0) {
        r3 = *(u32*)(sp + 0x28);
        r0 = *(u32*)(sp + 0x1C);
        if ((s32)r3 == (s32)r0) {
            r0 = *(u32*)(sp + 0x24);
            if ((s32)r0 == (s32)-0x1) goto L_8012C9B8;
        }
        r0 = *(u32*)(sp + 0x24);
        f30 = *(f32*)lbl_8047D038;
        if ((s32)r0 != (s32)-0x1) {
            r3 = r31;
            r4 = (u32)sp + 0x10;
            r5 = (u32)sp + 0xc;
            fn_800EC4D0();
            r3 = r31;
            fn_800EC53C();
            f2 = *(f32*)(sp + 0x10);
            f0 = *(f32*)(sp + 0xC);
            f0 = f2 / f0;
            f30 = f0 * f1;
        }
        r4 = *(u32*)(sp + 0x1C);
        r3 = r31;
        fn_800ECCA8();
        f1 = f30;
        r3 = r31;
        fn_800ECA78();
    L_8012C9B8: ;
        f1 = *(f32*)lbl_8047D040;
        r3 = r31;
        fn_800EC9DC();
        goto L_8012CA5C;
    }
    r3 = *(u32*)(sp + 0x28);
    r0 = *(u32*)(sp + 0x1C);
    if ((s32)r3 == (s32)r0) {
        r3 = *(u32*)(sp + 0x24);
        r0 = *(u32*)(sp + 0x18);
        if ((s32)r3 == (s32)r0) goto L_8012CA3C;
    }
    r0 = *(u32*)(sp + 0x24);
    f30 = *(f32*)lbl_8047D038;
    if ((s32)r0 == (s32)-0x1) {
        r3 = r31;
        fn_800EC53C();
        f30 = f1;
    }
    r4 = *(u32*)(sp + 0x1C);
    r3 = r31;
    r5 = *(u32*)(sp + 0x18);
    fn_800EC5FC();
    r3 = r31;
    r4 = (u32)sp + 0x10;
    r5 = (u32)sp + 0xc;
    fn_800EC4D0();
    f1 = *(f32*)(sp + 0xC);
    r3 = r31;
    f0 = *(f32*)(sp + 0x10);
    f0 = f1 / f0;
    f1 = f30 * f0;
    fn_800ECA78();
L_8012CA3C: ;
    f0 = *(f32*)lbl_8047D080;
    r3 = r31;
    f31 = f31 - f0;
    f1 = f31;
    fn_800EC5B8();
    f1 = *(f32*)lbl_8047D040;
    r3 = r31;
    fn_800EC9DC();
L_8012CA5C: ;
    /* psq_l f31, 0x58((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x50);
    /* psq_l f30, 0x48((u32)sp), 0, qr0 */;
    f30 = *(f64*)(sp + 0x40);
    r31 = *(u32*)(sp + 0x3C);
    r30 = *(u32*)(sp + 0x38);
    return;
}
/* 0x70 | fn_8012D2BC | generic */
u32 fn_8012D2BC(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    /* refs: lbl_80426BD0, lbl_8047D030, lbl_8047D034 */
    fn_800F9318();
    fn_800E3D6C();
    return 0;
}
/* 0x70 | fn_8012D32C | generic */
u32 fn_8012D32C(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    /* refs: lbl_80426BD0, lbl_8047D030, lbl_8047D034 */
    fn_800F9318();
    fn_800E3D98();
    return 0;
}
/* 0x8012D39C | 0x454 */
void fn_8012D39C(void) {
    extern u8 lbl_80478AC0[];
    extern u8 lbl_8047D038[];
    extern u8 lbl_8047D048[];
    extern u8 lbl_8047D050[];
    extern u8 lbl_8047D058[];
    extern u8 lbl_8047D080[];
    extern u8 lbl_8047D0A8[];
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    f32 f9 = 0.0f;
    f32 f10 = 0.0f;
    f32 f11 = 0.0f;
    f2 = *(f32*)((u8*)r4 + 0x8);
    f0 = *(f32*)((u8*)r3 + 0x8);
    f5 = *(f32*)((u8*)r4 + 0x0);
    f6 = f2 - f0;
    f2 = *(f32*)((u8*)r3 + 0x0);
    f0 = *(f32*)lbl_8047D0A8;
    f8 = f5 - f2;
    f2 = f6 * f6;
    f5 = f8 * f8 + f2;
    if (f5 < f0) {
        r0 = 0x0;
        goto L_8012D4E0;
    }
    f0 = *(f32*)lbl_8047D038;
    if (f5 > f0) {
        /* frsqrte f2, f5 */;
        f4 = *(f64*)lbl_8047D048;
        f3 = *(f64*)lbl_8047D050;
        f0 = f2 * f2;
        f2 = f4 * f2;
        f0 = -(f5 * f0 - f3);
        f2 = f2 * f0;
        f0 = f2 * f2;
        f2 = f4 * f2;
        f0 = -(f5 * f0 - f3);
        f2 = f2 * f0;
        f0 = f2 * f2;
        f2 = f4 * f2;
        f0 = -(f5 * f0 - f3);
        f0 = f2 * f0;
        f5 = f5 * f0;
        f5 = (f32)f5;
        goto L_8012D4AC;
    }
    f0 = *(f64*)lbl_8047D058;
    if (f5 < f0) {
        r8 = (u32)lbl_80478AC0;
        f5 = *(f32*)lbl_80478AC0;
        goto L_8012D4AC;
    }
    *(f32*)(sp + 0x8) = f5;
    r0 = (0x7f80 << 16);
    r9 = *(u32*)(sp + 0x8);
    r8 = r9 & 0x7F800000;
    if ((s32)r8 != (s32)r0) {
        if ((s32)r8 < (s32)r0) {
            if ((s32)r8 == (s32)0x0) goto L_8012D480;
        }
        goto L_8012D498;
    }
    r0 = r9 & 0x7FFFFF;
    if ((s32)r8 != (s32)0x0) {
        r0 = 0x1;
        goto L_8012D49C;
    }
    r0 = 0x2;
    goto L_8012D49C;
L_8012D480: ;
    r0 = r9 & 0x7FFFFF;
    if ((s32)r8 != (s32)0x0) {
        r0 = 0x5;
        goto L_8012D49C;
    }
    r0 = 0x3;
    goto L_8012D49C;
L_8012D498: ;
    r0 = 0x4;
L_8012D49C: ;
    if ((s32)r0 == (s32)0x1) {
        r8 = (u32)lbl_80478AC0;
        f5 = *(f32*)lbl_80478AC0;
    }
L_8012D4AC: ;
    f0 = *(f32*)lbl_8047D080;
    f3 = -f6;
    f2 = *(f32*)((u8*)r4 + 0x0);
    r0 = 0x1;
    f7 = f0 / f5;
    f0 = *(f32*)((u8*)r3 + 0x8);
    f6 = *(f32*)((u8*)r3 + 0x0);
    f5 = *(f32*)((u8*)r4 + 0x8);
    f0 = f2 * f0;
    f3 = f3 * f7;
    f4 = f8 * f7;
    f0 = f6 * f5 - f0;
    f7 = f7 * f0;
L_8012D4E0: ;
    if ((s32)r0 == (s32)0x0) {
        r3 = -0x1;
        return;
    }
    f5 = f4 * f4;
    f6 = *(f32*)lbl_8047D080;
    f2 = -f7;
    f0 = *(f32*)lbl_8047D038;
    f5 = f3 * f3 + f5;
    f9 = f6 / f5;
    f2 = f2 * f9;
    f0 = f3 * f2;
    f2 = f4 * f2;
    if (f9 > f0) {
        /* frsqrte f6, f9 */;
        f8 = *(f64*)lbl_8047D048;
        f7 = *(f64*)lbl_8047D050;
        f5 = f6 * f6;
        f6 = f8 * f6;
        f5 = -(f9 * f5 - f7);
        f6 = f6 * f5;
        f5 = f6 * f6;
        f6 = f8 * f6;
        f5 = -(f9 * f5 - f7);
        f6 = f6 * f5;
        f5 = f6 * f6;
        f6 = f8 * f6;
        f5 = -(f9 * f5 - f7);
        f5 = f6 * f5;
        f6 = f9 * f5;
        f6 = (f32)f6;
        goto L_8012D5F0;
    }
    f5 = *(f64*)lbl_8047D058;
    if (f9 < f5) {
        r3 = (u32)lbl_80478AC0;
        f6 = *(f32*)lbl_80478AC0;
        goto L_8012D5F0;
    }
    *(f32*)(sp + 0x10) = f9;
    r0 = (0x7f80 << 16);
    r4 = *(u32*)(sp + 0x10);
    r3 = r4 & 0x7F800000;
    if ((s32)r3 != (s32)r0) {
        if ((s32)r3 < (s32)r0) {
            if ((s32)r3 == (s32)0x0) goto L_8012D5BC;
        }
        goto L_8012D5D4;
    }
    r0 = r4 & 0x7FFFFF;
    if ((s32)r3 != (s32)0x0) {
        r0 = 0x1;
        goto L_8012D5D8;
    }
    r0 = 0x2;
    goto L_8012D5D8;
L_8012D5BC: ;
    r0 = r4 & 0x7FFFFF;
    if ((s32)r3 != (s32)0x0) {
        r0 = 0x5;
        goto L_8012D5D8;
    }
    r0 = 0x3;
    goto L_8012D5D8;
L_8012D5D4: ;
    r0 = 0x4;
L_8012D5D8: ;
    if ((s32)r0 == (s32)0x1) {
        r3 = (u32)lbl_80478AC0;
        f6 = *(f32*)lbl_80478AC0;
        goto L_8012D5F0;
    }
    f6 = f9;
L_8012D5F0: ;
    f5 = -f3;
    f9 = *(f32*)lbl_8047D0A8;
    f3 = f4 * f6;
    f4 = f5 * f6;
    f6 = f3 * f3;
    f5 = f4 * f4;
    f5 = f6 + f5;
    if (f5 < f9) {
        r3 = 0x0;
        return;
    }
    f8 = *(f32*)((u8*)r5 + 0x0);
    f7 = f1 * f1;
    f6 = *(f32*)((u8*)r5 + 0x8);
    f1 = -f9;
    f8 = f8 - f0;
    f11 = f6 - f2;
    f6 = f4 * f8;
    f6 = f3 * f11 - f6;
    f6 = f6 * f6;
    f10 = f5 * f7 - f6;
    if (f10 < f1) {
        r3 = 0x0;
        return;
    }
    f1 = f4 * f11;
    f9 = f3 * f8 + f1;
    if (f10 < f9) {
        f5 = f9 / f5;
        f1 = *(f32*)lbl_8047D038;
        r3 = 0x1;
        f3 = f3 * f5 + f0;
        f0 = f4 * f5 + f2;
        *(f32*)((u8*)r7 + 0x0) = f3;
        *(f32*)((u8*)r7 + 0x4) = f1;
        *(f32*)((u8*)r7 + 0x8) = f0;
        return;
    }
    f1 = *(f32*)lbl_8047D038;
    if (f10 > f1) {
        /* frsqrte f6, f10 */;
        f8 = *(f64*)lbl_8047D048;
        f7 = *(f64*)lbl_8047D050;
        f1 = f6 * f6;
        f6 = f8 * f6;
        f1 = -(f10 * f1 - f7);
        f6 = f6 * f1;
        f1 = f6 * f6;
        f6 = f8 * f6;
        f1 = -(f10 * f1 - f7);
        f6 = f6 * f1;
        f1 = f6 * f6;
        f6 = f8 * f6;
        f1 = -(f10 * f1 - f7);
        f1 = f6 * f1;
        f10 = f10 * f1;
        f10 = (f32)f10;
        goto L_8012D760;
    }
    f1 = *(f64*)lbl_8047D058;
    if (f10 < f1) {
        r3 = (u32)lbl_80478AC0;
        f10 = *(f32*)lbl_80478AC0;
        goto L_8012D760;
    }
    *(f32*)(sp + 0xC) = f10;
    r0 = (0x7f80 << 16);
    r4 = *(u32*)(sp + 0xC);
    r3 = r4 & 0x7F800000;
    if ((s32)r3 != (s32)r0) {
        if ((s32)r3 < (s32)r0) {
            if ((s32)r3 == (s32)0x0) goto L_8012D734;
        }
        goto L_8012D74C;
    }
    r0 = r4 & 0x7FFFFF;
    if ((s32)r3 != (s32)0x0) {
        r0 = 0x1;
        goto L_8012D750;
    }
    r0 = 0x2;
    goto L_8012D750;
L_8012D734: ;
    r0 = r4 & 0x7FFFFF;
    if ((s32)r3 != (s32)0x0) {
        r0 = 0x5;
        goto L_8012D750;
    }
    r0 = 0x3;
    goto L_8012D750;
L_8012D74C: ;
    r0 = 0x4;
L_8012D750: ;
    if ((s32)r0 == (s32)0x1) {
        r3 = (u32)lbl_80478AC0;
        f10 = *(f32*)lbl_80478AC0;
    }
L_8012D760: ;
    f7 = *(f32*)lbl_8047D080;
    f6 = f9 - f10;
    f1 = f9 + f10;
    f8 = *(f32*)((u8*)r6 + 0x8);
    f7 = f7 / f5;
    f5 = *(f32*)((u8*)r6 + 0x0);
    f6 = f7 * f6;
    f1 = f7 * f1;
    f7 = f3 * f6 + f0;
    f6 = f4 * f6 + f2;
    f9 = f3 * f1 + f0;
    f4 = f4 * f1 + f2;
    f3 = f5 - f7;
    f2 = f8 - f6;
    f1 = f5 - f9;
    f0 = f8 - f4;
    f3 = f3 * f3;
    f2 = f2 * f2;
    f1 = f1 * f1;
    f0 = f0 * f0;
    f2 = f3 + f2;
    f0 = f1 + f0;
    if (f2 < f0) {
        *(f32*)((u8*)r7 + 0x0) = f7;
        f0 = *(f32*)lbl_8047D038;
        *(f32*)((u8*)r7 + 0x4) = f0;
        *(f32*)((u8*)r7 + 0x8) = f6;
    } else {
        *(f32*)((u8*)r7 + 0x0) = f9;
        f0 = *(f32*)lbl_8047D038;
        *(f32*)((u8*)r7 + 0x4) = f0;
        *(f32*)((u8*)r7 + 0x8) = f4;
    }
    r3 = 0x2;
    return;
}
/* 0x8012D7F0 | 0x6A4 */
void fn_8012D7F0(void) {
    extern u8 lbl_80478AC0[];
    extern u8 lbl_8047D030[];
    extern u8 lbl_8047D034[];
    extern u8 lbl_8047D038[];
    extern u8 lbl_8047D048[];
    extern u8 lbl_8047D050[];
    extern u8 lbl_8047D058[];
    extern u8 lbl_8047D060[];
    extern u8 lbl_8047D068[];
    extern u8 lbl_8047D080[];
    extern u8 lbl_8047D0AC[];
    extern void fn_800A3A78();
    extern void fn_800A3AC0();
    extern void fn_800D3088();
    extern void fn_800E3D98();
    extern void fn_800F9318();
    extern void fn_8012D39C();
    u8 sp[0xD0];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    *(f64*)(sp + 0xC0) = f31;
    /* psq_st f31, 0xc8((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0xB0) = f30;
    /* psq_st f30, 0xb8((u32)sp), 0, qr0 */;
    r27 = r3;
    r3 = (u32)&lbl_80426BD0;
    r6 = r27 << 5;
    r28 = r4;
    r0 = (u32)&lbl_80426BD0;
    r29 = r5;
    r3 = r0 + r6;
    f31 = *(f32*)((u8*)r3 + 0x8);
    fn_800D3088();
    r0 = (0x4330 << 16);
    r4 = (u32)&lbl_80426BD0;
    r3 = *(u32*)lbl_8047D030;
    *(u32*)(sp + 0x90) = r0;
    r4 = *(u32*)&lbl_80426BD0;
    r0 = *(u32*)lbl_8047D034;
    f1 = *(f64*)lbl_8047D068;
    f0 = *(f64*)(sp + 0x90);
    f30 = f0 - f1;
    *(u32*)(sp + 0x20) = r0;
    if (((s32)r4 >= (s32)0x0) && ((s32)r4 < (s32)0x2)) {
        r0 = r4 << 2;
        r3 = (u32)sp + 0x1c;
        r31 = *(u32*)(r3 + r0);
    }
    r4 = r31;
    r3 = 0x0;
    fn_800F9318();
    r4 = (u32)sp + 0x84;
    fn_800E3D98();
    r3 = (u32)&lbl_80426BD0;
    r3 = (u32)&lbl_80426BD0;
    r0 = *(u32*)((u8*)r3 + 0x48);
    if ((s32)r0 <= (s32)0x0) {
        r31 = 0x0;
    } else {
        r3 = *(u32*)((u8*)r3 + 0x44);
        /* subic. r3, r3, 0x1 */;
        if ((s32)r0 < (s32)0x0) {
            r3 = r3 + 0x14;
        }
        r0 = r3 * 0xc;
        r3 = (u32)&lbl_80426BD0;
        r31 = 0x1;
        r3 = (u32)&lbl_80426BD0;
        r5 = r3 + r0;
        r4 = *(u32*)((u8*)r5 + 0x4C);
        r3 = *(u32*)((u8*)r5 + 0x50);
        r0 = *(u32*)((u8*)r5 + 0x54);
        *(u32*)(sp + 0x5C) = r0;
    }
    r3 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0x18) = r0;
    if (((s32)r27 >= (s32)0x0) && ((s32)r27 < (s32)0x2)) {
        r0 = r27 << 2;
        r3 = (u32)sp + 0x14;
        r30 = *(u32*)(r3 + r0);
    }
    r4 = r30;
    r3 = 0x0;
    fn_800F9318();
    r4 = (u32)sp + 0x78;
    fn_800E3D98();
    f2 = *(f32*)(sp + 0x84);
    f0 = *(f32*)(sp + 0x78);
    f1 = *(f32*)(sp + 0x8C);
    f4 = f2 - f0;
    f0 = *(f32*)(sp + 0x80);
    f3 = *(f32*)lbl_8047D038;
    f2 = f1 - f0;
    f1 = f4 * f4;
    *(f32*)(sp + 0x6C) = f4;
    f0 = f2 * f2;
    *(f32*)(sp + 0x70) = f3;
    *(f32*)(sp + 0x74) = f2;
    f0 = f1 + f0;
    if (f0 > f3) {
        /* frsqrte f2, f0 */;
        f4 = *(f64*)lbl_8047D048;
        f3 = *(f64*)lbl_8047D050;
        f1 = f2 * f2;
        f2 = f4 * f2;
        f1 = -(f0 * f1 - f3);
        f2 = f2 * f1;
        f1 = f2 * f2;
        f2 = f4 * f2;
        f1 = -(f0 * f1 - f3);
        f2 = f2 * f1;
        f1 = f2 * f2;
        f2 = f4 * f2;
        f1 = -(f0 * f1 - f3);
        f1 = f2 * f1;
        f0 = f0 * f1;
        f0 = (f32)f0;
        goto L_8012DA34;
    }
    f1 = *(f64*)lbl_8047D058;
    if (f0 < f1) {
        r3 = (u32)lbl_80478AC0;
        f0 = *(f32*)lbl_80478AC0;
        goto L_8012DA34;
    }
    *(f32*)(sp + 0x10) = f0;
    r0 = (0x7f80 << 16);
    r4 = *(u32*)(sp + 0x10);
    r3 = r4 & 0x7F800000;
    if ((s32)r3 != (s32)r0) {
        if ((s32)r3 < (s32)r0) {
            if ((s32)r3 == (s32)0x0) goto L_8012DA08;
        }
        goto L_8012DA20;
    }
    r0 = r4 & 0x7FFFFF;
    if ((s32)r3 != (s32)0x0) {
        r0 = 0x1;
        goto L_8012DA24;
    }
    r0 = 0x2;
    goto L_8012DA24;
L_8012DA08: ;
    r0 = r4 & 0x7FFFFF;
    if ((s32)r3 != (s32)0x0) {
        r0 = 0x5;
        goto L_8012DA24;
    }
    r0 = 0x3;
    goto L_8012DA24;
L_8012DA20: ;
    r0 = 0x4;
L_8012DA24: ;
    if ((s32)r0 == (s32)0x1) {
        r3 = (u32)lbl_80478AC0;
        f0 = *(f32*)lbl_80478AC0;
    }
L_8012DA34: ;
    r0 = r31 & 0xFF;
    if ((s32)r0 != (s32)0x1) {
        f4 = *(f32*)(sp + 0x78);
        f1 = *(f32*)(sp + 0x54);
        f3 = *(f32*)(sp + 0x80);
        f2 = *(f32*)(sp + 0x5C);
        f4 = f4 - f1;
        f1 = *(f32*)lbl_8047D038;
        f2 = f3 - f2;
        f3 = f4 * f4;
        f2 = f2 * f2;
        f5 = f3 + f2;
        if (f5 > f1) {
            /* frsqrte f2, f5 */;
            f4 = *(f64*)lbl_8047D048;
            f3 = *(f64*)lbl_8047D050;
            f1 = f2 * f2;
            f2 = f4 * f2;
            f1 = -(f5 * f1 - f3);
            f2 = f2 * f1;
            f1 = f2 * f2;
            f2 = f4 * f2;
            f1 = -(f5 * f1 - f3);
            f2 = f2 * f1;
            f1 = f2 * f2;
            f2 = f4 * f2;
            f1 = -(f5 * f1 - f3);
            f1 = f2 * f1;
            f5 = f5 * f1;
            f5 = (f32)f5;
            goto L_8012DB38;
        }
        f1 = *(f64*)lbl_8047D058;
        if (f5 < f1) {
            r3 = (u32)lbl_80478AC0;
            f5 = *(f32*)lbl_80478AC0;
            goto L_8012DB38;
        }
        *(f32*)(sp + 0xC) = f5;
        r0 = (0x7f80 << 16);
        r4 = *(u32*)(sp + 0xC);
        r3 = r4 & 0x7F800000;
        if ((s32)r3 != (s32)r0) {
            if ((s32)r3 < (s32)r0) {
                if ((s32)r3 == (s32)0x0) goto L_8012DB0C;
            }
            goto L_8012DB24;
        }
        r0 = r4 & 0x7FFFFF;
        if ((s32)r3 != (s32)0x0) {
            r0 = 0x1;
            goto L_8012DB28;
        }
        r0 = 0x2;
        goto L_8012DB28;
    L_8012DB0C: ;
        r0 = r4 & 0x7FFFFF;
        if ((s32)r3 != (s32)0x0) {
            r0 = 0x5;
            goto L_8012DB28;
        }
        r0 = 0x3;
        goto L_8012DB28;
    L_8012DB24: ;
        r0 = 0x4;
    L_8012DB28: ;
        if ((s32)r0 == (s32)0x1) {
            r3 = (u32)lbl_80478AC0;
            f5 = *(f32*)lbl_80478AC0;
        }
    L_8012DB38: ;
        f1 = f5;
        goto L_8012DB44;
    }
    f1 = *(f32*)lbl_8047D038;
L_8012DB44: ;
    r0 = r31 & 0xFF;
    if ((s32)r0 != (s32)0x1) {
        /* cror eq, lt, eq */;
        if (f0 != f1) {
            f1 = *(f32*)lbl_8047D060;
            f1 = f1 + f31;
            /* cror eq, lt, eq */;
            if (f0 != f1) goto L_8012DBEC;
        }
    }
    f1 = *(f32*)lbl_8047D060;
    f1 = f1 + f31;
    if (f0 > f1) {
        f1 = f0 - f31;
        if (f1 > f30) {
            f1 = f30;
        }
        f1 = f1 / f0;
        goto L_8012DBC4;
    }
    f1 = *(f32*)lbl_8047D0AC;
    if (f0 < f1) {
        f0 = f1 - f0;
        if (f0 > f30) {
            f0 = f30;
        }
        f1 = -f0;
        f0 = *(f32*)lbl_8047D0AC;
        f1 = f1 / f0;
        goto L_8012DBC4;
    }
    f1 = *(f32*)lbl_8047D038;
L_8012DBC4: ;
    r4 = r28;
    r3 = (u32)sp + 0x6c;
    fn_800A3AC0();
    r3 = *(u32*)(sp + 0x6C);
    r0 = *(u32*)(sp + 0x70);
    *(u32*)((u8*)r29 + 0x0) = r3;
    *(u32*)((u8*)r29 + 0x4) = r0;
    r0 = *(u32*)(sp + 0x74);
    *(u32*)((u8*)r29 + 0x8) = r0;
    goto L_8012DE70;
L_8012DBEC: ;
    f2 = *(f32*)(sp + 0x54);
    f0 = *(f32*)(sp + 0x78);
    f1 = *(f32*)(sp + 0x5C);
    f4 = f2 - f0;
    f0 = *(f32*)(sp + 0x80);
    f3 = *(f32*)lbl_8047D038;
    f2 = f1 - f0;
    f1 = f4 * f4;
    *(f32*)(sp + 0x6C) = f4;
    f0 = f2 * f2;
    *(f32*)(sp + 0x70) = f3;
    *(f32*)(sp + 0x74) = f2;
    f4 = f1 + f0;
    if (f4 > f3) {
        /* frsqrte f1, f4 */;
        f3 = *(f64*)lbl_8047D048;
        f2 = *(f64*)lbl_8047D050;
        f0 = f1 * f1;
        f1 = f3 * f1;
        f0 = -(f4 * f0 - f2);
        f1 = f1 * f0;
        f0 = f1 * f1;
        f1 = f3 * f1;
        f0 = -(f4 * f0 - f2);
        f1 = f1 * f0;
        f0 = f1 * f1;
        f1 = f3 * f1;
        f0 = -(f4 * f0 - f2);
        f0 = f1 * f0;
        f4 = f4 * f0;
        f4 = (f32)f4;
        goto L_8012DCF4;
    }
    f0 = *(f64*)lbl_8047D058;
    if (f4 < f0) {
        r3 = (u32)lbl_80478AC0;
        f4 = *(f32*)lbl_80478AC0;
        goto L_8012DCF4;
    }
    *(f32*)(sp + 0x8) = f4;
    r0 = (0x7f80 << 16);
    r4 = *(u32*)(sp + 0x8);
    r3 = r4 & 0x7F800000;
    if ((s32)r3 != (s32)r0) {
        if ((s32)r3 < (s32)r0) {
            if ((s32)r3 == (s32)0x0) goto L_8012DCC8;
        }
        goto L_8012DCE0;
    }
    r0 = r4 & 0x7FFFFF;
    if ((s32)r3 != (s32)0x0) {
        r0 = 0x1;
        goto L_8012DCE4;
    }
    r0 = 0x2;
    goto L_8012DCE4;
L_8012DCC8: ;
    r0 = r4 & 0x7FFFFF;
    if ((s32)r3 != (s32)0x0) {
        r0 = 0x5;
        goto L_8012DCE4;
    }
    r0 = 0x3;
    goto L_8012DCE4;
L_8012DCE0: ;
    r0 = 0x4;
L_8012DCE4: ;
    if ((s32)r0 == (s32)0x1) {
        r3 = (u32)lbl_80478AC0;
        f4 = *(f32*)lbl_80478AC0;
    }
L_8012DCF4: ;
    if (f4 > f30) {
        f1 = f30 / f4;
        r3 = (u32)sp + 0x6c;
        r4 = (u32)sp + 0x60;
        fn_800A3AC0();
    } else {
        r4 = *(u32*)(sp + 0x6C);
        r3 = *(u32*)(sp + 0x70);
        r0 = *(u32*)(sp + 0x74);
        *(u32*)(sp + 0x68) = r0;
    }
    r3 = (u32)sp + 0x78;
    r4 = (u32)sp + 0x60;
    r5 = (u32)sp + 0x3c;
    fn_800A3A78();
    f1 = f31;
    r3 = (u32)sp + 0x78;
    r6 = r3;
    r4 = (u32)sp + 0x3c;
    r5 = (u32)sp + 0x84;
    r7 = (u32)sp + 0x48;
    fn_8012D39C();
    if ((s32)r3 > (s32)0x0) {
        f0 = *(f32*)(sp + 0x7C);
        r3 = (u32)sp + 0x3c;
        r4 = (u32)sp + 0x78;
        r5 = (u32)sp + 0x24;
        *(f32*)(sp + 0x4C) = f0;
        ((void(*)(void))fn_800A3A9C)();
        f6 = *(f32*)(sp + 0x24);
        f5 = *(f32*)(sp + 0x28);
        f1 = f6 * f6;
        f7 = *(f32*)(sp + 0x2C);
        f0 = f5 * f5;
        f30 = *(f32*)lbl_8047D038;
        f2 = f7 * f7;
        f0 = f1 + f0;
        f4 = f2 + f0;
        if (f30 == f4) {
            f2 = *(f32*)(sp + 0x78);
            f1 = *(f32*)(sp + 0x7C);
            f0 = *(f32*)(sp + 0x80);
            *(f32*)(sp + 0x30) = f2;
            *(f32*)(sp + 0x34) = f1;
            *(f32*)(sp + 0x38) = f0;
        } else {
            f1 = *(f32*)(sp + 0x4C);
            r3 = (u32)sp + 0x24;
            f0 = *(f32*)(sp + 0x7C);
            r4 = r3;
            f2 = *(f32*)(sp + 0x48);
            f0 = f1 - f0;
            f1 = *(f32*)(sp + 0x78);
            f3 = *(f32*)(sp + 0x50);
            f1 = f2 - f1;
            f2 = *(f32*)(sp + 0x80);
            f0 = f5 * f0;
            f2 = f3 - f2;
            f0 = f6 * f1 + f0;
            f0 = f7 * f2 + f0;
            f30 = f0 / f4;
            f1 = f30;
            fn_800A3AC0();
            r3 = (u32)sp + 0x24;
            r4 = (u32)sp + 0x78;
            r5 = (u32)sp + 0x30;
            fn_800A3A78();
        }
        f0 = *(f32*)lbl_8047D038;
        /* cror eq, gt, eq */;
        if (f30 == f0) {
            f0 = *(f32*)lbl_8047D080;
            /* cror eq, lt, eq */;
            if (f30 == f0) {
                r3 = (u32)sp + 0x48;
                r4 = (u32)sp + 0x78;
                r5 = (u32)sp + 0x60;
                ((void(*)(void))fn_800A3A9C)();
    }
    }
    }
    r3 = *(u32*)(sp + 0x60);
    r0 = *(u32*)(sp + 0x64);
    *(u32*)((u8*)r28 + 0x0) = r3;
    *(u32*)((u8*)r28 + 0x4) = r0;
    r0 = *(u32*)(sp + 0x68);
    *(u32*)((u8*)r28 + 0x8) = r0;
    r3 = *(u32*)(sp + 0x60);
    r0 = *(u32*)(sp + 0x64);
    *(u32*)((u8*)r29 + 0x0) = r3;
    *(u32*)((u8*)r29 + 0x4) = r0;
    r0 = *(u32*)(sp + 0x68);
    *(u32*)((u8*)r29 + 0x8) = r0;
L_8012DE70: ;
    /* psq_l f31, 0xc8((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0xC0);
    /* psq_l f30, 0xb8((u32)sp), 0, qr0 */;
    f30 = *(f64*)(sp + 0xB0);
    return;
}
/* 0x8012DE94 | 0x4F4 */
void fn_8012DE94(void) {
    extern u8 lbl_80478AC0[];
    extern u8 lbl_8047D030[];
    extern u8 lbl_8047D034[];
    extern u8 lbl_8047D038[];
    extern u8 lbl_8047D048[];
    extern u8 lbl_8047D050[];
    extern u8 lbl_8047D058[];
    extern u8 lbl_8047D0B0[];
    extern void fn_800D3088();
    extern void fn_800E3C64();
    extern void fn_800E3D98();
    extern void fn_800F9318();
    extern void fn_8012CA84();
    extern void fn_8012D7F0();
    extern void fn_8018C0A8();
    u8 sp[0x110];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r16 = 0;
    u32 r17 = 0;
    u32 r18 = 0;
    u32 r19 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f31 = 0.0f;
    *(f64*)(sp + 0x100) = f31;
    /* psq_st f31, 0x108((u32)sp), 0, qr0 */;
    r6 = (u32)&lbl_80426BD0;
    r5 = *(u32*)lbl_8047D030;
    r6 = (u32)&lbl_80426BD0;
    r0 = *(u32*)lbl_8047D034;
    r6 = *(u32*)((u8*)r6 + 0x0);
    r30 = r3;
    *(u32*)(sp + 0x48) = r0;
    if (((s32)r6 >= (s32)0x0) && ((s32)r6 < (s32)0x2)) {
        r0 = r6 << 2;
        r3 = (u32)sp + 0x44;
        r4 = *(u32*)(r3 + r0);
    }
    r3 = 0x0;
    fn_800F9318();
    r4 = (u32)sp + 0xa8;
    fn_800E3D98();
    r3 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0x40) = r0;
    if (((s32)r30 >= (s32)0x0) && ((s32)r30 < (s32)0x2)) {
        r0 = r30 << 2;
        r3 = (u32)sp + 0x3c;
        r16 = *(u32*)(r3 + r0);
    }
    r4 = r16;
    r3 = 0x0;
    fn_800F9318();
    r4 = (u32)sp + 0x9c;
    fn_800E3D98();
    f1 = *(f32*)(sp + 0xB0);
    f0 = *(f32*)(sp + 0xA4);
    f2 = *(f32*)(sp + 0xA8);
    f3 = f1 - f0;
    f1 = *(f32*)(sp + 0x9C);
    f0 = *(f32*)lbl_8047D038;
    f2 = f2 - f1;
    f1 = f3 * f3;
    f1 = f2 * f2 + f1;
    if (f1 > f0) {
    } else {
        f0 = *(f64*)lbl_8047D058;
    }
    r3 = r30;
    r4 = (u32)sp + 0x84;
    r5 = (u32)sp + 0x60;
    fn_8012D7F0();
    r3 = r30;
    r4 = (u32)sp + 0x84;
    r5 = (u32)sp + 0x60;
    fn_8012CA84();
    r3 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0x38) = r0;
    if (((s32)r30 >= (s32)0x0) && ((s32)r30 < (s32)0x2)) {
        r0 = r30 << 2;
        r3 = (u32)sp + 0x34;
        r18 = *(u32*)(r3 + r0);
    }
    r4 = r18;
    r3 = 0x0;
    fn_800F9318();
    r4 = (u32)sp + 0x90;
    fn_800E3D98();
    f3 = *(f32*)(sp + 0xA8);
    f0 = *(f32*)(sp + 0x90);
    f2 = *(f32*)(sp + 0xB0);
    f1 = *(f32*)(sp + 0x98);
    f3 = f3 - f0;
    f0 = *(f32*)lbl_8047D038;
    f1 = f2 - f1;
    f2 = f3 * f3;
    f1 = f1 * f1;
    f31 = f2 + f1;
    if (f31 > f0) {
        /* frsqrte f1, f31 */;
        f3 = *(f64*)lbl_8047D048;
        f2 = *(f64*)lbl_8047D050;
        f0 = f1 * f1;
        f1 = f3 * f1;
        f0 = -(f31 * f0 - f2);
        f1 = f1 * f0;
        f0 = f1 * f1;
        f1 = f3 * f1;
        f0 = -(f31 * f0 - f2);
        f1 = f1 * f0;
        f0 = f1 * f1;
        f1 = f3 * f1;
        f0 = -(f31 * f0 - f2);
        f0 = f1 * f0;
        f31 = f31 * f0;
        f31 = (f32)f31;
        goto L_8012E0CC;
    }
    f0 = *(f64*)lbl_8047D058;
    if (f31 < f0) {
        r3 = (u32)lbl_80478AC0;
        f31 = *(f32*)lbl_80478AC0;
        goto L_8012E0CC;
    }
    *(f32*)(sp + 0x8) = f31;
    r0 = (0x7f80 << 16);
    r4 = *(u32*)(sp + 0x8);
    r3 = r4 & 0x7F800000;
    if ((s32)r3 != (s32)r0) {
        if ((s32)r3 < (s32)r0) {
            if ((s32)r3 == (s32)0x0) goto L_8012E0A0;
        }
        goto L_8012E0B8;
    }
    r0 = r4 & 0x7FFFFF;
    if ((s32)r3 != (s32)0x0) {
        r0 = 0x1;
        goto L_8012E0BC;
    }
    r0 = 0x2;
    goto L_8012E0BC;
L_8012E0A0: ;
    r0 = r4 & 0x7FFFFF;
    if ((s32)r3 != (s32)0x0) {
        r0 = 0x5;
        goto L_8012E0BC;
    }
    r0 = 0x3;
    goto L_8012E0BC;
L_8012E0B8: ;
    r0 = 0x4;
L_8012E0BC: ;
    if ((s32)r0 == (s32)0x1) {
        r3 = (u32)lbl_80478AC0;
        f31 = *(f32*)lbl_80478AC0;
    }
L_8012E0CC: ;
    r3 = (u32)sp + 0x90;
    r4 = (u32)sp + 0x9c;
    r5 = (u32)sp + 0x78;
    ((void(*)(void))fn_800A3A9C)();
    f0 = *(f32*)lbl_8047D0B0;
    if (f31 < f0) {
        r3 = (u32)&lbl_80426BD0;
        r0 = r30 << 5;
        r3 = (u32)&lbl_80426BD0;
        r4 = 0x12c;
        r3 = r3 + r0;
        *(u32*)((u8*)r3 + 0x10) = r4;
        goto L_8012E36C;
    }
    fn_800D3088();
    r4 = (u32)&lbl_80426BD0;
    r0 = r30 << 5;
    r4 = (u32)&lbl_80426BD0;
    r31 = r4 + r0;
    r0 = *(u32*)((u8*)r31 + 0x10);
    /* subf. r0, r3, r0 */;
    r31 += 16; *(u32*)r31 = r0;
    if (f31 <= f0) {
        r3 = *(u32*)lbl_8047D030;
        r0 = *(u32*)lbl_8047D034;
        *(u32*)(sp + 0x30) = r0;
        if (((s32)r30 >= (s32)0x0) && ((s32)r30 < (s32)0x2)) {
            r0 = r30 << 2;
            r3 = (u32)sp + 0x2c;
            r19 = *(u32*)(r3 + r0);
        }
        r4 = r19;
        r3 = 0x0;
        fn_800F9318();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
        } else {
            fn_800E3C64();
        }
        r0 = r3 & 0xFF;
        if ((u32)r3 == (u32)0x0) {
            r3 = (u32)&lbl_80426BD0;
            r20 = r30 << 2;
            r24 = (u32)sp + 0x24;
            r23 = (u32)sp + 0xc;
            r22 = (u32)sp + 0x1c;
            r21 = (u32)sp + 0x14;
            r29 = (u32)&lbl_80426BD0;
            r18 = 0x0;
            r19 = 0x0;
            goto L_8012E2B0;
        L_8012E1A4: ;
            r3 = *(u32*)lbl_8047D030;
            r0 = *(u32*)lbl_8047D034;
            *(u32*)(sp + 0x28) = r0;
            if (((s32)r30 >= (s32)0x0) && ((s32)r30 < (s32)0x2)) {
                r28 = *(u32*)(r24 + r20);
            }
            r4 = r28;
            r3 = 0x0;
            fn_800F9318();
            if ((s32)r30 == (s32)0x2) {
                r16 = 0x0;
                goto L_8012E2A0;
            }
            /* addic. r0, (u32)sp, 0x6c */;
            if ((s32)r30 != (s32)0x2) {
                r3 = *(u32*)lbl_8047D030;
                r0 = *(u32*)lbl_8047D034;
                *(u32*)(sp + 0x10) = r0;
                if (((s32)r30 >= (s32)0x0) && ((s32)r30 < (s32)0x2)) {
                    r25 = *(u32*)(r23 + r20);
                }
                r4 = r25;
                r3 = 0x0;
                fn_800F9318();
                r4 = (u32)sp + 0x54;
                fn_800E3D98();
                r3 = *(u32*)lbl_8047D030;
                r0 = *(u32*)lbl_8047D034;
                *(u32*)(sp + 0x20) = r0;
                if (((s32)r30 >= (s32)0x0) && ((s32)r30 < (s32)0x2)) {
                    r27 = *(u32*)(r22 + r20);
                }
                r4 = r27;
                r5 = (u32)sp + 0x6c;
                r3 = 0x0;
                fn_8018C0A8();
            }
            r3 = r16;
            fn_800E3C64();
            /* addic. r0, (u32)sp, 0x6c */;
            r16 = r3;
            if ((s32)r30 != (s32)0x2) {
                r3 = *(u32*)lbl_8047D030;
                r0 = *(u32*)lbl_8047D034;
                *(u32*)(sp + 0x18) = r0;
                if (((s32)r30 >= (s32)0x0) && ((s32)r30 < (s32)0x2)) {
                    r26 = *(u32*)(r21 + r20);
                }
                r4 = r26;
                r5 = (u32)sp + 0x54;
                r3 = 0x0;
                fn_8018C0A8();
    }
        L_8012E2A0: ;
            r0 = r16 & 0xFF;
            if ((s32)r30 == (s32)0x2) {
                r18 = 0x1;
            }
            r19 = r19 + 0x1;
        L_8012E2B0: ;
            r0 = *(u32*)((u8*)r29 + 0x48);
            if ((s32)r19 >= (s32)r0) {
                r6 = 0x0;
                goto L_8012E314;
            }
            if ((s32)r19 >= (s32)0x14) {
                r6 = 0x0;
                goto L_8012E314;
            }
            r0 = *(u32*)((u8*)r29 + 0x44);
            r3 = r0 - r19;
            /* subic. r3, r3, 0x1 */;
            if ((s32)r19 < (s32)0x14) {
                r3 = r3 + 0x14;
            }
            r4 = r3 * 0xc;
            r3 = (u32)&lbl_80426BD0;
            r6 = 0x1;
            r0 = (u32)&lbl_80426BD0;
            r5 = r0 + r4;
            r4 = *(u32*)((u8*)r5 + 0x4C);
            r3 = *(u32*)((u8*)r5 + 0x50);
            r0 = *(u32*)((u8*)r5 + 0x54);
            *(u32*)(sp + 0x74) = r0;
        L_8012E314: ;
            r0 = r6 & 0xFF;
            if ((s32)r19 == (s32)0x14) goto L_8012E324;
            r0 = r18 & 0xFF;
            if ((s32)r19 == (s32)0x14) goto L_8012E1A4;
        L_8012E324: ;
            r0 = r18 & 0xFF;
            if ((s32)r19 != (s32)0x14) {
                r3 = *(u32*)lbl_8047D030;
                r0 = *(u32*)lbl_8047D034;
                *(u32*)(sp + 0x50) = r0;
                if (((s32)r30 >= (s32)0x0) && ((s32)r30 < (s32)0x2)) {
                    r3 = (u32)sp + 0x4c;
                    r17 = *(u32*)(r3 + r20);
                }
                r4 = r17;
                r5 = (u32)sp + 0x6c;
                r3 = 0x0;
                fn_8018C0A8();
                r0 = 0x12c;
                *(u32*)((u8*)r31 + 0x0) = r0;
    }
    }
    }
L_8012E36C: ;
    /* psq_l f31, 0x108((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x100);
    return;
}
/* 0x8012E388 | 0x430 */
void fn_8012E388(void) {
    extern u8 lbl_80478AC0[];
    extern u8 lbl_8047D030[];
    extern u8 lbl_8047D034[];
    extern u8 lbl_8047D038[];
    extern u8 lbl_8047D048[];
    extern u8 lbl_8047D050[];
    extern u8 lbl_8047D058[];
    extern u8 lbl_8047D078[];
    extern u8 lbl_8047D084[];
    extern u8 lbl_8047D0B4[];
    extern u8 lbl_8047D0B8[];
    extern u8 lbl_8047D0BC[];
    extern u8 lbl_8047D0C0[];
    extern u8 lbl_8047D0C4[];
    extern u8 lbl_8047D0C8[];
    extern void fn_800CE148();
    extern void fn_800F7A08();
    extern void fn_800F7A7C();
    extern void fn_800F7BC4();
    extern void fn_800F9318();
    extern void fn_80166458();
    extern void fn_80176684();
    extern void fn_80177A38();
    extern void fn_8018790C();
    extern void fn_8018805C();
    extern void fn_80188214();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f31 = 0.0f;
    *(f64*)(sp + 0x60) = f31;
    /* psq_st f31, 0x68((u32)sp), 0, qr0 */;
    r29 = r3;
    r26 = r4;
    r3 = 0x0;
    r4 = 0x2;
    fn_800F9318();
    r3 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0x10) = r0;
    *(u32*)(sp + 0x20) = r0;
    if (((s32)r29 >= (s32)0x0) && ((s32)r29 < (s32)0x2)) {
        r0 = r29 << 2;
        r3 = (u32)sp + 0x1c;
        r27 = *(u32*)(r3 + r0);
    }
    r3 = *(u32*)(sp + 0xC);
    r0 = *(u32*)(sp + 0x10);
    *(u32*)(sp + 0x18) = r0;
    if (((s32)r29 >= (s32)0x0) && ((s32)r29 < (s32)0x2)) {
        r0 = r29 << 2;
        r3 = (u32)sp + 0x14;
        r28 = *(u32*)(r3 + r0);
    }
    r4 = r28;
    r3 = 0x0;
    fn_800F9318();
    r3 = 0x1;
    r4 = 0x1;
    fn_800F7A7C();
    r0 = r3;
    r3 = 0x1;
    r31 = r0;
    r4 = 0x1;
    fn_800F7A08();
    r0 = r3;
    r3 = 0x1;
    r30 = r0;
    r4 = 0x0;
    fn_800F7A7C();
    r0 = r3;
    r3 = 0x1;
    r29 = r0;
    r4 = 0x0;
    fn_800F7A08();
    r0 = (s8)r31;
    r28 = r3;
    if ((s32)r29 == (s32)0x2) {
        r0 = (s8)r30;
        if ((s32)r29 == (s32)0x2) {
            r3 = 0x1;
            fn_800F7BC4();
            r0 = r3 & 0x00000008;
            if ((s32)r29 != (s32)0x2) {
                r30 = -0x38;
            }
            r3 = 0x1;
            fn_800F7BC4();
            r0 = r3 & 0x00000004;
            if ((s32)r29 != (s32)0x2) {
                r30 = 0x38;
            }
            r3 = 0x1;
            fn_800F7BC4();
            r0 = r3 & 0x1;
            if ((s32)r29 != (s32)0x2) {
                r31 = -0x38;
            }
            r3 = 0x1;
            fn_800F7BC4();
            r0 = r3 & 0x00000002;
            if ((s32)r29 != (s32)0x2) {
                r31 = 0x38;
            }
            r0 = (s8)r31;
            r29 = r31;
            r28 = r30;
            if ((s32)r29 == (s32)0x2) {
                r0 = (s8)r30;
                if ((s32)r29 == (s32)0x2) {
                    fn_80177A38();
    }
    }
    }
    }
    r0 = (s8)r31;
    if ((s32)r29 == (s32)0x2) {
        r0 = (s8)r30;
        if ((s32)r29 == (s32)0x2) goto L_8012E788;
    }
    r0 = (s8)r31;
    if ((s32)r0 > (s32)0x38) {
        r31 = 0x38;
        goto L_8012E518;
    }
    if ((s32)r0 < (s32)-0x38) {
        r31 = -0x38;
    }
L_8012E518: ;
    r0 = (s8)r30;
    if ((s32)r0 > (s32)0x38) {
        r30 = 0x38;
        goto L_8012E538;
    }
    if ((s32)r0 < (s32)-0x38) {
        r30 = -0x38;
    }
L_8012E538: ;
    r0 = (s8)r31;
    r0 = (s8)r31;
    r3 = -r0;
    if ((s32)r0 > (s32)-0x38) {
        r3 = r0;
    }
    /* xoris r0, r3, 0x8000 */;
    r4 = (0x4330 << 16);
    *(u32*)(sp + 0x34) = r0;
    r0 = (s8)r30;
    f2 = *(f64*)lbl_8047D0C8;
    r3 = (s8)r30;
    r3 = -r0;
    f0 = *(f32*)lbl_8047D0B4;
    f1 = *(f64*)(sp + 0x30);
    f1 = f1 - f2;
    f5 = f1 / f0;
    if ((s32)r0 > (s32)-0x38) {
        r3 = r0;
    }
    /* xoris r3, r3, 0x8000 */;
    r0 = (0x4330 << 16);
    f3 = *(f64*)lbl_8047D0C8;
    *(u32*)(sp + 0x38) = r0;
    f1 = *(f32*)lbl_8047D0B4;
    f2 = *(f64*)(sp + 0x38);
    f0 = *(f32*)lbl_8047D038;
    f2 = f2 - f3;
    f6 = f2 / f1;
    f1 = f6 * f6;
    f4 = f5 * f5 + f1;
    if (f4 > f0) {
        /* frsqrte f1, f4 */;
        f3 = *(f64*)lbl_8047D048;
        f2 = *(f64*)lbl_8047D050;
        f0 = f1 * f1;
        f1 = f3 * f1;
        f0 = -(f4 * f0 - f2);
        f1 = f1 * f0;
        f0 = f1 * f1;
        f1 = f3 * f1;
        f0 = -(f4 * f0 - f2);
        f1 = f1 * f0;
        f0 = f1 * f1;
        f1 = f3 * f1;
        f0 = -(f4 * f0 - f2);
        f0 = f1 * f0;
        f4 = f4 * f0;
        f4 = (f32)f4;
        goto L_8012E688;
    }
    f0 = *(f64*)lbl_8047D058;
    if (f4 < f0) {
        r3 = (u32)lbl_80478AC0;
        f4 = *(f32*)lbl_80478AC0;
        goto L_8012E688;
    }
    *(f32*)(sp + 0x8) = f4;
    r0 = (0x7f80 << 16);
    r4 = *(u32*)(sp + 0x8);
    r3 = r4 & 0x7F800000;
    if ((s32)r3 != (s32)r0) {
        if ((s32)r3 < (s32)r0) {
            if ((s32)r3 == (s32)0x0) goto L_8012E65C;
        }
        goto L_8012E674;
    }
    r0 = r4 & 0x7FFFFF;
    if ((s32)r3 != (s32)0x0) {
        r0 = 0x1;
        goto L_8012E678;
    }
    r0 = 0x2;
    goto L_8012E678;
L_8012E65C: ;
    r0 = r4 & 0x7FFFFF;
    if ((s32)r3 != (s32)0x0) {
        r0 = 0x5;
        goto L_8012E678;
    }
    r0 = 0x3;
    goto L_8012E678;
L_8012E674: ;
    r0 = 0x4;
L_8012E678: ;
    if ((s32)r0 == (s32)0x1) {
        r3 = (u32)lbl_80478AC0;
        f4 = *(f32*)lbl_80478AC0;
    }
L_8012E688: ;
    *(f32*)((u8*)r26 + 0x0) = f4;
    f0 = *(f32*)lbl_8047D084;
    f1 = *(f32*)((u8*)r26 + 0x0);
    if (f1 > f0) {
        *(f32*)((u8*)r26 + 0x0) = f0;
    }
    r0 = (s8)r29;
    if ((s32)r0 > (s32)-0x2 && (s32)r0 < (s32)0x2) {
        r0 = (s8)r28;
        if ((s32)r0 > (s32)-0x2) {
            if ((s32)r0 < (s32)0x2) goto L_8012E760;
        }
    }
    f0 = *(f32*)lbl_8047D0B8;
    if (f6 < f0) {
        f2 = *(f32*)lbl_8047D0BC;
    } else {
        f1 = f5 / f6;
        f0 = *(f32*)lbl_8047D078;
        if (f1 > f0) {
            f1 = f0;
        }
        f0 = *(f32*)lbl_8047D0C0;
        f1 = f1 / f0;
        fn_800CE148();
        f1 = (f32)f1;
        f0 = *(f32*)lbl_8047D0BC;
        f2 = f0 * f1;
    }
    r0 = (s8)r30;
    if (f1 >= f0) {
        f31 = f2;
    } else {
        f0 = *(f32*)lbl_8047D0C4;
        f31 = f0 - f2;
    }
    r0 = (s8)r31;
    if (f1 < f0) {
        r0 = (s8)r30;
        if (f1 < f0) goto L_8012E740;
        f1 = *(f32*)lbl_8047D0C4;
        f0 = f1 - f2;
        f31 = f1 + f0;
    }
    goto L_8012E748;
L_8012E740: ;
    f0 = *(f32*)lbl_8047D0C4;
    f31 = f0 + f2;
L_8012E748: ;
    fn_80176684();
    f1 = f31 + f1;
    f2 = *(f32*)((u8*)r26 + 0x0);
    r4 = r27;
    r3 = 0x0;
    fn_8018805C();
L_8012E760: ;
    f1 = *(f32*)((u8*)r26 + 0x0);
    r4 = r27;
    r3 = 0x0;
    fn_80188214();
    r3 = 0x0;
    r4 = 0x7d0;
    fn_800F9318();
    r4 = (u32)sp + 0x24;
    fn_80166458();
    goto L_8012E79C;
L_8012E788: ;
    f0 = *(f32*)lbl_8047D038;
    r4 = r27;
    r3 = 0x0;
    *(f32*)((u8*)r26 + 0x0) = f0;
    fn_8018790C();
L_8012E79C: ;
    /* psq_l f31, 0x68((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x60);
    return;
}
/* 0x8012E7B8 | 0x41C */
void fn_8012E7B8(void) {
    extern u8 lbl_80478AC0[];
    extern u8 lbl_8047D030[];
    extern u8 lbl_8047D034[];
    extern u8 lbl_8047D038[];
    extern u8 lbl_8047D048[];
    extern u8 lbl_8047D050[];
    extern u8 lbl_8047D058[];
    extern u8 lbl_8047D068[];
    extern u8 lbl_8047D080[];
    extern u8 lbl_8047D0D0[];
    extern u8 lbl_8047D0D4[];
    extern void fn_800A3AC0();
    extern void fn_800A3C00();
    extern void fn_800D3088();
    extern void fn_800E3D98();
    extern void fn_800F7AF0();
    extern void fn_800F7BC4();
    extern void fn_800F9318();
    extern void fn_8012C0B4();
    extern void fn_8012C660();
    extern void fn_8012E388();
    extern void fn_801887D8();
    u8 sp[0xB0];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f31 = 0.0f;
    *(f64*)(sp + 0xA0) = f31;
    /* psq_st f31, 0xa8((u32)sp), 0, qr0 */;
    r28 = r3;
    r3 = 0x1;
    fn_800F7AF0();
    r27 = r3;
    r3 = 0x1;
    fn_800F7BC4();
    r0 = r3 & r27;
    r0 = r0 & 0x00000100;
    if ((s32)r0 != (s32)0) {
        r3 = r28;
        fn_8012C0B4();
        r0 = r3 & 0xFF;
        if ((s32)r0 != (s32)0) {
            f1 = *(f32*)lbl_8047D038;
            goto L_8012EBB8;
        }
    }
    r3 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0x2C) = r0;
    if (((s32)r28 >= (s32)0x0) && ((s32)r28 < (s32)0x2)) {
        r0 = r28 << 2;
        r3 = (u32)sp + 0x28;
        r26 = *(u32*)(r3 + r0);
    }
    r4 = r26;
    r3 = 0x0;
    fn_800F9318();
    r4 = (u32)sp + 0x68;
    fn_800E3D98();
    r3 = r28;
    r4 = (u32)sp + 0xc;
    fn_8012E388();
    r3 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0x24) = r0;
    if (((s32)r28 >= (s32)0x0) && ((s32)r28 < (s32)0x2)) {
        r0 = r28 << 2;
        r3 = (u32)sp + 0x20;
        r25 = *(u32*)(r3 + r0);
    }
    r4 = r25;
    r3 = 0x0;
    fn_800F9318();
    r4 = (u32)sp + 0x5c;
    fn_800E3D98();
    r3 = (u32)sp + 0x5c;
    r4 = (u32)sp + 0x68;
    r5 = (u32)sp + 0x50;
    ((void(*)(void))fn_800A3A9C)();
    fn_800D3088();
    r0 = (0x4330 << 16);
    r3 = (u32)sp + 0x50;
    f2 = *(f64*)lbl_8047D068;
    *(u32*)(sp + 0x78) = r0;
    r4 = r3;
    f0 = *(f32*)lbl_8047D080;
    f1 = *(f64*)(sp + 0x78);
    f1 = f1 - f2;
    f1 = f0 / f1;
    fn_800A3AC0();
    r3 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0x34) = r0;
    if (((s32)r28 >= (s32)0x0) && ((s32)r28 < (s32)0x2)) {
        r0 = r28 << 2;
        r3 = (u32)sp + 0x30;
        r29 = *(u32*)(r3 + r0);
    }
    r4 = r29;
    r5 = (u32)sp + 0x50;
    r3 = 0x0;
    fn_801887D8();
    f31 = f1;
    f0 = *(f32*)lbl_8047D0D0;
    if (f31 < f0) {
        f1 = *(f32*)(sp + 0xC);
        /* cror eq, gt, eq */;
        if (f1 != f0) goto L_8012E93C;
        f31 = f0;
    }
    goto L_8012E940;
L_8012E93C: ;
    f31 = f1;
L_8012E940: ;
    r3 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0x1C) = r0;
    if (((s32)r28 >= (s32)0x0) && ((s32)r28 < (s32)0x2)) {
        r0 = r28 << 2;
        r3 = (u32)sp + 0x18;
        r31 = *(u32*)(r3 + r0);
    }
    r4 = r31;
    r3 = 0x0;
    fn_800F9318();
    f1 = f31;
    r4 = r28;
    fn_8012C660();
    r4 = (u32)&lbl_80426BD0;
    r3 = *(u32*)lbl_8047D030;
    r4 = (u32)&lbl_80426BD0;
    r0 = *(u32*)lbl_8047D034;
    r4 = *(u32*)((u8*)r4 + 0x0);
    r31 = 0x0;
    *(u32*)(sp + 0x14) = r0;
    if (((s32)r4 >= (s32)0x0) && ((s32)r4 < (s32)0x2)) {
        r0 = r4 << 2;
        r3 = (u32)sp + 0x10;
        r30 = *(u32*)(r3 + r0);
    }
    r4 = r30;
    r3 = 0x0;
    fn_800F9318();
    r4 = (u32)sp + 0x44;
    fn_800E3D98();
    r3 = (u32)&lbl_80426BD0;
    r3 = (u32)&lbl_80426BD0;
    r0 = *(u32*)((u8*)r3 + 0x48);
    if ((s32)r0 <= (s32)0x0) {
        r7 = 0x0;
    } else {
        r4 = *(u32*)((u8*)r3 + 0x44);
        /* subic. r4, r4, 0x1 */;
        if ((s32)r0 < (s32)0x0) {
            r4 = r4 + 0x14;
        }
        r0 = r4 * 0xc;
        r4 = (u32)&lbl_80426BD0;
        r7 = 0x1;
        r4 = (u32)&lbl_80426BD0;
        r6 = r4 + r0;
        r5 = *(u32*)((u8*)r6 + 0x4C);
        r4 = *(u32*)((u8*)r6 + 0x50);
        r0 = *(u32*)((u8*)r6 + 0x54);
        *(u32*)(sp + 0x40) = r0;
    }
    r0 = r7 & 0xFF;
    if ((s32)r0 != (s32)0x0) {
        f3 = *(f32*)(sp + 0x38);
        f0 = *(f32*)(sp + 0x44);
        f2 = *(f32*)(sp + 0x40);
        f1 = *(f32*)(sp + 0x4C);
        f3 = f3 - f0;
        f0 = *(f32*)lbl_8047D038;
        f1 = f2 - f1;
        f2 = f3 * f3;
        f1 = f1 * f1;
        f4 = f2 + f1;
        if (f4 > f0) {
            /* frsqrte f1, f4 */;
            f3 = *(f64*)lbl_8047D048;
            f2 = *(f64*)lbl_8047D050;
            f0 = f1 * f1;
            f1 = f3 * f1;
            f0 = -(f4 * f0 - f2);
            f1 = f1 * f0;
            f0 = f1 * f1;
            f1 = f3 * f1;
            f0 = -(f4 * f0 - f2);
            f1 = f1 * f0;
            f0 = f1 * f1;
            f1 = f3 * f1;
            f0 = -(f4 * f0 - f2);
            f0 = f1 * f0;
            f4 = f4 * f0;
            f4 = (f32)f4;
            goto L_8012EB30;
        }
        f0 = *(f64*)lbl_8047D058;
        if (f4 < f0) {
            r4 = (u32)lbl_80478AC0;
            f4 = *(f32*)lbl_80478AC0;
            goto L_8012EB30;
        }
        *(f32*)(sp + 0x8) = f4;
        r0 = (0x7f80 << 16);
        r5 = *(u32*)(sp + 0x8);
        r4 = r5 & 0x7F800000;
        if ((s32)r4 != (s32)r0) {
            if ((s32)r4 < (s32)r0) {
                if ((s32)r4 == (s32)0x0) goto L_8012EB04;
            }
            goto L_8012EB1C;
        }
        r0 = r5 & 0x7FFFFF;
        if ((s32)r4 != (s32)0x0) {
            r0 = 0x1;
            goto L_8012EB20;
        }
        r0 = 0x2;
        goto L_8012EB20;
    L_8012EB04: ;
        r0 = r5 & 0x7FFFFF;
        if ((s32)r4 != (s32)0x0) {
            r0 = 0x5;
            goto L_8012EB20;
        }
        r0 = 0x3;
        goto L_8012EB20;
    L_8012EB1C: ;
        r0 = 0x4;
    L_8012EB20: ;
        if ((s32)r0 == (s32)0x1) {
            r4 = (u32)lbl_80478AC0;
            f4 = *(f32*)lbl_80478AC0;
        }
    L_8012EB30: ;
        f0 = *(f32*)lbl_8047D0D4;
        if (f4 > f0) {
            r31 = 0x1;
        }
        goto L_8012EB48;
    }
    r31 = 0x1;
L_8012EB48: ;
    r0 = r31 & 0xFF;
    if (f4 != f0) {
        r4 = (u32)&lbl_80426BD0;
        r5 = *(u32*)(sp + 0x44);
        r7 = (u32)&lbl_80426BD0;
        r4 = *(u32*)(sp + 0x48);
        r6 = *(u32*)((u8*)r7 + 0x44);
        r0 = *(u32*)(sp + 0x4C);
        r6 = r6 * 0xc;
        r6 = r7 + r6;
        *(u32*)((u8*)r6 + 0x4C) = r5;
        *(u32*)((u8*)r6 + 0x50) = r4;
        *(u32*)((u8*)r6 + 0x54) = r0;
        r4 = *(u32*)((u8*)r7 + 0x44);
        r0 = r4 + 0x1;
        *(u32*)((u8*)r7 + 0x44) = r0;
        if ((s32)r0 >= (s32)0x14) {
            r0 = 0x0;
            *(u32*)((u8*)r7 + 0x44) = r0;
        }
        r4 = *(u32*)((u8*)r3 + 0x48);
        if ((s32)r4 < (s32)0x14) {
            r0 = r4 + 0x1;
            *(u32*)((u8*)r3 + 0x48) = r0;
    }
    }
    r3 = (u32)sp + 0x68;
    r4 = (u32)sp + 0x5c;
    fn_800A3C00();
L_8012EBB8: ;
    /* psq_l f31, 0xa8((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0xA0);
    return;
}
/* 0x8012EBD4 | 0x3E4 */
void fn_8012EBD4(void) {
    extern u8 lbl_80272A38[];
    extern u8 lbl_8047D030[];
    extern u8 lbl_8047D034[];
    extern u8 lbl_8047D064[];
    extern void fn_8000D710();
    extern void fn_800D3088();
    extern void fn_800F7434();
    extern void fn_800F7AF0();
    extern void fn_800F7BC4();
    extern void fn_800F9318();
    extern void fn_80102620();
    extern void fn_80116D30();
    extern void fn_8012BEB4();
    extern void fn_8012DE94();
    extern void fn_8012E7B8();
    extern void fn_801337B0();
    extern void fn_80177A38();
    extern void fn_8018C424();
    u8 sp[0xB0];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    *(f64*)(sp + 0xA0) = f31;
    /* psq_st f31, 0xa8((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x90) = f30;
    /* psq_st f30, 0x98((u32)sp), 0, qr0 */;
    fn_801337B0();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) {
        r3 = 0x0;
        goto L_8012EF94;
    }
    r3 = 0xca;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) {
        r3 = 0x0;
        goto L_8012EF94;
    }
    fn_80177A38();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x6) {
        r3 = 0x0;
        goto L_8012EF94;
    }
    r3 = (u32)&lbl_80426BD0;
    r4 = (u32)&lbl_80426BD0;
    r3 = *(u32*)((u8*)r4 + 0x188);
    if ((s32)r3 > (s32)0x0) {
        r3 = 0x0;
        *(u32*)((u8*)r4 + 0x188) = r0;
        goto L_8012EF94;
    }
    r4 = *(u32*)((u8*)r4 + 0x0);
    r3 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0x14) = r0;
    if (((s32)r4 >= (s32)0x0) && ((s32)r4 < (s32)0x2)) {
        r0 = r4 << 2;
        r3 = (u32)sp + 0x10;
        r26 = *(u32*)(r3 + r0);
    }
    r4 = r26;
    r3 = 0x0;
    r5 = (0x8000 << 16);
    fn_8018C424();
    r0 = r3 & 0xFF;
    if ((s32)r4 != (s32)0x2) {
        r3 = 0x0;
        goto L_8012EF94;
    }
    r3 = (u32)&lbl_80426BD0;
    r29 = (u32)&lbl_80426BD0;
    r3 = *(u32*)((u8*)r29 + 0x18C);
    if ((u32)r3 != (u32)0x0) {
        r0 = 0x0;
        r5 = *(u32*)((u8*)r29 + 0x190);
        *(u32*)((u8*)r29 + 0x18C) = r0;
        r4 = 0x4;
        r6 = *(u32*)((u8*)r29 + 0x194);
        r7 = *(u32*)((u8*)r29 + 0x198);
        r8 = *(u32*)((u8*)r29 + 0x19C);
        fn_800F7434();
        r3 = 0x0;
        goto L_8012EF94;
    }
    r28 = r29;
    r26 = 0x0;
    while (r0 = *(u32*)((u8*)r29 + 0x410), (s32)r26 < (s32)r0) {

    r4 = *(u16*)((u8*)r28 + 0x1D0);
    r3 = 0x3;
    fn_80116D30();
    r28 = r28 + 0x34;
    r26 = r26 + 0x1;
    }

    r3 = (u32)&lbl_80426BD0;
    r31 = r29;
    r28 = (u32)&lbl_80426BD0;
    r26 = 0x0;
    while (r0 = *(u32*)((u8*)r28 + 0x418), (s32)r26 < (s32)r0) {

    r4 = *(u16*)((u8*)r31 + 0x370);
    r3 = 0x2;
    fn_80116D30();
    r31 = r31 + 0x34;
    r26 = r26 + 0x1;
    }

    r3 = (u32)&lbl_80426BD0;
    r27 = r29;
    r31 = (u32)&lbl_80426BD0;
    r26 = 0x0;
    while (r0 = *(u32*)((u8*)r31 + 0x414), (s32)r26 < (s32)r0) {

    r4 = *(u16*)((u8*)r27 + 0x2A0);
    r3 = 0x1;
    fn_80116D30();
    r27 = r27 + 0x34;
    r26 = r26 + 0x1;
    }

    r0 = *(u32*)((u8*)r29 + 0x18C);
    r3 = 0x0;
    *(u32*)((u8*)r31 + 0x414) = r3;
    *(u32*)((u8*)r28 + 0x418) = r3;
    *(u32*)((u8*)r29 + 0x410) = r3;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0x0;
        goto L_8012EF94;
    }
    r3 = 0x1;
    fn_800F7AF0();
    r31 = r3;
    r3 = 0x1;
    fn_800F7BC4();
    r0 = r3 & r31;
    r0 = r0 & 0x00001C00;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0x0;
        fn_8000D710();
        r3 = 0x0;
        goto L_8012EF94;
    }
    r28 = 0x0;
    r27 = r29;
    do {
        if ((s32)r28 >= (s32)0x0) {
            if ((s32)r28 < (s32)0x2) goto L_8012EDF4;
        }
        r0 = 0x0;
        goto L_8012EE0C;
    L_8012EDF4: ;
        r0 = *(u16*)((u8*)r27 + 0x4);
        r0 = r0 & 0x1;
        if ((s32)r28 == (s32)0x2) {
            r0 = 0x0;
            goto L_8012EE0C;
        }
        r0 = 0x1;
    L_8012EE0C: ;
        r0 = r0 & 0xFF;
        if ((s32)r28 != (s32)0x2) {
            r3 = (u32)&lbl_80426BD0;
            r0 = *(u32*)&lbl_80426BD0;
            if ((s32)r28 != (s32)r0) {
                r3 = r28;
                fn_8012DE94();
        }
        }
        r28 = r28 + 0x1;
        r27 = r27 + 0x20;
    } while ((s32)r28 < (s32)0x2);
    r3 = (u32)&lbl_80426BD0;
    r3 = (u32)&lbl_80426BD0;
    r3 = *(u32*)((u8*)r3 + 0x0);
    fn_8012E7B8();
    r3 = (u32)lbl_80272A38;
    r4 = (u32)&lbl_80426BD0;
    r10 = (u32)lbl_80272A38;
    f30 = f1;
    r9 = *(u32*)((u8*)r10 + 0x0);
    r11 = (u32)&lbl_80426BD0;
    r8 = *(u32*)((u8*)r10 + 0x4);
    r7 = *(u32*)((u8*)r10 + 0x8);
    r6 = *(u32*)((u8*)r10 + 0xC);
    r5 = *(u32*)((u8*)r10 + 0x10);
    r4 = *(u32*)((u8*)r10 + 0x14);
    r3 = *(u32*)((u8*)r10 + 0x18);
    r0 = *(u32*)((u8*)r10 + 0x1C);
    r31 = *(u32*)((u8*)r11 + 0x0);
    *(u32*)(sp + 0x34) = r0;
    fn_800D3088();
    r3 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0xC) = r0;
    if (((s32)r31 >= (s32)0x0) && ((s32)r31 < (s32)0x2)) {
        r0 = r31 << 2;
        r3 = (u32)sp + 0x8;
        r30 = *(u32*)(r3 + r0);
    }
    r4 = r30;
    r3 = 0x0;
    fn_800F9318();
    if ((u32)r3 != (u32)0x0) {
        r4 = (u32)&lbl_80426BD0;
        r6 = r31 << 5;
        r0 = (u32)&lbl_80426BD0;
        r7 = r31 << 4;
        r5 = (u32)sp + 0x18;
        r4 = r3;
        r6 = r0 + r6;
        r3 = (u32)sp + 0x38;
        r5 = r5 + r7;
        r6 = r6 + 0x14;
        fn_8012BEB4();
    }
    r3 = 0x8ae;
    ((void(*)(void))fn_801906A0)();
    r0 = __cntlzw(r3);
    /* extrwi. r0, r0, 8, 19 */;
    if ((u32)r3 != (u32)0x0) {
        r3 = (u32)&lbl_80426BD0;
        f31 = *(f32*)lbl_8047D064;
        r30 = (u32)&lbl_80426BD0;
        f0 = *(f32*)((u8*)r30 + 0x13C);
        f0 = f0 + f30;
        *(f32*)((u8*)r30 + 0x13C) = f0;
        goto L_8012EF80;
    L_8012EF44: ;
        r28 = 0x0;
        r27 = r29;
        do {
            r12 = *(u32*)((u8*)r27 + 0x140);
            if ((u32)r12 != (u32)0x0) {
                r3 = *(u32*)((u8*)r27 + 0x144);
                ctr_fn = (void(*)(void))r12;
                ctr_fn();
            }
            r28 = r28 + 0x1;
            r27 = r27 + 0x8;
        } while ((s32)r28 < (s32)0x8);
        f0 = *(f32*)((u8*)r30 + 0x13C);
        f0 = f0 - f31;
        *(f32*)((u8*)r30 + 0x13C) = f0;
    L_8012EF80: ;
        f0 = *(f32*)((u8*)r30 + 0x13C);
        /* cror eq, gt, eq */;
        if (f0 == f31) goto L_8012EF44;
    }
    r3 = 0x0;
L_8012EF94: ;
    /* psq_l f31, 0xa8((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0xA0);
    /* psq_l f30, 0x98((u32)sp), 0, qr0 */;
    f30 = *(f64*)(sp + 0x90);
    return;
}
/* 0x50 | fn_8012EFB8 | framed_no_calls */
u32 fn_8012EFB8(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_8047D030, lbl_8047D034 */
    return 1;
}
/* 0x8012F008 | 0x114 */
void fn_8012F008(void) {
    extern u8 lbl_8047D030[];
    extern u8 lbl_8047D034[];
    extern void fn_80188AF4();
    extern void fn_80188F78();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    if ((s32)r0 >= (s32)0 && (s32)r31 < (s32)0x2) {
    if ((s32)r3 >= (s32)0x0 && (s32)r3 < (s32)0x2) {
    r4 = (u32)&lbl_80426BD0;
    r0 = r3 << 5;
    r4 = (u32)&lbl_80426BD0;
    r4 = r4 + r0;
    r0 = *(u16*)((u8*)r4 + 0x4);
    r0 = r0 & 0x1;
    if ((s32)r3 == (s32)0x2) {
        r0 = 0x0;
    } else {
    r0 = 0x1;
    }
    } else {
    r0 = 0x0;
    }
    r0 = r0 & 0xFF;
    if ((s32)r3 == (s32)0x2) {
        r3 = 0x0;
    } else {
    r4 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0xC) = r0;
    if (((s32)r3 >= (s32)0x0) && ((s32)r3 < (s32)0x2)) {
        r0 = r3 << 2;
        r4 = (u32)sp + 0x8;
        r29 = *(u32*)(r4 + r0);
    }
    r4 = (u32)&lbl_80426BD0;
    r0 = r3 << 5;
    r3 = (u32)&lbl_80426BD0;
    r30 = r3 + r0;
    r0 = *(u32*)((u8*)r30 + 0xC);
    if ((s32)r0 != (s32)0x1) {
    } else {
        r4 = r29;
        r3 = 0x0;
        fn_80188AF4();
    }
    if ((s32)r31 != (s32)0x1) {
    } else {
        r4 = r29;
        r3 = 0x0;
        fn_80188F78();
    }
    *(u32*)((u8*)r30 + 0x0) = r31;
    r3 = 0x1;
    }
    } else {
    r3 = 0x0;
    }
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}
/* 0x8012F11C | 0x34 */
u32 fn_8012F11C(s32 idx) {
    u16 val;
    if (idx < 0 || idx >= 2) { return 0; }
    val = *(u16*)(lbl_80426BD0 + (u32)idx * 32 + 4);
    return (u32)(val & 1);
}
/* 0x8012F150 | 0xAC */
void fn_8012F150(void) {
    extern u8 lbl_8047D038[];
    extern u8 lbl_8047D0D4[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    if ((s32)r3 < (s32)0x0) { r3 = 0x0; return; }
    if ((s32)r3 >= (s32)0x2) {
        r3 = 0x0;
        return;
    }
    r4 = (u32)&lbl_80426BD0;
    r0 = *(u32*)&lbl_80426BD0;
    if ((s32)r3 == (s32)r0) {
        r3 = 0x0;
        return;
    }
    r0 = r3 << 5;
    f1 = *(f32*)lbl_8047D038;
    r3 = r4 + r0;
    f2 = *(f32*)lbl_8047D0D4;
    r0 = *(u16*)((u8*)r3 + 0x4);
    r5 = 0x0;
    r0 = r0 & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0x4) = r0;
    r0 = *(u32*)((u8*)r4 + 0x0);
    r0 = r0 << 5;
    r3 = r4 + r0;
    *(f32*)((u8*)r3 + 0x8) = f1;
    r0 = *(u16*)((u8*)r4 + 0x4);
    r0 = r0 & 0x1;
    if ((s32)r3 != (s32)r0) {
        r0 = *(u32*)((u8*)r4 + 0x0);
        if ((s32)r0 != (s32)r5) {
            *(f32*)((u8*)r4 + 0x8) = f2;
            f2 = f2 + f2;
    }
    }
    r0 = *(u16*)((u8*)r4 + 0x24);
    r3 = r4 + 0x20;
    r5 = 0x1;
    r0 = r0 & 0x1;
    if ((s32)r0 != (s32)r5) {
        r0 = *(u32*)((u8*)r4 + 0x0);
        if ((s32)r0 != (s32)r5) {
            *(f32*)((u8*)r3 + 0x8) = f2;
    }
    }
    r3 = 0x1;
    return;
}
/* 0x8012F1FC | 0x210 */
void fn_8012F1FC(void) {
    extern u8 lbl_8047D030[];
    extern u8 lbl_8047D034[];
    extern u8 lbl_8047D038[];
    extern u8 lbl_8047D0D4[];
    extern void fn_80188AF4();
    extern void fn_80188F78();
    extern void fn_8018C1E8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    if ((s32)r0 < (s32)0) { r3 = 0x0; return; }
    if ((s32)r28 >= (s32)0x2) {
        r3 = 0x0;
        return;
    }
    if ((s32)r28 >= (s32)0x0 && (s32)r28 < (s32)0x2) {
    r3 = (u32)&lbl_80426BD0;
    r0 = r28 << 5;
    r3 = (u32)&lbl_80426BD0;
    r3 = r3 + r0;
    r0 = *(u16*)((u8*)r3 + 0x4);
    r0 = r0 & 0x1;
    if ((s32)r28 == (s32)0x2) {
        r0 = 0x0;
    } else {
    r0 = 0x1;
    }
    } else {
    r0 = 0x0;
    }
    r0 = r0 & 0xFF;
    if ((s32)r28 != (s32)0x2) {
        r3 = 0x1;
        return;
    }
    r3 = (u32)&lbl_80426BD0;
    r29 = 0x1;
    r0 = (u32)&lbl_80426BD0;
    r4 = r28 << 5;
    r3 = r0 + r4;
    r0 = *(u16*)((u8*)r3 + 0x4);
    r0 = r0 | 0x1;
    /* sthu r0, 0x4(r3) */;
    if ((s32)r29 >= (s32)0x0 && (s32)r29 < (s32)0x2) {
        if ((s32)r28 >= (s32)0x0 && (s32)r28 < (s32)0x2) {
        r0 = *(u16*)((u8*)r3 + 0x0);
        r0 = r0 & 0x1;
        if ((s32)r28 == (s32)0x2) {
            r0 = 0x0;
        } else {
        r0 = 0x1;
        }
        } else {
        r0 = 0x0;
        }
        r0 = r0 & 0xFF;
        if ((s32)r28 != (s32)0x2) {
            r3 = *(u32*)lbl_8047D030;
            r0 = *(u32*)lbl_8047D034;
            *(u32*)(sp + 0x14) = r0;
            if (((s32)r28 >= (s32)0x0) && ((s32)r28 < (s32)0x2)) {
                r0 = r28 << 2;
                r3 = (u32)sp + 0x10;
                r31 = *(u32*)(r3 + r0);
            }
            r3 = (u32)&lbl_80426BD0;
            r0 = (u32)&lbl_80426BD0;
            r27 = r0 + r4;
            r0 = *(u32*)((u8*)r27 + 0xC);
            if ((s32)r0 != (s32)0x1) {
            } else {
                r4 = r31;
                r3 = 0x0;
                fn_80188AF4();
            }
            if ((s32)r29 != (s32)0x1) {
            } else {
                r4 = r31;
                r3 = 0x0;
                fn_80188F78();
            }
            r0 = 0x1;
            *(u32*)((u8*)r27 + 0x0) = r0;
        }
    }
    r3 = (u32)&lbl_80426BD0;
    f1 = *(f32*)lbl_8047D038;
    r4 = (u32)&lbl_80426BD0;
    f2 = *(f32*)lbl_8047D0D4;
    r0 = *(u32*)((u8*)r4 + 0x0);
    r5 = 0x0;
    r0 = r0 << 5;
    r3 = r4 + r0;
    *(f32*)((u8*)r3 + 0x8) = f1;
    r0 = *(u16*)((u8*)r4 + 0x4);
    r0 = r0 & 0x1;
    if ((s32)r29 != (s32)0x1) {
        r0 = *(u32*)((u8*)r4 + 0x0);
        if ((s32)r0 != (s32)r5) {
            *(f32*)((u8*)r4 + 0x8) = f2;
            f2 = f2 + f2;
    }
    }
    r0 = *(u16*)((u8*)r4 + 0x24);
    r3 = r4 + 0x20;
    r5 = 0x1;
    r0 = r0 & 0x1;
    if ((s32)r0 != (s32)r5) {
        r0 = *(u32*)((u8*)r4 + 0x0);
        if ((s32)r0 != (s32)r5) {
            *(f32*)((u8*)r3 + 0x8) = f2;
    }
    }
    r3 = *(u32*)lbl_8047D030;
    r0 = *(u32*)lbl_8047D034;
    *(u32*)(sp + 0xC) = r0;
    if (((s32)r28 >= (s32)0x0) && ((s32)r28 < (s32)0x2)) {
        r0 = r28 << 2;
        r3 = (u32)sp + 0x8;
        r30 = *(u32*)(r3 + r0);
    }
    r4 = r30;
    r3 = 0x0;
    r5 = 0x1;
    fn_8018C1E8();
    r3 = 0x1;
    return;
}
/* 0x8012F40C | 0x204 */
void fn_8012F40C(void) {
    extern u8 lbl_8047D030[];
    extern u8 lbl_8047D034[];
    extern u8 lbl_8047D038[];
    extern u8 lbl_8047D0D4[];
    extern void fn_80188AF4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    if ((s32)r3 >= (s32)0x0 && (s32)r3 < (s32)0x2) {
    r5 = (u32)&lbl_80426BD0;
    r0 = r3 << 5;
    r5 = (u32)&lbl_80426BD0;
    r5 = r5 + r0;
    r0 = *(u16*)((u8*)r5 + 0x4);
    r0 = r0 & 0x1;
    if ((s32)r3 == (s32)0x2) {
        r0 = 0x0;
    } else {
    r0 = 0x1;
    }
    } else {
    r0 = 0x0;
    }
    r0 = r0 & 0xFF;
    if ((s32)r3 == (s32)0x2) {
        r3 = 0x0;
        r31 = *(u32*)(sp + 0x1C);
        return;
    }
    r5 = (u32)&lbl_80426BD0;
    /* stwu r3, lbl_80426BD0@l(r5) */;
    if ((s32)r3 >= (s32)0x0 && (s32)r3 < (s32)0x2) {
    r0 = r3 << 5;
    r5 = r5 + r0;
    r0 = *(u16*)((u8*)r5 + 0x4);
    r0 = r0 & 0x1;
    if ((s32)r3 == (s32)0x2) {
        r0 = 0x0;
    } else {
    r0 = 0x1;
    }
    } else {
    r0 = 0x0;
    }
    r0 = r0 & 0xFF;
    if ((s32)r3 != (s32)0x2) {
        r5 = (u32)&lbl_80426BD0;
        r0 = r3 << 5;
        r5 = (u32)&lbl_80426BD0;
        r5 = r5 + r0;
        r0 = *(u32*)((u8*)r5 + 0xC);
    } else {
        r0 = 0x2;
    }
    if ((s32)r0 == (s32)0x1) {
        if ((s32)r3 >= (s32)0x0 && (s32)r3 < (s32)0x2) {
        r5 = (u32)&lbl_80426BD0;
        r0 = r3 << 5;
        r5 = (u32)&lbl_80426BD0;
        r5 = r5 + r0;
        r0 = *(u16*)((u8*)r5 + 0x4);
        r0 = r0 & 0x1;
        if ((s32)r3 == (s32)0x2) {
            r0 = 0x0;
        } else {
        r0 = 0x1;
        }
        } else {
        r0 = 0x0;
        }
        r0 = r0 & 0xFF;
        if ((s32)r3 != (s32)0x2) {
            r5 = *(u32*)lbl_8047D030;
            r0 = *(u32*)lbl_8047D034;
            *(u32*)(sp + 0xC) = r0;
            if (((s32)r3 >= (s32)0x0) && ((s32)r3 < (s32)0x2)) {
                r0 = r3 << 2;
                r4 = (u32)sp + 0x8;
                r4 = *(u32*)(r4 + r0);
            }
            r5 = (u32)&lbl_80426BD0;
            r0 = r3 << 5;
            r3 = (u32)&lbl_80426BD0;
            r31 = r3 + r0;
            r0 = *(u32*)((u8*)r31 + 0xC);
            if ((s32)r0 != (s32)0x1) {
            } else {
                r3 = 0x0;
                fn_80188AF4();
            }
            r0 = 0x0;
            *(u32*)((u8*)r31 + 0x0) = r0;
        }
    }
    r3 = (u32)&lbl_80426BD0;
    f1 = *(f32*)lbl_8047D038;
    r4 = (u32)&lbl_80426BD0;
    f2 = *(f32*)lbl_8047D0D4;
    r0 = *(u32*)((u8*)r4 + 0x0);
    r5 = 0x0;
    r0 = r0 << 5;
    r3 = r4 + r0;
    *(f32*)((u8*)r3 + 0x8) = f1;
    r0 = *(u16*)((u8*)r4 + 0x4);
    r0 = r0 & 0x1;
    if ((s32)r0 != (s32)0x1) {
        r0 = *(u32*)((u8*)r4 + 0x0);
        if ((s32)r0 != (s32)r5) {
            *(f32*)((u8*)r4 + 0x8) = f2;
            f2 = f2 + f2;
    }
    }
    r0 = *(u16*)((u8*)r4 + 0x24);
    r3 = r4 + 0x20;
    r5 = 0x1;
    r0 = r0 & 0x1;
    if ((s32)r0 != (s32)r5) {
        r0 = *(u32*)((u8*)r4 + 0x0);
        if ((s32)r0 != (s32)r5) {
            *(f32*)((u8*)r3 + 0x8) = f2;
    }
    }
    r3 = (u32)&lbl_80426BD0;
    r0 = 0x0;
    r4 = (u32)&lbl_80426BD0;
    r3 = 0x1;
    *(u32*)((u8*)r4 + 0x44) = r0;
    *(u32*)((u8*)r4 + 0x48) = r0;
    r31 = *(u32*)(sp + 0x1C);
    return;
}
/* 0x8012F610 | 0x4C8 */
void fn_8012F610(void) {
    extern u8 lbl_8047D030[];
    extern u8 lbl_8047D034[];
    extern u8 lbl_8047D038[];
    extern u8 lbl_8047D07C[];
    extern u8 lbl_8047D0AC[];
    extern u8 lbl_8047D0D8[];
    extern void fn_800CDBE0();
    extern void fn_800CE148();
    extern void fn_800E3D6C();
    extern void fn_800E3D98();
    extern void fn_800E4170();
    extern void fn_800F9318();
    extern void fn_800FF548();
    extern void fn_8010E138();
    extern void fn_80188AF4();
    extern void fn_80188F78();
    extern void fn_8018C0A8();
    extern void fn_8018C1E8();
    u8 sp[0x110];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;
    *(f64*)(sp + 0x100) = f31;
    /* psq_st f31, 0x108((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0xF0) = f30;
    /* psq_st f30, 0xf8((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0xE0) = f29;
    /* psq_st f29, 0xe8((u32)sp), 0, qr0 */;
    fn_800FF548();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r4 = (u32)&lbl_80426BD0;
        r3 = *(u32*)lbl_8047D030;
        r4 = (u32)&lbl_80426BD0;
        r0 = *(u32*)lbl_8047D034;
        r4 = *(u32*)((u8*)r4 + 0x0);
        *(u32*)(sp + 0x1C) = r0;
        if (((s32)r4 >= (s32)0x0) && ((s32)r4 < (s32)0x2)) {
            r0 = r4 << 2;
            r3 = (u32)sp + 0x18;
            r24 = *(u32*)(r3 + r0);
        }
        r4 = r24;
        r3 = 0x0;
        fn_800F9318();
        r4 = (u32)sp + 0x44;
        fn_800E3D98();
        r4 = (u32)&lbl_80426BD0;
        r3 = *(u32*)lbl_8047D030;
        r4 = (u32)&lbl_80426BD0;
        r0 = *(u32*)lbl_8047D034;
        r4 = *(u32*)((u8*)r4 + 0x0);
        *(u32*)(sp + 0x14) = r0;
        if (((s32)r4 >= (s32)0x0) && ((s32)r4 < (s32)0x2)) {
            r0 = r4 << 2;
            r3 = (u32)sp + 0x10;
            r28 = *(u32*)(r3 + r0);
        }
        r4 = r28;
        r3 = 0x0;
        fn_800F9318();
        r4 = (u32)sp + 0x50;
        fn_800E3D6C();
        f1 = *(f32*)(sp + 0x54);
        fn_800CE148();
        f31 = (f32)f1;
        f1 = *(f32*)(sp + 0x54);
        fn_800CDBE0();
        f1 = (f32)f1;
        f0 = *(f32*)(sp + 0x48);
        r3 = (u32)&lbl_80426BD0;
        f31 = -f31;
        *(f32*)(sp + 0x3C) = f0;
        r28 = (u32)&lbl_80426BD0;
        f30 = -f1;
        f29 = *(f32*)lbl_8047D0AC;
        r26 = (u32)sp + 0x30;
        r25 = (u32)sp + 0x8;
        r24 = 0x0;
        do {
            if ((s32)r24 >= (s32)0x0) {
                if ((s32)r24 < (s32)0x2) goto L_8012F734;
            }
            r0 = 0x0;
            goto L_8012F74C;
        L_8012F734: ;
            r0 = *(u16*)((u8*)r28 + 0x4);
            r0 = r0 & 0x1;
            if ((s32)r24 == (s32)0x2) {
                r0 = 0x0;
                goto L_8012F74C;
            }
            r0 = 0x1;
        L_8012F74C: ;
            r0 = r0 & 0xFF;
            if ((s32)r24 != (s32)0x2) {
                r3 = (u32)&lbl_80426BD0;
                r0 = *(u32*)&lbl_80426BD0;
                if ((s32)r24 != (s32)r0) {
                    f1 = *(f32*)(sp + 0x44);
                    r3 = (u32)sp + 0x38;
                    f0 = *(f32*)(sp + 0x4C);
                    r4 = (u32)sp + 0x5c;
                    f2 = f31 * f29 + f1;
                    f1 = *(f32*)(sp + 0x48);
                    f0 = f30 * f29 + f0;
                    *(f32*)(sp + 0x3C) = f1;
                    *(f32*)(sp + 0x38) = f2;
                    *(f32*)(sp + 0x40) = f0;
                    fn_8010E138();
                    if ((s32)r3 > (s32)0x0) {
                        if ((s32)r3 < (s32)0x2) goto L_8012F814;
                        f5 = *(f32*)lbl_8047D0D8;
                        r4 = (u32)sp + 0x5c;
                        f2 = *(f32*)(sp + 0x3C);
                        r0 = 0x0;
                        f4 = f5;
                        f0 = *(f32*)lbl_8047D07C;
                        ctr_fn = (void(*)(void))r3;
                        if ((s32)r3 > (s32)0x0) {
                            do {
                                f3 = *(f32*)((u8*)r4 + 0x0);
                                if (f4 < f3) {
                                    f4 = f3;
                                }
                                f1 = f3 - f2;
                                /* cror eq, gt, eq */;
                                if ((f1 != f0) && (f5 < f3)) {
                                    f5 = f3;
                                    r0 = 0x1;
                                }
                                r4 = r4 + 0xc;
                            } while (--ctr != 0);
                        }
                        if ((s32)r0 == (s32)0x0) goto L_8012F80C;
                        *(f32*)(sp + 0x3C) = f5;
                }
                    goto L_8012F81C;
                L_8012F80C: ;
                    *(f32*)(sp + 0x3C) = f4;
                    goto L_8012F81C;
                L_8012F814: ;
                    f0 = *(f32*)(sp + 0x5C);
                    *(f32*)(sp + 0x3C) = f0;
                L_8012F81C: ;
                    f0 = *(f32*)lbl_8047D0AC;
                    r3 = *(u32*)lbl_8047D030;
                    r0 = *(u32*)lbl_8047D034;
                    f29 = f29 + f0;
                    *(u32*)(sp + 0x34) = r0;
                    if (((s32)r24 >= (s32)0x0) && ((s32)r24 < (s32)0x2)) {
                        r31 = *(u32*)((u8*)r26 + 0x0);
                    }
                    r4 = r31;
                    r5 = (u32)sp + 0x38;
                    r3 = 0x0;
                    fn_8018C0A8();
                    r3 = *(u32*)lbl_8047D030;
                    r0 = *(u32*)lbl_8047D034;
                    *(u32*)(sp + 0xC) = r0;
                    if (((s32)r24 >= (s32)0x0) && ((s32)r24 < (s32)0x2)) {
                        r27 = *(u32*)((u8*)r25 + 0x0);
                    }
                    r4 = r27;
                    r3 = 0x0;
                    fn_800F9318();
                    r4 = (u32)sp + 0x50;
                    fn_800E4170();
                }
            }
            r24 = r24 + 0x1;
            r26 = r26 + 0x4;
            r25 = r25 + 0x4;
            r28 = r28 + 0x20;
        } while ((s32)r24 < (s32)0x2);
    }
    r3 = (u32)&lbl_80426BD0;
    r25 = (u32)sp + 0x28;
    r26 = (u32)&lbl_80426BD0;
    r27 = 0x0;
    r24 = r26;
    do {
        if ((s32)r27 >= (s32)0x0) {
            if ((s32)r27 < (s32)0x2) goto L_8012F8D4;
        }
        r5 = 0x0;
        goto L_8012F8EC;
    L_8012F8D4: ;
        r0 = *(u16*)((u8*)r24 + 0x4);
        r0 = r0 & 0x1;
        if ((s32)r27 == (s32)0x2) {
            r5 = 0x0;
            goto L_8012F8EC;
        }
        r5 = 0x1;
    L_8012F8EC: ;
        r3 = *(u32*)lbl_8047D030;
        r0 = *(u32*)lbl_8047D034;
        *(u32*)(sp + 0x2C) = r0;
        if (((s32)r27 >= (s32)0x0) && ((s32)r27 < (s32)0x2)) {
            r30 = *(u32*)((u8*)r25 + 0x0);
        }
        r4 = r30;
        r3 = 0x0;
        fn_8018C1E8();
        r27 = r27 + 0x1;
        r25 = r25 + 0x4;
        r24 = r24 + 0x20;
    } while ((s32)r27 < (s32)0x2);
    f0 = *(f32*)lbl_8047D038;
    r25 = (u32)sp + 0x20;
    r27 = 0x0;
    *(f32*)((u8*)r26 + 0x14) = f0;
    *(f32*)((u8*)r26 + 0x18) = f0;
    *(f32*)((u8*)r26 + 0x1C) = f0;
    *(f32*)((u8*)r26 + 0x20) = f0;
    *(f32*)((u8*)r26 + 0x34) = f0;
    *(f32*)((u8*)r26 + 0x38) = f0;
    *(f32*)((u8*)r26 + 0x3C) = f0;
    *(f32*)((u8*)r26 + 0x40) = f0;
    do {
        if ((s32)r27 >= (s32)0x0) {
            if ((s32)r27 < (s32)0x2) goto L_8012F974;
        }
        r0 = 0x0;
        goto L_8012F98C;
    L_8012F974: ;
        r0 = *(u16*)((u8*)r26 + 0x4);
        r0 = r0 & 0x1;
        if ((s32)r27 == (s32)0x2) {
            r0 = 0x0;
            goto L_8012F98C;
        }
        r0 = 0x1;
    L_8012F98C: ;
        r0 = r0 & 0xFF;
        if ((s32)r27 != (s32)0x2) {
            if ((s32)r27 >= (s32)0x0) {
                if ((s32)r27 < (s32)0x2) goto L_8012F9AC;
            }
            r0 = 0x0;
            goto L_8012F9C4;
        L_8012F9AC: ;
            r0 = *(u16*)((u8*)r26 + 0x4);
            r0 = r0 & 0x1;
            if ((s32)r27 == (s32)0x2) {
                r0 = 0x0;
                goto L_8012F9C4;
            }
            r0 = 0x1;
        L_8012F9C4: ;
            r0 = r0 & 0xFF;
            if ((s32)r27 != (s32)0x2) {
                r24 = *(u32*)((u8*)r26 + 0xC);
            } else {
                r24 = 0x2;
            }
            if ((s32)r24 >= (s32)0x0 && (s32)r24 < (s32)0x2) {
                if ((s32)r27 >= (s32)0x0) {
                    if ((s32)r27 < (s32)0x2) goto L_8012FA00;
                }
                r0 = 0x0;
                goto L_8012FA18;
            L_8012FA00: ;
                r0 = *(u16*)((u8*)r26 + 0x4);
                r0 = r0 & 0x1;
                if ((s32)r27 == (s32)0x2) {
                    r0 = 0x0;
                    goto L_8012FA18;
                }
                r0 = 0x1;
            L_8012FA18: ;
                r0 = r0 & 0xFF;
                if ((s32)r27 != (s32)0x2) {
                    r3 = *(u32*)lbl_8047D030;
                    r0 = *(u32*)lbl_8047D034;
                    *(u32*)(sp + 0x24) = r0;
                    if (((s32)r27 >= (s32)0x0) && ((s32)r27 < (s32)0x2)) {
                        r29 = *(u32*)((u8*)r25 + 0x0);
                    }
                    r0 = *(u32*)((u8*)r26 + 0xC);
                    if ((s32)r0 != (s32)0x1) {
                    } else {
                        r4 = r29;
                        r3 = 0x0;
                        fn_80188AF4();
                    }
                    if ((s32)r24 != (s32)0x1) {
                    } else {
                        r4 = r29;
                        r3 = 0x0;
                        fn_80188F78();
                    }
                    *(u32*)((u8*)r26 + 0xC) = r24;
            }
            }
        }
        r27 = r27 + 0x1;
        r25 = r25 + 0x4;
        r26 = r26 + 0x20;
    } while ((s32)r27 < (s32)0x2);
    r3 = (u32)&lbl_80426BD0;
    f0 = *(f32*)lbl_8047D038;
    r3 = (u32)&lbl_80426BD0;
    r0 = 0x12c;
    *(u32*)((u8*)r3 + 0x10) = r0;
    *(u32*)((u8*)r3 + 0x30) = r0;
    *(f32*)((u8*)r3 + 0x13C) = f0;
    /* psq_l f31, 0x108((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x100);
    /* psq_l f30, 0xf8((u32)sp), 0, qr0 */;
    f30 = *(f64*)(sp + 0xF0);
    /* psq_l f29, 0xe8((u32)sp), 0, qr0 */;
    f29 = *(f64*)(sp + 0xE0);
    return;
}
/* 0x8012FAD8 | 0x1FC */
void fn_8012FAD8(void) {
    extern u8 lbl_802729C0[];
    extern u8 lbl_80272A10[];
    extern void fn_8006AE18();
    extern void fn_8011393C();
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;
    r3 = (u32)lbl_802729C0;
    r0 = 0xa;
    r5 = (u32)sp + 0x2c;
    ctr_fn = (void(*)(void))r0;
    do {
        r3 = *(u32*)((u8*)r4 + 0x4);
        r0 = *(u32*)((u8*)r4 + 0x8);
        *(u32*)((u8*)r5 + 0x4) = r3;
        r5 += 8; *(u32*)r5 = r0;
    } while (--ctr != 0);
    r4 = (u32)lbl_80272A10;
    r3 = 0x8ae;
    r31 = (u32)lbl_80272A10;
    r12 = *(u32*)((u8*)r31 + 0x0);
    r11 = *(u32*)((u8*)r31 + 0x4);
    r10 = *(u32*)((u8*)r31 + 0x8);
    r9 = *(u32*)((u8*)r31 + 0xC);
    r8 = *(u32*)((u8*)r31 + 0x10);
    r7 = *(u32*)((u8*)r31 + 0x14);
    r6 = *(u32*)((u8*)r31 + 0x18);
    r5 = *(u32*)((u8*)r31 + 0x1C);
    r4 = *(u32*)((u8*)r31 + 0x20);
    r0 = *(u32*)((u8*)r31 + 0x24);
    *(u32*)(sp + 0x2C) = r0;
    ((void(*)(void))fn_801906A0)();
    r0 = __cntlzw(r3);
    /* extrwi. r0, r0, 8, 19 */;
    if ((s32)r0 != (s32)0) {
        r3 = (0xf7 << 16);
        r3 = r3 + 0x400;
        r31 = *(u32*)(sp + 0x8C);
        return;
    }
    fn_8011393C();
    r0 = 0x2;
    r4 = (u32)sp + 0x30;
    r5 = 0x0;
    ctr_fn = (void(*)(void))r0;
    do {
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r3 != (u32)r0) {
        r0 = *(u32*)((u8*)r4 + 0x4);
        r5 = r5 + 0x1;
        if ((u32)r3 != (u32)r0) {
            r0 = *(u32*)((u8*)r4 + 0x8);
            r5 = r5 + 0x1;
            if ((u32)r3 != (u32)r0) {
                r0 = *(u32*)((u8*)r4 + 0xC);
                r5 = r5 + 0x1;
                if ((u32)r3 != (u32)r0) {
                    r0 = *(u32*)((u8*)r4 + 0x10);
                    r5 = r5 + 0x1;
                    if ((u32)r3 != (u32)r0) {
                        r0 = *(u32*)((u8*)r4 + 0x14);
                        r5 = r5 + 0x1;
                        if ((u32)r3 != (u32)r0) {
                            r0 = *(u32*)((u8*)r4 + 0x18);
                            r5 = r5 + 0x1;
                            if ((u32)r3 != (u32)r0) {
                                r0 = *(u32*)((u8*)r4 + 0x1C);
                                r5 = r5 + 0x1;
                                if ((u32)r3 != (u32)r0) {
                                    r0 = *(u32*)((u8*)r4 + 0x20);
                                    r5 = r5 + 0x1;
                                    if ((u32)r3 != (u32)r0) {
                                        r0 = *(u32*)((u8*)r4 + 0x24);
                                        r5 = r5 + 0x1;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    if ((u32)r3 == (u32)r0) break;
    r4 = r4 + 0x28;
    r5 = r5 + 0x1;
    } while (--ctr != 0);
    if ((s32)r5 >= (s32)0x14) {
        r3 = (0xf7 << 16);
        r3 = r3 + 0x400;
        r31 = *(u32*)(sp + 0x8C);
        return;
    }
    fn_8006AE18();
    r0 = *(u32*)(sp + 0x8);
    r4 = 0x0;
    if ((s32)r3 != (s32)r0) {
        r0 = *(u32*)(sp + 0x10);
        r4 = 0x1;
        if ((s32)r3 != (s32)r0) {
            r0 = *(u32*)(sp + 0x18);
            r4 = 0x2;
            if ((s32)r3 != (s32)r0) {
                r0 = *(u32*)(sp + 0x20);
                r4 = 0x3;
                if ((s32)r3 != (s32)r0) {
                    r0 = *(u32*)(sp + 0x28);
                    r4 = 0x4;
                    if ((s32)r3 != (s32)r0) {
                        r4 = 0x5;
    }
    }
    }
    }
    }
    r0 = r4 << 3;
    r3 = (u32)sp + 0xc;
    r3 = *(u32*)(r3 + r0);
    r31 = *(u32*)(sp + 0x8C);
    return;
}
/* 0x8012FCD4 | 0x380 */
void fn_8012FCD4(void) {
    extern u8 lbl_802729C0[];
    extern u8 lbl_80272A10[];
    extern u8 lbl_8047D030[];
    extern u8 lbl_8047D034[];
    extern u8 lbl_8047D038[];
    extern void fn_8006AE18();
    extern void fn_800EB528();
    extern void fn_800F9318();
    extern void fn_800FF548();
    extern void fn_8011393C();
    extern void fn_8012C660();
    extern void fn_8012F610();
    extern void fn_80189328();
    extern void fn_8018BF24();
    extern void fn_8018C0A8();
    extern void fn_8018C8F4();
    extern void fn_8018CB5C();
    extern void fn_8018D998();
    extern void fn_8018E050();
    u8 sp[0xB0];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;
    r29 = r3;
    r30 = r4;
    fn_800FF548();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = (u32)lbl_802729C0;
        r0 = 0xa;
        r3 = (u32)lbl_802729C0;
        r5 = (u32)sp + 0x3c;
        ctr_fn = (void(*)(void))r0;
        do {
            r3 = *(u32*)((u8*)r4 + 0x4);
            r0 = *(u32*)((u8*)r4 + 0x8);
            *(u32*)((u8*)r5 + 0x4) = r3;
            r5 += 8; *(u32*)r5 = r0;
        } while (--ctr != 0);
        r4 = (u32)lbl_80272A10;
        r3 = 0x8ae;
        r28 = (u32)lbl_80272A10;
        r12 = *(u32*)((u8*)r28 + 0x0);
        r11 = *(u32*)((u8*)r28 + 0x4);
        r10 = *(u32*)((u8*)r28 + 0x8);
        r9 = *(u32*)((u8*)r28 + 0xC);
        r8 = *(u32*)((u8*)r28 + 0x10);
        r7 = *(u32*)((u8*)r28 + 0x14);
        r6 = *(u32*)((u8*)r28 + 0x18);
        r5 = *(u32*)((u8*)r28 + 0x1C);
        r4 = *(u32*)((u8*)r28 + 0x20);
        r0 = *(u32*)((u8*)r28 + 0x24);
        *(u32*)(sp + 0x3C) = r0;
        ((void(*)(void))fn_801906A0)();
        r0 = __cntlzw(r3);
        /* extrwi. r0, r0, 8, 19 */;
        if ((s32)r0 != (s32)0) {
            r3 = (0xf7 << 16);
            r5 = r3 + 0x400;
            goto L_8012FED0;
        }
        fn_8011393C();
        r0 = 0x2;
        r5 = (u32)sp + 0x40;
        r4 = 0x0;
        ctr_fn = (void(*)(void))r0;
    L_8012FDB0: ;
        r0 = *(u32*)((u8*)r5 + 0x0);
        if ((u32)r3 != (u32)r0) {
            r0 = *(u32*)((u8*)r5 + 0x4);
            r4 = r4 + 0x1;
            if ((u32)r3 != (u32)r0) {
                r0 = *(u32*)((u8*)r5 + 0x8);
                r4 = r4 + 0x1;
                if ((u32)r3 != (u32)r0) {
                    r0 = *(u32*)((u8*)r5 + 0xC);
                    r4 = r4 + 0x1;
                    if ((u32)r3 != (u32)r0) {
                        r0 = *(u32*)((u8*)r5 + 0x10);
                        r4 = r4 + 0x1;
                        if ((u32)r3 != (u32)r0) {
                            r0 = *(u32*)((u8*)r5 + 0x14);
                            r4 = r4 + 0x1;
                            if ((u32)r3 != (u32)r0) {
                                r0 = *(u32*)((u8*)r5 + 0x18);
                                r4 = r4 + 0x1;
                                if ((u32)r3 != (u32)r0) {
                                    r0 = *(u32*)((u8*)r5 + 0x1C);
                                    r4 = r4 + 0x1;
                                    if ((u32)r3 != (u32)r0) {
                                        r0 = *(u32*)((u8*)r5 + 0x20);
                                        r4 = r4 + 0x1;
                                        if ((u32)r3 != (u32)r0) {
                                            r0 = *(u32*)((u8*)r5 + 0x24);
                                            r4 = r4 + 0x1;
        }
        }
        }
        }
        }
        }
        }
        }
        }
        if ((u32)r3 == (u32)r0) goto L_8012FE58;
        r5 = r5 + 0x28;
        r4 = r4 + 0x1;
        if (--ctr != 0) goto L_8012FDB0;
    L_8012FE58: ;
        if ((s32)r4 >= (s32)0x14) {
            r3 = (0xf7 << 16);
            r5 = r3 + 0x400;
            goto L_8012FED0;
        }
        fn_8006AE18();
        r0 = *(u32*)(sp + 0x18);
        r4 = 0x0;
        if ((s32)r3 != (s32)r0) {
            r0 = *(u32*)(sp + 0x20);
            r4 = 0x1;
            if ((s32)r3 != (s32)r0) {
                r0 = *(u32*)(sp + 0x28);
                r4 = 0x2;
                if ((s32)r3 != (s32)r0) {
                    r0 = *(u32*)(sp + 0x30);
                    r4 = 0x3;
                    if ((s32)r3 != (s32)r0) {
                        r0 = *(u32*)(sp + 0x38);
                        r4 = 0x4;
                        if ((s32)r3 != (s32)r0) {
                            r4 = 0x5;
        }
        }
        }
        }
        }
        r0 = r4 << 3;
        r3 = (u32)sp + 0x1c;
        r5 = *(u32*)(r3 + r0);
    L_8012FED0: ;
        r3 = 0x0;
        r4 = 0x64;
        fn_8018E050();
        r4 = (0xf3 << 16);
        r3 = 0x0;
        r5 = r4 + 0x400;
        r4 = 0x65;
        fn_8018E050();
        goto L_8012FF0C;
    }
    r3 = 0x0;
    r4 = 0x64;
    fn_8018D998();
    r3 = 0x0;
    r4 = 0x65;
    fn_8018D998();
L_8012FF0C: ;
    r26 = (u32)sp + 0x10;
    r28 = (u32)sp + 0x8;
    r27 = r26;
    r25 = 0x0;
    do {
        r3 = *(u32*)lbl_8047D030;
        r0 = *(u32*)lbl_8047D034;
        *(u32*)(sp + 0xC) = r0;
        if (((s32)r25 >= (s32)0x0) && ((s32)r25 < (s32)0x2)) {
            r31 = *(u32*)((u8*)r28 + 0x0);
        }
        r4 = r31;
        r3 = 0x0;
        fn_800F9318();
        *(u32*)((u8*)r27 + 0x0) = r3;
        r3 = *(u32*)((u8*)r27 + 0x0);
        fn_800EB528();
        r25 = r25 + 0x1;
        r27 = r27 + 0x4;
        r28 = r28 + 0x4;
    } while ((s32)r25 < (s32)0x2);
    r3 = 0x0;
    r4 = 0x64;
    fn_8018CB5C();
    r3 = 0x0;
    r4 = 0x65;
    fn_8018CB5C();
    fn_800FF548();
    r0 = r3 & 0xFF;
    if ((s32)r25 == (s32)0x2) {
        r4 = (0x4000 << 16);
        r3 = 0x0;
        r5 = r4 + 0xf00;
        r4 = 0x64;
        fn_8018C8F4();
        r3 = 0x0;
        r4 = 0x65;
        r5 = 0x701;
        fn_8018C8F4();
    }
    r3 = 0x0;
    r4 = 0x65;
    r5 = 0x1;
    fn_80189328();
    fn_800FF548();
    r0 = r3 & 0xFF;
    if ((s32)r25 == (s32)0x2) {
        r5 = r29;
        r3 = 0x0;
        r4 = 0x64;
        fn_8018C0A8();
        r5 = r30;
        r3 = 0x0;
        r4 = 0x64;
        fn_8018BF24();
    }
    r27 = 0x0;
    do {
        r3 = *(u32*)((u8*)r26 + 0x0);
        r4 = r27;
        f1 = *(f32*)lbl_8047D038;
        fn_8012C660();
        r27 = r27 + 0x1;
        r26 = r26 + 0x4;
    } while ((s32)r27 < (s32)0x2);
    fn_8012F610();
    r3 = (u32)&lbl_80426BD0;
    r0 = 0x0;
    r4 = (u32)&lbl_80426BD0;
    r3 = 0x0;
    *(u32*)((u8*)r4 + 0x44) = r0;
    *(u32*)((u8*)r4 + 0x48) = r0;
    *(u32*)((u8*)r4 + 0x414) = r0;
    *(u32*)((u8*)r4 + 0x418) = r0;
    *(u32*)((u8*)r4 + 0x410) = r0;
    *(u32*)((u8*)r4 + 0x188) = r0;
    return;
}
/* 0x80130054 | 0x1F8 */
void fn_80130054(void) {
    extern u8 lbl_8047D030[];
    extern u8 lbl_8047D034[];
    extern u8 lbl_8047D038[];
    extern u8 lbl_8047D0D4[];
    extern void fn_8012A5B0();
    extern void fn_80188AF4();
    extern void fn_80188F78();
    extern void fn_8018C1E8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    r3 = 0x8ae;
    r29 = 0x0;
    ((void(*)(void))fn_801906A0)();
    r0 = __cntlzw(r3);
    /* extrwi. r0, r0, 8, 19 */;
    if ((s32)r0 != (s32)0) {
        r3 = 0x0;
        r4 = 0x18;
        r5 = 0x0;
        fn_8012A5B0();
        if ((s32)r3 != (s32)0x0) {
            r29 = 0x1;
    }
    }
    r0 = r29 & 0xFF;
    if ((s32)r3 != (s32)0x0) {
        r3 = (u32)&lbl_80426BD0;
        r31 = (u32)&lbl_80426BD0;
        r3 = *(u16*)((u8*)r31 + 0x24);
        r0 = r3 & 0x1;
        if ((s32)r3 == (s32)0x0) {
            r29 = 0x1;
            r0 = r3 | 0x1;
            *(u16*)((u8*)r31 + 0x24) = r0;
            if ((s32)r29 >= (s32)0x0) {
                if ((s32)r29 < (s32)0x2) {
                    r0 = r0 & 0x1;
                    if ((s32)r29 != (s32)0x2) {
                        r0 = *(u32*)((u8*)r31 + 0x2C);
                        r30 = *(u32*)lbl_8047D034;
                        r3 = *(u32*)lbl_8047D030;
                        if ((s32)r0 != (s32)0x1) {
                        } else {
                            r4 = r30;
                            r3 = 0x0;
                            fn_80188AF4();
                        }
                        if ((s32)r29 != (s32)0x1) {
                        } else {
                            r4 = r30;
                            r3 = 0x0;
                            fn_80188F78();
                        }
                        r0 = 0x1;
                        *(u32*)((u8*)r31 + 0x2C) = r0;
            }
            }
            }
            r3 = (u32)&lbl_80426BD0;
            f1 = *(f32*)lbl_8047D038;
            r4 = (u32)&lbl_80426BD0;
            f2 = *(f32*)lbl_8047D0D4;
            r0 = *(u32*)((u8*)r4 + 0x0);
            r5 = 0x0;
            r0 = r0 << 5;
            r3 = r4 + r0;
            *(f32*)((u8*)r3 + 0x8) = f1;
            r0 = *(u16*)((u8*)r4 + 0x4);
            r0 = r0 & 0x1;
            if ((s32)r29 != (s32)0x1) {
                r0 = *(u32*)((u8*)r4 + 0x0);
                if ((s32)r0 != (s32)r5) {
                    *(f32*)((u8*)r4 + 0x8) = f2;
                    f2 = f2 + f2;
            }
            }
            r0 = *(u16*)((u8*)r4 + 0x24);
            r3 = r4 + 0x20;
            r5 = 0x1;
            r0 = r0 & 0x1;
            if ((s32)r0 != (s32)r5) {
                r0 = *(u32*)((u8*)r4 + 0x0);
                if ((s32)r0 != (s32)r5) {
                    *(f32*)((u8*)r3 + 0x8) = f2;
            }
            }
            r4 = *(u32*)lbl_8047D034;
            r3 = 0x0;
            r0 = *(u32*)lbl_8047D030;
            r5 = 0x1;
            *(u32*)(sp + 0x8) = r0;
            fn_8018C1E8();
        }
    } else {
    r3 = (u32)&lbl_80426BD0;
    r5 = (u32)&lbl_80426BD0;
    r0 = *(u32*)((u8*)r5 + 0x0);
    if ((s32)r0 != (s32)0x1) {
        r4 = *(u16*)((u8*)r5 + 0x24);
        r0 = r0 << 5;
        r3 = r5 + r0;
        f1 = *(f32*)lbl_8047D038;
        r0 = r4 & 0x0000FFFE;
        f2 = *(f32*)lbl_8047D0D4;
        *(u16*)((u8*)r5 + 0x24) = r0;
        r4 = 0x0;
        *(f32*)((u8*)r3 + 0x8) = f1;
        r0 = *(u16*)((u8*)r5 + 0x4);
        r0 = r0 & 0x1;
        if ((s32)r0 != (s32)0x1) {
            r0 = *(u32*)((u8*)r5 + 0x0);
            if ((s32)r0 != (s32)r4) {
                *(f32*)((u8*)r5 + 0x8) = f2;
                f2 = f2 + f2;
        }
        }
        r0 = *(u16*)((u8*)r5 + 0x24);
        r3 = r5 + 0x20;
        r4 = 0x1;
        r0 = r0 & 0x1;
        if ((s32)r0 != (s32)r4) {
            r0 = *(u32*)((u8*)r5 + 0x0);
            if ((s32)r0 != (s32)r4) {
                *(f32*)((u8*)r3 + 0x8) = f2;
    }
    }
    }
    }
    r31 = *(u32*)(sp + 0x2C);
    r30 = *(u32*)(sp + 0x28);
    r29 = *(u32*)(sp + 0x24);
    return;
}
/* 0x8013024C | 0x414 */
void fn_8013024C(void) {
    extern u8 lbl_8047D030[];
    extern u8 lbl_8047D034[];
    extern u8 lbl_8047D038[];
    extern u8 lbl_8047D0D4[];
    extern void fn_80188AF4();
    extern void fn_80188F78();
    extern void fn_8018C1E8();
    extern void fn_8012AC9C();
    extern void fn_8012AD50();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    r4 = 0x0;
    r3 = (u32)&lbl_80426BD0;
    r0 = r4 & 0x1;
    r31 = (u32)&lbl_80426BD0;
    *(u16*)((u8*)r31 + 0x4) = r4;
    *(u16*)((u8*)r31 + 0x24) = r4;
    *(u32*)((u8*)r31 + 0x0) = r4;
    *(u32*)((u8*)r31 + 0x188) = r4;
    if ((s32)r0 == (s32)0) {
        r29 = 0x1;
        r0 = r4 | 0x1;
        *(u16*)((u8*)r31 + 0x4) = r0;
        if ((s32)r29 >= (s32)0x0) {
            if ((s32)r29 < (s32)0x2) {
                r0 = r0 & 0x1;
                if ((s32)r29 != (s32)0x2) {
                    r0 = *(u32*)((u8*)r31 + 0xC);
                    r30 = *(u32*)lbl_8047D030;
                    r3 = *(u32*)lbl_8047D034;
                    if ((s32)r0 != (s32)0x1) {
                    } else {
                        r4 = r30;
                        r3 = 0x0;
                        fn_80188AF4();
                    }
                    if ((s32)r29 != (s32)0x1) {
                    } else {
                        r4 = r30;
                        r3 = 0x0;
                        fn_80188F78();
                    }
                    r0 = 0x1;
                    *(u32*)((u8*)r31 + 0xC) = r0;
        }
        }
        }
        r3 = (u32)&lbl_80426BD0;
        f1 = *(f32*)lbl_8047D038;
        r4 = (u32)&lbl_80426BD0;
        f2 = *(f32*)lbl_8047D0D4;
        r0 = *(u32*)((u8*)r4 + 0x0);
        r5 = 0x0;
        r0 = r0 << 5;
        r3 = r4 + r0;
        *(f32*)((u8*)r3 + 0x8) = f1;
        r0 = *(u16*)((u8*)r4 + 0x4);
        r0 = r0 & 0x1;
        if ((s32)r29 != (s32)0x1) {
            r0 = *(u32*)((u8*)r4 + 0x0);
            if ((s32)r0 != (s32)r5) {
                *(f32*)((u8*)r4 + 0x8) = f2;
                f2 = f2 + f2;
        }
        }
        r0 = *(u16*)((u8*)r4 + 0x24);
        r3 = r4 + 0x20;
        r5 = 0x1;
        r0 = r0 & 0x1;
        if ((s32)r0 != (s32)r5) {
            r0 = *(u32*)((u8*)r4 + 0x0);
            if ((s32)r0 != (s32)r5) {
                *(f32*)((u8*)r3 + 0x8) = f2;
        }
        }
        r4 = *(u32*)lbl_8047D030;
        r3 = 0x0;
        r0 = *(u32*)lbl_8047D034;
        r5 = 0x1;
        *(u32*)(sp + 0x14) = r0;
        fn_8018C1E8();
    }
    r0 = *(u16*)((u8*)r31 + 0x4);
    r4 = r0 & 0x1;
    if ((s32)r0 != (s32)r5) {
        r0 = 0x0;
        r3 = (u32)&lbl_80426BD0;
        /* stwu r0, lbl_80426BD0@l(r3) */;
        if ((u32)r4 != (u32)0x0) {
            r0 = *(u32*)((u8*)r3 + 0xC);
        } else {
            r0 = 0x2;
        }
        if (((s32)r0 == (s32)0x1) && ((s32)r4 != (s32)0x0)) {
            r3 = (u32)&lbl_80426BD0;
            r4 = *(u32*)lbl_8047D030;
            r31 = (u32)&lbl_80426BD0;
            r3 = *(u32*)lbl_8047D034;
            r0 = *(u32*)((u8*)r31 + 0xC);
            if ((s32)r0 != (s32)0x1) {
            } else {
                r3 = 0x0;
                fn_80188AF4();
            }
            r0 = 0x0;
            *(u32*)((u8*)r31 + 0xC) = r0;
        }
        r3 = (u32)&lbl_80426BD0;
        f1 = *(f32*)lbl_8047D038;
        r4 = (u32)&lbl_80426BD0;
        f2 = *(f32*)lbl_8047D0D4;
        r0 = *(u32*)((u8*)r4 + 0x0);
        r5 = 0x0;
        r0 = r0 << 5;
        r3 = r4 + r0;
        *(f32*)((u8*)r3 + 0x8) = f1;
        r0 = *(u16*)((u8*)r4 + 0x4);
        r0 = r0 & 0x1;
        if ((s32)r0 != (s32)0x1) {
            r0 = *(u32*)((u8*)r4 + 0x0);
            if ((s32)r0 != (s32)r5) {
                *(f32*)((u8*)r4 + 0x8) = f2;
                f2 = f2 + f2;
        }
        }
        r0 = *(u16*)((u8*)r4 + 0x24);
        r3 = r4 + 0x20;
        r5 = 0x1;
        r0 = r0 & 0x1;
        if ((s32)r0 != (s32)r5) {
            r0 = *(u32*)((u8*)r4 + 0x0);
            if ((s32)r0 != (s32)r5) {
                *(f32*)((u8*)r3 + 0x8) = f2;
        }
        }
        r3 = (u32)&lbl_80426BD0;
        r0 = 0x0;
        r3 = (u32)&lbl_80426BD0;
        *(u32*)((u8*)r3 + 0x44) = r0;
        *(u32*)((u8*)r3 + 0x48) = r0;
    }
    r3 = (u32)&lbl_80426BD0;
    f0 = *(f32*)lbl_8047D038;
    r5 = (u32)&lbl_80426BD0;
    r6 = 0x0;
    *(u32*)((u8*)r5 + 0x18C) = r6;
    r7 = r5 + 0x140;
    *(f32*)((u8*)r5 + 0x13C) = f0;
    *(u32*)((u8*)r5 + 0x140) = r6;
    *(u32*)((u8*)r5 + 0x148) = r6;
    *(u32*)((u8*)r5 + 0x150) = r6;
    *(u32*)((u8*)r5 + 0x158) = r6;
    *(u32*)((u8*)r5 + 0x160) = r6;
    *(u32*)((u8*)r5 + 0x168) = r6;
    *(u32*)((u8*)r5 + 0x170) = r6;
    *(u32*)((u8*)r5 + 0x178) = r6;
    *(u32*)((u8*)r5 + 0x180) = r6;
    r0 = *(u32*)((u8*)r5 + 0x140);
    if ((u32)r0 != (u32)0x0) {
        r3 = r5 + 0x8;
        r6 = 0x1;
        r0 = *(u32*)((u8*)r5 + 0x148);
        if ((u32)r0 != (u32)0x0) {
            r0 = *(u32*)((u8*)r3 + 0x148);
            r6 = 0x2;
            r3 = r3 + 0x8;
            if ((u32)r0 != (u32)0x0) {
                r0 = *(u32*)((u8*)r3 + 0x148);
                r6 = 0x3;
                r3 = r3 + 0x8;
                if ((u32)r0 != (u32)0x0) {
                    r0 = *(u32*)((u8*)r3 + 0x148);
                    r6 = 0x4;
                    r3 = r3 + 0x8;
                    if ((u32)r0 != (u32)0x0) {
                        r0 = *(u32*)((u8*)r3 + 0x148);
                        r6 = 0x5;
                        r3 = r3 + 0x8;
                        if ((u32)r0 != (u32)0x0) {
                            r0 = *(u32*)((u8*)r3 + 0x148);
                            r6 = 0x6;
                            r3 = r3 + 0x8;
                            if ((u32)r0 != (u32)0x0) {
                                r0 = *(u32*)((u8*)r3 + 0x148);
                                r6 = 0x7;
                                if ((u32)r0 != (u32)0x0) {
                                    r6 = 0x8;
    }
    }
    }
    }
    }
    }
    }
    }
    if ((s32)r6 < (s32)0x8) {
        r4 = (u32)fn_8012AD50;
        r3 = (u32)&lbl_80426BD0;
        r4 = (u32)fn_8012AD50;
        r6 = r6 << 3;
        r0 = (u32)&lbl_80426BD0;
        *(u32*)(r7 + r6) = r4;
        r3 = r0 + r6;
        r0 = 0x0;
        *(u32*)((u8*)r3 + 0x144) = r0;
    }
    r3 = (u32)&lbl_80426BD0;
    r6 = 0x0;
    r3 = (u32)&lbl_80426BD0;
    *(u32*)((u8*)r3 + 0x184) = r6;
    r0 = *(u32*)((u8*)r5 + 0x140);
    if ((u32)r0 != (u32)0x0) {
        r3 = r5 + 0x8;
        r6 = 0x1;
        r0 = *(u32*)((u8*)r5 + 0x148);
        if ((u32)r0 != (u32)0x0) {
            r0 = *(u32*)((u8*)r3 + 0x148);
            r6 = 0x2;
            r3 = r3 + 0x8;
            if ((u32)r0 != (u32)0x0) {
                r0 = *(u32*)((u8*)r3 + 0x148);
                r6 = 0x3;
                r3 = r3 + 0x8;
                if ((u32)r0 != (u32)0x0) {
                    r0 = *(u32*)((u8*)r3 + 0x148);
                    r6 = 0x4;
                    r3 = r3 + 0x8;
                    if ((u32)r0 != (u32)0x0) {
                        r0 = *(u32*)((u8*)r3 + 0x148);
                        r6 = 0x5;
                        r3 = r3 + 0x8;
                        if ((u32)r0 != (u32)0x0) {
                            r0 = *(u32*)((u8*)r3 + 0x148);
                            r6 = 0x6;
                            r3 = r3 + 0x8;
                            if ((u32)r0 != (u32)0x0) {
                                r0 = *(u32*)((u8*)r3 + 0x148);
                                r6 = 0x7;
                                if ((u32)r0 != (u32)0x0) {
                                    r6 = 0x8;
    }
    }
    }
    }
    }
    }
    }
    }
    if ((s32)r6 < (s32)0x8) {
        r4 = (u32)fn_8012AC9C;
        r3 = (u32)&lbl_80426BD0;
        r4 = (u32)fn_8012AC9C;
        r5 = r6 << 3;
        r0 = (u32)&lbl_80426BD0;
        *(u32*)(r7 + r5) = r4;
        r3 = r0 + r5;
        r0 = 0x0;
        *(u32*)((u8*)r3 + 0x144) = r0;
    }
    r31 = *(u32*)(sp + 0x2C);
    r30 = *(u32*)(sp + 0x28);
    r29 = *(u32*)(sp + 0x24);
    return;
}
/* 0x80130660 | 0x110 */
void fn_80130660(void) {
    extern void fn_800FA280();
    extern void fn_80123EF0();
    extern void fn_801240C4();
    extern void fn_80124410();
    extern void fn_8012546C();
    extern void fn_801254B4();
    extern void fn_80129E20();
    extern void fn_801353C0();
    extern void fn_80135938();
    extern void fn_8025FF9C();
    u8 sp[0x150];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r4 = 0x5;
    r30 = r3;
    r3 = 0x0;
    fn_80135938();
    r31 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0x4;
    fn_80135938();
    r6 = r3 & 0xFF;
    r7 = r31;
    r3 = (u32)sp + 0x8;
    r4 = 0x9;
    r5 = 0x3;
    fn_801353C0();
    r3 = (u32)sp + 0xc;
    r6 = (u32)sp + 0x8;
    r4 = 0xfb;
    r5 = 0xa;
    fn_801240C4();
    r3 = (u32)sp + 0xc;
    r4 = 0x0;
    r5 = 0x99;
    r6 = 0x0;
    r7 = 0x46;
    fn_801254B4();
    r3 = 0x12af;
    fn_800FA280();
    r9 = r3;
    r3 = (u32)sp + 0xc;
    r4 = 0xff;
    r5 = 0xa;
    r6 = 0x4;
    r7 = 0x1;
    r8 = 0x7991;
    fn_80123EF0();
    r3 = (u32)sp + 0xc;
    r4 = -0x1;
    r5 = -0x1;
    r6 = 0x0;
    r7 = 0x7991;
    fn_80124410();
    r0 = r3;
    r3 = (u32)sp + 0xc;
    r7 = r0;
    r4 = 0x0;
    r5 = 0x6f;
    r6 = 0x0;
    fn_801254B4();
    r3 = (u32)sp + 0xc;
    fn_8012546C();
    r4 = (u32)sp + 0xc;
    r3 = 0x0;
    fn_8025FF9C();
    r3 = r30;
    r4 = (u32)sp + 0xc;
    r5 = 0x1;
    fn_80129E20();
    return;
}
/* 0x80130770 | 0x120 */
void fn_80130770(void) {
    extern void fn_800FA280();
    extern void fn_80123110();
    extern void fn_80123EF0();
    extern void fn_801240C4();
    extern void fn_80124410();
    extern void fn_8012546C();
    extern void fn_801254B4();
    extern void fn_80129E20();
    extern void fn_801353C0();
    extern void fn_80135938();
    extern void fn_8025FF9C();
    u8 sp[0x150];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r4 = 0x5;
    r30 = r3;
    r3 = 0x0;
    fn_80135938();
    r31 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0x4;
    fn_80135938();
    r6 = r3 & 0xFF;
    r7 = r31;
    r3 = (u32)sp + 0x8;
    r4 = 0x9;
    r5 = 0x3;
    fn_801353C0();
    r3 = (u32)sp + 0xc;
    r6 = (u32)sp + 0x8;
    r4 = 0x19;
    r5 = 0xa;
    fn_801240C4();
    r3 = (u32)sp + 0xc;
    r4 = 0x0;
    r5 = 0x99;
    r6 = 0x0;
    r7 = 0x46;
    fn_801254B4();
    r3 = 0x12ae;
    fn_800FA280();
    r9 = r3;
    r3 = (u32)sp + 0xc;
    r4 = 0xff;
    r5 = 0xa;
    r6 = 0x4;
    r7 = 0x0;
    r8 = 0x7991;
    fn_80123EF0();
    r3 = (u32)sp + 0xc;
    r4 = -0x1;
    r5 = -0x1;
    r6 = 0x0;
    r7 = 0x7991;
    fn_80124410();
    r0 = r3;
    r3 = (u32)sp + 0xc;
    r7 = r0;
    r4 = 0x0;
    r5 = 0x6f;
    r6 = 0x0;
    fn_801254B4();
    r3 = (u32)sp + 0xc;
    r4 = 0xca;
    r5 = 0x1;
    fn_80123110();
    r3 = (u32)sp + 0xc;
    fn_8012546C();
    r4 = (u32)sp + 0xc;
    r3 = 0x0;
    fn_8025FF9C();
    r3 = r30;
    r4 = (u32)sp + 0xc;
    r5 = 0x1;
    fn_80129E20();
    return;
}
/* 0x80130890 | 0x110 */
void fn_80130890(void) {
    extern void fn_800FA280();
    extern void fn_80123EF0();
    extern void fn_801240C4();
    extern void fn_80124410();
    extern void fn_8012546C();
    extern void fn_801254B4();
    extern void fn_80129E20();
    extern void fn_801353C0();
    extern void fn_80135938();
    extern void fn_8025FF9C();
    u8 sp[0x150];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r4 = 0x5;
    r30 = r3;
    r3 = 0x0;
    fn_80135938();
    r31 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0x4;
    fn_80135938();
    r6 = r3 & 0xFF;
    r7 = r31;
    r3 = (u32)sp + 0x8;
    r4 = 0x8;
    r5 = 0x3;
    fn_801353C0();
    r3 = (u32)sp + 0xc;
    r6 = (u32)sp + 0x8;
    r4 = 0xfa;
    r5 = 0x46;
    fn_801240C4();
    r3 = (u32)sp + 0xc;
    r4 = 0x0;
    r5 = 0x99;
    r6 = 0x0;
    r7 = 0x46;
    fn_801254B4();
    r3 = 0x12ad;
    fn_800FA280();
    r9 = r3;
    r3 = (u32)sp + 0xc;
    r4 = 0xff;
    r5 = 0x46;
    r6 = 0x4;
    r7 = 0x0;
    r8 = 0x2740;
    fn_80123EF0();
    r3 = (u32)sp + 0xc;
    r4 = -0x1;
    r5 = -0x1;
    r6 = 0x0;
    r7 = 0x2740;
    fn_80124410();
    r0 = r3;
    r3 = (u32)sp + 0xc;
    r7 = r0;
    r4 = 0x0;
    r5 = 0x6f;
    r6 = 0x0;
    fn_801254B4();
    r3 = (u32)sp + 0xc;
    fn_8012546C();
    r4 = (u32)sp + 0xc;
    r3 = 0x0;
    fn_8025FF9C();
    r3 = r30;
    r4 = (u32)sp + 0xc;
    r5 = 0x1;
    fn_80129E20();
    return;
}
/* 0x801309A0 | 0xE8 */
void fn_801309A0(void) {
    extern void fn_800FA280();
    extern void fn_80123EF0();
    extern void fn_801240C4();
    extern void fn_80124410();
    extern void fn_8012546C();
    extern void fn_801254B4();
    extern void fn_80129E20();
    extern void fn_80135938();
    extern void fn_8025FF9C();
    u8 sp[0x150];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r9 = 0;
    u32 r31 = 0;
    r4 = 0x1;
    r31 = r3;
    r3 = 0x0;
    fn_80135938();
    r6 = r3;
    r3 = (u32)sp + 0x8;
    r4 = 0x161;
    r5 = 0xd;
    fn_801240C4();
    r3 = (u32)sp + 0x8;
    r4 = 0x0;
    r5 = 0x99;
    r6 = 0x0;
    r7 = 0x46;
    fn_801254B4();
    r3 = 0x12ac;
    fn_800FA280();
    r4 = (0x1 << 16);
    r9 = r3;
    r3 = (u32)sp + 0x8;
    r4 = 0xfe;
    r5 = 0xd;
    r6 = 0x4;
    r7 = 0x0;
    fn_80123EF0();
    r4 = (0x1 << 16);
    r3 = (u32)sp + 0x8;
    r4 = -0x1;
    r5 = -0x1;
    r6 = 0x0;
    fn_80124410();
    r0 = r3;
    r3 = (u32)sp + 0x8;
    r7 = r0;
    r4 = 0x0;
    r5 = 0x6f;
    r6 = 0x0;
    fn_801254B4();
    r3 = (u32)sp + 0x8;
    fn_8012546C();
    r4 = (u32)sp + 0x8;
    r3 = 0x0;
    fn_8025FF9C();
    r3 = r31;
    r4 = (u32)sp + 0x8;
    r5 = 0x1;
    fn_80129E20();
    return;
}
/* 0x80130A88 | 0x128 */
void fn_80130A88(void) {
    extern void fn_80123D58();
    extern void fn_801240C4();
    extern void fn_80124410();
    extern void fn_8012546C();
    extern void fn_801254B4();
    extern void fn_80129F20();
    extern void fn_8012A5B0();
    extern void fn_80135938();
    u8 sp[0x150];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r4 = 0x2;
    r5 = 0x0;
    r30 = r3;
    fn_8012A5B0();
    r0 = r3;
    r3 = 0x0;
    r31 = r0;
    r4 = 0x1;
    fn_80135938();
    r6 = r3;
    r3 = (u32)sp + 0x8;
    r4 = 0xc4;
    r5 = 0x19;
    fn_801240C4();
    r3 = (u32)sp + 0x8;
    r4 = 0x0;
    r5 = 0x5d;
    fn_80123D58();
    r3 = (u32)sp + 0x8;
    r4 = 0x1;
    r5 = 0xd8;
    fn_80123D58();
    r3 = (u32)sp + 0x8;
    r4 = 0x2;
    r5 = 0x73;
    fn_80123D58();
    r3 = (u32)sp + 0x8;
    r4 = 0x3;
    r5 = 0x10e;
    fn_80123D58();
    r7 = r31;
    r3 = (u32)sp + 0x8;
    r4 = 0x0;
    r5 = -0x1;
    r6 = 0x0;
    fn_80124410();
    r0 = r3;
    r3 = (u32)sp + 0x8;
    r7 = r0;
    r4 = 0x0;
    r5 = 0x6f;
    r6 = 0x0;
    fn_801254B4();
    r3 = (u32)sp + 0x8;
    r4 = 0x0;
    r5 = 0x79;
    r6 = 0x0;
    r7 = 0x3ff4;
    fn_801254B4();
    r3 = (u32)sp + 0x8;
    r4 = 0x0;
    r5 = 0x99;
    r6 = 0x0;
    r7 = 0xdc;
    fn_801254B4();
    r3 = (u32)sp + 0x8;
    fn_8012546C();
    r3 = r30;
    r4 = (u32)sp + 0x8;
    r5 = 0xfe;
    r6 = 0x4;
    r7 = 0x0;
    fn_80129F20();
    return;
}
/* 0x80130BB0 | 0x128 */
void fn_80130BB0(void) {
    extern void fn_80123D58();
    extern void fn_801240C4();
    extern void fn_80124410();
    extern void fn_8012546C();
    extern void fn_801254B4();
    extern void fn_80129F20();
    extern void fn_8012A5B0();
    extern void fn_80135938();
    u8 sp[0x150];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r4 = 0x2;
    r5 = 0x0;
    r30 = r3;
    fn_8012A5B0();
    r0 = r3;
    r3 = 0x0;
    r31 = r0;
    r4 = 0x1;
    fn_80135938();
    r6 = r3;
    r3 = (u32)sp + 0x8;
    r4 = 0xc5;
    r5 = 0x1a;
    fn_801240C4();
    r3 = (u32)sp + 0x8;
    r4 = 0x0;
    r5 = 0x2c;
    fn_80123D58();
    r3 = (u32)sp + 0x8;
    r4 = 0x1;
    r5 = 0x122;
    fn_80123D58();
    r3 = (u32)sp + 0x8;
    r4 = 0x2;
    r5 = 0x10d;
    fn_80123D58();
    r3 = (u32)sp + 0x8;
    r4 = 0x3;
    r5 = 0x121;
    fn_80123D58();
    r7 = r31;
    r3 = (u32)sp + 0x8;
    r4 = 0x0;
    r5 = -0x1;
    r6 = 0x0;
    fn_80124410();
    r0 = r3;
    r3 = (u32)sp + 0x8;
    r7 = r0;
    r4 = 0x0;
    r5 = 0x6f;
    r6 = 0x0;
    fn_801254B4();
    r3 = (u32)sp + 0x8;
    r4 = 0x0;
    r5 = 0x79;
    r6 = 0x0;
    r7 = 0x4a6f;
    fn_801254B4();
    r3 = (u32)sp + 0x8;
    r4 = 0x0;
    r5 = 0x99;
    r6 = 0x0;
    r7 = 0xdc;
    fn_801254B4();
    r3 = (u32)sp + 0x8;
    fn_8012546C();
    r3 = r30;
    r4 = (u32)sp + 0x8;
    r5 = 0xfe;
    r6 = 0x4;
    r7 = 0x0;
    fn_80129F20();
    return;
}