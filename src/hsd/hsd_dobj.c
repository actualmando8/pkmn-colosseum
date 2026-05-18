/**
 * @file hsd_dobj.c
 * @brief HSD DObj - Display object implementation.
 *
 * Colosseum address: 0x80198F7C (HSD_DObjInit)
 * Adapted from doldecomp/melee src/sysdolphin/baselib/dobj.c
 */

#include "hsd/hsd_dobj.h"
#include "hsd/hsd_aobj.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_mobj.h"
#include "hsd/hsd_pobj.h"

static void DObjInfoInit(void);

static HSD_DObjInfo hsdDObj = { DObjInfoInit };
static HSD_ClassInfo* default_class = NULL;

/* ========================================================================= */
/*  Flag accessors                                                           */
/* ========================================================================= */

u32 HSD_DObjGetFlags(HSD_DObj* dobj)
{
    HSD_ASSERT(0, dobj);
    return dobj->flags;
}

void HSD_DObjSetFlags(HSD_DObj* dobj, u32 flags)
{
    HSD_ASSERT(0, dobj);
    dobj->flags |= flags;
}

void HSD_DObjClearFlags(HSD_DObj* dobj, u32 flags)
{
    HSD_ASSERT(0, dobj);
    dobj->flags &= ~flags;
}

/* ========================================================================= */
/*  Animation                                                                */
/* ========================================================================= */

void HSD_DObjAddAnim(HSD_DObj* dobj, HSD_MatAnim* mat_anim,
                     HSD_ShapeAnimDObj* sh_anim)
{
    if (dobj == NULL) {
        return;
    }
    HSD_MObjAddAnim(dobj->mobj, mat_anim);
    if (sh_anim != NULL) {
        HSD_PObjAddAnim(dobj->pobj, sh_anim->shapeanim);
    }
}

void HSD_DObjAddAnimAll(HSD_DObj* dobj, HSD_MatAnim* matanim,
                        HSD_ShapeAnimDObj* shapeanimdobj)
{
    HSD_DObj* d;
    HSD_MatAnim* ma;
    HSD_ShapeAnimDObj* sa;

    d = dobj;
    ma = matanim;
    sa = shapeanimdobj;

    while (d != NULL) {
        HSD_DObjAddAnim(d, ma, sa);
        d = d->next;
        if (ma != NULL) ma = ma->next;
        if (sa != NULL) sa = sa->next;
    }
}

void HSD_DObjReqAnimAll(HSD_DObj* dobj, f32 startframe)
{
    HSD_DObj* d;
    for (d = dobj; d != NULL; d = d->next) {
        HSD_MObjReqAnim(d->mobj, startframe);
        /* PObj shape anim req would go here */
    }
}

void HSD_DObjAnim(HSD_DObj* dobj)
{
    if (dobj != NULL) {
        HSD_MObjAnim(dobj->mobj);
        HSD_PObjAnim(dobj->pobj);
    }
}

void HSD_DObjAnimAll(HSD_DObj* dobj)
{
    HSD_DObj* d;
    for (d = dobj; d != NULL; d = d->next) {
        HSD_DObjAnim(d);
    }
}

/* ========================================================================= */
/*  Load                                                                     */
/* ========================================================================= */

static int DObjLoad(HSD_DObj* dobj, HSD_DObjDesc* desc)
{
    if (dobj->mobj != NULL) {
        HSD_MObjRemove(dobj->mobj);
    }
    dobj->mobj = HSD_MObjLoadDesc(desc->mobjdesc);
    if (dobj->pobj != NULL) {
        HSD_PObjRemoveAll(dobj->pobj);
    }
    dobj->pobj = HSD_PObjLoadDesc(desc->pobjdesc);
    return 0;
}

HSD_DObj* HSD_DObjLoadDesc(HSD_DObjDesc* desc)
{
    HSD_DObj* dobj = NULL;
    HSD_DObj* first = NULL;
    HSD_DObj* prev = NULL;
    HSD_DObjDesc* d;

    for (d = desc; d != NULL; d = d->next) {
        HSD_ClassInfo* info;
        if (d->class_name == NULL ||
            !(info = hsdSearchClassInfo(d->class_name)))
        {
            dobj = HSD_DObjAlloc();
        } else {
            dobj = hsdNew(info);
            HSD_ASSERT(0, dobj);
        }
        HSD_DOBJ_METHOD(dobj)->load(dobj, d);
        if (prev != NULL) {
            prev->next = dobj;
        } else {
            first = dobj;
        }
        prev = dobj;
    }
    return first;
}

