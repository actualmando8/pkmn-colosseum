/**
 * @file wazaSequenceSys.c
 * @brief wazaSequenceSys: waza sequence system-level state, resource lifetime,
 * and effect (WazaEffect/WazaEffectTblEntry) helpers.
 *
 * Split from the former game/battle/battle_waza.c CodeCandidate bucket
 * (0x801D1470-0x801DE698); see config/GC6E01/splits.txt for the exact
 * address range of this translation unit. Shared typedefs and cross-TU
 * forward declarations live in include/game/battle/battle_waza_types.h.
 */

#define fn_801DA698 fn_801DA698_decl
#define fn_801DA5C4 fn_801DA5C4_decl
#define fn_801DA224 fn_801DA224_decl
#define fn_801DA448 fn_801DA448_decl
#define fn_801DA4E8 fn_801DA4E8_decl
#define fn_801DA74C fn_801DA74C_decl
#define fn_801DA7AC fn_801DA7AC_decl
#define fn_801DA9E8 fn_801DA9E8_decl
#define fn_801DAAAC fn_801DAAAC_decl
#define fn_801DABAC fn_801DABAC_decl
#define fn_801DB8FC fn_801DB8FC_decl
#define wazaSequenceSysResetAnimationExcept wazaSequenceSysResetAnimationExcept_decl
#include "game/battle/battle_waza_types.h"
#undef fn_801DA698
#undef fn_801DA5C4
#undef fn_801DA224
#undef fn_801DA448
#undef fn_801DA4E8
#undef fn_801DA74C
#undef fn_801DA7AC
#undef fn_801DA9E8
#undef fn_801DAAAC
#undef fn_801DABAC
#undef fn_801DB8FC
#undef wazaSequenceSysResetAnimationExcept


/**
 * fn_801D9E1C - Pokemon motion get complete. Returns obj->0x77 or 0 if NULL.
 * Address: 0x801D9E1C | Size: 0x18
 */
u8 fn_801D9E1C(void* obj) {
    if (obj != NULL) {
        return *(u8*)((u8*)obj + 0x77);
    }
    return 0;
}

/**
 * fn_801D9E34 - Pokemon motion cancel.
 * Address: 0x801D9E34 | Size: 0x58
 */
void fn_801D9E34(void* obj) {
    extern void fn_80118A68();

    if (*(u32*)(lbl_80467CC0 + 0xC) != 0 && *(u32*)((u8*)obj + 0x84) != 0) {
        fn_80118A68(*(u32*)((u8*)obj + 0x84), 1);
        *(u32*)((u8*)obj + 0x84) = 0;
    }
}

/* =========================================================================
 * WAZA EFFECT HELPERS (0x801D9E8C - 0x801DAC78)
 * ========================================================================= */


#define WAZA_SDATA2_FLOAT(addr) extern const f32 lbl_##addr
#define WAZA_SDATA2_VALUE(addr) lbl_##addr
WAZA_SDATA2_FLOAT(8047E388);
WAZA_SDATA2_FLOAT(8047E38C);
WAZA_SDATA2_FLOAT(8047E390);
WAZA_SDATA2_FLOAT(8047E394);
WAZA_SDATA2_FLOAT(8047E398);
WAZA_SDATA2_FLOAT(8047E39C);
#undef WAZA_SDATA2_FLOAT


/**
 * fn_801D9E8C - Waza effect interpolation (position lerp).
 * Address: 0x801D9E8C | Size: 0x188
 */
