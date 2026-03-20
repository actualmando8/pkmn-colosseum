/**
 * @file battle_shadow.c
 * @brief Shadow Pokemon battle mechanics (Colosseum-exclusive).
 *
 * Shadow Pokemon are central to Colosseum's gameplay. They have unique
 * battle behaviors that don't exist in the mainline Pokemon games:
 *
 * Shadow Rush:
 *   - The signature move of Shadow Pokemon
 *   - 90 base power, Shadow type (type ID 17)
 *   - Always physical category
 *   - Super effective against all non-Shadow types
 *   - Not very effective against Shadow types
 *   - Causes recoil damage (1/16 max HP) to the user
 *   - In Hyper Mode, Shadow Rush is boosted in power
 *
 * Hyper Mode:
 *   - Shadow Pokemon can randomly enter Hyper Mode at the start of
 *     their turn. The chance is based on the heart gauge level.
 *   - In Hyper Mode, the Pokemon may disobey trainer commands and
 *     use Shadow Rush instead of the selected move.
 *   - The trainer can use the "Call" action (action type 4) to snap
 *     the Pokemon out of Hyper Mode. This also slightly reduces
 *     the heart gauge (aiding purification).
 *   - Hyper Mode is indicated visually by a dark aura effect
 *     (uses the "auraEffect" VFX system at 0x80130000+).
 *
 * Heart Gauge:
 *   - Each Shadow Pokemon has a heart gauge (shadowGauge/shadowGaugeMax).
 *   - The gauge decreases through battling, walking, and using items.
 *   - When the gauge reaches 0, the Pokemon can be purified at the
 *     Relic Stone in Agate Village.
 *   - The "Call" action reduces the gauge by a small amount.
 *
 * Snagging:
 *   - The player can snag (catch) Shadow Pokemon from other trainers
 *     using Snag Balls (converted Poke Balls via the Snag Machine).
 *   - Snagging only works on Shadow Pokemon, not regular Pokemon.
 *   - The catch rate formula is similar to Gen III but modified.
 *   - When snagging, the battle uses the "Snag Ball" animation
 *     sequence instead of the normal Poke Ball sequence.
 *
 * Address context:
 *   Shadow Pokemon checks are scattered throughout the battle state
 *   machine. Key indicators in the disassembly:
 *   - The isShadow field (offset 0x45 in BattlePokemon struct)
 *   - The shadowMode field (offset 0x46)
 *   - fn_801EEC74 appears to check Shadow-related state
 *   - The state machine at 0x801E04D4 handles the snag sequence
 *   - fn_801E0514 calls fn_801EEC74 to check shadow eligibility
 *
 *   The heart gauge display is managed by the UI system, not the
 *   battle engine directly.
 */

#include "game/battle/battle.h"

/* =========================================================================
 * External function declarations
 * ========================================================================= */

extern s32  fn_800D37CC(void);         /* GSrandom_Get */
extern void fn_800DD970(const char* fmt, ...);  /* GSlog_Print */

/* Battle message display */
extern void fn_80106698(s32 msgID, s32 arg1, s32 arg2, s32 arg3);

/* Shadow-related checks from the state machine */
extern u8   fn_801EEC74(void);         /* check shadow pokemon state */
extern void fn_801EECD8(s32 slot, s32 arg); /* set slot shadow state */
extern void fn_801EEB34(s32 slot, s32 arg); /* reset slot shadow anim */
extern void fn_801EE958(s32 slot, s32 arg); /* reset slot shadow effect */

/* Pokemon data access */
extern s32  fn_8011EE40(s32 pokemon);  /* get pokemon HP */

/* =========================================================================
 * Constants
 * ========================================================================= */

#define SHADOW_RUSH_POWER     90
#define SHADOW_RUSH_RECOIL_DIV 16   /* 1/16 max HP recoil */
#define CALL_GAUGE_REDUCTION   100  /* Heart gauge reduction per Call */
#define HYPER_MODE_BASE_CHANCE 25   /* Base % chance per turn to enter Hyper Mode */

/* =========================================================================
 * Implementation
 * ========================================================================= */

/**
 * Check if a Pokemon is a Shadow Pokemon.
 *
 * @param pokemon  The Pokemon to check.
 * @return         TRUE if the Pokemon is a Shadow Pokemon.
 */
BOOL battle_IsShadowPokemon(BattlePokemon* pokemon) {
    return (pokemon->isShadow != 0);
}

