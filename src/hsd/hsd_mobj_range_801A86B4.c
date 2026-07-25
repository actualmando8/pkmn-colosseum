#include "dolphin/types.h"
#include "dolphin/mtx.h"

/* =========================================================================
 * Partial banked source for reserved split unit 0x801A86B4 - 0x801AA608.
 * Only the functions that byte-match under GC/1.3 are provided; the rest of
 * the range (including the five float-heavy matrix routines HSD_MtxSRT /
 * 8B94 / 8D1C / 98CC / 9DF0, which are 16-62% source-refinement fleet
 * targets, and the empty-stub dispatchers) stay as extracted asm.
 * ========================================================================= */

/* Object / class helpers (DTK names). */
extern void  fn_801A6960(void* ptr);   /* HSD_MemFree  */
extern void* fn_801A6928(s32 size);    /* HSD_MemAlloc */
extern s32   HSD_GetNbBits(u32 value);
extern void* memset(void* dst, int value, u32 size);
extern void  __assert(const char* file, u32 line, const char* expression);
extern void  hsdInitClassInfo(void* info, void* parent, const char* library,
                              const char* name, u32 size, u32 flags);

/* Data / global symbols (DTK names). */
extern void* lbl_8047B2E0;           /* free-list pool chain head (sbss) */
extern u8    lbl_8036CBF0[];         /* data heap descriptor             */
extern u8    hsdObj[];               /* data class info                  */
extern u8    lbl_8036C638[];         /* data parent class info           */
extern u32   lbl_8036CC40[];         /* performance counters             */
extern const char lbl_80274EC8[];    /* rodata string                    */
extern const char lbl_80274E90[];    /* objalloc.c strings               */
extern const char lbl_8047DC98;      /* non-null allocation data         */
extern const char lbl_8047DCA0;      /* sdata2 string                    */
extern const char lbl_8047DCA8;      /* sdata2 string                    */
extern const char lbl_8047DCB0;      /* sdata2 string                    */

/* ------------------------------------------------------------------------ */
/*  mtx.c - sysdolphin matrix builders                                       */
/*                                                                           */
/*  The retail range keeps the sysdolphin mtx.c bodies; the one Colosseum    */
/*  deviation from the shipped Melee revision is the reciprocal used by      */
/*  HSD_MtxSRT / HSD_MtxSRTQuat, which biases the divisor away from zero     */
/*  by a small epsilon instead of dividing directly.                         */
/* ------------------------------------------------------------------------ */

extern const f32 lbl_80478ACC; /* reciprocal guard epsilon (rodata) */

extern f64 sin(f64 x);
extern f64 cos(f64 x);

/* MSL's math.h defines these float entry points as double-call inlines. */
static inline f32 sinf(f32 x)
{
    f64 r = sin(x);
    return (f32) r;
}

static inline f32 cosf(f32 x)
{
    f64 r = cos(x);
    return (f32) r;
}

/* Address: 0x801A8884 | Size: 0x310 */
void HSD_MtxSRT(f32 m[3][4], Vec* vec1, Vec* vec2, Vec* vec3, Vec* vec4)
{
    f32 vec1x_2;
    f32 vec1y_2;
    f32 vec1z_2;
    f32 vec1x_1;
    f32 vec1y_1;
    f32 vec1z_1;
    f32 vec1x;
    f32 vec1y;
    f32 vec1z;

    f32 sinX = sinf(vec2->x);
    f32 cosX = cosf(vec2->x);
    f32 sinY = sinf(vec2->y);
    f32 cosY = cosf(vec2->y);
    f32 sinZ = sinf(vec2->z);
    f32 cosZ = cosf(vec2->z);

    vec1x_2 = vec1x_1 = vec1x = vec1->x;
    vec1y_2 = vec1y_1 = vec1y = vec1->y;
    vec1z_2 = vec1z_1 = vec1z = vec1->z;

    if (vec4 != NULL) {
        f32 temp1 =
            1.0f / (vec4->x >= 0.0f ? vec4->x + lbl_80478ACC
                                    : vec4->x - lbl_80478ACC);
        f32 temp2 =
            1.0f / (vec4->y >= 0.0f ? vec4->y + lbl_80478ACC
                                    : vec4->y - lbl_80478ACC);
        f32 temp3 =
            1.0f / (vec4->z >= 0.0f ? vec4->z + lbl_80478ACC
                                    : vec4->z - lbl_80478ACC);

        vec1y_2 *= vec4->y * temp1;
        vec1z_2 *= vec4->z * temp1;
        vec1x_1 *= vec4->x * temp2;
        vec1z_1 *= vec4->z * temp2;
        vec1x *= vec4->x * temp3;
        vec1y *= vec4->y * temp3;
    }

    m[0][0] = cosZ * (vec1x_2 * cosY);
    m[1][0] = sinZ * (vec1x_1 * cosY);
    m[2][0] = -vec1x * sinY;
    m[0][1] = vec1y_2 * ((cosZ * (sinX * sinY)) - (cosX * sinZ));
    m[1][1] = vec1y_1 * ((sinZ * (sinX * sinY)) + (cosX * cosZ));
    m[2][1] = cosY * (vec1y * sinX);
    m[0][2] = vec1z_2 * ((cosZ * (cosX * sinY)) + (sinX * sinZ));
    m[1][2] = vec1z_1 * ((sinZ * (cosX * sinY)) - (sinX * cosZ));
    m[2][2] = cosY * (vec1z * cosX);
    m[0][3] = vec3->x;
    m[1][3] = vec3->y;
    m[2][3] = vec3->z;
}