void fn_801D9E8C(void* effect) {
    WazaEffect* fx;
    void* target;
    u32 ctx;
    f32 val;
    s32 scale_selector;
    u8 scale_buf[4];

    if (effect == NULL) return;
    fx = (WazaEffect*)effect;
    ctx = *(s32*)(lbl_80467CC0 + 0xC);
    if (ctx == 0) return;
    if (fx->effect_handle != 0) return;

    fx->effect_handle = fn_801190DC(ctx, 0, 0);
    if (fx->effect_handle == 0) return;

    if ((fx->flags & 1) != 1) {
        fn_80118C88(fx->effect_handle, 0);
    }

    {
        s32 val_idx = *(s32*)(fx->table_bytes + fx->index * sizeof(WazaEffectTblEntry) + 0x50);
        if (val_idx > 0) {
            target = GSmodelGetPart(fx->handle, val_idx);
        } else {
            target = NULL;
        }
    }

    if (target == NULL) return;

    if (effect != NULL) {
        scale_selector = fx->scale_selector;
    } else {
        scale_selector = 0;
    }

    switch (scale_selector) {
    case -2: val = WAZA_SDATA2_VALUE(8047E388); break;
    case -1: val = WAZA_SDATA2_VALUE(8047E38C); break;
    case 1:  val = WAZA_SDATA2_VALUE(8047E390); break;
    case 2:  val = WAZA_SDATA2_VALUE(8047E394); break;
    case 3:  val = WAZA_SDATA2_VALUE(8047E398); break;
    default: val = WAZA_SDATA2_VALUE(8047E39C); break;
    }

    set__5GSvecFfff(scale_buf, val, val, val);

    fn_80118FB0(fx->effect_handle, (s32)target, 4, 0, 1, 0);
    fn_80118D18(fx->effect_handle, 1);
    fn_80118DE0(fx->effect_handle, scale_buf, 1, 0);
    GSpartFree((s32)target);
}
#undef WAZA_SDATA2_VALUE

/**
 * fn_801DA014 - Waza effect timer cancel.
 * Address: 0x801DA014 | Size: 0x5C
 */
#pragma dont_inline on
void fn_801DA014(void* effect) {
    extern void fn_80118A68();

    if (effect != NULL && *(u32*)(lbl_80467CC0 + 0x8) != 0 && *(u32*)((u8*)effect + 0x80) != 0) {
        fn_80118A68(*(u32*)((u8*)effect + 0x80), 1);
        *(u32*)((u8*)effect + 0x80) = 0;
    }
}
#pragma dont_inline reset

/**
 * fn_801DA070 - Waza effect bezier curve eval.
 * Address: 0x801DA070 | Size: 0x1B4
 */
#define WAZA_SDATA2_VALUE(addr) lbl_##addr
void fn_801DA070(void* effect) {
    WazaEffect* fx;
    void* target;
    void* entry;
    u32 ctx;
    s32 n;
    f32 val;
    s32 scale_selector;
    u8 scale_buf[4];

    fx = (WazaEffect*)effect;
    if (fx == NULL) return;
    ctx = *(u32*)(lbl_80467CC0 + 0x8);
    if (ctx == 0) return;
    if (fx->field_80 != 0) return;

    fx->field_80 = fn_801190DC(ctx, 0, 0);
    if (fx->field_80 == 0) return;

    if ((fx->flags & 1) != 1) {
        fn_80118C88(fx->field_80, 0);
    }

    entry = fx->table_bytes + fx->index * sizeof(WazaEffectTblEntry);
    if ((u8)GSmodelCenterNull(fx->handle) != 0) {
        n = fn_800EE0E8(fx->handle) - 1;
    } else {
        n = *(s32*)((u8*)entry + 0x54);
    }

    if (n > 0) {
        target = GSmodelGetPart(fx->handle, n);
    } else {
        target = NULL;
    }

    if (target == NULL) return;

    if (effect != NULL) {
        scale_selector = fx->scale_selector;
    } else {
        scale_selector = 0;
    }

    switch (scale_selector) {
    case -2: val = WAZA_SDATA2_VALUE(8047E388); break;
    case -1: val = WAZA_SDATA2_VALUE(8047E38C); break;
    case 1:  val = WAZA_SDATA2_VALUE(8047E390); break;
    case 2:  val = WAZA_SDATA2_VALUE(8047E394); break;
    case 3:  val = WAZA_SDATA2_VALUE(8047E398); break;
    default: val = WAZA_SDATA2_VALUE(8047E39C); break;
    }

    set__5GSvecFfff(scale_buf, val, val, val);

    fn_80118FB0(fx->field_80, (s32)target, 4, 0, 1, 0);
    fn_80118D18(fx->field_80, 1);
    fn_80118CAC(fx->field_80, 1);
    fn_80118DE0(fx->field_80, scale_buf, 1, 0);
    GSpartFree((s32)target);
}
#undef WAZA_SDATA2_VALUE

