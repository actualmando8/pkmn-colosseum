/**
 * @file fade_effect.c
 * @brief Battle grid fade-effect hook function table (per-effect slot
 *        position/rotation/scale accessor callbacks) and the combined
 *        slot transform updater.
 *
 * Address range: 0x801C4814 - 0x801C4CB8 (13 functions).
 *
 * Split out of the former monolithic battle_grid.c CodeCandidate bucket
 * (0x801C0F20 - 0x801C4CB8, split pass 2026-07-07). This is a distinct
 * XD translation unit (game/pxdvs/app/fade/fade_effect.cpp).
 *
 * NOTE (symbol swap fix, naming pass 2026-07-07): the previously-applied
 * names fadeEffectHookFunction_fadein_Init (0x801C483C) and
 * fadeEffectHookFunction_trainer_Init (0x801C4864) were swapped: 0x801C483C
 * registers the 0x704 body (trainer-sized; matches XD's trainer body
 * 0xC38) while 0x801C4864 registers the 0x34 body (fadein-sized; matches
 * XD's fadein body 0x70), and XD's real order is trainer_Init THEN
 * fadein_Init. The two function bodies below have been relabeled
 * accordingly (bodies unchanged, only the two names traded places) so
 * this TU is monotonic against XD. External callers (game/data/
 * data_80375938.c's fight_encount_wipe_data table, include/game/battle/
 * battle.h, include/game/battle/battle_waza_types.h) reference these
 * functions purely by name and need no changes: they now correctly
 * resolve to the swapped addresses.
 */

#include "dolphin/types.h"
#include "game/battle/battle_grid_types.h"

extern void fadeSetFunctionOnly(s32 arg0); /* game/effect/fade.c, renamed from fn_801C431C */

