/**
 * @file gs_gfx_range_800D3FA4.c
 * @brief GS render engine segment -- split from gs_render.c.
 *
 * XD source unit: GSgfxM unknown (best guess layer.cpp analog)
 * Address range: 0x800D3FA4 - 0x800D6B00 (44 functions)
 *
 * Zero anchors -> gate-2 trivially passes. Shape: big fns 0x654/0x97C/0x56C followed by ~35 medium 0x64-0xB4 methods, resembling XD layer.cpp (0x3AD4, RenderStats 0xF74 monster + many mid-size GSgfxLayer methods), but not conclusive; honest range name.
 *
 * Split from src/game/gs_render.c (physical XD source-unit split).
 * The dead #ifdef PCPORT reference block (never defined in configure.py,
 * same situation as gs_gfx.c) was stripped during the split.
 */

#include "dolphin/types.h"

/* ===== External SDK / engine functions ===== */
extern void  GSlogWrite(const char*, ...);             /* OSReport / GSlog */
extern void* memcpy(void* dst, const void* src, u32 n);
extern void* memset(void* dst, int val, u32 size);

/* External functions referenced from asm wrappers */
extern void DCFlushRange(void* addr, u32 size);
extern u64 OSGetTime(void);
extern void fn_800D3EC4(s32, f32, f32, f32, f32, f32, f32);
extern void fn_800D4F98(u32, u32, ...);
extern void fn_800D67BC(u16);
extern void fn_800D892C(u32);
extern u32 OSGetTick(void);
extern void fn_801E17A8(void);
extern void fn_801B25C4(u32);
extern s32 HSD_CObjSetCurrent(void*);
extern void GSlightSetupLights(void*);
extern void fn_800E3604(u32, u32);
extern void fn_80118104(u32, u32);
extern void fn_80195A48(void);
extern void fn_800D87AC(s32);
extern u32* fn_800D461C(u32*);
extern void fn_800DA2BC(u32, u32, u32);
extern void fn_800DA1E8(u32, u32, u32);
extern void fn_800DA100(u32, u32, u8, u32, u32, u8);
extern void fn_800D88DC(u32);
extern void fn_800D888C(u32);
extern void fn_800D9B58(f32, f32, f32, f32);
extern void fn_800D9FB4(u32);
extern void fn_800DA028(u32);
extern void fn_800D7820(u32);
extern void fn_800D6A00(u32);
extern void fn_800D65CC(u16, u16, u16);
extern void fn_800D5CB8(u32, u8, u8, u8, u8);
extern void fn_800D6728(void);

/* GSmem */
extern u16   _toolentryAlloc__FUl(u32 size);                    /* GSmemAllocRaw */
extern void* fn_800E27B0(u16 handle);                  /* GSmemGetPtr */

/* SDK GX functions */
extern void  fn_800AA2F0(void);                        /* GXSetViewport */
extern void  fn_800BD640(void);                        /* GXSetProjection */
extern void  fn_800BD744(void);                        /* GXLoadPosMtxImm */
extern void  GXInvalidateTexAll(void);                        /* GXInvalidateTexAll */

typedef struct GSgfxState {
    u8 pad_000[0x48c];
    u16 requestBufferHandle;
    u16 pad_48e;
    void* requestBuffer;
    void* requestBufferPos;
    u32 requestBufferSize;
    u8 requestBufferFlag;
} GSgfxState;

typedef struct GSgfxRequestStrings {
    char alreadyAllocated[0x28];
    char allocationFailed[0x24];
    char allocationSucceeded[1];
} GSgfxRequestStrings;

/* ===== String constants (rodata) ===== */
extern const char lbl_80270440[]; /* "GSgfx: invalid matrix index" */
extern const char lbl_802703C0[]; /* request-buffer status strings */
extern const char lbl_80270460[]; /* "GSgfx: matrix stack underflow!" */
extern const char lbl_80270480[]; /* "GSgfx: matrix stack overflow!" */
extern const char lbl_802704A0[]; /* "0123456789ABCDEF" */
extern const char lbl_80270528[]; /* "GSmaterialSetPEdescr: Warning..." */
extern const char lbl_8027056C[]; /* "GSmaterialCreate: Run out of materials..." */
extern const char lbl_802705C0[]; /* "GSmaterial MObj" */
extern const char lbl_802705D0[]; /* "GSmaterial: Unsupported texture format..." */
extern const char lbl_80270610[]; /* "GSmaterial: Error creating environment map..." */

/* ===== BSS / global state ===== */
extern u32 lbl_8047AA80;   /* GSgfx state pointer (sda21) */
extern u32 lbl_8047AA8C;
extern u8 lbl_80400248[];  /* GSgfx state backup buffer (0x5A0 bytes) */
extern u8 lbl_80400B28[];  /* light/material command buffer */
extern u8 lbl_803147C8[];
extern f32 lbl_8047CA20;
extern f32 lbl_8047CA24;
extern f32 lbl_8047CA28;

/* ===== Combined forward-decls (duplicated across split segments) ===== */

/* No-op functions (1) */
/* Address: 0x800DC874 | Size: 0x4 */
/* Forward declarations for self-referencing asm blocks */
extern void fn_800D6B00(void);
extern void fn_800D724C(u32 idx);
extern void fn_800D7268(u32 idx);
extern void fn_800D72A4(u32 idx);
extern void fn_800D72C4(u32 idx);
extern void fn_800D72E4(u32 idx);
extern void fn_800D7304(u32 idx);
extern void fn_800D7328(u32 idx);
extern void fn_800D7344(u32 idx);
extern void fn_800D7360(u32 idx);
extern void fn_800D737C(u32 idx);
extern void fn_800D7398(u32 idx);
extern void fn_800D73C4(u32 idx);
extern void fn_800D73F8(void);
extern void fn_800D740C(void);
extern void fn_800D7420(void);
extern void fn_800D7444(void);
extern void fn_800D7468(void);
extern void fn_800D748C(void);
extern void fn_800D74A0(void);
extern void fn_800D74B4(void);
extern void fn_800D74D0(void);
extern void fn_800D74EC(void);
extern void fn_800D7508(void);
extern void fn_800D7524(void);
extern void fn_800D7540(void);
extern void fn_800D7564(void);
extern void fn_800D7588(void);
extern void fn_800D75AC(void);
extern void fn_800D7650(u8*);
extern void fn_800D7868(u8*, u32, u32, u32, u32, u8, u32, u8);
extern void fn_800D7940(u32, u16);
extern void fn_800D7A70(u32);
extern void fn_800DB098(void);
extern void fn_800DB758(u16);
extern void lightGetFrameCount__FP9_HSD_AObj(u8*);
extern void fn_800DE09C(void);
extern void fn_800DE128(void);
extern void fn_800E09E8(void*, void*, u32);
extern u8 fn_800E0E14(u32, u32);
extern u32 _matGSmatObjMakeTExp(void*, void*, void*, void*, void*);
extern void _matGSmatEnableEnvMapExt(u8*);
extern s32 _matGSmatObjLoad(u8*);
extern void fn_800E0290(void*, void*, void*);
extern void fn_800E02C4(void*);
extern void fn_800E02E8(void*, f32);
extern void fn_800E032C(void*, f32);
extern void fn_800E0370(void*, f32);
extern void fn_800E03E8(void*, f32, f32, f32);
extern void fn_800E0628(void*, void*);
extern void fn_800E064C(void*);
extern void GSmtx44Perspective(u8*);
extern void GSmtx44Ortho(void*, f32, f32, f32, f32, f32, f32);
extern void fn_800E0C78(void);
extern void GSmathInitCosTable(void);


/* ===== Combined externs (duplicated across all gs_render.c split segments;
 * de-duplicated by identifier from the whole original TU so any
 * cross-segment call/reference resolves regardless of which segment
 * the callee's real definition ended up in). ===== */