/**
 * fn_801DA224 - Apply effect state flags.
 * Address: 0x801DA224 | Size: 0xA0
 */
void fn_801DA224(void* effect, s32 flags) {
    extern void fn_801DCF00();

    if (effect != NULL) {
        u8 value = flags;

        if ((flags & 4) == 4) {
            fn_801DD078(effect);
        } else {
            fn_801DD028(effect);
        }

        if ((value & 8) == 8) {
            fn_801DCFD8(effect);
        } else {
            fn_801DCF84(effect);
        }

        if ((value & 2) == 2) {
            fn_801DCF00(effect);
        } else {
            fn_801DCEA8(effect);
        }

        *(u8*)((u8*)effect + 0x18) = flags;
    }
}

/**
 * fn_801DA2C4 - Reset an effect's trajectory and make its resources visible.
 * Address: 0x801DA2C4 | Size: 0x90
 */
void fn_801DA2C4(void* effect, f32 radius, f32 t) {
    WazaEffect* fx = effect;
    u32 handle;

    if (fx != NULL) {
        fn_801DD028(fx);
        fn_801DCF84(fx);
        fn_801DCEA8(fx);

        if (fx != NULL && (handle = fx->handle) != 0) {
            fx->flags |= 1;
            GSmodelSetVisibility(handle, 1);
            if (fx->field_80 != 0) {
                fn_80118C88(fx->field_80, 1);
            }
            if (fx->effect_handle != 0) {
                fn_80118C88(fx->effect_handle, 1);
            }
        }
    }
}

/**
 * fn_801DA354 - Waza effect get trajectory type. Returns effect->0x18 or 0 if NULL.
 * Address: 0x801DA354 | Size: 0x18
 */
u8 fn_801DA354(void* effect) {
    if (effect != NULL) {
        return *(u8*)((u8*)effect + 0x18);
    }
    return 0;
}

/**
 * fn_801DA36C - Waza effect set trajectory type.
 * Address: 0x801DA36C | Size: 0x60
 */
void fn_801DA36C(void* effect, s32 trajType) {
    extern void fn_801DCEA8();
    extern void fn_801DCF84();
    extern void fn_801DD028();

    if (effect != NULL) {
        switch ((u8)trajType) {
        case 1:
            fn_801DD028(effect);
            return;
        case 2:
            fn_801DCF84(effect);
            return;
        case 3:
            fn_801DCEA8(effect);
            return;
        }
    }
}

/**
 * fn_801DA3CC - Waza effect set velocity.
 * Address: 0x801DA3CC | Size: 0x60
 */
void fn_801DA3CC(void* effect, s32 trajType) {
    extern void fn_801DCF00();
    extern void fn_801DCFD8();
    extern void fn_801DD078();

    if (effect != NULL) {
        switch ((u8)trajType) {
        case 1:
            fn_801DD078(effect);
            return;
        case 2:
            fn_801DCFD8(effect);
            return;
        case 3:
            fn_801DCF00(effect);
            return;
        }
    }
}

/**
 * fn_801DA42C - Waza effect get bit 0 of field 0x18.
 * Address: 0x801DA42C | Size: 0x1C
 */
u32 fn_801DA42C(void* effect) {
    if (effect != NULL) {
        return *(u8*)((u8*)effect + 0x18) & 1;
    }
    return 0;
}

/**
 * fn_801DA448 - Set visibility on particle banks attached to an effect.
 * Address: 0x801DA448 | Size: 0xA0
 */
void fn_801DA448(void* effect, u32 visible) {
    extern void fn_80118C20();
    u8* sequence;
    u8* model;

    if (effect != NULL) {
        sequence = *(u8**)((u8*)effect + 0x68);
        while (sequence != NULL) {
            if (*(u8*)(sequence + 0x14) != 0) {
                model = *(u8**)(sequence + 0x24);
                while (model != NULL) {
                    if (*(s32*)(model + 4) == 3 && *(s32*)(model + 0x18) == 0 &&
                        *(void**)(model + 0x88) != NULL) {
                        fn_80118C20(*(void**)(model + 0x88), visible);
                    }
                    model = *(u8**)(model + 0xA8);
                }
            }
            sequence = *(u8**)(sequence + 0x34);
        }
    }
}

