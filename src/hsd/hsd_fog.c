/**
 * @file hsd_fog.c
 * @brief HSD Fog and FogAdj implementation.
 *
 * Colosseum address: 0x8019B7C0 (HSD_FogAdjInit)
 * Adapted from doldecomp/melee src/sysdolphin/baselib/fog.c
 */

#include "hsd/hsd_fog.h"
#include "hsd/hsd_aobj.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_object.h"

extern void* memcpy(void* dst, const void* src, u32 size);

static void FogInfoInit(void);
static void FogAdjInfoInit(void);

static HSD_FogInfo hsdFog = { FogInfoInit };
static HSD_FogAdjInfo hsdFogAdj = { FogAdjInfoInit };

/* ========================================================================= */
/*  FogAdj                                                                   */
/* ========================================================================= */

void HSD_FogAdjInit(HSD_FogAdj* fogadj, HSD_FogAdjDesc* desc)
{
    if (fogadj == NULL || desc == NULL) {
        return;
    }
    fogadj->center = (s16) desc->center;
    fogadj->width = desc->width;
    memcpy(fogadj->mtx, desc->mtx, sizeof(f32) * 16);
}

HSD_FogAdj* HSD_FogAdjLoadDesc(HSD_FogAdjDesc* desc)
{
    HSD_FogAdj* fogadj;

    if (desc == NULL) {
        return NULL;
    }

    fogadj = HSD_FogAdjAlloc();
    HSD_FogAdjInit(fogadj, desc);
    return fogadj;
}

HSD_FogAdj* HSD_FogAdjAlloc(void)
{
    HSD_FogAdj* fogadj;
    fogadj = (HSD_FogAdj*) hsdNew(&hsdFogAdj.parent.parent);
    HSD_ASSERT(0, fogadj);
    return fogadj;
}

/* ========================================================================= */
/*  Fog                                                                      */
/* ========================================================================= */

void HSD_FogInit(HSD_Fog* fog, HSD_FogDesc* desc)
{
    if (fog == NULL || desc == NULL) {
        return;
    }
    fog->type = desc->type;
    fog->start = desc->start;
    fog->end = desc->end;
    fog->color = desc->color;
    fog->fog_adj = HSD_FogAdjLoadDesc(desc->fogadjdesc);
}

HSD_Fog* HSD_FogLoadDesc(HSD_FogDesc* desc)
{
    HSD_Fog* fog;

    if (desc == NULL) {
        return NULL;
    }

    fog = HSD_FogAlloc();
    HSD_FogInit(fog, desc);
    return fog;
}

HSD_Fog* HSD_FogAlloc(void)
{
    HSD_Fog* fog;
    fog = (HSD_Fog*) hsdNew(&hsdFog.parent.parent);
    HSD_ASSERT(0, fog);
    return fog;
}

void HSD_FogReqAnim(HSD_Fog* fog, f32 frame)
{
    if (fog != NULL) {
        HSD_AObjReqAnim(fog->aobj, frame);
    }
}

void HSD_FogInterpretAnim(HSD_Fog* fog)
{
    if (fog == NULL) {
        return;
    }
    /* Interpret AObj -> update fog parameters */
}

/* ========================================================================= */
/*  Class init                                                               */
/* ========================================================================= */

static void FogRelease(HSD_Class* o)
{
    HSD_Fog* fog = (HSD_Fog*) o;
    HSD_AObjRemove(fog->aobj);
    HSD_OBJECT_PARENT_INFO(&hsdFog)->release(o);
}

static void FogInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdFog), HSD_CLASS_INFO(&hsdObj),
                     "sysdolphin_base_library", "hsd_fog",
                     sizeof(HSD_FogInfo), sizeof(HSD_Fog));
    HSD_CLASS_INFO(&hsdFog)->release = FogRelease;
}

static void FogAdjRelease(HSD_Class* o)
{
    HSD_FogAdj* fogadj = (HSD_FogAdj*) o;
    HSD_AObjRemove(fogadj->aobj);
    HSD_OBJECT_PARENT_INFO(&hsdFogAdj)->release(o);
}

static void FogAdjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdFogAdj), HSD_CLASS_INFO(&hsdObj),
                     "sysdolphin_base_library", "hsd_fogadj",
                     sizeof(HSD_FogAdjInfo), sizeof(HSD_FogAdj));
    HSD_CLASS_INFO(&hsdFogAdj)->release = FogAdjRelease;
}

/* =========================================================================
 *  Internal stubs: 0x8019B490-0x8019C3C4 (20 functions)
 * ========================================================================= */