/* Address: 0x801A8B94 | Size: 0x188 */
void HSD_MkRotationMtx(f32 arg0[3][4], Vec* arg1)
{
    f32 sinX = sinf(arg1->x);
    f32 cosX = cosf(arg1->x);
    f32 sinY = sinf(arg1->y);
    f32 cosY = cosf(arg1->y);
    f32 sinZ = sinf(arg1->z);
    f32 cosZ = cosf(arg1->z);

    arg0[0][0] = cosY * cosZ;
    arg0[1][0] = cosY * sinZ;
    arg0[2][0] = -sinY;
    arg0[0][1] = (cosZ * (sinX * sinY)) - (cosX * sinZ);
    arg0[1][1] = (sinZ * (sinX * sinY)) + (cosX * cosZ);
    arg0[2][1] = sinX * cosY;
    arg0[0][2] = (cosZ * (cosX * sinY)) + (sinX * sinZ);
    arg0[1][2] = (sinZ * (cosX * sinY)) - (sinX * cosZ);
    arg0[2][2] = cosX * cosY;
    arg0[0][3] = 0.0f;
    arg0[1][3] = 0.0f;
    arg0[2][3] = 0.0f;
}

/* Address: 0x801A9570 | Size: 0x1C  -- already-banked (GC/1.3, calibration) */
void HSD_MtxGetTranslate(f32 mtx[3][4], f32* vec) {
    vec[0] = mtx[0][3];
    vec[1] = mtx[1][3];
    vec[2] = mtx[2][3];
}

/* Address: 0x801AA350 | Size: 0xC  -- already-banked (GC/1.3, calibration) */
void _HSD_ObjAllocForgetMemory(void) {
    lbl_8047B2E0 = 0;
}

/* Address: 0x801AA35C | Size: 0x13C */
void HSD_ObjAllocInit(void* data, u32 size, u32 align)
{
    void** current;

    if (data == NULL) {
        __assert(lbl_80274E90, 0x1AE, &lbl_8047DC98);
    }

    if (data != NULL) {
        current = &lbl_8047B2E0;
        while (*current != NULL) {
            if (*current == data) {
                *current = *(void**)((u8*)*current + 0x28);
                break;
            }
            current = (void**)((u8*)*current + 0x28);
        }
    } else {
        lbl_8047B2E0 = NULL;
    }

    memset(data, 0, 0x2C);
    *(s32*)((u8*)data + 0x14) = -1;
    *(u32*)((u8*)data + 0x18) = 0;
    *(s32*)((u8*)data + 0x1C) = -1;

    if (align > 0x20) {
        __assert(lbl_80274E90, 0x1B9, lbl_80274E90 + 0xC);
    }
    if (HSD_GetNbBits(align) != 1) {
        __assert(lbl_80274E90, 0x1BA, lbl_80274E90 + 0x18);
    }

    *(u32*)((u8*)data + 0x24) = align;
    *(u32*)((u8*)data + 0x20) = (size + align - 1) & ~(align - 1);
    *(void**)((u8*)data + 0x28) = lbl_8047B2E0;
    lbl_8047B2E0 = data;
}

/* Address: 0x801AA498 | Size: 0x34  -- HSD_ObjFree (from backup hsd_pobj_disp.c) */
#pragma push
#pragma optimization_level 1
void HSD_ObjFree(void* list, void* data)
{
    void* l = list;
    *(u32*)((u8*)l + 0x8) = *(u32*)((u8*)l + 0x8) - 1;
    fn_801A6960(data);
}
#pragma pop

/* Address: 0x801AA4CC | Size: 0x6C  -- HSD_ObjAlloc (from backup hsd_pobj_disp.c) */
#pragma push
#pragma optimization_level 1
void* HSD_ObjAlloc(void* list)
{
    void* l = list;

    if ((*(u8*)l & 0x80) >> 7) {
        if (*(u32*)((u8*)l + 0x8) >= *(u32*)((u8*)l + 0x14)) {
            return NULL;
        }
    }

    *(u32*)((u8*)l + 0x8) += 1;
    if (*(u32*)((u8*)l + 0x8) > *(u32*)((u8*)l + 0x10)) {
        *(u32*)((u8*)l + 0x10) = *(u32*)((u8*)l + 0x8);
    }

    return fn_801A6928(*(u32*)((u8*)l + 0x20));
}
#pragma pop

/* Address: 0x801AA538 | Size: 0x30  -- HSD_ObjSetHeap (from backup hsd_pobj_disp.c) */
#pragma push
#pragma optimization_level 0
void HSD_ObjSetHeap(void* a, void* b)
{
    *(volatile void**)((u8*)lbl_8036CBF0 + 0x4) = b;
    *(volatile void**)((u8*)lbl_8036CBF0 + 0x0) = b;
    *(volatile void**)((u8*)lbl_8036CBF0 + 0xc) = a;
    *(volatile void**)((u8*)lbl_8036CBF0 + 0x8) = a;
}
#pragma pop

/* Address: 0x801AA568 | Size: 0x44  -- PObj class info init (small)
 * (from backup hsd_pobj_disp.c) */
void ObjInfoInit_801AA568(void)
{
    hsdInitClassInfo(hsdObj,
                     (void*) lbl_8036C638, (char*) lbl_80274EC8,
                     (char*) &lbl_8047DCA0, 0x3c, 0x8);
}

/* Address: 0x801AA5AC | Size: 0x5C */
void fn_801AA5AC(s32 n)
{
    if (n >= 32) {
        __assert(&lbl_8047DCA8, 0xA4, &lbl_8047DCB0);
    }
    lbl_8036CC40[n + 4] += 1;
}
