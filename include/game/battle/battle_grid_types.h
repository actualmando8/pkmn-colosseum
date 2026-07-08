/**
 * @file battle_grid_types.h
 * @brief Shared types for the split battle grid translation units.
 *
 * battle_grid.c (0x801C0F20 - 0x801C4CB8) was one CodeCandidate bucket
 * covering 5 true XD translation units. It was split into:
 *   - hsd/hsd_aobj_range_801C01C8.c (extended: 0x801C01C8 - 0x801C2AE8)
 *   - game/battle/battle_camera.c   (0x801C2AE8 - 0x801C3108)
 *   - game/battle/battle_grid.c     (0x801C3108 - 0x801C4078, shrunk)
 *   - game/effect/fade.c            (0x801C4078 - 0x801C4814)
 *   - game/effect/fade_effect.c     (0x801C4814 - 0x801C4CB8)
 *
 * The battle grid scene work layout (4-slot double battle grid: Player
 * Left/Right, Enemy Left/Right) is referenced across several of these
 * files, so it lives here instead of being duplicated per-TU.
 */
#ifndef BATTLE_GRID_TYPES_H
#define BATTLE_GRID_TYPES_H

#include "dolphin/types.h"

#define BATTLE_POS_ENEMY_LEFT    2
#define BATTLE_TOTAL_POKEMON     4

typedef struct BattleGridSceneSlot {
    s32 active;
    void* jobj;
    f32 posX;
    f32 posY;
    f32 posZ;
    u8 pad_14[0x3C];
    f32 rotationY;
    f32 scale;
    s32 animType;
    u8 pad_5C[4];
    f32 blend;
    u8 pad_64[0x0C];
} BattleGridSceneSlot;

typedef struct BattleGridSceneWork {
    u8 pad_00[0x20];
    BattleGridSceneSlot slots[BATTLE_TOTAL_POKEMON];
} BattleGridSceneWork;

/* Battle scene animation context (BattleGridSceneWork) and camera state,
 * shared (by extern) across the split battle grid TUs. */
extern u8 lbl_80466E50[0x1E0]; /* battle scene animation context */
extern u8 lbl_80467030[0x20];  /* BattleCameraState */

#endif /* BATTLE_GRID_TYPES_H */
