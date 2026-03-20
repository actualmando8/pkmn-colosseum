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


/* Field subsystems -- forward declarations (defined below) */
u32  fn_80115094(void);
void fn_80115A38(u32 entry);
void fn_80117C84(void);

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void floorUpdateFieldCamera(void) {
    /* TODO: match -- 436 bytes at 0x80117514 */
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSfield_TransitionStateMachine(void) {
    /* TODO: match -- 5456 bytes at 0x8012640C */
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSfield_RenderPass(void) {
    /* TODO: match -- 3928 bytes at 0x801254B4 */
}
#pragma pop

/* ==================================================================
 * fn_80124A60 -- Field object batch update
 *
 * 2008 bytes. Updates all active field objects in a single pass.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSfield_ObjectBatchUpdate(void) {
    /* TODO: match -- 2008 bytes at 0x80124A60 */
}
#pragma pop

/* ==================================================================
 * fn_8012CA84 -- Field event/trigger processing
 *
 * 2104 bytes. Processes field event triggers (door warps, NPC
 * interaction zones, item pickups, etc.).
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSfield_ProcessTriggers(void) {
    /* TODO: match -- 2104 bytes at 0x8012CA84 */
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off

/* fn_8011E4A4 */ u16 GSfield_GetObjAttrU16(void* obj, u16 slot) {
    /* TODO: match -- 52 bytes at 0x8011E4A4 */
}

/* fn_8011E4D8 */ u16 GSfield_GetObjType(void* obj) {
    /* TODO: match -- 24 bytes at 0x8011E4D8 */
}

/* fn_8011E4F0 */ u8 GSfield_GetObjSubtype(void* obj) {
    /* TODO: match -- 24 bytes at 0x8011E4F0 */
}

/* fn_8011E508 */ u8 GSfield_GetObjFlags(void* obj) {
    /* TODO: match -- 24 bytes at 0x8011E508 */
}

/* fn_8011E520 */ u8 GSfield_GetObjState(void* obj) {
    /* TODO: match -- 24 bytes at 0x8011E520 */
}

/* fn_8011E538 */ u16 GSfield_GetObjGroupId(void* obj) {
    /* TODO: match -- 24 bytes at 0x8011E538 */
}

/* fn_8011E550 */ u16 GSfield_GetObjRegionId(void* obj) {
    /* TODO: match -- 44 bytes at 0x8011E550 */
}

#pragma pop

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
void fn_8012546C(void);
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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80114D6C(void) {
    /* TODO: match -- 160 bytes at 0x80114D6C */
}
#pragma pop

/* 0x6C | fn_80114E0C | nullcheck_call_flag */
u32 fn_80114E0C(void* obj) {
    if (fn_800FF548() == 0) { return 0; }
    fn_800FC39C(obj);
    return 1;
}

/* 0x80114E78 | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80114E78(void) {
    /* TODO: match -- 160 bytes at 0x80114E78 */
}
#pragma pop

/* 0x6C | fn_80114F18 | nullcheck_call_flag */
u32 fn_80114F18(void* obj) {
    if (fn_800FF548() == 0) { return 0; }
    fn_800FC244(obj);
    return 1;
}

/* 0x80114F84 | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80114F84(void) {
    /* TODO: match -- 160 bytes at 0x80114F84 */
}
#pragma pop

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
u32 fn_80115094(void) {
    fn_801ED674();
    return 1;
}

/* 0x801150B8 | 36 bytes | call_return_const */
u32 fn_801150B8(void) {
    fn_801193BC();
    return 1;
}

/* 0x801150DC | 36 bytes | call_return_const */
u32 fn_801150DC(void) {
    fn_800D2738();
    return 1;
}

/* 0x80115100 | 36 bytes | call_return_const */
u32 fn_80115100(void) {
    fn_800DCD98();
    return 1;
}

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
u32 fn_80115208(void) {
    fn_8010CC04();
    return 1;
}

/* 0x8011522C | 36 bytes | call_return_const */
u32 fn_8011522C(void) {
    fn_800EF5A4();
    return 1;
}

/* 0x80115250 | 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80115250(void) {
    /* TODO: match -- 12 bytes at 0x80115250 */
}
#pragma pop

/* 0x8011525C | 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011525C(void) {
    /* TODO: match -- 12 bytes at 0x8011525C */
}
#pragma pop

/* 0x80115268 | 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80115268(void) {
    /* TODO: match -- 12 bytes at 0x80115268 */
}
#pragma pop

/* 0x80115274 | 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80115274(void) {
    /* TODO: match -- 12 bytes at 0x80115274 */
}
#pragma pop

/* 0x80115280 | 0x10C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80115280(void) {
    /* TODO: match -- 268 bytes at 0x80115280 */
}
#pragma pop

/* 0x8011538C | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011538C(void) {
    /* TODO: match -- 160 bytes at 0x8011538C */
}
#pragma pop

/* 0x8011542C | 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011542C(void) {
    /* TODO: match -- 136 bytes at 0x8011542C */
}
#pragma pop

/* 0x801154B4 | 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801154B4(void) {
    /* TODO: match -- 136 bytes at 0x801154B4 */
}
#pragma pop

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
    /* TODO: decompile -- signature fixed to match declaration */
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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80115CB4(void) {
    /* TODO: match -- 176 bytes at 0x80115CB4 */
}
#pragma pop

/* 0x80115D64 | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80115D64(void) {
    /* TODO: match -- 160 bytes at 0x80115D64 */
}
#pragma pop

/* 0x68 | fn_80115E04 | global_cond_call */
u32 fn_80115E04(void) {
    /* uses lbl_80272708 */
    if (1 /* field check */) { return -1; }
    fn_800DD970("");
    return 0;
}

/* 0x80115E6C | 0x2F8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80115E6C(void) {
    /* TODO: match -- 760 bytes at 0x80115E6C */
}
#pragma pop

/* 0x80116164 | 0x30C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80116164(void) {
    /* TODO: match -- 780 bytes at 0x80116164 */
}
#pragma pop

/* 0x80116470 | 0x4E8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80116470(void) {
    /* TODO: match -- 1256 bytes at 0x80116470 */
}
#pragma pop

/* 0x80116958 | 0x3D8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80116958(void) {
    /* TODO: match -- 984 bytes at 0x80116958 */
}
#pragma pop

/* 0x80116D30 | 0x13C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80116D30(void) {
    /* TODO: match -- 316 bytes at 0x80116D30 */
}
#pragma pop

/* 0x80116E6C | 0x18 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80116E6C(void) {
    /* TODO: match -- 24 bytes at 0x80116E6C */
}
#pragma pop

/* 0x80116E84 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80116E84(void) {
    /* TODO: match -- 44 bytes at 0x80116E84 */
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80116EF8(void) {
    /* TODO: match -- 28 bytes at 0x80116EF8 */
}
#pragma pop

/* 0x80116F14 | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80116F14(void) {
    /* TODO: match -- 28 bytes at 0x80116F14 */
}
#pragma pop

/* 0x80116F30 | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80116F30(void) {
    /* TODO: match -- 28 bytes at 0x80116F30 */
}
#pragma pop

