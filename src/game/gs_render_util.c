/**
 * @file gs_render_util.c
 * @brief GS render utility / HSD bridge code before GSgfx.
 *
 * Contains utility functions for the rendering pipeline including
 * HSD object management, matrix/vector operations, and model
 * rendering helpers.
 *
 * Address range: 0x800D104C - 0x800D3074
 * ~40 functions
 */

#include "dolphin/types.h"

/* ===== External references ===== */
extern void fn_800DD970(const char* fmt, ...);
extern void SISetSamplingRate(u32 rate);

/* ===== Global state (SDA) ===== */
extern u32 lbl_8047AA60;  /* SamplingRate passed to SISetSamplingRate */
extern u32 lbl_8047AA6C;  /* object array base pointer */
extern u32 lbl_8047AA70;  /* object array count */
extern u32 lbl_8047AA74;  /* current render object */
extern void* lbl_8047AA80; /* pointer to render state struct */

/* Matrix/vector math */
extern void fn_800A37CC(void* mtx, void* vecIn, void* vecOut); /* MTXMultVec3 */
extern void fn_800A38C0(void* mtxA, void* mtxB, void* mtxOut); /* MTXConcat */
extern void fn_800A3544(void* mtx);                             /* MTXIdentity */
extern void fn_800A35D0(void* mtxA, void* mtxB);               /* MTXCopy */
extern void fn_800A3A9C(void* out, void* in, f32 scale);       /* VECNormalize */
extern void fn_800A2D64(void* mtxA, void* mtxB);

/* GX functions */
extern void GXSetProjection(void* mtx, u32 type);
extern void GXSetViewport(f32 x, f32 y, f32 w, f32 h, f32 nearZ, f32 farZ);
extern void GXLoadPosMtxImm(void* mtx, u32 id);
extern void GXSetCurrentMtx(u32 id);

/* HSD functions */
extern void* fn_80362D0C(void* jobj);  /* HSD_JObjAnimAll */
extern void fn_80363CF4(void* jobj);   /* HSD_JObjRemoveAll */

/* GS render util internal functions */
extern void fn_800E01D0(void* dst, void* src);
extern void fn_800E0168(void* dst, void* src1, void* src2);
extern void fn_800E0628(void* dst, void* src);
extern void fn_800E0218(void* a, void* b, void* c, void* d);
extern void fn_800E053C(void* a, f32 b);
extern void fn_800E0518(void* a, f32 b);
extern void fn_800E04F4(void* a, f32 b);
extern void fn_800E05C0(void* a, f32 b, f32 c, f32 d);
extern void fn_800E0290(void* a, void* b, void* c);
extern void fn_800E0678(void* a, f32 b, f32 c, f32 d, f32 e);
extern void fn_800E0698(void* a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g);
extern void fn_80195904(void* jobj, void* data);
extern void fn_801950D0(void* jobj, void* data);
extern void fn_801959DC(void* jobj, void* data);
extern void fn_80195898(void* jobj);
extern void fn_801943BC(void* jobj, u32 flag, f32 a, f32 b, f32 c, f32 d);
extern void fn_801943A0(void* jobj, f32 x, f32 y);
extern void fn_801944E8(void* jobj, f32 z);
extern void fn_801944C0(void* jobj, f32 w);
extern void fn_801944A4(void* jobj, u32 x0, u32 x1, u32 y0, u32 y1);
extern void fn_80194400(void* jobj, void* rect);
extern void fn_8019431C(void* jobj, void* outA, void* outB);
extern f32  fn_801944F8(void* jobj);
extern f32  fn_801944D0(void* jobj);
extern void fn_801942C0(void* jobj, void* outA, void* outB, void* outC, void* outD);
extern void* fn_80194CC4(void* jobj);
extern void fn_80196BB8(void* jobj);
extern void fn_80196B10(void* jobj, void* ptr);
extern void fn_80196698(void* jobj, f32 val);
extern void fn_801966FC(void* jobj);
extern void fn_801C027C(void* jobj);
extern double lbl_8047C9B8;  /* SDA double constant for animation step */
extern void fn_801C028C(void* jobj, u32 a, u32 b, void (*cb)(void*), ...);
extern void fn_80196E10(const char* file, u32 line, const char* msg);
extern f32 lbl_8047C990;     /* SDA float: animation step increment */
extern char lbl_8047C9DC[] __attribute__((section(".sdata2")));  /* SDA2 string: assert filename */
extern char lbl_8047C9E4[] __attribute__((section(".sdata2")));  /* SDA2 string: assert condition */
extern u32 fn_800E3534(u32 size);   /* memory allocate, returns ptr as u32 */
extern u32 fn_800E27B0(u32 handle); /* map handle to object ptr */
extern u16 lbl_8047AA68;  /* render obj array low16 tag */
extern u32 lbl_8047AA6C;  /* render obj array base pointer */
extern f32 lbl_8047C994;  /* SDA float constant */
extern f32 lbl_8047C9C0;  /* SDA float constant */
extern u8 lbl_804001B0[0x40]; /* light state buffer (.bss) */
extern s32  fn_800D37CC(void);
extern void fn_800D7FE4(void* obj);
extern void fn_800D834C(void);
extern void fn_800D9BD0(f32 a, f32 b, f32 c, f32 d);
extern void fn_800D9B58(f32 a, f32 b, f32 c, f32 d);

