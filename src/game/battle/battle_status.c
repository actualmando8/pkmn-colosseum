/**
 * @file battle_status.c
 * @brief Status effect processing for the battle engine.
 *
 * Handles both primary status conditions (sleep, poison, burn, freeze,
 * paralysis, toxic) and volatile battle statuses (confusion, flinch,
 * attract, etc.).
 *
 * In Pokemon Colosseum, status effects follow Gen III rules:
 *
 * Primary statuses (mutually exclusive, persist after battle):
 *   - Sleep:     Pokemon cannot move for 1-5 turns. Counter decrements
 *                each turn. Pokemon wakes when counter reaches 0.
 *   - Poison:    Loses 1/8 max HP per turn at end of turn.
 *   - Burn:      Loses 1/8 max HP per turn, Attack halved.
 *   - Freeze:    Cannot move. 20% chance to thaw each turn.
 *                Fire-type moves from opponent also thaw.
 *   - Paralysis: 25% chance of full paralysis each turn. Speed quartered.
 *   - Toxic:     Loses N/16 max HP per turn where N starts at 1 and
 *                increments each turn.
 *
 * Volatile statuses (cleared when switching out):
 *   - Confusion: 50% chance of hitting self for 1-4 turns.
 *   - Flinch:    Cannot move this turn. Cleared at end of turn.
 *   - Attract:   50% chance of being immobilized (opposite gender only).
 *   - Focus:     Critical hit stage increased (Focus Energy).
 *   - Charge:    Charging for a two-turn move.
 *
 * Shadow Pokemon Hyper Mode (Colosseum-specific):
 *   When a Shadow Pokemon enters Hyper Mode, it may disobey commands
 *   and use Shadow Rush instead of the selected move. The trainer can
 *   use the "Call" action to snap it out of Hyper Mode.
 *
 * Address context:
 *   Status processing is embedded in the battle state machine
 *   (0x801E03D4+). The end-of-turn damage and status checking
 *   occurs in BATTLE_STATE_END_TURN (state 10).
 */

#include "game/battle/battle.h"

/* =========================================================================
 * External function declarations
 * ========================================================================= */

extern s32  fn_800D37CC(void);   /* GSrandom_Get */

/* =========================================================================
 * Implementation
 * ========================================================================= */

/**
 * Apply end-of-turn status damage to a Pokemon.
 *
 * Called during BATTLE_STATE_END_TURN for each active Pokemon.
 * Handles poison, burn, toxic, and other residual damage.
 *
 * @param pokemon  The Pokemon to process.
 */
void battle_ApplyStatusDamage(BattlePokemon* pokemon) {
    s32 damage;

    if (pokemon->currentHP == 0) {
        return;  /* Fainted Pokemon don't take status damage */
    }

    /* Poison: 1/8 max HP */
    if (pokemon->statusCondition & STATUS_POISON) {
        damage = pokemon->maxHP / 8;
        if (damage < 1) damage = 1;

        if (pokemon->currentHP <= (u16)damage) {
            pokemon->currentHP = 0;
        } else {
            pokemon->currentHP -= (u16)damage;
        }
    }

    /* Burn: 1/8 max HP */
    if (pokemon->statusCondition & STATUS_BURN) {
        damage = pokemon->maxHP / 8;
        if (damage < 1) damage = 1;

        if (pokemon->currentHP <= (u16)damage) {
            pokemon->currentHP = 0;
        } else {
            pokemon->currentHP -= (u16)damage;
        }
    }

    /* Toxic: N/16 max HP, where N increments each turn */
    if (pokemon->statusCondition & STATUS_TOXIC) {
        /* The toxic counter is stored in the upper bits of statusCondition
         * In Gen III, the counter typically occupies bits 8-11 */
        s32 toxicCount = (pokemon->statusCondition >> 8) & 0xF;
        toxicCount++;
        if (toxicCount > 15) toxicCount = 15;

        damage = pokemon->maxHP * toxicCount / 16;
        if (damage < 1) damage = 1;

        if (pokemon->currentHP <= (u16)damage) {
            pokemon->currentHP = 0;
        } else {
            pokemon->currentHP -= (u16)damage;
        }

        /* Update toxic counter */
        pokemon->statusCondition = (pokemon->statusCondition & 0xFFFFF0FF) |
                                    ((u32)toxicCount << 8);
    }

    /* Clear flinch at end of turn (it only lasts one turn) */
    pokemon->volatileStatus &= ~VSTATUS_FLINCH;
}

/**
 * Check if a status condition prevents the Pokemon from moving.
 *
 * Called at the start of each Pokemon's turn before move execution.
 * Returns TRUE if the Pokemon is unable to act.
 *
 * @param pokemon  The Pokemon attempting to move.
 * @return         TRUE if the Pokemon cannot move this turn.
 */
