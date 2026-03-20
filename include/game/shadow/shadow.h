#ifndef GAME_SHADOW_H
#define GAME_SHADOW_H

#include "dolphin/types.h"

/**
 * @file shadow.h
 * @brief Shadow Pokemon System -- Overworld management.
 *
 * This header covers the *overworld* (non-battle) Shadow Pokemon system:
 *   - Heart gauge tracking and purification progress
 *   - Purification methods: walking, cologne, calling, time decay, Relic Stone
 *   - Shadow Pokemon registration (which trainers have which shadow Pokemon)
 *   - Snagging state tracking (which shadow Pokemon have been caught)
 *   - Shadow Pokemon data tables (species, trainer, heart gauge initial values)
 *
 * For the *battle* Shadow Pokemon mechanics (Hyper Mode, Shadow Rush, snagging
 * during battle), see game/battle/battle.h and battle_shadow.c.
 *
 * The Shadow Pokemon data is loaded from common_rel.fdat (index 80/81 in
 * the CommonIndexes table). Each shadow Pokemon entry defines:
 *   - The Pokemon species
 *   - The trainer who owns it
 *   - The initial heart gauge value
 *   - The shadow move set
 *   - Catch difficulty modifiers
 *
 * Runtime state is tracked in the save file per-Pokemon:
 *   SavePokemon.shadowPokemonID (0xD8) -- Shadow ID (0 = not shadow)
 *   SavePokemon.purificationCounter (0xDC) -- Heart gauge value (signed)
 *   SavePokemon.expStored (0xE0) -- EXP accumulated while shadow
 *
 * The heart gauge decreases over time. When it reaches 0, the Pokemon
 * is eligible for purification at the Relic Stone in Agate Village.
 *
 * Colosseum has 48 Shadow Pokemon in total (shadow IDs 1-48).
 */

/* =========================================================================
 * Constants
 * ========================================================================= */

/** Total number of Shadow Pokemon in Colosseum. */
#define SHADOW_POKEMON_COUNT    48

/** Shadow Pokemon ID range. */
#define SHADOW_ID_NONE          0
#define SHADOW_ID_MIN           1
#define SHADOW_ID_MAX           48

/** Shadow Pokemon states (overworld). */
#define SHADOW_STATE_UNENCOUNTERED  0   /* Not yet seen */
#define SHADOW_STATE_ENCOUNTERED    1   /* Seen but not caught */
#define SHADOW_STATE_SNAGGED        2   /* Caught by player */
#define SHADOW_STATE_PURIFIED       3   /* Fully purified */

/** Heart gauge constants. */
#define HEART_GAUGE_MAX_DEFAULT  10000  /* Default maximum heart gauge */
#define HEART_GAUGE_PURIFIED     0      /* Gauge value when purifiable */

/** Purification method IDs. */
#define PURIFY_METHOD_WALK       0      /* Walking with the Pokemon */
#define PURIFY_METHOD_BATTLE     1      /* Participating in battles */
#define PURIFY_METHOD_CALL       2      /* Calling during battle */
#define PURIFY_METHOD_COLOGNE    3      /* Using cologne items */
#define PURIFY_METHOD_TIME       4      /* Passive time decay */
#define PURIFY_METHOD_RELIC      5      /* Relic Stone (instant purify when gauge=0) */
#define PURIFY_METHOD_COUNT      6

/** Cologne item IDs. */
#define ITEM_JOY_SCENT          0xDC    /* Joy Scent cologne */
#define ITEM_EXCITE_SCENT       0xDD    /* Excite Scent cologne */
#define ITEM_VIVID_SCENT        0xDE    /* Vivid Scent cologne */

/** Heart gauge reduction amounts per purification method. */
#define PURIFY_WALK_AMOUNT       1      /* Per step */
#define PURIFY_BATTLE_AMOUNT     100    /* Per battle participated */
#define PURIFY_CALL_AMOUNT       100    /* Per successful call */
#define PURIFY_JOY_SCENT_AMT    300    /* Joy Scent item */
#define PURIFY_EXCITE_SCENT_AMT 600    /* Excite Scent item */
#define PURIFY_VIVID_SCENT_AMT  900    /* Vivid Scent item */

/** Time flow constants for passive decay (in game frames). */
#define PURIFY_TIME_INTERVAL    1800    /* Frames between passive decrements */
#define PURIFY_TIME_AMOUNT      1       /* Amount decreased per interval */

/**
 * Nature-based purification rate multipliers.
 * Each nature has a set of purification rate modifiers stored in the
 * Nature data structure (offsets 0x00-0x04 in the 0x28-byte entry):
 *   +0x00: Battle purification rate
 *   +0x01: Walk purification rate
 *   +0x02: Call purification rate
 *   +0x03: Day Care purification rate (unused in Colosseum)
 *   +0x04: Cologne purification rate
 *
 * These are multiplied with the base purification amount. A value of
 * 100 means 1x (normal), higher values mean faster purification.
 */