extern u8 lbl_8047AA91;
extern u32 lbl_8047AA80;
extern void fn_800B944C(u32, u32);
extern f32 lbl_8047CA30;
extern f32 lbl_8047CA34;
extern f32 lbl_8047CA38;
extern void fn_800B9404(u32, u32);
extern void fn_800D7230(void);
extern void fn_800D75D0(void);
extern void fn_800B928C(u32, u32, u16);
extern u8 lbl_80314350[];
extern u8 lbl_804001F0[];
extern void fn_800D6A80(u16, s32, u32*, u32*);
extern u8 lbl_804007E8[];
extern void fn_800B7D74(u32, u32, u32, u32, u8);
extern void fn_800B7D3C(void);
extern void fn_800B7874(u32, u32);
extern void fn_800B84E0(u32, u32, u8);
extern u8 lbl_80314370[];
extern u8 lbl_803143B4[];
extern u8 lbl_803143D8[];
extern u8 lbl_803143A8[];
extern u32 lbl_8047AAB0;
extern u32 lbl_8047AAAC;
extern u8 lbl_803144D0[];
extern u32 lbl_8047AAB4;
extern u16 lbl_8047AAA8;
extern u32 GScameraGetActiveCamera(void);
extern u32 fn_800D1D00(void);
extern u32 fn_800D1B3C(void);
extern u32 GScameraGetProjMatrixPtr(void);
extern void GXLoadPosMtxImm(u32, u32);
extern void GXLoadNrmMtxImm(u32, u32);
extern void fn_800BD554(u32);
extern u8 lbl_8047AAC8;
extern u8 lbl_80314610[];
extern u32 lbl_8047AAC0;
extern u8 lbl_80400948[];
extern u32 lbl_8047AAC4;
extern u32 lbl_8047AABC;
extern u16 lbl_8047AAB8;
extern void fn_800B857C(u32, u32, u32, u32, u32, u32);
extern void GXLoadTexMtxImm(void*, u32, u32);
extern u8 lbl_80314404[];
extern u8 lbl_80314454[];
extern u8 lbl_803144A8[];
extern u8 lbl_80314424[];
extern void fn_800BAE34();
extern void fn_800BACA0();
extern void fn_800BB098();
extern void GXLoadTexObj();
extern u8 lbl_80314530[];
extern u32 lbl_8047CA40;
extern u32 lbl_8047CA48;
extern u8 lbl_80314510[];
extern u8 lbl_803144F0[];
extern void fn_800BBC34(u32);
extern void fn_800BBC0C(u32);
extern void fn_800BA6B0();
extern void fn_800BA6F4();
extern void fn_800B884C();
extern void fn_800BC8C8();
extern void fn_800BBAF8();
extern void fn_800BB97C();
extern void fn_800BB81C();
extern void fn_800BC6F0();
extern void fn_800BC228();
extern void fn_800BC290();
extern void fn_800BC1A0();
extern void fn_800BC1E4();
extern void fn_800BC454();
extern void fn_800BC4C0();
extern void fn_800BB780();
extern void fn_800BBC7C();
extern void fn_800BBCE0();
extern void fn_800BBE8C();
extern void fn_800BBF98();
extern void fn_800BBFDC();
extern void fn_800BC3E0();
extern void fn_800BC580();
extern void fn_800BD2E0(void*, u32);
extern f32 lbl_8047CA50;
extern f32 lbl_8047CA54;
extern void GScameraSetViewport(void*, u16, u16, u16, u16);
extern u32 lbl_8047CA60;
extern u32 lbl_8047CA68;
extern u32 lbl_8047CA58;
extern void fn_800BD7A0(u32, u32, u32, u32);
extern void fn_800D2150(u32, u16, u16, u16, u16);
extern void HSD_FogSet(u32);
extern u32 lbl_8047AA8C;
extern void GXSetClipMode(u32);
extern void fn_800B94F0(u32);
extern u8 lbl_8031453C[];
extern void fn_800BCFDC(u32);
extern void fn_800BC618(u32, u8, u32, u32, u8);
extern u8 lbl_8031457C[];
extern u8 lbl_8031456C[];
extern void GXSetZMode(u32, u32, u32);
extern void fn_800BCEBC(u32);
extern u8 lbl_8031454C[];
extern void GXSetDstAlpha(u32);
extern void GXSetBlendMode(u32, u32, u32, u32);
extern u8 lbl_803145D0[];
extern u32 lbl_8031459C[];
extern u32 lbl_803145A8[];
extern void jumptable_803152B8();
extern void jumptable_80315340();
extern void jumptable_80315320();
extern void fn_800E24B0(u16);
extern void fn_800E209C(u16);
extern void GXCallDisplayList(u32, u32);
extern void fn_800E2AF8(u16);
extern u16 fn_800E2C04(u32, u32);
extern u32 lbl_8047AAD8;
extern u32 lbl_8047AAD4;
extern void jumptable_80315364();
extern u32 GStextureUnlockImage(void*);
extern u8 lbl_80400EE0[];
extern u8 lbl_8047AAE0;
extern void GStextureGetFormat(void);
extern void GStextureSetWrap(void);
extern void GStextureSetFilter(void);
extern void* GStextureLockImage(void*, u32);
extern void GStextureConvertFromHW(void);
extern void HSD_LObjReqAnimAll(void*, f32);
extern void HSD_LObjAnimAll(void*);
extern u32 lbl_8047AAEC;
extern u32 lbl_8047CA80;
extern f32 lbl_8047CA70;
extern u32 lbl_8047CA74;
extern f32 lbl_8047CA78;
extern u32 lbl_8047AAF0;
extern void HSD_LObjSetPosition();
extern void HSD_LObjSetInterest();
extern void HSD_LObjRemoveAnimAll(void*);
extern void HSD_LObjAddAnimAll(void*, void*);
extern void HSD_ForeachAnim(void*, u32, u32, void*, u32, ...);
extern s32 fn_800D37CC(void);
extern void HSD_AObjSetRate(void);
extern f32 lbl_8047AAF4;
extern f32 lbl_8047CA88;
extern void GSlightSetAnimIndex(u8*, u32);
extern void HSD_LObjGetPosition(void*, void*);
extern void HSD_LObjGetInterest(void*, void*);
extern void HSD_LObjSetColor(u32, u8*);
extern void HSD_LObjClearFlags(u32, u32);
extern void HSD_LObjSetFlags(u32, u32);
extern u32 HSD_LObjLoadDesc(void*);
extern u32 lbl_8047CA8C;
extern u16 lbl_8047AAE8;
extern void __assert(u8*, u32, u8*);
extern u8 lbl_8047CA90;
extern u8 lbl_8047CA98;
extern void HSD_LObjDeleteCurrentAll(void*);
extern void HSD_LObjAddCurrentAll(void);
extern void HSD_LObjSetup(void*);
extern u32 lbl_8047AAF8;
extern u32 lbl_8047AB08;
extern u32 lbl_8047AAFA;
extern u32 lbl_8047AAFC;
extern void OSTicksToCalendarTime(void);
extern void logVsnprintf_float(void);
extern u32 strlen(const char* s);
extern u32 lbl_8047AB11;
extern u8 lbl_80400F30[];
extern u8 lbl_802704B4[];
extern u8 lbl_80400F44[];
extern u32 lbl_8047AB0C;
extern u32 lbl_8047AB00;
extern u32 lbl_8047AB04;
extern u8 lbl_80401044[];
extern u8 lbl_80401058[];
extern u32 lbl_8047AB10;
extern void jumptable_80315388();
extern void __va_arg();
extern u8 lbl_80401168[];
extern u8 lbl_80401178[];
extern u32 lbl_80478AE8;
extern u8 lbl_8047CAA0[];
extern u8 lbl_8047CAA8[];
extern void GXDrawDone(u32);
extern void HSD_ImageDescFree(u32);
extern void* HSD_ImageDescAlloc(void);
extern u16 GStextureGetXsize(void*);
extern u16 GStextureGetYsize(void*);
extern void* GStextureGetGXformat(void*, u32);
extern u8 GStextureGetMiplevels(void*);
extern f32 lbl_8047CAC8;
extern void HSD_MObjSetAlpha(u32, ...);
extern f64 lbl_8047CAD0;
extern f32 lbl_8047CACC;
extern void HSD_TObjRemove(void*);
extern void HSD_MObjCompileTev(void*);
extern u32 HSD_MObjGetFlags(void*);
extern void HSD_MObjClearFlags(void*, u32);
extern void HSD_MObjSetFlags(void*, void*);
extern u32 lbl_8047AB20;
extern u32 lbl_8047AB1C;
extern void HSD_MObjSetDefaultClass(void*);
extern u16 lbl_8047AB18;
extern u8 lbl_80315490[];
extern void hsdInitClassInfo(void*, void*, void*, void*, u32, u32);
extern u8 lbl_8036CB30[];
extern void HSD_TExpGetType();
extern void fn_801B6DC0();
extern void HSD_TExpCnst();
extern void fn_801B707C();
extern void fn_801B6E74();
extern void fn_801B64EC();
extern void fn_801B6CD8();
extern void fn_801B5F08();
extern void HSD_ImageDescRemove(void);
extern void HSD_TObjLoadDesc(void);
extern void HSD_MObjGetTObj(void);
extern void HSD_MObjAddTObjNext(void);
extern void fn_801A6DA0(void);
extern u8 lbl_803154E4[];
extern void PSMTXMultVec(void*, void*, void*);
extern void PSVECCrossProduct(void*, void*, void*);
extern void PSVECDotProduct(void);
extern void PSVECSquareDistance(void);
extern void PSVECDistance(void);
extern void PSVECNormalize(void*, void*);
extern void PSVECMag(void);
extern void PSVECScale(void*, void*, f32);
extern const f32 lbl_8047CAD8;
extern void PSVECSubtract(void*, void*, void*);
extern void PSVECAdd(void*, void*, void*);
extern f32 lbl_8047CADC;
extern void C_MTXLookAt(void);
extern void PSMTXTranspose(void*, void*);
extern void PSMTXInverse(void*, void*);
extern void PSMTXConcat(void*, void*, void*);
extern void PSMTXScaleApply(void*, void*);
extern void PSMTXRotRad(void*, u32);
extern void PSMTXTransApply(void*, void*, f32, f32, f32);
extern void PSMTXQuat(void);
extern u8 lbl_80315568[];
extern void C_MTXPerspective(void);
extern void C_MTXOrtho(void);
extern void C_QUATSlerp(void*, void*, void*);
extern void fn_801ADAAC(void*, void*);
extern void C_QUATRotAxisRad(void);
extern void PSQUATMultiply(void*, void*, void*);
extern u32 lbl_8047CAE4;
extern u32 lbl_8047CAE0;
extern u32 lbl_8047CAE8;
extern f32 lbl_8047CAF0;
extern f32 lbl_8047CAF4;
extern u32 lbl_8047CB00;
extern u32 lbl_8047CAF8;
extern u32 lbl_8047CB08;
extern u32 lbl_8047CAFC;
extern f32 fn_801ADC7C(void);
extern f32 lbl_8047CB10;
extern u32 fn_801ADCD8(void);
extern u32 lbl_80478C94;
extern f64 fmod(f64 x, f64 y);
extern s32 __cvt_fp2unsigned(f32 x);
extern f32 lbl_8047CB20;
extern f32 lbl_8047CB1C;
extern f32 lbl_8047CB24;
extern f64 lbl_8047CB28;
extern f32 lbl_8047CB18;
extern f32 lbl_8047CB34;
extern f32 lbl_8047CB30;
extern f32 lbl_804011B8[];
extern f64 cos(f32);
extern f32 lbl_8047CB38;
extern f64 lbl_8047CB40;
extern u8 lbl_80270658[];
extern u32 lbl_8047AB30;
extern u32 lbl_8047AB68;
extern u32 lbl_8047AB64;
extern u32 lbl_8047AB38;
extern u32 lbl_8047AB34;
extern u32 lbl_8047AB28;
extern u32 lbl_8047AB4C;
extern u32 lbl_8047AB48;
extern u32 lbl_8047AB60;
extern u32 lbl_8047AB5C;
extern u32 lbl_8047AB58;
extern u32 lbl_8047AB54;
extern u32 lbl_8047AB50;
extern u32 lbl_8047CB50;
extern u32 lbl_8047CB48;
extern u32 lbl_8047AB3C;