/**
 * fn_801DA4E8 - Set visibility on a waza effect and its attached effects.
 * Address: 0x801DA4E8 | Size: 0xC4
 */
void fn_801DA4E8(void* effect, u32 visible) {
    WazaEffect* fx = effect;
    u32 handle;

    if (fx != NULL && (handle = fx->handle) != 0) {
        if ((u8)visible != 0) {
            fx->flags |= 1;
            GSmodelSetVisibility(handle, 1);
            if (fx->field_80 != 0) {
                fn_80118C88(fx->field_80, 1);
            }
            if (fx->effect_handle != 0) {
                fn_80118C88(fx->effect_handle, 1);
            }
        } else {
            u8 flags = fx->flags;

            if ((flags & 1) != 0) {
                fx->flags = flags ^ 1;
            }
            GSmodelSetVisibility(handle, 0);
            if (fx->field_80 != 0) {
                fn_80118C88(fx->field_80, 0);
            }
            if (fx->effect_handle != 0) {
                fn_80118C88(fx->effect_handle, 0);
            }
        }
    }
}

/**
 * fn_801DA5AC - Waza effect set fields 0x16=0, 0x17=r4.
 * Address: 0x801DA5AC | Size: 0x18
 */
void fn_801DA5AC(void* effect, u8 val) {
    if (effect == NULL) return;
    *(u8*)((u8*)effect + 0x16) = 0;
    *(u8*)((u8*)effect + 0x17) = val;
}

/**
 * fn_801DA5C4 - Check whether every active effect sequence has finished.
 * Address: 0x801DA5C4 | Size: 0xD4
 */
u8 fn_801DA5C4(s32 timeType) {
    extern s32 wazaSequenceSysGetWazaTime();
    u8* pool = lbl_80467CC0;
    WazaEffect* effect;
    s32 i = 0;
    s32 count;
    void* waza;

    effect = *(WazaEffect**)pool;
    count = *(u16*)(pool + 4);
    for (; i < count; i++, effect++) {
        if (effect->active != 0) {
            waza = *(void**)((u8*)effect + 0x6C);
            if (waza != NULL) {
                if (waza == NULL) {
                    return TRUE;
                }

                if ((u8)timeType == 6) {
                    if (*(u8*)((u8*)waza + 0x14) != 0 && *(s8*)((u8*)waza + 0x15) != -1) {
                        return FALSE;
                    }
                    return TRUE;
                }

                return *(s32*)waza >=
                       wazaSequenceSysGetWazaTime(*(void**)((u8*)waza + 0x3C), waza, timeType);
            }
        }
    }
    return TRUE;
}

/**
 * fn_801DA698 - Waza effect tick lifetime.
 * Address: 0x801DA698 | Size: 0xB4
 */
u8 fn_801DA698(void* sequence, s32 moveID, s32 variant, s32 timeType) {
    extern s32 wazaSequenceSysGetWazaTime();
    void* waza;

    if (sequence == NULL) {
        return FALSE;
    }

    waza = GetWaza__12NullSequenceCFUsUs(sequence, moveID, variant);
    if (waza == NULL) {
        return TRUE;
    }

    if ((u8)timeType == 6) {
        if (*(u8*)((u8*)waza + 0x14) != 0 && *(s8*)((u8*)waza + 0x15) != -1) {
            return FALSE;
        }
        return TRUE;
    }

    return *(s32*)waza >= wazaSequenceSysGetWazaTime(*(void**)((u8*)waza + 0x3C), waza, timeType);
}

/**
 * fn_801DA74C - Get the time for a move's sequence entry.
 * Address: 0x801DA74C | Size: 0x60
 */
s32 fn_801DA74C(void* sequence, s32 moveID, s32 variant, s32 timeType) {
    extern s32 wazaSequenceSysGetWazaTime();
    void* waza;

    if (sequence == NULL) {
        return 0;
    }

    waza = GetWaza__12NullSequenceCFUsUs(sequence, moveID, variant);
    if (waza != NULL) {
        return wazaSequenceSysGetWazaTime(sequence, waza, timeType);
    }
    return 0;
}

