/**
 * @file battle_move.c
 * @brief Move execution pipeline for the battle engine.
 *
 * The move execution pipeline handles the complete sequence of events
 * when a Pokemon uses a move in battle:
 *
 *   1. Validate the move can be used (PP check, status check)
 *   2. Check accuracy and determine if the move hits
 *   3. For damaging moves: calculate and apply damage
 *   4. For status moves: apply the effect
 *   5. Process secondary effects (stat changes, status infliction)
 *   6. Trigger ability reactions
 *   7. Update PP
 *
 * In Colosseum's double battle format, target selection is critical:
 *   - Single-target moves can hit any of the 4 Pokemon on field
 *   - Spread moves hit both opponents (reduced to 50% power)
 *   - Self-targeting moves affect only the user
 *   - Partner-targeting moves affect the ally
 *
 * The move execution is driven by the battle state machine at
 * 0x801E03D4 (fn_801E03D4 / battleStateMachine_Main). Each state
 * transition is controlled by the 13-entry jump table at
 * jumptable_803751B8. The execution flow is:
 *
 *   BATTLE_STATE_COMMAND_SEL (3) -> BATTLE_STATE_TARGET_SEL (4)
 *     -> BATTLE_STATE_TURN_ORDER (5) -> BATTLE_STATE_EXECUTE_MOVE (6)
 *     -> BATTLE_STATE_APPLY_DAMAGE (7) -> BATTLE_STATE_CHECK_FAINT (8)
 *
 * The actual move animation is handled by the Waza system (0x801D7230+)
 * which loads and plays visual sequences for each move.
 *
 * Address context:
 *   Move execution logic is embedded in the state machine cases.
 *   The state at 0x801E04E4 handles move validation.
 *   The state at 0x801E054C handles the waza animation trigger.
 *   The state at 0x801E0584 handles the command completion.
 *   fn_80106698 is called repeatedly to display battle messages.
 */

#include "game/battle/battle.h"

/* =========================================================================
 * External function declarations
 * ========================================================================= */

/* Random number generation */
extern s32  fn_800D37CC(void);   /* GSrandom_Get */

/* Battle message display */
extern void fn_80106698(s32 msgID, s32 arg1, s32 arg2, s32 arg3);

/* Pokemon data access (People/NPC system) */
extern s32  fn_80129280(s32 side, s32 slotType);    /* get battle party */
extern s32  fn_8012AC08(s32 party, u16 slotIdx);    /* get pokemon from party */
extern s32  fn_80129D64(s32 pokemon, s32 move);      /* check move validity */
extern s32  fn_8011EE40(s32 pokemon);                /* get pokemon HP */
extern s32  fn_8011F4F0(s32 pokemon);                /* get pokemon species */

/* Waza (move animation) system */
extern void fn_801DAEF8(s32 count);      /* load waza data */
extern void fn_801D7464(void);           /* wazaSequenceLoad */
extern void fn_801D7B94(void);           /* wazaSequenceUpdate */
extern void fn_801D84F4(void);           /* wazaSequenceEntryStart */

/* Battle state checks */
extern s32  fn_8001E184(void);           /* wait for event completion */
extern s32  fn_8001BDF4(s32 a, s32 b, s32 c); /* check condition */

/* Move data lookup */
extern void fn_80132A38(s32 msgType, s32 species);  /* display move name */

/* =========================================================================
 * Implementation
 * ========================================================================= */

/**
 * Check if a move hits based on accuracy.
 *
 * Gen III accuracy formula:
 *   hitChance = moveAccuracy * accStage / evaStage
 *
 * Accuracy/evasion stage modifiers:
 *   Stage -6: 3/9  Stage -5: 3/8  Stage -4: 3/7  Stage -3: 3/6
 *   Stage -2: 3/5  Stage -1: 3/4  Stage  0: 3/3
 *   Stage +1: 4/3  Stage +2: 5/3  Stage +3: 6/3
 *   Stage +4: 7/3  Stage +5: 8/3  Stage +6: 9/3
 *
 * @param attacker  The attacking Pokemon.
 * @param defender  The defending Pokemon.
 * @param move      The move data.
 * @return          TRUE if the move hits.
 */