/* =========================================================================
 * Structures
 * ========================================================================= */

/**
 * Shadow Pokemon data table entry.
 * Loaded from common_rel.fdat (CommonIndexes 80/81).
 *
 * Each entry defines one of the 48 Shadow Pokemon in the game.
 * The data table is indexed by Shadow ID (1-based; 0 = not shadow).
 */
typedef struct ShadowPokemonData {
    /* 0x00 */ u16  species;            /* Pokemon species index */
    /* 0x02 */ u16  shadowMoveID;       /* Shadow Rush variant / shadow move */
    /* 0x04 */ u16  trainerIndex;       /* Trainer table index (who owns this shadow) */
    /* 0x06 */ u16  padding_06;
    /* 0x08 */ s32  heartGaugeInitial;  /* Starting heart gauge value */
    /* 0x0C */ u16  catchRate;          /* Modified catch rate for snagging */
    /* 0x0E */ u8   level;              /* Level when encountered */
    /* 0x0F */ u8   floorID;            /* Floor/area where encountered */
    /* 0x10 */ u16  purifyBonusFlag;    /* GSflag ID set upon purification */
    /* 0x12 */ u16  encounterFlag;      /* GSflag ID for encounter tracking */
    /* 0x14 */ u16  snagFlag;           /* GSflag ID set when snagged */
    /* 0x16 */ u16  padding_16;
} ShadowPokemonData;

/**
 * Shadow Pokemon registration entry.
 * Tracks the runtime state of each shadow Pokemon across the game.
 * One entry per shadow Pokemon ID (48 total).
 *
 * This is stored in the save data alongside the flag system.
 */
typedef struct ShadowRegistration {
    /* 0x00 */ u8   state;              /* SHADOW_STATE_* */
    /* 0x01 */ u8   trainerDefeated;    /* Has the owning trainer been defeated? */
    /* 0x02 */ u8   encounterCount;     /* Times encountered without snagging */
    /* 0x03 */ u8   padding_03;
    /* 0x04 */ s32  currentGauge;       /* Current heart gauge value */
    /* 0x08 */ s32  maxGauge;           /* Maximum heart gauge value */
    /* 0x0C */ u16  speciesIndex;       /* Pokemon species for this shadow */
    /* 0x0E */ u16  padding_0E;
} ShadowRegistration;

/**
 * Purification rate entry (per nature).
 * These are the first 5 bytes of the Nature data structure (0x28 bytes).
 */
typedef struct PurifyRates {
    /* 0x00 */ u8  battleRate;
    /* 0x01 */ u8  walkRate;
    /* 0x02 */ u8  callRate;
    /* 0x03 */ u8  dayCareRate;  /* Unused in Colosseum */
    /* 0x04 */ u8  cologneRate;
} PurifyRates;

/**
 * Global Shadow Pokemon system state.
 * Manages the registry of all shadow Pokemon and purification tracking.
 */
typedef struct ShadowSystem {
    ShadowRegistration  registry[SHADOW_POKEMON_COUNT]; /* Per-shadow tracking */
    ShadowPokemonData*  dataTable;      /* Pointer to loaded shadow data */
    u32                 dataCount;      /* Number of entries in data table */
    u32                 timeCounter;    /* Frame counter for passive decay */
    u32                 walkCounter;    /* Step counter for walk purification */
    BOOL                initialized;    /* TRUE after shadow_Init() */
} ShadowSystem;

/* =========================================================================
 * Function declarations -- Initialization
 * ========================================================================= */

/**
 * Initialize the Shadow Pokemon system.
 * Loads the shadow data table from common_rel.fdat and sets up
 * the registration array. Called during game initialization.
 *
 * @param dataPtr   Pointer to the shadow data table in common_rel.
 * @param count     Number of shadow Pokemon entries.
 */
void shadow_Init(ShadowPokemonData* dataPtr, u32 count);

/**
 * Reset the shadow registration state.
 * Called when starting a new game.
 */
void shadow_Reset(void);

/* =========================================================================
 * Function declarations -- Heart Gauge / Purification
 * ========================================================================= */

/**
 * Get the current heart gauge value for a shadow Pokemon.
 *
 * @param shadowID  Shadow Pokemon ID (1-48).
 * @return          Current heart gauge value, or -1 if invalid.
 */
s32 shadow_GetHeartGauge(u16 shadowID);

/**
 * Set the heart gauge value for a shadow Pokemon.
 *
 * @param shadowID  Shadow Pokemon ID (1-48).
 * @param value     New heart gauge value (clamped to 0..max).
 */
void shadow_SetHeartGauge(u16 shadowID, s32 value);

/**
 * Reduce the heart gauge by the specified amount.
 * Applies the nature-based purification rate multiplier.
 *
 * @param shadowID  Shadow Pokemon ID.
 * @param amount    Base reduction amount.
 * @param method    Purification method (PURIFY_METHOD_*).
 * @param nature    Pokemon's nature index (0-24) for rate lookup.
 */
