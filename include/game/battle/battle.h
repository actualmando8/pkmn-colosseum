#ifndef GAME_BATTLE_H
#define GAME_BATTLE_H

#include "dolphin/types.h"

/**
 * @file battle.h
 * @brief Pokemon Colosseum Battle Engine - Core structures and declarations.
 *
 * The battle engine spans 0x801C3108 - 0x801F000C (~180KB, ~525 functions)
 * and is organized into the following subsystems:
 *
 *   0x801C3108 - 0x801C53BC : Battle Grid (scene layout, 36 functions)
 *   0x801C53BC - 0x801D7230 : Battle Logic (damage, types, status, ~205 functions)
 *   0x801D7230 - 0x801E03D4 : Waza/Move Animation System (~108 functions)
 *   0x801E03D4 - 0x801EF02C : Battle State Machine (~150 functions)
 *   0x801EF02C - 0x801F000C : Battle Core / Fight Flow (26 functions)
 *
 * Colosseum uses exclusively Double Battles with up to 4 Pokemon on the
 * field at once (2 per side). The engine is built on Gen III mechanics
 * with additions for Shadow Pokemon (Hyper Mode, Shadow Rush, snagging).
 *
 * Key data structures live in BSS:
 *   lbl_8046D500 (0x230 bytes) : Battle state machine context
 *   lbl_8046AC60 (0x100 bytes) : Battle transfer / communication context
 *   lbl_8046A440 (0xA20 bytes) : Battle timer / round tracking context
 *   lbl_80467030 (0x20  bytes) : Battle camera context
 *   lbl_80466E50 (0x1E0 bytes) : Battle scene animation context
 *
 * The battle state machine uses a large switch-case (jumptable at
 * jumptable_803751B8) with 13+ states managing the flow from
 * encounter setup through move selection, execution, and resolution.
 */

/* =========================================================================
 * Forward declarations
 * ========================================================================= */

/* Pokemon data structure size is 0xE0 (224) bytes, indexed via mulli rN, rN, 0xE0 */
struct BattlePokemon;

/* =========================================================================
 * Constants
 * ========================================================================= */

/* Battle positions: Colosseum is always Double Battle */
#define BATTLE_POS_PLAYER_LEFT   0
#define BATTLE_POS_PLAYER_RIGHT  1
#define BATTLE_POS_ENEMY_LEFT    2
#define BATTLE_POS_ENEMY_RIGHT   3
#define BATTLE_NUM_POSITIONS     4

/* Number of Pokemon per side in a Double Battle */
#define BATTLE_POKEMON_PER_SIDE  2
#define BATTLE_TOTAL_POKEMON     4

/* Pokemon types (Gen III type IDs) */
#define TYPE_NORMAL    0
#define TYPE_FIGHTING  1
#define TYPE_FLYING    2
#define TYPE_POISON    3
#define TYPE_GROUND    4
#define TYPE_ROCK      5
#define TYPE_BUG       6
#define TYPE_GHOST     7
#define TYPE_STEEL     8
#define TYPE_FIRE      9
#define TYPE_WATER    10
#define TYPE_GRASS    11
#define TYPE_ELECTRIC 12
#define TYPE_PSYCHIC  13
#define TYPE_ICE      14
#define TYPE_DRAGON   15
#define TYPE_DARK     16
#define TYPE_SHADOW   17  /* Colosseum-exclusive Shadow type */
#define TYPE_COUNT    18

/* Type effectiveness multipliers (stored as fixed-point: 0=immune, 5=0.5x, 10=1x, 20=2x) */
#define TYPE_EFF_IMMUNE     0
#define TYPE_EFF_NOT_VERY   5
#define TYPE_EFF_NORMAL    10
#define TYPE_EFF_SUPER     20

