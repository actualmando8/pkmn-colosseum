/**
 * @file battle_type.c
 * @brief Type effectiveness lookup for the battle engine.
 *
 * Pokemon Colosseum uses the standard Gen III type chart with the addition
 * of the Shadow type (type ID 17). Shadow-type moves are super effective
 * against all non-Shadow types and not very effective against Shadow types.
 *
 * In the Gen III engine, type effectiveness is stored as a flat table of
 * (attacking_type, defending_type, effectiveness) triples. The table is
 * terminated by a 0xFF sentinel. Effectiveness values use fixed-point
 * encoding: 0 = immune, 5 = 0.5x (not very effective), 10 = 1x (neutral),
 * 20 = 2x (super effective).
 *
 * For dual-typed defenders, effectiveness is calculated by looking up each
 * defending type separately and multiplying the results:
 *   result = eff1 * eff2 (using the fixed-point scale)
 *
 * In Colosseum specifically, Shadow Rush always deals neutral damage against
 * Shadow Pokemon but super effective damage against all others. This is
 * hardcoded in the Shadow battle mechanics rather than in this table.
 *
 * Address context:
 *   The type table is stored in the .data section and referenced by battle
 *   state machine functions. The lookup logic is inlined into the damage
 *   calculation path within the state machine around 0x801E03D4.
 */

#include "game/battle/battle.h"

/* =========================================================================
 * Type Effectiveness Table
 * ========================================================================= */

/**
 * Gen III type effectiveness chart.
 * Each entry: { attacking_type, defending_type, effectiveness }
 * Only non-neutral matchups are listed (neutral is the default).
 * Terminated by 0xFF in the attacking_type field.
 *
 * This table encodes all 324 type matchups (18x18 with Shadow type).
 * Entries with TYPE_EFF_NORMAL (10) are omitted since that's the default.
 */
