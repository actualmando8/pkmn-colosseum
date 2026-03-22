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
void fn_8019B490(void) {
    extern void fn_80199AF8();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f31 = 0.0f;

    f31 = f1;
    r31 = r5;
    r30 = r4;
    r29 = r3;
    while (r29 != 0) {

        if (r29 != 0) {
            tmp = *(u8*)((u8*)r29 + 0x12);
            if (tmp == 6) {
                f1 = f31;
                r3 = r29;
                r4 = r30;
                r5 = r31;
                fn_80199AF8();
            }
            if (r29 != 0) {
                tmp = *(u8*)((u8*)r29 + 0x10);
                tmp = tmp & 0x000000F0;
                *(u8*)((u8*)r29 + 0x10) = tmp;
        }
        }
        r29 = *(u32*)((u8*)r29 + 0x0);

    }
    return;
}

/* 0x8019B528 | 0xC0 */
void fn_8019B528(void) {
    u8 sp[0x20];
    extern u8 lbl_8047DA3C[];
    extern u8 lbl_8047DA58[];
    u32 tmp = 0;
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

    if (r3 != 0) {
        while (r3 != 0) {

            if (r3 != 0) {
                r4 = *(u32*)((u8*)r3 + 0x8);
                tmp = 0x43300000;
                r6 = 0x0;
                f6 = *(f64*)lbl_8047DA58;
                r5 = 0x0;
                *(u32*)((u8*)r3 + 0x4) = r4;
                r4 = 0x0;
                tmp = 0x0;
                f4 = *(f32*)lbl_8047DA3C;
                r7 = *(s16*)((u8*)r3 + 0x18);
                f3 = *(f32*)lbl_8047DA3C;
                f2 = *(f32*)lbl_8047DA3C;
                f0 = *(f32*)lbl_8047DA3C;
                f5 = *(f64*)((u8*)(u32)sp + 0x8);
                f5 = f5 - f6;
                f5 = f5 + f1;
                *(f32*)((u8*)r3 + 0x1C) = f5;
                *(u8*)((u8*)r3 + 0x11) = r6;
                *(u8*)((u8*)r3 + 0x12) = r5;
                r5 = *(u8*)((u8*)r3 + 0x10);
                r5 = r5 & 0xFFFFFFBF;
                *(u8*)((u8*)r3 + 0x10) = r5;
                *(u16*)((u8*)r3 + 0x16) = r4;
                *(u16*)((u8*)r3 + 0x1A) = tmp;
                *(f32*)((u8*)r3 + 0x20) = f4;
                *(f32*)((u8*)r3 + 0x24) = f3;
                *(f32*)((u8*)r3 + 0x28) = f2;
                *(f32*)((u8*)r3 + 0x2C) = f0;
                if (r3 != 0) {
                    tmp = *(u8*)((u8*)r3 + 0x10);
                    tmp = tmp & 0x000000F0;
                    tmp = tmp | 0x1;
                    *(u8*)((u8*)r3 + 0x10) = tmp;
            }
            }
            r3 = *(u32*)((u8*)r3 + 0x0);

        }
    }
    return;
}