/* 0x80116F4C | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80116F4C(void) {
    /* TODO: match -- 28 bytes at 0x80116F4C */
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80117038(void) {
    /* TODO: match -- 28 bytes at 0x80117038 */
}
#pragma pop

/* 0x80117054 | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80117054(void) {
    /* TODO: match -- 28 bytes at 0x80117054 */
}
#pragma pop

/* 0x80117070 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80117070(void) {
    /* TODO: match -- 52 bytes at 0x80117070 */
}
#pragma pop

/* 0x78 | fn_801170A4 | generic */
u32 fn_801170A4(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_800FF56C();
    fn_80115C48();
    fn_80115A80();
    fn_80115684();
    return 0;
}

/* 0x8011711C | 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011711C(void) {
    /* TODO: match -- 56 bytes at 0x8011711C */
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801171C8(void) {
    /* TODO: match -- 360 bytes at 0x801171C8 */
}
#pragma pop

/* 0x80117330 | 0x194 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80117330(void) {
    /* TODO: match -- 404 bytes at 0x80117330 */
}
#pragma pop

/* 0x801174C4 | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801174C4(void) {
    /* TODO: match -- 40 bytes at 0x801174C4 */
}
#pragma pop

/* 0x801174F4 | 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801174F4(void) {
    /* TODO: match -- 12 bytes at 0x801174F4 */
}
#pragma pop

/* 0x80117500 | 0x14 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80117500(void) {
    /* TODO: match -- 20 bytes at 0x80117500 */
}
#pragma pop

/* 0x801176C8 | 0x254 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801176C8(void) {
    /* TODO: match -- 596 bytes at 0x801176C8 */
}
#pragma pop

/* 0x8011791C | 0x1B8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011791C(void) {
    /* TODO: match -- 440 bytes at 0x8011791C */
}
#pragma pop

/* 0x80117AD4 | 16 bytes | global_getter */
u32 fn_80117AD4(void) {
    return *(u32*)((u8*)lbl_804083D0 + 0x10);
}

/* 0x80117AE4 | 0x1A0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80117AE4(void) {
    /* TODO: match -- 416 bytes at 0x80117AE4 */
}
#pragma pop

/* 0x80117C84 | 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80117C84(void) {
    /* TODO: match -- 144 bytes at 0x80117C84 */
}
#pragma pop

/* 0x80117D14 | 0x144 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80117D14(void) {
    /* TODO: match -- 324 bytes at 0x80117D14 */
}
#pragma pop

/* 0x80117E58 | 0x1C8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80117E58(void) {
    /* TODO: match -- 456 bytes at 0x80117E58 */
}
#pragma pop

/* 0x48 | fn_80118020 | single_call_straight */
void fn_80118020(void) {
    fn_800FF4D4();
}

/* 0x80118070 | 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80118070(void) {
    /* TODO: match -- 144 bytes at 0x80118070 */
}
#pragma pop

/* 0x80118100 | 0x4 | void_stub */
void fn_80118100(void) {
}

/* 0x80118104 | 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80118104(void) {
    /* TODO: match -- 172 bytes at 0x80118104 */
}
#pragma pop

/* 0x801181B0 | 0x23C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801181B0(void) {
    /* TODO: match -- 572 bytes at 0x801181B0 */
}
#pragma pop

/* 0x801183EC | 0x488 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801183EC(void) {
    /* TODO: match -- 1160 bytes at 0x801183EC */
}
#pragma pop

/* 0x80118874 | 0x1F4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80118874(void) {
    /* TODO: match -- 500 bytes at 0x80118874 */
}
#pragma pop

/* 0x80118A68 | 0x1B8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80118A68(void) {
    /* TODO: match -- 440 bytes at 0x80118A68 */
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80118DA8(void) {
    /* TODO: match -- 56 bytes at 0x80118DA8 */
}
#pragma pop

/* 0x80118DE0 | 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80118DE0(void) {
    /* TODO: match -- 172 bytes at 0x80118DE0 */
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80118F7C(void) {
    /* TODO: match -- 52 bytes at 0x80118F7C */
}
#pragma pop

/* 0x80118FB0 | 0x12C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80118FB0(void) {
    /* TODO: match -- 300 bytes at 0x80118FB0 */
}
#pragma pop

/* 0x801190DC | 0x2E0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801190DC(void) {
    /* TODO: match -- 736 bytes at 0x801190DC */
}
#pragma pop

/* 0x801193BC | 0x1F0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801193BC(void) {
    /* TODO: match -- 496 bytes at 0x801193BC */
}
#pragma pop

/* 0x801195AC | 0x278 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801195AC(void) {
    /* TODO: match -- 632 bytes at 0x801195AC */
}
#pragma pop

/* 0x80119824 | 0x10C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80119824(void) {
    /* TODO: match -- 268 bytes at 0x80119824 */
}
#pragma pop

/* 0x80119930 | 0x2A0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80119930(void) {
    /* TODO: match -- 672 bytes at 0x80119930 */
}
#pragma pop

/* 0x80119BD0 | 0x1C0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80119BD0(void) {
    /* TODO: match -- 448 bytes at 0x80119BD0 */
}
#pragma pop

/* 0x40 | fn_80119D90 | index_lookup */
u8 fn_80119D90(u16 idx) {
    void* entry;
    if (idx >= lbl_80478B48) { return 0; }
    entry = (u8*)lbl_8035BBA8 + idx * 0x14;
    if (entry == NULL) { return 0; }
    return *(u8*)((u8*)entry + 0x4);
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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011A0A8(void) {
    /* TODO: match -- 472 bytes at 0x8011A0A8 */
}
#pragma pop

/* 0x8011A280 | 0x164 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011A280(void) {
    /* TODO: match -- 356 bytes at 0x8011A280 */
}
#pragma pop

/* 0x8011A3E4 | 0x18C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011A3E4(void* obj) {
    /* TODO: match -- 396 bytes at 0x8011A3E4 */
}
#pragma pop

/* 0x8011A570 | 0x164 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011A570(void) {
    /* TODO: match -- 356 bytes at 0x8011A570 */
}
#pragma pop

/* 0x8011A6D4 | 0x18C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011A6D4(void* obj) {
    /* TODO: match -- 396 bytes at 0x8011A6D4 */
}
#pragma pop

/* 0x8011A860 | 0x18C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011A860(void* obj) {
    /* TODO: match -- 396 bytes at 0x8011A860 */
}
#pragma pop

/* 0x8011A9EC | 0x164 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011A9EC(void) {
    /* TODO: match -- 356 bytes at 0x8011A9EC */
}
#pragma pop

/* 0x8011AB50 | 0x164 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011AB50(void) {
    /* TODO: match -- 356 bytes at 0x8011AB50 */
}
#pragma pop

/* 0x8011ACB4 | 0x18C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011ACB4(void* obj) {
    /* TODO: match -- 396 bytes at 0x8011ACB4 */
}
#pragma pop

/* 0x8011AE40 | 0x18C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011AE40(void* obj) {
    /* TODO: match -- 396 bytes at 0x8011AE40 */
}
#pragma pop

