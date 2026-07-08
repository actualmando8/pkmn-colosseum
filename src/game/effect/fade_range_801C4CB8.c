/**
 * @file fade_range_801C4CB8.c
 * @brief fade effect system, 0x801C4CB8 - 0x801C766C.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) — mixed-block split pass, 2026-07-01.
 * All functions asm-only until matched.
 *
 * Boundary bug fix (battle_grid.c 5-way split, 2026-07-07): fn_801C4CB8
 * (0x704 bytes) was physically sitting in the old monolithic
 * game/battle/battle_grid.c despite being outside that file's own
 * declared splits.txt range (which ended at 0x801C4CB8, exclusive) --
 * i.e. it always belonged to this unit's range, just misplaced in
 * source. Relocated here so this unit scores real progress instead of
 * 0%. Registered by game/effect/fade_effect.c's
 * fadeEffectHookFunction_trainer_Init hook stub.
 */
#include "dolphin/types.h"
#include "game/battle/battle_grid_types.h"

extern void fn_80362D0C(void* jobj); /* HSD_JObjAnimAll */
extern void battleGridRemovePokemon(s32 slot); /* game/battle/battle_grid.c */

/**
 * fn_801C4CB8 - Grid full render update.
 * Address: 0x801C4CB8 | Size: 0x704
 * Large function handling the complete grid render pass:
 * updates all slot transforms, applies animations, renders models.
 */
void fn_801C4CB8(void) {
    s32 i;
    u8* state = (u8*)lbl_80466E50;

    /* Full grid render update:
     * 1. Update camera from BattleCameraState
     * 2. Update all slot transforms
     * 3. Animate all slot models
     * 4. Render all active slots
     */

    /* Update camera */
    {
        u8* cam = (u8*)lbl_80467030;
        s32 seqType = *(s32*)(cam + 0x0C);
        if (seqType != 0) {
            f32 timer = *(f32*)(cam + 0x18);
            timer += 1.0f;
            *(f32*)(cam + 0x18) = timer;
        }
    }

    /* Update all grid slots */
    for (i = 0; i < BATTLE_TOTAL_POKEMON; i++) {
        u8* slot = state + 0x20 + (i * 0x70);
        s32 active = *(s32*)(slot + 0x00);

        if (active == 0) {
            continue;
        }

        /* Animate model */
        {
            void* jobj = *(void**)(slot + 0x04);
            if (jobj != NULL) {
                fn_80362D0C(jobj); /* HSD_JObjAnimAll */
            }
        }

        /* Apply current transform */
        battleGridRemovePokemon(i);
    }
}