/* 0x8019B5E8 | 0x168 */
void fn_8019B5E8(void) {
    extern u8 lbl_802747AC[];
    extern u8 lbl_80465378[];
    extern void fn_8019970C();
    extern void fn_80199A84();
    extern void fn_8019B5E8();
    extern void fn_8019B750();
    extern void fn_801AA498();
    u8 sp[0x30];
    u32 tmp = 0;
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

    /* mr. r31, r3 */;
    if ((s32)tmp == 0) return;
    r30 = *(u32*)((u8*)r31 + 0x0);
    if (r30 != 0) {
        r29 = *(u32*)((u8*)r30 + 0x0);
        if (r29 != 0) {
            r28 = *(u32*)((u8*)r29 + 0x0);
            if (r28 != 0) {
                r27 = *(u32*)((u8*)r28 + 0x0);
                if (r27 != 0) {
                    r26 = *(u32*)((u8*)r27 + 0x0);
                    if (r26 != 0) {
                        r25 = *(u32*)((u8*)r26 + 0x0);
                        if (r25 != 0) {
                            r24 = *(u32*)((u8*)r25 + 0x0);
                            if (r24 != 0) {
                                r23 = *(u32*)((u8*)r24 + 0x0);
                                if (r23 != 0) {
                                    r3 = *(u32*)((u8*)r23 + 0x0);
                                    fn_8019B5E8();
                                    r3 = r23;
                                    fn_8019B750();
                                }
                                if (r24 != 0) {
                                    r3 = r24;
                                    fn_8019970C();
                                }
                            }
                            if (r25 != 0) {
                                r3 = (u32)lbl_80465378;
                                r4 = r25;
                                r3 = (u32)lbl_80465378;
                                fn_801AA498();
                                r3 = (u32)lbl_80465378;
                                r4 = (u32)lbl_802747AC;
                                r3 = (u32)lbl_80465378;
                                r4 = (u32)lbl_802747AC;
                                fn_80199A84();
                            }
                        }
                        r3 = (u32)lbl_80465378;
                        r4 = r26;
                        r3 = (u32)lbl_80465378;
                        fn_801AA498();
                    }
                    r3 = (u32)lbl_80465378;
                    r4 = r27;
                    r3 = (u32)lbl_80465378;
                    fn_801AA498();
                }
                r3 = (u32)lbl_80465378;
                r4 = r28;
                r3 = (u32)lbl_80465378;
                fn_801AA498();
            }
            r3 = (u32)lbl_80465378;
            r4 = r29;
            r3 = (u32)lbl_80465378;
            fn_801AA498();
        }
        r3 = (u32)lbl_80465378;
        r4 = r30;
        r3 = (u32)lbl_80465378;
        fn_801AA498();
    }
    if (r31 == 0) return;
    r3 = (u32)lbl_80465378;
    r4 = r31;
    r3 = (u32)lbl_80465378;
    fn_801AA498();

    return;
}

/* 0x8019B750 | 0x34 */
/* hsdFogRegister - Add a fog object to the fog list if non-NULL. */
void fn_8019B750(void* fogObj) {
    extern u8 lbl_80465378[];
    extern void fn_801AA498(u8* list, void* obj);

    if (fogObj != NULL) {
        fn_801AA498(lbl_80465378, fogObj);
    }
}