/* 0x8011AFCC | 0x164 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011AFCC(void) {
    /* TODO: match -- 356 bytes at 0x8011AFCC */
}
#pragma pop

/* 0x8011B130 | 0x190 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011B130(void* obj) {
    /* TODO: match -- 400 bytes at 0x8011B130 */
}
#pragma pop

/* 0x8011B2C0 | 0x184 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011B2C0(void) {
    /* TODO: match -- 388 bytes at 0x8011B2C0 */
}
#pragma pop

/* 0x8011B444 | 0x238 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011B444(void* obj) {
    /* TODO: match -- 568 bytes at 0x8011B444 */
}
#pragma pop

/* 0x8011B67C | 0x10C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011B67C(void* obj) {
    /* TODO: match -- 268 bytes at 0x8011B67C */
}
#pragma pop

/* 0x8011B788 | 0x1C8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011B788(void) {
    /* TODO: match -- 456 bytes at 0x8011B788 */
}
#pragma pop

/* 0x8011B950 | 0xBC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011B950(void) {
    /* TODO: match -- 188 bytes at 0x8011B950 */
}
#pragma pop

/* 0x8011BA0C | 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011BA0C(void) {
    /* TODO: match -- 180 bytes at 0x8011BA0C */
}
#pragma pop

/* 0x8011BAC0 | 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011BAC0(void) {
    /* TODO: match -- 172 bytes at 0x8011BAC0 */
}
#pragma pop

/* 0x6C | fn_8011BB6C | single_call_straight */
void fn_8011BB6C(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_8011BEB4();
}

/* 0x8011BBD8 | 0x2DC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011BBD8(void) {
    /* TODO: match -- 732 bytes at 0x8011BBD8 */
}
#pragma pop

/* 0x8011BEB4 | 0x31C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011BEB4(void) {
    /* TODO: match -- 796 bytes at 0x8011BEB4 */
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011C430(void) {
    /* TODO: match -- 32 bytes at 0x8011C430 */
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011C588(void) {
    /* TODO: match -- 48 bytes at 0x8011C588 */
}
#pragma pop

/* 0x8011CA34 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011CA34(void) {
    /* TODO: match -- 44 bytes at 0x8011CA34 */
}
#pragma pop

/* 0x8011CA60 | 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011CA60(void) {
    /* TODO: match -- 60 bytes at 0x8011CA60 */
}
#pragma pop

/* 0x8011CAB8 | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011CAB8(void) {
    /* TODO: match -- 40 bytes at 0x8011CAB8 */
}
#pragma pop

/* 0x8011CAE0 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011CAE0(void) {
    /* TODO: match -- 48 bytes at 0x8011CAE0 */
}
#pragma pop

/* 0x8011CB10 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011CB10(void) {
    /* TODO: match -- 44 bytes at 0x8011CB10 */
}
#pragma pop

/* 0x8011CB6C | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011CB6C(void) {
    /* TODO: match -- 44 bytes at 0x8011CB6C */
}
#pragma pop

/* 0x8011CBC8 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011CBC8(void) {
    /* TODO: match -- 44 bytes at 0x8011CBC8 */
}
#pragma pop

/* 0x8011CBF4 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011CBF4(void) {
    /* TODO: match -- 48 bytes at 0x8011CBF4 */
}
#pragma pop

/* 0x8011CC24 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011CC24(void) {
    /* TODO: match -- 48 bytes at 0x8011CC24 */
}
#pragma pop

/* 0x8011CE18 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011CE18(void) {
    /* TODO: match -- 44 bytes at 0x8011CE18 */
}
#pragma pop

/* 0x8011CE44 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011CE44(void) {
    /* TODO: match -- 48 bytes at 0x8011CE44 */
}
#pragma pop

/* 0x8011CE74 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011CE74(void) {
    /* TODO: match -- 44 bytes at 0x8011CE74 */
}
#pragma pop

/* 0x8011CED0 | 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011CED0(void) {
    /* TODO: match -- 32 bytes at 0x8011CED0 */
}
#pragma pop

/* 0x8011CEF0 | 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011CEF0(void) {
    /* TODO: match -- 36 bytes at 0x8011CEF0 */
}
#pragma pop

/* 0x8011CF44 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011CF44(void) {
    /* TODO: match -- 44 bytes at 0x8011CF44 */
}
#pragma pop

/* 0x8011CF70 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011CF70(void) {
    /* TODO: match -- 44 bytes at 0x8011CF70 */
}
#pragma pop

/* 0x40 | fn_8011CFEC | generic */
void fn_8011CFEC(u32 arg1, u32 arg2) {

}

/* 0x40 | fn_8011D02C | generic */
void fn_8011D02C(u32 arg1, u32 arg2) {

}

/* 0x40 | fn_8011D06C | generic */
void fn_8011D06C(u32 arg1, u32 arg2) {

}

/* 0x8011D0AC | 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011D0AC(void) {
    /* TODO: match -- 32 bytes at 0x8011D0AC */
}
#pragma pop

/* 0x40 | fn_8011D0CC | generic */
void fn_8011D0CC(u32 arg1, u32 arg2) {

}

/* 0x40 | fn_8011D10C | generic */
void fn_8011D10C(u32 arg1, u32 arg2) {

}

/* 0x40 | fn_8011D14C | generic */
void fn_8011D14C(u32 arg1, u32 arg2) {

}

/* 0x40 | fn_8011D18C | generic */
void fn_8011D18C(u32 arg1, u32 arg2) {

}

/* 0x40 | fn_8011D1CC | generic */
void fn_8011D1CC(u32 arg1, u32 arg2) {

}

/* 0x8011D20C | 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011D20C(void) {
    /* TODO: match -- 32 bytes at 0x8011D20C */
}
#pragma pop

/* 0x8011D22C | 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011D22C(void) {
    /* TODO: match -- 32 bytes at 0x8011D22C */
}
#pragma pop

/* 0x8011D24C | 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011D24C(void) {
    /* TODO: match -- 36 bytes at 0x8011D24C */
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011D480(void) {
    /* TODO: match -- 20 bytes at 0x8011D480 */
}
#pragma pop

/* 0x68 | fn_8011D504 | generic */
void fn_8011D504(u32 arg1) {
    /* refs: lbl_80478F90, lbl_80478F94 */
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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011D8D8(void) {
    /* TODO: match -- 28 bytes at 0x8011D8D8 */
}
#pragma pop

/* 0x8011D904 | 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011D904(void) {
    /* TODO: match -- 32 bytes at 0x8011D904 */
}
#pragma pop

/* 0x8011D924 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011D924(void) {
    /* TODO: match -- 52 bytes at 0x8011D924 */
}
#pragma pop

/* 0x8011D958 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011D958(void) {
    /* TODO: match -- 52 bytes at 0x8011D958 */
}
#pragma pop

/* 0x8011D98C | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011D98C(void) {
    /* TODO: match -- 52 bytes at 0x8011D98C */
}
#pragma pop

/* 0x8011D9C0 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011D9C0(void) {
    /* TODO: match -- 52 bytes at 0x8011D9C0 */
}
#pragma pop

