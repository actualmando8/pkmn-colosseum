/**
 * @file people_field.h
 * @brief People/NPC field-level system -- data management, movement,
 *        animation, rendering, and script integration.
 *
 * This header covers the "field people" subsystem which handles NPCs
 * during overworld gameplay. It is distinct from the higher-level
 * people.h system (0x80180C78+) which manages the floor-level spawn
 * lifecycle.
 *
 * The field people system operates on two main data structures:
 *
 *   PeopleFieldEntry (0x28 bytes) -- compact NPC data for field rendering.
 *     Stored in gPeopleFieldArray (lbl_80363CE8), indexed via
 *     gPeopleFieldLookup (lbl_803681E8).
 *
 *   PeopleFieldWork (0x404 bytes) -- full NPC state for field behavior.
 *     Stored in gPeopleFieldWorkArray (lbl_8047AF48), allocated by
 *     peopleFieldSystemInit.
 *
 * Address range: 0x80140588 - 0x801652DC (approximately 0x24D54 bytes)
 */
#ifndef GAME_PEOPLE_PEOPLE_FIELD_H
#define GAME_PEOPLE_PEOPLE_FIELD_H

#include "dolphin/types.h"

/* =========================================================================
 * PeopleFieldEntry -- compact NPC data (0x28 bytes per entry)
 * ========================================================================= */

typedef struct PeopleFieldEntry {
    /* 0x00 */ f32    field_00;
    /* 0x04 */ u8     flags_04;
    /* 0x05 */ s8     field_05;
    /* 0x06 */ s8     field_06;
    /* 0x07 */ s8     field_07;
    /* 0x08 */ u32    field_08;
    /* 0x0C */ u8     field_0C;
    /* 0x0D */ u8     field_0D;
    /* 0x0E */ u8     field_0E;
    /* 0x0F */ u8     field_0F;
    /* 0x10 */ f32    posX;
    /* 0x14 */ f32    posY;
    /* 0x18 */ f32    posZ;
    /* 0x1C */ f32    rotAngle;
    /* 0x20 */ f32    scale;
    /* 0x24 */ u32    modelRef;
} PeopleFieldEntry;

/* =========================================================================
 * PeopleFieldWork -- full NPC state (0x404 bytes per slot)
 *
 * Key fields recovered from fn_8014D000 (init) and fn_801557EC (reset).
 * ========================================================================= */

typedef struct PeopleFieldWork {
    /* 0x000 */ u8     coreState[0xF4];
    /* 0x0F4 */ s32    entityID;        /* -1 = unassigned */
    /* 0x0F8 */ u8     pad_0F8[0x0C];
    /* 0x104 */ u8     resetFlag;
    /* 0x105 */ u8     pad_105[0x07];
    /* 0x10C */ u32    field_10C;
    /* 0x110 */ u32    field_110;
    /* 0x114 */ u32    flagsLo;         /* 64-bit flags, low word */
    /* 0x118 */ u32    flagsHi;         /* 64-bit flags, high word */
    /* 0x11C */ u8     pad_11C[0x04];
    /* 0x120 */ u8     defaultAnimBank;
    /* 0x121 */ u8     animBankA;
    /* 0x122 */ u8     animBankB;
    /* 0x123 */ u8     animBankC;
    /* 0x124 */ u8     pad_124[0x0B];
    /* 0x12F */ u8     prevAnimBank;
    /* 0x130 */ u8     currentAnimBank;
    /* 0x131 */ u8     pad_131[0x1F];
    /* 0x150 */ u16    motionStateA;
    /* 0x152 */ u8     pad_152[0x02];
    /* 0x154 */ u32    motionConfigA;
    /* 0x158 */ u32    motionConfigB;
    /* 0x15C */ u8     pad_15C[0x10];
    /* 0x16C */ u16    motionStateB;
    /* 0x16E */ u8     pad_16E[0x02];
    /* 0x170 */ u32    blendTargetA;
    /* 0x174 */ u32    blendTargetB;
    /* 0x178 */ u8     pad_178[0x08];
    /* 0x180 */ u32    blendSourceA;
    /* 0x184 */ u32    blendSourceB;
    /* 0x188 */ u8     pad_188[0x08];
    /* 0x190 */ u8     motionTypeA;
    /* 0x191 */ u8     motionTypeB;
    /* 0x192 */ u8     motionBlendFlag;
    /* 0x193 */ u8     motionSpeed;
    /* 0x194 */ u8     pad_194[0x0C];
    /* 0x1A0 */ f32    interpFactorA;
    /* 0x1A4 */ u32    interpPointCount;
    /* 0x1A8 */ u8     pad_1A8[0x10];
    /* 0x1B8 */ u32    field_1B8;
    /* 0x1BC */ u8     pad_1BC[0x4C];
    /* 0x208 */ u8     initAnimBankA;
    /* 0x209 */ u8     initAnimSlot;
    /* 0x20A */ u8     initAnimBankB;
    /* 0x20B */ u8     initAnimBankC;
    /* 0x20C */ u8     initAnimBankD;
    /* 0x20D */ u8     initDefaultAnim;
    /* 0x20E */ u8     initMotionType;
    /* 0x20F */ u8     pad_20F;
    /* 0x210 */ u8     initMotionSpeed;
    /* 0x211 */ u8     pad_211[0x1F3];
} PeopleFieldWork;

