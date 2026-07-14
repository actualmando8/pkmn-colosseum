/**
 * @file fight_action_range_801F0898.c
 * @brief Fight-action creation, priority, and FIFO dispatch.
 *
 * Split out of the former game/pokemon.c CodeCandidate bucket
 * (0x801F000C-0x801F7F80), which was mislabeled "pokemon" but is
 * entirely the XD-era fight-engine cluster. Address range covered by
 * this translation unit: 0x801F0898-0x801F150C (6 functions), per
 * config/GC6E01/splits.txt.
 */

#include "game/pokemon_fight_types.h"

/* 0x801F0898 | size: 0x90 | medium */
u32 fightActionGetKindDataId(u32 param) {
    extern u32 fightActionDataBiosGetKind();
    extern u32 fightActionBiosGetFightActionDataPtr();
    extern u32 fightActionBiosGetKind();
    u32 valid;
    if (param != 0) goto chk1;
    valid = 0;
    goto join;
    chk1:
    if ((u16)fightActionBiosGetKind() != 0) goto chk2;
    valid = 0;
    goto join;
    chk2:
    if (fightActionBiosGetFightActionDataPtr(param) != 0) goto set1;
    valid = 0;
    goto join;
    set1:
    valid = 1;
    join:
    if ((u8)valid == 0)
        return 0;
    if (fightActionBiosGetFightActionDataPtr(param) == 0)
        return 0;
    return fightActionDataBiosGetKind();
}

/* 0x801F0928 | size: 0xA8 | medium */
s32 fightActionGetPri(u32 param) {
    extern u32 fightActionKindDataBiosGetPri();
    extern u32 fightActionKindDataBiosGetPtr();
    extern u32 fightActionDataBiosGetKind();
    extern u32 fightActionBiosGetFightActionDataPtr();
    extern u32 fightActionBiosGetKind();
    u32 valid;
    if (param != 0) goto chk1;
    valid = 0;
    goto join;
    chk1:
    if ((u16)fightActionBiosGetKind() != 0) goto chk2;
    valid = 0;
    goto join;
    chk2:
    if (fightActionBiosGetFightActionDataPtr(param) != 0) goto set1;
    valid = 0;
    goto join;
    set1:
    valid = 1;
    join:
    if ((u8)valid == 0)
        return -0x80;
    if (fightActionBiosGetFightActionDataPtr(param) == 0)
        return -0x80;
    fightActionDataBiosGetKind();
    if (fightActionKindDataBiosGetPtr() == 0)
        return -0x80;
    return fightActionKindDataBiosGetPri();
}