/* 0x8011D9F4 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011D9F4(void) {
    /* TODO: match -- 52 bytes at 0x8011D9F4 */
}
#pragma pop

/* 0x8011DA28 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011DA28(void) {
    /* TODO: match -- 52 bytes at 0x8011DA28 */
}
#pragma pop

/* 0x8011DA5C | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011DA5C(void) {
    /* TODO: match -- 52 bytes at 0x8011DA5C */
}
#pragma pop

/* 0x8011DA90 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011DA90(void) {
    /* TODO: match -- 52 bytes at 0x8011DA90 */
}
#pragma pop

/* 0x8011DAC4 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011DAC4(void) {
    /* TODO: match -- 52 bytes at 0x8011DAC4 */
}
#pragma pop

/* 0x8011DAF8 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011DAF8(void) {
    /* TODO: match -- 52 bytes at 0x8011DAF8 */
}
#pragma pop

/* 0x8011DB2C | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011DB2C(void) {
    /* TODO: match -- 52 bytes at 0x8011DB2C */
}
#pragma pop

/* 0x8011DB60 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011DB60(void) {
    /* TODO: match -- 52 bytes at 0x8011DB60 */
}
#pragma pop

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

/* 0x48 | fn_8011DC6C | generic */
void fn_8011DC6C(u32 arg1, u32 arg2) {

}

/* 0x8011DCC4 | 0xBC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011DCC4(void) {
    /* TODO: match -- 188 bytes at 0x8011DCC4 */
}
#pragma pop

/* 0x7C | fn_8011DD80 | generic */
void fn_8011DD80(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6) {
    fn_8011F260();
    fn_80123E70();
}

/* 0x8011DDFC | 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011DDFC(void) {
    /* TODO: match -- 60 bytes at 0x8011DDFC */
}
#pragma pop

/* 0x8011DE48 | 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011DE48(void) {
    /* TODO: match -- 32 bytes at 0x8011DE48 */
}
#pragma pop

/* 0x8011DE68 | 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011DE68(void) {
    /* TODO: match -- 32 bytes at 0x8011DE68 */
}
#pragma pop

/* 0x8011DEA8 | 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011DEA8(void) {
    /* TODO: match -- 60 bytes at 0x8011DEA8 */
}
#pragma pop

/* 0x70 | fn_8011DEE4 | generic */
void fn_8011DEE4(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_800F9D24();
    fn_800F9D24();
}

/* 0x8011DF54 | 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011DF54(void) {
    /* TODO: match -- 60 bytes at 0x8011DF54 */
}
#pragma pop

/* 0x8011E048 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011E048(void) {
    /* TODO: match -- 48 bytes at 0x8011E048 */
}
#pragma pop

/* 0x8011E078 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011E078(void) {
    /* TODO: match -- 52 bytes at 0x8011E078 */
}
#pragma pop

/* 0x8011E0F4 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011E0F4(void) {
    /* TODO: match -- 52 bytes at 0x8011E0F4 */
}
#pragma pop

/* 0x8011E128 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011E128(void) {
    /* TODO: match -- 52 bytes at 0x8011E128 */
}
#pragma pop

/* 0x48 | fn_8011E1D4 | generic */
u32 fn_8011E1D4(u32 arg1, u32 arg2) {
    return 0;
}

/* 0x48 | fn_8011E21C | generic */
u32 fn_8011E21C(u32 arg1, u32 arg2) {
    return 0;
}

/* 0x48 | fn_8011E264 | generic */
u32 fn_8011E264(u32 arg1, u32 arg2) {
    return 0;
}

/* 0x8011E2AC | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011E2AC(void) {
    /* TODO: match -- 48 bytes at 0x8011E2AC */
}
#pragma pop

/* 0x48 | fn_8011E2DC | generic */
u32 fn_8011E2DC(u32 arg1, u32 arg2) {
    return 0;
}

/* 0x48 | fn_8011E324 | generic */
u32 fn_8011E324(u32 arg1, u32 arg2) {
    return 0;
}

/* 0x48 | fn_8011E36C | generic */
u32 fn_8011E36C(u32 arg1, u32 arg2) {
    return 0;
}

/* 0x48 | fn_8011E3B4 | generic */
u32 fn_8011E3B4(u32 arg1, u32 arg2) {
    return 0;
}

/* 0x48 | fn_8011E3FC | generic */
u32 fn_8011E3FC(u32 arg1, u32 arg2) {
    return 0;
}

/* 0x8011E444 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011E444(void) {
    /* TODO: match -- 48 bytes at 0x8011E444 */
}
#pragma pop

/* 0x8011E474 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011E474(void) {
    /* TODO: match -- 48 bytes at 0x8011E474 */
}
#pragma pop

/* 0x8011E57C | 44 bytes | compound_getter */
u16 fn_8011E57C(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x90;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x8);
}

/* 0x8011E5A8 | 44 bytes | compound_getter */
u16 fn_8011E5A8(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x90;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x6);
}

/* 0x8011E5D4 | 44 bytes | compound_getter */
u16 fn_8011E5D4(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x90;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x4);
}

/* 0x8011E600 | 44 bytes | compound_getter */
u16 fn_8011E600(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x90;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x2);
}

/* 0x8011E62C | 44 bytes | compound_getter */
u16 fn_8011E62C(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x90;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x0);
}

/* 0x8011E658 | 44 bytes | compound_getter */
u16 fn_8011E658(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x84;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0xA);
}

/* 0x8011E684 | 44 bytes | compound_getter */
u16 fn_8011E684(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x84;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x8);
}

/* 0x8011E6B0 | 44 bytes | compound_getter */
u16 fn_8011E6B0(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x84;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x6);
}

/* 0x8011E6DC | 44 bytes | compound_getter */
u16 fn_8011E6DC(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x84;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x4);
}

/* 0x8011E708 | 44 bytes | compound_getter */
u16 fn_8011E708(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x84;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x2);
}

/* 0x8011E734 | 44 bytes | compound_getter */
u16 fn_8011E734(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x84;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x0);
}

/* 0x8011E778 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011E778(void) {
    /* TODO: match -- 44 bytes at 0x8011E778 */
}
#pragma pop

/* 0x8011E7A4 | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011E7A4(void) {
    /* TODO: match -- 28 bytes at 0x8011E7A4 */
}
#pragma pop

/* 0x74 | fn_8011E868 | generic */
u32 fn_8011E868(u32 arg1, u32 arg2) {
    /* refs: lbl_80478F90, lbl_80478F94 */
    return 0;
}

/* 0x8011E90C | 44 bytes | compound_getter */
u8 fn_8011E90C(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0xC);
}

/* 0x8011E938 | 44 bytes | compound_getter */
u8 fn_8011E938(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0xB);
}

/* 0x8011E964 | 44 bytes | compound_getter */
u8 fn_8011E964(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0xA);
}

/* 0x8011E990 | 44 bytes | compound_getter */
u8 fn_8011E990(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x9);
}

/* 0x8011E9BC | 44 bytes | compound_getter */
u8 fn_8011E9BC(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x8);
}

/* 0x8011E9E8 | 44 bytes | compound_getter */
u8 fn_8011E9E8(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x7);
}