/* 0x8019B490 | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019B490(void) {
    extern void fn_80199AF8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x18) = f31;
    f31 = f1;
    r31 = r5;
    r30 = r4;
    r29 = r3;
    goto L_8019B500;
L_8019B4C0: ;
    if ((u32)r29 == (u32)0x0) goto L_8019B4FC;
    r0 = *(u8*)((u8*)r29 + 0x12);
    if ((u32)r0 != (u32)0x6) goto L_8019B4E8;
    f1 = f31;
    r3 = r29;
    r4 = r30;
    r5 = r31;
    fn_80199AF8();
L_8019B4E8: ;
    if ((u32)r29 == (u32)0x0) goto L_8019B4FC;
    r0 = *(u8*)((u8*)r29 + 0x10);
    r0 = r0 & 0x000000F0;
    *(u8*)((u8*)r29 + 0x10) = r0;
L_8019B4FC: ;
    r29 = *(u32*)((u8*)r29 + 0x0);
L_8019B500: ;
    if ((u32)r29 != (u32)0x0) goto L_8019B4C0;
    f31 = *(f64*)(sp + 0x18);
    r31 = *(u32*)(sp + 0x14);
    r30 = *(u32*)(sp + 0x10);
    r29 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x8019B528 | 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019B528(void) {
    extern u8 lbl_8047DA3C[];
    extern u8 lbl_8047DA58[];
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;

    if ((u32)r3 == (u32)0x0) goto L_8019B5E0;
    goto L_8019B5D8;
L_8019B538: ;
    if ((u32)r3 == (u32)0x0) goto L_8019B5D4;
    r4 = *(u32*)((u8*)r3 + 0x8);
    r0 = (0x4330 << 16);
    r6 = 0x0;
    f6 = *(f64*)lbl_8047DA58;
    r5 = 0x0;
    *(u32*)((u8*)r3 + 0x4) = r4;
    r4 = 0x0;
    r0 = 0x0;
    f4 = *(f32*)lbl_8047DA3C;
    r7 = *(s16*)((u8*)r3 + 0x18);
    f3 = *(f32*)lbl_8047DA3C;
    /* xoris r7, r7, 0x8000 */;
    f2 = *(f32*)lbl_8047DA3C;
    f0 = *(f32*)lbl_8047DA3C;
    f5 = *(f64*)((u8*)r1 + 0x8);
    f5 = f5 - f6;
    f5 = f5 + f1;
    *(f32*)((u8*)r3 + 0x1C) = f5;
    *(u8*)((u8*)r3 + 0x11) = r6;
    *(u8*)((u8*)r3 + 0x12) = r5;
    r5 = *(u8*)((u8*)r3 + 0x10);
    r5 = r5 & 0xFFFFFFBF;
    *(u8*)((u8*)r3 + 0x10) = r5;
    *(u16*)((u8*)r3 + 0x16) = r4;
    *(u16*)((u8*)r3 + 0x1A) = r0;
    *(f32*)((u8*)r3 + 0x20) = f4;
    *(f32*)((u8*)r3 + 0x24) = f3;
    *(f32*)((u8*)r3 + 0x28) = f2;
    *(f32*)((u8*)r3 + 0x2C) = f0;
    if ((u32)r3 == (u32)0x0) goto L_8019B5D4;
    r0 = *(u8*)((u8*)r3 + 0x10);
    r0 = r0 & 0x000000F0;
    r0 = r0 | 0x1;
    *(u8*)((u8*)r3 + 0x10) = r0;
L_8019B5D4: ;
    r3 = *(u32*)((u8*)r3 + 0x0);
L_8019B5D8: ;
    if ((u32)r3 != (u32)0x0) goto L_8019B538;
L_8019B5E0: ;
    return;
}
#pragma pop

/* 0x8019B5E8 | 0x168 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019B5E8(void) {
    extern u8 lbl_802747AC[];
    extern u8 lbl_80465378[];
    extern void fn_8019970C();
    extern void fn_80199A84();
    extern void fn_8019B5E8();
    extern void fn_8019B750();
    extern void fn_801AA498();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r23, 0xc(r1) */;
    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) goto L_8019B73C;
    r30 = *(u32*)((u8*)r31 + 0x0);
    if ((u32)r30 == (u32)0x0) goto L_8019B724;
    r29 = *(u32*)((u8*)r30 + 0x0);
    if ((u32)r29 == (u32)0x0) goto L_8019B70C;
    r28 = *(u32*)((u8*)r29 + 0x0);
    if ((u32)r28 == (u32)0x0) goto L_8019B6F4;
    r27 = *(u32*)((u8*)r28 + 0x0);
    if ((u32)r27 == (u32)0x0) goto L_8019B6DC;
    r26 = *(u32*)((u8*)r27 + 0x0);
    if ((u32)r26 == (u32)0x0) goto L_8019B6C4;
    r25 = *(u32*)((u8*)r26 + 0x0);
    if ((u32)r25 == (u32)0x0) goto L_8019B6AC;
    r24 = *(u32*)((u8*)r25 + 0x0);
    if ((u32)r24 == (u32)0x0) goto L_8019B680;
    r23 = *(u32*)((u8*)r24 + 0x0);
    if ((u32)r23 == (u32)0x0) goto L_8019B670;
    r3 = *(u32*)((u8*)r23 + 0x0);
    fn_8019B5E8();
    r3 = r23;
    fn_8019B750();
L_8019B670: ;
    if ((u32)r24 == (u32)0x0) goto L_8019B680;
    r3 = r24;
    fn_8019970C();
L_8019B680: ;
    if ((u32)r25 == (u32)0x0) goto L_8019B6AC;
    r3 = (u32)lbl_80465378;
    r4 = r25;
    r3 = (u32)lbl_80465378;
    fn_801AA498();
    r3 = (u32)lbl_80465378;
    r4 = (u32)lbl_802747AC;
    r3 = (u32)lbl_80465378;
    r4 = (u32)lbl_802747AC;
    fn_80199A84();
L_8019B6AC: ;
    if ((u32)r26 == (u32)0x0) goto L_8019B6C4;
    r3 = (u32)lbl_80465378;
    r4 = r26;
    r3 = (u32)lbl_80465378;
    fn_801AA498();