/* 0x8019B784 | 0x30 */
void fn_8019B784(void) {
    extern u8 lbl_80465378[];
    extern void fn_801AA35C();
    u8 sp[0x10];
    u32 tmp = 0;
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
void fn_8019B874(void) {
    extern u8 lbl_8036C7E8[];
    extern void fn_801C25E4();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r30 = r3;
    r31 = *(u32*)((u8*)r3 + 0xC);
    if (r31 != 0) {
        r3 = 0x10000;
        r4 = *(u16*)((u8*)r31 + 0x4);
        tmp = r3 & 0xFFFF;
        tmp = tmp - r4;
        tmp = __cntlzw(tmp);
        /* srwi. r3, tmp, 5 */;
        if (r31 != 0) {
        } else {

            tmp = *(u16*)((u8*)r31 + 0x4);
            r3 = *(u16*)((u8*)r31 + 0x4);
            r4 = __cntlzw(tmp);
            *(u16*)((u8*)r31 + 0x4) = tmp;
            r3 = (u32)r4 >> 5;
        }
        if ((s32)r3 != 0) {
            if (r31 != 0) {
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
    }
    }
    }
    r3 = *(u32*)((u8*)r30 + 0x1C);
    fn_801C25E4();
    r4 = (u32)lbl_8036C7E8;
    r3 = r30;
    r4 = (u32)lbl_8036C7E8;
    r4 = *(u32*)((u8*)r4 + 0x14);
    r12 = *(u32*)((u8*)r4 + 0x30);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    return;
}

/* 0x8019B948 | 0x230 */
void fn_8019B948(void) {
    extern u8 lbl_8047DA68[];
    extern u8 lbl_8047DA6C[];
    extern u8 lbl_8047DA70[];
    extern u8 jumptable_8036C864[];
    u8 sp[0x100];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    void (*ctr_fn)(void) = 0;

    if (r3 == 0) return;
    if (r4 > 0x15) return;
    r6 = (u32)jumptable_8036C864;
    tmp = r4 << 2;
    r4 = (u32)jumptable_8036C864;
    r4 = *(u32*)(r4 + tmp);
    ctr_fn = (void(*)(void))r4;
    f0 = *(f32*)((u8*)r5 + 0x0);
    *(f32*)((u8*)r3 + 0x10) = f0;
    return;
    f0 = *(f32*)((u8*)r5 + 0x0);
    *(f32*)((u8*)r3 + 0x14) = f0;
    return;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DA68;
    /* cror eq, lt, eq */;
    if (f1 == f0) {
        f1 = *(f32*)lbl_8047DA68;
    } else {
        f0 = *(f32*)lbl_8047DA6C;
        /* cror eq, gt, eq */;
        if (f1 == f0) {
            f1 = *(f32*)lbl_8047DA6C;
        }
    }
    f0 = *(f32*)lbl_8047DA70;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(u8*)((u8*)r3 + 0x18) = tmp;
    return;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DA68;
    /* cror eq, lt, eq */;
    if (f1 == f0) {
        f1 = *(f32*)lbl_8047DA68;
    } else {
        f0 = *(f32*)lbl_8047DA6C;
        /* cror eq, gt, eq */;
        if (f1 == f0) {
            f1 = *(f32*)lbl_8047DA6C;
        }
    }
    f0 = *(f32*)lbl_8047DA70;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(u8*)((u8*)r3 + 0x19) = tmp;
    return;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DA68;
    /* cror eq, lt, eq */;
    if (f1 == f0) {
        f1 = *(f32*)lbl_8047DA68;
    } else {
        f0 = *(f32*)lbl_8047DA6C;
        /* cror eq, gt, eq */;
        if (f1 == f0) {
            f1 = *(f32*)lbl_8047DA6C;
        }
    }
    f0 = *(f32*)lbl_8047DA70;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(u8*)((u8*)r3 + 0x1A) = tmp;
    return;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DA68;
    /* cror eq, lt, eq */;
    if (f1 == f0) {
        f1 = *(f32*)lbl_8047DA68;
    } else {
        f0 = *(f32*)lbl_8047DA6C;
        /* cror eq, gt, eq */;
        if (f1 == f0) {
            f1 = *(f32*)lbl_8047DA6C;
        }
    }
    f0 = *(f32*)lbl_8047DA70;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(u8*)((u8*)r3 + 0x1B) = tmp;
    return;
    f0 = *(f32*)((u8*)r5 + 0x0);
    f0 = (f64)(s32)f0;
    if (r3 != 0) {
        r3 = *(u32*)((u8*)r3 + 0xC);
    } else {

        r3 = 0x0;
    }
    if (r3 == 0) return;
    if ((s32)tmp <= (s32)-0x140) {
        tmp = -0x140;
        *(u16*)((u8*)r3 + 0xC) = tmp;
        return;
    }
    if ((s32)tmp >= 0x140) {
        tmp = 0x140;
        *(u16*)((u8*)r3 + 0xC) = tmp;
        return;
    }
    *(u16*)((u8*)r3 + 0xC) = tmp;
    return;
    f0 = *(f32*)((u8*)r5 + 0x0);
    f0 = (f64)(s32)f0;
    if (r3 != 0) {
        r3 = *(u32*)((u8*)r3 + 0xC);
    } else {

        r3 = 0x0;
    }
    if (r3 == 0) return;
    if ((s32)tmp <= 0) {
        tmp = 0x0;
        *(u16*)((u8*)r3 + 0xE) = tmp;
        return;
    }
    if ((s32)tmp >= 0x280) {
        tmp = 0x280;
        *(u16*)((u8*)r3 + 0xE) = tmp;
        return;
    }
    *(u16*)((u8*)r3 + 0xE) = tmp;

    return;
}

/* 0x8019BB78 | 0x1A0 */
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
    u32 tmp = 0;
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
    if ((s32)tmp == 0) {
        r3 = (u32)lbl_8047DA74;
        r4 = 0xa1;
        r5 = (u32)lbl_8047DA80;
        fn_80196E10();
    }
    /* mr. r31, r29 */;
    if ((s32)tmp == 0) {
        r3 = (u32)lbl_8047DA74;
        r4 = 0xae;
        r5 = (u32)lbl_8047DA80;
        fn_80196E10();
    }
    if (r31 != 0) {
        if (r30 != 0) {
            tmp = *(u32*)((u8*)r30 + 0x0);
            *(u32*)((u8*)r31 + 0x8) = tmp;
            f0 = *(f32*)((u8*)r30 + 0x8);
            *(f32*)((u8*)r31 + 0x10) = f0;
            f0 = *(f32*)((u8*)r30 + 0xC);
            *(f32*)((u8*)r31 + 0x14) = f0;
            tmp = *(u32*)((u8*)r30 + 0x10);
            *(u32*)((u8*)r31 + 0x18) = tmp;
        } else {
            r3 = (u32)sp + 0x8;
            fn_800BD768();
            tmp = 0x2;
            r5 = 0xff;
            *(u32*)((u8*)r31 + 0x8) = tmp;
            r4 = 0xff;
            r3 = 0xff;
            tmp = 0xff;
            f0 = *(f32*)(sp + 0x18);
            *(f32*)((u8*)r31 + 0x10) = f0;
            f0 = *(f32*)(sp + 0x1C);
            *(f32*)((u8*)r31 + 0x14) = f0;
            *(u8*)((u8*)r31 + 0x18) = r5;
            *(u8*)((u8*)r31 + 0x19) = r4;
            *(u8*)((u8*)r31 + 0x1A) = r3;
            *(u8*)((u8*)r31 + 0x1B) = tmp;
        }
    }
    tmp = *(u32*)((u8*)r30 + 0x4);
    if (tmp == 0) { r3 = r31; return; }
    r3 = (u32)lbl_8036C828;
    r30 = *(u32*)((u8*)r30 + 0x4);
    r3 = (u32)lbl_8036C828;
    fn_80193828();
    /* mr. r29, r3 */;
    if (tmp == 0) {
        r3 = (u32)lbl_8047DA74;
        r4 = 0xf9;
        r5 = (u32)lbl_8047DA7C;
        fn_80196E10();
    }
    if (r29 == 0) {
        r3 = (u32)lbl_8047DA74;
        r4 = 0x109;
        r5 = (u32)lbl_8047DA7C;
        fn_80196E10();
    }
    if (r29 != 0) {
        if (r30 != 0) {
            tmp = *(u32*)((u8*)r30 + 0x0);
            r3 = r29 + 0x10;
            r4 = r30 + 0x8;
            r5 = 0x40;
            *(u32*)((u8*)r29 + 0x8) = tmp;
            tmp = *(u16*)((u8*)r30 + 0x6);
            *(u16*)((u8*)r29 + 0xE) = tmp;
            tmp = *(u16*)((u8*)r30 + 0x4);
            *(u16*)((u8*)r29 + 0xC) = tmp;
            memcpy((void*)r3, (const void*)r4, (u32)r5);
        } else {
            tmp = 0x0;
            r5 = 0x0;
            *(u32*)((u8*)r29 + 0x8) = tmp;
            tmp = 0x0;
            r3 = r29 + 0x10;
            r4 = 0x0;
            *(u16*)((u8*)r29 + 0xE) = r5;
            r5 = 0x40;
            *(u16*)((u8*)r29 + 0xC) = tmp;
            memset((void*)r3, (int)r4, (u32)r5);
        }
    }
    *(u32*)((u8*)r31 + 0xC) = r29;

    r3 = r31;
    return;
}

