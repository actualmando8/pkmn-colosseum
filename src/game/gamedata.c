/**
 * @file gamedata.c
 * @brief Decompiled functions.
 *
 * Address range: 0x80135030 - 0x80135A30
 *
 * Split out of the former game/effect/effect_util.c CodeCandidate
 * bucket (0x8013151C - 0x80137114); see effect_util_types.h for
 * shared cross-TU declarations.
 */

#include "dolphin/types.h"
#include "game/effect/effect_util_types.h"


/* 0x80135030 | 0x138 */
#if 0
asm void gamedatasaveSetStatus(void) {
#include "src/game/effect/effect_util_gamedatasaveSetStatus.inc"
}
#else
void gamedatasaveSetStatus(void* ptr, u16 kind, u32 value) {
    void* sub;

    if (kind == 0 || kind >= 0xB) {
        return;
    }

    if (ptr == NULL) {
        ptr = (void*)savedataGetStatus(0, 0);
        if (ptr == NULL) {
            return;
        }
        ptr = (void*)savedataGetStatus((u32)ptr, 1);
        if (ptr == NULL) {
            return;
        }
    }

    sub = gamedatasaveBiosGetPtr(ptr);
    if (sub == NULL) {
        return;
    }

    switch (kind) {
    case 1:
        gamedatasaveBiosSetPtr(ptr, (void*)value);
        break;
    case 2:
        gamedatasaveBiosSetSavernd(sub, value);
        break;
    case 3:
        gamedatasaveBiosSetSavecount(sub, value);
        break;
    case 4:
        gamedatasaveBiosSetFloorid(sub, value);
        break;
    case 5:
        gamedatasaveBiosSetPlaytime(sub, (f32)(s32)value);
        break;
    case 6:
        gamedatasaveBiosSetPrevfloorid(sub, value);
        break;
    case 7:
        gamedatasaveBiosSetFloorposindex(sub, (u8)value);
        break;
    case 8:
        gamedatasaveBiosSetOptionNoVibration(sub, (u8)value);
        break;
    case 9:
        gamedatasaveBiosSetOptionAudio(sub, (u8)value);
        break;
    default:
        break;
    }
}
#endif


/* 0x80135168 | 0x124 */
#if 0
asm void gamedatasaveGetStatus(void) {
#include "src/game/effect/effect_util_gamedatasaveGetStatus.inc"
}
#else
u32 gamedatasaveGetStatus(void* ptr, u16 kind) {
    void* base = NULL;
    void* sub;

    if (kind == 0 || kind >= 0xB) {
        return 0;
    }
    if (ptr == NULL) {
        base = (void*)savedataGetStatus(0, 0);
        if (base == NULL) {
            return 0;
        }
        ptr = (void*)savedataGetStatus((u32)base, 1);
        if (ptr == NULL) {
            return 0;
        }
    }
    sub = gamedatasaveBiosGetPtr(ptr);
    if (sub == NULL) {
        return 0;
    }
    switch (kind) {
    case 0:
        return (u32)base;
    case 1:
        return gamedatasaveBiosGetSavernd(sub);
    case 2:
        return gamedatasaveBiosGetSavecount(sub);
    case 3:
        return gamedatasaveBiosGetFloorid(sub);
    case 4:
        return (s32)gamedatasaveBiosGetPlaytime(sub);
    case 5:
        return gamedatasaveBiosGetPrevfloorid(sub);
    case 6:
        return gamedatasaveBiosGetFloorposindex(sub);
    case 7:
        return gamedatasaveBiosGetOptionNoVibration(sub);
    case 8:
        return gamedatasaveBiosGetOptionAudio(sub);
    default:
        return 0;
    }
}
#endif


/* 0x8013528C | 0xAC */
#if 0
asm void fn_8013528C(void) {
#include "src/game/effect/effect_util_fn_8013528C.inc"
}
#else
#pragma optimization_level 4
void fn_8013528C(void* ptr, u8 r4, u8 r5, u8 r6, u8 r7) {
    void* base;
    if (ptr == 0) return;
    gamedataInit(ptr);
    if (ptr == 0) {
        base = (void*)savedataGetStatus(0, 0);
        if (base == 0) return;
        base = (void*)savedataGetStatus((u32)base, 1);
        if (base == 0) return;
    } else {
        base = ptr;
    }
    base = (void*)gamedataBiosGetGamedataAtttestPtr(base);
    if (base == 0) return;
    fn_801353C0(base, r4, r5, r6, r7);
}
#endif


/* 0x80135338 | 0x88 */
#if 0
asm void gamedataInit(void) {
#include "src/game/effect/effect_util_gamedataInit.inc"
}
#else
#pragma optimization_level 4
void gamedataInit(void* ptr) {
    void* base;
    if (ptr == 0) {
        base = (void*)savedataGetStatus(0, 0);
        if (base == 0) return;
        base = (void*)savedataGetStatus((u32)base, 1);
        if (base == 0) return;
    } else {
        base = ptr;
    }
    base = (void*)gamedataBiosGetGamedataAtttestPtr(base);
    if (base == 0) return;
    fn_80135708(base);
    gamedatasaveInit(ptr);
}
#endif