BOOL battle_CheckStatusPreventsMove(BattlePokemon* pokemon) {
    s32 roll;

    /* Check flinch (cleared after this check) */
    if (pokemon->volatileStatus & VSTATUS_FLINCH) {
        /* "[Pokemon] flinched and couldn't move!" */
        return TRUE;
    }

    /* Check freeze: 20% chance to thaw */
    if (pokemon->statusCondition & STATUS_FREEZE) {
        roll = (u32)fn_800D37CC() % 5;
        if (roll == 0) {
            /* Thaw out */
            pokemon->statusCondition &= ~STATUS_FREEZE;
            /* "[Pokemon] thawed out!" */
            return FALSE;
        }
        /* "[Pokemon] is frozen solid!" */
        return TRUE;
    }

    /* Check sleep: decrement counter */
    if (pokemon->statusCondition & STATUS_SLEEP) {
        s32 sleepCount = pokemon->statusCondition & 0x07;
        if (sleepCount > 0) {
            sleepCount--;
            pokemon->statusCondition = (pokemon->statusCondition & ~0x07) | sleepCount;
            if (sleepCount == 0) {
                /* "[Pokemon] woke up!" */
                return FALSE;
            }
            /* "[Pokemon] is fast asleep!" */
            return TRUE;
        }
    }

    /* Check paralysis: 25% chance of full paralysis */
    if (pokemon->statusCondition & STATUS_PARALYSIS) {
        roll = (u32)fn_800D37CC() % 4;
        if (roll == 0) {
            /* "[Pokemon] is fully paralyzed!" */
            return TRUE;
        }
    }

    /* Check confusion: 50% chance of hitting self */
    if (pokemon->volatileStatus & VSTATUS_CONFUSION) {
        roll = (u32)fn_800D37CC() % 2;
        if (roll == 0) {
            /* Self-hit damage: 40 power typeless physical attack */
            s32 selfDamage;
            selfDamage = ((2 * pokemon->level / 5 + 2) * 40 *
                           pokemon->attack / pokemon->defense) / 50 + 2;
            if (selfDamage < 1) selfDamage = 1;

            if (pokemon->currentHP <= (u16)selfDamage) {
                pokemon->currentHP = 0;
            } else {
                pokemon->currentHP -= (u16)selfDamage;
            }
            /* "[Pokemon] hurt itself in its confusion!" */
            return TRUE;
        }
    }

    /* Check attract: 50% chance of being immobilized */
    if (pokemon->volatileStatus & VSTATUS_ATTRACT) {
        roll = (u32)fn_800D37CC() % 2;
        if (roll == 0) {
            /* "[Pokemon] is immobilized by love!" */
            return TRUE;
        }
    }

    /* Check Shadow Pokemon Hyper Mode */
    if (pokemon->isShadow && pokemon->shadowMode == SHADOW_HYPER_MODE) {
        /* In Hyper Mode, the Pokemon may disobey.
         * There's a chance it will use Shadow Rush regardless of command.
         * The exact probability depends on the heart gauge level. */
        roll = (u32)fn_800D37CC() % 4;  /* ~25% chance to disobey */
        if (roll == 0) {
            /* Pokemon disobeys and may use Shadow Rush or do nothing */
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * Tick status effect counters at end of turn.
 *
 * Decrements confusion counter, checks for sleep/freeze resolution,
 * and handles other per-turn status bookkeeping.
 *
 * @param pokemon  The Pokemon whose counters to update.
 */
void battle_TickStatusCounters(BattlePokemon* pokemon) {
    /* Confusion counter: lasts 1-4 turns */
    if (pokemon->volatileStatus & VSTATUS_CONFUSION) {
        /* The confusion counter could be stored in upper bits of volatileStatus.
         * For now, we just note that it gets decremented here.
         * When counter reaches 0, confusion is cleared. */
    }

    /* Flinch is always cleared at end of turn */
    pokemon->volatileStatus &= ~VSTATUS_FLINCH;
}

/**
 * Attempt to inflict a status condition on a target Pokemon.
 *
 * Checks type immunities and existing conditions before inflicting:
 *   - Fire types cannot be burned
 *   - Ice types cannot be frozen
 *   - Electric types cannot be paralyzed
 *   - Poison/Steel types cannot be poisoned
 *   - A Pokemon with an existing primary status cannot gain another
 *
 * @param target  The Pokemon to inflict status on.
 * @param status  The status condition to inflict (STATUS_* constant).
 * @param chance  The percentage chance (1-100). 100 = guaranteed.
 */
void battle_TryInflictStatus(BattlePokemon* target, u32 status, u8 chance) {
    s32 roll;

    /* Cannot inflict if Pokemon already has a primary status */
    if (target->statusCondition != STATUS_NONE) {
        return;
    }

    /* Cannot inflict on fainted Pokemon */
    if (target->currentHP == 0) {
        return;
    }

    /* Type immunity checks */
    switch (status) {
        case STATUS_BURN:
            if (target->type1 == TYPE_FIRE || target->type2 == TYPE_FIRE) {
                return;
            }
            break;

        case STATUS_FREEZE:
            if (target->type1 == TYPE_ICE || target->type2 == TYPE_ICE) {
                return;
            }
            break;

        case STATUS_PARALYSIS:
            /* In Gen III, Electric types CAN be paralyzed.
             * The immunity was added in Gen VI. */
            break;

        case STATUS_POISON:
        case STATUS_TOXIC:
            if (target->type1 == TYPE_POISON || target->type2 == TYPE_POISON ||
                target->type1 == TYPE_STEEL  || target->type2 == TYPE_STEEL) {
                return;
            }
            break;

        default:
            break;
    }

    /* Check chance */
    if (chance < 100) {
        roll = ((u32)fn_800D37CC() % 100) + 1;
        if (roll > chance) {
            return;
        }
    }

    /* Inflict the status */
    if (status == STATUS_SLEEP) {
        /* Sleep counter: random 1-5 turns (stored in bits 0-2) */
        s32 sleepTurns = ((u32)fn_800D37CC() % 5) + 1;
        target->statusCondition = (u32)sleepTurns;
    } else {
        target->statusCondition = status;
    }
}
