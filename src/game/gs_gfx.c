/**
 * @file gs_gfx.c
 * @brief GSgfx -- Genius Sonority graphics subsystem.
 *
 * Only the fn_-named leaf functions below (fn_800D3094, fn_800D30A0,
 * fn_800D30AC, fn_800D3190, fn_800D3410, fn_800D36B4, fn_800D377C) are
 * real members of this unit; several are already byte-matched.
 *
 * This file previously also carried a second, parallel implementation of
 * the unit under invented friendly names (GSgfxInit, GSgfxSetVideoMode,
 * GSgfxEnableRendering, GSgfxSwapBuffers, GSgfxSetDrawMode,
 * GSgfxGetFrameCount, GSgfxGetTickCount), guarded by `#ifdef PCPORT`.
 * PCPORT is never defined anywhere in this build (no such target exists
 * in configure.py), so that whole block was dead code that could never
 * be compiled into any real target. None of the seven names appear in
 * config/GC6E01/symbols.txt, and their claimed sizes didn't consistently
 * match the real fn_ symbols living at those addresses (e.g. fn_800D3088
 * is really 0xC bytes, not the claimed 0x68; fn_800D30F0 is really 0xA0,
 * not the claimed 0xF0). It was fiction from the old campaign transplant
 * and has been deleted wholesale.
 *
 * Address range: 0x800D3074 - 0x800D3E4C (approx.)
 */

#include "dolphin/types.h"
#include "game/gs_gfx.h"

/* ===== Data referenced by the real fn_ functions below ===== */
extern u8 lbl_804001F0[];  /* GX state / FIFO state block */

#ifndef PCPORT
void fn_800D3074(u32 flag) {
    if (flag == 0) {
        return;
    }

    ((GSgfxState*)lbl_8047AA80)->renderEnabled = flag;
}

u32 fn_800D3088(void) {
    return lbl_8047AA80->frameDelta;
}