/* Status conditions (primary - mutually exclusive) */
#define STATUS_NONE       0x00
#define STATUS_SLEEP      0x07  /* Bits 0-2: sleep counter */
#define STATUS_POISON     0x08
#define STATUS_BURN       0x10
#define STATUS_FREEZE     0x20
#define STATUS_PARALYSIS  0x40
#define STATUS_TOXIC      0x80

/* Volatile status (can stack) */
#define VSTATUS_CONFUSION   (1 << 0)
#define VSTATUS_FLINCH      (1 << 1)
#define VSTATUS_ATTRACT     (1 << 2)
#define VSTATUS_FOCUS       (1 << 3)
#define VSTATUS_CHARGE      (1 << 4)

/* Move categories */
#define MOVE_CAT_PHYSICAL  0
#define MOVE_CAT_SPECIAL   1
#define MOVE_CAT_STATUS    2

/* Shadow Pokemon states */
#define SHADOW_NORMAL      0
#define SHADOW_HYPER_MODE  1

/* Battle mode constants (from menuToolBattle.c assert string) */
#define BATTLEMODE_BATTLEYAMA100  0  /* Story mode battle */

/* Battle grid model slots (from fn_801C3D64 / fn_801C3F10) */
#define GRID_SLOT_POKEMON  0
#define GRID_SLOT_TRAINER  1

/* Battle state machine states (from jumptable_803751B8, 13 entries) */
#define BATTLE_STATE_INIT          0
#define BATTLE_STATE_INTRO         1
#define BATTLE_STATE_SEND_OUT      2
#define BATTLE_STATE_COMMAND_SEL   3
#define BATTLE_STATE_TARGET_SEL    4
#define BATTLE_STATE_TURN_ORDER    5
#define BATTLE_STATE_EXECUTE_MOVE  6
#define BATTLE_STATE_APPLY_DAMAGE  7
#define BATTLE_STATE_CHECK_FAINT   8
#define BATTLE_STATE_SWITCH_IN     9
#define BATTLE_STATE_END_TURN     10
#define BATTLE_STATE_RESULT       11
#define BATTLE_STATE_CLEANUP      12

/* Waza (move) sequence entry types (from wazaSequenceEntryStart error strings) */
#define WAZA_ENTRY_PARTICLE  0
#define WAZA_ENTRY_MODEL     1
#define WAZA_ENTRY_CAMERA    2
#define WAZA_ENTRY_SOUND     3

/* Scene object IDs referenced by battle_FightEnd (from lbl_80279B84 rodata table) */
/* These are the 21 scene object indices cleaned up during fight end */
#define BATTLE_SCENE_OBJ_COUNT  21

/* =========================================================================
 * Structures
 * ========================================================================= */

/**
 * Battle camera state, stored at lbl_80467030.
 * Controls the 3D camera during battle (pan, zoom, rotation).
 * Size: 0x20 bytes.
 */
typedef struct BattleCameraState {
    void*  pCameraObj;       /* 0x00: Camera scene object pointer */
    void*  pCameraObj2;      /* 0x04: Secondary camera object */
    u16    frameCounter;     /* 0x08: Frame counter for animations */
    u16    cameraMode;       /* 0x0A: Current camera behavior mode */
    u16    padding_0C;       /* 0x0C: Camera mode subcategory */
    u16    padding_0E;       /* 0x0E */
    f32    currentAngle;     /* 0x10: Current rotation angle */
    f32    targetAngle;      /* 0x14: Target rotation angle */
    f32    maxAngle;         /* 0x18: Maximum angle bound */
    u32    reserved;         /* 0x1C */
} BattleCameraState;

/**
 * Battle grid entry for a single position on the field.
 * Used by battleGridSetup (fn_801C3430) for scene layout.
 */
typedef struct BattleGridSlot {
    void*  pModel;           /* 0x00: HSD JObj pointer for the model */
    u8     side;             /* 0x04: 0 = player side, 1 = enemy side */
    u8     position;         /* 0x05: 0 = left, 1 = right */
    u8     occupied;         /* 0x06: 1 if a Pokemon is in this slot */
    u8     padding;          /* 0x07 */
} BattleGridSlot;

