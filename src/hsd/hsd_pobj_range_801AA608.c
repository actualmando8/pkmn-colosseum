#include "dolphin/types.h"
#include "hsd/hsd_pobj.h"

/* =========================================================================
 * Partial banked source for reserved split unit 0x801AA608 - 0x801AE000.
 * Only the functions that byte-match under GC/1.3 are provided (from backup
 * hsd_pobj_disp.c); the remaining functions (dispatch/vtable reconstructions
 * that are 22-75% source-refinement fleet targets, plus the empty-stub
 * renderers) stay as extracted asm. objdiff pairs by symbol name.
 * ========================================================================= */

/* Helper functions (DTK names). */
extern s32   fn_801A6990(void* obj);
extern void  fn_800B7D3C(void);
extern void  fn_801C27F4(void* aobj, void* pobj, void* method);
extern void  PObjRelease(void);
extern void  PObjAmnesia(void* pobj);
extern void  PObjSetupMtx(void);
extern void  PObjLoad(void);
extern void  PObjUpdateFunc(HSD_PObj* pobj, s32 idx, f32* weight_ptr);

/* Data / global symbols (DTK names). */
extern u8    lbl_8036CCD0[];         /* PObj class info (data)      */
extern u8    lbl_8036C638[];         /* parent class info           */
extern u8    lbl_80274EE0[];         /* class library name          */
extern u8    lbl_80274EF8[];         /* class name                  */
extern void* lbl_8047B2E8;           /* cached default instance     */
extern void* lbl_8047B2EC;           /* active normal desc          */
extern void* lbl_8047B2F0;           /* active color desc           */
extern void* lbl_8047B2F4;           /* normal count                */
extern void* lbl_8047B2F8;           /* color count                 */
extern u32   lbl_8047B2FC;           /* display list marker         */
extern void* lbl_8047B300;           /* display list end marker     */
extern void* lbl_8047B308;           /* active texture desc         */
extern u32   lbl_8047B30C;           /* texture count               */
extern void* lbl_80478C90;           /* RNG default state instance  */
extern void* lbl_80478C94;           /* RNG current state pointer   */

/* Address: 0x801AA608 | Size: 0xC8  -- PObj class info init */
#pragma push
#pragma optimization_level 1
void HSD_PObjInit(void)
{
    hsdInitClassInfo((HSD_ClassInfo*) lbl_8036CCD0,
                     (HSD_ClassInfo*) lbl_8036C638, (char*) lbl_80274EE0,
                     (char*) lbl_80274EF8, 0x4C, 0x1C);

    *(void**) ((u8*) lbl_8036CCD0 + 0x30) = (void*) PObjRelease;
    *(void**) ((u8*) lbl_8036CCD0 + 0x38) = (void*) PObjAmnesia;
    *(void**) ((u8*) lbl_8036CCD0 + 0x3C) = (void*) HSD_PObjDisp;
    *(void**) ((u8*) lbl_8036CCD0 + 0x40) = (void*) PObjSetupMtx;
    *(void**) ((u8*) lbl_8036CCD0 + 0x44) = (void*) PObjLoad;
    *(void**) ((u8*) lbl_8036CCD0 + 0x48) = (void*) PObjUpdateFunc;
}
#pragma pop

/* Address: 0x801AA6D0 | Size: 0xB8  -- PObj remove */
#pragma push
#pragma optimization_level 1
void PObjAmnesia(void* pobj)
{
    void* p = pobj;

    if (p == lbl_8047B2E8) {
        lbl_8047B2E8 = NULL;
    }

    if (p == (void*)lbl_8036CCD0) {
        s32 r;

        r = fn_801A6990(lbl_8047B2EC);
        if (r != 0) {
            lbl_8047B2EC = NULL;
            lbl_8047B2F4 = NULL;
        }

        r = fn_801A6990(lbl_8047B2F0);
        if (r != 0) {
            lbl_8047B2F0 = NULL;
            lbl_8047B2F8 = NULL;
        }

        lbl_8047B2FC = 0;
        lbl_8047B300 = NULL;
    }

    {
        void** ci = *(void***)((u8*)lbl_8036CCD0 + 0x14);
        ((void(*)(void*))ci[0x38/4])(p);
    }
}
#pragma pop

/* Address: 0x801ACD7C | Size: 0x30  -- Draw sync + clear display list state */
#pragma push
#pragma optimization_level 1
void HSD_ClearVtxDesc(void)
{
    fn_800B7D3C();
    lbl_8047B2FC = 0;
    lbl_8047B300 = NULL;
}
#pragma pop