L_8019B6C4: ;
    if ((u32)r27 == (u32)0x0) goto L_8019B6DC;
    r3 = (u32)lbl_80465378;
    r4 = r27;
    r3 = (u32)lbl_80465378;
    fn_801AA498();
L_8019B6DC: ;
    if ((u32)r28 == (u32)0x0) goto L_8019B6F4;
    r3 = (u32)lbl_80465378;
    r4 = r28;
    r3 = (u32)lbl_80465378;
    fn_801AA498();
L_8019B6F4: ;
    if ((u32)r29 == (u32)0x0) goto L_8019B70C;
    r3 = (u32)lbl_80465378;
    r4 = r29;
    r3 = (u32)lbl_80465378;
    fn_801AA498();
L_8019B70C: ;
    if ((u32)r30 == (u32)0x0) goto L_8019B724;
    r3 = (u32)lbl_80465378;
    r4 = r30;
    r3 = (u32)lbl_80465378;
    fn_801AA498();
L_8019B724: ;
    if ((u32)r31 == (u32)0x0) goto L_8019B73C;
    r3 = (u32)lbl_80465378;
    r4 = r31;
    r3 = (u32)lbl_80465378;
    fn_801AA498();
L_8019B73C: ;
    /* lmw r23, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x8019B750 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019B750(void) {
    extern u8 lbl_80465378[];
    extern void fn_801AA498();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    if ((u32)r3 == (u32)0x0) goto L_8019B774;
    r5 = (u32)lbl_80465378;
    r4 = r3;
    r3 = (u32)lbl_80465378;
    fn_801AA498();
L_8019B774: ;
    return;
}
#pragma pop

/* 0x8019B784 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019B784(void) {
    extern u8 lbl_80465378[];
    extern void fn_801AA35C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r3 = (u32)lbl_80465378;
    r4 = 0x30;
    r3 = (u32)lbl_80465378;
    r5 = 0x4;
    fn_801AA35C();
    return;
}
#pragma pop

/* 0x8019B7B4 | 0xC */
void fn_8019B7B4(void) {
}

/* 0x48 | fn_8019B7C0 | single_call_straight */
void fn_8019B7C0(void) {
    fn_80193B30();
}

/* 0x6C | fn_8019B808 | single_call_straight */
void fn_8019B808(void) {
    fn_80193B30();
}

/* 0x8019B874 | 0xD4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019B874(void) {
    extern u8 lbl_8036C7E8[];
    extern void fn_801C25E4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r30 = r3;
    r31 = *(u32*)((u8*)r3 + 0xC);
    if ((u32)r31 == (u32)0x0) goto L_8019B90C;
    r3 = (0x1 << 16);
    r4 = *(u16*)((u8*)r31 + 0x4);
    /* subi r3, r3, 0x1 */;
    r0 = r3 & 0xFFFF;
    r0 = r0 - r4;
    r0 = __cntlzw(r0);
    /* srwi. r3, r0, 5 */;
    if ((u32)r31 == (u32)0x0) goto L_8019B8BC;
    goto L_8019B8D4;
L_8019B8BC: ;
    r0 = *(u16*)((u8*)r31 + 0x4);
    r3 = *(u16*)((u8*)r31 + 0x4);
    r4 = __cntlzw(r0);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r31 + 0x4) = r0;
    r3 = (u32)r4 >> 5;
L_8019B8D4: ;
    if ((s32)r3 == (s32)0x0) goto L_8019B90C;
    if ((u32)r31 == (u32)0x0) goto L_8019B90C;
    r4 = *(u32*)((u8*)r31 + 0x0);
    r3 = r31;
    r12 = *(u32*)((u8*)r4 + 0x30);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r4 = *(u32*)((u8*)r31 + 0x0);
    r3 = r31;
    r12 = *(u32*)((u8*)r4 + 0x34);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_8019B90C: ;
    r3 = *(u32*)((u8*)r30 + 0x1C);
    fn_801C25E4();
    r4 = (u32)lbl_8036C7E8;
    r3 = r30;
    r4 = (u32)lbl_8036C7E8;
    r4 = *(u32*)((u8*)r4 + 0x14);
    r12 = *(u32*)((u8*)r4 + 0x30);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x8019B948 | 0x230 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019B948(void) {
    extern u8 lbl_8047DA68[];
    extern u8 lbl_8047DA6C[];
    extern u8 lbl_8047DA70[];
    extern u8 jumptable_8036C864[];
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u8 sp[0x100];

    if ((u32)r3 == (u32)0x0) goto L_8019BB70;
    if ((u32)r4 > (u32)0x15) goto L_8019BB70;
    r6 = (u32)jumptable_8036C864;
    r0 = r4 << 2;
    r4 = (u32)jumptable_8036C864;
    r4 = *(u32*)(r4 + r0);
    ctr_fn = (void(*)(void))r4;
    /* indirect jump via ctr */;
    f0 = *(f32*)((u8*)r5 + 0x0);
    *(f32*)((u8*)r3 + 0x10) = f0;
    goto L_8019BB70;
    f0 = *(f32*)((u8*)r5 + 0x0);
    *(f32*)((u8*)r3 + 0x14) = f0;
    goto L_8019BB70;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DA68;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_8019B9A8;
    f1 = *(f32*)lbl_8047DA68;
    goto L_8019B9BC;
L_8019B9A8: ;
    f0 = *(f32*)lbl_8047DA6C;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_8019B9BC;
    f1 = *(f32*)lbl_8047DA6C;
L_8019B9BC: ;
    f0 = *(f32*)lbl_8047DA70;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x8) = f0;
    *(u8*)((u8*)r3 + 0x18) = r0;
    goto L_8019BB70;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DA68;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_8019B9F4;
    f1 = *(f32*)lbl_8047DA68;
    goto L_8019BA08;
