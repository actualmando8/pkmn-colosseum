/**
 * @file battle_ai.c
 * @brief Trainer AI decision making for the battle engine.
 *
 * The AI system chooses actions for computer-controlled trainers in
 * Colosseum's double battles. Each AI trainer controls 2 Pokemon
 * simultaneously and must make decisions for both each turn.
 *
 * AI architecture (from disassembly analysis):
 *
 * The AI appears to be a scoring-based system where each possible
 * action is evaluated and assigned a score. The AI then selects
 * the action with the highest score (with some randomness for
 * lower-difficulty trainers).
 *
 * Scoring factors identified:
 *   - Type effectiveness of moves against targets
 *   - Remaining HP of the AI's Pokemon
 *   - Remaining HP of the player's Pokemon
 *   - Whether a move would KO the target
 *   - Status moves vs. damaging moves
 *   - Switching benefit (type advantage, health)
 *   - Item usage (healing, status removal)
 *   - Target selection in doubles (spread moves vs. focused fire)
 *
 * AI difficulty levels (from menuCB_Battle.c strings):
 *   The game uses different battle modes (BATTLEMODE_BATTLEYAMA100
 *   from menuToolBattle.c) which affect AI behavior. Story mode
 *   trainers generally use simpler AI, while Colosseum challenge
 *   trainers (Mt. Battle, Colosseum towers) use more sophisticated
 *   evaluation.
 *
 * Doubles-specific AI considerations:
 *   - Avoid hitting ally with spread moves (Earthquake, Surf)
 *   - Coordinate attacks on the same target for KOs
 *   - Use Protect strategically to scout opponent moves
 *   - Consider switching to counter opponent type matchups
 *
 * Address context:
 *   The AI evaluation is embedded in the battle state machine's
 *   command selection state (BATTLE_STATE_COMMAND_SEL). When it's
 *   the AI's turn, the state machine calls into functions at
 *   approximately 0x801E5F44 which use mulli r3, r31, 0xE0 to
 *   index into the Pokemon data array, evaluating each possible
 *   move against each possible target.
 *
 *   The scoring loop at 0x801E6124 (mulli r3, r0, 0xE0) iterates
 *   over opponent Pokemon data, and the block at 0x801E650C-651C
 *   processes all 6 Pokemon in a party (mulli for indices 0-5
 *   with stride 0xE0).
 */

#include "game/battle/battle.h"

/* =========================================================================
 * External function declarations
 * ========================================================================= */

extern s32  fn_800D37CC(void);   /* GSrandom_Get */

/* Pokemon data access */
extern s32  fn_80129280(s32 side, s32 slotType);
extern s32  fn_8012AC08(s32 party, u16 slotIdx);
extern s32  fn_8011EE40(s32 pokemon);

/* =========================================================================
 * Constants
 * ========================================================================= */

#define AI_SCORE_BASE          100
#define AI_SCORE_TYPE_BONUS     40   /* Bonus for super-effective moves */
#define AI_SCORE_TYPE_PENALTY  -20   /* Penalty for not-very-effective moves */
#define AI_SCORE_KO_BONUS       60   /* Bonus if move would KO target */
#define AI_SCORE_STAB_BONUS     15   /* Bonus for STAB moves */
#define AI_SCORE_STATUS_BONUS   30   /* Bonus for status moves on healthy targets */
#define AI_SCORE_ALLY_HIT_PENALTY -80 /* Penalty for hitting ally with spread move */
#define AI_SCORE_RANDOM_RANGE   20   /* Random variance for less predictable AI */

/* =========================================================================
 * Implementation
 * ========================================================================= */

/**
 * Evaluate a single move against a single target.
 *
 * Returns a score indicating how good this move-target combination is.
 * Higher scores are better.
 *
 * @param aiSlot     The AI Pokemon's battle slot.
 * @param targetSlot The target's battle slot.
 * @param moveID     The move to evaluate.
 * @return           Score value (higher = better choice).
 */