/* 0x801F0B00 | size: 0x404 */
u32 fightActionCreateAndFlowFifo(void* action, void* motoAction, void* actorTarget,
                                 u32 kind, void* buff, void* actionData)
{
    extern u8 lbl_8046D790[];
    extern u32 lbl_8047B5E8;
    extern u32 lbl_8047B5EC;
    extern void fightActionBiosSetKind(void*, u32);
    extern void fightActionBiosSetBuff(void*, void*);
    extern void fightActionBiosSetFightActionDataPtr(void*, void*);
    extern void fightActionBiosSetDispBuff(void*, u32, u32);
    extern void fightActionBiosSetBuffDataPtr(void*, u32);
    extern void fightActionBiosSetBuffDataId(void*, void*);
    extern void fightActionBiosSetActorFightTargetPtr(void*, void*);
    extern void fightActionBiosSetMotoFightActionDataPtr(void*, void*);
    extern void fightActionBiosSetFifoBanme(void*, s32);
    extern u16 fightActionBiosGetKind(void*);
    extern void* fightActionBiosGetBuff(void*);
    extern void* fightActionBiosGetFightActionDataPtr(void*);
    extern u32 fightActionDataBiosGetKind(void*);
    extern void* fightActionDataBiosGetBuff(void*);
    extern void* fightActionKindDataBiosGetPtr(u32);
    extern void* fightActionKindDataBiosGetFlowFuncPtr(void*);
    extern void fn_8020D968(void*, void*);

    struct Entry { u32 word[12]; };
    void* found;
    u32 i;
    u32 result;
    u32 fifoIndex;
    struct Entry* fifoEntry;

    fightActionBiosSetKind(action, 0);
    fightActionBiosSetBuff(action, 0);
    fightActionBiosSetFightActionDataPtr(action, 0);
    for (i = 0; (u16)i < 4; i++) {
        fightActionBiosSetDispBuff(action, i, 0);
    }
    fightActionBiosSetBuffDataPtr(action, 0);
    fightActionBiosSetBuffDataId(action, 0);
    fightActionBiosSetActorFightTargetPtr(action, 0);
    fightActionBiosSetMotoFightActionDataPtr(action, 0);
    fightActionBiosSetFifoBanme(action, -1);

    fightActionBiosSetKind(action, kind);
    fightActionBiosSetBuff(action, buff);
    fightActionBiosSetFightActionDataPtr(action, actionData);
    fightActionBiosSetActorFightTargetPtr(action, actorTarget);

    found = 0;
    if (action != 0 && (fightActionBiosGetKind(action) & 0xFFFF) != 0 &&
        fightActionBiosGetFightActionDataPtr(action) != 0) {
        u32 actionKind = fightActionBiosGetKind(action) & 0xFFFF;
        void* actionBuff = fightActionBiosGetBuff(action);
        u8* data = fightActionBiosGetFightActionDataPtr(action);
        u32 index = 0;

        while (1) {
            void* entry = data + ((index & 0xFFFF) * 8);
            u32 entryKind = fightActionDataBiosGetKind(entry);

            if ((entryKind & 0xFFFF) == 0) {
                break;
            }
            if (actionKind == (entryKind & 0xFFFF) &&
                actionBuff == fightActionDataBiosGetBuff(entry)) {
                found = entry;
                break;
            }
            index++;
        }
        if (found != 0) {
            fightActionBiosSetFightActionDataPtr(action, found);
        }
    }

    if (found == 0) {
        fightActionBiosSetKind(action, 0);
        fightActionBiosSetBuff(action, 0);
        fightActionBiosSetFightActionDataPtr(action, 0);
        for (i = 0; (u16)i < 4; i++) {
            fightActionBiosSetDispBuff(action, i, 0);
        }
        fightActionBiosSetBuffDataPtr(action, 0);
        fightActionBiosSetBuffDataId(action, 0);
        fightActionBiosSetActorFightTargetPtr(action, 0);
        fightActionBiosSetMotoFightActionDataPtr(action, 0);
        fightActionBiosSetFifoBanme(action, -1);
        result = 4;
    } else {
        fightActionBiosSetBuffDataId(action, buff);
        fightActionBiosSetMotoFightActionDataPtr(action, motoAction);
        result = 1;
    }

    if ((u8)result != 1) {
        return result;
    }

    fifoIndex = lbl_8047B5E8;
    if (((fifoIndex + 1) & 0x1F) == lbl_8047B5EC) {
        return 2;
    }

    fifoEntry = (struct Entry*)(lbl_8046D790 + fifoIndex * 0x30);
    *fifoEntry = *(struct Entry*)action;
    lbl_8047B5E8 = (fifoIndex + 1) & 0x1F;
    fightActionBiosSetFifoBanme(action, fifoIndex);
    fightActionBiosSetFifoBanme(fifoEntry, fifoIndex);

    if (action != 0 && (fightActionBiosGetKind(action) & 0xFFFF) != 0 &&
        fightActionBiosGetFightActionDataPtr(action) != 0) {
        void* flow = fightActionKindDataBiosGetFlowFuncPtr(
            fightActionKindDataBiosGetPtr(
                fightActionDataBiosGetKind(fightActionBiosGetFightActionDataPtr(action))));
        if (flow != 0) {
            result = ((u32 (*)(void*))flow)(action);
        } else {
            result = 1;
        }
    } else {
        result = 0;
    }

    if ((u8)result != 1) {
        if (lbl_8047B5E8 != lbl_8047B5EC) {
            lbl_8047B5EC = (lbl_8047B5EC + 1) & 0x1F;
        }
    } else {
        fn_8020D968(fifoEntry, action);
    }

    return result;
}


/* 0x801F0F04 | size: 0x188 | medium */
u32 fightActionFlowFifo(void* param) {
    extern u8 lbl_8046D790[];
    extern u32 lbl_8047B5E8;
    extern u32 lbl_8047B5EC;
    extern void fightActionBiosSetFifoBanme(void*, u32);
    extern u16 fightActionBiosGetKind(void*);
    extern void* fightActionBiosGetFightActionDataPtr(void*);
    extern void fightActionDataBiosGetKind(void*);
    extern void fightActionKindDataBiosGetPtr(void*);
    extern void fightActionKindDataBiosGetFlowFuncPtr(void*);
    extern void fn_8020D968(void*, void*);
    struct Entry { u32 data[12]; };
    void* src;
    u32 idx;
    struct Entry* slot;
    u32 result;
    void* fn;

    src = param;
    idx = lbl_8047B5E8;
    if (((idx + 1) & 0x1F) != lbl_8047B5EC) {
        slot = (struct Entry*)((u8*)lbl_8046D790 + idx * 0x30);
        *(struct Entry*)slot = *(struct Entry*)src;
        lbl_8047B5E8 = (idx + 1) & 0x1F;
        fightActionBiosSetFifoBanme(src, idx);
        fightActionBiosSetFifoBanme(slot, idx);
    } else {
        slot = 0;
    }
    if (slot == 0) {
        return 0x2;
    }
    {
        u32 active = 0;
        if (src != 0) {
            if ((fightActionBiosGetKind(src) & 0xFFFF) != 0) {
                if (fightActionBiosGetFightActionDataPtr(src) != 0) {
                    active = 1;
                }
            }
        }
        if ((active & 0xFF) == 0) {
            result = 0;
        } else {
            fn = fightActionBiosGetFightActionDataPtr(src);
            fightActionDataBiosGetKind(src);
            fightActionKindDataBiosGetPtr(src);
            fightActionKindDataBiosGetFlowFuncPtr(src);
            if (fn != 0) {
                result = ((u32(*)(void*))fn)(src);
            } else {
                result = 1;
            }
        }
    }
    if ((result & 0xFF) != 1) {
        if (lbl_8047B5E8 != lbl_8047B5EC) {
            lbl_8047B5EC = (lbl_8047B5EC + 1) & 0x1F;
        }
        return result;
    }
    fn_8020D968(slot, src);
    return result;
}

