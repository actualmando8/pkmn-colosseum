/**
 * @file sequence.c
 * @brief sequence / modelSequence lineage: Colosseum-era precursor of XD's
 * ModelSequence + NullSequence classes.
 *
 * Split from the former game/battle/battle_waza.c CodeCandidate bucket
 * (0x801D1470-0x801DE698); see config/GC6E01/splits.txt for the exact
 * address range of this translation unit. Shared typedefs and cross-TU
 * forward declarations live in include/game/battle/battle_waza_types.h.
 */

#include "game/battle/battle_waza_types.h"


/**
 * fn_801DCDA8 - Waza field effect get type.
 * Address: 0x801DCDA8 | Size: 0x24
 */
void* fn_801DCDA8(void* obj, s32 fieldEffect) {
    void* cur = *(void**)((u8*)obj + 0x24);

    while (cur != NULL) {
        if (*(s32*)cur == fieldEffect) {
            return cur;
        }
        cur = *(void**)((u8*)cur + 0xA8);
    }

    return cur;
}

/**
 * fn_801DCDCC - Waza field effect set type.
 * Address: 0x801DCDCC | Size: 0x40
 */
s32 fn_801DCDCC(void* obj) {
    if (obj == NULL) {
        return 0;
    }

    if (*(u8*)((u8*)obj + 0x77) == 0) {
        return 0;
    }

    if (*(u8*)((u8*)obj + 0x4E) != 0) {
        return *(u8*)((u8*)obj + 0x4F);
    }

    return 0;
}

/**
 * fn_801DCE0C - Waza field effect render.
 * Address: 0x801DCE0C | Size: 0x9C
 */
void fn_801DCE0C(void* obj) {
    extern void GSmodelEnableColorSwap();
    extern void GSmodelEnableModulation();

    s32 handle;

    if (obj != NULL && *(u8*)((u8*)obj + 0x4F) == 0 && *(u8*)((u8*)obj + 0x4E) != 0) {
        handle = *(s32*)((u8*)obj + 0x24);
        if (*(u8*)((u8*)obj + 0x4C) != 0) {
            GSmodelEnableColorSwap(handle, *(s32*)((u8*)obj + 0x38), *(s32*)((u8*)obj + 0x3C),
                        *(s32*)((u8*)obj + 0x40), *(s32*)((u8*)obj + 0x44));
        }
        if (*(u8*)((u8*)obj + 0x4D) != 0) {
            *(u8*)((u8*)obj + 0x4B) = 0xFF;
            GSmodelEnableModulation(handle, (u8*)obj + 0x48);
        }
        *(u8*)((u8*)obj + 0x4F) = 1;
    }
}

/**
 * fn_801DCEA8 - Waza field effect clear.
 * Address: 0x801DCEA8 | Size: 0x58
 */
extern void GSmodelRemoveNull(void* obj);
extern void fn_801DEF0C(void* obj, s32 arg1, s32 arg2);
void fn_801DCEA8(void* obj) {
    u8 flags = *(u8*)((u8*)obj + 0x18);

    if ((flags & 2) == 2) {
        *(u8*)((u8*)obj + 0x18) = flags ^ 2;
        GSmodelRemoveNull(*(void**)((u8*)obj + 0x24));
        fn_801DEF0C(obj, 1, 0);
    }
}

/**
 * fn_801DCF00 - Waza lighting override set.
 * Address: 0x801DCF00 | Size: 0x84
 */
void fn_801DCF00(u32 color, f32 intensity) {
    extern u8 GSmodelIsRootNullAdded(s32);
    extern void GSmodelGetRootPosition(s32, void*);
    extern void GSmodelAddNull(s32, void*, s32, s32);
    extern void fn_801DEF0C(void*, s32, s32);

    void* obj;
    u8 flags;

    obj = (void*)color;
    flags = *(u8*)((u8*)obj + 0x18);
    if ((flags & 2) != 2) {
        *(u8*)((u8*)obj + 0x18) = flags | 2;
        if (GSmodelIsRootNullAdded(*(s32*)((u8*)obj + 0x24)) != 0) {
            GSmodelGetRootPosition(*(s32*)((u8*)obj + 0x24), (u8*)obj + 0x5C);
        } else {
            GSmodelAddNull(*(s32*)((u8*)obj + 0x24), (u8*)obj + 0x5C, 0, 0);
        }
        fn_801DEF0C(obj, 1, 0);
    }
}

/**
 * fn_801DCF84 - Waza lighting override clear.
 * Address: 0x801DCF84 | Size: 0x54
 */
void fn_801DCF84(void* obj) {
    u8 flags = *(u8*)((u8*)obj + 0x18);

    if ((flags & 8) == 8) {
        *(u8*)((u8*)obj + 0x18) = flags ^ 8;
        fn_801DEF0C(obj, 1, 1);
        fn_801DA014(obj);
    }
}

