/**
 * @file wazaSequence.c
 * @brief wazaSequence: waza sequence core -- start/stop/load/update and entry
 * linking.
 *
 * Split from the former game/battle/battle_waza.c CodeCandidate bucket
 * (0x801D1470-0x801DE698); see config/GC6E01/splits.txt for the exact
 * address range of this translation unit. Shared typedefs and cross-TU
 * forward declarations live in include/game/battle/battle_waza_types.h.
 */

#include "game/battle/battle_waza_types.h"


/**
 * wazaSequenceUpdate - Waza rendering setup.
 * Address: 0x801DB988 | Size: 0x188
 */
void wazaSequenceUpdate(void) {
    /* TODO: Waza rendering setup (0x188 bytes) */
}

/**
 * wazaSequenceApplyStop - Waza rendering update.
 * Address: 0x801DBB10 | Size: 0x120
 */
void wazaSequenceApplyStop(void* obj) {
    extern void GSmodelLinkToGSparticleBank(s32, s32);
    extern void fn_800E3CC8(s32, s32);
    extern void fn_80118874();
    extern void battleGridResetModelVisibilityFlags(void);
    extern void fn_801D3034();
    extern void wazaSequenceEntryStop();
    extern void fn_801DEF0C(void*, s32, s32);

    u8* effect;
    u8* owner;
    void* node;

    effect = (u8*)obj;
    if (effect != NULL) {
        owner = *(u8**)(effect + 0x3C);
        if (*(u8*)(effect + 0x14) != 0) {
            if (*(u8*)(effect + 0x16) != 0) {
                fn_801D3034(owner);
            }
            if (*(u8*)(owner + 0x75) == 0) {
                fn_801DEF0C(owner, 1, 0);
            }
            fn_800E3CC8(*(s32*)(owner + 0x24), 0);
            if ((*(u32*)(effect + 0x8) & 0x08000000) != 0) {
                GSmodelLinkToGSparticleBank(*(s32*)(owner + 0x24), *(s32*)(owner + 0x28));
            }
            if ((*(u32*)(effect + 0x8) & 0x04000000) != 0) {
                battleGridResetModelVisibilityFlags();
            }
            node = *(void**)(effect + 0x24);
            while (node != NULL) {
                wazaSequenceEntryStop(node, 1);
                node = *(void**)((u8*)node + 0xA8);
            }
            node = *(void**)(effect + 0x24);
            while (node != NULL) {
                if (*(s32*)((u8*)node + 0x4) == 3 && *(s32*)((u8*)node + 0x18) == 0
                    && *(u32*)((u8*)node + 0x88) != 0) {
                    fn_80118874(*(u32*)((u8*)node + 0x88), 1);
                }
                node = *(void**)((u8*)node + 0xA8);
            }
            *(u32*)(owner + 0x6C) = 0;
            *(u8*)(effect + 0x14) = 0;
            *(u8*)(effect + 0x15) = 0;
        }
    }
}

/**
 * fn_801DBC30 - Waza rendering cleanup.
 * Address: 0x801DBC30 | Size: 0x9C
 */
void fn_801DBC30(void* obj) {
    extern void fn_800E3CC8(s32, s32);
    extern void fn_801D3034(void*);
    extern void fn_801DEF0C(void*, s32, s32);

    void* owner;
    s32 kind;

    if (obj != NULL) {
        owner = *(void**)((u8*)obj + 0x3C);
        if (*(u8*)((u8*)obj + 0x14) != 0 && *(void**)((u8*)owner + 0x6C) == obj) {
            kind = *(s32*)((u8*)obj + 0xC);
            if (kind >= 0xB || kind < 9) {
                fn_801DEF0C(owner, 1, 0);
            }
            if (*(u8*)((u8*)obj + 0x16) != 0) {
                fn_801D3034(owner);
            }
            fn_800E3CC8(*(s32*)((u8*)owner + 0x24), 0);
            *(u32*)((u8*)owner + 0x6C) = 0;
        }
    }
}

/**
 * wazaSequenceStart - Waza blend effect setup.
 * Address: 0x801DBCCC | Size: 0x110
 */
