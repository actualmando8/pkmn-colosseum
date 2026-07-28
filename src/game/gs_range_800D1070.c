/**
 * @file gs_range_800D1070.c
 * @brief gs-engine code, 0x800D1070 - 0x800D13C4 (1 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

extern u32 lbl_8047AA6C;
extern u32 lbl_8047AA70;
extern f32 lbl_8047C990;
extern f32 lbl_8047C994;
extern f32 lbl_8047C998;
extern f32 lbl_80478ACC;
extern double lbl_8047C9A0;
extern void fn_800E01D0(void*, void*);
extern void fn_800E0168(void*, void*, void*);
extern void fn_800E0628(void*, void*);
extern void fn_800E0218(void*, void*, void*, void*);
extern void fn_800E053C(void*, f32);
extern void fn_800E0518(void*, f32);
extern void fn_800E04F4(void*, f32);
extern void fn_800E05C0(void*, f32, f32, f32);
extern void fn_800E0290(void*, void*, void*);
extern void HSD_CObjReqAnim(void*, f32);
extern void HSD_CObjAnim(void*);
extern void HSD_CObjGetEyePosition(void*, void*);
extern void HSD_CObjGetUpVector(void*, void*);
extern void HSD_CObjGetInterest(void*, void*);
extern void* HSD_CObjGetViewingMtxPtr(void*);

void fn_800D1070(u32 ticks)
{
    u32 offset;
    u32 index;

    offset = 0;
    for (index = 0; index < lbl_8047AA70; index++, offset += 0x128) {
        u8* object = (u8*)lbl_8047AA6C + offset;
        if (object[0] == 1) {
            if (object[4] == 1) {
                s8 direction;
                f32 speed;
                f32 lastFrame;
                f32 rate;
                s32 mode;
                f32 step;
                f32 threshold;

                fn_800E01D0(object + 0x7C, object + 0x70);
                HSD_CObjReqAnim(*(void**)(object + 0xC),
                                *(f32*)(object + 0x11C));
                HSD_CObjAnim(*(void**)(object + 0xC));
                HSD_CObjGetEyePosition(*(void**)(object + 0xC),
                                       object + 0x70);
                HSD_CObjGetUpVector(*(void**)(object + 0xC),
                                    object + 0xF4);
                HSD_CObjGetInterest(*(void**)(object + 0xC),
                                    object + 0x100);

                direction = *(s8*)(object + 0x125);
                rate = *(f32*)(object + 0x118);
                lastFrame = *(f32*)(object + 0x120);
                speed = *(f32*)(object + 0x11C);
                mode = *(s32*)(object + 0x10C);
                step = rate * (f32)ticks;
                threshold = lastFrame - lbl_8047C990;

                if (direction == -1) {
                    *(f32*)(object + 0x11C) = speed - step;
                } else if (direction == 1) {
                    *(f32*)(object + 0x11C) = speed + step;
                }
                speed = *(f32*)(object + 0x11C);
                if (mode == 1) {
                    if (speed >= threshold) {
                        *(s8*)(object + 0x125) = -1;
                    } else if (speed <= lbl_8047C998) {
                        *(s8*)(object + 0x125) = 1;
                    }
                } else if (mode == 0) {
                    threshold -= lbl_8047C994;
                    if (speed >= threshold) {
                        object[0x124] = 1;
                        *(s8*)(object + 0x125) = 0;
                        *(f32*)(object + 0x11C) = threshold;
                    }
                }
                object[2] = 1;
            }

            if (object[4] != 0) {
                fn_800E0628(object + 0x94,
                            HSD_CObjGetViewingMtxPtr(*(void**)(object + 0xC)));
                HSD_CObjGetEyePosition(*(void**)(object + 0xC),
                                       object + 0x70);
                HSD_CObjGetUpVector(*(void**)(object + 0xC),
                                    object + 0xF4);
                HSD_CObjGetInterest(*(void**)(object + 0xC),
                                    object + 0x100);
            } else if (object[1] == 1) {
                f32 delta[3];
                f32 value;
                fn_800E0168(delta, object + 0x70, object + 0x100);
                if ((delta[0] < 0 ? -delta[0] : delta[0]) < lbl_80478ACC
                 && (delta[1] < 0 ? -delta[1] : delta[1]) < lbl_80478ACC
                 && (delta[2] < 0 ? -delta[2] : delta[2]) < lbl_80478ACC) {
                    value = *(f32*)(object + 0x100);
                    *(f32*)(object + 0x100) = (f32)(value + lbl_8047C9A0);
                }
                fn_800E0218(object + 0x94, object + 0x70, object + 0xF4,
                            object + 0x100);
            } else {
                f32 x[3][4];
                f32 y[3][4];
                f32 translation[3][4];
                fn_800E053C(object + 0x94, -*(f32*)(object + 0x88));
                fn_800E0518(x, -*(f32*)(object + 0x8C));
                fn_800E04F4(y, -*(f32*)(object + 0x90));
                fn_800E05C0(translation, -*(f32*)(object + 0x70),
                            -*(f32*)(object + 0x74),
                            -*(f32*)(object + 0x78));
                fn_800E0290(object + 0x94, object + 0x94, x);
                fn_800E0290(object + 0x94, object + 0x94, y);
                fn_800E0290(object + 0x94, object + 0x94, translation);
            }
            object[2] = 0;
        }
    }
}