static const TypeMatchup sTypeChart[] = {
    /* Normal attacking */
    { TYPE_NORMAL,   TYPE_ROCK,     TYPE_EFF_NOT_VERY },
    { TYPE_NORMAL,   TYPE_STEEL,    TYPE_EFF_NOT_VERY },
    { TYPE_NORMAL,   TYPE_GHOST,    TYPE_EFF_IMMUNE   },

    /* Fighting attacking */
    { TYPE_FIGHTING, TYPE_NORMAL,   TYPE_EFF_SUPER    },
    { TYPE_FIGHTING, TYPE_ICE,      TYPE_EFF_SUPER    },
    { TYPE_FIGHTING, TYPE_ROCK,     TYPE_EFF_SUPER    },
    { TYPE_FIGHTING, TYPE_DARK,     TYPE_EFF_SUPER    },
    { TYPE_FIGHTING, TYPE_STEEL,    TYPE_EFF_SUPER    },
    { TYPE_FIGHTING, TYPE_FLYING,   TYPE_EFF_NOT_VERY },
    { TYPE_FIGHTING, TYPE_POISON,   TYPE_EFF_NOT_VERY },
    { TYPE_FIGHTING, TYPE_BUG,      TYPE_EFF_NOT_VERY },
    { TYPE_FIGHTING, TYPE_PSYCHIC,  TYPE_EFF_NOT_VERY },
    { TYPE_FIGHTING, TYPE_GHOST,    TYPE_EFF_IMMUNE   },

    /* Flying attacking */
    { TYPE_FLYING,   TYPE_GRASS,    TYPE_EFF_SUPER    },
    { TYPE_FLYING,   TYPE_FIGHTING, TYPE_EFF_SUPER    },
    { TYPE_FLYING,   TYPE_BUG,      TYPE_EFF_SUPER    },
    { TYPE_FLYING,   TYPE_ELECTRIC, TYPE_EFF_NOT_VERY },
    { TYPE_FLYING,   TYPE_ROCK,     TYPE_EFF_NOT_VERY },
    { TYPE_FLYING,   TYPE_STEEL,    TYPE_EFF_NOT_VERY },

    /* Poison attacking */
    { TYPE_POISON,   TYPE_GRASS,    TYPE_EFF_SUPER    },
    { TYPE_POISON,   TYPE_POISON,   TYPE_EFF_NOT_VERY },
    { TYPE_POISON,   TYPE_GROUND,   TYPE_EFF_NOT_VERY },
    { TYPE_POISON,   TYPE_ROCK,     TYPE_EFF_NOT_VERY },
    { TYPE_POISON,   TYPE_GHOST,    TYPE_EFF_NOT_VERY },
    { TYPE_POISON,   TYPE_STEEL,    TYPE_EFF_IMMUNE   },

    /* Ground attacking */
    { TYPE_GROUND,   TYPE_FIRE,     TYPE_EFF_SUPER    },
    { TYPE_GROUND,   TYPE_ELECTRIC, TYPE_EFF_SUPER    },
    { TYPE_GROUND,   TYPE_POISON,   TYPE_EFF_SUPER    },
    { TYPE_GROUND,   TYPE_ROCK,     TYPE_EFF_SUPER    },
    { TYPE_GROUND,   TYPE_STEEL,    TYPE_EFF_SUPER    },
    { TYPE_GROUND,   TYPE_GRASS,    TYPE_EFF_NOT_VERY },
    { TYPE_GROUND,   TYPE_BUG,      TYPE_EFF_NOT_VERY },
    { TYPE_GROUND,   TYPE_FLYING,   TYPE_EFF_IMMUNE   },

    /* Rock attacking */
    { TYPE_ROCK,     TYPE_FIRE,     TYPE_EFF_SUPER    },
    { TYPE_ROCK,     TYPE_ICE,      TYPE_EFF_SUPER    },
    { TYPE_ROCK,     TYPE_FLYING,   TYPE_EFF_SUPER    },
    { TYPE_ROCK,     TYPE_BUG,      TYPE_EFF_SUPER    },
    { TYPE_ROCK,     TYPE_FIGHTING, TYPE_EFF_NOT_VERY },
    { TYPE_ROCK,     TYPE_GROUND,   TYPE_EFF_NOT_VERY },
    { TYPE_ROCK,     TYPE_STEEL,    TYPE_EFF_NOT_VERY },

    /* Bug attacking */
    { TYPE_BUG,      TYPE_GRASS,    TYPE_EFF_SUPER    },
    { TYPE_BUG,      TYPE_PSYCHIC,  TYPE_EFF_SUPER    },
    { TYPE_BUG,      TYPE_DARK,     TYPE_EFF_SUPER    },
    { TYPE_BUG,      TYPE_FIRE,     TYPE_EFF_NOT_VERY },
    { TYPE_BUG,      TYPE_FIGHTING, TYPE_EFF_NOT_VERY },
    { TYPE_BUG,      TYPE_FLYING,   TYPE_EFF_NOT_VERY },
    { TYPE_BUG,      TYPE_POISON,   TYPE_EFF_NOT_VERY },
    { TYPE_BUG,      TYPE_GHOST,    TYPE_EFF_NOT_VERY },
    { TYPE_BUG,      TYPE_STEEL,    TYPE_EFF_NOT_VERY },

    /* Ghost attacking */
    { TYPE_GHOST,    TYPE_PSYCHIC,  TYPE_EFF_SUPER    },
    { TYPE_GHOST,    TYPE_GHOST,    TYPE_EFF_SUPER    },
    { TYPE_GHOST,    TYPE_DARK,     TYPE_EFF_NOT_VERY },
    { TYPE_GHOST,    TYPE_STEEL,    TYPE_EFF_NOT_VERY },
    { TYPE_GHOST,    TYPE_NORMAL,   TYPE_EFF_IMMUNE   },

    /* Steel attacking */
    { TYPE_STEEL,    TYPE_ICE,      TYPE_EFF_SUPER    },
    { TYPE_STEEL,    TYPE_ROCK,     TYPE_EFF_SUPER    },
    { TYPE_STEEL,    TYPE_FIRE,     TYPE_EFF_NOT_VERY },
    { TYPE_STEEL,    TYPE_WATER,    TYPE_EFF_NOT_VERY },
    { TYPE_STEEL,    TYPE_ELECTRIC, TYPE_EFF_NOT_VERY },
    { TYPE_STEEL,    TYPE_STEEL,    TYPE_EFF_NOT_VERY },

    /* Fire attacking */
    { TYPE_FIRE,     TYPE_GRASS,    TYPE_EFF_SUPER    },
    { TYPE_FIRE,     TYPE_ICE,      TYPE_EFF_SUPER    },
    { TYPE_FIRE,     TYPE_BUG,      TYPE_EFF_SUPER    },
    { TYPE_FIRE,     TYPE_STEEL,    TYPE_EFF_SUPER    },
    { TYPE_FIRE,     TYPE_FIRE,     TYPE_EFF_NOT_VERY },
    { TYPE_FIRE,     TYPE_WATER,    TYPE_EFF_NOT_VERY },
    { TYPE_FIRE,     TYPE_ROCK,     TYPE_EFF_NOT_VERY },
    { TYPE_FIRE,     TYPE_DRAGON,   TYPE_EFF_NOT_VERY },

    /* Water attacking */
    { TYPE_WATER,    TYPE_FIRE,     TYPE_EFF_SUPER    },
    { TYPE_WATER,    TYPE_GROUND,   TYPE_EFF_SUPER    },
    { TYPE_WATER,    TYPE_ROCK,     TYPE_EFF_SUPER    },
    { TYPE_WATER,    TYPE_WATER,    TYPE_EFF_NOT_VERY },
    { TYPE_WATER,    TYPE_GRASS,    TYPE_EFF_NOT_VERY },
    { TYPE_WATER,    TYPE_DRAGON,   TYPE_EFF_NOT_VERY },

    /* Grass attacking */
    { TYPE_GRASS,    TYPE_WATER,    TYPE_EFF_SUPER    },
    { TYPE_GRASS,    TYPE_GROUND,   TYPE_EFF_SUPER    },
    { TYPE_GRASS,    TYPE_ROCK,     TYPE_EFF_SUPER    },
    { TYPE_GRASS,    TYPE_FIRE,     TYPE_EFF_NOT_VERY },
    { TYPE_GRASS,    TYPE_GRASS,    TYPE_EFF_NOT_VERY },
    { TYPE_GRASS,    TYPE_POISON,   TYPE_EFF_NOT_VERY },
    { TYPE_GRASS,    TYPE_FLYING,   TYPE_EFF_NOT_VERY },
    { TYPE_GRASS,    TYPE_BUG,      TYPE_EFF_NOT_VERY },
    { TYPE_GRASS,    TYPE_DRAGON,   TYPE_EFF_NOT_VERY },
    { TYPE_GRASS,    TYPE_STEEL,    TYPE_EFF_NOT_VERY },

    /* Electric attacking */
    { TYPE_ELECTRIC, TYPE_WATER,    TYPE_EFF_SUPER    },
    { TYPE_ELECTRIC, TYPE_FLYING,   TYPE_EFF_SUPER    },
    { TYPE_ELECTRIC, TYPE_ELECTRIC, TYPE_EFF_NOT_VERY },
    { TYPE_ELECTRIC, TYPE_GRASS,    TYPE_EFF_NOT_VERY },
    { TYPE_ELECTRIC, TYPE_DRAGON,   TYPE_EFF_NOT_VERY },
    { TYPE_ELECTRIC, TYPE_GROUND,   TYPE_EFF_IMMUNE   },

    /* Psychic attacking */
    { TYPE_PSYCHIC,  TYPE_FIGHTING, TYPE_EFF_SUPER    },
    { TYPE_PSYCHIC,  TYPE_POISON,   TYPE_EFF_SUPER    },
    { TYPE_PSYCHIC,  TYPE_PSYCHIC,  TYPE_EFF_NOT_VERY },
    { TYPE_PSYCHIC,  TYPE_STEEL,    TYPE_EFF_NOT_VERY },
    { TYPE_PSYCHIC,  TYPE_DARK,     TYPE_EFF_IMMUNE   },

    /* Ice attacking */
    { TYPE_ICE,      TYPE_GRASS,    TYPE_EFF_SUPER    },
    { TYPE_ICE,      TYPE_GROUND,   TYPE_EFF_SUPER    },
    { TYPE_ICE,      TYPE_FLYING,   TYPE_EFF_SUPER    },
    { TYPE_ICE,      TYPE_DRAGON,   TYPE_EFF_SUPER    },
    { TYPE_ICE,      TYPE_FIRE,     TYPE_EFF_NOT_VERY },
    { TYPE_ICE,      TYPE_WATER,    TYPE_EFF_NOT_VERY },
    { TYPE_ICE,      TYPE_ICE,      TYPE_EFF_NOT_VERY },
    { TYPE_ICE,      TYPE_STEEL,    TYPE_EFF_NOT_VERY },

    /* Dragon attacking */
    { TYPE_DRAGON,   TYPE_DRAGON,   TYPE_EFF_SUPER    },
    { TYPE_DRAGON,   TYPE_STEEL,    TYPE_EFF_NOT_VERY },

    /* Dark attacking */
    { TYPE_DARK,     TYPE_PSYCHIC,  TYPE_EFF_SUPER    },
    { TYPE_DARK,     TYPE_GHOST,    TYPE_EFF_SUPER    },
    { TYPE_DARK,     TYPE_FIGHTING, TYPE_EFF_NOT_VERY },
    { TYPE_DARK,     TYPE_DARK,     TYPE_EFF_NOT_VERY },
    { TYPE_DARK,     TYPE_STEEL,    TYPE_EFF_NOT_VERY },

    /* Shadow attacking (Colosseum-exclusive) */
    /* Shadow Rush is super effective against all non-Shadow Pokemon */
    { TYPE_SHADOW,   TYPE_SHADOW,   TYPE_EFF_NOT_VERY },
    /* All other types are super effective (handled by default case) */

    /* Sentinel */
    { 0xFF, 0xFF, 0xFF }
};

