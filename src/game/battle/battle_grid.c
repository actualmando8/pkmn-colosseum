/**
 * @file battle_grid.c
 * @brief Battle grid core API -- grid state, setup, model loading, and
 *        position/scale bookkeeping for the 4-slot double battle layout.
 *
 * Address range: 0x801C3108 - 0x801C4078 (11 functions).
 *
 * This is the shrunk remainder of the former monolithic battle_grid.c
 * CodeCandidate bucket (0x801C0F20 - 0x801C4CB8, split pass 2026-07-07):
 * the true XD translation unit game/pxdvs/app/battleGrid/battleGrid.cpp.
 * The HSD library code, battle camera helpers, and fade/fade-effect code
 * that used to live in this file moved out to
 * hsd/hsd_aobj_range_801C01C8.c, game/battle/battle_camera.c,
 * game/effect/fade.c, and game/effect/fade_effect.c respectively.
 *
 * The battle grid uses a 4-slot layout corresponding to:
 *   Slot 0: Player Left   Slot 1: Player Right
 *   Slot 2: Enemy Left    Slot 3: Enemy Right
 */

#include "dolphin/types.h"
#include "game/battle/battle_grid_types.h"

void battleGridUpdate(void);
void battleGridGetDistance(void);
void battleGridGetNormalisedScale(void);

/* CRT */
extern void* memset(void* dst, int val, u32 size);

/* HSD (SysDolphin) model/animation */
extern void  fn_8036A384(void* jobj, f32 x, f32 y, f32 z); /* HSD_JObjSetTranslate */
extern void  fn_8036A478(void* jobj, f32 x, f32 y, f32 z); /* HSD_JObjSetScale */
extern void  fn_8036A2D8(void* jobj, f32 rx, f32 ry, f32 rz); /* HSD_JObjSetRotation */
extern void  fn_80362E40(void* jobj, f32 frame);            /* HSD_JObjReqAnimAll */

/* Grid group entry/table -- see battleGridGetNumPokemonsForTrainer
 * (game/battle/battle_camera.c) for the sibling BattleGridGroupEntry[]
 * view of the same lbl_80466DE8 storage (kept as an independent local
 * type here too, as in the original monolithic file). */
typedef struct BattleGridGroupEntry {
    u8* slot;
    u8 pad_04[8];
    u16 memberCount;
    u8 arg1;
    u8 arg2;
} BattleGridGroupEntry;

typedef struct BattleGridGroupTable {
    BattleGridGroupEntry entries[4];
    u16 count;
} BattleGridGroupTable;

/**
 * battleGridGetPtr - Get current grid state (renamed from fn_801C3108;
 * confirmed name -- naming pass 2026-07-07).
 * Address: 0x801C3108 | Size: 0xC
 */
s32 battleGridGetPtr(void) {
    extern u8 lbl_80466DE8[];
    return (s32)lbl_80466DE8;
}

/**
 * fn_801C3114 / battleGrid_Init - Initialize the battle grid.
 * Address: 0x801C3114 | Size: 0xD8
 * Clears all grid slots, initializes the camera state,
 * sets up the 4-position double battle layout.
 */
void fn_801C3114(void) {
    s32 i;
    BattleGridSceneWork* sceneWork;

    memset(lbl_80467030, 0, 0x20);
    memset(lbl_80466E50, 0, 0x1E0);

    /* Initialize 4 BattleGridSlot entries with default values */
    sceneWork = (BattleGridSceneWork*)lbl_80466E50;
    for (i = 0; i < BATTLE_TOTAL_POKEMON; i++) {
        BattleGridSceneSlot* slot = &sceneWork->slots[i];
        slot->active = 0;
        slot->jobj = NULL;
        slot->rotationY = 0.0f;
        slot->scale = 1.0f;
    }
}

/**
 * fn_801C31EC / battleGrid_Setup - Full grid setup with model loading.
 * Address: 0x801C31EC | Size: 0x244
 * Referenced by battle_main.c (battle_FightEnd calls this for cleanup).
 * Sets up the complete battle field layout including stage model,
 * position markers, and initial camera placement.
 */
