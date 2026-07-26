/**
 * @file gs_range_8018FE30.c
 * @brief gs-engine, 0x8018FE30 - 0x80190E34.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) - mixed-block split pass, 2026-07-01.
 * All functions asm-only until matched.
 */
#include "dolphin/types.h"

typedef struct FlagStateEntry {
    u32 unused;
    u32* buffer;
} FlagStateEntry;

typedef struct FlagDefinition {
    u8 typeAndWidth;
    u8 _pad01[3];
    u16 bitPosition;
    u8 _pad06[2];
} FlagDefinition;

extern const char lbl_802741F8[];
extern void GSlogWrite(const char* fmt, ...);

void GSflagInitBitPos(FlagDefinition* definitions, u32 count, u32 capacity1,
                      u32 capacity2, u32 capacity3)
{
    u16 next1 = 0;
    u16 next2 = 0;
    u16 next3 = 0;
    u32 i;

    for (i = 0; i < count; i++, definitions++) {
        u32 width = definitions->typeAndWidth & 0x3F;
        u32 type;

        if (width > 32) {
            GSlogWrite(lbl_802741F8 + 0xF8, i, width, 32);
            definitions->typeAndWidth =
                (definitions->typeAndWidth & 0xC0) | 32;
        } else if (width == 0) {
            GSlogWrite(lbl_802741F8 + 0x144, i, width, 32);
            definitions->typeAndWidth =
                (definitions->typeAndWidth & 0xC0) | 1;
        }

        width = definitions->typeAndWidth & 0x3F;
        type = (definitions->typeAndWidth >> 6) & 3;
        switch (type) {
        case 1:
            definitions->bitPosition = next1;
            next1 += width;
            break;
        case 2:
            definitions->bitPosition = next2;
            next2 += width;
            break;
        case 3:
            definitions->bitPosition = next3;
            next3 += width;
            break;
        }
    }

    if (capacity1 <= ((next1 + 31) >> 5)) {
        GSlogWrite(lbl_802741F8 + 0x18C, (next1 + 31) >> 5, capacity1);
    }
    if (capacity2 <= ((next2 + 31) >> 5)) {
        GSlogWrite(lbl_802741F8 + 0x1D8, (next2 + 31) >> 5, capacity2);
    }
    if (capacity3 <= ((next3 + 31) >> 5)) {
        GSlogWrite(lbl_802741F8 + 0x224, (next3 + 31) >> 5, capacity3);
    }
}

u8 fn_801902E0(s32 flagId)
{
    extern u8* lbl_80478F9C;
    extern FlagStateEntry* lbl_80478EEC;
    extern u32 lbl_8036C568[];
    extern const char lbl_80274284[];
    extern void GSlogWrite(const char* fmt, ...);
    u32 typeAndWidth;
    u32* buffer;
    u8* definitions;
    u32 definitionOffset;
    FlagStateEntry* states;
    u32 bitWidth;
    u32 bitOffset;
    u32 wordIndex;
    u32 bitPosition;
    u32 lowWord;
    u32 highWord;
    u32 shiftAmount;
    u32 mask;
    u32 value;
    u8 result;

    definitionOffset = flagId << 3;
    definitions = lbl_80478F9C;
    states = lbl_80478EEC;
    typeAndWidth = definitions[definitionOffset];
    buffer = states[(typeAndWidth & 0xC0) >> 6].buffer;

    if (buffer == 0) {
        GSlogWrite(lbl_80274284);
        value = 0;
    } else {
        definitions += definitionOffset;
        bitWidth = typeAndWidth & 0x3F;
        bitOffset = *(u16*)(definitions + 4);
        wordIndex = bitOffset >> 5;
        bitPosition = bitOffset & 0x1F;

        if (bitWidth > 1) {
            lowWord = buffer[wordIndex];
            highWord = buffer[wordIndex + 1];
            shiftAmount = 32 - bitPosition;
            lowWord >>= bitPosition;
            highWord <<= shiftAmount;
            mask = lbl_8036C568[bitWidth];
            value = (highWord | lowWord) & mask;
        } else {
            value = buffer[wordIndex];
            value = (value >> bitPosition) & 1;
        }
    }

    if (value == 0) {
        result = 0;
    } else {
        result = 1;
    }
    return result;
}

u32 fn_801906A0(s32 flagId)
{
    extern u8* lbl_80478F9C;
    extern FlagStateEntry* lbl_80478EEC;
    extern u32 lbl_8036C568[];
    extern const char lbl_80274284[];
    extern void GSlogWrite(const char* fmt, ...);
    u32 typeAndWidth;
    u32* buffer;
    u8* definitions;
    u32 definitionOffset;
    FlagStateEntry* states;
    u32 bitWidth;
    u32 bitOffset;
    u32 wordIndex;
    u32 bitPosition;
    u32 lowWord;
    u32 highWord;
    u32 shiftAmount;
    u32 mask;
    u32 value;

    definitionOffset = flagId << 3;
    definitions = lbl_80478F9C;
    states = lbl_80478EEC;
    typeAndWidth = definitions[definitionOffset];
    buffer = states[(typeAndWidth & 0xC0) >> 6].buffer;

    if (buffer == 0) {
        GSlogWrite(lbl_80274284);
        value = 0;
    } else {
        definitions += definitionOffset;
        bitWidth = typeAndWidth & 0x3F;
        bitOffset = *(u16*)(definitions + 4);
        wordIndex = bitOffset >> 5;
        bitPosition = bitOffset & 0x1F;

        if (bitWidth > 1) {
            lowWord = buffer[wordIndex];
            highWord = buffer[wordIndex + 1];
            shiftAmount = 32 - bitPosition;
            lowWord >>= bitPosition;
            highWord <<= shiftAmount;
            mask = lbl_8036C568[bitWidth];
            value = (highWord | lowWord) & mask;
        } else {
            value = buffer[wordIndex];
            value = (value >> bitPosition) & 1;
        }
    }

    return value;
}

void GSflagClear(s32 level)
{
    extern FlagStateEntry* lbl_80478EEC;
    extern const char lbl_802742B8[];
    extern void GSlogWrite(const char* fmt, ...);
    FlagStateEntry* states;
    u32* buffer;
    u32 wordCount;
    u32 i;

    states = lbl_80478EEC;
    buffer = states[level].buffer;
    if (buffer == 0) {
        GSlogWrite(lbl_802742B8);
    } else {
        wordCount = states[level].unused;
        for (i = 0; i < wordCount; i++) {
            buffer[i] = 0;
        }
    }
}
