/**
 * @file move_data.c
 * @brief Move data accessors via common_rel.
 *
 * Provides accessor functions for reading move data from the common_rel
 * data tables. Each move is 0x38 (56) bytes, stored at common_rel
 * offset 0x11E048.
 *
 * Move IDs follow Gen III indexing (1 = Pound, 2 = Karate Chop, etc.).
 * Shadow moves start at index 0x0164 (356). Move ID 0 is the "null move"
 * (Struggle is handled separately in the battle engine).
 *
 * Address context:
 *   Move data accessors are small functions in the battle logic region
 *   (0x801C53BC - 0x801D7230). The disassembly shows:
 *
 *     mulli r0, r3, 0x38        ; moveID * 56
 *     add   r3, rMoveTableBase, r0
 *     lbz   r3, <offset>(r3)   ; read field
 *     blr
 *
 *   The move table base is loaded from the resolved common_rel index:
 *     lwz   r4, sIndexPointers + COMMON_INDEX_MOVES*4(r13)
 *
 *   The battle damage calculation references multiple move fields:
 *     - basePower (0x17) for damage formula
 *     - type (0x02) for STAB and effectiveness
 *     - makesContact (0x06) for Rough Skin, etc.
 *     - priority (0x00) for turn order
 *     - accuracy (0x04) for hit check
 */

#include "game/data/common_rel.h"

/* ===================================================================
 * Core move data accessor
 *
 * CommonRel_GetMoveData is declared in common_rel.h and defined
 * in common_rel.c. The functions below provide field-level access.
 * =================================================================== */

/* ===================================================================
 * Primary battle-relevant accessors
 * =================================================================== */

/**
 * Get the base power of a move.
 * Power of 0 indicates a status move or variable-power move.
 */
u8 MoveData_GetBasePower(u16 moveID) {
    CommonMoveData* move;

    move = CommonRel_GetMoveData(moveID);
    if (move == NULL) {
        return 0;
    }
    return move->basePower;
}

/**
 * Get the type of a move.
 * Returns a type index (0-17, see battle.h TYPE_* constants).
 */
u8 MoveData_GetType(u16 moveID) {
    CommonMoveData* move;

    move = CommonRel_GetMoveData(moveID);
    if (move == NULL) {
        return 0;
    }
    return move->type;
}

/**
 * Get the accuracy of a move (0-100).
 * Accuracy of 0 means the move always hits (e.g., Swift).
 */
u8 MoveData_GetAccuracy(u16 moveID) {
    CommonMoveData* move;

    move = CommonRel_GetMoveData(moveID);
    if (move == NULL) {
        return 0;
    }
    return move->accuracy;
}

/**
 * Get the base PP (Power Points) of a move.
 */
u8 MoveData_GetBasePP(u16 moveID) {
    CommonMoveData* move;

    move = CommonRel_GetMoveData(moveID);
    if (move == NULL) {
        return 0;
    }
    return move->basePP;
}

/**
 * Get the priority of a move.
 * Normal priority is 0. Higher values move first.
 * Stored as unsigned; 255 means -1 priority.
 */
u8 MoveData_GetPriority(u16 moveID) {
    CommonMoveData* move;

    move = CommonRel_GetMoveData(moveID);
    if (move == NULL) {
        return 0;
    }
    return move->priority;
}

/**
 * Get the effect ID of a move.
 * This indexes into the Gen III effect table for secondary effects
 * (stat changes, status infliction, flinch, etc.).
 */
u8 MoveData_GetEffect(u16 moveID) {
    CommonMoveData* move;

    move = CommonRel_GetMoveData(moveID);
    if (move == NULL) {
        return 0;
    }
    return move->effect;
}

/**
 * Get the secondary effect accuracy (chance to trigger, 0-100).
 */
u8 MoveData_GetEffectAccuracy(u16 moveID) {
    CommonMoveData* move;

    move = CommonRel_GetMoveData(moveID);
    if (move == NULL) {
        return 0;
    }
    return move->effectAccuracy;
}

/* ===================================================================
 * Flag accessors
 * =================================================================== */