#if !defined(GS_GFX_RANGE_SPLIT) || \
    defined(GS_GFX_RANGE_800D3FA4_800D45F8)
/* ==================================================================
 * fn_800D3FA4 -- GSgfx_BeginFrame
 *
 * Set up the rendering pipeline for a new frame. This is a large
 * function (0x654 = 1620 bytes) that configures:
 *   - GX FIFO
 *   - Clear color and Z buffer
 *   - Viewport and scissor
 *   - Default blend modes
 *   - TEV stages
 *   - Projection matrix
 * ================================================================== */
void fn_800D3FA4(u32 flags, u8 setupCamera, u8 resetQueue) {
    u8* state;
    u8* stats;
    u32 oldMode;
    u32 oldMask;
    u8 oldAlpha;
    u32 start;
    u32 cursor;
    u32 camera;

    state = (u8*)lbl_8047AA80;
    stats = lbl_804001F0;

    cursor = *(u32*)(state + 0x494) - *(u32*)(state + 0x490);
    if (cursor > *(u32*)(stats + 0x28)) {
        *(u32*)(stats + 0x28) = cursor;
    }

    oldMode = *(u32*)(state + 0x00);
    oldMask = *(u32*)(state + 0x04);
    *(u32*)(state + 0x00) = 2;
    HSD_FogSet(lbl_8047AA8C);

    state = (u8*)lbl_8047AA80;
    state[0x1B] = 0;
    *(u32*)(stats + 0x40) = 0;
    *(u32*)(stats + 0x44) = 0;
    *(u32*)(stats + 0x48) = 0;

    if (flags & 1) {
        fn_801E17A8();
    }

    if (flags & 0x10) {
        start = OSGetTick();
        *(u32*)((u8*)lbl_8047AA80 + 0x04) = 0x10;
        if (setupCamera != 0) {
            camera = GScameraGetActiveCamera();
            if (camera != 0) {
                fn_801B25C4(0x7F);
                if (HSD_CObjSetCurrent(*(void**)(camera + 0x0C)) != 0) {
                    GSlightSetupLights(*(void**)(camera + 0x0C));
                    fn_800E3604(0x10, 0);
                    fn_80118104(0x10, 0);
                    HSD_FogSet(lbl_8047AA8C);
                    fn_80195A48();
                }
                fn_800D87AC(-1);
            }
        }
        cursor = *(u32*)((u8*)lbl_8047AA80 + 0x490);
        while (cursor < *(u32*)((u8*)lbl_8047AA80 + 0x494)) {
            cursor = (u32)fn_800D461C((u32*)cursor);
        }
        *(u32*)(stats + 0x40) = OSGetTick() - start;
    }

    if (flags & 0x1000) {
        start = OSGetTick();
        *(u32*)((u8*)lbl_8047AA80 + 0x04) = 0x1000;
        if (setupCamera != 0) {
            camera = GScameraGetActiveCamera();
            if (camera != 0) {
                fn_801B25C4(0x7F);
                if (HSD_CObjSetCurrent(*(void**)(camera + 0x0C)) != 0) {
                    GSlightSetupLights(*(void**)(camera + 0x0C));
                    fn_800E3604(0x1000, 0);
                    fn_80118104(0x1000, 0);
                    HSD_FogSet(lbl_8047AA8C);
                    fn_80195A48();
                }
                fn_800D87AC(-1);
            }
        }
        *(u32*)(stats + 0x44) = OSGetTick() - start;
    }

    if (flags & 0x2000) {
        start = OSGetTick();
        *(u32*)((u8*)lbl_8047AA80 + 0x04) = 0x2000;
        if (setupCamera != 0) {
            camera = GScameraGetActiveCamera();
            if (camera != 0) {
                fn_801B25C4(0x7F);
                if (HSD_CObjSetCurrent(*(void**)(camera + 0x0C)) != 0) {
                    GSlightSetupLights(*(void**)(camera + 0x0C));
                    fn_800E3604(0x2000, 0);
                    fn_80118104(0x2000, 0);
                    HSD_FogSet(lbl_8047AA8C);
                    fn_80195A48();
                }
                fn_800D87AC(-1);
            }
        }
        cursor = *(u32*)((u8*)lbl_8047AA80 + 0x490);
        while (cursor < *(u32*)((u8*)lbl_8047AA80 + 0x494)) {
            cursor = (u32)fn_800D461C((u32*)cursor);
        }
        *(u32*)(stats + 0x48) = OSGetTick() - start;
    }

    *(u32*)((u8*)lbl_8047AA80 + 0x04) = -1;
    oldAlpha = ((u8*)lbl_8047AA80)[0x1A];
    ((u8*)lbl_8047AA80)[0x1A] = ((u8*)lbl_8047AA80)[0x1B];
    fn_800DA2BC(0, 0, 1);
    fn_800DA1E8(1, 7, 2);
    fn_800DA100(0, 7, 0, 1, 7, 0);
    fn_800D88DC(1);
    fn_800D888C(6);
    fn_800D9B58(lbl_8047CA20, lbl_8047CA20, lbl_8047CA24, lbl_8047CA28);
    fn_800D9FB4(0);
    fn_800DA028(0);
    fn_800D7820((u32)lbl_803147C8);
    fn_800D6A00(4);
    fn_800D67BC(4);
    fn_800D65CC(0, 0, 0x752F);
    fn_800D5CB8(0, 0, 0, 0, 0xC4);
    fn_800D65CC(0x280, 0, 0x752F);
    fn_800D5CB8(0, 0, 0, 0, 0xC4);
    fn_800D65CC(0, 0x1E0, 0x752F);
    fn_800D5CB8(0, 0, 0, 0, 0xC4);
    fn_800D65CC(0x280, 0x1E0, 0x752F);
    fn_800D5CB8(0, 0, 0, 0, 0xC4);
    fn_800D6728();
    ((u8*)lbl_8047AA80)[0x1A] = oldAlpha;
    fn_800DA2BC(1, 1, 1);
    fn_800DA1E8(1, 7, 2);
    fn_800D9FB4(1);

    flags &= (u32)-0x102;
    state = (u8*)lbl_8047AA80;
    state[0x1B] = 1;
    *(u32*)(stats + 0x4C) = 0;
    *(u32*)(stats + 0x50) = 0;
    *(u32*)(stats + 0x54) = 0;

    if (flags & 1) {
        fn_801E17A8();
    }

    if (flags & 0x10) {
        start = OSGetTick();
        *(u32*)((u8*)lbl_8047AA80 + 0x04) = 0x10;
        if (setupCamera != 0) {
            camera = GScameraGetActiveCamera();
            if (camera != 0) {
                fn_801B25C4(0x7F);
                if (HSD_CObjSetCurrent(*(void**)(camera + 0x0C)) != 0) {
                    GSlightSetupLights(*(void**)(camera + 0x0C));
                    fn_800E3604(0x10, 1);
                    fn_80118104(0x10, 1);
                    HSD_FogSet(lbl_8047AA8C);
                    fn_80195A48();
                }
                fn_800D87AC(-1);
            }
        }
        cursor = *(u32*)((u8*)lbl_8047AA80 + 0x490);
        while (cursor < *(u32*)((u8*)lbl_8047AA80 + 0x494)) {
            cursor = (u32)fn_800D461C((u32*)cursor);
        }
        *(u32*)(stats + 0x4C) = OSGetTick() - start;
    }

    if (flags & 0x1000) {
        start = OSGetTick();
        *(u32*)((u8*)lbl_8047AA80 + 0x04) = 0x1000;
        if (setupCamera != 0) {
            camera = GScameraGetActiveCamera();
            if (camera != 0) {
                fn_801B25C4(0x7F);
                if (HSD_CObjSetCurrent(*(void**)(camera + 0x0C)) != 0) {
                    GSlightSetupLights(*(void**)(camera + 0x0C));
                    fn_800E3604(0x1000, 1);
                    fn_80118104(0x1000, 1);
                    HSD_FogSet(lbl_8047AA8C);
                    fn_80195A48();
                }
                fn_800D87AC(-1);
            }
        }
        *(u32*)(stats + 0x50) = OSGetTick() - start;
    }

    if (flags & 0x2000) {
        start = OSGetTick();
        *(u32*)((u8*)lbl_8047AA80 + 0x04) = 0x2000;
        if (setupCamera != 0) {
            camera = GScameraGetActiveCamera();
            if (camera != 0) {
                fn_801B25C4(0x7F);
                if (HSD_CObjSetCurrent(*(void**)(camera + 0x0C)) != 0) {
                    GSlightSetupLights(*(void**)(camera + 0x0C));
                    fn_800E3604(0x2000, 1);
                    fn_80118104(0x2000, 1);
                    HSD_FogSet(lbl_8047AA8C);
                    fn_80195A48();
                }
                fn_800D87AC(-1);
            }
        }
        cursor = *(u32*)((u8*)lbl_8047AA80 + 0x490);
        while (cursor < *(u32*)((u8*)lbl_8047AA80 + 0x494)) {
            cursor = (u32)fn_800D461C((u32*)cursor);
        }
        *(u32*)(stats + 0x54) = OSGetTick() - start;
    }

    ((u8*)lbl_8047AA80)[0x1B] = 0;
    fn_800DA2BC(1, 1, 1);
    fn_800DA1E8(1, 2, 2);
    *(u32*)((u8*)lbl_8047AA80 + 0x00) = oldMode;
    *(u32*)((u8*)lbl_8047AA80 + 0x04) = oldMask;
    if (resetQueue != 0) {
        *(u32*)((u8*)lbl_8047AA80 + 0x494) = *(u32*)((u8*)lbl_8047AA80 + 0x490);
    }
}
#endif


