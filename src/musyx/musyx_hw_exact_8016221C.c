/**
 * @file musyx_hw_exact_8016221C.c
 * @brief Exact MusyX hardware translation unit, 0x8016221C - 0x80162FB0.
 */

#include "dolphin/types.h"

typedef struct PeopleFieldMoveSlot {
    u8 pad_00[0x1C];
    u32 field_1C;
    u8 pad_20[0x4];
    u32 flags_24[0x13];
    u16 field_70;
    u8 pad_72[0x1E];
    u8 field_90;
    u8 pad_91[0x3];
    u32 field_94;
    u32 field_98;
    u8 field_9C;
    u8 pad_9D[0x3];
    u8 field_A0;
    u8 pad_A1[0x2B];
    u16 field_CC;
    u16 field_CE;
    u16 field_D0;
    u16 field_D2;
    u8 field_D4;
    u8 pad_D5[0x13];
    u32 field_E8;
    u8 active;
    u8 field_ED;
    u8 field_EE;
    u8 pad_EF;
    u32 field_F0;
} PeopleFieldMoveSlot;

typedef union HwVolumeStudio {
    struct {
        u32 allocation;
        u8 pad_04[0x24];
        u32 auxAllocation;
        u8 pad_2C[0x24];
        u8 state;
        u8 isMaster;
        u8 numInputs;
        u8 pad_53;
        s32 type;
        u8 pad_58[0x64];
    } named;
    u8 bytes[0xBC];
} HwVolumeStudio;

typedef union HwVolumeInfo {
    struct {
        f32 volL;
        f32 volR;
        f32 volS;
        f32 volAuxAL;
        f32 volAuxAR;
        f32 volAuxAS;
        f32 volAuxBL;
        f32 volAuxBR;
        f32 volAuxBS;
    } named;
    f32 values[9];
} HwVolumeInfo;

typedef struct HwVolumeVoice {
    u8 pad_00[0x24];
    u32 changed[5];
    u32 pitch[5];
    u16 volL;
    u16 volR;
    u16 volS;
    u16 volLa;
    u16 volRa;
    u16 volSa;
    u16 volLb;
    u16 volRb;
    u16 volSb;
    u8 pad_5E[0x72];
    u16 itdShiftL;
    u16 itdShiftR;
    u8 pad_D4[0x10];
    struct {
        u8 pitch;
        u8 vol;
        u8 volA;
        u8 volB;
    } lastUpdate;
    u32 virtualSampleID;
    u8 state;
    u8 postBreak;
    u8 startupBreak;
    u8 studio;
    u32 flags;
} HwVolumeVoice;

typedef struct PeopleStudioState {
    u8 pad_00[0xAC];
    u32 field_AC;
    u32 field_B0;
    u32 field_B4;
    u32 field_B8;
} PeopleStudioState;

extern u32 lbl_8047B024;
extern u8 lbl_8047B050;
extern u16 lbl_80478BF8;
extern u16 lbl_80478C00;
extern volatile const u16 lbl_80273448[];
extern HwVolumeStudio lbl_80447E60[];
extern volatile const f32 lbl_8047D4D8;
extern f32 lbl_8047D4DC;

extern void salActivateVoice(u8* ptr, u8 studio);
extern void salDeactivateVoice(void* ptr);
extern void salActivateStudio(void);
extern void fn_8015AAA0(u32 studio);
extern u32 fn_8015D54C(void* studio, void* arg);
extern u32 fn_8015D5F4(void* studio, void* arg);
extern void salCalcVolume(u32 table, f32* out, f32 volume, u32 pan, u32 span,
                          f32 auxA, f32 auxB, u32 itd, u32 dpl2);
extern u32 aramGetStreamBufferAddress(u32 index, u32* out);
extern void aramUploadData(void* dst, void* src, u32 size, u32 type, u32 arg7,
                           u32 arg8);
extern void DCStoreRange(void* addr, u32 size);
extern void fn_80163CA8(void);
extern void aramFreeStreamBuffer(void);

extern u8 lbl_8036944C[];
extern u8 lbl_8047AF18;
extern u32 lbl_8047B028;
extern u8 lbl_8047B05D;
extern u8 lbl_8047B05E;
extern u8 lbl_8047B05F;

