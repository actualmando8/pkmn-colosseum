/**
 * @file musyx_voice_last_exact_80158934.c
 * @brief MusyX last-started voice bookkeeping, 0x80158934 - 0x80158BB4.
 */

#include "dolphin/types.h"

typedef struct SynthVoiceLastStarted {
    u8 pad_000[0xF4];
    u32 id;
    u8 pad_0F8[0x121 - 0xF8];
    u8 midi;
    u8 midiSet;
} SynthVoiceLastStarted;

extern u8 lbl_80446E50[64];
extern u8 lbl_80446E90[8][16];

u32 voiceIsLastStarted(SynthVoiceLastStarted* voice)
{
    u32 index;

    if (voice->id != 0xFFFFFFFF && voice->midi != 0xFF) {
        index = voice->id & 0xFF;
        if (voice->midiSet == 0xFF) {
            if (lbl_80446E50[index] == index) {
                return TRUE;
            }
        } else if (lbl_80446E90[voice->midiSet][voice->midi] == index) {
            return TRUE;
        }
    }

    return FALSE;
}

void voiceSetLastStarted(SynthVoiceLastStarted* voice)
{
    u32 index;

    if (voice->id != 0xFFFFFFFF && voice->midi != 0xFF) {
        index = voice->id & 0xFF;
        if (voice->midiSet == 0xFF) {
            lbl_80446E50[index] = index;
        } else {
            lbl_80446E90[voice->midiSet][voice->midi] = index;
        }
    }
}

void voiceResetLastStarted(SynthVoiceLastStarted* voice)
{
    u32 index;

    if (voice->id != 0xFFFFFFFF && voice->midi != 0xFF) {
        index = voice->id & 0xFF;
        if (voice->midiSet == 0xFF) {
            if (lbl_80446E50[index] == index) {
                lbl_80446E50[index] = 0xFF;
            }
        } else if (index == lbl_80446E90[voice->midiSet][voice->midi]) {
            lbl_80446E90[voice->midiSet][voice->midi] = 0xFF;
        }
    }
}

void voiceInitLastStarted(void)
{
    u32 i;
    u32 j;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 16; j++) {
            lbl_80446E90[i][j] = 0xFF;
        }
    }

    for (j = 0; j < 64; j++) {
        lbl_80446E50[j] = 0xFF;
    }
}