/**
 * Check if a move makes physical contact.
 * Contact moves trigger abilities like Rough Skin and Static.
 */
BOOL MoveData_MakesContact(u16 moveID) {
    CommonMoveData* move;

    move = CommonRel_GetMoveData(moveID);
    if (move == NULL) {
        return FALSE;
    }
    return (move->makesContact != 0) ? TRUE : FALSE;
}

/**
 * Check if a move is blocked by Protect/Detect.
 */
BOOL MoveData_BlockedByProtect(u16 moveID) {
    CommonMoveData* move;

    move = CommonRel_GetMoveData(moveID);
    if (move == NULL) {
        return FALSE;
    }
    return (move->blockedByProtect != 0) ? TRUE : FALSE;
}

/**
 * Check if a move can be reflected by Magic Coat.
 */
BOOL MoveData_CanReflect(u16 moveID) {
    CommonMoveData* move;

    move = CommonRel_GetMoveData(moveID);
    if (move == NULL) {
        return FALSE;
    }
    return (move->magicCoatReflects != 0) ? TRUE : FALSE;
}

/**
 * Check if a move can be stolen by Snatch.
 */
BOOL MoveData_CanSnatch(u16 moveID) {
    CommonMoveData* move;

    move = CommonRel_GetMoveData(moveID);
    if (move == NULL) {
        return FALSE;
    }
    return (move->snatchSteals != 0) ? TRUE : FALSE;
}

/**
 * Check if a move can be copied by Mirror Move.
 */
BOOL MoveData_CanMirrorMove(u16 moveID) {
    CommonMoveData* move;

    move = CommonRel_GetMoveData(moveID);
    if (move == NULL) {
        return FALSE;
    }
    return (move->mirrorMoveCopies != 0) ? TRUE : FALSE;
}

/**
 * Check if a move can cause flinch with King's Rock.
 */
BOOL MoveData_KingsRockFlinch(u16 moveID) {
    CommonMoveData* move;

    move = CommonRel_GetMoveData(moveID);
    if (move == NULL) {
        return FALSE;
    }
    return (move->kingsRockFlinch != 0) ? TRUE : FALSE;
}

/**
 * Check if a move is sound-based (blocked by Soundproof).
 */
BOOL MoveData_IsSoundBased(u16 moveID) {
    CommonMoveData* move;

    move = CommonRel_GetMoveData(moveID);
    if (move == NULL) {
        return FALSE;
    }
    return (move->soundBased != 0) ? TRUE : FALSE;
}

/**
 * Check if a move is an HM move (or has the shadow flag in XD).
 * In Colosseum, this flag is 1 for HM moves.
 */
BOOL MoveData_IsHM(u16 moveID) {
    CommonMoveData* move;

    move = CommonRel_GetMoveData(moveID);
    if (move == NULL) {
        return FALSE;
    }
    return (move->hmFlag != 0) ? TRUE : FALSE;
}

/* ===================================================================
 * Target and category helpers
 * =================================================================== */

/**
 * Get the target selection flags for a move.
 * Determines whether the move targets one opponent, both, self, etc.
 */
u8 MoveData_GetTargets(u16 moveID) {
    CommonMoveData* move;

    move = CommonRel_GetMoveData(moveID);
    if (move == NULL) {
        return 0;
    }
    return move->targets;
}

/**
 * Get the 16-bit effect ID for a move.
 * This is the extended effect identifier used in some battle calculations.
 */
u16 MoveData_GetEffectID(u16 moveID) {
    CommonMoveData* move;

    move = CommonRel_GetMoveData(moveID);
    if (move == NULL) {
        return 0;
    }
    return move->effectID;
}

/* ===================================================================
 * String / display accessors
 * =================================================================== */

u16 MoveData_GetNameStringID(u16 moveID) {
    CommonMoveData* move;

    move = CommonRel_GetMoveData(moveID);
    if (move == NULL) {
        return 0;
    }
    return move->nameStringID;
}

u16 MoveData_GetDescriptionID(u16 moveID) {
    CommonMoveData* move;

    move = CommonRel_GetMoveData(moveID);
    if (move == NULL) {
        return 0;
    }
    return move->descriptionID;
}