/* 0x801F108C | size: 0xE4 */
void fightActionFifoInit(void) {
    extern u8 lbl_8046D790[];
    extern u32 lbl_8047B5E8;
    extern u32 lbl_8047B5EC;
    extern void fightActionBiosSetKind(void*, u32);
    extern void fightActionBiosSetBuff(void*, u32);
    extern void fightActionBiosSetFightActionDataPtr(void*, u32);
    extern void fightActionBiosSetDispBuff(void*, u32, u32);
    extern void fightActionBiosSetBuffDataPtr(void*, u32);
    extern void fightActionBiosSetBuffDataId(void*, u32);
    extern void fightActionBiosSetActorFightTargetPtr(void*, u32);
    extern void fightActionBiosSetMotoFightActionDataPtr(void*, u32);
    extern void fightActionBiosSetFifoBanme(void*, s32);
    u32 j;
    void* action;
    u32 i;

    lbl_8047B5EC = 0;
    lbl_8047B5E8 = 0;
    for (i = 0; (u16)i < 32; i++) {
        action = &lbl_8046D790[(u16)i * 0x30];
        fightActionBiosSetKind(action, 0);
        fightActionBiosSetBuff(action, 0);
        fightActionBiosSetFightActionDataPtr(action, 0);
        for (j = 0; (u16)j < 4; j++) {
            fightActionBiosSetDispBuff(action, j, 0);
        }
        fightActionBiosSetBuffDataPtr(action, 0);
        fightActionBiosSetBuffDataId(action, 0);
        fightActionBiosSetActorFightTargetPtr(action, 0);
        fightActionBiosSetMotoFightActionDataPtr(action, 0);
        fightActionBiosSetFifoBanme(action, -1);
    }
}

/* 0x801F1170 | size: 0x5C | small */
u32 fightActionCheckValid(void* param) {
    extern u16 fightActionBiosGetKind(void*);
    extern void* fightActionBiosGetFightActionDataPtr(void*);
    void* obj;

    obj = param;
    if (obj == 0) {
        return 0;
    }
    if ((fightActionBiosGetKind(obj) & 0xFFFF) == 0) {
        return 0;
    }
    return -(s32)fightActionBiosGetFightActionDataPtr(obj) != 0;
}