extern void fn_8014DF20(void);
extern void fn_8014E7CC(void);
extern void fn_801496A0(u32 ticks);
extern void fn_8015F620(void);
extern void fn_801603C0(u8 ctrl, u8 channel, u8 set, u8 value);
extern u32 fn_80163810(void* ptr, u32 size);
extern u32 fn_80164148(u32 flags);
extern u32 fn_80164204(void);
extern u32 fn_80164398(void);
extern u32 fn_801643B8(void);
extern u32 adsrConvertTimeCents(s32 time);
extern u8* salAiGetDest(void);
extern void salCtrlDsp(u32 destination);
extern void salHandleAuxProcessing(void);
extern u32 salInitAi(void (*callback)(void), u32 flags, u32 frequency);
extern u32 salExitAi(void);
extern u32 salInitDspCtrl(u8 voices, u8 studios, u32 flags);
extern u32 salExitDspCtrl(void);
extern void fn_801640C4(void);
extern void fn_80164324(void);
extern void hwInitIrq(void);
extern void hwEnableIrq(void);
extern void hwDisableIrq(void);
extern void synthHandle(u32 ticks);
extern void vsSampleUpdates(void);

void fn_8016245C(u8 offset);
void fn_801629A4(u32 index, u8 value);
void fn_801629D0(u32 index, u8 value);
void hwSetITDMode(u32 index, u8 flag);

typedef struct HwIrqVoice {
    u8 pad_00[0x24];
    u32 changed[5];
} HwIrqVoice;

static inline HwIrqVoice* sndGetIrqVoice(u8 voice)
{
    return (HwIrqVoice*)((u8*)lbl_8047B024 + voice * 0xF4);
}

static void snd_handle_irq(void)
{
    u8 frame;
    u8 voice;
    u8 i;

    if (lbl_8047AF18 == 0) {
        return;
    }

    fn_8014E7CC();
    fn_80164398();
    salCtrlDsp((u32)salAiGetDest());
    fn_801643B8();
    fn_80164398();
    salHandleAuxProcessing();
    fn_801643B8();
    fn_80164398();

    lbl_8047B05F ^= 1;
    lbl_8047B05E = (lbl_8047B05E + 1) % 3;
    for (voice = 0; voice < lbl_8047B05D; voice++) {
        for (i = 0; i < 5; i++) {
            sndGetIrqVoice(voice)->changed[i] = 0;
        }
    }
    fn_801643B8();

    for (frame = 0; frame < 5; frame++) {
        fn_80164398();
        fn_8016245C(frame);
        fn_801496A0(0x100);
        synthHandle(0x100);
        fn_801643B8();
    }

    fn_80164398();
    fn_8016245C(0);
    fn_8015F620();
    fn_801643B8();
    fn_80164398();
    fn_8014DF20();
    fn_801643B8();
    fn_80164398();
    vsSampleUpdates();
    fn_801643B8();
}

u32 hwInit(u32 frequency, u16 voices, u32 studios, u32 flags)
{
    hwInitIrq();
    lbl_8047B05F = 0;
    lbl_8047B05E = 0;
    lbl_8047B028 = 0;
    if (salInitAi(snd_handle_irq, flags, frequency) != 0
        && salInitDspCtrl(voices, studios, (u8)(flags & 1)) != 0
        && fn_80164148(flags) != 0) {
        hwEnableIrq();
        fn_801640C4();
        return 0;
    }
    return -1;
}

void hwExit(void)
{
    hwDisableIrq();
    fn_80164204();
    salExitDspCtrl();
    salExitAi();
    hwEnableIrq();
    fn_80164324();
}

void fn_8016245C(u8 offset)
{
    lbl_8047B050 = offset;
}

u8 fn_80162464(void)
{
    return lbl_8047B050;
}

u32 fn_8016246C(u32 index)
{
    PeopleFieldMoveSlot* voices = (PeopleFieldMoveSlot*)lbl_8047B024;
    return voices[index].active != 0;
}

void fn_8016248C(u32 callback)
{
    lbl_8047B028 = callback;
}

void fn_80162494(u32 index, u32 priority)
{
    PeopleFieldMoveSlot* voices = (PeopleFieldMoveSlot*)lbl_8047B024;
    voices[index].field_1C = priority;
}