/**
 * fadeEffectHookFunction_Doku_Init - Grid get slot X position (renamed
 * from fn_801C4814; confirmed name -- naming pass 2026-07-07). Registers
 * fadeEffectHookFunction_Doku (fn_801C4A44); fadeEffectDokuStart/Stop
 * exist in game/effect/fade.c.
 * Address: 0x801C4814 | Size: 0x28
 */
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
f32 fadeEffectHookFunction_Doku_Init(s32 slot) {
    extern void fadeEffectHookFunction_Doku(s32 slot, f32 x, f32 y, f32 z, f32 rot, f32 scale); /* renamed from fn_801C4A44 */

    fadeSetFunctionOnly((s32)fadeEffectHookFunction_Doku);
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on

/**
 * fadeEffectHookFunction_trainer_Init - Grid get slot Y position (symbol
 * swap fix: this body was previously misnamed fadeEffectHookFunction_
 * fadein_Init; see file header note).
 * Address: 0x801C483C | Size: 0x28
 */
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
f32 fadeEffectHookFunction_trainer_Init(s32 slot) {
    extern void fn_801C4CB8(void);

    fadeSetFunctionOnly((s32)fn_801C4CB8);
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on

/**
 * fadeEffectHookFunction_fadein_Init - Grid get slot Z position (symbol
 * swap fix: this body was previously misnamed fadeEffectHookFunction_
 * trainer_Init; see file header note).
 * Address: 0x801C4864 | Size: 0x28
 */
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
f32 fadeEffectHookFunction_fadein_Init(s32 slot) {
    extern f32 fn_801C54FC(void);

    fadeSetFunctionOnly((s32)fn_801C54FC);
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on

/**
 * fadeEffectHookFunction_fadeout_in_Init - Grid set slot X position.
 * Address: 0x801C488C | Size: 0x28
 */
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
void fadeEffectHookFunction_fadeout_in_Init(s32 slot, f32 x) {
    extern void fn_801C5530(void);

    fadeSetFunctionOnly((s32)fn_801C5530);
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on

/**
 * fadeEffectHookFunction_carde_Init - Grid set slot Y position.
 * Address: 0x801C48B4 | Size: 0x28
 */
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
void fadeEffectHookFunction_carde_Init(s32 slot, f32 y) {
    extern f32 fadeEffectHookFunction_carde(void); /* renamed from fn_801C4C98 */

    fadeSetFunctionOnly((s32)fadeEffectHookFunction_carde);
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on

/**
 * fadeEffectHookFunction_boss_Init - Grid set slot Z position.
 * Address: 0x801C48DC | Size: 0x28
 */
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
void fadeEffectHookFunction_boss_Init(s32 slot, f32 z) {
    extern void fn_801C55D8(void);

    fadeSetFunctionOnly((s32)fn_801C55D8);
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on

/**
 * fadeEffectHookFunction_yoko_or_tate_or_ball_Init - Grid set slot full position.
 * Address: 0x801C4904 | Size: 0x70
 */
#pragma scheduling off
void fadeEffectHookFunction_yoko_or_tate_or_ball_Init(s32 slot, f32 x, f32 y, f32 z) {
    extern s32 fn_801C6908(s32);
    extern void fn_801C5F6C(void);
    extern void fn_801C5ED0(void);
    extern void fn_801C5898(void);
    s32 result = fn_801C6908(3);

    switch (result) {
    case 0:
        fadeSetFunctionOnly((s32)fn_801C5F6C);
        break;
    case 1:
        fadeSetFunctionOnly((s32)fn_801C5ED0);
        break;
    case 2:
    default:
        fadeSetFunctionOnly((s32)fn_801C5898);
        break;
    }
}
#pragma scheduling on

/**
 * fadeEffectHookFunction_ball_Init - Grid get slot rotation.
 * Address: 0x801C4974 | Size: 0x28
 */
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
f32 fadeEffectHookFunction_ball_Init(s32 slot) {
    extern f32 fn_801C5898(void);

    fadeSetFunctionOnly((s32)fn_801C5898);
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on

/**
 * fadeEffectHookFunction_yoko_or_tate_Init - Grid set slot rotation.
 * Address: 0x801C499C | Size: 0x58
 */
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
void fadeEffectHookFunction_yoko_or_tate_Init(s32 slot, f32 rotation) {
    extern s32 fn_801C6908(s32);
    extern void fn_801C5F6C(void);
    extern void fn_801C5ED0(void);
    s32 result = fn_801C6908(2);
    switch (result) {
    case 0:
        fadeSetFunctionOnly((s32)fn_801C5F6C);
        break;
    case 1:
    default:
        fadeSetFunctionOnly((s32)fn_801C5ED0);
        break;
    }
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on

/**
 * fadeEffectHookFunction_tate_Init - Grid get slot scale.
 * Address: 0x801C49F4 | Size: 0x28
 */
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
f32 fadeEffectHookFunction_tate_Init(s32 slot) {
    extern f32 fn_801C5ED0(void);

    fadeSetFunctionOnly((s32)fn_801C5ED0);
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on

/**
 * fadeEffectHookFunction_yoko_Init - Grid set slot scale.
 * Address: 0x801C4A1C | Size: 0x28
 */
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
void fadeEffectHookFunction_yoko_Init(s32 slot, f32 scale) {
    extern void fn_801C5F6C(void);

    fadeSetFunctionOnly((s32)fn_801C5F6C);
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on

/**
 * fadeEffectHookFunction_Doku - Grid complex slot update (position +
 * rotation + scale) (renamed from fn_801C4A44; confirmed name -- naming
 * pass 2026-07-07).
 * Address: 0x801C4A44 | Size: 0x254
 */
void fadeEffectHookFunction_Doku(s32 slot, f32 x, f32 y, f32 z, f32 rot, f32 scale) {
    extern void  fn_8036A384(void* jobj, f32 x, f32 y, f32 z); /* HSD_JObjSetTranslate */
    extern void  fn_8036A2D8(void* jobj, f32 rx, f32 ry, f32 rz); /* HSD_JObjSetRotation */
    extern void  fn_8036A478(void* jobj, f32 x, f32 y, f32 z); /* HSD_JObjSetScale */
    BattleGridSceneWork* state = (BattleGridSceneWork*)lbl_80466E50;
    BattleGridSceneSlot* slotData;

    if (slot < 0 || slot >= BATTLE_TOTAL_POKEMON) {
        return;
    }

    slotData = &state->slots[slot];

    /* Set all transform properties */
    slotData->posX = x;
    slotData->posY = y;
    slotData->posZ = z;
    slotData->rotationY = rot;
    slotData->scale = scale;

    /* Apply to JObj */
    {
        void* jobj = slotData->jobj;
        if (jobj != NULL) {
            fn_8036A384(jobj, x, y, z);
            fn_8036A2D8(jobj, 0.0f, rot, 0.0f);
            fn_8036A478(jobj, scale, scale, scale);
        }
    }
}

/**
 * fadeEffectHookFunction_carde - Get grid rotation callback (renamed from
 * fn_801C4C98; confirmed name -- naming pass 2026-07-07).
 * Address: 0x801C4C98 | Size: 0x20
 */
f32 fadeEffectHookFunction_carde(void) {
    extern f32 fn_801C5F6C(void);
    return fn_801C5F6C();
}