/* Address: 0x801AD214 | Size: 0x74  -- Walk pobj list, vtable[0x30] + [0x34] */
#pragma push
#pragma peephole off
void HSD_PObjRemoveAll(HSD_PObj* pobj)
{
    HSD_PObj* next;
    HSD_PObj* cur = pobj;

    while (cur != NULL) {
        next = cur->next;
        if (cur != NULL) {
            void** vtbl = *(void***)cur;
            ((void(*)(void*))vtbl[0x30 / 4])(cur);
            vtbl = *(void***)cur;
            ((void(*)(void*))vtbl[0x34 / 4])(cur);
        }
        cur = next;
    }
}
#pragma pop

/* Address: 0x801AD288 | Size: 0xCC  -- Load PObj descriptor */
HSD_PObj* HSD_PObjLoadDesc(HSD_PObjDesc* desc)
{
    extern HSD_ClassInfo* fn_80193748(const char* class_name);
    extern void* fn_80193828(HSD_ClassInfo* info);
    extern char lbl_8047DCB8;
    extern char lbl_8047DD10;
    extern void __assert(const char*, s32, const char*);
    HSD_ClassInfo* info;
    HSD_PObj* pobj;

    if (desc == NULL) {
        goto return_null;
    }

    if (*(volatile u32*) &desc->class_name != 0) {
        info = fn_80193748(desc->class_name);
        if (info != NULL) {
            goto alloc_named;
        }
    }

    if (*(volatile u32*) &lbl_8047B2E8 != 0) {
        info = lbl_8047B2E8;
    } else {
        info = (HSD_ClassInfo*) lbl_8036CCD0;
    }
    pobj = fn_80193828(info);
    if (pobj == NULL) {
        __assert(&lbl_8047DCB8, 0x2A9, &lbl_8047DD10);
    }
    goto load;

alloc_named:
    pobj = fn_80193828(info);
    if (pobj == NULL) {
        __assert(&lbl_8047DCB8, 0x247, &lbl_8047DD10);
    }

load:
    HSD_POBJ_METHOD(pobj)->load(pobj, desc);
    return pobj;

return_null:
    return NULL;
}

/* Address: 0x801AD61C | Size: 0x5C  -- Walk pobj list, call reqAnim */
void HSD_PObjAnimAll(HSD_PObj* pobj)
{
    HSD_PObj* cur;

    if (pobj == NULL) {
        return;
    }

    cur = pobj;
    while (cur != NULL) {
        if (cur != NULL) {
            void** vtbl = *(void***)cur;
            fn_801C27F4(*(void**)((u8*)cur + 0x18), cur, vtbl[0x48 / 4]);
        }
        cur = cur->next;
    }
}

/* Address: 0x801AD678 | Size: 0x4C  -- Set shape blend weight */
void PObjUpdateFunc(HSD_PObj* pobj, s32 idx, f32* weight_ptr)
{
    HSD_PObj* p = pobj;
    HSD_ShapeSet* shapeset;

    if (p == NULL) return;

    if ((p->flags & 0x3000) != 0x1000) return;

    shapeset = p->u.shape_set;

    if (shapeset->flags & 0x2) {
        shapeset->blend.bp[idx - 2] = *weight_ptr;
    } else {
        shapeset->blend.bl = *weight_ptr;
    }
}

/* Address: 0x801AD6C4 | Size: 0x74  -- Request PObj animation by flags */
#pragma push
#pragma optimization_level 1
void HSD_PObjReqAnimAllByFlags(f32 val, HSD_PObj* pobj, u32 flags)
{
    HSD_PObj* cur;

    if (pobj == NULL) {
        return;
    }

    cur = pobj;
    while (cur != NULL) {
        if (cur != NULL) {
            if (flags & 0x8) {
                HSD_AObjReqAnim(*(void**)((u8*)cur + 0x18), val);
            }
        }
        cur = cur->next;
    }
}
#pragma pop

/* Address: 0x801AD738 | Size: 0x94  -- Add shape animation to PObj list */
void fn_801AD738(HSD_PObj* pobj, HSD_ShapeAnim* anim)
{
    HSD_PObj* p;
    HSD_ShapeAnim* a;

    if (pobj == NULL || anim == NULL) {
        return;
    }

    p = pobj;
    a = anim;
    while (p != NULL) {
        if (p != NULL) {
            if (*(HSD_AObj* volatile*) ((u8*) p + 0x18) != NULL) {
                HSD_AObjRemove(*(HSD_AObj**) ((u8*) p + 0x18));
            }
            *(HSD_AObj**) ((u8*) p + 0x18) =
                HSD_AObjLoadDesc(a->aobjdesc);
        }
        p = p->next;
        if (a != NULL) {
            a = a->next;
        } else {
            a = NULL;
        }
    }
}