s32 battle_AIEvaluateMove(s32 aiSlot, s32 targetSlot, u16 moveID) {
    s32 score;

    /*
     * The full AI evaluation requires access to the move data table,
     * Pokemon stats, and type information. The scoring logic is
     * embedded in the battle state machine at fn_801E03D4.
     *
     * Observed patterns from the disassembly:
     *
     * 1. The AI loads Pokemon data using mulli rN, rN, 0xE0 to index
     *    into the party array. Each Pokemon is 224 bytes.
     *
     * 2. For each of the AI Pokemon's 4 moves, and for each of the
     *    4 possible targets (2 opponents + 2 allies), a score is
     *    calculated.
     *
     * 3. The score considers:
     *    - Move power * type effectiveness
     *    - Whether the move would reduce target HP to 0
     *    - Status conditions (don't inflict what's already there)
     *    - PP remaining (prefer moves with more PP)
     *    - Random factor for unpredictability
     *
     * 4. The best (move, target) pair is selected.
     *
     * 5. If no offensive move scores well, the AI may choose to
     *    switch Pokemon or use an item.
     *
     * This stub returns a base score. Full implementation requires
     * the move data table and complete Pokemon data accessors.
     */

    score = AI_SCORE_BASE;

    /* Add random variance */
    score += ((u32)fn_800D37CC() % AI_SCORE_RANDOM_RANGE) -
             (AI_SCORE_RANDOM_RANGE / 2);

    return score;
}

/**
 * Choose an action for an AI-controlled trainer's Pokemon.
 *
 * Evaluates all possible actions (4 moves x 4 targets + switch + item)
 * and selects the best one.
 *
 * In double battles, the AI must also coordinate between its two
 * active Pokemon. The second Pokemon's choice may be influenced by
 * what the first Pokemon is doing (e.g., don't both target the same
 * enemy if one can KO it alone).
 *
 * @param trainerSlot  The trainer's slot (0 = player, 1 = opponent).
 * @param outAction    Output: the chosen action.
 */
void battle_AIChooseAction(s32 trainerSlot, TurnAction* outAction) {
    s32 bestScore;
    s32 score;
    s32 bestMove;
    s32 bestTarget;
    s32 moveIdx;
    s32 targetIdx;

    bestScore = -9999;
    bestMove = 0;
    bestTarget = BATTLE_POS_PLAYER_LEFT;  /* Default target */

    /*
     * Iterate over all move-target combinations and score each.
     *
     * In the original binary, this loop at ~0x801E5F44 iterates:
     *   for each move (0-3):
     *     for each target (0-3):
     *       score = evaluate(move, target)
     *       if score > bestScore: update best
     *
     * The AI also considers:
     *   - Switching to a better matchup
     *   - Using items (potions, status heals)
     *   - Using "Call" on Shadow Pokemon in Hyper Mode
     *
     * For story mode trainers, there may be scripted behavior
     * overrides via the script interpreter (psinterpret.c).
     */

    for (moveIdx = 0; moveIdx < 4; moveIdx++) {
        for (targetIdx = 0; targetIdx < BATTLE_TOTAL_POKEMON; targetIdx++) {
            /* Skip targeting own side's Pokemon with damaging moves
             * (simplified - full check needed for healing moves) */
            if (trainerSlot == 1) {
                /* AI is opponent: targets are player slots 0-1 */
                if (targetIdx >= BATTLE_POS_ENEMY_LEFT) {
                    continue;
                }
            } else {
                /* AI is player side: targets are enemy slots 2-3 */
                if (targetIdx < BATTLE_POS_ENEMY_LEFT) {
                    continue;
                }
            }

            score = battle_AIEvaluateMove(trainerSlot, targetIdx, 0);

            if (score > bestScore) {
                bestScore = score;
                bestMove = moveIdx;
                bestTarget = targetIdx;
            }
        }
    }

    /* Fill in the action */
    outAction->actionType = 0;  /* Fight */
    outAction->moveIndex = (u8)bestMove;
    outAction->targetSlot = (u8)bestTarget;
    outAction->priority = 0;    /* Filled in later based on move data */
    outAction->moveID = 0;      /* Filled in later from Pokemon move list */
    outAction->speedValue = 0;  /* Filled in later from Pokemon speed stat */
}