void hwInitSamplePlayback(u32 index, u16 sampleId, void* sampleInfo,
                          u32 resetAdsr, u32 priority, u32 callbackValue,
                          u32 setSrc, u8 itdMode)
{
    typedef struct HardwareSampleInfo {
        u32 words[8];
    } HardwareSampleInfo;
    typedef struct {
        u8 pad_00[0x18];
        u32 field_18;
        u32 field_1C;
        u8 pad_20[0x4];
        u32 flags_24[0x13];
        u16 field_70;
        u8 pad_72[0x2];
        HardwareSampleInfo sample;
        u32 field_94;
        u32 field_98;
        u8 field_9C;
        u8 pad_9D[0x3];
        u8 field_A0;
        u8 pad_A1[0x3];
        u8 field_A4;
        u8 pad_A5[0x13];
        u32 field_B8;
        u32 field_BC;
        u16 field_C0;
        u8 pad_C2[0x2];
        u32 field_C4;
        u8 pad_C8[0x1C];
        u8 bytes_E4[4];
        u8 pad_E8[0x8];
        u32 field_F0;
    } PeopleFieldState;
    u8 i;
    u32 flags = 0;

#define HW_PLAYBACK_VOICES (*(PeopleFieldState**)&lbl_8047B024)
    for (i = 0; i <= lbl_8047B050; i++) {
        flags |= HW_PLAYBACK_VOICES[index].flags_24[i] & 0x20;
        HW_PLAYBACK_VOICES[index].flags_24[i] = 0;
    }

    HW_PLAYBACK_VOICES[index].flags_24[0] = flags;
    HW_PLAYBACK_VOICES[index].field_1C = priority;
    HW_PLAYBACK_VOICES[index].field_18 = callbackValue;
    HW_PLAYBACK_VOICES[index].field_F0 = 0;
    HW_PLAYBACK_VOICES[index].field_70 = sampleId;
    HW_PLAYBACK_VOICES[index].sample = *(HardwareSampleInfo*)sampleInfo;

    if (resetAdsr != 0) {
        HW_PLAYBACK_VOICES[index].field_A4 = 0;
        HW_PLAYBACK_VOICES[index].field_B8 = 0;
        HW_PLAYBACK_VOICES[index].field_BC = 0;
        HW_PLAYBACK_VOICES[index].field_C0 = 0x7FFF;
        HW_PLAYBACK_VOICES[index].field_C4 = 0;
    }

    HW_PLAYBACK_VOICES[index].bytes_E4[0] = 0xFF;
    HW_PLAYBACK_VOICES[index].bytes_E4[1] = 0xFF;
    HW_PLAYBACK_VOICES[index].bytes_E4[2] = 0xFF;
    HW_PLAYBACK_VOICES[index].bytes_E4[3] = 0xFF;

    if (setSrc != 0) {
        fn_801629A4(index, 0);
        fn_801629D0(index, 1);
    }

    hwSetITDMode(index, itdMode);
#undef HW_PLAYBACK_VOICES
}

void hwBreak(u32 index)
{
    u32 offset = index * 0xF4;
    PeopleFieldMoveSlot* entry;
    u8* ptr;

    entry = (PeopleFieldMoveSlot*)((u8*)lbl_8047B024 + offset);
    if (entry->active == 1 && lbl_8047B050 == 0) {
        entry->field_EE = 1;
    }
    ptr = (u8*)lbl_8047B024 + offset;
    ptr += (u32)lbl_8047B050 * 4;
    *(u32*)(ptr + 0x24) |= 0x20;
}