/* Address: 0x801AB538 | Size: 0xC0  -- Get matrix-mark pair */
#pragma push
#pragma optimization_level 1
void HSD_PObjGetMtxMark(s32 index, u32* first, u32* second)
{
    extern void __assert(const char*, s32, const char*);
    extern char lbl_8047DCB8;
    extern char lbl_8047DCE0;
    extern char lbl_8047DCE4;
    extern u32 lbl_80465678[];

    if (first == NULL) {
        __assert(&lbl_8047DCB8, 0x663, &lbl_8047DCE0);
    }
    if (second == NULL) {
        __assert(&lbl_8047DCB8, 0x664, &lbl_8047DCE4);
    }

    if (index < 0 || index >= 2) {
        *first = 0;
        *second = 0;
    } else {
        *first = lbl_80465678[2 * index];
        *second = lbl_80465678[2 * index + 1];
    }
}
#pragma pop

/* Address: 0x801AB5F8 | Size: 0x44  -- Set one mtx-mark slot */
#pragma push
#pragma optimization_level 1
void fn_801AB5F8(s32 index, void* ptr, s32 value)
{
    extern u8 lbl_80465678[];
    s32 i = index;

    if (i >= 2) {
        return;
    }
    if (i < 0) {
        goto store;
    }
    if (i < 2) {
        return;
    }

store:
    *(void**) ((u8*) lbl_80465678 + (u32) i * 8) = ptr;
    *(s32*) ((u8*) lbl_80465678 + (u32) i * 8 + 4) = value;
}
#pragma pop

/* Address: 0x801AB63C | Size: 0x40  -- Set both mtx-mark slots */
#pragma push
#pragma optimization_level 1
void fn_801AB63C(u32 first, u32 second)
{
    extern u8 lbl_80465678[];
    s32 i;

    for (i = 0; i < 2; i++) {
        ((u32*) lbl_80465678)[2 * i] = first;
        ((u32*) lbl_80465678)[2 * i + 1] = second;
    }
}
#pragma pop

/* Address: 0x801ADC08 | Size: 0x34  -- Forget RNG memory state */
void _HSD_RandForgetMemory(void)
{
    s32 r = fn_801A6990(lbl_80478C94);
    if (r != 0) {
        lbl_80478C94 = &lbl_80478C90;
    }
}

/* Address: 0x801ADC3C | Size: 0x40  -- Bounded RNG */
#pragma push
#pragma peephole off
s32 fn_801ADC3C(s32 max)
{
    s32 rand;

    rand = ((s32*)lbl_80478C94)[0];
    rand = (rand * 0x343fd) + 0x269fc3;
    ((s32*)lbl_80478C94)[0] = rand;
    return (s32)(((s32)max * (s16)(((s32*)lbl_80478C94)[0] >> 16)) / 0x10000);
}
#pragma pop

/* Address: 0x801ADC7C | Size: 0x5C  -- LCG next, normalized float return */
#pragma push
#pragma peephole off
f32 fn_801ADC7C(void)
{
    extern const f32 lbl_8047DD40;
    u32* state;
    volatile u32* state_v;
    u32 next;

    state = (u32*) lbl_80478C94;
    state_v = (u32*) lbl_80478C94;
    next = (*state * 0x343FD) + 0x269EC3;
    *state_v = next;

    return (f32) (*(u32*) lbl_80478C94 >> 16) / lbl_8047DD40;
}
#pragma pop

/* Address: 0x801ADCD8 | Size: 0x34  -- LCG next, u16 return */
#pragma push
#pragma peephole off
u32 fn_801ADCD8(void)
{
    u32* state;
    volatile u32* state_v;
    u32 next;

    state = (u32*)lbl_80478C94;
    state_v = (u32*)lbl_80478C94;
    next = (*state * 0x343fd) + 0x269ec3;
    *state_v = next;

    return *(u32*)lbl_80478C94 >> 16;
}
#pragma pop

/* Address: 0x801ADD0C | Size: 0x3C  -- Deactivate texture anim state */
#pragma push
#pragma optimization_level 1
void _HSD_RObjForgetMemory(void)
{
    s32 r = fn_801A6990(lbl_8047B308);
    if (r != 0) {
        lbl_8047B308 = NULL;
        lbl_8047B30C = 0;
    }
}
#pragma pop