/* 0x8019BD18 | 0x2D0 */
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
    u32 tmp = 0;
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

    /* mr. r28, r3 */;
    if ((s32)tmp == 0) {
        tmp = *(u32*)lbl_8047E720;
        r4 = (u32)sp + 0xc;
        f1 = *(f32*)lbl_8047DA68;
        r3 = 0x0;
        *(u32*)(sp + 0x10) = tmp;
        f2 = *(f32*)lbl_8047DA68;
        f3 = *(f32*)lbl_8047DA68;
        *(u32*)(sp + 0xC) = tmp;
        f4 = *(f32*)lbl_8047DA68;
        fn_800BC8F8();
        return;
    }
    fn_801942B8();
    /* mr. r30, r3 */;
    if ((s32)tmp == 0) {
        r4 = (u32)lbl_802747DC;
        r3 = (u32)lbl_8047DA74;
        r5 = (u32)lbl_802747DC;
        r4 = 0x58;
        fn_80196D78();
    }
    tmp = *(u32*)((u8*)r28 + 0x18);
    r3 = r30;
    r31 = (u32)sp + 0x8;
    *(u32*)(sp + 0x8) = tmp;
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
    if (r28 != 0) {
        r30 = *(u32*)((u8*)r28 + 0xC);
    } else {

        r30 = 0x0;
    }
    if (r30 != 0) {
        r31 = *(u32*)((u8*)r30 + 0x8);
    } else {

        r31 = 0x0;
    }
    if (r30 == 0 || (tmp = r31 & 0x7, r30 == 0)) {
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x0;
        fn_800BCCDC();
        return;
    }
    r3 = (u32)sp + 0x30;
    fn_800BD768();
    tmp = r31 & 0x1;
    if (r30 != 0) {
        f4 = *(f32*)(sp + 0x30);
        f3 = *(f32*)(sp + 0x38);
        if (r30 != 0) {
            r3 = *(s16*)((u8*)r30 + 0xC);
        } else {

            r3 = -0x1;
        }
        r3 = r3 + 0x140;
        tmp = 0x43300000;
        *(u32*)(sp + 0xA0) = tmp;
        f2 = *(f64*)lbl_8047DA90;
        f0 = *(f32*)lbl_8047DA84;
        f1 = f1 - f2;
        f1 = f3 * f1;
        f0 = f1 / f0;
        f0 = f4 + f0;
        f0 = (f64)(s32)f0;
    } else {

        f2 = *(f32*)(sp + 0x38);
        f1 = *(f32*)lbl_8047DA88;
        f0 = *(f32*)(sp + 0x30);
        f0 = f2 * f1 + f0;
        f0 = (f64)(s32)f0;
    }
    tmp = r31 & 0x00000002;
    if (r30 != 0) {
        if (r30 != 0) {
            r28 = *(u16*)((u8*)r30 + 0xE);
        } else {
            r28 = -0x1;
        }
    } else {
        f0 = *(f32*)(sp + 0x38);
        f0 = (f64)(s32)f0;
    }
    tmp = r31 & 0x00000004;
    if (r30 != 0) {
        if (r30 != 0) {
            r30 = r30 + 0x10;
        } else {
            r30 = 0x0;
        }
    } else {
        r30 = (u32)sp + 0x5c;
        r3 = (u32)sp + 0x5c;
        r4 = 0x0;
        r5 = 0x40;
        memset((void*)r3, (int)r4, (u32)r5);
        r3 = (u32)sp + 0x14;
        fn_800BD454();
        f0 = *(f32*)(sp + 0x14);
        f0 = (f64)(s32)f0;
        if ((s32)tmp == 0) {
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
        } else {
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
        }
    }
    r5 = r30;
    r3 = (u32)sp + 0x48;
    r4 = r28 & 0xFFFF;
    fn_800BCB14();
    r4 = r29 & 0xFFFF;
    r5 = (u32)sp + 0x48;
    r3 = 0x1;
    fn_800BCCDC();

    return;
}