/**
 * fn_801DA7AC - Clear child sequences from every active effect.
 * Address: 0x801DA7AC | Size: 0x90
 */
void fn_801DA7AC(void) {
    extern void wazaSequenceApplyStop(void* obj);
    extern void wazaSequenceFree(void* obj);
    u8* pool;
    WazaEffect* effect;
    s32 i;
    s32 count;
    void* child;
    void* next;

    pool = lbl_80467CC0;
    effect = *(WazaEffect**)pool;
    count = *(u16*)(pool + 4);
    for (i = 0; i < count; i++, effect++) {
        if (effect->active != 0) {
            child = *(void**)((u8*)effect + 0x68);
            while (child != NULL) {
                next = *(void**)((u8*)child + 0x34);
                if (*(u8*)((u8*)child + 0x14) != 0) {
                    wazaSequenceApplyStop(child);
                }
                wazaSequenceFree(child);
                child = next;
            }
            *(void**)((u8*)effect + 0x68) = NULL;
        }
    }
}

/**
 * fn_801DA83C - Waza effect pool free.
 * Address: 0x801DA83C | Size: 0x88
 */
void fn_801DA83C(void* effect) {
    extern void wazaSequenceApplyStop(void* obj);
    extern void wazaSequenceFree(void* obj);

    void* cur;
    void* next;

    if (effect != NULL) {
        cur = *(void**)((u8*)effect + 0x68);
        if (*(u8*)((u8*)effect + 0x74) == 0) {
            return;
        }
        while (cur != NULL) {
            next = *(void**)((u8*)cur + 0x34);
            if (*(u8*)((u8*)cur + 0x14) != 0) {
                wazaSequenceApplyStop(cur);
            }
            wazaSequenceFree(cur);
            cur = next;
        }
        *(void**)((u8*)effect + 0x68) = NULL;
    }
}

/**
 * fn_801DA8C4 - Waza resolve/update helper.
 * Address: 0x801DA8C4 | Size: 0x50
 */
extern void* GetWaza__12NullSequenceCFUsUs();
extern void wazaSequenceApplyStop(void* obj);
extern void wazaSequenceFree(void* obj);
void fn_801DA8C4(void* obj) {
    void* resolved;
    if (obj != NULL) {
        resolved = GetWaza__12NullSequenceCFUsUs(obj);
        if (resolved != NULL) {
            if (*(u8*)((u8*)resolved + 0x14) != 0) {
                wazaSequenceApplyStop(resolved);
            }
            wazaSequenceFree(resolved);
        }
    }
}

/**
 * fn_801DA914 - Waza effect pool get used count.
 * Address: 0x801DA914 | Size: 0x38
 */
void fn_801DA914(void* obj) {
    void* resolved;
    if (obj != NULL) {
        resolved = GetWaza__12NullSequenceCFUsUs(obj);
        if (resolved != NULL) {
            *(u8*)((u8*)resolved + 0x16) = 0;
        }
    }
}

/**
 * fn_801DA94C - Waza resolved effect has active non-minus-one byte.
 * Address: 0x801DA94C | Size: 0x68
 */
s32 fn_801DA94C(void* obj) {
    void* resolved;
    u8 result;
    if (obj == NULL) {
        return 0;
    }
    resolved = GetWaza__12NullSequenceCFUsUs(obj);
    if (resolved != NULL) {
        result = 0;
        if (*(u8*)((u8*)resolved + 0x14) != 0) {
            if (*(s8*)((u8*)resolved + 0x15) != -1) {
                result = 1;
            }
        }
        return result;
    }
    return 0;
}

/**
 * fn_801DA9B4 - Waza effect pool clear all.
 * Address: 0x801DA9B4 | Size: 0x34
 */
void fn_801DA9B4(void* obj) {
    void* resolved;
    if (obj != NULL) {
        resolved = GetWaza__12NullSequenceCFUsUs(obj);
        if (resolved != NULL) {
            wazaSequenceApplyStop(resolved);
        }
    }
}

/**
 * fn_801DA9E8 - Waza sound effect play.
 * Address: 0x801DA9E8 | Size: 0xC4
 */