/* =========================================================================
 * Data management API (people_data.c: 0x80140588 - 0x80144574)
 * ========================================================================= */

/** Look up a PeopleFieldEntry by u16 index (fn_801440A0).
 *  The most-called function -- 48 external callers. */
PeopleFieldEntry* peopleFieldGetByIndex(u16 index);

/** Look up an NPC by group+index ID (fn_80142984).
 *  Called by 18 external functions. */
PeopleFieldEntry* peopleFieldGetByID(u32 groupId, u32 index);

/** Get extended NPC entry with validation (fn_801429E8).
 *  Called by 28 external functions. */
void* peopleFieldGetEntry(u32 param);

/** Allocate or find an NPC slot (fn_80142CF4).
 *  Called by 38 external functions. */
void* peopleFieldAlloc(u32 a, u32 b, u32 c, u32 d);

/** Open/spawn an NPC from field data (fn_80140588, 0x514 bytes). */
s32 peopleFieldOpen(void* entry, u32 group, u32 index, void* spawnData, u32 force);

/** Get a raw NPC slot pointer (fn_80140A9C). */
void* peopleFieldGetSlot(u32 index);

/** Load NPC model and configure (fn_80140ACC, 0x83C bytes). */
void peopleFieldLoadModel(void* entry);

/** Per-frame update for a single NPC (fn_80141308, 0x1060 bytes). */
void peopleFieldUpdate(void* entry);

/** Cleanup/release an NPC slot (fn_80142368). */
void peopleFieldCleanup(void* entry);

/** Configure NPC properties after model load (fn_801425E8). */
void peopleFieldSetup(void* entry);

/** Set NPC state (fn_80142A88). */
void peopleFieldSetState(void* entry, u32 state);

/** Apply motion/animation to NPC (fn_80142B24). */
void peopleFieldApplyMotion(void* entry, void* motion);

/** Release NPC slot (fn_80142EF8). */
void peopleFieldRelease(void* entry);

/** Initialize the field people data system (fn_801431AC, 0x4F0 bytes). */
void peopleFieldDataInit(void);

/* =========================================================================
 * Field behavior API (people_field.c: 0x80144574 - 0x801652DC)
 * ========================================================================= */

/** Initialize the field people subsystem (fn_8014D000, 0x574 bytes).
 *  Allocates NPC work array (0x404 bytes per slot). */
void peopleFieldSystemInit(void* param, u32 count);

/** Main NPC spawn function (fn_80144574, 0x1DE8 bytes). */
void peopleFieldSpawnMain(void* entry);

/** Script command handler for NPCs (fn_8014C984, 0x530 bytes).
 *  Processes bytecodes: 0xFA=walk, 0xFB=stop, 0xFC=run,
 *  0xFD=face, 0xFE=wait, 0xFF=anim. */
void peopleFieldScriptCommand(void* work, u16 index, u8 cmd, u8 param, u8 extra);

/** Reset NPC to idle state (fn_801557EC, 0xF58 bytes). */
void peopleFieldResetState(void* work);

/** Main script processing loop (fn_8015B250, 0x21B8 bytes). */
void peopleFieldScriptMain(void* work);

/** Motion system update (fn_801603C0, 0x608 bytes). */
void peopleFieldMotionUpdate(void* work);

/** Motion system main processing (fn_80161134, 0x4A0 bytes). */
void peopleFieldMotionMain(void* work);

/** Utility initialization (fn_80161A9C, 0x284 bytes). */
void peopleFieldUtilInit(void* work);

/** Animation interpolation (fn_80164DD0, 0x50C bytes). */
void peopleFieldAnimInterp(void* work, void* entry, u32 param);

/** NPC movement with interpolation (fn_80162A58, 0x2C0 bytes). */
void peopleFieldMoveApply(u32 index, u32 param, f32 x, f32 y, f32 z, u32 extra);

#endif /* GAME_PEOPLE_PEOPLE_FIELD_H */