L_8019B9F4: ;
    f0 = *(f32*)lbl_8047DA6C;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_8019BA08;
    f1 = *(f32*)lbl_8047DA6C;
L_8019BA08: ;
    f0 = *(f32*)lbl_8047DA70;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x8) = f0;
    *(u8*)((u8*)r3 + 0x19) = r0;
    goto L_8019BB70;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DA68;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_8019BA40;
    f1 = *(f32*)lbl_8047DA68;
    goto L_8019BA54;
L_8019BA40: ;
    f0 = *(f32*)lbl_8047DA6C;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_8019BA54;
    f1 = *(f32*)lbl_8047DA6C;
L_8019BA54: ;
    f0 = *(f32*)lbl_8047DA70;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x8) = f0;
    *(u8*)((u8*)r3 + 0x1A) = r0;
    goto L_8019BB70;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DA68;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_8019BA8C;
    f1 = *(f32*)lbl_8047DA68;
    goto L_8019BAA0;
L_8019BA8C: ;
    f0 = *(f32*)lbl_8047DA6C;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_8019BAA0;
    f1 = *(f32*)lbl_8047DA6C;
L_8019BAA0: ;
    f0 = *(f32*)lbl_8047DA70;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x8) = f0;
    *(u8*)((u8*)r3 + 0x1B) = r0;
    goto L_8019BB70;
    f0 = *(f32*)((u8*)r5 + 0x0);
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x8) = f0;
    if ((u32)r3 == (u32)0x0) goto L_8019BADC;
    r3 = *(u32*)((u8*)r3 + 0xC);
    goto L_8019BAE0;
L_8019BADC: ;
    r3 = 0x0;
L_8019BAE0: ;
    if ((u32)r3 == (u32)0x0) goto L_8019BB70;
    if ((s32)r0 > (s32)-0x140) goto L_8019BAFC;
    r0 = -0x140;
    *(u16*)((u8*)r3 + 0xC) = r0;
    goto L_8019BB70;
L_8019BAFC: ;
    if ((s32)r0 < (s32)0x140) goto L_8019BB10;
    r0 = 0x140;
    *(u16*)((u8*)r3 + 0xC) = r0;
    goto L_8019BB70;
L_8019BB10: ;
    *(u16*)((u8*)r3 + 0xC) = r0;
    goto L_8019BB70;
    f0 = *(f32*)((u8*)r5 + 0x0);
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x8) = f0;
    if ((u32)r3 == (u32)0x0) goto L_8019BB38;
    r3 = *(u32*)((u8*)r3 + 0xC);
    goto L_8019BB3C;
L_8019BB38: ;
    r3 = 0x0;
L_8019BB3C: ;
    if ((u32)r3 == (u32)0x0) goto L_8019BB70;
    if ((s32)r0 > (s32)0x0) goto L_8019BB58;
    r0 = 0x0;
    *(u16*)((u8*)r3 + 0xE) = r0;
    goto L_8019BB70;
L_8019BB58: ;
    if ((s32)r0 < (s32)0x280) goto L_8019BB6C;
    r0 = 0x280;
    *(u16*)((u8*)r3 + 0xE) = r0;
    goto L_8019BB70;
L_8019BB6C: ;
    *(u16*)((u8*)r3 + 0xE) = r0;
L_8019BB70: ;
    return;
}
#pragma pop

/* 0x8019BB78 | 0x1A0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019BB78(void) {
    extern u8 lbl_8036C7E8[];
    extern u8 lbl_8036C828[];
    extern u8 lbl_8047DA74[];
    extern u8 lbl_8047DA7C[];
    extern u8 lbl_8047DA80[];
    extern void fn_800BD768();
    extern void fn_80193828();
    extern void fn_80196E10();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f9 = 0.0f;

    r4 = (u32)lbl_8036C7E8;
    r30 = r3;
    r3 = (u32)lbl_8036C7E8;
    fn_80193828();
    /* mr. r29, r3 */;
    if ((s32)r0 != (s32)0) goto L_8019BBB8;
    r3 = (u32)lbl_8047DA74;
    r4 = 0xa1;
    r5 = (u32)lbl_8047DA80;
    fn_80196E10();
L_8019BBB8: ;
    /* mr. r31, r29 */;
    if ((s32)r0 != (s32)0) goto L_8019BBD0;
    r3 = (u32)lbl_8047DA74;
    r4 = 0xae;
    r5 = (u32)lbl_8047DA80;
    fn_80196E10();
L_8019BBD0: ;
    if ((u32)r31 == (u32)0x0) goto L_8019BC44;
    if ((u32)r30 == (u32)0x0) goto L_8019BC04;
    r0 = *(u32*)((u8*)r30 + 0x0);
    *(u32*)((u8*)r31 + 0x8) = r0;
    f0 = *(f32*)((u8*)r30 + 0x8);
    *(f32*)((u8*)r31 + 0x10) = f0;
    f0 = *(f32*)((u8*)r30 + 0xC);
    *(f32*)((u8*)r31 + 0x14) = f0;
    r0 = *(u32*)((u8*)r30 + 0x10);
    *(u32*)((u8*)r31 + 0x18) = r0;
    goto L_8019BC44;