u16 MoveData_GetAnimationID(u16 moveID) {
    CommonMoveData* move;

    move = CommonRel_GetMoveData(moveID);
    if (move == NULL) {
        return 0;
    }
    return move->animationID;
}

/**
 * Gen IV+ move-based physical/special split override table.
 *
 * In Gen III, category was determined by type. In Gen IV+, each move
 * has its own category. This table lists moves that CHANGED category
 * from Gen III to Gen IV. Moves NOT in this table keep their Gen III
 * (type-based) category.
 *
 * Move IDs use standard Pokemon numbering (1-based).
 *
 * Format: { moveID, category }
 *   0 = physical, 1 = special
 *
 * Sorted by moveID.
 * Terminated with moveID == 0.
 */
typedef struct {
    u16 moveID;
    u8  category;
} MoveCategoryOverride;

/*
 * Only moves that CHANGED category from Gen III to Gen IV are listed.
 * Based on the user's moveset list provided.
 */
static const MoveCategoryOverride sMoveCategoryOverrides[] = {
    /* Became Physical in Gen IV (were Special by type in Gen III) */
    { 8,   0 },   /* Fire Punch - Fire type, now Physical */
    { 86,  0 },   /* Thunderpunch - Electric type, now Physical */
    { 127, 0 },   /* Waterfall - Water type, now Physical */
    { 200, 0 },   /* Outrage - Dragon type, now Physical */
    { 209, 0 },   /* Spark - Electric type, now Physical */
    { 242, 0 },   /* Crunch - Dark type, now Physical */
    { 303, 0 },   /* Leaf Blade - Grass type, now Physical */
    { 337, 0 },   /* Dragon Claw - Dragon type, now Physical */
    { 338, 0 },   /* Ice Punch - Ice type, now Physical */

    /* Became Special in Gen IV (were Physical by type in Gen III) */
    { 2,   1 },   /* Acid - Poison type, now Special */
    { 5,   1 },   /* Petal Dance - Grass type, now Special */
    { 13,  1 },   /* Razor Wind - Normal type, now Special */
    { 16,  1 },   /* Gust - Flying type, now Special */
    { 46,  1 },   /* Sonicboom - Normal type, now Special */
    { 55,  1 },   /* Water Gun - Water type, now Special */
    { 56,  1 },   /* Hydro Pump - Water type, now Special */
    { 57,  1 },   /* Surf - Water type, now Special */
    { 60,  1 },   /* Psybeam - Psychic type, now Special */
    { 61,  1 },   /* Bubble Beam - Water type, now Special */
    { 82,  1 },   /* Dragon Rage - Dragon type, now Special */
    { 83,  1 },   /* Fire Spin - Fire type, now Special */
    { 84,  1 },   /* Thundershock - Electric type, now Special */
    { 85,  1 },   /* Thunderbolt - Electric type, now Special */
    { 87,  1 },   /* Thunder - Electric type, now Special */
    { 93,  1 },   /* Confusion - Psychic type, now Special */
    { 94,  1 },   /* Psychic - Psychic type, now Special */
    { 101, 1 },   /* Night Shade - Ghost type, now Special */
    { 120, 1 },   /* Smog - Poison type, now Special */
    { 124, 1 },   /* Sludge - Poison type, now Special */
    { 126, 1 },   /* Fire Blast - Fire type, now Special */
    { 129, 1 },   /* Swift - Normal type, now Special */
    { 130, 1 },   /* Bubble - Water type, now Special */
    { 134, 1 },   /* Aurora Beam - Ice type, now Special */
    { 138, 1 },   /* Dream Eater - Psychic type, now Special */
    { 149, 1 },   /* Psywave - Psychic type, now Special */
    { 161, 1 },   /* Tri Attack - Normal type, now Special */
    { 173, 1 },   /* Snore - Normal type, now Special */
    { 177, 1 },   /* Aeroblast - Flying type, now Special */
    { 189, 1 },   /* Mud-Slap - Ground type, now Special */
    { 196, 1 },   /* Icy Wind - Ice type, now Special */
    { 218, 1 },   /* Sludge Bomb - Poison type, now Special */
    { 239, 1 },   /* Twister - Dragon type, now Special */
    { 246, 1 },   /* Ancient Power - Rock type, now Special */
    { 247, 1 },   /* Shadow Ball - Ghost type, now Special */
    { 248, 1 },   /* Future Sight - Psychic type, now Special */
    { 250, 1 },   /* Whirlpool - Water type, now Special */
    { 257, 1 },   /* Heat Wave - Fire type, now Special */
    { 268, 1 },   /* Zap Cannon - Electric type, now Special */
    { 297, 1 },   /* Mist Ball - Psychic type, now Special */
    { 304, 1 },   /* Hyper Voice - Normal type, now Special */
    { 311, 1 },   /* Weather Ball - Normal type, now Special */
    { 314, 1 },   /* Air Cutter - Flying type, now Special */
    { 318, 1 },   /* Silver Wind - Bug type, now Special */
    { 323, 1 },   /* Water Spout - Water type, now Special */
    { 326, 1 },   /* Extrasensory - Psychic type, now Special */
    { 329, 1 },   /* Sheer Cold - Ice type, now Special */
    { 330, 1 },   /* Muddy Water - Water type, now Special */
    { 341, 1 },   /* Mud Shot - Ground type, now Special */
    { 345, 1 },   /* Magical Leaf - Grass type, now Special */
    { 348, 1 },   /* Luster Purge - Psychic type, now Special */
    { 351, 1 },   /* Shock Wave - Electric type, now Special */
    { 352, 1 },   /* Water Pulse - Water type, now Special */
    { 356, 1 },   /* Doom Desire - Steel type, now Special */
    { 406, 1 },   /* Dragon Pulse - Dragon type, now Special */
    { 435, 1 },   /* Dragonbreath - Dragon type, now Special */
    { 444, 1 },   /* Signal Beam - Bug type, now Special */

    /* Terminate */
    { 0, 0 },
};