/* 0x801353C0 | 0x170 */
#if 0
asm void fn_801353C0(void) {
#include "src/game/effect/effect_util_fn_801353C0.inc"
}
#else
#pragma optimization_level 4
void fn_801353C0(void* ptr, u8 r4, u8 r5, u8 r6, u8 r7) {
    void* base;
    if (ptr == 0) return;
    if ((r4 & 0xFF) == 0) return;
    if ((r5 & 0xFF) == 0) return;
    if ((r6 & 0xFF) == 0) return;
    if ((r7 & 0xFF) == 0) return;
    fn_80135708(ptr);
    /* A60 */
    if (ptr == 0) {
        base = (void*)savedataGetStatus(0, 0);
        if (base == 0) { base = 0; } else { base = (void*)savedataGetStatus((u32)base, 1); }
        base = (void*)gamedataBiosGetGamedataAtttestPtr(base);
        if (base != 0) { gamedataAttestBiosSetVerId(base, (u8)(r4 & 0xFF)); }
    } else {
        base = (void*)gamedataBiosGetGamedataAtttestPtr(ptr);
        if (base != 0) { gamedataAttestBiosSetVerId(base, (u8)(r4 & 0xFF)); }
    }
    /* A50 */
    if (ptr == 0) {
        base = (void*)savedataGetStatus(0, 0);
        if (base == 0) { base = 0; } else { base = (void*)savedataGetStatus((u32)base, 1); }
        base = (void*)gamedataBiosGetGamedataAtttestPtr(base);
        if (base != 0) { gamedataAttestBiosSetGenId(base, (u8)(r5 & 0xFF)); }
    } else {
        base = (void*)gamedataBiosGetGamedataAtttestPtr(ptr);
        if (base != 0) { gamedataAttestBiosSetGenId(base, (u8)(r5 & 0xFF)); }
    }
    /* A40 */
    if (ptr == 0) {
        base = (void*)savedataGetStatus(0, 0);
        if (base == 0) { base = 0; } else { base = (void*)savedataGetStatus((u32)base, 1); }
        base = (void*)gamedataBiosGetGamedataAtttestPtr(base);
        if (base != 0) { gamedataAttestBiosSetAreaId(base, (u8)(r6 & 0xFF)); }
    } else {
        base = (void*)gamedataBiosGetGamedataAtttestPtr(ptr);
        if (base != 0) { gamedataAttestBiosSetAreaId(base, (u8)(r6 & 0xFF)); }
    }
    /* A30 */
    if (ptr == 0) {
        base = (void*)savedataGetStatus(0, 0);
        if (base == 0) return;
        base = (void*)savedataGetStatus((u32)base, 1);
        if (base == 0) return;
    } else {
        base = ptr;
    }
    base = (void*)gamedataBiosGetGamedataAtttestPtr(base);
    if (base == 0) return;
    gamedataAttestBiosSetLangareaId(base, (u8)(r7 & 0xFF));
}
#endif


/* 0x80135530 | 0x1D8 */
#if 0
asm void fn_80135530(void) {
#include "src/game/effect/effect_util_fn_80135530.inc"
}
#else
#pragma optimization_level 4
u32 fn_80135530(void* ptr) {
    void* base; u32 r0; u32 r3;
    if (ptr == 0) return 0;
    /* AB8 */
    if (ptr == 0) {
        base = (void*)savedataGetStatus(0, 0);
        if (base == 0) { r0 = 0; } else {
            base = (void*)savedataGetStatus((u32)base, 1);
            if (base == 0) { r0 = 0; } else { base = (void*)gamedataBiosGetGamedataAtttestPtr(base); r0 = base ? ((u32)gamedataAttestBiosGetVerId(base) & 0xFF) : 0; }
        }
    } else {
        base = (void*)gamedataBiosGetGamedataAtttestPtr(ptr);
        if (base != 0) { r0 = (u32)gamedataAttestBiosGetVerId(base) & 0xFF; } else { r0 = 0; }
    }
    if (r0 == 0) return 0;
    /* AA0 */
    if (ptr == 0) {
        base = (void*)savedataGetStatus(0, 0);
        if (base == 0) { r0 = 0; } else {
            base = (void*)savedataGetStatus((u32)base, 1);
            if (base == 0) { r0 = 0; } else { base = (void*)gamedataBiosGetGamedataAtttestPtr(base); r0 = base ? ((u32)gamedataAttestBiosGetGenId(base) & 0xFF) : 0; }
        }
    } else {
        base = (void*)gamedataBiosGetGamedataAtttestPtr(ptr);
        if (base != 0) { r0 = (u32)gamedataAttestBiosGetGenId(base) & 0xFF; } else { r0 = 0; }
    }
    if (r0 == 0) return 0;
    /* A88 */
    if (ptr == 0) {
        base = (void*)savedataGetStatus(0, 0);
        if (base == 0) { r0 = 0; } else {
            base = (void*)savedataGetStatus((u32)base, 1);
            if (base == 0) { r0 = 0; } else { base = (void*)gamedataBiosGetGamedataAtttestPtr(base); r0 = base ? ((u32)gamedataAttestBiosGetAreaId(base) & 0xFF) : 0; }
        }
    } else {
        base = (void*)gamedataBiosGetGamedataAtttestPtr(ptr);
        if (base != 0) { r0 = (u32)gamedataAttestBiosGetAreaId(base) & 0xFF; } else { r0 = 0; }
    }
    if (r0 == 0) return 0;
    /* A70 */
    if (ptr == 0) {
        base = (void*)savedataGetStatus(0, 0);
        if (base == 0) { r3 = 0; } else {
            base = (void*)savedataGetStatus((u32)base, 1);
            if (base == 0) { r3 = 0; } else { base = (void*)gamedataBiosGetGamedataAtttestPtr(base); if (base) { r3 = (u32)gamedataAttestBiosGetLangareaId(base) & 0xFF; } else { r3 = 0; } }
        }
    } else {
        base = (void*)gamedataBiosGetGamedataAtttestPtr(ptr);
        if (base != 0) { r3 = (u32)gamedataAttestBiosGetLangareaId(base) & 0xFF; } else { r3 = 0; }
    }
    return (r3 != 0) ? 1 : 0;
}
#endif