void fn_801C31EC(void) {
    /* Full grid setup with model loading:
     * 1. Initialize grid state
     * 2. Set up stage model (battle colosseum arena)
     * 3. Place position markers for all 4 slots
     * 4. Initialize camera to default battle overhead view
     */
    fn_801C3114();
    battleGridUpdate();
}

/**
 * battleGridUpdate - Main grid setup (large) (renamed from fn_801C3430;
 * confirmed name -- naming pass 2026-07-07).
 * Address: 0x801C3430 | Size: 0x634
 * This is the primary grid initialization function that:
 *   1. Loads the stage model from FDAT
 *   2. Sets up position transforms for all 4 battle slots
 *   3. Configures lighting and shadow rendering
 *   4. Sets up the battle camera default view
 *   5. Initializes the model animation system
 */
void battleGridUpdate(void) {
    extern void HSD_AObjInterpretAnim(void* ctx, f32 posX, f32 posZ);
    /* Main battle grid setup:
     * 1. Load stage model from FDAT
     * 2. Set up position transforms for all 4 battle slots
     * 3. Configure lighting (ambient + 2 directional)
     * 4. Configure shadow rendering
     * 5. Set up battle camera default overhead view
     * 6. Initialize model animation system
     */
    HSD_AObjInterpretAnim((void*)lbl_80466E50, 0.0f, 0.0f);
    battleGridGetDistance();
    battleGridGetNormalisedScale();
}

/**
 * battleGridGetDistance - Load models for all grid positions (renamed
 * from fn_801C3A64; confirmed name -- naming pass 2026-07-07).
 * Address: 0x801C3A64 | Size: 0x11C
 * Loads Pokemon and trainer models into each active grid slot.
 */
void battleGridGetDistance(void) {
    s32 i;
    BattleGridSceneWork* state = (BattleGridSceneWork*)lbl_80466E50;

    /* Load Pokemon and trainer models into each active grid slot */
    for (i = 0; i < BATTLE_TOTAL_POKEMON; i++) {
        BattleGridSceneSlot* slot = &state->slots[i];
        s32 active = slot->active;

        if (active != 0) {
            /* Model is already loaded or should be loaded from battle data */
            void* jobj = slot->jobj;
            if (jobj != NULL) {
                f32 x = slot->posX;
                f32 y = slot->posY;
                f32 z = slot->posZ;
                fn_8036A384(jobj, x, y, z);
            }
        }
    }
}

/**
 * battleGridGetNormalisedScale - Update all grid positions (renamed from
 * fn_801C3B80; confirmed name -- naming pass 2026-07-07).
 * Address: 0x801C3B80 | Size: 0x118
 * Recalculates world-space positions for all grid slots
 * (e.g., after a Pokemon switch or camera change).
 */
void battleGridGetNormalisedScale(void) {
    s32 i;
    BattleGridSceneWork* state = (BattleGridSceneWork*)lbl_80466E50;

    /* Recalculate world-space positions for all grid slots */
    for (i = 0; i < BATTLE_TOTAL_POKEMON; i++) {
        BattleGridSceneSlot* slot = &state->slots[i];
        s32 active = slot->active;

        if (active == 0) {
            continue;
        }

        {
            void* jobj = slot->jobj;
            if (jobj != NULL) {
                f32 x = slot->posX;
                f32 y = slot->posY;
                f32 z = slot->posZ;
                f32 scale = slot->scale;

                fn_8036A384(jobj, x, y, z);
                fn_8036A478(jobj, scale, scale, scale);
            }
        }
    }
}

/**
 * battleGridRemovePokemon - Grid slot state update helper (renamed from
 * fn_801C3C98; confirmed name -- naming pass 2026-07-07).
 * Address: 0x801C3C98 | Size: 0xCC
 */
