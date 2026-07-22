/**
 * @file gamedata.c
 * @brief Decompiled functions.
 *
 * Address range: 0x8013528C - 0x80135A30
 *
 * Split out of the former game/effect/effect_util.c CodeCandidate
 * bucket (0x8013151C - 0x80137114); see effect_util_types.h for
 * shared cross-TU declarations.
 */

#include "dolphin/types.h"
/* This TU recovers the return type hidden by the shared legacy declaration. */
#define gamedataGetStatus gamedataGetStatus_legacy_decl
#include "game/effect/effect_util_types.h"
#undef gamedataGetStatus

typedef struct GameData GameData;
typedef struct GameDataAttest GameDataAttest;

static inline void gamedataSetStatusInline(GameData* ptr, u32 kind, u32 value)
{
    GameDataAttest* attest;

    if ((u16)kind == 0 || (u16)kind >= 7) {
        return;
    }
    if (ptr == NULL) {
        ptr = (GameData*)savedataGetStatus(0, 0);
        if (ptr == NULL) {
            return;
        }
        ptr = (GameData*)savedataGetStatus((u32)ptr, 1);
        if (ptr == NULL) {
            return;
        }
    }
    attest = (GameDataAttest*)gamedataBiosGetGamedataAtttestPtr(ptr);
    if (attest == NULL) {
        return;
    }
    switch ((u16)kind) {
    case 1:
        gamedataBiosSetGamedataAtttestPtr((u32*)ptr, (u32*)value);
        break;
    case 2:
        gamedataAttestBiosSetVerId(attest, (u8)value);
        break;
    case 3:
        gamedataAttestBiosSetGenId(attest, (u8)value);
        break;
    case 4:
        gamedataAttestBiosSetAreaId(attest, (u8)value);
        break;
    case 5:
        gamedataAttestBiosSetLangareaId(attest, (u8)value);
        break;
    default:
        break;
    }
}

static inline u32 gamedataGetStatusInline(GameData* ptr, u32 kind)
{
    GameDataAttest* attest;

    if ((u16)kind == 0 || (u16)kind >= 7) {
        return 0;
    }
    if (ptr == NULL) {
        ptr = (GameData*)savedataGetStatus(0, 0);
        if (ptr == NULL) {
            return 0;
        }
        ptr = (GameData*)savedataGetStatus((u32)ptr, 1);
        if (ptr == NULL) {
            return 0;
        }
    }
    attest = (GameDataAttest*)gamedataBiosGetGamedataAtttestPtr(ptr);
    if (attest == NULL) {
        return 0;
    }
    switch ((u16)kind) {
    case 1:
        return (u32)attest;
    case 2:
        return gamedataAttestBiosGetVerId(attest);
    case 3:
        return gamedataAttestBiosGetGenId(attest);
    case 4:
        return gamedataAttestBiosGetAreaId(attest);
    case 5:
        return gamedataAttestBiosGetLangareaId(attest);
    default:
        return 0;
    }
}

/* 0x8013528C | 0xAC */
void gamedataCreate(void* ptr, u8 r4, u8 r5, u8 r6, u8 r7) {
    if (ptr == NULL) {
        return;
    }
    gamedataInit(ptr);
    ptr = (void*)gamedataGetStatusInline((GameData*)ptr, 1);
    if (ptr == NULL) {
        return;
    }
    gamedataAttestCreate(ptr, r4, r5, r6, r7);
}

/* 0x80135338 | 0x88 */
void gamedataInit(void* ptr) {
    void* attest = (void*)gamedataGetStatusInline((GameData*)ptr, 1);

    if (attest == NULL) {
        return;
    }
    gamedataAttestInit(attest);
    gamedatasaveInit(ptr);
}


