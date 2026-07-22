/**
 * @file musyx_input_aux_exact_80161934.c
 * @brief Exact natural-C MusyX auxiliary-input initialization functions.
 */

#include "dolphin/types.h"

typedef struct CtrlSource {
    u8 midiCtrl;
    u8 combine;
    s32 scale;
} CtrlSource;

typedef struct CtrlDest {
    CtrlSource source[4];
    u16 oldValue;
    u8 numSource;
} CtrlDest;

typedef struct SndInputVoice {
    u8 pad_000[0xA8];
    u8 timeUsedByInput;
    u8 pad_0A9[0x1D4 - 0x0A9];
    u8 lfoUsedByInput[2];
    u8 pad_1D6[0x214 - 0x1D6];
    u32 midiDirtyFlags;
    CtrlDest inpVolume;
    CtrlDest inpPanning;
    CtrlDest inpSurroundPanning;
    CtrlDest inpPitchBend;
    CtrlDest inpDoppler;
    CtrlDest inpModulation;
    CtrlDest inpPedal;
    CtrlDest inpPortamento;
    CtrlDest inpPreAuxA;
    CtrlDest inpReverb;
    CtrlDest inpPreAuxB;
    CtrlDest inpPostAuxB;
    CtrlDest inpTremolo;
} SndInputVoice;

extern u32 lbl_80449390[];
extern u32 lbl_80369C90[];
extern u32 lbl_80369CA0[];
extern u8 lbl_80435B74[];
extern u8 lbl_804356F4[];

extern u32 _GetInputValue(u8* obj, u8* motionBase, u8 midi, u8 midiSet);

u32 fn_80161934(u8 idx, u8 index, u8 midi, u8 midiSet)
{
    u32 mask =
        lbl_80369C90[index] & ((u32(*)[16])lbl_80449390)[midiSet][midi];
    u32 nonzero = mask != 0;

    if (nonzero) {
        ((u32(*)[16])lbl_80449390)[midiSet][midi] &=
            ~lbl_80369C90[index];
    }
    if (nonzero) {
        return _GetInputValue(NULL,
                              lbl_80435B74 + (u32)idx * 0x90 +
                                  (u32)index * 0x24,
                              midi, midiSet);
    } else {
        u32 offset = (u32)idx * 0x90;
        u8* input = lbl_80435B74;
        input += offset;
        input += (u32)index * 0x24;
        return *(u16*)(input + 0x20);
    }
}

u32 fn_801619E8(u8 idx, u8 index, u8 midi, u8 midiSet)
{
    u32 mask =
        lbl_80369CA0[index] & ((u32(*)[16])lbl_80449390)[midiSet][midi];
    u32 nonzero = mask != 0;

    if (nonzero) {
        ((u32(*)[16])lbl_80449390)[midiSet][midi] &=
            ~lbl_80369CA0[index];
    }
    if (nonzero) {
        return _GetInputValue(NULL,
                              lbl_804356F4 + (u32)idx * 0x90 +
                                  (u32)index * 0x24,
                              midi, midiSet);
    } else {
        u32 offset = (u32)idx * 0x90;
        u8* input = lbl_804356F4;
        input += offset;
        input += (u32)index * 0x24;
        return *(u16*)(input + 0x20);
    }
}

static inline void inpResetGlobalMIDIDirtyFlags(void)
{
    u32 i;
    u32 j;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 16; j++) {
            ((u32(*)[16])lbl_80449390)[i][j] = 0xFF;
        }
    }
}

void fn_80161A9C(SndInputVoice* voice)
{
    u32 i;
    u32 studio;

    if (voice != NULL) {
        voice->inpVolume.source[0].midiCtrl = 7;
        voice->inpVolume.source[0].combine = 0;
        voice->inpVolume.source[0].scale = 0x10000;
        voice->inpVolume.source[1].midiCtrl = 11;
        voice->inpVolume.source[1].combine = 2;
        voice->inpVolume.source[1].scale = 0x10000;
        voice->inpVolume.numSource = 2;
        voice->inpPanning.source[0].midiCtrl = 10;
        voice->inpPanning.source[0].combine = 0;
        voice->inpPanning.source[0].scale = 0x10000;
        voice->inpPanning.numSource = 1;
        voice->inpSurroundPanning.source[0].midiCtrl = 131;
        voice->inpSurroundPanning.source[0].combine = 0;
        voice->inpSurroundPanning.source[0].scale = 0x10000;
        voice->inpSurroundPanning.numSource = 1;
        voice->inpPitchBend.source[0].midiCtrl = 128;
        voice->inpPitchBend.source[0].combine = 0;
        voice->inpPitchBend.source[0].scale = 0x10000;
        voice->inpPitchBend.numSource = 1;
        voice->inpModulation.source[0].midiCtrl = 1;
        voice->inpModulation.source[0].combine = 0;
        voice->inpModulation.source[0].scale = 0x10000;
        voice->inpModulation.numSource = 1;
        voice->inpPedal.source[0].midiCtrl = 64;
        voice->inpPedal.source[0].combine = 0;
        voice->inpPedal.source[0].scale = 0x10000;
        voice->inpPedal.numSource = 1;
        voice->inpPortamento.source[0].midiCtrl = 65;
        voice->inpPortamento.source[0].combine = 0;
        voice->inpPortamento.source[0].scale = 0x10000;
        voice->inpPortamento.numSource = 1;
        voice->inpPreAuxA.numSource = 0;
        voice->inpReverb.source[0].midiCtrl = 91;
        voice->inpReverb.source[0].combine = 0;
        voice->inpReverb.source[0].scale = 0x10000;
        voice->inpReverb.numSource = 1;
        voice->inpPreAuxB.numSource = 0;
        voice->inpPostAuxB.source[0].midiCtrl = 93;
        voice->inpPostAuxB.source[0].combine = 0;
        voice->inpPostAuxB.source[0].scale = 0x10000;
        voice->inpPostAuxB.numSource = 1;
        voice->inpDoppler.source[0].midiCtrl = 132;
        voice->inpDoppler.source[0].combine = 0;
        voice->inpDoppler.source[0].scale = 0x10000;
        voice->inpDoppler.numSource = 1;
        voice->inpTremolo.numSource = 0;
        voice->midiDirtyFlags = 0x1FFF;
        voice->lfoUsedByInput[0] = 0;
        voice->lfoUsedByInput[1] = 0;
        voice->timeUsedByInput = 0;
    } else {
        CtrlDest (*auxA)[4] = (CtrlDest(*)[4])lbl_80435B74;
        CtrlDest (*auxB)[4] = (CtrlDest(*)[4])lbl_804356F4;

        for (studio = 0; studio < 8; studio++) {
            for (i = 0; i < 4; i++) {
                auxA[studio][i].numSource = 0;
                auxB[studio][i].numSource = 0;
            }
        }
        inpResetGlobalMIDIDirtyFlags();
    }
}