void hwSetADSR(u32 index, void* data, u8 mode)
{
    typedef struct {
        u8 pad_00[0x18];
        u32 field_18;
        u32 field_1C;
        u8 pad_20[0x4];
        u32 flags_24[0x13];
        u16 field_70;
        u8 pad_72[0x2];
        u32 words_74[0x8];
        u32 field_94;
        u32 field_98;
        u8 field_9C;
        u8 pad_9D[0x3];
        u8 field_A0;
        u8 pad_A1[0x3];
        u8 field_A4;
        u8 pad_A5[0x13];
        u32 field_B8;
        u32 field_BC;
        u16 field_C0;
        u8 pad_C2[0x2];
        u32 field_C4;
        u8 pad_C8[0x2];
        u8 field_CA;
        u8 pad_CB[0x19];
        u8 bytes_E4[4];
        u8 pad_E8[0x8];
        u32 field_F0;
    } PeopleFieldState;
    typedef struct {
        u16 field_00;
        u16 field_02;
        u16 field_04;
        u16 field_06;
    } PeopleFieldMode0Args;
    typedef struct {
        u32 field_00;
        u32 field_04;
        u16 field_08;
        u16 field_0A;
    } PeopleFieldMode12Args;

#define HW_ADSR_VOICES (*(PeopleFieldState**)&lbl_8047B024)
    switch (mode) {
    case 0: {
        PeopleFieldMode0Args* args = data;
        u32 value;
        HW_ADSR_VOICES[index].field_A4 = 0;
        HW_ADSR_VOICES[index].field_B8 = args->field_00;
        HW_ADSR_VOICES[index].field_BC = args->field_02;
        value = args->field_04 << 3;
        if (value > 0x7FFF) {
            value = 0x7FFF;
        }
        HW_ADSR_VOICES[index].field_C0 = (u16)value;
        HW_ADSR_VOICES[index].field_C4 = args->field_06;
        break;
    }
    case 1:
    case 2: {
        PeopleFieldMode12Args* args = data;
        HW_ADSR_VOICES[index].field_A4 = 1;
        HW_ADSR_VOICES[index].field_CA = 0;
        if (mode == 1) {
            HW_ADSR_VOICES[index].field_B8 =
                (u16)adsrConvertTimeCents(args->field_00);
            HW_ADSR_VOICES[index].field_BC =
                (u16)adsrConvertTimeCents(args->field_04);
            {
                s32 scale = args->field_08 >> 2;
                if ((u32)scale > 0x3FF) {
                    scale = 0x3FF;
                }
                HW_ADSR_VOICES[index].field_C0 =
                    (u16)(0xC1 - lbl_8036944C[scale]);
            }
        } else {
            HW_ADSR_VOICES[index].field_B8 = (u16)args->field_00;
            HW_ADSR_VOICES[index].field_BC = (u16)args->field_04;
            HW_ADSR_VOICES[index].field_C0 = args->field_08;
        }
        HW_ADSR_VOICES[index].field_C4 = args->field_0A;
        break;
    }
    }

    HW_ADSR_VOICES[index].flags_24[0] |= 0x10;
#undef HW_ADSR_VOICES
}

#pragma push
#pragma optimization_level 2
void fn_80162858(u32 index, u32 val1, u32 val2) {
    u32 offset = index * 0xF4;
    {
        PeopleFieldMoveSlot* entry1 =
            (PeopleFieldMoveSlot*)((u8*)lbl_8047B024 + offset);
        entry1->field_94 = val1;
    }
    {
        PeopleFieldMoveSlot* entry2 =
            (PeopleFieldMoveSlot*)((u8*)lbl_8047B024 + offset);
        entry2->field_98 = val2;
    }
}
#pragma pop

#pragma push
#pragma optimization_level 2
u8 fn_80162878(u32 index) {
    PeopleFieldMoveSlot* entries =
        (*(PeopleFieldMoveSlot* volatile*)&lbl_8047B024);
    return entries[index].field_9C;
}
#pragma pop

#pragma push
#pragma optimization_level 2
u8 fn_8016288C(u32 index) {
    PeopleFieldMoveSlot* entries =
        (*(PeopleFieldMoveSlot* volatile*)&lbl_8047B024);
    return entries[index].field_90;
}
#pragma pop

#pragma push
#pragma optimization_level 2
u16 fn_801628A0(u32 index) {
    PeopleFieldMoveSlot* entries =
        (*(PeopleFieldMoveSlot* volatile*)&lbl_8047B024);
    return entries[index].field_70;
}
#pragma pop

#pragma push
#pragma optimization_level 2
void fn_801628B4(u32 index, u8 val) {
    PeopleFieldMoveSlot* entries =
        (*(PeopleFieldMoveSlot* volatile*)&lbl_8047B024);
    entries[index].field_A0 = val;
}
#pragma pop

#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
void hwStart(u32 index, u8 studio) {
#define HW_VOICES (*(PeopleFieldMoveSlot* volatile*)&lbl_8047B024)
    HW_VOICES[index].field_D4 = lbl_8047B050;
    salActivateVoice((u8*)&HW_VOICES[index], studio);
#undef HW_VOICES
}
#pragma pop

void hwKeyOff(u32 index) {
    PeopleFieldMoveSlot* entries =
        (*(PeopleFieldMoveSlot* volatile*)&lbl_8047B024);
    entries[index].flags_24[lbl_8047B050] |= 0x40;
}