/* Render-command stream callees. */
extern void fn_800D30AC(void);
extern void fn_800D55D0(f32);
extern void fn_800D5648(f32);
extern void fn_800D56C0(u8);
extern void fn_800D5724(u32, u8);
extern void fn_800D579C(u32, u16);
extern void fn_800D5814(u32, u8, u8);
extern void fn_800D58A0(u32, s16, s16);
extern void fn_800D592C(u32, u16, u16);
extern void fn_800D59B8(u32, f32, f32);
extern void fn_800D5A38(u32, u8);
extern void fn_800D5AB0(u32, u16);
extern void fn_800D5B28(u32, u16);
extern void fn_800D5BA0(u32, u32);
extern void fn_800D5C18(u32, u8, u8, u8);
extern void fn_800D5D6C(u8);
extern void fn_800D5DD0(u16);
extern void fn_800D5E34(s8, s8, s8);
extern void fn_800D5EB4(s16, s16, s16);
extern void fn_800D5F34(f32, f32, f32);
extern void fn_800D5FA4(u8);
extern void fn_800D6028(u16);
extern void fn_800D60AC(s8, s8);
extern void fn_800D6148(u8, u8);
extern void fn_800D61E4(s16, s16);
extern void fn_800D6280(u16, u16);
extern void fn_800D631C(f32, f32);
extern void fn_800D63B0(s8, s8, s8);
extern void fn_800D6464(u8, u8, u8);
extern void fn_800D6518(s16, s16, s16);
extern void fn_800D6680(f32, f32, f32);
extern void fn_800D76A8(u32, u16);
extern void fn_800D7C74(void);
extern void fn_800D7D10(u8, u32);
extern void fn_800D7D90(u8, void*);
extern void fn_800D7E5C(void);
extern void fn_800D7F14(void*);
extern void fn_800D7FE4(void*);
extern void fn_800D8088(void*);
extern void fn_800D8154(f32, f32, f32);
extern void fn_800D81EC(f32, f32, f32);
extern void fn_800D8284(f32, f32, f32);
extern void fn_800D834C(void);
extern void fn_800D848C(u32, u32, u32, void*);
extern void fn_800D85D4(u32, u32);
extern void GSgfxDLDraw(u32);
extern void fn_800D9BD0(f32, f32, f32, f32);
extern void fn_800D9C24(u16, u16, u16, u16);
extern void fn_800D9D68(u16, u16, u16, u16);
extern void fn_800D9E4C(u32);
extern void fn_800D9ED8(u32);
extern void fn_800D9F40(u32);
extern void fn_800DA08C(u32);
extern void fn_800DA3B0(u32, u8);
extern void fn_800DA428(u32);
extern void fn_800DA4C4(u32, u32, u32);
extern void fn_800DB900(u32, void*, s8);
extern void fn_800DB988(u32, u32, u32);
extern void fn_800DB9F0(u32, u32, u32);
extern void fn_800DBA54(u8);
extern void fn_800DBAA4(u32);
extern void fn_800DBB0C(u32, u32, u32);
extern void fn_800DBB84(u32, u32, u32);
extern void fn_800DBBFC(u32, u32, u16, u16, u16, u16, u32, u32, u32, u32);
extern void fn_800DBCE4(u32, u32, u8, u8, u32);
extern void fn_800DBD70(u32, u32, u32, u32, u32, u32, u32, u8, u8, u32);
extern void fn_800DBE5C(u32);
extern void fn_800DBEB4(u32, void*);
extern void fn_800DBF1C(u32, u32);
extern void fn_800DBF78(u32, u32);
extern void fn_800DBFD4(u32, u32, u32, u32, u32);
extern void fn_800DC04C(u32, u32, u32, u32, u8, u32);
extern void fn_800DC0D4(u32, u32, u32, u32, u32);
extern void fn_800DC14C(u32, u32, u32, u32, u8, u32);
extern void fn_800DC1D4(u8);
extern void fn_800DC224(u32, u32, u32, u32, u32);

#if !defined(GS_GFX_RANGE_SPLIT) || \
    defined(GS_GFX_RANGE_800D461C_800D55D0)
/* ==================================================================
 * fn_800D461C -- GSlog_PrintFormatted
 *
 * Varargs printf-like function for the GS debug logging system.
 * At 0x97C (2428) bytes, this is a substantial printf implementation.
 * Uses "0123456789ABCDEF" for hex digit lookup.
 * ================================================================== */