/**
 * Stat stage modifiers for a Pokemon in battle.
 * Range: -6 to +6 (stored as signed bytes).
 */
typedef struct StatStages {
    s8 attack;
    s8 defense;
    s8 spAttack;
    s8 spDefense;
    s8 speed;
    s8 accuracy;
    s8 evasion;
    s8 padding;
} StatStages;

/**
 * Per-Pokemon battle state.
 * This is the runtime representation of a Pokemon during battle.
 * Size: 0xE0 (224) bytes based on mulli indexing in disassembly.
 */
typedef struct BattlePokemon {
    /* 0x00 */ u16  species;
    /* 0x02 */ u16  currentHP;
    /* 0x04 */ u16  maxHP;
    /* 0x06 */ u16  attack;
    /* 0x08 */ u16  defense;
    /* 0x0A */ u16  spAttack;
    /* 0x0C */ u16  spDefense;
    /* 0x0E */ u16  speed;
    /* 0x10 */ u8   level;
    /* 0x11 */ u8   type1;
    /* 0x12 */ u8   type2;
    /* 0x13 */ u8   ability;
    /* 0x14 */ u32  statusCondition;
    /* 0x18 */ u32  volatileStatus;
    /* 0x1C */ StatStages statStages;
    /* 0x24 */ u16  moves[4];
    /* 0x2C */ u8   movePP[4];
    /* 0x30 */ u8   moveMaxPP[4];
    /* 0x34 */ u16  heldItem;
    /* 0x36 */ u8   gender;
    /* 0x37 */ u8   nature;
    /* 0x38 */ u32  experience;
    /* 0x3C */ u32  personalityValue;
    /* 0x40 */ u16  trainerID;
    /* 0x42 */ u16  secretID;
    /* 0x44 */ u8   friendship;
    /* 0x45 */ u8   isShadow;
    /* 0x46 */ u8   shadowMode;          /* 0 = normal, 1 = Hyper Mode */
    /* 0x47 */ u8   padding_47;
    /* 0x48 */ u16  shadowGauge;         /* Heart gauge for purification */
    /* 0x4A */ u16  shadowGaugeMax;
    /* 0x4C */ u8   evHP, evAtk, evDef, evSpAtk, evSpDef, evSpd;
    /* 0x52 */ u8   ivHP, ivAtk, ivDef, ivSpAtk, ivSpDef, ivSpd;
    /* 0x58 */ u8   padding_58[0x88];    /* remaining fields TBD */
} BattlePokemon;

/**
 * Turn action selected by a player or AI for one Pokemon.
 */
typedef struct TurnAction {
    u8   actionType;        /* 0 = fight, 1 = switch, 2 = item, 3 = run, 4 = call (shadow) */
    u8   moveIndex;         /* Index into BattlePokemon.moves[] */
    u8   targetSlot;        /* Target battle position */
    u8   priority;          /* Move priority level */
    u16  moveID;            /* Move ID for this action */
    u16  speedValue;        /* Effective speed for turn ordering */
} TurnAction;

/**
 * Move data entry from the move table.
 * Based on Gen III move data structure.
 */
typedef struct MoveData {
    u8   effect;
    u8   basePower;
    u8   type;
    u8   accuracy;
    u8   pp;
    u8   effectChance;
    u8   target;
    s8   priority;
    u8   flags;           /* contact, sound-based, etc. */
    u8   category;        /* physical / special / status */
    u8   padding[2];
} MoveData;

/**
 * Type effectiveness table entry.
 * In Gen III, type matchups are stored as a flat table of
 * (attacking_type, defending_type, effectiveness) triples,
 * terminated by 0xFF.
 */
typedef struct TypeMatchup {
    u8 attackType;
    u8 defendType;
    u8 effectiveness;     /* TYPE_EFF_* constant */
} TypeMatchup;