#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
void hwSetPitch(u32 index, u16 value) {
    typedef struct {
        u8 pad_00[0x24];
        u32 words_24[0x30];
        u8 activeWordIndex;
        u8 pad_E5[0x0F];
    } PeopleFieldState;
    PeopleFieldState* entries =
        (*(PeopleFieldState* volatile*)&lbl_8047B024);
    PeopleFieldState* entry = &entries[index];
    u32 scaledValue;

    if ((u16)value >= 0x4000) {
        value = 0x3FFF;
    }
    if (entry->activeWordIndex != 0xFF) {
        scaledValue = (u16)value << 4;
        if (entry->words_24[5 + entry->activeWordIndex] == scaledValue) {
            return;
        }
    }
    scaledValue = (u16)value << 4;
    entry->words_24[5 + lbl_8047B050] = scaledValue;
    entry->words_24[lbl_8047B050] |= 8;
    entry->activeWordIndex = lbl_8047B050;
}
#pragma pop

#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
void fn_801629A4(u32 index, u8 value) {
    PeopleFieldMoveSlot* entries =
        (*(PeopleFieldMoveSlot* volatile*)&lbl_8047B024);
    entries[index].field_CC = (&lbl_80478BF8)[(u8)value];
    entries[index].flags_24[0] |= 0x100;
}
#pragma pop

#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
void fn_801629D0(u32 index, u8 value) {
    PeopleFieldMoveSlot* entries =
        (*(PeopleFieldMoveSlot* volatile*)&lbl_8047B024);
    entries[index].field_CE = (&lbl_80478C00)[(u8)value];
    entries[index].flags_24[0] |= 0x80;
}
#pragma pop

#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
void hwSetITDMode(u32 index, u8 flag) {
#define HW_VOICES (*(PeopleFieldMoveSlot* volatile*)&lbl_8047B024)
    if (flag == 0) {
        HW_VOICES[index].field_F0 |= 0x80000000;
        HW_VOICES[index].field_D0 = 0x10;
        HW_VOICES[index].field_D2 = 0x10;
    } else {
        HW_VOICES[index].field_F0 &= ~0x80000000u;
    }
#undef HW_VOICES
}
#pragma pop

static inline void hwSetupITD(HwVolumeVoice* voice, u8 pan) {
    voice->itdShiftL = lbl_80273448[pan];
    voice->itdShiftR = 32 - lbl_80273448[pan];
    voice->changed[0] |= 0x200;
}

#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
void hwSetVolume(u32 voice, u32 table, f32 volume, u32 pan, u32 span,
                 f32 auxA, f32 auxB) {
    HwVolumeInfo volumeInfo;
    u16 left;
    u16 right;
    u16 surround;
    HwVolumeVoice* dspVoice = (HwVolumeVoice*)lbl_8047B024;
    HwVolumeVoice* dspVoicePtr = &dspVoice[voice];

    {
        f32 one = lbl_8047D4D8;
        if (volume >= one) {
            volume = one;
        }
    }
    {
        f32 one = lbl_8047D4D8;
        if (auxA >= one) {
            auxA = one;
        }
    }
    {
        f32 one = lbl_8047D4D8;
        if (auxB >= one) {
            auxB = one;
        }
    }

    {
        u32 hasITD = (dspVoicePtr->flags & 0x80000000) != 0;
        u32 dpl2 = lbl_80447E60[dspVoicePtr->studio].named.type == 1;

        salCalcVolume(table, volumeInfo.values, volume, pan, span, auxA, auxB,
                      hasITD, dpl2);
    }

    left = lbl_8047D4DC * volumeInfo.named.volL;
    right = lbl_8047D4DC * volumeInfo.named.volR;
    surround = lbl_8047D4DC * volumeInfo.named.volS;
    if (dspVoicePtr->lastUpdate.vol == 0xFF || dspVoicePtr->volL != left ||
        dspVoicePtr->volR != right || dspVoicePtr->volS != surround) {
        dspVoicePtr->volL = left;
        dspVoicePtr->volR = right;
        dspVoicePtr->volS = surround;
        dspVoicePtr->changed[0] |= 1;
        dspVoicePtr->lastUpdate.vol = 0;
    }

    left = lbl_8047D4DC * volumeInfo.named.volAuxAL;
    right = lbl_8047D4DC * volumeInfo.named.volAuxAR;
    surround = lbl_8047D4DC * volumeInfo.named.volAuxAS;
    if (dspVoicePtr->lastUpdate.volA == 0xFF || dspVoicePtr->volLa != left ||
        dspVoicePtr->volRa != right || dspVoicePtr->volSa != surround) {
        dspVoicePtr->volLa = left;
        dspVoicePtr->volRa = right;
        dspVoicePtr->volSa = surround;
        dspVoicePtr->changed[0] |= 2;
        dspVoicePtr->lastUpdate.volA = 0;
    }

    left = lbl_8047D4DC * volumeInfo.named.volAuxBL;
    right = lbl_8047D4DC * volumeInfo.named.volAuxBR;
    surround = lbl_8047D4DC * volumeInfo.named.volAuxBS;
    if (dspVoicePtr->lastUpdate.volB == 0xFF || dspVoicePtr->volLb != left ||
        dspVoicePtr->volRb != right || dspVoicePtr->volSb != surround) {
        dspVoicePtr->volLb = left;
        dspVoicePtr->volRb = right;
        dspVoicePtr->volSb = surround;
        dspVoicePtr->changed[0] |= 4;
        dspVoicePtr->lastUpdate.volB = 0;
    }

    if (dspVoicePtr->flags & 0x80000000) {
        hwSetupITD(dspVoicePtr, pan >> 16);
    }
}
#pragma pop