u32 fn_800D3094(void) {
    return *(u32*)((u8*)lbl_8047AA80 + 0x4C);
}
void fn_800D30A0(u32 val) {
    *(u32*)((u8*)lbl_8047AA80 + 0x48) = val;
}
extern void fn_800D4F98(s32 arg0, ...);
extern void GXFlush(void);
#if 0
asm void fn_800D30AC(void) {
#include "src/game/gs_gfx_fn_800D30AC.inc"
}
#else
void fn_800D30AC(void) {
    u8* state = (u8*)lbl_8047AA80;

    if (*(s32*)(state + 0x0) == 1) {
        fn_800D4F98(4, 0);
    } else {
        GXFlush();
    }
}
#endif
extern void GXSetDrawDone(void);
extern void OSYieldThread(void);
extern void fn_8019C6FC(void);
extern void GSgfxBackFBDoFrame(void);
extern void fn_801BF8A0(s32 a);
extern void fn_801E16F0(void);
extern void fn_801BF6AC(void);
extern void GStextureConvertFromHW(void* a, s32 b);
extern void fn_800B8E74(void);
extern void GXInvalidateTexAll(void);
extern void fn_800D1070(void* a);
extern void fn_800DC6D8(void* a);
extern void fn_800E3884(void* a, s32 b);
extern void fn_801181B0(s32 a);
extern u32 OSGetTick(void);
extern u8 lbl_8047AA91;
extern u8 lbl_8047AA90;
#if 0
asm void fn_800D3190(void) {
#include "src/game/gs_gfx_fn_800D3190.inc"
}
#else
void fn_800D3190(void) {
    u32* state;
    u32 sc;
    u32 startTick;
    u32* r31ptr;
    u8 r30;
    u32 tick;
    u32 div;
    s32 r29count;
    u8 r29b;
    u32* s4;
    u32 chk;

    state = (u32*)lbl_8047AA80;
    sc = state[0xC / 4];
    if ((u32)(sc + 0x01020000U) == 0xFEFEU) {
        return;
    }

    lbl_8047AA91 = 0;
    GXFlush();
    GXSetDrawDone();
    startTick = OSGetTick();
    r31ptr = (u32*)0x80000000;
    r30 = 1;

    while (lbl_8047AA91 == 0) {
        OSYieldThread();
        if (lbl_8047AA91 == 0) {
            tick = OSGetTick();
            div = (tick - startTick) / (r31ptr[0xF8 / 4] >> 2);
            if (div > 3) {
                lbl_8047AA91 = r30;
            }
        }
    }

    fn_8019C6FC();

    s4 = (u32*)lbl_8047AA80;
    sc = s4[0xC / 4];
    if (sc != 0) {
        chk = sc + 0x01020000U;
        r29b = 1;
        if (chk != 0xFEFEU) {
            if (((u8*)s4)[0x49D] == 0) {
                GSgfxBackFBDoFrame();
                fn_801BF8A0(0);
                fn_801E16F0();
                fn_801BF6AC();
            } else {
                if (sc != 0) {
                    GStextureConvertFromHW((void*)sc, 1);
                    r29b = 0;
                }
            }
            ((u8*)lbl_8047AA80)[0x49D] = 1;
            if (r29b != 0) {
                fn_800B8E74();
                if (*(u32*)((u8*)lbl_8047AA80 + 0xC) != 0) {
                    GXInvalidateTexAll();
                }
            }
        }
    } else {
        u32* p = (u32*)&lbl_804001F0;
        p[0x0 / 4] = p[0x4 / 4];
        p[0x8 / 4] = p[0xC / 4];
        p[0x10 / 4] += 1;
        p[0x4 / 4] = 0;
        p[0xC / 4] = 0;
    }

    *(u32*)((u8*)lbl_8047AA80 + 0xC) = 0xFEFEFEFEU;

    if (lbl_8047AA90 == 0) {
        return;
    }

    startTick = OSGetTick();
    fn_800D1070((void*)*(u32*)((u8*)lbl_8047AA80 + 0x54));
    tick = OSGetTick();
    ((u32*)&lbl_804001F0)[0x2C / 4] += tick - startTick;

    startTick = OSGetTick();
    fn_800DC6D8((void*)*(u32*)((u8*)lbl_8047AA80 + 0x54));
    tick = OSGetTick();
    ((u32*)&lbl_804001F0)[0x30 / 4] += tick - startTick;

    startTick = OSGetTick();
    fn_800E3884((void*)*(u32*)((u8*)lbl_8047AA80 + 0x54), 0);
    tick = OSGetTick();
    ((u32*)&lbl_804001F0)[0x34 / 4] += tick - startTick;

    r29count = *(s32*)((u8*)lbl_8047AA80 + 0x54);
    ((u32*)&lbl_804001F0)[0x38 / 4] = 0;

    while (r29count-- != 0) {
        startTick = OSGetTick();
        fn_800E3884((void*)1, 1);
        tick = OSGetTick();
        ((u32*)&lbl_804001F0)[0x34 / 4] += tick - startTick;

        startTick = OSGetTick();
        fn_801181B0(1);
        tick = OSGetTick();
        ((u32*)&lbl_804001F0)[0x38 / 4] += tick - startTick;
    }
}
#endif
extern void fn_800D13C4(void* a);
extern void fn_800DC874(void* a);
extern void fn_800E3928(void* a);
extern void fn_801183EC(void* a);
extern void modelShadowRender__FP10GSgfxLayer(void);
extern void fn_8019C708(s32 a);
extern void HSD_SetEraseColor(u8 a, u8 b, u8 c, u8 d);
extern void HSD_EraseRect(s32 a, s32 b, s32 c, f32 d, f32 e, f32 f, f32 g, f32 h);
extern u8 lbl_8047AA90;
extern f32 lbl_8047CA00;
extern f32 lbl_8047CA08;
extern f32 lbl_8047CA04;
#if 0
asm void fn_800D3410(void) {
#include "src/game/gs_gfx_fn_800D3410.inc"
}
#else
void fn_800D3410(void* arg0, u8 arg1) {
    u32* state;
    u32 sc;
    u8 r30;
    u32 startTick;
    u32 tick;

    state = (u32*)lbl_8047AA80;
    sc = state[0xC / 4];

    if ((u32)(sc + 0x01020000U) == 0xFEFEU) {
        /* sentinel matched — store arg0 into state->0xC and process sub-block */
        state[0xC / 4] = (u32)arg0;
        r30 = 0;

        {
            u32* s4 = (u32*)lbl_8047AA80;
            sc = s4[0xC / 4];
            if ((u32)(sc + 0x01020000U) != 0xFEFEU) {
                if (((u8*)s4)[0x49D] == 0) {
                    GSgfxBackFBDoFrame();
                    fn_801BF8A0(0);
                    fn_801E16F0();
                    fn_801BF6AC();
                } else {
                    if (sc != 0) {
                        GStextureConvertFromHW((void*)sc, 1);
                        r30 = 0;
                    }
                }
                ((u8*)lbl_8047AA80)[0x49D] = 1;
                if (r30 != 0) {
                    fn_800B8E74();
                    if (*(u32*)((u8*)lbl_8047AA80 + 0xC) != 0) {
                        GXInvalidateTexAll();
                    }
                }
            }
        }

    /* store arg1 into lbl_8047AA90 (SDA21 var) */
    lbl_8047AA90 = arg1;

    if (arg1 != 0) {
        /* reload state sub ptr and timing base inline each call (matches target) */
        startTick = OSGetTick();
        fn_800D13C4((void*)*(u32*)((u8*)lbl_8047AA80 + 0x54));
        tick = OSGetTick();
        ((u32*)&lbl_804001F0)[0x2C / 4] = tick - startTick;

        startTick = OSGetTick();
        fn_800DC874((void*)*(u32*)((u8*)lbl_8047AA80 + 0x54));
        tick = OSGetTick();
        ((u32*)&lbl_804001F0)[0x30 / 4] = tick - startTick;

        startTick = OSGetTick();
        fn_800E3928((void*)*(u32*)((u8*)lbl_8047AA80 + 0x54));
        tick = OSGetTick();
        ((u32*)&lbl_804001F0)[0x34 / 4] = tick - startTick;

        startTick = OSGetTick();
        fn_801183EC((void*)*(u32*)((u8*)lbl_8047AA80 + 0x54));
        tick = OSGetTick();
        ((u32*)&lbl_804001F0)[0x38 / 4] = tick - startTick;
    }

    if (arg0 == 0) {
        startTick = OSGetTick();
        modelShadowRender__FP10GSgfxLayer();
        tick = OSGetTick();
        ((u32*)&lbl_804001F0)[0x3C / 4] = tick - startTick;

        fn_8019C708(0);

        if (((u8*)lbl_8047AA80)[0x19] != 0) {
            HSD_SetEraseColor(
                ((u8*)lbl_8047AA80)[0x1C],
                ((u8*)lbl_8047AA80)[0x1D],
                ((u8*)lbl_8047AA80)[0x1E],
                ((u8*)lbl_8047AA80)[0x1F]
            );
            HSD_EraseRect(1, 1, 0,
                lbl_8047CA00,
                lbl_8047CA08,
                lbl_8047CA00,
                lbl_8047CA04,
                lbl_8047CA00);
        }

        ((u8*)lbl_8047AA80)[0x49D] = 0;
    } else {
        fn_8019C708(3);
    }
    }
}
#endif

