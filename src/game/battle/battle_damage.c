/**
 * @file battle_damage.c
 * @brief Damage calculation using the Gen III formula.
 *
 * Pokemon Colosseum uses the standard Generation III damage formula:
 *
 *   baseDamage = ((2 * level / 5 + 2) * power * atk / def) / 50 + 2
 *   finalDamage = baseDamage * modifier1 * modifier2 * ... * random / 100
 *
 * Modifiers applied in order:
 *   1. STAB (Same-Type Attack Bonus): 1.5x if move type matches attacker type
 *   2. Type effectiveness: 0x, 0.5x, 1x, or 2x per defending type
 *   3. Critical hit: 2x (ignores negative stat changes on attacker,
 *      ignores positive stat changes on defender)
 *   4. Random factor: 85-100 (integer, uniform)
 *   5. Items (e.g., Choice Band for physical, etc.)
 *   6. Abilities (e.g., Blaze, Overgrow, etc.)
 *   7. Weather (e.g., rain boosts Water, sun boosts Fire)
 *   8. Shadow Rush special handling
 *
 * In Colosseum, the category (physical/special) is determined by the
 * move's type, following the Gen III physical/special split by type:
 *   Physical: Normal, Fighting, Flying, Poison, Ground, Rock, Bug, Ghost, Steel
 *   Special:  Fire, Water, Grass, Electric, Psychic, Ice, Dragon, Dark
 *
 * The stat used for attack/defense depends on this:
 *   Physical moves: use Attack / Defense
 *   Special moves:  use Sp. Attack / Sp. Defense
 *
 * Stat stage modifiers follow the standard table:
 *   Stage -6: 2/8  Stage -5: 2/7  Stage -4: 2/6  Stage -3: 2/5
 *   Stage -2: 2/4  Stage -1: 2/3  Stage  0: 2/2
 *   Stage +1: 3/2  Stage +2: 4/2  Stage +3: 5/2
 *   Stage +4: 6/2  Stage +5: 7/2  Stage +6: 8/2
 *
 * Address context:
 *   The damage calculation is embedded within the battle state machine
 *   (0x801E03D4 - 0x801EF02C) rather than existing as standalone
 *   functions. The integer multiply/divide patterns found in the
 *   disassembly around 0x801E3D68 and the state machine case handlers
 *   implement this formula inline.
 */

#include "game/battle/battle.h"

/* =========================================================================
 * External function declarations
 * ========================================================================= */

/* Random number generation */
extern s32  fn_800D37CC(void);   /* GSrandom_Get - returns random u32 */
extern void fn_800D3074(s32 seed); /* GSrandom_Seed */

/* =========================================================================
 * Constants
 * ========================================================================= */

/**
 * Stat stage multiplier table.
 * Index 0 = stage -6, index 6 = stage 0, index 12 = stage +6.
 * Stored as numerator/denominator pairs.
 */
static const s32 sStatStageNumerator[13] = {
    2, 2, 2, 2, 2, 2, 2, 3, 4, 5, 6, 7, 8
};

static const s32 sStatStageDenominator[13] = {
    8, 7, 6, 5, 4, 3, 2, 2, 2, 2, 2, 2, 2
};

/**
 * Physical/special split by type (Gen III rules).
 * TRUE = physical, FALSE = special.
 */
static const u8 sTypeIsPhysical[TYPE_COUNT] = {
    /* Normal   */ 1,
    /* Fighting */ 1,
    /* Flying   */ 1,
    /* Poison   */ 1,
    /* Ground   */ 1,
    /* Rock     */ 1,
    /* Bug      */ 1,
    /* Ghost    */ 1,
    /* Steel    */ 1,
    /* Fire     */ 0,
    /* Water    */ 0,
    /* Grass    */ 0,
    /* Electric */ 0,
    /* Psychic  */ 0,
    /* Ice      */ 0,
    /* Dragon   */ 0,
    /* Dark     */ 0,
    /* Shadow   */ 1,  /* Shadow Rush is treated as physical */
};

/* =========================================================================
 * Implementation
 * ========================================================================= */

/**
 * Apply a stat stage modifier to a base stat value.
 *
 * @param baseStat  The Pokemon's unmodified stat value.
 * @param stage     The stat stage (-6 to +6).
 * @return          The modified stat value.
 */
s32 battle_ApplyStatStage(s32 baseStat, s8 stage) {
    s32 idx;

    /* Clamp stage to valid range */
    if (stage < -6) stage = -6;
    if (stage >  6) stage =  6;

    idx = stage + 6;
    return (baseStat * sStatStageNumerator[idx]) / sStatStageDenominator[idx];
}

/**
 * Check if a move gets STAB (Same-Type Attack Bonus).
 *
 * @param attacker  The attacking Pokemon.
 * @param moveType  The move's type.
 * @return          TRUE if STAB applies.
 */
BOOL battle_IsSTAB(BattlePokemon* attacker, u8 moveType) {
    return (attacker->type1 == moveType || attacker->type2 == moveType);
}

/**
 * Calculate whether a critical hit occurs.
 *
 * Gen III critical hit rates:
 *   Stage 0: 1/16 (6.25%)
 *   Stage 1: 1/8  (12.5%)  - high-crit moves (e.g., Slash)
 *   Stage 2: 1/4  (25%)    - Focus Energy + high-crit
 *   Stage 3: 1/3  (33.3%)
 *   Stage 4: 1/2  (50%)
 *
 * @param attacker  The attacking Pokemon.
 * @param move      The move data.
 * @return          1 if critical hit, 0 otherwise.
 */