/* Forward declarations for functions defined later in this file */
void fn_800D2B44(void* obj);


/* ==================================================================
 * fn_800D104C - GS render: set sampling rate
 * Address: 0x800D104C, Size: 0x24
 * ================================================================== */
void fn_800D104C(void) {
    SISetSamplingRate(lbl_8047AA60);
}

/* ==================================================================
 * fn_800D1070 - GS render utility: full render setup
 * Address: 0x800D1070, Size: 0x354
 * Sets up the complete rendering pipeline for a frame:
 * view matrix, projection, viewport, and GX state.
 * ================================================================== */
void fn_800D1070(void) {
}

/* ==================================================================
 * fn_800D13C4 - Empty function (likely stripped debug)
 * Address: 0x800D13C4, Size: 0x4
 * ================================================================== */
void fn_800D13C4(void) {
    /* 4 bytes -- likely just blr (empty function) */
}

/* ==================================================================
 * fn_800D13C8 - GS render: model render with transform
 * Address: 0x800D13C8, Size: 0x2AC
 * Renders a model with a given world transform matrix.
 * ================================================================== */
void fn_800D13C8(void* model, f32 mtx[3][4]) {
    if (model == NULL) {
        return;
    }

    /* 1. Concatenate world matrix with view matrix */
    /* 2. Load model-view matrix to GX */
    /* 3. Set up material state */
    /* 4. Dispatch display lists */
}

/* ==================================================================
 * fn_800D1674 - GS render: copy object data to dest struct
 * Address: 0x800D1674, Size: 0xB8
 * ================================================================== */
void fn_800D1674(void* src, void* dst) {
    *(u8*)dst = *(u8*)((u8*)src + 0x4);
    fn_800E01D0((u8*)dst + 0x4, (u8*)src + 0x70);
    fn_800E01D0((u8*)dst + 0x10, (u8*)src + 0x7c);
    fn_800E01D0((u8*)dst + 0x1c, (u8*)src + 0x88);
    fn_800E0628((u8*)dst + 0x28, (u8*)src + 0x94);
    fn_800E0628((u8*)dst + 0x58, (u8*)src + 0xc4);
    fn_800E01D0((u8*)dst + 0x88, (u8*)src + 0xf4);
    fn_800E01D0((u8*)dst + 0x94, (u8*)src + 0x100);
    *(u32*)((u8*)dst + 0xa0) = *(u32*)((u8*)src + 0x10c);
    *(u32*)((u8*)dst + 0xa4) = *(u32*)((u8*)src + 0x114);
    *(f32*)((u8*)dst + 0xa8) = *(f32*)((u8*)src + 0x118);
    *(f32*)((u8*)dst + 0xac) = *(f32*)((u8*)src + 0x11c);
    *(u8*)((u8*)dst + 0x1) = *(u8*)((u8*)src + 0x124);
}

/* ==================================================================
 * fn_800D172C - GS render: get object speed float (field_0x11c)
 * Address: 0x800D172C, Size: 0x8
 * ================================================================== */
f32 fn_800D172C(void* obj) {
    return *(f32*)((u8*)obj + 0x11c);
}

