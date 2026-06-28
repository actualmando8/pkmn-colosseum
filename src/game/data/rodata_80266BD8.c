#include "dolphin/types.h"

#pragma section ".rodata"
#define RODATA __declspec(section ".rodata")

typedef struct TitleMessagePairEntry {
    u16 id;
    u16 pad;
    u32 messageId;
} TitleMessagePairEntry;

typedef struct TitleMessageChoiceEntry {
    u16 id;
    u16 pad;
    u32 messageId;
    u32 followupMessageId;
} TitleMessageChoiceEntry;

/*
 * Mixed game UI .rodata tables referenced from gs_event_exec.c, gs_pcbox.c,
 * and gs_title.c. The pcbox/title users copy these as 32-bit words.
 */
RODATA const u32 lbl_80266BD8[6] = {
    0x66, 0, 0,
    0x67, 0, 0,
};

RODATA const u32 lbl_80266BF0[4] = {
    0x278, 0x277, 0x276, 0x275,
};

RODATA const u32 lbl_80266C00[4] = {
    0x27F, 0x27D, 0x27B, 0x279,
};

RODATA const u32 lbl_80266C10[4] = {
    0x280, 0x27E, 0x27C, 0x27A,
};

RODATA const u32 lbl_80266C20[4] = {
    0x213A44F2u, 0x11272BF2u, 0xFFFFFF38u, 0,
};

RODATA const TitleMessageChoiceEntry lbl_80266C30[3] = {
    { 0x21F, 0, 0x3AF6, 0x3AF8 },
    { 0x220, 0, 0x4275, 0x3AF9 },
    { 0x221, 0, 0x4276, 0x3AFA },
};

RODATA const TitleMessagePairEntry lbl_80266C54[5] = {
    { 0x21A, 0, 0x3B34 },
    { 0x21D, 0, 0x3B36 },
    { 0x21C, 0, 0x3B38 },
    { 0x21B, 0, 0x3B30 },
    { 0x223, 0, 0x44C5 },
};