#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
void fn_80162D18(u32 index) {
    PeopleFieldMoveSlot* entries =
        (*(PeopleFieldMoveSlot* volatile*)&lbl_8047B024);
    salDeactivateVoice((u8*)&entries[index]);
}
#pragma pop

#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
void hwSetAUXProcessingCallbacks(u8 index, u32 a, u32 b, u32 c, u32 d) {
    PeopleStudioState* entries = (PeopleStudioState*)lbl_80447E60;
    entries[(u8)index].field_AC = a;
    entries[(u8)index].field_B4 = b;
    entries[(u8)index].field_B0 = c;
    entries[(u8)index].field_B8 = d;
}
#pragma pop

#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
void fn_80162D6C(void) { salActivateStudio(); }
#pragma pop

#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
void fn_80162D8C(u32 studio) { fn_8015AAA0(studio); }
#pragma pop

#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
void fn_80162DAC(u8 index, u32 arg1) {
    PeopleStudioState* entries = (PeopleStudioState*)lbl_80447E60;
    fn_8015D54C((u8*)&entries[(u8)index], (void*)arg1);
}
#pragma pop

#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
void fn_80162DE0(u8 index, u32 arg1) {
    PeopleStudioState* entries = (PeopleStudioState*)lbl_80447E60;
    fn_8015D5F4((u8*)&entries[(u8)index], (void*)arg1);
}
#pragma pop

#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
u32 fn_80162E14(u32 idx) {
    typedef struct PeopleFieldEntry {
        u8 pad_00[0x20];
        u32 dim_20;
        u8 pad_24[0x54];
        u32 dim_78;
        u8 pad_7C[0x14];
        u8 kind_90;
        u8 pad_91[0x5B];
        u8 flag_EC;
        u8 pad_ED[0x07];
    } PeopleFieldEntry;
    PeopleFieldEntry* entries =
        (*(PeopleFieldEntry* volatile*)&lbl_8047B024);

    if (entries[idx].flag_EC != 2) {
        return 0;
    }
    switch (entries[idx].kind_90) {
    case 0:
    case 1:
    case 4:
    case 5: {
        PeopleFieldEntry* entry =
            (PeopleFieldEntry*)((u32)entries + idx * 0xF4);
        u32 small = entry->dim_78;
        u32 big = entry->dim_20;
        u32 value = ((big - (small << 1)) >> 4) * 0xE;
        u32 lo = big & 0xF;
        if (lo >= 2) {
            value = lo + value;
            value -= 2;
        }
        return value;
    }
    case 3:
        return entries[idx].dim_20 - entries[idx].dim_78;
    case 2:
        return entries[idx].dim_20 - (entries[idx].dim_78 >> 1);
    default:
        return idx;
    }
}
#pragma pop

#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
void hwFlushStream(u8* dstBase, u32 srcOffset, u32 size, u32 streamIndex,
                   u32 arg7, u32 arg8) {
    u32 unusedOut;
    u8* srcBase =
        (u8*)aramGetStreamBufferAddress(streamIndex, &unusedOut);
    u8* dst;

    size += srcOffset & 0x1F;
    srcOffset &= ~0x1F;
    dst = dstBase + srcOffset;
    size = (size + 0x1F) & ~0x1F;
    DCStoreRange(dst, size);
    aramUploadData(dst, srcBase + srcOffset, size, 1, arg7, arg8);
}
#pragma pop

#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
void fn_80162F48(void) { fn_80163CA8(); }
#pragma pop

#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
void fn_80162F68(void) { aramFreeStreamBuffer(); }
#pragma pop

#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
void fn_80162F88(void* index) {
    aramGetStreamBufferAddress((u32)index, 0);
}
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80162FAC(void) {}
#pragma pop