/**
 * Battle fight flow state, stored at sda21 offsets.
 * Small state variables controlling the overall fight lifecycle.
 *   lbl_8047B5D0 (u32) : Thread handle for the battle main loop
 *   lbl_8047B5D4 (u8)  : Saved scene ID before battle
 *   lbl_8047B5D5 (u8)  : Saved VSync mode before battle
 *   lbl_8047B5D6 (u16) : Battle result code (win/lose/draw)
 *   lbl_8047B5D8 (u16) : Battle status flags
 *   lbl_8047B5DA (u8)  : Fight-in-progress flag
 */

/* =========================================================================
 * Battle Grid (0x801C3108 - 0x801C53BC)
 * ========================================================================= */

/* fn_801C3108 */ void battleGrid_GetState(void);
/* fn_801C3114 */ void battleGrid_Init(void);
/* fn_801C31EC */ void battleGrid_Setup(void);
/* fn_801C3430 */ void battleGridSetup(void);                 /* 0x634 bytes */
/* fn_801C3A64 */ void battleGridLoadModels(void);            /* 0x11C bytes */
/* fn_801C3B80 */ void battleGridUpdatePositions(void);       /* 0x118 bytes */
/* fn_801C3D64 */ void battleGridReplacePokemon(void* model); /* 0xD8 bytes */
/* fn_801C3F10 */ void battleGridReplaceTrainer(void* model); /* 0xAC bytes */

/* =========================================================================
 * Battle Logic / Scene (0x801C53BC - 0x801D7230)
 * ========================================================================= */

/* Camera control */
/* fn_801C5898 */ void battleCamera_Update(void* camObj, void* target,
                                           f32 speed, f32 fov, f32 near, f32 far);
/* fn_801C6008 */ void battleCamera_SetView(u8 transition, void* target,
                                            f32 speed, f32 zoom);
/* fn_801C63C0 */ void battleCamera_Interpolate(void* target, void* params,
                                                f32 t, f32 speed, f32 zoom, f32 blend);
/* fn_801C6688 */ void battleCamera_SetRotation(f32 angle);
/* fn_801C6934 */ void battleScene_Init(void* stageModel, f32 scale);
/* fn_801C6AE8 */ void battleScene_SetupSlot(s32 row, s32 col, u8 type);
/* fn_801C71B0 */ void battleScene_UpdateAnimations(void);

/* Pokemon model & animation in battle scene */
/* fn_801C7730 */ s32  battleScene_PlacePokemon(s32 side, s32 slot);     /* 0xDCC bytes */
/* fn_801C89F8 */ void battleScene_EncounterSequence(s32 side, s32 slot); /* state machine */
/* fn_801C8E14 */ s32  battleScene_AnimatePokemon(s32 pokemonID, s32 animIdx,
                                                   s16 moveID, u8 animType); /* 0xAFC bytes */

/* =========================================================================
 * Battle Core / Fight Flow (0x801EF02C - 0x801F000C)
 * ========================================================================= */

/* fn_801EF02C */ void battle_InitSlots(void);
/* fn_801EF080 */ void battle_ResetSlotAnimations(void);
/* fn_801EF0D4 */ void battle_ResetSlotEffects(void);
/* fn_801EF128 */ void battle_SetupParty(void* partyData);
/* fn_801EF374 */ void battle_FightEnd(void);   /* "---------- fight end !! ----------" */
/* fn_801EF488 */ void battle_FightCleanup(void);
/* fn_801EF4B0 */ void battle_FightStart(void); /* "---------- fight start !! ----------" */
/* fn_801EF5C0 */ void battle_FightReset(void);
/* fn_801EF61C */ void battle_SetResult(u16 result);
/* fn_801EF624 */ u16  battle_GetResult(void);
/* fn_801EF62C */ void battle_SetStatusFlags(u16 flags);
/* fn_801EF634 */ u16  battle_GetStatusFlags(void);
/* fn_801EF63C */ u8   battle_IsFightInProgress(void);
/* fn_801EFA08 */ void battle_MainLoop(void);   /* 0x5BC bytes - the main battle loop */