/* 0x8019BFE8 | 0x110 */
void fn_8019BFE8(void) {
    extern u8 lbl_80274800[];
    extern u8 lbl_8047DA98[];
    extern void fn_80196E10();
    u8 sp[0x20];
    u32 tmp = 0;
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
    tmp = *(u32*)((u8*)r28 + 0x8);
    r30 = r3;
    if (r30 >= tmp) {
        r4 = (u32)lbl_80274800;
        r3 = (u32)lbl_8047DA98;
        r5 = (u32)lbl_80274800;
        r4 = 0x71;
        fn_80196E10();
    }
    r3 = *(u32*)((u8*)r28 + 0x4);
    tmp = r30 << 2;
    tmp = *(u32*)(r3 + tmp);
    if (tmp == 0) {
        r30 = 0x0;
    } else {
        r3 = *(u32*)((u8*)r28 + 0x4);
        tmp = r30 << 2;
        r30 = *(u32*)(r3 + tmp);
        while (r30 != 0) {
            r6 = *(u32*)((u8*)r28 + 0x0);
            r3 = r28;
            r5 = r29;
            r4 = *(u32*)((u8*)r30 + 0x4);
            r12 = *(u32*)((u8*)r6 + 0x40);
            ctr_fn = (void(*)(void))r12;
            ctr_fn();
            if ((s32)r3 == 0) {
                r30 = 0x0;
                break;
            }
            r30 = *(u32*)((u8*)r30 + 0x0);
        }
        if (r30 != 0) {
            /* loop ended without break - fell through */
        } else {
            r30 = 0x0;
        }
    }
    r3 = r30;
    if (r31 != 0) {
        tmp = -r3;
        tmp = tmp | r3;
        tmp = (u32)tmp >> 31;
        *(u32*)((u8*)r31 + 0x0) = tmp;
    }
    if (r3 != 0) {
        r3 = *(u32*)((u8*)r3 + 0x8);
    } else {

        r3 = 0x0;
    }
    return;
}

