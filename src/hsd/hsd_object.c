/**
 * @file hsd_object.c
 * @brief HSD base object class initialization + GObj/flag internals.
 *
 * Colosseum address range: 0x80190E34 - 0x80191484
 * Adapted from doldecomp/melee src/sysdolphin/baselib/object.c
 */

#include "hsd/hsd_object.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"

HSD_ClassInfo hsdObj = { ObjInfoInit };
extern HSD_ClassInfo hsdClass;

/* ========================================================================= */
/*  Functions in range 0x80190E34 - 0x80191484                               */
/* ========================================================================= */

/* 0x80190E34 | 0x2C */
void fn_80190E34(void) {
    extern void fn_800E01D0();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r5 = r3;
    r3 = r4;
    r4 = r5 + 0x28;
    fn_800E01D0();
    return;
}

/* 0x80190E60 | 0x2B8 */
void fn_80190E60(void) {
    extern u8 lbl_80314638[];
    extern void fn_800D2248();
    extern void fn_800D5CB8();
    extern void fn_800D6680();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    extern void fn_800DA028();
    extern void fn_800DA4C4();
    extern void fn_800DFF98();
    extern void fn_800E0290();
    extern void fn_800E048C();
    extern void fn_800E04F4();
    extern void fn_800E0518();
    extern void fn_800E053C();
    extern void fn_800E0560();
    u8 sp[0x120];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    r31 = r3;
    r3 = (u32)sp + 0x20;
    r4 = *(u32*)((u8*)r31 + 0x8);
    f1 = *(f32*)((u8*)r4 + 0x0);
    fn_800E053C();
    r4 = *(u32*)((u8*)r31 + 0x8);
    r3 = (u32)sp + 0x50;
    f1 = *(f32*)((u8*)r4 + 0x4);
    fn_800E0518();
    r4 = *(u32*)((u8*)r31 + 0x8);
    r3 = (u32)sp + 0x80;
    f1 = *(f32*)((u8*)r4 + 0x8);
    fn_800E04F4();
    r4 = *(u32*)((u8*)r31 + 0xC);
    r3 = (u32)sp + 0xb0;
    f1 = *(f32*)((u8*)r4 + 0x0);
    f2 = *(f32*)((u8*)r4 + 0x4);
    f3 = *(f32*)((u8*)r4 + 0x8);
    fn_800E048C();
    r4 = *(u32*)((u8*)r31 + 0x4);
    r3 = (u32)sp + 0xe0;
    fn_800E0560();
    r3 = (u32)sp + 0x20;
    r4 = (u32)sp + 0x50;
    r5 = r3;
    fn_800E0290();
    r3 = (u32)sp + 0x20;
    r4 = (u32)sp + 0x80;
    r5 = r3;
    fn_800E0290();
    r3 = (u32)sp + 0x20;
    r4 = (u32)sp + 0xb0;
    r5 = r3;
    fn_800E0290();
    r3 = (u32)sp + 0x20;
    r4 = (u32)sp + 0xe0;
    r5 = r3;
    fn_800E0290();
    r3 = (u32)sp + 0x14;
    r4 = (u32)sp + 0x20;
    r5 = r31 + 0x10;
    fn_800DFF98();
    r3 = (u32)sp + 0x8;
    r4 = (u32)sp + 0x20;
    r5 = r31 + 0x1c;
    fn_800DFF98();
    fn_800D2248();
    r3 = 0x1;
    fn_800D88DC();
    r3 = 0x6;
    fn_800D888C();
    r3 = 0x0;
    fn_800DA028();
    r3 = 0x1;
    r4 = 0x6;
    r5 = 0x7;
    fn_800DA4C4();
    r3 = (u32)lbl_80314638;
    r3 = (u32)lbl_80314638;
    fn_800D7820();
    r3 = 0x4;
    fn_800D6A00();
    r3 = 0xa;
    fn_800D67BC();
    f1 = *(f32*)(sp + 0x14);
    f2 = *(f32*)(sp + 0x18);
    f3 = *(f32*)(sp + 0x1C);
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    r7 = 0x80;
    fn_800D5CB8();
    f1 = *(f32*)(sp + 0x14);
    f2 = *(f32*)(sp + 0xC);
    f3 = *(f32*)(sp + 0x1C);
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    r7 = 0x80;
    fn_800D5CB8();
    f1 = *(f32*)(sp + 0x8);
    f2 = *(f32*)(sp + 0x18);
    f3 = *(f32*)(sp + 0x1C);
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    r7 = 0x80;
    fn_800D5CB8();
    f1 = *(f32*)(sp + 0x8);
    f2 = *(f32*)(sp + 0xC);
    f3 = *(f32*)(sp + 0x1C);
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    r7 = 0x80;
    fn_800D5CB8();
    f1 = *(f32*)(sp + 0x8);
    f2 = *(f32*)(sp + 0x18);
    f3 = *(f32*)(sp + 0x10);
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    r7 = 0x80;
    fn_800D5CB8();
    f1 = *(f32*)(sp + 0x8);
    f2 = *(f32*)(sp + 0xC);
    f3 = *(f32*)(sp + 0x10);
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    r7 = 0x80;
    fn_800D5CB8();
    f1 = *(f32*)(sp + 0x14);
    f2 = *(f32*)(sp + 0x18);
    f3 = *(f32*)(sp + 0x10);
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    r7 = 0x80;
    fn_800D5CB8();
    f1 = *(f32*)(sp + 0x14);
    f2 = *(f32*)(sp + 0xC);
    f3 = *(f32*)(sp + 0x10);
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    r7 = 0x80;
    fn_800D5CB8();
    f1 = *(f32*)(sp + 0x14);
    f2 = *(f32*)(sp + 0x18);
    f3 = *(f32*)(sp + 0x1C);
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    r7 = 0x80;
    fn_800D5CB8();
    f1 = *(f32*)(sp + 0x14);
    f2 = *(f32*)(sp + 0xC);
    f3 = *(f32*)(sp + 0x1C);
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    r7 = 0x80;
    fn_800D5CB8();
    fn_800D6728();
    return;
}

