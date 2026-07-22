#include "dolphin/types.h"
#include "game/ps.h"
#include "game/script/script.h"

extern PSGeneratorState* lbl_8047B188;
extern void* lbl_80452DC8[];
extern const char lbl_80273820[];
extern const char lbl_8027382C[];

extern PSParticle* _psListGetFirst(s32 linkNo);
extern void _psListDelete(PSParticle* pp, PSParticle* parent);
extern void psKillGeneratorID(s32 familyId);
extern s32 psRemoveParticleAppSRT(PSParticle* pp);
extern void fn_801A05EC(void* renderObj);
extern void __assert(const char* file, u32 line, const char* msg);

void psSetRandomVelocityScaling(PSGeneratorState* gen, u8 enabled)
{
    if (enabled) {
        gen->generatorFlags |= 0x200;
    } else {
        gen->generatorFlags = (u32)gen->generatorFlags & ~0x200;
    }
}

void psSetNodeScaling(PSGeneratorState* gen, u8 enabled)
{
    if (enabled) {
        gen->generatorFlags |= 0x100;
    } else {
        gen->generatorFlags = (u32)gen->generatorFlags & ~0x100;
    }
}

void psSetTornadoScaling(PSGeneratorState* gen, u8 enabledA, u8 enabledB)
{
    if (enabledA) {
        gen->generatorFlags |= 0x40;
    } else {
        gen->generatorFlags = (u32)gen->generatorFlags & ~0x40;
    }

    if (enabledB) {
        gen->generatorFlags |= 0x80;
    } else {
        gen->generatorFlags = (u32)gen->generatorFlags & ~0x80;
    }
}

void psSetParticleTexScaling(PSGeneratorState* gen, u8 enabled)
{
    if (enabled) {
        gen->generatorFlags |= 0x20;
    } else {
        gen->generatorFlags = (u32)gen->generatorFlags & ~0x20;
    }
}

void psSetOffsetRotationInLocal(PSGeneratorState* gen, u8 enabledA, u8 enabledB)
{
    if (enabledA) {
        gen->generatorFlags |= 0x8;
        if (enabledB) {
            gen->generatorFlags |= 0x10;
        } else {
            gen->generatorFlags = (u32)gen->generatorFlags & ~0x10;
        }
    } else {
        gen->generatorFlags = (u32)gen->generatorFlags & ~0x8;
        gen->generatorFlags = (u32)gen->generatorFlags & ~0x10;
    }
}

void psSetVelocityRotationInLocal(PSGeneratorState* gen, u8 enabled)
{
    if (enabled) {
        gen->generatorFlags |= 0x4;
    } else {
        gen->generatorFlags = (u32)gen->generatorFlags & ~0x4;
    }
}

void psUnlinkChildGensFromJObj(PSGeneratorState* gen)
{
    gen->generatorFlags = (u32)gen->generatorFlags & ~0x1;
}

void psLinkChildGensToJObj(PSGeneratorState* gen, void* jobj)
{
    gen->generatorFlags |= 0x1;
    gen->linkedJObj = jobj;
}

u32 psGetParticleChildCount(PSGeneratorState* gen)
{
    u32 count = 0;
    PSGeneratorState* child = lbl_8047B188;

    while (child != NULL) {
        if (child->familyId == gen->familyId) {
            count += child->childCount;
        }
        child = child->next;
    }

    return count;
}

u32 psGetGeneratorChildMaxLife(PSGeneratorState* gen)
{
    u32 maxLife = gen->maxLife;
    PSGeneratorState* child = lbl_8047B188;

    while (child != NULL) {
        if (child->familyId == gen->familyId && child->maxLife > maxLife) {
            maxLife = child->maxLife;
        }
        child = child->next;
    }

    return maxLife;
}

