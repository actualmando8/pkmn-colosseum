/**
 * @file gs_range_8008C7B0.c
 * @brief gs-engine code, 0x8008C7B0 - 0x800980E0 (84 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"
#include "game/gs_material.h"

extern u32 lbl_8047A690;
extern u32 lbl_8047A694;
extern f32 lbl_8047C1D0; /* 0.833333313f -- PAL-adjusted 1-unit wait */
extern f32 lbl_8047C1D4; /* 0.0f */
extern f32 lbl_8047C1D8; /* 1.0f */
extern f32 lbl_8047C1DC; /* 83.3333282f -- PAL-adjusted 100-unit wait */
extern f32 lbl_8047C1E0; /* {41.6666641f, 0.0f} -- PAL-adjusted 50-unit wait */

/* 0x80091564 | size: 0x210 */
void fn_80091564(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void fn_800E8FA0(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void fn_800E3C08(u32 handle, u32 val);
    extern void fn_800E3C00(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 fn_800E9108(u32 handle, u32 val);
    extern void fn_800E8FE8(u32 handle, u32 val);
    extern void fn_800E900C(u32 handle, u32 val, u32 *param);
    extern void fn_80176E0C(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void fn_8011288C(u32 a, u32 b);

    u32 waitFrames;
    u32 elapsed;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x06DC1605);
    lbl_8047A694 = GSresGetResource(ctx, 0x06DC1001);
    fn_800E8FA0(0x280, 0x1E0);
    fn_801CB7C4(0x06DC1000);

    fn_800E3C08(GSresGetResource(ctx, 0x06DC1000), GSresGetResource(ctx, 0x11261400));
    fn_800E3C00(GSresGetResource(ctx, 0x06DC1000), 4);

    fn_801CB834(0x06DC1000, 4, 0, 0);
    waitFrames = 100;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1DC;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    iconHandle = fn_801CBA0C(0x06BD0400);
    fn_801CB834(iconHandle, 7, 0, 0);

    iconResult = GSresGetResource(ctx, iconHandle);
    fn_800E9108(iconResult, 2);
    fn_800E8FE8(iconResult, lbl_8047A690);
    fn_800E900C(iconResult, 1, &lbl_8047A694);

    fn_80176E0C(ctx, 0x0C421800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0C3D1000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x81);
    fn_8011288C(0, 0);
}

/* 0x80091774 | size: 0x210 */
void fn_80091774(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void fn_800E8FA0(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void fn_800E3C08(u32 handle, u32 val);
    extern void fn_800E3C00(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 fn_800E9108(u32 handle, u32 val);
    extern void fn_800E8FE8(u32 handle, u32 val);
    extern void fn_800E900C(u32 handle, u32 val, u32 *param);
    extern void fn_80176E0C(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void fn_8011288C(u32 a, u32 b);

    u32 waitFrames;
    u32 elapsed;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x06DC1605);
    lbl_8047A694 = GSresGetResource(ctx, 0x06DC1001);
    fn_800E8FA0(0x280, 0x1E0);
    fn_801CB7C4(0x06DC1000);

    fn_800E3C08(GSresGetResource(ctx, 0x06DC1000), GSresGetResource(ctx, 0x11251400));
    fn_800E3C00(GSresGetResource(ctx, 0x06DC1000), 4);

    fn_801CB834(0x06DC1000, 3, 0, 0);
    waitFrames = 100;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1DC;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    iconHandle = fn_801CBA0C(0x06BD0400);
    fn_801CB834(iconHandle, 6, 0, 0);

    iconResult = GSresGetResource(ctx, iconHandle);
    fn_800E9108(iconResult, 2);
    fn_800E8FE8(iconResult, lbl_8047A690);
    fn_800E900C(iconResult, 1, &lbl_8047A694);

    fn_80176E0C(ctx, 0x0C411800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0C3C1000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x82);
    fn_8011288C(0, 0);
}

/* 0x80091984 | size: 0x210 */
void fn_80091984(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void fn_800E8FA0(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void fn_800E3C08(u32 handle, u32 val);
    extern void fn_800E3C00(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 fn_800E9108(u32 handle, u32 val);
    extern void fn_800E8FE8(u32 handle, u32 val);
    extern void fn_800E900C(u32 handle, u32 val, u32 *param);
    extern void fn_80176E0C(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void fn_8011288C(u32 a, u32 b);

    u32 waitFrames;
    u32 elapsed;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x06DC1605);
    lbl_8047A694 = GSresGetResource(ctx, 0x06DC1001);
    fn_800E8FA0(0x280, 0x1E0);
    fn_801CB7C4(0x06DC1000);

    fn_800E3C08(GSresGetResource(ctx, 0x06DC1000), GSresGetResource(ctx, 0x11241400));
    fn_800E3C00(GSresGetResource(ctx, 0x06DC1000), 4);

    fn_801CB834(0x06DC1000, 2, 0, 0);
    waitFrames = 100;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1DC;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    iconHandle = fn_801CBA0C(0x06BD0400);
    fn_801CB834(iconHandle, 5, 0, 1);

    iconResult = GSresGetResource(ctx, iconHandle);
    fn_800E9108(iconResult, 2);
    fn_800E8FE8(iconResult, lbl_8047A690);
    fn_800E900C(iconResult, 1, &lbl_8047A694);

    fn_80176E0C(ctx, 0x0C401800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0C3B1000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x82);
    fn_8011288C(0, 0);
}

/* 0x80091B94 | size: 0x210 */
void fn_80091B94(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void fn_800E8FA0(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void fn_800E3C08(u32 handle, u32 val);
    extern void fn_800E3C00(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 fn_800E9108(u32 handle, u32 val);
    extern void fn_800E8FE8(u32 handle, u32 val);
    extern void fn_800E900C(u32 handle, u32 val, u32 *param);
    extern void fn_80176E0C(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void fn_8011288C(u32 a, u32 b);

    u32 waitFrames;
    u32 elapsed;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x06DC1605);
    lbl_8047A694 = GSresGetResource(ctx, 0x06DC1001);
    fn_800E8FA0(0x280, 0x1E0);
    fn_801CB7C4(0x06DC1000);

    fn_800E3C08(GSresGetResource(ctx, 0x06DC1000), GSresGetResource(ctx, 0x11221400));
    fn_800E3C00(GSresGetResource(ctx, 0x06DC1000), 4);

    fn_801CB834(0x06DC1000, 0, 0, 0);
    waitFrames = 100;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1DC;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    iconHandle = fn_801CBA0C(0x06BD0400);
    fn_801CB834(iconHandle, 1, 0, 0);

    iconResult = GSresGetResource(ctx, iconHandle);
    fn_800E9108(iconResult, 2);
    fn_800E8FE8(iconResult, lbl_8047A690);
    fn_800E900C(iconResult, 1, &lbl_8047A694);

    fn_80176E0C(ctx, 0x0C3E1800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x10491000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x82);
    fn_8011288C(0, 0);
}

/* 0x8008CACC | size: 0x30C */
void fn_8008CACC(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void fn_800E8FA0(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void fn_800E3C08(u32 handle, u32 val);
    extern void fn_800E3C00(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 fn_800E9108(u32 handle, u32 val);
    extern void fn_800E8FE8(u32 handle, u32 val);
    extern void fn_800E900C(u32 handle, u32 val, u32 *param);
    extern void fn_80176E0C(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void fn_8011288C(u32 a, u32 b);
    extern void fn_80118874(void *texture, u32 flag);
    extern void fn_800ECCA8(u32 handle, u32 val);
    extern void GSmodelGetFrameCount(u32 handle, f32 *out, u32 flag);
    extern void fn_800ECA78(u32 handle, f32 val);
    extern void fn_800ECB74(u32 handle, u32 val);
    extern void fn_800EC990(u32 handle);

    u32 waitFrames;
    u32 elapsed;
    GSmaterialEntry *material;
    f32 frame;
    u32 handle2;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x0CE61602);
    lbl_8047A694 = GSresGetResource(ctx, 0x0CE61002);
    fn_800E8FA0(0x280, 0x1E0);
    fn_801CB834(0x0CE61000, 0, 0, 0);

    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    fn_801CB7C4(0x0CE61000);

    material = (GSmaterialEntry *)GSresGetResource(ctx, 0x0CE61000);
    fn_80118874(material->texture, 1);
    material->texture = NULL;

    frame = lbl_8047C1D4;
    handle2 = GSresGetResource(ctx, 0x0CE61004);
    fn_800ECCA8(handle2, 0);
    GSmodelGetFrameCount(handle2, &frame, 0);
    frame = frame - lbl_8047C1D8;
    fn_800ECCA8(handle2, 0);
    fn_800ECA78(handle2, frame);
    fn_800ECB74(handle2, 0);
    fn_800EC990(handle2);

    fn_800E3C08(GSresGetResource(ctx, 0x0CE61000), GSresGetResource(ctx, 0x11211400));
    fn_800E3C00(GSresGetResource(ctx, 0x0CE61000), 4);

    fn_801CB834(0x0CE61000, 2, 0, 0);
    waitFrames = 100;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1DC;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    iconHandle = fn_801CBA0C(0x06BC0400);
    iconResult = GSresGetResource(ctx, iconHandle);
    fn_800E9108(iconResult, 2);
    fn_800E8FE8(iconResult, lbl_8047A690);
    fn_800E900C(iconResult, 1, &lbl_8047A694);

    fn_80176E0C(ctx, 0x0D021800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0D0C1000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);
    fn_801CB834(iconHandle, 4, 0, 1);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x89);
    fn_8011288C(0, 0);
}

/* 0x8008FE94 | size: 0x26C */
void fn_8008FE94(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void fn_800E8FA0(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 fn_800E9108(u32 handle, u32 val);
    extern void fn_800E8FE8(u32 handle, u32 val);
    extern void fn_800E900C(u32 handle, u32 val, u32 *param);
    extern void fn_80176E0C(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void fn_8011288C(u32 a, u32 b);
    extern void fn_80118874(void *texture, u32 flag);
    extern void fn_800ECCA8(u32 handle, u32 val);
    extern void GSmodelGetFrameCount(u32 handle, f32 *out, u32 flag);
    extern void fn_800ECA78(u32 handle, f32 val);
    extern void fn_800ECB74(u32 handle, u32 val);
    extern void fn_800EC990(u32 handle);

    u32 waitFrames;
    u32 elapsed;
    GSmaterialEntry *material;
    f32 frame;
    u32 handle2;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x0CE61602);
    lbl_8047A694 = GSresGetResource(ctx, 0x0CE61002);
    fn_800E8FA0(0x280, 0x1E0);
    fn_801CB834(0x0CE61000, 0, 0, 0);

    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    fn_801CB7C4(0x0CE61000);

    material = (GSmaterialEntry *)GSresGetResource(ctx, 0x0CE61000);
    fn_80118874(material->texture, 1);
    material->texture = NULL;

    frame = lbl_8047C1D4;
    handle2 = GSresGetResource(ctx, 0x0CE61004);
    fn_800ECCA8(handle2, 0);
    GSmodelGetFrameCount(handle2, &frame, 0);
    frame = frame - lbl_8047C1D8;
    fn_800ECCA8(handle2, 0);
    fn_800ECA78(handle2, frame);
    fn_800ECB74(handle2, 0);
    fn_800EC990(handle2);

    iconHandle = fn_801CBA0C(0x06BD0400);
    iconResult = GSresGetResource(ctx, iconHandle);
    fn_800E9108(iconResult, 2);
    fn_800E8FE8(iconResult, lbl_8047A690);
    fn_800E900C(iconResult, 1, &lbl_8047A694);

    fn_80176E0C(ctx, 0x0CF41800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0CEB1000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);
    fn_801CB834(iconHandle, 2, 0, 1);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x89);
    fn_8011288C(0, 0);
}

/* 0x8008CDD8 | size: 0x2C8 */
void fn_8008CDD8(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void fn_800E8FA0(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void fn_800E3C08(u32 handle, u32 val);
    extern void fn_800E3C00(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern void scriptWaitSyncMotion(u32 id, u32 val);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 fn_800E9108(u32 handle, u32 val);
    extern void fn_800E8FE8(u32 handle, u32 val);
    extern void fn_800E900C(u32 handle, u32 val, u32 *param);
    extern void fn_80176E0C(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void fn_8011288C(u32 a, u32 b);
    extern void fn_80118874(void *texture, u32 flag);
    extern void fn_800ECCA8(u32 handle, u32 val);
    extern void GSmodelGetFrameCount(u32 handle, f32 *out, u32 flag);
    extern void fn_800ECA78(u32 handle, f32 val);
    extern void fn_800ECB74(u32 handle, u32 val);
    extern void fn_800EC990(u32 handle);

    u32 waitFrames;
    u32 elapsed;
    GSmaterialEntry *material;
    f32 frame;
    u32 handle2;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x0CE61602);
    lbl_8047A694 = GSresGetResource(ctx, 0x0CE61002);
    fn_800E8FA0(0x280, 0x1E0);
    fn_801CB834(0x0CE61000, 0, 0, 0);

    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    fn_801CB7C4(0x0CE61000);

    material = (GSmaterialEntry *)GSresGetResource(ctx, 0x0CE61000);
    fn_80118874(material->texture, 1);
    material->texture = NULL;

    frame = lbl_8047C1D4;
    handle2 = GSresGetResource(ctx, 0x0CE61004);
    fn_800ECCA8(handle2, 0);
    GSmodelGetFrameCount(handle2, &frame, 0);
    frame = frame - lbl_8047C1D8;
    fn_800ECCA8(handle2, 0);
    fn_800ECA78(handle2, frame);
    fn_800ECB74(handle2, 0);
    fn_800EC990(handle2);

    iconHandle = fn_801CBA0C(0x06AF0400);
    iconResult = GSresGetResource(ctx, iconHandle);
    fn_800E9108(iconResult, 2);
    fn_800E8FE8(iconResult, lbl_8047A690);
    fn_800E900C(iconResult, 1, &lbl_8047A694);

    fn_800E3C08(GSresGetResource(ctx, iconHandle), GSresGetResource(ctx, 0x11511400));
    fn_800E3C00(GSresGetResource(ctx, iconHandle), 4);

    fn_80176E0C(ctx, 0x0D011800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0D0B1000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);
    fn_801CB834(iconHandle, 0xB, 0, 0);
    scriptWaitSyncMotion(iconHandle, 1);
    fn_801CB834(iconHandle, 0xC, 0, 0);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x89);
    fn_8011288C(0, 0);
}

/* 0x8008D0A0 | size: 0x2A8 */
void fn_8008D0A0(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void fn_800E8FA0(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void fn_800E3C08(u32 handle, u32 val);
    extern void fn_800E3C00(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 fn_800E9108(u32 handle, u32 val);
    extern void fn_800E8FE8(u32 handle, u32 val);
    extern void fn_800E900C(u32 handle, u32 val, u32 *param);
    extern void fn_80176E0C(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void fn_8011288C(u32 a, u32 b);
    extern void fn_80118874(void *texture, u32 flag);
    extern void fn_800ECCA8(u32 handle, u32 val);
    extern void GSmodelGetFrameCount(u32 handle, f32 *out, u32 flag);
    extern void fn_800ECA78(u32 handle, f32 val);
    extern void fn_800ECB74(u32 handle, u32 val);
    extern void fn_800EC990(u32 handle);

    u32 waitFrames;
    u32 elapsed;
    GSmaterialEntry *material;
    f32 frame;
    u32 handle2;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x0CE61602);
    lbl_8047A694 = GSresGetResource(ctx, 0x0CE61002);
    fn_800E8FA0(0x280, 0x1E0);
    fn_801CB834(0x0CE61000, 0, 0, 0);

    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    fn_801CB7C4(0x0CE61000);

    material = (GSmaterialEntry *)GSresGetResource(ctx, 0x0CE61000);
    fn_80118874(material->texture, 1);
    material->texture = NULL;

    frame = lbl_8047C1D4;
    handle2 = GSresGetResource(ctx, 0x0CE61004);
    fn_800ECCA8(handle2, 0);
    GSmodelGetFrameCount(handle2, &frame, 0);
    frame = frame - lbl_8047C1D8;
    fn_800ECCA8(handle2, 0);
    fn_800ECA78(handle2, frame);
    fn_800ECB74(handle2, 0);
    fn_800EC990(handle2);

    iconHandle = fn_801CBA0C(0x06AF0400);
    iconResult = GSresGetResource(ctx, iconHandle);
    fn_800E9108(iconResult, 2);
    fn_800E8FE8(iconResult, lbl_8047A690);
    fn_800E900C(iconResult, 1, &lbl_8047A694);

    fn_800E3C08(GSresGetResource(ctx, iconHandle), GSresGetResource(ctx, 0x11511400));
    fn_800E3C00(GSresGetResource(ctx, iconHandle), 4);

    fn_80176E0C(ctx, 0x0D001800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0D0A1000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);
    fn_801CB834(iconHandle, 9, 0, 1);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x89);
    fn_8011288C(0, 0);
}

/* 0x8008EC28 | size: 0x2A8 */
void fn_8008EC28(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void fn_800E8FA0(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void fn_800E3C08(u32 handle, u32 val);
    extern void fn_800E3C00(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 fn_800E9108(u32 handle, u32 val);
    extern void fn_800E8FE8(u32 handle, u32 val);
    extern void fn_800E900C(u32 handle, u32 val, u32 *param);
    extern void fn_80176E0C(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void fn_8011288C(u32 a, u32 b);
    extern void fn_80118874(void *texture, u32 flag);
    extern void fn_800ECCA8(u32 handle, u32 val);
    extern void GSmodelGetFrameCount(u32 handle, f32 *out, u32 flag);
    extern void fn_800ECA78(u32 handle, f32 val);
    extern void fn_800ECB74(u32 handle, u32 val);
    extern void fn_800EC990(u32 handle);

    u32 waitFrames;
    u32 elapsed;
    GSmaterialEntry *material;
    f32 frame;
    u32 handle2;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x0CE61602);
    lbl_8047A694 = GSresGetResource(ctx, 0x0CE61002);
    fn_800E8FA0(0x280, 0x1E0);
    fn_801CB834(0x0CE61000, 0, 0, 0);

    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    fn_801CB7C4(0x0CE61000);

    material = (GSmaterialEntry *)GSresGetResource(ctx, 0x0CE61000);
    fn_80118874(material->texture, 1);
    material->texture = NULL;

    frame = lbl_8047C1D4;
    handle2 = GSresGetResource(ctx, 0x0CE61004);
    fn_800ECCA8(handle2, 0);
    GSmodelGetFrameCount(handle2, &frame, 0);
    frame = frame - lbl_8047C1D8;
    fn_800ECCA8(handle2, 0);
    fn_800ECA78(handle2, frame);
    fn_800ECB74(handle2, 0);
    fn_800EC990(handle2);

    iconHandle = fn_801CBA0C(0x06AF0400);
    iconResult = GSresGetResource(ctx, iconHandle);
    fn_800E9108(iconResult, 2);
    fn_800E8FE8(iconResult, lbl_8047A690);
    fn_800E900C(iconResult, 1, &lbl_8047A694);

    fn_800E3C08(GSresGetResource(ctx, iconHandle), GSresGetResource(ctx, 0x11511400));
    fn_800E3C00(GSresGetResource(ctx, iconHandle), 4);

    fn_80176E0C(ctx, 0x0CFB1800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0D051000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);
    fn_801CB834(iconHandle, 7, 0, 1);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x89);
    fn_8011288C(0, 0);
}

/* 0x8008EED0 | size: 0x2C0 */
void fn_8008EED0(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void fn_800E8FA0(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void fn_800E3C08(u32 handle, u32 val);
    extern void fn_800E3C00(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern void scriptWaitSyncMotion(u32 id, u32 val);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 fn_800E9108(u32 handle, u32 val);
    extern void fn_800E8FE8(u32 handle, u32 val);
    extern void fn_800E900C(u32 handle, u32 val, u32 *param);
    extern void fn_80176E0C(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void fn_8011288C(u32 a, u32 b);
    extern void fn_80118874(void *texture, u32 flag);
    extern void fn_800ECCA8(u32 handle, u32 val);
    extern void GSmodelGetFrameCount(u32 handle, f32 *out, u32 flag);
    extern void fn_800ECA78(u32 handle, f32 val);
    extern void fn_800ECB74(u32 handle, u32 val);
    extern void fn_800EC990(u32 handle);

    u32 waitFrames;
    u32 elapsed;
    GSmaterialEntry *material;
    f32 frame;
    u32 handle2;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x0CE61602);
    lbl_8047A694 = GSresGetResource(ctx, 0x0CE61002);
    fn_800E8FA0(0x280, 0x1E0);
    fn_801CB834(0x0CE61000, 0, 0, 0);

    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    fn_801CB7C4(0x0CE61000);

    material = (GSmaterialEntry *)GSresGetResource(ctx, 0x0CE61000);
    fn_80118874(material->texture, 1);
    material->texture = NULL;

    frame = lbl_8047C1D4;
    handle2 = GSresGetResource(ctx, 0x0CE61004);
    fn_800ECCA8(handle2, 0);
    GSmodelGetFrameCount(handle2, &frame, 0);
    frame = frame - lbl_8047C1D8;
    fn_800ECCA8(handle2, 0);
    fn_800ECA78(handle2, frame);
    fn_800ECB74(handle2, 0);
    fn_800EC990(handle2);

    iconHandle = fn_801CBA0C(0x06AF0400);
    fn_801CB834(iconHandle, 4, 0, 0);
    iconResult = GSresGetResource(ctx, iconHandle);
    fn_800E9108(iconResult, 2);
    fn_800E8FE8(iconResult, lbl_8047A690);
    fn_800E900C(iconResult, 1, &lbl_8047A694);

    fn_800E3C08(GSresGetResource(ctx, iconHandle), GSresGetResource(ctx, 0x11511400));
    fn_800E3C00(GSresGetResource(ctx, iconHandle), 4);

    fn_80176E0C(ctx, 0x0CFA1800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0D031000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);
    scriptWaitSyncMotion(iconHandle, 1);
    fn_801CB834(iconHandle, 5, 0, 0);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x89);
    fn_8011288C(0, 0);
}

/* 0x8008F190 | size: 0x394 */
void fn_8008F190(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void fn_800E8FA0(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void fn_800E3C08(u32 handle, u32 val);
    extern void fn_800E3C00(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern void scriptWaitSyncMotion(u32 id, u32 val);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 fn_800E9108(u32 handle, u32 val);
    extern void fn_800E8FE8(u32 handle, u32 val);
    extern void fn_800E900C(u32 handle, u32 val, u32 *param);
    extern void fn_80176E0C(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void fn_8011288C(u32 a, u32 b);
    extern void fn_80118874(void *texture, u32 flag);
    extern void fn_800ECCA8(u32 handle, u32 val);
    extern void GSmodelGetFrameCount(u32 handle, f32 *out, u32 flag);
    extern void fn_800ECA78(u32 handle, f32 val);
    extern void fn_800ECB74(u32 handle, u32 val);
    extern void fn_800EC990(u32 handle);

    u32 waitFrames;
    u32 elapsed;
    GSmaterialEntry *material;
    f32 frame;
    u32 handle2;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x0CE61602);
    lbl_8047A694 = GSresGetResource(ctx, 0x0CE61002);
    fn_800E8FA0(0x280, 0x1E0);
    fn_801CB834(0x0CE61000, 0, 0, 0);

    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    fn_801CB7C4(0x0CE61000);

    material = (GSmaterialEntry *)GSresGetResource(ctx, 0x0CE61000);
    fn_80118874(material->texture, 1);
    material->texture = NULL;

    frame = lbl_8047C1D4;
    handle2 = GSresGetResource(ctx, 0x0CE61004);
    fn_800ECCA8(handle2, 0);
    GSmodelGetFrameCount(handle2, &frame, 0);
    frame = frame - lbl_8047C1D8;
    fn_800ECCA8(handle2, 0);
    fn_800ECA78(handle2, frame);
    fn_800ECB74(handle2, 0);
    fn_800EC990(handle2);

    fn_800E3C08(GSresGetResource(ctx, 0x0CE61000), GSresGetResource(ctx, 0x111B1400));
    fn_800E3C00(GSresGetResource(ctx, 0x0CE61000), 4);

    fn_801CB834(0x0CE61000, 3, 0, 0);
    waitFrames = 0x32;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1E0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    iconHandle = fn_801CBA0C(0x06AF0400);
    fn_801CB834(iconHandle, 0, 0, 0);
    iconResult = GSresGetResource(ctx, iconHandle);
    fn_800E9108(iconResult, 2);
    fn_800E8FE8(iconResult, lbl_8047A690);
    fn_800E900C(iconResult, 1, &lbl_8047A694);

    fn_800E3C08(GSresGetResource(ctx, iconHandle), GSresGetResource(ctx, 0x11511400));
    fn_800E3C00(GSresGetResource(ctx, iconHandle), 4);

    fn_80176E0C(ctx, 0x0CF91800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0CF81000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);
    fn_801CB834(iconHandle, 1, 0, 0);
    scriptWaitSyncMotion(iconHandle, 1);
    fn_801CB834(iconHandle, 2, 0, 0);
    scriptWaitSyncMotion(iconHandle, 1);
    fn_801CB834(iconHandle, 3, 0, 0);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x89);
    fn_8011288C(0, 0);
}

/* 0x8008FBF4 | size: 0x2A0 */
void fn_8008FBF4(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void fn_800E8FA0(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void fn_800E3C08(u32 handle, u32 val);
    extern void fn_800E3C00(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 fn_800E9108(u32 handle, u32 val);
    extern void fn_800E8FE8(u32 handle, u32 val);
    extern void fn_800E900C(u32 handle, u32 val, u32 *param);
    extern void fn_80176E0C(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void fn_8011288C(u32 a, u32 b);
    extern void fn_80118874(void *texture, u32 flag);
    extern void fn_800ECCA8(u32 handle, u32 val);
    extern void GSmodelGetFrameCount(u32 handle, f32 *out, u32 flag);
    extern void fn_800ECA78(u32 handle, f32 val);
    extern void fn_800ECB74(u32 handle, u32 val);
    extern void fn_800EC990(u32 handle);

    u32 waitFrames;
    u32 elapsed;
    GSmaterialEntry *material;
    f32 frame;
    u32 handle2;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x0CE61602);
    lbl_8047A694 = GSresGetResource(ctx, 0x0CE61002);
    fn_800E8FA0(0x280, 0x1E0);
    fn_801CB834(0x0CE61000, 0, 0, 0);

    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    fn_801CB7C4(0x0CE61000);

    material = (GSmaterialEntry *)GSresGetResource(ctx, 0x0CE61000);
    fn_80118874(material->texture, 1);
    material->texture = NULL;

    frame = lbl_8047C1D4;
    handle2 = GSresGetResource(ctx, 0x0CE61004);
    fn_800ECCA8(handle2, 0);
    GSmodelGetFrameCount(handle2, &frame, 0);
    frame = frame - lbl_8047C1D8;
    fn_800ECCA8(handle2, 0);
    fn_800ECA78(handle2, frame);
    fn_800ECB74(handle2, 0);
    fn_800EC990(handle2);

    iconHandle = fn_801CBA0C(0x06AF0400);
    fn_801CB834(iconHandle, 0, 0, 1);
    iconResult = GSresGetResource(ctx, iconHandle);
    fn_800E9108(iconResult, 2);
    fn_800E8FE8(iconResult, lbl_8047A690);
    fn_800E900C(iconResult, 1, &lbl_8047A694);

    fn_800E3C08(GSresGetResource(ctx, iconHandle), GSresGetResource(ctx, 0x11511400));
    fn_800E3C00(GSresGetResource(ctx, iconHandle), 4);

    fn_80176E0C(ctx, 0x0CF51800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0CEC1000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x89);
    fn_8011288C(0, 0);
}

static inline u32 fn_80090720_getHandle2(u32 ctx) {
    extern u32 GSresGetResource(u32 ctx, u32 id);
    return GSresGetResource(ctx, 0x0CE61004);
}

/* 0x80090720 | size: 0x2C4 */
void fn_80090720(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void fn_800E8FA0(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern void scriptWaitSyncMotion(u32 id, u32 val);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 fn_800E9108(u32 handle, u32 val);
    extern void fn_800E8FE8(u32 handle, u32 val);
    extern void fn_800E900C(u32 handle, u32 val, u32 *param);
    extern void fn_80176E0C(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void fn_8011288C(u32 a, u32 b);
    extern void fn_80118874(void *texture, u32 flag);
    extern void fn_800ECCA8(u32 handle, u32 val);
    extern void GSmodelGetFrameCount(u32 handle, f32 *out, u32 flag);
    extern void fn_800ECA78(u32 handle, f32 val);
    extern void fn_800ECB74(u32 handle, u32 val);
    extern void fn_800EC990(u32 handle);

    u32 waitFrames;
    u32 elapsed;
    GSmaterialEntry *material;
    f32 frame;
    u32 new_var;
    u32 handle2;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x0CE61602);
    lbl_8047A694 = GSresGetResource(ctx, 0x0CE61002);
    fn_800E8FA0(0x280, 0x1E0);

    frame = lbl_8047C1D4;
    handle2 = fn_80090720_getHandle2(ctx);
    fn_800ECCA8(handle2, 1);
    GSmodelGetFrameCount(handle2, &frame, 0);
    frame = frame - lbl_8047C1D8;
    fn_800ECCA8(handle2, 1);
    fn_800ECA78(handle2, frame);
    fn_800ECB74(handle2, 0);
    fn_800EC990(handle2);

    fn_801CB834(0x0CE61000, 0, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    fn_801CB7C4(0x0CE61000);

    material = (GSmaterialEntry *)GSresGetResource(ctx, 0x0CE61000);
    fn_80118874(material->texture, 1);
    material->texture = NULL;

    iconHandle = fn_801CBA0C(0x06BC0400);
    iconResult = GSresGetResource(ctx, iconHandle);
    fn_800E9108(iconResult, 2);
    fn_800E8FE8(iconResult, lbl_8047A690);
    fn_800E900C(iconResult, 1, &lbl_8047A694);

    fn_80176E0C(ctx, 0x0CF21800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0CE91000);
    new_var = iconHandle;
    fn_801845E4(ctx, new_var, ctx, finalResult, 0);
    fn_801CB834(new_var, 1, 0, 0);
    fn_801CB834(0x0CE61004, 0, 0, 0);
    scriptWaitSyncMotion(iconHandle, 1);
    fn_801CB834(iconHandle, 2, 0, 0);
    scriptWaitSyncMotion(new_var, 1);
    fn_801CB834(new_var, 3, 0, 0);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x89);
    fn_8011288C(0, 0);
}

/* 0x8008C7B0 | size: 0x31C */
void fn_8008C7B0(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void fn_800E8FA0(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void fn_800E3C08(u32 handle, u32 val);
    extern void fn_800E3C00(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 fn_800E9108(u32 handle, u32 val);
    extern void fn_800E8FE8(u32 handle, u32 val);
    extern void fn_800E900C(u32 handle, u32 val, u32 *param);
    extern void fn_80176E0C(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_80190528(u32 id);
    extern void fn_800FF58C(u32 id);
    extern void fn_8011288C(u32 a, u32 b);
    extern void fn_80118874(void *texture, u32 flag);
    extern void fn_800ECCA8(u32 handle, u32 val);
    extern void GSmodelGetFrameCount(u32 handle, f32 *out, u32 flag);
    extern void fn_800ECA78(u32 handle, f32 val);
    extern void fn_800ECB74(u32 handle, u32 val);
    extern void fn_800EC990(u32 handle);

    u32 waitFrames;
    u32 elapsed;
    GSmaterialEntry *material;
    f32 frame;
    u32 handle2;
    u32 iconHandle;
    u32 iconHandle2;
    u32 b2;
    u32 tmpA;
    u32 tmpB;

    lbl_8047A690 = GSresGetResource(ctx, 0x0CE61602);
    lbl_8047A694 = GSresGetResource(ctx, 0x0CE61002);
    fn_800E8FA0(0x280, 0x1E0);
    fn_801CB834(0x0CE61000, 0, 0, 0);

    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    fn_801CB7C4(0x0CE61000);

    material = (GSmaterialEntry *)GSresGetResource(ctx, 0x0CE61000);
    fn_80118874(material->texture, 1);
    material->texture = NULL;

    frame = lbl_8047C1D4;
    handle2 = GSresGetResource(ctx, 0x0CE61004);
    fn_800ECCA8(handle2, 0);
    GSmodelGetFrameCount(handle2, &frame, 0);
    frame = frame - lbl_8047C1D8;
    fn_800ECCA8(handle2, 0);
    fn_800ECA78(handle2, frame);
    fn_800ECB74(handle2, 0);
    fn_800EC990(handle2);

    iconHandle = fn_801CBA0C(0x06AF0400);
    iconHandle2 = fn_801CBA0C(0x0B720400);

    b2 = GSresGetResource(ctx, iconHandle);
    fn_800E9108(b2, 2);
    fn_800E8FE8(b2, lbl_8047A690);
    fn_800E900C(b2, 1, &lbl_8047A694);

    b2 = GSresGetResource(ctx, iconHandle2);
    fn_800E9108(b2, 2);
    fn_800E8FE8(b2, lbl_8047A690);
    fn_800E900C(b2, 1, &lbl_8047A694);

    fn_800E3C08(GSresGetResource(ctx, iconHandle), GSresGetResource(ctx, 0x11511400));
    fn_800E3C00(GSresGetResource(ctx, iconHandle), 4);

    fn_80176E0C(ctx, 0x0D041800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    tmpA = fn_801CBA0C(0x0D0D1000);
    tmpB = fn_801CBA0C(0x0D0D1001);
    fn_801845E4(ctx, iconHandle, ctx, tmpA, 0);
    fn_801845E4(ctx, iconHandle2, ctx, tmpB, 0);
    fn_801CB834(iconHandle, 9, 0, 1);
    fn_801CB834(iconHandle2, 6, 0, 1);

    cameraWaitSyncAnime(1);
    fn_80190528(0x8D0);
    fn_800FF58C(1);
    fn_8011288C(0, 0);
}