/* 0x8011EA14 | 44 bytes | compound_getter */
u8 fn_8011EA14(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x6);
}

/* 0x8011EA40 | 44 bytes | compound_getter */
u8 fn_8011EA40(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x5);
}

/* 0x8011EA6C | 44 bytes | compound_getter */
u8 fn_8011EA6C(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x4);
}

/* 0x8011EA98 | 44 bytes | compound_getter */
u8 fn_8011EA98(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x3);
}

/* 0x8011EAC4 | 44 bytes | compound_getter */
u8 fn_8011EAC4(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x2);
}

/* 0x8011EAF0 | 44 bytes | compound_getter */
u8 fn_8011EAF0(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x1);
}

/* 0x8011EB1C | 44 bytes | compound_getter */
u8 fn_8011EB1C(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xBD;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x0);
}

/* 0x8011EB60 | 44 bytes | compound_getter */
u8 fn_8011EB60(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xB7;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x4);
}

/* 0x8011EB8C | 44 bytes | compound_getter */
u8 fn_8011EB8C(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xB7;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x3);
}

/* 0x8011EBB8 | 44 bytes | compound_getter */
u8 fn_8011EBB8(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xB7;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x2);
}

/* 0x8011EBE4 | 44 bytes | compound_getter */
u8 fn_8011EBE4(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xB7;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x1);
}

/* 0x8011EC10 | 44 bytes | compound_getter */
u8 fn_8011EC10(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xB7;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x0);
}

/* 0x8011EC3C | 44 bytes | compound_getter */
u8 fn_8011EC3C(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xB2;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x4);
}

/* 0x8011EC68 | 44 bytes | compound_getter */
u8 fn_8011EC68(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xB2;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x3);
}

/* 0x8011EC94 | 44 bytes | compound_getter */
u8 fn_8011EC94(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xB2;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x2);
}

/* 0x8011ECC0 | 44 bytes | compound_getter */
u8 fn_8011ECC0(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xB2;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x1);
}

/* 0x8011ECEC | 44 bytes | compound_getter */
u8 fn_8011ECEC(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xB2;
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x0);
}

/* 0x50 | fn_8011ED18 | guarded_call */
u32 fn_8011ED18(void) {
    if (1 /* guard r3 != 0 */) { return 0; }
    fn_801EEEB8();
    return 0;
}

/* 0x5C | fn_8011ED68 | generic */
u32 fn_8011ED68(u32 arg1) {
    return 1;
}

/* 0x8011EDC4 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011EDC4(void) {
    /* TODO: match -- 52 bytes at 0x8011EDC4 */
}
#pragma pop

/* 0x8011EE70 | 44 bytes | compound_getter */
u16 fn_8011EE70(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xA4;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0xA);
}

/* 0x8011EE9C | 44 bytes | compound_getter */
u16 fn_8011EE9C(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xA4;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x8);
}

/* 0x8011EEC8 | 44 bytes | compound_getter */
u16 fn_8011EEC8(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xA4;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x6);
}

/* 0x8011EEF4 | 44 bytes | compound_getter */
u16 fn_8011EEF4(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xA4;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x4);
}

/* 0x8011EF20 | 44 bytes | compound_getter */
u16 fn_8011EF20(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xA4;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x2);
}

/* 0x8011EF4C | 44 bytes | compound_getter */
u16 fn_8011EF4C(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0xA4;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x0);
}

/* 0x8011EF78 | 44 bytes | compound_getter */
u16 fn_8011EF78(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x98;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0xA);
}

/* 0x8011EFA4 | 44 bytes | compound_getter */
u16 fn_8011EFA4(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x98;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x8);
}

/* 0x8011EFD0 | 44 bytes | compound_getter */
u16 fn_8011EFD0(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x98;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x6);
}

/* 0x8011EFFC | 44 bytes | compound_getter */
u16 fn_8011EFFC(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x98;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x4);
}

/* 0x8011F028 | 44 bytes | compound_getter */
u16 fn_8011F028(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x98;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x2);
}

/* 0x8011F054 | 44 bytes | compound_getter */
u16 fn_8011F054(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x98;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x0);
}

/* 0x8011F080 | 44 bytes | compound_getter */
u16 fn_8011F080(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x8C;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0xA);
}

/* 0x8011F0AC | 44 bytes | compound_getter */
u16 fn_8011F0AC(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x8C;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x8);
}

/* 0x8011F0D8 | 44 bytes | compound_getter */
u16 fn_8011F0D8(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x8C;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x6);
}

/* 0x8011F104 | 44 bytes | compound_getter */
u16 fn_8011F104(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x8C;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x4);
}

/* 0x8011F130 | 44 bytes | compound_getter */
u16 fn_8011F130(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x8C;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x2);
}

/* 0x8011F15C | 44 bytes | compound_getter */
u16 fn_8011F15C(void* ptr) {
    void* sub;
    if (ptr == NULL) { return 0; }
    sub = (u8*)ptr + 0x8C;
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x0);
}

/* 0x8011F1B8 | 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011F1B8(void) {
    /* TODO: match -- 56 bytes at 0x8011F1B8 */
}
#pragma pop

/* 0x8011F1F0 | 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011F1F0(void) {
    /* TODO: match -- 56 bytes at 0x8011F1F0 */
}
#pragma pop

/* 0x8011F228 | 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011F228(void) {
    /* TODO: match -- 56 bytes at 0x8011F228 */
}
#pragma pop

/* 0x8011F260 | 0x1FC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011F260(void) {
    /* TODO: match -- 508 bytes at 0x8011F260 */
}
#pragma pop

/* 0x8011F474 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011F474(void) {
    /* TODO: match -- 52 bytes at 0x8011F474 */
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011F5E0(void) {
    /* TODO: match -- 28 bytes at 0x8011F5E0 */
}
#pragma pop

/* 0x8011F5FC | 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011F5FC(void) {
    /* TODO: match -- 56 bytes at 0x8011F5FC */
}
#pragma pop

/* 0x8011F634 | 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011F634(void) {
    /* TODO: match -- 164 bytes at 0x8011F634 */
}
#pragma pop

/* 0x8011F6D8 | 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011F6D8(void) {
    /* TODO: match -- 164 bytes at 0x8011F6D8 */
}
#pragma pop

/* 0x8011F77C | 0x194 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011F77C(void) {
    /* TODO: match -- 404 bytes at 0x8011F77C */
}
#pragma pop

/* 0x8011F910 | 0x2BC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011F910(void) {
    /* TODO: match -- 700 bytes at 0x8011F910 */
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011FC74(void) {
    /* TODO: match -- 48 bytes at 0x8011FC74 */
}
#pragma pop

/* 0x8011FCA4 | 0x124 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011FCA4(void) {
    /* TODO: match -- 292 bytes at 0x8011FCA4 */
}
#pragma pop

/* 0x8011FDC8 | 0x504 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011FDC8(void) {
    /* TODO: match -- 1284 bytes at 0x8011FDC8 */
}
#pragma pop

/* 0x801202CC | 0x1DC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801202CC(void) {
    /* TODO: match -- 476 bytes at 0x801202CC */
}
#pragma pop