BOOL battle_CheckAccuracy(BattlePokemon* attacker, BattlePokemon* defender,
                          const MoveData* move) {
    s32 accuracy;
    s32 accStage;
    s32 roll;

    /* Accuracy of 0 means the move always hits (e.g., Swift, Aerial Ace) */
    if (move->accuracy == 0) {
        return TRUE;
    }

    accuracy = move->accuracy;

    /* Apply accuracy/evasion stage modifiers */
    accStage = attacker->statStages.accuracy - defender->statStages.evasion;

    /* Clamp to [-6, +6] */
    if (accStage < -6) accStage = -6;
    if (accStage >  6) accStage =  6;

    /* Apply stage modifier using the accuracy/evasion table */
    if (accStage >= 0) {
        accuracy = accuracy * (3 + accStage) / 3;
    } else {
        accuracy = accuracy * 3 / (3 - accStage);
    }

    /* Cap at 100 (some abilities can raise above 100, but base cap is 100) */
    /* Note: in Gen III, accuracy can exceed 100 but is effectively capped
     * because the random roll is 1-100. */

    /* Roll for hit */
    roll = ((u32)fn_800D37CC() % 100) + 1;

    return (roll <= accuracy);
}

/**
 * Apply a move's secondary effect to the target.
 *
 * Secondary effects include stat changes, status conditions,
 * and other modifications that happen after damage is dealt.
 * Each effect has an associated chance (effectChance field).
 *
 * @param target  The target Pokemon.
 * @param move    The move data containing the effect information.
 */
void battle_ApplyMoveEffect(BattlePokemon* target, const MoveData* move) {
    s32 roll;

    /* No effect to apply */
    if (move->effect == 0) {
        return;
    }

    /* Check effect chance (0 means always, otherwise percentage) */
    if (move->effectChance > 0) {
        roll = ((u32)fn_800D37CC() % 100) + 1;
        if (roll > move->effectChance) {
            return;
        }
    }

    /* Apply the effect based on effect ID.
     * The full effect table is extensive (200+ effects in Gen III).
     * Key effects for Colosseum:
     *
     *   Effect 1:  Lower target's Attack by 1 stage
     *   Effect 2:  Lower target's Defense by 1 stage
     *   Effect 3:  Lower target's Speed by 1 stage
     *   Effect 4:  Lower target's Sp.Attack by 1 stage
     *   Effect 5:  Lower target's Sp.Defense by 1 stage
     *   Effect 6:  Lower target's Accuracy by 1 stage
     *   Effect 7:  Raise user's Attack by 1 stage
     *   Effect 8:  Raise user's Defense by 1 stage
     *   Effect 9:  Raise user's Speed by 1 stage
     *   Effect 10: Raise user's Sp.Attack by 1 stage
     *   Effect 11: Raise user's Sp.Defense by 1 stage
     *   Effect 30: Flinch
     *   Effect 31: Burn
     *   Effect 32: Freeze
     *   Effect 33: Paralyze
     *   Effect 34: Poison
     *   Effect 35: Confusion
     *   Effect 36: Sleep
     *
     * The full effect handler is implemented in the battle state machine
     * and is too large to decompile standalone. This function provides
     * the framework. */

    switch (move->effect) {
        /* Stat lowering effects */
        case 1:
            if (target->statStages.attack > -6) target->statStages.attack--;
            break;
        case 2:
            if (target->statStages.defense > -6) target->statStages.defense--;
            break;
        case 3:
            if (target->statStages.speed > -6) target->statStages.speed--;
            break;
        case 4:
            if (target->statStages.spAttack > -6) target->statStages.spAttack--;
            break;
        case 5:
            if (target->statStages.spDefense > -6) target->statStages.spDefense--;
            break;
        case 6:
            if (target->statStages.accuracy > -6) target->statStages.accuracy--;
            break;

        /* Volatile status effects */
        case 30:
            target->volatileStatus |= VSTATUS_FLINCH;
            break;

        /* Primary status effects */
        case 31:
            battle_TryInflictStatus(target, STATUS_BURN, 100);
            break;
        case 32:
            battle_TryInflictStatus(target, STATUS_FREEZE, 100);
            break;
        case 33:
            battle_TryInflictStatus(target, STATUS_PARALYSIS, 100);
            break;
        case 34:
            battle_TryInflictStatus(target, STATUS_POISON, 100);
            break;
        case 35:
            target->volatileStatus |= VSTATUS_CONFUSION;
            break;

        default:
            /* Many more effects exist but require full state machine
             * decompilation to implement correctly */
            break;
    }
}