/* 0x801F11CC | size: 0x294 | large */
s32 fightActionCreate(void* p1, void* p2, void* p3, void* p4, void* p5, void* p6) {
    extern void fightActionBiosSetFifoBanme(void*, s32);
    extern void* fightActionDataBiosGetBuff(void*);
    extern u32 fightActionDataBiosGetKind(void*);
    extern void fightActionBiosSetDispBuff(void*, u32, u32);
    extern void fightActionBiosSetMotoFightActionDataPtr(void*, void*);
    extern void fightActionBiosSetBuffDataId(void*, void*);
    extern void fightActionBiosSetBuffDataPtr(void*, u32);
    extern void fightActionBiosSetActorFightTargetPtr(void*, void*);
    extern void fightActionBiosSetFightActionDataPtr(void*, void*);
    extern void fightActionBiosSetBuff(void*, void*);
    extern void fightActionBiosSetKind(void*, void*);
    extern void* fightActionBiosGetFightActionDataPtr(void*);
    extern void* fightActionBiosGetBuff(void*);
    extern u32 fightActionBiosGetKind(void*);
    void* param2;
    void* obj;
    void* param3;
    void* param4;
    void* param5;
    void* param6;
    u32 r25;
    u32 r27;
    u32 r28;
    void* r26;
    void* r24;

    param2 = p2;
    obj = p1;
    param3 = p3;
    param4 = p4;
    param5 = p5;
    param6 = p6;

    fightActionBiosSetKind(obj, 0);
    fightActionBiosSetBuff(obj, 0);
    fightActionBiosSetFightActionDataPtr(obj, 0);
    r25 = 0;
    while ((r25 & 0xFFFF) < 4) {
        fightActionBiosSetDispBuff(obj, r25, 0);
        r25++;
    }
    fightActionBiosSetBuffDataPtr(obj, 0);
    fightActionBiosSetBuffDataId(obj, 0);
    fightActionBiosSetActorFightTargetPtr(obj, 0);
    fightActionBiosSetMotoFightActionDataPtr(obj, 0);
    fightActionBiosSetFifoBanme(obj, -1);
    fightActionBiosSetKind(obj, param4);
    fightActionBiosSetBuff(obj, param5);
    fightActionBiosSetFightActionDataPtr(obj, param6);
    fightActionBiosSetActorFightTargetPtr(obj, param3);

    {
        u32 active = 0;
        if (obj != 0) {
            if ((fightActionBiosGetKind(obj) & 0xFFFF) != 0) {
                if (fightActionBiosGetFightActionDataPtr(obj) != 0) {
                    active = 1;
                }
            }
        }
        if ((active & 0xFF) == 0) {
            r24 = 0;
            goto end;
        }
    }

    r27 = fightActionBiosGetKind(obj);
    r25 = (u32)fightActionBiosGetBuff(obj);
    r26 = fightActionBiosGetFightActionDataPtr(obj);
    if (r26 != 0) {
        param6 = 0;
        r28 = r27 & 0xFFFF;
        while (1) {
            r24 = (void*)((u8*)r26 + ((u32)param6 & 0xFFFF) * 8);
            r27 = fightActionDataBiosGetKind(r24);
            if ((r27 & 0xFFFF) != 0) {
                if (r28 == (fightActionDataBiosGetKind(r24) & 0xFFFF) && (void*)r25 == fightActionDataBiosGetBuff(r24)) {
                    goto found;
                }
                param6 = (void*)((u32)param6 + 1);
                continue;
            }
            r24 = 0;
            break;
        }
    } else {
        r24 = 0;
    }
found:
    if (r24 != 0) {
        fightActionBiosSetFightActionDataPtr(obj, r24);
    }
end:
    if (r24 == 0) {
        fightActionBiosSetKind(obj, 0);
        fightActionBiosSetBuff(obj, 0);
        fightActionBiosSetFightActionDataPtr(obj, 0);
        r25 = 0;
        while ((r25 & 0xFFFF) < 4) {
            fightActionBiosSetDispBuff(obj, r25, 0);
            r25++;
        }
        fightActionBiosSetBuffDataPtr(obj, 0);
        fightActionBiosSetBuffDataId(obj, 0);
        fightActionBiosSetActorFightTargetPtr(obj, 0);
        fightActionBiosSetMotoFightActionDataPtr(obj, 0);
        fightActionBiosSetFifoBanme(obj, -1);
        return 4;
    }
    fightActionBiosSetBuffDataId(obj, param5);
    fightActionBiosSetMotoFightActionDataPtr(obj, param2);
    return 1;
}

/* 0x801F1460 | size: 0xB4 | fightActionInit */
void fightActionInit(u8* ptr) {
    extern void fightActionBiosSetKind(u8*, u32);
    extern void fightActionBiosSetBuff(u8*, u32);
    extern void fightActionBiosSetFightActionDataPtr(u8*, u32);
    extern void fightActionBiosSetDispBuff(u8*, u32, u32);
    extern void fightActionBiosSetBuffDataPtr(u8*, u32);
    extern void fightActionBiosSetBuffDataId(u8*, u32);
    extern void fightActionBiosSetActorFightTargetPtr(u8*, u32);
    extern void fightActionBiosSetMotoFightActionDataPtr(u8*, u32);
    extern void fightActionBiosSetFifoBanme(u8*, s32);
    u32 i;

    fightActionBiosSetKind(ptr, 0);
    fightActionBiosSetBuff(ptr, 0);
    fightActionBiosSetFightActionDataPtr(ptr, 0);
    for (i = 0; (u16)i < 4; i++) {
        fightActionBiosSetDispBuff(ptr, i, 0);
    }
    fightActionBiosSetBuffDataPtr(ptr, 0);
    fightActionBiosSetBuffDataId(ptr, 0);
    fightActionBiosSetActorFightTargetPtr(ptr, 0);
    fightActionBiosSetMotoFightActionDataPtr(ptr, 0);
    fightActionBiosSetFifoBanme(ptr, -1);
}