/* 0x801204A8 | 0x1CC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801204A8(void) {
    /* TODO: match -- 460 bytes at 0x801204A8 */
}
#pragma pop

/* 0x80120674 | 0x1F8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80120674(void) {
    /* TODO: match -- 504 bytes at 0x80120674 */
}
#pragma pop

/* 0x8012086C | 0x294 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012086C(void) {
    /* TODO: match -- 660 bytes at 0x8012086C */
}
#pragma pop

/* 0x80120B00 | 0x16C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80120B00(void) {
    /* TODO: match -- 364 bytes at 0x80120B00 */
}
#pragma pop

/* 0x70 | fn_80120C6C | generic */
u32 fn_80120C6C(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    /* refs: lbl_80478F90 */
    fn_8012640C();
    return 0;
}

/* 0x80120CDC | 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80120CDC(void) {
    /* TODO: match -- 144 bytes at 0x80120CDC */
}
#pragma pop

/* 0x64 | fn_80120D6C | generic */
u32 fn_80120D6C(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_8012640C();
    fn_80143B08();
    fn_80143ABC();
    return -1;
}

/* 0x80120DD0 | 0x210 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80120DD0(void) {
    /* TODO: match -- 528 bytes at 0x80120DD0 */
}
#pragma pop

/* 0x80120FE0 | 0x218 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80120FE0(void) {
    /* TODO: match -- 536 bytes at 0x80120FE0 */
}
#pragma pop

/* 0x801211F8 | 0x218 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801211F8(void) {
    /* TODO: match -- 536 bytes at 0x801211F8 */
}
#pragma pop

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

/* 0x64 | fn_80121BB4 | generic */
void fn_80121BB4(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_8011F5FC();
    fn_8011F5FC();
    fn_8011F5FC();
}

/* 0x80121C18 | 0x428 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80121C18(void) {
    /* TODO: match -- 1064 bytes at 0x80121C18 */
}
#pragma pop

/* 0x80122040 | 0x2F4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80122040(void) {
    /* TODO: match -- 756 bytes at 0x80122040 */
}
#pragma pop

/* 0x80122334 | 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80122334(void) {
    /* TODO: match -- 60 bytes at 0x80122334 */
}
#pragma pop

/* 0x80122370 | 0x360 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80122370(void) {
    /* TODO: match -- 864 bytes at 0x80122370 */
}
#pragma pop

/* 0x801226D0 | 0x324 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801226D0(void) {
    /* TODO: match -- 804 bytes at 0x801226D0 */
}
#pragma pop

/* 0x7C | fn_801229F4 | generic */
u32 fn_801229F4(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_8012640C();
    fn_8012640C();
    fn_8011CE74();
    fn_8011CE44();
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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80122BC0(void) {
    /* TODO: match -- 164 bytes at 0x80122BC0 */
}
#pragma pop

/* 0x80122C64 | 0x178 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80122C64(void) {
    /* TODO: match -- 376 bytes at 0x80122C64 */
}
#pragma pop

/* 0x80122DDC | 0x218 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80122DDC(void) {
    /* TODO: match -- 536 bytes at 0x80122DDC */
}
#pragma pop

/* 0x80122FF4 | 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80122FF4(void) {
    /* TODO: match -- 156 bytes at 0x80122FF4 */
}
#pragma pop

/* 0x50 | fn_80123090 | multi_call_cond */
u32 fn_80123090(void) {
    if (fn_8012640C() != 0) { return 0; }
    fn_80142CF4();
    return 0;
}

/* 0x801230E0 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801230E0(void) {
    /* TODO: match -- 48 bytes at 0x801230E0 */
}
#pragma pop

/* 0x80123110 | 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80123110(void) {
    /* TODO: match -- 148 bytes at 0x80123110 */
}
#pragma pop

/* 0x801231A4 | 0x13C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801231A4(void) {
    /* TODO: match -- 316 bytes at 0x801231A4 */
}
#pragma pop

/* 0x801232E0 | 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801232E0(void) {
    /* TODO: match -- 136 bytes at 0x801232E0 */
}
#pragma pop

/* 0x80123368 | 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80123368(void) {
    /* TODO: match -- 140 bytes at 0x80123368 */
}
#pragma pop

/* 0x801233F4 | 0x190 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801233F4(void) {
    /* TODO: match -- 400 bytes at 0x801233F4 */
}
#pragma pop

/* 0x80123584 | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80123584(void) {
    /* TODO: match -- 152 bytes at 0x80123584 */
}
#pragma pop

/* 0x8012361C | 0xDC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012361C(void) {
    /* TODO: match -- 220 bytes at 0x8012361C */
}
#pragma pop

/* 0x801236F8 | 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801236F8(void) {
    /* TODO: match -- 192 bytes at 0x801236F8 */
}
#pragma pop

/* 0x801237B8 | 0x3A4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801237B8(void) {
    /* TODO: match -- 932 bytes at 0x801237B8 */
}
#pragma pop

/* 0x80123B5C | 0xF8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80123B5C(void) {
    /* TODO: match -- 248 bytes at 0x80123B5C */
}
#pragma pop

/* 0x80 | fn_80123C54 | generic */
void fn_80123C54(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6) {
    fn_8011F260();
    fn_8011F260();
    fn_8011F5E0();
    fn_8011F5E0();
    fn_8011F5E0();
}

/* 0x80123CD4 | 0x84 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80123CD4(void) {
    /* TODO: match -- 132 bytes at 0x80123CD4 */
}
#pragma pop

/* 0x80123D58 | 0x118 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80123D58(void) {
    /* TODO: match -- 280 bytes at 0x80123D58 */
}
#pragma pop

/* 0x80 | fn_80123E70 | generic */
u32 fn_80123E70(void) {
    fn_8012640C();
    fn_8012640C();
    fn_8011BB6C(0, 0, 0, 0, 0);
    return 0;
}

/* 0x80123EF0 | 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80123EF0(void) {
    /* TODO: match -- 204 bytes at 0x80123EF0 */
}
#pragma pop

/* 0x80123FBC | 0x108 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80123FBC(void) {
    /* TODO: match -- 264 bytes at 0x80123FBC */
}
#pragma pop

/* 0x801240C4 | 0x34C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801240C4(void) {
    /* TODO: match -- 844 bytes at 0x801240C4 */
}
#pragma pop

/* 0x80124410 | 0x4B4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80124410(void) {
    /* TODO: match -- 1204 bytes at 0x80124410 */
}
#pragma pop

/* 0x801248C4 | 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801248C4(void) {
    /* TODO: match -- 180 bytes at 0x801248C4 */
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80125238(void) {
    /* TODO: match -- 168 bytes at 0x80125238 */
}
#pragma pop

/* 0x801252E0 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801252E0(void) {
    /* TODO: match -- 52 bytes at 0x801252E0 */
}
#pragma pop

/* 0x7C | fn_80125314 | generic */
void fn_80125314(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_801254B4();
    fn_801254B4();
    fn_801254B4();
}

/* 0x80125390 | 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80125390(void) {
    /* TODO: match -- 148 bytes at 0x80125390 */
}
#pragma pop