/**
 * Look up a move in the category override table using linear search.
 * Returns the override category if found, or 0xFF if not found.
 */
static u8 MoveData_GetCategoryOverride(u16 moveID) {
    u32 i;
    for (i = 0; sMoveCategoryOverrides[i].moveID != 0; i++) {
        if (sMoveCategoryOverrides[i].moveID == moveID) {
            return sMoveCategoryOverrides[i].category;
        }
    }
    return 0xFF;
}

/**
 * Determine the move category (physical/special/status).
 *
 * Uses Gen IV+ move-based split where each move has its own category.
 * Moves in the override table use their explicit category.
 * Moves not in the table fall back to the Gen III type-based split.
 *
 * @param moveID  Move index
 * @return        0 = physical, 1 = special, 2 = status
 */
u8 MoveData_GetCategory(u16 moveID) {
    CommonMoveData* move;
    u8 type;
    u8 override;

    move = CommonRel_GetMoveData(moveID);
    if (move == NULL) {
        return 2; /* default to status */
    }

    /* Status moves have no base power */
    if (move->basePower == 0) {
        return 2;
    }

    /* Check Gen IV+ override table first */
    override = MoveData_GetCategoryOverride(moveID);
    if (override != 0xFF) {
        return override;
    }

    /* Fall back to Gen III type-based split for moves not in override table */
    type = move->type;
    switch (type) {
        case  0: /* Normal   */
        case  1: /* Fighting */
        case  2: /* Flying   */
        case  3: /* Poison   */
        case  4: /* Ground   */
        case  5: /* Rock     */
        case  6: /* Bug      */
        case  7: /* Ghost    */
        case  8: /* Steel    */
            return 0; /* physical */

        case  9: /* Fire     */
        case 10: /* Water    */
        case 11: /* Grass    */
        case 12: /* Electric */
        case 13: /* Psychic  */
        case 14: /* Ice      */
        case 15: /* Dragon   */
        case 16: /* Dark     */
            return 1; /* special */

        case 17: /* Shadow   */
            return 0; /* Shadow Rush is physical in Colosseum */

        default:
            return 0;
    }
}