void shadow_ReduceHeartGauge(u16 shadowID, s32 amount, u8 method, u8 nature);

/**
 * Check if a shadow Pokemon's heart gauge has reached zero
 * (eligible for purification at the Relic Stone).
 *
 * @param shadowID  Shadow Pokemon ID.
 * @return          TRUE if gauge is 0 (purifiable).
 */
BOOL shadow_IsPurifiable(u16 shadowID);

/**
 * Purify a shadow Pokemon.
 * Called when the player uses the Relic Stone on a Pokemon whose
 * heart gauge is 0. This:
 *   - Sets the shadow state to PURIFIED
 *   - Converts stored EXP to real EXP
 *   - Replaces Shadow Rush with a normal move
 *   - Sets the purification GSflag
 *   - Triggers the purification cutscene
 *
 * @param shadowID  Shadow Pokemon ID.
 * @param pokemon   Pointer to the save Pokemon data.
 */
void shadow_Purify(u16 shadowID, void* pokemon);

/**
 * Apply walking purification.
 * Called each step while a shadow Pokemon is in the active party.
 * Reduces the heart gauge by PURIFY_WALK_AMOUNT, scaled by nature.
 *
 * @param pokemon   Pointer to the save Pokemon data.
 */
void shadow_WalkPurify(void* pokemon);

/**
 * Apply cologne item purification.
 * Called when the player uses a cologne item on a shadow Pokemon.
 *
 * @param pokemon   Pointer to the save Pokemon data.
 * @param itemID    The cologne item ID used.
 */
void shadow_ColognePurify(void* pokemon, u16 itemID);

/**
 * Apply time-based passive purification.
 * Called per frame; internally tracks a timer and only applies
 * reduction at PURIFY_TIME_INTERVAL intervals.
 */
void shadow_TimePurify(void);

/**
 * Apply battle purification.
 * Called at the end of a battle for each shadow Pokemon that
 * participated.
 *
 * @param pokemon   Pointer to the save Pokemon data.
 */
void shadow_BattlePurify(void* pokemon);

/* =========================================================================
 * Function declarations -- Registration / Snagging
 * ========================================================================= */

/**
 * Register a shadow Pokemon encounter.
 * Called when the player battles a trainer who has a shadow Pokemon.
 * Sets the shadow's state to ENCOUNTERED if not already snagged.
 *
 * @param shadowID  Shadow Pokemon ID.
 */
void shadow_RegisterEncounter(u16 shadowID);

/**
 * Register that a shadow Pokemon has been snagged.
 * Called after a successful snag during battle.
 * Sets the shadow's state to SNAGGED and fires the snag GSflag.
 *
 * @param shadowID  Shadow Pokemon ID.
 */
void shadow_RegisterSnag(u16 shadowID);

/**
 * Check if a shadow Pokemon has been snagged.
 *
 * @param shadowID  Shadow Pokemon ID.
 * @return          TRUE if the shadow has been caught.
 */
BOOL shadow_IsSnagged(u16 shadowID);

/**
 * Check if a shadow Pokemon has been purified.
 *
 * @param shadowID  Shadow Pokemon ID.
 * @return          TRUE if purified.
 */
BOOL shadow_IsPurified(u16 shadowID);

/**
 * Get the shadow state for a given shadow ID.
 *
 * @param shadowID  Shadow Pokemon ID.
 * @return          SHADOW_STATE_* value.
 */
u8 shadow_GetState(u16 shadowID);

/**
 * Get the shadow Pokemon data entry for a given shadow ID.
 *
 * @param shadowID  Shadow Pokemon ID (1-based).
 * @return          Pointer to the data entry, or NULL if invalid.
 */
ShadowPokemonData* shadow_GetData(u16 shadowID);

/**
 * Get the total number of shadow Pokemon snagged so far.
 *
 * @return  Count of snagged shadow Pokemon (0-48).
 */
u32 shadow_GetSnagCount(void);

/**
 * Get the total number of shadow Pokemon purified so far.
 *
 * @return  Count of purified shadow Pokemon (0-48).
 */
u32 shadow_GetPurifyCount(void);

/**
 * Check if all 48 shadow Pokemon have been snagged.
 *
 * @return  TRUE if all are snagged (or purified).
 */
BOOL shadow_AllSnagged(void);

/**
 * Check if all 48 shadow Pokemon have been purified.
 * This is the condition for the Mt. Battle Ho-Oh reward.
 *
 * @return  TRUE if all are purified.
 */
BOOL shadow_AllPurified(void);

/* =========================================================================
 * Function declarations -- Save/Load
 * ========================================================================= */

/**
 * Save shadow registration state to save data.
 *
 * @param saveBuffer  Pointer to the save data region for shadow state.
 */
void shadow_SaveState(void* saveBuffer);

/**
 * Load shadow registration state from save data.
 *
 * @param saveBuffer  Pointer to the save data region for shadow state.
 */
void shadow_LoadState(void* saveBuffer);

#endif /* GAME_SHADOW_H */