L_8019BC04: ;
    r3 = r1 + 0x8;
    fn_800BD768();
    r0 = 0x2;
    r5 = 0xff;
    *(u32*)((u8*)r31 + 0x8) = r0;
    r4 = 0xff;
    r3 = 0xff;
    r0 = 0xff;
    f0 = *(f32*)(sp + 0x18);
    *(f32*)((u8*)r31 + 0x10) = f0;
    f0 = *(f32*)(sp + 0x1C);
    *(f32*)((u8*)r31 + 0x14) = f0;
    *(u8*)((u8*)r31 + 0x18) = r5;
    *(u8*)((u8*)r31 + 0x19) = r4;
    *(u8*)((u8*)r31 + 0x1A) = r3;
    *(u8*)((u8*)r31 + 0x1B) = r0;
L_8019BC44: ;
    r0 = *(u32*)((u8*)r30 + 0x4);
    if ((u32)r0 == (u32)0x0) goto L_8019BCF8;
    r3 = (u32)lbl_8036C828;
    r30 = *(u32*)((u8*)r30 + 0x4);
    r3 = (u32)lbl_8036C828;
    fn_80193828();
    /* mr. r29, r3 */;
    if ((u32)r0 != (u32)0x0) goto L_8019BC78;
    r3 = (u32)lbl_8047DA74;
    r4 = 0xf9;
    r5 = (u32)lbl_8047DA7C;
    fn_80196E10();
L_8019BC78: ;
    if ((u32)r29 != (u32)0x0) goto L_8019BC90;
    r3 = (u32)lbl_8047DA74;
    r4 = 0x109;
    r5 = (u32)lbl_8047DA7C;
    fn_80196E10();
L_8019BC90: ;
    if ((u32)r29 == (u32)0x0) goto L_8019BCF4;
    if ((u32)r30 == (u32)0x0) goto L_8019BCCC;
    r0 = *(u32*)((u8*)r30 + 0x0);
    r3 = r29 + 0x10;
    r4 = r30 + 0x8;
    r5 = 0x40;
    *(u32*)((u8*)r29 + 0x8) = r0;
    r0 = *(u16*)((u8*)r30 + 0x6);
    *(u16*)((u8*)r29 + 0xE) = r0;
    r0 = *(u16*)((u8*)r30 + 0x4);
    *(u16*)((u8*)r29 + 0xC) = r0;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    goto L_8019BCF4;
L_8019BCCC: ;
    r0 = 0x0;
    r5 = 0x0;
    *(u32*)((u8*)r29 + 0x8) = r0;
    r0 = 0x0;
    r3 = r29 + 0x10;
    r4 = 0x0;
    *(u16*)((u8*)r29 + 0xE) = r5;
    r5 = 0x40;
    *(u16*)((u8*)r29 + 0xC) = r0;
    memset((void*)r3, (int)r4, (u32)r5);
L_8019BCF4: ;
    *(u32*)((u8*)r31 + 0xC) = r29;
L_8019BCF8: ;
    r3 = r31;
    r31 = *(u32*)(sp + 0x2C);
    r30 = *(u32*)(sp + 0x28);
    r29 = *(u32*)(sp + 0x24);
    return;
}
#pragma pop

/* 0x8019BD18 | 0x2D0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019BD18(void) {
    extern u8 lbl_802747DC[];
    extern u8 lbl_8047DA68[];
    extern u8 lbl_8047DA6C[];
    extern u8 lbl_8047DA74[];
    extern u8 lbl_8047DA84[];
    extern u8 lbl_8047DA88[];
    extern u8 lbl_8047DA8C[];
    extern u8 lbl_8047DA90[];
    extern u8 lbl_8047E720[];
    extern void fn_800BC8F8();
    extern void fn_800BCB14();
    extern void fn_800BCCDC();
    extern void fn_800BD454();
    extern void fn_800BD768();
    extern void fn_801942B8();
    extern void fn_801944D0();
    extern void fn_801944F8();
    extern void fn_80196D78();
    u8 sp[0xE0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    *(f64*)(sp + 0xD0) = f31;
    /* psq_st f31, 0xd8(r1), 0, qr0 */;
    /* mr. r28, r3 */;
    if ((s32)r0 != (s32)0) goto L_8019BD74;
    r0 = *(u32*)lbl_8047E720;
    r4 = r1 + 0xc;
    f1 = *(f32*)lbl_8047DA68;
    r3 = 0x0;
    *(u32*)(sp + 0x10) = r0;
    f2 = *(f32*)lbl_8047DA68;
    r0 = *(u32*)(sp + 0x10);
    f3 = *(f32*)lbl_8047DA68;
    *(u32*)(sp + 0xC) = r0;
    f4 = *(f32*)lbl_8047DA68;
    fn_800BC8F8();
    goto L_8019BFC0;
L_8019BD74: ;
    fn_801942B8();
    /* mr. r30, r3 */;
    if ((s32)r0 != (s32)0) goto L_8019BD94;
    r4 = (u32)lbl_802747DC;
    r3 = (u32)lbl_8047DA74;
    r5 = (u32)lbl_802747DC;
    r4 = 0x58;
    fn_80196D78();