/* 0x48 | fn_80125424 | generic */
void fn_80125424(void) {
    fn_801254B4();
    fn_8012546C();
}

/* 0x48 | fn_8012546C | generic */
void fn_8012546C(void) {
    fn_8012640C();
    fn_8012795C();
}

/* 0x8012795C | 0x700 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012795C(void) {
    /* TODO: match -- 1792 bytes at 0x8012795C */
}
#pragma pop

/* 0x8012805C | 0x2A4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012805C(void) {
    /* TODO: match -- 676 bytes at 0x8012805C */
}
#pragma pop

/* 0x80128300 | 0x224 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80128300(void) {
    /* TODO: match -- 548 bytes at 0x80128300 */
}
#pragma pop

/* 0x80128524 | 0x1A4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80128524(void) {
    /* TODO: match -- 420 bytes at 0x80128524 */
}
#pragma pop

/* 0x801286C8 | 0x39C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801286C8(void) {
    /* TODO: match -- 924 bytes at 0x801286C8 */
}
#pragma pop

/* 0x80128A64 | 0x25C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80128A64(void) {
    /* TODO: match -- 604 bytes at 0x80128A64 */
}
#pragma pop

/* 0x80128CC0 | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80128CC0(void) {
    /* TODO: match -- 28 bytes at 0x80128CC0 */
}
#pragma pop

/* 0x80128CDC | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80128CDC(void) {
    /* TODO: match -- 28 bytes at 0x80128CDC */
}
#pragma pop

/* 0x80128CF8 | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80128CF8(void) {
    /* TODO: match -- 28 bytes at 0x80128CF8 */
}
#pragma pop

/* 0x80128D14 | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80128D14(void) {
    /* TODO: match -- 28 bytes at 0x80128D14 */
}
#pragma pop

/* 0x80128D30 | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80128D30(void) {
    /* TODO: match -- 28 bytes at 0x80128D30 */
}
#pragma pop

/* 0x80128D4C | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80128D4C(void) {
    /* TODO: match -- 28 bytes at 0x80128D4C */
}
#pragma pop

/* 0x80128D68 | 24 bytes | nc_addi_ptr */
void* fn_80128D68(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x7D20;
}

/* 0x80128D80 | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80128D80(void) {
    /* TODO: match -- 28 bytes at 0x80128D80 */
}
#pragma pop

/* 0x80128D9C | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80128D9C(void) {
    /* TODO: match -- 28 bytes at 0x80128D9C */
}
#pragma pop

/* 0x80128DB8 | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80128DB8(void) {
    /* TODO: match -- 28 bytes at 0x80128DB8 */
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80128E14(void) {
    /* TODO: match -- 16 bytes at 0x80128E14 */
}
#pragma pop

/* 0x80128E2C | 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80128E2C(void) {
    /* TODO: match -- 12 bytes at 0x80128E2C */
}
#pragma pop

/* 0x80128E38 | 0x25C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80128E38(void) {
    /* TODO: match -- 604 bytes at 0x80128E38 */
}
#pragma pop

/* 0x80129094 | 0x1EC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80129094(void) {
    /* TODO: match -- 492 bytes at 0x80129094 */
}
#pragma pop

/* 0x80129280 | 0x104 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80129280(void) {
    /* TODO: match -- 260 bytes at 0x80129280 */
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80129514(void) {
    /* TODO: match -- 136 bytes at 0x80129514 */
}
#pragma pop

/* 0x8012959C | 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012959C(void) {
    /* TODO: match -- 180 bytes at 0x8012959C */
}
#pragma pop

/* 0x80129650 | 0xC8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80129650(void) {
    /* TODO: match -- 200 bytes at 0x80129650 */
}
#pragma pop

/* 0x80129718 | 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80129718(void) {
    /* TODO: match -- 192 bytes at 0x80129718 */
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801298B8(void) {
    /* TODO: match -- 144 bytes at 0x801298B8 */
}
#pragma pop

/* 0x80 | fn_80129948 | generic */
void fn_80129948(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, u32 arg7) {
    fn_80129BC8();
    fn_80140A9C();
}

/* 0x801299C8 | 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801299C8(void) {
    /* TODO: match -- 176 bytes at 0x801299C8 */
}
#pragma pop

/* 0x80129A78 | 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80129A78(void) {
    /* TODO: match -- 180 bytes at 0x80129A78 */
}
#pragma pop

/* 0x80129B2C | 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80129B2C(void) {
    /* TODO: match -- 156 bytes at 0x80129B2C */
}
#pragma pop

/* 0x80129BC8 | 0x19C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80129BC8(void) {
    /* TODO: match -- 412 bytes at 0x80129BC8 */
}
#pragma pop

/* 0x80129D64 | 0xBC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80129D64(void) {
    /* TODO: match -- 188 bytes at 0x80129D64 */
}
#pragma pop

/* 0x80129E20 | 0x100 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80129E20(void) {
    /* TODO: match -- 256 bytes at 0x80129E20 */
}
#pragma pop

/* 0x80129F20 | 0x16C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80129F20(void) {
    /* TODO: match -- 364 bytes at 0x80129F20 */
}
#pragma pop

/* 0x8012A08C | 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012A08C(void) {
    /* TODO: match -- 164 bytes at 0x8012A08C */
}
#pragma pop

/* 0x74 | fn_8012A130 | multi_call_cond */
u32 fn_8012A130(void) {
    /* fn_8012A5B0() */
    fn_800F9EE4();
    fn_8012A5B0();
    return 0;
}

/* 0x8012A1A4 | 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012A1A4(void) {
    /* TODO: match -- 164 bytes at 0x8012A1A4 */
}
#pragma pop

/* 0x8012A248 | 0x208 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012A248(void) {
    /* TODO: match -- 520 bytes at 0x8012A248 */
}
#pragma pop

/* 0x8012A450 | 0x160 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012A450(void) {
    /* TODO: match -- 352 bytes at 0x8012A450 */
}
#pragma pop

/* 0x8012A5B0 | 0x1C4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012A5B0(void) {
    /* TODO: match -- 452 bytes at 0x8012A5B0 */
}
#pragma pop

/* 0x8012A7DC | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012A7DC(void) {
    /* TODO: match -- 48 bytes at 0x8012A7DC */
}
#pragma pop

/* 0x8012A824 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012A824(void) {
    /* TODO: match -- 48 bytes at 0x8012A824 */
}
#pragma pop

/* 0x8012A86C | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012A86C(void) {
    /* TODO: match -- 48 bytes at 0x8012A86C */
}
#pragma pop

/* 0x8012A89C | 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012A89C(void) {
    /* TODO: match -- 56 bytes at 0x8012A89C */
}
#pragma pop

/* 0x8012A8D4 | 24 bytes | nc_addi_ptr */
void* fn_8012A8D4(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0xAC2;
}

/* 0x8012AA64 | 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012AA64(void) {
    /* TODO: match -- 56 bytes at 0x8012AA64 */
}
#pragma pop

/* 0x8012AA9C | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012AA9C(void) {
    /* TODO: match -- 52 bytes at 0x8012AA9C */
}
#pragma pop