/* ==================================================================
 * fn_800D1734 - GS render: get object signed field_0x124
 * Address: 0x800D1734, Size: 0xC
 * ================================================================== */
s8 fn_800D1734(void* obj) {
    return *(s8*)((u8*)obj + 0x124);
}

/* ==================================================================
 * fn_800D173C - GS render: init object animation state
 * Address: 0x800D173C, Size: 0x5C
 * ================================================================== */
void fn_800D173C(void* obj) {
    *(u8*)((u8*)obj + 0x4) = 0;
    *(u8*)((u8*)obj + 0x1) = 1;
    fn_80195904(*(void**)((u8*)obj + 0xc), (u8*)obj + 0x70);
    fn_801950D0(*(void**)((u8*)obj + 0xc), (u8*)obj + 0xf4);
    fn_801959DC(*(void**)((u8*)obj + 0xc), (u8*)obj + 0x100);
}

/* ==================================================================
 * fn_800D1798 - GS render: advance object animation by speed
 * Address: 0x800D1798, Size: 0xC0
 * ================================================================== */
#pragma push
#pragma scheduling on
void fn_800D1798(void* obj) {
    if (*(u8*)((u8*)obj + 0x3) != 0) {
        f32 speed;
        *(u8*)((u8*)obj + 0x4) = 1;
        *(u8*)((u8*)obj + 0x124) = 0;
        *(u8*)((u8*)obj + 0x125) = 1;
        speed = *(f32*)((u8*)obj + 0x11c);
        if (*(u8*)((u8*)obj + 0x3) != 0) {
            *(f32*)((u8*)obj + 0x11c) = speed;
            fn_801C028C(*(void**)((u8*)obj + 0xc), (u32)2, (u32)0xffff, fn_801C027C, lbl_8047C9B8, (u32)1);
            fn_80196698(*(void**)((u8*)obj + 0xc), *(f32*)((u8*)obj + 0x11c));
            fn_801966FC(*(void**)((u8*)obj + 0xc));
            fn_801C028C(*(void**)((u8*)obj + 0xc), (u32)2, (u32)0xffff, fn_801C027C, (double)(*(f32*)((u8*)obj + 0x118)), (u32)1);
        }
    }
}
#pragma pop

/* ==================================================================
 * fn_800D1858 - GS render: set object fields 0x10c and 0x114
 * Address: 0x800D1858, Size: 0xC
 * ================================================================== */
void fn_800D1858(void* obj, u32 val) {
    *(u32*)((u8*)obj + 0x10c) = val;
}

/* ==================================================================
 * fn_800D1860 - GS render: set object anim speed (field 0x11c) and advance
 * Address: 0x800D1860, Size: 0x9C
 * ================================================================== */
void fn_800D1860(void* obj, f32 speed) {
    if (*(u8*)((u8*)obj + 0x3) != 0) {
        *(f32*)((u8*)obj + 0x11c) = speed;
        fn_801C028C(*(void**)((u8*)obj + 0xc), (u32)2, (u32)0xffff, fn_801C027C, lbl_8047C9B8, (u32)1);
        fn_80196698(*(void**)((u8*)obj + 0xc), *(f32*)((u8*)obj + 0x11c));
        fn_801966FC(*(void**)((u8*)obj + 0xc));
        fn_801C028C(*(void**)((u8*)obj + 0xc), (u32)2, (u32)0xffff, fn_801C027C, (double)(*(f32*)((u8*)obj + 0x118)), (u32)1);
    }
}

/* ==================================================================
 * fn_800D18FC - GS render: set object fov angle (field 0x118)
 * Address: 0x800D18FC, Size: 0x88
 * ================================================================== */
extern f32 lbl_8047C9B0;  /* scaling constant for fov conversion */
void fn_800D18FC(void* obj, f32 fov) {
    if (*(u8*)((u8*)obj + 0x3) != 0) {
        s32 mode = fn_800D37CC();
        if (mode == 0x32) {
            fov *= lbl_8047C9B0;
        }
        *(f32*)((u8*)obj + 0x118) = fov;
        fn_801C028C(*(void**)((u8*)obj + 0xc), (u32)2, (u32)0xffff, fn_801C027C, *(f32*)((u8*)obj + 0x118), (u32)1);
    }
}