u32* fn_800D461C(u32* command)
{
    u32* p = command + 1;
    u32 copied;
    u32 argument;

    switch (command[0]) {
    case 1:
        fn_800D6A00(p[0]); p += 1; break;
    case 2:
        fn_800D67BC((u16)p[0]); p += 1; break;
    case 3:
        fn_800D6728(); break;
    case 4:
        fn_800D30AC(); break;
    case 5:
        fn_800D6680(((f32*)p)[0], ((f32*)p)[1], ((f32*)p)[2]); p += 3; break;
    case 6:
        fn_800D65CC((u16)p[0], (u16)p[1], (u16)p[2]); p += 3; break;
    case 7:
        fn_800D6518((s16)p[0], (s16)p[1], (s16)p[2]); p += 3; break;
    case 8:
        fn_800D6464((u8)p[0], (u8)p[1], (u8)p[2]); p += 3; break;
    case 9:
        fn_800D63B0((s8)p[0], (s8)p[1], (s8)p[2]); p += 3; break;
    case 10:
        fn_800D631C(((f32*)p)[0], ((f32*)p)[1]); p += 2; break;
    case 11:
        fn_800D6280((u16)p[0], (u16)p[1]); p += 2; break;
    case 12:
        fn_800D61E4((s16)p[0], (s16)p[1]); p += 2; break;
    case 13:
        fn_800D6148((u8)p[0], (u8)p[1]); p += 2; break;
    case 14:
        fn_800D60AC((s8)p[0], (s8)p[1]); p += 2; break;
    case 15:
        fn_800D6028((u16)p[0]); p += 1; break;
    case 16:
        fn_800D5FA4((u8)p[0]); p += 1; break;
    case 17:
        fn_800D5F34(((f32*)p)[0], ((f32*)p)[1], ((f32*)p)[2]); p += 3; break;
    case 18:
        fn_800D5EB4((s16)p[0], (s16)p[1], (s16)p[2]); p += 3; break;
    case 19:
        fn_800D5E34((s8)p[0], (s8)p[1], (s8)p[2]); p += 3; break;
    case 20:
        fn_800D5DD0((u16)p[0]); p += 1; break;
    case 21:
        fn_800D5D6C((u8)p[0]); p += 1; break;
    case 22:
        fn_800D5CB8(p[0], (u8)p[1], (u8)p[2], (u8)p[3], (u8)p[4]); p += 5; break;
    case 23:
        fn_800D5C18(p[0], (u8)p[1], (u8)p[2], (u8)p[3]); p += 4; break;
    case 24:
        fn_800D5BA0(p[0], p[1]); p += 2; break;
    case 25:
        fn_800D5B28(p[0], (u16)p[1]); p += 2; break;
    case 26:
        fn_800D5AB0(p[0], (u16)p[1]); p += 2; break;
    case 27:
        fn_800D5A38(p[0], (u8)p[1]); p += 2; break;
    case 28:
        fn_800D59B8(p[0], ((f32*)p)[1], ((f32*)p)[2]); p += 3; break;
    case 29:
        fn_800D592C(p[0], (u16)p[1], (u16)p[2]); p += 3; break;
    case 30:
        fn_800D58A0(p[0], (s16)p[1], (s16)p[2]); p += 3; break;
    case 31:
        fn_800D5814(p[0], (u8)p[1], (u8)p[2]); p += 3; break;
    case 32:
        fn_800D58A0(p[0], (s8)p[1], (s8)p[2]); p += 3; break;
    case 33:
        fn_800D579C(p[0], (u16)p[1]); p += 2; break;
    case 34:
        fn_800D5724(p[0], (u8)p[1]); p += 2; break;
    case 35:
        fn_800D56C0((u8)p[0]); p += 1; break;
    case 36:
        fn_800D5648(((f32*)p)[0]); p += 1; break;
    case 37:
        fn_800D55D0(((f32*)p)[0]); p += 1; break;
    case 38:
        fn_800D85D4(p[0], p[1]); p += 2; break;
    case 39:
        fn_800D848C(p[0], p[1], p[2], p + 3); p += 15; break;
    case 40:
        fn_800D88DC(p[0]); p += 1; break;
    case 41:
        fn_800D888C(p[0]); p += 1; break;
    case 42:
        GSgfxDLDraw(p[0]); p += 1; break;
    case 43:
        fn_800DA4C4(p[0], p[1], p[2]); p += 3; break;
    case 44:
        fn_800DA428(p[0]); p += 1; break;
    case 45:
        fn_800DA3B0(p[0], (u8)p[1]); p += 2; break;
    case 46:
        fn_800DA2BC(p[0], p[1], p[2]); p += 3; break;
    case 47:
        fn_800DA1E8(p[0], p[1], p[2]); p += 3; break;
    case 48:
        fn_800DA100(p[0], p[1], (u8)p[2], p[3], p[4], (u8)p[5]); p += 6; break;
    case 49:
        fn_800DA08C(p[0]); p += 1; break;
    case 50:
        fn_800DA028(p[0]); p += 1; break;
    case 51:
        fn_800D9FB4(p[0]); p += 1; break;
    case 52:
        fn_800D9F40(p[0]); p += 1; break;
    case 53:
        fn_800D9ED8(p[0]); p += 1; break;
    case 54:
        fn_800D9E4C(p[0]); p += 1; break;
    case 55:
        fn_800D9D68((u16)p[0], (u16)p[1], (u16)p[2], (u16)p[3]); p += 4; break;
    case 56:
        fn_800D9C24((u16)p[0], (u16)p[1], (u16)p[2], (u16)p[3]); p += 4; break;
    case 57:
        fn_800D9BD0(((f32*)p)[0], ((f32*)p)[1], ((f32*)p)[2], ((f32*)p)[3]); p += 4; break;
    case 58:
        fn_800D9B58(((f32*)p)[0], ((f32*)p)[1], ((f32*)p)[2], ((f32*)p)[3]); p += 4; break;
    case 59:
        fn_800D834C(); break;
    case 60:
        fn_800D8284(((f32*)p)[0], ((f32*)p)[1], ((f32*)p)[2]); p += 3; break;
    case 61:
        fn_800D81EC(((f32*)p)[0], ((f32*)p)[1], ((f32*)p)[2]); p += 3; break;
    case 62:
        fn_800D8154(((f32*)p)[0], ((f32*)p)[1], ((f32*)p)[2]); p += 3; break;
    case 63:
        fn_800D8088(p); p += 12; break;
    case 64:
        fn_800D7FE4(p); p += 12; break;
    case 65:
        fn_800D7F14(p); p += 12; break;
    case 66:
        fn_800D7E5C(); break;
    case 67:
        argument = (u8)*p++; fn_800D7D90(argument, p); p += 12; break;
    case 68:
        fn_800D7D10((u8)p[0], p[1]); p += 2; break;
    case 69:
        fn_800D7C74(); break;
    case 70:
        fn_800D7820(p[0]); p += 1; break;
    case 71:
        fn_800D76A8(p[0], (u16)p[1]); p += 2; break;
    case 72:
        fn_800DC224(p[0], p[1], p[2], p[3], p[4]); p += 5; break;
    case 73:
        fn_800DC1D4((u8)p[0]); p += 1; break;
    case 74:
        fn_800DC14C(p[0], p[1], p[2], p[3], (u8)p[4], p[5]); p += 6; break;
    case 75:
        fn_800DC0D4(p[0], p[1], p[2], p[3], p[4]); p += 5; break;
    case 76:
        fn_800DC04C(p[0], p[1], p[2], p[3], (u8)p[4], p[5]); p += 6; break;
    case 77:
        fn_800DBFD4(p[0], p[1], p[2], p[3], p[4]); p += 5; break;
    case 78:
        fn_800DBF78(p[0], p[1]); p += 2; break;
    case 79:
        fn_800DBF1C(p[0], p[1]); p += 2; break;
    case 80:
        argument = p[0];
        p++;
        memcpy(&copied, p, sizeof(copied));
        p++;
        fn_800DBEB4(argument, &copied);
        break;
    case 81:
        fn_800DBE5C(p[0]); p += 1; break;
    case 82:
        fn_800DBD70(p[2], p[3], p[4], p[5], p[6], p[7], p[8], (u8)p[9], (u8)p[0], p[1]); p += 10; break;
    case 83:
        fn_800DBCE4(p[0], p[1], (u8)p[2], (u8)p[3], p[4]); p += 5; break;
    case 84:
        fn_800DBBFC(p[2], p[3], (u16)p[4], (u16)p[5], (u16)p[6], (u16)p[7], p[8], p[9], p[0], p[1]); p += 10; break;
    case 85:
        fn_800DBB84(p[0], p[1], p[2]); p += 3; break;
    case 86:
        fn_800DBB0C(p[0], p[1], p[2]); p += 3; break;
    case 87:
        fn_800DBAA4(p[0]); p += 1; break;
    case 88:
        fn_800DBA54((u8)p[0]); p += 1; break;
    case 89:
        fn_800DB9F0(p[0], p[1], p[2]); p += 3; break;
    case 90:
        fn_800DB988(p[0], p[1], p[2]); p += 3; break;
    case 91:
        argument = p[0];
        p++;
        fn_800DB900(argument, p, (s8)p[25]);
        p += 25;
        break;
    }

    return p;
}
#endif


#if !defined(GS_GFX_RANGE_SPLIT) || \
    defined(GS_GFX_RANGE_800D461C_800D55D0)
/* ==================================================================
 * fn_800D4F98 -- GSlog_QueueCommand
 *
 * Queue a rendering command via the GSlog debug/command system.
 * Used by lighting, material, and draw functions to batch commands.
 * 1388 bytes.
 * ================================================================== */
typedef struct GSgfxCommandState {
    u8 pad_000[0x494];
    u32* commandWrite;
} GSgfxCommandState;

void fn_800D4F98(u32 opcode, u32 paramCount, ...)
{
    typedef struct GSvaList {
        u8 gpr;
        u8 fpr;
        u16 reserved;
        u32* overflow;
        u32* saveArea;
    } GSvaList;
    typedef GSvaList GSvaListArray[1];
    extern void* __va_arg(void*, u32);
    GSgfxCommandState* state;
    GSvaListArray ap;
    union {
        f32 f;
        u32 u;
    } value;

#define PUT_WORD(word)                                                       \
    do {                                                                     \
        *state->commandWrite++ = (u32)(word);                                \
    } while (0)
#define NEXT_WORD() (*(u32*)__va_arg(ap, 1))
#define NEXT_FLOAT()                                                         \
    (value.f = (f32)*(f64*)__va_arg(ap, 3), value.u)

    state = (GSgfxCommandState*)lbl_8047AA80;
    __builtin_va_info(&ap);
    PUT_WORD(opcode);

    switch (paramCount) {
    case 10:
        PUT_WORD(NEXT_WORD());
    case 9:
        PUT_WORD(NEXT_WORD());
    case 8:
        PUT_WORD(NEXT_WORD());
    case 7:
        PUT_WORD(NEXT_WORD());
    case 6:
        PUT_WORD(NEXT_WORD());
    case 5:
        PUT_WORD(NEXT_WORD());
    case 4:
        PUT_WORD(NEXT_WORD());
    case 3:
        PUT_WORD(NEXT_WORD());
    case 2:
        PUT_WORD(NEXT_WORD());
    case 1:
        PUT_WORD(NEXT_WORD());
        break;

    case 14:
        PUT_WORD(NEXT_FLOAT());
    case 13:
        PUT_WORD(NEXT_FLOAT());
    case 12:
        PUT_WORD(NEXT_FLOAT());
    case 11:
        PUT_WORD(NEXT_FLOAT());
        break;

    case 15:
        PUT_WORD(NEXT_WORD());
        PUT_WORD(NEXT_FLOAT());
        PUT_WORD(NEXT_FLOAT());
        break;

    case 16:
        memcpy(state->commandWrite, *(void**)__va_arg(ap, 1), 0x30);
        state->commandWrite += 0x30 / sizeof(u32);
        break;

    case 17:
        PUT_WORD(NEXT_WORD());
        memcpy(state->commandWrite, *(void**)__va_arg(ap, 1), 0x30);
        state->commandWrite += 0x30 / sizeof(u32);
        break;

    case 18:
        PUT_WORD(NEXT_WORD());
        memcpy(state->commandWrite, *(void**)__va_arg(ap, 1), 0x18);
        state->commandWrite = (u32*)((u8*)state->commandWrite + 0x60);
        PUT_WORD(NEXT_WORD());
        break;

    case 19:
        PUT_WORD(NEXT_WORD());
        PUT_WORD(NEXT_WORD());
        PUT_WORD(NEXT_WORD());
        memcpy(state->commandWrite, *(void**)__va_arg(ap, 1), 0x30);
        state->commandWrite += 0x30 / sizeof(u32);
        break;

    case 20:
        PUT_WORD(NEXT_WORD());
        PUT_WORD(*(u32*)__va_arg(ap, 0));
        break;
    }

#undef NEXT_FLOAT
#undef NEXT_WORD
#undef PUT_WORD
}
#endif