void fn_801DA9E8(void* sequence, s32 moveID, s32 variant) {
    u8* pool = lbl_80467CC0;
    WazaEffect* entry;
    s32 i = 0;
    int count;
    void* waza;

    entry = *(WazaEffect**)pool;
    count = *(u16*)(pool + 4);
    for (; i < count; i++, entry++) {
        if (entry->active != 0) {
            waza = GetWaza__12NullSequenceCFUsUs(entry, moveID, variant);
            if (waza != NULL && *(u8*)((u8*)waza + 0x14) != 0) {
                wazaSequenceApplyStop(waza);
            }
        }
    }

    if (sequence != NULL) {
        waza = GetWaza__12NullSequenceCFUsUs(sequence, moveID, variant);
        if (sequence != NULL) {
            *(u8*)((u8*)sequence + 0x16) = 0;
            *(u8*)((u8*)sequence + 0x17) = 0;
        }
        if (waza != NULL) {
            wazaSequenceStart((s32)waza);
        }
    }
}

/**
 * fn_801DAAAC - Draw every model layer attached to a waza effect.
 * Address: 0x801DAAAC | Size: 0x100
 */
void fn_801DAAAC(void* effectPtr) {
    extern void GSmodelForceAnimTransformUpdate(void* model);
    extern u32 fn_800E3CBC(void* model);
    extern void GSmodelDrawModel(void* model, u32 flags);
    extern void fn_801DB8FC();
    extern void* GScameraGetActiveCamera(void);
    extern s32 HSD_CObjSetCurrent(void* cobj);
    extern void fn_80118104();
    extern void fn_80195A48(void);
    u8* effect = effectPtr;
    u32 flags;
    u8* entry;
    u32 modelID;
    s32 modelGroup;
    s32 pass;
    void* camera;

    if (effect != NULL) {
        GSmodelForceAnimTransformUpdate(*(void**)(effect + 0x24));
        for (modelGroup = 0; modelGroup < 2; modelGroup++) {
            modelID = modelGroup != 0;
            for (pass = 0; pass < 3; pass++) {
                if (pass == 0) {
                    flags = 0x10;
                } else if (pass == 1) {
                    flags = 0x1000;
                } else {
                    flags = 0x2000;
                }

                if (modelID == (u8)fn_800E3CBC(*(void**)(effect + 0x24))) {
                    GSmodelDrawModel(*(void**)(effect + 0x24), flags);
                }

                entry = *(u8**)(effect + 0x68);
                while (entry != NULL) {
                    if (*(u8*)(entry + 0x14) != 0) {
                        fn_801DB8FC(entry, flags, modelID);
                    }
                    entry = *(u8**)(entry + 0x34);
                }

                camera = GScameraGetActiveCamera();
                if (camera != NULL && HSD_CObjSetCurrent(*(void**)((u8*)camera + 0xC))) {
                    fn_80118104(flags, modelID);
                    fn_80195A48();
                }
            }
        }
    }
}

/**
 * fn_801DABAC - Get the scale for a waza selector.
 * Address: 0x801DABAC | Size: 0x78
 */
f32 fn_801DABAC(void* obj) {
    s32 selector;

    if (obj != NULL) {
        selector = *(s32*)((u8*)obj + 0x10);
    } else {
        selector = 0;
    }

    switch (selector) {
    case -2:
        return lbl_8047E388;
    case -1:
        return lbl_8047E38C;
    case 1:
        return lbl_8047E390;
    case 2:
        return lbl_8047E394;
    case 3:
        return lbl_8047E398;
    default:
        return lbl_8047E39C;
    }
}

/**
 * fn_801DAC24 - Waza get field 0x10.
 * Address: 0x801DAC24 | Size: 0x18
 */
u32 fn_801DAC24(void* obj) {
    if (obj != NULL) {
        return *(u32*)((u8*)obj + 0x10);
    }
    return 0;
}

/**
 * fn_801DAC3C - Waza get field 0x24.
 * Address: 0x801DAC3C | Size: 0x18
 */
u32 fn_801DAC3C(void* obj) {
    if (obj != NULL) {
        return *(u32*)((u8*)obj + 0x24);
    }
    return 0;
}