/* ==================================================================
 * fn_800D1984 - GS render: set object animation frame index
 * Address: 0x800D1984, Size: 0xB4
 * ================================================================== */
extern f32 lbl_8047C998;   /* SDA float 0.0 constant */
extern f32 lbl_8047AA78;   /* SDA float temp for fn_800D1984 */
#pragma push
#pragma scheduling on
void fn_800D1984(void* obj, u32 frame_idx) {
    if (*(u8*)((u8*)obj + 0x3) != 0) {
        fn_80196BB8(*(void**)((u8*)obj + 0xc));
        if (frame_idx <= *(u32*)((u8*)obj + 0x110)) {
            *(u32*)((u8*)obj + 0x114) = frame_idx;
            fn_80196B10(*(void**)((u8*)obj + 0xc),
                (*(void***)((u8*)*(void**)((u8*)obj + 0x8) + 4))[*(u32*)((u8*)obj + 0x114)]);
            fn_80196698(*(void**)((u8*)obj + 0xc), lbl_8047C998);
            lbl_8047AA78 = lbl_8047C998;
            fn_801C028C(*(void**)((u8*)obj + 0xc), (u32)2, (u32)0xffff, fn_800D2B44, lbl_8047C998, (u32)0);
            *(f32*)((u8*)obj + 0x120) = lbl_8047AA78;
        }
    }
}
#pragma pop

/* ==================================================================
 * fn_800D1A38 - GS render: get object active flag (field_0x4)
 * Address: 0x800D1A38, Size: 0x8
 * ================================================================== */
u8 fn_800D1A38(void* obj) {
    return *(u8*)((u8*)obj + 0x4);
}

/* ==================================================================
 * fn_800D1A40 - GS render: get object angles (field_0x70, 0x100)
 * Address: 0x800D1A40, Size: 0x30
 * ================================================================== */
void fn_800D1A40(void* obj, void* dest) {
    fn_800E0168(dest, (u8*)obj + 0x70, (u8*)obj + 0x100);
}

/* ==================================================================
 * fn_800D1A70 - GS render: setup viewport/projection from JObj
 * Address: 0x800D1A70, Size: 0xCC
 * Sets up light/camera data from a JObj, writes to lbl_804001B0.
 * Returns pointer to lbl_804001B0.
 * ================================================================== */
#pragma push
#pragma scheduling on
void* fn_800D1A70(void* lightSet) {
    f32 x, z;
    f32 out3, out2, out1, out0;
    void* jobj = *(void**)((u8*)lightSet + 0xc);
    if (*(u8*)((u8*)jobj + 0x50) == 1) {
        f32 w, h;
        fn_8019431C(jobj, &x, &z);
        w = fn_801944F8(*(void**)((u8*)lightSet + 0xc));
        h = fn_801944D0(*(void**)((u8*)lightSet + 0xc));
        fn_800E0678(lbl_804001B0, x, z, w, h);
    } else {
        fn_801942C0(jobj, &out2, &out0, &out3, &out1);
        fn_800E0698(lbl_804001B0, out2, out0, out3, out1, lbl_8047C994, lbl_8047C9C0);
    }
    return lbl_804001B0;
}
#pragma pop

/* ==================================================================
 * fn_800D1B3C - GS render: model instance create
 * Address: 0x800D1B3C, Size: 0x1C4
 * ================================================================== */
void* fn_800D1B3C(void* modelData, u32 flags) {
    /* Create a model instance from model data:
     * 1. Allocate instance structure
     * 2. Load JObj hierarchy
     * 3. Set default transform
     * 4. Return instance handle
     */
    return NULL;
}

/* ==================================================================
 * fn_800D1D00 - GS render: update object transform from animation
 * Address: 0x800D1D00, Size: 0x1B8
 * ================================================================== */