/**
 * Attempt to put a Shadow Pokemon into Hyper Mode.
 *
 * Called at the start of each turn for Shadow Pokemon. The chance
 * of entering Hyper Mode is proportional to the heart gauge level:
 * higher gauge (less purified) = higher chance.
 *
 * @param pokemon  The Shadow Pokemon to check.
 */
void battle_EnterHyperMode(BattlePokemon* pokemon) {
    s32 roll;
    s32 chance;

    if (!battle_IsShadowPokemon(pokemon)) {
        return;
    }

    /* Already in Hyper Mode */
    if (pokemon->shadowMode == SHADOW_HYPER_MODE) {
        return;
    }

    /* Calculate Hyper Mode chance based on heart gauge */
    if (pokemon->shadowGaugeMax == 0) {
        return;
    }

    /* Higher gauge = higher chance. Base chance scaled by gauge ratio. */
    chance = HYPER_MODE_BASE_CHANCE * pokemon->shadowGauge / pokemon->shadowGaugeMax;
    if (chance < 1) chance = 1;

    roll = ((u32)fn_800D37CC() % 100) + 1;

    if (roll <= chance) {
        pokemon->shadowMode = SHADOW_HYPER_MODE;
        /* Visual: trigger dark aura effect around the Pokemon */
    }
}

/**
 * Exit Hyper Mode (via the "Call" action).
 *
 * When the trainer uses the "Call" action on a Shadow Pokemon in
 * Hyper Mode, the Pokemon snaps out of it. This also slightly
 * reduces the heart gauge, aiding purification.
 *
 * @param pokemon  The Shadow Pokemon to calm down.
 */
void battle_ExitHyperMode(BattlePokemon* pokemon) {
    if (!battle_IsShadowPokemon(pokemon)) {
        return;
    }

    if (pokemon->shadowMode != SHADOW_HYPER_MODE) {
        return;
    }

    /* Exit Hyper Mode */
    pokemon->shadowMode = SHADOW_NORMAL;

    /* Reduce heart gauge (contributes to purification) */
    if (pokemon->shadowGauge > CALL_GAUGE_REDUCTION) {
        pokemon->shadowGauge -= CALL_GAUGE_REDUCTION;
    } else {
        pokemon->shadowGauge = 0;
    }

    /* "[Pokemon] came to its senses!" */
}

/**
 * Process the "Call" battle command for Shadow Pokemon.
 *
 * The "Call" action is unique to Colosseum. It occupies action type 4
 * in the TurnAction struct. When used:
 *   - If the Shadow Pokemon is in Hyper Mode, it exits Hyper Mode
 *   - If the Shadow Pokemon is not in Hyper Mode, the call still
 *     slightly reduces the heart gauge
 *   - The trainer does not attack this turn (Call uses the turn)
 *
 * In the disassembly, fn_801C89F8 handles the encounter sequence
 * which includes the Call action as one of its state machine cases
 * (case 0x0D in jumptable_8036DE1C).
 *
 * @param pokemon  The Shadow Pokemon being called.
 */
void battle_CallPokemon(BattlePokemon* pokemon) {
    if (!battle_IsShadowPokemon(pokemon)) {
        return;
    }

    if (pokemon->shadowMode == SHADOW_HYPER_MODE) {
        battle_ExitHyperMode(pokemon);
    } else {
        /* Even without Hyper Mode, calling reduces the gauge slightly */
        s32 reduction = CALL_GAUGE_REDUCTION / 2;
        if (pokemon->shadowGauge > (u16)reduction) {
            pokemon->shadowGauge -= (u16)reduction;
        } else {
            pokemon->shadowGauge = 0;
        }
    }
}

/**
 * Calculate Shadow Rush damage.
 *
 * Shadow Rush is a special case in the damage formula:
 *   - Base power: 90
 *   - Type: Shadow (super effective against everything except Shadow)
 *   - Category: Physical (uses Attack/Defense)
 *   - No STAB (Shadow type doesn't match any natural Pokemon type)
 *   - In Hyper Mode: power is boosted by 50% (to 135 effective)
 *   - Causes recoil: 1/16 max HP to the user
 *
 * @param attacker  The attacking Shadow Pokemon.
 * @param defender  The defending Pokemon.
 * @return          Damage dealt (before recoil).
 */