#if !defined(GS_GFX_RANGE_SPLIT) || \
    defined(GS_GFX_RANGE_800D45F8_800D461C)
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D45F8(void) {
#include "src/game/gs_render_fn_800D45F8.inc"
}
#else
u32 fn_800D45F8(void) { return *(u32*)lbl_8047AA80; }
#endif

extern u32 lbl_8047AA80;
#if 0
asm void fn_800D4604(void) {
#include "src/game/gs_render_fn_800D4604.inc"
}
#else
void fn_800D4604(u32 val) { *(u32*)lbl_8047AA80 = val; }
#endif

void fn_800D4610(u8 val) {
    GSgfxState* state = (GSgfxState*)lbl_8047AA80;

    state->requestBufferFlag = val;
}
#endif

#if !defined(GS_GFX_RANGE_SPLIT) || \
    defined(GS_GFX_RANGE_800D461C_800D55D0)
void fn_800D5504(u32 size) {
    const GSgfxRequestStrings* strings =
        (const GSgfxRequestStrings*)lbl_802703C0;

    if (((GSgfxState*)lbl_8047AA80)->requestBuffer != 0) {
        GSlogWrite(strings->alreadyAllocated);
        return;
    }

    ((GSgfxState*)lbl_8047AA80)->requestBufferHandle =
        _toolentryAlloc__FUl(size);
    if (((GSgfxState*)lbl_8047AA80)->requestBufferHandle == 0) {
        GSlogWrite(strings->allocationFailed);
        return;
    }

    ((GSgfxState*)lbl_8047AA80)->requestBuffer = fn_800E27B0(
        ((GSgfxState*)lbl_8047AA80)->requestBufferHandle);
    ((GSgfxState*)lbl_8047AA80)->requestBufferPos =
        ((GSgfxState*)lbl_8047AA80)->requestBuffer;
    ((GSgfxState*)lbl_8047AA80)->requestBufferSize = size;
    *(u32*)(lbl_804001F0 + 0x28) = 0;
    GSlogWrite(strings->allocationSucceeded,
               ((GSgfxState*)lbl_8047AA80)->requestBufferSize,
               ((GSgfxState*)lbl_8047AA80)->requestBuffer);
}
#endif

#if !defined(GS_GFX_RANGE_SPLIT) || \
    defined(GS_GFX_RANGE_800D55D0_800D56C0)
extern f32 lbl_8047CA30;
extern f32 lbl_8047CA34;
extern f32 lbl_8047CA38;
/* fn_800D55D0/fn_800D5648 @ 93.33: the dropped-float vararg restored
 * (fn_800D4F98(.., val) -> crset cr1eq). Residual is a single prologue
 * scheduling diff: target issues `lfs f0, lbl_8047CA30@sda21` BEFORE the
 * `stw r0, 0x14(r1)` LR-save; CW always emits the LR store first.
 * scheduling 604 reschedules the whole fn (regress 80); not per-spot
 * controllable. Prologue-scheduler wall. */
#if 0
asm void fn_800D55D0(void) {
#include "src/game/gs_render_fn_800D55D0.inc"
}
#else
void fn_800D55D0(f32 val) {
    if (val < lbl_8047CA30 || val > lbl_8047CA34) return;
    if (*(s32*)lbl_8047AA80 == 1) { fn_800D4F98(0x25, 0xb, val); }
    else {
        u32 tmp;
        tmp = (u32)(s32)(lbl_8047CA38 * val);
        fn_800B944C(tmp, 0);
    }
}
#endif

extern f32 lbl_8047CA30;
extern f32 lbl_8047CA34;
extern f32 lbl_8047CA38;
#if 0
asm void fn_800D5648(void) {
#include "src/game/gs_render_fn_800D5648.inc"
}
#else
void fn_800D5648(f32 val) {
    if (val < lbl_8047CA30 || val > lbl_8047CA34) return;
    if (*(s32*)lbl_8047AA80 == 1) { fn_800D4F98(0x24, 0xb, val); }
    else {
        u32 tmp;
        tmp = (u32)(s32)(lbl_8047CA38 * val);
        fn_800B9404(tmp, 0);
    }
}
#endif
#endif

#if !defined(GS_GFX_RANGE_SPLIT) || \
    defined(GS_GFX_RANGE_800D56C0_800D67BC)
#if 0
asm void fn_800D56C0(void) {
#include "src/game/gs_render_fn_800D56C0.inc"
}
#else
void fn_800D56C0(u8 val) {
    if (!*(u8*)(lbl_8047AA80 + 0x47e) && *(s32*)lbl_8047AA80 == 1) {
        fn_800D4F98(0x23, 1, (u8)val);
    } else {
        *(u32*)(lbl_8047AA80 + 0x4a0) = (u32)fn_800D7230;
        *(u8*)(lbl_8047AA80 + 0x4a4) = val;
    }
}
#endif

#if 0
asm void fn_800D5724(void) {
#include "src/game/gs_render_fn_800D5724.inc"
}
#else
void fn_800D5724(u32 idx, u8 val) {
    if (!*(u8*)(lbl_8047AA80 + 0x47e) && *(s32*)lbl_8047AA80 == 1) {
        fn_800D4F98(0x22, 2, idx, (u32)val);
    } else {
        *(u32*)(lbl_8047AA80 + 0x500 + idx * 4) = (u32)fn_800D724C;
        *(u8*)(lbl_8047AA80 + 0x520 + idx * 16) = val;
    }
}
#endif

#if 0
asm void fn_800D579C(void) {
#include "src/game/gs_render_fn_800D579C.inc"
}
#else
void fn_800D579C(u32 idx, u16 val) {
    if (!*(u8*)(lbl_8047AA80 + 0x47e) && *(s32*)lbl_8047AA80 == 1) {
        fn_800D4F98(0x21, 2, idx, (u32)val);
    } else {
        *(u32*)(lbl_8047AA80 + 0x500 + idx * 4) = (u32)fn_800D7268;
        *(u16*)(lbl_8047AA80 + 0x522 + idx * 16) = val;
    }
}
#endif

#if 0
asm void fn_800D5814(void) {
#include "src/game/gs_render_fn_800D5814.inc"
}
#else
void fn_800D5814(u32 idx, u8 a, u8 b) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) { fn_800D4F98(0x1f, 3, idx, (u32)a, (u32)b); }
    else {
        *(s32*)(state + 0x500 + idx * 4) = (u32)fn_800D72A4;
        *(u8*)(lbl_8047AA80 + 0x520 + idx * 16) = a;
        *(u8*)(lbl_8047AA80 + 0x521 + idx * 16) = b;
    }
}
#endif

#if 0
asm void fn_800D58A0(void) {
#include "src/game/gs_render_fn_800D58A0.inc"
}
#else
void fn_800D58A0(u32 idx, s16 a, s16 b) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) { fn_800D4F98(0x1e, 3, idx, (s32)a, (s32)b); }
    else {
        *(s32*)(state + 0x500 + idx * 4) = (u32)fn_800D72C4;
        *(u16*)(lbl_8047AA80 + 0x522 + idx * 16) = a;
        *(u16*)(lbl_8047AA80 + 0x524 + idx * 16) = b;
    }
}
#endif

#if 0
asm void fn_800D592C(void) {
#include "src/game/gs_render_fn_800D592C.inc"
}
#else
void fn_800D592C(u32 idx, u16 a, u16 b) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) { fn_800D4F98(0x1d, 3, idx, (u32)a, (u32)b); }
    else {
        *(s32*)(state + 0x500 + idx * 4) = (u32)fn_800D72E4;
        *(u16*)(lbl_8047AA80 + 0x522 + idx * 16) = a;
        *(u16*)(lbl_8047AA80 + 0x524 + idx * 16) = b;
    }
}
#endif

#if 0
asm void fn_800D59B8(void) {
#include "src/game/gs_render_fn_800D59B8.inc"
}
#else
void fn_800D59B8(u32 idx, f32 x, f32 y) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) { fn_800D4F98(0x1c, 0xf, idx, x, y); }
    else {
        *(u32*)(state + 0x500 + idx * 4) = (u32)fn_800D7304;
        *(f32*)(lbl_8047AA80 + 0x528 + idx * 16) = x;
        *(f32*)(lbl_8047AA80 + 0x52c + idx * 16) = y;
    }
}
#endif

#if 0
asm void fn_800D5A38(void) {
#include "src/game/gs_render_fn_800D5A38.inc"
}
#else
void fn_800D5A38(u32 idx, u8 val) {
    if (!*(u8*)(lbl_8047AA80 + 0x47e) && *(s32*)lbl_8047AA80 == 1) {
        fn_800D4F98(0x1b, 2, idx, (u32)val);
    } else {
        *(u32*)(lbl_8047AA80 + 0x4e0 + idx * 4) = (u32)fn_800D7328;
        *(u8*)(lbl_8047AA80 + 0x4e8 + idx * 12) = val;
    }
}
#endif

#if 0
asm void fn_800D5AB0(void) {
#include "src/game/gs_render_fn_800D5AB0.inc"
}
#else
void fn_800D5AB0(u32 idx, u16 val) {
    if (!*(u8*)(lbl_8047AA80 + 0x47e) && *(s32*)lbl_8047AA80 == 1) {
        fn_800D4F98(0x1a, 2, idx, (u32)val);
    } else {
        *(u32*)(lbl_8047AA80 + 0x4e0 + idx * 4) = (u32)fn_800D7344;
        *(u16*)(lbl_8047AA80 + 0x4ec + idx * 12) = val;
    }
}
#endif