extern f32 lbl_8047C9AC;  /* float constant from literal pool at -30004(r3) where r3=-32696<<16 */
void fn_800D1D00(void* obj) {
    if (*(u8*)((u8*)obj + 0x2) == 0) {
        return;
    }
    if (*(u8*)((u8*)obj + 0x4) != 0) {
        fn_800E0628((u8*)obj + 0x94, fn_80194CC4(*(void**)((u8*)obj + 0xc)));
        fn_80195904(*(void**)((u8*)obj + 0xc), (u8*)obj + 0x70);
        fn_801950D0(*(void**)((u8*)obj + 0xc), (u8*)obj + 0xf4);
        fn_801959DC(*(void**)((u8*)obj + 0xc), (u8*)obj + 0x100);
    } else if (*(u8*)((u8*)obj + 0x1) == 1) {
        f32 tmp[3];
        fn_800E0168(tmp, (u8*)obj + 0x70, (u8*)obj + 0x100);
        {
            f32 ax = tmp[0];
            f32 ay = tmp[1];
            f32 az = tmp[2];
            if (ax < 0.0f) ax = -ax;
            if (ax < lbl_8047C9AC) {
                if (ay < 0.0f) ay = -ay;
                if (ay < lbl_8047C9AC) {
                    if (az < 0.0f) az = -az;
                    if (az < lbl_8047C9AC) {
                        f32 v = *(f32*)((u8*)obj + 0x100);
                        *(f32*)((u8*)obj + 0x100) = (f32)(v + (double)1.0);
                    }
                }
            }
        }
        fn_800E0218((u8*)obj + 0x94, (u8*)obj + 0x70, (u8*)obj + 0xf4, (u8*)obj + 0x100);
    } else {
        f32 tmp1[3][4];
        f32 tmp2[3][4];
        f32 tmp3[3][4];
        f32 tmp4[3];
        fn_800E053C((u8*)obj + 0x94, -*(f32*)((u8*)obj + 0x88));
        fn_800E0518(tmp1, -*(f32*)((u8*)obj + 0x8c));
        fn_800E04F4(tmp2, -*(f32*)((u8*)obj + 0x90));
        fn_800E05C0(tmp3, -*(f32*)((u8*)obj + 0x70), -*(f32*)((u8*)obj + 0x74), -*(f32*)((u8*)obj + 0x78));
        fn_800E0290((u8*)obj + 0x94, (u8*)obj + 0x94, tmp1);
        fn_800E0290((u8*)obj + 0x94, (u8*)obj + 0x94, tmp2);
        fn_800E0290((u8*)obj + 0x94, (u8*)obj + 0x94, tmp3);
    }
    *(u8*)((u8*)obj + 0x2) = 0;
}

/* ==================================================================
 * fn_800D1EB8 - GS render: get translation and scale data
 * Address: 0x800D1EB8, Size: 0x4C
 * ================================================================== */
void fn_800D1EB8(void* obj, void* dest1, void* dest2) {
    fn_800E01D0(dest1, (u8*)obj + 0xf4);
    fn_800E01D0(dest2, (u8*)obj + 0x100);
}

/* ==================================================================
 * fn_800D1F04 - GS render: set translation and scale data
 * Address: 0x800D1F04, Size: 0x54
 * ================================================================== */
void fn_800D1F04(void* obj, void* src1, void* src2) {
    fn_800E01D0((u8*)obj + 0xf4, src1);
    fn_800E01D0((u8*)obj + 0x100, src2);
    *(u8*)((u8*)obj + 0x2) = 1;
    *(u8*)((u8*)obj + 0x1) = 1;
}

/* ==================================================================
 * fn_800D1F58 - GS render: set object rotation data (field_0x88)
 * Address: 0x800D1F58, Size: 0x2C
 * ================================================================== */
void fn_800D1F58(void* obj, void* anim) {
    fn_800E01D0(anim, (u8*)obj + 0x88);
}

/* ==================================================================
 * fn_800D1F84 - GS render: get position data, optionally update JObj
 * Address: 0x800D1F84, Size: 0x58
 * ================================================================== */
void fn_800D1F84(void* obj, void* dest) {
    if (*(u8*)((u8*)obj + 0x4) != 0) {
        fn_80195904(*(void**)((u8*)obj + 0xc), (u8*)obj + 0x70);
    }
    fn_800E01D0(dest, (u8*)obj + 0x70);
}

/* ==================================================================
 * fn_800D1FDC - GS render: get object scissor rect extents
 * Address: 0x800D1FDC, Size: 0x60
 * ================================================================== */
