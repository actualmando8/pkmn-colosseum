/**
 * @file pokemon_fight_types.h
 * @brief Shared externs/typedefs for the fight-engine cluster split out of
 * the former game/pokemon.c bucket (0x801F000C-0x801F7F80).
 *
 * That bucket was mislabeled "pokemon" but is entirely the XD-era
 * fight-engine cluster: fightTarget/fightAction/fightFloor/fightFloorBios/
 * fightSide/fightSideBios/fightTrainer(head). This header centralizes the
 * file-scope externs and forward declarations that were previously
 * duplicated at the top of pokemon.c, so every split translation unit can
 * include one header instead of re-declaring cross-TU prototypes.
 */

#ifndef GAME_POKEMON_FIGHT_TYPES_H
#define GAME_POKEMON_FIGHT_TYPES_H

#include "game/pokemon.h"

/* =========================================================================
 * External function declarations
 * ========================================================================= */

/* dbgMenuFightGetMsgSpeedToFrame: GSthread_GetCurrentContext or similar - called from fightMainWaitFrame */
extern u32 dbgMenuFightGetMsgSpeedToFrame(void);

/* _threadSwitch: VSync/frame wait - called in frame loop */
extern void _threadSwitch(void);

/* fn_800D3088: Returns frame delta or similar timing value */
extern u32 fn_800D3088(void);

/* fightFloorDataBiosGetPtr: Resolve party slot to Pokemon pointer by index */
extern struct Pokemon* fightFloorDataBiosGetPtr(u32 index);

/* fightFloorBiosGetFightFloorPtr: Get currently selected/active Pokemon */
extern struct Pokemon* fightFloorBiosGetFightFloorPtr(void);

/* fightFloorGetNowPtr: Get Pokemon system context pointer */
extern struct Pokemon* fightFloorGetNowPtr(void);

/* fightOutPokemonGetFightActionPri: Pokemon nature/friendship comparison helper */
extern s8 fightOutPokemonGetFightActionPri(struct Pokemon* pokemon);

/* Forward declarations for fightFloorGetStatus asm wrapper */
extern u32 jumptable_803754AC[];
extern void fn_80119ED0(void);
extern void fn_8011B444(void);
extern void fn_8011B67C(void);
extern void pokemonGetStatus(void);
extern void fn_801EF634(void);
extern void fightTrainerGetValidFightOutPokemonPtr(void);
extern void fightEncountGetEnvSndDataId(void);
extern void fightEncountGetBgmSndDataId(void);
extern void fightEncountDataBiosGetFightFloorDataId(void);
extern void fightEncountDataBiosGetTrainer(void);
extern void fightEncountDataBiosGetFightKind(void);
extern void fightEncountDataBiosGetPtr(void);
extern void fightTypeGetFightSideFightOutPokemonMax(void);
extern void fightTypeDataBiosGetFightoutPokemonNum(void);
extern void fightTypeDataBiosGetEntryPokemonNum(void);
extern void fightTypeDataBiosGetTrainerNum(void);
extern void fightTypeDataBiosGetName(void);
extern void fightTypeDataBiosGetPtr(void);

#pragma pack(2)
typedef struct CopyBuf {
    u32 a;
    u32 b;
    u32 c;
    u32 d;
    u32 e;
    u32 f;
    u16 g;
} CopyBuf;
#pragma pack()

extern const CopyBuf lbl_80279C28;
extern void fightKindDataBiosGetHostEnemyMsgFlag(void);
extern void fightKindDataBiosGetName(void);
extern void fightKindDataBiosGetPokemonStatusMenuSubbarFlag(void);
extern void fightKindDataBiosGetDarkpokemonHypermodeFlag(void);
extern void fightKindDataBiosGetMonohiroiFlag(void);
extern void fightKindDataBiosGetDorobouFlag(void);
extern void fightKindDataBiosGetBossFlag(void);
extern void fightKindDataBiosGetKeikentihueruFlag(void);
extern void fightKindDataBiosGetDoItemSoubiTokukoutokubouupFlag(void);
extern void fightKindDataBiosGetDoHizukiMiyaburiFlag(void);
extern void fightKindDataBiosGetDoHizukiAiFlag(void);
extern void fightKindDataBiosGetDoCriticalAttackFlag(void);
extern void fightKindDataBiosGetGetInfectPokerusFlag(void);
extern void fightKindDataBiosGetGetFriendFlag(void);
extern void fightKindDataBiosGetGetNekoniKobanFlag(void);
extern void fightKindDataBiosGetGetOkaneFlag(void);
extern void fightKindDataBiosGetOkanePoolFlag(void);
extern void fightKindDataBiosGetGetExpFlag(void);
extern void fightKindDataBiosGetDrawFlag(void);
extern void fightKindDataBiosGetNigeruFlag(void);
extern void fightKindDataBiosGetCallFlag(void);
extern void fightKindDataBiosGetUseItemFlag(void);
extern void fightKindDataBiosGetDoZukanTukamaetaFlag(void);
extern void fightKindDataBiosGetDoZukanMitaFlag(void);
extern void fightKindDataBiosGetDoBadgeCheckFlag(void);
extern void fightKindDataBiosGetPtr(void);
extern void fightKindDataBiosGetBackSaveDataFlag(void);

extern u32 lbl_8047B5F0;

/* Forward declarations for converted accessor functions */
u16 fightFloorDataBiosGetFloorDataId(u8* ptr);
u32 fightFloorDataBiosGetName(u8* ptr);
u32 fightFloorDataBiosGetFightSideDataId(u8* ptr, u8 idx);

/* Forward declarations for functions referenced by fightFloorGetStatus dispatcher */
u8* fightFloorBiosGetFightOutPokemonPtrAryPtr(u8* ptr);
u32 fightFloorBiosGetFightOutPokemonPtrAry(u8* ptr, u16 idx);
u8* fightFloorBiosGetFightSidePtr(u8* ptr, u16 idx);
u8* fightFloorBiosGetJoutaiPtr(u8* ptr, u16 idx);
u32 fightSideGetValidFightTrainerPtr(u32 param_1);
u32 fightSideCheckValid(u32 param_1);
void fightTargetGetPtrAsNowFightType(u32 slotType, u32 idx);
u32 fightTargetGetPtr(u32 slotType, u32 ptr, u32 count);
void fightFloorLoopValidFightTrainer(u32, void (*)(u32, u32, u32), u32, u8);
void fightFloorSortFightOutPokemonPtrArySub(void*, u32*, u16, u32);
s32 fightFloorCmpfightOutPokemonNimbleness(void*, void*, void*, u8);
u8 fightFloorSetStatus(u32, u16, u32, u16, u32);
void fightFloorDataBiosSetName(u8* ptr, u32 val);
void fightFloorDataBiosSetFloorDataId(u8* ptr, u16 val);
void fightSideRegistFightSideEnemyPokemonFightAll(u32, u32, u32, u32, u32);
s32 fightSideGetFightPokemonMaxHp(u32, u32, u32);
s32 fightSideGetFightPokemonNokoriHp(u32, u32, u32);
void fightSideInitAry(u32, u16);
void fightTrainerToMenuBallStatus(u8* ptr, u8* arr);
void fightTrainerGetTemotiNormalItemDataIdAry(u8* ptr, u16* out, u16 count, u8 mode);

#endif /* GAME_POKEMON_FIGHT_TYPES_H */
