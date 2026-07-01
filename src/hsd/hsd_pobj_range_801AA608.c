#include "dolphin/types.h"

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
extern void  fn_801C29C4(f32 val, void* aobj);

/* Data / global symbols (DTK names). */
extern u8    lbl_8036CCD0[];         /* PObj class info (data)      */
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

/* Address: 0x801AA6D0 | Size: 0xB8  -- PObj remove */
#pragma push
#pragma optimization_level 1
void fn_801AA6D0(void* pobj)
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
void fn_801ACD7C(void)
{
    fn_800B7D3C();
    lbl_8047B2FC = 0;
    lbl_8047B300 = NULL;
}
#pragma pop

/* Address: 0x801AD214 | Size: 0x74  -- Walk pobj list, vtable[0x30] + [0x34] */
#pragma push
#pragma peephole off
void HSD_PObjRemoveAll(void* pobj)
{
    void* next;
    void* cur = pobj;

    while (cur != NULL) {
        next = *(void**)((u8*)cur + 0x4);
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

/* Address: 0x801AD61C | Size: 0x5C  -- Walk pobj list, call reqAnim */
void HSD_PObjAnimAll(void* pobj)
{
    void* cur;

    if (pobj == NULL) {
        return;
    }

    cur = pobj;
    while (cur != NULL) {
        if (cur != NULL) {
            void** vtbl = *(void***)cur;
            fn_801C27F4(*(void**)((u8*)cur + 0x18), cur, vtbl[0x48 / 4]);
        }
        cur = *(void**)((u8*)cur + 0x4);
    }
}

/* Address: 0x801AD678 | Size: 0x4C  -- Set shape blend weight */
void PObjUpdateFunc(void* pobj, s32 idx, f32* weight_ptr)
{
    void* p = pobj;

    if (p == NULL) return;

    {
        u16 flags = *(u16*)((u8*)p + 0xc);
        if ((flags & 0x3000) != 0x1000) return;
    }

    {
        void* shapeset = *(void**)((u8*)p + 0x14);
        u16 ssflags = *(u16*)shapeset;

        if (ssflags & 0x2) {
            f32* arr = *(f32**)((u8*)shapeset + 0x1c);
            arr[idx - 2] = *weight_ptr;
        } else {
            *(f32*)((u8*)shapeset + 0x1c) = *weight_ptr;
        }
    }
}

/* Address: 0x801AD6C4 | Size: 0x74  -- Request PObj animation by flags */
#pragma push
#pragma optimization_level 1
void HSD_PObjReqAnimAllByFlags(f32 val, void* pobj, u32 flags)
{
    void* cur;

    if (pobj == NULL) {
        return;
    }

    cur = pobj;
    while (cur != NULL) {
        if (cur != NULL) {
            if (flags & 0x8) {
                fn_801C29C4(val, *(void**)((u8*)cur + 0x18));
            }
        }
        cur = *(void**)((u8*)cur + 0x4);
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

/* Address: 0x801ADD0C | Size: 0x3C  -- Deactivate texture anim state */
#pragma push
#pragma optimization_level 1
void fn_801ADD0C(void)
{
    s32 r = fn_801A6990(lbl_8047B308);
    if (r != 0) {
        lbl_8047B308 = NULL;
        lbl_8047B30C = 0;
    }
}
#pragma pop