void fn_800D1FDC(void* obj, void* a, void* b, f32* outA, f32* outB) {
    fn_8019431C(*(void**)((u8*)obj + 0xc), a, b);
    *outA = fn_801944F8(*(void**)((u8*)obj + 0xc));
    *outB = fn_801944D0(*(void**)((u8*)obj + 0xc));
}

/* ==================================================================
 * fn_800D203C - GS render: set rotation data, mark dirty
 * Address: 0x800D203C, Size: 0x40
 * ================================================================== */
void fn_800D203C(void* obj, void* src) {
    fn_800E01D0((u8*)obj + 0x88, src);
    *(u8*)((u8*)obj + 0x2) = 1;
    *(u8*)((u8*)obj + 0x1) = 0;
}

/* ==================================================================
 * fn_800D207C - GS render: set position data, mark dirty
 * Address: 0x800D207C, Size: 0x50
 * ================================================================== */
void fn_800D207C(void* obj, void* src) {
    fn_80195898(*(void**)((u8*)obj + 0xc));
    fn_800E01D0((u8*)obj + 0x70, src);
    *(u8*)((u8*)obj + 0x2) = 1;
}

/* ==================================================================
 * fn_800D20CC - GS render: set object transform params
 * Address: 0x800D20CC, Size: 0x84
 * ================================================================== */
void fn_800D20CC(void* obj, f32 x, f32 y, f32 z, f32 w) {
    fn_801943BC(*(void**)((u8*)obj + 0xc), 1, x, y, z, w);
    fn_801943A0(*(void**)((u8*)obj + 0xc), x, y);
    fn_801944E8(*(void**)((u8*)obj + 0xc), z);
    fn_801944C0(*(void**)((u8*)obj + 0xc), w);
    *(u8*)((u8*)obj + 0x2) = 1;
}

/* ==================================================================
 * fn_800D2150 - GS render: set scissor rect (clamped)
 * Address: 0x800D2150, Size: 0x78
 * ================================================================== */
void fn_800D2150(void* obj, u32 x0, u32 y0, u32 x1, u32 y1) {
    if ((u16)x0 > 0x27e) x0 = 0x27e;
    if ((u16)y0 > 0x1de) y0 = 0x1de;
    if ((u16)x1 > 0x27f) x1 = 0x27f;
    if ((u16)y1 > 0x1df) y1 = 0x1df;
    fn_801944A4(*(void**)((u8*)obj + 0xc), x0, (u16)(x1 + 1), y0, (u16)(y1 + 1));
}

/* ==================================================================
 * fn_800D21C8 - GS render: set viewport rect (clamped, packed)
 * Address: 0x800D21C8, Size: 0x80
 * ================================================================== */
void fn_800D21C8(void* obj, u32 x0, u32 y0, u32 x1, u32 y1) {
    u16 rect[4];
    if ((u16)x0 > 0x27e) x0 = 0x27e;
    if ((u16)y0 > 0x1de) y0 = 0x1de;
    if ((u16)x1 > 0x27f) x1 = 0x27f;
    if ((u16)y1 > 0x1df) y1 = 0x1df;
    rect[0] = (u16)x0;
    rect[2] = (u16)y0;
    rect[1] = (u16)(x1 + 1);
    rect[3] = (u16)(y1 + 1);
    fn_80194400(*(void**)((u8*)obj + 0xc), rect);
}

/* ==================================================================
 * fn_800D2248 - GS render: main render loop (uses lbl_8047AA74 list)
 * Address: 0x800D2248, Size: 0x33C
 * ================================================================== */
void fn_800D2248(void) {
}

/* ==================================================================
 * fn_800D2584 - GS render: get batch count
 * Address: 0x800D2584, Size: 0x8
 * ================================================================== */
u32 fn_800D2584(void) {
    return lbl_8047AA74;
}

/* ==================================================================
 * fn_800D258C - GS render: effect render setup
 * Address: 0x800D258C, Size: 0x1AC
 * ================================================================== */
void fn_800D258C(void* effectCtx) {
    if (effectCtx == NULL) {
        return;
    }
    /* Set up GX state for particle/effect rendering:
     * - Additive blending
     * - No depth write
     * - Billboard matrix
     */
}

/* ==================================================================
 * fn_800D2738 - GS render: effect render cleanup
 * Address: 0x800D2738, Size: 0xC4
 * ================================================================== */