/* 0x8012AAD0 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012AAD0(void) {
    /* TODO: match -- 52 bytes at 0x8012AAD0 */
}
#pragma pop

/* 0x8012AB04 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012AB04(void) {
    /* TODO: match -- 52 bytes at 0x8012AB04 */
}
#pragma pop

/* 0x8012AB38 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012AB38(void) {
    /* TODO: match -- 52 bytes at 0x8012AB38 */
}
#pragma pop

/* 0x8012AB6C | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012AB6C(void) {
    /* TODO: match -- 52 bytes at 0x8012AB6C */
}
#pragma pop

/* 0x8012ABA0 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012ABA0(void) {
    /* TODO: match -- 52 bytes at 0x8012ABA0 */
}
#pragma pop

/* 0x8012ABD4 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012ABD4(void) {
    /* TODO: match -- 52 bytes at 0x8012ABD4 */
}
#pragma pop

/* 0x8012AC08 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012AC08(void) {
    /* TODO: match -- 52 bytes at 0x8012AC08 */
}
#pragma pop

/* 0x8012AC54 | 16 bytes | nc_bnelr */
u32 fn_8012AC54(void* ptr) {
    if (ptr != NULL) { return (u32)ptr; }
    return 0;
}

/* 0x8012AC64 | 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012AC64(void) {
    /* TODO: match -- 56 bytes at 0x8012AC64 */
}
#pragma pop

/* 0x8012AC9C | 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012AC9C(void) {
    /* TODO: match -- 180 bytes at 0x8012AC9C */
}
#pragma pop

/* 0x8012AD50 | 0x434 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012AD50(void) {
    /* TODO: match -- 1076 bytes at 0x8012AD50 */
}
#pragma pop

/* 0x8012B184 | 0x18 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012B184(void) {
    /* TODO: match -- 24 bytes at 0x8012B184 */
}
#pragma pop

/* 0x8012B19C | 0x448 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012B19C(void) {
    /* TODO: match -- 1096 bytes at 0x8012B19C */
}
#pragma pop

/* 0x8012B5E4 | 0x4EC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012B5E4(void) {
    /* TODO: match -- 1260 bytes at 0x8012B5E4 */
}
#pragma pop

/* 0x8012BAD0 | 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012BAD0(void) {
    /* TODO: match -- 32 bytes at 0x8012BAD0 */
}
#pragma pop

/* 0x8012BAF0 | 0xB8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012BAF0(void) {
    /* TODO: match -- 184 bytes at 0x8012BAF0 */
}
#pragma pop

/* 0x8012BBA8 | 0xFC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012BBA8(void) {
    /* TODO: match -- 252 bytes at 0x8012BBA8 */
}
#pragma pop

/* 0x8012BCA4 | 0x13C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012BCA4(void) {
    /* TODO: match -- 316 bytes at 0x8012BCA4 */
}
#pragma pop

/* 0x8012BDE0 | 0xD4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012BDE0(void) {
    /* TODO: match -- 212 bytes at 0x8012BDE0 */
}
#pragma pop

/* 0x8012BEB4 | 0x200 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012BEB4(void) {
    /* TODO: match -- 512 bytes at 0x8012BEB4 */
}
#pragma pop

/* 0x8012C0B4 | 0x48C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012C0B4(void) {
    /* TODO: match -- 1164 bytes at 0x8012C0B4 */
}
#pragma pop

/* 0x8012C540 | 0x120 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012C540(void) {
    /* TODO: match -- 288 bytes at 0x8012C540 */
}
#pragma pop

/* 0x8012C660 | 0x424 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012C660(void) {
    /* TODO: match -- 1060 bytes at 0x8012C660 */
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012D39C(void) {
    /* TODO: match -- 1108 bytes at 0x8012D39C */
}
#pragma pop

/* 0x8012D7F0 | 0x6A4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012D7F0(void) {
    /* TODO: match -- 1700 bytes at 0x8012D7F0 */
}
#pragma pop

/* 0x8012DE94 | 0x4F4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012DE94(void) {
    /* TODO: match -- 1268 bytes at 0x8012DE94 */
}
#pragma pop

/* 0x8012E388 | 0x430 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012E388(void) {
    /* TODO: match -- 1072 bytes at 0x8012E388 */
}
#pragma pop

/* 0x8012E7B8 | 0x41C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012E7B8(void) {
    /* TODO: match -- 1052 bytes at 0x8012E7B8 */
}
#pragma pop

/* 0x8012EBD4 | 0x3E4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012EBD4(void) {
    /* TODO: match -- 996 bytes at 0x8012EBD4 */
}
#pragma pop

/* 0x50 | fn_8012EFB8 | framed_no_calls */
u32 fn_8012EFB8(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_8047D030, lbl_8047D034 */
    return 1;
}

/* 0x8012F008 | 0x114 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012F008(void) {
    /* TODO: match -- 276 bytes at 0x8012F008 */
}
#pragma pop

/* 0x8012F11C | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012F11C(void) {
    /* TODO: match -- 52 bytes at 0x8012F11C */
}
#pragma pop

/* 0x8012F150 | 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012F150(void) {
    /* TODO: match -- 172 bytes at 0x8012F150 */
}
#pragma pop

/* 0x8012F1FC | 0x210 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012F1FC(void) {
    /* TODO: match -- 528 bytes at 0x8012F1FC */
}
#pragma pop

/* 0x8012F40C | 0x204 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012F40C(void) {
    /* TODO: match -- 516 bytes at 0x8012F40C */
}
#pragma pop

/* 0x8012F610 | 0x4C8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012F610(void) {
    /* TODO: match -- 1224 bytes at 0x8012F610 */
}
#pragma pop

/* 0x8012FAD8 | 0x1FC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012FAD8(void) {
    /* TODO: match -- 508 bytes at 0x8012FAD8 */
}
#pragma pop

/* 0x8012FCD4 | 0x380 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012FCD4(void) {
    /* TODO: match -- 896 bytes at 0x8012FCD4 */
}
#pragma pop

/* 0x80130054 | 0x1F8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80130054(void) {
    /* TODO: match -- 504 bytes at 0x80130054 */
}
#pragma pop

/* 0x8013024C | 0x414 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8013024C(void) {
    /* TODO: match -- 1044 bytes at 0x8013024C */
}
#pragma pop

/* 0x80130660 | 0x110 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80130660(void) {
    /* TODO: match -- 272 bytes at 0x80130660 */
}
#pragma pop

/* 0x80130770 | 0x120 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80130770(void) {
    /* TODO: match -- 288 bytes at 0x80130770 */
}
#pragma pop

/* 0x80130890 | 0x110 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80130890(void) {
    /* TODO: match -- 272 bytes at 0x80130890 */
}
#pragma pop

/* 0x801309A0 | 0xE8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801309A0(void) {
    /* TODO: match -- 232 bytes at 0x801309A0 */
}
#pragma pop

/* 0x80130A88 | 0x128 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80130A88(void) {
    /* TODO: match -- 296 bytes at 0x80130A88 */
}
#pragma pop

/* 0x80130BB0 | 0x128 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80130BB0(void) {
    /* TODO: match -- 296 bytes at 0x80130BB0 */
}
#pragma pop