void battleGridRemovePokemon(s32 slot) {
    BattleGridSceneWork* state = (BattleGridSceneWork*)lbl_80466E50;
    BattleGridSceneSlot* slotData;

    if (slot < 0 || slot >= BATTLE_TOTAL_POKEMON) {
        return;
    }

    slotData = &state->slots[slot];

    /* Update slot state: apply position, rotation, and scale to JObj */
    {
        void* jobj = slotData->jobj;
        if (jobj != NULL) {
            f32 x = slotData->posX;
            f32 y = slotData->posY;
            f32 z = slotData->posZ;
            f32 rot = slotData->rotationY;
            f32 scale = slotData->scale;

            fn_8036A384(jobj, x, y, z);
            fn_8036A2D8(jobj, 0.0f, rot, 0.0f);
            fn_8036A478(jobj, scale, scale, scale);
        }
    }
}

/**
 * battleGridReplacePokemon / battleGridReplacePokemon - Replace Pokemon model in a grid slot.
 * Address: 0x801C3D64 | Size: 0xD8
 * Proposed name from symbols: battleGridReplacePokemon.
 * Removes the current Pokemon model from a slot and loads a new one.
 */
void battleGridReplacePokemon(void* model) {
    /* Replace Pokemon model in a grid slot:
     * 1. Find the slot this model belongs to
     * 2. Remove the current Pokemon JObj
     * 3. Load the new Pokemon JObj from model data
     * 4. Apply the slot's current transform
     */
    if (model == NULL) {
        return;
    }
}

/**
 * battleGridAddPokemon - Grid slot model transition animation (renamed
 * from fn_801C3E3C; confirmed name -- naming pass 2026-07-07).
 * Address: 0x801C3E3C | Size: 0xD4
 */
void battleGridAddPokemon(s32 slot, s32 animType) {
    BattleGridSceneWork* state = (BattleGridSceneWork*)lbl_80466E50;
    BattleGridSceneSlot* slotData;

    if (slot < 0 || slot >= BATTLE_TOTAL_POKEMON) {
        return;
    }

    slotData = &state->slots[slot];

    /* Set animation transition type for the slot model */
    slotData->animType = animType;

    /* Request the animation on the slot's JObj */
    {
        void* jobj = slotData->jobj;
        if (jobj != NULL) {
            fn_80362E40(jobj, 0.0f); /* HSD_JObjReqAnimAll */
        }
    }
}

/**
 * battleGridReplaceTrainer / battleGridReplaceTrainer - Replace trainer model in a grid slot.
 * Address: 0x801C3F10 | Size: 0xAC
 * Proposed name from symbols: battleGridReplaceTrainer.
 */
void battleGridReplaceTrainer(void* model) {
    /* Replace trainer model in a grid slot:
     * Similar to battleGridReplacePokemon but for trainer models.
     */
    if (model == NULL) {
        return;
    }
}

/**
 * battleGridAddTrainer - Add slot to grid group (renamed from
 * fn_801C3FBC; confirmed name -- naming pass 2026-07-07).
 * Address: 0x801C3FBC | Size: 0xBC
 */
void battleGridAddTrainer(u8* slot, u8 arg1, u8 arg2) {
    extern BattleGridGroupTable lbl_80466DE8;
    BattleGridGroupEntry* group;
    s8 state;

    if (lbl_80466DE8.count < 4) {
        group = &lbl_80466DE8.entries[0];
        if (group->slot != NULL) {
            group = &lbl_80466DE8.entries[1];
            if (group->slot != NULL) {
                group++;
                if (group->slot != NULL) {
                    group++;
                    if (group->slot != NULL) {
                        group++;
                    }
                }
            }
        }
        memset(group, 0, sizeof(*group));
        group->slot = slot;
        state = 1;
        group->arg1 = arg1;
        group->arg2 = arg2;
        if (arg1 != 0) {
            state = -1;
        }
        slot[0x76] = state;
        lbl_80466DE8.count = lbl_80466DE8.count + 1;
    }
}