void psKillGeneratorChild(PSGeneratorState* gen)
{
    PSParticle* next;
    PSParticle* parent = NULL;
    PSParticle* pp;
    u16 familyId = gen->familyId;

    pp = _psListGetFirst(gen->linkNo);

    while (pp != NULL) {
        next = pp->next;

        if (pp->scriptId == familyId && pp->peopleObj != NULL && pp->peopleObj == gen) {
            if (pp->peopleObj != NULL) {
                ((PSGeneratorState*)pp->peopleObj)->childCount--;
            }
            if (pp->parentObj != NULL) {
                psRemoveParticleAppSRT(pp);
            }
            if (pp->flags & PS_FLAG_ATTACH_CAMERA) {
                u32 slotIdx = (pp->flags >> 12) & 0x7;
                void* camSlot = lbl_80452DC8[slotIdx];

                if (camSlot != NULL) {
                    fn_801A05EC(camSlot);
                    lbl_80452DC8[slotIdx] = NULL;
                }
            }
            _psListDelete(pp, parent);
        } else {
            parent = pp;
        }
        pp = next;
    }
}

void psKillFamily(s32 familyId, s32 linkNo)
{
    PSParticle* next;
    PSParticle* parent = NULL;
    PSParticle* pp;

    pp = _psListGetFirst(linkNo);

    while (pp != NULL) {
        next = pp->next;

        if (pp->scriptId == (u16)familyId) {
            if (pp->peopleObj != NULL) {
                ((PSGeneratorState*)pp->peopleObj)->childCount--;
            }
            if (pp->parentObj != NULL) {
                psRemoveParticleAppSRT(pp);
            }
            if (pp->flags & PS_FLAG_ATTACH_CAMERA) {
                u32 slotIdx = (pp->flags >> 12) & 0x7;
                void* camSlot = lbl_80452DC8[slotIdx];

                if (camSlot != NULL) {
                    fn_801A05EC(camSlot);
                    lbl_80452DC8[slotIdx] = NULL;
                }
            }
            _psListDelete(pp, parent);
        } else {
            parent = pp;
        }
        pp = next;
    }

    psKillGeneratorID(familyId);
}

void psDeletePntJObjwithParticle(PSParticle* pp)
{
    if (pp->flags & PS_FLAG_ATTACH_CAMERA) {
        u32 slotIdx = (pp->flags >> 12) & 0x7;
        void* camSlot = lbl_80452DC8[slotIdx];

        if (camSlot != NULL) {
            fn_801A05EC(camSlot);
            lbl_80452DC8[slotIdx] = NULL;
        }
    }
}

void psSetPointJObjNodup(void* renderObj, s32 index)
{
    void** slot;
    s32 i;

    if (index < 0 || index > 8) {
        return;
    }

    i = 0;
    do {
        if (lbl_80452DC8[i] == renderObj) {
            fn_801A05EC(lbl_80452DC8[i]);
            lbl_80452DC8[i] = NULL;
        }
        i++;
    } while (i < 8);

    if (index != 0) {
        void* oldRenderObj;

        slot = lbl_80452DC8;
        slot += index;
        oldRenderObj = *--slot;
        if (oldRenderObj != NULL) {
            fn_801A05EC(oldRenderObj);
        }
        *slot = renderObj;

        if (renderObj != NULL) {
            u16* refCount = (u16*)((u8*)renderObj + 4);

            (*refCount)++;
            if (*refCount == 0xFFFF) {
                __assert(lbl_80273820, 0x5D, lbl_8027382C);
            }
        }
    }
}

void psSetPointJObj(s32 index, void* renderObj)
{
    void** slot;
    s32 i;

    if (index < 0 || index > 8) {
        return;
    }

    if (index != 0) {
        void* oldRenderObj;

        slot = lbl_80452DC8;
        slot += index;
        oldRenderObj = *--slot;
        if (oldRenderObj == renderObj) {
            return;
        }
        if (oldRenderObj != NULL) {
            fn_801A05EC(oldRenderObj);
        }
        *slot = renderObj;

        if (renderObj != NULL) {
            u16* refCount = (u16*)((u8*)renderObj + 4);

            (*refCount)++;
            if (*refCount == 0xFFFF) {
                __assert(lbl_80273820, 0x5D, lbl_8027382C);
            }
        }
    } else {
        slot = lbl_80452DC8;
        i = 0;
        while (i < 8) {
            if (*slot == renderObj) {
                fn_801A05EC(*slot);
                *slot = NULL;
            }
            i++;
            slot++;
        }
    }
}