#if 0
asm void fn_800D5B28(void) {
#include "src/game/gs_render_fn_800D5B28.inc"
}
#else
void fn_800D5B28(u32 idx, u16 val) {
    if (!*(u8*)(lbl_8047AA80 + 0x47e) && *(s32*)lbl_8047AA80 == 1) {
        fn_800D4F98(0x19, 2, idx, (u32)val);
    } else {
        *(u32*)(lbl_8047AA80 + 0x4e0 + idx * 4) = (u32)fn_800D7360;
        *(u16*)(lbl_8047AA80 + 0x4ec + idx * 12) = val;
    }
}
#endif

#if 0
asm void fn_800D5BA0(void) {
#include "src/game/gs_render_fn_800D5BA0.inc"
}
#else
void fn_800D5BA0(u32 idx, u32 val) {
    if (!*(u8*)(lbl_8047AA80 + 0x47e) && *(s32*)lbl_8047AA80 == 1) {
        fn_800D4F98(0x18, 2, idx, val);
    } else {
        *(u32*)(lbl_8047AA80 + 0x4e0 + idx * 4) = (u32)fn_800D737C;
        *(u32*)(lbl_8047AA80 + 0x4f0 + idx * 12) = val;
    }
}
#endif

#if 0
asm void fn_800D5C18(void) {
#include "src/game/gs_render_fn_800D5C18.inc"
}
#else
void fn_800D5C18(u32 idx, u8 a, u8 b, u8 c) {
    u32 state = lbl_8047AA80;
    u32 off;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) {
        fn_800D4F98(0x17, 4, idx, (u32)a, (u32)b, (u32)c);
    } else {
        off = idx * 0xc;
        {
            u32 s = lbl_8047AA80;
            *(u32*)(s + 0x4e0 + idx * 4) = (u32)fn_800D7398;
        }
        {
            u32 s = lbl_8047AA80;
            *(u8*)(s + 0x4e8 + off) = a;
        }
        {
            u32 s = lbl_8047AA80;
            *(u8*)(s + 0x4e9 + off) = b;
        }
        {
            u32 s = lbl_8047AA80;
            *(u8*)(s + 0x4ea + off) = c;
        }
    }
}
#endif

#if 0
asm void fn_800D5CB8(void) {
#include "src/game/gs_render_fn_800D5CB8.inc"
}
#else
void fn_800D5CB8(u32 idx, u8 a, u8 b, u8 c, u8 d) {
    u32 state = lbl_8047AA80;
    u32 off;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) {
        fn_800D4F98(0x16, 5, idx, (u32)a, (u32)b, (u32)c, (u32)d);
    } else {
        off = idx * 0xc;
        {
            u32 s = lbl_8047AA80;
            *(u32*)(s + 0x4e0 + idx * 4) = (u32)fn_800D73C4;
        }
        {
            u32 s = lbl_8047AA80;
            *(u8*)(s + 0x4e8 + off) = a;
        }
        {
            u32 s = lbl_8047AA80;
            *(u8*)(s + 0x4e9 + off) = b;
        }
        {
            u32 s = lbl_8047AA80;
            *(u8*)(s + 0x4ea + off) = c;
        }
        {
            u32 s = lbl_8047AA80;
            *(u8*)(s + 0x4eb + off) = d;
        }
    }
}
#endif

#if 0
asm void fn_800D5D6C(void) {
#include "src/game/gs_render_fn_800D5D6C.inc"
}
#else
void fn_800D5D6C(u8 val) {
    if (!*(u8*)(lbl_8047AA80 + 0x47e) && *(s32*)lbl_8047AA80 == 1) {
        fn_800D4F98(0x15, 1, (u8)val);
    } else {
        *(s32*)(lbl_8047AA80 + 0x4c4) = (u32)fn_800D73F8;
        *(u8*)(lbl_8047AA80 + 0x4c8) = val;
    }
}
#endif

#if 0
asm void fn_800D5DD0(void) {
#include "src/game/gs_render_fn_800D5DD0.inc"
}
#else
void fn_800D5DD0(u16 val) {
    if (!*(u8*)(lbl_8047AA80 + 0x47e) && *(s32*)lbl_8047AA80 == 1) {
        fn_800D4F98(0x14, 1, (u16)val);
    } else {
        *(s32*)(lbl_8047AA80 + 0x4c4) = (u32)fn_800D740C;
        *(u16*)(lbl_8047AA80 + 0x4cc) = val;
    }
}
#endif

#if 0
asm void fn_800D5E34(void) {
#include "src/game/gs_render_fn_800D5E34.inc"
}
#else
void fn_800D5E34(s8 a, s8 b, s8 c) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) { fn_800D4F98(0x13, 3, (s32)a, (s32)b, (s32)c); }
    else {
        *(s32*)(state + 0x4c4) = (u32)fn_800D7420;
        *(u8*)(lbl_8047AA80 + 0x4c8) = a;
        *(u8*)(lbl_8047AA80 + 0x4c9) = b;
        *(u8*)(lbl_8047AA80 + 0x4ca) = c;
    }
}
#endif

#if 0
asm void fn_800D5EB4(void) {
#include "src/game/gs_render_fn_800D5EB4.inc"
}
#else
void fn_800D5EB4(s16 a, s16 b, s16 c) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) { fn_800D4F98(0x12, 3, (s32)a, (s32)b, (s32)c); }
    else {
        *(s32*)(state + 0x4c4) = (u32)fn_800D7444;
        *(u16*)(lbl_8047AA80 + 0x4cc) = a;
        *(u16*)(lbl_8047AA80 + 0x4ce) = b;
        *(u16*)(lbl_8047AA80 + 0x4d0) = c;
    }
}
#endif

#if 0
asm void fn_800D5F34(void) {
#include "src/game/gs_render_fn_800D5F34.inc"
}
#else
void fn_800D5F34(f32 x, f32 y, f32 z) {
    if (!*(u8*)(lbl_8047AA80 + 0x47e) && *(s32*)lbl_8047AA80 == 1) {
        fn_800D4F98(0x11, 0xd, x, y, z);
    } else {
        *(s32*)(lbl_8047AA80 + 0x4c4) = (u32)fn_800D7468;
        *(f32*)(lbl_8047AA80 + 0x4d4) = x;
        *(f32*)(lbl_8047AA80 + 0x4d8) = y;
        *(f32*)(lbl_8047AA80 + 0x4dc) = z;
    }
}
#endif

#if 0
asm void fn_800D5FA4(void) {
#include "src/game/gs_render_fn_800D5FA4.inc"
}
#else
void fn_800D5FA4(u8 val) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) { fn_800D4F98(0x10, 1, (u32)val); }
    else {
        fn_800D6B00();
        *(u32*)(lbl_8047AA80 + 0x4a8) = (u32)fn_800D748C;
        *(u8*)(lbl_8047AA80 + 0x4ac) = val;
        *(u8*)(lbl_8047AA80 + 0x49f) = 1;
    }
}
#endif

#if 0
asm void fn_800D6028(void) {
#include "src/game/gs_render_fn_800D6028.inc"
}
#else
void fn_800D6028(u16 val) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) { fn_800D4F98(0xf, 1, (u32)val); }
    else {
        fn_800D6B00();
        *(u32*)(lbl_8047AA80 + 0x4a8) = (u32)fn_800D74A0;
        *(u16*)(lbl_8047AA80 + 0x4b0) = val;
        *(u8*)(lbl_8047AA80 + 0x49f) = 1;
    }
}
#endif

#if 0
asm void fn_800D60AC(void) {
#include "src/game/gs_render_fn_800D60AC.inc"
}
#else
void fn_800D60AC(s8 a, s8 b) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) {
        fn_800D4F98(0xe, 2, (s32)a, (s32)b);
    } else {
        fn_800D6B00();
        *(u32*)(lbl_8047AA80 + 0x4a8) = (u32)fn_800D74B4;
        *(u8*)(lbl_8047AA80 + 0x4ac) = a;
        *(u8*)(lbl_8047AA80 + 0x4ad) = b;
        *(u8*)(lbl_8047AA80 + 0x49f) = 1;
    }
}
#endif

#if 0
asm void fn_800D6148(void) {
#include "src/game/gs_render_fn_800D6148.inc"
}
#else
void fn_800D6148(u8 a, u8 b) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) {
        fn_800D4F98(0xd, 2, (u32)a, (u32)b);
    } else {
        fn_800D6B00();
        *(u32*)(lbl_8047AA80 + 0x4a8) = (u32)fn_800D74D0;
        *(u8*)(lbl_8047AA80 + 0x4ac) = a;
        *(u8*)(lbl_8047AA80 + 0x4ad) = b;
        *(u8*)(lbl_8047AA80 + 0x49f) = 1;
    }
}
#endif

#if 0
asm void fn_800D61E4(void) {
#include "src/game/gs_render_fn_800D61E4.inc"
}
#else
void fn_800D61E4(s16 a, s16 b) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) {
        fn_800D4F98(0xc, 2, (s32)a, (s32)b);
    } else {
        fn_800D6B00();
        *(u32*)(lbl_8047AA80 + 0x4a8) = (u32)fn_800D74EC;
        *(u16*)(lbl_8047AA80 + 0x4b0) = a;
        *(u16*)(lbl_8047AA80 + 0x4b2) = b;
        *(u8*)(lbl_8047AA80 + 0x49f) = 1;
    }
}
#endif