extern void VIWaitForRetrace(void);
void fn_800D361C(u8 mode) {
    u32 diff;

    VIWaitForRetrace();
    diff = ((GSgfxState*)lbl_8047AA80)->xfbCount -
           ((GSgfxState*)lbl_8047AA80)->xfbAddr0;
    ((GSgfxState*)lbl_8047AA80)->frameDelta = diff;

    if ((mode == 1) != 0) {
        while (((GSgfxState*)lbl_8047AA80)->frameDelta <
               ((GSgfxState*)lbl_8047AA80)->renderEnabled) {
            VIWaitForRetrace();
            diff = ((GSgfxState*)lbl_8047AA80)->xfbCount -
                   ((GSgfxState*)lbl_8047AA80)->xfbAddr0;
            ((GSgfxState*)lbl_8047AA80)->frameDelta = diff;
        }

        if (((GSgfxState*)lbl_8047AA80)->vsyncFlag == 0) {
            ((GSgfxState*)lbl_8047AA80)->frameDelta =
                ((GSgfxState*)lbl_8047AA80)->renderEnabled;
        }
    }

    ((GSgfxState*)lbl_8047AA80)->xfbAddr0 =
        ((GSgfxState*)lbl_8047AA80)->xfbCount;
}

extern f32 lbl_8047C9F0;
#pragma push
#pragma peephole off
void fn_800D36B4(f32* in) {
    f32 s = lbl_8047C9F0;
    s32 b0 = (s32)(s * in[0]);
    s32 b1 = (s32)(s * in[1]);
    s32 b2 = (s32)(s * in[2]);
    s32 b3 = (s32)(s * in[3]);
    ((u8*)lbl_8047AA80)[0x19] = 1;
    ((u8*)lbl_8047AA80)[0x1c] = b0;
    ((u8*)lbl_8047AA80)[0x1d] = b1;
    ((u8*)lbl_8047AA80)[0x1e] = b2;
    ((u8*)lbl_8047AA80)[0x1f] = b3;
    if (((u8*)lbl_8047AA80)[0x1c] == 0 && ((u8*)lbl_8047AA80)[0x1d] == 0 &&
        ((u8*)lbl_8047AA80)[0x1e] == 0 && ((u8*)lbl_8047AA80)[0x1f] == 0) {
        ((u8*)lbl_8047AA80)[0x19] = 0;
    }
}
#pragma pop
extern void fn_8019C690();
void fn_800D377C(s32 mode) {
    switch (mode) {
    case 1:
        fn_8019C690(0, 0);
        break;
    case 2:
        fn_8019C690(1, 0);
        break;
    }
}