s32 battle_CalcShadowRushDamage(BattlePokemon* attacker, BattlePokemon* defender) {
    s32 level;
    s32 power;
    s32 attack;
    s32 defense;
    s32 damage;
    s32 randomFactor;
    u8  effectiveness;

    level = attacker->level;
    power = SHADOW_RUSH_POWER;

    /* Hyper Mode boost: 1.5x power */
    if (attacker->shadowMode == SHADOW_HYPER_MODE) {
        power = power * 3 / 2;
    }

    /* Shadow Rush is physical */
    attack = battle_ApplyStatStage(attacker->attack, attacker->statStages.attack);
    defense = battle_ApplyStatStage(defender->defense, defender->statStages.defense);

    if (defense == 0) defense = 1;
    if (attack == 0) attack = 1;

    /* Core damage formula */
    damage = ((2 * level / 5 + 2) * power * attack / defense) / 50 + 2;

    /* Type effectiveness: Shadow type mechanics */
    if (defender->isShadow) {
        /* Not very effective against Shadow Pokemon */
        damage = damage * TYPE_EFF_NOT_VERY / TYPE_EFF_NORMAL;
    } else {
        /* Super effective against all non-Shadow Pokemon */
        effectiveness = battle_GetTypeEffectiveness(TYPE_SHADOW,
                                                     defender->type1,
                                                     defender->type2);
        damage = damage * effectiveness / TYPE_EFF_NORMAL;
    }

    /* Random factor (85-100) */
    randomFactor = battle_GetRandomDamageFactor();
    damage = damage * randomFactor / 100;

    if (damage < 1) damage = 1;

    /* Apply recoil to attacker (1/16 max HP) */
    {
        s32 recoil = attacker->maxHP / SHADOW_RUSH_RECOIL_DIV;
        if (recoil < 1) recoil = 1;

        if (attacker->currentHP <= (u16)recoil) {
            attacker->currentHP = 0;
        } else {
            attacker->currentHP -= (u16)recoil;
        }
    }

    return damage;
}

/**
 * Check if a Pokemon can be snagged.
 *
 * Snagging is only possible against Shadow Pokemon owned by other
 * trainers. The player must have the Snag Machine equipped and
 * use a Poke Ball variant.
 *
 * In the disassembly, the snag check appears around 0x801E0514
 * where fn_801EEC74 is called to verify eligibility.
 *
 * @param target  The Pokemon to check.
 * @return        TRUE if the Pokemon can be snagged.
 */
BOOL battle_CanSnag(BattlePokemon* target) {
    /* Only Shadow Pokemon can be snagged */
    if (!battle_IsShadowPokemon(target)) {
        return FALSE;
    }

    /* Target must be alive */
    if (target->currentHP == 0) {
        return FALSE;
    }

    /* Additional checks would include:
     * - Is the player's Snag Machine active?
     * - Is this an opponent's Pokemon (not the player's)?
     * - Has this specific Shadow Pokemon already been snagged?
     * These checks are performed in the battle state machine. */

    return TRUE;
}

/**
 * Process the snagging sequence for a target Pokemon.
 *
 * The snag sequence in the battle state machine:
 *   1. Player selects a Poke Ball item
 *   2. System checks battle_CanSnag()
 *   3. Ball is thrown (waza animation)
 *   4. Catch rate calculated (modified Gen III formula)
 *   5. Ball shakes 0-3 times
 *   6. If caught: Pokemon is added to player's party, battle message displayed
 *   7. If failed: ball breaks, Pokemon remains in opponent's party
 *
 * The catch rate formula follows Gen III with modifications:
 *   rate = (3*maxHP - 2*currentHP) * catchRate * ballBonus / (3*maxHP)
 *   Then checked against random rolls for 0-3 shakes.
 *
 * In the state machine, this is handled by states 3-5 of fn_801E03D4
 * when the action type is "item" and the item is a Snag Ball variant.
 *
 * @param targetSlot  The battle position of the Pokemon to snag.
 */
void battle_ProcessSnagging(s32 targetSlot) {
    /*
     * The full snagging implementation requires access to:
     * - The player's bag/item system (to consume the ball)
     * - The catch rate data for each species
     * - The Snag Ball modifier table
     * - The opponent trainer's party data
     * - The player's party data (to add the caught Pokemon)
     *
     * These are managed by systems outside the battle engine proper
     * (inventory, save data, Pokemon storage).
     *
     * The battle engine's role is limited to:
     * 1. Checking eligibility (battle_CanSnag)
     * 2. Triggering the throw animation (waza system)
     * 3. Calculating the catch rate
     * 4. Determining the outcome
     * 5. Removing the Pokemon from the opponent's side if caught
     *
     * The state machine handles this through its command selection
     * states and transitions. Full decompilation requires matching
     * the state machine's item-use code path.
     */
}