/**
 * fn_801DCFD8 - Waza lighting override get active.
 * Address: 0x801DCFD8 | Size: 0x50
 */
extern void GSmodelStopAnimation(void* obj);
void fn_801DCFD8(void* obj) {
    u8 flags = *(u8*)((u8*)obj + 0x18);

    if ((flags & 8) != 8) {
        *(u8*)((u8*)obj + 0x18) = flags | 8;
        GSmodelStopAnimation(*(void**)((u8*)obj + 0x24));
        fn_801DA070(obj);
    }
}

/**
 * fn_801DD028 - Waza lighting ambient set.
 * Address: 0x801DD028 | Size: 0x50
 */
extern void fn_801DF33C(void* obj);
void fn_801DD028(void* obj) {
    u8 flags = *(u8*)((u8*)obj + 0x18);

    if ((flags & 4) == 4) {
        fn_801DF33C(obj);
        *(u8*)((u8*)obj + 0x18) = *(u8*)((u8*)obj + 0x18) ^ 4;
        fn_801D9E34(obj);
    }
}

/**
 * fn_801DD078 - Waza lighting ambient get.
 * Address: 0x801DD078 | Size: 0x50
 */
extern void fn_801DF3D4(void* obj);
void fn_801DD078(void* obj) {
    u8 flags = *(u8*)((u8*)obj + 0x18);

    if ((flags & 4) != 4) {
        fn_801DF3D4(obj);
        *(u8*)((u8*)obj + 0x18) = *(u8*)((u8*)obj + 0x18) | 4;
        fn_801D9E8C(obj);
    }
}


/**
 * GetWaza__12NullSequenceCFUsUs - Waza lighting reset.
 * Address: 0x801DD0C8 | Size: 0x38
 */
void* GetWaza__12NullSequenceCFUsUs(void* obj, s32 search_key1, s32 search_key2)
{
    WazaFxNode* cur = ((WazaFxOwner*)obj)->first_child;

    while (cur != NULL) {
        if (cur->field_2C == (u16)search_key1 && cur->field_2E == (u16)search_key2) {
            return cur;
        }
        cur = cur->next;
    }

    return cur;
}

/**
 * fn_801DD100 - 0x801DD100 | Size: 0x58
 * Two-arg (owner, obj) per the caller in wazaSequence.c; the prior
 * (u32 filterColor) signature was a placeholder.
 */
void fn_801DD100(u8* p, u8* q) {
    if (p == NULL) return;
    if (q == NULL) {
        *(u16*)(p + 50) = 0;
        *(u16*)(p + 52) = 0;
        *(s32*)(*(u8**)(p + 44) + 144) = 0;
    } else {
        *(u16*)(p + 50) = (u8)*(u32*)(q + 12);
        *(u16*)(p + 52) = 0;
        *(s32*)(*(u8**)(p + 44) + *(u32*)(q + 12) * 212 + 144) = *(s32*)(q + 16);
    }
}

/**
 * fn_801DD158 - Waza color filter update.
 * Address: 0x801DD158 | Size: 0xE4
 */
void fn_801DD158(void* obj) {
    /* TODO: Color filter update (0xE4 bytes) */
}

/**
 * fn_801DD23C - Waza color filter transition.
 * Address: 0x801DD23C | Size: 0x1A8
 */
void fn_801DD23C(void* obj) {
    extern void fn_800E24B0(u16);
    extern void fn_800E209C(u16);
    extern void GSmodelDisableColorSwap(u32);
    extern void GSmodelDisableModulation(u32);
    extern void GSmodelSetAnimEndedCallback();
    extern void fn_801DA4E8();
    extern void fn_801193BC(s32);
    extern void fn_800F9210();
    extern void Unload__13ModelSequenceFPUc(void*);
    extern void wazaSequenceSysFreeSequenceResource(void* obj);
    extern void fn_801D9E34(void* obj);
    extern void fn_801DA014(void* obj);

    u8* data;
    u16 id;
    u8 enabled;
    u32 handle;

    data = (u8*)obj;
    if (data != NULL) {
        id = *(u16*)(data + 0x30);
        if (id != 0) {
            fn_800E24B0(id);
            fn_800E209C(id);
        }

        if (data == NULL) {
            enabled = 0;
        } else if (*(u8*)(data + 0x77) == 0) {
            enabled = 0;
        } else if (*(u8*)(data + 0x4E) == 0) {
            enabled = 0;
        } else {
            enabled = *(u8*)(data + 0x4F);
        }

        if (enabled != 0 && data != NULL && *(u8*)(data + 0x4F) != 0 && *(u8*)(data + 0x4E) != 0) {
            handle = *(u32*)(data + 0x24);
            if (*(u8*)(data + 0x4C) != 0) {
                GSmodelDisableColorSwap(handle);
            }
            if (*(u8*)(data + 0x4D) != 0) {
                GSmodelDisableModulation(handle);
            }
            *(u8*)(data + 0x4F) = 0;
        }

        if (*(u32*)(data + 0x24) != 0) {
            GSmodelSetAnimEndedCallback(*(u32*)(data + 0x24), 0, 0);
        }

        fn_801DA4E8(data, 0);

        if (*(u32*)(data + 0x0C) != 0) {
            fn_801193BC(*(s32*)(data + 0x28));
            fn_800F9210(*(u32*)data, *(u32*)(data + 0x0C));
        }

        Unload__13ModelSequenceFPUc(data + 0x50);

        if (*(u32*)data != 0) {
            if (*(u32*)(data + 4) != 0) {
                fn_800F9210(*(u32*)data, *(u32*)(data + 4));
            }
            if (*(u32*)(data + 8) != 0) {
                fn_800F9210(*(u32*)data, *(u32*)(data + 8));
            }
            id = *(u16*)(data + 0x7C);
            if (id != 0) {
                fn_800E24B0(id);
                fn_800E209C(id);
            }
        }

        wazaSequenceSysFreeSequenceResource(data);
        fn_801D9E34(data);
        fn_801DA014(data);
        memset(data, 0, 0x8C);
    }
}