/**
 * fn_801DAC54 - Waza check field 0x6C nonzero.
 * Address: 0x801DAC54 | Size: 0x24
 */
u32 fn_801DAC54(void* obj) {
    if (obj == NULL) {
        return 0;
    }
    return *(u32*)((u8*)obj + 0x6C) != 0;
}

/**
 * fn_801DAC78 - Waza get field 0x70.
 * Address: 0x801DAC78 | Size: 0x18
 */
u16 fn_801DAC78(void* obj) {
    if (obj != NULL) {
        return *(u16*)((u8*)obj + 0x70);
    }
    return 0;
}

/* =========================================================================
 * WAZA SYSTEM LIFECYCLE (0x801DAC90 - 0x801DB100)
 *
 * System-level init/cleanup/reset functions.
 * Referenced by battle_main.c and battle_logic.c.
 * ========================================================================= */

/**
 * wazaSequenceSysRelease - Waza system cleanup.
 * Address: 0x801DAC90 | Size: 0x130
 * Referenced by battle_main.c (battle_FightEnd).
 * Stops all active waza effects, frees all allocated memory.
 */
void wazaSequenceSysRelease(void) {
    /* TODO: Waza system cleanup (0x130 bytes)
     * 1. Stops all active particle effects
     * 2. Removes all model JObjs
     * 3. Stops all sound effects
     * 4. Frees sequence data
     * 5. Clears the waza context
     */
}

/**
 * fn_801DADC0 - Waza system partial reset.
 * Address: 0x801DADC0 | Size: 0x138
 */
void fn_801DADC0(void) {
    /* TODO: Waza system partial reset (0x138 bytes) */
}

/**
 * fn_801DAEF8 - Waza system initialization.
 * Address: 0x801DAEF8 | Size: 0x168
 * Referenced by battle_main.c (battle_FightStart).
 * Allocates waza context and prepares the system for move animations.
 */
void fn_801DAEF8(s32 count) {
    /* TODO: Waza system init (0x168 bytes)
     * 1. Allocates waza context structure
     * 2. Initializes effect pool with 'count' entries
     * 3. Clears all sequence data
     * 4. Resets frame counters
     */
}

/**
 * wazaSequenceSysGetResID - Waza system get initialized.
 * Address: 0x801DB060 | Size: 0x28
 */
BOOL wazaSequenceSysGetResID(void) {
    if ((u32)(lbl_8047B410 + 0x10000) == 0xFFFF) {
        lbl_8047B410 = 0;
    }

    lbl_8047B410++;
    return lbl_8047B410;
}

/**
 * fn_801DB088 - Waza system reset.
 * Address: 0x801DB088 | Size: 0x78
 * Referenced by battle_main.c (battle_FightCleanup).
 */
void fn_801DB088(void)
{
    u8* pool = lbl_80467CC0;
    WazaEffect* entry;
    s32 i = 0;
    int count;

    entry = *((WazaEffect**) pool);
    count = *((u16*) (pool + 4));
    for (; i < count; i++, entry++) {
        if (entry->active != 0) {
            fn_801DD158(entry);
            fn_801DF1D0(entry);
        }
    }

    {
        void fn_801D2D28();
        fn_801D2D28();
    }
}

/**
 * fn_801DB100 - Waza system get context.
 * Address: 0x801DB100 | Size: 0x54
 */
void fn_801DB100(void* obj) {
    if (obj != NULL) {
        if (*(u8*)((u8*)obj + 0x74) != 0) {
            fn_801DD3E4(obj);
            fn_801DD23C(obj);
        }

        memset(obj, 0, 0x8C);
    }
}

/* =========================================================================
 * WAZA EXTENDED FUNCTIONS (0x801DB154 - 0x801E03D4)
 *
 * Extended waza/scene animation functions including the remaining
 * move effect handlers, transition effects, and rendering helpers.
 * ========================================================================= */

/**
 * fn_801DB154 - Waza sequence data lookup.
 * Address: 0x801DB154 | Size: 0x78
 */
void* fn_801DB154(void) {
    u8* pool = lbl_80467CC0;
    u8* entry = *(u8**)pool;
    u16 count = *(u16*)(pool + 4);
    s32 i;

    for (i = 0; i < count; i++, entry += 0x8C) {
        if (*(u8*)(entry + 0x74) == 0) {
            memset(entry, 0, 0x8C);
            *(u8*)(entry + 0x74) = 1;
            return entry;
        }
    }

    return NULL;
}

