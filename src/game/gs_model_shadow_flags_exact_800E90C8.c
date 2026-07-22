#include "dolphin/types.h"

typedef struct GSmodel GSmodel;

void GSmodelClearShadowFlags(GSmodel* model, u32 flags)
{
    u32* modelFlags = (u32*)model;

    if ((flags & 1U) != 0U) {
        modelFlags[0] &= ~0x10000000U;
    }
    if ((flags & 2U) != 0U) {
        modelFlags[0] &= ~0x20000000U;
    }
    if ((flags & 4U) != 0U) {
        modelFlags[0] &= ~0x40000000U;
    }
}

void GSmodelSetShadowFlags(GSmodel* model, u32 flags)
{
    u32* modelFlags = (u32*)model;

    if ((flags & 1U) != 0U) {
        modelFlags[0] |= 0x10000000U;
    }
    if ((flags & 2U) != 0U) {
        modelFlags[0] |= 0x20000000U;
    }
    if ((flags & 4U) != 0U) {
        modelFlags[0] |= 0x40000000U;
    }
}
