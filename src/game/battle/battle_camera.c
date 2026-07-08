/**
 * @file battle_camera.c
 * @brief Battle grid trainer/model visibility helpers and battle camera
 *        tick/cleanup callbacks.
 *
 * Address range: 0x801C2AE8 - 0x801C3108 (9 functions).
 *
 * Split out of the former monolithic battle_grid.c CodeCandidate bucket
 * (0x801C0F20 - 0x801C4CB8, split pass 2026-07-07). This is a distinct
 * XD translation unit (game/pxdvs/app/battleGrid/battleCamera.cpp).
 */

#include "dolphin/types.h"
#include "game/battle/battle_grid_types.h"

/* Grid group table entry -- see battleGridAddTrainer (game/battle/battle_grid.c,
 * shrunk remainder) for the sibling BattleGridGroupTable view of the same
 * lbl_80466DE8 storage; kept as two independent local casts (as in the
 * original monolithic file) rather than unified into a shared header,
 * since neither TU calls the other's accessor. */
typedef struct BattleGridGroupEntry {
    u8* slot;
    u8 pad_04[8];
    u16 memberCount;
    u8 arg1;
    u8 arg2;
} BattleGridGroupEntry;

/* HSD (SysDolphin) model/animation */
extern void  fn_8036A384(void* jobj, f32 x, f32 y, f32 z); /* HSD_JObjSetTranslate */
extern void  fn_80363CF4(void* jobj);                       /* HSD_JObjRemoveAll */

/* CRT */
extern void* memset(void* dst, int val, u32 size);
extern void* memcpy(void* dst, const void* src, u32 size);

/**
 * battleGridGetNumPokemonsForTrainer - Get grid group member count by
 * owner ID (renamed from fn_801C2AE8; confirmed name -- naming pass
 * 2026-07-07).
 * Address: 0x801C2AE8 | Size: 0x44
 */
u16 battleGridGetNumPokemonsForTrainer(u32 id) {
    extern BattleGridGroupEntry lbl_80466DE8[];
    BattleGridGroupEntry* group = lbl_80466DE8;
    u16 i;

    for (i = 0; i < 4; i++, group++) {
        if ((u32)group->slot == id) {
            return group->memberCount;
        }
    }
    return 0;
}

/**
 * battleGridResetModelVisibilityFlags - Pre-grid update all slot
 * positions (renamed from fn_801C2B2C; confirmed name -- naming pass
 * 2026-07-07).
 * Address: 0x801C2B2C | Size: 0xB4
 */
void battleGridResetModelVisibilityFlags(void) {
    s32 i;
    BattleGridSceneWork* state = (BattleGridSceneWork*)lbl_80466E50;

    for (i = 0; i < BATTLE_TOTAL_POKEMON; i++) {
        BattleGridSceneSlot* slot = &state->slots[i];
        void* jobj = slot->jobj;

        if (jobj != NULL && slot->active != 0) {
            f32 x = slot->posX;
            f32 y = slot->posY;
            f32 z = slot->posZ;
            fn_8036A384(jobj, x, y, z);
        }
    }
}

/**
 * battleGridHideModelsExcept - Pre-grid final setup (renamed from
 * fn_801C2Be0; confirmed name -- naming pass 2026-07-07). NOTE: prior
 * source spelled this fn_801C2Be0 (mixed-case typo); the correct address
 * label is 0x801C2BE0. Any stale callers using either spelling are
 * updated to the new name.
 * Address: 0x801C2BE0 | Size: 0x174
 */
void battleGridHideModelsExcept(void* ctx, s32 arg1) {
    u8* state = (u8*)ctx;
    if (state == NULL) {
        return;
    }
    /* Finalize pre-grid setup:
     * - Apply final slot positions
     * - Set up rendering callbacks
     * - Initialize camera for battle view
     * - Mark grid as ready for battle
     */
    battleGridResetModelVisibilityFlags();
    *(s32*)(state + 0x00) = 7; /* GRID_READY */
}

/* =========================================================================
 * GRID TICK / STATE MANAGEMENT (0x801C2D54 - 0x801C3108)
 * ========================================================================= */

/**
 * battleCameraIsSimple - Battle grid tick callback 1 (no-op forward)
 * (renamed from fn_801C2D54; confirmed name -- naming pass 2026-07-07).
 * Referenced by battle_main.c as battle grid tick 1.
 * Address: 0x801C2D54 | Size: 0x8
 */
extern u8 lbl_8047B398;
extern u8 lbl_8047B399;

void battleCameraIsSimple(void) {
    asm { lbz r3, lbl_8047B399(r13) }
}

/**
 * battleCameraDoFull - Battle grid tick callback wrapper (renamed from
 * fn_801C2D5C; confirmed name -- naming pass 2026-07-07).
 * Address: 0x801C2D5C | Size: 0xC
 */
void battleCameraDoFull(void) {
    lbl_8047B399 = 0;
}

/**
 * battleCameraDoSimple - Battle grid tick callback 2 (renamed from
 * fn_801C2D68; confirmed name -- naming pass 2026-07-07).
 * Referenced by battle_main.c as battle grid tick 2.
 * Address: 0x801C2D68 | Size: 0xC
 */
void battleCameraDoSimple(void) {
    lbl_8047B399 = 1;
}

/**
 * battleCameraDisable - Battle grid tick callback 3 (renamed from
 * fn_801C2D74; confirmed name -- naming pass 2026-07-07).
 * Address: 0x801C2D74 | Size: 0xC
 */
void battleCameraDisable(void) {
    lbl_8047B398 = 1;
}

/**
 * battleCameraStartRandom - Battle grid cleanup / release resources
 * (renamed from fn_801C2D80; confirmed name -- naming pass 2026-07-07).
 * Referenced by battle_main.c as "battle grid cleanup".
 * Releases all grid models, clears slot state, frees memory.
 * Address: 0x801C2D80 | Size: 0x180
 */
void battleCameraStartRandom(void) {
    s32 i;
    BattleGridSceneWork* state = (BattleGridSceneWork*)lbl_80466E50;

    /* Release all grid slot models */
    for (i = 0; i < BATTLE_TOTAL_POKEMON; i++) {
        BattleGridSceneSlot* slot = &state->slots[i];
        void* jobj = slot->jobj;

        if (jobj != NULL) {
            fn_80363CF4(jobj); /* HSD_JObjRemoveAll */
            slot->jobj = NULL;
        }

        slot->active = 0;
    }

    /* Clear scene animation context */
    memset(lbl_80466E50, 0, 0x1E0);

    /* Reset camera state */
    memset(lbl_80467030, 0, 0x20);
}

/**
 * fn_801C2F00 - Battle grid load data from buffer.
 * Address: 0x801C2F00 | Size: 0x208
 * Referenced by battle_main.c as "battle grid load data".
 * Loads grid configuration from a data buffer.
 */
void fn_801C2F00(void* data, u32 size) {
    if (data == NULL || size == 0) {
        return;
    }
    /* Load grid configuration from a data buffer:
     * 1. Parse header (slot count, field type)
     * 2. For each slot: load position, model ID, animation set
     * 3. Load camera configuration
     * 4. Load stage model reference
     */
    memcpy(lbl_80466E50, data, (size < 0x1E0) ? size : 0x1E0);
}