/**
 * wazaSequenceSysFreeSequenceResource - Waza sequence data validate.
 * Address: 0x801DB1CC | Size: 0xBC
 */
void wazaSequenceSysFreeSequenceResource(void* obj) {
    u16 id = *(u16*)((u8*)obj + 0x72);

    if (id != 0) {
        u8* pool = lbl_80467CC0;
        u8* entry = *(u8**)pool;
        u16 count = *(u16*)(pool + 4);
        s32 matches = 0;
        s32 i;

        for (i = 0; i < count; i++, entry += 0x8C) {
            if (*(u16*)(entry + 0x72) == id) {
                matches++;
            }
        }

        if (matches == 1) {
            while (fn_8017B2CC(id) == 1) {
                _threadSwitch();
            }

            fn_800F915C(id);
            fn_8017B1CC(id);
        }

        *(u16*)((u8*)obj + 0x72) = 0;
    }
}

/**
 * wazaSequenceSysFreeWazaResource - Waza sequence data parse.
 * Address: 0x801DB288 | Size: 0x170
 */
void wazaSequenceSysFreeWazaResource(void* seqData, s32 moveID) {
    /* TODO: Parse sequence data (0x170 bytes) */
}

/**
 * wazaSequenceSysGetWazaTime - Waza sequence data complex parse.
 * Address: 0x801DB3F8 | Size: 0x450
 */
void wazaSequenceSysGetWazaTime(void* seqData) {
    /* TODO: Complex sequence data parse (0x450 bytes) */
}

/**
 * wazaSequenceSysGetModelShadowLight__Fv - Waza data get move count.
 * Address: 0x801DB848 | Size: 0x8
 */
extern s32 lbl_8047B418;
s32 wazaSequenceSysGetModelShadowLight__Fv(void) {
    return lbl_8047B418;
}

/**
 * wazaSequenceSysGetModelShadowCount__Fv - Waza data get entry count for move.
 * Address: 0x801DB850 | Size: 0x8
 */
extern s32 lbl_8047B414;
s32 wazaSequenceSysGetModelShadowCount__Fv(s32 moveID) {
    return lbl_8047B414;
}

/**
 * wazaSequenceSysGetModelShadowList__Fv - Waza data get move flags.
 * Address: 0x801DB858 | Size: 0xC
 */
extern u8 lbl_80467C80[];
void* wazaSequenceSysGetModelShadowList__Fv(s32 moveID) {
    return lbl_80467C80;
}

/**
 * wazaSequenceSysResetAnimationExcept - Reset every active animation except one.
 * Address: 0x801DB864 | Size: 0x98
 */
void wazaSequenceSysResetAnimationExcept(WazaEffect* except) {
    u8* pool = lbl_80467CC0;
    WazaEffect* effect;
    s32 i = 0;
    int count;

    effect = *(WazaEffect**)pool;
    count = *(u16*)(pool + 4);
    for (; i < count; i++, effect++) {
        if (except != effect && effect->active != 0 && *(void**)((u8*)effect + 0x24) != NULL) {
            fn_801DEF0C(effect, 1, 0);
        }
    }
}

/**
 * fn_801DB8FC - Draw matching models from a sequence entry.
 * Address: 0x801DB8FC | Size: 0x8C
 */
void fn_801DB8FC(void* entry, u32 drawFlags, u8 modelID) {
    extern u32 fn_800E3CBC(void* model);
    extern void GSmodelDrawModel(void* model, u32 flags);
    u8* node;

    node = *(u8**)((u8*)entry + 0x24);
    while (node != NULL) {
        if (*(s32*)(node + 0x6C) == 1) {
            switch (*(s32*)(node + 4)) {
            case 2:
                if (modelID == (u8)fn_800E3CBC(*(void**)(node + 0xA4))) {
                    GSmodelDrawModel(*(void**)(node + 0xA4), drawFlags);
                }
                break;
            }
        }
        node = *(u8**)(node + 0xA8);
    }
}