/* =========================================================================
 * Battle State Machine (0x801E03D4 - 0x801EF02C)
 * ========================================================================= */

/* fn_801E03D4 */ s32  battleStateMachine_Main(void);  /* 13-state switch via jumptable_803751B8 */
/* fn_801E6684 */ void battleStateMachine_Animate(void);   /* ~6.8KB, PS-heavy animation */
/* fn_801E810C */ void battleStateMachine_MoveExec(void);  /* ~6.8KB, move execution */
/* fn_801E9B98 */ void battleStateMachine_Resolve(void);   /* ~6.8KB, damage resolution */

/* =========================================================================
 * Damage Calculation (decompiled from state machine internals)
 * ========================================================================= */

/**
 * Gen III damage formula:
 *   damage = ((2 * level / 5 + 2) * power * atk / def) / 50 + 2
 *   damage *= STAB modifier (1.5x if move type matches pokemon type)
 *   damage *= type effectiveness
 *   damage *= random factor (85-100) / 100
 *   damage *= critical hit (2x)
 *   damage *= other modifiers (abilities, items, weather, etc.)
 */
s32  battle_CalcDamage(BattlePokemon* attacker, BattlePokemon* defender,
                       const MoveData* move, u8 isCritical);
u8   battle_GetTypeEffectiveness(u8 attackType, u8 defType1, u8 defType2);
BOOL battle_IsSTAB(BattlePokemon* attacker, u8 moveType);
s32  battle_ApplyStatStage(s32 baseStat, s8 stage);
u8   battle_CalcCriticalHit(BattlePokemon* attacker, const MoveData* move);
s32  battle_GetRandomDamageFactor(void);

/* =========================================================================
 * Type Effectiveness
 * ========================================================================= */

u8   battle_CalcTypeMatchup(u8 atkType, u8 defType);

/* =========================================================================
 * Turn Order
 * ========================================================================= */

void battle_DetermineTurnOrder(TurnAction actions[], s32 count);
s32  battle_ComparePriority(const TurnAction* a, const TurnAction* b);

/* =========================================================================
 * Status Effects
 * ========================================================================= */

void battle_ApplyStatusDamage(BattlePokemon* pokemon);
BOOL battle_CheckStatusPreventsMove(BattlePokemon* pokemon);
void battle_TickStatusCounters(BattlePokemon* pokemon);
void battle_TryInflictStatus(BattlePokemon* target, u32 status, u8 chance);

/* =========================================================================
 * Move Execution
 * ========================================================================= */

s32  battle_ExecuteMove(s32 attackerSlot, s32 targetSlot, u16 moveID);
BOOL battle_CheckAccuracy(BattlePokemon* attacker, BattlePokemon* defender,
                          const MoveData* move);
void battle_ApplyMoveEffect(BattlePokemon* target, const MoveData* move);

/* =========================================================================
 * Shadow Pokemon
 * ========================================================================= */

BOOL battle_IsShadowPokemon(BattlePokemon* pokemon);
void battle_EnterHyperMode(BattlePokemon* pokemon);
void battle_ExitHyperMode(BattlePokemon* pokemon);
s32  battle_CalcShadowRushDamage(BattlePokemon* attacker, BattlePokemon* defender);
BOOL battle_CanSnag(BattlePokemon* target);
void battle_ProcessSnagging(s32 targetSlot);
void battle_CallPokemon(BattlePokemon* pokemon);

/* =========================================================================
 * AI
 * ========================================================================= */

void battle_AIChooseAction(s32 trainerSlot, TurnAction* outAction);
s32  battle_AIEvaluateMove(s32 aiSlot, s32 targetSlot, u16 moveID);

#endif /* GAME_BATTLE_H */