u8 battle_CalcCriticalHit(BattlePokemon* attacker, const MoveData* move) {
    s32 critStage = 0;
    s32 random;
    s32 threshold;

    /* Check for high critical hit ratio flag in move data */
    if (move->flags & 0x01) {
        critStage += 1;
    }

    /* Check for Focus Energy volatile status */
    if (attacker->volatileStatus & VSTATUS_FOCUS) {
        critStage += 2;
    }

    /* Determine threshold based on crit stage */
    switch (critStage) {
        case 0: threshold = 16; break;
        case 1: threshold = 8;  break;
        case 2: threshold = 4;  break;
        case 3: threshold = 3;  break;
        default: threshold = 2; break;
    }

    /* Roll for critical hit */
    random = (u32)fn_800D37CC() % threshold;
    return (random == 0) ? 1 : 0;
}

/**
 * Get a random damage factor between 85 and 100 (inclusive).
 * This is applied as the final multiplier: damage * factor / 100.
 *
 * @return  Random value in range [85, 100].
 */
s32 battle_GetRandomDamageFactor(void) {
    return 85 + ((u32)fn_800D37CC() % 16);
}

/**
 * Calculate damage using the Gen III formula.
 *
 * The core formula:
 *   damage = ((2 * level / 5 + 2) * power * atk / def) / 50 + 2
 *
 * Then modifiers are applied multiplicatively:
 *   - STAB: damage = damage * 3 / 2
 *   - Type effectiveness (per defending type)
 *   - Critical hit: damage * 2
 *   - Random: damage * [85..100] / 100
 *
 * @param attacker    The attacking Pokemon.
 * @param defender    The defending Pokemon.
 * @param move        The move being used.
 * @param isCritical  1 if critical hit, 0 otherwise.
 * @return            The final damage value (minimum 1 if move hits).
 */
s32 battle_CalcDamage(BattlePokemon* attacker, BattlePokemon* defender,
                      const MoveData* move, u8 isCritical) {
    s32 level;
    s32 power;
    s32 attack;
    s32 defense;
    s32 damage;
    u8  moveType;
    u8  effectiveness;
    s32 randomFactor;

    level = attacker->level;
    power = move->basePower;
    moveType = move->type;

    /* A move with 0 base power does no damage (status move) */
    if (power == 0) {
        return 0;
    }

    /* Determine physical or special based on move type (Gen III type-based split) */
    if (sTypeIsPhysical[moveType]) {
        /* Physical: use Attack / Defense */
        attack = attacker->attack;
        defense = defender->defense;

        if (isCritical) {
            /* Critical hits ignore negative attack stages and positive defense stages */
            if (attacker->statStages.attack > 0) {
                attack = battle_ApplyStatStage(attack, attacker->statStages.attack);
            }
            if (defender->statStages.defense < 0) {
                defense = battle_ApplyStatStage(defense, defender->statStages.defense);
            }
        } else {
            attack = battle_ApplyStatStage(attack, attacker->statStages.attack);
            defense = battle_ApplyStatStage(defense, defender->statStages.defense);
        }
    } else {
        /* Special: use Sp.Attack / Sp.Defense */
        attack = attacker->spAttack;
        defense = defender->spDefense;

        if (isCritical) {
            if (attacker->statStages.spAttack > 0) {
                attack = battle_ApplyStatStage(attack, attacker->statStages.spAttack);
            }
            if (defender->statStages.spDefense < 0) {
                defense = battle_ApplyStatStage(defense, defender->statStages.spDefense);
            }
        } else {
            attack = battle_ApplyStatStage(attack, attacker->statStages.spAttack);
            defense = battle_ApplyStatStage(defense, defender->statStages.spDefense);
        }
    }

    /* Prevent division by zero */
    if (defense == 0) defense = 1;
    if (attack == 0) attack = 1;

    /* Core damage formula */
    damage = ((2 * level / 5 + 2) * power * attack / defense) / 50 + 2;

    /* Apply STAB */
    if (battle_IsSTAB(attacker, moveType)) {
        damage = damage * 3 / 2;
    }

    /* Apply type effectiveness (both defending types) */
    effectiveness = battle_GetTypeEffectiveness(moveType,
                                                 defender->type1,
                                                 defender->type2);
    if (effectiveness == TYPE_EFF_IMMUNE) {
        return 0;
    }
    /* effectiveness is stored as: 5 = 0.5x, 10 = 1x, 20 = 2x
     * For two types, the values multiply:
     *   5*5=25 (0.25x), 5*10=50 (0.5x), 10*10=100 (1x),
     *   10*20=200 (2x), 20*20=400 (4x)
     * We normalize by dividing by 100 */
    damage = damage * effectiveness / 100;

    /* Apply critical hit multiplier */
    if (isCritical) {
        damage *= 2;
    }

    /* Apply random factor (85-100) */
    randomFactor = battle_GetRandomDamageFactor();
    damage = damage * randomFactor / 100;

    /* Minimum 1 damage for any damaging move that isn't type-immune */
    if (damage < 1) {
        damage = 1;
    }

    return damage;
}