/* ========================================================================= */
/*  Resolve refs                                                             */
/* ========================================================================= */

void HSD_DObjResolveRefs(HSD_DObj* dobj, HSD_DObjDesc* desc)
{
    if (dobj != NULL && desc != NULL) {
        HSD_PObjResolveRefsAll(dobj->pobj, desc->pobjdesc);
    }
}

void HSD_DObjResolveRefsAll(HSD_DObj* dobj, HSD_DObjDesc* desc)
{
    HSD_DObj* d;
    HSD_DObjDesc* dd;

    d = dobj;
    dd = desc;
    while (d != NULL && dd != NULL) {
        HSD_DObjResolveRefs(d, dd);
        d = d->next;
        dd = dd->next;
    }
}

/* ========================================================================= */
/*  Remove                                                                   */
/* ========================================================================= */

void HSD_DObjRemove(HSD_DObj* dobj)
{
    if (dobj != NULL) {
        HSD_CLASS_METHOD(dobj)->release((HSD_Class*) dobj);
        HSD_CLASS_METHOD(dobj)->destroy((HSD_Class*) dobj);
    }
}

void HSD_DObjRemoveAll(HSD_DObj* dobj)
{
    HSD_DObj* next;
    while (dobj != NULL) {
        next = dobj->next;
        HSD_DObjRemove(dobj);
        dobj = next;
    }
}

/* ========================================================================= */
/*  Alloc                                                                    */
/* ========================================================================= */

HSD_DObj* HSD_DObjAlloc(void)
{
    HSD_DObj* dobj;
    dobj = (HSD_DObj*) hsdNew(
        default_class ? default_class : (HSD_ClassInfo*) &hsdDObj);
    HSD_ASSERT(0, dobj);
    return dobj;
}

void HSD_DObjSetDefaultClass(HSD_ClassInfo* info)
{
    if (info) {
        HSD_ASSERT(0, hsdIsDescendantOf(info, &hsdDObj));
    }
    default_class = info;
}

/* ========================================================================= */
/*  Class lifecycle                                                          */
/* ========================================================================= */

static void DObjRelease(HSD_Class* o)
{
    HSD_DObj* dobj = (HSD_DObj*) o;
    HSD_MObjRemove(dobj->mobj);
    HSD_PObjRemoveAll(dobj->pobj);
    HSD_AObjRemove(dobj->aobj);
    HSD_PARENT_INFO(&hsdDObj)->release(o);
}

static void DObjAmnesia(HSD_ClassInfo* info)
{
    if (info == HSD_CLASS_INFO(default_class)) {
        default_class = NULL;
    }
    HSD_PARENT_INFO(&hsdDObj)->amnesia(info);
}

static void DObjInfoInit(void)
{
    hsdInitClassInfo((HSD_ClassInfo*) &hsdDObj, &hsdClass,
                     "sysdolphin_base_library", "hsd_dobj",
                     sizeof(HSD_DObjInfo), sizeof(HSD_DObj));
    ((HSD_ClassInfo*) &hsdDObj)->release = DObjRelease;
    ((HSD_ClassInfo*) &hsdDObj)->amnesia = DObjAmnesia;
    hsdDObj.load = DObjLoad;
}

/* ===================================================================
 * WP-0061: asm wrappers
 * Range: 0x80199014 - 0x80199794
 * =================================================================== */