/* =========================================================================
 * Implementation
 * ========================================================================= */

/**
 * Look up type effectiveness for a single type matchup.
 *
 * @param atkType  The attacking move's type.
 * @param defType  The defending Pokemon's type (one of potentially two).
 * @return         TYPE_EFF_* value (0, 5, 10, or 20).
 */
u8 battle_CalcTypeMatchup(u8 atkType, u8 defType) {
    const TypeMatchup* entry;

    /* Shadow type has special handling */
    if (atkType == TYPE_SHADOW) {
        if (defType == TYPE_SHADOW) {
            return TYPE_EFF_NOT_VERY;
        }
        return TYPE_EFF_SUPER;
    }

    /* Search the type chart for this matchup */
    for (entry = sTypeChart; entry->attackType != 0xFF; entry++) {
        if (entry->attackType == atkType && entry->defendType == defType) {
            return entry->effectiveness;
        }
    }

    /* Default: neutral effectiveness */
    return TYPE_EFF_NORMAL;
}

/**
 * Calculate combined type effectiveness against a dual-typed defender.
 *
 * Multiplies the effectiveness of the attacking type against each
 * defending type. The result is the product in the fixed-point scale.
 *
 * Examples:
 *   Super + Super = 20 * 20 / 10 = 40 (4x)
 *   Super + Normal = 20 * 10 / 10 = 20 (2x)
 *   Super + Not Very = 20 * 5 / 10 = 10 (1x)
 *   Not Very + Not Very = 5 * 5 / 10 = 2 (0.25x, rounded)
 *   Any + Immune = 0 (0x)
 *
 * The returned value is a composite that the damage formula can
 * apply directly: damage = damage * result / TYPE_EFF_NORMAL
 *
 * @param attackType  The move's type.
 * @param defType1    The defender's primary type.
 * @param defType2    The defender's secondary type.
 * @return            Combined effectiveness value.
 */
u8 battle_GetTypeEffectiveness(u8 attackType, u8 defType1, u8 defType2) {
    u8 eff1, eff2;

    eff1 = battle_CalcTypeMatchup(attackType, defType1);

    /* If immune to one type, total is immune */
    if (eff1 == TYPE_EFF_IMMUNE) {
        return TYPE_EFF_IMMUNE;
    }

    /* Single-typed Pokemon (type2 == type1 or type2 is TYPE_NORMAL placeholder) */
    if (defType1 == defType2) {
        return eff1;
    }

    eff2 = battle_CalcTypeMatchup(attackType, defType2);

    if (eff2 == TYPE_EFF_IMMUNE) {
        return TYPE_EFF_IMMUNE;
    }

    /* Multiply and normalize:
     * eff1 * eff2 / TYPE_EFF_NORMAL gives us the combined multiplier
     * in the same scale (5, 10, 20 -> 2.5, 5, 10, 20, 40) */
    return (u8)((s32)eff1 * (s32)eff2 / TYPE_EFF_NORMAL);
}