/**
 * Execute a move from one battle slot against another.
 *
 * This is the high-level move execution function that orchestrates
 * the complete sequence: accuracy check, damage calculation, effect
 * application, and PP deduction.
 *
 * In the original binary, this logic is spread across multiple state
 * machine states. This function consolidates the core flow.
 *
 * @param attackerSlot  The attacker's battle position (0-3).
 * @param targetSlot    The target's battle position (0-3).
 * @param moveID        The move ID to execute.
 * @return              Damage dealt (0 if missed or status move).
 */
s32 battle_ExecuteMove(s32 attackerSlot, s32 targetSlot, u16 moveID) {
    /*
     * The full implementation requires access to the battle Pokemon
     * array and move data tables, which are stored in BSS at
     * lbl_8046D500 and accessed via the People system functions.
     *
     * The state machine at fn_801E03D4 handles this sequence:
     *
     * State 3 (COMMAND_SEL):
     *   - Display message 0x3B21 "What will [Pokemon] do?"
     *   - Wait for player/AI input (fn_8001E184)
     *   - Transition to state 2 or 4 based on result
     *
     * State 4 (TARGET_SEL):
     *   - Get battle party via fn_80129280(0, 2)
     *   - Get Pokemon from party via fn_8012AC08(party, slot)
     *   - Check move validity via fn_80129D64
     *   - Transition to state 5 on success, 11 on failure
     *
     * State 5 (TURN_ORDER):
     *   - Determine execution order based on speed and priority
     *   - Transition to state 6
     *
     * State 6 (EXECUTE_MOVE):
     *   - Get target Pokemon species via fn_8011F4F0
     *   - Display move name via fn_80132A38(0x32, species)
     *   - Display message 0x3B24
     *   - Transition to state 12 (cleanup)
     *
     * The actual damage application happens through additional
     * functions called from within these state handlers.
     *
     * For now, return 0 as a stub until the full state machine
     * is decompiled with all necessary data accessors.
     */
    return 0;
}

/**
 * Determine turn order for all pending actions.
 *
 * In Colosseum's double battles, up to 4 actions occur per turn.
 * Turn order is determined by:
 *   1. Priority (higher priority moves go first)
 *   2. Speed (faster Pokemon go first among same-priority moves)
 *   3. Random tiebreaker for equal speed
 *
 * Special cases:
 *   - "Call" action (for Shadow Pokemon in Hyper Mode) has priority 0
 *   - Item use has priority +6 (always goes first in its bracket)
 *   - Switching out has priority +6
 *   - Quick Claw can randomly give +1 priority bracket
 *
 * @param actions  Array of turn actions to sort.
 * @param count    Number of actions (typically 4 in double battle).
 */
void battle_DetermineTurnOrder(TurnAction actions[], s32 count) {
    s32 i, j;
    TurnAction temp;

    /* Simple insertion sort by priority (descending), then speed (descending) */
    for (i = 1; i < count; i++) {
        temp = actions[i];
        j = i - 1;

        while (j >= 0 && battle_ComparePriority(&actions[j], &temp) < 0) {
            actions[j + 1] = actions[j];
            j--;
        }
        actions[j + 1] = temp;
    }
}

/**
 * Compare two turn actions for priority ordering.
 *
 * @return  Positive if a should go before b, negative if b first, 0 if tie.
 */
s32 battle_ComparePriority(const TurnAction* a, const TurnAction* b) {
    /* Higher priority goes first */
    if (a->priority != b->priority) {
        return (s32)a->priority - (s32)b->priority;
    }

    /* Same priority: higher speed goes first */
    if (a->speedValue != b->speedValue) {
        return (s32)a->speedValue - (s32)b->speedValue;
    }

    /* Speed tie: random tiebreaker */
    return ((u32)fn_800D37CC() & 1) ? 1 : -1;
}