/* Initialise the GS graphics core and its default render state. */
void GSgfxInit__FP15_GSgfxInitParms(u32 heapSize, u32 matrixSize,
                                    u32 projectionCount, u32 lightCount,
                                    s32 videoMode, u32 displayListSize)
{
    extern void GSmathInit(void);
    extern void _gfxScratchNotify__F15GSscratchNotifyPvUc(void);
    extern void* GSscratchAlloc(u32, void*);
    extern u16 _toolentryAlloc__FUl(u32);
    extern void* fn_800E27B0(u16);
    extern void GSlogWrite(const char*, ...);
    extern void fn_8019C3C4(s32, ...);
    extern void fn_8019CB70(void);
    extern void fn_800D37D4(u32, u32, u32, u32, u32, u32);
    extern void fn_801BF4C4(u32);
    extern void fn_8019C690(u32, u32);
    extern void fn_801C021C(void*);
    extern void fn_801C01C8(void*);
    extern void fn_80196C3C(void*);
    extern void fn_800D3F5C(void);
    extern void fn_800D3F50(void);
    extern void fn_800D3EC4(void);
    extern void fn_800D5504(u32);
    extern void fn_800D83E4(u32);
    extern void fn_800D7B80(u32);
    extern void fn_800DB890(u32);
    extern u32 fn_800D7894(void);
    extern void fn_800D9D68(u32, u32, u32, u32);
    extern void fn_800D9C24(u32, u32, u32, u32);
    extern void fn_800D87AC(s32);
    extern void fn_800DA2BC(u32, u32, u32);
    extern void fn_800DA1E8(u32, u32, u32);
    extern void fn_800DA100(u32, u32, u32, u32, u32, u32);
    extern void fn_800DA028(u32);
    extern void fn_800D9F40(u32);
    extern void fn_800DA08C(u32);
    extern void fn_800D9ED8(u32);
    extern void fn_800DC224(u32, u32, u32, u32, u32);
    extern void fn_800D9BD0(f32, f32, f32, f32);
    extern void GSgfxBackFBInit__Fv(void);
    extern u32 lbl_8047AAA0;
    extern u8 lbl_8047AA9C;
    extern u32 lbl_8047AA8C;
    extern u8 lbl_8047AA84;
    extern u8 lbl_8047AA85;
    extern u8 lbl_80312D30[];
    extern u8 lbl_803130F0[];
    extern u8 lbl_80312F4C[];
    extern u8 lbl_80466BC0[];
    extern char lbl_80270360[];
    extern char lbl_80270388[];
    extern f32 lbl_8047CA10;
    extern f32 lbl_8047CA14;
    extern f32 lbl_8047CA18;
    extern f32 lbl_8047CA1C;

    u16 handle;
    /* All known callers pass modes 1-4; the target has no default assignment. */
    void* renderMode;
    u32 previousMode;
    s32 i;
    u8* display;
    u32* fifoState;
    u8* state;

    GSmathInit();
    state = GSscratchAlloc(3, _gfxScratchNotify__F15GSscratchNotifyPvUc);
    if (state == 0) {
        handle = _toolentryAlloc__FUl(0x5A0);
        if (handle == 0) {
            GSlogWrite(lbl_80270360);
            return;
        }
        state = fn_800E27B0(handle);
    }

    lbl_8047AA80 = (GSgfxState*)state;
    *(u32*)(state + 0x000) = 2;
    *(s32*)((u8*)lbl_8047AA80 + 0x004) = -1;
    *(u32*)((u8*)lbl_8047AA80 + 0x008) = 0x10;
    *(u32*)((u8*)lbl_8047AA80 + 0x00C) = 0xFEFEFEFE;
    *(u32*)((u8*)lbl_8047AA80 + 0x010) = 0;
    *(u32*)((u8*)lbl_8047AA80 + 0x014) = 3;
    *(u8*)((u8*)lbl_8047AA80 + 0x018) = 0;
    *(u8*)((u8*)lbl_8047AA80 + 0x019) = 0;
    *(u8*)((u8*)lbl_8047AA80 + 0x01A) = 0;
    *(u8*)((u8*)lbl_8047AA80 + 0x01B) = 0;
    *(u8*)((u8*)lbl_8047AA80 + 0x49C) = 0;
    *(u8*)((u8*)lbl_8047AA80 + 0x49D) = 0;
    *(u32*)((u8*)lbl_8047AA80 + 0x020) = 0;
    *(u32*)((u8*)lbl_8047AA80 + 0x024) = 0;
    *(u32*)((u8*)lbl_8047AA80 + 0x028) = 0;
    *(u32*)((u8*)lbl_8047AA80 + 0x02C) = 0;
    *(u32*)((u8*)lbl_8047AA80 + 0x030) = 0;
    *(u32*)((u8*)lbl_8047AA80 + 0x034) = 0;
    *(u32*)((u8*)lbl_8047AA80 + 0x038) = 0;
    *(u32*)((u8*)lbl_8047AA80 + 0x03C) = 0;
    *(u32*)((u8*)lbl_8047AA80 + 0x040) = 0;
    *(u32*)((u8*)lbl_8047AA80 + 0x044) = 0;
    *(u8*)((u8*)lbl_8047AA80 + 0x47E) = 0;
    *(u32*)((u8*)lbl_8047AA80 + 0x480) = 0;
    *(u32*)((u8*)lbl_8047AA80 + 0x484) = 0;
    *(s32*)((u8*)lbl_8047AA80 + 0x488) = -1;
    *(u8*)((u8*)lbl_8047AA80 + 0x49F) = 0;

    fifoState = (u32*)lbl_804001F0;
    fifoState[0] = 0;
    fifoState[1] = 0;
    fifoState[2] = 0;
    fifoState[3] = 0;
    fifoState[4] = 0;
    fifoState[5] = 0;
    fifoState[6] = 0;
    fifoState[7] = 0;
    fifoState[8] = 0;
    fifoState[9] = 0;
    fifoState[10] = 0;
    fifoState[11] = 0;
    fifoState[12] = 0;
    fifoState[13] = 0;
    fifoState[14] = 0;
    fifoState[15] = 0;
    fifoState[16] = 0;
    fifoState[17] = 0;
    fifoState[18] = 0;
    fifoState[19] = 0;
    fifoState[20] = 0;
    fifoState[21] = 0;

    lbl_8047AAA0 = displayListSize;
    switch (videoMode) {
    case 1:
        renderMode = lbl_80312D30;
        break;
    case 2:
    case 3:
        renderMode = lbl_803130F0;
        break;
    case 4:
        renderMode = lbl_80312F4C;
        break;
    default:
        break;
    }
    if (videoMode == 2 || videoMode == 3) {
        lbl_8047AA9C = 1;
    } else {
        lbl_8047AA9C = 0;
    }

    fn_8019C3C4(1, 2);
    fn_8019C3C4(4, renderMode);
    fn_8019CB70();
    fn_800D37D4(videoMode, 2, 0, 2, 0, 0);
    fn_801BF4C4(1);
    fn_8019C690(0, 0);

    *(u32*)((u8*)lbl_8047AA80 + 0x048) = 0;
    *(u32*)((u8*)lbl_8047AA80 + 0x04C) = 0;
    *(u32*)((u8*)lbl_8047AA80 + 0x050) = 0;
    *(u32*)((u8*)lbl_8047AA80 + 0x054) = 0;
    *(u32*)((u8*)lbl_8047AA80 + 0x058) = 1;
    *(u8*)((u8*)lbl_8047AA80 + 0x05C) = 1;
    fn_801C021C(fn_800D3F5C);
    fn_801C01C8(fn_800D3F50);
    fn_80196C3C(fn_800D3EC4);

    fn_800D5504(heapSize);
    fn_800D83E4(matrixSize);
    fn_800D7B80(projectionCount);
    fn_800DB890(lightCount);
    *(u32*)((u8*)lbl_8047AA80 + 0x20) = fn_800D7894();

    display = lbl_80466BC0;
    previousMode = *(u32*)((u8*)lbl_8047AA80 + 0x00);
    *(u32*)((u8*)lbl_8047AA80 + 0x00) = 2;
    fn_800D9D68(0, 0, (u16)(*(u16*)(display + 4) - 1),
                 (u16)(*(u16*)(display + 6) - 1));
    fn_800D9C24(0, 0, (u16)(*(u16*)(display + 4) - 1),
                 (u16)(*(u16*)(display + 6) - 1));
    fn_800D87AC(-1);
    fn_800DA2BC(1, 1, 1);
    fn_800DA1E8(1, 2, 1);
    fn_800DA100(0, 7, 0, 1, 7, 0);
    fn_800DA028(2);
    fn_800D9F40(0);
    fn_800DA08C(1);
    fn_800D9ED8(0);

    for (i = 0; i < 0x10; i++) {
        fn_800DC224(i, 0, i, i, 0);
    }
    fn_800D9BD0(lbl_8047CA10, lbl_8047CA14,
                lbl_8047CA18, lbl_8047CA1C);
    state = (u8*)lbl_8047AA80;
    *(u32*)(state + 0x00) = previousMode;
    lbl_8047AA8C = 0;
    *(u32*)((u8*)lbl_8047AA80 + 0x00) = 1;
    GSgfxBackFBInit__Fv();
    fn_801BF4C4(0);
    lbl_8047AA84 = 1;
    lbl_8047AA85 = 0;
    GSlogWrite(lbl_80270388, lbl_8047AA80, 0x5A0);
}
#endif /* !PCPORT */