L_8019BD94: ;
    r0 = *(u32*)((u8*)r28 + 0x18);
    r3 = r30;
    r31 = r1 + 0x8;
    *(u32*)(sp + 0x8) = r0;
    fn_801944D0();
    f31 = f1;
    r3 = r30;
    fn_801944F8();
    f3 = f1;
    r3 = *(u32*)((u8*)r28 + 0x8);
    f4 = f31;
    f1 = *(f32*)((u8*)r28 + 0x10);
    f2 = *(f32*)((u8*)r28 + 0x14);
    r4 = r31;
    fn_800BC8F8();
    if ((u32)r28 == (u32)0x0) goto L_8019BDE0;
    r30 = *(u32*)((u8*)r28 + 0xC);
    goto L_8019BDE4;
L_8019BDE0: ;
    r30 = 0x0;
L_8019BDE4: ;
    if ((u32)r30 == (u32)0x0) goto L_8019BDF4;
    r31 = *(u32*)((u8*)r30 + 0x8);
    goto L_8019BDF8;
L_8019BDF4: ;
    r31 = 0x0;
L_8019BDF8: ;
    if ((u32)r30 == (u32)0x0) goto L_8019BFB0;
    r0 = r31 & 0x7;
    if ((u32)r30 == (u32)0x0) goto L_8019BFB0;
    r3 = r1 + 0x30;
    fn_800BD768();
    r0 = r31 & 0x1;
    if ((u32)r30 == (u32)0x0) goto L_8019BE74;
    f4 = *(f32*)(sp + 0x30);
    f3 = *(f32*)(sp + 0x38);
    if ((u32)r30 == (u32)0x0) goto L_8019BE30;
    r3 = *(s16*)((u8*)r30 + 0xC);
    goto L_8019BE34;
L_8019BE30: ;
    r3 = -0x1;
L_8019BE34: ;
    r3 = r3 + 0x140;
    r0 = (0x4330 << 16);
    /* xoris r3, r3, 0x8000 */;
    *(u32*)(sp + 0xA0) = r0;
    f2 = *(f64*)lbl_8047DA90;
    f0 = *(f32*)lbl_8047DA84;
    f1 = *(f64*)(sp + 0xA0);
    f1 = f1 - f2;
    f1 = f3 * f1;
    f0 = f1 / f0;
    f0 = f4 + f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0xA8) = f0;
    r29 = *(u32*)(sp + 0xAC);
    goto L_8019BE90;
L_8019BE74: ;
    f2 = *(f32*)(sp + 0x38);
    f1 = *(f32*)lbl_8047DA88;
    f0 = *(f32*)(sp + 0x30);
    f0 = f2 * f1 + f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0xB0) = f0;
    r29 = *(u32*)(sp + 0xB4);
L_8019BE90: ;
    r0 = r31 & 0x00000002;
    if ((u32)r30 == (u32)0x0) goto L_8019BEB0;
    if ((u32)r30 == (u32)0x0) goto L_8019BEA8;
    r28 = *(u16*)((u8*)r30 + 0xE);
    goto L_8019BEC0;
L_8019BEA8: ;
    r28 = -0x1;
    goto L_8019BEC0;
L_8019BEB0: ;
    f0 = *(f32*)(sp + 0x38);
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0xB8) = f0;
    r28 = *(u32*)(sp + 0xBC);
L_8019BEC0: ;
    r0 = r31 & 0x00000004;
    if ((u32)r30 == (u32)0x0) goto L_8019BEE0;
    if ((u32)r30 == (u32)0x0) goto L_8019BED8;
    r30 = r30 + 0x10;
    goto L_8019BF8C;
L_8019BED8: ;
    r30 = 0x0;
    goto L_8019BF8C;
L_8019BEE0: ;
    r30 = r1 + 0x5c;
    r3 = r1 + 0x5c;
    r4 = 0x0;
    r5 = 0x40;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = r1 + 0x14;
    fn_800BD454();
    f0 = *(f32*)(sp + 0x14);
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0xB8) = f0;
    r0 = *(u32*)(sp + 0xBC);
    if ((s32)r0 == (s32)0x0) goto L_8019BF18;
    goto L_8019BF54;
L_8019BF18: ;
    f6 = *(f32*)(sp + 0x18);
    f5 = *(f32*)(sp + 0x1C);
    f4 = *(f32*)(sp + 0x20);
    f3 = *(f32*)(sp + 0x24);
    f2 = *(f32*)(sp + 0x28);
    f1 = *(f32*)(sp + 0x2C);
    f0 = *(f32*)lbl_8047DA8C;
    *(f32*)(sp + 0x5C) = f6;
    *(f32*)(sp + 0x64) = f5;
    *(f32*)(sp + 0x70) = f4;
    *(f32*)(sp + 0x74) = f3;
    *(f32*)(sp + 0x84) = f2;
    *(f32*)(sp + 0x88) = f1;
    *(f32*)(sp + 0x94) = f0;
    goto L_8019BF8C;
L_8019BF54: ;
    f6 = *(f32*)(sp + 0x18);
    f5 = *(f32*)(sp + 0x1C);
    f4 = *(f32*)(sp + 0x20);
    f3 = *(f32*)(sp + 0x24);
    f2 = *(f32*)(sp + 0x28);
    f1 = *(f32*)(sp + 0x2C);
    f0 = *(f32*)lbl_8047DA6C;
    *(f32*)(sp + 0x5C) = f6;
    *(f32*)(sp + 0x68) = f5;
    *(f32*)(sp + 0x70) = f4;
    *(f32*)(sp + 0x78) = f3;
    *(f32*)(sp + 0x84) = f2;
    *(f32*)(sp + 0x88) = f1;
    *(f32*)(sp + 0x98) = f0;