/* 0x80191118 | 0x240 */
void fn_80191118(void) {
    extern u8 lbl_8047D8B8[];
    extern u8 lbl_8047D8BC[];
    extern u8 lbl_8047D8C0[];
    extern void fn_800D2F34();
    extern void fn_800DFF98();
    extern void fn_800E0290();
    extern void fn_800E048C();
    extern void fn_800E04F4();
    extern void fn_800E0518();
    extern void fn_800E053C();
    extern void fn_800E0560();
    u8 sp[0x130];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f8 = 0.0f;

    r31 = r3;
    r3 = (u32)sp + 0x38;
    r4 = *(u32*)((u8*)r31 + 0x8);
    f1 = *(f32*)((u8*)r4 + 0x0);
    fn_800E053C();
    r4 = *(u32*)((u8*)r31 + 0x8);
    r3 = (u32)sp + 0x68;
    f1 = *(f32*)((u8*)r4 + 0x4);
    fn_800E0518();
    r4 = *(u32*)((u8*)r31 + 0x8);
    r3 = (u32)sp + 0x98;
    f1 = *(f32*)((u8*)r4 + 0x8);
    fn_800E04F4();
    r4 = *(u32*)((u8*)r31 + 0xC);
    r3 = (u32)sp + 0xc8;
    f1 = *(f32*)((u8*)r4 + 0x0);
    f2 = *(f32*)((u8*)r4 + 0x4);
    f3 = *(f32*)((u8*)r4 + 0x8);
    fn_800E048C();
    r4 = *(u32*)((u8*)r31 + 0x4);
    r3 = (u32)sp + 0xf8;
    fn_800E0560();
    r3 = (u32)sp + 0x38;
    r4 = (u32)sp + 0x68;
    r5 = r3;
    fn_800E0290();
    r3 = (u32)sp + 0x38;
    r4 = (u32)sp + 0x98;
    r5 = r3;
    fn_800E0290();
    r3 = (u32)sp + 0x38;
    r4 = (u32)sp + 0xc8;
    r5 = r3;
    fn_800E0290();
    r3 = (u32)sp + 0x38;
    r4 = (u32)sp + 0xf8;
    r5 = r3;
    fn_800E0290();
    r3 = (u32)sp + 0x2c;
    r4 = (u32)sp + 0x38;
    r5 = r31 + 0x10;
    fn_800DFF98();
    r3 = (u32)sp + 0x20;
    r4 = (u32)sp + 0x38;
    r5 = r31 + 0x1c;
    fn_800DFF98();
    r3 = (u32)sp + 0x2c;
    r4 = (u32)sp + 0x14;
    fn_800D2F34();
    r31 = r3;
    r3 = (u32)sp + 0x20;
    r4 = (u32)sp + 0x8;
    fn_800D2F34();
    if ((s32)r31 == 0) { r3 = 0x2; return; }
    if ((s32)r3 == 0) {

        r3 = 0x2;
        return;
    }
    if (((s32)r31 == 1) && ((s32)r3 == 1)) {

        r3 = 0x0;
        return;
    }
    f2 = *(f32*)(sp + 0x14);
    f1 = *(f32*)lbl_8047D8B8;
    if (f2 < f1) {
        f0 = *(f32*)(sp + 0x8);
        if (f0 < f1) {
            r3 = 0x0;
            return;
    }
    }
    f1 = *(f32*)lbl_8047D8BC;
    if (f2 > f1) {
        f0 = *(f32*)(sp + 0x8);
        if (f0 > f1) {
            r3 = 0x0;
            return;
    }
    }
    f3 = *(f32*)(sp + 0x18);
    f1 = *(f32*)lbl_8047D8B8;
    if (f3 < f1) {
        f0 = *(f32*)(sp + 0xC);
        if (f0 < f1) {
            r3 = 0x0;
            return;
    }
    }
    f1 = *(f32*)lbl_8047D8C0;
    if (f3 > f1) {
        f0 = *(f32*)(sp + 0xC);
        if (f0 > f1) {
            r3 = 0x0;
            return;
    }
    }
    if ((s32)r31 == 1) { r3 = 0x1; return; }
    if ((s32)r3 == 1) {

        r3 = 0x1;
        return;
    }
    f1 = *(f32*)lbl_8047D8B8;
    if (f2 < f1) { r3 = 0x1; return; }
    f4 = *(f32*)(sp + 0x8);
    if (f4 < f1) {

        r3 = 0x1;
        return;
    }
    f0 = *(f32*)lbl_8047D8BC;
    if (f2 > f0) { r3 = 0x1; return; }
    if (f4 > f0) {

        r3 = 0x1;
        return;
    }
    if (f3 < f1) { r3 = 0x1; return; }
    f2 = *(f32*)(sp + 0xC);
    if (f2 < f1) {

        r3 = 0x1;
        return;
    }
    f0 = *(f32*)lbl_8047D8C0;
    if (f3 > f0) { r3 = 0x1; return; }
    if (f2 <= f0) { r3 = 0x2; return; }

    r3 = 0x1;
    return;

    r3 = 0x2;

    return;
}