/* 0x80135708 | 0x134 */
#if 0
asm void fn_80135708(void) {
#include "src/game/effect/effect_util_fn_80135708.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling on
void fn_80135708(void* ptr) {
    void* r31 = ptr;
    void* base;
    if (r31 == 0) goto _end;
    /* A60 */
    base = r31;
    if (r31 != 0) goto _a60_handler;
    base = (void*)savedataGetStatus(0, 0);
    if (base == 0) goto _a50;
    base = (void*)savedataGetStatus((u32)base, 1);
    if (base == 0) goto _a50;
_a60_handler:
    base = (void*)gamedataBiosGetGamedataAtttestPtr(base);
    if (base == 0) goto _a50;
    gamedataAttestBiosSetVerId(base, 0);
_a50:
    /* A50 */
    base = r31;
    if (r31 != 0) goto _a50_handler;
    base = (void*)savedataGetStatus(0, 0);
    if (base == 0) goto _a40;
    base = (void*)savedataGetStatus((u32)base, 1);
    if (base == 0) goto _a40;
_a50_handler:
    base = (void*)gamedataBiosGetGamedataAtttestPtr(base);
    if (base == 0) goto _a40;
    gamedataAttestBiosSetGenId(base, 0);
_a40:
    /* A40 */
    base = r31;
    if (r31 != 0) goto _a40_handler;
    base = (void*)savedataGetStatus(0, 0);
    if (base == 0) goto _a30;
    base = (void*)savedataGetStatus((u32)base, 1);
    if (base == 0) goto _a30;
_a40_handler:
    base = (void*)gamedataBiosGetGamedataAtttestPtr(base);
    if (base == 0) goto _a30;
    gamedataAttestBiosSetAreaId(base, 0);
_a30:
    /* A30 */
    base = r31;
    if (r31 != 0) goto _a30_handler;
    base = (void*)savedataGetStatus(0, 0);
    if (base == 0) goto _end;
    base = (void*)savedataGetStatus((u32)base, 1);
    if (base == 0) goto _end;
_a30_handler:
    base = (void*)gamedataBiosGetGamedataAtttestPtr(base);
    if (base == 0) goto _end;
    gamedataAttestBiosSetLangareaId(base, 0);
_end:;
}
#pragma scheduling off
#endif


typedef struct GameData GameData;
typedef struct GameDataAttest GameDataAttest;

/* 0x8013583C | 0xFC */
#if 0
asm void fn_8013583C(void) {
#include "src/game/effect/effect_util_fn_8013583C.inc"
}
#else
#pragma push
#pragma scheduling on
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
#pragma pop
#endif


/* 0x80135938 | 0xF8 */
#if 0
asm void fn_80135938(void) {
#include "src/game/effect/effect_util_fn_80135938.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma optimization_level 1
u8 fn_80135938(void* ptr, u16 effect_type) {
    void* base; u16 et;
    et = effect_type & 0xFFFF;
    if (et == 0 || et >= 7) return 0;
    if (ptr == 0) {
        base = (void*)savedataGetStatus(0, 0);
        if (base == 0) return 0;
        base = (void*)savedataGetStatus((u32)base, 1);
        if (base == 0) return 0;
    } else {
        base = ptr;
    }
    base = (void*)gamedataBiosGetGamedataAtttestPtr(base);
    if (base == 0) return 0;
    switch (et) {
        case 1: return 0;
        case 2: return (u8)gamedataAttestBiosGetVerId(base);
        case 3: return (u8)gamedataAttestBiosGetGenId(base);
        case 4: return (u8)gamedataAttestBiosGetAreaId(base);
        case 5: return (u8)gamedataAttestBiosGetLangareaId(base);
        default: return 0;
    }
}
#pragma pop
#endif
