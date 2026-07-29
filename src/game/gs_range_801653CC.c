/**
 * @file gs_range_801653CC.c
 * @brief Candidate gs-engine suffix, 0x80166E88 - 0x80167040.
 *
 * No source body is currently available for fn_80166E88.
 */
#include "game/gs_range_801653CC_shared.h"

typedef struct GSsndInitSettings {
    u32 field_00;
    u32 field_04;
} GSsndInitSettings;

extern const u32 lbl_8047D558;
extern const u32 lbl_8047D55C;
extern u32 lbl_8047B0C4;
extern u32 lbl_8047B0C8;
extern u32 lbl_8047B0CC;
extern u32 lbl_8047B0D0;
extern u32 lbl_8047B0D4;
extern u32 lbl_8047B0D8;
extern u32 lbl_8047B0DC;
extern u32 lbl_8047B0E0;
extern void* fn_80167BB0(u32 size);
extern void fn_80167A6C(void);
extern void fn_80167A14(void);
extern void fn_801679E4(void);
extern void _sndInitStack(void);
extern void fn_801631AC(GSsndInitSettings* settings);
extern s32 fn_8015FE88(u32, u32, u32, u32, u32, u32);
extern void sndAuxCallbackPrepareReverbHI(void* work);
extern void sndAuxCallbackReverbHI(void);
extern void sndSetAuxProcessingCallbacks(u32, void*, void*, u32, u32, u32,
                                         u32, u32, u32);
extern u32 GSsndGetOutputMode(void);
extern void fn_80166CC0(u32 mode);
extern void fn_80167040(void);
extern void sndSetReceiveMessageCallback(void* callback);

u32 fn_80166E88(u32 stackCount, u32 workSize, u32 extraStackCount,
                u32 emitterCount, u32 emitter3dCount)
{
    GSsndInitSettings settings;
    u32 totalStackCount;

    totalStackCount = stackCount + extraStackCount;
    settings.field_00 = lbl_8047D558;
    settings.field_04 = lbl_8047D55C;
    lbl_8047B0E8 = *lbl_80478FA8;
    lbl_8047B0E4 = -1;
    lbl_8047B0E0 = totalStackCount;

    lbl_8047B0DC = (u32)fn_80167BB0(totalStackCount * 0x14);
    if (lbl_8047B0DC == 0) {
        return 0;
    }
    fn_80167A6C();

    lbl_8047B0D8 = workSize;
    lbl_8047B0D4 = (u32)fn_80167BB0(workSize);
    if (lbl_8047B0D4 == 0) {
        return 0;
    }
    _sndInitStack();

    lbl_8047B0D0 = emitterCount;
    lbl_8047B0CC = (u32)fn_80167BB0(emitterCount * 0xD0);
    if (lbl_8047B0CC == 0) {
        return 0;
    }
    fn_80167A14();

    lbl_8047B0C8 = emitter3dCount;
    lbl_8047B0C4 = (u32)fn_80167BB0(emitter3dCount * 0x78);
    if (lbl_8047B0C4 == 0) {
        return 0;
    }
    fn_801679E4();

    fn_801631AC(&settings);
    if (fn_8015FE88(0x40, 0x30, 0x10, 1, 0, 0x9FC000) != 0) {
        return 0;
    }

    _sndSetReverbParm(0);
    sndAuxCallbackPrepareReverbHI(lbl_80452500);
    sndSetAuxProcessingCallbacks(0, sndAuxCallbackReverbHI, lbl_80452500,
                                 0xFF, 0, 0, 0, 0xFF, 0);
    fn_80166D48(0x7F, 0, 1, 0);
    fn_80166D48(0x64, 0, 0, 1);
    fn_80166CC0(GSsndGetOutputMode());
    sndSetReceiveMessageCallback(fn_80167040);
    return 1;
}