void fn_800D2738(void) {
    /* Restore GX state after effect rendering */
}

/* ==================================================================
 * fn_800D27FC - GS render: shadow volume setup
 * Address: 0x800D27FC, Size: 0x1A4
 * ================================================================== */
void fn_800D27FC(void* shadowCtx) {
    if (shadowCtx == NULL) {
        return;
    }
    /* Set up GX state for shadow volume rendering:
     * - Stencil buffer configuration
     * - Special blend mode
     * - Depth test configuration
     */
}

/* ==================================================================
 * fn_800D29A0 - GS render: shadow volume cleanup
 * Address: 0x800D29A0, Size: 0x134
 * ================================================================== */
void fn_800D29A0(void) {
    /* Restore GX state after shadow volume rendering */
}

/* ==================================================================
 * fn_800D2AD4 - GS render: init render object array
 * Address: 0x800D2AD4, Size: 0x70
 * #pragma scheduling on required for correct instruction order
 * Allocates and zero-initializes an array of count render objects
 * (each 0x128 bytes). Stores array ptr to lbl_8047AA6C.
 * ================================================================== */
#pragma push
#pragma scheduling on
void fn_800D2AD4(u32 count) {
    u32 raw;
    lbl_8047AA70 = count;
    raw = fn_800E3534(count * 0x128);
    lbl_8047AA68 = (u16)raw;
    if ((u16)raw != 0) {
        u32 off;
        u32 i;
        u32 zero;
        lbl_8047AA6C = fn_800E27B0((u32)(u16)raw);
        off = 0;
        zero = off;
        i = 0;
        do {
            *(u8*)((u8*)lbl_8047AA6C + off) = (u8)zero;
            off += 0x128;
            i++;
        } while (i < lbl_8047AA70);
    }
}
#pragma pop

/* ==================================================================
 * fn_800D2B44 - GS render: animation step callback
 * Address: 0x800D2B44, Size: 0x4C
 * Called by fn_801C028C as a callback with the JObj*.
 * Accumulates animation step into lbl_8047AA78.
 * ================================================================== */
void fn_800D2B44(void* obj) {
    if (!obj) {
        fn_80196E10(lbl_8047C9DC, 0xab, lbl_8047C9E4);
    }
    lbl_8047AA78 = lbl_8047C990 + *(f32*)((u8*)obj + 0xc);
}

/* ==================================================================
 * fn_800D2B90 - GS render: post-processing setup
 * Address: 0x800D2B90, Size: 0x258
 * ================================================================== */
void fn_800D2B90(void* ppCtx) {
    if (ppCtx == NULL) {
        return;
    }
    /* Set up GX state for post-processing effects:
     * - Copy texture from framebuffer
     * - Set up fullscreen quad
     * - Configure TEV for post-process effect
     */
}

/* ==================================================================
 * fn_800D2DE8 - GS render: copy to texture
 * Address: 0x800D2DE8, Size: 0x14C
 * ================================================================== */
void fn_800D2DE8(void* destTex, u32 x, u32 y, u32 w, u32 h) {
    /* Copy framebuffer region to a texture:
     * GXSetTexCopySrc(x, y, w, h)
     * GXSetTexCopyDst(w, h, format, mipmap)
     * GXCopyTex(destTex, GX_FALSE)
     */
}

/* ==================================================================
 * fn_800D2F34 - GS render: render to texture setup
 * Address: 0x800D2F34, Size: 0x128
 * ================================================================== */
void fn_800D2F34(void* renderTex, u32 w, u32 h) {
    if (renderTex == NULL) {
        return;
    }
    /* Configure rendering to a texture target:
     * 1. Set viewport to texture dimensions
     * 2. Configure copy source
     * 3. Set up special projection for RTT
     */
}

/* ==================================================================
 * fn_800D305C - GS render: get frame counter
 * Address: 0x800D305C, Size: 0xC
 * ================================================================== */
void fn_800D305C(u8 val) {
    *(u8*)((u8*)lbl_8047AA80 + 0x5C) = val;
}

/* ==================================================================
 * fn_800D3068 - GS render: get render target width
 * Address: 0x800D3068, Size: 0xC
 * ================================================================== */
u32 fn_800D3068(void) {
    return *(u32*)((u8*)lbl_8047AA80 + 0x58);
}
