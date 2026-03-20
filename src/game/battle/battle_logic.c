/**
 * @file battle_logic.c
 * @brief Battle engine logic -- moves, types, status, damage, AI, shadow (merged TU).
 *
 * This is a single translation unit containing all interleaved battle logic
 * functions. Link order analysis showed that battle_move.c, battle_type.c,
 * battle_status.c, battle_damage.c, battle_ai.c, and battle_shadow.c had
 * interleaved functions throughout the 0x801E03D4-0x801EF02C address range,
 * confirming they were originally compiled as one TU.
 *
 * battle_main.c remains separate (address range 0x801EF02C-0x801F000C).
 *
 * Subsystems:
 *   - Move execution:    accuracy, effects, turn order, priority
 *   - Type effectiveness: Gen III type chart + Shadow type
 *   - Status effects:     primary/volatile status, counters, immunities
 *   - Damage calculation: Gen III formula, STAB, crits, stat stages
 *   - AI:                 scoring-based move/target selection
 *   - Shadow Pokemon:     Hyper Mode, Shadow Rush, snagging, Call action
 *
 * Address range: 0x801E03D4 - 0x801EF02C
 */

#include "game/battle/battle.h"

/* =========================================================================
 * External function declarations
 * ========================================================================= */

/* Random number generation */
extern s32  fn_800D37CC(void);   /* GSrandom_Get */
extern void fn_800D3074(s32 seed); /* GSrandom_Seed */

/* Debug logging */
extern void fn_800DD970(const char* fmt, ...);  /* GSlog_Print */

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

/* Shadow-related checks from the state machine */
extern u8   fn_801EEC74(void);         /* check shadow pokemon state */
extern void fn_801EECD8(s32 slot, s32 arg); /* set slot shadow state */
extern void fn_801EEB34(s32 slot, s32 arg); /* reset slot shadow anim */
extern void fn_801EE958(s32 slot, s32 arg); /* reset slot shadow effect */

/* =========================================================================
 * Type Effectiveness Table (from battle_type.c)
 * ========================================================================= */

/**
 * Gen III type effectiveness chart.
 * Each entry: { attacking_type, defending_type, effectiveness }
 * Only non-neutral matchups are listed (neutral is the default).
 * Terminated by 0xFF in the attacking_type field.
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
    { TYPE_SHADOW,   TYPE_SHADOW,   TYPE_EFF_NOT_VERY },

    /* Sentinel */
    { 0xFF, 0xFF, 0xFF }
};

/* =========================================================================
 * Damage Calculation Constants (from battle_damage.c)
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
 * AI Constants (from battle_ai.c)
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
 * Shadow Pokemon Constants (from battle_shadow.c)
 * ========================================================================= */

#define SHADOW_RUSH_POWER     90
#define SHADOW_RUSH_RECOIL_DIV 16   /* 1/16 max HP recoil */
#define CALL_GAUGE_REDUCTION   100  /* Heart gauge reduction per Call */
#define HYPER_MODE_BASE_CHANCE 25   /* Base % chance per turn to enter Hyper Mode */

/* #######################################################################
 * MOVE EXECUTION (from battle_move.c)
 * ####################################################################### */

/**
 * Check if a move hits based on accuracy.
 *
 * Gen III accuracy formula:
 *   hitChance = moveAccuracy * accStage / evaStage
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

    /* Roll for hit */
    roll = ((u32)fn_800D37CC() % 100) + 1;

    return (roll <= accuracy);
}

/**
 * Apply a move's secondary effect to the target.
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
            break;
    }
}

/**
 * Execute a move from one battle slot against another.
 * Stub until full state machine is decompiled.
 */
s32 battle_ExecuteMove(s32 attackerSlot, s32 targetSlot, u16 moveID) {
    return 0;
}

/**
 * Determine turn order for all pending actions.
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

/* #######################################################################
 * TYPE EFFECTIVENESS (from battle_type.c)
 * ####################################################################### */

/**
 * Look up type effectiveness for a single type matchup.
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

    /* Multiply and normalize */
    return (u8)((s32)eff1 * (s32)eff2 / TYPE_EFF_NORMAL);
}

/* #######################################################################
 * STATUS EFFECTS (from battle_status.c)
 * ####################################################################### */

/**
 * Apply end-of-turn status damage to a Pokemon.
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
 */