L_8019BF8C: ;
    r5 = r30;
    r3 = r1 + 0x48;
    r4 = r28 & 0xFFFF;
    fn_800BCB14();
    r4 = r29 & 0xFFFF;
    r5 = r1 + 0x48;
    r3 = 0x1;
    fn_800BCCDC();
    goto L_8019BFC0;
L_8019BFB0: ;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x0;
    fn_800BCCDC();
L_8019BFC0: ;
    /* psq_l f31, 0xd8(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0xD0);
    r31 = *(u32*)(sp + 0xCC);
    r30 = *(u32*)(sp + 0xC8);
    r29 = *(u32*)(sp + 0xC4);
    r28 = *(u32*)(sp + 0xC0);
    return;
}
#pragma pop

/* 0x8019BFE8 | 0x110 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019BFE8(void) {
    extern u8 lbl_80274800[];
    extern u8 lbl_8047DA98[];
    extern void fn_80196E10();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r31 = r5;
    r29 = r4;
    r28 = r3;
    r6 = *(u32*)((u8*)r3 + 0x0);
    r12 = *(u32*)((u8*)r6 + 0x3C);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r0 = *(u32*)((u8*)r28 + 0x8);
    r30 = r3;
    if ((u32)r30 < (u32)r0) goto L_8019C044;
    r4 = (u32)lbl_80274800;
    r3 = (u32)lbl_8047DA98;
    r5 = (u32)lbl_80274800;
    r4 = 0x71;
    fn_80196E10();
L_8019C044: ;
    r3 = *(u32*)((u8*)r28 + 0x4);
    r0 = r30 << 2;
    r0 = *(u32*)(r3 + r0);
    if ((u32)r0 != (u32)0x0) goto L_8019C060;
    r30 = 0x0;
    goto L_8019C0A8;
L_8019C060: ;
    r3 = *(u32*)((u8*)r28 + 0x4);
    r0 = r30 << 2;
    r30 = *(u32*)(r3 + r0);
    goto L_8019C09C;
L_8019C070: ;
    r6 = *(u32*)((u8*)r28 + 0x0);
    r3 = r28;
    r5 = r29;
    r4 = *(u32*)((u8*)r30 + 0x4);
    r12 = *(u32*)((u8*)r6 + 0x40);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    if ((s32)r3 != (s32)0x0) goto L_8019C098;
    goto L_8019C0A8;
L_8019C098: ;
    r30 = *(u32*)((u8*)r30 + 0x0);
L_8019C09C: ;
    if ((u32)r30 != (u32)0x0) goto L_8019C070;
    r30 = 0x0;
L_8019C0A8: ;
    r3 = r30;
    if ((u32)r31 == (u32)0x0) goto L_8019C0C4;
    r0 = -r3;
    r0 = r0 | r3;
    r0 = (u32)r0 >> 31;
    *(u32*)((u8*)r31 + 0x0) = r0;
L_8019C0C4: ;
    if ((u32)r3 == (u32)0x0) goto L_8019C0D4;
    r3 = *(u32*)((u8*)r3 + 0x8);
    goto L_8019C0D8;
L_8019C0D4: ;
    r3 = 0x0;
L_8019C0D8: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x8019C0F8 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019C0F8(void) {
    extern u8 lbl_804653A8[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r3 = (u32)lbl_804653A8;
    r4 = 0x0;
    r3 = (u32)lbl_804653A8;
    r5 = 0x194;
    memset((void*)r3, (int)r4, (u32)r5);
    return;
}
#pragma pop

/* 0x8019C128 | 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019C128(void) {
    extern u8 lbl_804653A8[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    if ((u32)r3 != (u32)0x0) goto L_8019C138;
    r3 = (u32)lbl_804653A8;
    r3 = (u32)lbl_804653A8;
L_8019C138: ;
    r6 = (0x4470 << 16);
    /* subi r6, r6, 0x79a9 */;
    r6 = (u32)((u64)r6 * (u64)r4 >> 32);
    r0 = r4 - r6;
    r0 = (u32)r0 >> 1;
    r0 = r0 + r6;
    r0 = (u32)r0 >> 6;
    r0 = r0 * 0x65;
    r0 = r4 - r0;
    r0 = r0 << 2;
    r3 = *(u32*)(r3 + r0);
    goto L_8019C190;
L_8019C168: ;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((u32)r0 != (u32)r4) goto L_8019C18C;
    if ((u32)r5 == (u32)0x0) goto L_8019C184;
    r0 = 0x1;
    *(u32*)((u8*)r5 + 0x0) = r0;
L_8019C184: ;
    r3 = *(u32*)((u8*)r3 + 0x8);
    return;
L_8019C18C: ;
    r3 = *(u32*)((u8*)r3 + 0x0);
L_8019C190: ;
    if ((u32)r3 != (u32)0x0) goto L_8019C168;
    if ((u32)r5 == (u32)0x0) goto L_8019C1A8;
    r0 = 0x0;
    *(u32*)((u8*)r5 + 0x0) = r0;
L_8019C1A8: ;
    r3 = 0x0;
    return;
}
#pragma pop