/* 0x8019C0F8 | 0x30 */
void fn_8019C0F8(void) {
    extern u8 lbl_804653A8[];
    u8 sp[0x10];
    u32 tmp = 0;
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

/* 0x8019C128 | 0x88 */
void fn_8019C128(void) {
    extern u8 lbl_804653A8[];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    if (r3 == 0) {
        r3 = (u32)lbl_804653A8;
        r3 = (u32)lbl_804653A8;
    }
    r6 = 0x44700000;
    r6 = (u32)((u64)r6 * (u64)r4 >> 32);
    tmp = r4 - r6;
    tmp = (u32)tmp >> 1;
    tmp = tmp + r6;
    tmp = (u32)tmp >> 6;
    tmp = tmp * 0x65;
    tmp = r4 - tmp;
    tmp = tmp << 2;
    r3 = *(u32*)(r3 + tmp);
    while (r3 != 0) {

        tmp = *(u32*)((u8*)r3 + 0x4);
        if (tmp == r4) {
            if (r5 != 0) {
                tmp = 0x1;
                *(u32*)((u8*)r5 + 0x0) = tmp;
            }
            r3 = *(u32*)((u8*)r3 + 0x8);
            return;
        }
        r3 = *(u32*)((u8*)r3 + 0x0);

    }
    if (r5 != 0) {
        tmp = 0x0;
        *(u32*)((u8*)r5 + 0x0) = tmp;
    }
    r3 = 0x0;
    return;
}

/* 0x8019C1B0 | 0xB4 */
void fn_8019C1B0(void) {
    extern u8 lbl_804653A8[];
    extern u8 lbl_8046553C[];
    extern void fn_801AA498();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    if (r3 == 0) {
        r3 = (u32)lbl_804653A8;
        r3 = (u32)lbl_804653A8;
    }
    r5 = 0x44700000;
    r6 = 0x0;
    r5 = (u32)((u64)r5 * (u64)r4 >> 32);
    tmp = r4 - r5;
    tmp = (u32)tmp >> 1;
    tmp = tmp + r5;
    tmp = (u32)tmp >> 6;
    tmp = tmp * 0x65;
    tmp = r4 - tmp;
    r7 = tmp;
    tmp = tmp << 2;
    r5 = *(u32*)(r3 + tmp);
    while (r5 != 0) {

        tmp = *(u32*)((u8*)r5 + 0x4);
        if (tmp == r4) {
            if (r6 != 0) {
                tmp = *(u32*)((u8*)r5 + 0x0);
                *(u32*)((u8*)r6 + 0x0) = tmp;
            } else {

                r4 = *(u32*)((u8*)r5 + 0x0);
                tmp = r7 << 2;
                *(u32*)(r3 + tmp) = r4;
            }
            r3 = (u32)lbl_8046553C;
            r4 = r5;
            r3 = (u32)lbl_8046553C;
            fn_801AA498();
            return;
        }
        r6 = r5;
        r5 = *(u32*)((u8*)r5 + 0x0);

    }

    return;
}

/* 0x8019C264 | 0xF4 */
void fn_8019C264(void) {
    extern u8 lbl_804653A8[];
    extern u8 lbl_8046553C[];
    extern u8 lbl_8047DAA0[];
    extern u8 lbl_8047DAA8[];
    extern void fn_80196E10();
    extern void fn_801AA4CC();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* mr. r29, r3 */;
    r30 = r4;
    r31 = r5;
    if ((s32)tmp == 0) {
        r3 = (u32)lbl_804653A8;
        tmp = (u32)lbl_804653A8;
        r29 = tmp;
    }
    r3 = 0x44700000;
    r3 = (u32)((u64)r3 * (u64)r30 >> 32);
    tmp = r30 - r3;
    tmp = (u32)tmp >> 1;
    tmp = tmp + r3;
    tmp = (u32)tmp >> 6;
    tmp = tmp * 0x65;
    tmp = r30 - tmp;
    r27 = tmp;
    tmp = tmp << 2;
    r4 = *(u32*)(r29 + tmp);
    while (r4 != 0) {
        tmp = *(u32*)((u8*)r4 + 0x4);
        if (tmp == r30) break;
        r4 = *(u32*)((u8*)r4 + 0x0);
    }
    if (r4 != 0) {
        *(u32*)((u8*)r4 + 0x4) = r30;
        *(u32*)((u8*)r4 + 0x8) = r31;
    } else {

        r3 = (u32)lbl_8046553C;
        r3 = (u32)lbl_8046553C;
        fn_801AA4CC();
        /* mr. r28, r3 */;
        if (r4 == 0) {
            r3 = (u32)lbl_8047DAA0;
            r4 = 0x43;
            r5 = (u32)lbl_8047DAA8;
            fn_80196E10();
        }
        r3 = r28;
        r4 = 0x0;
        r5 = 0xc;
        memset((void*)r3, (int)r4, (u32)r5);
        *(u32*)((u8*)r28 + 0x4) = r30;
        r3 = r27 << 2;
        tmp = r27 << 2;
        r4 = r28;
        *(u32*)((u8*)r28 + 0x8) = r31;
        r3 = *(u32*)(r29 + r3);
        *(u32*)((u8*)r28 + 0x0) = r3;
        *(u32*)(r29 + tmp) = r4;
    }
    return;
}

/* 0x8019C358 | 0x30 */
void fn_8019C358(void) {
    extern u8 lbl_804653A8[];
    u8 sp[0x10];
    u32 tmp = 0;
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

/* 0x8019C388 | 0x30 */
void fn_8019C388(void) {
    extern u8 lbl_8046553C[];
    extern void fn_801AA35C();
    u8 sp[0x10];
    u32 tmp = 0;
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

/* 0x8019C3B8 | 0xC */
void fn_8019C3B8(void) {
}
