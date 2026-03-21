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
extern u32 lbl_8047B428;
extern u32 lbl_8047B42C;
extern u32 lbl_8047B430;
extern u8 lbl_8047B434;
extern u32 lbl_8047B438;
extern u8 lbl_8047B5C1;

extern void* fn_800F92D4(u32 size);
extern void fn_801E25C8(void);

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

/* #######################################################################
 * COVERAGE STUBS: battle state machine functions (0x801E03D4 - 0x801EF02C)
 * 138 functions remaining for full coverage of battle_logic.c TU.
 * These are pragma stubs for linker coverage -- replace with real
 * decompilations as disassembly analysis proceeds.
 * ####################################################################### */

#pragma push
#pragma force_active on

/* 0x801E03D4 | size: 0x388 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E03D4(void) {
    extern void fn_80029760();
    extern void fn_801069FC();
    extern void fn_80123FBC();
    extern u8 jumptable_803751B8[];
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r31 = 0x1;
    r29 = 0x0;
L_801E03F4: ;
    if ((u32)r29 > (u32)0xc) goto L_801E0738;
    r3 = (u32)jumptable_803751B8;
    r0 = r29 << 2;
    r3 = (u32)jumptable_803751B8;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r3 = 0x3b21;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x1;
    ((void(*)(void))fn_80106698)();
    ((void(*)(void))fn_8001E184)();
    r0 = (s8)r3;
    if ((u32)r29 != (u32)0xc) goto L_801E043C;
    r29 = 0x2;
    goto L_801E0738;
L_801E043C: ;
    r29 = 0x1;
    goto L_801E0738;
    r3 = 0x3b22;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x1;
    ((void(*)(void))fn_80106698)();
    r29 = 0xc;
    goto L_801E0738;
    r3 = 0x3b23;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x1;
    ((void(*)(void))fn_80106698)();
    r3 = 0x6;
    r4 = 0x0;
    r5 = 0x0;
    ((void(*)(void))fn_8001BDF4)();
    /* mr. r30, r3 */;
    if ((u32)r29 < (u32)0xc) goto L_801E0494;
    r29 = 0x3;
    goto L_801E0738;
L_801E0494: ;
    r29 = 0x1;
    goto L_801E0738;
    r3 = 0x0;
    r4 = 0x2;
    ((void(*)(void))fn_80129280)();
    r4 = r30 & 0xFFFF;
    r29 = r3;
    ((void(*)(void))fn_8012AC08)();
    r4 = r3;
    r3 = r29;
    ((void(*)(void))fn_80129D64)();
    r0 = r3 & 0xFF;
    if ((u32)r29 == (u32)0xc) goto L_801E04D0;
    r29 = 0x4;
    goto L_801E0738;
L_801E04D0: ;
    r29 = 0x5;
    goto L_801E0738;
    r3 = 0x0;
    r4 = 0x2;
    ((void(*)(void))fn_80129280)();
    r4 = r30 & 0xFFFF;
    ((void(*)(void))fn_8012AC08)();
    r29 = r3;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r29 != (u32)0xc) goto L_801E0504;
    r0 = 0x0;
    goto L_801E0534;
L_801E0504: ;
    r3 = r29;
    ((void(*)(void))fn_8011EE40)();
    r0 = r3 & 0xFFFF;
    if ((u32)r29 == (u32)0xc) goto L_801E0530;
    ((void(*)(void))fn_801EEC74)();
    r0 = r3 & 0xFF;
    if ((u32)r29 == (u32)0xc) goto L_801E0528;
    r0 = 0x1;
    goto L_801E0534;
L_801E0528: ;
    r0 = 0x0;
    goto L_801E0534;
L_801E0530: ;
    r0 = 0x1;
L_801E0534: ;
    r0 = r0 & 0xFF;
    if ((u32)r29 == (u32)0xc) goto L_801E0544;
    r29 = 0x6;
    goto L_801E0738;
L_801E0544: ;
    r29 = 0xb;
    goto L_801E0738;
    r3 = 0x0;
    r4 = 0x2;
    ((void(*)(void))fn_80129280)();
    r4 = r30 & 0xFFFF;
    ((void(*)(void))fn_8012AC08)();
    ((void(*)(void))fn_8011F4F0)();
    r4 = r3;
    r3 = 0x32;
    ((void(*)(void))fn_80132A38)();
    r3 = 0x3b24;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x1;
    ((void(*)(void))fn_80106698)();
    r29 = 0xc;
    goto L_801E0738;
    r3 = 0x0;
    r4 = 0x2;
    ((void(*)(void))fn_80129280)();
    r4 = r30 & 0xFFFF;
    ((void(*)(void))fn_8012AC08)();
    ((void(*)(void))fn_8011F4F0)();
    r4 = r3;
    r3 = 0x32;
    ((void(*)(void))fn_80132A38)();
    r3 = 0x3b25;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x1;
    ((void(*)(void))fn_80106698)();
    ((void(*)(void))fn_8001E184)();
    r0 = (s8)r3;
    if ((u32)r29 != (u32)0xc) goto L_801E05D8;
    r29 = 0x7;
    goto L_801E0738;
L_801E05D8: ;
    r29 = 0x1;
    goto L_801E0738;
    r3 = 0x3b26;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x1;
    ((void(*)(void))fn_80106698)();
    r4 = r30;
    r3 = 0x2;
    fn_80029760();
    if ((s32)r3 != (s32)0x0) goto L_801E0610;
    r29 = 0x9;
    goto L_801E0738;
L_801E0610: ;
    r29 = 0x8;
    goto L_801E0738;
    r3 = 0x0;
    r4 = 0x2;
    ((void(*)(void))fn_80129280)();
    r4 = r30 & 0xFFFF;
    ((void(*)(void))fn_8012AC08)();
    ((void(*)(void))fn_8011F4F0)();
    r4 = r3;
    r3 = 0x32;
    ((void(*)(void))fn_80132A38)();
    r3 = 0x3b27;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x1;
    ((void(*)(void))fn_80106698)();
    r29 = 0xc;
    goto L_801E0738;
    r3 = 0x0;
    r4 = 0x2;
    ((void(*)(void))fn_80129280)();
    r4 = r30 & 0xFFFF;
    ((void(*)(void))fn_8012AC08)();
    ((void(*)(void))fn_8011F4F0)();
    r4 = r3;
    r3 = 0x32;
    ((void(*)(void))fn_80132A38)();
    r3 = 0x3b1f;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x1;
    ((void(*)(void))fn_80106698)();
    ((void(*)(void))fn_8001E184)();
    r0 = (s8)r3;
    if ((s32)r3 != (s32)0x0) goto L_801E06A4;
    r29 = 0xa;
    goto L_801E0738;
L_801E06A4: ;
    r29 = 0x7;
    goto L_801E0738;
    r3 = 0x0;
    r4 = 0x2;
    ((void(*)(void))fn_80129280)();
    r4 = r30 & 0xFFFF;
    ((void(*)(void))fn_8012AC08)();
    ((void(*)(void))fn_8011F4F0)();
    r4 = r3;
    r3 = 0x32;
    ((void(*)(void))fn_80132A38)();
    r3 = 0x3b47;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x1;
    ((void(*)(void))fn_80106698)();
    r29 = 0xc;
    goto L_801E0738;
    r3 = 0x0;
    r4 = 0x2;
    ((void(*)(void))fn_80129280)();
    r4 = r30 & 0xFFFF;
    ((void(*)(void))fn_8012AC08)();
    ((void(*)(void))fn_8011F4F0)();
    r4 = r3;
    r3 = 0x32;
    ((void(*)(void))fn_80132A38)();
    r3 = 0x3b20;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x1;
    ((void(*)(void))fn_80106698)();
    r29 = 0xc;
    goto L_801E0738;
    r3 = 0x1;
    fn_801069FC();
    r31 = 0x0;
L_801E0738: ;
    if ((s32)r31 != (s32)0x0) goto L_801E03F4;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}
#pragma pop

/* 0x801E075C | size: 0x284 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E075C(void) {
    extern u8 lbl_80279A00[];
    extern u8 lbl_803750C8[];
    extern u8 lbl_8047E3F4[];
    extern u8 lbl_8047E400[];
    extern u8 lbl_8047E408[];
    extern u8 lbl_8047E414[];
    extern void fn_800D3088();
    extern void fn_800E01D0();
    extern void fn_800E4014();
    extern void fn_800E407C();
    extern void fn_800E43A4();
    extern void fn_800E4BF4();
    extern void fn_800F0308();
    extern void fn_80113D58();
    extern void fn_8011E15C();
    extern void fn_8011E778();
    extern void fn_8011F550();
    extern void fn_8011F5C8();
    extern void fn_80123FBC();
    extern void fn_80166AB8();
    u8 sp[0xE0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0xD0) = f31;
    /* psq_st f31, 0xd8(r1), 0, qr0 */;
    *(f64*)(sp + 0xC0) = f30;
    /* psq_st f30, 0xc8(r1), 0, qr0 */;
    *(f64*)(sp + 0xB0) = f29;
    /* psq_st f29, 0xb8(r1), 0, qr0 */;
    *(f64*)(sp + 0xA0) = f28;
    /* psq_st f28, 0xa8(r1), 0, qr0 */;
    *(f64*)(sp + 0x90) = f27;
    /* psq_st f27, 0x98(r1), 0, qr0 */;
    /* stmw r25, 0x74(r1) */;
    r4 = (u32)lbl_80279A00;
    r29 = r3;
    r31 = (u32)lbl_80279A00;
    r25 = 0x0;
    r7 = *(u32*)((u8*)r31 + 0x4C);
    r26 = 0x0;
    r6 = *(u32*)((u8*)r31 + 0x50);
    r30 = 0x1;
    r5 = *(u32*)((u8*)r31 + 0x54);
    r4 = *(u32*)((u8*)r31 + 0x58);
    r3 = *(u32*)((u8*)r31 + 0x5C);
    r0 = *(u32*)((u8*)r31 + 0x60);
    *(u32*)(sp + 0x10) = r0;
L_801E07DC: ;
    if ((s32)r26 == (s32)0x1) goto L_801E0920;
    if ((s32)r26 >= (s32)0x1) goto L_801E07F4;
    if ((s32)r26 >= (s32)0x0) goto L_801E0800;
    goto L_801E099C;
L_801E07F4: ;
    if ((s32)r26 == (s32)0x64) goto L_801E0980;
    goto L_801E099C;
L_801E0800: ;
    r3 = 0x0;
    r4 = 0x2;
    ((void(*)(void))fn_80129280)();
    r4 = r29 & 0xFFFF;
    ((void(*)(void))fn_8012AC08)();
    fn_8011F550();
    r25 = *(u32*)((u8*)r31 + 0x0);
    /* clrlslwi r0, r3, 24, 2 */;
    r26 = *(u32*)((u8*)r31 + 0x4);
    r3 = r1 + 0x20;
    r27 = *(u32*)((u8*)r31 + 0x8);
    r28 = *(u32*)((u8*)r31 + 0xC);
    r12 = *(u32*)((u8*)r31 + 0x10);
    r11 = *(u32*)((u8*)r31 + 0x14);
    r10 = *(u32*)((u8*)r31 + 0x18);
    r9 = *(u32*)((u8*)r31 + 0x1C);
    r8 = *(u32*)((u8*)r31 + 0x20);
    r7 = *(u32*)((u8*)r31 + 0x24);
    r6 = *(u32*)((u8*)r31 + 0x28);
    r5 = *(u32*)((u8*)r31 + 0x2C);
    r4 = *(u32*)((u8*)r31 + 0x30);
    r3 = *(u32*)(r3 + r0);
    fn_80113D58();
    r0 = r3;
    r4 = (u32)lbl_803750C8;
    r4 = (u32)lbl_803750C8;
    r3 = r1 + 0x14;
    r25 = r0;
    fn_800E01D0();
    r3 = 0x0;
    r4 = 0x2;
    ((void(*)(void))fn_80129280)();
    r4 = r29 & 0xFFFF;
    ((void(*)(void))fn_8012AC08)();
    r26 = r3;
    fn_8011F5C8();
    r0 = r3;
    r3 = r26;
    r26 = r0;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((s32)r26 == (s32)0x64) goto L_801E0900;
    r3 = r26;
    fn_8011E778();
    if ((u32)r3 == (u32)0x0) goto L_801E0900;
    fn_8011E15C();
    r3 = r3 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x0;
    fn_80166AB8();
L_801E0900: ;
    r3 = r25;
    r4 = r1 + 0x14;
    fn_800E43A4();
    r3 = r25;
    r4 = r1 + 0x8;
    fn_800E407C();
    r26 = 0x1;
    goto L_801E099C;
L_801E0920: ;
    f27 = *(f32*)lbl_8047E3F4;
    r28 = (0x4330 << 16);
    f28 = *(f64*)lbl_8047E400;
    f30 = *(f64*)lbl_8047E408;
    f31 = *(f32*)lbl_8047E414;
    goto L_801E0970;
L_801E0938: ;
    ((void(*)(void))fn_800D37CC)();
    /* xoris r0, r3, 0x8000 */;
    *(u32*)(sp + 0x5C) = r0;
    f0 = *(f64*)(sp + 0x58);
    f29 = f0 - f28;
    fn_800D3088();
    f0 = *(f64*)(sp + 0x60);
    f0 = f0 - f30;
    f0 = f0 / f29;
    f27 = f27 + f0;
    fn_800F0308();
L_801E0970: ;
    if (f27 < f31) goto L_801E0938;
    r26 = 0x64;
    goto L_801E099C;
L_801E0980: ;
    r3 = r25;
    r30 = 0x0;
    r4 = 0x0;
    fn_800E4014();
    r3 = r25;
    fn_800E4BF4();
    r25 = 0x0;
L_801E099C: ;
    if ((s32)r30 != (s32)0x0) goto L_801E07DC;
    /* psq_l f31, 0xd8(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0xD0);
    /* psq_l f30, 0xc8(r1), 0, qr0 */;
    f30 = *(f64*)(sp + 0xC0);
    /* psq_l f29, 0xb8(r1), 0, qr0 */;
    f29 = *(f64*)(sp + 0xB0);
    /* psq_l f28, 0xa8(r1), 0, qr0 */;
    f28 = *(f64*)(sp + 0xA0);
    /* psq_l f27, 0x98(r1), 0, qr0 */;
    f27 = *(f64*)(sp + 0x90);
    /* lmw r25, 0x74(r1) */;
    return;
}
#pragma pop

/* 0x801E09E0 | size: 0x598 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E09E0(void) {
    extern u8 lbl_80279A00[];
    extern u8 lbl_803750C8[];
    extern u8 lbl_8047E3F0[];
    extern u8 lbl_8047E3F4[];
    extern u8 lbl_8047E400[];
    extern u8 lbl_8047E408[];
    extern u8 lbl_8047E410[];
    extern u8 lbl_8047E414[];
    extern u8 lbl_8047E418[];
    extern u8 lbl_8047E41C[];
    extern u8 lbl_8047E420[];
    extern void fn_800D3088();
    extern void fn_800E01D0();
    extern void fn_800E4014();
    extern void fn_800E407C();
    extern void fn_800E43A4();
    extern void fn_800E4BF4();
    extern void fn_800EE150();
    extern void fn_800EE3BC();
    extern void fn_800EE828();
    extern void fn_800F0308();
    extern void fn_800FF56C();
    extern void fn_80113D58();
    extern void fn_80116958();
    extern void fn_8011E15C();
    extern void fn_8011E778();
    extern void fn_8011F550();
    extern void fn_8011F5C8();
    extern void fn_80166AB8();
    extern void fn_80183018();
    extern void fn_80183350();
    extern void fn_80185EE8();
    extern void fn_8018805C();
    extern void fn_8018A280();
    extern void fn_8018BDF4();
    extern void fn_801ED2DC();
    u8 sp[0xF0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0xE0) = f31;
    /* psq_st f31, 0xe8(r1), 0, qr0 */;
    *(f64*)(sp + 0xD0) = f30;
    /* psq_st f30, 0xd8(r1), 0, qr0 */;
    *(f64*)(sp + 0xC0) = f29;
    /* psq_st f29, 0xc8(r1), 0, qr0 */;
    *(f64*)(sp + 0xB0) = f28;
    /* psq_st f28, 0xb8(r1), 0, qr0 */;
    *(f64*)(sp + 0xA0) = f27;
    /* psq_st f27, 0xa8(r1), 0, qr0 */;
    /* stmw r24, 0x80(r1) */;
    r3 = (u32)lbl_80279A00;
    r4 = (u32)lbl_803750C8;
    r31 = (u32)lbl_80279A00;
    r29 = 0x0;
    r7 = *(u32*)((u8*)r31 + 0x34);
    r30 = (u32)lbl_803750C8;
    r6 = *(u32*)((u8*)r31 + 0x38);
    r25 = 0x0;
    r5 = *(u32*)((u8*)r31 + 0x3C);
    r28 = 0x1;
    r4 = *(u32*)((u8*)r31 + 0x40);
    r3 = *(u32*)((u8*)r31 + 0x44);
    r0 = *(u32*)((u8*)r31 + 0x48);
    *(u32*)(sp + 0x10) = r0;
    fn_800FF56C();
    r27 = r3;
L_801E0A6C: ;
    if ((s32)r25 == (s32)0xa) goto L_801E0DC0;
    if ((s32)r25 >= (s32)0xa) goto L_801E0A9C;
    if ((s32)r25 == (s32)0x1) goto L_801E0B9C;
    if ((s32)r25 >= (s32)0x1) goto L_801E0A90;
    if ((s32)r25 >= (s32)0x0) goto L_801E0AA8;
    goto L_801E0F34;
L_801E0A90: ;
    if ((s32)r25 >= (s32)0x3) goto L_801E0F34;
    goto L_801E0EAC;
L_801E0A9C: ;
    if ((s32)r25 == (s32)0x64) goto L_801E0F0C;
    goto L_801E0F34;
L_801E0AA8: ;
    r5 = r1 + 0x20;
    r3 = 0x4d;
    r4 = 0x1;
    fn_8018BDF4();
    r3 = 0x4d;
    r4 = 0x1;
    fn_80183350();
    r3 = (0x1da << 16);
    r3 = r3 + 0x1002;
    ((void(*)(void))fn_800F92D4)();
    r4 = 0x0;
    fn_800EE150();
    r4 = r1 + 0x2c;
    r24 = r3;
    r5 = 0x0;
    r6 = 0x0;
    fn_800EE3BC();
    r3 = r24;
    fn_800EE828();
    r3 = r1 + 0x2c;
    r4 = r30 + 0xc;
    fn_800E01D0();
    f1 = *(f32*)(sp + 0x2C);
    r3 = 0x4d;
    f2 = *(f32*)(sp + 0x30);
    r4 = 0x1;
    f3 = *(f32*)(sp + 0x34);
    r5 = 0x1;
    fn_80185EE8();
    r3 = 0x4d;
    r4 = 0x1;
    r5 = 0x1;
    fn_8018A280();
    r3 = r27;
    r4 = 0x2c;
    r5 = 0x0;
    fn_80116958();
    f27 = *(f32*)lbl_8047E3F4;
    r24 = (0x4330 << 16);
    f31 = *(f64*)lbl_8047E400;
    f29 = *(f64*)lbl_8047E408;
    f28 = *(f32*)lbl_8047E418;
    goto L_801E0B8C;
L_801E0B54: ;
    ((void(*)(void))fn_800D37CC)();
    /* xoris r0, r3, 0x8000 */;
    *(u32*)(sp + 0x74) = r0;
    f0 = *(f64*)(sp + 0x70);
    f30 = f0 - f31;
    fn_800D3088();
    f0 = *(f64*)(sp + 0x78);
    f0 = f0 - f29;
    f0 = f0 / f30;
    f27 = f27 + f0;
    fn_800F0308();
L_801E0B8C: ;
    if (f27 < f28) goto L_801E0B54;
    r25 = 0x1;
    goto L_801E0F34;
L_801E0B9C: ;
    r3 = r1 + 0x2c;
    r4 = r30 + 0x18;
    fn_800E01D0();
    f1 = *(f32*)(sp + 0x2C);
    r3 = 0x4d;
    f2 = *(f32*)(sp + 0x30);
    r4 = 0x1;
    f3 = *(f32*)(sp + 0x34);
    r5 = 0x1;
    fn_80185EE8();
    r3 = 0x4d;
    r4 = 0x1;
    r5 = 0x1;
    fn_8018A280();
    r3 = r27;
    r4 = 0x2c;
    r5 = 0x2;
    fn_80116958();
    f27 = *(f32*)lbl_8047E3F4;
    r24 = (0x4330 << 16);
    f31 = *(f64*)lbl_8047E400;
    f29 = *(f64*)lbl_8047E408;
    f28 = *(f32*)lbl_8047E3F0;
    goto L_801E0C34;
L_801E0BFC: ;
    ((void(*)(void))fn_800D37CC)();
    /* xoris r0, r3, 0x8000 */;
    *(u32*)(sp + 0x7C) = r0;
    f0 = *(f64*)(sp + 0x78);
    f30 = f0 - f31;
    fn_800D3088();
    f0 = *(f64*)(sp + 0x70);
    f0 = f0 - f29;
    f0 = f0 / f30;
    f27 = f27 + f0;
    fn_800F0308();
L_801E0C34: ;
    if (f27 < f28) goto L_801E0BFC;
    f2 = *(f32*)lbl_8047E410;
    r3 = 0x4d;
    f0 = *(f32*)(sp + 0x34);
    r4 = 0x1;
    f1 = *(f32*)(sp + 0x2C);
    r5 = 0x1;
    f3 = f2 + f0;
    f2 = *(f32*)(sp + 0x30);
    fn_80185EE8();
    r3 = 0x4d;
    r4 = 0x1;
    r5 = 0x1;
    fn_8018A280();
    r3 = r27;
    r4 = 0x2c;
    r5 = 0x0;
    fn_80116958();
    r3 = r1 + 0x2c;
    r4 = r30 + 0xc;
    fn_800E01D0();
    f1 = *(f32*)(sp + 0x2C);
    r3 = 0x4d;
    f2 = *(f32*)(sp + 0x30);
    r4 = 0x1;
    f3 = *(f32*)(sp + 0x34);
    r5 = 0x1;
    fn_80185EE8();
    r3 = 0x4d;
    r4 = 0x1;
    r5 = 0x1;
    fn_8018A280();
    r3 = r27;
    r4 = 0x2c;
    r5 = 0x2;
    fn_80116958();
    f27 = *(f32*)lbl_8047E3F4;
    r24 = (0x4330 << 16);
    f31 = *(f64*)lbl_8047E400;
    f29 = *(f64*)lbl_8047E408;
    f28 = *(f32*)lbl_8047E41C;
    goto L_801E0D18;
L_801E0CE0: ;
    ((void(*)(void))fn_800D37CC)();
    /* xoris r0, r3, 0x8000 */;
    *(u32*)(sp + 0x7C) = r0;
    f0 = *(f64*)(sp + 0x78);
    f30 = f0 - f31;
    fn_800D3088();
    f0 = *(f64*)(sp + 0x70);
    f0 = f0 - f29;
    f0 = f0 / f30;
    f27 = f27 + f0;
    fn_800F0308();
L_801E0D18: ;
    if (f27 < f28) goto L_801E0CE0;
    f1 = *(f32*)(sp + 0x20);
    r3 = 0x4d;
    f2 = *(f32*)(sp + 0x24);
    r4 = 0x1;
    f3 = *(f32*)(sp + 0x28);
    r5 = 0x1;
    fn_80185EE8();
    r3 = 0x4d;
    r4 = 0x1;
    r5 = 0x1;
    fn_8018A280();
    f1 = *(f32*)lbl_8047E3F4;
    r3 = 0x4d;
    f2 = *(f32*)lbl_8047E410;
    r4 = 0x1;
    fn_8018805C();
    f27 = *(f32*)lbl_8047E3F4;
    r25 = 0xa;
    f31 = *(f64*)lbl_8047E400;
    r24 = (0x4330 << 16);
    f29 = *(f64*)lbl_8047E408;
    f28 = *(f32*)lbl_8047E420;
    goto L_801E0DB4;
L_801E0D7C: ;
    ((void(*)(void))fn_800D37CC)();
    /* xoris r0, r3, 0x8000 */;
    *(u32*)(sp + 0x7C) = r0;
    f0 = *(f64*)(sp + 0x78);
    f30 = f0 - f31;
    fn_800D3088();
    f0 = *(f64*)(sp + 0x70);
    f0 = f0 - f29;
    f0 = f0 / f30;
    f27 = f27 + f0;
    fn_800F0308();
L_801E0DB4: ;
    if (f27 < f28) goto L_801E0D7C;
    goto L_801E0F34;
L_801E0DC0: ;
    r3 = 0x0;
    fn_801ED2DC();
    fn_8011F550();
    r24 = *(u32*)((u8*)r31 + 0x0);
    /* clrlslwi r0, r3, 24, 2 */;
    r25 = *(u32*)((u8*)r31 + 0x4);
    r3 = r1 + 0x38;
    r29 = *(u32*)((u8*)r31 + 0x8);
    r26 = *(u32*)((u8*)r31 + 0xC);
    r12 = *(u32*)((u8*)r31 + 0x10);
    r11 = *(u32*)((u8*)r31 + 0x14);
    r10 = *(u32*)((u8*)r31 + 0x18);
    r9 = *(u32*)((u8*)r31 + 0x1C);
    r8 = *(u32*)((u8*)r31 + 0x20);
    r7 = *(u32*)((u8*)r31 + 0x24);
    r6 = *(u32*)((u8*)r31 + 0x28);
    r5 = *(u32*)((u8*)r31 + 0x2C);
    r4 = *(u32*)((u8*)r31 + 0x30);
    r3 = *(u32*)(r3 + r0);
    fn_80113D58();
    r0 = r3;
    r3 = r1 + 0x14;
    r29 = r0;
    r4 = r30 + 0x0;
    fn_800E01D0();
    r3 = 0x0;
    fn_801ED2DC();
    if ((u32)r3 == (u32)0x0) goto L_801E0E8C;
    fn_8011F5C8();
    fn_8011E778();
    if ((u32)r3 == (u32)0x0) goto L_801E0E8C;
    fn_8011E15C();
    r3 = r3 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x0;
    fn_80166AB8();
L_801E0E8C: ;
    r3 = r29;
    r4 = r1 + 0x14;
    fn_800E43A4();
    r3 = r29;
    r4 = r1 + 0x8;
    fn_800E407C();
    r25 = 0x2;
    goto L_801E0F34;
L_801E0EAC: ;
    f27 = *(f32*)lbl_8047E3F4;
    r26 = (0x4330 << 16);
    f28 = *(f64*)lbl_8047E400;
    f30 = *(f64*)lbl_8047E408;
    f31 = *(f32*)lbl_8047E414;
    goto L_801E0EFC;
L_801E0EC4: ;
    ((void(*)(void))fn_800D37CC)();
    /* xoris r0, r3, 0x8000 */;
    *(u32*)(sp + 0x7C) = r0;
    f0 = *(f64*)(sp + 0x78);
    f29 = f0 - f28;
    fn_800D3088();
    f0 = *(f64*)(sp + 0x70);
    f0 = f0 - f30;
    f0 = f0 / f29;
    f27 = f27 + f0;
    fn_800F0308();
L_801E0EFC: ;
    if (f27 < f31) goto L_801E0EC4;
    r25 = 0x64;
    goto L_801E0F34;
L_801E0F0C: ;
    r28 = 0x0;
    r3 = 0x4d;
    r4 = 0x1;
    fn_80183018();
    r3 = r29;
    r4 = 0x0;
    fn_800E4014();
    r3 = r29;
    fn_800E4BF4();
    r29 = 0x0;
L_801E0F34: ;
    if ((s32)r28 != (s32)0x0) goto L_801E0A6C;
    /* psq_l f31, 0xe8(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0xE0);
    /* psq_l f30, 0xd8(r1), 0, qr0 */;
    f30 = *(f64*)(sp + 0xD0);
    /* psq_l f29, 0xc8(r1), 0, qr0 */;
    f29 = *(f64*)(sp + 0xC0);
    /* psq_l f28, 0xb8(r1), 0, qr0 */;
    f28 = *(f64*)(sp + 0xB0);
    /* psq_l f27, 0xa8(r1), 0, qr0 */;
    f27 = *(f64*)(sp + 0xA0);
    /* lmw r24, 0x80(r1) */;
    return;
}
#pragma pop

/* 0x801E0F78 | size: 0x3C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E0F78(void) {
    extern void fn_8011E15C();
    extern void fn_8011E778();
    extern void fn_80166AB8();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    fn_8011E778();
    if ((u32)r3 == (u32)0x0) goto L_801E0FA4;
    fn_8011E15C();
    r3 = r3 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x0;
    fn_80166AB8();
L_801E0FA4: ;
    return;
}
#pragma pop

/* 0x801E0FB4 | size: 0x1BC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E0FB4(void) {
    extern u8 lbl_80467CF8[];
    extern u8 lbl_8047B43C[];
    extern void fn_800D3190();
    extern void fn_800D3410();
    extern void fn_800D3FA4();
    extern void fn_800F0470();
    extern void fn_800FE6DC();
    extern void fn_800FE6F8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f5 = 0.0f;

    /* stmw r27, 0xc(r1) */;
    r29 = r3;
    r30 = r4;
    r31 = r5;
    r0 = *(u8*)&lbl_8047B420;
    if ((u32)r0 != (u32)0x0) goto L_801E0FE4;
    r27 = 0x1;
    goto L_801E112C;
L_801E0FE4: ;
    r0 = *(u32*)&lbl_8047B428;
    if ((s32)r0 == (s32)0x2) goto L_801E105C;
    if ((s32)r0 >= (s32)0x2) goto L_801E1004;
    if ((s32)r0 == (s32)0x0) goto L_801E1010;
    if ((s32)r0 >= (s32)0x0) goto L_801E1018;
    goto L_801E10A0;
L_801E1004: ;
    if ((s32)r0 >= (s32)0x4) goto L_801E10A0;
    goto L_801E1064;
L_801E1010: ;
    r27 = 0x1;
    goto L_801E10A0;
L_801E1018: ;
    r0 = 0x2;
    r3 = (u32)lbl_80467CF8;
    *(u32*)&lbl_8047B428 = r0;
    r28 = (u32)lbl_80467CF8;
    r27 = 0x0;
    goto L_801E1048;
L_801E1030: ;
    r3 = *(u32*)((u8*)r28 + 0x0);
    if ((u32)r3 == (u32)0x0) goto L_801E1040;
    fn_800FE6F8();
L_801E1040: ;
    r27 = r27 + 0x1;
    r28 = r28 + 0x4;
L_801E1048: ;
    r0 = *(u32*)&lbl_8047B42C;
    if ((u32)r27 < (u32)r0) goto L_801E1030;
    r27 = 0x1;
    goto L_801E10A0;
L_801E105C: ;
    r27 = 0x0;
    goto L_801E10A0;
L_801E1064: ;
    r28 = 0x0;
    r3 = (u32)lbl_80467CF8;
    *(u32*)&lbl_8047B428 = r28;
    r27 = (u32)lbl_80467CF8;
    goto L_801E1090;
L_801E1078: ;
    r3 = *(u32*)((u8*)r27 + 0x0);
    if ((u32)r3 == (u32)0x0) goto L_801E1088;
    fn_800FE6DC();
L_801E1088: ;
    r28 = r28 + 0x1;
    r27 = r27 + 0x4;
L_801E1090: ;
    r0 = *(u32*)&lbl_8047B42C;
    if ((u32)r28 < (u32)r0) goto L_801E1078;
    r27 = 0x0;
L_801E10A0: ;
    r0 = *(u32*)&lbl_8047B424;
    if ((s32)r0 == (s32)0x3) goto L_801E10C0;
    if ((s32)r0 >= (s32)0x3) goto L_801E10B4;
    goto L_801E1114;
L_801E10B4: ;
    if ((s32)r0 >= (s32)0x5) goto L_801E1114;
    goto L_801E10D8;
L_801E10C0: ;
    r0 = *(u32*)&lbl_8047B428;
    if ((s32)r0 == (s32)0x2) goto L_801E1114;
    r0 = 0x1;
    *(u32*)&lbl_8047B428 = r0;
    goto L_801E1114;
L_801E10D8: ;
    r0 = *(u32*)&lbl_8047B428;
    r4 = 0x2;
    if ((s32)r0 != (s32)0x0) goto L_801E10EC;
    r4 = 0x1;
L_801E10EC: ;
    r3 = *(u32*)&lbl_8047B430;
    *(u32*)&lbl_8047B428 = r4;
    r0 = r3 + 0x1;
    *(u32*)&lbl_8047B430 = r0;
    if ((u32)r0 < (u32)0x5) goto L_801E1114;
    r3 = 0x3;
    r0 = 0x0;
    *(u32*)&lbl_8047B428 = r3;
    *(u32*)&lbl_8047B430 = r0;
L_801E1114: ;
    r0 = *(u32*)&lbl_8047B428;
    if ((s32)r0 != (s32)0x2) goto L_801E112C;
    r3 = (0xe390 << 16);
    /* subi r3, r3, 0x6ef5 */;
    fn_800F0470();
L_801E112C: ;
    r0 = *(u8*)lbl_8047B43C;
    if ((u32)r0 == (u32)0x0) goto L_801E113C;
    r27 = 0x0;
L_801E113C: ;
    r4 = r27;
    r3 = 0x0;
    fn_800D3410();
    r3 = r29;
    r4 = r30;
    r5 = r31;
    fn_800D3FA4();
    fn_800D3190();
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x801E1170 | size: 0x1C */
void fn_801E1170(void) {
    lbl_8047B424 = 4;
    lbl_8047B428 = 3;
    lbl_8047B430 = 0;
}

/* 0x801E118C | size: 0x10 | tiny */
void fn_801E118C(void) { }

/* 0x801E119C | size: 0x14 | tiny */
void fn_801E119C(void) { }

/* 0x801E11B0 | size: 0x1C */
void fn_801E11B0(void) {
    u32 prev = lbl_8047B428;
    lbl_8047B424 = 1;
    if (prev == 2) { return; }
    lbl_8047B428 = 1;
}

/* 0x801E11D4 | size: 0xC | tiny */
void fn_801E11D4(void) { }

/* 0x801E11F0 | size: 0x68 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E11F0(void) {
    extern u8 lbl_80467CF8[];
    extern void fn_800FE6DC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = (u32)lbl_80467CF8;
    r31 = (u32)lbl_80467CF8;
    r30 = 0x0;
    *(u32*)&lbl_8047B424 = r30;
    *(u8*)&lbl_8047B420 = r30;
    goto L_801E1234;
L_801E121C: ;
    r3 = *(u32*)((u8*)r31 + 0x0);
    if ((u32)r3 == (u32)0x0) goto L_801E122C;
    fn_800FE6DC();
L_801E122C: ;
    r30 = r30 + 0x1;
    r31 = r31 + 0x4;
L_801E1234: ;
    r0 = *(u32*)&lbl_8047B42C;
    if ((u32)r30 < (u32)r0) goto L_801E121C;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x801E1258 | size: 0x1C */
void fn_801E1258(void) {
    lbl_8047B420 = 1;
    lbl_8047B424 = 2;
    lbl_8047B428 = 3;
}

/* 0x801E1274 | size: 0x2C */
void fn_801E1274(void) {
    lbl_8047B438 = (u32)fn_800F92D4(0x0B521200);
}

/* 0x801E12A0 | size: 0x60 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E12A0(void) {
    extern u8 lbl_80467CF8[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r4 = *(u32*)&lbl_8047B42C;
    r0 = r4 + 0x1;
    if ((u32)r0 < (u32)0x4) goto L_801E12B8;
    r3 = 0x0;
    return;
L_801E12B8: ;
    r4 = (u32)lbl_80467CF8;
    r0 = 0x4;
    r4 = (u32)lbl_80467CF8;
    r5 = r4;
    ctr_fn = (void(*)(void))r0;
L_801E12CC: ;
    r0 = *(u32*)((u8*)r5 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_801E12F0;
    r4 = *(u32*)&lbl_8047B42C;
    *(u32*)((u8*)r5 + 0x0) = r3;
    r3 = 0x1;
    r0 = r4 + 0x1;
    *(u32*)&lbl_8047B42C = r0;
    return;
L_801E12F0: ;
    r5 = r5 + 0x4;
    if (--ctr != 0) goto L_801E12CC;
    r3 = 0x0;
    return;
}
#pragma pop

/* 0x801E1300 | size: 0x68 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E1300(void) {
    extern u8 lbl_80467CF8[];
    extern u8 lbl_8047B43C[];
    extern void fn_800FE834();
    extern void fn_801E1368();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r3 = (u32)lbl_80467CF8;
    r6 = 0x0;
    r0 = 0x1;
    r3 = (u32)lbl_80467CF8;
    r4 = 0x0;
    *(u8*)&lbl_8047B420 = r6;
    r5 = 0x10;
    *(u32*)&lbl_8047B424 = r6;
    *(u32*)&lbl_8047B42C = r6;
    *(u32*)&lbl_8047B438 = r6;
    *(u8*)&lbl_8047B434 = r0;
    *(u8*)lbl_8047B43C = r6;
    memset((void*)r3, (int)r4, (u32)r5);
    r4 = (u32)fn_801E1368;
    r3 = 0x1;
    r6 = (u32)fn_801E1368;
    r5 = 0xa;
    r4 = 0xfd;
    fn_800FE834();
    return;
}
#pragma pop

/* 0x801E1368 | size: 0x368 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E1368(void) {
    extern u8 lbl_80314958[];
    extern u8 lbl_80314C78[];
    extern u8 lbl_8047B435[];
    extern u8 lbl_8047E428[];
    extern u8 lbl_8047E42C[];
    extern u8 lbl_8047E430[];
    extern u8 lbl_8047E434[];
    extern u8 lbl_8047E438[];
    extern u8 lbl_8047E43C[];
    extern u8 lbl_8047E440[];
    extern u8 lbl_8047E444[];
    extern u8 lbl_8047E448[];
    extern u8 lbl_8047E44C[];
    extern u8 lbl_8047E450[];
    extern u8 lbl_8047E454[];
    extern u8 lbl_8047E458[];
    extern u8 lbl_8047E45C[];
    extern u8 lbl_8047E464[];
    extern u8 lbl_8047E46C[];
    extern u8 lbl_8047E474[];
    extern void fn_800D59B8();
    extern void fn_800D5C18();
    extern void fn_800D6680();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D85D4();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    extern void fn_800D9B58();
    extern void fn_800D9ED8();
    extern void fn_800DA028();
    extern void fn_800DA1E8();
    extern void fn_800DA2BC();
    extern void fn_800DA4C4();
    extern void fn_800FAEF8();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;

    r0 = *(u8*)&lbl_8047B420;
    if ((u32)r0 == (u32)0x0) goto L_801E16C0;
    r0 = *(u8*)&lbl_8047B434;
    if ((u32)r0 != (u32)0x0) goto L_801E1390;
    goto L_801E16C0;
L_801E1390: ;
    r0 = *(u32*)&lbl_8047B438;
    if ((u32)r0 == (u32)0x0) goto L_801E16C0;
    r3 = 0x1;
    fn_800D88DC();
    r3 = 0x4;
    fn_800D888C();
    f1 = *(f32*)lbl_8047E428;
    f3 = *(f32*)lbl_8047E42C;
    f2 = f1;
    f4 = *(f32*)lbl_8047E430;
    fn_800D9B58();
    r3 = 0x0;
    r4 = 0x1;
    r5 = 0x1;
    fn_800DA4C4();
    r3 = 0x1;
    r4 = 0x1;
    r5 = 0x0;
    fn_800DA2BC();
    r3 = 0x0;
    r4 = 0x1;
    r5 = 0x1;
    fn_800DA1E8();
    r3 = 0x0;
    fn_800DA028();
    r3 = 0x1;
    fn_800D9ED8();
    r3 = 0x4;
    fn_800D6A00();
    r0 = *(u8*)lbl_8047B435;
    if ((u32)r0 == (u32)0x0) goto L_801E14C4;
    r3 = 0x2;
    fn_800D888C();
    r3 = (u32)lbl_80314958;
    r3 = (u32)lbl_80314958;
    fn_800D7820();
    r3 = 0x4;
    fn_800D67BC();
    f1 = *(f32*)lbl_8047E434;
    f2 = *(f32*)lbl_8047E438;
    f3 = *(f32*)lbl_8047E428;
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0x0;
    r6 = 0x0;
    fn_800D5C18();
    f1 = *(f32*)lbl_8047E43C;
    f2 = *(f32*)lbl_8047E438;
    f3 = *(f32*)lbl_8047E428;
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0x0;
    r6 = 0x0;
    fn_800D5C18();
    f1 = *(f32*)lbl_8047E434;
    f2 = *(f32*)lbl_8047E440;
    f3 = *(f32*)lbl_8047E428;
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0x0;
    r6 = 0x0;
    fn_800D5C18();
    f1 = *(f32*)lbl_8047E43C;
    f2 = *(f32*)lbl_8047E440;
    f3 = *(f32*)lbl_8047E428;
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0x0;
    r6 = 0x0;
    fn_800D5C18();
    fn_800D6728();
L_801E14C4: ;
    r3 = 0x2;
    fn_800D88DC();
    r3 = (u32)lbl_80314C78;
    r3 = (u32)lbl_80314C78;
    fn_800D7820();
    r4 = *(u32*)&lbl_8047B438;
    r3 = 0x0;
    fn_800D85D4();
    r3 = 0x4;
    fn_800D67BC();
    f1 = *(f32*)lbl_8047E444;
    f2 = *(f32*)lbl_8047E448;
    f3 = *(f32*)lbl_8047E428;
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    fn_800D5C18();
    f1 = *(f32*)lbl_8047E428;
    r3 = 0x0;
    f2 = f1;
    fn_800D59B8();
    f1 = *(f32*)lbl_8047E44C;
    f2 = *(f32*)lbl_8047E448;
    f3 = *(f32*)lbl_8047E428;
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    fn_800D5C18();
    f1 = *(f32*)lbl_8047E450;
    r3 = 0x0;
    f2 = *(f32*)lbl_8047E428;
    fn_800D59B8();
    f1 = *(f32*)lbl_8047E444;
    f2 = *(f32*)lbl_8047E454;
    f3 = *(f32*)lbl_8047E428;
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    fn_800D5C18();
    f1 = *(f32*)lbl_8047E428;
    r3 = 0x0;
    f2 = *(f32*)lbl_8047E450;
    fn_800D59B8();
    f1 = *(f32*)lbl_8047E44C;
    f2 = *(f32*)lbl_8047E454;
    f3 = *(f32*)lbl_8047E428;
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    fn_800D5C18();
    f1 = *(f32*)lbl_8047E450;
    r3 = 0x0;
    f2 = f1;
    fn_800D59B8();
    fn_800D6728();
    r0 = *(u32*)&lbl_8047B424;
    if ((s32)r0 == (s32)0x2) goto L_801E1628;
    if ((s32)r0 >= (s32)0x2) goto L_801E15E0;
    if ((s32)r0 == (s32)0x0) goto L_801E15F0;
    if ((s32)r0 >= (s32)0x0) goto L_801E160C;
    goto L_801E16B8;
L_801E15E0: ;
    if ((s32)r0 == (s32)0x4) goto L_801E16A0;
    if ((s32)r0 >= (s32)0x4) goto L_801E16B8;
    goto L_801E1644;
L_801E15F0: ;
    r3 = 0x230;
    r4 = 0x2c;
    r5 = -0x1;
    r6 = (u32)lbl_8047E458;
    /* crclr cr1eq */;
    fn_800FAEF8();
    goto L_801E16B8;
L_801E160C: ;
    r3 = 0x230;
    r4 = 0x2c;
    r5 = -0x1;
    r6 = (u32)lbl_8047E45C;
    /* crclr cr1eq */;
    fn_800FAEF8();
    goto L_801E16B8;
L_801E1628: ;
    r3 = 0x230;
    r4 = 0x2c;
    r5 = -0x1;
    r6 = (u32)lbl_8047E464;
    /* crclr cr1eq */;
    fn_800FAEF8();
    goto L_801E16B8;
L_801E1644: ;
    r0 = *(u32*)&lbl_8047B428;
    if ((s32)r0 >= (s32)0x2) goto L_801E165C;
    if ((s32)r0 >= (s32)0x0) goto L_801E1668;
    goto L_801E16B8;
L_801E165C: ;
    if ((s32)r0 >= (s32)0x4) goto L_801E16B8;
    goto L_801E1684;
L_801E1668: ;
    r3 = 0x230;
    r4 = 0x2c;
    r5 = -0x1;
    r6 = (u32)lbl_8047E46C;
    /* crclr cr1eq */;
    fn_800FAEF8();
    goto L_801E16B8;
L_801E1684: ;
    r3 = 0x230;
    r4 = 0x2c;
    r5 = -0x1;
    r6 = (u32)lbl_8047E45C;
    /* crclr cr1eq */;
    fn_800FAEF8();
    goto L_801E16B8;
L_801E16A0: ;
    r3 = 0x230;
    r4 = 0x2c;
    r5 = -0x1;
    r6 = (u32)lbl_8047E474;
    /* crclr cr1eq */;
    fn_800FAEF8();
L_801E16B8: ;
    r3 = 0x0;
    fn_800D9ED8();
L_801E16C0: ;
    return;
}
#pragma pop

/* 0x801E16D0 | size: 0x20 */
void fn_801E16D0(void) {
    fn_801E25C8();
}

/* 0x801E16F0 | size: 0xB8 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E16F0(void) {
    extern u8 lbl_8047B450[];
    extern void fn_800E202C();
    extern void fn_800E209C();
    extern void fn_800E24B0();
    extern void fn_800EE928();
    extern void fn_801E386C();
    extern void fn_801E38D8();
    extern void fn_801E3F54();
    extern void fn_801E4724();
    extern u8 lbl_8047B440;
    extern u8 lbl_8047B441;
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r31 = 0;

    r0 = *(u8*)&lbl_8047B440;
    if ((u32)r0 == (u32)0x0) goto L_801E1718;
    r0 = *(u8*)&lbl_8047B441;
    if ((u32)r0 != (u32)0x0) goto L_801E1720;
L_801E1718: ;
    r0 = 0x0;
    goto L_801E1724;
L_801E1720: ;
    r0 = 0x1;
L_801E1724: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_801E1794;
    fn_801E386C();
    fn_801E38D8();
    if ((s32)r3 == (s32)0x4) goto L_801E1794;
    if ((s32)r3 >= (s32)0x4) goto L_801E174C;
    if ((s32)r3 >= (s32)0x3) goto L_801E1754;
    goto L_801E1794;
L_801E174C: ;
    if ((s32)r3 >= (s32)0x6) goto L_801E1794;
L_801E1754: ;
    r0 = *(u8*)&lbl_8047B441;
    if ((u32)r0 == (u32)0x0) goto L_801E1794;
    fn_801E3F54();
    fn_801E4724();
    r3 = *(u32*)lbl_8047B450;
    fn_800E202C();
    r0 = r3 & 0xFFFF;
    r31 = r3;
    if ((u32)r0 == (u32)0x0) goto L_801E1788;
    fn_800E24B0();
    r3 = r31;
    fn_800E209C();
L_801E1788: ;
    r0 = 0x0;
    *(u8*)&lbl_8047B441 = r0;
    fn_800EE928();
L_801E1794: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801E17A8 | size: 0x68 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E17A8(void) {
    extern u8 lbl_80469030[];
    extern u8 lbl_8047B454[];
    extern u8 lbl_8047B458[];
    extern u8 lbl_8047B45C[];
    extern void fn_801E3978();
    extern u8 lbl_8047B440;
    extern u8 lbl_8047B441;
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    r0 = *(u8*)&lbl_8047B440;
    if ((u32)r0 == (u32)0x0) goto L_801E17CC;
    r0 = *(u8*)&lbl_8047B441;
    if ((u32)r0 != (u32)0x0) goto L_801E17D4;
L_801E17CC: ;
    r0 = 0x0;
    goto L_801E17D8;
L_801E17D4: ;
    r0 = 0x1;
L_801E17D8: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_801E1800;
    r4 = (u32)lbl_80469030;
    r3 = *(u32*)lbl_8047B45C;
    r7 = (u32)lbl_80469030;
    r4 = *(u32*)lbl_8047B458;
    r5 = *(u32*)lbl_8047B454;
    r6 = *(u32*)((u8*)r7 + 0x0);
    r7 = *(u32*)((u8*)r7 + 0x4);
    fn_801E3978();
L_801E1800: ;
    return;
}
#pragma pop

/* 0x801E1810 | size: 0x64 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E1810(void) {
    extern u8 lbl_8047B450[];
    extern void fn_800E202C();
    extern void fn_800E209C();
    extern void fn_800E24B0();
    extern void fn_800EE928();
    extern void fn_801E3F54();
    extern void fn_801E4724();
    extern u8 lbl_8047B441;
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r31 = 0;

    r0 = *(u8*)&lbl_8047B441;
    if ((u32)r0 == (u32)0x0) goto L_801E1860;
    fn_801E3F54();
    fn_801E4724();
    r3 = *(u32*)lbl_8047B450;
    fn_800E202C();
    r0 = r3 & 0xFFFF;
    r31 = r3;
    if ((u32)r0 == (u32)0x0) goto L_801E1854;
    fn_800E24B0();
    r3 = r31;
    fn_800E209C();
L_801E1854: ;
    r0 = 0x0;
    *(u8*)&lbl_8047B441 = r0;
    fn_800EE928();
L_801E1860: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801E1874 | size: 0x28 */
extern u8 lbl_8047B440;
extern u8 lbl_8047B441;
u32 fn_801E1874(void) {
    if (lbl_8047B440 == 0 || lbl_8047B441 == 0) { return 0; }
    return 1;
}

/* 0x801E189C | size: 0x88 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E189C(void) {
    extern u8 lbl_80467D08[];
    extern u8 lbl_80468020[];
    extern u8 lbl_8047B444[];
    extern u8 lbl_8047B448[];
    extern void fn_800A19CC();
    extern void fn_800A1F94();
    extern void fn_800F0308();
    extern void fn_801E1924();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;

    r5 = (u32)lbl_80468020;
    r7 = (u32)lbl_80467D08;
    r0 = 0x0;
    r9 = r4 & 0xFF;
    r8 = (u32)lbl_8047B448;
    *(u32*)((u8*)r8 + 0x4) = r9;
    r6 = (u32)fn_801E1924;
    r5 = (u32)lbl_80468020;
    r8 = 0x10;
    *(u32*)lbl_8047B444 = r0;
    r0 = (u32)lbl_80467D08;
    r4 = (u32)fn_801E1924;
    r6 = r5 + 0xffc;
    *(u32*)lbl_8047B448 = r3;
    r3 = r0;
    r5 = (u32)lbl_8047B448;
    r7 = 0x1000;
    r9 = 0x1;
    fn_800A19CC();
    r3 = (u32)lbl_80467D08;
    r3 = (u32)lbl_80467D08;
    fn_800A1F94();
    goto L_801E1908;
L_801E1904: ;
    fn_800F0308();
L_801E1908: ;
    r0 = *(u32*)lbl_8047B444;
    if ((s32)r0 == (s32)0x0) goto L_801E1904;
    return;
}
#pragma pop

/* 0x801E1924 | size: 0x208 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E1924(void) {
    extern u8 lbl_80279A68[];
    extern u8 lbl_80466BC0[];
    extern u8 lbl_80469020[];
    extern u8 lbl_80469030[];
    extern u8 lbl_8047B444[];
    extern u8 lbl_8047B450[];
    extern u8 lbl_8047B454[];
    extern u8 lbl_8047B458[];
    extern u8 lbl_8047B45C[];
    extern void fn_800E202C();
    extern void fn_800E209C();
    extern void fn_800E24B0();
    extern void fn_800E27B0();
    extern void fn_800E2B00();
    extern void fn_800EE928();
    extern void fn_800EE9BC();
    extern void fn_8014F2DC();
    extern void fn_801E3858();
    extern void fn_801E38E8();
    extern void fn_801E3930();
    extern void fn_801E4058();
    extern void fn_801E40F8();
    extern void fn_801E449C();
    extern void fn_801E4650();
    extern void fn_801E4778();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = (u32)lbl_80279A68;
    /* stmw r29, 0x14(r1) */;
    r31 = (u32)lbl_80279A68;
    r0 = *(u32*)((u8*)r3 + 0x4);
    r30 = *(u32*)((u8*)r3 + 0x0);
    r29 = r0 & 0xFF;
    fn_800EE9BC();
    r3 = r30;
    r4 = r29;
    fn_801E4778();
    if ((s32)r3 != (s32)0x0) goto L_801E1984;
    r4 = r30;
    r3 = r31 + 0x0;
    /* crclr cr1eq */;
    ((void(*)(void))fn_800DD970)();
    fn_800EE928();
    r0 = 0x1;
    r3 = 0x0;
    *(u32*)lbl_8047B444 = r0;
    goto L_801E1B18;
L_801E1984: ;
    r3 = (u32)lbl_80469030;
    r3 = (u32)lbl_80469030;
    fn_801E3930();
    r3 = (u32)lbl_80469020;
    r3 = (u32)lbl_80469020;
    fn_801E38E8();
    r4 = (u32)lbl_80466BC0;
    r3 = (u32)lbl_80469030;
    r5 = (u32)lbl_80466BC0;
    r4 = *(u32*)lbl_80469030;
    *(u32*)lbl_8047B45C = r5;
    r3 = *(u32*)((u8*)r3 + 0x4);
    r0 = *(u16*)((u8*)r5 + 0x4);
    r0 = r0 - r4;
    r0 = (u32)r0 >> 1;
    *(u32*)lbl_8047B458 = r0;
    r0 = *(u16*)((u8*)r5 + 0x6);
    r0 = r0 - r3;
    r0 = (u32)r0 >> 1;
    *(u32*)lbl_8047B454 = r0;
    fn_801E4650();
    r4 = 0x20;
    fn_800E2B00();
    r0 = r3 & 0xFFFF;
    if ((s32)r3 == (s32)0x0) goto L_801E19F0;
    fn_800E27B0();
    goto L_801E19F4;
L_801E19F0: ;
    r3 = 0x0;
L_801E19F4: ;
    *(u32*)lbl_8047B450 = r3;
    if ((u32)r3 != (u32)0x0) goto L_801E1A20;
    r3 = r31 + 0x30;
    /* crclr cr1eq */;
    ((void(*)(void))fn_800DD970)();
    fn_800EE928();
    r0 = 0x1;
    r3 = 0x0;
    *(u32*)lbl_8047B444 = r0;
    goto L_801E1B18;
L_801E1A20: ;
    fn_801E449C();
    r3 = (u32)lbl_80469020;
    r30 = (u32)lbl_80469020;
    r0 = *(u32*)((u8*)r30 + 0xC);
    if ((u32)r0 == (u32)0x1) goto L_801E1A50;
    OSGetTick();
    r4 = *(u32*)((u8*)r30 + 0xC);
    r0 = (u32)r3 / (u32)r4;
    r0 = r0 * r4;
    r5 = r3 - r0;
    goto L_801E1A54;
L_801E1A50: ;
    r5 = 0x0;
L_801E1A54: ;
    r3 = 0x0;
    r4 = 0x0;
    fn_801E40F8();
    if ((s32)r3 != (s32)0x0) goto L_801E1AA8;
    r3 = r31 + 0x5c;
    /* crclr cr1eq */;
    ((void(*)(void))fn_800DD970)();
    r3 = *(u32*)lbl_8047B450;
    fn_800E202C();
    r0 = r3 & 0xFFFF;
    r30 = r3;
    if ((s32)r3 == (s32)0x0) goto L_801E1A94;
    fn_800E24B0();
    r3 = r30;
    fn_800E209C();
L_801E1A94: ;
    fn_800EE928();
    r0 = 0x1;
    r3 = 0x0;
    *(u32*)lbl_8047B444 = r0;
    goto L_801E1B18;
L_801E1AA8: ;
    r3 = r1 + 0xc;
    r4 = r1 + 0x8;
    fn_801E3858();
    r3 = *(u32*)(sp + 0xC);
    r0 = r3 + (0x1 << 16);
    if ((u32)r0 == (u32)0xffff) goto L_801E1ADC;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x7f;
    r8 = 0x0;
    fn_8014F2DC();
L_801E1ADC: ;
    r3 = *(u32*)(sp + 0x8);
    r0 = r3 + (0x1 << 16);
    if ((u32)r0 == (u32)0xffff) goto L_801E1B04;
    r4 = 0x0;
    r5 = 0x7f;
    r6 = 0x0;
    r7 = 0x7f;
    r8 = 0x0;
    fn_8014F2DC();
L_801E1B04: ;
    fn_801E4058();
    r0 = 0x1;
    r3 = 0x0;
    *(u8*)&lbl_8047B441 = r0;
    *(u32*)lbl_8047B444 = r0;
L_801E1B18: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801E1B2C | size: 0x28 */
extern void fn_801E4A6C(void);
void fn_801E1B2C(void) {
    fn_801E4A6C();
    lbl_8047B440 = 1;
}

/* 0x801E1B54 | size: 0x30 */
extern u8 lbl_8046A3D0;
extern void fn_8009F230(void*, void*, u32);
void fn_801E1B54(void* val) {
    fn_8009F230(&lbl_8046A3D0, val, 1);
}

/* 0x801E1B84 | size: 0x34 */
extern void fn_8009F2F8(void*, void*, u32);
u32 fn_801E1B84(void) {
    u32 result;
    fn_8009F2F8(&lbl_8046A3D0, &result, 1);
    return result;
}

/* 0x801E1BB8 | size: 0x30 */
extern u8 lbl_8046A410;
void fn_801E1BB8(void* val) {
    fn_8009F230(&lbl_8046A410, val, 1);
}

/* 0x801E1BE8 | size: 0x34 */
extern u8 lbl_8046A3F0;
u32 fn_801E1BE8(void) {
    u32 result;
    fn_8009F2F8(&lbl_8046A3F0, &result, 1);
    return result;
}

/* 0x801E1C1C | size: 0xF0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E1C1C(void) {
    extern u8 lbl_80469040[];
    extern u8 lbl_8046AC60[];
    extern void fn_800A221C();
    extern void fn_800A541C();
    extern void fn_801E446C();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = (u32)lbl_8046AC60;
    r4 = (u32)lbl_80469040;
    /* stmw r26, 0x18(r1) */;
    r28 = (u32)lbl_8046AC60;
    r29 = 0x0;
    r27 = (u32)lbl_80469040;
    r31 = *(u32*)((u8*)r28 + 0xB8);
    r30 = *(u32*)((u8*)r28 + 0xBC);
L_801E1C48: ;
    r3 = r27 + 0x13d0;
    r4 = r1 + 0x8;
    r5 = 0x1;
    ((void(*)(void))fn_8009F2F8)();
    r26 = *(u32*)(sp + 0x8);
    r3 = r28;
    r5 = r30;
    r6 = r31;
    r4 = *(u32*)((u8*)r26 + 0x0);
    r7 = 0x2;
    fn_800A541C();
    if ((s32)r3 == (s32)r30) goto L_801E1CA4;
    if ((s32)r3 != (s32)-0x1) goto L_801E1C8C;
    r0 = -0x1;
    *(u32*)((u8*)r28 + 0xA8) = r0;
L_801E1C8C: ;
    if ((s32)r29 != (s32)0x0) goto L_801E1C9C;
    r3 = 0x0;
    fn_801E446C();
L_801E1C9C: ;
    r3 = r27 + 0x1000;
    fn_800A221C();
L_801E1CA4: ;
    *(u32*)((u8*)r26 + 0x4) = r29;
    r4 = r26;
    r3 = r27 + 0x13b0;
    r5 = 0x1;
    ((void(*)(void))fn_8009F230)();
    r0 = *(u32*)((u8*)r28 + 0xC0);
    r31 = r31 + r30;
    r6 = *(u32*)((u8*)r28 + 0x50);
    r4 = r29 + r0;
    r5 = *(u32*)((u8*)r26 + 0x0);
    r3 = (u32)r4 / (u32)r6;
    /* subi r0, r6, 0x1 */;
    r30 = *(u32*)((u8*)r5 + 0x0);
    r3 = r3 * r6;
    r3 = r4 - r3;
    if ((u32)r3 != (u32)r0) goto L_801E1D04;
    r0 = *(u8*)((u8*)r28 + 0xA6);
    r0 = r0 & 0x1;
    if ((u32)r3 == (u32)r0) goto L_801E1CFC;
    r31 = *(u32*)((u8*)r28 + 0x64);
    goto L_801E1D04;
L_801E1CFC: ;
    r3 = r27 + 0x1000;
    fn_800A221C();
L_801E1D04: ;
    r29 = r29 + 0x1;
    goto L_801E1C48;
}
#pragma pop

/* 0x801E1D0C | size: 0x3C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E1D0C(void) {
    extern u8 lbl_8046A040[];
    extern u8 lbl_8047B460[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r0 = *(u32*)lbl_8047B460;
    if ((s32)r0 == (s32)0x0) goto L_801E1D38;
    r3 = (u32)lbl_8046A040;
    r3 = (u32)lbl_8046A040;
    OSCancelThread();
    r0 = 0x0;
    *(u32*)lbl_8047B460 = r0;
L_801E1D38: ;
    return;
}
#pragma pop

/* 0x801E1D48 | size: 0x34 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E1D48(void) {
    extern u8 lbl_8046A040[];
    extern u8 lbl_8047B460[];
    extern void fn_800A1F94();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;

    r0 = *(u32*)lbl_8047B460;
    if ((s32)r0 == (s32)0x0) goto L_801E1D6C;
    r3 = (u32)lbl_8046A040;
    r3 = (u32)lbl_8046A040;
    fn_800A1F94();
L_801E1D6C: ;
    return;
}
#pragma pop

/* 0x801E1D7C | size: 0xA0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E1D7C(void) {
    extern u8 lbl_80469040[];
    extern u8 lbl_8047B460[];
    extern void fn_8009F1D0();
    extern void fn_800A19CC();
    extern void fn_801E1C1C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r31 = 0;

    r4 = (u32)fn_801E1C1C;
    r5 = (u32)lbl_80469040;
    r4 = (u32)fn_801E1C1C;
    r8 = r3;
    r7 = 0x1000;
    r31 = (u32)lbl_80469040;
    r6 = r31 + 0x0;
    r5 = 0x0;
    r3 = r31 + 0x1000;
    r9 = 0x1;
    r6 = r6 + 0x1000;
    fn_800A19CC();
    if ((s32)r3 != (s32)0x0) goto L_801E1DCC;
    r3 = 0x0;
    goto L_801E1E08;
L_801E1DCC: ;
    r3 = r31 + 0x13d0;
    r4 = r31 + 0x1368;
    r5 = 0xa;
    fn_8009F1D0();
    r3 = r31 + 0x13b0;
    r4 = r31 + 0x1340;
    r5 = 0xa;
    fn_8009F1D0();
    r3 = r31 + 0x1390;
    r4 = r31 + 0x1318;
    r5 = 0xa;
    fn_8009F1D0();
    r0 = 0x1;
    r3 = 0x1;
    *(u32*)lbl_8047B460 = r0;
L_801E1E08: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801E1E1C | size: 0x1DC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E1E1C(void) {
    extern u8 lbl_8047E494[];
    extern void fn_800B928C();
    extern void fn_800BA9E4();
    extern void fn_800BACA0();
    extern void fn_800BAFFC();
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    /* stmw r24, 0x70(r1) */;
    r24 = r4;
    r25 = r5;
    r30 = r6;
    r31 = r7;
    r26 = r10;
    r4 = r3;
    r27 = *(s16*)((u8*)r1 + 0x9A);
    r29 = r8;
    r5 = r8 & 0xFFFF;
    r28 = r9;
    r6 = r9 & 0xFFFF;
    r3 = r1 + 0x48;
    r7 = 0x1;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_800BA9E4();
    f1 = *(f32*)lbl_8047E494;
    r3 = r1 + 0x48;
    r4 = 0x0;
    r5 = 0x0;
    f2 = f1;
    r6 = 0x0;
    f3 = f1;
    r7 = 0x0;
    r8 = 0x0;
    fn_800BACA0();
    r3 = r1 + 0x48;
    r4 = 0x0;
    fn_800BAFFC();
    r3 = (s16)r29;
    r0 = (s16)r28;
    r28 = (s32)r3 >> 1;
    r4 = r24;
    r29 = (s32)r0 >> 1;
    r3 = r1 + 0x28;
    r5 = r28 & 0xFFFF;
    r7 = 0x1;
    r6 = r29 & 0xFFFF;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_800BA9E4();
    f1 = *(f32*)lbl_8047E494;
    r3 = r1 + 0x28;
    r4 = 0x0;
    r5 = 0x0;
    f2 = f1;
    r6 = 0x0;
    f3 = f1;
    r7 = 0x0;
    r8 = 0x0;
    fn_800BACA0();
    r3 = r1 + 0x28;
    r4 = 0x1;
    fn_800BAFFC();
    r4 = r25;
    r3 = r1 + 0x8;
    r5 = r28 & 0xFFFF;
    r6 = r29 & 0xFFFF;
    r7 = 0x1;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_800BA9E4();
    f1 = *(f32*)lbl_8047E494;
    r3 = r1 + 0x8;
    r4 = 0x0;
    r5 = 0x0;
    f2 = f1;
    r6 = 0x0;
    f3 = f1;
    r7 = 0x0;
    r8 = 0x0;
    fn_800BACA0();
    r3 = r1 + 0x8;
    r4 = 0x2;
    fn_800BAFFC();
    r3 = 0x80;
    r4 = 0x7;
    r5 = 0x4;
    fn_800B928C();
    r4 = (0xcc01 << 16);
    r0 = (s16)r31;
    *(u16*)((u8*)r4 + (-32768)) = r30;
    r6 = r0 + r27;
    r3 = 0x0;
    r5 = (s16)r30;
    *(u16*)((u8*)r4 + (-32768)) = r31;
    r0 = (s16)r26;
    r5 = r5 + r0;
    *(u16*)((u8*)r4 + (-32768)) = r3;
    r0 = 0x1;
    *(u16*)((u8*)r4 + (-32768)) = r3;
    *(u16*)((u8*)r4 + (-32768)) = r3;
    *(u16*)((u8*)r4 + (-32768)) = r5;
    *(u16*)((u8*)r4 + (-32768)) = r31;
    *(u16*)((u8*)r4 + (-32768)) = r3;
    *(u16*)((u8*)r4 + (-32768)) = r0;
    *(u16*)((u8*)r4 + (-32768)) = r3;
    *(u16*)((u8*)r4 + (-32768)) = r5;
    *(u16*)((u8*)r4 + (-32768)) = r6;
    *(u16*)((u8*)r4 + (-32768)) = r3;
    *(u16*)((u8*)r4 + (-32768)) = r0;
    *(u16*)((u8*)r4 + (-32768)) = r0;
    *(u16*)((u8*)r4 + (-32768)) = r30;
    *(u16*)((u8*)r4 + (-32768)) = r6;
    *(u16*)((u8*)r4 + (-32768)) = r3;
    *(u16*)((u8*)r4 + (-32768)) = r3;
    *(u16*)((u8*)r4 + (-32768)) = r0;
    /* lmw r24, 0x70(r1) */;
    return;
}
#pragma pop

/* 0x801E1FF8 | size: 0x4B8 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E1FF8(void) {
    extern u8 lbl_8047E480[];
    extern u8 lbl_8047E484[];
    extern u8 lbl_8047E488[];
    extern u8 lbl_8047E48C[];
    extern u8 lbl_8047E490[];
    extern u8 lbl_8047E494[];
    extern u8 lbl_8047E498[];
    extern u8 lbl_8047E49C[];
    extern u8 lbl_8047E4A0[];
    extern void fn_800A2D38();
    extern void fn_800A39E0();
    extern void fn_800B7874();
    extern void fn_800B7D3C();
    extern void fn_800B7D74();
    extern void fn_800B857C();
    extern void fn_800B884C();
    extern void fn_800B9E6C();
    extern void fn_800BA6B0();
    extern void fn_800BB29C();
    extern void fn_800BC1A0();
    extern void fn_800BC1E4();
    extern void fn_800BC228();
    extern void fn_800BC290();
    extern void fn_800BC36C();
    extern void fn_800BC3E0();
    extern void fn_800BC454();
    extern void fn_800BC4C0();
    extern void fn_800BC52C();
    extern void fn_800BC580();
    extern void fn_800BC6F0();
    extern void fn_800BC8C8();
    extern void fn_800BCDDC();
    extern void fn_800BCE30();
    extern void fn_800BCE5C();
    extern void fn_800BCE88();
    extern void fn_800BCEF4();
    extern void fn_800BD2E0();
    extern void fn_800BD4B4();
    extern void fn_800BD554();
    extern void fn_800BD744();
    extern void fn_800BD7A0();
    u8 sp[0xC0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;

    r4 = 0x0;
    /* stmw r30, 0xb8(r1) */;
    r31 = *(u16*)((u8*)r3 + 0x4);
    r30 = *(u16*)((u8*)r3 + 0x6);
    r3 = 0x0;
    fn_800BCEF4();
    r4 = (0x4330 << 16);
    /* xoris r3, r30, 0x8000 */;
    /* xoris r0, r31, 0x8000 */;
    f1 = *(f32*)lbl_8047E494;
    r3 = r1 + 0x4c;
    f4 = *(f64*)lbl_8047E4A0;
    f3 = f1;
    f0 = *(f64*)(sp + 0x90);
    f5 = f1;
    *(u32*)(sp + 0x9C) = r0;
    f2 = f0 - f4;
    f6 = *(f32*)lbl_8047E498;
    f0 = *(f64*)(sp + 0x98);
    f4 = f0 - f4;
    fn_800A39E0();
    r3 = r1 + 0x4c;
    r4 = 0x1;
    fn_800BD2E0();
    r3 = (0x4330 << 16);
    /* xoris r4, r31, 0x8000 */;
    /* xoris r0, r30, 0x8000 */;
    f1 = *(f32*)lbl_8047E494;
    f4 = *(f64*)lbl_8047E4A0;
    f2 = f1;
    f0 = *(f64*)(sp + 0xA0);
    f5 = f1;
    *(u32*)(sp + 0xAC) = r0;
    f3 = f0 - f4;
    f6 = *(f32*)lbl_8047E49C;
    f0 = *(f64*)(sp + 0xA8);
    f4 = f0 - f4;
    fn_800BD744();
    r5 = r31;
    r6 = r30;
    r3 = 0x0;
    r4 = 0x0;
    fn_800BD7A0();
    r3 = r1 + 0x1c;
    fn_800A2D38();
    r3 = r1 + 0x1c;
    r4 = 0x0;
    fn_800BD4B4();
    r3 = 0x0;
    fn_800BD554();
    r3 = 0x1;
    r4 = 0x7;
    r5 = 0x0;
    fn_800BCE88();
    r3 = 0x0;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x0;
    fn_800BCDDC();
    r3 = 0x1;
    fn_800BCE30();
    r3 = 0x0;
    fn_800BCE5C();
    r3 = 0x0;
    fn_800B9E6C();
    r3 = 0x0;
    fn_800BA6B0();
    r3 = 0x2;
    fn_800B884C();
    r3 = 0x0;
    r4 = 0x1;
    r5 = 0x4;
    r6 = 0x3c;
    r7 = 0x0;
    r8 = 0x7d;
    fn_800B857C();
    r3 = 0x1;
    r4 = 0x1;
    r5 = 0x4;
    r6 = 0x3c;
    r7 = 0x0;
    r8 = 0x7d;
    fn_800B857C();
    fn_800BB29C();
    fn_800B7D3C();
    r3 = 0x9;
    r4 = 0x1;
    fn_800B7874();
    r3 = 0xd;
    r4 = 0x1;
    fn_800B7874();
    r3 = 0x7;
    r4 = 0x9;
    r5 = 0x1;
    r6 = 0x3;
    r7 = 0x0;
    fn_800B7D74();
    r3 = 0x7;
    r4 = 0xd;
    r5 = 0x1;
    r6 = 0x2;
    r7 = 0x0;
    fn_800B7D74();
    r3 = 0x4;
    fn_800BC8C8();
    r3 = 0x0;
    r4 = 0x1;
    r5 = 0x1;
    r6 = 0xff;
    fn_800BC6F0();
    r3 = 0x0;
    r4 = 0xf;
    r5 = 0x8;
    r6 = 0xe;
    r7 = 0x2;
    fn_800BC1A0();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    fn_800BC228();
    r3 = 0x0;
    r4 = 0x7;
    r5 = 0x4;
    r6 = 0x6;
    r7 = 0x1;
    fn_800BC1E4();
    r3 = 0x0;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    fn_800BC290();
    r3 = 0x0;
    r4 = 0xc;
    fn_800BC454();
    r3 = 0x0;
    r4 = 0x1c;
    fn_800BC4C0();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x0;
    fn_800BC52C();
    r3 = 0x1;
    r4 = 0x1;
    r5 = 0x2;
    r6 = 0xff;
    fn_800BC6F0();
    r3 = 0x1;
    r4 = 0xf;
    r5 = 0x8;
    r6 = 0xe;
    r7 = 0x0;
    fn_800BC1A0();
    r3 = 0x1;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x1;
    r7 = 0x0;
    r8 = 0x0;
    fn_800BC228();
    r3 = 0x1;
    r4 = 0x7;
    r5 = 0x4;
    r6 = 0x6;
    r7 = 0x0;
    fn_800BC1E4();
    r3 = 0x1;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    fn_800BC290();
    r3 = 0x1;
    r4 = 0xd;
    fn_800BC454();
    r3 = 0x1;
    r4 = 0x1d;
    fn_800BC4C0();
    r3 = 0x1;
    r4 = 0x0;
    r5 = 0x0;
    fn_800BC52C();
    r3 = 0x2;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0xff;
    fn_800BC6F0();
    r3 = 0x2;
    r4 = 0xf;
    r5 = 0x8;
    r6 = 0xc;
    r7 = 0x0;
    fn_800BC1A0();
    r3 = 0x2;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    r8 = 0x0;
    fn_800BC228();
    r3 = 0x2;
    r4 = 0x4;
    r5 = 0x7;
    r6 = 0x7;
    r7 = 0x0;
    fn_800BC1E4();
    r3 = 0x2;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    r8 = 0x0;
    fn_800BC290();
    r3 = 0x2;
    r4 = 0x0;
    r5 = 0x0;
    fn_800BC52C();
    r3 = 0x3;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    fn_800BC6F0();
    r3 = 0x3;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0xe;
    r7 = 0xf;
    fn_800BC1A0();
    r3 = 0x3;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    r8 = 0x0;
    fn_800BC228();
    r3 = 0x3;
    r4 = 0x7;
    r5 = 0x7;
    r6 = 0x7;
    r7 = 0x7;
    fn_800BC1E4();
    r3 = 0x3;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    r8 = 0x0;
    fn_800BC290();
    r3 = 0x3;
    r4 = 0x0;
    r5 = 0x0;
    fn_800BC52C();
    r3 = 0x3;
    r4 = 0xe;
    fn_800BC454();
    r5 = *(u32*)lbl_8047E480;
    r4 = r1 + 0x14;
    r0 = *(u32*)lbl_8047E484;
    r3 = 0x1;
    *(u32*)(sp + 0x18) = r0;
    fn_800BC36C();
    r0 = *(u32*)lbl_8047E488;
    r4 = r1 + 0x10;
    r3 = 0x0;
    *(u32*)(sp + 0x10) = r0;
    fn_800BC3E0();
    r0 = *(u32*)lbl_8047E48C;
    r4 = r1 + 0xc;
    r3 = 0x1;
    *(u32*)(sp + 0xC) = r0;
    fn_800BC3E0();
    r0 = *(u32*)lbl_8047E490;
    r4 = r1 + 0x8;
    r3 = 0x2;
    *(u32*)(sp + 0x8) = r0;
    fn_800BC3E0();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x1;
    r6 = 0x2;
    r7 = 0x3;
    fn_800BC580();
    /* lmw r30, 0xb8(r1) */;
    return;
}
#pragma pop

/* 0x801E24B0 | size: 0x118 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E24B0(void) {
    extern void fn_800B884C();
    extern void fn_800BA6B0();
    extern void fn_800BC114();
    extern void fn_800BC52C();
    extern void fn_800BC580();
    extern void fn_800BC6F0();
    extern void fn_800BC8C8();
    extern void fn_800BCDDC();
    extern void fn_800BCE88();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    r3 = 0x1;
    r4 = 0x7;
    r5 = 0x0;
    fn_800BCE88();
    r3 = 0x0;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0xf;
    fn_800BCDDC();
    r3 = 0x1;
    fn_800B884C();
    r3 = 0x0;
    fn_800BA6B0();
    r3 = 0x1;
    fn_800BC8C8();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0xff;
    fn_800BC6F0();
    r3 = 0x0;
    r4 = 0x3;
    fn_800BC114();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x0;
    fn_800BC52C();
    r3 = 0x1;
    r4 = 0x0;
    r5 = 0x0;
    fn_800BC52C();
    r3 = 0x2;
    r4 = 0x0;
    r5 = 0x0;
    fn_800BC52C();
    r3 = 0x3;
    r4 = 0x0;
    r5 = 0x0;
    fn_800BC52C();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x1;
    r6 = 0x2;
    r7 = 0x3;
    fn_800BC580();
    r3 = 0x1;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x3;
    fn_800BC580();
    r3 = 0x2;
    r4 = 0x1;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x3;
    fn_800BC580();
    r3 = 0x3;
    r4 = 0x2;
    r5 = 0x2;
    r6 = 0x2;
    r7 = 0x3;
    fn_800BC580();
    return;
}
#pragma pop

/* 0x801E25C8 | size: 0x44 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E25C8(void) {
    extern u8 lbl_8046AC60[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = (u32)lbl_8046AC60;
    r4 = (u32)lbl_8046AC60;
    r0 = *(u32*)((u8*)r4 + 0xA0);
    if ((s32)r0 == (s32)0x0) goto L_801E2604;
    r0 = *(u8*)((u8*)r4 + 0xA4);
    if ((u32)r0 == (u32)0x0) goto L_801E2604;
    r3 = *(u32*)((u8*)r4 + 0xE8);
    if ((u32)r3 == (u32)0x0) goto L_801E2604;
    r3 = *(u32*)((u8*)r3 + 0xC);
    r0 = *(u32*)((u8*)r4 + 0xC0);
    r3 = r3 + r0;
    return;
L_801E2604: ;
    r3 = -0x1;
    return;
}
#pragma pop

/* 0x801E260C | size: 0x568 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E260C(void) {
    extern u8 lbl_8046A440[];
    extern u8 lbl_8046AC60[];
    extern u8 lbl_80478D00[];
    extern u8 lbl_80478D04[];
    extern u8 lbl_8047B470[];
    extern u8 lbl_8047B474[];
    extern void fn_8014E9B4();
    extern void fn_801E2B74();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r5 = (u32)lbl_8046AC60;
    r5 = (u32)lbl_8046AC60;
    r0 = 0x3e8;
    /* stmw r21, 0x24(r1) */;
    r7 = *(u32*)((u8*)r5 + 0x90);
    r7 = r7 * 0x28;
    r28 = (u32)r7 / (u32)r0;
    if ((u32)r7 == (u32)0x0) goto L_801E2B5C;
    r7 = (u32)lbl_8046A440;
    r8 = 0x0;
    r31 = (u32)lbl_8046A440;
    r7 = *(u32*)((u8*)r31 + 0x34);
    r0 = *(u32*)((u8*)r31 + 0x30);
    r7 = r7 + r4;
    /* addze r0, r0 */;
    r10 = r7 + r6;
    /* addze r9, r0 */;
    *(u32*)((u8*)r31 + 0x3C) = r10;
    *(u32*)((u8*)r31 + 0x38) = r9;
    goto L_801E26B4;
L_801E266C: ;
    r0 = r7 << 3;
    r7 = r31 + r0;
    r0 = *(u32*)((u8*)r7 + 0x4);
    r7 = *(u32*)((u8*)r7 + 0x0);
    r0 = r10 - r0;
    r0 = r9 - r7; /* -borrow */;
    r0 = r3 - r3; /* -borrow */;
    /* neg. r0, r0 */;
    if ((u32)r7 != (u32)0x0) goto L_801E26C4;
    r7 = *(u32*)((u8*)r31 + 0x28);
    r0 = r7 + 0x1;
    *(u32*)((u8*)r31 + 0x28) = r0;
    if ((s32)r0 < (s32)0x5) goto L_801E26A8;
    *(u32*)((u8*)r31 + 0x28) = r8;
L_801E26A8: ;
    r7 = *(u32*)((u8*)r5 + 0xE4);
    r0 = r7 + 0x1;
    *(u32*)((u8*)r5 + 0xE4) = r0;
L_801E26B4: ;
    r7 = *(u32*)((u8*)r31 + 0x28);
    r0 = *(u32*)((u8*)r31 + 0x2C);
    if ((s32)r7 != (s32)r0) goto L_801E266C;
L_801E26C4: ;
    r0 = r4 + r6;
    r30 = (u32)r28 >> 1;
    if ((u32)r0 < (u32)r30) goto L_801E2B5C;
    r29 = *(u32*)lbl_8047B470;
    if ((u32)r3 != (u32)r29) goto L_801E2900;
    r3 = (u32)lbl_8046AC60;
    r27 = (u32)lbl_8046AC60;
    r0 = *(u32*)((u8*)r27 + 0x8C);
    if ((u32)r0 != (u32)0x2) goto L_801E27D0;
    r3 = (u32)lbl_8046A440;
    r25 = *(u32*)lbl_8047B474;
    r26 = (u32)lbl_8046A440;
    r24 = r30;
    r23 = *(u32*)((u8*)r26 + 0x40);
    r22 = *(u32*)((u8*)r26 + 0x44);
L_801E270C: ;
    r3 = r29;
    r4 = r25;
    r5 = r24;
    r6 = r1 + 0x14;
    fn_801E2B74();
    r0 = *(u32*)(sp + 0x14);
    r22 = r22 + r3;
    /* addze r23, r23 */;
    if ((s32)r0 == (s32)0x0) goto L_801E27B4;
    if ((s32)r0 != (s32)0x1) goto L_801E2788;
    r0 = r3 << 1;
    r24 = r24 - r3;
    r29 = r29 + r0;
    if ((u32)r25 == (u32)0x0) goto L_801E2754;
    r25 = r25 + r0;
L_801E2754: ;
    r0 = *(u32*)((u8*)r31 + 0x2C);
    r0 = r0 << 3;
    r3 = r26 + r0;
    *(u32*)((u8*)r3 + 0x4) = r22;
    *(u32*)((u8*)r3 + 0x0) = r23;
    r3 = *(u32*)((u8*)r31 + 0x2C);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r31 + 0x2C) = r0;
    if ((s32)r0 < (s32)0x5) goto L_801E270C;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x2C) = r0;
    goto L_801E270C;
L_801E2788: ;
    r21 = r24 << 1;
    r3 = r29;
    r5 = r21;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
    if ((u32)r25 == (u32)0x0) goto L_801E27B4;
    r3 = r25;
    r5 = r21;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
L_801E27B4: ;
    r3 = *(u32*)((u8*)r26 + 0x44);
    r0 = *(u32*)((u8*)r26 + 0x40);
    r3 = r3 + r30;
    /* addze r0, r0 */;
    *(u32*)((u8*)r26 + 0x44) = r3;
    *(u32*)((u8*)r26 + 0x40) = r0;
    goto L_801E28A8;
L_801E27D0: ;
    r3 = (u32)lbl_8046A440;
    r23 = r30;
    r26 = (u32)lbl_8046A440;
    r22 = 0x0;
    r24 = *(u32*)((u8*)r26 + 0x40);
    r25 = *(u32*)((u8*)r26 + 0x44);
L_801E27E8: ;
    r3 = r29;
    r4 = r22;
    r5 = r23;
    r6 = r1 + 0x10;
    fn_801E2B74();
    r0 = *(u32*)(sp + 0x10);
    r25 = r25 + r3;
    /* addze r24, r24 */;
    if ((s32)r0 == (s32)0x0) goto L_801E2890;
    if ((s32)r0 != (s32)0x1) goto L_801E2864;
    r0 = r3 << 1;
    r23 = r23 - r3;
    r29 = r29 + r0;
    if ((u32)r22 == (u32)0x0) goto L_801E2830;
    r22 = r22 + r0;
L_801E2830: ;
    r0 = *(u32*)((u8*)r31 + 0x2C);
    r0 = r0 << 3;
    r3 = r26 + r0;
    *(u32*)((u8*)r3 + 0x4) = r25;
    *(u32*)((u8*)r3 + 0x0) = r24;
    r3 = *(u32*)((u8*)r31 + 0x2C);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r31 + 0x2C) = r0;
    if ((s32)r0 < (s32)0x5) goto L_801E27E8;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x2C) = r0;
    goto L_801E27E8;
L_801E2864: ;
    r21 = r23 << 1;
    r3 = r29;
    r5 = r21;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
    if ((u32)r22 == (u32)0x0) goto L_801E2890;
    r3 = r22;
    r5 = r21;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
L_801E2890: ;
    r3 = *(u32*)((u8*)r26 + 0x44);
    r0 = *(u32*)((u8*)r26 + 0x40);
    r3 = r3 + r30;
    /* addze r0, r0 */;
    *(u32*)((u8*)r26 + 0x44) = r3;
    *(u32*)((u8*)r26 + 0x40) = r0;
L_801E28A8: ;
    r3 = *(u32*)lbl_8047B470;
    r4 = r28;
    DCFlushRange();
    r3 = *(u32*)lbl_80478D00;
    r5 = r30;
    r4 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    fn_8014E9B4();
    r0 = *(u32*)((u8*)r27 + 0x8C);
    if ((u32)r0 != (u32)0x2) goto L_801E2B3C;
    r3 = *(u32*)lbl_8047B474;
    r4 = r28;
    DCFlushRange();
    r3 = *(u32*)lbl_80478D04;
    r5 = r30;
    r4 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    fn_8014E9B4();
    goto L_801E2B3C;
L_801E2900: ;
    r3 = (u32)lbl_8046AC60;
    r26 = (u32)lbl_8046AC60;
    r0 = *(u32*)((u8*)r26 + 0x8C);
    if ((u32)r0 != (u32)0x2) goto L_801E29FC;
    r3 = (u32)lbl_8046A440;
    r4 = r30 << 1;
    r27 = (u32)lbl_8046A440;
    r0 = *(u32*)lbl_8047B474;
    r25 = *(u32*)((u8*)r27 + 0x40);
    r24 = r30;
    r21 = *(u32*)((u8*)r27 + 0x44);
    r23 = r29 + r4;
    r22 = r0 + r4;
L_801E2938: ;
    r3 = r23;
    r4 = r22;
    r5 = r24;
    r6 = r1 + 0xc;
    fn_801E2B74();
    r0 = *(u32*)(sp + 0xC);
    r21 = r21 + r3;
    /* addze r25, r25 */;
    if ((s32)r0 == (s32)0x0) goto L_801E29E0;
    if ((s32)r0 != (s32)0x1) goto L_801E29B4;
    r0 = r3 << 1;
    r24 = r24 - r3;
    r23 = r23 + r0;
    if ((u32)r22 == (u32)0x0) goto L_801E2980;
    r22 = r22 + r0;
L_801E2980: ;
    r0 = *(u32*)((u8*)r31 + 0x2C);
    r0 = r0 << 3;
    r3 = r27 + r0;
    *(u32*)((u8*)r3 + 0x4) = r21;
    *(u32*)((u8*)r3 + 0x0) = r25;
    r3 = *(u32*)((u8*)r31 + 0x2C);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r31 + 0x2C) = r0;
    if ((s32)r0 < (s32)0x5) goto L_801E2938;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x2C) = r0;
    goto L_801E2938;
L_801E29B4: ;
    r21 = r24 << 1;
    r3 = r23;
    r5 = r21;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
    if ((u32)r22 == (u32)0x0) goto L_801E29E0;
    r3 = r22;
    r5 = r21;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
L_801E29E0: ;
    r3 = *(u32*)((u8*)r27 + 0x44);
    r0 = *(u32*)((u8*)r27 + 0x40);
    r3 = r3 + r30;
    /* addze r0, r0 */;
    *(u32*)((u8*)r27 + 0x44) = r3;
    *(u32*)((u8*)r27 + 0x40) = r0;
    goto L_801E2ADC;
L_801E29FC: ;
    r3 = (u32)lbl_8046A440;
    r0 = r30 << 1;
    r27 = (u32)lbl_8046A440;
    r21 = r30;
    r22 = *(u32*)((u8*)r27 + 0x40);
    r24 = r29 + r0;
    r25 = *(u32*)((u8*)r27 + 0x44);
    r23 = 0x0;
L_801E2A1C: ;
    r3 = r24;
    r4 = r23;
    r5 = r21;
    r6 = r1 + 0x8;
    fn_801E2B74();
    r0 = *(u32*)(sp + 0x8);
    r25 = r25 + r3;
    /* addze r22, r22 */;
    if ((s32)r0 == (s32)0x0) goto L_801E2AC4;
    if ((s32)r0 != (s32)0x1) goto L_801E2A98;
    r0 = r3 << 1;
    r21 = r21 - r3;
    r24 = r24 + r0;
    if ((u32)r23 == (u32)0x0) goto L_801E2A64;
    r23 = r23 + r0;
L_801E2A64: ;
    r0 = *(u32*)((u8*)r31 + 0x2C);
    r0 = r0 << 3;
    r3 = r27 + r0;
    *(u32*)((u8*)r3 + 0x4) = r25;
    *(u32*)((u8*)r3 + 0x0) = r22;
    r3 = *(u32*)((u8*)r31 + 0x2C);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r31 + 0x2C) = r0;
    if ((s32)r0 < (s32)0x5) goto L_801E2A1C;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x2C) = r0;
    goto L_801E2A1C;
L_801E2A98: ;
    r21 = r21 << 1;
    r3 = r24;
    r5 = r21;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
    if ((u32)r23 == (u32)0x0) goto L_801E2AC4;
    r3 = r23;
    r5 = r21;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
L_801E2AC4: ;
    r3 = *(u32*)((u8*)r27 + 0x44);
    r0 = *(u32*)((u8*)r27 + 0x40);
    r3 = r3 + r30;
    /* addze r0, r0 */;
    *(u32*)((u8*)r27 + 0x44) = r3;
    *(u32*)((u8*)r27 + 0x40) = r0;
L_801E2ADC: ;
    r0 = *(u32*)lbl_8047B470;
    r21 = r30 << 1;
    r4 = r28;
    r3 = r0 + r21;
    DCFlushRange();
    r3 = *(u32*)lbl_80478D00;
    r4 = r30;
    r5 = r30;
    r6 = 0x0;
    r7 = 0x0;
    fn_8014E9B4();
    r0 = *(u32*)((u8*)r26 + 0x8C);
    if ((u32)r0 != (u32)0x2) goto L_801E2B3C;
    r0 = *(u32*)lbl_8047B474;
    r4 = r30;
    r3 = r0 + r21;
    DCFlushRange();
    r3 = *(u32*)lbl_80478D04;
    r4 = r30;
    r5 = r30;
    r6 = 0x0;
    r7 = 0x0;
    fn_8014E9B4();
L_801E2B3C: ;
    r4 = *(u32*)((u8*)r31 + 0x34);
    r3 = r30;
    r0 = *(u32*)((u8*)r31 + 0x30);
    r4 = r4 + r30;
    /* addze r0, r0 */;
    *(u32*)((u8*)r31 + 0x34) = r4;
    *(u32*)((u8*)r31 + 0x30) = r0;
    goto L_801E2B60;
L_801E2B5C: ;
    r3 = 0x0;
L_801E2B60: ;
    /* lmw r21, 0x24(r1) */;
    return;
}
#pragma pop

/* 0x801E2B74 | size: 0x134 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E2B74(void) {
    extern u8 lbl_8046AC60[];
    extern void fn_801E4AC4();
    extern void fn_801E4B08();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r7 = (u32)lbl_8046AC60;
    /* stmw r26, 0x8(r1) */;
    r29 = (u32)lbl_8046AC60;
    r26 = r3;
    r27 = r4;
    r28 = r5;
    r30 = r6;
    r0 = *(u32*)((u8*)r29 + 0xEC);
    if ((u32)r0 != (u32)0x0) goto L_801E2BCC;
    r3 = 0x0;
    fn_801E4AC4();
    *(u32*)((u8*)r29 + 0xEC) = r3;
    if ((u32)r3 != (u32)0x0) goto L_801E2BCC;
    r0 = 0x2;
    r3 = 0x0;
    *(u32*)((u8*)r30 + 0x0) = r0;
    goto L_801E2C94;
L_801E2BCC: ;
    r3 = *(u32*)((u8*)r29 + 0xEC);
    r0 = *(u32*)((u8*)r3 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_801E2C90;
    if ((u32)r0 < (u32)r28) goto L_801E2BE8;
    r0 = r28;
L_801E2BE8: ;
    r4 = *(u32*)((u8*)r3 + 0x4);
    r31 = r0;
    if ((u32)r27 != (u32)0x0) goto L_801E2C1C;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r0 <= (u32)0x0) goto L_801E2C48;
L_801E2C04: ;
    r0 = *(s16*)((u8*)r4 + 0x2);
    r4 = r4 + 0x4;
    *(u16*)((u8*)r26 + 0x0) = r0;
    r26 = r26 + 0x2;
    if (--ctr != 0) goto L_801E2C04;
    goto L_801E2C48;
L_801E2C1C: ;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r0 <= (u32)0x0) goto L_801E2C48;
L_801E2C28: ;
    r0 = *(s16*)((u8*)r4 + 0x0);
    *(u16*)((u8*)r27 + 0x0) = r0;
    r27 = r27 + 0x2;
    r0 = *(s16*)((u8*)r4 + 0x2);
    r4 = r4 + 0x4;
    *(u16*)((u8*)r26 + 0x0) = r0;
    r26 = r26 + 0x2;
    if (--ctr != 0) goto L_801E2C28;
L_801E2C48: ;
    r3 = *(u32*)((u8*)r29 + 0xEC);
    r0 = *(u32*)((u8*)r3 + 0x8);
    r0 = r0 - r31;
    *(u32*)((u8*)r3 + 0x8) = r0;
    r3 = *(u32*)((u8*)r29 + 0xEC);
    *(u32*)((u8*)r3 + 0x4) = r4;
    r3 = *(u32*)((u8*)r29 + 0xEC);
    r0 = *(u32*)((u8*)r3 + 0x8);
    if ((u32)r0 != (u32)0x0) goto L_801E2C88;
    fn_801E4B08();
    r3 = 0x0;
    r0 = 0x1;
    *(u32*)((u8*)r29 + 0xEC) = r3;
    *(u32*)((u8*)r30 + 0x0) = r0;
    goto L_801E2C90;
L_801E2C88: ;
    r0 = 0x0;
    *(u32*)((u8*)r30 + 0x0) = r0;
L_801E2C90: ;
    r3 = r31;
L_801E2C94: ;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801E2CA8 | size: 0x848 | massive */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E2CA8(void) {
    extern u8 lbl_8046A440[];
    extern u8 lbl_80478D00[];
    extern u8 lbl_80478D04[];
    extern u8 lbl_8047B470[];
    extern u8 lbl_8047B474[];
    extern void fn_8014E9B4();
    extern void fn_801E2B74();
    u8 sp[0x60];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r17 = 0;
    u32 r18 = 0;
    u32 r19 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = (u32)lbl_8046A440;
    r5 = 0x3e8;
    /* stmw r17, 0x24(r1) */;
    r30 = (u32)lbl_8046A440;
    r31 = r30 + 0x0;
    r7 = r30 + 0x820;
    r0 = *(u32*)((u8*)r7 + 0x90);
    r3 = *(u32*)((u8*)r31 + 0x38);
    r6 = r0 * 0x28;
    r18 = *(u32*)((u8*)r31 + 0x40);
    r4 = *(u32*)((u8*)r31 + 0x3C);
    r19 = *(u32*)((u8*)r31 + 0x44);
    r0 = r3 ^ r18;
    r29 = (u32)r6 / (u32)r5;
    r5 = r4 ^ r19;
    /* or. r0, r5, r0 */;
    if ((s32)r0 != (s32)0) goto L_801E2EBC;
    r0 = *(u32*)((u8*)r7 + 0x8C);
    r17 = 0x0;
    *(u32*)((u8*)r31 + 0x44) = r17;
    *(u32*)((u8*)r31 + 0x40) = r17;
    if ((u32)r0 != (u32)0x2) goto L_801E2DE8;
    r20 = 0x0;
    r18 = *(u32*)lbl_8047B470;
    r17 = *(u32*)lbl_8047B474;
    r19 = r29;
    r21 = r20;
L_801E2D24: ;
    r3 = r18;
    r4 = r17;
    r5 = r19;
    r6 = r1 + 0x1c;
    fn_801E2B74();
    r0 = *(u32*)(sp + 0x1C);
    r21 = r21 + r3;
    /* addze r20, r20 */;
    if ((s32)r0 == (s32)0x0) goto L_801E2DCC;
    if ((s32)r0 != (s32)0x1) goto L_801E2DA0;
    r0 = r3 << 1;
    r19 = r19 - r3;
    r18 = r18 + r0;
    if ((u32)r17 == (u32)0x0) goto L_801E2D6C;
    r17 = r17 + r0;
L_801E2D6C: ;
    r0 = *(u32*)((u8*)r31 + 0x2C);
    r0 = r0 << 3;
    r3 = r31 + r0;
    *(u32*)((u8*)r3 + 0x4) = r21;
    *(u32*)((u8*)r3 + 0x0) = r20;
    r3 = *(u32*)((u8*)r31 + 0x2C);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r31 + 0x2C) = r0;
    if ((s32)r0 < (s32)0x5) goto L_801E2D24;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x2C) = r0;
    goto L_801E2D24;
L_801E2DA0: ;
    r19 = r19 << 1;
    r3 = r18;
    r5 = r19;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
    if ((u32)r17 == (u32)0x0) goto L_801E2DCC;
    r3 = r17;
    r5 = r19;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
L_801E2DCC: ;
    r3 = *(u32*)((u8*)r31 + 0x44);
    r0 = *(u32*)((u8*)r31 + 0x40);
    r3 = r3 + r29;
    /* addze r0, r0 */;
    *(u32*)((u8*)r31 + 0x44) = r3;
    *(u32*)((u8*)r31 + 0x40) = r0;
    goto L_801E3468;
L_801E2DE8: ;
    r20 = 0x0;
    r18 = *(u32*)lbl_8047B470;
    r19 = r29;
    r21 = r20;
L_801E2DF8: ;
    r3 = r18;
    r4 = r17;
    r5 = r19;
    r6 = r1 + 0x18;
    fn_801E2B74();
    r0 = *(u32*)(sp + 0x18);
    r21 = r21 + r3;
    /* addze r20, r20 */;
    if ((s32)r0 == (s32)0x0) goto L_801E2EA0;
    if ((s32)r0 != (s32)0x1) goto L_801E2E74;
    r0 = r3 << 1;
    r19 = r19 - r3;
    r18 = r18 + r0;
    if ((u32)r17 == (u32)0x0) goto L_801E2E40;
    r17 = r17 + r0;
L_801E2E40: ;
    r0 = *(u32*)((u8*)r31 + 0x2C);
    r0 = r0 << 3;
    r3 = r31 + r0;
    *(u32*)((u8*)r3 + 0x4) = r21;
    *(u32*)((u8*)r3 + 0x0) = r20;
    r3 = *(u32*)((u8*)r31 + 0x2C);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r31 + 0x2C) = r0;
    if ((s32)r0 < (s32)0x5) goto L_801E2DF8;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x2C) = r0;
    goto L_801E2DF8;
L_801E2E74: ;
    r19 = r19 << 1;
    r3 = r18;
    r5 = r19;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
    if ((u32)r17 == (u32)0x0) goto L_801E2EA0;
    r3 = r17;
    r5 = r19;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
L_801E2EA0: ;
    r3 = *(u32*)((u8*)r31 + 0x44);
    r0 = *(u32*)((u8*)r31 + 0x40);
    r3 = r3 + r29;
    /* addze r0, r0 */;
    *(u32*)((u8*)r31 + 0x44) = r3;
    *(u32*)((u8*)r31 + 0x40) = r0;
    goto L_801E3468;
L_801E2EBC: ;
    r24 = r29;
    r6 = r29;
    r5 = 0x0;
    __mod2u();
    r28 = r4;
    r3 = r18;
    r4 = r19;
    r6 = r24;
    r5 = 0x0;
    __mod2u();
    /* mr. r26, r4 */;
    if ((u32)r17 != (u32)0x0) goto L_801E2EF0;
    r26 = r29;
L_801E2EF0: ;
    if ((u32)r28 >= (u32)r26) goto L_801E3170;
    r0 = r26 - r28;
    r3 = *(u32*)lbl_8047B470;
    r17 = r28 << 1;
    r19 = r0 << 1;
    r4 = r3 + r17;
    r5 = r19;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r18 = r30 + 0x820;
    r0 = *(u32*)((u8*)r18 + 0x8C);
    if ((u32)r0 != (u32)0x2) goto L_801E2F34;
    r3 = *(u32*)lbl_8047B474;
    r5 = r19;
    r4 = r3 + r17;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
L_801E2F34: ;
    r17 = r30 + 0x0;
    r21 = 0x0;
    r20 = *(u32*)((u8*)r17 + 0x28);
    goto L_801E2F84;
L_801E2F44: ;
    r0 = r20 << 3;
    r22 = r30 + 0x0;
    r22 = r22 + r0;
    r6 = r24;
    r3 = *(u32*)((u8*)r22 + 0x0);
    r5 = 0x0;
    r4 = *(u32*)((u8*)r22 + 0x4);
    __mod2u();
    r0 = r4 - r28;
    r20 = r20 + 0x1;
    *(u32*)((u8*)r22 + 0x4) = r0;
    r0 = r3 - r21; /* -borrow */;
    *(u32*)((u8*)r22 + 0x0) = r0;
    if ((s32)r20 < (s32)0x5) goto L_801E2F84;
    r20 = 0x0;
L_801E2F84: ;
    r0 = *(u32*)((u8*)r17 + 0x2C);
    if ((s32)r20 != (s32)r0) goto L_801E2F44;
    r3 = (u32)r19 >> 1;
    r0 = *(u32*)((u8*)r18 + 0x8C);
    r20 = r29 - r3;
    r21 = 0x0;
    r18 = r29 - r20;
    r0 = *(u32*)lbl_8047B470;
    /* clrrwi r3, r19, 1 */;
    *(u32*)((u8*)r31 + 0x44) = r18;
    r4 = r0 + r3;
    *(u32*)((u8*)r31 + 0x40) = r21;
    if ((u32)r0 != (u32)0x2) goto L_801E309C;
    r0 = *(u32*)lbl_8047B474;
    r23 = r20;
    r22 = r4;
    r19 = r30 + 0x0;
    r21 = r0 + r3;
    r24 = 0x0;
L_801E2FD8: ;
    r3 = r22;
    r4 = r21;
    r5 = r23;
    r6 = r1 + 0x14;
    fn_801E2B74();
    r0 = *(u32*)(sp + 0x14);
    r18 = r18 + r3;
    /* addze r24, r24 */;
    if ((s32)r0 == (s32)0x0) goto L_801E3080;
    if ((s32)r0 != (s32)0x1) goto L_801E3054;
    r0 = r3 << 1;
    r23 = r23 - r3;
    r22 = r22 + r0;
    if ((u32)r21 == (u32)0x0) goto L_801E3020;
    r21 = r21 + r0;
L_801E3020: ;
    r0 = *(u32*)((u8*)r17 + 0x2C);
    r0 = r0 << 3;
    r3 = r19 + r0;
    *(u32*)((u8*)r3 + 0x4) = r18;
    *(u32*)((u8*)r3 + 0x0) = r24;
    r3 = *(u32*)((u8*)r17 + 0x2C);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r17 + 0x2C) = r0;
    if ((s32)r0 < (s32)0x5) goto L_801E2FD8;
    r0 = 0x0;
    *(u32*)((u8*)r17 + 0x2C) = r0;
    goto L_801E2FD8;
L_801E3054: ;
    r17 = r23 << 1;
    r3 = r22;
    r5 = r17;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
    if ((u32)r21 == (u32)0x0) goto L_801E3080;
    r3 = r21;
    r5 = r17;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
L_801E3080: ;
    r3 = *(u32*)((u8*)r31 + 0x44);
    r0 = *(u32*)((u8*)r31 + 0x40);
    r3 = r3 + r20;
    /* addze r0, r0 */;
    *(u32*)((u8*)r31 + 0x44) = r3;
    *(u32*)((u8*)r31 + 0x40) = r0;
    goto L_801E3468;
L_801E309C: ;
    r22 = r20;
    r19 = r4;
    r24 = r30 + 0x0;
    r23 = 0x0;
L_801E30AC: ;
    r3 = r19;
    r4 = r21;
    r5 = r22;
    r6 = r1 + 0x10;
    fn_801E2B74();
    r0 = *(u32*)(sp + 0x10);
    r18 = r18 + r3;
    /* addze r23, r23 */;
    if ((s32)r0 == (s32)0x0) goto L_801E3154;
    if ((s32)r0 != (s32)0x1) goto L_801E3128;
    r0 = r3 << 1;
    r22 = r22 - r3;
    r19 = r19 + r0;
    if ((u32)r21 == (u32)0x0) goto L_801E30F4;
    r21 = r21 + r0;
L_801E30F4: ;
    r0 = *(u32*)((u8*)r17 + 0x2C);
    r0 = r0 << 3;
    r3 = r24 + r0;
    *(u32*)((u8*)r3 + 0x4) = r18;
    *(u32*)((u8*)r3 + 0x0) = r23;
    r3 = *(u32*)((u8*)r17 + 0x2C);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r17 + 0x2C) = r0;
    if ((s32)r0 < (s32)0x5) goto L_801E30AC;
    r0 = 0x0;
    *(u32*)((u8*)r17 + 0x2C) = r0;
    goto L_801E30AC;
L_801E3128: ;
    r17 = r22 << 1;
    r3 = r19;
    r5 = r17;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
    if ((u32)r21 == (u32)0x0) goto L_801E3154;
    r3 = r21;
    r5 = r17;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
L_801E3154: ;
    r3 = *(u32*)((u8*)r31 + 0x44);
    r0 = *(u32*)((u8*)r31 + 0x40);
    r3 = r3 + r20;
    /* addze r0, r0 */;
    *(u32*)((u8*)r31 + 0x44) = r3;
    *(u32*)((u8*)r31 + 0x40) = r0;
    goto L_801E3468;
L_801E3170: ;
    r18 = (u32)r29 >> 2;
    r4 = *(u32*)lbl_8047B470;
    r5 = r18;
    r3 = r30 + 0xa0;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r23 = r29 - r28;
    r3 = *(u32*)lbl_8047B470;
    r25 = r28 << 1;
    r20 = r23 << 1;
    r4 = r3 + r25;
    r5 = r20;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r0 = *(u32*)lbl_8047B470;
    /* clrrwi r19, r20, 1 */;
    r5 = r18;
    r4 = r30 + 0xa0;
    r3 = r0 + r19;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r22 = r30 + 0x820;
    r0 = *(u32*)((u8*)r22 + 0x8C);
    if ((u32)r0 != (u32)0x2) goto L_801E31FC;
    r4 = *(u32*)lbl_8047B474;
    r5 = r18;
    r3 = r30 + 0xa0;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r3 = *(u32*)lbl_8047B474;
    r5 = r20;
    r4 = r3 + r25;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r0 = *(u32*)lbl_8047B474;
    r5 = r18;
    r4 = r30 + 0xa0;
    r3 = r0 + r19;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
L_801E31FC: ;
    r27 = r30 + 0x0;
    r21 = (u32)r29 >> 1;
    r18 = *(u32*)((u8*)r27 + 0x28);
    r20 = 0x0;
    goto L_801E3278;
L_801E3210: ;
    r0 = r18 << 3;
    r19 = r30 + 0x0;
    r19 = r19 + r0;
    r6 = r24;
    r3 = *(u32*)((u8*)r19 + 0x0);
    r5 = 0x0;
    r4 = *(u32*)((u8*)r19 + 0x4);
    __mod2u();
    r0 = r21 - r4;
    r0 = r20 - r3; /* -borrow */;
    r0 = r17 - r17; /* -borrow */;
    /* neg. r0, r0 */;
    if ((u32)r0 == (u32)0x2) goto L_801E3258;
    r0 = r4 - r28;
    *(u32*)((u8*)r19 + 0x4) = r0;
    r0 = r3 - r20; /* -borrow */;
    *(u32*)((u8*)r19 + 0x0) = r0;
    goto L_801E3268;
L_801E3258: ;
    r0 = r4 + r23;
    *(u32*)((u8*)r19 + 0x4) = r0;
    r0 = r3 + r20; /* +carry */;
    *(u32*)((u8*)r19 + 0x0) = r0;
L_801E3268: ;
    r18 = r18 + 0x1;
    if ((s32)r18 < (s32)0x5) goto L_801E3278;
    r18 = 0x0;
L_801E3278: ;
    r0 = *(u32*)((u8*)r27 + 0x2C);
    if ((s32)r18 != (s32)r0) goto L_801E3210;
    r0 = *(u32*)((u8*)r22 + 0x8C);
    r17 = r28 - r26;
    r3 = *(u32*)lbl_8047B470;
    r4 = r29 << 1;
    r18 = r29 - r17;
    r21 = 0x0;
    r3 = r3 + r4;
    *(u32*)((u8*)r31 + 0x44) = r18;
    r0 = r3 - r25;
    r3 = r26 << 1;
    *(u32*)((u8*)r31 + 0x40) = r21;
    r19 = r0 + r3;
    if ((u32)r0 != (u32)0x2) goto L_801E339C;
    r0 = *(u32*)lbl_8047B474;
    r22 = r17;
    r20 = r30 + 0x0;
    r23 = 0x0;
    r0 = r0 + r4;
    r0 = r0 - r25;
    r21 = r0 + r3;
L_801E32D8: ;
    r3 = r19;
    r4 = r21;
    r5 = r22;
    r6 = r1 + 0xc;
    fn_801E2B74();
    r0 = *(u32*)(sp + 0xC);
    r18 = r18 + r3;
    /* addze r23, r23 */;
    if ((s32)r0 == (s32)0x0) goto L_801E3380;
    if ((s32)r0 != (s32)0x1) goto L_801E3354;
    r0 = r3 << 1;
    r22 = r22 - r3;
    r19 = r19 + r0;
    if ((u32)r21 == (u32)0x0) goto L_801E3320;
    r21 = r21 + r0;
L_801E3320: ;
    r0 = *(u32*)((u8*)r27 + 0x2C);
    r0 = r0 << 3;
    r3 = r20 + r0;
    *(u32*)((u8*)r3 + 0x4) = r18;
    *(u32*)((u8*)r3 + 0x0) = r23;
    r3 = *(u32*)((u8*)r27 + 0x2C);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r27 + 0x2C) = r0;
    if ((s32)r0 < (s32)0x5) goto L_801E32D8;
    r0 = 0x0;
    *(u32*)((u8*)r27 + 0x2C) = r0;
    goto L_801E32D8;
L_801E3354: ;
    r18 = r22 << 1;
    r3 = r19;
    r5 = r18;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
    if ((u32)r21 == (u32)0x0) goto L_801E3380;
    r3 = r21;
    r5 = r18;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
L_801E3380: ;
    r3 = *(u32*)((u8*)r31 + 0x44);
    r0 = *(u32*)((u8*)r31 + 0x40);
    r3 = r3 + r17;
    /* addze r0, r0 */;
    *(u32*)((u8*)r31 + 0x44) = r3;
    *(u32*)((u8*)r31 + 0x40) = r0;
    goto L_801E3468;
L_801E339C: ;
    r22 = r17;
    r20 = r30 + 0x0;
    r23 = 0x0;
L_801E33A8: ;
    r3 = r19;
    r4 = r21;
    r5 = r22;
    r6 = r1 + 0x8;
    fn_801E2B74();
    r0 = *(u32*)(sp + 0x8);
    r18 = r18 + r3;
    /* addze r23, r23 */;
    if ((s32)r0 == (s32)0x0) goto L_801E3450;
    if ((s32)r0 != (s32)0x1) goto L_801E3424;
    r0 = r3 << 1;
    r22 = r22 - r3;
    r19 = r19 + r0;
    if ((u32)r21 == (u32)0x0) goto L_801E33F0;
    r21 = r21 + r0;
L_801E33F0: ;
    r0 = *(u32*)((u8*)r27 + 0x2C);
    r0 = r0 << 3;
    r3 = r20 + r0;
    *(u32*)((u8*)r3 + 0x4) = r18;
    *(u32*)((u8*)r3 + 0x0) = r23;
    r3 = *(u32*)((u8*)r27 + 0x2C);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r27 + 0x2C) = r0;
    if ((s32)r0 < (s32)0x5) goto L_801E33A8;
    r0 = 0x0;
    *(u32*)((u8*)r27 + 0x2C) = r0;
    goto L_801E33A8;
L_801E3424: ;
    r18 = r22 << 1;
    r3 = r19;
    r5 = r18;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
    if ((u32)r21 == (u32)0x0) goto L_801E3450;
    r3 = r21;
    r5 = r18;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
L_801E3450: ;
    r3 = *(u32*)((u8*)r31 + 0x44);
    r0 = *(u32*)((u8*)r31 + 0x40);
    r3 = r3 + r17;
    /* addze r0, r0 */;
    *(u32*)((u8*)r31 + 0x44) = r3;
    *(u32*)((u8*)r31 + 0x40) = r0;
L_801E3468: ;
    r0 = 0x0;
    r5 = r30 + 0x0;
    r17 = r29 << 1;
    *(u32*)((u8*)r5 + 0x34) = r0;
    r3 = *(u32*)lbl_8047B470;
    r4 = r17;
    *(u32*)((u8*)r5 + 0x30) = r0;
    *(u32*)((u8*)r31 + 0x3C) = r0;
    *(u32*)((u8*)r31 + 0x38) = r0;
    DCFlushRange();
    r3 = *(u32*)lbl_80478D00;
    r5 = r29;
    r4 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    fn_8014E9B4();
    r3 = r30 + 0x820;
    r0 = *(u32*)((u8*)r3 + 0x8C);
    if ((u32)r0 != (u32)0x2) goto L_801E34DC;
    r3 = *(u32*)lbl_8047B474;
    r4 = r17;
    DCFlushRange();
    r3 = *(u32*)lbl_80478D04;
    r5 = r29;
    r4 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    fn_8014E9B4();
L_801E34DC: ;
    /* lmw r17, 0x24(r1) */;
    return;
}
#pragma pop

/* 0x801E34F0 | size: 0x368 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E34F0(void) {
    extern u8 lbl_8046A440[];
    extern u8 lbl_8046AC60[];
    extern u8 lbl_80478D00[];
    extern u8 lbl_80478D04[];
    extern u8 lbl_8047B470[];
    extern u8 lbl_8047B474[];
    extern void fn_8014E9B4();
    extern void fn_8014EE40();
    extern void fn_8014F838();
    extern void fn_801E2B74();
    extern void fn_801E260C();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = (u32)fn_801E260C;
    r4 = (u32)lbl_8046AC60;
    r7 = 0x3e8;
    r9 = 0x0;
    r3 = (u32)fn_801E260C;
    /* stmw r24, 0x30(r1) */;
    r30 = (u32)lbl_8046AC60;
    r4 = (0x3 << 16);
    r0 = 0x1;
    r10 = 0x0;
    r6 = *(u32*)((u8*)r30 + 0x90);
    r5 = *(u32*)((u8*)r30 + 0x8C);
    r8 = r6 * 0x28;
    /* subi r5, r5, 0x2 */;
    r29 = (u32)r8 / (u32)r7;
    /* subic r7, r5, 0x1 */;
    r5 = 0x40;
    r4 = r7 - r7; /* -borrow */;
    r8 = r5 & ~r4;
    r5 = r29;
    r3 = 0xff;
    *(u32*)(sp + 0x18) = r0;
    r7 = 0x7f;
    r9 = 0x0;
    r4 = *(u32*)lbl_8047B470;
    fn_8014EE40();
    r0 = r3 + (0x1 << 16);
    *(u32*)lbl_80478D00 = r3;
    if ((u32)r0 != (u32)0xffff) goto L_801E3590;
    r3 = 0x0;
    goto L_801E3844;
L_801E3590: ;
    r0 = *(u32*)((u8*)r30 + 0x8C);
    if ((u32)r0 != (u32)0x2) goto L_801E3608;
    r6 = 0x0;
    r3 = (u32)fn_801E260C;
    r4 = (0x3 << 16);
    r0 = (u32)fn_801E260C;
    r5 = r29;
    r3 = 0xff;
    r7 = 0x7f;
    r8 = 0x7f;
    r9 = 0x0;
    r10 = 0x0;
    *(u32*)(sp + 0x14) = r0;
    r4 = *(u32*)lbl_8047B474;
    r6 = *(u32*)((u8*)r30 + 0x90);
    fn_8014EE40();
    r0 = r3 + (0x1 << 16);
    *(u32*)lbl_80478D04 = r3;
    if ((u32)r0 != (u32)0xffff) goto L_801E3608;
    r3 = *(u32*)lbl_80478D00;
    fn_8014F838();
    r3 = 0x0;
    goto L_801E3844;
L_801E3608: ;
    r0 = *(u32*)((u8*)r30 + 0x8C);
    r3 = (u32)lbl_8046A440;
    r31 = (u32)lbl_8046A440;
    r24 = 0x0;
    *(u32*)((u8*)r31 + 0x28) = r24;
    *(u32*)((u8*)r31 + 0x2C) = r24;
    *(u32*)((u8*)r31 + 0x34) = r24;
    *(u32*)((u8*)r31 + 0x30) = r24;
    *(u32*)((u8*)r31 + 0x3C) = r24;
    *(u32*)((u8*)r31 + 0x38) = r24;
    *(u32*)((u8*)r31 + 0x44) = r24;
    *(u32*)((u8*)r31 + 0x40) = r24;
    if ((u32)r0 != (u32)0x2) goto L_801E3718;
    r25 = 0x0;
    r27 = *(u32*)lbl_8047B470;
    r28 = *(u32*)lbl_8047B474;
    r26 = r29;
    r24 = r25;
L_801E3654: ;
    r3 = r27;
    r4 = r28;
    r5 = r26;
    r6 = r1 + 0x24;
    fn_801E2B74();
    r0 = *(u32*)(sp + 0x24);
    r24 = r24 + r3;
    /* addze r25, r25 */;
    if ((s32)r0 == (s32)0x0) goto L_801E36FC;
    if ((s32)r0 != (s32)0x1) goto L_801E36D0;
    r0 = r3 << 1;
    r26 = r26 - r3;
    r27 = r27 + r0;
    if ((u32)r28 == (u32)0x0) goto L_801E369C;
    r28 = r28 + r0;
L_801E369C: ;
    r0 = *(u32*)((u8*)r31 + 0x2C);
    r0 = r0 << 3;
    r3 = r31 + r0;
    *(u32*)((u8*)r3 + 0x4) = r24;
    *(u32*)((u8*)r3 + 0x0) = r25;
    r3 = *(u32*)((u8*)r31 + 0x2C);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r31 + 0x2C) = r0;
    if ((s32)r0 < (s32)0x5) goto L_801E3654;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x2C) = r0;
    goto L_801E3654;
L_801E36D0: ;
    r24 = r26 << 1;
    r3 = r27;
    r5 = r24;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
    if ((u32)r28 == (u32)0x0) goto L_801E36FC;
    r3 = r28;
    r5 = r24;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
L_801E36FC: ;
    r3 = *(u32*)((u8*)r31 + 0x44);
    r0 = *(u32*)((u8*)r31 + 0x40);
    r3 = r3 + r29;
    /* addze r0, r0 */;
    *(u32*)((u8*)r31 + 0x44) = r3;
    *(u32*)((u8*)r31 + 0x40) = r0;
    goto L_801E37E8;
L_801E3718: ;
    r27 = 0x0;
    r25 = *(u32*)lbl_8047B470;
    r26 = r29;
    r28 = r27;
L_801E3728: ;
    r3 = r25;
    r4 = r24;
    r5 = r26;
    r6 = r1 + 0x20;
    fn_801E2B74();
    r0 = *(u32*)(sp + 0x20);
    r28 = r28 + r3;
    /* addze r27, r27 */;
    if ((s32)r0 == (s32)0x0) goto L_801E37D0;
    if ((s32)r0 != (s32)0x1) goto L_801E37A4;
    r0 = r3 << 1;
    r26 = r26 - r3;
    r25 = r25 + r0;
    if ((u32)r24 == (u32)0x0) goto L_801E3770;
    r24 = r24 + r0;
L_801E3770: ;
    r0 = *(u32*)((u8*)r31 + 0x2C);
    r0 = r0 << 3;
    r3 = r31 + r0;
    *(u32*)((u8*)r3 + 0x4) = r28;
    *(u32*)((u8*)r3 + 0x0) = r27;
    r3 = *(u32*)((u8*)r31 + 0x2C);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r31 + 0x2C) = r0;
    if ((s32)r0 < (s32)0x5) goto L_801E3728;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x2C) = r0;
    goto L_801E3728;
L_801E37A4: ;
    r26 = r26 << 1;
    r3 = r25;
    r5 = r26;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
    if ((u32)r24 == (u32)0x0) goto L_801E37D0;
    r3 = r24;
    r5 = r26;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
L_801E37D0: ;
    r3 = *(u32*)((u8*)r31 + 0x44);
    r0 = *(u32*)((u8*)r31 + 0x40);
    r3 = r3 + r29;
    /* addze r0, r0 */;
    *(u32*)((u8*)r31 + 0x44) = r3;
    *(u32*)((u8*)r31 + 0x40) = r0;
L_801E37E8: ;
    r24 = r29 << 1;
    r3 = *(u32*)lbl_8047B470;
    r4 = r24;
    DCFlushRange();
    r3 = *(u32*)lbl_80478D00;
    r5 = r29;
    r4 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    fn_8014E9B4();
    r0 = *(u32*)((u8*)r30 + 0x8C);
    if ((u32)r0 != (u32)0x2) goto L_801E3840;
    r3 = *(u32*)lbl_8047B474;
    r4 = r24;
    DCFlushRange();
    r3 = *(u32*)lbl_80478D04;
    r5 = r29;
    r4 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    fn_8014E9B4();
L_801E3840: ;
    r3 = 0x1;
L_801E3844: ;
    /* lmw r24, 0x30(r1) */;
    return;
}
#pragma pop

/* 0x801E3858 | size: 0x14 | tiny */
void fn_801E3858(void) { }

/* 0x801E386C | size: 0x6C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E386C(void) {
    extern u8 lbl_8046A494[];
    extern u8 lbl_8047B468[];
    extern void fn_801E4F34();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    r0 = *(u32*)lbl_8047B468;
    if ((s32)r0 == (s32)0x0) goto L_801E38C4;
    r3 = (u32)lbl_8046A494;
    r31 = (u32)lbl_8046A494;
L_801E3890: ;
    r3 = r31;
    r4 = r1 + 0x8;
    r5 = 0x0;
    ((void(*)(void))fn_8009F2F8)();
    if ((s32)r3 != (s32)0x1) goto L_801E38B0;
    r3 = *(u32*)(sp + 0x8);
    goto L_801E38B4;
L_801E38B0: ;
    r3 = 0x0;
L_801E38B4: ;
    if ((u32)r3 == (u32)0x0) goto L_801E38C4;
    fn_801E4F34();
    goto L_801E3890;
L_801E38C4: ;
    r31 = *(u32*)(sp + 0x1C);
    return;
}
#pragma pop

/* 0x801E38D8 | size: 0x10 | tiny */
void fn_801E38D8(void) { }

/* 0x801E38E8 | size: 0x48 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E38E8(void) {
    extern u8 lbl_8046AC60[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r4 = (u32)lbl_8046AC60;
    r4 = (u32)lbl_8046AC60;
    r0 = *(u32*)((u8*)r4 + 0xA0);
    if ((s32)r0 == (s32)0x0) goto L_801E391C;
    r4 = r4 + 0x8c;
    r5 = 0x10;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r3 = 0x1;
    goto L_801E3920;
L_801E391C: ;
    r3 = 0x0;
L_801E3920: ;
    return;
}
#pragma pop

/* 0x801E3930 | size: 0x48 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E3930(void) {
    extern u8 lbl_8046AC60[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r4 = (u32)lbl_8046AC60;
    r4 = (u32)lbl_8046AC60;
    r0 = *(u32*)((u8*)r4 + 0xA0);
    if ((s32)r0 == (s32)0x0) goto L_801E3964;
    r4 = r4 + 0x80;
    r5 = 0xc;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r3 = 0x1;
    goto L_801E3968;
L_801E3964: ;
    r3 = 0x0;
L_801E3968: ;
    return;
}
#pragma pop

/* 0x801E3978 | size: 0xD8 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E3978(void) {
    extern u8 lbl_8046AC60[];
    extern void fn_801E1E1C();
    extern void fn_801E1FF8();
    extern void fn_801E24B0();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r8 = (u32)lbl_8046AC60;
    /* stmw r27, 0x1c(r1) */;
    r31 = (u32)lbl_8046AC60;
    r27 = r4;
    r28 = r5;
    r29 = r6;
    r30 = r7;
    r0 = *(u32*)((u8*)r31 + 0xA0);
    if ((s32)r0 == (s32)0x0) goto L_801E3A38;
    r0 = *(u8*)((u8*)r31 + 0xA4);
    if ((u32)r0 == (u32)0x0) goto L_801E3A38;
    r0 = *(u32*)((u8*)r31 + 0xE8);
    if ((u32)r0 == (u32)0x0) goto L_801E3A38;
    fn_801E1FF8();
    r5 = *(u32*)((u8*)r31 + 0xE8);
    r0 = (s16)r30;
    r3 = (u32)lbl_8046AC60;
    r6 = (s16)r27;
    *(u32*)(sp + 0x8) = r0;
    r3 = (u32)lbl_8046AC60;
    r7 = (s16)r28;
    r10 = (s16)r29;
    r4 = *(u32*)((u8*)r3 + 0x80);
    r0 = *(u32*)((u8*)r3 + 0x84);
    r3 = *(u32*)((u8*)r5 + 0x0);
    r8 = (s16)r4;
    r4 = *(u32*)((u8*)r5 + 0x4);
    r9 = (s16)r0;
    r5 = *(u32*)((u8*)r5 + 0x8);
    fn_801E1E1C();
    fn_801E24B0();
    r4 = *(u32*)((u8*)r31 + 0xE8);
    r3 = (u32)lbl_8046AC60;
    r3 = (u32)lbl_8046AC60;
    r4 = *(u32*)((u8*)r4 + 0xC);
    r0 = *(u32*)((u8*)r3 + 0xC0);
    r3 = *(u32*)((u8*)r3 + 0x50);
    r4 = r4 + r0;
    r0 = (u32)r4 / (u32)r3;
    r0 = r0 * r3;
    r3 = r4 - r0;
    goto L_801E3A3C;
L_801E3A38: ;
    r3 = -0x1;
L_801E3A3C: ;
    /* lmw r27, 0x1c(r1) */;
    return;
}
#pragma pop

/* 0x801E3A50 | size: 0x504 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E3A50(void) {
    extern u8 lbl_8046A494[];
    extern u8 lbl_8046AC60[];
    extern u8 lbl_80478D00[];
    extern u8 lbl_80478D04[];
    extern u8 lbl_8047B46C[];
    extern u8 lbl_8047E4A8[];
    extern void fn_800AA2F0();
    extern void fn_800C4928();
    extern void fn_8014FF0C();
    extern void fn_80150564();
    extern void fn_801E4EF0();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r12 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    void (*ctr_fn)(void) = 0;

    /* stmw r29, 0x14(r1) */;
    r12 = *(u32*)lbl_8047B46C;
    if ((u32)r12 == (u32)0x0) goto L_801E3A74;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_801E3A74: ;
    r3 = (u32)lbl_8046AC60;
    r30 = -0x1;
    r31 = (u32)lbl_8046AC60;
    r0 = *(u32*)((u8*)r31 + 0xA0);
    if ((s32)r0 == (s32)0x0) goto L_801E3F40;
    r0 = *(u8*)((u8*)r31 + 0xA4);
    if ((u32)r0 != (u32)0x2) goto L_801E3F40;
    r0 = *(u32*)((u8*)r31 + 0xA8);
    if ((s32)r0 != (s32)0x0) goto L_801E3AB0;
    r0 = *(u32*)((u8*)r31 + 0xAC);
    if ((s32)r0 == (s32)0x0) goto L_801E3AC8;
L_801E3AB0: ;
    r3 = (u32)lbl_8046AC60;
    r0 = 0x5;
    r3 = (u32)lbl_8046AC60;
    *(u8*)((u8*)r31 + 0xA4) = r0;
    *(u8*)((u8*)r3 + 0xA5) = r0;
    goto L_801E3F40;
L_801E3AC8: ;
    r3 = *(u32*)((u8*)r31 + 0xCC);
    r7 = 0x1;
    r0 = *(u32*)((u8*)r31 + 0xC8);
    r6 = 0x0;
    r5 = r3 + r7;
    r4 = r0 + r6; /* +carry */;
    *(u32*)((u8*)r31 + 0xCC) = r5;
    r3 = r5 ^ r6;
    r0 = r4 ^ r6;
    *(u32*)((u8*)r31 + 0xC8) = r4;
    /* or. r0, r3, r0 */;
    if ((s32)r0 != (s32)0x0) goto L_801E3C48;
    r3 = *(u32*)((u8*)r31 + 0x88);
    r0 = r3 & 0x1;
    if ((s32)r0 == (s32)0x0) goto L_801E3B18;
    fn_800AA2F0();
    if ((u32)r3 != (u32)0x0) goto L_801E3B38;
    r7 = 0x1;
    goto L_801E3B3C;
L_801E3B18: ;
    r0 = r3 & 0x00000002;
    if ((u32)r3 == (u32)0x0) goto L_801E3B3C;
    fn_800AA2F0();
    if ((u32)r3 != (u32)0x1) goto L_801E3B38;
    r7 = 0x1;
    goto L_801E3B3C;
    goto L_801E3B3C;
L_801E3B38: ;
    r7 = 0x0;
L_801E3B3C: ;
    if ((s32)r7 == (s32)0x0) goto L_801E3C38;
    r3 = (u32)lbl_8046AC60;
    r29 = (u32)lbl_8046AC60;
    r0 = *(u8*)((u8*)r29 + 0xA7);
    if ((u32)r0 == (u32)0x0) goto L_801E3C28;
    r3 = *(u32*)((u8*)r29 + 0xE4);
    r0 = *(u32*)((u8*)r29 + 0xE0);
    r0 = r0 - r3;
    if ((s32)r0 > (s32)0x1) goto L_801E3B9C;
    r3 = 0x0;
    fn_801E4EF0();
    r5 = (u32)lbl_8046AC60;
    r4 = *(u32*)((u8*)r29 + 0xE0);
    r6 = (u32)lbl_8046AC60;
    r30 = r3;
    r5 = *(u32*)((u8*)r6 + 0xD8);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r29 + 0xE0) = r0;
    /* subi r0, r5, 0x1 */;
    *(u32*)((u8*)r6 + 0xD8) = r0;
    goto L_801E3E60;
L_801E3B9C: ;
    r3 = *(u32*)lbl_80478D00;
    r0 = r3 + (0x1 << 16);
    if ((u32)r0 == (u32)0xffff) goto L_801E3BF0;
    fn_8014FF0C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0xffff) goto L_801E3BF0;
    r3 = *(u32*)lbl_80478D04;
    r0 = r3 + (0x1 << 16);
    if ((u32)r0 == (u32)0xffff) goto L_801E3BE8;
    fn_8014FF0C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0xffff) goto L_801E3BDC;
    r0 = 0x1;
    goto L_801E3BF4;
L_801E3BDC: ;
    r3 = *(u32*)lbl_80478D00;
    fn_80150564();
    goto L_801E3BF0;
L_801E3BE8: ;
    r0 = 0x1;
    goto L_801E3BF4;
L_801E3BF0: ;
    r0 = 0x0;
L_801E3BF4: ;
    if ((s32)r0 != (s32)0x0) goto L_801E3C14;
    r3 = (u32)lbl_8046AC60;
    r0 = 0x5;
    r3 = (u32)lbl_8046AC60;
    *(u8*)((u8*)r31 + 0xA4) = r0;
    *(u8*)((u8*)r3 + 0xA5) = r0;
    goto L_801E3F40;
L_801E3C14: ;
    r3 = (u32)lbl_8046AC60;
    r0 = 0x2;
    r3 = (u32)lbl_8046AC60;
    *(u8*)((u8*)r3 + 0xA5) = r0;
    goto L_801E3E60;
L_801E3C28: ;
    r3 = 0x0;
    fn_801E4EF0();
    r30 = r3;
    goto L_801E3E60;
L_801E3C38: ;
    r0 = -0x1;
    *(u32*)((u8*)r31 + 0xCC) = r0;
    *(u32*)((u8*)r31 + 0xC8) = r0;
    goto L_801E3E60;
L_801E3C48: ;
    r0 = *(u8*)((u8*)r31 + 0xA7);
    if ((u32)r0 == (u32)0x0) goto L_801E3CE8;
    r3 = r5 ^ r7;
    r0 = r4 ^ r6;
    /* or. r0, r3, r0 */;
    if ((u32)r0 != (u32)0x0) goto L_801E3CE8;
    r0 = *(u8*)((u8*)r31 + 0xA5);
    if ((u32)r0 == (u32)0x2) goto L_801E3CE8;
    r3 = *(u32*)lbl_80478D00;
    r0 = r3 + (0x1 << 16);
    if ((u32)r0 == (u32)0xffff) goto L_801E3CC4;
    fn_8014FF0C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0xffff) goto L_801E3CC4;
    r3 = *(u32*)lbl_80478D04;
    r0 = r3 + (0x1 << 16);
    if ((u32)r0 == (u32)0xffff) goto L_801E3CBC;
    fn_8014FF0C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0xffff) goto L_801E3CB0;
    r0 = 0x1;
    goto L_801E3CC8;
L_801E3CB0: ;
    r3 = *(u32*)lbl_80478D00;
    fn_80150564();
    goto L_801E3CC4;
L_801E3CBC: ;
    r0 = 0x1;
    goto L_801E3CC8;
L_801E3CC4: ;
    r0 = 0x0;
L_801E3CC8: ;
    if ((s32)r0 != (s32)0x0) goto L_801E3CE0;
    r0 = 0x5;
    *(u8*)((u8*)r31 + 0xA5) = r0;
    *(u8*)((u8*)r31 + 0xA4) = r0;
    goto L_801E3F40;
L_801E3CE0: ;
    r0 = 0x2;
    *(u8*)((u8*)r31 + 0xA5) = r0;
L_801E3CE8: ;
    r3 = (u32)lbl_8046AC60;
    r3 = (u32)lbl_8046AC60;
    r4 = *(u32*)((u8*)r3 + 0x88);
    r0 = r4 & 0x1;
    if ((s32)r0 == (s32)0x0) goto L_801E3D10;
    fn_800AA2F0();
    if ((u32)r3 != (u32)0x0) goto L_801E3DF0;
    r0 = 0x1;
    goto L_801E3DF4;
L_801E3D10: ;
    r0 = r4 & 0x00000002;
    if ((u32)r3 == (u32)0x0) goto L_801E3D2C;
    fn_800AA2F0();
    if ((u32)r3 != (u32)0x1) goto L_801E3DF0;
    r0 = 0x1;
    goto L_801E3DF4;
L_801E3D2C: ;
    f1 = *(f32*)lbl_8047E4A8;
    f0 = *(f32*)((u8*)r3 + 0x4C);
    f0 = f1 * f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x8) = f0;
    r29 = *(u32*)(sp + 0xC);
    VIGetTvFormat();
    if ((u32)r3 != (u32)0x1) goto L_801E3D90;
    r8 = *(u32*)((u8*)r31 + 0xCC);
    r0 = (s32)r29 >> 31;
    r4 = *(u32*)((u8*)r31 + 0xC8);
    r5 = 0x0;
    r3 = (u32)((u64)r8 * (u64)r29 >> 32);
    r6 = 0x1388;
    r7 = r4 * r29;
    r0 = r8 * r0;
    r3 = r3 + r7;
    r4 = r8 * r29;
    r3 = r3 + r0;
    fn_800C4928();
    r3 = (u32)lbl_8046AC60;
    r3 = (u32)lbl_8046AC60;
    *(u32*)((u8*)r3 + 0xD4) = r4;
    goto L_801E3DCC;
L_801E3D90: ;
    r8 = *(u32*)((u8*)r31 + 0xCC);
    r0 = (s32)r29 >> 31;
    r4 = *(u32*)((u8*)r31 + 0xC8);
    r5 = 0x0;
    r3 = (u32)((u64)r8 * (u64)r29 >> 32);
    r6 = 0x176a;
    r7 = r4 * r29;
    r0 = r8 * r0;
    r3 = r3 + r7;
    r4 = r8 * r29;
    r3 = r3 + r0;
    fn_800C4928();
    r3 = (u32)lbl_8046AC60;
    r3 = (u32)lbl_8046AC60;
    *(u32*)((u8*)r3 + 0xD4) = r4;
L_801E3DCC: ;
    r3 = (u32)lbl_8046AC60;
    r3 = (u32)lbl_8046AC60;
    r0 = *(u32*)((u8*)r3 + 0xD0);
    r4 = *(u32*)((u8*)r3 + 0xD4);
    if ((s32)r0 == (s32)r4) goto L_801E3DF0;
    *(u32*)((u8*)r3 + 0xD0) = r4;
    r0 = 0x1;
    goto L_801E3DF4;
L_801E3DF0: ;
    r0 = 0x0;
L_801E3DF4: ;
    if ((s32)r0 == (s32)0x0) goto L_801E3E60;
    r0 = *(u8*)((u8*)r31 + 0xA7);
    if ((u32)r0 == (u32)0x0) goto L_801E3E54;
    r3 = (u32)lbl_8046AC60;
    r29 = (u32)lbl_8046AC60;
    r3 = *(u32*)((u8*)r29 + 0xE4);
    r0 = *(u32*)((u8*)r29 + 0xE0);
    r0 = r0 - r3;
    if ((s32)r0 > (s32)0x1) goto L_801E3E60;
    r3 = 0x0;
    fn_801E4EF0();
    r5 = (u32)lbl_8046AC60;
    r4 = *(u32*)((u8*)r29 + 0xE0);
    r6 = (u32)lbl_8046AC60;
    r30 = r3;
    r5 = *(u32*)((u8*)r6 + 0xD8);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r29 + 0xE0) = r0;
    /* subi r0, r5, 0x1 */;
    *(u32*)((u8*)r6 + 0xD8) = r0;
    goto L_801E3E60;
L_801E3E54: ;
    r3 = 0x0;
    fn_801E4EF0();
    r30 = r3;
L_801E3E60: ;
    if ((u32)r30 == (u32)0x0) goto L_801E3E9C;
    r0 = r30 + (0x1 << 16);
    if ((u32)r0 == (u32)0xffff) goto L_801E3E9C;
    r3 = (u32)lbl_8046AC60;
    r29 = (u32)lbl_8046AC60;
    r4 = *(u32*)((u8*)r29 + 0xE8);
    if ((u32)r4 == (u32)0x0) goto L_801E3E98;
    r3 = (u32)lbl_8046A494;
    r5 = 0x0;
    r3 = (u32)lbl_8046A494;
    ((void(*)(void))fn_8009F230)();
L_801E3E98: ;
    *(u32*)((u8*)r29 + 0xE8) = r30;
L_801E3E9C: ;
    r3 = (u32)lbl_8046AC60;
    r5 = (u32)lbl_8046AC60;
    r0 = *(u8*)((u8*)r5 + 0xA6);
    r0 = r0 & 0x1;
    if ((u32)r4 != (u32)0x0) goto L_801E3F40;
    r0 = *(u8*)((u8*)r5 + 0xA7);
    if ((u32)r0 == (u32)0x0) goto L_801E3EF0;
    r4 = *(u32*)((u8*)r5 + 0xE4);
    r3 = *(u32*)((u8*)r5 + 0xC0);
    r0 = *(u32*)((u8*)r5 + 0x50);
    r3 = r4 + r3;
    if ((u32)r3 != (u32)r0) goto L_801E3F40;
    r0 = *(u32*)((u8*)r5 + 0xEC);
    if ((u32)r0 != (u32)0x0) goto L_801E3F40;
    r0 = 0x3;
    *(u8*)((u8*)r5 + 0xA5) = r0;
    *(u8*)((u8*)r31 + 0xA4) = r0;
    goto L_801E3F40;
L_801E3EF0: ;
    r3 = *(u32*)((u8*)r5 + 0xE8);
    if ((u32)r3 == (u32)0x0) goto L_801E3F0C;
    r3 = *(u32*)((u8*)r3 + 0xC);
    r0 = *(u32*)((u8*)r5 + 0xC0);
    r5 = r3 + r0;
    goto L_801E3F14;
L_801E3F0C: ;
    r3 = *(u32*)((u8*)r5 + 0xC0);
    /* subi r5, r3, 0x1 */;
L_801E3F14: ;
    r3 = (u32)lbl_8046AC60;
    r4 = (u32)lbl_8046AC60;
    r3 = *(u32*)((u8*)r4 + 0x50);
    /* subi r0, r3, 0x1 */;
    if ((u32)r5 != (u32)r0) goto L_801E3F40;
    if ((u32)r30 != (u32)0x0) goto L_801E3F40;
    r0 = 0x3;
    *(u8*)((u8*)r4 + 0xA5) = r0;
    *(u8*)((u8*)r31 + 0xA4) = r0;
L_801E3F40: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801E3F54 | size: 0x104 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E3F54(void) {
    extern u8 lbl_8046A494[];
    extern u8 lbl_8046AC60[];
    extern u8 lbl_80478D00[];
    extern u8 lbl_80478D04[];
    extern u8 lbl_8047B46C[];
    extern void fn_800A7AFC();
    extern void fn_800A8850();
    extern void fn_8014F838();
    extern void fn_801E1D0C();
    extern void fn_801E4DAC();
    extern void fn_801E5400();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    r3 = (u32)lbl_8046AC60;
    r4 = (u32)lbl_8046AC60;
    r0 = *(u32*)((u8*)r4 + 0xA0);
    if ((s32)r0 == (s32)0x0) goto L_801E4044;
    r0 = *(u8*)((u8*)r4 + 0xA4);
    if ((u32)r0 == (u32)0x0) goto L_801E4044;
    r0 = 0x0;
    r3 = *(u32*)lbl_8047B46C;
    *(u8*)((u8*)r4 + 0xA5) = r0;
    *(u8*)((u8*)r4 + 0xA4) = r0;
    fn_800A8850();
    r3 = (u32)lbl_8046AC60;
    r3 = (u32)lbl_8046AC60;
    r0 = *(u32*)((u8*)r3 + 0xB0);
    if ((s32)r0 != (s32)0x0) goto L_801E3FB4;
    fn_800A7AFC();
    fn_801E1D0C();
L_801E3FB4: ;
    fn_801E5400();
    r3 = (u32)lbl_8046AC60;
    r3 = (u32)lbl_8046AC60;
    r0 = *(u8*)((u8*)r3 + 0xA7);
    if ((u32)r0 == (u32)0x0) goto L_801E3FFC;
    r3 = *(u32*)lbl_80478D00;
    fn_8014F838();
    r3 = *(u32*)lbl_80478D04;
    r0 = -0x1;
    *(u32*)lbl_80478D00 = r0;
    r0 = r3 + (0x1 << 16);
    if ((u32)r0 == (u32)0xffff) goto L_801E3FF8;
    fn_8014F838();
    r0 = -0x1;
    *(u32*)lbl_80478D04 = r0;
L_801E3FF8: ;
    fn_801E4DAC();
L_801E3FFC: ;
    r3 = (u32)lbl_8046A494;
    r31 = (u32)lbl_8046A494;
L_801E4004: ;
    r3 = r31;
    r4 = r1 + 0x8;
    r5 = 0x0;
    ((void(*)(void))fn_8009F2F8)();
    if ((s32)r3 != (s32)0x1) goto L_801E4024;
    r0 = *(u32*)(sp + 0x8);
    goto L_801E4028;
L_801E4024: ;
    r0 = 0x0;
L_801E4028: ;
    if ((u32)r0 != (u32)0x0) goto L_801E4004;
    r3 = (u32)lbl_8046AC60;
    r0 = 0x0;
    r3 = (u32)lbl_8046AC60;
    *(u32*)((u8*)r3 + 0xA8) = r0;
    *(u32*)((u8*)r3 + 0xAC) = r0;
L_801E4044: ;
    r31 = *(u32*)(sp + 0x1C);
    return;
}
#pragma pop

/* 0x801E4058 | size: 0xA0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E4058(void) {
    extern u8 lbl_8046AC60[];
    extern void fn_801E2CA8();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r3 = (u32)lbl_8046AC60;
    r31 = (u32)lbl_8046AC60;
    r0 = *(u32*)((u8*)r31 + 0xA0);
    if ((s32)r0 == (s32)0x0) goto L_801E40E0;
    r0 = *(u8*)((u8*)r31 + 0xA4);
    if ((u32)r0 == (u32)0x1) goto L_801E4090;
    if ((u32)r0 != (u32)0x4) goto L_801E40E0;
L_801E4090: ;
    if ((u32)r0 != (u32)0x4) goto L_801E40B0;
    r3 = (u32)lbl_8046AC60;
    r3 = (u32)lbl_8046AC60;
    r0 = *(u8*)((u8*)r3 + 0xA7);
    if ((u32)r0 == (u32)0x0) goto L_801E40B0;
    fn_801E2CA8();
L_801E40B0: ;
    r3 = (u32)lbl_8046AC60;
    r6 = 0x2;
    r4 = (u32)lbl_8046AC60;
    r5 = 0x0;
    r0 = -0x1;
    *(u8*)((u8*)r31 + 0xA4) = r6;
    r3 = 0x1;
    *(u32*)((u8*)r4 + 0xD0) = r5;
    *(u32*)((u8*)r4 + 0xD4) = r5;
    *(u32*)((u8*)r4 + 0xCC) = r0;
    *(u32*)((u8*)r4 + 0xC8) = r0;
    goto L_801E40E4;
L_801E40E0: ;
    r3 = 0x0;
L_801E40E4: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801E40F8 | size: 0x374 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E40F8(void) {
    extern u8 lbl_8046A440[];
    extern u8 lbl_8047B46C[];
    extern u8 lbl_8047B478[];
    extern void fn_8009F1D0();
    extern void fn_800A541C();
    extern void fn_800A8850();
    extern void fn_801E1BB8();
    extern void fn_801E1D48();
    extern void fn_801E1D7C();
    extern void fn_801E34F0();
    extern void fn_801E4B08();
    extern void fn_801E4DE8();
    extern void fn_801E4E1C();
    extern void fn_801E4F34();
    extern void fn_801E543C();
    extern void fn_801E5470();
    extern void fn_801E3A50();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;

    r6 = (u32)lbl_8046A440;
    /* stmw r25, 0x14(r1) */;
    r29 = (u32)lbl_8046A440;
    r30 = r29 + 0x820;
    r28 = r3;
    r27 = r4;
    r31 = r5;
    r0 = *(u32*)((u8*)r30 + 0xA0);
    if ((s32)r0 == (s32)0x0) goto L_801E4454;
    r0 = *(u8*)((u8*)r30 + 0xA4);
    if ((u32)r0 != (u32)0x0) goto L_801E4454;
    if ((s32)r28 <= (s32)0x0) goto L_801E41C0;
    r5 = *(u32*)((u8*)r30 + 0x60);
    if ((u32)r5 != (u32)0x0) goto L_801E4154;
    r3 = 0x0;
    goto L_801E4458;
L_801E4154: ;
    r0 = *(u32*)((u8*)r30 + 0x50);
    if ((u32)r0 <= (u32)r28) goto L_801E41B8;
    /* subi r0, r28, 0x1 */;
    r3 = r30;
    r0 = r0 << 2;
    r4 = r29 + 0xa0;
    r6 = r5 + r0;
    r5 = 0x20;
    r7 = 0x2;
    fn_800A541C();
    if ((s32)r3 >= (s32)0x0) goto L_801E4190;
    r3 = 0x0;
    goto L_801E4458;
L_801E4190: ;
    r3 = r29 + 0xa0;
    r4 = *(u32*)((u8*)r30 + 0x64);
    r6 = *(s16*)((u8*)r29 + 0xA0);
    r0 = *(s16*)((u8*)r3 + 0x2);
    r3 = r4 + r6;
    *(u32*)((u8*)r30 + 0xC0) = r28;
    r0 = r0 - r6;
    *(u32*)((u8*)r30 + 0xB8) = r3;
    *(u32*)((u8*)r30 + 0xBC) = r0;
    goto L_801E41D4;
L_801E41B8: ;
    r3 = 0x0;
    goto L_801E4458;
L_801E41C0: ;
    r3 = *(u32*)((u8*)r30 + 0x64);
    r0 = *(u32*)((u8*)r30 + 0x54);
    *(u32*)((u8*)r30 + 0xB8) = r3;
    *(u32*)((u8*)r30 + 0xBC) = r0;
    *(u32*)((u8*)r30 + 0xC0) = r28;
L_801E41D4: ;
    r28 = r29 + 0x820;
    r0 = *(u8*)((u8*)r28 + 0xA7);
    if ((u32)r0 == (u32)0x0) goto L_801E4204;
    if ((s32)r31 < (s32)0x0) goto L_801E41F8;
    r0 = *(u32*)((u8*)r28 + 0x98);
    if ((u32)r31 < (u32)r0) goto L_801E4200;
L_801E41F8: ;
    r3 = 0x0;
    goto L_801E4458;
L_801E4200: ;
    *(u32*)((u8*)r28 + 0xDC) = r31;
L_801E4204: ;
    r31 = r29 + 0x820;
    r4 = r27 & 0x1;
    r0 = *(u32*)((u8*)r31 + 0xB0);
    r3 = 0x0;
    *(u8*)((u8*)r31 + 0xA6) = r4;
    *(u32*)((u8*)r31 + 0xD8) = r3;
    if ((s32)r0 == (s32)0x0) goto L_801E4288;
    r4 = *(u32*)((u8*)r31 + 0xB4);
    r3 = r31;
    r5 = *(u32*)((u8*)r31 + 0x58);
    r7 = 0x2;
    r6 = *(u32*)((u8*)r31 + 0x64);
    fn_800A541C();
    if ((s32)r3 >= (s32)0x0) goto L_801E424C;
    r3 = 0x0;
    goto L_801E4458;
L_801E424C: ;
    r4 = *(u32*)((u8*)r31 + 0xB4);
    r3 = 0x14;
    r0 = *(u32*)((u8*)r31 + 0xB8);
    r5 = *(u32*)((u8*)r31 + 0x64);
    r0 = r4 + r0;
    r25 = r0 - r5;
    r4 = r25;
    fn_801E5470();
    r0 = *(u8*)((u8*)r28 + 0xA7);
    if ((u32)r0 == (u32)0x0) goto L_801E42B4;
    r4 = r25;
    r3 = 0xc;
    fn_801E4E1C();
    goto L_801E42B4;
L_801E4288: ;
    r3 = 0x14;
    r4 = 0x0;
    fn_801E5470();
    r0 = *(u8*)((u8*)r28 + 0xA7);
    if ((u32)r0 == (u32)0x0) goto L_801E42AC;
    r3 = 0xc;
    r4 = 0x0;
    fn_801E4E1C();
L_801E42AC: ;
    r3 = 0x8;
    fn_801E1D7C();
L_801E42B4: ;
    r0 = *(u32*)((u8*)r31 + 0xB0);
    if ((s32)r0 != (s32)0x0) goto L_801E42E8;
    r25 = 0x0;
    r26 = r29 + 0x820;
    r27 = r25;
L_801E42CC: ;
    r3 = r27 + 0xf0;
    r3 = r26 + r3;
    fn_801E1BB8();
    r25 = r25 + 0x1;
    r27 = r27 + 0xc;
    if ((s32)r25 < (s32)0xa) goto L_801E42CC;
L_801E42E8: ;
    r26 = r29 + 0x820;
    r25 = 0x0;
    r27 = 0x0;
L_801E42F4: ;
    r3 = r27 + 0x168;
    r3 = r26 + r3;
    fn_801E4F34();
    r25 = r25 + 0x1;
    r27 = r27 + 0x10;
    if ((s32)r25 < (s32)0x3) goto L_801E42F4;
    r0 = *(u8*)((u8*)r28 + 0xA7);
    if ((u32)r0 == (u32)0x0) goto L_801E4344;
    r26 = r29 + 0x820;
    r25 = 0x0;
    r27 = 0x0;
L_801E4328: ;
    r3 = r27 + 0x198;
    r3 = r26 + r3;
    fn_801E4B08();
    r25 = r25 + 0x1;
    r27 = r27 + 0xc;
    if ((s32)r25 < (s32)0x3) goto L_801E4328;
L_801E4344: ;
    r3 = r29 + 0x74;
    r4 = (u32)lbl_8047B478;
    r5 = 0x2;
    fn_8009F1D0();
    fn_801E543C();
    r0 = *(u8*)((u8*)r28 + 0xA7);
    if ((u32)r0 == (u32)0x0) goto L_801E4368;
    fn_801E4DE8();
L_801E4368: ;
    r0 = *(u32*)((u8*)r31 + 0xB0);
    if ((s32)r0 != (s32)0x0) goto L_801E4378;
    fn_801E1D48();
L_801E4378: ;
    r0 = *(u8*)((u8*)r28 + 0xA7);
    if ((u32)r0 == (u32)0x0) goto L_801E43CC;
    r3 = r29 + 0x74;
    r4 = r1 + 0x8;
    r5 = 0x1;
    ((void(*)(void))fn_8009F2F8)();
    r3 = r29 + 0x74;
    r4 = r1 + 0xc;
    r5 = 0x1;
    ((void(*)(void))fn_8009F2F8)();
    r0 = *(u32*)(sp + 0x8);
    if ((s32)r0 == (s32)0x0) goto L_801E43C4;
    r0 = *(u32*)(sp + 0xC);
    if ((s32)r0 == (s32)0x0) goto L_801E43C4;
    r0 = 0x1;
    goto L_801E43F4;
L_801E43C4: ;
    r0 = 0x0;
    goto L_801E43F4;
L_801E43CC: ;
    r3 = r29 + 0x74;
    r4 = r1 + 0x8;
    r5 = 0x1;
    ((void(*)(void))fn_8009F2F8)();
    r0 = *(u32*)(sp + 0x8);
    if ((s32)r0 == (s32)0x0) goto L_801E43F0;
    r0 = 0x1;
    goto L_801E43F4;
L_801E43F0: ;
    r0 = 0x0;
L_801E43F4: ;
    if ((s32)r0 != (s32)0x0) goto L_801E4404;
    r3 = 0x0;
    goto L_801E4458;
L_801E4404: ;
    r0 = *(u8*)((u8*)r28 + 0xA7);
    r6 = 0x1;
    r4 = r29 + 0x820;
    r5 = 0x0;
    r3 = -0x1;
    *(u8*)((u8*)r30 + 0xA4) = r6;
    *(u8*)((u8*)r4 + 0xA5) = r5;
    *(u32*)((u8*)r4 + 0xE8) = r5;
    *(u32*)((u8*)r4 + 0xEC) = r5;
    *(u32*)((u8*)r4 + 0xE0) = r3;
    *(u32*)((u8*)r4 + 0xE4) = r5;
    if ((u32)r0 == (u32)0x0) goto L_801E443C;
    fn_801E34F0();
L_801E443C: ;
    r3 = (u32)fn_801E3A50;
    r3 = (u32)fn_801E3A50;
    fn_800A8850();
    *(u32*)lbl_8047B46C = r3;
    r3 = 0x1;
    goto L_801E4458;
L_801E4454: ;
    r3 = 0x0;
L_801E4458: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801E446C | size: 0x30 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E446C(void) {
    extern u8 lbl_8046A4B4[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r5 = (u32)lbl_8046A4B4;
    r4 = r3;
    r3 = (u32)lbl_8046A4B4;
    r5 = 0x1;
    ((void(*)(void))fn_8009F230)();
    return;
}
#pragma pop

/* 0x801E449C | size: 0x1B4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E449C(void) {
    extern u8 lbl_8046AC60[];
    extern u8 lbl_8047B470[];
    extern u8 lbl_8047B474[];
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f4 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r4 = (u32)lbl_8046AC60;
    r5 = (u32)lbl_8046AC60;
    /* stmw r25, 0x14(r1) */;
    r0 = *(u32*)((u8*)r5 + 0xA0);
    if ((s32)r0 == (s32)0x0) goto L_801E4638;
    r0 = *(u8*)((u8*)r5 + 0xA4);
    if ((u32)r0 != (u32)0x0) goto L_801E4638;
    r0 = *(u32*)((u8*)r5 + 0xB0);
    r31 = r3;
    if ((s32)r0 == (s32)0x0) goto L_801E44EC;
    r0 = *(u32*)((u8*)r5 + 0x58);
    *(u32*)((u8*)r5 + 0xB4) = r3;
    r31 = r31 + r0;
    goto L_801E4518;
L_801E44EC: ;
    r0 = 0xa;
    r3 = 0x0;
    ctr_fn = (void(*)(void))r0;
L_801E44F8: ;
    r4 = r5 + r3;
    r3 = r3 + 0xc;
    *(u32*)((u8*)r4 + 0xF0) = r31;
    r4 = *(u32*)((u8*)r5 + 0x44);
    r0 = r4 + 0x1f;
    /* clrrwi r0, r0, 5 */;
    r31 = r31 + r0;
    if (--ctr != 0) goto L_801E44F8;
L_801E4518: ;
    r3 = (u32)lbl_8046AC60;
    r27 = 0x0;
    r29 = (u32)lbl_8046AC60;
    r30 = 0x0;
    r3 = *(u32*)((u8*)r29 + 0x80);
    r0 = *(u32*)((u8*)r29 + 0x84);
    r4 = r3 * r0;
    r3 = (u32)r4 >> 2;
    r4 = r4 + 0x1f;
    r0 = r3 + 0x1f;
    /* clrrwi r26, r4, 5 */;
    /* clrrwi r25, r0, 5 */;
L_801E4548: ;
    r28 = r29 + r30;
    r3 = r31;
    *(u32*)((u8*)r28 + 0x168) = r31;
    r4 = r26;
    DCInvalidateRange();
    r31 = r31 + r26;
    r4 = r25;
    *(u32*)((u8*)r28 + 0x16C) = r31;
    r3 = r31;
    DCInvalidateRange();
    r31 = r31 + r25;
    r4 = r25;
    *(u32*)((u8*)r28 + 0x170) = r31;
    r3 = r31;
    DCInvalidateRange();
    r27 = r27 + 0x1;
    r31 = r31 + r25;
    r30 = r30 + 0x10;
    if ((u32)r27 < (u32)0x3) goto L_801E4548;
    r3 = (u32)lbl_8046AC60;
    r5 = (u32)lbl_8046AC60;
    r0 = *(u8*)((u8*)r5 + 0xA7);
    if ((u32)r0 == (u32)0x0) goto L_801E4624;
    r0 = 0x3;
    r6 = 0x0;
    r3 = 0x0;
    ctr_fn = (void(*)(void))r0;
L_801E45BC: ;
    r4 = r5 + r3;
    r3 = r3 + 0xc;
    *(u32*)((u8*)r4 + 0x198) = r31;
    *(u32*)((u8*)r4 + 0x19C) = r31;
    *(u32*)((u8*)r4 + 0x1A0) = r6;
    r0 = *(u32*)((u8*)r5 + 0x48);
    r4 = r0 << 2;
    r0 = r4 + 0x1f;
    /* clrrwi r0, r0, 5 */;
    r31 = r31 + r0;
    if (--ctr != 0) goto L_801E45BC;
    r4 = (u32)lbl_8046AC60;
    r3 = 0x1f4;
    r5 = (u32)lbl_8046AC60;
    *(u32*)lbl_8047B470 = r31;
    r4 = *(u32*)((u8*)r5 + 0x90);
    r0 = *(u32*)((u8*)r5 + 0x8C);
    r4 = r4 * 0x28;
    r3 = (u32)r4 / (u32)r3;
    r0 = r3 + 0x1f;
    /* clrrwi r0, r0, 5 */;
    r31 = r31 + r0;
    if ((u32)r0 != (u32)0x2) goto L_801E4624;
    *(u32*)lbl_8047B474 = r31;
    r31 = r31 + r0;
L_801E4624: ;
    r4 = (u32)lbl_8046AC60;
    r3 = 0x1;
    r4 = (u32)lbl_8046AC60;
    *(u32*)((u8*)r4 + 0x9C) = r31;
    goto L_801E463C;
L_801E4638: ;
    r3 = 0x0;
L_801E463C: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801E4650 | size: 0xD4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E4650(void) {
    extern u8 lbl_8046AC60[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    f32 f4 = 0.0f;

    r3 = (u32)lbl_8046AC60;
    r3 = (u32)lbl_8046AC60;
    r0 = *(u32*)((u8*)r3 + 0xA0);
    if ((s32)r0 == (s32)0x0) goto L_801E471C;
    r0 = *(u32*)((u8*)r3 + 0xB0);
    if ((s32)r0 == (s32)0x0) goto L_801E4680;
    r3 = *(u32*)((u8*)r3 + 0x58);
    r0 = r3 + 0x1f;
    /* clrrwi r7, r0, 5 */;
    goto L_801E4690;
L_801E4680: ;
    r3 = *(u32*)((u8*)r3 + 0x44);
    r0 = r3 + 0x1f;
    /* clrrwi r0, r0, 5 */;
    r7 = r0 * 0xa;
L_801E4690: ;
    r3 = (u32)lbl_8046AC60;
    r6 = (u32)lbl_8046AC60;
    r4 = *(u32*)((u8*)r6 + 0x80);
    r3 = *(u32*)((u8*)r6 + 0x84);
    r0 = *(u8*)((u8*)r6 + 0xA7);
    r3 = r4 * r3;
    r0 = r3 + 0x1f;
    r3 = (u32)r3 >> 2;
    /* clrrwi r4, r0, 5 */;
    r0 = r3 + 0x1f;
    r3 = r4 * 0x3;
    /* clrrwi r0, r0, 5 */;
    r0 = r0 * 0x3;
    r7 = r7 + r3;
    r7 = r7 + r0;
    r7 = r7 + r0;
    if ((u32)r0 == (u32)0x0) goto L_801E4714;
    r3 = *(u32*)((u8*)r6 + 0x90);
    r0 = 0x1f4;
    r5 = *(u32*)((u8*)r6 + 0x48);
    r3 = r3 * 0x28;
    r4 = *(u32*)((u8*)r6 + 0x8C);
    r5 = r5 << 2;
    r3 = (u32)r3 / (u32)r0;
    r0 = r5 + 0x1f;
    /* clrrwi r5, r0, 5 */;
    r0 = r3 + 0x1f;
    /* clrrwi r0, r0, 5 */;
    r0 = r4 * r0;
    r3 = r5 * 0x3;
    r7 = r7 + r3;
    r7 = r7 + r0;
L_801E4714: ;
    r3 = r7 + 0x1000;
    return;
L_801E471C: ;
    r3 = 0x0;
    return;
}
#pragma pop

/* 0x801E4724 | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E4724(void) {
    extern u8 lbl_8046AC60[];
    extern void fn_800A50E4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;

    r3 = (u32)lbl_8046AC60;
    r3 = (u32)lbl_8046AC60;
    r0 = *(u32*)((u8*)r3 + 0xA0);
    if ((s32)r0 == (s32)0x0) goto L_801E4764;
    r0 = *(u8*)((u8*)r3 + 0xA4);
    if ((u32)r0 != (u32)0x0) goto L_801E4764;
    r0 = 0x0;
    *(u32*)((u8*)r3 + 0xA0) = r0;
    fn_800A50E4();
    r3 = 0x1;
    goto L_801E4768;
L_801E4764: ;
    r3 = 0x0;
L_801E4768: ;
    return;
}
#pragma pop

/* 0x801E4778 | size: 0x2F4 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E4778(void) {
    extern u8 lbl_8046A4E0[];
    extern u8 lbl_8046AC60[];
    extern u8 lbl_8047B468[];
    extern u8 lbl_8047E4AC[];
    extern void fn_800A501C();
    extern void fn_800A50E4();
    extern void fn_800A541C();
    extern void fn_800CA7FC();
    extern void fn_801ECA10();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r24, 0x10(r1) */;
    r24 = r3;
    r31 = r4;
    fn_801ECA10();
    if ((s32)r3 != (s32)0x0) goto L_801E47A4;
    r3 = 0x0;
    goto L_801E4A58;
L_801E47A4: ;
    r0 = *(u32*)lbl_8047B468;
    if ((s32)r0 != (s32)0x0) goto L_801E47B8;
    r3 = 0x0;
    goto L_801E4A58;
L_801E47B8: ;
    r3 = (u32)lbl_8046AC60;
    r30 = (u32)lbl_8046AC60;
    r0 = *(u32*)((u8*)r30 + 0xA0);
    if ((s32)r0 == (s32)0x0) goto L_801E47D4;
    r3 = 0x0;
    goto L_801E4A58;
L_801E47D4: ;
    r29 = r30 + 0x80;
    r4 = 0x0;
    r3 = r29;
    r5 = 0xc;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = (u32)lbl_8046AC60;
    r4 = 0x0;
    r3 = (u32)lbl_8046AC60;
    r5 = 0x10;
    r28 = r3 + 0x8c;
    r3 = r28;
    memset((void*)r3, (int)r4, (u32)r5);
    r4 = (u32)lbl_8046AC60;
    r3 = r24;
    r4 = (u32)lbl_8046AC60;
    fn_800A501C();
    if ((s32)r3 != (s32)0x0) goto L_801E4824;
    r3 = 0x0;
    goto L_801E4A58;
L_801E4824: ;
    r3 = (u32)lbl_8046AC60;
    r4 = (u32)lbl_8046A4E0;
    r3 = (u32)lbl_8046AC60;
    r5 = 0x40;
    r4 = (u32)lbl_8046A4E0;
    r6 = 0x0;
    r7 = 0x2;
    fn_800A541C();
    if ((s32)r3 >= (s32)0x0) goto L_801E4860;
    r3 = (u32)lbl_8046AC60;
    r3 = (u32)lbl_8046AC60;
    fn_800A50E4();
    r3 = 0x0;
    goto L_801E4A58;
L_801E4860: ;
    r4 = (u32)lbl_8046AC60;
    r3 = (u32)lbl_8046A4E0;
    r4 = (u32)lbl_8046AC60;
    r5 = 0x30;
    r24 = r4 + 0x3c;
    r4 = (u32)lbl_8046A4E0;
    r3 = r24;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r3 = r24;
    r4 = (u32)lbl_8047E4AC;
    fn_800CA7FC();
    if ((s32)r3 == (s32)0x0) goto L_801E48A8;
    r3 = (u32)lbl_8046AC60;
    r3 = (u32)lbl_8046AC60;
    fn_800A50E4();
    r3 = 0x0;
    goto L_801E4A58;
L_801E48A8: ;
    r3 = (u32)lbl_8046AC60;
    r3 = (u32)lbl_8046AC60;
    r4 = *(u32*)((u8*)r3 + 0x40);
    /* subis r0, r4, 0x1 */;
    if ((u32)r0 == (u32)0x1000) goto L_801E48CC;
    fn_800A50E4();
    r3 = 0x0;
    goto L_801E4A58;
L_801E48CC: ;
    r24 = *(u32*)((u8*)r3 + 0x5C);
    r4 = (u32)lbl_8046A4E0;
    r4 = (u32)lbl_8046A4E0;
    r5 = 0x20;
    r6 = r24;
    r7 = 0x2;
    fn_800A541C();
    if ((s32)r3 >= (s32)0x0) goto L_801E4904;
    r3 = (u32)lbl_8046AC60;
    r3 = (u32)lbl_8046AC60;
    fn_800A50E4();
    r3 = 0x0;
    goto L_801E4A58;
L_801E4904: ;
    r4 = (u32)lbl_8046AC60;
    r3 = (u32)lbl_8046A4E0;
    r4 = (u32)lbl_8046AC60;
    r5 = 0x14;
    r26 = r4 + 0x6c;
    r4 = (u32)lbl_8046A4E0;
    r3 = r26;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r3 = (u32)lbl_8046AC60;
    r25 = r24 + 0x14;
    r0 = 0x0;
    r24 = 0x0;
    r27 = (u32)lbl_8046AC60;
    *(u8*)((u8*)r27 + 0xA7) = r0;
    goto L_801E4A24;
L_801E4940: ;
    r3 = r27 + r24;
    r0 = *(u8*)((u8*)r3 + 0x70);
    if ((s32)r0 == (s32)0x1) goto L_801E49B8;
    if ((s32)r0 >= (s32)0x1) goto L_801E4A18;
    if ((s32)r0 >= (s32)0x0) goto L_801E4960;
    goto L_801E4A18;
L_801E4960: ;
    r3 = (u32)lbl_8046AC60;
    r4 = (u32)lbl_8046A4E0;
    r3 = (u32)lbl_8046AC60;
    r6 = r25;
    r4 = (u32)lbl_8046A4E0;
    r5 = 0x20;
    r7 = 0x2;
    fn_800A541C();
    if ((s32)r3 >= (s32)0x0) goto L_801E499C;
    r3 = (u32)lbl_8046AC60;
    r3 = (u32)lbl_8046AC60;
    fn_800A50E4();
    r3 = 0x0;
    goto L_801E4A58;
L_801E499C: ;
    r4 = (u32)lbl_8046A4E0;
    r3 = r29;
    r4 = (u32)lbl_8046A4E0;
    r5 = 0xc;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r25 = r25 + 0xc;
    goto L_801E4A20;
L_801E49B8: ;
    r3 = (u32)lbl_8046AC60;
    r4 = (u32)lbl_8046A4E0;
    r3 = (u32)lbl_8046AC60;
    r6 = r25;
    r4 = (u32)lbl_8046A4E0;
    r5 = 0x20;
    r7 = 0x2;
    fn_800A541C();
    if ((s32)r3 >= (s32)0x0) goto L_801E49F4;
    r3 = (u32)lbl_8046AC60;
    r3 = (u32)lbl_8046AC60;
    fn_800A50E4();
    r3 = 0x0;
    goto L_801E4A58;
L_801E49F4: ;
    r4 = (u32)lbl_8046A4E0;
    r3 = r28;
    r4 = (u32)lbl_8046A4E0;
    r5 = 0x10;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r0 = 0x1;
    r25 = r25 + 0x10;
    *(u8*)((u8*)r27 + 0xA7) = r0;
    goto L_801E4A20;
L_801E4A18: ;
    r3 = 0x0;
    goto L_801E4A58;
L_801E4A20: ;
    r24 = r24 + 0x1;
L_801E4A24: ;
    r0 = *(u32*)((u8*)r26 + 0x0);
    if ((u32)r24 < (u32)r0) goto L_801E4940;
    r3 = (u32)lbl_8046AC60;
    r0 = 0x1;
    r4 = (u32)lbl_8046AC60;
    r5 = 0x0;
    *(u8*)((u8*)r4 + 0xA5) = r5;
    r3 = 0x1;
    *(u8*)((u8*)r4 + 0xA4) = r5;
    *(u8*)((u8*)r4 + 0xA6) = r5;
    *(u32*)((u8*)r4 + 0xB0) = r31;
    *(u32*)((u8*)r30 + 0xA0) = r0;
L_801E4A58: ;
    /* lmw r24, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x801E4A6C | size: 0x58 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E4A6C(void) {
    extern u8 lbl_8046A440[];
    extern u8 lbl_8047B468[];
    extern void fn_8009F1D0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    r3 = (u32)lbl_8046A440;
    r4 = 0x0;
    r5 = 0x1c0;
    r31 = (u32)lbl_8046A440;
    r3 = r31 + 0x820;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = r31 + 0x54;
    r4 = r31 + 0x48;
    r5 = 0x3;
    fn_8009F1D0();
    r0 = 0x1;
    r3 = 0x1;
    *(u32*)lbl_8047B468 = r0;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801E4AC4 | size: 0x44 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E4AC4(void) {
    extern u8 lbl_8046AE38[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r4 = (u32)lbl_8046AE38;
    r5 = r3;
    r3 = (u32)lbl_8046AE38;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8009F2F8)();
    if ((s32)r3 != (s32)0x1) goto L_801E4AF4;
    r3 = *(u32*)(sp + 0x8);
    goto L_801E4AF8;
L_801E4AF4: ;
    r3 = 0x0;
L_801E4AF8: ;
    return;
}
#pragma pop

/* 0x801E4B08 | size: 0x30 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E4B08(void) {
    extern u8 lbl_8046AE58[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r5 = (u32)lbl_8046AE58;
    r4 = r3;
    r3 = (u32)lbl_8046AE58;
    r5 = 0x0;
    ((void(*)(void))fn_8009F230)();
    return;
}
#pragma pop

/* 0x801E4B38 | size: 0x148 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E4B38(void) {
    extern u8 lbl_8046AC60[];
    extern u8 lbl_8046AE20[];
    extern void fn_800A221C();
    extern void fn_801E446C();
    extern void fn_801ECAB0();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r4 = (u32)lbl_8046AC60;
    /* stmw r24, 0x10(r1) */;
    r31 = (u32)lbl_8046AC60;
    r4 = (u32)lbl_8046AE20;
    r29 = r3;
    r30 = (u32)lbl_8046AE20;
    r27 = 0x0;
    r28 = *(u32*)((u8*)r31 + 0xBC);
L_801E4B64: ;
    r0 = *(u32*)((u8*)r31 + 0x6C);
    r25 = r29 + 0x8;
    r3 = r30 + 0x38;
    r4 = r1 + 0x8;
    r6 = r0 << 2;
    r5 = 0x1;
    r26 = r6 + 0x8;
    r26 = r29 + r26;
    ((void(*)(void))fn_8009F2F8)();
    r0 = *(u32*)((u8*)r31 + 0x6C);
    r4 = 0x0;
    r24 = *(u32*)(sp + 0x8);
    ctr_fn = (void(*)(void))r0;
    if ((u32)r0 <= (u32)0x0) goto L_801E4C04;
L_801E4BA0: ;
    r3 = r31 + r4;
    r0 = *(u8*)((u8*)r3 + 0x70);
    if ((s32)r0 == (s32)0x1) goto L_801E4BB4;
    goto L_801E4BF0;
L_801E4BB4: ;
    r4 = *(u32*)((u8*)r25 + 0x0);
    r5 = 0x0;
    r0 = *(u32*)((u8*)r31 + 0xDC);
    r3 = *(u32*)((u8*)r24 + 0x0);
    r0 = r4 * r0;
    r4 = r26 + r0;
    fn_801ECAB0();
    *(u32*)((u8*)r24 + 0x8) = r3;
    r4 = r24;
    r3 = r30 + 0x18;
    r5 = 0x1;
    r0 = *(u32*)((u8*)r24 + 0x0);
    *(u32*)((u8*)r24 + 0x4) = r0;
    ((void(*)(void))fn_8009F230)();
    goto L_801E4C04;
L_801E4BF0: ;
    r0 = *(u32*)((u8*)r25 + 0x0);
    r25 = r25 + 0x4;
    r4 = r4 + 0x1;
    r26 = r26 + r0;
    if (--ctr != 0) goto L_801E4BA0;
L_801E4C04: ;
    r0 = *(u32*)((u8*)r31 + 0xC0);
    r5 = *(u32*)((u8*)r31 + 0x50);
    r4 = r27 + r0;
    r3 = (u32)r4 / (u32)r5;
    /* subi r0, r5, 0x1 */;
    r3 = r3 * r5;
    r3 = r4 - r3;
    if ((u32)r3 != (u32)r0) goto L_801E4C5C;
    r0 = *(u8*)((u8*)r31 + 0xA6);
    r0 = r0 & 0x1;
    if ((u32)r3 == (u32)r0) goto L_801E4C40;
    r28 = *(u32*)((u8*)r29 + 0x0);
    r29 = *(u32*)((u8*)r31 + 0xB4);
    goto L_801E4C68;
L_801E4C40: ;
    if ((s32)r27 >= (s32)0x2) goto L_801E4C50;
    r3 = 0x1;
    fn_801E446C();
L_801E4C50: ;
    r3 = r30 + 0x1058;
    fn_800A221C();
    goto L_801E4C68;
L_801E4C5C: ;
    r0 = *(u32*)((u8*)r29 + 0x0);
    r29 = r29 + r28;
    r28 = r0;
L_801E4C68: ;
    if ((s32)r27 != (s32)0x2) goto L_801E4C78;
    r3 = 0x1;
    fn_801E446C();
L_801E4C78: ;
    r27 = r27 + 0x1;
    goto L_801E4B64;
}
#pragma pop

/* 0x801E4C80 | size: 0x12C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E4C80(void) {
    extern u8 lbl_8046AC60[];
    extern u8 lbl_8046AE38[];
    extern u8 lbl_8046AE58[];
    extern void fn_801E1B54();
    extern void fn_801E1BE8();
    extern void fn_801E446C();
    extern void fn_801ECAB0();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r4 = (u32)lbl_8046AC60;
    r3 = (u32)lbl_8046AE58;
    /* stmw r25, 0x14(r1) */;
    r30 = (u32)lbl_8046AC60;
    r28 = 0x0;
    r31 = (u32)lbl_8046AE58;
L_801E4CA4: ;
    fn_801E1BE8();
    r0 = *(u32*)((u8*)r30 + 0x6C);
    r29 = r3;
    r6 = *(u32*)((u8*)r29 + 0x0);
    r3 = r31;
    r4 = r0 << 2;
    r5 = 0x1;
    r27 = r4 + 0x8;
    r26 = r6 + 0x8;
    r4 = r1 + 0x8;
    r27 = r6 + r27;
    ((void(*)(void))fn_8009F2F8)();
    r0 = *(u32*)((u8*)r30 + 0x6C);
    r4 = 0x0;
    r25 = *(u32*)(sp + 0x8);
    ctr_fn = (void(*)(void))r0;
    if ((u32)r0 <= (u32)0x0) goto L_801E4D54;
L_801E4CEC: ;
    r3 = r30 + r4;
    r0 = *(u8*)((u8*)r3 + 0x70);
    if ((s32)r0 == (s32)0x1) goto L_801E4D00;
    goto L_801E4D40;
L_801E4D00: ;
    r4 = *(u32*)((u8*)r26 + 0x0);
    r5 = 0x0;
    r0 = *(u32*)((u8*)r30 + 0xDC);
    r3 = *(u32*)((u8*)r25 + 0x0);
    r0 = r4 * r0;
    r4 = r27 + r0;
    fn_801ECAB0();
    *(u32*)((u8*)r25 + 0x8) = r3;
    r3 = (u32)lbl_8046AE38;
    r3 = (u32)lbl_8046AE38;
    r4 = r25;
    r0 = *(u32*)((u8*)r25 + 0x0);
    r5 = 0x1;
    *(u32*)((u8*)r25 + 0x4) = r0;
    ((void(*)(void))fn_8009F230)();
    goto L_801E4D54;
L_801E4D40: ;
    r0 = *(u32*)((u8*)r26 + 0x0);
    r26 = r26 + 0x4;
    r4 = r4 + 0x1;
    r27 = r27 + r0;
    if (--ctr != 0) goto L_801E4CEC;
L_801E4D54: ;
    if ((s32)r28 >= (s32)0x2) goto L_801E4D8C;
    r0 = *(u8*)((u8*)r30 + 0xA6);
    r0 = r0 & 0x1;
    if ((s32)r28 != (s32)0x2) goto L_801E4D8C;
    r3 = *(u32*)((u8*)r30 + 0x50);
    r5 = *(u32*)((u8*)r29 + 0x4);
    r4 = *(u32*)((u8*)r30 + 0xC0);
    /* subi r0, r3, 0x1 */;
    r3 = r5 + r4;
    if ((u32)r3 != (u32)r0) goto L_801E4D8C;
    r3 = 0x1;
    fn_801E446C();
L_801E4D8C: ;
    if ((s32)r28 != (s32)0x2) goto L_801E4D9C;
    r3 = 0x1;
    fn_801E446C();
L_801E4D9C: ;
    r3 = r29;
    fn_801E1B54();
    r28 = r28 + 0x1;
    goto L_801E4CA4;
}
#pragma pop

/* 0x801E4DAC | size: 0x3C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E4DAC(void) {
    extern u8 lbl_8046BE78[];
    extern u8 lbl_8047B480[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r0 = *(u32*)lbl_8047B480;
    if ((s32)r0 == (s32)0x0) goto L_801E4DD8;
    r3 = (u32)lbl_8046BE78;
    r3 = (u32)lbl_8046BE78;
    OSCancelThread();
    r0 = 0x0;
    *(u32*)lbl_8047B480 = r0;
L_801E4DD8: ;
    return;
}
#pragma pop

/* 0x801E4DE8 | size: 0x34 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E4DE8(void) {
    extern u8 lbl_8046BE78[];
    extern u8 lbl_8047B480[];
    extern void fn_800A1F94();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;

    r0 = *(u32*)lbl_8047B480;
    if ((s32)r0 == (s32)0x0) goto L_801E4E0C;
    r3 = (u32)lbl_8046BE78;
    r3 = (u32)lbl_8046BE78;
    fn_800A1F94();
L_801E4E0C: ;
    return;
}
#pragma pop

/* 0x801E4E1C | size: 0xD4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E4E1C(void) {
    extern u8 lbl_8046AE20[];
    extern u8 lbl_8047B480[];
    extern void fn_8009F1D0();
    extern void fn_800A19CC();
    extern void fn_801E4B38();
    extern void fn_801E4C80();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r31 = 0;

    r5 = (u32)lbl_8046AE20;
    r31 = (u32)lbl_8046AE20;
    if ((u32)r4 == (u32)0x0) goto L_801E4E78;
    r5 = (u32)fn_801E4B38;
    r6 = r31 + 0x58;
    r0 = (u32)fn_801E4B38;
    r8 = r3;
    r5 = r4;
    r3 = r31 + 0x1058;
    r4 = r0;
    r7 = 0x1000;
    r9 = 0x1;
    r6 = r6 + 0x1000;
    fn_800A19CC();
    if ((s32)r3 != (s32)0x0) goto L_801E4EB0;
    r3 = 0x0;
    goto L_801E4EDC;
L_801E4E78: ;
    r4 = (u32)fn_801E4C80;
    r6 = r31 + 0x58;
    r8 = r3;
    r3 = r31 + 0x1058;
    r4 = (u32)fn_801E4C80;
    r5 = 0x0;
    r7 = 0x1000;
    r9 = 0x1;
    r6 = r6 + 0x1000;
    fn_800A19CC();
    if ((s32)r3 != (s32)0x0) goto L_801E4EB0;
    r3 = 0x0;
    goto L_801E4EDC;
L_801E4EB0: ;
    r3 = r31 + 0x38;
    r4 = r31 + 0xc;
    r5 = 0x3;
    fn_8009F1D0();
    r3 = r31 + 0x18;
    r4 = r31 + 0x0;
    r5 = 0x3;
    fn_8009F1D0();
    r0 = 0x1;
    r3 = 0x1;
    *(u32*)lbl_8047B480 = r0;
L_801E4EDC: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801E4EF0 | size: 0x44 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E4EF0(void) {
    extern u8 lbl_8046C1A8[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r4 = (u32)lbl_8046C1A8;
    r5 = r3;
    r3 = (u32)lbl_8046C1A8;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8009F2F8)();
    if ((s32)r3 != (s32)0x1) goto L_801E4F20;
    r3 = *(u32*)(sp + 0x8);
    goto L_801E4F24;
L_801E4F20: ;
    r3 = 0x0;
L_801E4F24: ;
    return;
}
#pragma pop

/* 0x801E4F34 | size: 0x30 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E4F34(void) {
    extern u8 lbl_8046C1C8[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r5 = (u32)lbl_8046C1C8;
    r4 = r3;
    r3 = (u32)lbl_8046C1C8;
    r5 = 0x0;
    ((void(*)(void))fn_8009F230)();
    return;
}
#pragma pop

/* 0x801E4F64 | size: 0x1F0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E4F64(void) {
    extern u8 lbl_8046AC60[];
    extern u8 lbl_8046C190[];
    extern u8 lbl_8047B48C[];
    extern void fn_800A221C();
    extern void fn_801E446C();
    extern void fn_801E5548();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = (u32)lbl_8046AC60;
    /* stmw r23, 0x1c(r1) */;
    r31 = (u32)lbl_8046AC60;
    r4 = (u32)lbl_8046C190;
    r29 = r3;
    r30 = (u32)lbl_8046C190;
    r27 = 0x0;
    r28 = *(u32*)((u8*)r31 + 0xBC);
L_801E4F90: ;
    r0 = *(u8*)((u8*)r31 + 0xA7);
    if ((u32)r0 == (u32)0x0) goto L_801E500C;
    goto L_801E5000;
L_801E4FA0: ;
    OSDisableInterrupts();
    r4 = *(u32*)((u8*)r31 + 0xD8);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r31 + 0xD8) = r0;
    OSRestoreInterrupts();
    r0 = *(u32*)((u8*)r31 + 0xC0);
    r5 = *(u32*)((u8*)r31 + 0x50);
    r4 = r27 + r0;
    r3 = (u32)r4 / (u32)r5;
    /* subi r0, r5, 0x1 */;
    r3 = r3 * r5;
    r3 = r4 - r3;
    if ((u32)r3 != (u32)r0) goto L_801E4FF0;
    r0 = *(u8*)((u8*)r31 + 0xA6);
    r0 = r0 & 0x1;
    if ((u32)r3 == (u32)r0) goto L_801E500C;
    r28 = *(u32*)((u8*)r29 + 0x0);
    r29 = *(u32*)((u8*)r31 + 0xB4);
    goto L_801E4FFC;
L_801E4FF0: ;
    r0 = *(u32*)((u8*)r29 + 0x0);
    r29 = r29 + r28;
    r28 = r0;
L_801E4FFC: ;
    r27 = r27 + 0x1;
L_801E5000: ;
    r0 = *(u32*)((u8*)r31 + 0xD8);
    if ((s32)r0 < (s32)0x0) goto L_801E4FA0;
L_801E500C: ;
    r0 = *(u32*)((u8*)r31 + 0x6C);
    r25 = r29 + 0x8;
    r3 = r30 + 0x38;
    r4 = r1 + 0x8;
    r6 = r0 << 2;
    r5 = 0x1;
    r26 = r6 + 0x8;
    r26 = r29 + r26;
    ((void(*)(void))fn_8009F2F8)();
    r23 = *(u32*)(sp + 0x8);
    r24 = 0x0;
    goto L_801E50D0;
L_801E503C: ;
    r3 = r31 + r24;
    r0 = *(u8*)((u8*)r3 + 0x70);
    if ((s32)r0 == (s32)0x0) goto L_801E5050;
    goto L_801E50C0;
L_801E5050: ;
    r4 = *(u32*)((u8*)r23 + 0x0);
    r3 = r26;
    r5 = *(u32*)((u8*)r23 + 0x4);
    r6 = *(u32*)((u8*)r23 + 0x8);
    r7 = *(u32*)((u8*)r31 + 0x9C);
    fn_801E5548();
    *(u32*)((u8*)r31 + 0xAC) = r3;
    if ((s32)r3 == (s32)0x0) goto L_801E5098;
    r0 = *(u32*)lbl_8047B48C;
    if ((s32)r0 == (s32)0x0) goto L_801E5090;
    r3 = 0x0;
    fn_801E446C();
    r0 = 0x0;
    *(u32*)lbl_8047B48C = r0;
L_801E5090: ;
    r3 = r30 + 0x1058;
    fn_800A221C();
L_801E5098: ;
    *(u32*)((u8*)r23 + 0xC) = r27;
    r4 = r23;
    r3 = r30 + 0x18;
    r5 = 0x1;
    ((void(*)(void))fn_8009F230)();
    OSDisableInterrupts();
    r4 = *(u32*)((u8*)r31 + 0xD8);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r31 + 0xD8) = r0;
    OSRestoreInterrupts();
L_801E50C0: ;
    r0 = *(u32*)((u8*)r25 + 0x0);
    r25 = r25 + 0x4;
    r24 = r24 + 0x1;
    r26 = r26 + r0;
L_801E50D0: ;
    r0 = *(u32*)((u8*)r31 + 0x6C);
    if ((u32)r24 < (u32)r0) goto L_801E503C;
    r0 = *(u32*)lbl_8047B48C;
    if ((s32)r0 == (s32)0x0) goto L_801E50F8;
    r3 = 0x1;
    fn_801E446C();
    r0 = 0x0;
    *(u32*)lbl_8047B48C = r0;
L_801E50F8: ;
    r0 = *(u32*)((u8*)r31 + 0xC0);
    r5 = *(u32*)((u8*)r31 + 0x50);
    r4 = r27 + r0;
    r3 = (u32)r4 / (u32)r5;
    /* subi r0, r5, 0x1 */;
    r3 = r3 * r5;
    r3 = r4 - r3;
    if ((u32)r3 != (u32)r0) goto L_801E5140;
    r0 = *(u8*)((u8*)r31 + 0xA6);
    r0 = r0 & 0x1;
    if ((u32)r3 == (u32)r0) goto L_801E5134;
    r28 = *(u32*)((u8*)r29 + 0x0);
    r29 = *(u32*)((u8*)r31 + 0xB4);
    goto L_801E514C;
L_801E5134: ;
    r3 = r30 + 0x1058;
    fn_800A221C();
    goto L_801E514C;
L_801E5140: ;
    r0 = *(u32*)((u8*)r29 + 0x0);
    r29 = r29 + r28;
    r28 = r0;
L_801E514C: ;
    r27 = r27 + 0x1;
    goto L_801E4F90;
}
#pragma pop

/* 0x801E5154 | size: 0x2AC | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E5154(void) {
    extern u8 lbl_8046AC60[];
    extern u8 lbl_8046C190[];
    extern u8 lbl_8047B48C[];
    extern void fn_800A221C();
    extern void fn_801E1B84();
    extern void fn_801E1BB8();
    extern void fn_801E1BE8();
    extern void fn_801E446C();
    extern void fn_801E5548();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = (u32)lbl_8046C190;
    r3 = (u32)lbl_8046AC60;
    /* stmw r24, 0x10(r1) */;
    r30 = (u32)lbl_8046C190;
    r31 = (u32)lbl_8046AC60;
L_801E5174: ;
    r0 = *(u8*)((u8*)r31 + 0xA7);
    if ((u32)r0 == (u32)0x0) goto L_801E52E4;
    goto L_801E52D8;
L_801E5184: ;
    fn_801E1B84();
    r28 = r3;
    r5 = *(u32*)((u8*)r31 + 0x50);
    r3 = *(u32*)((u8*)r31 + 0xC0);
    r4 = *(u32*)((u8*)r28 + 0x4);
    /* subi r0, r5, 0x1 */;
    r4 = r4 + r3;
    r3 = (u32)r4 / (u32)r5;
    r3 = r3 * r5;
    r3 = r4 - r3;
    if ((u32)r3 != (u32)r0) goto L_801E52BC;
    r0 = *(u8*)((u8*)r31 + 0xA6);
    r0 = r0 & 0x1;
    if ((u32)r3 != (u32)r0) goto L_801E52BC;
    r0 = *(u32*)((u8*)r31 + 0x6C);
    r3 = r30 + 0x38;
    r7 = *(u32*)((u8*)r28 + 0x0);
    r4 = r1 + 0xc;
    r6 = r0 << 2;
    r5 = 0x1;
    r26 = r6 + 0x8;
    r25 = r7 + 0x8;
    r26 = r7 + r26;
    ((void(*)(void))fn_8009F2F8)();
    r3 = (u32)lbl_8046AC60;
    r29 = *(u32*)(sp + 0xC);
    r27 = (u32)lbl_8046AC60;
    r24 = 0x0;
    goto L_801E5294;
L_801E51FC: ;
    r3 = r27 + r24;
    r0 = *(u8*)((u8*)r3 + 0x70);
    if ((s32)r0 == (s32)0x0) goto L_801E5210;
    goto L_801E5284;
L_801E5210: ;
    r4 = *(u32*)((u8*)r29 + 0x0);
    r3 = r26;
    r5 = *(u32*)((u8*)r29 + 0x4);
    r6 = *(u32*)((u8*)r29 + 0x8);
    r7 = *(u32*)((u8*)r31 + 0x9C);
    fn_801E5548();
    *(u32*)((u8*)r31 + 0xAC) = r3;
    if ((s32)r3 == (s32)0x0) goto L_801E5258;
    r0 = *(u32*)lbl_8047B48C;
    if ((s32)r0 == (s32)0x0) goto L_801E5250;
    r3 = 0x0;
    fn_801E446C();
    r0 = 0x0;
    *(u32*)lbl_8047B48C = r0;
L_801E5250: ;
    r3 = r30 + 0x1058;
    fn_800A221C();
L_801E5258: ;
    r0 = *(u32*)((u8*)r28 + 0x4);
    r4 = r29;
    r3 = r30 + 0x18;
    r5 = 0x1;
    *(u32*)((u8*)r29 + 0xC) = r0;
    ((void(*)(void))fn_8009F230)();
    OSDisableInterrupts();
    r4 = *(u32*)((u8*)r31 + 0xD8);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r31 + 0xD8) = r0;
    OSRestoreInterrupts();
L_801E5284: ;
    r0 = *(u32*)((u8*)r25 + 0x0);
    r25 = r25 + 0x4;
    r24 = r24 + 0x1;
    r26 = r26 + r0;
L_801E5294: ;
    r0 = *(u32*)((u8*)r31 + 0x6C);
    if ((u32)r24 < (u32)r0) goto L_801E51FC;
    r0 = *(u32*)lbl_8047B48C;
    if ((s32)r0 == (s32)0x0) goto L_801E52BC;
    r3 = 0x1;
    fn_801E446C();
    r0 = 0x0;
    *(u32*)lbl_8047B48C = r0;
L_801E52BC: ;
    r3 = r28;
    fn_801E1BB8();
    OSDisableInterrupts();
    r4 = *(u32*)((u8*)r31 + 0xD8);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r31 + 0xD8) = r0;
    OSRestoreInterrupts();
L_801E52D8: ;
    r0 = *(u32*)((u8*)r31 + 0xD8);
    if ((s32)r0 < (s32)0x0) goto L_801E5184;
L_801E52E4: ;
    r0 = *(u8*)((u8*)r31 + 0xA7);
    if ((u32)r0 == (u32)0x0) goto L_801E52F8;
    fn_801E1B84();
    goto L_801E52FC;
L_801E52F8: ;
    fn_801E1BE8();
L_801E52FC: ;
    r0 = *(u32*)((u8*)r31 + 0x6C);
    r29 = r3;
    r6 = *(u32*)((u8*)r3 + 0x0);
    r3 = r30 + 0x38;
    r5 = r0 << 2;
    r4 = r1 + 0x8;
    r24 = r5 + 0x8;
    r25 = r6 + 0x8;
    r24 = r6 + r24;
    r5 = 0x1;
    ((void(*)(void))fn_8009F2F8)();
    r27 = *(u32*)(sp + 0x8);
    r26 = 0x0;
    goto L_801E53CC;
L_801E5334: ;
    r3 = r31 + r26;
    r0 = *(u8*)((u8*)r3 + 0x70);
    if ((s32)r0 == (s32)0x0) goto L_801E5348;
    goto L_801E53BC;
L_801E5348: ;
    r4 = *(u32*)((u8*)r27 + 0x0);
    r3 = r24;
    r5 = *(u32*)((u8*)r27 + 0x4);
    r6 = *(u32*)((u8*)r27 + 0x8);
    r7 = *(u32*)((u8*)r31 + 0x9C);
    fn_801E5548();
    *(u32*)((u8*)r31 + 0xAC) = r3;
    if ((s32)r3 == (s32)0x0) goto L_801E5390;
    r0 = *(u32*)lbl_8047B48C;
    if ((s32)r0 == (s32)0x0) goto L_801E5388;
    r3 = 0x0;
    fn_801E446C();
    r0 = 0x0;
    *(u32*)lbl_8047B48C = r0;
L_801E5388: ;
    r3 = r30 + 0x1058;
    fn_800A221C();
L_801E5390: ;
    r0 = *(u32*)((u8*)r29 + 0x4);
    r4 = r27;
    r3 = r30 + 0x18;
    r5 = 0x1;
    *(u32*)((u8*)r27 + 0xC) = r0;
    ((void(*)(void))fn_8009F230)();
    OSDisableInterrupts();
    r4 = *(u32*)((u8*)r31 + 0xD8);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r31 + 0xD8) = r0;
    OSRestoreInterrupts();
L_801E53BC: ;
    r0 = *(u32*)((u8*)r25 + 0x0);
    r25 = r25 + 0x4;
    r26 = r26 + 0x1;
    r24 = r24 + r0;
L_801E53CC: ;
    r0 = *(u32*)((u8*)r31 + 0x6C);
    if ((u32)r26 < (u32)r0) goto L_801E5334;
    r0 = *(u32*)lbl_8047B48C;
    if ((s32)r0 == (s32)0x0) goto L_801E53F4;
    r3 = 0x1;
    fn_801E446C();
    r0 = 0x0;
    *(u32*)lbl_8047B48C = r0;
L_801E53F4: ;
    r3 = r29;
    fn_801E1BB8();
    goto L_801E5174;
}
#pragma pop

/* 0x801E5400 | size: 0x3C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E5400(void) {
    extern u8 lbl_8046D1E8[];
    extern u8 lbl_8047B488[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r0 = *(u32*)lbl_8047B488;
    if ((s32)r0 == (s32)0x0) goto L_801E542C;
    r3 = (u32)lbl_8046D1E8;
    r3 = (u32)lbl_8046D1E8;
    OSCancelThread();
    r0 = 0x0;
    *(u32*)lbl_8047B488 = r0;
L_801E542C: ;
    return;
}
#pragma pop

/* 0x801E543C | size: 0x34 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E543C(void) {
    extern u8 lbl_8046D1E8[];
    extern u8 lbl_8047B488[];
    extern void fn_800A1F94();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;

    r0 = *(u32*)lbl_8047B488;
    if ((s32)r0 == (s32)0x0) goto L_801E5460;
    r3 = (u32)lbl_8046D1E8;
    r3 = (u32)lbl_8046D1E8;
    fn_800A1F94();
L_801E5460: ;
    return;
}
#pragma pop

/* 0x801E5470 | size: 0xD8 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E5470(void) {
    extern u8 lbl_8046C190[];
    extern u8 lbl_8047B488[];
    extern u8 lbl_8047B48C[];
    extern void fn_8009F1D0();
    extern void fn_800A19CC();
    extern void fn_801E4F64();
    extern void fn_801E5154();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r31 = 0;

    r5 = (u32)lbl_8046C190;
    r31 = (u32)lbl_8046C190;
    if ((u32)r4 == (u32)0x0) goto L_801E54CC;
    r5 = (u32)fn_801E4F64;
    r6 = r31 + 0x58;
    r0 = (u32)fn_801E4F64;
    r8 = r3;
    r5 = r4;
    r3 = r31 + 0x1058;
    r4 = r0;
    r7 = 0x1000;
    r9 = 0x1;
    r6 = r6 + 0x1000;
    fn_800A19CC();
    if ((s32)r3 != (s32)0x0) goto L_801E5504;
    r3 = 0x0;
    goto L_801E5534;
L_801E54CC: ;
    r4 = (u32)fn_801E5154;
    r6 = r31 + 0x58;
    r8 = r3;
    r3 = r31 + 0x1058;
    r4 = (u32)fn_801E5154;
    r5 = 0x0;
    r7 = 0x1000;
    r9 = 0x1;
    r6 = r6 + 0x1000;
    fn_800A19CC();
    if ((s32)r3 != (s32)0x0) goto L_801E5504;
    r3 = 0x0;
    goto L_801E5534;
L_801E5504: ;
    r3 = r31 + 0x38;
    r4 = r31 + 0xc;
    r5 = 0x3;
    fn_8009F1D0();
    r3 = r31 + 0x18;
    r4 = r31 + 0x0;
    r5 = 0x3;
    fn_8009F1D0();
    r0 = 0x1;
    r3 = 0x1;
    *(u32*)lbl_8047B488 = r0;
    *(u32*)lbl_8047B48C = r0;
L_801E5534: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801E5548 | size: 0x244 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E5548(void) {
    extern u8 lbl_8047B5AC[];
    extern u8 lbl_8047B5B0[];
    extern u8 lbl_8047B5B4[];
    extern void fn_8009B388();
    extern void fn_801E578C();
    extern void fn_801E57D0();
    extern void fn_801E590C();
    extern void fn_801E5A28();
    extern void fn_801E5DE4();
    extern void fn_801E62D8();
    extern void fn_801E6578();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0x24(r1) */;
    /* mr. r27, r3 */;
    r28 = r4 + 0x0;
    r29 = r5 + 0x0;
    r30 = r6 + 0x0;
    r31 = r7 + 0x0;
    if ((s32)r0 == (s32)0) goto L_801E573C;
    if ((u32)r28 == (u32)0x0) goto L_801E5744;
    if ((u32)r29 == (u32)0x0) goto L_801E5744;
    if ((u32)r30 == (u32)0x0) goto L_801E5744;
    if ((u32)r31 == (u32)0x0) goto L_801E574C;
    PPCMfhid2();
    r0 = r3 & 0x10000000;
    if ((u32)r31 == (u32)0x0) goto L_801E576C;
    r0 = *(u32*)lbl_8047B5B4;
    if ((s32)r0 == (s32)0x0) goto L_801E5774;
    *(u32*)lbl_8047B5AC = r31;
    r4 = 0x6bc;
    r3 = *(u32*)lbl_8047B5AC;
    r0 = r3 + 0x1f;
    /* clrrwi r3, r0, 5 */;
    *(u32*)lbl_8047B5B0 = r3;
    r0 = r3 + 0x6bc;
    *(u32*)lbl_8047B5AC = r0;
    r3 = *(u32*)lbl_8047B5B0;
    fn_8009B388();
    r3 = *(u32*)lbl_8047B5B0;
    r4 = 0x21;
    r0 = 0x0;
    *(u32*)((u8*)r3 + 0x6A4) = r4;
    r31 = 0x0;
    r3 = *(u32*)lbl_8047B5B0;
    *(u16*)((u8*)r3 + 0x698) = r0;
    r3 = *(u32*)lbl_8047B5B0;
    *(u32*)((u8*)r3 + 0x69C) = r27;
L_801E55F4: ;
    r4 = *(u32*)lbl_8047B5B0;
    r3 = *(u32*)((u8*)r4 + 0x69C);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x69C) = r0;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 != (u32)0xff) goto L_801E575C;
    goto L_801E5620;
L_801E5614: ;
    r3 = *(u32*)((u8*)r4 + 0x0);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x0) = r0;
L_801E5620: ;
    r3 = *(u32*)lbl_8047B5B0;
    r4 = r3 + 0x69c;
    r3 = *(u32*)((u8*)r3 + 0x69C);
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0xff) goto L_801E5614;
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x0) = r0;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 > (u32)0xd7) goto L_801E567C;
    if ((u32)r0 != (u32)0xc4) goto L_801E5664;
    fn_801E5DE4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0xc4) goto L_801E5764;
    goto L_801E5718;
L_801E5664: ;
    if ((u32)r0 != (u32)0xc0) goto L_801E5754;
    fn_801E57D0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0xc0) goto L_801E5764;
    goto L_801E5718;
L_801E567C: ;
    if ((u32)r0 < (u32)0xd8) goto L_801E56DC;
    if ((u32)r0 > (u32)0xdf) goto L_801E56DC;
    if ((u32)r0 != (u32)0xdd) goto L_801E569C;
    fn_801E62D8();
    goto L_801E5718;
L_801E569C: ;
    if ((u32)r0 != (u32)0xdb) goto L_801E56B4;
    fn_801E5A28();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0xdb) goto L_801E5764;
    goto L_801E5718;
L_801E56B4: ;
    if ((u32)r0 != (u32)0xda) goto L_801E56D0;
    fn_801E590C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0xda) goto L_801E5764;
    r31 = 0x1;
    goto L_801E5718;
L_801E56D0: ;
    if ((u32)r0 == (u32)0xd8) goto L_801E5718;
    goto L_801E5754;
L_801E56DC: ;
    if ((u32)r0 < (u32)0xe0) goto L_801E5718;
    if ((u32)r0 < (u32)0xe0) goto L_801E56F0;
    if ((u32)r0 <= (u32)0xef) goto L_801E56F8;
L_801E56F0: ;
    if ((u32)r0 != (u32)0xfe) goto L_801E5754;
L_801E56F8: ;
    r3 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r3 + 0x69C);
    r5 = r3 + 0x69c;
    r3 = *(u8*)((u8*)r4 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x1);
    r0 = (r0 & ~0x0000FF00) | (((r3 << 8) | ((u32)r3 >> 24)) & 0x0000FF00);
    r0 = r4 + r0;
    *(u32*)((u8*)r5 + 0x0) = r0;
L_801E5718: ;
    r0 = r31 & 0xFF;
    if ((u32)r0 == (u32)0xfe) goto L_801E55F4;
    fn_801E578C();
    r3 = r28 + 0x0;
    r4 = r29 + 0x0;
    r5 = r30 + 0x0;
    fn_801E6578();
    r3 = 0x0;
    goto L_801E5778;
L_801E573C: ;
    r3 = 0x19;
    goto L_801E5778;
L_801E5744: ;
    r3 = 0x1b;
    goto L_801E5778;
L_801E574C: ;
    r3 = 0x1a;
    goto L_801E5778;
L_801E5754: ;
    r3 = 0xb;
    goto L_801E5778;
L_801E575C: ;
    r3 = 0x3;
    goto L_801E5778;
L_801E5764: ;
    r3 = r3 & 0xFF;
    goto L_801E5778;
L_801E576C: ;
    r3 = 0x1c;
    goto L_801E5778;
L_801E5774: ;
    r3 = 0x1d;
L_801E5778: ;
    /* lmw r27, 0x24(r1) */;
    return;
}
#pragma pop

/* 0x801E578C | size: 0x44 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E578C(void) {
    extern u8 lbl_8046D618[];
    extern u8 lbl_8047B5AC[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r4 = *(u32*)lbl_8047B5AC;
    r3 = (u32)lbl_8046D618;
    r5 = (u32)lbl_8046D618;
    r0 = r4 + 0x1f;
    /* clrrwi r6, r0, 5 */;
    *(u32*)((u8*)r5 + 0x0) = r6;
    r3 = r6 + 0x80;
    r0 = r6 + 0x100;
    *(u32*)((u8*)r5 + 0x4) = r3;
    r4 = r6 + 0x180;
    r3 = r6 + 0x200;
    *(u32*)((u8*)r5 + 0x8) = r0;
    r0 = r6 + 0x280;
    *(u32*)((u8*)r5 + 0xC) = r4;
    *(u32*)((u8*)r5 + 0x10) = r3;
    *(u32*)((u8*)r5 + 0x14) = r0;
    return;
}
#pragma pop

/* 0x801E57D0 | size: 0x13C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E57D0(void) {
    extern u8 lbl_8047B5B0[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    r4 = *(u32*)lbl_8047B5B0;
    r3 = *(u32*)((u8*)r4 + 0x69C);
    r0 = r3 + 0x2;
    *(u32*)((u8*)r4 + 0x69C) = r0;
    r4 = *(u32*)lbl_8047B5B0;
    r3 = *(u32*)((u8*)r4 + 0x69C);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x69C) = r0;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x8) goto L_801E5804;
    r3 = 0xa;
    return;
L_801E5804: ;
    r5 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r5 + 0x69C);
    r3 = *(u8*)((u8*)r4 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x1);
    r0 = (r0 & ~0x0000FF00) | (((r3 << 8) | ((u32)r3 >> 24)) & 0x0000FF00);
    *(u16*)((u8*)r5 + 0x694) = r0;
    r4 = *(u32*)lbl_8047B5B0;
    r3 = *(u32*)((u8*)r4 + 0x69C);
    r0 = r3 + 0x2;
    *(u32*)((u8*)r4 + 0x69C) = r0;
    r5 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r5 + 0x69C);
    r3 = *(u8*)((u8*)r4 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x1);
    r0 = (r0 & ~0x0000FF00) | (((r3 << 8) | ((u32)r3 >> 24)) & 0x0000FF00);
    *(u16*)((u8*)r5 + 0x692) = r0;
    r4 = *(u32*)lbl_8047B5B0;
    r3 = *(u32*)((u8*)r4 + 0x69C);
    r0 = r3 + 0x2;
    *(u32*)((u8*)r4 + 0x69C) = r0;
    r4 = *(u32*)lbl_8047B5B0;
    r3 = *(u32*)((u8*)r4 + 0x69C);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x69C) = r0;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x3) goto L_801E5878;
    r3 = 0xc;
    return;
L_801E5878: ;
    r7 = 0x0;
    r6 = 0x0;
    goto L_801E58F8;
L_801E5884: ;
    r4 = *(u32*)lbl_8047B5B0;
    r0 = r7 & 0xFF;
    r3 = *(u32*)((u8*)r4 + 0x69C);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x69C) = r0;
    r4 = *(u32*)lbl_8047B5B0;
    r3 = *(u32*)((u8*)r4 + 0x69C);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x69C) = r0;
    r3 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 != (u32)0x3) goto L_801E58B8;
    if ((u32)r3 != (u32)0x22) goto L_801E58C8;
L_801E58B8: ;
    r0 = r7 & 0xFF;
    if ((u32)r3 == (u32)0x22) goto L_801E58D0;
    if ((u32)r3 == (u32)0x11) goto L_801E58D0;
L_801E58C8: ;
    r3 = 0x13;
    return;
L_801E58D0: ;
    r5 = *(u32*)lbl_8047B5B0;
    r0 = r6 + 0x680;
    r6 = r6 + 0x6;
    r4 = *(u32*)((u8*)r5 + 0x69C);
    r7 = r7 + 0x1;
    r3 = r4 + 0x1;
    *(u32*)((u8*)r5 + 0x69C) = r3;
    r4 = *(u8*)((u8*)r4 + 0x0);
    r3 = *(u32*)lbl_8047B5B0;
    *(u8*)(r3 + r0) = r4;
L_801E58F8: ;
    r0 = r7 & 0xFF;
    if ((u32)r0 < (u32)0x3) goto L_801E5884;
    r3 = 0x0;
    return;
}
#pragma pop

/* 0x801E590C | size: 0x11C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E590C(void) {
    extern u8 lbl_8047B5B0[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;

    r4 = *(u32*)lbl_8047B5B0;
    r3 = *(u32*)((u8*)r4 + 0x69C);
    r0 = r3 + 0x2;
    *(u32*)((u8*)r4 + 0x69C) = r0;
    r4 = *(u32*)lbl_8047B5B0;
    r3 = *(u32*)((u8*)r4 + 0x69C);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x69C) = r0;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x3) goto L_801E5940;
    r3 = 0xc;
    return;
L_801E5940: ;
    r9 = 0x0;
    r8 = 0x0;
    goto L_801E59D0;
L_801E594C: ;
    r6 = *(u32*)lbl_8047B5B0;
    r0 = r8 + 0x681;
    r4 = r8 + 0x682;
    r5 = *(u32*)((u8*)r6 + 0x69C);
    r3 = 0x1;
    r5 = r5 + 0x1;
    *(u32*)((u8*)r6 + 0x69C) = r5;
    r7 = *(u32*)lbl_8047B5B0;
    r6 = *(u32*)((u8*)r7 + 0x69C);
    r5 = r6 + 0x1;
    *(u32*)((u8*)r7 + 0x69C) = r5;
    r7 = *(u8*)((u8*)r6 + 0x0);
    r5 = *(u32*)lbl_8047B5B0;
    r6 = (s32)r7 >> 4;
    *(u8*)(r5 + r0) = r6;
    r7 = r7 & 0xF;
    r0 = r3 << r6;
    r5 = *(u32*)lbl_8047B5B0;
    *(u8*)(r5 + r4) = r7;
    r4 = *(u32*)lbl_8047B5B0;
    r4 = *(u8*)((u8*)r4 + 0x6A8);
    /* and. r0, r4, r0 */;
    if ((u32)r0 != (u32)0x3) goto L_801E59B0;
    r3 = 0xf;
    return;
L_801E59B0: ;
    r0 = r7 + 0x1;
    r0 = r3 << r0;
    /* and. r0, r4, r0 */;
    if ((u32)r0 != (u32)0x3) goto L_801E59C8;
    r3 = 0xf;
    return;
L_801E59C8: ;
    r8 = r8 + 0x6;
    r9 = r9 + 0x1;
L_801E59D0: ;
    r0 = r9 & 0xFF;
    if ((u32)r0 < (u32)0x3) goto L_801E594C;
    r5 = *(u32*)lbl_8047B5B0;
    r0 = 0x0;
    r3 = 0x0;
    r4 = *(u32*)((u8*)r5 + 0x69C);
    r4 = r4 + 0x3;
    *(u32*)((u8*)r5 + 0x69C) = r4;
    r5 = *(u32*)lbl_8047B5B0;
    r4 = *(u16*)((u8*)r5 + 0x692);
    r4 = r4 + 0xf;
    r4 = (s32)r4 >> 4;
    /* addze r4, r4 */;
    *(u16*)((u8*)r5 + 0x696) = r4;
    r4 = *(u32*)lbl_8047B5B0;
    *(u16*)((u8*)r4 + 0x684) = r0;
    r4 = *(u32*)lbl_8047B5B0;
    *(u16*)((u8*)r4 + 0x68A) = r0;
    r4 = *(u32*)lbl_8047B5B0;
    *(u16*)((u8*)r4 + 0x690) = r0;
    return;
}
#pragma pop

/* 0x801E5A28 | size: 0x3BC | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E5A28(void) {
    extern u8 lbl_80279AE8[];
    extern u8 lbl_8047B5B0[];
    extern u8 lbl_8047E4B0[];
    u8 sp[0x188];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    r4 = (u32)lbl_80279AE8;
    r4 = (u32)lbl_80279AE8;
    /* stmw r21, 0x15c(r1) */;
    r6 = r4 + 0x50;
    r3 = *(u32*)lbl_8047B5B0;
    r5 = *(u32*)((u8*)r3 + 0x69C);
    r8 = r3 + 0x69c;
    r3 = r1 + 0x14;
    r7 = *(u8*)((u8*)r5 + 0x0);
    r0 = r5 + 0x2;
    r5 = *(u8*)((u8*)r5 + 0x1);
    r5 = (r5 & ~0x0000FF00) | (((r7 << 8) | ((u32)r7 >> 24)) & 0x0000FF00);
    *(u32*)((u8*)r8 + 0x0) = r0;
    r7 = r5 & 0xFFFF;
    r0 = (0x4330 << 16);
    f0 = *(f64*)lbl_8047E4B0;
    /* subi r7, r7, 0x2 */;
L_801E5A70: ;
    r11 = *(u32*)lbl_8047B5B0;
    r5 = r4 + 0x0;
    r9 = 0x0;
    r10 = *(u32*)((u8*)r11 + 0x69C);
    r8 = r10 + 0x1;
    *(u32*)((u8*)r11 + 0x69C) = r8;
    r8 = *(u8*)((u8*)r10 + 0x0);
    goto L_801E5C50;
L_801E5A90: ;
    r30 = *(u32*)lbl_8047B5B0;
    r28 = r9 + 0x1;
    r29 = r9 + 0x2;
    r27 = *(u32*)((u8*)r30 + 0x69C);
    r12 = r9 + 0x3;
    r11 = r9 + 0x4;
    r10 = r27 + 0x1;
    *(u32*)((u8*)r30 + 0x69C) = r10;
    r10 = r9 + 0x5;
    r28 = r28 & 0xFFFF;
    r30 = *(u8*)((u8*)r27 + 0x0);
    r29 = r29 & 0xFFFF;
    r27 = *(u8*)((u8*)r5 + 0x0);
    r12 = r12 & 0xFFFF;
    r27 = r27 << 2;
    *(u32*)(sp + 0x150) = r0;
    r11 = r11 & 0xFFFF;
    r10 = r10 & 0xFFFF;
    f1 = *(f64*)(sp + 0x150);
    f1 = f1 - f0;
    *(f32*)(r3 + r27) = f1;
    r27 = *(u32*)lbl_8047B5B0;
    r31 = *(u32*)((u8*)r27 + 0x69C);
    r30 = r31 + 0x1;
    *(u32*)((u8*)r27 + 0x69C) = r30;
    r30 = *(u8*)((u8*)r31 + 0x0);
    r28 = *(u8*)(r4 + r28);
    r28 = r28 << 2;
    *(u32*)(sp + 0x148) = r0;
    f1 = *(f64*)(sp + 0x148);
    f1 = f1 - f0;
    *(f32*)(r3 + r28) = f1;
    r28 = *(u32*)lbl_8047B5B0;
    r31 = *(u32*)((u8*)r28 + 0x69C);
    r30 = r31 + 0x1;
    *(u32*)((u8*)r28 + 0x69C) = r30;
    r30 = *(u8*)((u8*)r31 + 0x0);
    r29 = *(u8*)(r4 + r29);
    r29 = r29 << 2;
    *(u32*)(sp + 0x140) = r0;
    f1 = *(f64*)(sp + 0x140);
    f1 = f1 - f0;
    *(f32*)(r3 + r29) = f1;
    r31 = *(u32*)lbl_8047B5B0;
    r30 = *(u32*)((u8*)r31 + 0x69C);
    r29 = r30 + 0x1;
    *(u32*)((u8*)r31 + 0x69C) = r29;
    r29 = *(u8*)((u8*)r30 + 0x0);
    r12 = *(u8*)(r4 + r12);
    r12 = r12 << 2;
    *(u32*)(sp + 0x138) = r0;
    f1 = *(f64*)(sp + 0x138);
    f1 = f1 - f0;
    *(f32*)(r3 + r12) = f1;
    r30 = *(u32*)lbl_8047B5B0;
    r29 = *(u32*)((u8*)r30 + 0x69C);
    r12 = r29 + 0x1;
    *(u32*)((u8*)r30 + 0x69C) = r12;
    r12 = *(u8*)((u8*)r29 + 0x0);
    r11 = *(u8*)(r4 + r11);
    r11 = r11 << 2;
    *(u32*)(sp + 0x130) = r0;
    f1 = *(f64*)(sp + 0x130);
    f1 = f1 - f0;
    *(f32*)(r3 + r11) = f1;
    r29 = *(u32*)lbl_8047B5B0;
    r12 = *(u32*)((u8*)r29 + 0x69C);
    r11 = r12 + 0x1;
    *(u32*)((u8*)r29 + 0x69C) = r11;
    r11 = *(u8*)((u8*)r12 + 0x0);
    r10 = *(u8*)(r4 + r10);
    r10 = r10 << 2;
    *(u32*)(sp + 0x128) = r0;
    f1 = *(f64*)(sp + 0x128);
    f1 = f1 - f0;
    *(f32*)(r3 + r10) = f1;
    r30 = *(u32*)lbl_8047B5B0;
    r11 = r9 + 0x6;
    r10 = r9 + 0x7;
    r29 = *(u32*)((u8*)r30 + 0x69C);
    r11 = r11 & 0xFFFF;
    r10 = r10 & 0xFFFF;
    r12 = r29 + 0x1;
    *(u32*)((u8*)r30 + 0x69C) = r12;
    r5 = r5 + 0x8;
    r9 = r9 + 0x8;
    r12 = *(u8*)((u8*)r29 + 0x0);
    r11 = *(u8*)(r4 + r11);
    r11 = r11 << 2;
    *(u32*)(sp + 0x120) = r0;
    f1 = *(f64*)(sp + 0x120);
    f1 = f1 - f0;
    *(f32*)(r3 + r11) = f1;
    r29 = *(u32*)lbl_8047B5B0;
    r12 = *(u32*)((u8*)r29 + 0x69C);
    r11 = r12 + 0x1;
    *(u32*)((u8*)r29 + 0x69C) = r11;
    r11 = *(u8*)((u8*)r12 + 0x0);
    r10 = *(u8*)(r4 + r10);
    r10 = r10 << 2;
    *(u32*)(sp + 0x118) = r0;
    f1 = *(f64*)(sp + 0x118);
    f1 = f1 - f0;
    *(f32*)(r3 + r10) = f1;
L_801E5C50: ;
    r10 = r9 & 0xFFFF;
    if ((u32)r10 < (u32)0x40) goto L_801E5A90;
    r29 = *(u32*)lbl_8047B5B0;
    r27 = r6 + 0x0;
    r28 = r8 << 8;
    r5 = 0x0;
    r8 = 0x0;
    goto L_801E5DBC;
L_801E5C74: ;
    /* clrlslwi r11, r5, 16, 2 */;
    f1 = *(f64*)((u8*)r27 + 0x0);
    f2 = *(f32*)(r3 + r11);
    r9 = r5 + 0x1;
    /* clrlslwi r12, r9, 16, 2 */;
    f3 = *(f64*)((u8*)r6 + 0x0);
    f1 = f2 * f1;
    r9 = r5 + 0x2;
    /* clrlslwi r26, r9, 16, 2 */;
    r9 = r5 + 0x3;
    f1 = f3 * f1;
    /* clrlslwi r25, r9, 16, 2 */;
    r10 = r5 + 0x4;
    /* clrlslwi r24, r10, 16, 2 */;
    f1 = (f32)f1;
    r9 = r29 + r11;
    r10 = r5 + 0x7;
    *(f32*)(r28 + r9) = f1;
    r9 = r5 + 0x5;
    /* clrlslwi r23, r9, 16, 2 */;
    f2 = *(f32*)(r3 + r12);
    r9 = r5 + 0x6;
    f1 = *(f64*)((u8*)r27 + 0x0);
    /* clrlslwi r22, r9, 16, 2 */;
    r9 = r29 + r12;
    f1 = f2 * f1;
    f3 = *(f64*)((u8*)r6 + 0x8);
    /* clrlslwi r21, r10, 16, 2 */;
    r30 = r29 + r26;
    r31 = r29 + r25;
    f1 = f3 * f1;
    r12 = r29 + r24;
    r11 = r29 + r23;
    r10 = r29 + r22;
    r5 = r5 + 0x8;
    f1 = (f32)f1;
    r8 = r8 + 0x1;
    *(f32*)(r28 + r9) = f1;
    r9 = r29 + r21;
    f2 = *(f32*)(r3 + r26);
    f1 = *(f64*)((u8*)r27 + 0x0);
    f3 = *(f64*)((u8*)r6 + 0x10);
    f1 = f2 * f1;
    f1 = f3 * f1;
    f1 = (f32)f1;
    *(f32*)(r28 + r30) = f1;
    f2 = *(f32*)(r3 + r25);
    f1 = *(f64*)((u8*)r27 + 0x0);
    f3 = *(f64*)((u8*)r6 + 0x18);
    f1 = f2 * f1;
    f1 = f3 * f1;
    f1 = (f32)f1;
    *(f32*)(r28 + r31) = f1;
    f2 = *(f32*)(r3 + r24);
    f1 = *(f64*)((u8*)r27 + 0x0);
    f3 = *(f64*)((u8*)r6 + 0x20);
    f1 = f2 * f1;
    f1 = f3 * f1;
    f1 = (f32)f1;
    *(f32*)(r28 + r12) = f1;
    f2 = *(f32*)(r3 + r23);
    f1 = *(f64*)((u8*)r27 + 0x0);
    f3 = *(f64*)((u8*)r6 + 0x28);
    f1 = f2 * f1;
    f1 = f3 * f1;
    f1 = (f32)f1;
    *(f32*)(r28 + r11) = f1;
    f2 = *(f32*)(r3 + r22);
    f1 = *(f64*)((u8*)r27 + 0x0);
    f3 = *(f64*)((u8*)r6 + 0x30);
    f1 = f2 * f1;
    f1 = f3 * f1;
    f1 = (f32)f1;
    *(f32*)(r28 + r10) = f1;
    f1 = *(f64*)((u8*)r27 + 0x0);
    r27 = r27 + 0x8;
    f2 = *(f32*)(r3 + r21);
    f3 = *(f64*)((u8*)r6 + 0x38);
    f1 = f2 * f1;
    f1 = f3 * f1;
    f1 = (f32)f1;
    *(f32*)(r28 + r9) = f1;
L_801E5DBC: ;
    r9 = r8 & 0xFFFF;
    if ((u32)r9 < (u32)0x8) goto L_801E5C74;
    /* subi r7, r7, 0x41 */;
    r5 = r7 & 0xFFFF;
    if ((u32)r9 != (u32)0x8) goto L_801E5A70;
    r3 = 0x0;
    /* lmw r21, 0x15c(r1) */;
    return;
}
#pragma pop

/* 0x801E5DE4 | size: 0x1E0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E5DE4(void) {
    extern u8 lbl_8047B544[];
    extern u8 lbl_8047B548[];
    extern u8 lbl_8047B54C[];
    extern u8 lbl_8047B5AC[];
    extern u8 lbl_8047B5B0[];
    extern void fn_801E5FC4();
    extern void fn_801E60B4();
    extern void fn_801E611C();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = *(u32*)lbl_8047B5AC;
    r3 = *(u32*)lbl_8047B5B0;
    r0 = r4 + 0x101;
    *(u32*)lbl_8047B548 = r4;
    r5 = r3 + 0x69c;
    *(u32*)lbl_8047B54C = r0;
    r3 = *(u32*)((u8*)r3 + 0x69C);
    r4 = *(u8*)((u8*)r3 + 0x0);
    r0 = r3 + 0x2;
    r3 = *(u8*)((u8*)r3 + 0x1);
    r3 = (r3 & ~0x0000FF00) | (((r4 << 8) | ((u32)r4 >> 24)) & 0x0000FF00);
    r30 = r3 & 0xFFFF;
    *(u32*)((u8*)r5 + 0x0) = r0;
    /* subi r30, r30, 0x2 */;
L_801E5E34: ;
    r5 = *(u32*)lbl_8047B5B0;
    r29 = 0x0;
    r3 = 0x0;
    r4 = *(u32*)((u8*)r5 + 0x69C);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r5 + 0x69C) = r0;
    r6 = *(u8*)((u8*)r4 + 0x0);
    r4 = *(u32*)lbl_8047B5B0;
    r0 = (s32)r6 >> 4;
    r5 = *(u32*)((u8*)r4 + 0x69C);
    /* clrlslwi r4, r6, 28, 1 */;
    r0 = r0 & 0xFF;
    r0 = r4 + r0;
    *(u32*)lbl_8047B544 = r5;
    r31 = r0 & 0xFF;
    goto L_801E5F38;
L_801E5E74: ;
    r5 = *(u32*)lbl_8047B5B0;
    r3 = r3 + 0x8;
    r4 = *(u32*)((u8*)r5 + 0x69C);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r5 + 0x69C) = r0;
    r5 = *(u32*)lbl_8047B5B0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    r4 = *(u32*)((u8*)r5 + 0x69C);
    r29 = r29 + r0;
    r0 = r4 + 0x1;
    *(u32*)((u8*)r5 + 0x69C) = r0;
    r5 = *(u32*)lbl_8047B5B0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    r4 = *(u32*)((u8*)r5 + 0x69C);
    r29 = r29 + r0;
    r0 = r4 + 0x1;
    *(u32*)((u8*)r5 + 0x69C) = r0;
    r5 = *(u32*)lbl_8047B5B0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    r4 = *(u32*)((u8*)r5 + 0x69C);
    r29 = r29 + r0;
    r0 = r4 + 0x1;
    *(u32*)((u8*)r5 + 0x69C) = r0;
    r5 = *(u32*)lbl_8047B5B0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    r4 = *(u32*)((u8*)r5 + 0x69C);
    r29 = r29 + r0;
    r0 = r4 + 0x1;
    *(u32*)((u8*)r5 + 0x69C) = r0;
    r5 = *(u32*)lbl_8047B5B0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    r4 = *(u32*)((u8*)r5 + 0x69C);
    r29 = r29 + r0;
    r0 = r4 + 0x1;
    *(u32*)((u8*)r5 + 0x69C) = r0;
    r5 = *(u32*)lbl_8047B5B0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    r4 = *(u32*)((u8*)r5 + 0x69C);
    r29 = r29 + r0;
    r0 = r4 + 0x1;
    *(u32*)((u8*)r5 + 0x69C) = r0;
    r5 = *(u32*)lbl_8047B5B0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    r4 = *(u32*)((u8*)r5 + 0x69C);
    r29 = r29 + r0;
    r0 = r4 + 0x1;
    *(u32*)((u8*)r5 + 0x69C) = r0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    r29 = r29 + r0;
L_801E5F38: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 < (u32)0x10) goto L_801E5E74;
    r3 = r31 * 0xe0;
    r5 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r5 + 0x69C);
    r0 = r3 + 0x340;
    *(u32*)(r5 + r0) = r4;
    r0 = r29 & 0xFFFF;
    r4 = *(u32*)lbl_8047B5B0;
    r3 = *(u32*)((u8*)r4 + 0x69C);
    r0 = r3 + r0;
    *(u32*)((u8*)r4 + 0x69C) = r0;
    fn_801E5FC4();
    fn_801E60B4();
    r3 = r31;
    fn_801E611C();
    r5 = *(u32*)lbl_8047B5B0;
    r0 = r29 + 0x11;
    r30 = r30 - r0;
    r0 = 0x1;
    r4 = *(u8*)((u8*)r5 + 0x6A8);
    r3 = r0 << r31;
    r3 = r4 | r3;
    r0 = r30 & 0xFFFF;
    *(u8*)((u8*)r5 + 0x6A8) = r3;
    if ((u32)r0 != (u32)0x10) goto L_801E5E34;
    r3 = 0x0;
    return;
}
#pragma pop

/* 0x801E5FC4 | size: 0xF0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E5FC4(void) {
    extern u8 lbl_8047B544[];
    extern u8 lbl_8047B548[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r6 = 0x0;
    r7 = 0x1;
L_801E5FCC: ;
    r3 = *(u32*)lbl_8047B544;
    /* subi r0, r7, 0x1 */;
    r5 = r7 & 0xFF;
    r8 = *(u8*)(r3 + r0);
    r3 = r8 + 0x0;
    if ((s32)r8 == (s32)0x0) goto L_801E6098;
    /* srwi. r0, r3, 3 */;
    ctr_fn = (void(*)(void))r0;
    if ((s32)r8 == (s32)0x0) goto L_801E6080;
L_801E5FF4: ;
    r4 = *(u32*)lbl_8047B548;
    r0 = r6;
    r6 = r6 + 0x1;
    *(u8*)(r4 + r0) = r5;
    r0 = r6 + 0x0;
    r6 = r6 + 0x1;
    r4 = *(u32*)lbl_8047B548;
    *(u8*)(r4 + r0) = r5;
    r0 = r6 + 0x0;
    r6 = r6 + 0x1;
    r4 = *(u32*)lbl_8047B548;
    *(u8*)(r4 + r0) = r5;
    r0 = r6 + 0x0;
    r6 = r6 + 0x1;
    r4 = *(u32*)lbl_8047B548;
    *(u8*)(r4 + r0) = r5;
    r0 = r6 + 0x0;
    r6 = r6 + 0x1;
    r4 = *(u32*)lbl_8047B548;
    *(u8*)(r4 + r0) = r5;
    r0 = r6 + 0x0;
    r6 = r6 + 0x1;
    r4 = *(u32*)lbl_8047B548;
    *(u8*)(r4 + r0) = r5;
    r0 = r6 + 0x0;
    r6 = r6 + 0x1;
    r4 = *(u32*)lbl_8047B548;
    *(u8*)(r4 + r0) = r5;
    r0 = r6 + 0x0;
    r6 = r6 + 0x1;
    r4 = *(u32*)lbl_8047B548;
    *(u8*)(r4 + r0) = r5;
    if (--ctr != 0) goto L_801E5FF4;
    r3 = r3 & 0x7;
    if ((s32)r8 == (s32)0x0) goto L_801E6098;
L_801E6080: ;
    ctr_fn = (void(*)(void))r3;
L_801E6084: ;
    r4 = *(u32*)lbl_8047B548;
    r0 = r6;
    r6 = r6 + 0x1;
    *(u8*)(r4 + r0) = r5;
    if (--ctr != 0) goto L_801E6084;
L_801E6098: ;
    r7 = r7 + 0x1;
    if ((s32)r7 <= (s32)0x10) goto L_801E5FCC;
    r3 = *(u32*)lbl_8047B548;
    r0 = 0x0;
    *(u8*)(r3 + r6) = r0;
    return;
}
#pragma pop

/* 0x801E60B4 | size: 0x68 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E60B4(void) {
    extern u8 lbl_8047B548[];
    extern u8 lbl_8047B54C[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;

    r6 = *(u32*)lbl_8047B548;
    r8 = 0x0;
    r5 = *(u32*)lbl_8047B54C;
    r9 = 0x0;
    r7 = *(u8*)((u8*)r6 + 0x0);
    r3 = 0x1;
    goto L_801E6108;
L_801E60D0: ;
    r4 = r7 & 0xFF;
    goto L_801E60E8;
L_801E60D8: ;
    /* clrlslwi r0, r8, 16, 1 */;
    *(u16*)(r5 + r0) = r9;
    r8 = r8 + 0x1;
    r9 = r9 + 0x1;
L_801E60E8: ;
    r0 = r8 & 0xFFFF;
    r0 = *(u8*)(r6 + r0);
    if ((u32)r4 == (u32)r0) goto L_801E60D8;
    r0 = r9 & 0xFFFF;
    r0 = r0 << r3;
    r9 = r0 & 0xFFFF;
    r7 = r7 + 0x1;
L_801E6108: ;
    r0 = r8 & 0xFFFF;
    r0 = *(u8*)(r6 + r0);
    if ((u32)r0 != (u32)0x0) goto L_801E60D0;
    return;
}
#pragma pop

/* 0x801E611C | size: 0x1BC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E611C(void) {
    extern u8 lbl_8047B544[];
    extern u8 lbl_8047B54C[];
    extern u8 lbl_8047B5B0[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r0 = r3 & 0xFF;
    r4 = *(u32*)lbl_8047B5B0;
    r3 = r0 * 0xe0;
    r7 = r3 + 0x300;
    r0 = 0x4;
    r7 = r4 + r7;
    ctr_fn = (void(*)(void))r0;
    r6 = r7 + 0x4;
    r8 = 0x0;
    r9 = 0x1;
L_801E6144: ;
    r3 = *(u32*)lbl_8047B544;
    /* subi r5, r9, 0x1 */;
    r0 = *(u8*)(r3 + r5);
    if ((u32)r0 == (u32)0x0) goto L_801E6190;
    r3 = *(u32*)lbl_8047B54C;
    r0 = r8 << 1;
    r0 = *(u16*)(r3 + r0);
    r0 = r8 - r0;
    *(u32*)((u8*)r6 + 0x8C) = r0;
    r3 = *(u32*)lbl_8047B544;
    r4 = *(u32*)lbl_8047B54C;
    r0 = *(u8*)(r3 + r5);
    r8 = r8 + r0;
    r3 = r8 << 1;
    /* subi r0, r3, 0x2 */;
    r0 = *(u16*)(r4 + r0);
    *(u32*)((u8*)r6 + 0x44) = r0;
    goto L_801E619C;
L_801E6190: ;
    r0 = -0x1;
    *(u32*)((u8*)r6 + 0x44) = r0;
    *(u32*)((u8*)r6 + 0x8C) = r0;
L_801E619C: ;
    r3 = *(u32*)lbl_8047B544;
    r5 = r9 + 0x0;
    r9 = r9 + 0x1;
    r0 = *(u8*)(r3 + r5);
    r6 = r6 + 0x4;
    if ((u32)r0 == (u32)0x0) goto L_801E61F0;
    r3 = *(u32*)lbl_8047B54C;
    r0 = r8 << 1;
    r0 = *(u16*)(r3 + r0);
    r0 = r8 - r0;
    *(u32*)((u8*)r6 + 0x8C) = r0;
    r3 = *(u32*)lbl_8047B544;
    r4 = *(u32*)lbl_8047B54C;
    r0 = *(u8*)(r3 + r5);
    r8 = r8 + r0;
    r3 = r8 << 1;
    /* subi r0, r3, 0x2 */;
    r0 = *(u16*)(r4 + r0);
    *(u32*)((u8*)r6 + 0x44) = r0;
    goto L_801E61FC;
L_801E61F0: ;
    r0 = -0x1;
    *(u32*)((u8*)r6 + 0x44) = r0;
    *(u32*)((u8*)r6 + 0x8C) = r0;
L_801E61FC: ;
    r3 = *(u32*)lbl_8047B544;
    r5 = r9 + 0x0;
    r9 = r9 + 0x1;
    r0 = *(u8*)(r3 + r5);
    r6 = r6 + 0x4;
    if ((u32)r0 == (u32)0x0) goto L_801E6250;
    r3 = *(u32*)lbl_8047B54C;
    r0 = r8 << 1;
    r0 = *(u16*)(r3 + r0);
    r0 = r8 - r0;
    *(u32*)((u8*)r6 + 0x8C) = r0;
    r3 = *(u32*)lbl_8047B544;
    r4 = *(u32*)lbl_8047B54C;
    r0 = *(u8*)(r3 + r5);
    r8 = r8 + r0;
    r3 = r8 << 1;
    /* subi r0, r3, 0x2 */;
    r0 = *(u16*)(r4 + r0);
    *(u32*)((u8*)r6 + 0x44) = r0;
    goto L_801E625C;
L_801E6250: ;
    r0 = -0x1;
    *(u32*)((u8*)r6 + 0x44) = r0;
    *(u32*)((u8*)r6 + 0x8C) = r0;
L_801E625C: ;
    r3 = *(u32*)lbl_8047B544;
    r5 = r9 + 0x0;
    r9 = r9 + 0x1;
    r0 = *(u8*)(r3 + r5);
    r6 = r6 + 0x4;
    if ((u32)r0 == (u32)0x0) goto L_801E62B0;
    r3 = *(u32*)lbl_8047B54C;
    r0 = r8 << 1;
    r0 = *(u16*)(r3 + r0);
    r0 = r8 - r0;
    *(u32*)((u8*)r6 + 0x8C) = r0;
    r3 = *(u32*)lbl_8047B544;
    r4 = *(u32*)lbl_8047B54C;
    r0 = *(u8*)(r3 + r5);
    r8 = r8 + r0;
    r3 = r8 << 1;
    /* subi r0, r3, 0x2 */;
    r0 = *(u16*)(r4 + r0);
    *(u32*)((u8*)r6 + 0x44) = r0;
    goto L_801E62BC;
L_801E62B0: ;
    r0 = -0x1;
    *(u32*)((u8*)r6 + 0x44) = r0;
    *(u32*)((u8*)r6 + 0x8C) = r0;
L_801E62BC: ;
    r6 = r6 + 0x4;
    r9 = r9 + 0x1;
    if (--ctr != 0) goto L_801E6144;
    r3 = (0x10 << 16);
    /* subi r0, r3, 0x1 */;
    *(u32*)((u8*)r7 + 0x88) = r0;
    return;
}
#pragma pop

/* 0x801E62D8 | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E62D8(void) {
    extern u8 lbl_8047B5B0[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r3 = *(u32*)lbl_8047B5B0;
    r0 = 0x1;
    *(u8*)((u8*)r3 + 0x6A9) = r0;
    r4 = *(u32*)lbl_8047B5B0;
    r3 = *(u32*)((u8*)r4 + 0x69C);
    r0 = r3 + 0x2;
    *(u32*)((u8*)r4 + 0x69C) = r0;
    r5 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r5 + 0x69C);
    r3 = *(u8*)((u8*)r4 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x1);
    r0 = (r0 & ~0x0000FF00) | (((r3 << 8) | ((u32)r3 >> 24)) & 0x0000FF00);
    *(u16*)((u8*)r5 + 0x6AA) = r0;
    r4 = *(u32*)lbl_8047B5B0;
    r3 = *(u32*)((u8*)r4 + 0x69C);
    r0 = r3 + 0x2;
    *(u32*)((u8*)r4 + 0x69C) = r0;
    r3 = *(u32*)lbl_8047B5B0;
    r0 = *(u16*)((u8*)r3 + 0x6AA);
    *(u16*)((u8*)r3 + 0x6AC) = r0;
    return;
}
#pragma pop

/* 0x801E632C | size: 0x24C | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E632C(void) {
    extern u8 lbl_8047B4A0[];
    extern u8 lbl_8047B4C0[];
    extern u8 lbl_8047B4E0[];
    extern u8 lbl_8047B500[];
    extern u8 lbl_8047B520[];
    extern u8 lbl_8047B540[];
    extern u8 lbl_8047B5B0[];
    u8 sp[0x18];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r3 = *(u32*)lbl_8047B5B0;
    r0 = *(u32*)((u8*)r3 + 0x69C);
    r4 = r3 + 0x6a4;
    r3 = *(u32*)((u8*)r3 + 0x6A4);
    /* clrrwi r5, r0, 2 */;
    r0 = r0 & 0x3;
    if ((u32)r3 == (u32)0x21) goto L_801E6368;
    r0 = 0x3 - r0;
    r0 = r0 << 3;
    r0 = r3 - r0;
    *(u32*)((u8*)r4 + 0x0) = r0;
    goto L_801E6374;
L_801E6368: ;
    r3 = r0 << 3;
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x0) = r0;
L_801E6374: ;
    r4 = *(u32*)lbl_8047B5B0;
    r3 = 0x0;
    r8 = 0x0;
    *(u32*)((u8*)r4 + 0x69C) = r5;
    r0 = *(u32*)((u8*)r5 + 0x0);
    r4 = *(u32*)lbl_8047B5B0;
    *(u32*)((u8*)r4 + 0x6A0) = r0;
L_801E6390: ;
    r4 = *(u32*)lbl_8047B5B0;
    r0 = 0x1;
    r0 = r0 << r3;
    r4 = *(u8*)((u8*)r4 + 0x6A8);
    /* and. r0, r4, r0 */;
    if ((u32)r3 == (u32)0x21) goto L_801E64BC;
    r0 = 0x10;
    ctr_fn = (void(*)(void))r0;
    r7 = r8 + 0x0;
    r12 = 0x0;
L_801E63B8: ;
    r0 = *(u32*)lbl_8047B5B0;
    r5 = 0xff;
    r31 = 0x0;
    r4 = r0 + r12;
    r0 = r4 + 0x300;
    *(u8*)(r8 + r0) = r5;
    goto L_801E6428;
L_801E63D4: ;
    r11 = *(u32*)lbl_8047B5B0;
    r4 = 0x4 - r31;
    r0 = r31 << 2;
    r5 = r8 + r11;
    r10 = r5 + r0;
    r0 = *(u32*)((u8*)r10 + 0x348);
    r9 = (u32)r12 >> r4;
    if ((s32)r9 > (s32)r0) goto L_801E6424;
    r6 = *(u32*)((u8*)r5 + 0x340);
    r5 = r31 + 0x1;
    r4 = *(u32*)((u8*)r10 + 0x390);
    r0 = r11 + 0x300;
    r31 = 0x63;
    r4 = r4 + r6;
    r4 = *(u8*)(r9 + r4);
    *(u8*)(r7 + r0) = r4;
    r4 = *(u32*)lbl_8047B5B0;
    r0 = r4 + 0x320;
    *(u8*)(r7 + r0) = r5;
L_801E6424: ;
    r31 = r31 + 0x1;
L_801E6428: ;
    if ((u32)r31 < (u32)0x5) goto L_801E63D4;
    r0 = *(u32*)lbl_8047B5B0;
    r12 = r12 + 0x1;
    r5 = 0xff;
    r4 = r0 + r12;
    r0 = r4 + 0x300;
    *(u8*)(r8 + r0) = r5;
    r31 = 0x0;
    r7 = r7 + 0x1;
    goto L_801E64A8;
L_801E6454: ;
    r11 = *(u32*)lbl_8047B5B0;
    r4 = 0x4 - r31;
    r0 = r31 << 2;
    r5 = r8 + r11;
    r10 = r5 + r0;
    r0 = *(u32*)((u8*)r10 + 0x348);
    r9 = (u32)r12 >> r4;
    if ((s32)r9 > (s32)r0) goto L_801E64A4;
    r6 = *(u32*)((u8*)r5 + 0x340);
    r5 = r31 + 0x1;
    r4 = *(u32*)((u8*)r10 + 0x390);
    r0 = r11 + 0x300;
    r31 = 0x63;
    r4 = r4 + r6;
    r4 = *(u8*)(r9 + r4);
    *(u8*)(r7 + r0) = r4;
    r4 = *(u32*)lbl_8047B5B0;
    r0 = r4 + 0x320;
    *(u8*)(r7 + r0) = r5;
L_801E64A4: ;
    r31 = r31 + 0x1;
L_801E64A8: ;
    if ((u32)r31 < (u32)0x5) goto L_801E6454;
    r7 = r7 + 0x1;
    r12 = r12 + 0x1;
    if (--ctr != 0) goto L_801E63B8;
L_801E64BC: ;
    r3 = r3 + 0x1;
    r8 = r8 + 0xe0;
    if ((u32)r3 < (u32)0x4) goto L_801E6390;
    r9 = *(u32*)lbl_8047B5B0;
    r4 = *(u8*)((u8*)r9 + 0x682);
    r0 = *(u8*)((u8*)r9 + 0x688);
    r3 = *(u8*)((u8*)r9 + 0x68E);
    r5 = r4 << 1;
    r7 = *(u8*)((u8*)r9 + 0x687);
    r4 = r0 << 1;
    r6 = *(u8*)((u8*)r9 + 0x68D);
    r0 = *(u8*)((u8*)r9 + 0x681);
    r3 = r3 << 1;
    r7 = r7 << 1;
    r6 = r6 << 1;
    r5 = r5 + 0x1;
    r4 = r4 + 0x1;
    r3 = r3 + 0x1;
    r0 = r0 << 1;
    r8 = r0 * 0xe0;
    r7 = r7 * 0xe0;
    r6 = r6 * 0xe0;
    r5 = r5 * 0xe0;
    r4 = r4 * 0xe0;
    r3 = r3 * 0xe0;
    r8 = r8 + 0x300;
    r7 = r7 + 0x300;
    r6 = r6 + 0x300;
    r5 = r5 + 0x300;
    r4 = r4 + 0x300;
    r0 = r3 + 0x300;
    r8 = r9 + r8;
    r3 = r9 + r7;
    *(u32*)lbl_8047B4A0 = r8;
    r6 = r9 + r6;
    r5 = r9 + r5;
    *(u32*)lbl_8047B4C0 = r3;
    r3 = r9 + r4;
    r0 = r9 + r0;
    *(u32*)lbl_8047B4E0 = r6;
    *(u32*)lbl_8047B500 = r5;
    *(u32*)lbl_8047B520 = r3;
    *(u32*)lbl_8047B540 = r0;
    r31 = *(u32*)(sp + 0x14);
    return;
}
#pragma pop

/* 0x801E6578 | size: 0x10C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E6578(void) {
    extern u8 lbl_8047B5A4[];
    extern u8 lbl_8047B5A8[];
    extern u8 lbl_8047B5B0[];
    extern void fn_801E632C();
    extern void fn_801E6684();
    extern void fn_801E810C();
    extern void fn_801E9B98();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r6 = *(u32*)lbl_8047B5B0;
    *(u32*)((u8*)r6 + 0x6B0) = r3;
    r6 = *(u32*)lbl_8047B5B0;
    *(u32*)((u8*)r6 + 0x6B4) = r4;
    r4 = *(u32*)lbl_8047B5B0;
    *(u32*)((u8*)r4 + 0x6B8) = r5;
    r4 = *(u32*)lbl_8047B5B0;
    r31 = *(u16*)((u8*)r4 + 0x698);
    r30 = *(u16*)((u8*)r4 + 0x694);
    r4 = 0; /* mfspr GQR5 */;
    r0 = 0; /* mfspr GQR6 */;
    *(u32*)lbl_8047B5A4 = r4;
    *(u32*)lbl_8047B5A8 = r0;
    r3 = 0x7;
    r3 = r3 | (0x7 << 16);
    /* mtspr GQR5, r3 */;
    r3 = 0x3d04;
    r3 = r3 | (0x3d04 << 16);
    /* mtspr GQR6, r3 */;
    fn_801E632C();
    r4 = *(u32*)lbl_8047B5B0;
    r0 = *(u16*)((u8*)r4 + 0x692);
    if ((u32)r0 != (u32)0x200) goto L_801E6610;
    if ((u32)r30 != (u32)0x1c0) goto L_801E6610;
    goto L_801E6600;
L_801E65F8: ;
    fn_801E6684();
    r31 = r31 + 0x10;
L_801E6600: ;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 < (u32)r30) goto L_801E65F8;
    goto L_801E665C;
L_801E6610: ;
    r4 = *(u32*)lbl_8047B5B0;
    r0 = *(u16*)((u8*)r4 + 0x692);
    if ((u32)r0 != (u32)0x280) goto L_801E6650;
    if ((u32)r30 != (u32)0x1e0) goto L_801E6650;
    goto L_801E6634;
L_801E662C: ;
    fn_801E810C();
    r31 = r31 + 0x10;
L_801E6634: ;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 < (u32)r30) goto L_801E662C;
    goto L_801E665C;
    goto L_801E6650;
L_801E6648: ;
    fn_801E9B98();
    r31 = r31 + 0x10;
L_801E6650: ;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 < (u32)r30) goto L_801E6648;
L_801E665C: ;
    r4 = *(u32*)lbl_8047B5A4;
    r0 = *(u32*)lbl_8047B5A8;
    /* mtspr GQR5, r4 */;
    /* mtspr GQR6, r0 */;
    return;
}
#pragma pop

/* 0x801E6684 | size: 0x1A88 | massive */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E6684(void) {
    extern u8 lbl_8046D500[];
    extern u8 lbl_8047B560[];
    extern u8 lbl_8047B580[];
    extern u8 lbl_8047B5A0[];
    extern u8 lbl_8047B5B0[];
    extern u8 lbl_8047E4B8[];
    extern u8 lbl_8047E4BC[];
    extern u8 lbl_8047E4C0[];
    extern u8 lbl_8047E4C4[];
    extern u8 lbl_8047E4C8[];
    extern void fn_8009B55C();
    extern void fn_8009B614();
    extern void fn_801EB644();
    extern void fn_801EBCC0();
    extern void fn_801EC368();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    f32 f9 = 0.0f;
    f32 f10 = 0.0f;
    f32 f11 = 0.0f;
    f32 f12 = 0.0f;
    f32 f13 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;
    u8 sp[0x100];

    r3 = (u32)lbl_8046D500;
    *(f64*)(sp + 0x30) = f31;
    *(f64*)(sp + 0x28) = f30;
    *(f64*)(sp + 0x20) = f29;
    *(f64*)(sp + 0x18) = f28;
    *(f64*)(sp + 0x10) = f27;
    r31 = (u32)lbl_8046D500;
    r3 = 0x3;
    fn_8009B614();
    f27 = *(f32*)lbl_8047E4B8;
    r30 = 0x0;
    f28 = *(f32*)lbl_8047E4BC;
    f29 = *(f32*)lbl_8047E4C0;
    f30 = *(f32*)lbl_8047E4C4;
    f31 = *(f32*)lbl_8047E4C8;
    goto L_801E8064;
L_801E66D8: ;
    r3 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r31 + 0x118);
    fn_801EB644();
    r3 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r31 + 0x11C);
    fn_801EB644();
    r3 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r31 + 0x120);
    fn_801EB644();
    r3 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r31 + 0x124);
    fn_801EB644();
    r3 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r31 + 0x128);
    fn_801EBCC0();
    r3 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r31 + 0x12C);
    fn_801EC368();
    r3 = *(u32*)((u8*)r31 + 0x100);
    r0 = 0x200;
    r4 = *(u32*)lbl_8047B5B0;
    /* subi r9, r31, 0x8 */;
    *(u32*)lbl_8047B560 = r3;
    /* clrlslwi r3, r30, 24, 4 */;
    *(u32*)lbl_8047B580 = r0;
    r0 = *(u8*)((u8*)r4 + 0x680);
    r0 = r0 << 8;
    r0 = r4 + r0;
    *(u32*)lbl_8047B5A0 = r0;
    r8 = *(u32*)((u8*)r31 + 0x118);
    r7 = *(u32*)lbl_8047B5A0;
    r4 = 0x8;
    ctr_fn = (void(*)(void))r4;
L_801E675C: ;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_mul f7, f7, f6 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* or. r6, r6, r0 */;
L_801E677C: ;
    if ((s32)r6 != (s32)0x0) goto L_801E68C8;
    /* ps_merge00 f4, f7, f7 */;
    /* psq_st f4, 0x8(r9), 0, qr0 */;
    if ((s32)r5 != (s32)0x0) goto L_801E682C;
    /* psq_st f4, 0x10(r9), 0, qr0 */;
    /* psq_st f4, 0x18(r9), 0, qr0 */;
    if ((s32)r4 != (s32)0x0) goto L_801E67B8;
    r7 = r7 + 0x20;
    /* psq_stu f4, 0x20(r9), 0, qr0 */;
    r8 = r8 + 0x10;
    if (--ctr != 0) goto L_801E675C;
    goto L_801E6990;
L_801E67B8: ;
    r8 = r8 + 0x10;
    /* ps_msub f13, f7, f28, f7 */;
    r7 = r7 + 0x20;
    /* ps_merge00 f2, f7, f7 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f1, f28, f29 */;
    /* ps_msub f12, f7, f27, f13 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f10, f7, f13 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_nmsub f11, f7, f1, f12 */;
    /* ps_add f8, f2, f10 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_merge11 f9, f12, f11 */;
    /* ps_sub f10, f2, f10 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_add f3, f2, f9 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_sub f9, f2, f9 */;
    /* psq_stu f8, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f9, f9, f9 */;
    /* psq_stu f3, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f10, f10, f10 */;
    r6 = r6 | r0;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E677C;
    goto L_801E6990;
L_801E682C: ;
    /* psq_l f1, 0x4(r8), 0, qr5 */;
    /* psq_l f2, 0x8(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_mul f1, f1, f2 */;
    r7 = r7 + 0x20;
    /* ps_sub f12, f7, f1 */;
    /* ps_add f13, f7, f1 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_madd f11, f1, f27, f12 */;
    /* ps_nmsub f10, f1, f27, f13 */;
    /* ps_mul f3, f12, f28 */;
    /* ps_merge00 f11, f13, f11 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_nmsub f9, f1, f30, f3 */;
    /* ps_merge00 f10, f10, f12 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f13 */;
    /* ps_nmsub f8, f7, f29, f3 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f13, f13, f9 */;
    /* ps_msub f3, f12, f27, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_add f2, f11, f13 */;
    /* ps_sub f8, f8, f3 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f12, f3, f8 */;
    /* ps_sub f11, f11, f13 */;
    /* psq_stu f2, 0x8(r9), 0, qr0 */;
    /* ps_add f4, f10, f12 */;
    /* ps_sub f1, f10, f12 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f1, f1, f1 */;
    /* ps_merge10 f11, f11, f11 */;
    /* psq_stu f1, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f11, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E677C;
    goto L_801E6990;
L_801E68C8: ;
    /* psq_l f2, 0x4(r8), 0, qr5 */;
    /* psq_l f10, 0x8(r7), 0, qr0 */;
    /* ps_mul f2, f2, f10 */;
    /* psq_l f13, 0x8(r8), 0, qr5 */;
    /* psq_l f9, 0x10(r7), 0, qr0 */;
    /* ps_merge01 f4, f7, f2 */;
    /* psq_l f12, 0xc(r8), 0, qr5 */;
    /* ps_merge01 f1, f2, f7 */;
    /* psq_l f8, 0x18(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_madd f11, f13, f9, f4 */;
    /* ps_nmsub f10, f13, f9, f4 */;
    /* ps_madd f9, f12, f8, f1 */;
    /* ps_nmsub f8, f12, f8, f1 */;
    r7 = r7 + 0x20;
    /* ps_add f4, f11, f9 */;
    /* ps_sub f12, f11, f9 */;
    /* ps_msub f13, f8, f27, f9 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_sub f3, f8, f10 */;
    /* ps_add f1, f10, f13 */;
    /* ps_sub f13, f10, f13 */;
    /* ps_mul f3, f3, f28 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_merge00 f1, f4, f1 */;
    /* ps_nmsub f9, f10, f30, f3 */;
    /* ps_msub f11, f8, f29, f3 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f4 */;
    /* ps_merge00 f13, f13, f12 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_madd f10, f12, f27, f9 */;
    /* ps_merge11 f8, f4, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_sub f11, f11, f10 */;
    /* ps_add f12, f1, f8 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f11, f10, f11 */;
    /* ps_sub f4, f1, f8 */;
    /* ps_mul f7, f7, f6 */;
    /* ps_add f10, f13, f11 */;
    /* ps_sub f9, f13, f11 */;
    /* ps_merge10 f10, f10, f10 */;
    /* psq_stu f12, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f4, f4, f4 */;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E677C;
L_801E6990: ;
    r8 = *(u32*)lbl_8047B560;
    r9 = r31;
    r0 = *(u32*)lbl_8047B580;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    r4 = r3 << 2;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    r6 = r0 << 2;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    r7 = r4;
    /* ps_add f9, f7, f6 */;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f3, f7, f6 */;
    r6 = r7 + r6;
    /* ps_add f9, f9, f31 */;
    r0 = 0x3;
    /* ps_add f8, f5, f4 */;
    r5 = r8 + r7;
    /* ps_sub f2, f5, f4 */;
    /* ps_add f4, f9, f8 */;
    r4 = r8 + r6;
    /* ps_add f3, f3, f31 */;
    ctr_fn = (void(*)(void))r0;
L_801E69E8: ;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    r9 = r9 + 0x8;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f9, f9, f8 */;
    r7 = r7 + 0x2;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    r6 = r6 + 0x2;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* ps_add f9, f7, f6 */;
    /* psq_st f3, 0x0(r4), 0, qr6 */;
    /* ps_sub f3, f7, f6 */;
    /* ps_add f9, f9, f31 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    /* ps_add f8, f5, f4 */;
    /* ps_sub f2, f5, f4 */;
    /* psq_st f1, 0x10(r4), 0, qr6 */;
    r5 = r8 + r7;
    /* ps_add f4, f9, f8 */;
    /* psq_st f0, 0x18(r4), 0, qr6 */;
    /* ps_add f3, f3, f31 */;
    r4 = r8 + r6;
    if (--ctr != 0) goto L_801E69E8;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_sub f9, f9, f8 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* psq_st f0, 0x18(r4), 0, qr6 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f1, 0x10(r4), 0, qr6 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* psq_st f3, 0x0(r4), 0, qr6 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    r8 = *(u32*)((u8*)r31 + 0x11C);
    r7 = *(u32*)lbl_8047B5A0;
    r10 = r3 + 0x8;
    /* subi r9, r31, 0x8 */;
    r4 = 0x8;
    ctr_fn = (void(*)(void))r4;
L_801E6B6C: ;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_mul f7, f7, f6 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* or. r6, r6, r0 */;
L_801E6B8C: ;
    if ((s32)r6 != (s32)0x0) goto L_801E6CD8;
    /* ps_merge00 f4, f7, f7 */;
    /* psq_st f4, 0x8(r9), 0, qr0 */;
    if ((s32)r5 != (s32)0x0) goto L_801E6C3C;
    /* psq_st f4, 0x10(r9), 0, qr0 */;
    /* psq_st f4, 0x18(r9), 0, qr0 */;
    if ((s32)r4 != (s32)0x0) goto L_801E6BC8;
    r7 = r7 + 0x20;
    /* psq_stu f4, 0x20(r9), 0, qr0 */;
    r8 = r8 + 0x10;
    if (--ctr != 0) goto L_801E6B6C;
    goto L_801E6DA0;
L_801E6BC8: ;
    r8 = r8 + 0x10;
    /* ps_msub f13, f7, f28, f7 */;
    r7 = r7 + 0x20;
    /* ps_merge00 f2, f7, f7 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f1, f28, f29 */;
    /* ps_msub f12, f7, f27, f13 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f10, f7, f13 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_nmsub f11, f7, f1, f12 */;
    /* ps_add f8, f2, f10 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_merge11 f9, f12, f11 */;
    /* ps_sub f10, f2, f10 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_add f3, f2, f9 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_sub f9, f2, f9 */;
    /* psq_stu f8, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f9, f9, f9 */;
    /* psq_stu f3, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f10, f10, f10 */;
    r6 = r6 | r0;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E6B8C;
    goto L_801E6DA0;
L_801E6C3C: ;
    /* psq_l f1, 0x4(r8), 0, qr5 */;
    /* psq_l f2, 0x8(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_mul f1, f1, f2 */;
    r7 = r7 + 0x20;
    /* ps_sub f12, f7, f1 */;
    /* ps_add f13, f7, f1 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_madd f11, f1, f27, f12 */;
    /* ps_nmsub f10, f1, f27, f13 */;
    /* ps_mul f3, f12, f28 */;
    /* ps_merge00 f11, f13, f11 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_nmsub f9, f1, f30, f3 */;
    /* ps_merge00 f10, f10, f12 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f13 */;
    /* ps_nmsub f8, f7, f29, f3 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f13, f13, f9 */;
    /* ps_msub f3, f12, f27, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_add f2, f11, f13 */;
    /* ps_sub f8, f8, f3 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f12, f3, f8 */;
    /* ps_sub f11, f11, f13 */;
    /* psq_stu f2, 0x8(r9), 0, qr0 */;
    /* ps_add f4, f10, f12 */;
    /* ps_sub f1, f10, f12 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f1, f1, f1 */;
    /* ps_merge10 f11, f11, f11 */;
    /* psq_stu f1, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f11, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E6B8C;
    goto L_801E6DA0;
L_801E6CD8: ;
    /* psq_l f2, 0x4(r8), 0, qr5 */;
    /* psq_l f10, 0x8(r7), 0, qr0 */;
    /* ps_mul f2, f2, f10 */;
    /* psq_l f13, 0x8(r8), 0, qr5 */;
    /* psq_l f9, 0x10(r7), 0, qr0 */;
    /* ps_merge01 f4, f7, f2 */;
    /* psq_l f12, 0xc(r8), 0, qr5 */;
    /* ps_merge01 f1, f2, f7 */;
    /* psq_l f8, 0x18(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_madd f11, f13, f9, f4 */;
    /* ps_nmsub f10, f13, f9, f4 */;
    /* ps_madd f9, f12, f8, f1 */;
    /* ps_nmsub f8, f12, f8, f1 */;
    r7 = r7 + 0x20;
    /* ps_add f4, f11, f9 */;
    /* ps_sub f12, f11, f9 */;
    /* ps_msub f13, f8, f27, f9 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_sub f3, f8, f10 */;
    /* ps_add f1, f10, f13 */;
    /* ps_sub f13, f10, f13 */;
    /* ps_mul f3, f3, f28 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_merge00 f1, f4, f1 */;
    /* ps_nmsub f9, f10, f30, f3 */;
    /* ps_msub f11, f8, f29, f3 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f4 */;
    /* ps_merge00 f13, f13, f12 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_madd f10, f12, f27, f9 */;
    /* ps_merge11 f8, f4, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_sub f11, f11, f10 */;
    /* ps_add f12, f1, f8 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f11, f10, f11 */;
    /* ps_sub f4, f1, f8 */;
    /* ps_mul f7, f7, f6 */;
    /* ps_add f10, f13, f11 */;
    /* ps_sub f9, f13, f11 */;
    /* ps_merge10 f10, f10, f10 */;
    /* psq_stu f12, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f4, f4, f4 */;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E6B8C;
L_801E6DA0: ;
    r8 = *(u32*)lbl_8047B560;
    r9 = r31;
    r0 = *(u32*)lbl_8047B580;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    r10 = r10 << 2;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    r6 = r0 << 2;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    r7 = r10;
    /* ps_add f9, f7, f6 */;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f3, f7, f6 */;
    r6 = r7 + r6;
    /* ps_add f9, f9, f31 */;
    r0 = 0x3;
    /* ps_add f8, f5, f4 */;
    r5 = r8 + r7;
    /* ps_sub f2, f5, f4 */;
    /* ps_add f4, f9, f8 */;
    r4 = r8 + r6;
    /* ps_add f3, f3, f31 */;
    ctr_fn = (void(*)(void))r0;
L_801E6DF8: ;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    r9 = r9 + 0x8;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f9, f9, f8 */;
    r7 = r7 + 0x2;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    r6 = r6 + 0x2;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* ps_add f9, f7, f6 */;
    /* psq_st f3, 0x0(r4), 0, qr6 */;
    /* ps_sub f3, f7, f6 */;
    /* ps_add f9, f9, f31 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    /* ps_add f8, f5, f4 */;
    /* ps_sub f2, f5, f4 */;
    /* psq_st f1, 0x10(r4), 0, qr6 */;
    r5 = r8 + r7;
    /* ps_add f4, f9, f8 */;
    /* psq_st f0, 0x18(r4), 0, qr6 */;
    /* ps_add f3, f3, f31 */;
    r4 = r8 + r6;
    if (--ctr != 0) goto L_801E6DF8;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_sub f9, f9, f8 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* psq_st f0, 0x18(r4), 0, qr6 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f1, 0x10(r4), 0, qr6 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* psq_st f3, 0x0(r4), 0, qr6 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    r8 = *(u32*)((u8*)r31 + 0x120);
    r7 = *(u32*)lbl_8047B5A0;
    /* subi r9, r31, 0x8 */;
    r4 = 0x8;
    ctr_fn = (void(*)(void))r4;
L_801E6F78: ;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_mul f7, f7, f6 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    r4 = *(u16*)((u8*)r8 + 0x2);
    r6 = r6 | r0;
L_801E6F98: ;
    if ((s32)r6 != (s32)0x0) goto L_801E70E4;
    /* ps_merge00 f4, f7, f7 */;
    /* psq_st f4, 0x8(r9), 0, qr0 */;
    if ((s32)r5 != (s32)0x0) goto L_801E7048;
    /* psq_st f4, 0x10(r9), 0, qr0 */;
    /* psq_st f4, 0x18(r9), 0, qr0 */;
    if ((s32)r4 != (s32)0x0) goto L_801E6FD4;
    r7 = r7 + 0x20;
    /* psq_stu f4, 0x20(r9), 0, qr0 */;
    r8 = r8 + 0x10;
    if (--ctr != 0) goto L_801E6F78;
    goto L_801E71AC;
L_801E6FD4: ;
    /* ps_msub f13, f7, f28, f7 */;
    r8 = r8 + 0x10;
    /* ps_merge00 f2, f7, f7 */;
    r7 = r7 + 0x20;
    /* ps_sub f1, f28, f29 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_msub f12, f7, f27, f13 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f10, f7, f13 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_nmsub f11, f7, f1, f12 */;
    /* ps_add f8, f2, f10 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_merge11 f9, f12, f11 */;
    /* ps_sub f10, f2, f10 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_add f3, f2, f9 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_sub f9, f2, f9 */;
    /* psq_stu f8, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f9, f9, f9 */;
    /* psq_stu f3, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f10, f10, f10 */;
    r6 = r6 | r0;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E6F98;
    goto L_801E71AC;
L_801E7048: ;
    /* psq_l f1, 0x4(r8), 0, qr5 */;
    /* psq_l f2, 0x8(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_mul f1, f1, f2 */;
    r7 = r7 + 0x20;
    /* ps_sub f12, f7, f1 */;
    /* ps_add f13, f7, f1 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_madd f11, f1, f27, f12 */;
    /* ps_nmsub f10, f1, f27, f13 */;
    /* ps_mul f3, f12, f28 */;
    /* ps_merge00 f11, f13, f11 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_nmsub f9, f1, f30, f3 */;
    /* ps_merge00 f10, f10, f12 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f13 */;
    /* ps_nmsub f8, f7, f29, f3 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f13, f13, f9 */;
    /* ps_msub f3, f12, f27, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_add f2, f11, f13 */;
    /* ps_sub f8, f8, f3 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f12, f3, f8 */;
    /* ps_sub f11, f11, f13 */;
    /* psq_stu f2, 0x8(r9), 0, qr0 */;
    /* ps_add f4, f10, f12 */;
    /* ps_sub f1, f10, f12 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f1, f1, f1 */;
    /* ps_merge10 f11, f11, f11 */;
    /* psq_stu f1, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f11, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E6F98;
    goto L_801E71AC;
L_801E70E4: ;
    /* psq_l f2, 0x4(r8), 0, qr5 */;
    /* psq_l f10, 0x8(r7), 0, qr0 */;
    /* ps_mul f2, f2, f10 */;
    /* psq_l f13, 0x8(r8), 0, qr5 */;
    /* psq_l f9, 0x10(r7), 0, qr0 */;
    /* ps_merge01 f4, f7, f2 */;
    /* psq_l f12, 0xc(r8), 0, qr5 */;
    /* ps_merge01 f1, f2, f7 */;
    /* psq_l f8, 0x18(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_madd f11, f13, f9, f4 */;
    /* ps_nmsub f10, f13, f9, f4 */;
    /* ps_madd f9, f12, f8, f1 */;
    /* ps_nmsub f8, f12, f8, f1 */;
    r7 = r7 + 0x20;
    /* ps_add f4, f11, f9 */;
    /* ps_sub f12, f11, f9 */;
    /* ps_msub f13, f8, f27, f9 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_sub f3, f8, f10 */;
    /* ps_add f1, f10, f13 */;
    /* ps_sub f13, f10, f13 */;
    /* ps_mul f3, f3, f28 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_merge00 f1, f4, f1 */;
    /* ps_nmsub f9, f10, f30, f3 */;
    /* ps_msub f11, f8, f29, f3 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f4 */;
    /* ps_merge00 f13, f13, f12 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_madd f10, f12, f27, f9 */;
    /* ps_merge11 f8, f4, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_sub f11, f11, f10 */;
    /* ps_add f12, f1, f8 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f11, f10, f11 */;
    /* ps_sub f4, f1, f8 */;
    /* ps_mul f7, f7, f6 */;
    /* ps_add f10, f13, f11 */;
    /* ps_sub f9, f13, f11 */;
    /* ps_merge10 f10, f10, f10 */;
    /* psq_stu f12, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f4, f4, f4 */;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E6F98;
L_801E71AC: ;
    r8 = *(u32*)lbl_8047B560;
    r9 = r31;
    r0 = *(u32*)lbl_8047B580;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    r7 = r0 << 3;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    r4 = r3 << 2;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    r6 = r0 << 2;
    /* ps_add f9, f7, f6 */;
    r7 = r7 + r4;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f3, f7, f6 */;
    r6 = r7 + r6;
    /* ps_add f9, f9, f31 */;
    r0 = 0x3;
    /* ps_add f8, f5, f4 */;
    r5 = r8 + r7;
    /* ps_sub f2, f5, f4 */;
    /* ps_add f4, f9, f8 */;
    r4 = r8 + r6;
    /* ps_add f3, f3, f31 */;
    ctr_fn = (void(*)(void))r0;
L_801E7208: ;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    r9 = r9 + 0x8;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f9, f9, f8 */;
    r7 = r7 + 0x2;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    r6 = r6 + 0x2;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* ps_add f9, f7, f6 */;
    /* psq_st f3, 0x0(r4), 0, qr6 */;
    /* ps_sub f3, f7, f6 */;
    /* ps_add f9, f9, f31 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    /* ps_add f8, f5, f4 */;
    /* ps_sub f2, f5, f4 */;
    /* psq_st f1, 0x10(r4), 0, qr6 */;
    r5 = r8 + r7;
    /* ps_add f4, f9, f8 */;
    /* psq_st f0, 0x18(r4), 0, qr6 */;
    /* ps_add f3, f3, f31 */;
    r4 = r8 + r6;
    if (--ctr != 0) goto L_801E7208;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_sub f9, f9, f8 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* psq_st f0, 0x18(r4), 0, qr6 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    /* ps_sub f2, f12, f11 */;
    /* psq_st f1, 0x10(r4), 0, qr6 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* psq_st f3, 0x0(r4), 0, qr6 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    r8 = *(u32*)((u8*)r31 + 0x124);
    r7 = *(u32*)lbl_8047B5A0;
    r9 = r3 + 0x8;
    /* subi r10, r31, 0x8 */;
    r4 = 0x8;
    ctr_fn = (void(*)(void))r4;
L_801E738C: ;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_mul f7, f7, f6 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    r4 = *(u16*)((u8*)r8 + 0x2);
    r6 = r6 | r0;
L_801E73AC: ;
    if ((s32)r6 != (s32)0x0) goto L_801E74F8;
    /* ps_merge00 f4, f7, f7 */;
    /* psq_st f4, 0x8(r10), 0, qr0 */;
    if ((s32)r5 != (s32)0x0) goto L_801E745C;
    /* psq_st f4, 0x10(r10), 0, qr0 */;
    /* psq_st f4, 0x18(r10), 0, qr0 */;
    if ((s32)r4 != (s32)0x0) goto L_801E73E8;
    r7 = r7 + 0x20;
    /* psq_stu f4, 0x20(r10), 0, qr0 */;
    r8 = r8 + 0x10;
    if (--ctr != 0) goto L_801E738C;
    goto L_801E75C0;
L_801E73E8: ;
    /* ps_msub f13, f7, f28, f7 */;
    r8 = r8 + 0x10;
    /* ps_merge00 f2, f7, f7 */;
    r7 = r7 + 0x20;
    /* ps_sub f1, f28, f29 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_msub f12, f7, f27, f13 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f10, f7, f13 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_nmsub f11, f7, f1, f12 */;
    /* ps_add f8, f2, f10 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_merge11 f9, f12, f11 */;
    /* ps_sub f10, f2, f10 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_add f3, f2, f9 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_sub f9, f2, f9 */;
    /* psq_stu f8, 0x8(r10), 0, qr0 */;
    /* ps_merge10 f9, f9, f9 */;
    /* psq_stu f3, 0x8(r10), 0, qr0 */;
    /* ps_merge10 f10, f10, f10 */;
    r6 = r6 | r0;
    /* psq_stu f9, 0x8(r10), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f10, 0x8(r10), 0, qr0 */;
    if (--ctr != 0) goto L_801E73AC;
    goto L_801E75C0;
L_801E745C: ;
    /* psq_l f1, 0x4(r8), 0, qr5 */;
    /* psq_l f2, 0x8(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_mul f1, f1, f2 */;
    r7 = r7 + 0x20;
    /* ps_sub f12, f7, f1 */;
    /* ps_add f13, f7, f1 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_madd f11, f1, f27, f12 */;
    /* ps_nmsub f10, f1, f27, f13 */;
    /* ps_mul f3, f12, f28 */;
    /* ps_merge00 f11, f13, f11 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_nmsub f9, f1, f30, f3 */;
    /* ps_merge00 f10, f10, f12 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f13 */;
    /* ps_nmsub f8, f7, f29, f3 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f13, f13, f9 */;
    /* ps_msub f3, f12, f27, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_add f2, f11, f13 */;
    /* ps_sub f8, f8, f3 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f12, f3, f8 */;
    /* ps_sub f11, f11, f13 */;
    /* psq_stu f2, 0x8(r10), 0, qr0 */;
    /* ps_add f4, f10, f12 */;
    /* ps_sub f1, f10, f12 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r10), 0, qr0 */;
    /* ps_merge10 f1, f1, f1 */;
    /* ps_merge10 f11, f11, f11 */;
    /* psq_stu f1, 0x8(r10), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f11, 0x8(r10), 0, qr0 */;
    if (--ctr != 0) goto L_801E73AC;
    goto L_801E75C0;
L_801E74F8: ;
    /* psq_l f2, 0x4(r8), 0, qr5 */;
    /* psq_l f10, 0x8(r7), 0, qr0 */;
    /* ps_mul f2, f2, f10 */;
    /* psq_l f13, 0x8(r8), 0, qr5 */;
    /* psq_l f9, 0x10(r7), 0, qr0 */;
    /* ps_merge01 f4, f7, f2 */;
    /* psq_l f12, 0xc(r8), 0, qr5 */;
    /* ps_merge01 f1, f2, f7 */;
    /* psq_l f8, 0x18(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_madd f11, f13, f9, f4 */;
    /* ps_nmsub f10, f13, f9, f4 */;
    /* ps_madd f9, f12, f8, f1 */;
    /* ps_nmsub f8, f12, f8, f1 */;
    r7 = r7 + 0x20;
    /* ps_add f4, f11, f9 */;
    /* ps_sub f12, f11, f9 */;
    /* ps_msub f13, f8, f27, f9 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_sub f3, f8, f10 */;
    /* ps_add f1, f10, f13 */;
    /* ps_sub f13, f10, f13 */;
    /* ps_mul f3, f3, f28 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_merge00 f1, f4, f1 */;
    /* ps_nmsub f9, f10, f30, f3 */;
    /* ps_msub f11, f8, f29, f3 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f4 */;
    /* ps_merge00 f13, f13, f12 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_madd f10, f12, f27, f9 */;
    /* ps_merge11 f8, f4, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_sub f11, f11, f10 */;
    /* ps_add f12, f1, f8 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f11, f10, f11 */;
    /* ps_sub f4, f1, f8 */;
    /* ps_mul f7, f7, f6 */;
    /* ps_add f10, f13, f11 */;
    /* ps_sub f9, f13, f11 */;
    /* ps_merge10 f10, f10, f10 */;
    /* psq_stu f12, 0x8(r10), 0, qr0 */;
    /* ps_merge10 f4, f4, f4 */;
    /* psq_stu f9, 0x8(r10), 0, qr0 */;
    /* psq_stu f10, 0x8(r10), 0, qr0 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r10), 0, qr0 */;
    if (--ctr != 0) goto L_801E73AC;
L_801E75C0: ;
    r8 = *(u32*)lbl_8047B560;
    r10 = r31;
    r0 = *(u32*)lbl_8047B580;
    /* psq_l f7, 0x0(r10), 0, qr0 */;
    r7 = r0 << 3;
    /* psq_l f6, 0x80(r10), 0, qr0 */;
    r9 = r9 << 2;
    /* psq_l f5, 0x40(r10), 0, qr0 */;
    r6 = r0 << 2;
    /* ps_add f9, f7, f6 */;
    r7 = r7 + r9;
    /* psq_l f4, 0xc0(r10), 0, qr0 */;
    /* ps_sub f3, f7, f6 */;
    r6 = r7 + r6;
    /* ps_add f9, f9, f31 */;
    r0 = 0x3;
    /* ps_add f8, f5, f4 */;
    r5 = r8 + r7;
    /* ps_sub f2, f5, f4 */;
    /* ps_add f4, f9, f8 */;
    r4 = r8 + r6;
    /* ps_add f3, f3, f31 */;
    ctr_fn = (void(*)(void))r0;
L_801E761C: ;
    /* psq_l f11, 0x20(r10), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r10), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r10), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r10), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    r10 = r10 + 0x8;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* psq_l f7, 0x0(r10), 0, qr0 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* psq_l f6, 0x80(r10), 0, qr0 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* psq_l f5, 0x40(r10), 0, qr0 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_l f4, 0xc0(r10), 0, qr0 */;
    /* ps_sub f9, f9, f8 */;
    r7 = r7 + 0x2;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    r6 = r6 + 0x2;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* ps_add f9, f7, f6 */;
    /* psq_st f3, 0x0(r4), 0, qr6 */;
    /* ps_sub f3, f7, f6 */;
    /* ps_add f9, f9, f31 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    /* ps_add f8, f5, f4 */;
    /* ps_sub f2, f5, f4 */;
    /* psq_st f1, 0x10(r4), 0, qr6 */;
    r5 = r8 + r7;
    /* ps_add f4, f9, f8 */;
    /* psq_st f0, 0x18(r4), 0, qr6 */;
    /* ps_add f3, f3, f31 */;
    r4 = r8 + r6;
    if (--ctr != 0) goto L_801E761C;
    /* psq_l f11, 0x20(r10), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r10), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r10), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r10), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_sub f9, f9, f8 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* psq_st f0, 0x18(r4), 0, qr6 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    /* ps_sub f2, f12, f11 */;
    /* psq_st f1, 0x10(r4), 0, qr6 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* psq_st f3, 0x0(r4), 0, qr6 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    r4 = *(u32*)((u8*)r31 + 0x104);
    r0 = 0x100;
    r5 = *(u32*)lbl_8047B5B0;
    *(u32*)lbl_8047B560 = r4;
    r3 = (u32)r3 >> 1;
    /* subi r9, r31, 0x8 */;
    *(u32*)lbl_8047B580 = r0;
    r0 = *(u8*)((u8*)r5 + 0x686);
    r0 = r0 << 8;
    r0 = r5 + r0;
    *(u32*)lbl_8047B5A0 = r0;
    r8 = *(u32*)((u8*)r31 + 0x128);
    r7 = *(u32*)lbl_8047B5A0;
    r4 = 0x8;
    ctr_fn = (void(*)(void))r4;
L_801E77C4: ;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_mul f7, f7, f6 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* or. r6, r6, r0 */;
L_801E77E4: ;
    if ((s32)r6 != (s32)0x0) goto L_801E7930;
    /* ps_merge00 f4, f7, f7 */;
    /* psq_st f4, 0x8(r9), 0, qr0 */;
    if ((s32)r5 != (s32)0x0) goto L_801E7894;
    /* psq_st f4, 0x10(r9), 0, qr0 */;
    /* psq_st f4, 0x18(r9), 0, qr0 */;
    if ((s32)r4 != (s32)0x0) goto L_801E7820;
    r7 = r7 + 0x20;
    /* psq_stu f4, 0x20(r9), 0, qr0 */;
    r8 = r8 + 0x10;
    if (--ctr != 0) goto L_801E77C4;
    goto L_801E79F8;
L_801E7820: ;
    r8 = r8 + 0x10;
    /* ps_msub f13, f7, f28, f7 */;
    r7 = r7 + 0x20;
    /* ps_merge00 f2, f7, f7 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f1, f28, f29 */;
    /* ps_msub f12, f7, f27, f13 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f10, f7, f13 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_nmsub f11, f7, f1, f12 */;
    /* ps_add f8, f2, f10 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_merge11 f9, f12, f11 */;
    /* ps_sub f10, f2, f10 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_add f3, f2, f9 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_sub f9, f2, f9 */;
    /* psq_stu f8, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f9, f9, f9 */;
    /* psq_stu f3, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f10, f10, f10 */;
    r6 = r6 | r0;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E77E4;
    goto L_801E79F8;
L_801E7894: ;
    /* psq_l f1, 0x4(r8), 0, qr5 */;
    /* psq_l f2, 0x8(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_mul f1, f1, f2 */;
    r7 = r7 + 0x20;
    /* ps_sub f12, f7, f1 */;
    /* ps_add f13, f7, f1 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_madd f11, f1, f27, f12 */;
    /* ps_nmsub f10, f1, f27, f13 */;
    /* ps_mul f3, f12, f28 */;
    /* ps_merge00 f11, f13, f11 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_nmsub f9, f1, f30, f3 */;
    /* ps_merge00 f10, f10, f12 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f13 */;
    /* ps_nmsub f8, f7, f29, f3 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f13, f13, f9 */;
    /* ps_msub f3, f12, f27, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_add f2, f11, f13 */;
    /* ps_sub f8, f8, f3 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f12, f3, f8 */;
    /* ps_sub f11, f11, f13 */;
    /* psq_stu f2, 0x8(r9), 0, qr0 */;
    /* ps_add f4, f10, f12 */;
    /* ps_sub f1, f10, f12 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f1, f1, f1 */;
    /* ps_merge10 f11, f11, f11 */;
    /* psq_stu f1, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f11, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E77E4;
    goto L_801E79F8;
L_801E7930: ;
    /* psq_l f2, 0x4(r8), 0, qr5 */;
    /* psq_l f10, 0x8(r7), 0, qr0 */;
    /* ps_mul f2, f2, f10 */;
    /* psq_l f13, 0x8(r8), 0, qr5 */;
    /* psq_l f9, 0x10(r7), 0, qr0 */;
    /* ps_merge01 f4, f7, f2 */;
    /* psq_l f12, 0xc(r8), 0, qr5 */;
    /* ps_merge01 f1, f2, f7 */;
    /* psq_l f8, 0x18(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_madd f11, f13, f9, f4 */;
    /* ps_nmsub f10, f13, f9, f4 */;
    /* ps_madd f9, f12, f8, f1 */;
    /* ps_nmsub f8, f12, f8, f1 */;
    r7 = r7 + 0x20;
    /* ps_add f4, f11, f9 */;
    /* ps_sub f12, f11, f9 */;
    /* ps_msub f13, f8, f27, f9 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_sub f3, f8, f10 */;
    /* ps_add f1, f10, f13 */;
    /* ps_sub f13, f10, f13 */;
    /* ps_mul f3, f3, f28 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_merge00 f1, f4, f1 */;
    /* ps_nmsub f9, f10, f30, f3 */;
    /* ps_msub f11, f8, f29, f3 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f4 */;
    /* ps_merge00 f13, f13, f12 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_madd f10, f12, f27, f9 */;
    /* ps_merge11 f8, f4, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_sub f11, f11, f10 */;
    /* ps_add f12, f1, f8 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f11, f10, f11 */;
    /* ps_sub f4, f1, f8 */;
    /* ps_mul f7, f7, f6 */;
    /* ps_add f10, f13, f11 */;
    /* ps_sub f9, f13, f11 */;
    /* ps_merge10 f10, f10, f10 */;
    /* psq_stu f12, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f4, f4, f4 */;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E77E4;
L_801E79F8: ;
    r8 = *(u32*)lbl_8047B560;
    r9 = r31;
    r0 = *(u32*)lbl_8047B580;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    r4 = r3 << 2;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    r6 = r0 << 2;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    r7 = r4;
    /* ps_add f9, f7, f6 */;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f3, f7, f6 */;
    r6 = r7 + r6;
    /* ps_add f9, f9, f31 */;
    r0 = 0x3;
    /* ps_add f8, f5, f4 */;
    r5 = r8 + r7;
    /* ps_sub f2, f5, f4 */;
    /* ps_add f4, f9, f8 */;
    r4 = r8 + r6;
    /* ps_add f3, f3, f31 */;
    ctr_fn = (void(*)(void))r0;
L_801E7A50: ;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    r9 = r9 + 0x8;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f9, f9, f8 */;
    r7 = r7 + 0x2;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    r6 = r6 + 0x2;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* ps_add f9, f7, f6 */;
    /* psq_st f3, 0x0(r4), 0, qr6 */;
    /* ps_sub f3, f7, f6 */;
    /* ps_add f9, f9, f31 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    /* ps_add f8, f5, f4 */;
    /* ps_sub f2, f5, f4 */;
    /* psq_st f1, 0x10(r4), 0, qr6 */;
    r5 = r8 + r7;
    /* ps_add f4, f9, f8 */;
    /* psq_st f0, 0x18(r4), 0, qr6 */;
    /* ps_add f3, f3, f31 */;
    r4 = r8 + r6;
    if (--ctr != 0) goto L_801E7A50;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_sub f9, f9, f8 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* psq_st f0, 0x18(r4), 0, qr6 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f1, 0x10(r4), 0, qr6 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* psq_st f3, 0x0(r4), 0, qr6 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    r0 = *(u32*)((u8*)r31 + 0x108);
    r4 = *(u32*)lbl_8047B5B0;
    /* subi r8, r31, 0x8 */;
    *(u32*)lbl_8047B560 = r0;
    r0 = *(u8*)((u8*)r4 + 0x68C);
    r0 = r0 << 8;
    r0 = r4 + r0;
    *(u32*)lbl_8047B5A0 = r0;
    r9 = *(u32*)((u8*)r31 + 0x12C);
    r7 = *(u32*)lbl_8047B5A0;
    r4 = 0x8;
    ctr_fn = (void(*)(void))r4;
L_801E7BEC: ;
    /* psq_l f7, 0x0(r9), 0, qr5 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    r6 = *(u32*)((u8*)r9 + 0xC);
    r0 = *(u32*)((u8*)r9 + 0x8);
    /* ps_mul f7, f7, f6 */;
    r5 = *(u32*)((u8*)r9 + 0x4);
    r4 = *(u16*)((u8*)r9 + 0x2);
    /* or. r6, r6, r0 */;
L_801E7C0C: ;
    if ((s32)r6 != (s32)0x0) goto L_801E7D58;
    /* ps_merge00 f4, f7, f7 */;
    /* psq_st f4, 0x8(r8), 0, qr0 */;
    if ((s32)r5 != (s32)0x0) goto L_801E7CBC;
    /* psq_st f4, 0x10(r8), 0, qr0 */;
    /* psq_st f4, 0x18(r8), 0, qr0 */;
    if ((s32)r4 != (s32)0x0) goto L_801E7C48;
    r7 = r7 + 0x20;
    /* psq_stu f4, 0x20(r8), 0, qr0 */;
    r9 = r9 + 0x10;
    if (--ctr != 0) goto L_801E7BEC;
    goto L_801E7E20;
L_801E7C48: ;
    r9 = r9 + 0x10;
    /* ps_msub f13, f7, f28, f7 */;
    r7 = r7 + 0x20;
    /* ps_merge00 f2, f7, f7 */;
    r5 = *(u32*)((u8*)r9 + 0x4);
    /* ps_sub f1, f28, f29 */;
    /* ps_msub f12, f7, f27, f13 */;
    r4 = *(u16*)((u8*)r9 + 0x2);
    /* ps_merge11 f10, f7, f13 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_nmsub f11, f7, f1, f12 */;
    /* ps_add f8, f2, f10 */;
    /* psq_l f7, 0x0(r9), 0, qr5 */;
    /* ps_merge11 f9, f12, f11 */;
    /* ps_sub f10, f2, f10 */;
    r6 = *(u32*)((u8*)r9 + 0xC);
    /* ps_add f3, f2, f9 */;
    r0 = *(u32*)((u8*)r9 + 0x8);
    /* ps_sub f9, f2, f9 */;
    /* psq_stu f8, 0x8(r8), 0, qr0 */;
    /* ps_merge10 f9, f9, f9 */;
    /* psq_stu f3, 0x8(r8), 0, qr0 */;
    /* ps_merge10 f10, f10, f10 */;
    r6 = r6 | r0;
    /* psq_stu f9, 0x8(r8), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f10, 0x8(r8), 0, qr0 */;
    if (--ctr != 0) goto L_801E7C0C;
    goto L_801E7E20;
L_801E7CBC: ;
    /* psq_l f1, 0x4(r9), 0, qr5 */;
    /* psq_l f2, 0x8(r7), 0, qr0 */;
    r9 = r9 + 0x10;
    /* ps_mul f1, f1, f2 */;
    r7 = r7 + 0x20;
    /* ps_sub f12, f7, f1 */;
    /* ps_add f13, f7, f1 */;
    r6 = *(u32*)((u8*)r9 + 0xC);
    /* ps_madd f11, f1, f27, f12 */;
    /* ps_nmsub f10, f1, f27, f13 */;
    /* ps_mul f3, f12, f28 */;
    /* ps_merge00 f11, f13, f11 */;
    r0 = *(u32*)((u8*)r9 + 0x8);
    /* ps_nmsub f9, f1, f30, f3 */;
    /* ps_merge00 f10, f10, f12 */;
    r5 = *(u32*)((u8*)r9 + 0x4);
    /* ps_sub f9, f9, f13 */;
    /* ps_nmsub f8, f7, f29, f3 */;
    r4 = *(u16*)((u8*)r9 + 0x2);
    /* ps_merge11 f13, f13, f9 */;
    /* ps_msub f3, f12, f27, f9 */;
    /* psq_l f7, 0x0(r9), 0, qr5 */;
    /* ps_add f2, f11, f13 */;
    /* ps_sub f8, f8, f3 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f12, f3, f8 */;
    /* ps_sub f11, f11, f13 */;
    /* psq_stu f2, 0x8(r8), 0, qr0 */;
    /* ps_add f4, f10, f12 */;
    /* ps_sub f1, f10, f12 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r8), 0, qr0 */;
    /* ps_merge10 f1, f1, f1 */;
    /* ps_merge10 f11, f11, f11 */;
    /* psq_stu f1, 0x8(r8), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f11, 0x8(r8), 0, qr0 */;
    if (--ctr != 0) goto L_801E7C0C;
    goto L_801E7E20;
L_801E7D58: ;
    /* psq_l f2, 0x4(r9), 0, qr5 */;
    /* psq_l f10, 0x8(r7), 0, qr0 */;
    /* ps_mul f2, f2, f10 */;
    /* psq_l f13, 0x8(r9), 0, qr5 */;
    /* psq_l f9, 0x10(r7), 0, qr0 */;
    /* ps_merge01 f4, f7, f2 */;
    /* psq_l f12, 0xc(r9), 0, qr5 */;
    /* ps_merge01 f1, f2, f7 */;
    /* psq_l f8, 0x18(r7), 0, qr0 */;
    r9 = r9 + 0x10;
    /* ps_madd f11, f13, f9, f4 */;
    /* ps_nmsub f10, f13, f9, f4 */;
    /* ps_madd f9, f12, f8, f1 */;
    /* ps_nmsub f8, f12, f8, f1 */;
    r7 = r7 + 0x20;
    /* ps_add f4, f11, f9 */;
    /* ps_sub f12, f11, f9 */;
    /* ps_msub f13, f8, f27, f9 */;
    r6 = *(u32*)((u8*)r9 + 0xC);
    /* ps_sub f3, f8, f10 */;
    /* ps_add f1, f10, f13 */;
    /* ps_sub f13, f10, f13 */;
    /* ps_mul f3, f3, f28 */;
    r0 = *(u32*)((u8*)r9 + 0x8);
    /* ps_merge00 f1, f4, f1 */;
    /* ps_nmsub f9, f10, f30, f3 */;
    /* ps_msub f11, f8, f29, f3 */;
    r5 = *(u32*)((u8*)r9 + 0x4);
    /* ps_sub f9, f9, f4 */;
    /* ps_merge00 f13, f13, f12 */;
    r4 = *(u16*)((u8*)r9 + 0x2);
    /* ps_madd f10, f12, f27, f9 */;
    /* ps_merge11 f8, f4, f9 */;
    /* psq_l f7, 0x0(r9), 0, qr5 */;
    /* ps_sub f11, f11, f10 */;
    /* ps_add f12, f1, f8 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f11, f10, f11 */;
    /* ps_sub f4, f1, f8 */;
    /* ps_mul f7, f7, f6 */;
    /* ps_add f10, f13, f11 */;
    /* ps_sub f9, f13, f11 */;
    /* ps_merge10 f10, f10, f10 */;
    /* psq_stu f12, 0x8(r8), 0, qr0 */;
    /* ps_merge10 f4, f4, f4 */;
    /* psq_stu f9, 0x8(r8), 0, qr0 */;
    /* psq_stu f10, 0x8(r8), 0, qr0 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r8), 0, qr0 */;
    if (--ctr != 0) goto L_801E7C0C;
L_801E7E20: ;
    r7 = *(u32*)lbl_8047B560;
    r8 = r31;
    r0 = *(u32*)lbl_8047B580;
    /* psq_l f7, 0x0(r8), 0, qr0 */;
    r3 = r3 << 2;
    /* psq_l f6, 0x80(r8), 0, qr0 */;
    r5 = r0 << 2;
    /* psq_l f5, 0x40(r8), 0, qr0 */;
    r6 = r3;
    /* ps_add f9, f7, f6 */;
    /* psq_l f4, 0xc0(r8), 0, qr0 */;
    /* ps_sub f3, f7, f6 */;
    r5 = r6 + r5;
    /* ps_add f9, f9, f31 */;
    r0 = 0x3;
    /* ps_add f8, f5, f4 */;
    r4 = r7 + r6;
    /* ps_sub f2, f5, f4 */;
    /* ps_add f4, f9, f8 */;
    r3 = r7 + r5;
    /* ps_add f3, f3, f31 */;
    ctr_fn = (void(*)(void))r0;
L_801E7E78: ;
    /* psq_l f11, 0x20(r8), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r8), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r8), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r8), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    r8 = r8 + 0x8;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* psq_l f7, 0x0(r8), 0, qr0 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* psq_l f6, 0x80(r8), 0, qr0 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* psq_l f5, 0x40(r8), 0, qr0 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_l f4, 0xc0(r8), 0, qr0 */;
    /* ps_sub f9, f9, f8 */;
    r6 = r6 + 0x2;
    /* psq_st f2, 0x0(r4), 0, qr6 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r4), 0, qr6 */;
    r5 = r5 + 0x2;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r4), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* ps_add f9, f7, f6 */;
    /* psq_st f3, 0x0(r3), 0, qr6 */;
    /* ps_sub f3, f7, f6 */;
    /* ps_add f9, f9, f31 */;
    /* psq_st f2, 0x8(r3), 0, qr6 */;
    /* ps_add f8, f5, f4 */;
    /* ps_sub f2, f5, f4 */;
    /* psq_st f1, 0x10(r3), 0, qr6 */;
    r4 = r7 + r6;
    /* ps_add f4, f9, f8 */;
    /* psq_st f0, 0x18(r3), 0, qr6 */;
    /* ps_add f3, f3, f31 */;
    r3 = r7 + r5;
    if (--ctr != 0) goto L_801E7E78;
    /* psq_l f11, 0x20(r8), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r8), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r8), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r8), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_st f2, 0x0(r4), 0, qr6 */;
    /* ps_sub f9, f9, f8 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* psq_st f0, 0x18(r3), 0, qr6 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f1, 0x10(r3), 0, qr6 */;
    /* psq_st f3, 0x10(r4), 0, qr6 */;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r4), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* psq_st f3, 0x0(r3), 0, qr6 */;
    /* psq_st f2, 0x8(r3), 0, qr6 */;
    r4 = *(u32*)lbl_8047B5B0;
    r0 = *(u8*)((u8*)r4 + 0x6A9);
    if ((u32)r0 == (u32)0x0) goto L_801E8060;
    r3 = *(u16*)((u8*)r4 + 0x6AC);
    /* subi r3, r3, 0x1 */;
    r0 = r3 & 0xFFFF;
    *(u16*)((u8*)r4 + 0x6AC) = r3;
    if ((u32)r0 != (u32)0x0) goto L_801E8060;
    r3 = *(u32*)lbl_8047B5B0;
    r0 = *(u16*)((u8*)r3 + 0x6AA);
    *(u16*)((u8*)r3 + 0x6AC) = r0;
    r4 = *(u32*)lbl_8047B5B0;
    r3 = *(u32*)((u8*)r4 + 0x6A4);
    r0 = r3 + 0x6;
    /* clrrwi r3, r0, 3 */;
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x6A4) = r0;
    r3 = *(u32*)lbl_8047B5B0;
    r0 = *(u32*)((u8*)r3 + 0x6A4);
    if ((u32)r0 <= (u32)0x21) goto L_801E8044;
    r0 = 0x21;
    *(u32*)((u8*)r3 + 0x6A4) = r0;
L_801E8044: ;
    r3 = *(u32*)lbl_8047B5B0;
    r0 = 0x0;
    *(u16*)((u8*)r3 + 0x684) = r0;
    r3 = *(u32*)lbl_8047B5B0;
    *(u16*)((u8*)r3 + 0x68A) = r0;
    r3 = *(u32*)lbl_8047B5B0;
    *(u16*)((u8*)r3 + 0x690) = r0;
L_801E8060: ;
    r30 = r30 + 0x1;
L_801E8064: ;
    r3 = *(u32*)lbl_8047B5B0;
    r4 = r30 & 0xFF;
    r0 = *(u16*)((u8*)r3 + 0x696);
    if ((s32)r4 < (s32)r0) goto L_801E66D8;
    r3 = *(u32*)((u8*)r3 + 0x6B0);
    r5 = 0x2000;
    r4 = *(u32*)((u8*)r31 + 0x100);
    fn_8009B55C();
    r3 = *(u32*)lbl_8047B5B0;
    r5 = 0x800;
    r4 = *(u32*)((u8*)r31 + 0x104);
    r3 = *(u32*)((u8*)r3 + 0x6B4);
    fn_8009B55C();
    r3 = *(u32*)lbl_8047B5B0;
    r5 = 0x800;
    r4 = *(u32*)((u8*)r31 + 0x108);
    r3 = *(u32*)((u8*)r3 + 0x6B8);
    fn_8009B55C();
    r4 = *(u32*)lbl_8047B5B0;
    r3 = *(u32*)((u8*)r4 + 0x6B0);
    r0 = r3 + 0x2000;
    *(u32*)((u8*)r4 + 0x6B0) = r0;
    r4 = *(u32*)lbl_8047B5B0;
    r3 = *(u32*)((u8*)r4 + 0x6B4);
    r0 = r3 + 0x800;
    *(u32*)((u8*)r4 + 0x6B4) = r0;
    r4 = *(u32*)lbl_8047B5B0;
    r3 = *(u32*)((u8*)r4 + 0x6B8);
    r0 = r3 + 0x800;
    *(u32*)((u8*)r4 + 0x6B8) = r0;
    f31 = *(f64*)((u8*)r1 + 0x30);
    f30 = *(f64*)((u8*)r1 + 0x28);
    f29 = *(f64*)((u8*)r1 + 0x20);
    f28 = *(f64*)((u8*)r1 + 0x18);
    f27 = *(f64*)((u8*)r1 + 0x10);
    return;
}
#pragma pop

/* 0x801E810C | size: 0x1A8C | massive */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E810C(void) {
    extern u8 lbl_8046D500[];
    extern u8 lbl_8047B560[];
    extern u8 lbl_8047B580[];
    extern u8 lbl_8047B5A0[];
    extern u8 lbl_8047B5B0[];
    extern u8 lbl_8047E4B8[];
    extern u8 lbl_8047E4BC[];
    extern u8 lbl_8047E4C0[];
    extern u8 lbl_8047E4C4[];
    extern u8 lbl_8047E4C8[];
    extern void fn_8009B55C();
    extern void fn_8009B614();
    extern void fn_801EB644();
    extern void fn_801EBCC0();
    extern void fn_801EC368();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    f32 f9 = 0.0f;
    f32 f10 = 0.0f;
    f32 f11 = 0.0f;
    f32 f12 = 0.0f;
    f32 f13 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;
    u8 sp[0x100];

    r3 = (u32)lbl_8046D500;
    *(f64*)(sp + 0x30) = f31;
    *(f64*)(sp + 0x28) = f30;
    *(f64*)(sp + 0x20) = f29;
    *(f64*)(sp + 0x18) = f28;
    *(f64*)(sp + 0x10) = f27;
    r31 = (u32)lbl_8046D500;
    r3 = 0x3;
    fn_8009B614();
    f27 = *(f32*)lbl_8047E4B8;
    r30 = 0x0;
    f28 = *(f32*)lbl_8047E4BC;
    f29 = *(f32*)lbl_8047E4C0;
    f30 = *(f32*)lbl_8047E4C4;
    f31 = *(f32*)lbl_8047E4C8;
    goto L_801E9AF0;
L_801E8160: ;
    r3 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r31 + 0x118);
    fn_801EB644();
    r3 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r31 + 0x11C);
    fn_801EB644();
    r3 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r31 + 0x120);
    fn_801EB644();
    r3 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r31 + 0x124);
    fn_801EB644();
    r3 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r31 + 0x128);
    fn_801EBCC0();
    r3 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r31 + 0x12C);
    fn_801EC368();
    r3 = *(u32*)((u8*)r31 + 0x10C);
    r0 = 0x280;
    r4 = *(u32*)lbl_8047B5B0;
    /* subi r9, r31, 0x8 */;
    *(u32*)lbl_8047B560 = r3;
    /* clrlslwi r3, r30, 24, 4 */;
    *(u32*)lbl_8047B580 = r0;
    r0 = *(u8*)((u8*)r4 + 0x680);
    r0 = r0 << 8;
    r0 = r4 + r0;
    *(u32*)lbl_8047B5A0 = r0;
    r8 = *(u32*)((u8*)r31 + 0x118);
    r7 = *(u32*)lbl_8047B5A0;
    r4 = 0x8;
    ctr_fn = (void(*)(void))r4;
L_801E81E4: ;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_mul f7, f7, f6 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* or. r6, r6, r0 */;
L_801E8204: ;
    if ((s32)r6 != (s32)0x0) goto L_801E8350;
    /* ps_merge00 f4, f7, f7 */;
    /* psq_st f4, 0x8(r9), 0, qr0 */;
    if ((s32)r5 != (s32)0x0) goto L_801E82B4;
    /* psq_st f4, 0x10(r9), 0, qr0 */;
    /* psq_st f4, 0x18(r9), 0, qr0 */;
    if ((s32)r4 != (s32)0x0) goto L_801E8240;
    r7 = r7 + 0x20;
    /* psq_stu f4, 0x20(r9), 0, qr0 */;
    r8 = r8 + 0x10;
    if (--ctr != 0) goto L_801E81E4;
    goto L_801E8418;
L_801E8240: ;
    r8 = r8 + 0x10;
    /* ps_msub f13, f7, f28, f7 */;
    r7 = r7 + 0x20;
    /* ps_merge00 f2, f7, f7 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f1, f28, f29 */;
    /* ps_msub f12, f7, f27, f13 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f10, f7, f13 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_nmsub f11, f7, f1, f12 */;
    /* ps_add f8, f2, f10 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_merge11 f9, f12, f11 */;
    /* ps_sub f10, f2, f10 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_add f3, f2, f9 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_sub f9, f2, f9 */;
    /* psq_stu f8, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f9, f9, f9 */;
    /* psq_stu f3, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f10, f10, f10 */;
    r6 = r6 | r0;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E8204;
    goto L_801E8418;
L_801E82B4: ;
    /* psq_l f1, 0x4(r8), 0, qr5 */;
    /* psq_l f2, 0x8(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_mul f1, f1, f2 */;
    r7 = r7 + 0x20;
    /* ps_sub f12, f7, f1 */;
    /* ps_add f13, f7, f1 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_madd f11, f1, f27, f12 */;
    /* ps_nmsub f10, f1, f27, f13 */;
    /* ps_mul f3, f12, f28 */;
    /* ps_merge00 f11, f13, f11 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_nmsub f9, f1, f30, f3 */;
    /* ps_merge00 f10, f10, f12 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f13 */;
    /* ps_nmsub f8, f7, f29, f3 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f13, f13, f9 */;
    /* ps_msub f3, f12, f27, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_add f2, f11, f13 */;
    /* ps_sub f8, f8, f3 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f12, f3, f8 */;
    /* ps_sub f11, f11, f13 */;
    /* psq_stu f2, 0x8(r9), 0, qr0 */;
    /* ps_add f4, f10, f12 */;
    /* ps_sub f1, f10, f12 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f1, f1, f1 */;
    /* ps_merge10 f11, f11, f11 */;
    /* psq_stu f1, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f11, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E8204;
    goto L_801E8418;
L_801E8350: ;
    /* psq_l f2, 0x4(r8), 0, qr5 */;
    /* psq_l f10, 0x8(r7), 0, qr0 */;
    /* ps_mul f2, f2, f10 */;
    /* psq_l f13, 0x8(r8), 0, qr5 */;
    /* psq_l f9, 0x10(r7), 0, qr0 */;
    /* ps_merge01 f4, f7, f2 */;
    /* psq_l f12, 0xc(r8), 0, qr5 */;
    /* ps_merge01 f1, f2, f7 */;
    /* psq_l f8, 0x18(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_madd f11, f13, f9, f4 */;
    /* ps_nmsub f10, f13, f9, f4 */;
    /* ps_madd f9, f12, f8, f1 */;
    /* ps_nmsub f8, f12, f8, f1 */;
    r7 = r7 + 0x20;
    /* ps_add f4, f11, f9 */;
    /* ps_sub f12, f11, f9 */;
    /* ps_msub f13, f8, f27, f9 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_sub f3, f8, f10 */;
    /* ps_add f1, f10, f13 */;
    /* ps_sub f13, f10, f13 */;
    /* ps_mul f3, f3, f28 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_merge00 f1, f4, f1 */;
    /* ps_nmsub f9, f10, f30, f3 */;
    /* ps_msub f11, f8, f29, f3 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f4 */;
    /* ps_merge00 f13, f13, f12 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_madd f10, f12, f27, f9 */;
    /* ps_merge11 f8, f4, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_sub f11, f11, f10 */;
    /* ps_add f12, f1, f8 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f11, f10, f11 */;
    /* ps_sub f4, f1, f8 */;
    /* ps_mul f7, f7, f6 */;
    /* ps_add f10, f13, f11 */;
    /* ps_sub f9, f13, f11 */;
    /* ps_merge10 f10, f10, f10 */;
    /* psq_stu f12, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f4, f4, f4 */;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E8204;
L_801E8418: ;
    r8 = *(u32*)lbl_8047B560;
    r9 = r31;
    r0 = *(u32*)lbl_8047B580;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    r4 = r3 << 2;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    r6 = r0 << 2;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    r7 = r4;
    /* ps_add f9, f7, f6 */;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f3, f7, f6 */;
    r6 = r7 + r6;
    /* ps_add f9, f9, f31 */;
    r0 = 0x3;
    /* ps_add f8, f5, f4 */;
    r5 = r8 + r7;
    /* ps_sub f2, f5, f4 */;
    /* ps_add f4, f9, f8 */;
    r4 = r8 + r6;
    /* ps_add f3, f3, f31 */;
    ctr_fn = (void(*)(void))r0;
L_801E8470: ;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    r9 = r9 + 0x8;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f9, f9, f8 */;
    r7 = r7 + 0x2;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    r6 = r6 + 0x2;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* ps_add f9, f7, f6 */;
    /* psq_st f3, 0x0(r4), 0, qr6 */;
    /* ps_sub f3, f7, f6 */;
    /* ps_add f9, f9, f31 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    /* ps_add f8, f5, f4 */;
    /* ps_sub f2, f5, f4 */;
    /* psq_st f1, 0x10(r4), 0, qr6 */;
    r5 = r8 + r7;
    /* ps_add f4, f9, f8 */;
    /* psq_st f0, 0x18(r4), 0, qr6 */;
    /* ps_add f3, f3, f31 */;
    r4 = r8 + r6;
    if (--ctr != 0) goto L_801E8470;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_sub f9, f9, f8 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* psq_st f0, 0x18(r4), 0, qr6 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f1, 0x10(r4), 0, qr6 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* psq_st f3, 0x0(r4), 0, qr6 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    r8 = *(u32*)((u8*)r31 + 0x11C);
    r7 = *(u32*)lbl_8047B5A0;
    r10 = r3 + 0x8;
    /* subi r9, r31, 0x8 */;
    r4 = 0x8;
    ctr_fn = (void(*)(void))r4;
L_801E85F4: ;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_mul f7, f7, f6 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* or. r6, r6, r0 */;
L_801E8614: ;
    if ((s32)r6 != (s32)0x0) goto L_801E8760;
    /* ps_merge00 f4, f7, f7 */;
    /* psq_st f4, 0x8(r9), 0, qr0 */;
    if ((s32)r5 != (s32)0x0) goto L_801E86C4;
    /* psq_st f4, 0x10(r9), 0, qr0 */;
    /* psq_st f4, 0x18(r9), 0, qr0 */;
    if ((s32)r4 != (s32)0x0) goto L_801E8650;
    r7 = r7 + 0x20;
    /* psq_stu f4, 0x20(r9), 0, qr0 */;
    r8 = r8 + 0x10;
    if (--ctr != 0) goto L_801E85F4;
    goto L_801E8828;
L_801E8650: ;
    r8 = r8 + 0x10;
    /* ps_msub f13, f7, f28, f7 */;
    r7 = r7 + 0x20;
    /* ps_merge00 f2, f7, f7 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f1, f28, f29 */;
    /* ps_msub f12, f7, f27, f13 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f10, f7, f13 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_nmsub f11, f7, f1, f12 */;
    /* ps_add f8, f2, f10 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_merge11 f9, f12, f11 */;
    /* ps_sub f10, f2, f10 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_add f3, f2, f9 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_sub f9, f2, f9 */;
    /* psq_stu f8, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f9, f9, f9 */;
    /* psq_stu f3, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f10, f10, f10 */;
    r6 = r6 | r0;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E8614;
    goto L_801E8828;
L_801E86C4: ;
    /* psq_l f1, 0x4(r8), 0, qr5 */;
    /* psq_l f2, 0x8(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_mul f1, f1, f2 */;
    r7 = r7 + 0x20;
    /* ps_sub f12, f7, f1 */;
    /* ps_add f13, f7, f1 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_madd f11, f1, f27, f12 */;
    /* ps_nmsub f10, f1, f27, f13 */;
    /* ps_mul f3, f12, f28 */;
    /* ps_merge00 f11, f13, f11 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_nmsub f9, f1, f30, f3 */;
    /* ps_merge00 f10, f10, f12 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f13 */;
    /* ps_nmsub f8, f7, f29, f3 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f13, f13, f9 */;
    /* ps_msub f3, f12, f27, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_add f2, f11, f13 */;
    /* ps_sub f8, f8, f3 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f12, f3, f8 */;
    /* ps_sub f11, f11, f13 */;
    /* psq_stu f2, 0x8(r9), 0, qr0 */;
    /* ps_add f4, f10, f12 */;
    /* ps_sub f1, f10, f12 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f1, f1, f1 */;
    /* ps_merge10 f11, f11, f11 */;
    /* psq_stu f1, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f11, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E8614;
    goto L_801E8828;
L_801E8760: ;
    /* psq_l f2, 0x4(r8), 0, qr5 */;
    /* psq_l f10, 0x8(r7), 0, qr0 */;
    /* ps_mul f2, f2, f10 */;
    /* psq_l f13, 0x8(r8), 0, qr5 */;
    /* psq_l f9, 0x10(r7), 0, qr0 */;
    /* ps_merge01 f4, f7, f2 */;
    /* psq_l f12, 0xc(r8), 0, qr5 */;
    /* ps_merge01 f1, f2, f7 */;
    /* psq_l f8, 0x18(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_madd f11, f13, f9, f4 */;
    /* ps_nmsub f10, f13, f9, f4 */;
    /* ps_madd f9, f12, f8, f1 */;
    /* ps_nmsub f8, f12, f8, f1 */;
    r7 = r7 + 0x20;
    /* ps_add f4, f11, f9 */;
    /* ps_sub f12, f11, f9 */;
    /* ps_msub f13, f8, f27, f9 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_sub f3, f8, f10 */;
    /* ps_add f1, f10, f13 */;
    /* ps_sub f13, f10, f13 */;
    /* ps_mul f3, f3, f28 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_merge00 f1, f4, f1 */;
    /* ps_nmsub f9, f10, f30, f3 */;
    /* ps_msub f11, f8, f29, f3 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f4 */;
    /* ps_merge00 f13, f13, f12 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_madd f10, f12, f27, f9 */;
    /* ps_merge11 f8, f4, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_sub f11, f11, f10 */;
    /* ps_add f12, f1, f8 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f11, f10, f11 */;
    /* ps_sub f4, f1, f8 */;
    /* ps_mul f7, f7, f6 */;
    /* ps_add f10, f13, f11 */;
    /* ps_sub f9, f13, f11 */;
    /* ps_merge10 f10, f10, f10 */;
    /* psq_stu f12, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f4, f4, f4 */;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E8614;
L_801E8828: ;
    r8 = *(u32*)lbl_8047B560;
    r9 = r31;
    r0 = *(u32*)lbl_8047B580;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    r10 = r10 << 2;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    r6 = r0 << 2;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    r7 = r10;
    /* ps_add f9, f7, f6 */;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f3, f7, f6 */;
    r6 = r7 + r6;
    /* ps_add f9, f9, f31 */;
    r0 = 0x3;
    /* ps_add f8, f5, f4 */;
    r5 = r8 + r7;
    /* ps_sub f2, f5, f4 */;
    /* ps_add f4, f9, f8 */;
    r4 = r8 + r6;
    /* ps_add f3, f3, f31 */;
    ctr_fn = (void(*)(void))r0;
L_801E8880: ;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    r9 = r9 + 0x8;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f9, f9, f8 */;
    r7 = r7 + 0x2;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    r6 = r6 + 0x2;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* ps_add f9, f7, f6 */;
    /* psq_st f3, 0x0(r4), 0, qr6 */;
    /* ps_sub f3, f7, f6 */;
    /* ps_add f9, f9, f31 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    /* ps_add f8, f5, f4 */;
    /* ps_sub f2, f5, f4 */;
    /* psq_st f1, 0x10(r4), 0, qr6 */;
    r5 = r8 + r7;
    /* ps_add f4, f9, f8 */;
    /* psq_st f0, 0x18(r4), 0, qr6 */;
    /* ps_add f3, f3, f31 */;
    r4 = r8 + r6;
    if (--ctr != 0) goto L_801E8880;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_sub f9, f9, f8 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* psq_st f0, 0x18(r4), 0, qr6 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f1, 0x10(r4), 0, qr6 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* psq_st f3, 0x0(r4), 0, qr6 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    r8 = *(u32*)((u8*)r31 + 0x120);
    r7 = *(u32*)lbl_8047B5A0;
    /* subi r9, r31, 0x8 */;
    r4 = 0x8;
    ctr_fn = (void(*)(void))r4;
L_801E8A00: ;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_mul f7, f7, f6 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    r4 = *(u16*)((u8*)r8 + 0x2);
    r6 = r6 | r0;
L_801E8A20: ;
    if ((s32)r6 != (s32)0x0) goto L_801E8B6C;
    /* ps_merge00 f4, f7, f7 */;
    /* psq_st f4, 0x8(r9), 0, qr0 */;
    if ((s32)r5 != (s32)0x0) goto L_801E8AD0;
    /* psq_st f4, 0x10(r9), 0, qr0 */;
    /* psq_st f4, 0x18(r9), 0, qr0 */;
    if ((s32)r4 != (s32)0x0) goto L_801E8A5C;
    r7 = r7 + 0x20;
    /* psq_stu f4, 0x20(r9), 0, qr0 */;
    r8 = r8 + 0x10;
    if (--ctr != 0) goto L_801E8A00;
    goto L_801E8C34;
L_801E8A5C: ;
    /* ps_msub f13, f7, f28, f7 */;
    r8 = r8 + 0x10;
    /* ps_merge00 f2, f7, f7 */;
    r7 = r7 + 0x20;
    /* ps_sub f1, f28, f29 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_msub f12, f7, f27, f13 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f10, f7, f13 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_nmsub f11, f7, f1, f12 */;
    /* ps_add f8, f2, f10 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_merge11 f9, f12, f11 */;
    /* ps_sub f10, f2, f10 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_add f3, f2, f9 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_sub f9, f2, f9 */;
    /* psq_stu f8, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f9, f9, f9 */;
    /* psq_stu f3, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f10, f10, f10 */;
    r6 = r6 | r0;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E8A20;
    goto L_801E8C34;
L_801E8AD0: ;
    /* psq_l f1, 0x4(r8), 0, qr5 */;
    /* psq_l f2, 0x8(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_mul f1, f1, f2 */;
    r7 = r7 + 0x20;
    /* ps_sub f12, f7, f1 */;
    /* ps_add f13, f7, f1 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_madd f11, f1, f27, f12 */;
    /* ps_nmsub f10, f1, f27, f13 */;
    /* ps_mul f3, f12, f28 */;
    /* ps_merge00 f11, f13, f11 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_nmsub f9, f1, f30, f3 */;
    /* ps_merge00 f10, f10, f12 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f13 */;
    /* ps_nmsub f8, f7, f29, f3 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f13, f13, f9 */;
    /* ps_msub f3, f12, f27, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_add f2, f11, f13 */;
    /* ps_sub f8, f8, f3 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f12, f3, f8 */;
    /* ps_sub f11, f11, f13 */;
    /* psq_stu f2, 0x8(r9), 0, qr0 */;
    /* ps_add f4, f10, f12 */;
    /* ps_sub f1, f10, f12 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f1, f1, f1 */;
    /* ps_merge10 f11, f11, f11 */;
    /* psq_stu f1, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f11, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E8A20;
    goto L_801E8C34;
L_801E8B6C: ;
    /* psq_l f2, 0x4(r8), 0, qr5 */;
    /* psq_l f10, 0x8(r7), 0, qr0 */;
    /* ps_mul f2, f2, f10 */;
    /* psq_l f13, 0x8(r8), 0, qr5 */;
    /* psq_l f9, 0x10(r7), 0, qr0 */;
    /* ps_merge01 f4, f7, f2 */;
    /* psq_l f12, 0xc(r8), 0, qr5 */;
    /* ps_merge01 f1, f2, f7 */;
    /* psq_l f8, 0x18(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_madd f11, f13, f9, f4 */;
    /* ps_nmsub f10, f13, f9, f4 */;
    /* ps_madd f9, f12, f8, f1 */;
    /* ps_nmsub f8, f12, f8, f1 */;
    r7 = r7 + 0x20;
    /* ps_add f4, f11, f9 */;
    /* ps_sub f12, f11, f9 */;
    /* ps_msub f13, f8, f27, f9 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_sub f3, f8, f10 */;
    /* ps_add f1, f10, f13 */;
    /* ps_sub f13, f10, f13 */;
    /* ps_mul f3, f3, f28 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_merge00 f1, f4, f1 */;
    /* ps_nmsub f9, f10, f30, f3 */;
    /* ps_msub f11, f8, f29, f3 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f4 */;
    /* ps_merge00 f13, f13, f12 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_madd f10, f12, f27, f9 */;
    /* ps_merge11 f8, f4, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_sub f11, f11, f10 */;
    /* ps_add f12, f1, f8 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f11, f10, f11 */;
    /* ps_sub f4, f1, f8 */;
    /* ps_mul f7, f7, f6 */;
    /* ps_add f10, f13, f11 */;
    /* ps_sub f9, f13, f11 */;
    /* ps_merge10 f10, f10, f10 */;
    /* psq_stu f12, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f4, f4, f4 */;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E8A20;
L_801E8C34: ;
    r8 = *(u32*)lbl_8047B560;
    r9 = r31;
    r0 = *(u32*)lbl_8047B580;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    r7 = r0 << 3;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    r4 = r3 << 2;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    r6 = r0 << 2;
    /* ps_add f9, f7, f6 */;
    r7 = r7 + r4;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f3, f7, f6 */;
    r6 = r7 + r6;
    /* ps_add f9, f9, f31 */;
    r0 = 0x3;
    /* ps_add f8, f5, f4 */;
    r5 = r8 + r7;
    /* ps_sub f2, f5, f4 */;
    /* ps_add f4, f9, f8 */;
    r4 = r8 + r6;
    /* ps_add f3, f3, f31 */;
    ctr_fn = (void(*)(void))r0;
L_801E8C90: ;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    r9 = r9 + 0x8;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f9, f9, f8 */;
    r7 = r7 + 0x2;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    r6 = r6 + 0x2;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* ps_add f9, f7, f6 */;
    /* psq_st f3, 0x0(r4), 0, qr6 */;
    /* ps_sub f3, f7, f6 */;
    /* ps_add f9, f9, f31 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    /* ps_add f8, f5, f4 */;
    /* ps_sub f2, f5, f4 */;
    /* psq_st f1, 0x10(r4), 0, qr6 */;
    r5 = r8 + r7;
    /* ps_add f4, f9, f8 */;
    /* psq_st f0, 0x18(r4), 0, qr6 */;
    /* ps_add f3, f3, f31 */;
    r4 = r8 + r6;
    if (--ctr != 0) goto L_801E8C90;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_sub f9, f9, f8 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* psq_st f0, 0x18(r4), 0, qr6 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    /* ps_sub f2, f12, f11 */;
    /* psq_st f1, 0x10(r4), 0, qr6 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* psq_st f3, 0x0(r4), 0, qr6 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    r8 = *(u32*)((u8*)r31 + 0x124);
    r7 = *(u32*)lbl_8047B5A0;
    r9 = r3 + 0x8;
    /* subi r10, r31, 0x8 */;
    r4 = 0x8;
    ctr_fn = (void(*)(void))r4;
L_801E8E14: ;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_mul f7, f7, f6 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    r4 = *(u16*)((u8*)r8 + 0x2);
    r6 = r6 | r0;
L_801E8E34: ;
    if ((s32)r6 != (s32)0x0) goto L_801E8F80;
    /* ps_merge00 f4, f7, f7 */;
    /* psq_st f4, 0x8(r10), 0, qr0 */;
    if ((s32)r5 != (s32)0x0) goto L_801E8EE4;
    /* psq_st f4, 0x10(r10), 0, qr0 */;
    /* psq_st f4, 0x18(r10), 0, qr0 */;
    if ((s32)r4 != (s32)0x0) goto L_801E8E70;
    r7 = r7 + 0x20;
    /* psq_stu f4, 0x20(r10), 0, qr0 */;
    r8 = r8 + 0x10;
    if (--ctr != 0) goto L_801E8E14;
    goto L_801E9048;
L_801E8E70: ;
    /* ps_msub f13, f7, f28, f7 */;
    r8 = r8 + 0x10;
    /* ps_merge00 f2, f7, f7 */;
    r7 = r7 + 0x20;
    /* ps_sub f1, f28, f29 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_msub f12, f7, f27, f13 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f10, f7, f13 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_nmsub f11, f7, f1, f12 */;
    /* ps_add f8, f2, f10 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_merge11 f9, f12, f11 */;
    /* ps_sub f10, f2, f10 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_add f3, f2, f9 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_sub f9, f2, f9 */;
    /* psq_stu f8, 0x8(r10), 0, qr0 */;
    /* ps_merge10 f9, f9, f9 */;
    /* psq_stu f3, 0x8(r10), 0, qr0 */;
    /* ps_merge10 f10, f10, f10 */;
    r6 = r6 | r0;
    /* psq_stu f9, 0x8(r10), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f10, 0x8(r10), 0, qr0 */;
    if (--ctr != 0) goto L_801E8E34;
    goto L_801E9048;
L_801E8EE4: ;
    /* psq_l f1, 0x4(r8), 0, qr5 */;
    /* psq_l f2, 0x8(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_mul f1, f1, f2 */;
    r7 = r7 + 0x20;
    /* ps_sub f12, f7, f1 */;
    /* ps_add f13, f7, f1 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_madd f11, f1, f27, f12 */;
    /* ps_nmsub f10, f1, f27, f13 */;
    /* ps_mul f3, f12, f28 */;
    /* ps_merge00 f11, f13, f11 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_nmsub f9, f1, f30, f3 */;
    /* ps_merge00 f10, f10, f12 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f13 */;
    /* ps_nmsub f8, f7, f29, f3 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f13, f13, f9 */;
    /* ps_msub f3, f12, f27, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_add f2, f11, f13 */;
    /* ps_sub f8, f8, f3 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f12, f3, f8 */;
    /* ps_sub f11, f11, f13 */;
    /* psq_stu f2, 0x8(r10), 0, qr0 */;
    /* ps_add f4, f10, f12 */;
    /* ps_sub f1, f10, f12 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r10), 0, qr0 */;
    /* ps_merge10 f1, f1, f1 */;
    /* ps_merge10 f11, f11, f11 */;
    /* psq_stu f1, 0x8(r10), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f11, 0x8(r10), 0, qr0 */;
    if (--ctr != 0) goto L_801E8E34;
    goto L_801E9048;
L_801E8F80: ;
    /* psq_l f2, 0x4(r8), 0, qr5 */;
    /* psq_l f10, 0x8(r7), 0, qr0 */;
    /* ps_mul f2, f2, f10 */;
    /* psq_l f13, 0x8(r8), 0, qr5 */;
    /* psq_l f9, 0x10(r7), 0, qr0 */;
    /* ps_merge01 f4, f7, f2 */;
    /* psq_l f12, 0xc(r8), 0, qr5 */;
    /* ps_merge01 f1, f2, f7 */;
    /* psq_l f8, 0x18(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_madd f11, f13, f9, f4 */;
    /* ps_nmsub f10, f13, f9, f4 */;
    /* ps_madd f9, f12, f8, f1 */;
    /* ps_nmsub f8, f12, f8, f1 */;
    r7 = r7 + 0x20;
    /* ps_add f4, f11, f9 */;
    /* ps_sub f12, f11, f9 */;
    /* ps_msub f13, f8, f27, f9 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_sub f3, f8, f10 */;
    /* ps_add f1, f10, f13 */;
    /* ps_sub f13, f10, f13 */;
    /* ps_mul f3, f3, f28 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_merge00 f1, f4, f1 */;
    /* ps_nmsub f9, f10, f30, f3 */;
    /* ps_msub f11, f8, f29, f3 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f4 */;
    /* ps_merge00 f13, f13, f12 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_madd f10, f12, f27, f9 */;
    /* ps_merge11 f8, f4, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_sub f11, f11, f10 */;
    /* ps_add f12, f1, f8 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f11, f10, f11 */;
    /* ps_sub f4, f1, f8 */;
    /* ps_mul f7, f7, f6 */;
    /* ps_add f10, f13, f11 */;
    /* ps_sub f9, f13, f11 */;
    /* ps_merge10 f10, f10, f10 */;
    /* psq_stu f12, 0x8(r10), 0, qr0 */;
    /* ps_merge10 f4, f4, f4 */;
    /* psq_stu f9, 0x8(r10), 0, qr0 */;
    /* psq_stu f10, 0x8(r10), 0, qr0 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r10), 0, qr0 */;
    if (--ctr != 0) goto L_801E8E34;
L_801E9048: ;
    r8 = *(u32*)lbl_8047B560;
    r10 = r31;
    r0 = *(u32*)lbl_8047B580;
    /* psq_l f7, 0x0(r10), 0, qr0 */;
    r7 = r0 << 3;
    /* psq_l f6, 0x80(r10), 0, qr0 */;
    r9 = r9 << 2;
    /* psq_l f5, 0x40(r10), 0, qr0 */;
    r6 = r0 << 2;
    /* ps_add f9, f7, f6 */;
    r7 = r7 + r9;
    /* psq_l f4, 0xc0(r10), 0, qr0 */;
    /* ps_sub f3, f7, f6 */;
    r6 = r7 + r6;
    /* ps_add f9, f9, f31 */;
    r0 = 0x3;
    /* ps_add f8, f5, f4 */;
    r5 = r8 + r7;
    /* ps_sub f2, f5, f4 */;
    /* ps_add f4, f9, f8 */;
    r4 = r8 + r6;
    /* ps_add f3, f3, f31 */;
    ctr_fn = (void(*)(void))r0;
L_801E90A4: ;
    /* psq_l f11, 0x20(r10), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r10), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r10), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r10), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    r10 = r10 + 0x8;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* psq_l f7, 0x0(r10), 0, qr0 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* psq_l f6, 0x80(r10), 0, qr0 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* psq_l f5, 0x40(r10), 0, qr0 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_l f4, 0xc0(r10), 0, qr0 */;
    /* ps_sub f9, f9, f8 */;
    r7 = r7 + 0x2;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    r6 = r6 + 0x2;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* ps_add f9, f7, f6 */;
    /* psq_st f3, 0x0(r4), 0, qr6 */;
    /* ps_sub f3, f7, f6 */;
    /* ps_add f9, f9, f31 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    /* ps_add f8, f5, f4 */;
    /* ps_sub f2, f5, f4 */;
    /* psq_st f1, 0x10(r4), 0, qr6 */;
    r5 = r8 + r7;
    /* ps_add f4, f9, f8 */;
    /* psq_st f0, 0x18(r4), 0, qr6 */;
    /* ps_add f3, f3, f31 */;
    r4 = r8 + r6;
    if (--ctr != 0) goto L_801E90A4;
    /* psq_l f11, 0x20(r10), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r10), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r10), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r10), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_sub f9, f9, f8 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* psq_st f0, 0x18(r4), 0, qr6 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    /* ps_sub f2, f12, f11 */;
    /* psq_st f1, 0x10(r4), 0, qr6 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* psq_st f3, 0x0(r4), 0, qr6 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    r4 = *(u32*)((u8*)r31 + 0x110);
    r0 = 0x140;
    r5 = *(u32*)lbl_8047B5B0;
    *(u32*)lbl_8047B560 = r4;
    r3 = (u32)r3 >> 1;
    /* subi r9, r31, 0x8 */;
    *(u32*)lbl_8047B580 = r0;
    r0 = *(u8*)((u8*)r5 + 0x686);
    r0 = r0 << 8;
    r0 = r5 + r0;
    *(u32*)lbl_8047B5A0 = r0;
    r8 = *(u32*)((u8*)r31 + 0x128);
    r7 = *(u32*)lbl_8047B5A0;
    r4 = 0x8;
    ctr_fn = (void(*)(void))r4;
L_801E924C: ;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_mul f7, f7, f6 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* or. r6, r6, r0 */;
L_801E926C: ;
    if ((s32)r6 != (s32)0x0) goto L_801E93B8;
    /* ps_merge00 f4, f7, f7 */;
    /* psq_st f4, 0x8(r9), 0, qr0 */;
    if ((s32)r5 != (s32)0x0) goto L_801E931C;
    /* psq_st f4, 0x10(r9), 0, qr0 */;
    /* psq_st f4, 0x18(r9), 0, qr0 */;
    if ((s32)r4 != (s32)0x0) goto L_801E92A8;
    r7 = r7 + 0x20;
    /* psq_stu f4, 0x20(r9), 0, qr0 */;
    r8 = r8 + 0x10;
    if (--ctr != 0) goto L_801E924C;
    goto L_801E9480;
L_801E92A8: ;
    r8 = r8 + 0x10;
    /* ps_msub f13, f7, f28, f7 */;
    r7 = r7 + 0x20;
    /* ps_merge00 f2, f7, f7 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f1, f28, f29 */;
    /* ps_msub f12, f7, f27, f13 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f10, f7, f13 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_nmsub f11, f7, f1, f12 */;
    /* ps_add f8, f2, f10 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_merge11 f9, f12, f11 */;
    /* ps_sub f10, f2, f10 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_add f3, f2, f9 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_sub f9, f2, f9 */;
    /* psq_stu f8, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f9, f9, f9 */;
    /* psq_stu f3, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f10, f10, f10 */;
    r6 = r6 | r0;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E926C;
    goto L_801E9480;
L_801E931C: ;
    /* psq_l f1, 0x4(r8), 0, qr5 */;
    /* psq_l f2, 0x8(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_mul f1, f1, f2 */;
    r7 = r7 + 0x20;
    /* ps_sub f12, f7, f1 */;
    /* ps_add f13, f7, f1 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_madd f11, f1, f27, f12 */;
    /* ps_nmsub f10, f1, f27, f13 */;
    /* ps_mul f3, f12, f28 */;
    /* ps_merge00 f11, f13, f11 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_nmsub f9, f1, f30, f3 */;
    /* ps_merge00 f10, f10, f12 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f13 */;
    /* ps_nmsub f8, f7, f29, f3 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f13, f13, f9 */;
    /* ps_msub f3, f12, f27, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_add f2, f11, f13 */;
    /* ps_sub f8, f8, f3 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f12, f3, f8 */;
    /* ps_sub f11, f11, f13 */;
    /* psq_stu f2, 0x8(r9), 0, qr0 */;
    /* ps_add f4, f10, f12 */;
    /* ps_sub f1, f10, f12 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f1, f1, f1 */;
    /* ps_merge10 f11, f11, f11 */;
    /* psq_stu f1, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f11, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E926C;
    goto L_801E9480;
L_801E93B8: ;
    /* psq_l f2, 0x4(r8), 0, qr5 */;
    /* psq_l f10, 0x8(r7), 0, qr0 */;
    /* ps_mul f2, f2, f10 */;
    /* psq_l f13, 0x8(r8), 0, qr5 */;
    /* psq_l f9, 0x10(r7), 0, qr0 */;
    /* ps_merge01 f4, f7, f2 */;
    /* psq_l f12, 0xc(r8), 0, qr5 */;
    /* ps_merge01 f1, f2, f7 */;
    /* psq_l f8, 0x18(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_madd f11, f13, f9, f4 */;
    /* ps_nmsub f10, f13, f9, f4 */;
    /* ps_madd f9, f12, f8, f1 */;
    /* ps_nmsub f8, f12, f8, f1 */;
    r7 = r7 + 0x20;
    /* ps_add f4, f11, f9 */;
    /* ps_sub f12, f11, f9 */;
    /* ps_msub f13, f8, f27, f9 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_sub f3, f8, f10 */;
    /* ps_add f1, f10, f13 */;
    /* ps_sub f13, f10, f13 */;
    /* ps_mul f3, f3, f28 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_merge00 f1, f4, f1 */;
    /* ps_nmsub f9, f10, f30, f3 */;
    /* ps_msub f11, f8, f29, f3 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f4 */;
    /* ps_merge00 f13, f13, f12 */;
    r4 = *(u16*)((u8*)r8 + 0x2);
    /* ps_madd f10, f12, f27, f9 */;
    /* ps_merge11 f8, f4, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_sub f11, f11, f10 */;
    /* ps_add f12, f1, f8 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f11, f10, f11 */;
    /* ps_sub f4, f1, f8 */;
    /* ps_mul f7, f7, f6 */;
    /* ps_add f10, f13, f11 */;
    /* ps_sub f9, f13, f11 */;
    /* ps_merge10 f10, f10, f10 */;
    /* psq_stu f12, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f4, f4, f4 */;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801E926C;
L_801E9480: ;
    r8 = *(u32*)lbl_8047B560;
    r9 = r31;
    r0 = *(u32*)lbl_8047B580;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    r4 = r3 << 2;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    r6 = r0 << 2;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    r7 = r4;
    /* ps_add f9, f7, f6 */;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f3, f7, f6 */;
    r6 = r7 + r6;
    /* ps_add f9, f9, f31 */;
    r0 = 0x3;
    /* ps_add f8, f5, f4 */;
    r5 = r8 + r7;
    /* ps_sub f2, f5, f4 */;
    /* ps_add f4, f9, f8 */;
    r4 = r8 + r6;
    /* ps_add f3, f3, f31 */;
    ctr_fn = (void(*)(void))r0;
L_801E94D8: ;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    r9 = r9 + 0x8;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f9, f9, f8 */;
    r7 = r7 + 0x2;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    r6 = r6 + 0x2;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* ps_add f9, f7, f6 */;
    /* psq_st f3, 0x0(r4), 0, qr6 */;
    /* ps_sub f3, f7, f6 */;
    /* ps_add f9, f9, f31 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    /* ps_add f8, f5, f4 */;
    /* ps_sub f2, f5, f4 */;
    /* psq_st f1, 0x10(r4), 0, qr6 */;
    r5 = r8 + r7;
    /* ps_add f4, f9, f8 */;
    /* psq_st f0, 0x18(r4), 0, qr6 */;
    /* ps_add f3, f3, f31 */;
    r4 = r8 + r6;
    if (--ctr != 0) goto L_801E94D8;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_sub f9, f9, f8 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* psq_st f0, 0x18(r4), 0, qr6 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f1, 0x10(r4), 0, qr6 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* psq_st f3, 0x0(r4), 0, qr6 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    r0 = *(u32*)((u8*)r31 + 0x114);
    r4 = *(u32*)lbl_8047B5B0;
    /* subi r8, r31, 0x8 */;
    *(u32*)lbl_8047B560 = r0;
    r0 = *(u8*)((u8*)r4 + 0x68C);
    r0 = r0 << 8;
    r0 = r4 + r0;
    *(u32*)lbl_8047B5A0 = r0;
    r9 = *(u32*)((u8*)r31 + 0x12C);
    r7 = *(u32*)lbl_8047B5A0;
    r4 = 0x8;
    ctr_fn = (void(*)(void))r4;
L_801E9674: ;
    /* psq_l f7, 0x0(r9), 0, qr5 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    r6 = *(u32*)((u8*)r9 + 0xC);
    r0 = *(u32*)((u8*)r9 + 0x8);
    /* ps_mul f7, f7, f6 */;
    r5 = *(u32*)((u8*)r9 + 0x4);
    r4 = *(u16*)((u8*)r9 + 0x2);
    /* or. r6, r6, r0 */;
L_801E9694: ;
    if ((s32)r6 != (s32)0x0) goto L_801E97E0;
    /* ps_merge00 f4, f7, f7 */;
    /* psq_st f4, 0x8(r8), 0, qr0 */;
    if ((s32)r5 != (s32)0x0) goto L_801E9744;
    /* psq_st f4, 0x10(r8), 0, qr0 */;
    /* psq_st f4, 0x18(r8), 0, qr0 */;
    if ((s32)r4 != (s32)0x0) goto L_801E96D0;
    r7 = r7 + 0x20;
    /* psq_stu f4, 0x20(r8), 0, qr0 */;
    r9 = r9 + 0x10;
    if (--ctr != 0) goto L_801E9674;
    goto L_801E98A8;
L_801E96D0: ;
    r9 = r9 + 0x10;
    /* ps_msub f13, f7, f28, f7 */;
    r7 = r7 + 0x20;
    /* ps_merge00 f2, f7, f7 */;
    r5 = *(u32*)((u8*)r9 + 0x4);
    /* ps_sub f1, f28, f29 */;
    /* ps_msub f12, f7, f27, f13 */;
    r4 = *(u16*)((u8*)r9 + 0x2);
    /* ps_merge11 f10, f7, f13 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_nmsub f11, f7, f1, f12 */;
    /* ps_add f8, f2, f10 */;
    /* psq_l f7, 0x0(r9), 0, qr5 */;
    /* ps_merge11 f9, f12, f11 */;
    /* ps_sub f10, f2, f10 */;
    r6 = *(u32*)((u8*)r9 + 0xC);
    /* ps_add f3, f2, f9 */;
    r0 = *(u32*)((u8*)r9 + 0x8);
    /* ps_sub f9, f2, f9 */;
    /* psq_stu f8, 0x8(r8), 0, qr0 */;
    /* ps_merge10 f9, f9, f9 */;
    /* psq_stu f3, 0x8(r8), 0, qr0 */;
    /* ps_merge10 f10, f10, f10 */;
    r6 = r6 | r0;
    /* psq_stu f9, 0x8(r8), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f10, 0x8(r8), 0, qr0 */;
    if (--ctr != 0) goto L_801E9694;
    goto L_801E98A8;
L_801E9744: ;
    /* psq_l f1, 0x4(r9), 0, qr5 */;
    /* psq_l f2, 0x8(r7), 0, qr0 */;
    r9 = r9 + 0x10;
    /* ps_mul f1, f1, f2 */;
    r7 = r7 + 0x20;
    /* ps_sub f12, f7, f1 */;
    /* ps_add f13, f7, f1 */;
    r6 = *(u32*)((u8*)r9 + 0xC);
    /* ps_madd f11, f1, f27, f12 */;
    /* ps_nmsub f10, f1, f27, f13 */;
    /* ps_mul f3, f12, f28 */;
    /* ps_merge00 f11, f13, f11 */;
    r0 = *(u32*)((u8*)r9 + 0x8);
    /* ps_nmsub f9, f1, f30, f3 */;
    /* ps_merge00 f10, f10, f12 */;
    r5 = *(u32*)((u8*)r9 + 0x4);
    /* ps_sub f9, f9, f13 */;
    /* ps_nmsub f8, f7, f29, f3 */;
    r4 = *(u16*)((u8*)r9 + 0x2);
    /* ps_merge11 f13, f13, f9 */;
    /* ps_msub f3, f12, f27, f9 */;
    /* psq_l f7, 0x0(r9), 0, qr5 */;
    /* ps_add f2, f11, f13 */;
    /* ps_sub f8, f8, f3 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f12, f3, f8 */;
    /* ps_sub f11, f11, f13 */;
    /* psq_stu f2, 0x8(r8), 0, qr0 */;
    /* ps_add f4, f10, f12 */;
    /* ps_sub f1, f10, f12 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r8), 0, qr0 */;
    /* ps_merge10 f1, f1, f1 */;
    /* ps_merge10 f11, f11, f11 */;
    /* psq_stu f1, 0x8(r8), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f11, 0x8(r8), 0, qr0 */;
    if (--ctr != 0) goto L_801E9694;
    goto L_801E98A8;
L_801E97E0: ;
    /* psq_l f2, 0x4(r9), 0, qr5 */;
    /* psq_l f10, 0x8(r7), 0, qr0 */;
    /* ps_mul f2, f2, f10 */;
    /* psq_l f13, 0x8(r9), 0, qr5 */;
    /* psq_l f9, 0x10(r7), 0, qr0 */;
    /* ps_merge01 f4, f7, f2 */;
    /* psq_l f12, 0xc(r9), 0, qr5 */;
    /* ps_merge01 f1, f2, f7 */;
    /* psq_l f8, 0x18(r7), 0, qr0 */;
    r9 = r9 + 0x10;
    /* ps_madd f11, f13, f9, f4 */;
    /* ps_nmsub f10, f13, f9, f4 */;
    /* ps_madd f9, f12, f8, f1 */;
    /* ps_nmsub f8, f12, f8, f1 */;
    r7 = r7 + 0x20;
    /* ps_add f4, f11, f9 */;
    /* ps_sub f12, f11, f9 */;
    /* ps_msub f13, f8, f27, f9 */;
    r6 = *(u32*)((u8*)r9 + 0xC);
    /* ps_sub f3, f8, f10 */;
    /* ps_add f1, f10, f13 */;
    /* ps_sub f13, f10, f13 */;
    /* ps_mul f3, f3, f28 */;
    r0 = *(u32*)((u8*)r9 + 0x8);
    /* ps_merge00 f1, f4, f1 */;
    /* ps_nmsub f9, f10, f30, f3 */;
    /* ps_msub f11, f8, f29, f3 */;
    r5 = *(u32*)((u8*)r9 + 0x4);
    /* ps_sub f9, f9, f4 */;
    /* ps_merge00 f13, f13, f12 */;
    r4 = *(u16*)((u8*)r9 + 0x2);
    /* ps_madd f10, f12, f27, f9 */;
    /* ps_merge11 f8, f4, f9 */;
    /* psq_l f7, 0x0(r9), 0, qr5 */;
    /* ps_sub f11, f11, f10 */;
    /* ps_add f12, f1, f8 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f11, f10, f11 */;
    /* ps_sub f4, f1, f8 */;
    /* ps_mul f7, f7, f6 */;
    /* ps_add f10, f13, f11 */;
    /* ps_sub f9, f13, f11 */;
    /* ps_merge10 f10, f10, f10 */;
    /* psq_stu f12, 0x8(r8), 0, qr0 */;
    /* ps_merge10 f4, f4, f4 */;
    /* psq_stu f9, 0x8(r8), 0, qr0 */;
    /* psq_stu f10, 0x8(r8), 0, qr0 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r8), 0, qr0 */;
    if (--ctr != 0) goto L_801E9694;
L_801E98A8: ;
    r7 = *(u32*)lbl_8047B560;
    r8 = r31;
    r0 = *(u32*)lbl_8047B580;
    /* psq_l f7, 0x0(r8), 0, qr0 */;
    r3 = r3 << 2;
    /* psq_l f6, 0x80(r8), 0, qr0 */;
    r5 = r0 << 2;
    /* psq_l f5, 0x40(r8), 0, qr0 */;
    r6 = r3;
    /* ps_add f9, f7, f6 */;
    /* psq_l f4, 0xc0(r8), 0, qr0 */;
    /* ps_sub f3, f7, f6 */;
    r5 = r6 + r5;
    /* ps_add f9, f9, f31 */;
    r0 = 0x3;
    /* ps_add f8, f5, f4 */;
    r4 = r7 + r6;
    /* ps_sub f2, f5, f4 */;
    /* ps_add f4, f9, f8 */;
    r3 = r7 + r5;
    /* ps_add f3, f3, f31 */;
    ctr_fn = (void(*)(void))r0;
L_801E9900: ;
    /* psq_l f11, 0x20(r8), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r8), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r8), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r8), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    r8 = r8 + 0x8;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* psq_l f7, 0x0(r8), 0, qr0 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* psq_l f6, 0x80(r8), 0, qr0 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* psq_l f5, 0x40(r8), 0, qr0 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_l f4, 0xc0(r8), 0, qr0 */;
    /* ps_sub f9, f9, f8 */;
    r6 = r6 + 0x2;
    /* psq_st f2, 0x0(r4), 0, qr6 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r4), 0, qr6 */;
    r5 = r5 + 0x2;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r4), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* ps_add f9, f7, f6 */;
    /* psq_st f3, 0x0(r3), 0, qr6 */;
    /* ps_sub f3, f7, f6 */;
    /* ps_add f9, f9, f31 */;
    /* psq_st f2, 0x8(r3), 0, qr6 */;
    /* ps_add f8, f5, f4 */;
    /* ps_sub f2, f5, f4 */;
    /* psq_st f1, 0x10(r3), 0, qr6 */;
    r4 = r7 + r6;
    /* ps_add f4, f9, f8 */;
    /* psq_st f0, 0x18(r3), 0, qr6 */;
    /* ps_add f3, f3, f31 */;
    r3 = r7 + r5;
    if (--ctr != 0) goto L_801E9900;
    /* psq_l f11, 0x20(r8), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r8), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r8), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r8), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_st f2, 0x0(r4), 0, qr6 */;
    /* ps_sub f9, f9, f8 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* psq_st f0, 0x18(r3), 0, qr6 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f1, 0x10(r3), 0, qr6 */;
    /* psq_st f3, 0x10(r4), 0, qr6 */;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r4), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* psq_st f3, 0x0(r3), 0, qr6 */;
    /* psq_st f2, 0x8(r3), 0, qr6 */;
    r4 = *(u32*)lbl_8047B5B0;
    r0 = *(u8*)((u8*)r4 + 0x6A9);
    if ((u32)r0 == (u32)0x0) goto L_801E9AEC;
    r3 = *(u16*)((u8*)r4 + 0x6AC);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r4 + 0x6AC) = r0;
    r3 = *(u32*)lbl_8047B5B0;
    r0 = *(u16*)((u8*)r3 + 0x6AC);
    if ((u32)r0 != (u32)0x0) goto L_801E9AEC;
    r0 = *(u16*)((u8*)r3 + 0x6AA);
    *(u16*)((u8*)r3 + 0x6AC) = r0;
    r4 = *(u32*)lbl_8047B5B0;
    r3 = *(u32*)((u8*)r4 + 0x6A4);
    r0 = r3 + 0x6;
    /* clrrwi r3, r0, 3 */;
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x6A4) = r0;
    r3 = *(u32*)lbl_8047B5B0;
    r0 = *(u32*)((u8*)r3 + 0x6A4);
    if ((u32)r0 <= (u32)0x20) goto L_801E9AD0;
    r0 = 0x21;
    *(u32*)((u8*)r3 + 0x6A4) = r0;
L_801E9AD0: ;
    r3 = *(u32*)lbl_8047B5B0;
    r0 = 0x0;
    *(u16*)((u8*)r3 + 0x684) = r0;
    r3 = *(u32*)lbl_8047B5B0;
    *(u16*)((u8*)r3 + 0x68A) = r0;
    r3 = *(u32*)lbl_8047B5B0;
    *(u16*)((u8*)r3 + 0x690) = r0;
L_801E9AEC: ;
    r30 = r30 + 0x1;
L_801E9AF0: ;
    r3 = *(u32*)lbl_8047B5B0;
    r4 = r30 & 0xFF;
    r0 = *(u16*)((u8*)r3 + 0x696);
    if ((s32)r4 < (s32)r0) goto L_801E8160;
    r3 = *(u32*)((u8*)r3 + 0x6B0);
    r5 = 0x2800;
    r4 = *(u32*)((u8*)r31 + 0x10C);
    fn_8009B55C();
    r3 = *(u32*)lbl_8047B5B0;
    r5 = 0xa00;
    r4 = *(u32*)((u8*)r31 + 0x110);
    r3 = *(u32*)((u8*)r3 + 0x6B4);
    fn_8009B55C();
    r3 = *(u32*)lbl_8047B5B0;
    r5 = 0xa00;
    r4 = *(u32*)((u8*)r31 + 0x114);
    r3 = *(u32*)((u8*)r3 + 0x6B8);
    fn_8009B55C();
    r4 = *(u32*)lbl_8047B5B0;
    r3 = *(u32*)((u8*)r4 + 0x6B0);
    r0 = r3 + 0x2800;
    *(u32*)((u8*)r4 + 0x6B0) = r0;
    r4 = *(u32*)lbl_8047B5B0;
    r3 = *(u32*)((u8*)r4 + 0x6B4);
    r0 = r3 + 0xa00;
    *(u32*)((u8*)r4 + 0x6B4) = r0;
    r4 = *(u32*)lbl_8047B5B0;
    r3 = *(u32*)((u8*)r4 + 0x6B8);
    r0 = r3 + 0xa00;
    *(u32*)((u8*)r4 + 0x6B8) = r0;
    f31 = *(f64*)((u8*)r1 + 0x30);
    f30 = *(f64*)((u8*)r1 + 0x28);
    f29 = *(f64*)((u8*)r1 + 0x20);
    f28 = *(f64*)((u8*)r1 + 0x18);
    f27 = *(f64*)((u8*)r1 + 0x10);
    return;
}
#pragma pop

/* 0x801E9B98 | size: 0x1AAC | massive */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801E9B98(void) {
    extern u8 lbl_8046D500[];
    extern u8 lbl_8047B560[];
    extern u8 lbl_8047B580[];
    extern u8 lbl_8047B5A0[];
    extern u8 lbl_8047B5B0[];
    extern u8 lbl_8047E4B8[];
    extern u8 lbl_8047E4BC[];
    extern u8 lbl_8047E4C0[];
    extern u8 lbl_8047E4C4[];
    extern u8 lbl_8047E4C8[];
    extern void fn_8009B55C();
    extern void fn_8009B614();
    extern void fn_801EB644();
    extern void fn_801EBCC0();
    extern void fn_801EC368();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    f32 f9 = 0.0f;
    f32 f10 = 0.0f;
    f32 f11 = 0.0f;
    f32 f12 = 0.0f;
    f32 f13 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;
    u8 sp[0x100];

    r4 = (u32)lbl_8046D500;
    *(f64*)(sp + 0x38) = f31;
    *(f64*)(sp + 0x30) = f30;
    *(f64*)(sp + 0x28) = f29;
    *(f64*)(sp + 0x20) = f28;
    *(f64*)(sp + 0x18) = f27;
    r30 = (u32)lbl_8046D500;
    r3 = *(u32*)lbl_8047B5B0;
    r28 = *(u16*)((u8*)r3 + 0x692);
    r3 = 0x3;
    fn_8009B614();
    f27 = *(f32*)lbl_8047E4B8;
    r31 = (u32)r28 >> 1;
    f28 = *(f32*)lbl_8047E4BC;
    r29 = 0x0;
    f29 = *(f32*)lbl_8047E4C0;
    f30 = *(f32*)lbl_8047E4C4;
    f31 = *(f32*)lbl_8047E4C8;
    goto L_801EB588;
L_801E9C00: ;
    r3 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r30 + 0x118);
    fn_801EB644();
    r3 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r30 + 0x11C);
    fn_801EB644();
    r3 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r30 + 0x120);
    fn_801EB644();
    r3 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r30 + 0x124);
    fn_801EB644();
    r3 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r30 + 0x128);
    fn_801EBCC0();
    r3 = *(u32*)lbl_8047B5B0;
    r4 = *(u32*)((u8*)r30 + 0x12C);
    fn_801EC368();
    r0 = *(u32*)((u8*)r30 + 0x10C);
    /* clrlslwi r4, r29, 24, 4 */;
    r5 = *(u32*)lbl_8047B5B0;
    /* subi r3, r30, 0x8 */;
    *(u32*)lbl_8047B560 = r0;
    *(u32*)lbl_8047B580 = r28;
    r0 = *(u8*)((u8*)r5 + 0x680);
    r0 = r0 << 8;
    r0 = r5 + r0;
    *(u32*)lbl_8047B5A0 = r0;
    r9 = *(u32*)((u8*)r30 + 0x118);
    r8 = *(u32*)lbl_8047B5A0;
    r5 = 0x8;
    ctr_fn = (void(*)(void))r5;
L_801E9C80: ;
    /* psq_l f7, 0x0(r9), 0, qr5 */;
    /* psq_l f6, 0x0(r8), 0, qr0 */;
    r7 = *(u32*)((u8*)r9 + 0xC);
    r0 = *(u32*)((u8*)r9 + 0x8);
    /* ps_mul f7, f7, f6 */;
    r6 = *(u32*)((u8*)r9 + 0x4);
    r5 = *(u16*)((u8*)r9 + 0x2);
    /* or. r7, r7, r0 */;
L_801E9CA0: ;
    if ((s32)r7 != (s32)0x0) goto L_801E9DEC;
    /* ps_merge00 f4, f7, f7 */;
    /* psq_st f4, 0x8(r3), 0, qr0 */;
    if ((s32)r6 != (s32)0x0) goto L_801E9D50;
    /* psq_st f4, 0x10(r3), 0, qr0 */;
    /* psq_st f4, 0x18(r3), 0, qr0 */;
    if ((s32)r5 != (s32)0x0) goto L_801E9CDC;
    r8 = r8 + 0x20;
    /* psq_stu f4, 0x20(r3), 0, qr0 */;
    r9 = r9 + 0x10;
    if (--ctr != 0) goto L_801E9C80;
    goto L_801E9EB4;
L_801E9CDC: ;
    r9 = r9 + 0x10;
    /* ps_msub f13, f7, f28, f7 */;
    r8 = r8 + 0x20;
    /* ps_merge00 f2, f7, f7 */;
    r6 = *(u32*)((u8*)r9 + 0x4);
    /* ps_sub f1, f28, f29 */;
    /* ps_msub f12, f7, f27, f13 */;
    r5 = *(u16*)((u8*)r9 + 0x2);
    /* ps_merge11 f10, f7, f13 */;
    /* psq_l f6, 0x0(r8), 0, qr0 */;
    /* ps_nmsub f11, f7, f1, f12 */;
    /* ps_add f8, f2, f10 */;
    /* psq_l f7, 0x0(r9), 0, qr5 */;
    /* ps_merge11 f9, f12, f11 */;
    /* ps_sub f10, f2, f10 */;
    r7 = *(u32*)((u8*)r9 + 0xC);
    /* ps_add f3, f2, f9 */;
    r0 = *(u32*)((u8*)r9 + 0x8);
    /* ps_sub f9, f2, f9 */;
    /* psq_stu f8, 0x8(r3), 0, qr0 */;
    /* ps_merge10 f9, f9, f9 */;
    /* psq_stu f3, 0x8(r3), 0, qr0 */;
    /* ps_merge10 f10, f10, f10 */;
    r7 = r7 | r0;
    /* psq_stu f9, 0x8(r3), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f10, 0x8(r3), 0, qr0 */;
    if (--ctr != 0) goto L_801E9CA0;
    goto L_801E9EB4;
L_801E9D50: ;
    /* psq_l f1, 0x4(r9), 0, qr5 */;
    /* psq_l f2, 0x8(r8), 0, qr0 */;
    r9 = r9 + 0x10;
    /* ps_mul f1, f1, f2 */;
    r8 = r8 + 0x20;
    /* ps_sub f12, f7, f1 */;
    /* ps_add f13, f7, f1 */;
    r7 = *(u32*)((u8*)r9 + 0xC);
    /* ps_madd f11, f1, f27, f12 */;
    /* ps_nmsub f10, f1, f27, f13 */;
    /* ps_mul f3, f12, f28 */;
    /* ps_merge00 f11, f13, f11 */;
    r0 = *(u32*)((u8*)r9 + 0x8);
    /* ps_nmsub f9, f1, f30, f3 */;
    /* ps_merge00 f10, f10, f12 */;
    r6 = *(u32*)((u8*)r9 + 0x4);
    /* ps_sub f9, f9, f13 */;
    /* ps_nmsub f8, f7, f29, f3 */;
    r5 = *(u16*)((u8*)r9 + 0x2);
    /* ps_merge11 f13, f13, f9 */;
    /* ps_msub f3, f12, f27, f9 */;
    /* psq_l f7, 0x0(r9), 0, qr5 */;
    /* ps_add f2, f11, f13 */;
    /* ps_sub f8, f8, f3 */;
    /* psq_l f6, 0x0(r8), 0, qr0 */;
    /* ps_merge11 f12, f3, f8 */;
    /* ps_sub f11, f11, f13 */;
    /* psq_stu f2, 0x8(r3), 0, qr0 */;
    /* ps_add f4, f10, f12 */;
    /* ps_sub f1, f10, f12 */;
    r7 = r7 | r0;
    /* psq_stu f4, 0x8(r3), 0, qr0 */;
    /* ps_merge10 f1, f1, f1 */;
    /* ps_merge10 f11, f11, f11 */;
    /* psq_stu f1, 0x8(r3), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f11, 0x8(r3), 0, qr0 */;
    if (--ctr != 0) goto L_801E9CA0;
    goto L_801E9EB4;
L_801E9DEC: ;
    /* psq_l f2, 0x4(r9), 0, qr5 */;
    /* psq_l f10, 0x8(r8), 0, qr0 */;
    /* ps_mul f2, f2, f10 */;
    /* psq_l f13, 0x8(r9), 0, qr5 */;
    /* psq_l f9, 0x10(r8), 0, qr0 */;
    /* ps_merge01 f4, f7, f2 */;
    /* psq_l f12, 0xc(r9), 0, qr5 */;
    /* ps_merge01 f1, f2, f7 */;
    /* psq_l f8, 0x18(r8), 0, qr0 */;
    r9 = r9 + 0x10;
    /* ps_madd f11, f13, f9, f4 */;
    /* ps_nmsub f10, f13, f9, f4 */;
    /* ps_madd f9, f12, f8, f1 */;
    /* ps_nmsub f8, f12, f8, f1 */;
    r8 = r8 + 0x20;
    /* ps_add f4, f11, f9 */;
    /* ps_sub f12, f11, f9 */;
    /* ps_msub f13, f8, f27, f9 */;
    r7 = *(u32*)((u8*)r9 + 0xC);
    /* ps_sub f3, f8, f10 */;
    /* ps_add f1, f10, f13 */;
    /* ps_sub f13, f10, f13 */;
    /* ps_mul f3, f3, f28 */;
    r0 = *(u32*)((u8*)r9 + 0x8);
    /* ps_merge00 f1, f4, f1 */;
    /* ps_nmsub f9, f10, f30, f3 */;
    /* ps_msub f11, f8, f29, f3 */;
    r6 = *(u32*)((u8*)r9 + 0x4);
    /* ps_sub f9, f9, f4 */;
    /* ps_merge00 f13, f13, f12 */;
    r5 = *(u16*)((u8*)r9 + 0x2);
    /* ps_madd f10, f12, f27, f9 */;
    /* ps_merge11 f8, f4, f9 */;
    /* psq_l f7, 0x0(r9), 0, qr5 */;
    /* ps_sub f11, f11, f10 */;
    /* ps_add f12, f1, f8 */;
    /* psq_l f6, 0x0(r8), 0, qr0 */;
    /* ps_merge11 f11, f10, f11 */;
    /* ps_sub f4, f1, f8 */;
    /* ps_mul f7, f7, f6 */;
    /* ps_add f10, f13, f11 */;
    /* ps_sub f9, f13, f11 */;
    /* ps_merge10 f10, f10, f10 */;
    /* psq_stu f12, 0x8(r3), 0, qr0 */;
    /* ps_merge10 f4, f4, f4 */;
    /* psq_stu f9, 0x8(r3), 0, qr0 */;
    /* psq_stu f10, 0x8(r3), 0, qr0 */;
    r7 = r7 | r0;
    /* psq_stu f4, 0x8(r3), 0, qr0 */;
    if (--ctr != 0) goto L_801E9CA0;
L_801E9EB4: ;
    r9 = *(u32*)lbl_8047B560;
    r3 = r30;
    r0 = *(u32*)lbl_8047B580;
    /* psq_l f7, 0x0(r3), 0, qr0 */;
    r5 = r4 << 2;
    /* psq_l f6, 0x80(r3), 0, qr0 */;
    r7 = r0 << 2;
    /* psq_l f5, 0x40(r3), 0, qr0 */;
    r8 = r5;
    /* ps_add f9, f7, f6 */;
    /* psq_l f4, 0xc0(r3), 0, qr0 */;
    /* ps_sub f3, f7, f6 */;
    r7 = r8 + r7;
    /* ps_add f9, f9, f31 */;
    r0 = 0x3;
    /* ps_add f8, f5, f4 */;
    r6 = r9 + r8;
    /* ps_sub f2, f5, f4 */;
    /* ps_add f4, f9, f8 */;
    r5 = r9 + r7;
    /* ps_add f3, f3, f31 */;
    ctr_fn = (void(*)(void))r0;
L_801E9F0C: ;
    /* psq_l f11, 0x20(r3), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r3), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r3), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r3), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    r3 = r3 + 0x8;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* psq_l f7, 0x0(r3), 0, qr0 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* psq_l f6, 0x80(r3), 0, qr0 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* psq_l f5, 0x40(r3), 0, qr0 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_l f4, 0xc0(r3), 0, qr0 */;
    /* ps_sub f9, f9, f8 */;
    r8 = r8 + 0x2;
    /* psq_st f2, 0x0(r6), 0, qr6 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r6), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r6), 0, qr6 */;
    r7 = r7 + 0x2;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r6), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* ps_add f9, f7, f6 */;
    /* psq_st f3, 0x0(r5), 0, qr6 */;
    /* ps_sub f3, f7, f6 */;
    /* ps_add f9, f9, f31 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f8, f5, f4 */;
    /* ps_sub f2, f5, f4 */;
    /* psq_st f1, 0x10(r5), 0, qr6 */;
    r6 = r9 + r8;
    /* ps_add f4, f9, f8 */;
    /* psq_st f0, 0x18(r5), 0, qr6 */;
    /* ps_add f3, f3, f31 */;
    r5 = r9 + r7;
    if (--ctr != 0) goto L_801E9F0C;
    /* psq_l f11, 0x20(r3), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r3), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r3), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r3), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_st f2, 0x0(r6), 0, qr6 */;
    /* ps_sub f9, f9, f8 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* psq_st f0, 0x18(r5), 0, qr6 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r6), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f1, 0x10(r5), 0, qr6 */;
    /* psq_st f3, 0x10(r6), 0, qr6 */;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r6), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* psq_st f3, 0x0(r5), 0, qr6 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    r8 = *(u32*)((u8*)r30 + 0x11C);
    r7 = *(u32*)lbl_8047B5A0;
    r10 = r4 + 0x8;
    /* subi r9, r30, 0x8 */;
    r3 = 0x8;
    ctr_fn = (void(*)(void))r3;
L_801EA090: ;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_mul f7, f7, f6 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    r3 = *(u16*)((u8*)r8 + 0x2);
    /* or. r6, r6, r0 */;
L_801EA0B0: ;
    if ((s32)r6 != (s32)0x0) goto L_801EA1FC;
    /* ps_merge00 f4, f7, f7 */;
    /* psq_st f4, 0x8(r9), 0, qr0 */;
    if ((s32)r5 != (s32)0x0) goto L_801EA160;
    /* psq_st f4, 0x10(r9), 0, qr0 */;
    /* psq_st f4, 0x18(r9), 0, qr0 */;
    if ((s32)r3 != (s32)0x0) goto L_801EA0EC;
    r7 = r7 + 0x20;
    /* psq_stu f4, 0x20(r9), 0, qr0 */;
    r8 = r8 + 0x10;
    if (--ctr != 0) goto L_801EA090;
    goto L_801EA2C4;
L_801EA0EC: ;
    r8 = r8 + 0x10;
    /* ps_msub f13, f7, f28, f7 */;
    r7 = r7 + 0x20;
    /* ps_merge00 f2, f7, f7 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f1, f28, f29 */;
    /* ps_msub f12, f7, f27, f13 */;
    r3 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f10, f7, f13 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_nmsub f11, f7, f1, f12 */;
    /* ps_add f8, f2, f10 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_merge11 f9, f12, f11 */;
    /* ps_sub f10, f2, f10 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_add f3, f2, f9 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_sub f9, f2, f9 */;
    /* psq_stu f8, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f9, f9, f9 */;
    /* psq_stu f3, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f10, f10, f10 */;
    r6 = r6 | r0;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801EA0B0;
    goto L_801EA2C4;
L_801EA160: ;
    /* psq_l f1, 0x4(r8), 0, qr5 */;
    /* psq_l f2, 0x8(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_mul f1, f1, f2 */;
    r7 = r7 + 0x20;
    /* ps_sub f12, f7, f1 */;
    /* ps_add f13, f7, f1 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_madd f11, f1, f27, f12 */;
    /* ps_nmsub f10, f1, f27, f13 */;
    /* ps_mul f3, f12, f28 */;
    /* ps_merge00 f11, f13, f11 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_nmsub f9, f1, f30, f3 */;
    /* ps_merge00 f10, f10, f12 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f13 */;
    /* ps_nmsub f8, f7, f29, f3 */;
    r3 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f13, f13, f9 */;
    /* ps_msub f3, f12, f27, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_add f2, f11, f13 */;
    /* ps_sub f8, f8, f3 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f12, f3, f8 */;
    /* ps_sub f11, f11, f13 */;
    /* psq_stu f2, 0x8(r9), 0, qr0 */;
    /* ps_add f4, f10, f12 */;
    /* ps_sub f1, f10, f12 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f1, f1, f1 */;
    /* ps_merge10 f11, f11, f11 */;
    /* psq_stu f1, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f11, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801EA0B0;
    goto L_801EA2C4;
L_801EA1FC: ;
    /* psq_l f2, 0x4(r8), 0, qr5 */;
    /* psq_l f10, 0x8(r7), 0, qr0 */;
    /* ps_mul f2, f2, f10 */;
    /* psq_l f13, 0x8(r8), 0, qr5 */;
    /* psq_l f9, 0x10(r7), 0, qr0 */;
    /* ps_merge01 f4, f7, f2 */;
    /* psq_l f12, 0xc(r8), 0, qr5 */;
    /* ps_merge01 f1, f2, f7 */;
    /* psq_l f8, 0x18(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_madd f11, f13, f9, f4 */;
    /* ps_nmsub f10, f13, f9, f4 */;
    /* ps_madd f9, f12, f8, f1 */;
    /* ps_nmsub f8, f12, f8, f1 */;
    r7 = r7 + 0x20;
    /* ps_add f4, f11, f9 */;
    /* ps_sub f12, f11, f9 */;
    /* ps_msub f13, f8, f27, f9 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_sub f3, f8, f10 */;
    /* ps_add f1, f10, f13 */;
    /* ps_sub f13, f10, f13 */;
    /* ps_mul f3, f3, f28 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_merge00 f1, f4, f1 */;
    /* ps_nmsub f9, f10, f30, f3 */;
    /* ps_msub f11, f8, f29, f3 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f4 */;
    /* ps_merge00 f13, f13, f12 */;
    r3 = *(u16*)((u8*)r8 + 0x2);
    /* ps_madd f10, f12, f27, f9 */;
    /* ps_merge11 f8, f4, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_sub f11, f11, f10 */;
    /* ps_add f12, f1, f8 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f11, f10, f11 */;
    /* ps_sub f4, f1, f8 */;
    /* ps_mul f7, f7, f6 */;
    /* ps_add f10, f13, f11 */;
    /* ps_sub f9, f13, f11 */;
    /* ps_merge10 f10, f10, f10 */;
    /* psq_stu f12, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f4, f4, f4 */;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801EA0B0;
L_801EA2C4: ;
    r8 = *(u32*)lbl_8047B560;
    r9 = r30;
    r0 = *(u32*)lbl_8047B580;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    r10 = r10 << 2;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    r6 = r0 << 2;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    r7 = r10;
    /* ps_add f9, f7, f6 */;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f3, f7, f6 */;
    r6 = r7 + r6;
    /* ps_add f9, f9, f31 */;
    r0 = 0x3;
    /* ps_add f8, f5, f4 */;
    r5 = r8 + r7;
    /* ps_sub f2, f5, f4 */;
    /* ps_add f4, f9, f8 */;
    r3 = r8 + r6;
    /* ps_add f3, f3, f31 */;
    ctr_fn = (void(*)(void))r0;
L_801EA31C: ;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    r9 = r9 + 0x8;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f9, f9, f8 */;
    r7 = r7 + 0x2;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    r6 = r6 + 0x2;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* ps_add f9, f7, f6 */;
    /* psq_st f3, 0x0(r3), 0, qr6 */;
    /* ps_sub f3, f7, f6 */;
    /* ps_add f9, f9, f31 */;
    /* psq_st f2, 0x8(r3), 0, qr6 */;
    /* ps_add f8, f5, f4 */;
    /* ps_sub f2, f5, f4 */;
    /* psq_st f1, 0x10(r3), 0, qr6 */;
    r5 = r8 + r7;
    /* ps_add f4, f9, f8 */;
    /* psq_st f0, 0x18(r3), 0, qr6 */;
    /* ps_add f3, f3, f31 */;
    r3 = r8 + r6;
    if (--ctr != 0) goto L_801EA31C;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_sub f9, f9, f8 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* psq_st f0, 0x18(r3), 0, qr6 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f1, 0x10(r3), 0, qr6 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* psq_st f3, 0x0(r3), 0, qr6 */;
    /* psq_st f2, 0x8(r3), 0, qr6 */;
    r8 = *(u32*)((u8*)r30 + 0x120);
    r7 = *(u32*)lbl_8047B5A0;
    /* subi r9, r30, 0x8 */;
    r3 = 0x8;
    ctr_fn = (void(*)(void))r3;
L_801EA49C: ;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_mul f7, f7, f6 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    r3 = *(u16*)((u8*)r8 + 0x2);
    r6 = r6 | r0;
L_801EA4BC: ;
    if ((s32)r6 != (s32)0x0) goto L_801EA608;
    /* ps_merge00 f4, f7, f7 */;
    /* psq_st f4, 0x8(r9), 0, qr0 */;
    if ((s32)r5 != (s32)0x0) goto L_801EA56C;
    /* psq_st f4, 0x10(r9), 0, qr0 */;
    /* psq_st f4, 0x18(r9), 0, qr0 */;
    if ((s32)r3 != (s32)0x0) goto L_801EA4F8;
    r7 = r7 + 0x20;
    /* psq_stu f4, 0x20(r9), 0, qr0 */;
    r8 = r8 + 0x10;
    if (--ctr != 0) goto L_801EA49C;
    goto L_801EA6D0;
L_801EA4F8: ;
    /* ps_msub f13, f7, f28, f7 */;
    r8 = r8 + 0x10;
    /* ps_merge00 f2, f7, f7 */;
    r7 = r7 + 0x20;
    /* ps_sub f1, f28, f29 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_msub f12, f7, f27, f13 */;
    r3 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f10, f7, f13 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_nmsub f11, f7, f1, f12 */;
    /* ps_add f8, f2, f10 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_merge11 f9, f12, f11 */;
    /* ps_sub f10, f2, f10 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_add f3, f2, f9 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_sub f9, f2, f9 */;
    /* psq_stu f8, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f9, f9, f9 */;
    /* psq_stu f3, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f10, f10, f10 */;
    r6 = r6 | r0;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801EA4BC;
    goto L_801EA6D0;
L_801EA56C: ;
    /* psq_l f1, 0x4(r8), 0, qr5 */;
    /* psq_l f2, 0x8(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_mul f1, f1, f2 */;
    r7 = r7 + 0x20;
    /* ps_sub f12, f7, f1 */;
    /* ps_add f13, f7, f1 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_madd f11, f1, f27, f12 */;
    /* ps_nmsub f10, f1, f27, f13 */;
    /* ps_mul f3, f12, f28 */;
    /* ps_merge00 f11, f13, f11 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_nmsub f9, f1, f30, f3 */;
    /* ps_merge00 f10, f10, f12 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f13 */;
    /* ps_nmsub f8, f7, f29, f3 */;
    r3 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f13, f13, f9 */;
    /* ps_msub f3, f12, f27, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_add f2, f11, f13 */;
    /* ps_sub f8, f8, f3 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f12, f3, f8 */;
    /* ps_sub f11, f11, f13 */;
    /* psq_stu f2, 0x8(r9), 0, qr0 */;
    /* ps_add f4, f10, f12 */;
    /* ps_sub f1, f10, f12 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f1, f1, f1 */;
    /* ps_merge10 f11, f11, f11 */;
    /* psq_stu f1, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f11, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801EA4BC;
    goto L_801EA6D0;
L_801EA608: ;
    /* psq_l f2, 0x4(r8), 0, qr5 */;
    /* psq_l f10, 0x8(r7), 0, qr0 */;
    /* ps_mul f2, f2, f10 */;
    /* psq_l f13, 0x8(r8), 0, qr5 */;
    /* psq_l f9, 0x10(r7), 0, qr0 */;
    /* ps_merge01 f4, f7, f2 */;
    /* psq_l f12, 0xc(r8), 0, qr5 */;
    /* ps_merge01 f1, f2, f7 */;
    /* psq_l f8, 0x18(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_madd f11, f13, f9, f4 */;
    /* ps_nmsub f10, f13, f9, f4 */;
    /* ps_madd f9, f12, f8, f1 */;
    /* ps_nmsub f8, f12, f8, f1 */;
    r7 = r7 + 0x20;
    /* ps_add f4, f11, f9 */;
    /* ps_sub f12, f11, f9 */;
    /* ps_msub f13, f8, f27, f9 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_sub f3, f8, f10 */;
    /* ps_add f1, f10, f13 */;
    /* ps_sub f13, f10, f13 */;
    /* ps_mul f3, f3, f28 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_merge00 f1, f4, f1 */;
    /* ps_nmsub f9, f10, f30, f3 */;
    /* ps_msub f11, f8, f29, f3 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f4 */;
    /* ps_merge00 f13, f13, f12 */;
    r3 = *(u16*)((u8*)r8 + 0x2);
    /* ps_madd f10, f12, f27, f9 */;
    /* ps_merge11 f8, f4, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_sub f11, f11, f10 */;
    /* ps_add f12, f1, f8 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f11, f10, f11 */;
    /* ps_sub f4, f1, f8 */;
    /* ps_mul f7, f7, f6 */;
    /* ps_add f10, f13, f11 */;
    /* ps_sub f9, f13, f11 */;
    /* ps_merge10 f10, f10, f10 */;
    /* psq_stu f12, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f4, f4, f4 */;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801EA4BC;
L_801EA6D0: ;
    r8 = *(u32*)lbl_8047B560;
    r9 = r30;
    r0 = *(u32*)lbl_8047B580;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    r7 = r0 << 3;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    r3 = r4 << 2;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    r6 = r0 << 2;
    /* ps_add f9, f7, f6 */;
    r7 = r7 + r3;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f3, f7, f6 */;
    r6 = r7 + r6;
    /* ps_add f9, f9, f31 */;
    r0 = 0x3;
    /* ps_add f8, f5, f4 */;
    r5 = r8 + r7;
    /* ps_sub f2, f5, f4 */;
    /* ps_add f4, f9, f8 */;
    r3 = r8 + r6;
    /* ps_add f3, f3, f31 */;
    ctr_fn = (void(*)(void))r0;
L_801EA72C: ;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    r9 = r9 + 0x8;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f9, f9, f8 */;
    r7 = r7 + 0x2;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    r6 = r6 + 0x2;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* ps_add f9, f7, f6 */;
    /* psq_st f3, 0x0(r3), 0, qr6 */;
    /* ps_sub f3, f7, f6 */;
    /* ps_add f9, f9, f31 */;
    /* psq_st f2, 0x8(r3), 0, qr6 */;
    /* ps_add f8, f5, f4 */;
    /* ps_sub f2, f5, f4 */;
    /* psq_st f1, 0x10(r3), 0, qr6 */;
    r5 = r8 + r7;
    /* ps_add f4, f9, f8 */;
    /* psq_st f0, 0x18(r3), 0, qr6 */;
    /* ps_add f3, f3, f31 */;
    r3 = r8 + r6;
    if (--ctr != 0) goto L_801EA72C;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_sub f9, f9, f8 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* psq_st f0, 0x18(r3), 0, qr6 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    /* ps_sub f2, f12, f11 */;
    /* psq_st f1, 0x10(r3), 0, qr6 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* psq_st f3, 0x0(r3), 0, qr6 */;
    /* psq_st f2, 0x8(r3), 0, qr6 */;
    r8 = *(u32*)((u8*)r30 + 0x124);
    r7 = *(u32*)lbl_8047B5A0;
    r9 = r4 + 0x8;
    /* subi r10, r30, 0x8 */;
    r3 = 0x8;
    ctr_fn = (void(*)(void))r3;
L_801EA8B0: ;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_mul f7, f7, f6 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    r3 = *(u16*)((u8*)r8 + 0x2);
    r6 = r6 | r0;
L_801EA8D0: ;
    if ((s32)r6 != (s32)0x0) goto L_801EAA1C;
    /* ps_merge00 f4, f7, f7 */;
    /* psq_st f4, 0x8(r10), 0, qr0 */;
    if ((s32)r5 != (s32)0x0) goto L_801EA980;
    /* psq_st f4, 0x10(r10), 0, qr0 */;
    /* psq_st f4, 0x18(r10), 0, qr0 */;
    if ((s32)r3 != (s32)0x0) goto L_801EA90C;
    r7 = r7 + 0x20;
    /* psq_stu f4, 0x20(r10), 0, qr0 */;
    r8 = r8 + 0x10;
    if (--ctr != 0) goto L_801EA8B0;
    goto L_801EAAE4;
L_801EA90C: ;
    /* ps_msub f13, f7, f28, f7 */;
    r8 = r8 + 0x10;
    /* ps_merge00 f2, f7, f7 */;
    r7 = r7 + 0x20;
    /* ps_sub f1, f28, f29 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_msub f12, f7, f27, f13 */;
    r3 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f10, f7, f13 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_nmsub f11, f7, f1, f12 */;
    /* ps_add f8, f2, f10 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_merge11 f9, f12, f11 */;
    /* ps_sub f10, f2, f10 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_add f3, f2, f9 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_sub f9, f2, f9 */;
    /* psq_stu f8, 0x8(r10), 0, qr0 */;
    /* ps_merge10 f9, f9, f9 */;
    /* psq_stu f3, 0x8(r10), 0, qr0 */;
    /* ps_merge10 f10, f10, f10 */;
    r6 = r6 | r0;
    /* psq_stu f9, 0x8(r10), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f10, 0x8(r10), 0, qr0 */;
    if (--ctr != 0) goto L_801EA8D0;
    goto L_801EAAE4;
L_801EA980: ;
    /* psq_l f1, 0x4(r8), 0, qr5 */;
    /* psq_l f2, 0x8(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_mul f1, f1, f2 */;
    r7 = r7 + 0x20;
    /* ps_sub f12, f7, f1 */;
    /* ps_add f13, f7, f1 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_madd f11, f1, f27, f12 */;
    /* ps_nmsub f10, f1, f27, f13 */;
    /* ps_mul f3, f12, f28 */;
    /* ps_merge00 f11, f13, f11 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_nmsub f9, f1, f30, f3 */;
    /* ps_merge00 f10, f10, f12 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f13 */;
    /* ps_nmsub f8, f7, f29, f3 */;
    r3 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f13, f13, f9 */;
    /* ps_msub f3, f12, f27, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_add f2, f11, f13 */;
    /* ps_sub f8, f8, f3 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f12, f3, f8 */;
    /* ps_sub f11, f11, f13 */;
    /* psq_stu f2, 0x8(r10), 0, qr0 */;
    /* ps_add f4, f10, f12 */;
    /* ps_sub f1, f10, f12 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r10), 0, qr0 */;
    /* ps_merge10 f1, f1, f1 */;
    /* ps_merge10 f11, f11, f11 */;
    /* psq_stu f1, 0x8(r10), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f11, 0x8(r10), 0, qr0 */;
    if (--ctr != 0) goto L_801EA8D0;
    goto L_801EAAE4;
L_801EAA1C: ;
    /* psq_l f2, 0x4(r8), 0, qr5 */;
    /* psq_l f10, 0x8(r7), 0, qr0 */;
    /* ps_mul f2, f2, f10 */;
    /* psq_l f13, 0x8(r8), 0, qr5 */;
    /* psq_l f9, 0x10(r7), 0, qr0 */;
    /* ps_merge01 f4, f7, f2 */;
    /* psq_l f12, 0xc(r8), 0, qr5 */;
    /* ps_merge01 f1, f2, f7 */;
    /* psq_l f8, 0x18(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_madd f11, f13, f9, f4 */;
    /* ps_nmsub f10, f13, f9, f4 */;
    /* ps_madd f9, f12, f8, f1 */;
    /* ps_nmsub f8, f12, f8, f1 */;
    r7 = r7 + 0x20;
    /* ps_add f4, f11, f9 */;
    /* ps_sub f12, f11, f9 */;
    /* ps_msub f13, f8, f27, f9 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_sub f3, f8, f10 */;
    /* ps_add f1, f10, f13 */;
    /* ps_sub f13, f10, f13 */;
    /* ps_mul f3, f3, f28 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_merge00 f1, f4, f1 */;
    /* ps_nmsub f9, f10, f30, f3 */;
    /* ps_msub f11, f8, f29, f3 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f4 */;
    /* ps_merge00 f13, f13, f12 */;
    r3 = *(u16*)((u8*)r8 + 0x2);
    /* ps_madd f10, f12, f27, f9 */;
    /* ps_merge11 f8, f4, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_sub f11, f11, f10 */;
    /* ps_add f12, f1, f8 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f11, f10, f11 */;
    /* ps_sub f4, f1, f8 */;
    /* ps_mul f7, f7, f6 */;
    /* ps_add f10, f13, f11 */;
    /* ps_sub f9, f13, f11 */;
    /* ps_merge10 f10, f10, f10 */;
    /* psq_stu f12, 0x8(r10), 0, qr0 */;
    /* ps_merge10 f4, f4, f4 */;
    /* psq_stu f9, 0x8(r10), 0, qr0 */;
    /* psq_stu f10, 0x8(r10), 0, qr0 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r10), 0, qr0 */;
    if (--ctr != 0) goto L_801EA8D0;
L_801EAAE4: ;
    r8 = *(u32*)lbl_8047B560;
    r10 = r30;
    r0 = *(u32*)lbl_8047B580;
    /* psq_l f7, 0x0(r10), 0, qr0 */;
    r7 = r0 << 3;
    /* psq_l f6, 0x80(r10), 0, qr0 */;
    r9 = r9 << 2;
    /* psq_l f5, 0x40(r10), 0, qr0 */;
    r6 = r0 << 2;
    /* ps_add f9, f7, f6 */;
    r7 = r7 + r9;
    /* psq_l f4, 0xc0(r10), 0, qr0 */;
    /* ps_sub f3, f7, f6 */;
    r6 = r7 + r6;
    /* ps_add f9, f9, f31 */;
    r0 = 0x3;
    /* ps_add f8, f5, f4 */;
    r5 = r8 + r7;
    /* ps_sub f2, f5, f4 */;
    /* ps_add f4, f9, f8 */;
    r3 = r8 + r6;
    /* ps_add f3, f3, f31 */;
    ctr_fn = (void(*)(void))r0;
L_801EAB40: ;
    /* psq_l f11, 0x20(r10), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r10), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r10), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r10), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    r10 = r10 + 0x8;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* psq_l f7, 0x0(r10), 0, qr0 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* psq_l f6, 0x80(r10), 0, qr0 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* psq_l f5, 0x40(r10), 0, qr0 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_l f4, 0xc0(r10), 0, qr0 */;
    /* ps_sub f9, f9, f8 */;
    r7 = r7 + 0x2;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    r6 = r6 + 0x2;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* ps_add f9, f7, f6 */;
    /* psq_st f3, 0x0(r3), 0, qr6 */;
    /* ps_sub f3, f7, f6 */;
    /* ps_add f9, f9, f31 */;
    /* psq_st f2, 0x8(r3), 0, qr6 */;
    /* ps_add f8, f5, f4 */;
    /* ps_sub f2, f5, f4 */;
    /* psq_st f1, 0x10(r3), 0, qr6 */;
    r5 = r8 + r7;
    /* ps_add f4, f9, f8 */;
    /* psq_st f0, 0x18(r3), 0, qr6 */;
    /* ps_add f3, f3, f31 */;
    r3 = r8 + r6;
    if (--ctr != 0) goto L_801EAB40;
    /* psq_l f11, 0x20(r10), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r10), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r10), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r10), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_sub f9, f9, f8 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* psq_st f0, 0x18(r3), 0, qr6 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    /* ps_sub f2, f12, f11 */;
    /* psq_st f1, 0x10(r3), 0, qr6 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* psq_st f3, 0x0(r3), 0, qr6 */;
    /* psq_st f2, 0x8(r3), 0, qr6 */;
    r0 = *(u32*)((u8*)r30 + 0x110);
    r3 = *(u32*)lbl_8047B5B0;
    r4 = (u32)r4 >> 1;
    *(u32*)lbl_8047B560 = r0;
    /* subi r9, r30, 0x8 */;
    *(u32*)lbl_8047B580 = r31;
    r0 = *(u8*)((u8*)r3 + 0x686);
    r0 = r0 << 8;
    r0 = r3 + r0;
    *(u32*)lbl_8047B5A0 = r0;
    r8 = *(u32*)((u8*)r30 + 0x128);
    r7 = *(u32*)lbl_8047B5A0;
    r3 = 0x8;
    ctr_fn = (void(*)(void))r3;
L_801EACE4: ;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_mul f7, f7, f6 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    r3 = *(u16*)((u8*)r8 + 0x2);
    /* or. r6, r6, r0 */;
L_801EAD04: ;
    if ((s32)r6 != (s32)0x0) goto L_801EAE50;
    /* ps_merge00 f4, f7, f7 */;
    /* psq_st f4, 0x8(r9), 0, qr0 */;
    if ((s32)r5 != (s32)0x0) goto L_801EADB4;
    /* psq_st f4, 0x10(r9), 0, qr0 */;
    /* psq_st f4, 0x18(r9), 0, qr0 */;
    if ((s32)r3 != (s32)0x0) goto L_801EAD40;
    r7 = r7 + 0x20;
    /* psq_stu f4, 0x20(r9), 0, qr0 */;
    r8 = r8 + 0x10;
    if (--ctr != 0) goto L_801EACE4;
    goto L_801EAF18;
L_801EAD40: ;
    r8 = r8 + 0x10;
    /* ps_msub f13, f7, f28, f7 */;
    r7 = r7 + 0x20;
    /* ps_merge00 f2, f7, f7 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f1, f28, f29 */;
    /* ps_msub f12, f7, f27, f13 */;
    r3 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f10, f7, f13 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_nmsub f11, f7, f1, f12 */;
    /* ps_add f8, f2, f10 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_merge11 f9, f12, f11 */;
    /* ps_sub f10, f2, f10 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_add f3, f2, f9 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_sub f9, f2, f9 */;
    /* psq_stu f8, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f9, f9, f9 */;
    /* psq_stu f3, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f10, f10, f10 */;
    r6 = r6 | r0;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801EAD04;
    goto L_801EAF18;
L_801EADB4: ;
    /* psq_l f1, 0x4(r8), 0, qr5 */;
    /* psq_l f2, 0x8(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_mul f1, f1, f2 */;
    r7 = r7 + 0x20;
    /* ps_sub f12, f7, f1 */;
    /* ps_add f13, f7, f1 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_madd f11, f1, f27, f12 */;
    /* ps_nmsub f10, f1, f27, f13 */;
    /* ps_mul f3, f12, f28 */;
    /* ps_merge00 f11, f13, f11 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_nmsub f9, f1, f30, f3 */;
    /* ps_merge00 f10, f10, f12 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f13 */;
    /* ps_nmsub f8, f7, f29, f3 */;
    r3 = *(u16*)((u8*)r8 + 0x2);
    /* ps_merge11 f13, f13, f9 */;
    /* ps_msub f3, f12, f27, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_add f2, f11, f13 */;
    /* ps_sub f8, f8, f3 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f12, f3, f8 */;
    /* ps_sub f11, f11, f13 */;
    /* psq_stu f2, 0x8(r9), 0, qr0 */;
    /* ps_add f4, f10, f12 */;
    /* ps_sub f1, f10, f12 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f1, f1, f1 */;
    /* ps_merge10 f11, f11, f11 */;
    /* psq_stu f1, 0x8(r9), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f11, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801EAD04;
    goto L_801EAF18;
L_801EAE50: ;
    /* psq_l f2, 0x4(r8), 0, qr5 */;
    /* psq_l f10, 0x8(r7), 0, qr0 */;
    /* ps_mul f2, f2, f10 */;
    /* psq_l f13, 0x8(r8), 0, qr5 */;
    /* psq_l f9, 0x10(r7), 0, qr0 */;
    /* ps_merge01 f4, f7, f2 */;
    /* psq_l f12, 0xc(r8), 0, qr5 */;
    /* ps_merge01 f1, f2, f7 */;
    /* psq_l f8, 0x18(r7), 0, qr0 */;
    r8 = r8 + 0x10;
    /* ps_madd f11, f13, f9, f4 */;
    /* ps_nmsub f10, f13, f9, f4 */;
    /* ps_madd f9, f12, f8, f1 */;
    /* ps_nmsub f8, f12, f8, f1 */;
    r7 = r7 + 0x20;
    /* ps_add f4, f11, f9 */;
    /* ps_sub f12, f11, f9 */;
    /* ps_msub f13, f8, f27, f9 */;
    r6 = *(u32*)((u8*)r8 + 0xC);
    /* ps_sub f3, f8, f10 */;
    /* ps_add f1, f10, f13 */;
    /* ps_sub f13, f10, f13 */;
    /* ps_mul f3, f3, f28 */;
    r0 = *(u32*)((u8*)r8 + 0x8);
    /* ps_merge00 f1, f4, f1 */;
    /* ps_nmsub f9, f10, f30, f3 */;
    /* ps_msub f11, f8, f29, f3 */;
    r5 = *(u32*)((u8*)r8 + 0x4);
    /* ps_sub f9, f9, f4 */;
    /* ps_merge00 f13, f13, f12 */;
    r3 = *(u16*)((u8*)r8 + 0x2);
    /* ps_madd f10, f12, f27, f9 */;
    /* ps_merge11 f8, f4, f9 */;
    /* psq_l f7, 0x0(r8), 0, qr5 */;
    /* ps_sub f11, f11, f10 */;
    /* ps_add f12, f1, f8 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f11, f10, f11 */;
    /* ps_sub f4, f1, f8 */;
    /* ps_mul f7, f7, f6 */;
    /* ps_add f10, f13, f11 */;
    /* ps_sub f9, f13, f11 */;
    /* ps_merge10 f10, f10, f10 */;
    /* psq_stu f12, 0x8(r9), 0, qr0 */;
    /* ps_merge10 f4, f4, f4 */;
    /* psq_stu f9, 0x8(r9), 0, qr0 */;
    /* psq_stu f10, 0x8(r9), 0, qr0 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r9), 0, qr0 */;
    if (--ctr != 0) goto L_801EAD04;
L_801EAF18: ;
    r8 = *(u32*)lbl_8047B560;
    r9 = r30;
    r0 = *(u32*)lbl_8047B580;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    r3 = r4 << 2;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    r6 = r0 << 2;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    r7 = r3;
    /* ps_add f9, f7, f6 */;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f3, f7, f6 */;
    r6 = r7 + r6;
    /* ps_add f9, f9, f31 */;
    r0 = 0x3;
    /* ps_add f8, f5, f4 */;
    r5 = r8 + r7;
    /* ps_sub f2, f5, f4 */;
    /* ps_add f4, f9, f8 */;
    r3 = r8 + r6;
    /* ps_add f3, f3, f31 */;
    ctr_fn = (void(*)(void))r0;
L_801EAF70: ;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    r9 = r9 + 0x8;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* psq_l f7, 0x0(r9), 0, qr0 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* psq_l f6, 0x80(r9), 0, qr0 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* psq_l f5, 0x40(r9), 0, qr0 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_l f4, 0xc0(r9), 0, qr0 */;
    /* ps_sub f9, f9, f8 */;
    r7 = r7 + 0x2;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    r6 = r6 + 0x2;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* ps_add f9, f7, f6 */;
    /* psq_st f3, 0x0(r3), 0, qr6 */;
    /* ps_sub f3, f7, f6 */;
    /* ps_add f9, f9, f31 */;
    /* psq_st f2, 0x8(r3), 0, qr6 */;
    /* ps_add f8, f5, f4 */;
    /* ps_sub f2, f5, f4 */;
    /* psq_st f1, 0x10(r3), 0, qr6 */;
    r5 = r8 + r7;
    /* ps_add f4, f9, f8 */;
    /* psq_st f0, 0x18(r3), 0, qr6 */;
    /* ps_add f3, f3, f31 */;
    r3 = r8 + r6;
    if (--ctr != 0) goto L_801EAF70;
    /* psq_l f11, 0x20(r9), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r9), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r9), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r9), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_st f2, 0x0(r5), 0, qr6 */;
    /* ps_sub f9, f9, f8 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* psq_st f0, 0x18(r3), 0, qr6 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r5), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f1, 0x10(r3), 0, qr6 */;
    /* psq_st f3, 0x10(r5), 0, qr6 */;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r5), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* psq_st f3, 0x0(r3), 0, qr6 */;
    /* psq_st f2, 0x8(r3), 0, qr6 */;
    r0 = *(u32*)((u8*)r30 + 0x114);
    r3 = *(u32*)lbl_8047B5B0;
    /* subi r8, r30, 0x8 */;
    *(u32*)lbl_8047B560 = r0;
    r0 = *(u8*)((u8*)r3 + 0x68C);
    r0 = r0 << 8;
    r0 = r3 + r0;
    *(u32*)lbl_8047B5A0 = r0;
    r9 = *(u32*)((u8*)r30 + 0x12C);
    r7 = *(u32*)lbl_8047B5A0;
    r3 = 0x8;
    ctr_fn = (void(*)(void))r3;
L_801EB10C: ;
    /* psq_l f7, 0x0(r9), 0, qr5 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    r6 = *(u32*)((u8*)r9 + 0xC);
    r0 = *(u32*)((u8*)r9 + 0x8);
    /* ps_mul f7, f7, f6 */;
    r5 = *(u32*)((u8*)r9 + 0x4);
    r3 = *(u16*)((u8*)r9 + 0x2);
    /* or. r6, r6, r0 */;
L_801EB12C: ;
    if ((s32)r6 != (s32)0x0) goto L_801EB278;
    /* ps_merge00 f4, f7, f7 */;
    /* psq_st f4, 0x8(r8), 0, qr0 */;
    if ((s32)r5 != (s32)0x0) goto L_801EB1DC;
    /* psq_st f4, 0x10(r8), 0, qr0 */;
    /* psq_st f4, 0x18(r8), 0, qr0 */;
    if ((s32)r3 != (s32)0x0) goto L_801EB168;
    r7 = r7 + 0x20;
    /* psq_stu f4, 0x20(r8), 0, qr0 */;
    r9 = r9 + 0x10;
    if (--ctr != 0) goto L_801EB10C;
    goto L_801EB340;
L_801EB168: ;
    r9 = r9 + 0x10;
    /* ps_msub f13, f7, f28, f7 */;
    r7 = r7 + 0x20;
    /* ps_merge00 f2, f7, f7 */;
    r5 = *(u32*)((u8*)r9 + 0x4);
    /* ps_sub f1, f28, f29 */;
    /* ps_msub f12, f7, f27, f13 */;
    r3 = *(u16*)((u8*)r9 + 0x2);
    /* ps_merge11 f10, f7, f13 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_nmsub f11, f7, f1, f12 */;
    /* ps_add f8, f2, f10 */;
    /* psq_l f7, 0x0(r9), 0, qr5 */;
    /* ps_merge11 f9, f12, f11 */;
    /* ps_sub f10, f2, f10 */;
    r6 = *(u32*)((u8*)r9 + 0xC);
    /* ps_add f3, f2, f9 */;
    r0 = *(u32*)((u8*)r9 + 0x8);
    /* ps_sub f9, f2, f9 */;
    /* psq_stu f8, 0x8(r8), 0, qr0 */;
    /* ps_merge10 f9, f9, f9 */;
    /* psq_stu f3, 0x8(r8), 0, qr0 */;
    /* ps_merge10 f10, f10, f10 */;
    r6 = r6 | r0;
    /* psq_stu f9, 0x8(r8), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f10, 0x8(r8), 0, qr0 */;
    if (--ctr != 0) goto L_801EB12C;
    goto L_801EB340;
L_801EB1DC: ;
    /* psq_l f1, 0x4(r9), 0, qr5 */;
    /* psq_l f2, 0x8(r7), 0, qr0 */;
    r9 = r9 + 0x10;
    /* ps_mul f1, f1, f2 */;
    r7 = r7 + 0x20;
    /* ps_sub f12, f7, f1 */;
    /* ps_add f13, f7, f1 */;
    r6 = *(u32*)((u8*)r9 + 0xC);
    /* ps_madd f11, f1, f27, f12 */;
    /* ps_nmsub f10, f1, f27, f13 */;
    /* ps_mul f3, f12, f28 */;
    /* ps_merge00 f11, f13, f11 */;
    r0 = *(u32*)((u8*)r9 + 0x8);
    /* ps_nmsub f9, f1, f30, f3 */;
    /* ps_merge00 f10, f10, f12 */;
    r5 = *(u32*)((u8*)r9 + 0x4);
    /* ps_sub f9, f9, f13 */;
    /* ps_nmsub f8, f7, f29, f3 */;
    r3 = *(u16*)((u8*)r9 + 0x2);
    /* ps_merge11 f13, f13, f9 */;
    /* ps_msub f3, f12, f27, f9 */;
    /* psq_l f7, 0x0(r9), 0, qr5 */;
    /* ps_add f2, f11, f13 */;
    /* ps_sub f8, f8, f3 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f12, f3, f8 */;
    /* ps_sub f11, f11, f13 */;
    /* psq_stu f2, 0x8(r8), 0, qr0 */;
    /* ps_add f4, f10, f12 */;
    /* ps_sub f1, f10, f12 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r8), 0, qr0 */;
    /* ps_merge10 f1, f1, f1 */;
    /* ps_merge10 f11, f11, f11 */;
    /* psq_stu f1, 0x8(r8), 0, qr0 */;
    /* ps_mul f7, f7, f6 */;
    /* psq_stu f11, 0x8(r8), 0, qr0 */;
    if (--ctr != 0) goto L_801EB12C;
    goto L_801EB340;
L_801EB278: ;
    /* psq_l f2, 0x4(r9), 0, qr5 */;
    /* psq_l f10, 0x8(r7), 0, qr0 */;
    /* ps_mul f2, f2, f10 */;
    /* psq_l f13, 0x8(r9), 0, qr5 */;
    /* psq_l f9, 0x10(r7), 0, qr0 */;
    /* ps_merge01 f4, f7, f2 */;
    /* psq_l f12, 0xc(r9), 0, qr5 */;
    /* ps_merge01 f1, f2, f7 */;
    /* psq_l f8, 0x18(r7), 0, qr0 */;
    r9 = r9 + 0x10;
    /* ps_madd f11, f13, f9, f4 */;
    /* ps_nmsub f10, f13, f9, f4 */;
    /* ps_madd f9, f12, f8, f1 */;
    /* ps_nmsub f8, f12, f8, f1 */;
    r7 = r7 + 0x20;
    /* ps_add f4, f11, f9 */;
    /* ps_sub f12, f11, f9 */;
    /* ps_msub f13, f8, f27, f9 */;
    r6 = *(u32*)((u8*)r9 + 0xC);
    /* ps_sub f3, f8, f10 */;
    /* ps_add f1, f10, f13 */;
    /* ps_sub f13, f10, f13 */;
    /* ps_mul f3, f3, f28 */;
    r0 = *(u32*)((u8*)r9 + 0x8);
    /* ps_merge00 f1, f4, f1 */;
    /* ps_nmsub f9, f10, f30, f3 */;
    /* ps_msub f11, f8, f29, f3 */;
    r5 = *(u32*)((u8*)r9 + 0x4);
    /* ps_sub f9, f9, f4 */;
    /* ps_merge00 f13, f13, f12 */;
    r3 = *(u16*)((u8*)r9 + 0x2);
    /* ps_madd f10, f12, f27, f9 */;
    /* ps_merge11 f8, f4, f9 */;
    /* psq_l f7, 0x0(r9), 0, qr5 */;
    /* ps_sub f11, f11, f10 */;
    /* ps_add f12, f1, f8 */;
    /* psq_l f6, 0x0(r7), 0, qr0 */;
    /* ps_merge11 f11, f10, f11 */;
    /* ps_sub f4, f1, f8 */;
    /* ps_mul f7, f7, f6 */;
    /* ps_add f10, f13, f11 */;
    /* ps_sub f9, f13, f11 */;
    /* ps_merge10 f10, f10, f10 */;
    /* psq_stu f12, 0x8(r8), 0, qr0 */;
    /* ps_merge10 f4, f4, f4 */;
    /* psq_stu f9, 0x8(r8), 0, qr0 */;
    /* psq_stu f10, 0x8(r8), 0, qr0 */;
    r6 = r6 | r0;
    /* psq_stu f4, 0x8(r8), 0, qr0 */;
    if (--ctr != 0) goto L_801EB12C;
L_801EB340: ;
    r7 = *(u32*)lbl_8047B560;
    r8 = r30;
    r0 = *(u32*)lbl_8047B580;
    /* psq_l f7, 0x0(r8), 0, qr0 */;
    r3 = r4 << 2;
    /* psq_l f6, 0x80(r8), 0, qr0 */;
    r5 = r0 << 2;
    /* psq_l f5, 0x40(r8), 0, qr0 */;
    r6 = r3;
    /* ps_add f9, f7, f6 */;
    /* psq_l f4, 0xc0(r8), 0, qr0 */;
    /* ps_sub f3, f7, f6 */;
    r5 = r6 + r5;
    /* ps_add f9, f9, f31 */;
    r0 = 0x3;
    /* ps_add f8, f5, f4 */;
    r4 = r7 + r6;
    /* ps_sub f2, f5, f4 */;
    /* ps_add f4, f9, f8 */;
    r3 = r7 + r5;
    /* ps_add f3, f3, f31 */;
    ctr_fn = (void(*)(void))r0;
L_801EB398: ;
    /* psq_l f11, 0x20(r8), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r8), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r8), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r8), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    r8 = r8 + 0x8;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* psq_l f7, 0x0(r8), 0, qr0 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* psq_l f6, 0x80(r8), 0, qr0 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* psq_l f5, 0x40(r8), 0, qr0 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_l f4, 0xc0(r8), 0, qr0 */;
    /* ps_sub f9, f9, f8 */;
    r6 = r6 + 0x2;
    /* psq_st f2, 0x0(r4), 0, qr6 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f3, 0x10(r4), 0, qr6 */;
    r5 = r5 + 0x2;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r4), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* ps_add f9, f7, f6 */;
    /* psq_st f3, 0x0(r3), 0, qr6 */;
    /* ps_sub f3, f7, f6 */;
    /* ps_add f9, f9, f31 */;
    /* psq_st f2, 0x8(r3), 0, qr6 */;
    /* ps_add f8, f5, f4 */;
    /* ps_sub f2, f5, f4 */;
    /* psq_st f1, 0x10(r3), 0, qr6 */;
    r4 = r7 + r6;
    /* ps_add f4, f9, f8 */;
    /* psq_st f0, 0x18(r3), 0, qr6 */;
    /* ps_add f3, f3, f31 */;
    r3 = r7 + r5;
    if (--ctr != 0) goto L_801EB398;
    /* psq_l f11, 0x20(r8), 0, qr0 */;
    /* ps_msub f2, f2, f27, f8 */;
    /* psq_l f10, 0x60(r8), 0, qr0 */;
    /* ps_sub f12, f9, f8 */;
    /* ps_add f1, f3, f2 */;
    /* psq_l f9, 0xa0(r8), 0, qr0 */;
    /* ps_sub f13, f3, f2 */;
    /* psq_l f8, 0xe0(r8), 0, qr0 */;
    /* ps_add f3, f9, f10 */;
    /* ps_sub f9, f9, f10 */;
    /* ps_add f2, f11, f8 */;
    /* ps_sub f11, f11, f8 */;
    /* ps_add f8, f2, f3 */;
    /* ps_sub f10, f2, f3 */;
    /* ps_add f3, f9, f11 */;
    /* ps_add f2, f4, f8 */;
    /* ps_mul f3, f3, f28 */;
    /* ps_sub f0, f4, f8 */;
    /* ps_madd f9, f9, f30, f3 */;
    /* psq_st f2, 0x0(r4), 0, qr6 */;
    /* ps_sub f9, f9, f8 */;
    /* ps_msub f11, f11, f29, f3 */;
    /* psq_st f0, 0x18(r3), 0, qr6 */;
    /* ps_add f2, f1, f9 */;
    /* ps_msub f10, f10, f27, f9 */;
    /* ps_sub f1, f1, f9 */;
    /* psq_st f2, 0x8(r4), 0, qr6 */;
    /* ps_add f3, f13, f10 */;
    /* ps_add f11, f11, f10 */;
    /* psq_st f1, 0x10(r3), 0, qr6 */;
    /* psq_st f3, 0x10(r4), 0, qr6 */;
    /* ps_sub f2, f12, f11 */;
    /* ps_add f3, f12, f11 */;
    /* psq_st f2, 0x18(r4), 0, qr6 */;
    /* ps_sub f2, f13, f10 */;
    /* psq_st f3, 0x0(r3), 0, qr6 */;
    /* psq_st f2, 0x8(r3), 0, qr6 */;
    r5 = *(u32*)lbl_8047B5B0;
    r0 = *(u8*)((u8*)r5 + 0x6A9);
    if ((u32)r0 == (u32)0x0) goto L_801EB584;
    r4 = *(u16*)((u8*)r5 + 0x6AC);
    /* subi r0, r4, 0x1 */;
    *(u16*)((u8*)r5 + 0x6AC) = r0;
    r4 = *(u32*)lbl_8047B5B0;
    r0 = *(u16*)((u8*)r4 + 0x6AC);
    if ((u32)r0 != (u32)0x0) goto L_801EB584;
    r0 = *(u16*)((u8*)r4 + 0x6AA);
    *(u16*)((u8*)r4 + 0x6AC) = r0;
    r4 = *(u32*)lbl_8047B5B0;
    r3 = *(u32*)((u8*)r4 + 0x6A4);
    r0 = r3 + 0x6;
    /* clrrwi r3, r0, 3 */;
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x6A4) = r0;
    r3 = *(u32*)lbl_8047B5B0;
    r0 = *(u32*)((u8*)r3 + 0x6A4);
    if ((u32)r0 <= (u32)0x20) goto L_801EB568;
    r0 = 0x21;
    *(u32*)((u8*)r3 + 0x6A4) = r0;
L_801EB568: ;
    r3 = *(u32*)lbl_8047B5B0;
    r0 = 0x0;
    *(u16*)((u8*)r3 + 0x684) = r0;
    r3 = *(u32*)lbl_8047B5B0;
    *(u16*)((u8*)r3 + 0x68A) = r0;
    r3 = *(u32*)lbl_8047B5B0;
    *(u16*)((u8*)r3 + 0x690) = r0;
L_801EB584: ;
    r29 = r29 + 0x1;
L_801EB588: ;
    r3 = *(u32*)lbl_8047B5B0;
    r4 = r29 & 0xFF;
    r0 = *(u16*)((u8*)r3 + 0x696);
    if ((s32)r4 < (s32)r0) goto L_801E9C00;
    r29 = (u32)r28 >> 4;
    r3 = *(u32*)((u8*)r3 + 0x6B0);
    r4 = *(u32*)((u8*)r30 + 0x10C);
    /* extlwi r5, r28, 24, 4 */;
    fn_8009B55C();
    r3 = *(u32*)lbl_8047B5B0;
    r5 = r29 << 6;
    r4 = *(u32*)((u8*)r30 + 0x110);
    r3 = *(u32*)((u8*)r3 + 0x6B4);
    fn_8009B55C();
    r3 = *(u32*)lbl_8047B5B0;
    r5 = r29 << 6;
    r4 = *(u32*)((u8*)r30 + 0x114);
    r3 = *(u32*)((u8*)r3 + 0x6B8);
    fn_8009B55C();
    r5 = *(u32*)lbl_8047B5B0;
    r0 = r29 << 8;
    r4 = r29 << 6;
    r3 = *(u32*)((u8*)r5 + 0x6B0);
    r0 = r3 + r0;
    *(u32*)((u8*)r5 + 0x6B0) = r0;
    r3 = *(u32*)lbl_8047B5B0;
    r0 = *(u32*)((u8*)r3 + 0x6B4);
    r0 = r0 + r4;
    *(u32*)((u8*)r3 + 0x6B4) = r0;
    r3 = *(u32*)lbl_8047B5B0;
    r0 = *(u32*)((u8*)r3 + 0x6B8);
    r0 = r0 + r4;
    *(u32*)((u8*)r3 + 0x6B8) = r0;
    f31 = *(f64*)((u8*)r1 + 0x38);
    f30 = *(f64*)((u8*)r1 + 0x30);
    f29 = *(f64*)((u8*)r1 + 0x28);
    f28 = *(f64*)((u8*)r1 + 0x20);
    f27 = *(f64*)((u8*)r1 + 0x18);
    return;
}
#pragma pop

/* 0x801EB644 | size: 0x67C | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EB644(void) {
    extern u8 lbl_80279AE8[];
    extern u8 lbl_8047B4A0[];
    extern u8 lbl_8047B500[];
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* dcbz r0, r4 */;
    r12 = *(u32*)lbl_8047B4A0;
    r11 = *(u32*)((u8*)r3 + 0x6A4);
    r9 = r12 + 0x20;
    r10 = *(u32*)((u8*)r3 + 0x6A0);
    r5 = r11 + 0x4;
    /* rlwnm r8, r10, r5, 27, 31 */;
    if ((s32)r11 > (s32)0x1c) goto L_801EB738;
    r5 = *(u8*)(r12 + r8);
    r9 = *(u8*)(r9 + r8);
    if ((s32)r5 == (s32)0xff) goto L_801EB694;
    r11 = r11 + r9;
    *(u32*)((u8*)r3 + 0x6A4) = r11;
    goto L_801EB8DC;
L_801EB694: ;
    r6 = r12 + 0x44;
    r11 = r11 + 0x5;
    r0 = 0x14;
    r5 = 0x5;
    r6 = r6 + 0x14;
L_801EB6A8: ;
    r8 = r8 << 1;
    if ((s32)r11 == (s32)0x21) goto L_801EB6C8;
    /* rlwnm r9, r10, r11, 31, 31 */;
    r0 = *(u32*)((u8*)r6 + 0x4);
    r8 = r8 | r9;
    r11 = r11 + 0x1;
    goto L_801EB70C;
L_801EB6C8: ;
    r9 = *(u32*)((u8*)r3 + 0x69C);
    r11 = 0x1;
    r10 = *(u32*)((u8*)r9 + 0x4);
    r0 = *(u32*)((u8*)r6 + 0x4);
    *(u32*)((u8*)r3 + 0x69C) = r9;
    r8 = (r8 & ~0x00000001) | (((r10 << 1) | ((u32)r10 >> 31)) & 0x00000001);
    *(u32*)((u8*)r3 + 0x6A0) = r10;
    goto L_801EB6F8;
L_801EB6E8: ;
    r8 = r8 << 1;
    /* rlwnm r9, r10, r11, 31, 31 */;
    r0 = *(u32*)((u8*)r6 + 0x4);
    r8 = r8 | r9;
L_801EB6F8: ;
    r11 = r11 + 0x1;
    r5 = r5 + 0x1;
    if ((s32)r8 > (s32)r0) goto L_801EB6E8;
    goto L_801EB718;
L_801EB70C: ;
    r5 = r5 + 0x1;
    if ((s32)r8 > (s32)r0) goto L_801EB6A8;
L_801EB718: ;
    *(u32*)((u8*)r3 + 0x6A4) = r11;
    r0 = r5 << 2;
    r5 = r12 + r0;
    r6 = *(u32*)((u8*)r12 + 0x40);
    r0 = *(u32*)((u8*)r5 + 0x8C);
    r0 = r0 + r6;
    r5 = *(u8*)(r8 + r0);
    goto L_801EB8DC;
L_801EB738: ;
    r8 = *(u32*)((u8*)r3 + 0x69C);
    if ((s32)r11 == (s32)0x21) goto L_801EB7F4;
    /* rlwnm r5, r10, r5, 27, 31 */;
    if ((s32)r11 == (s32)0x20) goto L_801EB778;
    r8 = *(u8*)(r12 + r5);
    r9 = *(u8*)(r9 + r5);
    r5 = r11 + r9;
    if ((s32)r8 == (s32)0xff) goto L_801EB858;
    *(u32*)((u8*)r3 + 0x6A4) = r5;
    if ((s32)r5 > (s32)0x21) goto L_801EB858;
    r5 = r8;
    goto L_801EB8DC;
L_801EB778: ;
    r10 = *(u32*)((u8*)r8 + 0x4);
    *(u32*)((u8*)r3 + 0x69C) = r8;
    r5 = (r5 & ~0x0000000F) | (((r10 << 4) | ((u32)r10 >> 28)) & 0x0000000F);
    r8 = *(u8*)(r12 + r5);
    r9 = *(u8*)(r9 + r5);
    *(u32*)((u8*)r3 + 0x6A0) = r10;
    *(u32*)((u8*)r3 + 0x6A4) = r9;
    if ((s32)r8 == (s32)0xff) goto L_801EB7A4;
    r5 = r8;
    goto L_801EB8DC;
L_801EB7A4: ;
    r6 = r12 + 0x44;
    r11 = 0x14;
    r6 = r6 + 0x14;
    r8 = r5 << 27;
    r11 = 0x5;
    r8 = (r8 & ~0x7FFFFFFF) | (((r10 << 31) | ((u32)r10 >> 1)) & 0x7FFFFFFF);
L_801EB7BC: ;
    r10 = 0x1f - r11;
    r0 = *(u32*)((u8*)r6 + 0x4);
    r5 = (u32)r8 >> r10;
    r11 = r11 + 0x1;
    if ((s32)r5 > (s32)r0) goto L_801EB7BC;
    *(u32*)((u8*)r3 + 0x6A4) = r11;
L_801EB7D8: ;
    r0 = r11 << 2;
    r7 = *(u32*)((u8*)r12 + 0x40);
    r6 = r12 + r0;
    r0 = *(u32*)((u8*)r6 + 0x8C);
    r0 = r0 + r7;
    r5 = *(u8*)(r5 + r0);
    goto L_801EB8DC;
L_801EB7F4: ;
    r10 = *(u32*)((u8*)r8 + 0x4);
    r5 = (u32)r10 >> 27;
    *(u32*)((u8*)r3 + 0x69C) = r8;
    r11 = *(u8*)(r12 + r5);
    r9 = *(u8*)(r9 + r5);
    *(u32*)((u8*)r3 + 0x6A0) = r10;
    r9 = r9 + 0x1;
    if ((s32)r11 == (s32)0xff) goto L_801EB824;
    *(u32*)((u8*)r3 + 0x6A4) = r9;
    r5 = r11;
    goto L_801EB8DC;
L_801EB824: ;
    r11 = 0x5;
    r6 = 0x14;
L_801EB82C: ;
    r8 = 0x1f - r11;
    r11 = r11 + 0x1;
    r6 = r6 + 0x4;
    r5 = (u32)r10 >> r8;
    r7 = r12 + r6;
    r0 = *(u32*)((u8*)r7 + 0x44);
    if ((s32)r5 > (s32)r0) goto L_801EB82C;
    r0 = r11 + 0x1;
    *(u32*)((u8*)r3 + 0x6A4) = r0;
    goto L_801EB7D8;
L_801EB858: ;
    r0 = 0x21 - r11;
    r5 = -0x1;
    r7 = r5 << r0;
    r5 = r10 & ~r7;
    r7 = r12 + 0x44;
    r8 = *(u32*)((u8*)r3 + 0x69C);
    r6 = 0x21 - r11;
    r11 = r6 + 0x1;
    r6 = r6 << 2;
    r10 = *(u32*)((u8*)r8 + 0x4);
    r7 = r7 + r6;
    *(u32*)((u8*)r3 + 0x69C) = r8;
    r5 = r5 << 1;
    *(u32*)((u8*)r3 + 0x6A0) = r10;
    r5 = (r5 & ~0x00000001) | (((r10 << 1) | ((u32)r10 >> 31)) & 0x00000001);
    r6 = *(u32*)((u8*)r7 + 0x4);
    r8 = 0x2;
    goto L_801EB8B4;
L_801EB8A0: ;
    r5 = r5 << 1;
    r11 = r11 + 0x1;
    r6 = *(u32*)((u8*)r7 + 0x4);
    r5 = r5 + r9;
    r8 = r8 + 0x1;
L_801EB8B4: ;
    /* rlwnm r9, r10, r8, 31, 31 */;
    if ((s32)r5 > (s32)r6) goto L_801EB8A0;
    *(u32*)((u8*)r3 + 0x6A4) = r8;
    r0 = r11 << 2;
    r6 = r12 + r0;
    r7 = *(u32*)((u8*)r12 + 0x40);
    r0 = *(u32*)((u8*)r6 + 0x8C);
    r0 = r0 + r7;
    r5 = *(u8*)(r5 + r0);
L_801EB8DC: ;
    r0 = 0x20;
    /* dcbz r4, r0 */;
    r7 = 0x0;
    r0 = 0x40;
    /* dcbz r4, r0 */;
    if ((s32)r5 == (s32)0x0) goto L_801EB978;
    r7 = *(u32*)((u8*)r3 + 0x6A4);
    r8 = 0x21 - r7;
    r6 = *(u32*)((u8*)r3 + 0x6A0);
    /* subfc. r9, r8, r5 */;
    /* subi r10, r7, 0x1 */;
    if ((s32)r5 > (s32)0x0) goto L_801EB928;
    r0 = r7 + r5;
    r7 = r6 << r10;
    *(u32*)((u8*)r3 + 0x6A4) = r0;
    r0 = 0x20 - r5;
    r7 = (u32)r7 >> r0;
    goto L_801EB954;
L_801EB928: ;
    r7 = *(u32*)((u8*)r3 + 0x69C);
    r0 = r6 << r10;
    r6 = *(u32*)((u8*)r7 + 0x4);
    r9 = r9 + 0x1;
    *(u32*)((u8*)r3 + 0x6A0) = r6;
    r6 = (u32)r6 >> r8;
    *(u32*)((u8*)r3 + 0x69C) = r7;
    r0 = r6 + r0;
    *(u32*)((u8*)r3 + 0x6A4) = r9;
    r9 = 0x20 - r5;
    r7 = (u32)r0 >> r9;
L_801EB954: ;
    r0 = (s16)r7;
    r6 = __cntlzw(r0);
    r0 = 0x20 - r5;
    if ((s32)r6 <= (s32)r0) goto L_801EB978;
    r0 = -0x1;
    r0 = r0 << r5;
    r7 = r0 + r7;
    r7 = r7 + 0x1;
L_801EB978: ;
    r0 = 0x60;
    /* dcbz r4, r0 */;
    r0 = *(s16*)((u8*)r3 + 0x684);
    r0 = r0 + r7;
    *(u16*)((u8*)r3 + 0x684) = r0;
    *(u16*)((u8*)r4 + 0x0) = r0;
    r8 = *(u32*)lbl_8047B500;
    r6 = *(u32*)((u8*)r3 + 0x6A4);
    r7 = r8 + 0x20;
    r0 = *(u32*)((u8*)r3 + 0x6A0);
    r5 = 0x1;
    r9 = (u32)lbl_80279AE8;
    r10 = (u32)lbl_80279AE8;
    goto L_801EBC9C;
L_801EB9B0: ;
    r31 = r6 + 0x4;
    /* rlwnm r12, r0, r31, 27, 31 */;
    if ((s32)r6 > (s32)0x1c) goto L_801EBA74;
    r30 = *(u8*)(r8 + r12);
    r31 = *(u8*)(r7 + r12);
    if ((s32)r30 == (s32)0xff) goto L_801EB9D8;
    r6 = r6 + r31;
    goto L_801EBBFC;
L_801EB9D8: ;
    r6 = r6 + 0x5;
    r9 = r8 + 0x44;
    r11 = 0x14;
    r31 = 0x5;
    r9 = r9 + 0x14;
L_801EB9EC: ;
    r12 = r12 << 1;
    if ((s32)r6 == (s32)0x21) goto L_801EBA0C;
    /* rlwnm r30, r0, r6, 31, 31 */;
    r11 = *(u32*)((u8*)r9 + 0x4);
    r12 = r12 | r30;
    r6 = r6 + 0x1;
    goto L_801EBA4C;
L_801EBA0C: ;
    r30 = *(u32*)((u8*)r3 + 0x69C);
    r6 = 0x1;
    r0 = *(u32*)((u8*)r30 + 0x4);
    r11 = *(u32*)((u8*)r9 + 0x4);
    *(u32*)((u8*)r3 + 0x69C) = r30;
    r12 = (r12 & ~0x00000001) | (((r0 << 1) | ((u32)r0 >> 31)) & 0x00000001);
    goto L_801EBA38;
L_801EBA28: ;
    r12 = r12 << 1;
    /* rlwnm r30, r0, r6, 31, 31 */;
    r11 = *(u32*)((u8*)r9 + 0x4);
    r12 = r12 | r30;
L_801EBA38: ;
    r6 = r6 + 0x1;
    r31 = r31 + 0x1;
    if ((s32)r12 > (s32)r11) goto L_801EBA28;
    goto L_801EBA58;
L_801EBA4C: ;
    r31 = r31 + 0x1;
    if ((s32)r12 > (s32)r11) goto L_801EB9EC;
L_801EBA58: ;
    r9 = r31 << 2;
    r11 = *(u32*)((u8*)r8 + 0x40);
    r9 = r8 + r9;
    r9 = *(u32*)((u8*)r9 + 0x8C);
    r9 = r9 + r11;
    r30 = *(u8*)(r12 + r9);
    goto L_801EBBFC;
L_801EBA74: ;
    r12 = *(u32*)((u8*)r3 + 0x69C);
    if ((s32)r6 == (s32)0x21) goto L_801EBAB0;
    /* rlwnm r31, r0, r31, 27, 31 */;
    if ((s32)r6 == (s32)0x20) goto L_801EBB14;
    r30 = *(u8*)(r8 + r31);
    r29 = *(u8*)(r7 + r31);
    r31 = r6 + r29;
    if ((s32)r30 == (s32)0xff) goto L_801EBB80;
    if ((s32)r31 > (s32)0x21) goto L_801EBB80;
    r6 = r31;
    goto L_801EBBFC;
L_801EBAB0: ;
    r0 = *(u32*)((u8*)r12 + 0x4);
    r31 = (u32)r0 >> 27;
    *(u32*)((u8*)r3 + 0x69C) = r12;
    r30 = *(u8*)(r8 + r31);
    r12 = *(u8*)(r7 + r31);
    r6 = r12 + 0x1;
    if ((s32)r30 == (s32)0xff) goto L_801EBAD4;
    goto L_801EBBFC;
L_801EBAD4: ;
    r30 = 0x5;
    r6 = 0x14;
L_801EBADC: ;
    r12 = 0x1f - r30;
    r30 = r30 + 0x1;
    r6 = r6 + 0x4;
    r31 = (u32)r0 >> r12;
    r12 = r8 + r6;
    r9 = *(u32*)((u8*)r12 + 0x44);
    if ((s32)r31 > (s32)r9) goto L_801EBADC;
    r11 = *(u32*)((u8*)r8 + 0x40);
    r6 = r30 + 0x1;
    r9 = *(u32*)((u8*)r12 + 0x8C);
    r9 = r9 + r11;
    r30 = *(u8*)(r31 + r9);
    goto L_801EBBFC;
L_801EBB14: ;
    r0 = *(u32*)((u8*)r12 + 0x4);
    *(u32*)((u8*)r3 + 0x69C) = r12;
    r31 = (r31 & ~0x0000000F) | (((r0 << 4) | ((u32)r0 >> 28)) & 0x0000000F);
    r30 = *(u8*)(r8 + r31);
    r6 = *(u8*)(r7 + r31);
    if ((s32)r30 == (s32)0xff) goto L_801EBB34;
    goto L_801EBBFC;
L_801EBB34: ;
    r9 = r8 + 0x44;
    r6 = 0x14;
    r9 = r9 + 0x14;
    r12 = r31 << 27;
    r6 = 0x5;
    r12 = (r12 & ~0x7FFFFFFF) | (((r0 << 31) | ((u32)r0 >> 1)) & 0x7FFFFFFF);
L_801EBB4C: ;
    r30 = 0x1f - r6;
    r11 = *(u32*)((u8*)r9 + 0x4);
    r31 = (u32)r12 >> r30;
    r6 = r6 + 0x1;
    if ((s32)r31 > (s32)r11) goto L_801EBB4C;
    r9 = r6 << 2;
    r11 = *(u32*)((u8*)r8 + 0x40);
    r9 = r8 + r9;
    r9 = *(u32*)((u8*)r9 + 0x8C);
    r9 = r9 + r11;
    r30 = *(u8*)(r31 + r9);
    goto L_801EBBFC;
L_801EBB80: ;
    r9 = 0x21 - r6;
    r11 = -0x1;
    r9 = r11 << r9;
    r31 = r0 & ~r9;
    r9 = r8 + 0x44;
    r12 = *(u32*)((u8*)r3 + 0x69C);
    r11 = 0x21 - r6;
    r30 = r11 + 0x1;
    r11 = r11 << 2;
    r0 = *(u32*)((u8*)r12 + 0x4);
    r9 = r9 + r11;
    *(u32*)((u8*)r3 + 0x69C) = r12;
    r31 = r31 << 1;
    r31 = (r31 & ~0x00000001) | (((r0 << 1) | ((u32)r0 >> 31)) & 0x00000001);
    r11 = *(u32*)((u8*)r9 + 0x4);
    r6 = 0x2;
    goto L_801EBBD8;
L_801EBBC4: ;
    r31 = r31 << 1;
    r30 = r30 + 0x1;
    r11 = *(u32*)((u8*)r9 + 0x4);
    r31 = r31 + r29;
    r6 = r6 + 0x1;
L_801EBBD8: ;
    /* rlwnm r29, r0, r6, 31, 31 */;
    if ((s32)r31 > (s32)r11) goto L_801EBBC4;
    r9 = r30 << 2;
    r11 = *(u32*)((u8*)r8 + 0x40);
    r9 = r8 + r9;
    r9 = *(u32*)((u8*)r9 + 0x8C);
    r9 = r9 + r11;
    r30 = *(u8*)(r31 + r9);
L_801EBBFC: ;
    r29 = r30 & 0xf;
    r30 = (s32)r30 >> 4;
    if ((s32)r31 == (s32)r11) goto L_801EBC8C;
    r5 = r5 + r30;
    r31 = 0x21 - r6;
    /* subfc. r12, r31, r29 */;
    /* subi r9, r6, 0x1 */;
    if ((s32)r31 > (s32)r11) goto L_801EBC30;
    r6 = r6 + r29;
    r11 = r0 << r9;
    r9 = 0x20 - r29;
    r30 = (u32)r11 >> r9;
    goto L_801EBC54;
L_801EBC30: ;
    r11 = *(u32*)((u8*)r3 + 0x69C);
    r9 = r0 << r9;
    r0 = *(u32*)((u8*)r11 + 0x4);
    r6 = r12 + 0x1;
    *(u32*)((u8*)r3 + 0x69C) = r11;
    r11 = (u32)r0 >> r31;
    r9 = r11 + r9;
    r12 = 0x20 - r29;
    r30 = (u32)r9 >> r12;
L_801EBC54: ;
    r11 = __cntlzw(r30);
    r9 = 0x20 - r29;
    if ((s32)r11 <= (s32)r9) goto L_801EBC74;
    r9 = -0x1;
    r9 = r9 << r29;
    r30 = r9 + r30;
    r30 = r30 + 0x1;
L_801EBC74: ;
    r9 = r10 + r5;
    r9 = *(u8*)((u8*)r9 + 0x0);
    r11 = (s16)r30;
    r9 = r9 << 1;
    *(u16*)(r4 + r9) = r11;
    goto L_801EBC98;
L_801EBC8C: ;
    if ((s32)r30 != (s32)0xf) goto L_801EBCA4;
    r5 = r5 + 0xf;
L_801EBC98: ;
    r5 = r5 + 0x1;
L_801EBC9C: ;
    if ((s32)r5 < (s32)0x40) goto L_801EB9B0;
L_801EBCA4: ;
    *(u32*)((u8*)r3 + 0x6A4) = r6;
    *(u32*)((u8*)r3 + 0x6A0) = r0;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}
#pragma pop

/* 0x801EBCC0 | size: 0x6A8 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EBCC0(void) {
    extern u8 lbl_80279AE8[];
    extern u8 lbl_8047B4C0[];
    extern u8 lbl_8047B520[];
    u8 sp[0x18];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* dcbz r0, r4 */;
    r12 = *(u32*)lbl_8047B4C0;
    r11 = *(u32*)((u8*)r3 + 0x6A4);
    r9 = r12 + 0x20;
    r10 = *(u32*)((u8*)r3 + 0x6A0);
    r5 = r11 + 0x4;
    /* rlwnm r8, r10, r5, 27, 31 */;
    if ((s32)r11 > (s32)0x1c) goto L_801EBDB0;
    r5 = *(u8*)(r12 + r8);
    r9 = *(u8*)(r9 + r8);
    if ((s32)r5 == (s32)0xff) goto L_801EBD0C;
    r11 = r11 + r9;
    *(u32*)((u8*)r3 + 0x6A4) = r11;
    goto L_801EBF54;
L_801EBD0C: ;
    r6 = r12 + 0x44;
    r11 = r11 + 0x5;
    r0 = 0x14;
    r5 = 0x5;
    r6 = r6 + 0x14;
L_801EBD20: ;
    r8 = r8 << 1;
    if ((s32)r11 == (s32)0x21) goto L_801EBD40;
    /* rlwnm r9, r10, r11, 31, 31 */;
    r0 = *(u32*)((u8*)r6 + 0x4);
    r8 = r8 | r9;
    r11 = r11 + 0x1;
    goto L_801EBD84;
L_801EBD40: ;
    r9 = *(u32*)((u8*)r3 + 0x69C);
    r11 = 0x1;
    r10 = *(u32*)((u8*)r9 + 0x4);
    r0 = *(u32*)((u8*)r6 + 0x4);
    *(u32*)((u8*)r3 + 0x69C) = r9;
    r8 = (r8 & ~0x00000001) | (((r10 << 1) | ((u32)r10 >> 31)) & 0x00000001);
    *(u32*)((u8*)r3 + 0x6A0) = r10;
    goto L_801EBD70;
L_801EBD60: ;
    r8 = r8 << 1;
    /* rlwnm r9, r10, r11, 31, 31 */;
    r0 = *(u32*)((u8*)r6 + 0x4);
    r8 = r8 | r9;
L_801EBD70: ;
    r11 = r11 + 0x1;
    r5 = r5 + 0x1;
    if ((s32)r8 > (s32)r0) goto L_801EBD60;
    goto L_801EBD90;
L_801EBD84: ;
    r5 = r5 + 0x1;
    if ((s32)r8 > (s32)r0) goto L_801EBD20;
L_801EBD90: ;
    *(u32*)((u8*)r3 + 0x6A4) = r11;
    r0 = r5 << 2;
    r5 = r12 + r0;
    r6 = *(u32*)((u8*)r12 + 0x40);
    r0 = *(u32*)((u8*)r5 + 0x8C);
    r0 = r0 + r6;
    r5 = *(u8*)(r8 + r0);
    goto L_801EBF54;
L_801EBDB0: ;
    r8 = *(u32*)((u8*)r3 + 0x69C);
    if ((s32)r11 == (s32)0x21) goto L_801EBE6C;
    /* rlwnm r5, r10, r5, 27, 31 */;
    if ((s32)r11 == (s32)0x20) goto L_801EBDF0;
    r8 = *(u8*)(r12 + r5);
    r9 = *(u8*)(r9 + r5);
    r5 = r11 + r9;
    if ((s32)r8 == (s32)0xff) goto L_801EBED0;
    *(u32*)((u8*)r3 + 0x6A4) = r5;
    if ((s32)r5 > (s32)0x21) goto L_801EBED0;
    r5 = r8;
    goto L_801EBF54;
L_801EBDF0: ;
    r10 = *(u32*)((u8*)r8 + 0x4);
    *(u32*)((u8*)r3 + 0x69C) = r8;
    r5 = (r5 & ~0x0000000F) | (((r10 << 4) | ((u32)r10 >> 28)) & 0x0000000F);
    r8 = *(u8*)(r12 + r5);
    r9 = *(u8*)(r9 + r5);
    *(u32*)((u8*)r3 + 0x6A0) = r10;
    *(u32*)((u8*)r3 + 0x6A4) = r9;
    if ((s32)r8 == (s32)0xff) goto L_801EBE1C;
    r5 = r8;
    goto L_801EBF54;
L_801EBE1C: ;
    r6 = r12 + 0x44;
    r11 = 0x14;
    r6 = r6 + 0x14;
    r8 = r5 << 27;
    r11 = 0x5;
    r8 = (r8 & ~0x7FFFFFFF) | (((r10 << 31) | ((u32)r10 >> 1)) & 0x7FFFFFFF);
L_801EBE34: ;
    r10 = 0x1f - r11;
    r0 = *(u32*)((u8*)r6 + 0x4);
    r5 = (u32)r8 >> r10;
    r11 = r11 + 0x1;
    if ((s32)r5 > (s32)r0) goto L_801EBE34;
    *(u32*)((u8*)r3 + 0x6A4) = r11;
L_801EBE50: ;
    r0 = r11 << 2;
    r7 = *(u32*)((u8*)r12 + 0x40);
    r6 = r12 + r0;
    r0 = *(u32*)((u8*)r6 + 0x8C);
    r0 = r0 + r7;
    r5 = *(u8*)(r5 + r0);
    goto L_801EBF54;
L_801EBE6C: ;
    r10 = *(u32*)((u8*)r8 + 0x4);
    r5 = (u32)r10 >> 27;
    *(u32*)((u8*)r3 + 0x69C) = r8;
    r11 = *(u8*)(r12 + r5);
    r9 = *(u8*)(r9 + r5);
    *(u32*)((u8*)r3 + 0x6A0) = r10;
    r9 = r9 + 0x1;
    if ((s32)r11 == (s32)0xff) goto L_801EBE9C;
    *(u32*)((u8*)r3 + 0x6A4) = r9;
    r5 = r11;
    goto L_801EBF54;
L_801EBE9C: ;
    r11 = 0x5;
    r6 = 0x14;
L_801EBEA4: ;
    r8 = 0x1f - r11;
    r11 = r11 + 0x1;
    r6 = r6 + 0x4;
    r5 = (u32)r10 >> r8;
    r7 = r12 + r6;
    r0 = *(u32*)((u8*)r7 + 0x44);
    if ((s32)r5 > (s32)r0) goto L_801EBEA4;
    r0 = r11 + 0x1;
    *(u32*)((u8*)r3 + 0x6A4) = r0;
    goto L_801EBE50;
L_801EBED0: ;
    r0 = 0x21 - r11;
    r5 = -0x1;
    r7 = r5 << r0;
    r5 = r10 & ~r7;
    r7 = r12 + 0x44;
    r8 = *(u32*)((u8*)r3 + 0x69C);
    r6 = 0x21 - r11;
    r11 = r6 + 0x1;
    r6 = r6 << 2;
    r10 = *(u32*)((u8*)r8 + 0x4);
    r7 = r7 + r6;
    *(u32*)((u8*)r3 + 0x69C) = r8;
    r5 = r5 << 1;
    *(u32*)((u8*)r3 + 0x6A0) = r10;
    r5 = (r5 & ~0x00000001) | (((r10 << 1) | ((u32)r10 >> 31)) & 0x00000001);
    r6 = *(u32*)((u8*)r7 + 0x4);
    r8 = 0x2;
    goto L_801EBF2C;
L_801EBF18: ;
    r5 = r5 << 1;
    r11 = r11 + 0x1;
    r6 = *(u32*)((u8*)r7 + 0x4);
    r5 = r5 + r9;
    r8 = r8 + 0x1;
L_801EBF2C: ;
    /* rlwnm r9, r10, r8, 31, 31 */;
    if ((s32)r5 > (s32)r6) goto L_801EBF18;
    *(u32*)((u8*)r3 + 0x6A4) = r8;
    r0 = r11 << 2;
    r6 = r12 + r0;
    r7 = *(u32*)((u8*)r12 + 0x40);
    r0 = *(u32*)((u8*)r6 + 0x8C);
    r0 = r0 + r7;
    r5 = *(u8*)(r5 + r0);
L_801EBF54: ;
    r0 = 0x20;
    /* dcbz r4, r0 */;
    r7 = 0x0;
    r0 = 0x40;
    /* dcbz r4, r0 */;
    if ((s32)r5 == (s32)0x0) goto L_801EBFF0;
    r9 = *(u32*)((u8*)r3 + 0x6A4);
    r10 = 0x21 - r9;
    r7 = *(u32*)((u8*)r3 + 0x6A0);
    /* subfc. r11, r10, r5 */;
    /* subi r12, r9, 0x1 */;
    if ((s32)r5 > (s32)0x0) goto L_801EBFA0;
    r0 = r9 + r5;
    r9 = r7 << r12;
    *(u32*)((u8*)r3 + 0x6A4) = r0;
    r0 = 0x20 - r5;
    r7 = (u32)r9 >> r0;
    goto L_801EBFCC;
L_801EBFA0: ;
    r9 = *(u32*)((u8*)r3 + 0x69C);
    r0 = r7 << r12;
    r7 = *(u32*)((u8*)r9 + 0x4);
    r11 = r11 + 0x1;
    *(u32*)((u8*)r3 + 0x6A0) = r7;
    r7 = (u32)r7 >> r10;
    *(u32*)((u8*)r3 + 0x69C) = r9;
    r0 = r7 + r0;
    *(u32*)((u8*)r3 + 0x6A4) = r11;
    r11 = 0x20 - r5;
    r7 = (u32)r0 >> r11;
L_801EBFCC: ;
    r0 = (s16)r7;
    r6 = __cntlzw(r0);
    r0 = 0x20 - r5;
    if ((s32)r6 <= (s32)r0) goto L_801EBFF0;
    r0 = -0x1;
    r0 = r0 << r5;
    r7 = r0 + r7;
    r7 = r7 + 0x1;
L_801EBFF0: ;
    r0 = 0x60;
    /* dcbz r4, r0 */;
    r0 = *(s16*)((u8*)r3 + 0x68A);
    r5 = (u32)lbl_80279AE8;
    r8 = (u32)lbl_80279AE8;
    r0 = r0 + r7;
    *(u16*)((u8*)r3 + 0x68A) = r0;
    r6 = 0x1;
    *(u16*)((u8*)r4 + 0x0) = r0;
    goto L_801EC350;
L_801EC018: ;
    r30 = *(u32*)lbl_8047B520;
    r31 = *(u32*)((u8*)r3 + 0x6A4);
    r11 = r30 + 0x20;
    r12 = *(u32*)((u8*)r3 + 0x6A0);
    r5 = r31 + 0x4;
    /* rlwnm r10, r12, r5, 27, 31 */;
    if ((s32)r31 > (s32)0x1c) goto L_801EC0F8;
    r5 = *(u8*)(r30 + r10);
    r11 = *(u8*)(r11 + r10);
    if ((s32)r5 == (s32)0xff) goto L_801EC054;
    r31 = r31 + r11;
    *(u32*)((u8*)r3 + 0x6A4) = r31;
    goto L_801EC29C;
L_801EC054: ;
    r7 = r30 + 0x44;
    r31 = r31 + 0x5;
    r0 = 0x14;
    r5 = 0x5;
    r7 = r7 + 0x14;
L_801EC068: ;
    r10 = r10 << 1;
    if ((s32)r31 == (s32)0x21) goto L_801EC088;
    /* rlwnm r11, r12, r31, 31, 31 */;
    r0 = *(u32*)((u8*)r7 + 0x4);
    r10 = r10 | r11;
    r31 = r31 + 0x1;
    goto L_801EC0CC;
L_801EC088: ;
    r11 = *(u32*)((u8*)r3 + 0x69C);
    r31 = 0x1;
    r12 = *(u32*)((u8*)r11 + 0x4);
    r0 = *(u32*)((u8*)r7 + 0x4);
    *(u32*)((u8*)r3 + 0x69C) = r11;
    r10 = (r10 & ~0x00000001) | (((r12 << 1) | ((u32)r12 >> 31)) & 0x00000001);
    *(u32*)((u8*)r3 + 0x6A0) = r12;
    goto L_801EC0B8;
L_801EC0A8: ;
    r10 = r10 << 1;
    /* rlwnm r11, r12, r31, 31, 31 */;
    r0 = *(u32*)((u8*)r7 + 0x4);
    r10 = r10 | r11;
L_801EC0B8: ;
    r31 = r31 + 0x1;
    r5 = r5 + 0x1;
    if ((s32)r10 > (s32)r0) goto L_801EC0A8;
    goto L_801EC0D8;
L_801EC0CC: ;
    r5 = r5 + 0x1;
    if ((s32)r10 > (s32)r0) goto L_801EC068;
L_801EC0D8: ;
    *(u32*)((u8*)r3 + 0x6A4) = r31;
    r0 = r5 << 2;
    r5 = r30 + r0;
    r7 = *(u32*)((u8*)r30 + 0x40);
    r0 = *(u32*)((u8*)r5 + 0x8C);
    r0 = r0 + r7;
    r5 = *(u8*)(r10 + r0);
    goto L_801EC29C;
L_801EC0F8: ;
    r10 = *(u32*)((u8*)r3 + 0x69C);
    if ((s32)r31 == (s32)0x21) goto L_801EC1B4;
    /* rlwnm r5, r12, r5, 27, 31 */;
    if ((s32)r31 == (s32)0x20) goto L_801EC138;
    r10 = *(u8*)(r30 + r5);
    r11 = *(u8*)(r11 + r5);
    r5 = r31 + r11;
    if ((s32)r10 == (s32)0xff) goto L_801EC218;
    *(u32*)((u8*)r3 + 0x6A4) = r5;
    if ((s32)r5 > (s32)0x21) goto L_801EC218;
    r5 = r10;
    goto L_801EC29C;
L_801EC138: ;
    r12 = *(u32*)((u8*)r10 + 0x4);
    *(u32*)((u8*)r3 + 0x69C) = r10;
    r5 = (r5 & ~0x0000000F) | (((r12 << 4) | ((u32)r12 >> 28)) & 0x0000000F);
    r10 = *(u8*)(r30 + r5);
    r11 = *(u8*)(r11 + r5);
    *(u32*)((u8*)r3 + 0x6A0) = r12;
    *(u32*)((u8*)r3 + 0x6A4) = r11;
    if ((s32)r10 == (s32)0xff) goto L_801EC164;
    r5 = r10;
    goto L_801EC29C;
L_801EC164: ;
    r7 = r30 + 0x44;
    r31 = 0x14;
    r7 = r7 + 0x14;
    r10 = r5 << 27;
    r31 = 0x5;
    r10 = (r10 & ~0x7FFFFFFF) | (((r12 << 31) | ((u32)r12 >> 1)) & 0x7FFFFFFF);
L_801EC17C: ;
    r12 = 0x1f - r31;
    r0 = *(u32*)((u8*)r7 + 0x4);
    r5 = (u32)r10 >> r12;
    r31 = r31 + 0x1;
    if ((s32)r5 > (s32)r0) goto L_801EC17C;
    *(u32*)((u8*)r3 + 0x6A4) = r31;
L_801EC198: ;
    r0 = r31 << 2;
    r9 = *(u32*)((u8*)r30 + 0x40);
    r7 = r30 + r0;
    r0 = *(u32*)((u8*)r7 + 0x8C);
    r0 = r0 + r9;
    r5 = *(u8*)(r5 + r0);
    goto L_801EC29C;
L_801EC1B4: ;
    r12 = *(u32*)((u8*)r10 + 0x4);
    r5 = (u32)r12 >> 27;
    *(u32*)((u8*)r3 + 0x69C) = r10;
    r31 = *(u8*)(r30 + r5);
    r11 = *(u8*)(r11 + r5);
    *(u32*)((u8*)r3 + 0x6A0) = r12;
    r11 = r11 + 0x1;
    if ((s32)r31 == (s32)0xff) goto L_801EC1E4;
    *(u32*)((u8*)r3 + 0x6A4) = r11;
    r5 = r31;
    goto L_801EC29C;
L_801EC1E4: ;
    r31 = 0x5;
    r7 = 0x14;
L_801EC1EC: ;
    r10 = 0x1f - r31;
    r31 = r31 + 0x1;
    r7 = r7 + 0x4;
    r5 = (u32)r12 >> r10;
    r9 = r30 + r7;
    r0 = *(u32*)((u8*)r9 + 0x44);
    if ((s32)r5 > (s32)r0) goto L_801EC1EC;
    r0 = r31 + 0x1;
    *(u32*)((u8*)r3 + 0x6A4) = r0;
    goto L_801EC198;
L_801EC218: ;
    r0 = 0x21 - r31;
    r5 = -0x1;
    r9 = r5 << r0;
    r5 = r12 & ~r9;
    r9 = r30 + 0x44;
    r10 = *(u32*)((u8*)r3 + 0x69C);
    r7 = 0x21 - r31;
    r31 = r7 + 0x1;
    r7 = r7 << 2;
    r12 = *(u32*)((u8*)r10 + 0x4);
    r9 = r9 + r7;
    *(u32*)((u8*)r3 + 0x69C) = r10;
    r5 = r5 << 1;
    *(u32*)((u8*)r3 + 0x6A0) = r12;
    r5 = (r5 & ~0x00000001) | (((r12 << 1) | ((u32)r12 >> 31)) & 0x00000001);
    r7 = *(u32*)((u8*)r9 + 0x4);
    r10 = 0x2;
    goto L_801EC274;
L_801EC260: ;
    r5 = r5 << 1;
    r31 = r31 + 0x1;
    r7 = *(u32*)((u8*)r9 + 0x4);
    r5 = r5 + r11;
    r10 = r10 + 0x1;
L_801EC274: ;
    /* rlwnm r11, r12, r10, 31, 31 */;
    if ((s32)r5 > (s32)r7) goto L_801EC260;
    *(u32*)((u8*)r3 + 0x6A4) = r10;
    r0 = r31 << 2;
    r7 = r30 + r0;
    r9 = *(u32*)((u8*)r30 + 0x40);
    r0 = *(u32*)((u8*)r7 + 0x8C);
    r0 = r0 + r9;
    r5 = *(u8*)(r5 + r0);
L_801EC29C: ;
    r30 = r5 & 0xF;
    r7 = (s32)r5 >> 4;
    if ((s32)r5 == (s32)r7) goto L_801EC340;
    r6 = r6 + r7;
    r9 = *(u32*)((u8*)r3 + 0x6A4);
    r10 = 0x21 - r9;
    r7 = *(u32*)((u8*)r3 + 0x6A0);
    /* subf. r11, r10, r30 */;
    /* subi r12, r9, 0x1 */;
    if ((s32)r5 > (s32)r7) goto L_801EC2DC;
    r0 = r9 + r30;
    r9 = r7 << r12;
    *(u32*)((u8*)r3 + 0x6A4) = r0;
    r0 = 0x20 - r30;
    r7 = (u32)r9 >> r0;
    goto L_801EC308;
L_801EC2DC: ;
    r9 = *(u32*)((u8*)r3 + 0x69C);
    r0 = r7 << r12;
    r7 = *(u32*)((u8*)r9 + 0x4);
    r11 = r11 + 0x1;
    *(u32*)((u8*)r3 + 0x6A0) = r7;
    r7 = (u32)r7 >> r10;
    *(u32*)((u8*)r3 + 0x69C) = r9;
    r0 = r7 + r0;
    *(u32*)((u8*)r3 + 0x6A4) = r11;
    r11 = 0x20 - r30;
    r7 = (u32)r0 >> r11;
L_801EC308: ;
    r5 = __cntlzw(r7);
    r0 = 0x20 - r30;
    if ((s32)r5 <= (s32)r0) goto L_801EC328;
    r0 = -0x1;
    r0 = r0 << r30;
    r7 = r0 + r7;
    r7 = r7 + 0x1;
L_801EC328: ;
    r5 = r8 + r6;
    r0 = *(u8*)((u8*)r5 + 0x0);
    r5 = (s16)r7;
    r0 = r0 << 1;
    *(u16*)(r4 + r0) = r5;
    goto L_801EC34C;
L_801EC340: ;
    if ((s32)r7 != (s32)0xf) goto L_801EC358;
    r6 = r6 + 0xf;
L_801EC34C: ;
    r6 = r6 + 0x1;
L_801EC350: ;
    if ((s32)r6 < (s32)0x40) goto L_801EC018;
L_801EC358: ;
    r31 = *(u32*)(sp + 0x14);
    r30 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x801EC368 | size: 0x6A8 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EC368(void) {
    extern u8 lbl_80279AE8[];
    extern u8 lbl_8047B4E0[];
    extern u8 lbl_8047B540[];
    u8 sp[0x18];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* dcbz r0, r4 */;
    r12 = *(u32*)lbl_8047B4E0;
    r11 = *(u32*)((u8*)r3 + 0x6A4);
    r9 = r12 + 0x20;
    r10 = *(u32*)((u8*)r3 + 0x6A0);
    r5 = r11 + 0x4;
    /* rlwnm r8, r10, r5, 27, 31 */;
    if ((s32)r11 > (s32)0x1c) goto L_801EC458;
    r5 = *(u8*)(r12 + r8);
    r9 = *(u8*)(r9 + r8);
    if ((s32)r5 == (s32)0xff) goto L_801EC3B4;
    r11 = r11 + r9;
    *(u32*)((u8*)r3 + 0x6A4) = r11;
    goto L_801EC5FC;
L_801EC3B4: ;
    r6 = r12 + 0x44;
    r11 = r11 + 0x5;
    r0 = 0x14;
    r5 = 0x5;
    r6 = r6 + 0x14;
L_801EC3C8: ;
    r8 = r8 << 1;
    if ((s32)r11 == (s32)0x21) goto L_801EC3E8;
    /* rlwnm r9, r10, r11, 31, 31 */;
    r0 = *(u32*)((u8*)r6 + 0x4);
    r8 = r8 | r9;
    r11 = r11 + 0x1;
    goto L_801EC42C;
L_801EC3E8: ;
    r9 = *(u32*)((u8*)r3 + 0x69C);
    r11 = 0x1;
    r10 = *(u32*)((u8*)r9 + 0x4);
    r0 = *(u32*)((u8*)r6 + 0x4);
    *(u32*)((u8*)r3 + 0x69C) = r9;
    r8 = (r8 & ~0x00000001) | (((r10 << 1) | ((u32)r10 >> 31)) & 0x00000001);
    *(u32*)((u8*)r3 + 0x6A0) = r10;
    goto L_801EC418;
L_801EC408: ;
    r8 = r8 << 1;
    /* rlwnm r9, r10, r11, 31, 31 */;
    r0 = *(u32*)((u8*)r6 + 0x4);
    r8 = r8 | r9;
L_801EC418: ;
    r11 = r11 + 0x1;
    r5 = r5 + 0x1;
    if ((s32)r8 > (s32)r0) goto L_801EC408;
    goto L_801EC438;
L_801EC42C: ;
    r5 = r5 + 0x1;
    if ((s32)r8 > (s32)r0) goto L_801EC3C8;
L_801EC438: ;
    *(u32*)((u8*)r3 + 0x6A4) = r11;
    r0 = r5 << 2;
    r5 = r12 + r0;
    r6 = *(u32*)((u8*)r12 + 0x40);
    r0 = *(u32*)((u8*)r5 + 0x8C);
    r0 = r0 + r6;
    r5 = *(u8*)(r8 + r0);
    goto L_801EC5FC;
L_801EC458: ;
    r8 = *(u32*)((u8*)r3 + 0x69C);
    if ((s32)r11 == (s32)0x21) goto L_801EC514;
    /* rlwnm r5, r10, r5, 27, 31 */;
    if ((s32)r11 == (s32)0x20) goto L_801EC498;
    r8 = *(u8*)(r12 + r5);
    r9 = *(u8*)(r9 + r5);
    r5 = r11 + r9;
    if ((s32)r8 == (s32)0xff) goto L_801EC578;
    *(u32*)((u8*)r3 + 0x6A4) = r5;
    if ((s32)r5 > (s32)0x21) goto L_801EC578;
    r5 = r8;
    goto L_801EC5FC;
L_801EC498: ;
    r10 = *(u32*)((u8*)r8 + 0x4);
    *(u32*)((u8*)r3 + 0x69C) = r8;
    r5 = (r5 & ~0x0000000F) | (((r10 << 4) | ((u32)r10 >> 28)) & 0x0000000F);
    r8 = *(u8*)(r12 + r5);
    r9 = *(u8*)(r9 + r5);
    *(u32*)((u8*)r3 + 0x6A0) = r10;
    *(u32*)((u8*)r3 + 0x6A4) = r9;
    if ((s32)r8 == (s32)0xff) goto L_801EC4C4;
    r5 = r8;
    goto L_801EC5FC;
L_801EC4C4: ;
    r6 = r12 + 0x44;
    r11 = 0x14;
    r6 = r6 + 0x14;
    r8 = r5 << 27;
    r11 = 0x5;
    r8 = (r8 & ~0x7FFFFFFF) | (((r10 << 31) | ((u32)r10 >> 1)) & 0x7FFFFFFF);
L_801EC4DC: ;
    r10 = 0x1f - r11;
    r0 = *(u32*)((u8*)r6 + 0x4);
    r5 = (u32)r8 >> r10;
    r11 = r11 + 0x1;
    if ((s32)r5 > (s32)r0) goto L_801EC4DC;
    *(u32*)((u8*)r3 + 0x6A4) = r11;
L_801EC4F8: ;
    r0 = r11 << 2;
    r7 = *(u32*)((u8*)r12 + 0x40);
    r6 = r12 + r0;
    r0 = *(u32*)((u8*)r6 + 0x8C);
    r0 = r0 + r7;
    r5 = *(u8*)(r5 + r0);
    goto L_801EC5FC;
L_801EC514: ;
    r10 = *(u32*)((u8*)r8 + 0x4);
    r5 = (u32)r10 >> 27;
    *(u32*)((u8*)r3 + 0x69C) = r8;
    r11 = *(u8*)(r12 + r5);
    r9 = *(u8*)(r9 + r5);
    *(u32*)((u8*)r3 + 0x6A0) = r10;
    r9 = r9 + 0x1;
    if ((s32)r11 == (s32)0xff) goto L_801EC544;
    *(u32*)((u8*)r3 + 0x6A4) = r9;
    r5 = r11;
    goto L_801EC5FC;
L_801EC544: ;
    r11 = 0x5;
    r6 = 0x14;
L_801EC54C: ;
    r8 = 0x1f - r11;
    r11 = r11 + 0x1;
    r6 = r6 + 0x4;
    r5 = (u32)r10 >> r8;
    r7 = r12 + r6;
    r0 = *(u32*)((u8*)r7 + 0x44);
    if ((s32)r5 > (s32)r0) goto L_801EC54C;
    r0 = r11 + 0x1;
    *(u32*)((u8*)r3 + 0x6A4) = r0;
    goto L_801EC4F8;
L_801EC578: ;
    r0 = 0x21 - r11;
    r5 = -0x1;
    r7 = r5 << r0;
    r5 = r10 & ~r7;
    r7 = r12 + 0x44;
    r8 = *(u32*)((u8*)r3 + 0x69C);
    r6 = 0x21 - r11;
    r11 = r6 + 0x1;
    r6 = r6 << 2;
    r10 = *(u32*)((u8*)r8 + 0x4);
    r7 = r7 + r6;
    *(u32*)((u8*)r3 + 0x69C) = r8;
    r5 = r5 << 1;
    *(u32*)((u8*)r3 + 0x6A0) = r10;
    r5 = (r5 & ~0x00000001) | (((r10 << 1) | ((u32)r10 >> 31)) & 0x00000001);
    r6 = *(u32*)((u8*)r7 + 0x4);
    r8 = 0x2;
    goto L_801EC5D4;
L_801EC5C0: ;
    r5 = r5 << 1;
    r11 = r11 + 0x1;
    r6 = *(u32*)((u8*)r7 + 0x4);
    r5 = r5 + r9;
    r8 = r8 + 0x1;
L_801EC5D4: ;
    /* rlwnm r9, r10, r8, 31, 31 */;
    if ((s32)r5 > (s32)r6) goto L_801EC5C0;
    *(u32*)((u8*)r3 + 0x6A4) = r8;
    r0 = r11 << 2;
    r6 = r12 + r0;
    r7 = *(u32*)((u8*)r12 + 0x40);
    r0 = *(u32*)((u8*)r6 + 0x8C);
    r0 = r0 + r7;
    r5 = *(u8*)(r5 + r0);
L_801EC5FC: ;
    r0 = 0x20;
    /* dcbz r4, r0 */;
    r7 = 0x0;
    r0 = 0x40;
    /* dcbz r4, r0 */;
    if ((s32)r5 == (s32)0x0) goto L_801EC698;
    r9 = *(u32*)((u8*)r3 + 0x6A4);
    r10 = 0x21 - r9;
    r7 = *(u32*)((u8*)r3 + 0x6A0);
    /* subf. r11, r10, r5 */;
    /* subi r12, r9, 0x1 */;
    if ((s32)r5 > (s32)0x0) goto L_801EC648;
    r0 = r9 + r5;
    r9 = r7 << r12;
    *(u32*)((u8*)r3 + 0x6A4) = r0;
    r0 = 0x20 - r5;
    r7 = (u32)r9 >> r0;
    goto L_801EC674;
L_801EC648: ;
    r9 = *(u32*)((u8*)r3 + 0x69C);
    r0 = r7 << r12;
    r7 = *(u32*)((u8*)r9 + 0x4);
    r11 = r11 + 0x1;
    *(u32*)((u8*)r3 + 0x6A0) = r7;
    r7 = (u32)r7 >> r10;
    *(u32*)((u8*)r3 + 0x69C) = r9;
    r0 = r7 + r0;
    *(u32*)((u8*)r3 + 0x6A4) = r11;
    r11 = 0x20 - r5;
    r7 = (u32)r0 >> r11;
L_801EC674: ;
    r0 = (s16)r7;
    r6 = __cntlzw(r0);
    r0 = 0x20 - r5;
    if ((s32)r6 <= (s32)r0) goto L_801EC698;
    r0 = -0x1;
    r0 = r0 << r5;
    r7 = r0 + r7;
    r7 = r7 + 0x1;
L_801EC698: ;
    r0 = 0x60;
    /* dcbz r4, r0 */;
    r0 = *(s16*)((u8*)r3 + 0x690);
    r5 = (u32)lbl_80279AE8;
    r8 = (u32)lbl_80279AE8;
    r0 = r0 + r7;
    *(u16*)((u8*)r3 + 0x690) = r0;
    r6 = 0x1;
    *(u16*)((u8*)r4 + 0x0) = r0;
    goto L_801EC9F8;
L_801EC6C0: ;
    r30 = *(u32*)lbl_8047B540;
    r31 = *(u32*)((u8*)r3 + 0x6A4);
    r11 = r30 + 0x20;
    r12 = *(u32*)((u8*)r3 + 0x6A0);
    r5 = r31 + 0x4;
    /* rlwnm r10, r12, r5, 27, 31 */;
    if ((s32)r31 > (s32)0x1c) goto L_801EC7A0;
    r5 = *(u8*)(r30 + r10);
    r11 = *(u8*)(r11 + r10);
    if ((s32)r5 == (s32)0xff) goto L_801EC6FC;
    r31 = r31 + r11;
    *(u32*)((u8*)r3 + 0x6A4) = r31;
    goto L_801EC944;
L_801EC6FC: ;
    r7 = r30 + 0x44;
    r31 = r31 + 0x5;
    r0 = 0x14;
    r5 = 0x5;
    r7 = r7 + 0x14;
L_801EC710: ;
    r10 = r10 << 1;
    if ((s32)r31 == (s32)0x21) goto L_801EC730;
    /* rlwnm r11, r12, r31, 31, 31 */;
    r0 = *(u32*)((u8*)r7 + 0x4);
    r10 = r10 | r11;
    r31 = r31 + 0x1;
    goto L_801EC774;
L_801EC730: ;
    r11 = *(u32*)((u8*)r3 + 0x69C);
    r31 = 0x1;
    r12 = *(u32*)((u8*)r11 + 0x4);
    r0 = *(u32*)((u8*)r7 + 0x4);
    *(u32*)((u8*)r3 + 0x69C) = r11;
    r10 = (r10 & ~0x00000001) | (((r12 << 1) | ((u32)r12 >> 31)) & 0x00000001);
    *(u32*)((u8*)r3 + 0x6A0) = r12;
    goto L_801EC760;
L_801EC750: ;
    r10 = r10 << 1;
    /* rlwnm r11, r12, r31, 31, 31 */;
    r0 = *(u32*)((u8*)r7 + 0x4);
    r10 = r10 | r11;
L_801EC760: ;
    r31 = r31 + 0x1;
    r5 = r5 + 0x1;
    if ((s32)r10 > (s32)r0) goto L_801EC750;
    goto L_801EC780;
L_801EC774: ;
    r5 = r5 + 0x1;
    if ((s32)r10 > (s32)r0) goto L_801EC710;
L_801EC780: ;
    *(u32*)((u8*)r3 + 0x6A4) = r31;
    r0 = r5 << 2;
    r5 = r30 + r0;
    r7 = *(u32*)((u8*)r30 + 0x40);
    r0 = *(u32*)((u8*)r5 + 0x8C);
    r0 = r0 + r7;
    r5 = *(u8*)(r10 + r0);
    goto L_801EC944;
L_801EC7A0: ;
    r10 = *(u32*)((u8*)r3 + 0x69C);
    if ((s32)r31 == (s32)0x21) goto L_801EC85C;
    /* rlwnm r5, r12, r5, 27, 31 */;
    if ((s32)r31 == (s32)0x20) goto L_801EC7E0;
    r10 = *(u8*)(r30 + r5);
    r11 = *(u8*)(r11 + r5);
    r5 = r31 + r11;
    if ((s32)r10 == (s32)0xff) goto L_801EC8C0;
    *(u32*)((u8*)r3 + 0x6A4) = r5;
    if ((s32)r5 > (s32)0x21) goto L_801EC8C0;
    r5 = r10;
    goto L_801EC944;
L_801EC7E0: ;
    r12 = *(u32*)((u8*)r10 + 0x4);
    *(u32*)((u8*)r3 + 0x69C) = r10;
    r5 = (r5 & ~0x0000000F) | (((r12 << 4) | ((u32)r12 >> 28)) & 0x0000000F);
    r10 = *(u8*)(r30 + r5);
    r11 = *(u8*)(r11 + r5);
    *(u32*)((u8*)r3 + 0x6A0) = r12;
    *(u32*)((u8*)r3 + 0x6A4) = r11;
    if ((s32)r10 == (s32)0xff) goto L_801EC80C;
    r5 = r10;
    goto L_801EC944;
L_801EC80C: ;
    r7 = r30 + 0x44;
    r31 = 0x14;
    r7 = r7 + 0x14;
    r10 = r5 << 27;
    r31 = 0x5;
    r10 = (r10 & ~0x7FFFFFFF) | (((r12 << 31) | ((u32)r12 >> 1)) & 0x7FFFFFFF);
L_801EC824: ;
    r12 = 0x1f - r31;
    r0 = *(u32*)((u8*)r7 + 0x4);
    r5 = (u32)r10 >> r12;
    r31 = r31 + 0x1;
    if ((s32)r5 > (s32)r0) goto L_801EC824;
    *(u32*)((u8*)r3 + 0x6A4) = r31;
L_801EC840: ;
    r0 = r31 << 2;
    r9 = *(u32*)((u8*)r30 + 0x40);
    r7 = r30 + r0;
    r0 = *(u32*)((u8*)r7 + 0x8C);
    r0 = r0 + r9;
    r5 = *(u8*)(r5 + r0);
    goto L_801EC944;
L_801EC85C: ;
    r12 = *(u32*)((u8*)r10 + 0x4);
    r5 = (u32)r12 >> 27;
    *(u32*)((u8*)r3 + 0x69C) = r10;
    r31 = *(u8*)(r30 + r5);
    r11 = *(u8*)(r11 + r5);
    *(u32*)((u8*)r3 + 0x6A0) = r12;
    r11 = r11 + 0x1;
    if ((s32)r31 == (s32)0xff) goto L_801EC88C;
    *(u32*)((u8*)r3 + 0x6A4) = r11;
    r5 = r31;
    goto L_801EC944;
L_801EC88C: ;
    r31 = 0x5;
    r7 = 0x14;
L_801EC894: ;
    r10 = 0x1f - r31;
    r31 = r31 + 0x1;
    r7 = r7 + 0x4;
    r5 = (u32)r12 >> r10;
    r9 = r30 + r7;
    r0 = *(u32*)((u8*)r9 + 0x44);
    if ((s32)r5 > (s32)r0) goto L_801EC894;
    r0 = r31 + 0x1;
    *(u32*)((u8*)r3 + 0x6A4) = r0;
    goto L_801EC840;
L_801EC8C0: ;
    r0 = 0x21 - r31;
    r5 = -0x1;
    r9 = r5 << r0;
    r5 = r12 & ~r9;
    r9 = r30 + 0x44;
    r10 = *(u32*)((u8*)r3 + 0x69C);
    r7 = 0x21 - r31;
    r31 = r7 + 0x1;
    r7 = r7 << 2;
    r12 = *(u32*)((u8*)r10 + 0x4);
    r9 = r9 + r7;
    *(u32*)((u8*)r3 + 0x69C) = r10;
    r5 = r5 << 1;
    *(u32*)((u8*)r3 + 0x6A0) = r12;
    r5 = (r5 & ~0x00000001) | (((r12 << 1) | ((u32)r12 >> 31)) & 0x00000001);
    r7 = *(u32*)((u8*)r9 + 0x4);
    r10 = 0x2;
    goto L_801EC91C;
L_801EC908: ;
    r5 = r5 << 1;
    r31 = r31 + 0x1;
    r7 = *(u32*)((u8*)r9 + 0x4);
    r5 = r5 + r11;
    r10 = r10 + 0x1;
L_801EC91C: ;
    /* rlwnm r11, r12, r10, 31, 31 */;
    if ((s32)r5 > (s32)r7) goto L_801EC908;
    *(u32*)((u8*)r3 + 0x6A4) = r10;
    r0 = r31 << 2;
    r7 = r30 + r0;
    r9 = *(u32*)((u8*)r30 + 0x40);
    r0 = *(u32*)((u8*)r7 + 0x8C);
    r0 = r0 + r9;
    r5 = *(u8*)(r5 + r0);
L_801EC944: ;
    r30 = r5 & 0xF;
    r7 = (s32)r5 >> 4;
    if ((s32)r5 == (s32)r7) goto L_801EC9E8;
    r6 = r6 + r7;
    r9 = *(u32*)((u8*)r3 + 0x6A4);
    r10 = 0x21 - r9;
    r7 = *(u32*)((u8*)r3 + 0x6A0);
    /* subf. r11, r10, r30 */;
    /* subi r12, r9, 0x1 */;
    if ((s32)r5 > (s32)r7) goto L_801EC984;
    r0 = r9 + r30;
    r9 = r7 << r12;
    *(u32*)((u8*)r3 + 0x6A4) = r0;
    r0 = 0x20 - r30;
    r7 = (u32)r9 >> r0;
    goto L_801EC9B0;
L_801EC984: ;
    r9 = *(u32*)((u8*)r3 + 0x69C);
    r0 = r7 << r12;
    r7 = *(u32*)((u8*)r9 + 0x4);
    r11 = r11 + 0x1;
    *(u32*)((u8*)r3 + 0x6A0) = r7;
    r7 = (u32)r7 >> r10;
    *(u32*)((u8*)r3 + 0x69C) = r9;
    r0 = r7 + r0;
    *(u32*)((u8*)r3 + 0x6A4) = r11;
    r11 = 0x20 - r30;
    r7 = (u32)r0 >> r11;
L_801EC9B0: ;
    r5 = __cntlzw(r7);
    r0 = 0x20 - r30;
    if ((s32)r5 <= (s32)r0) goto L_801EC9D0;
    r0 = -0x1;
    r0 = r0 << r30;
    r7 = r0 + r7;
    r7 = r7 + 0x1;
L_801EC9D0: ;
    r5 = r8 + r6;
    r0 = *(u8*)((u8*)r5 + 0x0);
    r5 = (s16)r7;
    r0 = r0 << 1;
    *(u16*)(r4 + r0) = r5;
    goto L_801EC9F4;
L_801EC9E8: ;
    if ((s32)r7 != (s32)0xf) goto L_801ECA00;
    r6 = r6 + 0xf;
L_801EC9F4: ;
    r6 = r6 + 0x1;
L_801EC9F8: ;
    if ((s32)r6 < (s32)0x40) goto L_801EC6C0;
L_801ECA00: ;
    r31 = *(u32*)(sp + 0x14);
    r30 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x801ECA10 | size: 0xA0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801ECA10(void) {
    extern u8 lbl_8046D500[];
    extern u8 lbl_80478D08[];
    extern u8 lbl_8047B5B4[];
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    r4 = (u32)lbl_8046D500;
    r31 = (u32)lbl_8046D500;
    r3 = *(u32*)lbl_80478D08;
    OSRegisterVersion();
    r4 = (0xe000 << 16);
    *(u32*)((u8*)r31 + 0x100) = r4;
    r4 = r4 + 0x2000;
    *(u32*)((u8*)r31 + 0x104) = r4;
    r4 = r4 + 0x800;
    *(u32*)((u8*)r31 + 0x108) = r4;
    r4 = (0xe000 << 16);
    *(u32*)((u8*)r31 + 0x10C) = r4;
    r4 = r4 + 0x2800;
    *(u32*)((u8*)r31 + 0x110) = r4;
    r4 = r4 + 0xa00;
    *(u32*)((u8*)r31 + 0x114) = r4;
    r3 = 0x4;
    r3 = r3 | (0x4 << 16);
    /* mtspr GQR2, r3 */;
    r3 = 0x5;
    r3 = r3 | (0x5 << 16);
    /* mtspr GQR3, r3 */;
    r3 = 0x6;
    r3 = r3 | (0x6 << 16);
    /* mtspr GQR4, r3 */;
    r3 = 0x7;
    r3 = r3 | (0x7 << 16);
    /* mtspr GQR5, r3 */;
    r0 = 0x1;
    *(u32*)lbl_8047B5B4 = r0;
    r3 = 0x1;
    return;
}
#pragma pop

/* 0x801ECAB0 | size: 0x464 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801ECAB0(void) {
    extern void fn_800C4C50();
    extern void fn_800C4C98();
    extern void fn_801ECF14();
    extern void fn_801ECFA4();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r20, 0x28(r1) */;
    r31 = r4 + 0x0;
    if ((u32)r3 == (u32)0x0) goto L_801ECAD4;
    if ((u32)r31 != (u32)0x0) goto L_801ECADC;
L_801ECAD4: ;
    r3 = 0x0;
    goto L_801ECF00;
L_801ECADC: ;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r4 = r31 + 0x50;
    r27 = r31 + r0;
    r27 = r27 + 0x50;
    if ((s32)r5 != (s32)0x1) goto L_801ECB0C;
    r0 = *(u32*)((u8*)r31 + 0x4);
    r30 = r3 + 0x0;
    r26 = 0x1;
    r0 = r0 << 1;
    r29 = r3 + r0;
    goto L_801ECB18;
L_801ECB0C: ;
    r30 = r3 + 0x0;
    r29 = r3 + 0x2;
    r26 = 0x2;
L_801ECB18: ;
    r0 = *(u32*)((u8*)r31 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_801ECC74;
    r3 = r1 + 0x14;
    fn_801ECFA4();
    r25 = (0x1 << 16);
    r21 = *(s16*)((u8*)r31 + 0x48);
    r23 = (0x8000 << 16);
    r20 = *(s16*)((u8*)r31 + 0x4A);
    r27 = r26 << 1;
    /* subi r26, r25, 0x1 */;
    /* subi r24, r23, 0x1 */;
    r22 = 0x0;
    goto L_801ECC64;
L_801ECB50: ;
    r3 = r1 + 0x14;
    fn_801ECF14();
    r5 = *(u8*)((u8*)r1 + 0x1C);
    r6 = (s16)r20;
    r0 = *(u8*)((u8*)r1 + 0x1D);
    r4 = (s16)r21;
    r5 = r5 << 2;
    r5 = r31 + r5;
    r7 = *(s16*)((u8*)r5 + 0xA);
    r0 = r3 << r0;
    r3 = *(s16*)((u8*)r5 + 0x8);
    r5 = r0 << 11;
    r6 = r7 * r6;
    r4 = r3 * r4;
    r3 = (s32)r6 >> 31;
    r0 = (s32)r4 >> 31;
    r4 = r6 + r4;
    r3 = r3 + r0; /* +carry */;
    r0 = (s32)r5 >> 31;
    r4 = r4 + r5;
    r3 = r3 + r0; /* +carry */;
    r5 = 0x5;
    fn_800C4C50();
    r0 = r4 & r26;
    r0 = r0 & 0xFFFF;
    r6 = 0x0;
    r7 = r3 & r6;
    if ((u32)r0 <= (u32)0x8000) goto L_801ECBD0;
    r4 = r4 + r25;
    r3 = r3 + r6; /* +carry */;
    goto L_801ECBF0;
L_801ECBD0: ;
    if ((u32)r0 != (u32)0x8000) goto L_801ECBF0;
    r0 = r4 & r25;
    r5 = r0 ^ r6;
    r0 = r7 ^ r6;
    /* or. r0, r5, r0 */;
    if ((u32)r0 == (u32)0x8000) goto L_801ECBF0;
    r4 = r4 + r25;
    r3 = r3 + r6; /* +carry */;
L_801ECBF0: ;
    r0 = 0x0;
    /* xoris r6, r0, 0x8000 */;
    /* xoris r5, r3, 0x8000 */;
    r0 = r24 - r4;
    r5 = r6 - r5; /* -borrow */;
    r5 = r6 - r6; /* -borrow */;
    /* neg. r5, r5 */;
    if ((u32)r0 == (u32)0x8000) goto L_801ECC18;
    /* subi r4, r23, 0x1 */;
    r3 = 0x0;
L_801ECC18: ;
    r0 = -0x1;
    /* xoris r5, r0, 0x8000 */;
    /* xoris r6, r3, 0x8000 */;
    r0 = r4 - r23;
    r5 = r6 - r5; /* -borrow */;
    r5 = r6 - r6; /* -borrow */;
    /* neg. r5, r5 */;
    if ((u32)r0 == (u32)0x8000) goto L_801ECC40;
    r4 = (0x8000 << 16);
    r3 = -0x1;
L_801ECC40: ;
    r5 = 0x10;
    fn_800C4C98();
    *(u16*)((u8*)r29 + 0x0) = r4;
    r20 = r21;
    r21 = r4 + 0x0;
    *(u16*)((u8*)r30 + 0x0) = r4;
    r29 = r29 + r27;
    r30 = r30 + r27;
    r22 = r22 + 0x1;
L_801ECC64: ;
    r0 = *(u32*)((u8*)r31 + 0x4);
    if ((u32)r22 < (u32)r0) goto L_801ECB50;
    goto L_801ECEFC;
L_801ECC74: ;
    r3 = r1 + 0x14;
    fn_801ECFA4();
    r24 = (0x1 << 16);
    r22 = *(s16*)((u8*)r31 + 0x48);
    r25 = (0x8000 << 16);
    r20 = *(s16*)((u8*)r31 + 0x4A);
    r28 = r26 << 1;
    /* subi r23, r24, 0x1 */;
    /* subi r26, r25, 0x1 */;
    r21 = 0x0;
    goto L_801ECDAC;
L_801ECCA0: ;
    r3 = r1 + 0x14;
    fn_801ECF14();
    r5 = *(u8*)((u8*)r1 + 0x1C);
    r6 = (s16)r20;
    r0 = *(u8*)((u8*)r1 + 0x1D);
    r4 = (s16)r22;
    r5 = r5 << 2;
    r5 = r31 + r5;
    r7 = *(s16*)((u8*)r5 + 0xA);
    r0 = r3 << r0;
    r3 = *(s16*)((u8*)r5 + 0x8);
    r5 = r0 << 11;
    r6 = r7 * r6;
    r4 = r3 * r4;
    r3 = (s32)r6 >> 31;
    r0 = (s32)r4 >> 31;
    r4 = r6 + r4;
    r3 = r3 + r0; /* +carry */;
    r0 = (s32)r5 >> 31;
    r4 = r4 + r5;
    r3 = r3 + r0; /* +carry */;
    r5 = 0x5;
    fn_800C4C50();
    r0 = r4 & r23;
    r0 = r0 & 0xFFFF;
    r6 = 0x0;
    r7 = r3 & r6;
    if ((u32)r0 <= (u32)0x8000) goto L_801ECD20;
    r4 = r4 + r24;
    r3 = r3 + r6; /* +carry */;
    goto L_801ECD40;
L_801ECD20: ;
    if ((u32)r0 != (u32)0x8000) goto L_801ECD40;
    r0 = r4 & r24;
    r5 = r0 ^ r6;
    r0 = r7 ^ r6;
    /* or. r0, r5, r0 */;
    if ((u32)r0 == (u32)0x8000) goto L_801ECD40;
    r4 = r4 + r24;
    r3 = r3 + r6; /* +carry */;
L_801ECD40: ;
    r7 = 0x0;
    /* xoris r6, r7, 0x8000 */;
    /* xoris r5, r3, 0x8000 */;
    r0 = r26 - r4;
    r5 = r6 - r5; /* -borrow */;
    r5 = r6 - r6; /* -borrow */;
    /* neg. r5, r5 */;
    if ((u32)r0 == (u32)0x8000) goto L_801ECD68;
    /* subi r4, r25, 0x1 */;
    r3 = r7 + 0x0;
L_801ECD68: ;
    r7 = -0x1;
    /* xoris r6, r3, 0x8000 */;
    /* xoris r5, r7, 0x8000 */;
    r0 = r4 - r25;
    r5 = r6 - r5; /* -borrow */;
    r5 = r6 - r6; /* -borrow */;
    /* neg. r5, r5 */;
    if ((u32)r0 == (u32)0x8000) goto L_801ECD90;
    r4 = (0x8000 << 16);
    r3 = r7 + 0x0;
L_801ECD90: ;
    r5 = 0x10;
    fn_800C4C98();
    *(u16*)((u8*)r29 + 0x0) = r4;
    r20 = r22 + 0x0;
    r22 = r4 + 0x0;
    r29 = r29 + r28;
    r21 = r21 + 0x1;
L_801ECDAC: ;
    r0 = *(u32*)((u8*)r31 + 0x4);
    if ((u32)r21 < (u32)r0) goto L_801ECCA0;
    r3 = r1 + 0x14;
    r4 = r27 + 0x0;
    fn_801ECFA4();
    r26 = (0x1 << 16);
    r23 = *(s16*)((u8*)r31 + 0x4C);
    r27 = (0x8000 << 16);
    r21 = *(s16*)((u8*)r31 + 0x4E);
    /* subi r25, r26, 0x1 */;
    /* subi r29, r27, 0x1 */;
    r22 = 0x0;
    goto L_801ECEF0;
L_801ECDE4: ;
    r3 = r1 + 0x14;
    fn_801ECF14();
    r5 = *(u8*)((u8*)r1 + 0x1C);
    r6 = (s16)r21;
    r0 = *(u8*)((u8*)r1 + 0x1D);
    r4 = (s16)r23;
    r5 = r5 << 2;
    r5 = r31 + r5;
    r7 = *(s16*)((u8*)r5 + 0x2A);
    r0 = r3 << r0;
    r3 = *(s16*)((u8*)r5 + 0x28);
    r5 = r0 << 11;
    r6 = r7 * r6;
    r4 = r3 * r4;
    r3 = (s32)r6 >> 31;
    r0 = (s32)r4 >> 31;
    r4 = r6 + r4;
    r3 = r3 + r0; /* +carry */;
    r0 = (s32)r5 >> 31;
    r4 = r4 + r5;
    r3 = r3 + r0; /* +carry */;
    r5 = 0x5;
    fn_800C4C50();
    r0 = r4 & r25;
    r0 = r0 & 0xFFFF;
    r6 = 0x0;
    r7 = r3 & r6;
    if ((u32)r0 <= (u32)0x8000) goto L_801ECE64;
    r4 = r4 + r26;
    r3 = r3 + r6; /* +carry */;
    goto L_801ECE84;
L_801ECE64: ;
    if ((u32)r0 != (u32)0x8000) goto L_801ECE84;
    r0 = r4 & r26;
    r5 = r0 ^ r6;
    r0 = r7 ^ r6;
    /* or. r0, r5, r0 */;
    if ((u32)r0 == (u32)0x8000) goto L_801ECE84;
    r4 = r4 + r26;
    r3 = r3 + r6; /* +carry */;
L_801ECE84: ;
    r7 = 0x0;
    /* xoris r6, r7, 0x8000 */;
    /* xoris r5, r3, 0x8000 */;
    r0 = r29 - r4;
    r5 = r6 - r5; /* -borrow */;
    r5 = r6 - r6; /* -borrow */;
    /* neg. r5, r5 */;
    if ((u32)r0 == (u32)0x8000) goto L_801ECEAC;
    /* subi r4, r27, 0x1 */;
    r3 = r7 + 0x0;
L_801ECEAC: ;
    r7 = -0x1;
    /* xoris r6, r3, 0x8000 */;
    /* xoris r5, r7, 0x8000 */;
    r0 = r4 - r27;
    r5 = r6 - r5; /* -borrow */;
    r5 = r6 - r6; /* -borrow */;
    /* neg. r5, r5 */;
    if ((u32)r0 == (u32)0x8000) goto L_801ECED4;
    r4 = (0x8000 << 16);
    r3 = r7 + 0x0;
L_801ECED4: ;
    r5 = 0x10;
    fn_800C4C98();
    *(u16*)((u8*)r30 + 0x0) = r4;
    r21 = r23 + 0x0;
    r23 = r4 + 0x0;
    r30 = r30 + r28;
    r22 = r22 + 0x1;
L_801ECEF0: ;
    r0 = *(u32*)((u8*)r31 + 0x4);
    if ((u32)r22 < (u32)r0) goto L_801ECDE4;
L_801ECEFC: ;
    r3 = *(u32*)((u8*)r31 + 0x4);
L_801ECF00: ;
    /* lmw r20, 0x28(r1) */;
    return;
}
#pragma pop

/* 0x801ECF14 | size: 0x90 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801ECF14(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r0 = *(u32*)((u8*)r3 + 0x4);
    r0 = r0 & 0xF;
    if ((s32)r0 != (s32)0) goto L_801ECF58;
    r4 = *(u32*)((u8*)r3 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    /* extrwi r0, r0, 3, 25 */;
    *(u8*)((u8*)r3 + 0x8) = r0;
    r4 = *(u32*)((u8*)r3 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    r0 = r0 & 0xF;
    *(u8*)((u8*)r3 + 0x9) = r0;
    r4 = *(u32*)((u8*)r3 + 0x0);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r3 + 0x0) = r0;
    r4 = *(u32*)((u8*)r3 + 0x4);
    r0 = r4 + 0x2;
    *(u32*)((u8*)r3 + 0x4) = r0;
L_801ECF58: ;
    r0 = *(u32*)((u8*)r3 + 0x4);
    r0 = r0 & 0x1;
    if ((s32)r0 == (s32)0) goto L_801ECF80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r4 = *(u8*)((u8*)r5 + 0x0);
    r0 = r5 + 0x1;
    *(u32*)((u8*)r3 + 0x0) = r0;
    r0 = r4 << 28;
    r5 = (s32)r0 >> 28;
    goto L_801ECF90;
L_801ECF80: ;
    r4 = *(u32*)((u8*)r3 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    /* extlwi r0, r0, 4, 24 */;
    r5 = (s32)r0 >> 28;
L_801ECF90: ;
    r4 = *(u32*)((u8*)r3 + 0x4);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r3 + 0x4) = r0;
    r3 = r5;
    return;
}
#pragma pop

/* 0x801ECFA4 | size: 0x3C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801ECFA4(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    *(u32*)((u8*)r3 + 0x0) = r4;
    r0 = 0x2;
    *(u32*)((u8*)r3 + 0x4) = r0;
    r4 = *(u32*)((u8*)r3 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    /* extrwi r0, r0, 3, 25 */;
    *(u8*)((u8*)r3 + 0x8) = r0;
    r4 = *(u32*)((u8*)r3 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    r0 = r0 & 0xF;
    *(u8*)((u8*)r3 + 0x9) = r0;
    r4 = *(u32*)((u8*)r3 + 0x0);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r3 + 0x0) = r0;
    return;
}
#pragma pop

/* 0x801ECFE0 | size: 0xEC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801ECFE0(void) {
    extern void fn_8011ED68();
    extern void fn_8011EE28();
    extern void fn_80124A60();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* stmw r29, 0x14(r1) */;
    r29 = r4;
    r30 = r3;
    if ((u32)r3 != (u32)0x0) goto L_801ED010;
    r3 = 0x0;
    r4 = 0xb;
    ((void(*)(void))fn_80129280)();
    r30 = r3;
L_801ED010: ;
    if ((u32)r29 != (u32)0x0) goto L_801ED020;
    r3 = 0x0;
    goto L_801ED0B8;
L_801ED020: ;
    r3 = r30;
    if ((u32)r30 != (u32)0x0) goto L_801ED038;
    r3 = 0x0;
    r4 = 0xb;
    ((void(*)(void))fn_80129280)();
L_801ED038: ;
    /* addic. r31, r3, 0x8 */;
    if ((u32)r30 != (u32)0x0) goto L_801ED048;
    r3 = 0x0;
    goto L_801ED0B8;
L_801ED048: ;
    r0 = 0x27;
    /* subi r5, r29, 0x4 */;
    /* subi r4, r31, 0x4 */;
    ctr_fn = (void(*)(void))r0;
L_801ED058: ;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = r0;
    if (--ctr != 0) goto L_801ED058;
    r3 = r31;
    fn_8011ED68();
    r0 = r3 & 0xFF;
    if ((u32)r30 == (u32)0x0) goto L_801ED084;
    r3 = r31;
    fn_8011EE28();
L_801ED084: ;
    if ((u32)r30 != (u32)0x0) goto L_801ED09C;
    r3 = 0x0;
    r4 = 0xb;
    ((void(*)(void))fn_80129280)();
    r30 = r3;
L_801ED09C: ;
    if ((u32)r30 == (u32)0x0) goto L_801ED0AC;
    r0 = 0x0;
    *(u8*)((u8*)r30 + 0x0) = r0;
L_801ED0AC: ;
    r3 = r31;
    fn_80124A60();
    r3 = 0x1;
L_801ED0B8: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801ED0CC | size: 0x14C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801ED0CC(void) {
    extern void fn_8011ED68();
    extern void fn_8011EE28();
    extern void fn_8011F4A8();
    extern void fn_80121ADC();
    extern void fn_80121B4C();
    extern void fn_80124A60();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* stmw r29, 0x14(r1) */;
    r31 = r4;
    r29 = r3;
    if ((u32)r3 != (u32)0x0) goto L_801ED0FC;
    r3 = 0x0;
    r4 = 0xb;
    ((void(*)(void))fn_80129280)();
    r29 = r3;
L_801ED0FC: ;
    if ((u32)r31 != (u32)0x0) goto L_801ED10C;
    r3 = 0x0;
    goto L_801ED204;
L_801ED10C: ;
    r3 = r31;
    fn_8011F4A8();
    r30 = r3;
    r3 = r29;
    if ((u32)r29 != (u32)0x0) goto L_801ED130;
    r3 = 0x0;
    r4 = 0xb;
    ((void(*)(void))fn_80129280)();
L_801ED130: ;
    if ((u32)r3 == (u32)0x0) goto L_801ED13C;
    *(u8*)((u8*)r3 + 0x1) = r30;
L_801ED13C: ;
    r3 = r29;
    if ((u32)r29 != (u32)0x0) goto L_801ED154;
    r3 = 0x0;
    r4 = 0xb;
    ((void(*)(void))fn_80129280)();
L_801ED154: ;
    /* addic. r30, r3, 0x8 */;
    if ((u32)r29 != (u32)0x0) goto L_801ED164;
    r3 = 0x0;
    goto L_801ED204;
L_801ED164: ;
    r3 = r31;
    r4 = 0x3e;
    fn_80121ADC();
    r0 = r3 & 0xFF;
    if ((u32)r29 == (u32)0x0) goto L_801ED184;
    r3 = r31;
    r4 = 0x3e;
    fn_80121B4C();
L_801ED184: ;
    r0 = 0x27;
    /* subi r5, r30, 0x4 */;
    /* subi r4, r31, 0x4 */;
    ctr_fn = (void(*)(void))r0;
L_801ED194: ;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = r0;
    if (--ctr != 0) goto L_801ED194;
    r3 = r31;
    fn_8011ED68();
    r0 = r3 & 0xFF;
    if ((u32)r29 == (u32)0x0) goto L_801ED1C8;
    r3 = r31;
    fn_8011EE28();
    *(u32*)((u8*)r29 + 0x4) = r3;
    goto L_801ED1D0;
L_801ED1C8: ;
    r0 = 0x0;
    *(u32*)((u8*)r29 + 0x4) = r0;
L_801ED1D0: ;
    if ((u32)r29 != (u32)0x0) goto L_801ED1E8;
    r3 = 0x0;
    r4 = 0xb;
    ((void(*)(void))fn_80129280)();
    r29 = r3;
L_801ED1E8: ;
    if ((u32)r29 == (u32)0x0) goto L_801ED1F8;
    r0 = 0x1;
    *(u8*)((u8*)r29 + 0x0) = r0;
L_801ED1F8: ;
    r3 = r31;
    fn_80124A60();
    r3 = 0x1;
L_801ED204: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801ED218 | size: 0x34 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801ED218(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    if ((u32)r3 != (u32)0x0) goto L_801ED238;
    r3 = 0x0;
    r4 = 0xb;
    ((void(*)(void))fn_80129280)();
L_801ED238: ;
    r3 = *(u8*)((u8*)r3 + 0x0);
    return;
}
#pragma pop

/* 0x801ED24C | size: 0x48 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801ED24C(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    if ((u32)r3 != (u32)0x0) goto L_801ED26C;
    r3 = 0x0;
    r4 = 0xb;
    ((void(*)(void))fn_80129280)();
L_801ED26C: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_801ED280;
    r3 = *(u32*)((u8*)r3 + 0x4);
    goto L_801ED284;
L_801ED280: ;
    r3 = -0x1;
L_801ED284: ;
    return;
}
#pragma pop

/* 0x801ED294 | size: 0x48 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801ED294(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    if ((u32)r3 != (u32)0x0) goto L_801ED2B4;
    r3 = 0x0;
    r4 = 0xb;
    ((void(*)(void))fn_80129280)();
L_801ED2B4: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_801ED2C8;
    r3 = *(u8*)((u8*)r3 + 0x1);
    goto L_801ED2CC;
L_801ED2C8: ;
    r3 = 0xff;
L_801ED2CC: ;
    return;
}
#pragma pop

/* 0x801ED2DC | size: 0x34 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801ED2DC(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    if ((u32)r3 != (u32)0x0) goto L_801ED2FC;
    r3 = 0x0;
    r4 = 0xb;
    ((void(*)(void))fn_80129280)();
L_801ED2FC: ;
    r3 = r3 + 0x8;
    return;
}
#pragma pop

/* 0x801ED310 | size: 0x78 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801ED310(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r31 = r3;
    if ((u32)r3 != (u32)0x0) goto L_801ED33C;
    r3 = 0x0;
    r4 = 0xb;
    ((void(*)(void))fn_80129280)();
    r31 = r3;
L_801ED33C: ;
    if ((u32)r31 == (u32)0x0) goto L_801ED374;
    r3 = r31;
    if ((u32)r31 != (u32)0x0) goto L_801ED358;
    r3 = 0x0;
    r4 = 0xb;
    ((void(*)(void))fn_80129280)();
L_801ED358: ;
    if ((u32)r3 == (u32)0x0) goto L_801ED368;
    r0 = 0x0;
    *(u16*)((u8*)r3 + 0x2) = r0;
L_801ED368: ;
    r0 = 0x0;
    *(u8*)((u8*)r31 + 0x1) = r0;
    *(u8*)((u8*)r31 + 0x0) = r0;
L_801ED374: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801ED388 | size: 0x30 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801ED388(void) {
    extern u8 lbl_8047B5B8[];
    extern void fn_8012BDE0();
    extern void fn_801ED3B8();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = (u32)fn_801ED3B8;
    r4 = 0x0;
    r3 = (u32)fn_801ED3B8;
    fn_8012BDE0();
    *(u32*)lbl_8047B5B8 = r3;
    return;
}
#pragma pop

/* 0x801ED3B8 | size: 0x288 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801ED3B8(void) {
    extern void fn_8011EE28();
    extern void fn_8011F4A8();
    extern void fn_8011F4C0();
    extern void fn_8011F910();
    extern void fn_8012361C();
    extern void fn_80123FBC();
    extern void fn_80125424();
    extern void fn_801906A0();
    extern void fn_8019075C();
    extern void fn_801EE470();
    extern void fn_801EE4DC();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = 0x0;
    r4 = 0x2;
    /* stmw r27, 0x1c(r1) */;
    ((void(*)(void))fn_80129280)();
    /* mr. r29, r3 */;
    if ((s32)r0 == (s32)0) goto L_801ED4A4;
    r30 = 0x0;
    goto L_801ED498;
L_801ED3E4: ;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    ((void(*)(void))fn_8012AC08)();
    r28 = r3;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) goto L_801ED494;
    if ((u32)r28 == (u32)0x0) goto L_801ED494;
    r3 = r28;
    ((void(*)(void))fn_8011EE40)();
    r0 = r3 & 0xFFFF;
    r27 = r3;
    if ((u32)r28 == (u32)0x0) goto L_801ED42C;
    ((void(*)(void))fn_801EEC74)();
    r0 = r3 & 0xFF;
    if ((u32)r28 == (u32)0x0) goto L_801ED42C;
    r27 = 0x0;
L_801ED42C: ;
    r0 = r27 & 0xFFFF;
    if ((u32)r28 == (u32)0x0) goto L_801ED494;
    r3 = r28;
    fn_8011EE28();
    if ((u32)r3 == (u32)0x0) goto L_801ED494;
    r3 = r27;
    fn_801EE470();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)0x100) goto L_801ED478;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x1;
    fn_8011F910();
    r3 = r27;
    r4 = 0x0;
    fn_801EE4DC();
    goto L_801ED494;
L_801ED478: ;
    r3 = r27;
    fn_801EE470();
    r4 = r3;
    r3 = r27;
    r0 = r4 + 0x1;
    r4 = r0 & 0xFFFF;
    fn_801EE4DC();
L_801ED494: ;
    r30 = r30 + 0x1;
L_801ED498: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 < (u32)0x6) goto L_801ED3E4;
L_801ED4A4: ;
    r3 = 0xd0;
    fn_801906A0();
    if ((u32)r3 == (u32)0x0) goto L_801ED4DC;
    r3 = 0xd1;
    fn_801906A0();
    r31 = r3 & 0xFFFF;
    if ((u32)r31 >= (u32)0x2710) goto L_801ED4D0;
    r0 = r31 + 0x1;
    r31 = r0 & 0xFFFF;
L_801ED4D0: ;
    r4 = r31 & 0xFFFF;
    r3 = 0xd1;
    fn_8019075C();
L_801ED4DC: ;
    r3 = 0x0;
    r4 = 0xb;
    ((void(*)(void))fn_80129280)();
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_801ED62C;
    r3 = 0x0;
    r4 = 0xb;
    ((void(*)(void))fn_80129280)();
    /* addic. r27, r3, 0x8 */;
    r28 = 0x0;
    if ((u32)r0 == (u32)0x0) goto L_801ED54C;
    r3 = r27;
    ((void(*)(void))fn_8011EE40)();
    r0 = r3 & 0xFFFF;
    r28 = r3;
    if ((u32)r0 == (u32)0x0) goto L_801ED530;
    ((void(*)(void))fn_801EEC74)();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_801ED530;
    r28 = 0x0;
L_801ED530: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 == (u32)0x0) goto L_801ED54C;
    r3 = r28;
    ((void(*)(void))fn_801EEC74)();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_801ED54C;
    r28 = 0x0;
L_801ED54C: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 == (u32)0x0) goto L_801ED5B8;
    r3 = r27;
    fn_8011EE28();
    if ((u32)r3 == (u32)0x0) goto L_801ED614;
    r3 = r28;
    fn_801EE470();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)0x100) goto L_801ED598;
    r3 = r27;
    r4 = 0x0;
    r5 = 0x3;
    fn_8011F910();
    r3 = r28;
    r4 = 0x0;
    fn_801EE4DC();
    goto L_801ED614;
L_801ED598: ;
    r3 = r28;
    fn_801EE470();
    r4 = r3;
    r3 = r28;
    r0 = r4 + 0x1;
    r4 = r0 & 0xFFFF;
    fn_801EE4DC();
    goto L_801ED614;
L_801ED5B8: ;
    r3 = r27;
    fn_8011F4A8();
    r0 = r3 & 0xFF;
    if ((u32)r0 >= (u32)0x64) goto L_801ED614;
    r3 = r27;
    fn_8011F4C0();
    r0 = -0x1;
    r4 = r3;
    if ((u32)r4 >= (u32)r0) goto L_801ED5E8;
    r4 = r4 + 0x1;
L_801ED5E8: ;
    r3 = r27;
    fn_80125424();
    r0 = 0x0;
    r3 = r27;
    *(u8*)(sp + 0x8) = r0;
    fn_8011F4A8();
    r4 = r3;
    r3 = r27;
    r6 = r1 + 0x8;
    r5 = 0x1;
    fn_8012361C();
L_801ED614: ;
    r3 = 0x0;
    r4 = 0xb;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 == (u32)0x0) goto L_801ED62C;
    *(u16*)((u8*)r3 + 0x2) = r31;
L_801ED62C: ;
    /* lmw r27, 0x1c(r1) */;
    return;
}
#pragma pop

/* 0x801ED648 | size: 0x2C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801ED648(void) {
    extern u8 lbl_80375230[];
    extern void fn_800E01D0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r5 = (u32)lbl_80375230;
    r4 = r3;
    r3 = (u32)lbl_80375230;
    fn_800E01D0();
    return;
}
#pragma pop

/* 0x801ED674 | size: 0xC | tiny */
void fn_801ED674(void) { }

/* 0x801ED680 | size: 0xC0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801ED680(void) {
    extern u8 lbl_8046D630[];
    extern u8 lbl_8047B5C0[];
    extern u8 lbl_8047B5C4[];
    extern u8 lbl_8047B5C8[];
    extern void fn_800EFD3C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;

    r28 = r3;
    *(u32*)lbl_8047B5C4 = r28;
    r3 = *(u32*)((u8*)r3 + 0x0);
    /* subis r0, r3, 0x7b1e */;
    if ((u32)r0 != (u32)0xe3f0) goto L_801ED718;
    r4 = r28 + 0x1c;
    r3 = (u32)lbl_8046D630;
    *(u32*)lbl_8047B5C8 = r4;
    r31 = (u32)lbl_8046D630;
    r30 = 0x0;
    r0 = *(u16*)((u8*)r28 + 0x6);
    r0 = r0 * 0xc;
    r29 = r4 + r0;
    goto L_801ED708;
L_801ED6D8: ;
    r0 = *(u32*)((u8*)r29 + 0x4);
    r0 = r0 + r28;
    *(u32*)((u8*)r29 + 0x4) = r0;
    r3 = *(u32*)((u8*)r29 + 0x4);
    fn_800EFD3C();
    if ((u32)r3 == (u32)0x0) goto L_801ED718;
    r0 = *(u16*)((u8*)r29 + 0x0);
    r29 = r29 + 0x8;
    r30 = r30 + 0x1;
    r0 = r0 << 2;
    *(u32*)(r31 + r0) = r3;
L_801ED708: ;
    r3 = *(u32*)lbl_8047B5C4;
    r0 = *(u16*)((u8*)r3 + 0x4);
    if ((s32)r30 < (s32)r0) goto L_801ED6D8;
L_801ED718: ;
    r0 = 0x1;
    *(u8*)lbl_8047B5C0 = r0;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x801ED740 | size: 0x40 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801ED740(void) {
    extern u8 lbl_8047B5C0[];
    extern void fn_800FE834();
    extern void fn_801ED780();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    f32 f0 = 0.0f;

    r3 = (u32)fn_801ED780;
    r4 = 0xf0;
    r0 = 0x0;
    r6 = (u32)fn_801ED780;
    r3 = 0x1;
    *(u8*)lbl_8047B5C0 = r0;
    r5 = 0xa;
    *(u8*)&lbl_8047B5C1 = r0;
    fn_800FE834();
    return;
}
#pragma pop

/* 0x801ED780 | size: 0x8B4 | massive */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801ED780(void) {
    extern u8 lbl_80314958[];
    extern u8 lbl_80314C78[];
    extern u8 lbl_80375230[];
    extern u8 lbl_8046D630[];
    extern u8 lbl_8047B5C0[];
    extern u8 lbl_8047B5C4[];
    extern u8 lbl_8047B5C8[];
    extern u8 lbl_8047E4D0[];
    extern u8 lbl_8047E4D4[];
    extern u8 lbl_8047E4D8[];
    extern u8 lbl_8047E4DC[];
    extern u8 lbl_8047E4E0[];
    extern u8 lbl_8047E4E4[];
    extern u8 lbl_8047E4E8[];
    extern u8 lbl_8047E4EC[];
    extern u8 lbl_8047E4F0[];
    extern u8 lbl_8047E4F8[];
    extern u8 lbl_8047E500[];
    extern void fn_800D1F84();
    extern void fn_800D2584();
    extern void fn_800D2F34();
    extern void fn_800D59B8();
    extern void fn_800D5C18();
    extern void fn_800D6680();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D85D4();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    extern void fn_800D9B58();
    extern void fn_800D9ED8();
    extern void fn_800DA028();
    extern void fn_800DA1E8();
    extern void fn_800DA2BC();
    extern void fn_800DA4C4();
    extern void fn_800E008C();
    extern void fn_800E00AC();
    extern void fn_800E013C();
    extern void fn_800EF4F4();
    extern void fn_800EF4FC();
    extern void fn_80111C24();
    u8 sp[0xC0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0xB0) = f31;
    /* psq_st f31, 0xb8(r1), 0, qr0 */;
    *(f64*)(sp + 0xA0) = f30;
    /* psq_st f30, 0xa8(r1), 0, qr0 */;
    *(f64*)(sp + 0x90) = f29;
    /* psq_st f29, 0x98(r1), 0, qr0 */;
    *(f64*)(sp + 0x80) = f28;
    /* psq_st f28, 0x88(r1), 0, qr0 */;
    *(f64*)(sp + 0x70) = f27;
    /* psq_st f27, 0x78(r1), 0, qr0 */;
    /* stmw r27, 0x5c(r1) */;
    r0 = *(u8*)lbl_8047B5C0;
    if ((u32)r0 == (u32)0x0) goto L_801EDFF8;
    r0 = *(u8*)&lbl_8047B5C1;
    if ((u32)r0 != (u32)0x0) goto L_801ED7D4;
    goto L_801EDFF8;
L_801ED7D4: ;
    fn_800D2584();
    if ((u32)r3 == (u32)0x0) goto L_801EDFF8;
    r4 = r1 + 0x8;
    fn_800D1F84();
    r3 = (u32)lbl_80375230;
    r4 = r1 + 0x8;
    r3 = (u32)lbl_80375230;
    fn_80111C24();
    if ((s32)r3 == (s32)0x1) goto L_801EDFF8;
    r3 = (u32)lbl_80375230;
    r4 = r1 + 0x2c;
    r3 = (u32)lbl_80375230;
    fn_800D2F34();
    /* mr. r27, r3 */;
    if ((s32)r3 == (s32)0x1) goto L_801EDFF8;
    f3 = *(f32*)lbl_8047E4D0;
    r3 = r1 + 0x20;
    f0 = *(f32*)(sp + 0x2C);
    f2 = *(f32*)lbl_8047E4D4;
    f1 = *(f32*)(sp + 0x30);
    f3 = f3 - f0;
    f0 = *(f32*)lbl_8047E4D8;
    f1 = f2 - f1;
    *(f32*)(sp + 0x20) = f3;
    *(f32*)(sp + 0x24) = f1;
    *(f32*)(sp + 0x28) = f0;
    fn_800E008C();
    f31 = f1;
    f0 = *(f32*)lbl_8047E4D8;
    if (f31 <= f0) goto L_801ED85C;
    goto L_801ED860;
L_801ED85C: ;
    f1 = -f31;
L_801ED860: ;
    f0 = *(f32*)lbl_8047E4DC;
    if (f1 < f0) goto L_801EDFF8;
    r3 = *(u32*)lbl_8047B5C4;
    r0 = (0x4330 << 16);
    *(u32*)(sp + 0x38) = r0;
    r0 = *(u32*)((u8*)r3 + 0x8);
    f1 = *(f64*)lbl_8047E4F8;
    *(u32*)(sp + 0x3C) = r0;
    f0 = *(f64*)(sp + 0x38);
    f0 = f0 - f1;
    if (f31 <= f0) goto L_801ED898;
    goto L_801EDFF8;
L_801ED898: ;
    r3 = 0x3;
    fn_800D88DC();
    r3 = 0x4;
    fn_800D888C();
    f1 = *(f32*)lbl_8047E4D8;
    f3 = *(f32*)lbl_8047E4E0;
    f2 = f1;
    f4 = *(f32*)lbl_8047E4E4;
    fn_800D9B58();
    r3 = 0x1;
    r4 = 0x1;
    r5 = 0x1;
    fn_800DA4C4();
    r3 = 0x1;
    r4 = 0x1;
    r5 = 0x0;
    fn_800DA2BC();
    r3 = 0x0;
    r4 = 0x1;
    r5 = 0x1;
    fn_800DA1E8();
    r3 = 0x0;
    fn_800DA028();
    r3 = 0x1;
    fn_800D9ED8();
    if ((s32)r27 != (s32)0x2) goto L_801EDE64;
    f1 = f31;
    r3 = r1 + 0x20;
    r4 = r3;
    fn_800E00AC();
    r5 = *(u32*)lbl_8047B5C4;
    r3 = (0x4330 << 16);
    r4 = *(u32*)((u8*)r5 + 0xC);
    f4 = *(f64*)lbl_8047E4F8;
    f0 = *(f64*)(sp + 0x38);
    f0 = f0 - f4;
    if (f31 >= f0) goto L_801ED944;
    f1 = *(f32*)lbl_8047E4E8;
    goto L_801ED9AC;
L_801ED944: ;
    r0 = *(u32*)((u8*)r5 + 0x8);
    r0 = r0 - r4;
    f0 = *(f32*)lbl_8047E4D8;
    f1 = *(f64*)(sp + 0x40);
    *(u32*)(sp + 0x3C) = r0;
    f1 = f1 - f4;
    f3 = *(f64*)(sp + 0x38);
    f2 = f31 - f1;
    *(u32*)(sp + 0x4C) = r0;
    f3 = f3 - f4;
    f1 = *(f64*)(sp + 0x48);
    f2 = f3 - f2;
    f1 = f1 - f4;
    f1 = f2 / f1;
    if (f1 >= f0) goto L_801ED99C;
    f1 = f0;
    goto L_801ED9AC;
L_801ED99C: ;
    f0 = *(f32*)lbl_8047E4E8;
    if (f1 <= f0) goto L_801ED9AC;
    f1 = f0;
L_801ED9AC: ;
    f0 = *(f32*)lbl_8047E4EC;
    r3 = 0x4;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x48) = f0;
    r28 = *(u32*)(sp + 0x4C);
    fn_800D6A00();
    r3 = (u32)lbl_80314C78;
    r3 = (u32)lbl_80314C78;
    fn_800D7820();
    r4 = *(u32*)lbl_8047B5C4;
    r3 = (u32)lbl_8046D630;
    r30 = *(u32*)lbl_8047B5C8;
    r31 = (u32)lbl_8046D630;
    r27 = *(u16*)((u8*)r4 + 0x6);
    goto L_801EDCD0;
L_801ED9EC: ;
    r3 = *(u16*)((u8*)r30 + 0x2);
    r0 = *(u8*)((u8*)r30 + 0x0);
    r3 = r3 << 2;
    r29 = *(u32*)(r31 + r3);
    if ((u32)r0 != (u32)0x1) goto L_801EDAD8;
    r3 = r29;
    fn_800EF4FC();
    /* extrwi r3, r3, 15, 16 */;
    r0 = (0x4330 << 16);
    /* xoris r3, r3, 0x8000 */;
    *(u32*)(sp + 0x48) = r0;
    f3 = *(f64*)lbl_8047E500;
    r3 = r29;
    f1 = *(f32*)((u8*)r30 + 0x8);
    f2 = *(f64*)(sp + 0x48);
    f0 = *(f32*)(sp + 0x2C);
    f2 = f2 - f3;
    f30 = -(f1 * f2 - f0);
    fn_800EF4F4();
    /* extrwi r3, r3, 15, 16 */;
    r0 = (0x4330 << 16);
    /* xoris r3, r3, 0x8000 */;
    *(u32*)(sp + 0x40) = r0;
    f3 = *(f64*)lbl_8047E500;
    r3 = r29;
    f1 = *(f32*)((u8*)r30 + 0x8);
    f2 = *(f64*)(sp + 0x40);
    f0 = *(f32*)(sp + 0x30);
    f2 = f2 - f3;
    f29 = -(f1 * f2 - f0);
    fn_800EF4FC();
    /* extrwi r3, r3, 15, 16 */;
    r0 = (0x4330 << 16);
    /* xoris r3, r3, 0x8000 */;
    *(u32*)(sp + 0x38) = r0;
    f3 = *(f64*)lbl_8047E500;
    r3 = r29;
    f1 = *(f32*)((u8*)r30 + 0x8);
    f2 = *(f64*)(sp + 0x38);
    f0 = *(f32*)(sp + 0x2C);
    f2 = f2 - f3;
    f28 = f1 * f2 + f0;
    fn_800EF4F4();
    /* extrwi r3, r3, 15, 16 */;
    r0 = (0x4330 << 16);
    /* xoris r3, r3, 0x8000 */;
    *(u32*)(sp + 0x50) = r0;
    f3 = *(f64*)lbl_8047E500;
    f1 = *(f32*)((u8*)r30 + 0x8);
    f2 = *(f64*)(sp + 0x50);
    f0 = *(f32*)(sp + 0x30);
    f2 = f2 - f3;
    f27 = f1 * f2 + f0;
    goto L_801EDBE4;
L_801EDAD8: ;
    f1 = *(f32*)lbl_8047E4F0;
    r3 = r1 + 0x14;
    f0 = *(f32*)((u8*)r30 + 0x4);
    r4 = r1 + 0x20;
    f0 = f1 * f0;
    f1 = f0 * f31;
    fn_800E013C();
    r3 = r29;
    fn_800EF4FC();
    /* extrwi r3, r3, 15, 16 */;
    r0 = (0x4330 << 16);
    /* xoris r3, r3, 0x8000 */;
    *(u32*)(sp + 0x50) = r0;
    f3 = *(f64*)lbl_8047E500;
    r3 = r29;
    f1 = *(f32*)(sp + 0x14);
    f2 = *(f64*)(sp + 0x50);
    f0 = *(f32*)lbl_8047E4D0;
    f3 = f2 - f3;
    f2 = *(f32*)((u8*)r30 + 0x8);
    f0 = f1 + f0;
    f30 = -(f2 * f3 - f0);
    fn_800EF4F4();
    /* extrwi r3, r3, 15, 16 */;
    r0 = (0x4330 << 16);
    /* xoris r3, r3, 0x8000 */;
    *(u32*)(sp + 0x48) = r0;
    f3 = *(f64*)lbl_8047E500;
    r3 = r29;
    f1 = *(f32*)(sp + 0x18);
    f2 = *(f64*)(sp + 0x48);
    f0 = *(f32*)lbl_8047E4D4;
    f3 = f2 - f3;
    f2 = *(f32*)((u8*)r30 + 0x8);
    f0 = f1 + f0;
    f29 = -(f2 * f3 - f0);
    fn_800EF4FC();
    /* extrwi r3, r3, 15, 16 */;
    r0 = (0x4330 << 16);
    /* xoris r3, r3, 0x8000 */;
    *(u32*)(sp + 0x40) = r0;
    f3 = *(f64*)lbl_8047E500;
    r3 = r29;
    f1 = *(f32*)(sp + 0x14);
    f2 = *(f64*)(sp + 0x40);
    f0 = *(f32*)lbl_8047E4D0;
    f3 = f2 - f3;
    f2 = *(f32*)((u8*)r30 + 0x8);
    f0 = f1 + f0;
    f28 = f2 * f3 + f0;
    fn_800EF4F4();
    /* extrwi r3, r3, 15, 16 */;
    r0 = (0x4330 << 16);
    /* xoris r3, r3, 0x8000 */;
    *(u32*)(sp + 0x38) = r0;
    f3 = *(f64*)lbl_8047E500;
    f1 = *(f32*)(sp + 0x18);
    f2 = *(f64*)(sp + 0x38);
    f0 = *(f32*)lbl_8047E4D4;
    f3 = f2 - f3;
    f2 = *(f32*)((u8*)r30 + 0x8);
    f0 = f1 + f0;
    f27 = f2 * f3 + f0;
L_801EDBE4: ;
    r4 = r29;
    r3 = 0x0;
    fn_800D85D4();
    r3 = 0x4;
    fn_800D67BC();
    f1 = f30;
    f3 = *(f32*)lbl_8047E4D8;
    f2 = f29;
    fn_800D6680();
    r4 = r28 & 0xFF;
    r3 = 0x0;
    r5 = r4;
    r6 = r4;
    fn_800D5C18();
    f1 = *(f32*)lbl_8047E4D8;
    r3 = 0x0;
    f2 = f1;
    fn_800D59B8();
    f1 = f28;
    f3 = *(f32*)lbl_8047E4D8;
    f2 = f29;
    fn_800D6680();
    r4 = r28 & 0xFF;
    r3 = 0x0;
    r5 = r4;
    r6 = r4;
    fn_800D5C18();
    f1 = *(f32*)lbl_8047E4E8;
    r3 = 0x0;
    f2 = *(f32*)lbl_8047E4D8;
    fn_800D59B8();
    f1 = f30;
    f3 = *(f32*)lbl_8047E4D8;
    f2 = f27;
    fn_800D6680();
    r4 = r28 & 0xFF;
    r3 = 0x0;
    r5 = r4;
    r6 = r4;
    fn_800D5C18();
    f1 = *(f32*)lbl_8047E4D8;
    r3 = 0x0;
    f2 = *(f32*)lbl_8047E4E8;
    fn_800D59B8();
    f1 = f28;
    f3 = *(f32*)lbl_8047E4D8;
    f2 = f27;
    fn_800D6680();
    r4 = r28 & 0xFF;
    r3 = 0x0;
    r5 = r4;
    r6 = r4;
    fn_800D5C18();
    f1 = *(f32*)lbl_8047E4E8;
    r3 = 0x0;
    f2 = f1;
    fn_800D59B8();
    fn_800D6728();
    r30 = r30 + 0xc;
L_801EDCD0: ;
    /* subi r27, r27, 0x1 */;
    if ((s32)r27 != (s32)0x0) goto L_801ED9EC;
    r3 = *(u32*)lbl_8047B5C4;
    r0 = (0x4330 << 16);
    *(u32*)(sp + 0x50) = r0;
    r5 = *(u32*)((u8*)r3 + 0xC);
    f5 = *(f64*)lbl_8047E4F8;
    f0 = *(f64*)(sp + 0x50);
    f0 = f0 - f5;
    if (f31 >= f0) goto L_801EDFF8;
    r4 = *(u32*)((u8*)r3 + 0x10);
    *(u32*)(sp + 0x50) = r0;
    f0 = *(f64*)(sp + 0x50);
    f0 = f0 - f5;
    if (f31 >= f0) goto L_801EDD28;
    f1 = *(f32*)((u8*)r3 + 0x14);
    goto L_801EDD94;
L_801EDD28: ;
    r4 = r5 - r4;
    f4 = *(f32*)((u8*)r3 + 0x14);
    *(u32*)(sp + 0x48) = r0;
    f0 = *(f32*)lbl_8047E4D8;
    f1 = *(f64*)(sp + 0x48);
    f1 = f1 - f5;
    *(u32*)(sp + 0x50) = r0;
    f3 = *(f64*)(sp + 0x50);
    f2 = f31 - f1;
    f3 = f3 - f5;
    *(u32*)(sp + 0x40) = r0;
    f1 = *(f64*)(sp + 0x40);
    f2 = f3 - f2;
    f1 = f1 - f5;
    f1 = f2 / f1;
    f1 = f4 * f1;
    if (f1 >= f0) goto L_801EDD84;
    f1 = f0;
    goto L_801EDD94;
L_801EDD84: ;
    f0 = *(f32*)lbl_8047E4E8;
    if (f1 <= f0) goto L_801EDD94;
    f1 = f0;
L_801EDD94: ;
    f0 = *(f32*)lbl_8047E4EC;
    r3 = 0x2;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x50) = f0;
    r27 = *(u32*)(sp + 0x54);
    fn_800D888C();
    r3 = 0x4;
    fn_800D6A00();
    r3 = (u32)lbl_80314958;
    r3 = (u32)lbl_80314958;
    fn_800D7820();
    r3 = 0x4;
    fn_800D67BC();
    f1 = *(f32*)lbl_8047E4D8;
    f2 = f1;
    f3 = f1;
    fn_800D6680();
    r4 = r27 & 0xFF;
    r3 = 0x0;
    r5 = r4;
    r6 = r4;
    fn_800D5C18();
    f2 = *(f32*)lbl_8047E4D8;
    f1 = *(f32*)lbl_8047E4E0;
    f3 = f2;
    fn_800D6680();
    r4 = r27 & 0xFF;
    r3 = 0x0;
    r5 = r4;
    r6 = r4;
    fn_800D5C18();
    f1 = *(f32*)lbl_8047E4D8;
    f2 = *(f32*)lbl_8047E4E4;
    f3 = f1;
    fn_800D6680();
    r4 = r27 & 0xFF;
    r3 = 0x0;
    r5 = r4;
    r6 = r4;
    fn_800D5C18();
    f1 = *(f32*)lbl_8047E4E0;
    f2 = *(f32*)lbl_8047E4E4;
    f3 = *(f32*)lbl_8047E4D8;
    fn_800D6680();
    r4 = r27 & 0xFF;
    r3 = 0x0;
    r5 = r4;
    r6 = r4;
    fn_800D5C18();
    fn_800D6728();
    goto L_801EDFF8;
L_801EDE64: ;
    r3 = 0x2;
    r4 = 0x1;
    r5 = 0x1;
    fn_800DA4C4();
    r3 = *(u32*)lbl_8047B5C4;
    r0 = (0x4330 << 16);
    *(u32*)(sp + 0x50) = r0;
    r5 = *(u32*)((u8*)r3 + 0xC);
    f5 = *(f64*)lbl_8047E4F8;
    f0 = *(f64*)(sp + 0x50);
    f0 = f0 - f5;
    if (f31 >= f0) goto L_801EDFF8;
    r4 = *(u32*)((u8*)r3 + 0x10);
    *(u32*)(sp + 0x50) = r0;
    f0 = *(f64*)(sp + 0x50);
    f0 = f0 - f5;
    if (f31 >= f0) goto L_801EDEC0;
    f1 = *(f32*)((u8*)r3 + 0x18);
    goto L_801EDF2C;
L_801EDEC0: ;
    r4 = r5 - r4;
    f4 = *(f32*)((u8*)r3 + 0x18);
    *(u32*)(sp + 0x48) = r0;
    f0 = *(f32*)lbl_8047E4D8;
    f1 = *(f64*)(sp + 0x48);
    f1 = f1 - f5;
    *(u32*)(sp + 0x50) = r0;
    f3 = *(f64*)(sp + 0x50);
    f2 = f31 - f1;
    f3 = f3 - f5;
    *(u32*)(sp + 0x40) = r0;
    f1 = *(f64*)(sp + 0x40);
    f2 = f3 - f2;
    f1 = f1 - f5;
    f1 = f2 / f1;
    f1 = f4 * f1;
    if (f1 >= f0) goto L_801EDF1C;
    f1 = f0;
    goto L_801EDF2C;
L_801EDF1C: ;
    f0 = *(f32*)lbl_8047E4E8;
    if (f1 <= f0) goto L_801EDF2C;
    f1 = f0;
L_801EDF2C: ;
    f0 = *(f32*)lbl_8047E4EC;
    r3 = 0x2;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x50) = f0;
    r27 = *(u32*)(sp + 0x54);
    fn_800D888C();
    r3 = 0x4;
    fn_800D6A00();
    r3 = (u32)lbl_80314958;
    r3 = (u32)lbl_80314958;
    fn_800D7820();
    r3 = 0x4;
    fn_800D67BC();
    f1 = *(f32*)lbl_8047E4D8;
    f2 = f1;
    f3 = f1;
    fn_800D6680();
    r4 = r27 & 0xFF;
    r3 = 0x0;
    r5 = r4;
    r6 = r4;
    fn_800D5C18();
    f2 = *(f32*)lbl_8047E4D8;
    f1 = *(f32*)lbl_8047E4E0;
    f3 = f2;
    fn_800D6680();
    r4 = r27 & 0xFF;
    r3 = 0x0;
    r5 = r4;
    r6 = r4;
    fn_800D5C18();
    f1 = *(f32*)lbl_8047E4D8;
    f2 = *(f32*)lbl_8047E4E4;
    f3 = f1;
    fn_800D6680();
    r4 = r27 & 0xFF;
    r3 = 0x0;
    r5 = r4;
    r6 = r4;
    fn_800D5C18();
    f1 = *(f32*)lbl_8047E4E0;
    f2 = *(f32*)lbl_8047E4E4;
    f3 = *(f32*)lbl_8047E4D8;
    fn_800D6680();
    r4 = r27 & 0xFF;
    r3 = 0x0;
    r5 = r4;
    r6 = r4;
    fn_800D5C18();
    fn_800D6728();
L_801EDFF8: ;
    /* psq_l f31, 0xb8(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0xB0);
    /* psq_l f30, 0xa8(r1), 0, qr0 */;
    f30 = *(f64*)(sp + 0xA0);
    /* psq_l f29, 0x98(r1), 0, qr0 */;
    f29 = *(f64*)(sp + 0x90);
    /* psq_l f28, 0x88(r1), 0, qr0 */;
    f28 = *(f64*)(sp + 0x80);
    /* psq_l f27, 0x78(r1), 0, qr0 */;
    f27 = *(f64*)(sp + 0x70);
    /* lmw r27, 0x5c(r1) */;
    return;
}
#pragma pop

/* 0x801EE07C | size: 0x2C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EE07C(void) {
    extern u8 lbl_80478F78[];
    extern u8 lbl_80478F7C[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r4 = *(u32*)lbl_80478F78;
    r5 = r3 & 0xFFFF;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r5 < (u32)r0) goto L_801EE098;
    r3 = *(u32*)lbl_80478F7C;
    return;
L_801EE098: ;
    r3 = *(u32*)lbl_80478F7C;
    r0 = r5 << 3;
    r3 = r3 + r0;
    return;
}
#pragma pop

/* 0x801EE0A8 | size: 0x14 | tiny */
void fn_801EE0A8(void) { }

/* 0x801EE0BC | size: 0x50 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EE0BC(void) {
    extern void fn_801EF1E4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r31 = r3;
    r3 = 0x0;
    fn_801EF1E4();
    if ((u32)r3 == (u32)0x0) goto L_801EE0F4;
    r0 = r31 & 0xFFFF;
    r4 = r0 * 0x18;
    r0 = r4 + 0x4a4;
    r3 = *(s16*)(r3 + r0);
    goto L_801EE0F8;
L_801EE0F4: ;
    r3 = -0x1;
L_801EE0F8: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801EE10C | size: 0x68 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EE10C(void) {
    extern u8 lbl_80478F6C[];
    extern void fn_801EF1E4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = r3 & 0xFFFF;
    /* stmw r30, 0x8(r1) */;
    r0 = r3 * 0x38;
    r30 = r4;
    r5 = *(u32*)lbl_80478F6C;
    r4 = r5 + r0;
    if ((s32)r0 == (s32)0) goto L_801EE13C;
    if ((u32)r3 <= (u32)0x60) goto L_801EE140;
L_801EE13C: ;
    r4 = 0x0;
L_801EE140: ;
    if ((u32)r4 == (u32)0x0) goto L_801EE160;
    r31 = *(u16*)((u8*)r4 + 0xA);
    r3 = 0x0;
    fn_801EF1E4();
    r4 = r31 * 0x18;
    r0 = r4 + 0x49c;
    *(u8*)(r3 + r0) = r30;
L_801EE160: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801EE174 | size: 0x6C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EE174(void) {
    extern u8 lbl_80478F6C[];
    extern void fn_801EF1E4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r3 = r3 & 0xFFFF;
    r0 = r3 * 0x38;
    r4 = *(u32*)lbl_80478F6C;
    r4 = r4 + r0;
    if ((s32)r0 == (s32)0) goto L_801EE1A0;
    if ((u32)r3 <= (u32)0x60) goto L_801EE1A4;
L_801EE1A0: ;
    r4 = 0x0;
L_801EE1A4: ;
    if ((u32)r4 == (u32)0x0) goto L_801EE1C8;
    r31 = *(u16*)((u8*)r4 + 0xA);
    r3 = 0x0;
    fn_801EF1E4();
    r4 = r31 * 0x18;
    r0 = r4 + 0x49c;
    r3 = *(u8*)(r3 + r0);
    goto L_801EE1CC;
L_801EE1C8: ;
    r3 = 0x0;
L_801EE1CC: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801EE1E0 | size: 0x68 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EE1E0(void) {
    extern u8 lbl_80478F6C[];
    extern void fn_801EF1E4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = r3 & 0xFFFF;
    /* stmw r30, 0x8(r1) */;
    r0 = r3 * 0x38;
    r30 = r4;
    r5 = *(u32*)lbl_80478F6C;
    r4 = r5 + r0;
    if ((s32)r0 == (s32)0) goto L_801EE210;
    if ((u32)r3 <= (u32)0x60) goto L_801EE214;
L_801EE210: ;
    r4 = 0x0;
L_801EE214: ;
    if ((u32)r4 == (u32)0x0) goto L_801EE234;
    r31 = *(u16*)((u8*)r4 + 0xA);
    r3 = 0x0;
    fn_801EF1E4();
    r4 = r31 * 0x18;
    r0 = r4 + 0x4a0;
    *(u16*)(r3 + r0) = r30;
L_801EE234: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801EE248 | size: 0x6C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EE248(void) {
    extern u8 lbl_80478F6C[];
    extern void fn_801EF1E4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r3 = r3 & 0xFFFF;
    r0 = r3 * 0x38;
    r4 = *(u32*)lbl_80478F6C;
    r4 = r4 + r0;
    if ((s32)r0 == (s32)0) goto L_801EE274;
    if ((u32)r3 <= (u32)0x60) goto L_801EE278;
L_801EE274: ;
    r4 = 0x0;
L_801EE278: ;
    if ((u32)r4 == (u32)0x0) goto L_801EE29C;
    r31 = *(u16*)((u8*)r4 + 0xA);
    r3 = 0x0;
    fn_801EF1E4();
    r4 = r31 * 0x18;
    r0 = r4 + 0x4a0;
    r3 = *(u16*)(r3 + r0);
    goto L_801EE2A0;
L_801EE29C: ;
    r3 = 0x0;
L_801EE2A0: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801EE2B4 | size: 0x74 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EE2B4(void) {
    extern u8 lbl_80478F6C[];
    extern void fn_800F9E70();
    extern void fn_801EF1E4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = r3 & 0xFFFF;
    /* stmw r30, 0x8(r1) */;
    r0 = r3 * 0x38;
    r30 = r4;
    r5 = *(u32*)lbl_80478F6C;
    r4 = r5 + r0;
    if ((s32)r0 == (s32)0) goto L_801EE2E4;
    if ((u32)r3 <= (u32)0x60) goto L_801EE2E8;
L_801EE2E4: ;
    r4 = 0x0;
L_801EE2E8: ;
    if ((u32)r4 == (u32)0x0) goto L_801EE314;
    r31 = *(u16*)((u8*)r4 + 0xA);
    r3 = 0x0;
    fn_801EF1E4();
    r5 = r31 * 0x18;
    r0 = r3;
    r4 = r30;
    r3 = r5 + 0x490;
    r3 = r0 + r3;
    fn_800F9E70();
L_801EE314: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801EE328 | size: 0x70 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EE328(void) {
    extern u8 lbl_80478F6C[];
    extern void fn_801EF1E4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r3 = r3 & 0xFFFF;
    r0 = r3 * 0x38;
    r4 = *(u32*)lbl_80478F6C;
    r4 = r4 + r0;
    if ((s32)r0 == (s32)0) goto L_801EE354;
    if ((u32)r3 <= (u32)0x60) goto L_801EE358;
L_801EE354: ;
    r4 = 0x0;
L_801EE358: ;
    if ((u32)r4 == (u32)0x0) goto L_801EE380;
    r31 = *(u16*)((u8*)r4 + 0xA);
    r3 = 0x0;
    fn_801EF1E4();
    r4 = r31 * 0x18;
    r0 = r3;
    r3 = r4 + 0x490;
    r3 = r0 + r3;
    goto L_801EE384;
L_801EE380: ;
    r3 = 0x0;
L_801EE384: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801EE398 | size: 0xA8 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EE398(void) {
    extern u8 lbl_80375240[];
    extern u8 lbl_80478F6C[];
    extern void fn_801902E0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = (u32)lbl_80375240;
    /* stmw r30, 0x8(r1) */;
    r30 = 0x0;
    r31 = (u32)lbl_80375240;
    goto L_801EE41C;
L_801EE3B8: ;
    /* clrlslwi r0, r30, 16, 1 */;
    r3 = *(u32*)lbl_80478F6C;
    r4 = *(u16*)(r31 + r0);
    r0 = r4 * 0x38;
    r3 = r3 + r0;
    if ((u32)r4 == (u32)0x0) goto L_801EE3DC;
    if ((u32)r4 <= (u32)0x60) goto L_801EE3E0;
L_801EE3DC: ;
    r3 = 0x0;
L_801EE3E0: ;
    if ((u32)r3 == (u32)0x0) goto L_801EE404;
    r3 = *(u16*)((u8*)r3 + 0x14);
    if ((u32)r3 == (u32)0x0) goto L_801EE3FC;
    fn_801902E0();
    goto L_801EE408;
L_801EE3FC: ;
    r3 = 0x0;
    goto L_801EE408;
L_801EE404: ;
    r3 = 0x0;
L_801EE408: ;
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_801EE418;
    r3 = 0x0;
    goto L_801EE42C;
L_801EE418: ;
    r30 = r30 + 0x1;
L_801EE41C: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 < (u32)0x30) goto L_801EE3B8;
    r3 = 0x1;
L_801EE42C: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801EE440 | size: 0x28 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EE440(void) {
    extern u8 lbl_80375240[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r0 = r3 & 0xFFFF;
    if ((u32)r0 <= (u32)0x30) goto L_801EE454;
    r3 = 0x0;
    return;
L_801EE454: ;
    r4 = (u32)lbl_80375240;
    /* clrlslwi r0, r3, 16, 1 */;
    r3 = (u32)lbl_80375240;
    r3 = *(u16*)(r3 + r0);
    return;
}
#pragma pop

/* 0x801EE470 | size: 0x6C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EE470(void) {
    extern u8 lbl_80478F6C[];
    extern void fn_801EF1E4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r3 = r3 & 0xFFFF;
    r0 = r3 * 0x38;
    r4 = *(u32*)lbl_80478F6C;
    r4 = r4 + r0;
    if ((s32)r0 == (s32)0) goto L_801EE49C;
    if ((u32)r3 <= (u32)0x60) goto L_801EE4A0;
L_801EE49C: ;
    r4 = 0x0;
L_801EE4A0: ;
    if ((u32)r4 == (u32)0x0) goto L_801EE4C4;
    r31 = *(u16*)((u8*)r4 + 0xA);
    r3 = 0x0;
    fn_801EF1E4();
    r4 = r31 * 0xc;
    r0 = r4 + 0x6;
    r3 = *(u16*)(r3 + r0);
    goto L_801EE4C8;
L_801EE4C4: ;
    r3 = 0x0;
L_801EE4C8: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801EE4DC | size: 0x68 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EE4DC(void) {
    extern u8 lbl_80478F6C[];
    extern void fn_801EF1E4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = r3 & 0xFFFF;
    /* stmw r30, 0x8(r1) */;
    r0 = r3 * 0x38;
    r30 = r4;
    r5 = *(u32*)lbl_80478F6C;
    r4 = r5 + r0;
    if ((s32)r0 == (s32)0) goto L_801EE50C;
    if ((u32)r3 <= (u32)0x60) goto L_801EE510;
L_801EE50C: ;
    r4 = 0x0;
L_801EE510: ;
    if ((u32)r4 == (u32)0x0) goto L_801EE530;
    r31 = *(u16*)((u8*)r4 + 0xA);
    r3 = 0x0;
    fn_801EF1E4();
    r4 = r31 * 0xc;
    r0 = r4 + 0x6;
    *(u16*)(r3 + r0) = r30;
L_801EE530: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801EE544 | size: 0xD0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EE544(void) {
    extern u8 lbl_80478F64[];
    extern u8 lbl_80478F6C[];
    extern void fn_801906A0();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = r3 & 0xFFFF;
    /* stmw r29, 0x14(r1) */;
    r0 = r3 * 0x38;
    r29 = r4;
    r31 = 0x0;
    r5 = *(u32*)lbl_80478F6C;
    r4 = r5 + r0;
    if ((s32)r0 == (s32)0) goto L_801EE578;
    if ((u32)r3 <= (u32)0x60) goto L_801EE57C;
L_801EE578: ;
    r4 = 0x0;
L_801EE57C: ;
    if ((u32)r4 == (u32)0x0) goto L_801EE5FC;
    r0 = *(u16*)((u8*)r4 + 0x1E);
    r3 = *(u32*)lbl_80478F64;
    r0 = r0 << 3;
    r30 = r3 + r0;
L_801EE594: ;
    r0 = *(u32*)((u8*)r30 + 0x4);
    if ((u32)r0 == (u32)0x0) goto L_801EE5F4;
    r3 = *(u16*)((u8*)r30 + 0x2);
    if ((u32)r3 == (u32)0x0) goto L_801EE5D8;
    fn_801906A0();
    r0 = *(u8*)((u8*)r30 + 0x0);
    r3 = r3 & 0xFF;
    if ((u32)r3 != (u32)r0) goto L_801EE5EC;
    r31 = *(u32*)((u8*)r30 + 0x4);
    if ((u32)r29 == (u32)0x0) goto L_801EE5EC;
    r0 = *(u8*)((u8*)r30 + 0x1);
    *(u8*)((u8*)r29 + 0x0) = r0;
    goto L_801EE5EC;
L_801EE5D8: ;
    r31 = r0;
    if ((u32)r29 == (u32)0x0) goto L_801EE5EC;
    r0 = *(u8*)((u8*)r30 + 0x1);
    *(u8*)((u8*)r29 + 0x0) = r0;
L_801EE5EC: ;
    r30 = r30 + 0x8;
    goto L_801EE594;
L_801EE5F4: ;
    r3 = r31;
    goto L_801EE600;
L_801EE5FC: ;
    r3 = 0x0;
L_801EE600: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801EE614 | size: 0x68 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EE614(void) {
    extern u8 lbl_80478F6C[];
    extern void fn_801906A0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = r3 & 0xFFFF;
    r0 = r3 * 0x38;
    r4 = *(u32*)lbl_80478F6C;
    r4 = r4 + r0;
    if ((s32)r0 == (s32)0) goto L_801EE63C;
    if ((u32)r3 <= (u32)0x60) goto L_801EE640;
L_801EE63C: ;
    r4 = 0x0;
L_801EE640: ;
    if ((u32)r4 == (u32)0x0) goto L_801EE668;
    r3 = *(u16*)((u8*)r4 + 0x1C);
    if ((u32)r3 == (u32)0x0) goto L_801EE660;
    fn_801906A0();
    r3 = (s8)r3;
    goto L_801EE66C;
L_801EE660: ;
    r3 = 0x0;
    goto L_801EE66C;
L_801EE668: ;
    r3 = 0x0;
L_801EE66C: ;
    return;
}
#pragma pop

/* 0x801EE67C | size: 0xD4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EE67C(void) {
    extern u8 lbl_80478F6C[];
    extern void fn_8019075C();
    extern void fn_801EF1E4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r3 & 0xFFFF;
    r27 = r3;
    r28 = r4;
    r29 = r30 * 0x38;
    r0 = *(u32*)lbl_80478F6C;
    r31 = r0 + r29;
    if ((s32)r0 == (s32)0) goto L_801EE6B0;
    if ((u32)r30 <= (u32)0x60) goto L_801EE6B4;
L_801EE6B0: ;
    r31 = 0x0;
L_801EE6B4: ;
    if ((u32)r31 == (u32)0x0) goto L_801EE73C;
    r3 = 0x0;
    fn_801EF1E4();
    if ((u32)r3 == (u32)0x0) goto L_801EE6F0;
    r4 = r30 * 0x18;
    r5 = r4 + 0x4a4;
    r0 = *(s16*)(r3 + r5);
    if ((s32)r0 >= (s32)0x0) goto L_801EE6F0;
    r4 = *(u32*)((u8*)r3 + 0x0);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r3 + 0x0) = r0;
    *(u16*)(r3 + r5) = r4;
L_801EE6F0: ;
    r3 = *(u32*)lbl_80478F6C;
    r0 = r27 & 0xFFFF;
    r3 = r3 + r29;
    if ((s32)r0 == (s32)0x0) goto L_801EE708;
    if ((u32)r0 <= (u32)0x60) goto L_801EE70C;
L_801EE708: ;
    r3 = 0x0;
L_801EE70C: ;
    if ((u32)r3 == (u32)0x0) goto L_801EE728;
    r3 = *(u16*)((u8*)r3 + 0xC);
    if ((u32)r3 == (u32)0x0) goto L_801EE728;
    r4 = 0x1;
    fn_8019075C();
L_801EE728: ;
    r3 = *(u16*)((u8*)r31 + 0x1C);
    if ((u32)r3 == (u32)0x0) goto L_801EE73C;
    r4 = r28 & 0xFFFF;
    fn_8019075C();
L_801EE73C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x801EE750 | size: 0x6C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EE750(void) {
    extern u8 lbl_80478F6C[];
    extern void fn_801EF1E4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r3 = r3 & 0xFFFF;
    r0 = r3 * 0x38;
    r4 = *(u32*)lbl_80478F6C;
    r4 = r4 + r0;
    if ((s32)r0 == (s32)0) goto L_801EE77C;
    if ((u32)r3 <= (u32)0x60) goto L_801EE780;
L_801EE77C: ;
    r4 = 0x0;
L_801EE780: ;
    if ((u32)r4 == (u32)0x0) goto L_801EE7A4;
    r31 = *(u16*)((u8*)r4 + 0xA);
    r3 = 0x0;
    fn_801EF1E4();
    r4 = r31 * 0xc;
    r0 = r4 + 0x8;
    r3 = *(u32*)(r3 + r0);
    goto L_801EE7A8;
L_801EE7A4: ;
    r3 = 0x0;
L_801EE7A8: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801EE7BC | size: 0x68 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EE7BC(void) {
    extern u8 lbl_80478F6C[];
    extern void fn_801EF1E4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = r3 & 0xFFFF;
    /* stmw r30, 0x8(r1) */;
    r0 = r3 * 0x38;
    r30 = r4;
    r5 = *(u32*)lbl_80478F6C;
    r4 = r5 + r0;
    if ((s32)r0 == (s32)0) goto L_801EE7EC;
    if ((u32)r3 <= (u32)0x60) goto L_801EE7F0;
L_801EE7EC: ;
    r4 = 0x0;
L_801EE7F0: ;
    if ((u32)r4 == (u32)0x0) goto L_801EE810;
    r31 = *(u16*)((u8*)r4 + 0xA);
    r3 = 0x0;
    fn_801EF1E4();
    r4 = r31 * 0xc;
    r0 = r4 + 0x8;
    *(u32*)(r3 + r0) = r30;
L_801EE810: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801EE824 | size: 0x70 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EE824(void) {
    extern u8 lbl_80478F6C[];
    extern void fn_801906A0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r3 = r3 & 0xFFFF;
    r0 = r3 * 0x38;
    r5 = *(u32*)lbl_80478F6C;
    r5 = r5 + r0;
    if ((s32)r0 == (s32)0) goto L_801EE84C;
    if ((u32)r3 <= (u32)0x60) goto L_801EE850;
L_801EE84C: ;
    r5 = 0x0;
L_801EE850: ;
    if ((u32)r5 == (u32)0x0) goto L_801EE880;
    /* clrlslwi r0, r4, 16, 1 */;
    r3 = r5 + r0;
    r3 = *(u16*)((u8*)r3 + 0x24);
    if ((u32)r3 == (u32)0x0) goto L_801EE878;
    fn_801906A0();
    r3 = (s8)r3;
    goto L_801EE884;
L_801EE878: ;
    r3 = 0x0;
    goto L_801EE884;
L_801EE880: ;
    r3 = 0x0;
L_801EE884: ;
    return;
}
#pragma pop

/* 0x801EE894 | size: 0x60 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EE894(void) {
    extern u8 lbl_80478F6C[];
    extern void fn_8019075C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r3 = r3 & 0xFFFF;
    r0 = r3 * 0x38;
    r6 = *(u32*)lbl_80478F6C;
    r6 = r6 + r0;
    if ((s32)r0 == (s32)0) goto L_801EE8BC;
    if ((u32)r3 <= (u32)0x60) goto L_801EE8C0;
L_801EE8BC: ;
    r6 = 0x0;
L_801EE8C0: ;
    if ((u32)r6 == (u32)0x0) goto L_801EE8E4;
    /* clrlslwi r0, r4, 16, 1 */;
    r3 = r6 + r0;
    r3 = *(u16*)((u8*)r3 + 0x24);
    if ((u32)r3 == (u32)0x0) goto L_801EE8E4;
    r4 = (s8)r5;
    fn_8019075C();
L_801EE8E4: ;
    return;
}
#pragma pop

/* 0x801EE8F4 | size: 0x64 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EE8F4(void) {
    extern u8 lbl_80478F6C[];
    extern void fn_801902E0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = r3 & 0xFFFF;
    r0 = r3 * 0x38;
    r4 = *(u32*)lbl_80478F6C;
    r4 = r4 + r0;
    if ((s32)r0 == (s32)0) goto L_801EE91C;
    if ((u32)r3 <= (u32)0x60) goto L_801EE920;
L_801EE91C: ;
    r4 = 0x0;
L_801EE920: ;
    if ((u32)r4 == (u32)0x0) goto L_801EE944;
    r3 = *(u16*)((u8*)r4 + 0x16);
    if ((u32)r3 == (u32)0x0) goto L_801EE93C;
    fn_801902E0();
    goto L_801EE948;
L_801EE93C: ;
    r3 = 0x0;
    goto L_801EE948;
L_801EE944: ;
    r3 = 0x0;
L_801EE948: ;
    return;
}
#pragma pop

/* 0x801EEAD0 | size: 0x64 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EEAD0(void) {
    extern u8 lbl_80478F6C[];
    extern void fn_801902E0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = r3 & 0xFFFF;
    r0 = r3 * 0x38;
    r4 = *(u32*)lbl_80478F6C;
    r4 = r4 + r0;
    if ((s32)r0 == (s32)0) goto L_801EEAF8;
    if ((u32)r3 <= (u32)0x60) goto L_801EEAFC;
L_801EEAF8: ;
    r4 = 0x0;
L_801EEAFC: ;
    if ((u32)r4 == (u32)0x0) goto L_801EEB20;
    r3 = *(u16*)((u8*)r4 + 0x12);
    if ((u32)r3 == (u32)0x0) goto L_801EEB18;
    fn_801902E0();
    goto L_801EEB24;
L_801EEB18: ;
    r3 = 0x0;
    goto L_801EEB24;
L_801EEB20: ;
    r3 = 0x0;
L_801EEB24: ;
    return;
}
#pragma pop

/* 0x801EED30 | size: 0x58 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EED30(void) {
    extern u8 lbl_80478F6C[];
    extern void fn_8019075C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r3 = r3 & 0xFFFF;
    r0 = r3 * 0x38;
    r5 = *(u32*)lbl_80478F6C;
    r5 = r5 + r0;
    if ((s32)r0 == (s32)0) goto L_801EED58;
    if ((u32)r3 <= (u32)0x60) goto L_801EED5C;
L_801EED58: ;
    r5 = 0x0;
L_801EED5C: ;
    if ((u32)r5 == (u32)0x0) goto L_801EED78;
    r3 = *(u16*)((u8*)r5 + 0xE);
    if ((u32)r3 == (u32)0x0) goto L_801EED78;
    r4 = r4 & 0xFF;
    fn_8019075C();
L_801EED78: ;
    return;
}
#pragma pop

/* 0x801EED88 | size: 0x64 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EED88(void) {
    extern u8 lbl_80478F6C[];
    extern void fn_801902E0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = r3 & 0xFFFF;
    r0 = r3 * 0x38;
    r4 = *(u32*)lbl_80478F6C;
    r4 = r4 + r0;
    if ((s32)r0 == (s32)0) goto L_801EEDB0;
    if ((u32)r3 <= (u32)0x60) goto L_801EEDB4;
L_801EEDB0: ;
    r4 = 0x0;
L_801EEDB4: ;
    if ((u32)r4 == (u32)0x0) goto L_801EEDD8;
    r3 = *(u16*)((u8*)r4 + 0xC);
    if ((u32)r3 == (u32)0x0) goto L_801EEDD0;
    fn_801902E0();
    goto L_801EEDDC;
L_801EEDD0: ;
    r3 = 0x0;
    goto L_801EEDDC;
L_801EEDD8: ;
    r3 = 0x0;
L_801EEDDC: ;
    return;
}
#pragma pop

/* 0x801EEDEC | size: 0x58 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EEDEC(void) {
    extern u8 lbl_80478F6C[];
    extern void fn_8019075C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r3 = r3 & 0xFFFF;
    r0 = r3 * 0x38;
    r5 = *(u32*)lbl_80478F6C;
    r5 = r5 + r0;
    if ((s32)r0 == (s32)0) goto L_801EEE14;
    if ((u32)r3 <= (u32)0x60) goto L_801EEE18;
L_801EEE14: ;
    r5 = 0x0;
L_801EEE18: ;
    if ((u32)r5 == (u32)0x0) goto L_801EEE34;
    r3 = *(u16*)((u8*)r5 + 0xC);
    if ((u32)r3 == (u32)0x0) goto L_801EEE34;
    r4 = r4 & 0xFF;
    fn_8019075C();
L_801EEE34: ;
    return;
}
#pragma pop

/* 0x801EEE44 | size: 0x28 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EEE44(void) {
    extern u8 lbl_80478F6C[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = r3 & 0xFFFF;
    r4 = *(u32*)lbl_80478F6C;
    r0 = r3 * 0x38;
    r4 = r4 + r0;
    if ((s32)r0 == (s32)0) goto L_801EEE60;
    if ((u32)r3 <= (u32)0x60) goto L_801EEE64;
L_801EEE60: ;
    r4 = 0x0;
L_801EEE64: ;
    r3 = *(u8*)((u8*)r4 + 0x0);
    return;
}
#pragma pop

/* 0x801EEE6C | size: 0x4C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EEE6C(void) {
    extern void fn_801EF1E4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = r4;
    r3 = 0x0;
    fn_801EF1E4();
    if ((u32)r3 == (u32)0x0) goto L_801EEEA4;
    r0 = r30 & 0xFFFF;
    r4 = r0 * 0x18;
    r0 = r4 + 0x4a6;
    *(u16*)(r3 + r0) = r31;
L_801EEEA4: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801EEEB8 | size: 0x50 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EEEB8(void) {
    extern void fn_801EF1E4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r31 = r3;
    r3 = 0x0;
    fn_801EF1E4();
    if ((u32)r3 == (u32)0x0) goto L_801EEEF0;
    r0 = r31 & 0xFFFF;
    r4 = r0 * 0x18;
    r0 = r4 + 0x4a6;
    r3 = *(u16*)(r3 + r0);
    goto L_801EEEF4;
L_801EEEF0: ;
    r3 = 0x0;
L_801EEEF4: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801EEF08 | size: 0x38 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EEF08(void) {
    extern u8 lbl_80478F6C[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = r3 & 0xFFFF;
    r4 = *(u32*)lbl_80478F6C;
    r0 = r3 * 0x38;
    r4 = r4 + r0;
    if ((s32)r0 == (s32)0) goto L_801EEF24;
    if ((u32)r3 <= (u32)0x60) goto L_801EEF28;
L_801EEF24: ;
    r4 = 0x0;
L_801EEF28: ;
    if ((u32)r4 == (u32)0x0) goto L_801EEF38;
    r3 = *(u16*)((u8*)r4 + 0x8);
    return;
L_801EEF38: ;
    r3 = 0x0;
    return;
}
#pragma pop

/* 0x801EEF40 | size: 0x6C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EEF40(void) {
    extern u8 lbl_80478F6C[];
    extern void fn_801EF1E4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r3 = r3 & 0xFFFF;
    r0 = r3 * 0x38;
    r4 = *(u32*)lbl_80478F6C;
    r4 = r4 + r0;
    if ((s32)r0 == (s32)0) goto L_801EEF6C;
    if ((u32)r3 <= (u32)0x60) goto L_801EEF70;
L_801EEF6C: ;
    r4 = 0x0;
L_801EEF70: ;
    if ((u32)r4 == (u32)0x0) goto L_801EEF94;
    r31 = *(u16*)((u8*)r4 + 0xA);
    r3 = 0x0;
    fn_801EF1E4();
    r4 = r31 * 0xc;
    r0 = r4 + 0xc;
    r3 = *(u32*)(r3 + r0);
    goto L_801EEF98;
L_801EEF94: ;
    r3 = 0x0;
L_801EEF98: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801EEFAC | size: 0x48 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EEFAC(void) {
    extern u8 lbl_80478F6C[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r3 = r3 & 0xFFFF;
    r5 = *(u32*)lbl_80478F6C;
    r0 = r3 * 0x38;
    r5 = r5 + r0;
    if ((s32)r0 == (s32)0) goto L_801EEFC8;
    if ((u32)r3 <= (u32)0x60) goto L_801EEFCC;
L_801EEFC8: ;
    r5 = 0x0;
L_801EEFCC: ;
    if ((u32)r5 == (u32)0x0) goto L_801EEFEC;
    if ((s32)r4 != (s32)0x0) goto L_801EEFE4;
    r3 = *(u16*)((u8*)r5 + 0x4);
    return;
L_801EEFE4: ;
    r3 = *(u16*)((u8*)r5 + 0x6);
    return;
L_801EEFEC: ;
    r3 = 0x0;
    return;
}
#pragma pop

/* 0x801EEFF4 | size: 0x38 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801EEFF4(void) {
    extern u8 lbl_80478F6C[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = r3 & 0xFFFF;
    r4 = *(u32*)lbl_80478F6C;
    r0 = r3 * 0x38;
    r4 = r4 + r0;
    if ((s32)r0 == (s32)0) goto L_801EF010;
    if ((u32)r3 <= (u32)0x60) goto L_801EF014;
L_801EF010: ;
    r4 = 0x0;
L_801EF014: ;
    if ((u32)r4 == (u32)0x0) goto L_801EF024;
    r3 = *(u16*)((u8*)r4 + 0x2);
    return;
L_801EF024: ;
    r3 = 0x0;
    return;
}
#pragma pop


#pragma pop