/* 0x80199014 | 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80199014(void) {
#include "src/hsd/hsd_dobj_fn_80199014.inc"
}
#else
#pragma optimization_level 4
static void DObjDestroy(HSD_Class* o)
{
    if (o == (HSD_Class*) default_class) {
        default_class = NULL;
    }
    HSD_PARENT_INFO(&hsdDObj)->destroy(o);
}
#endif
#pragma pop

/* 0x8019905C | 0x5C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801A6D08(HSD_MObj* mobj);
extern void fn_801AD214(HSD_PObj* pobj);
extern void fn_801C25E4(HSD_AObj* aobj);
#if 0
asm void fn_8019905C(void) {
#include "src/hsd/hsd_dobj_fn_8019905C.inc"
}
#else
#pragma optimization_level 4
static void fn_8019905C(HSD_Class* o)
{
    HSD_DObj* dobj = HSD_DOBJ(o);
    fn_801A6D08(dobj->mobj);
    fn_801AD214(dobj->pobj);
    fn_801C25E4(dobj->aobj);
    HSD_PARENT_INFO(&hsdDObj)->init(o);
}
#endif
#pragma pop

/* 0x801990B8 | 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801A8470(HSD_MObj* mobj);
#if 0
asm void fn_801990B8(void) {
#include "src/hsd/hsd_dobj_fn_801990B8.inc"
}
#else
#pragma optimization_level 4
static void DObjDisp(HSD_DObj* dobj, void* vmtx, void* pmtx, u32 rendermode)
{
    HSD_PObj* pobj;
    fn_801A8470(dobj->mobj);
    if (!(rendermode & 0x04000000)) {
        void (**vtbl)(void) = (void (**)(void)) HSD_CLASS_METHOD(dobj->mobj);
        ((void (*)(HSD_MObj*, u32)) vtbl[0x3C / 4])(dobj->mobj, rendermode);
    }
    for (pobj = dobj->pobj; pobj != NULL; pobj = pobj->next) {
        void (**vtbl)(void) = (void (**)(void)) HSD_CLASS_METHOD(pobj);
        ((void (*)(HSD_PObj*, void*, void*, u32)) vtbl[0x3C / 4])(pobj, vmtx, pmtx, rendermode);
    }
    if (!(rendermode & 0x04000000)) {
        void (**vtbl)(void) = (void (**)(void)) HSD_CLASS_METHOD(dobj->mobj);
        ((void (*)(HSD_MObj*, u32)) vtbl[0x50 / 4])(dobj->mobj, rendermode);
    }
    fn_801A8470(NULL);
}
#endif
#pragma pop

/* 0x80199178 | 0x80 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801ACDAC(HSD_PObj* pobj, s32* out1, s32* out2);
#if 0
asm void fn_80199178(void) {
#include "src/hsd/hsd_dobj_fn_80199178.inc"
}
#else
#pragma optimization_level 4
void fn_80199178(HSD_DObj* dobj, s32* total_a, s32* total_b) {
    s32 sum_a;
    s32 sum_b;
    s32 a;
    s32 b;
    sum_a = 0;
    sum_b = 0;
    while (dobj != NULL) {
        fn_801ACDAC(dobj->pobj, &a, &b);
        dobj = dobj->next;
        sum_a += a;
        sum_b += b;
    }
    if (total_a != NULL) {
        *total_a = sum_a;
    }
    if (total_b != NULL) {
        *total_b = sum_b;
    }
}
#endif
#pragma pop

/* 0x801991F8 | 0x6C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801AD044(HSD_PObj* pobj, HSD_PObj* pobj2);
#if 0
asm void fn_801991F8(void) {
#include "src/hsd/hsd_dobj_fn_801991F8.inc"
}
#else
#pragma optimization_level 4
void fn_801991F8(HSD_DObj* dobj, HSD_DObj* desc) {
    HSD_DObj* dd;
    HSD_DObj* d;
    dd = desc;
    d = dobj;
    while (d != NULL && dd != NULL) {
        if (d != NULL && dd != NULL) {
            fn_801AD044(d->pobj, dd->pobj);
        }
        d = d->next;
        dd = dd->next;
    }
}
#endif
#pragma pop

/* 0x80199264 | 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80199264(void) {
#include "src/hsd/hsd_dobj_fn_80199264.inc"
}
#else
#pragma optimization_level 4
void fn_80199264(HSD_DObj* dobj) {
    HSD_DObj* next;
    HSD_DObj* d;
    d = dobj;
    while (d != NULL) {
        next = d->next;
        if (d != NULL) {
            HSD_CLASS_METHOD(d)->init((HSD_Class*) d);
            HSD_CLASS_METHOD(d)->release((HSD_Class*) d);
        }
        d = next;
    }
}
#endif
#pragma pop

/* 0x801992D8 | 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern HSD_ClassInfo* fn_80193748(const char* class_name);
extern void* fn_80193828(HSD_ClassInfo* info);
extern void fn_80196E10(const char* file, s32 line, const char* msg);
extern char lbl_8047DA18;
extern char lbl_8047DA20;
#if 0
asm void fn_801992D8(void) {
#include "src/hsd/hsd_dobj_fn_801992D8.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma optimization_level 1
HSD_DObj* fn_801992D8(HSD_DObjDesc* desc)
{
    HSD_DObj* dobj;
    HSD_ClassInfo* info;

    if (desc == NULL) {
        return NULL;
    }

    if (desc->class_name == NULL
        || (info = fn_80193748(desc->class_name)) == NULL)
    {
        info = default_class ? default_class : (HSD_ClassInfo*) &hsdDObj;
        dobj = (HSD_DObj*) fn_80193828(info);
        if (dobj == NULL) {
            fn_80196E10(&lbl_8047DA18, 0x214, &lbl_8047DA20);
        }
    } else {
        dobj = (HSD_DObj*) fn_80193828(info);
        if (dobj == NULL) {
            fn_80196E10(&lbl_8047DA18, 0x181, &lbl_8047DA20);
        }
    }

    {
        void (**vtbl)(void) = (void (**)(void)) HSD_CLASS_METHOD(dobj);
        ((int (*)(HSD_DObj*, HSD_DObjDesc*)) vtbl[0x40 / 4])(dobj, desc);
    }
    return dobj;
}
#pragma pop
#endif
#pragma pop

/* 0x801993A4 | 0x1C4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void OSReport(const char* fmt, ...);
extern HSD_MObj* fn_801A7B24(void* mobjdesc);
extern HSD_PObj* fn_801AD288(void* pobjdesc);
extern void fn_80196D78(const char* file, s32 line, const char* msg);
extern char lbl_8047DA28;
extern char lbl_8047DA18;
extern char lbl_8047DA20;
#if 0
asm void fn_801993A4(void) {
#include "src/hsd/hsd_dobj_fn_801993A4.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma optimization_level 1
static int fn_801993A4(HSD_DObj* dobj, void* desc_raw)
{
    void* desc;
    HSD_DObjDesc* subdesc;
    HSD_DObj* sub;
    HSD_ClassInfo* info;

    desc = desc_raw;
    subdesc = (HSD_DObjDesc*) *(u32*)((u8*)desc + 4);
    if (subdesc == NULL) {
        sub = NULL;
    } else {
        if (subdesc->class_name == NULL
            || (info = fn_80193748(subdesc->class_name)) == NULL)
        {
            info = default_class ? default_class : (HSD_ClassInfo*) &hsdDObj;
            sub = (HSD_DObj*) fn_80193828(info);
            if (sub == NULL) {
                fn_80196E10(&lbl_8047DA18, 0x214, &lbl_8047DA20);
            }
        } else {
            sub = (HSD_DObj*) fn_80193828(info);
            if (sub == NULL) {
                fn_80196E10(&lbl_8047DA18, 0x181, &lbl_8047DA20);
            }
        }
        {
            void (**vtbl)(void) = (void (**)(void)) HSD_CLASS_METHOD(sub);
            ((int (*)(HSD_DObj*, HSD_DObjDesc*)) vtbl[0x40 / 4])(sub, subdesc);
        }
    }
    dobj->next = sub;
    dobj->mobj = fn_801A7B24(*(void**)((u8*)desc + 8));
    dobj->pobj = fn_801AD288(*(void**)((u8*)desc + 0xC));

    if (dobj->mobj != NULL) {
        u32 type = *(u32*)((u8*)dobj->mobj + 4);
        type = type & 0x60000000;
        switch (type) {
        case 0x00000000:
            if (dobj != NULL) {
                dobj->flags = (dobj->flags & ~0x0F) | 0x02;
            }
            break;
        case 0x40000000:
            if (dobj != NULL) {
                dobj->flags = (dobj->flags & ~0x0F) | 0x08;
            }
            break;
        case 0x60000000:
            if (dobj != NULL) {
                dobj->flags = (dobj->flags & ~0x0F) | 0x04;
            }
            break;
        default:
            OSReport(*(char**)((u8*)0x80274758),
                     *(u32*)((u8*)dobj->mobj + 4));
            fn_80196D78(&lbl_8047DA18, 0x13F, &lbl_8047DA28);
            break;
        }
    }
    return 0;
}
#pragma pop
#endif
#pragma pop

/* 0x80199568 | 0x6C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801A7E3C(HSD_MObj* mobj);
extern void fn_801AD61C(HSD_PObj* pobj);
extern void fn_801C27F4(HSD_AObj* aobj, HSD_DObj* dobj, void* method);
#if 0
asm void fn_80199568(void) {
#include "src/hsd/hsd_dobj_fn_80199568.inc"
}
#else
#pragma optimization_level 4
void fn_80199568(HSD_DObj* dobj) {
    HSD_DObj* d;
    if (dobj == NULL) {
        return;
    }
    for (d = dobj; d != NULL; d = d->next) {
        if (d != NULL) {
            fn_801C27F4(d->aobj, d,
                        (void*) HSD_DOBJ_METHOD(d)->load);
            fn_801AD61C(d->pobj);
            fn_801A7E3C(d->mobj);
        }
    }
}
#endif
#pragma pop

/* 0x801995D4 | 0x80 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801A8354(f32 val, HSD_MObj* mobj, void* arg);
extern void fn_801AD6C4(f32 val, HSD_PObj* pobj, void* arg);
#if 0
asm void fn_801995D4(void) {
#include "src/hsd/hsd_dobj_fn_801995D4.inc"
}
#else
#pragma optimization_level 4
void fn_801995D4(HSD_DObj* dobj, f32 val, void* arg) {
    HSD_DObj* d;
    if (dobj == NULL) {
        return;
    }
    for (d = dobj; d != NULL; d = d->next) {
        if (d != NULL) {
            fn_801AD6C4(val, d->pobj, arg);
            fn_801A8354(val, d->mobj, arg);
        }
    }
}
#endif
#pragma pop

/* 0x80199654 | 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801A83BC(HSD_MObj* mobj, void* arg);
extern void fn_801AD738(HSD_PObj* pobj, void* arg);
#if 0
asm void fn_80199654(void) {
#include "src/hsd/hsd_dobj_fn_80199654.inc"
}
#else
#pragma optimization_level 4
void fn_80199654(HSD_DObj* dobj, void* matanim, void* shapeanim) {
    HSD_DObj* d;
    void* ma;
    void* sa;
    if (dobj == NULL) {
        return;
    }
    d = dobj;
    ma = matanim;
    sa = shapeanim;
    while (d != NULL) {
        if (d != NULL) {
            HSD_PObj* pobj;
            void* shapeargl;
            pobj = d->pobj;
            if (sa != NULL) {
                shapeargl = *(void**)((u8*)sa + 4);
            } else {
                shapeargl = NULL;
            }
            fn_801AD738(pobj, shapeargl);
            fn_801A83BC(d->mobj, ma);
        }
        d = d->next;
        if (ma != NULL) {
            ma = *(void**)ma;
        } else {
            ma = NULL;
        }
        if (sa != NULL) {
            sa = *(void**)sa;
        } else {
            sa = NULL;
        }
    }
}
#endif
#pragma pop

/* 0x80199704 | 0x8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern u32 lbl_8047AA44;
#if 0
asm void fn_80199704(void) {
#include "src/hsd/hsd_dobj_fn_80199704.inc"
}
#else
void fn_80199704(u32 val) {
    lbl_8047AA44 = val;
}
#endif
#pragma pop

/* 0x8019970C | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801AA498(void* list, void* data);
extern u8 lbl_80465378[];
#if 0
asm void fn_8019970C(void) {
#include "src/hsd/hsd_dobj_fn_8019970C.inc"
}
#else
#pragma optimization_level 4
void fn_8019970C(void* data) {
    fn_801AA498(lbl_80465378, data);
}
#endif
#pragma pop

/* 0x80199738 | 0x5C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void* fn_801AA4CC(void* list);
extern void fn_80196E10(const char* file, s32 line, const char* msg);
extern void* memset(void* dst, int val, u32 size);
extern char lbl_8047DA30;
extern char lbl_8047DA38;
#if 0
asm void fn_80199738(void) {
#include "src/hsd/hsd_dobj_fn_80199738.inc"
}
#else
#pragma optimization_level 4
void* fn_80199738(void) {
    void* p;
    p = fn_801AA4CC(lbl_80465378);
    if (p == NULL) {
        fn_80196E10(&lbl_8047DA30, 755, &lbl_8047DA38);
    }
    memset(p, 0, 0x30);
    return p;
}
#endif
#pragma pop

/* 0x80199794 | 0x2F0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_80199A84(void* list, void* data);
#if 1
asm void fn_80199794(void) {
#include "src/hsd/hsd_dobj_fn_80199794.inc"
}
#else
void fn_80199794(void) { /* TODO */ }
#endif
#pragma pop