#if 0
asm void fn_800D6280(void) {
#include "src/game/gs_render_fn_800D6280.inc"
}
#else
void fn_800D6280(u16 a, u16 b) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) {
        fn_800D4F98(0xb, 2, (u32)a, (u32)b);
    } else {
        fn_800D6B00();
        *(u32*)(lbl_8047AA80 + 0x4a8) = (u32)fn_800D7508;
        *(u16*)(lbl_8047AA80 + 0x4b0) = a;
        *(u16*)(lbl_8047AA80 + 0x4b2) = b;
        *(u8*)(lbl_8047AA80 + 0x49f) = 1;
    }
}
#endif

#if 0
asm void fn_800D631C(void) {
#include "src/game/gs_render_fn_800D631C.inc"
}
#else
void fn_800D631C(f32 a, f32 b) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) {
        fn_800D4F98(0xa, 0xc, a, b);
    } else {
        fn_800D6B00();
        *(u32*)(lbl_8047AA80 + 0x4a8) = (u32)fn_800D7524;
        *(f32*)(lbl_8047AA80 + 0x4b8) = a;
        *(f32*)(lbl_8047AA80 + 0x4bc) = b;
        *(u8*)(lbl_8047AA80 + 0x49f) = 1;
    }
}
#endif

#if 0
asm void fn_800D63B0(void) {
#include "src/game/gs_render_fn_800D63B0.inc"
}
#else
void fn_800D63B0(s8 a, s8 b, s8 c) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) {
        fn_800D4F98(9, 3, (s32)a, (s32)b, (s32)c);
    } else {
        fn_800D6B00();
        *(u32*)(lbl_8047AA80 + 0x4a8) = (u32)fn_800D7540;
        *(u8*)(lbl_8047AA80 + 0x4ac) = a;
        *(u8*)(lbl_8047AA80 + 0x4ad) = b;
        *(u8*)(lbl_8047AA80 + 0x4ae) = c;
        *(u8*)(lbl_8047AA80 + 0x49f) = 1;
    }
}
#endif

#if 0
asm void fn_800D6464(void) {
#include "src/game/gs_render_fn_800D6464.inc"
}
#else
void fn_800D6464(u8 a, u8 b, u8 c) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) {
        fn_800D4F98(8, 3, (u32)a, (u32)b, (u32)c);
    } else {
        fn_800D6B00();
        *(u32*)(lbl_8047AA80 + 0x4a8) = (u32)fn_800D7564;
        *(u8*)(lbl_8047AA80 + 0x4ac) = a;
        *(u8*)(lbl_8047AA80 + 0x4ad) = b;
        *(u8*)(lbl_8047AA80 + 0x4ae) = c;
        *(u8*)(lbl_8047AA80 + 0x49f) = 1;
    }
}
#endif

#if 0
asm void fn_800D6518(void) {
#include "src/game/gs_render_fn_800D6518.inc"
}
#else
void fn_800D6518(s16 a, s16 b, s16 c) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) {
        fn_800D4F98(7, 3, (s32)a, (s32)b, (s32)c);
    } else {
        fn_800D6B00();
        *(u32*)(lbl_8047AA80 + 0x4a8) = (u32)fn_800D7588;
        *(s16*)(lbl_8047AA80 + 0x4b0) = a;
        *(s16*)(lbl_8047AA80 + 0x4b2) = b;
        *(s16*)(lbl_8047AA80 + 0x4b4) = c;
        *(u8*)(lbl_8047AA80 + 0x49f) = 1;
    }
}
#endif

#if 0
asm void fn_800D65CC(void) {
#include "src/game/gs_render_fn_800D65CC.inc"
}
#else
void fn_800D65CC(u16 a, u16 b, u16 c) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) {
        fn_800D4F98(6, 3, (u32)a, (u32)b, (u32)c);
    } else {
        fn_800D6B00();
        *(u32*)(lbl_8047AA80 + 0x4a8) = (u32)fn_800D75AC;
        *(u16*)(lbl_8047AA80 + 0x4b0) = a;
        *(u16*)(lbl_8047AA80 + 0x4b2) = b;
        *(u16*)(lbl_8047AA80 + 0x4b4) = c;
        *(u8*)(lbl_8047AA80 + 0x49f) = 1;
    }
}
#endif

#if 0
asm void fn_800D6680(void) {
#include "src/game/gs_render_fn_800D6680.inc"
}
#else
void fn_800D6680(f32 a, f32 b, f32 c) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) {
        fn_800D4F98(5, 0xd, a, b, c);
    } else {
        fn_800D6B00();
        *(u32*)(lbl_8047AA80 + 0x4a8) = (u32)fn_800D75D0;
        *(f32*)(lbl_8047AA80 + 0x4b8) = a;
        *(f32*)(lbl_8047AA80 + 0x4bc) = b;
        *(f32*)(lbl_8047AA80 + 0x4c0) = c;
        *(u8*)(lbl_8047AA80 + 0x49f) = 1;
    }
}
#endif

#if 0
asm void fn_800D6728(void) {
#include "src/game/gs_render_fn_800D6728.inc"
}
#else
void fn_800D6728(void) {
    u32 state = lbl_8047AA80;
    if (*(u8*)(state + 0x47e) == 1) {
        fn_800D6B00();
        return;
    }
    if (*(s32*)state == 1) {
        fn_800D4F98(3, 0);
        return;
    }
    fn_800D6B00();
    {
        u32 s = lbl_8047AA80;
        if (*(u8*)(s + 0x1b) == *(u8*)(s + 0x1a) &&
            (*(u32*)(s + 0x4) & *(u32*)(s + 0x8)) &&
            *(u32*)(s + 0x24) == *(u32*)(s + 0x20)) {
            *(u32*)(s + 0x24) = 0;
        }
    }
}
#endif
#endif

#if !defined(GS_GFX_RANGE_SPLIT) || \
    defined(GS_GFX_RANGE_800D67BC_800D6A00)
extern u8 lbl_80314350[];
extern u8 lbl_804001F0[];
#if 0
asm void fn_800D67BC(void) {
#include "src/game/gs_render_fn_800D67BC.inc"
}
#else
void fn_800D67BC(u16 vertCount) {
    u32 state;
    u32 obj;
    u16 drawCount;

    state = lbl_8047AA80;
    if (*(u8*)(state + 0x47e) == 1) {
        fn_800DB758(vertCount);
        return;
    }
    if (*(s32*)state == 1) {
        fn_800D4F98(2, 1, (u32)vertCount);
        return;
    }
    if (*(u8*)(state + 0x1b) != *(u8*)(state + 0x1a)) {
        return;
    }
    if ((*(u32*)(state + 0x4) & *(u32*)(state + 0x8)) == 0) {
        return;
    }

    obj = *(u32*)(state + 0x24);
    if (obj == 0) {
        obj = state + 0x20;
        fn_800D7650((u8*)obj);
        fn_800D7868((u8*)obj, 1, 0, 1, 4, 0, 0, 0);
        if (*(u32*)(state + 0x10) & 4) {
            fn_800D7868((u8*)obj, 2, 0, 2, 4, 0, 0, 0);
        }
        if (*(u32*)(state + 0x10) & 1) {
            fn_800D7868((u8*)obj, 4, 0, 6, 10, 0, 0, 0);
        }
        if (*(u32*)(state + 0x10) & 2) {
            fn_800D7868((u8*)obj, 6, 0, 8, 4, 0, 0, 0);
        }
        *(u32*)(state + 0x24) = obj;
    }

    drawCount = vertCount;
    if (*(s32*)(state + 0x14) == 7) {
        drawCount = (u16)((vertCount & 0x7fff) << 1);
    }

    fn_800D7A70(obj);
    fn_800D892C(obj);
    fn_800B928C(((u32*)lbl_80314350)[*(u32*)(state + 0x14)],
                *(u32*)(obj + 0x4), drawCount);
    fn_800D6A80(drawCount, *(s32*)(state + 0x14),
                (u32*)(lbl_804001F0 + 0xc), (u32*)(lbl_804001F0 + 0x4));
}
#endif
#endif

#if !defined(GS_GFX_RANGE_SPLIT) || \
    defined(GS_GFX_RANGE_800D6A00_800D6B00)
#if 0
asm void fn_800D6A00(void) {
#include "src/game/gs_render_fn_800D6A00.inc"
}
#else
void fn_800D6A00(u32 val) {
    if (*(u8*)(lbl_8047AA80 + 0x47e) == 1) { *(s32*)(lbl_8047AA80 + 0x488) = val; }
    else if (*(s32*)lbl_8047AA80 == 1) { fn_800D4F98(0x1, 1, val); }
    else { *(s32*)(lbl_8047AA80 + 0x14) = val; }
}
#endif

#if 0
asm void fn_800D6A5C(void) {
#include "src/game/gs_render_fn_800D6A5C.inc"
}
#else
void fn_800D6A5C(u32 a, u32 b) { *(u32*)(lbl_804001F0 + 0xc) += a; *(u32*)(lbl_804001F0 + 0x4) += b; }
#endif

#if 0
asm void fn_800D6A80(void) {
#include "src/game/gs_render_fn_800D6A80.inc"
}
#else
void fn_800D6A80(u16 vertCount, s32 type, u32* totalVerts, u32* totalPrims) {
    *totalVerts += vertCount;
    switch (type) {
        case 3: *totalPrims += vertCount / 3; break;
        case 4:
        case 5: *totalPrims += vertCount - 2; break;
        case 6:
        case 7: *totalPrims += (vertCount >> 1); break;
    }
}
#endif
#endif