/* 0x8019C1B0 | 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019C1B0(void) {
    extern u8 lbl_804653A8[];
    extern u8 lbl_8046553C[];
    extern void fn_801AA498();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    if ((u32)r3 != (u32)0x0) goto L_8019C1CC;
    r3 = (u32)lbl_804653A8;
    r3 = (u32)lbl_804653A8;
L_8019C1CC: ;
    r5 = (0x4470 << 16);
    r6 = 0x0;
    /* subi r5, r5, 0x79a9 */;
    r5 = (u32)((u64)r5 * (u64)r4 >> 32);
    r0 = r4 - r5;
    r0 = (u32)r0 >> 1;
    r0 = r0 + r5;
    r0 = (u32)r0 >> 6;
    r0 = r0 * 0x65;
    r0 = r4 - r0;
    r7 = r0;
    r0 = r0 << 2;
    r5 = *(u32*)(r3 + r0);
    goto L_8019C24C;
L_8019C204: ;
    r0 = *(u32*)((u8*)r5 + 0x4);
    if ((u32)r0 != (u32)r4) goto L_8019C244;
    if ((u32)r6 == (u32)0x0) goto L_8019C224;
    r0 = *(u32*)((u8*)r5 + 0x0);
    *(u32*)((u8*)r6 + 0x0) = r0;
    goto L_8019C230;
L_8019C224: ;
    r4 = *(u32*)((u8*)r5 + 0x0);
    r0 = r7 << 2;
    *(u32*)(r3 + r0) = r4;
L_8019C230: ;
    r3 = (u32)lbl_8046553C;
    r4 = r5;
    r3 = (u32)lbl_8046553C;
    fn_801AA498();
    goto L_8019C254;
L_8019C244: ;
    r6 = r5;
    r5 = *(u32*)((u8*)r5 + 0x0);
L_8019C24C: ;
    if ((u32)r5 != (u32)0x0) goto L_8019C204;
L_8019C254: ;
    return;
}
#pragma pop

/* 0x8019C264 | 0xF4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019C264(void) {
    extern u8 lbl_804653A8[];
    extern u8 lbl_8046553C[];
    extern u8 lbl_8047DAA0[];
    extern u8 lbl_8047DAA8[];
    extern void fn_80196E10();
    extern void fn_801AA4CC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    /* mr. r29, r3 */;
    r30 = r4;
    r31 = r5;
    if ((s32)r0 != (s32)0) goto L_8019C290;
    r3 = (u32)lbl_804653A8;
    r0 = (u32)lbl_804653A8;
    r29 = r0;
L_8019C290: ;
    r3 = (0x4470 << 16);
    /* subi r3, r3, 0x79a9 */;
    r3 = (u32)((u64)r3 * (u64)r30 >> 32);
    r0 = r30 - r3;
    r0 = (u32)r0 >> 1;
    r0 = r0 + r3;
    r0 = (u32)r0 >> 6;
    r0 = r0 * 0x65;
    r0 = r30 - r0;
    r27 = r0;
    r0 = r0 << 2;
    r4 = *(u32*)(r29 + r0);
    goto L_8019C2D4;
L_8019C2C4: ;
    r0 = *(u32*)((u8*)r4 + 0x4);
    if ((u32)r0 == (u32)r30) goto L_8019C2DC;
    r4 = *(u32*)((u8*)r4 + 0x0);
L_8019C2D4: ;
    if ((u32)r4 != (u32)0x0) goto L_8019C2C4;
L_8019C2DC: ;
    if ((u32)r4 == (u32)0x0) goto L_8019C2F0;
    *(u32*)((u8*)r4 + 0x4) = r30;
    *(u32*)((u8*)r4 + 0x8) = r31;
    goto L_8019C344;
L_8019C2F0: ;
    r3 = (u32)lbl_8046553C;
    r3 = (u32)lbl_8046553C;
    fn_801AA4CC();
    /* mr. r28, r3 */;
    if ((u32)r4 != (u32)0x0) goto L_8019C314;
    r3 = (u32)lbl_8047DAA0;
    r4 = 0x43;
    r5 = (u32)lbl_8047DAA8;
    fn_80196E10();
L_8019C314: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xc;
    memset((void*)r3, (int)r4, (u32)r5);
    *(u32*)((u8*)r28 + 0x4) = r30;
    r3 = r27 << 2;
    r0 = r27 << 2;
    r4 = r28;
    *(u32*)((u8*)r28 + 0x8) = r31;
    r3 = *(u32*)(r29 + r3);
    *(u32*)((u8*)r28 + 0x0) = r3;
    *(u32*)(r29 + r0) = r4;
L_8019C344: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x8019C358 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019C358(void) {
    extern u8 lbl_804653A8[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r3 = (u32)lbl_804653A8;
    r4 = 0x0;
    r3 = (u32)lbl_804653A8;
    r5 = 0x194;
    memset((void*)r3, (int)r4, (u32)r5);
    return;
}
#pragma pop

/* 0x8019C388 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019C388(void) {
    extern u8 lbl_8046553C[];
    extern void fn_801AA35C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r3 = (u32)lbl_8046553C;
    r4 = 0xc;
    r3 = (u32)lbl_8046553C;
    r5 = 0x4;
    fn_801AA35C();
    return;
}
#pragma pop

/* 0x8019C3B8 | 0xC */
void fn_8019C3B8(void) {
}