/**
 * fn_801DD3E4 - Waza color filter clear.
 * Address: 0x801DD3E4 | Size: 0x78
 */
void fn_801DD3E4(void* obj) {
    extern void wazaSequenceApplyStop(void* obj);
    extern void wazaSequenceFree(void* obj);

    void* cur;
    void* next;

    if (obj != NULL) {
        cur = *(void**)((u8*)obj + 0x68);
        while (cur != NULL) {
            next = *(void**)((u8*)cur + 0x34);
            if (*(u8*)((u8*)cur + 0x14) != 0) {
                wazaSequenceApplyStop(cur);
            }
            wazaSequenceFree(cur);
            cur = next;
        }
        *(void**)((u8*)obj + 0x68) = NULL;
    }
}

/**
 * sequenceLoad - Waza scene snapshot.
 * Address: 0x801DD45C | Size: 0x18C
 */
void sequenceLoad(void) {
    /* TODO: Scene snapshot for transition effects (0x18C bytes) */
}

/**
 * fn_801DD5E8 - Waza complex transition effect.
 * Address: 0x801DD5E8 | Size: 0x564
 * Large function handling elaborate transition effects between
 * phases of a move animation.
 */
void fn_801DD5E8(void) {
    /* TODO: Complex transition effect (0x564 bytes) */
}

/**
 * fn_801DDB4C - Waza transition effect helper A.
 * Address: 0x801DDB4C | Size: 0xC4
 */
void fn_801DDB4C(void) {
    /* TODO: Transition effect helper A (0xC4 bytes) */
}

/**
 * fn_801DDC10 - Waza transition effect helper B.
 * Address: 0x801DDC10 | Size: 0x118
 */
void fn_801DDC10(void) {
    /* TODO: Transition effect helper B (0x118 bytes) */
}

/**
 * fn_801DDD28 - Waza transition effect helper C.
 * Address: 0x801DDD28 | Size: 0x1BC
 */
void fn_801DDD28(void) {
    /* TODO: Transition effect helper C (0x1BC bytes) */
}

/**
 * fn_801DDEE4 - Waza hit flash effect.
 * Address: 0x801DDEE4 | Size: 0x280
 */
void fn_801DDEE4(s32 slot, s32 flashType) {
    /* TODO: Hit flash effect (0x280 bytes) */
}

/**
 * fn_801DE164 - Waza hit flash get active.
 * Address: 0x801DE164 | Size: 0x2C
 */
BOOL fn_801DE164(s32 slot) {
    void* obj;

    obj = (void*)slot;
    if (obj == NULL) {
        return FALSE;
    }
    if (*(u8*)((u8*)obj + 0x75) != 0) {
        return *(s32*)((u8*)obj + 0x78);
    }
    return FALSE;
}

/**
 * fn_801DE190 - Waza hit flash update.
 * Address: 0x801DE190 | Size: 0x288
 */
void fn_801DE190(void) {
    /* TODO: Hit flash update (0x288 bytes) */
}

/**
 * fn_801DE418 - Waza HP drain effect.
 * Address: 0x801DE418 | Size: 0x180
 */
void fn_801DE418(s32 attackerSlot, s32 targetSlot) {
    /* TODO: HP drain effect (0x180 bytes) */
}

/**
 * fn_801DE598 - Waza HP drain update.
 * Address: 0x801DE598 | Size: 0xBC
 */
void fn_801DE598(void) {
    /* TODO: HP drain update (0xBC bytes) */
}

/**
 * sequenceAnimEndCallback - Waza HP drain get active.
 * Address: 0x801DE654 | Size: 0x44
 */
void sequenceAnimEndCallback(s32 arg0, s32 arg1) {
    extern void fn_801DE698();
    extern void _eyeTexAnimEnded();

    fn_801DE698();
    _eyeTexAnimEnded(arg0, arg1);
}