BOOL battle_CheckStatusPreventsMove(BattlePokemon* pokemon) {
    s32 roll;

    /* Check flinch (cleared after this check) */
    if (pokemon->volatileStatus & VSTATUS_FLINCH) {
        return TRUE;
    }

    /* Check freeze: 20% chance to thaw */
    if (pokemon->statusCondition & STATUS_FREEZE) {
        roll = (u32)fn_800D37CC() % 5;
        if (roll == 0) {
            pokemon->statusCondition &= ~STATUS_FREEZE;
            return FALSE;
        }
        return TRUE;
    }

    /* Check sleep: decrement counter */
    if (pokemon->statusCondition & STATUS_SLEEP) {
        s32 sleepCount = pokemon->statusCondition & 0x07;
        if (sleepCount > 0) {
            sleepCount--;
            pokemon->statusCondition = (pokemon->statusCondition & ~0x07) | sleepCount;
            if (sleepCount == 0) {
                return FALSE;
            }
            return TRUE;
        }
    }

    /* Check paralysis: 25% chance of full paralysis */
    if (pokemon->statusCondition & STATUS_PARALYSIS) {
        roll = (u32)fn_800D37CC() % 4;
        if (roll == 0) {
            return TRUE;
        }
    }

    /* Check confusion: 50% chance of hitting self */
    if (pokemon->volatileStatus & VSTATUS_CONFUSION) {
        roll = (u32)fn_800D37CC() % 2;
        if (roll == 0) {
            s32 selfDamage;
            selfDamage = ((2 * pokemon->level / 5 + 2) * 40 *
                           pokemon->attack / pokemon->defense) / 50 + 2;
            if (selfDamage < 1) selfDamage = 1;

            if (pokemon->currentHP <= (u16)selfDamage) {
                pokemon->currentHP = 0;
            } else {
                pokemon->currentHP -= (u16)selfDamage;
            }
            return TRUE;
        }
    }

    /* Check attract: 50% chance of being immobilized */
    if (pokemon->volatileStatus & VSTATUS_ATTRACT) {
        roll = (u32)fn_800D37CC() % 2;
        if (roll == 0) {
            return TRUE;
        }
    }

    /* Check Shadow Pokemon Hyper Mode */
    if (pokemon->isShadow && pokemon->shadowMode == SHADOW_HYPER_MODE) {
        roll = (u32)fn_800D37CC() % 4;  /* ~25% chance to disobey */
        if (roll == 0) {
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * Tick status effect counters at end of turn.
 */
void battle_TickStatusCounters(BattlePokemon* pokemon) {
    /* Confusion counter: lasts 1-4 turns */
    if (pokemon->volatileStatus & VSTATUS_CONFUSION) {
        /* The confusion counter could be stored in upper bits of volatileStatus. */
    }

    /* Flinch is always cleared at end of turn */
    pokemon->volatileStatus &= ~VSTATUS_FLINCH;
}

/**
 * Attempt to inflict a status condition on a target Pokemon.
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
        s32 sleepTurns = ((u32)fn_800D37CC() % 5) + 1;
        target->statusCondition = (u32)sleepTurns;
    } else {
        target->statusCondition = status;
    }
}

/* #######################################################################
 * DAMAGE CALCULATION (from battle_damage.c)
 * ####################################################################### */

/**
 * Apply a stat stage modifier to a base stat value.
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
 */
BOOL battle_IsSTAB(BattlePokemon* attacker, u8 moveType) {
    return (attacker->type1 == moveType || attacker->type2 == moveType);
}

/**
 * Calculate whether a critical hit occurs.
 */
u8 battle_CalcCriticalHit(BattlePokemon* attacker, const MoveData* move) {
    s32 critStage = 0;
    s32 random;
    s32 threshold;

    if (move->flags & 0x01) {
        critStage += 1;
    }

    if (attacker->volatileStatus & VSTATUS_FOCUS) {
        critStage += 2;
    }

    switch (critStage) {
        case 0: threshold = 16; break;
        case 1: threshold = 8;  break;
        case 2: threshold = 4;  break;
        case 3: threshold = 3;  break;
        default: threshold = 2; break;
    }

    random = (u32)fn_800D37CC() % threshold;
    return (random == 0) ? 1 : 0;
}

/**
 * Get a random damage factor between 85 and 100 (inclusive).
 */
s32 battle_GetRandomDamageFactor(void) {
    return 85 + ((u32)fn_800D37CC() % 16);
}

/**
 * Calculate damage using the Gen III formula.
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
        attack = attacker->attack;
        defense = defender->defense;

        if (isCritical) {
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

/* #######################################################################
 * AI (from battle_ai.c)
 * ####################################################################### */

/**
 * Evaluate a single move against a single target.
 */
s32 battle_AIEvaluateMove(s32 aiSlot, s32 targetSlot, u16 moveID) {
    s32 score;

    score = AI_SCORE_BASE;

    /* Add random variance */
    score += ((u32)fn_800D37CC() % AI_SCORE_RANDOM_RANGE) -
             (AI_SCORE_RANDOM_RANGE / 2);

    return score;
}

/**
 * Choose an action for an AI-controlled trainer's Pokemon.
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
    bestTarget = BATTLE_POS_PLAYER_LEFT;

    for (moveIdx = 0; moveIdx < 4; moveIdx++) {
        for (targetIdx = 0; targetIdx < BATTLE_TOTAL_POKEMON; targetIdx++) {
            if (trainerSlot == 1) {
                if (targetIdx >= BATTLE_POS_ENEMY_LEFT) {
                    continue;
                }
            } else {
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
    outAction->priority = 0;
    outAction->moveID = 0;
    outAction->speedValue = 0;
}

/* #######################################################################
 * SHADOW POKEMON (from battle_shadow.c)
 * ####################################################################### */

/**
 * Check if a Pokemon is a Shadow Pokemon.
 */
BOOL battle_IsShadowPokemon(BattlePokemon* pokemon) {
    return (pokemon->isShadow != 0);
}

/**
 * Attempt to put a Shadow Pokemon into Hyper Mode.
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

    chance = HYPER_MODE_BASE_CHANCE * pokemon->shadowGauge / pokemon->shadowGaugeMax;
    if (chance < 1) chance = 1;

    roll = ((u32)fn_800D37CC() % 100) + 1;

    if (roll <= chance) {
        pokemon->shadowMode = SHADOW_HYPER_MODE;
    }
}

/**
 * Exit Hyper Mode (via the "Call" action).
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
}

/**
 * Process the "Call" battle command for Shadow Pokemon.
 */
void battle_CallPokemon(BattlePokemon* pokemon) {
    if (!battle_IsShadowPokemon(pokemon)) {
        return;
    }

    if (pokemon->shadowMode == SHADOW_HYPER_MODE) {
        battle_ExitHyperMode(pokemon);
    } else {
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
        damage = damage * TYPE_EFF_NOT_VERY / TYPE_EFF_NORMAL;
    } else {
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
 */
BOOL battle_CanSnag(BattlePokemon* target) {
    if (!battle_IsShadowPokemon(target)) {
        return FALSE;
    }

    if (target->currentHP == 0) {
        return FALSE;
    }

    return TRUE;
}

/**
 * Process the snagging sequence for a target Pokemon.
 */
void battle_ProcessSnagging(s32 targetSlot) {
    /* Stub -- full implementation requires item/party/save systems */
}

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 8 functions matched
 * =================================================================== */

extern u8 lbl_8047B420;
extern u32 lbl_8047B424;
extern u8 lbl_8047B434;
extern u8 lbl_8047B5C1;

/* Address: 0x801E11CC | Size: 0x8 | Pattern: sda_getter */
u8 fn_801E11CC(void) {
    return lbl_8047B434;
}

/* Address: 0x801E11E0 | Size: 0x8 | Pattern: sda_getter */
u32 fn_801E11E0(void) {
    return lbl_8047B424;
}

/* Address: 0x801E11E8 | Size: 0x8 | Pattern: sda_getter */
u8 fn_801E11E8(void) {
    return lbl_8047B420;
}

/* Address: 0x801ED640 | Size: 0x8 | Pattern: sda_setter */
void fn_801ED640(u8 val) {
    lbl_8047B5C1 = val;
}

/* Address: 0x801EE034 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801EE034(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}

/* Address: 0x801EE04C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801EE04C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x1]);
}

/* Address: 0x801EE064 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801EE064(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}

/* Address: 0x801EE468 | Size: 0x8 | Pattern: return_constant */
u32 fn_801EE468(void) { return 48; }