/* ===================================================================
 * Generated: 0 pattern-matched + 5 stubs
 * Range: 0x8019B528 - 0x8019B7C0
 * =================================================================== */

/* 0x8019B528 | 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern u32 lbl_8047DA58;
extern u32 lbl_8047DA3C;
#if 1
asm void fn_8019B528(void) {
#include "src/hsd/hsd_dobj_fn_8019B528.inc"
}
#else
void fn_8019B528(void) {
    /* TODO: match -- 192 bytes at 0x8019B528 */
}
#endif
#pragma pop

/* 0x8019B5E8 | 0x168 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_8019970C(void* data);
extern void fn_801AA498(void* list, void* data);
extern void fn_80199A84(void* list, void* data);
extern void fn_8019B750(void* data);
extern u8 lbl_80465378[];
extern u8 lbl_802747AC[];
#if 0
asm void fn_8019B5E8(void) {
#include "src/hsd/hsd_dobj_fn_8019B5E8.inc"
}
#else
#pragma optimization_level 4
void fn_8019B5E8(void* data) {
    void* r31;
    void* r30;
    void* r29;
    void* r28;
    void* r27;
    void* r26;
    void* r25;
    void* r24;
    void* r23;
    r31 = data;
    if (r31 == NULL) {
        goto end;
    }
    r30 = *(void**)r31;
    if (r30 == NULL) {
        goto free_r31;
    }
    r29 = *(void**)r30;
    if (r29 == NULL) {
        goto free_r30;
    }
    r28 = *(void**)r29;
    if (r28 == NULL) {
        goto free_r29;
    }
    r27 = *(void**)r28;
    if (r27 == NULL) {
        goto free_r28;
    }
    r26 = *(void**)r27;
    if (r26 == NULL) {
        goto free_r27;
    }
    r25 = *(void**)r26;
    if (r25 == NULL) {
        goto free_r26;
    }
    r24 = *(void**)r25;
    if (r24 == NULL) {
        goto free_r25;
    }
    r23 = *(void**)r24;
    if (r23 == NULL) {
        goto free_r24;
    }
    fn_8019B5E8(*(void**)r23);
    fn_8019B750(r23);
free_r24:
    if (r24 != NULL) {
        fn_8019970C(r24);
    }
free_r25:
    if (r25 != NULL) {
        fn_801AA498(lbl_80465378, r25);
        fn_80199A84(lbl_80465378, lbl_802747AC);
    }
free_r26:
    if (r26 != NULL) {
        fn_801AA498(lbl_80465378, r26);
    }
free_r27:
    if (r27 != NULL) {
        fn_801AA498(lbl_80465378, r27);
    }
free_r28:
    if (r28 != NULL) {
        fn_801AA498(lbl_80465378, r28);
    }
free_r29:
    if (r29 != NULL) {
        fn_801AA498(lbl_80465378, r29);
    }
free_r30:
    if (r30 != NULL) {
        fn_801AA498(lbl_80465378, r30);
    }
free_r31:
    if (r31 != NULL) {
        fn_801AA498(lbl_80465378, r31);
    }
end:
    ;
}
#endif
#pragma pop

/* 0x8019B750 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_8019B750(void) {
#include "src/hsd/hsd_dobj_fn_8019B750.inc"
}
#else
#pragma optimization_level 4
void fn_8019B750(void* data) {
    if (data != NULL) {
        fn_801AA498(lbl_80465378, data);
    }
}
#endif
#pragma pop

/* 0x8019B784 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801AA35C(void* list, u32 size, u32 alignment);
#if 0
asm void fn_8019B784(void) {
#include "src/hsd/hsd_dobj_fn_8019B784.inc"
}
#else
void fn_8019B784(void) {
    fn_801AA35C(lbl_80465378, 0x30, 4);
}
#endif
#pragma pop

/* 0x8019B7B4 | 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_8019B7B4(void) {
#include "src/hsd/hsd_dobj_fn_8019B7B4.inc"
}
#else
void* fn_8019B7B4(void) {
    return lbl_80465378;
}
#endif
#pragma pop