/* 0x801353C0 | 0x170 */
void gamedataAttestCreate(void* ptr, u8 r4, u8 r5, u8 r6, u8 r7) {
    if (ptr == NULL) {
        return;
    }
    if (r4 == 0) {
        return;
    }
    if (r5 == 0) {
        return;
    }
    if (r6 == 0) {
        return;
    }
    if (r7 == 0) {
        return;
    }
    gamedataAttestInit(ptr);
    gamedataSetStatusInline((GameData*)ptr, 2, r4);
    gamedataSetStatusInline((GameData*)ptr, 3, r5);
    gamedataSetStatusInline((GameData*)ptr, 4, r6);
    gamedataSetStatusInline((GameData*)ptr, 5, r7);
}


/* 0x80135530 | 0x1D8 */
u32 gamedataAttestCheckValid(GameData* ptr) {
    if (ptr == NULL) {
        return 0;
    }
    if ((s32)gamedataGetStatusInline(ptr, 2) == 0) {
        return 0;
    }
    if ((s32)gamedataGetStatusInline(ptr, 3) == 0) {
        return 0;
    }
    if ((s32)gamedataGetStatusInline(ptr, 4) == 0) {
        return 0;
    }
    return gamedataGetStatusInline(ptr, 5) != 0;
}


/* 0x80135708 | 0x134 */
void gamedataAttestInit(void* ptr) {
    if (ptr == NULL) {
        return;
    }
    gamedataSetStatusInline((GameData*)ptr, 2, 0);
    gamedataSetStatusInline((GameData*)ptr, 3, 0);
    gamedataSetStatusInline((GameData*)ptr, 4, 0);
    gamedataSetStatusInline((GameData*)ptr, 5, 0);
}


/* 0x8013583C | 0xFC */
void gamedataSetStatus(ptr, effect_type, value)
GameData* ptr;
u32 effect_type;
u32 value;
{
    GameDataAttest* base;

    if ((u16)effect_type == 0 || (u16)effect_type >= 7) {
        return;
    }
    if (ptr == NULL) {
        ptr = (GameData*)savedataGetStatus(0, 0);
        if (ptr == NULL) {
            return;
        }
        ptr = (GameData*)savedataGetStatus((u32)ptr, 1);
        if (ptr == NULL) {
            return;
        }
    }
    base = (GameDataAttest*)gamedataBiosGetGamedataAtttestPtr(ptr);
    if (base == NULL) {
        return;
    }
    switch ((u16)effect_type) {
    case 1:
        gamedataBiosSetGamedataAtttestPtr((u32*)ptr, (u32*)value);
        break;
    case 2:
        gamedataAttestBiosSetVerId(base, (u8)value);
        break;
    case 3:
        gamedataAttestBiosSetGenId(base, (u8)value);
        break;
    case 4:
        gamedataAttestBiosSetAreaId(base, (u8)value);
        break;
    case 5:
        gamedataAttestBiosSetLangareaId(base, (u8)value);
        break;
    default:
        break;
    }
}


/* 0x80135938 | 0xF8 */
u32 gamedataGetStatus(ptr, effect_type)
GameData* ptr;
u32 effect_type;
{
    GameDataAttest* base;

    if ((u16)effect_type == 0 || (u16)effect_type >= 7) {
        return 0;
    }
    if (ptr == NULL) {
        ptr = (GameData*)savedataGetStatus(0, 0);
        if (ptr == NULL) {
            return 0;
        }
        ptr = (GameData*)savedataGetStatus((u32)ptr, 1);
        if (ptr == NULL) {
            return 0;
        }
    }
    base = (GameDataAttest*)gamedataBiosGetGamedataAtttestPtr(ptr);
    if (base == NULL) {
        return 0;
    }
    switch ((u16)effect_type) {
    case 1:
        return (u32)base;
    case 2:
        return gamedataAttestBiosGetVerId(base);
    case 3:
        return gamedataAttestBiosGetGenId(base);
    case 4:
        return gamedataAttestBiosGetAreaId(base);
    case 5:
        return gamedataAttestBiosGetLangareaId(base);
    default:
        return 0;
    }
}