void wazaSequenceStart(s32 blendType) {
    extern void wazaSequenceApplyStop();
    extern void wazaSequenceSysResetAnimationExcept();
    extern void fn_801DD100();
    extern void GSmodelLinkToGSparticleBank(s32, s32);
    extern void wazaSequencePokemonMotionStart();
    extern void battleGridHideModelsExcept();
    extern void battleCameraStartWaza();
    extern void wazaSequenceUpdate();

    u8* obj;
    u8* owner;
    u8* node;
    void* current;
    s32 bit;
    s32 handle;
    u32 flags;

    obj = (u8*)blendType;
    if (*(u8*)(obj + 0x14) == 0) {
        owner = *(u8**)(obj + 0x3C);
        flags = *(u32*)(obj + 0x08);
        current = *(void**)(owner + 0x6C);
        flags = (flags >> 1) & 1;
        node = *(u8**)(obj + 0x24);
        handle = *(s32*)(owner + 0x24);
        bit = flags;
        if (current != NULL) {
            wazaSequenceApplyStop(current);
        }
        if (*(u8*)(owner + 0x75) != 0 && *(u16*)(obj + 0x2E) == 2) {
            wazaSequenceSysResetAnimationExcept(owner);
        }
        fn_801DD100(owner, obj);
        if ((*(u32*)(obj + 0x08) & 0x08000000) != 0) {
            GSmodelLinkToGSparticleBank(handle, 0);
        }
        wazaSequencePokemonMotionStart(owner, bit);
        *(u8**)(owner + 0x6C) = obj;
        *(u8*)(obj + 0x14) = 1;
        if ((*(u32*)(obj + 0x08) & 0x04000000) != 0) {
            battleGridHideModelsExcept(owner);
        }
        if (*(u8*)(obj + 0x16) != 0) {
            battleCameraStartWaza(owner, obj);
        }
        while (node != NULL) {
            *(u32*)(node + 0x6C) = 0;
            node = *(u8**)(node + 0xA8);
        }
        *(u32*)(obj + 0x00) = 0;
        *(u8*)(obj + 0x15) = 0;
        wazaSequenceUpdate(obj);
    }
}

/**
 * wazaSequenceFree - Waza blend effect update.
 * Address: 0x801DBDDC | Size: 0x1D4
 */
void wazaSequenceFree(void* obj) {
    /* TODO: Blend effect update (0x1D4 bytes) */
}

/**
 * fn_801DBFB0 - Waza blend effect get state.
 * Address: 0x801DBFB0 | Size: 0x64
 */
s32 fn_801DBFB0(void) {
    extern u16 _toolentryAlloc__FUl(u32 size);
    extern void* fn_800E27B0(u16 handle);

    u16 handle;
    void* obj;

    handle = _toolentryAlloc__FUl(0x40);
    if (handle != 0) {
        obj = fn_800E27B0(handle);
        memset(obj, 0, 0x40);
        *(u16*)((u8*)obj + 0x2A) = handle;
        return (s32)obj;
    }
    return 0;
}

/**
 * wazaSequenceLoadData - Waza screen distortion effect.
 * Address: 0x801DC014 | Size: 0x2FC
 */
void wazaSequenceLoadData(s32 distortType, f32 intensity) {
    /* TODO: Screen distortion effect (0x2FC bytes) */
}

/**
 * wazaSequenceEntryLink - Waza screen distortion update.
 * Address: 0x801DC310 | Size: 0x15C
 */
void wazaSequenceEntryLink(void) {
    /* TODO: Screen distortion update (0x15C bytes) */
}

/**
 * fn_801DC46C - Waza screen overlay effect.
 * Address: 0x801DC46C | Size: 0x184
 */
void fn_801DC46C(s32 overlayType, u32 color) {
    /* TODO: Screen overlay effect (0x184 bytes) */
}

/**
 * fn_801DC5F0 - Waza screen overlay update.
 * Address: 0x801DC5F0 | Size: 0x22C
 */
void fn_801DC5F0(void) {
    /* TODO: Screen overlay update (0x22C bytes) */
}

/**
 * _wazaSequenceEffectEntryLoad - Waza screen effect composite.
 * Address: 0x801DC81C | Size: 0x284
 */
void _wazaSequenceEffectEntryLoad(void) {
    /* TODO: Screen effect composite (0x284 bytes) */
}

/**
 * _wazaSequenceParticleEntryLoad - Waza screen effect finalize.
 * Address: 0x801DCAA0 | Size: 0x128
 */
void _wazaSequenceParticleEntryLoad(void) {
    /* TODO: Screen effect finalize (0x128 bytes) */
}

/**
 * _wazaSequenceModelEntryLoad - Waza field effect handler.
 * Address: 0x801DCBC8 | Size: 0x1E0
 */
void _wazaSequenceModelEntryLoad(s32 fieldEffect) {
    /* TODO: Field effect handler (0x1E0 bytes)
     * Handles field-wide effects like weather, terrain changes.
     */
}