/* 0x80191358 | 0x108 */
/*
 * BoundsExpandPoint - Expand an axis-aligned bounding box to include a point.
 *
 * If the 'first' flag (offset 0x01) is set, initializes both min and max
 * to the point coordinates. Otherwise, expands the existing min/max.
 * Then recomputes the center (offset 0x28) from the updated bounds.
 *
 * Bounding box layout:
 *   +0x10: min (x, y, z)
 *   +0x1C: max (x, y, z)
 *   +0x28: center (computed)
 *
 * 0x80191358 | size: 0x108
 */
void fn_80191358(u8* bbox, f32 x, f32 y, f32 z) {
    extern void fn_800E0168(f32* center, f32* max, f32* min);
    extern void fn_800E01F4(f32* vec, f32 x, f32 y, f32 z);

    if (*(u8*)(bbox + 0x01) != 0) {
        /* First point: initialize both min and max */
        *(u8*)(bbox + 0x01) = 0;
        fn_800E01F4((f32*)(bbox + 0x10), x, y, z);
        fn_800E01F4((f32*)(bbox + 0x1C), x, y, z);
    } else {
        /* Expand min bounds */
        if (x < *(f32*)(bbox + 0x10)) { *(f32*)(bbox + 0x10) = x; }
        if (y < *(f32*)(bbox + 0x14)) { *(f32*)(bbox + 0x14) = y; }
        if (z < *(f32*)(bbox + 0x18)) { *(f32*)(bbox + 0x18) = z; }
        /* Expand max bounds */
        if (x > *(f32*)(bbox + 0x1C)) { *(f32*)(bbox + 0x1C) = x; }
        if (y > *(f32*)(bbox + 0x20)) { *(f32*)(bbox + 0x20) = y; }
        if (z > *(f32*)(bbox + 0x24)) { *(f32*)(bbox + 0x24) = z; }
    }

    /* Recompute center */
    fn_800E0168((f32*)(bbox + 0x28), (f32*)(bbox + 0x1C), (f32*)(bbox + 0x10));
}

/* 0x80191460 | 0xC -- small: ObjInfoInit */
void fn_80191460(void) {
    /* ObjInfoInit wrapper / trampoline */
}

void ObjInfoInit(void)
{
    hsdInitClassInfo(&hsdObj, &hsdClass, "sysdolphin_base_library", "hsd_obj",
                     sizeof(HSD_ObjInfo), sizeof(HSD_Obj));
}

/* ===================================================================
 * Accessor functions in this range
 * =================================================================== */

/* Address: 0x8019146C | Size: 0x8 | Pattern: simple_setter */
void fn_8019146C(u8* obj, u32 val) {
    *(u32*)((u8*)obj + 0xC) = val;
}

/* Address: 0x80191474 | Size: 0x8 | Pattern: simple_setter */
void fn_80191474(u8* obj, u32 val) {
    *(u32*)((u8*)obj + 0x8) = val;
}

/* Address: 0x8019147C | Size: 0x8 | Pattern: simple_setter */
void fn_8019147C(u8* obj, u32 val) {
    *(u32*)((u8*)obj + 0x4) = val;
}

/* 0x70 | fn_80191484 | generic */
void fn_80191484(void) {
    /* refs: lbl_8047B208, lbl_8047B20C, lbl_8047B210 */
    fn_800E3534();
    fn_800E27B0();
}
