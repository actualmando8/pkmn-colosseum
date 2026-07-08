/**
 * @file pokemon_data.c
 * @brief GSfield world segment -- split from gs_field_world.c.
 *
 * XD source unit: pokemon DataBios / pokemonBios accessor families
 * Address range: 0x8011CA60 - 0x8011F5FC (~289 functions)
 *
 * Split from src/game/gs_field_world.c (physical XD source-unit split of
 * the 734-function field-world bucket into its 12 constituent XD source
 * units). See gs_field_world.c split history for the address-range
 * evidence (anchor-name monotonicity checks) used to place this boundary.
 */
#include "dolphin/types.h"
#include "game/world/gs_field.h"
/* ===== External SDK / engine functions ===== */
extern void  GSlogWrite(const char* fmt, ...);         /* OSReport / GSlog */
extern void* memcpy(void* dst, const void* src, u32 n);
extern void* memset(void* dst, int val, u32 size);
/* GSmem */
extern u16   _toolentryAlloc__FUl(u32 size);                     /* GSmemAllocRaw */
extern void* fn_800E27B0(u16 handle);                   /* GSmemGetPtr */
extern void* fn_800E24B0(u16 handle);                   /* GSmemLock */
extern void  fn_800E209C(u16 handle);                   /* GSmemFree */
/* Matrix / vector */
extern void  PSMTXMultVec(void* mtx, void* vec, void* out); /* MTXMultVec3 */
extern void  PSVECSubtract(void* out, void* in, f32 s);     /* VEC normalize */
/* GSgfx renderer */
extern void  fn_800D7868(void* handle, u32 a, u32 b, u32 c,
                          u32 d, u32 e, u32 f, u32 g);     /* GSgfx draw setup */
extern void* fn_800D7894(void);                             /* GSgfx create render obj */
/* Real named labels referenced by remaining inline-asm wrappers in this TU. */
extern void _modelSetRotateEulerToQuatAll__FP9_HSD_JObj();
extern void cos();
extern void gamedataAttestCheckValid();
extern void gamedataAttestCreate();
extern void gamedataAttestInit();
extern void gamedataCreate();
extern void gamedataGetStatus();
extern void GScharCmp();
extern void GScharLenCpy();
extern void GSflagClear();
extern void GSmsgFontClose();
extern void itemGetStatus();
extern void LCStoreData();
extern void memoDataSet();
extern void menuSubGetPokemonSexForFightDisp();
extern void OSGetTick();
extern void psInitAppSRT();
extern void psInitParticle();
extern void psSetGeneratorAngleRadiusScale();
extern void psSetParticleVisibility();
extern void sin();
extern void statusGetStatus();
/* GSfloor / GScolsys */
extern void* fn_800FF56C(void);                             /* GSfloor get active */
extern void  GScolsys2GetObjEnable(u32 triIdx, void* outFlag);        /* GScolsys query */
/* ===== Index lookup globals ===== */
extern u8 lbl_8035BBA8[];  /* NPC table (BSS) */
extern u8 lbl_8035C430[];  /* field obj table (BSS) */
extern u32 lbl_80478B48;  /* NPC count (SDA) */
extern u32 lbl_80478B50;  /* field obj count (SDA) */
extern u32 lbl_80478F90;  /* obj header ptr (SDA) */
extern u32 lbl_80478F94;  /* obj data base (SDA) */
/* Field subsystems -- forward declarations (defined below) */
u32  _unloadFlare__FPvUlUl(void);
u32 floorDataBiosGetFileGroupID(u8* entry);
void fn_80117C84(void);
void pokemonResetBasisStatus(void* ptr);
/* ===== String constants (rodata) ===== */
extern const char lbl_80272770[]; /* "floorUpdateFieldCamera: error updating..." */
extern const char lbl_802724E8[]; /* "floorReadMapPreFunc: can't alloc..." */
extern const char lbl_80272520[]; /* "floorReadScriptPreFunc(): can't alloc..." */
extern const char lbl_8027255C[]; /* "floorReadFontPreFunc(): can't alloc..." */
extern const char lbl_80272594[]; /* "floorReadMsgPreFunc(): can't alloc..." */
extern const char lbl_802725CC[]; /* "floorReadNormalPreFunc(): can't alloc..." */
/* ===== Combined cross-segment declarations (file-scope forward
 * declarations duplicated across all gs_field_world.c split segments;
 * de-duplicated by identifier from the whole original TU -- includes
 * both the original file's own top-level externs/prototypes AND a
 * declaration synthesized from every function's own definition
 * signature, since in the original single-TU file a definition also
 * served as a forward declaration for any later same-file caller; now
 * that callers may live in a sibling segment file they need an
 * explicit declaration. Function-local (per-call-site) extern
 * declarations are NOT hoisted here -- they travel with their own
 * function body. ===== */
extern u8 lbl_8047AD71;
extern u32 lbl_8047ADC0;
extern u32 lbl_8047ADB8;
extern u8 lbl_804083D0[0x30];
extern u32 lbl_8047AD68;
extern u32 lbl_8047AD6C;
extern void fn_801ED674(void);
void fn_801193BC(void);
extern s32 pokemonWazaGetMaxPP(u8* ptr, u16 idx);
extern void wazaGetStatus(void);
extern void fn_8011F260(void);
extern void pokemonResetBasisStatus(void* ptr);
void pokemonSetLevelBasisStatus(void);
extern void heroItemGetItemKindToItemAryPtr(void);
extern void heroSetStatus();
extern void heroGetStatus(void);
extern void* GSresAllocResourceAlign(); /* K&R: called with 5 args, returns void* */
extern u8 fn_800FF548(void);
extern u32 _unloadScript__FPvUlUl(); /* K&R: asm void wrapper, used as function pointer */
extern u32 _unloadFont__FPvUlUl(); /* K&R: asm void wrapper, used as function pointer */
extern u32 _unloadMsg__FPvUlUl(); /* K&R: asm void wrapper, used as function pointer */
extern void fn_800EF5A4(void);
extern const char lbl_80272608[];
extern const char lbl_8027262C[];
extern u8 lbl_8035BB30[];
extern u8 lbl_8035BB50[];
extern u8 lbl_8035BB10[];
extern u8 lbl_8035BAF4[];
extern const char lbl_80272658[];
extern u8 lbl_8035BAD8[];
extern const char lbl_80272680[];
extern u8 lbl_8035BABC[];
extern u32 lbl_80478FB8;
extern u32 lbl_80478FBC;
extern u8 lbl_802726D4[];
extern u8 lbl_8035B8A0[];
extern void* floorDataBiosGetFieldCameraListPtr();
extern u32 floorDataBiosGetGroupID();
extern void* floorDataBiosGetPtr(u32 key);
extern void fn_8011791C(void);
extern void fn_80119930(void);
extern void fn_80119BD0();
extern u8 fn_80119D90(u16 idx);
extern u8 fn_80119DD0(u16 idx);
extern u16 fn_80119E10(u16 idx);
extern u8 fn_80119E50(u16 idx);
extern u8 fn_80119E90(u16 idx);
extern u16 fn_80119ED0(u16 idx);
extern u8 fn_80119F10(u16 idx);
extern u32 fn_80119F50(u16 idx);
extern void wazaDataBiosSetFightWazaWzxVariationFuncPtr(u8* ptr, u32 val);
extern void wazaDataBiosSetFightWazaWzxTypeFuncPtr(u8* ptr, u32 val);
extern u32 wazaDataBiosGetFightWazaWzxVariationFuncPtr(u8* ptr);
extern u8 wazaDataBiosGetTypeId(u8* ptr, u16 idx);
extern u32 wazaDataBiosGetFightWazaWzxTypeFuncPtr(u8* ptr);
extern void wazaDataBiosSetFightTrainerAiWazaDamageFuncPtr(u8* ptr, u32 val);
extern void wazaDataBiosSetFightTrainerAiWazaHitFuncPtr(u8* ptr, u32 val);
extern void wazaDataBiosSetFightTrainerAiWazaValueFuncPtr(u8* ptr, u32 val);
extern void wazaDataBiosSetTypeId(u8* ptr, u16 idx, u8 val);
extern u32 wazaDataBiosGetFightTrainerAiWazaDamageFuncPtr(u8* ptr);
extern u32 wazaDataBiosGetFightTrainerAiWazaHitFuncPtr(u8* ptr);
extern u32 wazaDataBiosGetFightTrainerAiWazaValueFuncPtr(u8* ptr);
extern void pokemonGetDarkPokemonLevel(void);
extern u32 pokemonDataCheckValid(u32 a, u16 key);
extern u8 fn_80121ADC(u8* ptr, u32 slot);
extern void pokemonSetWazaStatus(void);
extern u32 pokemonWazaCheckValid(u8* ptr, u32 arg2);
extern void pokemonInit(u8* ptr);
extern void pokemonEvolutionCreateAddPokemon(void);
extern void pokemonEvolution(void);
extern void savedataInit(void);
extern void heroAddPokedoru(u8* ptr, u32 offset);
extern s32 heroItemAddItemDataId(u8* ptr, u32 arg2, u32 arg3, u32 arg4);
extern u32 heroAddPokemon(u8* ptr, void* arg2);
extern void heroCreate(u8* ptr, u32 arg2, u8 arg3);
extern void heroInit();
extern void heroBiosSetPokecouponAll(); /* K&R: typed impl or conflict */
extern void heroBiosSetPokecoupon(); /* K&R: typed impl or conflict */
extern void heroBiosSetPokedoru(); /* K&R: typed impl or conflict */
extern void heroBiosSetHizukiNamePtr(); /* K&R: typed impl or conflict */
extern void heroBiosSetNamePtr(); /* K&R: typed impl or conflict */
extern void heroMoveTermEvent(void);
extern void heroMoveInitEvent(void);
extern void fn_8012CA84();
extern void heroPokemonGetEifie(u32 arg1);
extern void heroPokemonGetBlacky(u32 arg1);
extern s32 psGetGeneratorChildMaxLife(u32);
extern void* wazaDataBiosGetPtr(u16 idx);
extern u32 pokemonGetStatus();
extern void pokemonSetStatus();
extern u16 fn_8011E36C(u8* ptr, u16 idx);
extern u16 fn_8011E3B4(u8* ptr, u16 idx);
extern u8 fn_8011E3FC(u8* ptr, u16 idx);
extern u8 floorUpdateFieldCamera();
extern void updateAnimation__Ff15HEROMOVE_MEMBER(void);
extern void* heroBiosGetPokemonPtr(u8* ptr, u16 idx);
extern void* heroBiosGetHizukiNamePtr(void* ptr);
extern void* heroBiosGetHizukiItemPtr(u8* ptr, u16 idx);
extern void* heroBiosGetItemKoronPtr(u8* ptr, u16 idx);
extern void* heroBiosGetItemSeedPtr(u8* ptr, u16 idx);
extern void* heroBiosGetItemSkillPtr(u8* ptr, u16 idx);
extern void* heroBiosGetItemBallPtr(u8* ptr, u16 idx);
extern void* heroBiosGetExtraItemPtr(u8* ptr, u16 idx);
extern void* heroBiosGetItemNormalPtr(u8* ptr, u16 idx);
extern u32 heroBiosGetNamePtr(void* ptr);
extern u32 lbl_80478EBC;
extern u32 lbl_80478EB8;
extern void fn_80113F48(void);
extern void fn_8018C1E8(void);
extern void fn_801653CC(void);
extern void fn_80132A38(void);
extern void winMsgOpen(void);
extern void winMsgClose(void);
extern void pcboxDelItem(void);
extern void fn_8001E184(void);
void floorEventGetTresure(void);
extern void fn_8018B76C(void);
extern void fn_8018C7C8(void);
extern void fn_801902E0(void);
extern void fn_80166A28(void);
extern void peopleWaitSyncMotion(void);
extern void fn_80190528(void);
void floorEventCtrlTresure(void);
extern void floorGetResource(void);
extern void GSmodelSetAnimIndex(void);
extern void GSmodelSetAnimFrame(void);
extern void GSmodelSetAnimRate(void);
extern void GSmodelSetAnimType(void);
extern void GSmodelStartAnimation(void);
extern void _threadSwitch(void);
extern void GSmodelIsAnimating(void);
extern void GSmodelGetPart(void);
extern void GSpartGetTransform(void);
extern void GSpartFree();
extern void fn_8018AACC(void);
extern void peopleMoveCheck(u32 groupId, u32 index, u8 waitFlag);
extern void fn_8018805C(void);
extern void fn_80184470(void);
extern void fn_8018C0A8(void);
extern void fn_801669BC(void);
extern void GSmodelCanAnimate(void);
extern void fn_801845E4(void);
extern void fn_801860F8(void);
extern void GSmodelGetFrameCount(void);
extern void fn_800D37CC(void);
extern f32 lbl_8047CFA0;
extern u32 lbl_80478EC8;
extern u32 lbl_80478ECC;
extern f32 lbl_8047CFA4;
extern f32 lbl_8047CFA8;
extern f32 lbl_8047CFAC;
extern f64 lbl_8047CFB8;
extern f32 lbl_8047CFB4;
extern f32 lbl_8047CFB0;
void floorEventCtrlElevator(void);
extern void scriptSetCol(void);
extern void fn_801903B0(void);
extern void fn_8018C558(void);
extern void fn_8018C8F4(void);
void floorEventCtrlDoor(void);
extern void EvlogSet__FScUl();
extern void scriptSetEventColID();
extern void fn_800F7434();
extern u8 lbl_80272708[];
extern u32 lbl_80478EC4;
extern u32 lbl_80478EC0;
extern void* peopleInfoBiosGetPtrFromIndex(u16);
extern f32 lbl_8047CFD0;
extern u8 lbl_8047AD70;
extern f32 lbl_8047AD74;
extern f32 lbl_8047AD78;
extern f32 lbl_8047AD7C;
extern u8 GSscene_GetMode(void);
extern void cameraSetHeight(f32);
extern void cameraSetDistance(f32);
extern void cameraSetRotY(f32);
extern void GSscene_GetCameraPositionVector(void*);
extern f32 lbl_8047CFD8;
extern f32 lbl_8047CFD4;
extern void* GSresGetResource();
extern void GSmodelGetPosition(void*, void*);
extern void GSscene_GetCameraViewVector(void*);
extern f32 cameraGetHeight(void);
extern f32 cameraGetDistance(void);
extern f32 cameraGetRotY(void);
extern void fn_800E01F4(void* obj, f32 f1, f32 f2, f32 f3);
extern void GSmtxMakeYRotation(void*, f32);
extern void GSvecTransform(void*, void*, void*);
extern void GSvecAdd(void*, void*, void*);
extern f64 atan2(f32, f32);
extern void fn_801776E8(u32, void*, f32);
extern void fn_80177574(u32, void*, f32);
extern void fn_80177478(u32, void*, f32);
extern void GSgappTerminate(void);
extern void GSgappCreate(void);
void fn_801176C8(void);
extern void* GScameraGetActiveCamera();
extern void GSvecCopy();
extern u8 lbl_802727B8[];
extern void fn_80177A38(void); /* referenced by asm .inc wrappers (fn_801171C8/80117330/8011791C/8012E388/8012EBD4); was undefined -> broke the TU parse */
extern void GSmodelResetTextureChange(void);
extern void GSmodelFree(void);
extern void GStextureCreate(void);
extern void fn_80113D34(void);
extern void GSmodelSetVisibility(void);
extern void GSmodelLinkTexAnimToAnim(void);
extern void GSmodelSetTextureChange(void);
extern u32 lbl_80478B40;
extern u32 lbl_8047AD88;
extern u32 lbl_8047AD8C;
extern u32 lbl_8047AD90;
extern u32 lbl_8047AD94;
extern u32 lbl_8047AD80;
extern u32 lbl_8047AD84;
extern void fn_800EC134(u32);
extern void fn_800D4604();
extern void fn_800D377C();
extern void fn_800D3410();
extern void fn_800D9B24();
extern void fn_800D9AF0();
extern void fn_800D258C();
extern void fn_800D9D68();
extern void fn_800D9C24();
extern void _cameraLoadCameraMatrix__FP9_GScamera12GSgfxLayerID(void);
extern void GSmodelDrawModel();
extern void fn_800D3190(void);
extern void fn_800FF4D4(void);
extern u32 lbl_802727C8[];
extern void psSetBillboardCamera();
extern void fn_8016AB94();
extern void psGetParticleChildCount(void* ptr);
extern void psKillFamily();
extern void GSmodelSet60fpsAnimFlag();
extern void psUnlinkChildGensFromJObj();
extern void psKillGenerator();
extern u32 lbl_8047AD9C;
extern u32 lbl_8047ADA0;
void fn_801181B0(void);
extern void fn_800E06EC(void);
extern void GSvecTransformQuat(void);
extern void fn_800E0108(void);
extern void psInterpretParticles(void);
extern void psExecGenerator(void);
extern void fn_800057A0(void);
extern void jumptable_8035BB88();
extern u8 lbl_8047ADB0;
void fn_801183EC(void);
void fn_80118874(void);
extern void psSetParticleVisibility(); /* K&R: called with 0 or 1 args */
extern void psSetGeneratorAngleRadiusScale(void);
extern void psLinkChildGensToJObj(void);
extern f32 lbl_8047CFE8;
extern f32 lbl_8047CFEC;
extern void psCreateGeneratorID(void);
extern void fn_800D3094(void);
extern u32 lbl_8047ADAC;
extern u32 lbl_8047ADA8;
void fn_801190DC(void);
extern void psInitDataBank(void);
extern void DCFlushRange();
extern u8 lbl_802727D8[];
void fn_801195AC(void);
extern void fn_8016A01C(void);
extern void psInitGenerator(void);
extern void fn_8016AAF4(void);
extern void fn_8019733C(void);
extern void fn_8019D618(void);
extern void psSetPointJObjNodup(void);
extern void fn_8019D610(void);
extern u16 lbl_8047AD98;
extern u16 lbl_8047ADA4;
extern void GSmodelSearchModelList(void);
extern void GSmodelGetLinkedGSparticleBank(void);
extern void GSmodelIsRootNullAdded(void);
extern void GSpartGetJObjIndex(void);
extern void fn_800E3CBC(void);
extern void GSmodelGetGSparticleLinkAttachMode(void);
extern void GSmodelGetVisibility(void);
extern void fn_80135E44(void);
void fn_8011A0A8(void);
extern s32 kaisuuGetKaisuu(u32);
extern void jumptable_8035C260();
extern void fightWazaBiosSetWazaBanme(void);
extern void fightWazaBiosSetMotoWazaDataId(void);
extern void fightWazaBiosSetUseWazaDataId(void);
extern void fightWazaBiosSetTargetDataId(void);
extern void fightWazaBiosSetCritical(void);
extern void fightWazaBiosSetDamageValue(void);
extern void fightWazaBiosSetDamage(void);
extern void fightWazaBiosSetHitDamage(void);
extern void fightWazaBiosSetIryoku(void);
extern void fightWazaBiosSetZokusei(void);
extern void fightWazaBiosSetKaisuu(void);
extern void fightWazaBiosSetAutoMakeFlag(void);
extern void jumptable_8035C290();
void wazaSetStatus(void);
extern void fightWazaBiosGetWazaBanme(void);
extern void fightWazaBiosGetMotoWazaDataId(void);
extern void fightWazaBiosGetUseWazaDataId(void);
extern void fightWazaBiosGetTargetDataId(void);
extern void fightWazaBiosGetJoutaiPtr(void);
extern void fightWazaBiosGetCritical(void);
extern void fightWazaBiosGetDamageValue(void);
extern void fightWazaBiosGetDamage(void);
extern void fightWazaBiosGetHitDamage(void);
extern void fightWazaBiosGetIryoku(void);
extern void fightWazaBiosGetZokusei(void);
extern void fightWazaBiosGetKaisuu(void);
extern void fightWazaBiosGetAutoMakeFlag(void);
extern void jumptable_8035C35C();
extern u32 lbl_80478DF8;
extern u32 lbl_80478DFC;
extern u32 lbl_80478B78;
extern u32 lbl_8035F9A8[];
extern u32 lbl_80478E68;
extern u32 lbl_80478E6C;
extern u32 lbl_80478B70;
extern u8 lbl_8035F988[];
extern u32 lbl_80478B68;
extern u8 lbl_8035F5E0[];
extern u32 lbl_80478E58;
extern u32 lbl_80478E5C;
extern u32 lbl_80478E60;
extern u32 lbl_80478E64;
extern u32 lbl_80478B60;
extern u8 lbl_8035E940[];
extern void _flagSet(u32);
extern void* fn_801906A0(u32);
extern u32 lbl_80478F90;
extern u32 lbl_80478F94;
extern u8 lbl_80478B58[4];
extern u8 lbl_80478B5C[4];
extern f32 lbl_8047CFF0;
extern f64 lbl_8047D008;
extern f32 lbl_8047CFF4;
extern f64 lbl_8047D010;
extern f32 lbl_8047CFF8;
extern f32 lbl_8047CFFC;
extern f32 lbl_8047D000;
extern f32 lbl_8047D004;
extern void itemDataBiosGetPtr(void);
extern void itemDataBiosGetKind(void);
extern void itemDataBiosGetBuff(void);
extern f32 lbl_8047D018;
void pokemonAddDpFormPokemonDpFilterId(void);
extern void* fn_801EEEB8();
void pokemonSetDarkPokemonStatus(void);
extern void GScharCpy(void);
extern void fn_8010BBB8(void);
extern void fn_8001D994(void);
void pokemonToMenuPokemonStatus(void);
extern void fn_800FA280(void);
extern void fn_8010C4D4(void);
extern void fn_8010C46C(void);
extern void fn_800E0C54(void);
extern u8 lbl_8027296C[];
void pokemonCheckSetMonohiroi(void);
void pokemonAllKaihuku(void);
extern u32 fn_801DE190(u32 idx, u32 base, u32 flag);
extern void fn_801DA3CC(void);
extern void fn_801DA36C(void);
extern u32 lbl_80478F90; /* obj header ptr (SDA) */
extern void fn_80135530(void);
void pokemonGetFriendFormPokemonFriendFilterId(void);
extern u8 lbl_80272948[];
void pokemonGetEffortFromPokemon(void);
extern void fn_80008154(void);
extern void fn_80142CF4(void);
extern u32 sexGetPokemonSexRaitoKotei(u32);
extern void fn_801EE958(void);
extern void fn_801EEB34(void);
extern void memoDataSet(void);
void pokemonCheckFightOut(void);
extern void fn_80135AD0(void);
void pokemonCreate(void);
void pokemonCreateRndFit(void);
extern void fadeSet(void);
extern void fadeCheck(void);
extern void evolutionOpen(void);
extern f32 lbl_8047D020;
void pokemonEvolutionAll(void);
extern u8 lbl_802729A4[];
extern u8 lbl_80272998[];
extern void fn_800F9EE4(void);
extern void jumptable_80363468();
void getEvoPokemonLevelUp(void);
extern void itemDataBiosGetItemSoubiDataId(void);
void pokemonEvolutionCheck(void);
extern u8 lbl_80408400[];
extern void fn_8013528C(void);
extern void fn_800F9D04(void);
extern void gamedatasaveSetStatus(void);
extern u8 lbl_8047D028[8];
void savedataCreate(void);
extern void gamedataInit(void);
extern void pcboxInit(void);
extern void fn_801908D4(void);
extern void mailInitMailbox(void);
extern void sodateyaInit(void);
extern void fn_8006B6B4(void);
extern void memoInit(void);
extern void fn_80083CBC(void);
extern void fn_801EF128(void);
extern void exribbonInit(void);
extern void jumptable_803634A8();
extern void fn_80140A9C(void);
extern void fn_80140ACC(void);
extern void fn_80141308(void);
extern void fn_80142368(void);
extern void fn_80140588(void);
extern void fn_80134BC0(void);
extern void fn_80142A88(void);
extern void jumptable_803634F0();
extern void jumptable_80363558();
extern u8 lbl_80426BD0[];
extern void fadeEffectDokuStart(void);
extern void fn_8018C69C(void);
extern void fn_8018CA20(void);
extern void winMsgOpenField(void);
extern void winMsgCloseField(void);
extern void fn_801D0AFC(void);
extern void fn_80113FE8(void);
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
extern f32 lbl_8047D038;
void cbPoison__Fl15FootStepCounterl(void);
extern void fn_8018D998(void);
extern void peopleSearchID(void);
extern void peopleInfoBiosGetPtr(void);
extern void fn_8018F5E4(void);
extern void fn_8010F320(void);
extern void PSVECScale(void);
extern void PSVECAdd(void);
extern void fn_8010FDF8(void);
extern f32 lbl_8047D03C;
extern f32 lbl_8047D040;
extern f64 lbl_8047D048;
extern f64 lbl_8047D050;
extern f64 lbl_8047D058;
extern u8 lbl_80478AC0[4];
extern f32 lbl_8047D060;
void fn_8012B19C(void);
void heroMoveChkHinderClear(void);
extern void fn_800D3088(void);
extern f64 lbl_8047D068;
void getStep__FP8FOOTSTEPP8_GSmodelPiP8FOOTWORK(void);
extern void fn_8018CD08(void);
extern void fn_8018FCBC(void);
extern void fn_8018FC50(void);
extern void fn_800CE148(void);
extern void fn_800CDBE0(void);
extern void GScolsys2CheckGetEventID(void);
extern void fn_8018790C(void);
extern void fn_800F7D38(void);
extern void fn_800F7C8C(void);
extern void fn_8018BA04(void);
extern void fn_80187D48(void);
extern void fn_8018D7D0(void);
extern void fn_80183730(void);
extern void fn_8018397C(void);
extern void fn_801812E8(void);
extern void fn_80189490(void);
extern void fn_80183688(void);
extern f32 lbl_8047D070;
extern f32 lbl_8047D074;
extern f32 lbl_8047D078;
extern f32 lbl_8047D07C;
extern f32 lbl_8047D080;
void updateChat__F15HEROMOVE_MEMBER(void);
void heroMoveCheckEvent(void);
extern void fn_8018F4C8(void);
extern void GSmodelGetAnimIndex(void);
extern void GSmodelGetAnimFrame(void);
extern void GSmodelSetAnimBlend(void);
extern void GSmodelSetBlendFactor(void);
extern f32 lbl_8047D084;
extern f32 lbl_8047D088;
extern f32 lbl_8047D08C;
extern f32 lbl_8047D090;
extern f32 lbl_8047D0A8;
void fn_8012D39C(void);
extern f32 lbl_8047D0AC;
void fn_8012D7F0(void);
extern void fn_800E3C64(void);
extern f32 lbl_8047D0B0;
void fn_8012DE94(void);
extern void fn_800F7A7C(void);
extern void fn_800F7A08(void);
extern void fn_800F7BC4(void);
extern void fn_80188214(void);
extern void fn_80166458(void);
extern f64 lbl_8047D0C8;
extern f32 lbl_8047D0B4;
extern f32 lbl_8047D0B8;
extern f32 lbl_8047D0BC;
extern f32 lbl_8047D0C0;
extern f32 lbl_8047D0C4;
void fn_8012E388(void);
extern void fn_800F7AF0(void);
extern void fn_801887D8(void);
extern void PSVECDistance(void);
extern f32 lbl_8047D0D0;
extern f32 lbl_8047D0D4;
void moveLeader__F15HEROMOVE_MEMBER(void);
extern void dbgMenuIsOpen(void);
extern void menuIsCheck(void);
extern void fn_8018C424(void);
extern void fn_8000D710(void);
extern u8 lbl_80272A38[];
extern f32 lbl_8047D064;
void heroMoveMain(void);
extern void fn_80188AF4(u32, u32);
extern void fn_80188F78(u32, u32);
void fn_8012F1FC(void);
void fn_8012F40C(void);
extern void GSmodelGetRotation(void);
extern void fn_8010E138(void);
extern void GSmodelSetRotation(void);
extern f32 lbl_8047D0D8;
void initFloor__Fv(void);
extern void fn_8011393C(void);
extern void fn_8006AE18(void);
extern u8 lbl_802729C0[];
extern u8 lbl_80272A10[];
void heroMoveGetKenObjID(void);
extern void fn_8018E050(void);
extern void GSmodelEnableAnimBlend(void);
extern void fn_8018CB5C(void);
extern void fn_80189328(void);
extern void fn_8018BF24(void);
void heroMoveInit(void);
void heroMoveSyncWithHero(void);
void fn_8013024C(void);
extern void gamedataGetStatus(void);
extern void gamedataAttestCreate(void);
extern void fn_800F76E4();
extern void fn_80112700(void);
extern void GSmsgFontOpen();
extern void GSmsgOpen();
extern u8 fn_800FF554(void);
extern void fn_800F760C();
extern void fn_800FC2A8();
extern void GSmsgClose();
extern u8 lbl_8035BA98[];
extern u8 lbl_8035BA7C[];
extern u8 lbl_8035BA60[];
extern u8 lbl_8035BA48[];
extern u8 lbl_8035BA2C[];
extern u8 lbl_8035BA10[];
extern u8 lbl_8035B9F8[];
extern u8 lbl_8035B9DC[];
extern u8 lbl_8035B9C0[];
extern u8 lbl_8035B9A4[];
extern u8 lbl_8035B988[];
extern u8 lbl_8035B96C[];
extern u8 lbl_8035B950[];
extern u8 lbl_8035B938[];
extern u8 lbl_8035B904[];
extern u16 fn_801EF624();
extern u8 lbl_8035B8E8[];
extern const char lbl_802726AC[];
extern const char lbl_8035B8CC[];
extern u8 lbl_8035B8B4[];
extern u8 lbl_8035BB70[];
extern u32 lbl_8047CFC0;
extern u32 lbl_8047CFC8;
extern u32 lbl_8047CFC4;
extern void GSvecSquareDistance(void);
extern f32 lbl_8047CFDC;
extern f32 lbl_8047CFE0;
extern u32 lbl_80478B48; /* NPC count (SDA) */
extern u32 lbl_80478F94; /* obj data base (SDA) */
extern void* tasteDataGetPtr(void* ptr);
extern void tasteDataGetAisyou(void* ptr, u8 val);
extern void fn_80135708(void);
extern void fn_801FD938(void);
extern void fn_801FD928(void);
extern void fn_801FD918(void);
extern void fn_801FD908(void);
extern void fn_801FD8F8(void);
extern void fn_801FD8D0(void);
extern void fn_801FD8C0(void);
extern void fn_801FD8B0(void);
extern void fightWazaCheckWriteJoutaiDataId(void);
extern void fightWazaWriteJoutaiDataId(void);
extern void fightWazaIsJoutaiDataId(void);
extern void fightWazaInitJoutaiDataId(void);
extern void fightWazaInitJoutai(void);
extern void fn_801FD8A0(void);
extern void fn_801FD890(void);
extern void fn_801FD880(void);
extern void fn_801FD870(void);
extern void fn_801FD860(void);
extern void fn_801FD850(void);
extern void fn_801FD840(void);
extern void fn_801FD820(void);
extern void fn_801FD7F8(void);
extern void fn_801FCF7C(void);
extern void fn_801FCF6C(void);
extern void fn_801FCF5C(void);
extern void fn_801FCF4C(void);
extern void fn_801FCF3C(void);
extern void fn_801FCF2C(void);
extern void fn_801FCF1C(void);
extern void fn_801FCF0C(void);
extern void fn_801FCEFC(void);
extern void fn_801FD728(void);
extern void fn_801FD718(void);
extern void fn_801FD708(void);
extern void fn_801FD6F8(void);
extern void fn_801FD660(void);
extern void fn_801FD5F0(void);
extern void fn_801FD5C8(void);
extern void fn_801FD6E8(void);
extern void fn_801FD6D8(void);
extern void fn_801FD6C8(void);
extern void fn_801FD6B8(void);
extern void fn_801FD330(void);
extern void fn_801FD320(void);
extern void fn_801FD310(void);
extern void fn_801FD300(void);
extern void fn_801FD2F0(void);
extern void fn_801FD2E0(void);
extern void fn_801FD2D0(void);
extern void fn_801FD2C0(void);
extern void fn_801FD2B0(void);
extern void fn_801FD2A0(void);
extern void fn_801FD290(void);
extern void fn_801FD280(void);
extern void fn_801FD270(void);
extern void fn_801FD260(void);
extern void fn_801FD250(void);
extern void fn_801FD240(void);
extern void fn_801FD230(void);
extern void fn_801FD220(void);
extern void fn_801FD210(void);
extern void fn_801FD200(void);
extern void fn_801FD1F0(void);
extern void fn_801FD1E0(void);
extern void fn_801FD1D0(void);
extern void fn_801FD1C0(void);
extern void fn_801FD1B0(void);
extern void fn_801FD1A0(void);
extern void fn_801FD178(void);
extern void fn_801FD150(void);
extern void jumptable_8035E028();
extern void fn_8011E4A4(void);
extern void fn_801FDB60(void);
extern void fn_801FDB48(void);
extern void fn_801FDB14(void);
extern void fn_801FDAFC(void);
extern void fn_801FDAE4(void);
extern void fn_801FDACC(void);
extern void fn_801FDAB4(void);
extern void fn_801FD8E0(void);
extern void fn_801FDA9C(void);
extern void fn_801FDA84(void);
extern void fn_801FDA6C(void);
extern void fn_801FDA38(void);
extern void fn_801FDA20(void);
extern void fightOutPokemonGetUseWazaDataId(void);
extern void fightOutPokemonGetMotoWazaDataId(void);
extern void fightWazaIsHit(void);
extern void fn_801FDA08(void);
extern void fn_801FD9F0(void);
extern void fn_801FD9D8(void);
extern void fn_801FD9C0(void);
extern void fn_801FD9A8(void);
extern void fn_801FD990(void);
extern void fn_801FD978(void);
extern void fn_801FD960(void);
extern void fn_801FD948(void);
extern void fn_801FD808(void);
extern void fn_801FD064(void);
extern void fn_801FD04C(void);
extern void fn_801FD034(void);
extern void fn_801FD01C(void);
extern void fn_801FD004(void);
extern void fn_801FCFEC(void);
extern void fn_801FCFD4(void);
extern void fn_801FCFBC(void);
extern void fn_801FCFA4(void);
extern void fn_801FCF8C(void);
extern void fn_801FD7E0(void);
extern void fn_801FD7C8(void);
extern void fn_801FD7B0(void);
extern void fn_801FD798(void);
extern void fn_801FD684(void);
extern void fn_801FD648(void);
extern void fn_801FD614(void);
extern void fn_801FD5D8(void);
extern void fn_801FD5B0(void);
extern void fn_801FD780(void);
extern void fn_801FD768(void);
extern void fn_801FD750(void);
extern void fn_801FD738(void);
extern void fn_801FD598(void);
extern void fn_801FD580(void);
extern void fn_801FD568(void);
extern void fn_801FD550(void);
extern void fn_801FD538(void);
extern void fn_801FD520(void);
extern void fn_801FD508(void);
extern void fn_801FD4F0(void);
extern void fn_801FD4D8(void);
extern void fn_801FD4C0(void);
extern void fn_801FD4A8(void);
extern void fn_801FD490(void);
extern void fn_801FD478(void);
extern void fn_801FD460(void);
extern void fn_801FD448(void);
extern void fn_801FD430(void);
extern void fn_801FD418(void);
extern void fn_801FD400(void);
extern void fn_801FD3E8(void);
extern void fn_801FD3D0(void);
extern void fn_801FD3B8(void);
extern void fn_801FD3A0(void);
extern void fn_801FD388(void);
extern void fn_801FD370(void);
extern void fn_801FD358(void);
extern void fn_801FD340(void);
extern void fn_801FD188(void);
extern void fn_801FD160(void);
extern void fn_801FD11C(void);
extern void fightOutPokemonCheckFightOut(void);
extern void jumptable_8035E4B0();
extern void fn_801885C4(void);
extern void PSVECDotProduct(void);
extern void fn_8018F678(void);
extern void fn_8018F658(void);
extern f32 lbl_8047D094;
extern f32 lbl_8047D098;
extern f32 lbl_8047D09C;
extern f32 lbl_8047D0A0;
extern f32 lbl_8047D0A4;
extern u8 fn_801174EC(void);
extern void fn_80119F90(u8* ptr, u16 val);
extern void fn_80119FA0(u8* ptr, u32 val);
extern void fn_80119FB0(u8* ptr, u8 val);
extern void fn_80119FC0(u8* ptr, u8 val);
extern void fn_80119FD0(u8* ptr, u8 val);
extern void fn_80119FE0(u8* ptr, u16 val);
extern void fn_80119FF0(u8* ptr, u16 val);
extern u16 fn_8011A000(u8* ptr);
extern u32 fn_8011A018(u8* ptr);
extern u8 fn_8011A030(u8* ptr);
extern u8 fn_8011A048(u8* ptr);
extern u8 fn_8011A060(u8* ptr);
extern u16 fn_8011A078(u8* ptr);
extern u16 fn_8011A090(u8* ptr);
extern void wazaDataBiosSetRiskFlag(u8* ptr, u8 val);
extern u8 wazaDataBiosGetRiskFlag(u8* ptr);
extern void wazaDataBiosSetHidenFlag(u8* ptr, u8 val);
extern u8 wazaDataBiosGetHidenFlag(u8* ptr);
extern void fn_8011C5E0(u8* ptr, u8 val);
extern void fn_8011C5F0(u8* ptr, u8 val);
extern void wazaDataBiosSetDoc(u8* ptr, u32 val);
extern void wazaDataBiosSetWazawzxdataId(u8* ptr, u32 val);
extern void wazaDataBiosSetPressure(u8* ptr, u8 val);
extern void wazaDataBiosSetBouon(u8* ptr, u8 val);
extern void wazaDataBiosSetNegoto(u8* ptr, u8 val);
extern void wazaDataBiosSetNekonote(u8* ptr, u8 val);
extern void wazaDataBiosSetMonomane(u8* ptr, u8 val);
extern void wazaDataBiosSetYubiwohuru(u8* ptr, u8 val);
extern void wazaDataBiosSetOujanosirusi(u8* ptr, u8 val);
extern void wazaDataBiosSetOumugaesi(u8* ptr, u8 val);
extern void wazaDataBiosSetYokodori(u8* ptr, u8 val);
extern void wazaDataBiosSetMajikku(u8* ptr, u8 val);
extern void wazaDataBiosSetMamoru(u8* ptr, u8 val);
extern void wazaDataBiosSetDageki(u8* ptr, u8 val);
extern void wazaDataBiosSetAddFightKoukaAvg(u8* ptr, u8 val);
extern void wazaDataBiosSetFightAttackMsgId(u8* ptr, u32 val);
extern void wazaDataBiosSetSeqId(u8* ptr, u16 val);
extern void wazaDataBiosSetFightKoukaDataId(u8* ptr, u16 val);
extern void wazaDataBiosSetIryoku(u8* ptr, u16 val);
extern void wazaDataBiosSetAvg(u8* ptr, u8 val);
extern void wazaDataBiosSetRangeId(u8* ptr, u8 val);
extern void wazaDataBiosSetPri(u8* ptr, u8 val);
extern void wazaDataBiosSetZokuseiDataId(u8* ptr, u8 val);
extern void wazaDataBiosSetPp(u8* ptr, u8 val);
extern void wazaDataBiosSetName(u8* ptr, u32 val);
extern u8 fn_8011C790(u8* ptr);
extern u8 fn_8011C7A8(u8* ptr);
extern u32 wazaDataBiosGetDoc(u8* ptr);
extern u32 wazaDataBiosGetWazawzxdataId(u8* ptr);
extern u8 wazaDataBiosGetPressure(u8* ptr);
extern u8 wazaDataBiosGetBouon(u8* ptr);
extern u8 wazaDataBiosGetNegoto(u8* ptr);
extern u8 wazaDataBiosGetNekonote(u8* ptr);
extern u8 wazaDataBiosGetMonomane(u8* ptr);
extern u8 wazaDataBiosGetYubiwohuru(u8* ptr);
extern u8 wazaDataBiosGetOujanosirusi(u8* ptr);
extern u8 wazaDataBiosGetOumugaesi(u8* ptr);
extern u8 wazaDataBiosGetYokodori(u8* ptr);
extern u8 wazaDataBiosGetMajikku(u8* ptr);
extern u8 wazaDataBiosGetMamoru(u8* ptr);
extern u8 wazaDataBiosGetDageki(u8* ptr);
extern u8 wazaDataBiosGetAddFightKoukaAvg(u8* ptr);
extern u32 wazaDataBiosGetFightAttackMsgId(u8* ptr);
extern u32 wazaDataBiosGetFightAttackTunagiMsgId(u8* ptr);
extern u16 wazaDataBiosGetSeqId(u8* ptr);
extern u16 wazaDataBiosGetFightKoukaDataId(u8* ptr);
extern s16 wazaDataBiosGetIryoku(u8* ptr);
extern u8 wazaDataBiosGetAvg(u8* ptr);
extern u8 wazaDataBiosGetRangeId(u8* ptr);
extern s32 wazaDataBiosGetPri(u8* ptr);
extern u8 wazaDataBiosGetZokuseiDataId(u8* ptr);
extern u8 wazaDataBiosGetPp(u8* ptr);
extern u32 wazaDataBiosGetName(u8* ptr);
extern s32 fn_8011CA9C(u8* ptr);
extern u32 fn_8011CB3C(u8* ptr);
extern u32 fn_8011CB54(u8* ptr);
extern u8 fn_8011CB98(u8* ptr);
extern u8 fn_8011CBB0(u8* ptr);
extern u8 fn_8011CC54(u8* ptr);
extern u8 fn_8011CC6C(u8* ptr);
extern u8 fn_8011CC84(u8* ptr);
extern u8 fn_8011CC9C(u8* ptr);
extern u8 fn_8011CCB4(u8* ptr);
extern u8 fn_8011CCCC(u8* ptr);
extern u8 fn_8011CCE4(u8* ptr);
extern s32 fn_8011CCFC(u8* ptr);
extern s32 fn_8011CD18(u8* ptr);
extern s32 fn_8011CD34(u8* ptr);
extern s32 fn_8011CD50(u8* ptr);
extern s32 fn_8011CD6C(u8* ptr);
extern u8 fn_8011CD88(u8* ptr);
extern u8 fn_8011CDA0(u8* ptr);
extern u8 fn_8011CDB8(u8* ptr);
extern u8 fn_8011CDD0(u8* ptr);
extern u8 fn_8011CDE8(u8* ptr);
extern u32 fn_8011CE00(u8* ptr);
extern void fn_8011CEA0(u8* ptr, u8 val);
extern void fn_8011CEB0(u8* ptr, u8 val);
extern void fn_8011CEC0(u8* ptr, u16 val);
extern void fn_8011CF14(u8* ptr, u32 val);
extern void fn_8011CF24(u8* ptr, u16 val);
extern void fn_8011CF34(u8* ptr, u16 val);
extern void fn_8011CF9C(u8* ptr, u16 val);
extern void fn_8011CFAC(u8* ptr, u32 val);
extern void fn_8011CFBC(u8* ptr, u16 val);
extern void fn_8011CFCC(u8* ptr, u16 val);
extern void fn_8011CFDC(u8* ptr, u32 val);
extern void fn_8011D270(u8* ptr, u16 val);
extern void fn_8011D280(u8* ptr, u8 val);
extern void fn_8011D290(u8* ptr, u8 val);
extern void fn_8011D2A0(u8* ptr, u8 val);
extern void fn_8011D2B0(u8* ptr, u16 val);
extern void fn_8011D470(u8* ptr, u32 val);
extern void fn_8011D494(u8* ptr, u16 val);
extern void fn_8011D4A4(u8* ptr, u16 val);
extern void fn_8011D4B4(u8* ptr, u16 val);
extern void fn_8011D4C4(u8* ptr, u8 val);
extern void fn_8011D4D4(u8* ptr, u8 val);
extern void fn_8011D4E4(u8* ptr, u8 val);
extern void fn_8011D4F4(u8* ptr, u8 val);
extern void fn_8011D56C(u8* ptr, u8 val);
extern void fn_8011D57C(u8* ptr, u8 val);
extern void fn_8011D760(u8* ptr, u8 val);
extern void fn_8011D8F4(u8* ptr, u16 val);
extern void fn_8011DCB4(u8* ptr, u16 val);
extern void fn_8011DE38(u8* ptr, u32 val);
extern void fn_8011DE88(u8* ptr, u32 val);
extern void fn_8011DE98(u8* ptr, u32 val);
extern void fn_8011DF90(u8* ptr, u32 val);
extern void fn_8011DFA0(u8* ptr, u8 val);
extern void fn_8011DFB0(u8* ptr, u8 val);
extern void fn_8011DFC0(u8* ptr, u8 val);
extern void fn_8011DFD0(u8* ptr, u16 val);
extern void fn_8011DFE0(u8* ptr, u32 val);
extern void fn_8011DFF0(u8* ptr, u16 val);
extern u8 fn_8011E000(u8* ptr);
extern u8 fn_8011E018(u8* ptr);
extern u16 fn_8011E030(u8* ptr);
extern u32 fn_8011E0AC(u8* ptr);
extern u16 fn_8011E0C4(u8* ptr);
extern u16 fn_8011E0DC(u8* ptr);
extern u16 fn_8011E15C(u8* ptr);
extern u32 fn_8011E174(u8* ptr);
extern u16 fn_8011E18C(u8* ptr);
extern u16 fn_8011E1A4(u8* ptr);
extern u32 fn_8011E1BC(u8* ptr);
extern u16 fn_8011E4D8(u8* ptr);
extern u8 fn_8011E4F0(u8* ptr);
extern u8 fn_8011E508(u8* ptr);
extern u8 fn_8011E520(u8* ptr);
extern u16 fn_8011E538(u8* ptr);
extern u32 fn_8011E760(u8* ptr);
extern u16 fn_8011E7C0(u8* ptr);
extern u16 fn_8011E7D8(u8* ptr);
extern u16 fn_8011E7F0(u8* ptr);
extern u8 fn_8011E808(u8* ptr);
extern u8 fn_8011E820(u8* ptr);
extern u8 fn_8011E838(u8* ptr);
extern u8 fn_8011E850(u8* ptr);
extern u8 fn_8011E8DC(u8* ptr);
extern u8 fn_8011E8F4(u8* ptr);
extern u8 fn_8011EB48(u8* ptr);
extern u16 fn_8011EDF8(u8* ptr);
extern u32 fn_8011EE10(u8* ptr);
extern u32 fn_8011EE28(u8* ptr);
extern u16 fn_8011EE40(u8* ptr);
extern u16 fn_8011EE58(u8* ptr);
extern u16 fn_8011F188(u8* ptr);
extern u16 fn_8011F1A0(u8* ptr);
extern u32 fn_8011F45C(u8* ptr);
extern u8 fn_8011F4A8(u8* ptr);
extern u32 fn_8011F4C0(u8* ptr);
extern u32 fn_8011F520(u8* ptr);
extern u8 fn_8011F538(u8* ptr);
extern u8 fn_8011F550(u8* ptr);
extern u8 fn_8011F568(u8* ptr);
extern u16 fn_8011F580(u8* ptr);
extern u32 fn_8011F5B0(u8* ptr);
extern void heroBiosSetHomePlace(u8* ptr, u8 val);
extern u8 heroBiosGetHomePlace(u8* ptr);
extern u8 heroBiosGetHizukiFlag(u8* ptr);
extern void heroBiosSetHizukiFlag(u8* ptr, u8 val);
extern u32 heroBiosGetPokecouponAll(u8* ptr);
extern u32 heroBiosGetPokecoupon(u8* ptr);
extern u32 heroBiosGetPokedoru(u8* ptr);
extern u8 heroBiosGetBadge08Flag(u8* ptr);
extern u8 heroBiosGetBadge07Flag(u8* ptr);
extern u8 heroBiosGetBadge06Flag(u8* ptr);
extern u8 heroBiosGetBadge05Flag(u8* ptr);
extern u8 heroBiosGetBadge04Flag(u8* ptr);
extern u8 heroBiosGetBadge03Flag(u8* ptr);
extern u8 heroBiosGetBadge02Flag(u8* ptr);
extern u8 heroBiosGetBadge01Flag(u8* ptr);
extern void heroBiosSetBadge08Flag(u8* ptr, u8 val);
extern void heroBiosSetBadge07Flag(u8* ptr, u8 val);
extern void heroBiosSetBadge06Flag(u8* ptr, u8 val);
extern void heroBiosSetBadge05Flag(u8* ptr, u8 val);
extern void heroBiosSetBadge04Flag(u8* ptr, u8 val);
extern void heroBiosSetBadge03Flag(u8* ptr, u8 val);
extern void heroBiosSetBadge02Flag(u8* ptr, u8 val);
extern void heroBiosSetBadge01Flag(u8* ptr, u8 val);
extern u8 heroBiosGetSexDataId(u8* ptr);
extern void heroBiosSetSexDataId(u8* ptr, u8 val);
extern void heroBiosSetRnd(u8* ptr, u32 val);
extern u32 heroBiosGetRnd(u8* ptr);
extern u32 fn_80130CD8(void);
extern u16 fn_8011F5C8(u8* ptr);
extern u32 fn_80128E24(void);
extern void* floorReadMapPreFunc(void* owner, u32 param, u32 alloc_size);
extern void* floorReadScriptPreFunc(void* owner, u32 param, u32 alloc_size);
extern void* floorReadFontPreFunc(void* owner, u32 param, u32 alloc_size);
extern void* floorReadMsgPreFunc(void* owner, u32 param, u32 alloc_size);
extern u32 _unloadFlare__FPvUlUl(void);
extern u32 _unloadParticles__FPvUlUl(void);
extern u32 _unloadCamera__FPvUlUl(void);
extern u32 _unloadLight__FPvUlUl(void);
extern u32 _unloadColsys__FPvUlUl(void);
extern u32 _unloadTexture__FPvUlUl(void);
extern u32 floorReadMakeFogResID(u32 val);
extern u32 floorReadMakeCameraResID(u32 val);
extern u32 floorReadMakeLightResID(u32 val);
extern u32 floorReadMakeModelResID(u32 val);
extern u32 floorDataBiosGetShadowReciveNum(void* ptr);
extern void* floorDataBiosGetShadowReciveID(void* ptr, u32 idx);
extern void* floorDataBiosGetShadowLightID(void* ptr);
extern void* floorDataBiosGetSunResID(void* ptr);
extern void fn_8011553C(void* obj, u32 val);
extern void floorDataBiosSetMapResID(void* obj, u32 val);
extern u32 floorDataBiosGetFileGroupID(u8* entry);
extern void* floorDataBiosGetCurrentPtr(void);
extern void* fn_80115CB4(u32 param);
extern void fn_80115D64(u32 r25, u32 r26);
extern void fn_80116D30(u32 kind, u32 arg);
extern void floorCharacterBiosSetVisibility(u8* ptr, u8 val);
extern void floorCharacterBiosSetPos(u8* dst, f32* src);
extern u32 floorCharacterBiosGetTalkSctID(void* ptr);
extern u32 floorCharacterBiosGetMoveSctID(void* ptr);
extern u32 floorCharacterBiosGetNameID(void* ptr);
extern u32 floorCharacterBiosGetTalkWallThrough(u8* ptr);
extern u32 floorCharacterBiosGetTalkEndType(u8* ptr);
extern u32 floorCharacterBiosGetTalkStartType(u8* ptr);
extern u32 floorCharacterBiosGetMoveType(u8* ptr);
extern u32 floorCharacterBiosGetLoadInit(u8* ptr);
extern u32 floorCharacterBiosGetVisibility(u8* ptr);
extern void* floorCharacterBiosGetPeopleInfoPtr(u8* ptr);
extern void fn_8011711C(u32 arg);
extern void fn_80117154(void);
extern void fn_80117164(void);
extern void fn_801171C8(void);
extern void fn_80117330(f32 arg);
extern u32 fn_801174C4(void);
extern void fn_801174F4(void);
extern void fn_80117500(void);
extern u32 fn_80117AD4(void);
extern u8 fn_80117AE4(u32 arg1);
extern void fn_80117C84(void);
extern void fn_80117D14(void);
extern void fn_80117E58(void* arg);
extern void fn_80118020(void);
extern void fn_80118070(void);
extern void fn_80118100(void);
extern void fn_80118104(u32 a, u8 b);
extern void fn_80118A68(u8* obj, u32 notify);
extern void fn_80118C20(u8* arg1, void* arg2, u32 arg3, u32 arg4, u32 arg5);
extern void fn_80118C88(void* obj);
extern void fn_80118CAC(void* obj);
extern void fn_80118CD0(void* obj);
extern void fn_80118CF4(void* obj);
extern void fn_80118D18(void* obj);
extern void fn_80118D3C(void* obj);
extern void fn_80118D60(void* obj);
extern void fn_80118D84(void* obj);
extern s32 fn_80118DA8(u8* ptr);
extern void fn_80118DE0(u8* arg1, f32* arg2, u32 arg3, u32 arg4);
extern void fn_80118E8C(u8* arg1, f32* arg2, u32 arg3, u32 arg4, u32 arg5);
extern void fn_80118F04(u8* arg1, f32* arg2, u32 arg3, u32 arg4, u32 arg5);
extern void fn_80118F7C(u8* obj, void* arg);
extern void fn_80118FB0(u8* obj, u8* desc, u32 state, u32 byte5, u32 init_from_zero, u32 attach_model);
extern void fn_80119824(u32 count1, u32 count2);
extern void fn_8011A280(u8* arg1, u16 arg2, u32 arg3);
extern s32 fn_8011A3E4(void* obj, u16 val);
extern void fn_8011A570(u8* arg1, u16 arg2, u32 arg3);
extern s32 fn_8011A6D4(void* obj, u16 val);
extern s32 fn_8011A860(void* obj, u16 val);
extern void fn_8011A9EC(u8* arg1, u16 arg2, u32 arg3);
extern void fn_8011AB50(u8* arg1, u16 arg2, u32 arg3);
extern s32 fn_8011ACB4(void* obj, u16 val);
extern s32 fn_8011AE40(void* obj, u16 val);
extern void fn_8011AFCC(u8* arg1, u16 arg2, u32 arg3);
extern s32 fn_8011B130(void* obj, u16 val);
extern void fn_8011B2C0(void* obj, u16 id, u16 arg3);
extern s32 fn_8011B444(void* obj, u16 val);
extern s32 fn_8011B67C(void* obj, u16 val);
extern void fn_8011B788(void* obj, u16 id);
extern void fn_8011B950(u8* base, u16 count);
extern u32 fn_8011BA0C(u8 type);
extern u32 wazaIsWazaTypeId(u32 key, u8 target);
extern u8 wazaGetMaxPP(u32 arg1, u8 arg2);
extern u32 pokemonNakigoeDataBiosGetDataAddress(u8* ptr);
extern void* pokemonDpFilterDataBiosGetPtr(u16 idx);
extern s8 pokemonFriendFilterDataBiosGetValue(u8* ptr, u8 idx);
extern void* fn_8011CB10(u16 idx);
extern void* fn_8011CB6C(u16 idx);
extern void* pokemonSeikakuRateDataBiosGetPtr(u8 idx);
extern u8 fn_8011CBF4(u8* ptr, u8 idx);
extern u8 fn_8011CC24(u8* ptr, u8 idx);
extern void* fn_8011CE18(u8 idx);
extern u32 pokemonGrowDataBiosGetExp(u8* ptr, u8 idx);
extern void* fn_8011CE74(u8 idx);
extern void fn_8011CED0(u8* ptr, u16 idx, u8 val);
extern void fn_8011CEF0(u8* ptr, u16 idx, u16 val);
extern void fn_8011CF44(u8* ptr);
extern void fn_8011CF70(u8* ptr);
extern void fn_8011CFEC(u8* ptr, u16 idx, u8 val);
extern void fn_8011D02C(u8* ptr, u16 idx, u16 val);
extern void fn_8011D06C(u8* ptr, u16 idx, u32 val);
extern void fn_8011D0AC(u8* ptr, u16 idx, u8 val);
extern void fn_8011D0CC(u8* ptr, u16 idx, u16 val);
extern void fn_8011D10C(u8* ptr, u16 idx, u8 val);
extern void fn_8011D14C(u8* ptr, u16 idx, u16 val);
extern void fn_8011D18C(u8* ptr, u16 idx, u16 val);
extern void fn_8011D1CC(u8* ptr, u16 idx, u8 val);
extern void fn_8011D20C(u8* ptr, u16 idx, u8 val);
extern void fn_8011D22C(u8* ptr, u16 idx, u8 val);
extern void fn_8011D24C(u8* ptr, u16 idx, u16 val);
extern void fn_8011D2C0(void* ptr, u16 val);
extern void fn_8011D2E4(void* ptr, u16 val);
extern void fn_8011D308(void* ptr, u16 val);
extern void fn_8011D32C(void* ptr, u16 val);
extern void fn_8011D350(void* ptr, u16 val);
extern void fn_8011D374(void* ptr, u16 val);
extern void fn_8011D398(void* ptr, u16 val);
extern void fn_8011D3BC(void* ptr, u16 val);
extern void fn_8011D3E0(void* ptr, u16 val);
extern void fn_8011D404(void* ptr, u16 val);
extern void fn_8011D428(void* ptr, u16 val);
extern void fn_8011D44C(void* ptr, u16 val);
extern void fn_8011D480(u8* ptr, u8 val);
extern void fn_8011D504(u8* ptr, u8 val);
extern void fn_8011D58C(void* ptr, u8 val);
extern void fn_8011D5B0(void* ptr, u8 val);
extern void fn_8011D5D4(void* ptr, u8 val);
extern void fn_8011D5F8(void* ptr, u8 val);
extern void fn_8011D61C(void* ptr, u8 val);
extern void fn_8011D640(void* ptr, u8 val);
extern void fn_8011D664(void* ptr, u8 val);
extern void fn_8011D688(void* ptr, u8 val);
extern void fn_8011D6AC(void* ptr, u8 val);
extern void fn_8011D6D0(void* ptr, u8 val);
extern void fn_8011D6F4(void* ptr, u8 val);
extern void fn_8011D718(void* ptr, u8 val);
extern void fn_8011D73C(void* ptr, u8 val);
extern void fn_8011D770(void* ptr, u8 val);
extern void fn_8011D794(void* ptr, u8 val);
extern void fn_8011D7B8(void* ptr, u8 val);
extern void fn_8011D7DC(void* ptr, u8 val);
extern void fn_8011D800(void* ptr, u8 val);
extern void fn_8011D824(void* ptr, u8 val);
extern void fn_8011D848(void* ptr, u8 val);
extern void fn_8011D86C(void* ptr, u8 val);
extern void fn_8011D890(void* ptr, u8 val);
extern void fn_8011D8B4(void* ptr, u8 val);
extern void fn_8011D8D8(u8* ptr, s32 val);
extern void fn_8011D904(u8* ptr, u16 val);
extern void fn_8011D924(u8* ptr, u16 val);
extern void fn_8011D958(u8* ptr, u16 val);
extern void fn_8011D98C(u8* ptr, u16 val);
extern void fn_8011D9C0(u8* ptr, u16 val);
extern void fn_8011D9F4(u8* ptr, u16 val);
extern void fn_8011DA28(u8* ptr, u16 val);
extern void fn_8011DA5C(u8* ptr, u16 val);
extern void fn_8011DA90(u8* ptr, u16 val);
extern void fn_8011DAC4(u8* ptr, u16 val);
extern void fn_8011DAF8(u8* ptr, u16 val);
extern void fn_8011DB2C(u8* ptr, u16 val);
extern void fn_8011DB60(u8* ptr, u16 val);
extern void fn_8011DB94(void* ptr, u16 val);
extern void fn_8011DBB8(void* ptr, u16 val);
extern void fn_8011DBDC(void* ptr, u16 val);
extern void fn_8011DC00(void* ptr, u16 val);
extern void fn_8011DC24(void* ptr, u16 val);
extern void fn_8011DC48(void* ptr, u16 val);
extern void fn_8011DC6C(u8* ptr, u16 val);
extern void fn_8011DCC4(u8* ptr, u32 arg2, u8 arg3);
extern void fn_8011DD80(u32 arg1, s32 arg2, u8 maxVal);
extern void fn_8011DDFC(void* ctx, u32 p1, u32 value);
extern void fn_8011DE48(u8* ptr, u8 val);
extern void fn_8011DE68(u8* ptr, u16 val);
extern void fn_8011DEA8(u8* ptr, void* src);
extern void fn_8011DEE4(u8* ptr, void* src);
extern void fn_8011DF54(u8* ptr, void* src);
extern u8 fn_8011E048(u8* ptr, u16 idx);
extern u16 fn_8011E078(u8* ptr, u16 idx);
extern void* fn_8011E0F4(u8* ptr);
extern void* fn_8011E128(u8* ptr);
extern u8 fn_8011E2AC(u8* ptr, u16 idx);
extern u8 fn_8011E444(u8* ptr, u16 idx);
extern u8 fn_8011E474(u8* ptr, u16 idx);
extern u16 fn_8011E57C(void* ptr);
extern u16 fn_8011E5A8(void* ptr);
extern u16 fn_8011E5D4(void* ptr);
extern u16 fn_8011E600(void* ptr);
extern u16 fn_8011E62C(void* ptr);
extern u16 fn_8011E658(void* ptr);
extern u16 fn_8011E684(void* ptr);
extern u16 fn_8011E6B0(void* ptr);
extern u16 fn_8011E6DC(void* ptr);
extern u16 fn_8011E708(void* ptr);
extern u16 fn_8011E734(void* ptr);
extern void* fn_8011E778(u16 idx);
extern u8 fn_8011E7A4(u8* ptr);
extern u8 fn_8011E90C(void* ptr);
extern u8 fn_8011E938(void* ptr);
extern u8 fn_8011E964(void* ptr);
extern u8 fn_8011E990(void* ptr);
extern u8 fn_8011E9BC(void* ptr);
extern u8 fn_8011E9E8(void* ptr);
extern u8 fn_8011EA14(void* ptr);
extern u8 fn_8011EA40(void* ptr);
extern u8 fn_8011EA6C(void* ptr);
extern u8 fn_8011EA98(void* ptr);
extern u8 fn_8011EAC4(void* ptr);
extern u8 fn_8011EAF0(void* ptr);
extern u8 fn_8011EB1C(void* ptr);
extern u8 fn_8011EB60(void* ptr);
extern u8 fn_8011EB8C(void* ptr);
extern u8 fn_8011EBB8(void* ptr);
extern u8 fn_8011EBE4(void* ptr);
extern u8 fn_8011EC10(void* ptr);
extern u8 fn_8011EC3C(void* ptr);
extern u8 fn_8011EC68(void* ptr);
extern u8 fn_8011EC94(void* ptr);
extern u8 fn_8011ECC0(void* ptr);
extern u8 fn_8011ECEC(void* ptr);
extern void* fn_8011EDC4(u8* ptr, u16 idx);
extern u16 fn_8011EE70(void* ptr);
extern u16 fn_8011EE9C(void* ptr);
extern u16 fn_8011EEC8(void* ptr);
extern u16 fn_8011EEF4(void* ptr);
extern u16 fn_8011EF20(void* ptr);
extern u16 fn_8011EF4C(void* ptr);
extern u16 fn_8011EF78(void* ptr);
extern u16 fn_8011EFA4(void* ptr);
extern u16 fn_8011EFD0(void* ptr);
extern u16 fn_8011EFFC(void* ptr);
extern u16 fn_8011F028(void* ptr);
extern u16 fn_8011F054(void* ptr);
extern u16 fn_8011F080(void* ptr);
extern u16 fn_8011F0AC(void* ptr);
extern u16 fn_8011F0D8(void* ptr);
extern u16 fn_8011F104(void* ptr);
extern u16 fn_8011F130(void* ptr);
extern u16 fn_8011F15C(void* ptr);
extern u8 fn_8011F1B8(void* ctx, u32 p1);
extern u8 fn_8011F1F0(void* ctx, u32 p1);
extern u16 fn_8011F228(void* ctx, u32 p1);
extern void* fn_8011F474(u8* ptr, u16 idx);
extern void* fn_8011F4D8(void* ptr);
extern void* fn_8011F4F0(void* ptr);
extern void* fn_8011F508(void* ptr);
extern void* fn_8011F598(void* ptr);
extern void fn_8011F5E0(u32* dst, u32* src);
extern void pokemonBiosCopy(u32* dst, u32* src);
extern u32 fn_8011F634(u8* ptr);
extern u32 fn_8011F6D8(u8* ptr);
extern void pokemonSetDp(u8* ptr, f32 f1);
extern f32 pokemonGetDp(u8* ptr);
extern u8 pokemonIsDarkPokemon(u32 arg);
extern void pokemonToMenuPokemonStatusSubBar(u8* ptr, u8* out);
extern void pokemonToMenuWazaStatus(u8* ptr, u8* out);
extern void pokemonGetMezamerupower(u8* ptr, u16* out1, u16* out2);
extern u32 pokemonGetNowLevelToExp(u8* ptr);
extern u32 pokemonGetJoutaiMsgId(u8* ptr);
extern u32 pokemonGetJoutaiMenuSpriteId(u8* ptr);
extern u32 pokemonGetJoutaiDataId(u8* ptr);
extern void fn_80121484(void* obj, u32 arg2, u32 arg3);
extern void fn_801214FC(void* obj, u32 arg2, u32 arg3);
extern void fn_801215E4(void* obj, u32 arg2, u32 arg3);
extern void fn_8012173C(void* obj, u32 arg2, u32 arg3);
extern void fn_801217B4(void* obj, u32 arg2, u32 arg3);
extern void fn_8012190C(void* obj, u32 arg2, u32 arg3);
extern void fn_801219F4(void* obj, u32 arg2, u32 arg3);
extern void fn_80121B4C(void* obj, u32 arg2);
extern void pokemonReplace(u32* arg1, u32* arg2);
extern void* pokemonCreateSequence(void* arg);
extern void pokemonSetSequenceStatus(u8* ptr, void* obj);
extern u8 pokemonIsNokoriHpFollowing(u8* ptr, s32 b);
extern u32 pokemonIsJoutaiKaragenki(u8* ptr);
extern u32 pokemonIsJoutaiNormal(u8* ptr);
extern u16 pokemonGetSoubiItemBuff(u8* ptr);
extern u16 pokemonGetSoubiItemDataId(u32 arg);
extern u32 pokemonDoItemSoubi(u8* ptr, register u32 arg2, u8 flag);
extern u8 pokemonGetSex(u8* ptr);
extern void pokemonSetOnDarkPokemonFlag(u8* ptr, u8 flag);
extern void pokemonSetOnZukanFlag(u8* ptr, u8 flag);
extern u32 pokemonGetOboeWazaDataBanme(u8* ptr, u32 arg2);
extern s32 pokemonOboeWaza(u8* ptr, u8 target, u8* buf_ptr, u8* counter_ptr);
extern u16 pokemonGetOboeWazaDataId(u8* ptr, u8 arg2, u8* counter_ptr);
extern s32 pokemonSearchWazaDataId(u8* ptr, u16 target);
extern void pokemonWazaReplace(void* ptr, u32 idx, u32 arg);
extern void pokemonWazaCreate(u8* ptr, u32 slot, u32 val);
extern void pokemonSetCatchStatus(u8* arg1, u32 arg2, u8 arg3, u16 arg4, u8 arg5, u32 arg6, u32 arg7);
extern u32 pokemonCheckValid(u8* ptr);
extern u16 pokemonGetTokuseiDataId(u8* ptr);
extern void pokemonInitAry(u8* ptr, u16 count);
extern void pokemonInitDarkPokemon(u8* ptr);
extern void pokemonInitJoutai(u8* ptr);
extern void pokemonWazaInit(u8* ptr, u32 arg2);
extern u32 pokemonCheckRare(void* ctx);
extern void pokemonGrowBasisStatus(void* ptr, u32 arg2);
extern void* fn_80128CC0(void* ptr);
extern void* fn_80128CDC(void* ptr);
extern void* fn_80128CF8(void* ptr);
extern void* fn_80128D14(void* ptr);
extern void* fn_80128D30(void* ptr);
extern void* fn_80128D4C(void* ptr);
extern void* fn_80128D68(void* ptr);
extern void* fn_80128D80(void* ptr);
extern void* fn_80128D9C(void* ptr);
extern void* fn_80128DB8(void* ptr);
extern void* fn_80128DD4(void* ptr);
extern void* fn_80128DEC(void* ptr);
extern u32 fn_80128E04(void* ptr);
extern void fn_80128E14(void* ptr);
extern void* fn_80128E2C(void);
extern u32 savedataGetStatus(u8* arg1, u16 arg2);
extern void heroDecPokecoupon(u8* ptr, s32 offset);
extern void heroAddPokecoupon(u8* ptr, s32 offset);
extern void heroDecPokedoru(u8* ptr, u32 offset);
extern void fn_80129514(u8* ptr, s32 arg2, s32 arg3);
extern s32 fn_8012959C(u8* ptr, u32 arg2, u32 arg3, u32 arg4);
extern s32 fn_80129650(u8* ptr, u32 arg2, u32 arg3, u32 arg4);
extern u32 fn_80129718(u8* ptr, u32 arg2);
extern u32 heroHizukiItemGetItemAryPtr(u8* ptr, u16* out_a, u16* out_b, u8* out_c, u8* out_d);
extern void heroCheckSetMonohiroiAllTemotiPokemon(u8* arg1);
extern s32 heroItemCheckAddItemDataId(u8* ptr, u32 arg2);
extern void fn_80129948(u8* arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, u32 arg7);
extern s32 heroItemDecItemDataId(u8* ptr, u32 arg2, u32 arg3, u32 arg4);
extern u32 heroItemCheckHaveItemDataId(u8* ptr, u32 arg2);
extern u32 fn_80129D64(u8* ptr, u8* arg2);
extern s32 heroGetPokemon(u8* ptr, void* buf, u8 flag);
extern s32 heroCatchPokemon(u8* ptr, u8* buf, u32 arg3, u16 arg4, u8 flag);
extern void heroBiosCopy(u32* dst, u32* src);
extern void cbTsureFriend__Fl15FootStepCounterl(void);
extern void heroMoveSetLockFrame(s32 val);
extern void heroMoveAddAutoEvent(u32 a, u32 b, u32 c, u32 d, u32 e);
extern void fn_8012BAF0(u8 type, void* src, u32 val);
extern u32 fn_8012BDE0(u32 r3, u32 r4);
extern u32 heroMoveSetNeckMode(s32 idx, s32 state);
extern u32 heroMoveIsMember(s32 idx);
extern s32 heroMoveDismissMember(s32 idx);
extern void heroPokemonGetCelebi(u8* arg1);
extern void fn_80130770(u8* arg1);
extern void heroPokemonGetHouou(u8* arg1);
extern void fn_801309A0(u32 arg1);
extern void* floorReadScriptPostFunc(u32 a, u32 b);
extern void* floorReadFontPostFunc(u32 a, u32 b);
extern void* floorReadMsgPostFunc(u32 a, u32 b);
extern void* floorReadNormalPreFunc(u32 a, u32 b, u32 size);
extern void* floorDataBiosGetPosListPtr(u8* ptr);
extern void* floorDataBiosGetCharInfo(u8* ptr, u32 idx);
extern u32 floorDataBiosGetCharNum(u8* ptr);
extern u32 floorDataBiosGetPostFunc(u8* ptr);
extern u32 floorDataBiosGetMainFunc(u8* ptr);
extern u32 floorDataBiosGetPreFunc(u8* ptr);
extern u32 fn_80115840(u8* ptr);
extern u32 fn_80115888(u8* ptr);
extern u32 fn_801158D0(u8* ptr);
extern u32 fn_80115918(u8* ptr);
extern u32 fn_80115960(u8* ptr);
extern u32 fn_801159A8(u8* ptr);
extern u32 floorDataBiosGetFloorID(u8* ptr);
extern u8 floorDataBiosGetFloorKind(u8* ptr);
extern u32 floorDataBiosGetMapResID(u8* ptr);
extern u8 floorDataBiosGetArea(u8* ptr);
extern s32 floorEventChangeTresure(u32 index, u16 val, u8 byte);
extern s32 floorCharacterBiosGetRot(void* a, void* b);
extern u32 floorCharacterBiosGetPos(u8* ptr, void* obj);
extern u32 fn_801170A4(u8* arg1, u32 arg2);
extern u8 fn_8011E1D4(u8* ptr, s32 idx);
extern u16 fn_8011E21C(u8* ptr, s32 idx);
extern u32 fn_8011E264(u8* ptr, s32 idx);
extern u16 fn_8011E2DC(u8* ptr, s32 idx);
extern u8 fn_8011E324(u8* ptr, s32 idx);
extern u16 fn_8011E550(u8* ptr);
extern u8 fn_8011E868(u8* ptr);
extern void* fn_8011ED18(u8* ptr);
extern u32 fn_8011ED68(u8* ptr);
extern s32 pokemonGetTasteLike(u8* ptr, void* arg2);
extern s32 pokemonIsHpMantan(u8* ptr);
extern s32 fn_80121574(void* a, u16 b);
extern s32 fn_8012165C(void* a, u16 b);
extern s32 fn_801216CC(void* a, u16 b);
extern s32 fn_8012182C(void* a, u16 b);
extern s32 fn_8012189C(void* a, u16 b);
extern s32 fn_80121984(void* a, u16 b);
extern s32 fn_80121A6C(void* a, u16 b);
extern u8 pokemonGetAnnonKatati(u32 val);
extern u32 pokemonGetLevelToExp(u8* ptr, u8 idx);
extern s32 pokemonGetNowHpPercentage(u8* ptr);
extern u16 pokemonGetNowHpWaruValue(u8* ptr, s32 b);
extern u16 pokemonGetMaxHpWaruValue(u8* ptr, s32 b);
extern u16 pokemonGetSoubiItemSoubiDataId(u8* ptr);
extern void pokemonSetTokuseiFlag(u8* ptr, u32 arg2);
extern u32 heroCheckValid(u8* ptr);
extern void heroMoveGetHeroRot(u32 param);
extern void heroMoveGetHeroPos(u32 param);
extern u32 heroMoveGetResID(u32* out_zero, u32* out_val, s32 index);

/* Note: pokemonGetStatus is the master data-table resolver called with a
 * (context, slot, tableId, flags) signature across ~1769 call sites
 * throughout the codebase (see trainer.c). It is not yet decompiled
 * (missing from this unit); a prior fictional (void)-signature
 * "GSfield_TransitionStateMachine" definition claiming this address
 * has been removed -- the real function takes arguments and its
 * body is unrelated to field transitions. */
/* Note: pokemonSetStatus is the master data-table writer called with a
 * (context, slot, tableId, flags, value) signature across ~350 call
 * sites throughout the codebase (see trainer.c). It is not yet
 * decompiled (missing from this unit); a prior fictional
 * (void)-signature "GSfield_RenderPass" definition claiming this
 * address has been removed -- the real function takes arguments and
 * its body is unrelated to rendering. */
/* Note: pokemonInit (field-object init, 2008 bytes) is implemented and
 * matched at 100% further below in this file. A prior fictional
 * (void)-signature "GSfield_ObjectBatchUpdate" duplicate claiming
 * this address has been removed. */
/* Note: fn_8012CA84 (field movement/heading processor, 2104 bytes) is
 * implemented further below in this file. A prior fictional
 * (void)-signature "GSfield_ProcessTriggers" duplicate claiming this
 * address has been removed. */
/* ==================================================================
 * Small accessor functions -- field object getters
 *
 * The 0x8011E4A4-0x8011E800 range contains many tiny (0x18-0x34 byte)
 * functions that read specific fields from a field object struct.
 * Pattern: null check, then load from fixed offset and return.
 *
 * Example (fn_8011E4D8, 0x18 bytes):
 *   cmplwi r3, 0x0
 *   bne .ok
 *   li r3, 0x0
 *   blr
 *   .ok:
 *   lhz r3, 0x8(r3)    ; read u16 at offset 0x8
 *   blr
 * ================================================================== */
/* fn_8011E4A4 */ u16 GSfield_GetObjAttrU16(void* obj, u16 slot) {
    if (obj == NULL) { return 0; }
    if (slot >= 2) { return 0; }
    return *(u16*)((u8*)obj + slot * 2 + 0x70);
}
/* fn_8011E4D8 */ u16 GSfield_GetObjType(void* obj) {
    if (obj == NULL) { return 0; }
    return *(u16*)((u8*)obj + 0x8);
}
/* fn_8011E4F0 */ u8 GSfield_GetObjSubtype(void* obj) {
    if (obj == NULL) { return 0; }
    return *(u8*)((u8*)obj + 0x2);
}
/* fn_8011E508 */ u8 GSfield_GetObjFlags(void* obj) {
    if (obj == NULL) { return 0; }
    return *(u8*)((u8*)obj + 0x1);
}
/* fn_8011E520 */ u8 GSfield_GetObjState(void* obj) {
    if (obj == NULL) { return 0; }
    return *(u8*)((u8*)obj + 0x0);
}
/* fn_8011E538 */ u16 GSfield_GetObjGroupId(void* obj) {
    if (obj == NULL) { return 0; }
    return *(u16*)((u8*)obj + 0x6);
}
/* fn_8011E550 */ u16 GSfield_GetObjRegionId(void* obj) {
    u8* sub;
    if (obj == NULL) { return 0; }
    sub = (u8*)obj + 0x90;
    if (sub == NULL) { return 0; }
    return *(u16*)(sub + 0xA);
}
/* Address: 0x8011CA9C | Size: 0x1C | Pattern: nullcheck_getter_s8 */
s32 fn_8011CA9C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x0];
}
/* Address: 0x8011CB3C | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011CB3C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x8]);
}
/* Address: 0x8011CB54 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011CB54(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}
/* Address: 0x8011CB98 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CB98(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x1]);
}
/* Address: 0x8011CBB0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CBB0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}
/* Address: 0x8011CC54 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CC54(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x11]);
}
/* Address: 0x8011CC6C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CC6C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x10]);
}
/* Address: 0x8011CC84 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CC84(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xF]);
}
/* Address: 0x8011CC9C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CC9C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xE]);
}
/* Address: 0x8011CCB4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CCB4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xD]);
}
/* Address: 0x8011CCCC | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CCCC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xC]);
}
/* Address: 0x8011CCE4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CCE4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xB]);
}
/* Address: 0x8011CCFC | Size: 0x1C | Pattern: nullcheck_getter_s8 */
s32 fn_8011CCFC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x4];
}
/* Address: 0x8011CD18 | Size: 0x1C | Pattern: nullcheck_getter_s8 */
s32 fn_8011CD18(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x3];
}
/* Address: 0x8011CD34 | Size: 0x1C | Pattern: nullcheck_getter_s8 */
s32 fn_8011CD34(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x2];
}
/* Address: 0x8011CD50 | Size: 0x1C | Pattern: nullcheck_getter_s8 */
s32 fn_8011CD50(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x1];
}
/* Address: 0x8011CD6C | Size: 0x1C | Pattern: nullcheck_getter_s8 */
s32 fn_8011CD6C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x0];
}
/* Address: 0x8011CD88 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CD88(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x9]);
}
/* Address: 0x8011CDA0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CDA0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x8]);
}
/* Address: 0x8011CDB8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CDB8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x7]);
}
/* Address: 0x8011CDD0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CDD0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x6]);
}
/* Address: 0x8011CDE8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CDE8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x5]);
}
/* Address: 0x8011CE00 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011CE00(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x14]);
}
/* Address: 0x8011CEA0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CEA0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x4]) = val;
}
/* Address: 0x8011CEB0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CEB0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x3]) = val;
}
/* Address: 0x8011CEC0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CEC0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x16]) = val;
}
/* Address: 0x8011CF14 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CF14(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x2C]) = val;
}
/* Address: 0x8011CF24 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CF24(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x12]) = val;
}
/* Address: 0x8011CF34 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CF34(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x14]) = val;
}
/* Address: 0x8011CF9C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CF9C(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xE]) = val;
}
/* Address: 0x8011CFAC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CFAC(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x20]) = val;
}
/* Address: 0x8011CFBC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CFBC(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xC]) = val;
}
/* Address: 0x8011CFCC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CFCC(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xA]) = val;
}
/* Address: 0x8011CFDC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CFDC(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x1C]) = val;
}
/* Address: 0x8011D270 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D270(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x8]) = val;
}
/* Address: 0x8011D280 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D280(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x2]) = val;
}
/* Address: 0x8011D290 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D290(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x1]) = val;
}
/* Address: 0x8011D2A0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D2A0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x0]) = val;
}
/* Address: 0x8011D2B0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D2B0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x6]) = val;
}
/* Address: 0x8011D470 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D470(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x18]) = val;
}
/* Address: 0x8011D494 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D494(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xD6]) = val;
}
/* Address: 0x8011D4A4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D4A4(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xD2]) = val;
}
/* Address: 0x8011D4B4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D4B4(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xD4]) = val;
}
/* Address: 0x8011D4C4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D4C4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xD0]) = val;
}
/* Address: 0x8011D4D4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D4D4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xCF]) = val;
}
/* Address: 0x8011D4E4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D4E4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xCE]) = val;
}
/* Address: 0x8011D4F4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D4F4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xCD]) = val;
}
/* Address: 0x8011D56C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D56C(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xCB]) = val;
}
/* Address: 0x8011D57C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D57C(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xCA]) = val;
}
/* Address: 0x8011D760 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D760(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xBC]) = val;
}
/* Address: 0x8011D8F4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D8F4(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xD8]) = val;
}
/* Address: 0x8011DCB4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DCB4(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x88]) = val;
}
/* Address: 0x8011DE38 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DE38(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x74]) = val;
}
/* Address: 0x8011DE88 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DE88(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0xE0]) = val;
}
/* Address: 0x8011DE98 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DE98(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x5C]) = val;
}
/* Address: 0x8011DF90 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DF90(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x14]) = val;
}
/* Address: 0x8011DFA0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DFA0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x10]) = val;
}
/* Address: 0x8011DFB0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DFB0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xF]) = val;
}
/* Address: 0x8011DFC0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DFC0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xE]) = val;
}
/* Address: 0x8011DFD0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DFD0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xC]) = val;
}
/* Address: 0x8011DFE0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DFE0(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x4]) = val;
}
/* Address: 0x8011DFF0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DFF0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x0]) = val;
}
/* Address: 0x8011E000 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E000(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x4]);
}
/* Address: 0x8011E018 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E018(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x3]);
}
/* Address: 0x8011E030 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E030(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x16]);
}
/* Address: 0x8011E0AC | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011E0AC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x2C]);
}
/* Address: 0x8011E0C4 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E0C4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x12]);
}
/* Address: 0x8011E0DC | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E0DC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x14]);
}
/* Address: 0x8011E15C | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E15C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xE]);
}
/* Address: 0x8011E174 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011E174(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x20]);
}
/* Address: 0x8011E18C | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E18C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xC]);
}
/* Address: 0x8011E1A4 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E1A4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xA]);
}
/* Address: 0x8011E1BC | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011E1BC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x1C]);
}
/* Address: 0x8011E4D8 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E4D8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x8]);
}
/* Address: 0x8011E4F0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E4F0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x2]);
}
/* Address: 0x8011E508 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E508(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x1]);
}
/* Address: 0x8011E520 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E520(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}
/* Address: 0x8011E538 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E538(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x6]);
}
/* Address: 0x8011E760 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011E760(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x18]);
}
/* Address: 0x8011E7C0 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E7C0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xD6]);
}
/* Address: 0x8011E7D8 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E7D8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xD2]);
}
/* Address: 0x8011E7F0 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E7F0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xD4]);
}
/* Address: 0x8011E808 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E808(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xD0]);
}
/* Address: 0x8011E820 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E820(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xCF]);
}
/* Address: 0x8011E838 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E838(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xCE]);
}
/* Address: 0x8011E850 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E850(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xCD]);
}
/* Address: 0x8011E8DC | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E8DC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xCB]);
}
/* Address: 0x8011E8F4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E8F4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xCA]);
}
/* Address: 0x8011EB48 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011EB48(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xBC]);
}
/* Address: 0x8011EDF8 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011EDF8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xE4]);
}
/* Address: 0x8011EE10 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011EE10(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xE0]);
}
/* Address: 0x8011EE28 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011EE28(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xDC]);
}
/* Address: 0x8011EE40 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011EE40(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xD8]);
}
/* Address: 0x8011EE58 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011EE58(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xB0]);
}
/* Address: 0x8011F188 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011F188(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x8A]);
}
/* Address: 0x8011F1A0 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011F1A0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x88]);
}
/* Address: 0x8011F45C | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011F45C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x74]);
}
/* Address: 0x8011F4A8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011F4A8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x60]);
}
/* Address: 0x8011F4C0 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011F4C0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x5C]);
}
/* Address: 0x8011F520 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011F520(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x14]);
}
/* Address: 0x8011F538 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011F538(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x10]);
}
/* Address: 0x8011F550 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011F550(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xF]);
}
/* Address: 0x8011F568 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011F568(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xE]);
}
/* Address: 0x8011F580 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011F580(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xC]);
}
/* Address: 0x8011F5B0 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011F5B0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}
/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 2 functions matched
 * =================================================================== */
extern u32 lbl_8047ADB8;
/* Address: 0x8011F5C8 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011F5C8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}
/* 0x8011CA60 | 0x3C */
extern u32 lbl_80478B78;
extern u32 lbl_8035F9A8[];
u32 pokemonNakigoeDataBiosGetDataAddress(u8* ptr) {
    u16 idx;
    if (ptr == NULL) { return 0; }
    idx = *(u16*)(&ptr[0x10]);
    if (idx >= lbl_80478B78) { return 0; }
    return lbl_8035F9A8[idx];
}
/* 0x8011CAB8 | 0x28 */
extern u32 lbl_80478E68;
extern u32 lbl_80478E6C;
#pragma push
#pragma peephole off
void* pokemonDpFilterDataBiosGetPtr(u16 idx) {
    u32* hdr = (u32*)lbl_80478E68;
    if (idx >= hdr[0]) { return NULL; }
    return (u8*)lbl_80478E6C + idx;
}
#pragma pop
/* 0x8011CAE0 | 0x30 */
s8 pokemonFriendFilterDataBiosGetValue(u8* ptr, u8 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 3) { return 0; }
    return (s8)ptr[idx];
}
/* 0x8011CB10 | 0x2C */
extern u32 lbl_80478B70;
extern u8 lbl_8035F988[];
void* fn_8011CB10(u16 idx) {
    if (idx >= lbl_80478B70) { return NULL; }
    return (u8*)lbl_8035F988 + (u32)idx * 3;
}
/* 0x8011CB6C | 0x2C */
extern u32 lbl_80478B68;
extern u8 lbl_8035F5E0[];
void* fn_8011CB6C(u16 idx) {
    if (idx >= lbl_80478B68) { return NULL; }
    return (u8*)lbl_8035F5E0 + (u32)idx * 0xC;
}
/* 0x8011CBC8 | 0x2C */
extern u32 lbl_80478E58;
extern u32 lbl_80478E5C;
#pragma push
#pragma peephole off
void* pokemonSeikakuRateDataBiosGetPtr(u8 idx) {
    u32* hdr = (u32*)lbl_80478E58;
    if (idx >= hdr[0]) { return NULL; }
    return (u8*)lbl_80478E5C + (u32)idx * 2;
}
#pragma pop
/* 0x8011CBF4 | 0x30 */
u8 fn_8011CBF4(u8* ptr, u8 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 7) { return 0; }
    return ptr[idx + 0x1F];
}
/* 0x8011CC24 | 0x30 */
u8 fn_8011CC24(u8* ptr, u8 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 7) { return 0; }
    return ptr[idx + 0x18];
}
/* 0x8011CE18 | 0x2C */
extern u32 lbl_80478E60;
extern u32 lbl_80478E64;
void* fn_8011CE18(u8 idx) {
    u32* hdr = (u32*)lbl_80478E60;
    if (idx >= hdr[0]) { return NULL; }
    return (u8*)lbl_80478E64 + (u32)idx * 0x28;
}
/* 0x8011CE44 | 0x30 */
u32 pokemonGrowDataBiosGetExp(u8* ptr, u8 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 0x65) { return 0; }
    return *(u32*)(&ptr[(u32)idx * 4]);
}
/* 0x8011CE74 | 0x2C */
extern u32 lbl_80478B60;
extern u8 lbl_8035E940[];
void* fn_8011CE74(u8 idx) {
    if (idx >= lbl_80478B60) { return NULL; }
    return (u8*)lbl_8035E940 + (u32)idx * 0x194;
}
/* 0x8011CED0 | 0x20 */
void fn_8011CED0(u8* ptr, u16 idx, u8 val) {
    if (ptr == NULL) { return; }
    if (idx >= 2) { return; }
    ptr[idx + 0x6E] = val;
}
/* 0x8011CEF0 | 0x24 */
void fn_8011CEF0(u8* ptr, u16 idx, u16 val) {
    if (ptr == NULL) { return; }
    if (idx >= 8) { return; }
    ptr += idx * 2;
    *(u16*)(&ptr[0x74]) = val;
}
/* 0x8011CF44 | 0x2C */
extern void _flagSet(u32);
void fn_8011CF44(u8* ptr) {
    if (ptr == NULL) { return; }
    _flagSet(*(u32*)(&ptr[0x28]));
}
/* 0x8011CF70 | 0x2C */
void fn_8011CF70(u8* ptr) {
    if (ptr == NULL) { return; }
    _flagSet(*(u32*)(&ptr[0x24]));
}
/* 0x40 | fn_8011CFEC | compound_indexed_setter */
void fn_8011CFEC(u8* ptr, u16 idx, u8 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 2) {
        sub = NULL;
    } else {
        sub = ptr + idx * 8 + 0x10C;
    }
    if (sub == NULL) { return; }
    *(u8*)(sub + 0x0) = val;
}
/* 0x40 | fn_8011D02C | compound_indexed_setter */
void fn_8011D02C(u8* ptr, u16 idx, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 2) {
        sub = NULL;
    } else {
        sub = ptr + idx * 8 + 0x10C;
    }
    if (sub == NULL) { return; }
    *(u16*)(sub + 0x2) = val;
}
/* 0x40 | fn_8011D06C | compound_indexed_setter */
void fn_8011D06C(u8* ptr, u16 idx, u32 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 2) {
        sub = NULL;
    } else {
        sub = ptr + idx * 8 + 0x10C;
    }
    if (sub == NULL) { return; }
    *(u32*)(sub + 0x4) = val;
}
/* 0x8011D0AC | 0x20 */
void fn_8011D0AC(u8* ptr, u16 idx, u8 val) {
    if (ptr == NULL) { return; }
    if (idx >= 0x3A) { return; }
    ptr[idx + 0x34] = val;
}
/* 0x40 | fn_8011D0CC | compound_indexed_setter */
void fn_8011D0CC(u8* ptr, u16 idx, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 0x14) {
        sub = NULL;
    } else {
        sub = ptr + idx * 4 + 0xBA;
    }
    if (sub == NULL) { return; }
    *(u16*)(sub + 0x2) = val;
}
/* 0x40 | fn_8011D10C | compound_indexed_setter */
void fn_8011D10C(u8* ptr, u16 idx, u8 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 0x14) {
        sub = NULL;
    } else {
        sub = ptr + idx * 4 + 0xBA;
    }
    if (sub == NULL) { return; }
    *(u8*)(sub + 0x0) = val;
}
/* 0x40 | fn_8011D14C | compound_indexed_setter */
void fn_8011D14C(u8* ptr, u16 idx, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 5) {
        sub = NULL;
    } else {
        sub = ptr + idx * 6 + 0x9C;
    }
    if (sub == NULL) { return; }
    *(u16*)(sub + 0x4) = val;
}
/* 0x40 | fn_8011D18C | compound_indexed_setter */
void fn_8011D18C(u8* ptr, u16 idx, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 5) {
        sub = NULL;
    } else {
        sub = ptr + idx * 6 + 0x9C;
    }
    if (sub == NULL) { return; }
    *(u16*)(sub + 0x2) = val;
}
/* 0x40 | fn_8011D1CC | compound_indexed_setter */
void fn_8011D1CC(u8* ptr, u16 idx, u8 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 5) {
        sub = NULL;
    } else {
        sub = ptr + idx * 6 + 0x9C;
    }
    if (sub == NULL) { return; }
    *(u8*)(sub + 0x0) = val;
}
/* 0x8011D20C | 0x20 */
void fn_8011D20C(u8* ptr, u16 idx, u8 val) {
    if (ptr == NULL) { return; }
    if (idx >= 2) { return; }
    ptr[idx + 0x32] = val;
}
/* 0x8011D22C | 0x20 */
void fn_8011D22C(u8* ptr, u16 idx, u8 val) {
    if (ptr == NULL) { return; }
    if (idx >= 2) { return; }
    ptr[idx + 0x30] = val;
}
/* 0x8011D24C | 0x24 */
void fn_8011D24C(u8* ptr, u16 idx, u16 val) {
    if (ptr == NULL) { return; }
    if (idx >= 2) { return; }
    ptr += idx * 2;
    *(u16*)(&ptr[0x70]) = val;
}
/* 0x8011D2C0 | 36 bytes | compound_setter */
void fn_8011D2C0(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0xA) = val;
}
/* 0x8011D2E4 | 36 bytes | compound_setter */
void fn_8011D2E4(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x8) = val;
}
/* 0x8011D308 | 36 bytes | compound_setter */
void fn_8011D308(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x6) = val;
}
/* 0x8011D32C | 36 bytes | compound_setter */
void fn_8011D32C(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x4) = val;
}
/* 0x8011D350 | 36 bytes | compound_setter */
void fn_8011D350(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x2) = val;
}
/* 0x8011D374 | 36 bytes | compound_setter */
void fn_8011D374(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x0) = val;
}
/* 0x8011D398 | 36 bytes | compound_setter */
void fn_8011D398(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0xA) = val;
}
/* 0x8011D3BC | 36 bytes | compound_setter */
void fn_8011D3BC(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x8) = val;
}
/* 0x8011D3E0 | 36 bytes | compound_setter */
void fn_8011D3E0(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x6) = val;
}
/* 0x8011D404 | 36 bytes | compound_setter */
void fn_8011D404(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x4) = val;
}
/* 0x8011D428 | 36 bytes | compound_setter */
void fn_8011D428(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x2) = val;
}
/* 0x8011D44C | 36 bytes | compound_setter */
void fn_8011D44C(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x0) = val;
}
/* 0x8011D480 | 0x14 */
void fn_8011D480(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0xF8]) = (u32)val;
}
/* 0x68 | fn_8011D504 | compound_chained_setter */
void fn_8011D504(u8* ptr, u8 val) {
    u16 idx;
    void* entry;
    u8 check;
    if (ptr == NULL) { return; }
    if (ptr == NULL) {
        idx = 0;
    } else {
        idx = *(u16*)(ptr + 0x0);
    }
    if (idx >= *(u32*)lbl_80478F90) {
        entry = NULL;
    } else {
        entry = (u8*)lbl_80478F94 + (u32)idx * 0x11C;
    }
    if (entry == NULL) {
        check = 0;
    } else {
        check = *(u8*)((u8*)entry + 0x33);
    }
    if (check == 0) {
        val = 0;
    }
    *(u8*)(ptr + 0xCC) = val;
}
/* 0x8011D58C | 36 bytes | compound_setter */
void fn_8011D58C(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0xC) = val;
}
/* 0x8011D5B0 | 36 bytes | compound_setter */
void fn_8011D5B0(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0xB) = val;
}
/* 0x8011D5D4 | 36 bytes | compound_setter */
void fn_8011D5D4(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0xA) = val;
}
/* 0x8011D5F8 | 36 bytes | compound_setter */
void fn_8011D5F8(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x9) = val;
}
/* 0x8011D61C | 36 bytes | compound_setter */
void fn_8011D61C(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x8) = val;
}
/* 0x8011D640 | 36 bytes | compound_setter */
void fn_8011D640(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x7) = val;
}
/* 0x8011D664 | 36 bytes | compound_setter */
void fn_8011D664(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x6) = val;
}
/* 0x8011D688 | 36 bytes | compound_setter */
void fn_8011D688(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x5) = val;
}
/* 0x8011D6AC | 36 bytes | compound_setter */
void fn_8011D6AC(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x4) = val;
}
/* 0x8011D6D0 | 36 bytes | compound_setter */
void fn_8011D6D0(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x3) = val;
}
/* 0x8011D6F4 | 36 bytes | compound_setter */
void fn_8011D6F4(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x2) = val;
}
/* 0x8011D718 | 36 bytes | compound_setter */
void fn_8011D718(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x1) = val;
}
/* 0x8011D73C | 36 bytes | compound_setter */
void fn_8011D73C(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x0) = val;
}
/* 0x8011D770 | 36 bytes | compound_setter */
void fn_8011D770(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x4) = val;
}
/* 0x8011D794 | 36 bytes | compound_setter */
void fn_8011D794(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x3) = val;
}
/* 0x8011D7B8 | 36 bytes | compound_setter */
void fn_8011D7B8(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x2) = val;
}
/* 0x8011D7DC | 36 bytes | compound_setter */
void fn_8011D7DC(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x1) = val;
}
/* 0x8011D800 | 36 bytes | compound_setter */
void fn_8011D800(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x0) = val;
}
/* 0x8011D824 | 36 bytes | compound_setter */
void fn_8011D824(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x4) = val;
}
/* 0x8011D848 | 36 bytes | compound_setter */
void fn_8011D848(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x3) = val;
}
/* 0x8011D86C | 36 bytes | compound_setter */
void fn_8011D86C(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x2) = val;
}
/* 0x8011D890 | 36 bytes | compound_setter */
void fn_8011D890(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x1) = val;
}
/* 0x8011D8B4 | 36 bytes | compound_setter */
void fn_8011D8B4(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x0) = val;
}
/* 0x8011D8D8 | 0x1C */
void fn_8011D8D8(u8* ptr, s32 val) {
    if (ptr == NULL) { return; }
    if (val >= 0x639C) { val = 0x639C; }
    *(u32*)(&ptr[0xDC]) = (u32)val;
}
/* 0x8011D904 | 0x20 */
void fn_8011D904(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    if (val > 0xFF) { val = 0xFF; }
    *(u16*)(&ptr[0xB0]) = val;
}
/* 0x8011D924 | 0x34 */
void fn_8011D924(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0xA4;
    }
    if (sub == NULL) return;
    if (val > 0x1F) { val = 0x1F; }
    *(u16*)(sub + 0xA) = val;
}
/* 0x8011D958 | 0x34 */
void fn_8011D958(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0xA4;
    }
    if (sub == NULL) return;
    if (val > 0x1F) { val = 0x1F; }
    *(u16*)(sub + 0x8) = val;
}
/* 0x8011D98C | 0x34 */
void fn_8011D98C(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0xA4;
    }
    if (sub == NULL) return;
    if (val > 0x1F) { val = 0x1F; }
    *(u16*)(sub + 0x6) = val;
}
/* 0x8011D9C0 | 0x34 */
void fn_8011D9C0(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0xA4;
    }
    if (sub == NULL) return;
    if (val > 0x1F) { val = 0x1F; }
    *(u16*)(sub + 0x4) = val;
}
/* 0x8011D9F4 | 0x34 */
void fn_8011D9F4(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0xA4;
    }
    if (sub == NULL) return;
    if (val > 0x1F) { val = 0x1F; }
    *(u16*)(sub + 0x2) = val;
}
/* 0x8011DA28 | 0x34 */
void fn_8011DA28(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0xA4;
    }
    if (sub == NULL) return;
    if (val > 0x1F) { val = 0x1F; }
    *(u16*)(sub + 0x0) = val;
}
/* 0x8011DA5C | 0x34 */
void fn_8011DA5C(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0x98;
    }
    if (sub == NULL) return;
    if (val > 0xFF) { val = 0xFF; }
    *(u16*)(sub + 0xA) = val;
}
/* 0x8011DA90 | 0x34 */
void fn_8011DA90(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0x98;
    }
    if (sub == NULL) return;
    if (val > 0xFF) { val = 0xFF; }
    *(u16*)(sub + 0x8) = val;
}
/* 0x8011DAC4 | 0x34 */
void fn_8011DAC4(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0x98;
    }
    if (sub == NULL) return;
    if (val > 0xFF) { val = 0xFF; }
    *(u16*)(sub + 0x6) = val;
}
/* 0x8011DAF8 | 0x34 */
void fn_8011DAF8(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0x98;
    }
    if (sub == NULL) return;
    if (val > 0xFF) { val = 0xFF; }
    *(u16*)(sub + 0x4) = val;
}
/* 0x8011DB2C | 0x34 */
void fn_8011DB2C(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0x98;
    }
    if (sub == NULL) return;
    if (val > 0xFF) { val = 0xFF; }
    *(u16*)(sub + 0x2) = val;
}
/* 0x8011DB60 | 0x34 */
void fn_8011DB60(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0x98;
    }
    if (sub == NULL) return;
    if (val > 0xFF) { val = 0xFF; }
    *(u16*)(sub + 0x0) = val;
}
/* 0x8011DB94 | 36 bytes | compound_setter */
void fn_8011DB94(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0xA) = val;
}
/* 0x8011DBB8 | 36 bytes | compound_setter */
void fn_8011DBB8(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x8) = val;
}
/* 0x8011DBDC | 36 bytes | compound_setter */
void fn_8011DBDC(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x6) = val;
}
/* 0x8011DC00 | 36 bytes | compound_setter */
void fn_8011DC00(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x4) = val;
}
/* 0x8011DC24 | 36 bytes | compound_setter */
void fn_8011DC24(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x2) = val;
}
/* 0x8011DC48 | 36 bytes | compound_setter */
void fn_8011DC48(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x0) = val;
}
/* 0x48 | fn_8011DC6C | compound_clamp_setter */
void fn_8011DC6C(u8* ptr, u16 val) {
    void* sub;
    u16 subVal;
    if (ptr == NULL) { return; }
    *(u16*)(ptr + 0x8A) = val;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) {
        subVal = 0;
    } else {
        subVal = *(u16*)((u8*)sub + 0x0);
    }
    if (subVal >= *(u16*)(ptr + 0x8A)) { return; }
    *(u16*)(ptr + 0x8A) = subVal;
}
/* 0x8011DCC4 | 0xBC */
#if 0
asm void fn_8011DCC4(void) {
#include "src/game/gs_field_world_fn_8011DCC4.inc"
}
#else
void fn_8011DCC4(u8* ptr, u32 arg2, u8 arg3) {
    extern u8* fn_8011F260(u8* a, u32 b, u32 c);
    extern u32 pokemonWazaCheckValid(u8* a, u32 b);
    extern void* wazaDataBiosGetPtr(u32 val);
    u8* result;
    result = fn_8011F260(ptr, arg2, 0);
    if (result == NULL) { return; }
    if ((u8)arg3 > 3) { arg3 = 3; }
    if ((u8)pokemonWazaCheckValid(ptr, arg2) == 1) {
        u8* tmp = fn_8011F260(ptr, arg2, 1);
        u32 val = tmp == NULL ? 0 : *(u16*)tmp;
        if ((u8)wazaDataBiosGetPp(wazaDataBiosGetPtr(val)) <= 4) { return; }
    }
    result[3] = arg3;
}
#endif
/* 0x7C | fn_8011DD80 | call_clamp_store */
void fn_8011DD80(u32 arg1, s32 arg2, u8 maxVal) {
    extern void* fn_8011F260();
    extern u8 pokemonWazaGetMaxPP(u8* ptr, s32 idx);
    u8* result;
    u8 val;
    result = fn_8011F260(arg1, arg2, 0);
    if (result == NULL) { return; }
    val = pokemonWazaGetMaxPP((u8*)arg1, arg2);
    if (val < maxVal) {
        maxVal = val;
    }
    *(u8*)(result + 0x2) = maxVal;
}
/* 0x8011DDFC | 0x3C */
void fn_8011DDFC(void* ctx, u32 p1, u32 value) {
    extern void* fn_8011F260();
    void* result = fn_8011F260(ctx, p1, 0);
    if (result != 0) {
        *(u16*)result = value;
    }
}
/* 0x8011DE48 | 0x20 */
void fn_8011DE48(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    if (val > 0x64) { val = 0x64; }
    ptr[0x60] = val;
}
/* 0x8011DE68 | 0x20 */
void fn_8011DE68(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    if (val > 0xFF) { val = 0xFF; }
    *(u16*)(&ptr[0xE4]) = val;
}
/* 0x8011DEA8 | 0x3C */
void fn_8011DEA8(u8* ptr, void* src) {
    extern void GScharLenCpy();
    if (ptr == NULL) { return; }
    if (src == NULL) { return; }
    GScharLenCpy(ptr + 0x44, src, 0xB);
}
/* 0x70 | fn_8011DEE4 | dual_memcpy_setter */
void fn_8011DEE4(u8* ptr, void* src) {
    extern void GScharLenCpy();
    if (ptr == NULL) { return; }
    if (src == NULL) { return; }
    GScharLenCpy(ptr + 0x2E, src, 0xB);
    if (ptr == NULL) { return; }
    if (src == NULL) { return; }
    GScharLenCpy(ptr + 0x44, src, 0xB);
}
/* 0x8011DF54 | 0x3C */
void fn_8011DF54(u8* ptr, void* src) {
    extern void GScharLenCpy();
    if (ptr == NULL) { return; }
    if (src == NULL) { return; }
    GScharLenCpy(ptr + 0x18, src, 0xB);
}
/* 0x8011E048 | 0x30 */
u8 fn_8011E048(u8* ptr, u16 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 2) { return 0; }
    return ptr[idx + 0x6E];
}
/* 0x8011E078 | 0x34 */
u16 fn_8011E078(u8* ptr, u16 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 8) { return 0; }
    ptr += idx * 2;
    return *(u16*)(&ptr[0x74]);
}
/* 0x8011E0F4 | 0x34 */
extern void* fn_801906A0(u32);
void* fn_8011E0F4(u8* ptr) {
    if (ptr == NULL) { return NULL; }
    return fn_801906A0(*(u32*)(&ptr[0x28]));
}
/* 0x8011E128 | 0x34 */
void* fn_8011E128(u8* ptr) {
    if (ptr == NULL) { return NULL; }
    return fn_801906A0(*(u32*)(&ptr[0x24]));
}
/* 0x8011E2AC | 0x30 */
u8 fn_8011E2AC(u8* ptr, u16 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 0x3A) { return 0; }
    return ptr[idx + 0x34];
}
/* 0x8011E444 | 0x30 */
u8 fn_8011E444(u8* ptr, u16 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 2) { return 0; }
    return ptr[idx + 0x32];
}
/* 0x8011E474 | 0x30 */
u8 fn_8011E474(u8* ptr, u16 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 2) { return 0; }
    return ptr[idx + 0x30];
}
/* 0x8011E57C | 44 bytes | compound_getter */
u16 fn_8011E57C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x8);
}
/* 0x8011E5A8 | 44 bytes | compound_getter */
u16 fn_8011E5A8(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x6);
}
/* 0x8011E5D4 | 44 bytes | compound_getter */
u16 fn_8011E5D4(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x4);
}
/* 0x8011E600 | 44 bytes | compound_getter */
u16 fn_8011E600(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x2);
}
/* 0x8011E62C | 44 bytes | compound_getter */
u16 fn_8011E62C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x0);
}
/* 0x8011E658 | 44 bytes | compound_getter */
u16 fn_8011E658(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0xA);
}
/* 0x8011E684 | 44 bytes | compound_getter */
u16 fn_8011E684(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x8);
}
/* 0x8011E6B0 | 44 bytes | compound_getter */
u16 fn_8011E6B0(void* ptr) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x6);
}
/* 0x8011E6DC | 44 bytes | compound_getter */
u16 fn_8011E6DC(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x4);
}
/* 0x8011E708 | 44 bytes | compound_getter */
u16 fn_8011E708(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x2);
}
/* 0x8011E734 | 44 bytes | compound_getter */
u16 fn_8011E734(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x0);
}
/* 0x8011E778 | 0x2C */
extern u32 lbl_80478F90;
extern u32 lbl_80478F94;
void* fn_8011E778(u16 idx) {
    u32* hdr = (u32*)lbl_80478F90;
    if (idx >= hdr[0]) { return NULL; }
    return (u8*)lbl_80478F94 + (u32)idx * 0x11C;
}
/* 0x8011E7A4 | 0x1C */
u8 fn_8011E7A4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (u8)(*(u32*)(&ptr[0xF8]));
}
/* 0x8011E90C | 44 bytes | compound_getter */
u8 fn_8011E90C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0xC);
}
/* 0x8011E938 | 44 bytes | compound_getter */
u8 fn_8011E938(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0xB);
}
/* 0x8011E964 | 44 bytes | compound_getter */
u8 fn_8011E964(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0xA);
}
/* 0x8011E990 | 44 bytes | compound_getter */
u8 fn_8011E990(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x9);
}
/* 0x8011E9BC | 44 bytes | compound_getter */
u8 fn_8011E9BC(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x8);
}
/* 0x8011E9E8 | 44 bytes | compound_getter */
u8 fn_8011E9E8(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x7);
}
/* 0x8011EA14 | 44 bytes | compound_getter */
u8 fn_8011EA14(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x6);
}
/* 0x8011EA40 | 44 bytes | compound_getter */
u8 fn_8011EA40(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x5);
}
/* 0x8011EA6C | 44 bytes | compound_getter */
u8 fn_8011EA6C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x4);
}
/* 0x8011EA98 | 44 bytes | compound_getter */
u8 fn_8011EA98(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x3);
}
/* 0x8011EAC4 | 44 bytes | compound_getter */
u8 fn_8011EAC4(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x2);
}
/* 0x8011EAF0 | 44 bytes | compound_getter */
u8 fn_8011EAF0(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x1);
}
/* 0x8011EB1C | 44 bytes | compound_getter */
u8 fn_8011EB1C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x0);
}
/* 0x8011EB60 | 44 bytes | compound_getter */
u8 fn_8011EB60(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x4);
}
/* 0x8011EB8C | 44 bytes | compound_getter */
u8 fn_8011EB8C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x3);
}
/* 0x8011EBB8 | 44 bytes | compound_getter */
u8 fn_8011EBB8(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x2);
}
/* 0x8011EBE4 | 44 bytes | compound_getter */
u8 fn_8011EBE4(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x1);
}
/* 0x8011EC10 | 44 bytes | compound_getter */
u8 fn_8011EC10(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x0);
}
/* 0x8011EC3C | 44 bytes | compound_getter */
u8 fn_8011EC3C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x4);
}
/* 0x8011EC68 | 44 bytes | compound_getter */
u8 fn_8011EC68(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x3);
}
/* 0x8011EC94 | 44 bytes | compound_getter */
u8 fn_8011EC94(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x2);
}
/* 0x8011ECC0 | 44 bytes | compound_getter */
u8 fn_8011ECC0(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x1);
}
/* 0x8011ECEC | 44 bytes | compound_getter */
u8 fn_8011ECEC(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x0);
}
/* 0x8011EDC4 | 0x34 */
void* fn_8011EDC4(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 1) { return NULL; }
    return ptr + (u32)idx * 16 + 0xE8;
}
/* 0x8011EE70 | 44 bytes | compound_getter */
u16 fn_8011EE70(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xA4;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0xA);
}
/* 0x8011EE9C | 44 bytes | compound_getter */
u16 fn_8011EE9C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xA4;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x8);
}
/* 0x8011EEC8 | 44 bytes | compound_getter */
u16 fn_8011EEC8(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xA4;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x6);
}
/* 0x8011EEF4 | 44 bytes | compound_getter */
u16 fn_8011EEF4(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xA4;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x4);
}
/* 0x8011EF20 | 44 bytes | compound_getter */
u16 fn_8011EF20(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xA4;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x2);
}
/* 0x8011EF4C | 44 bytes | compound_getter */
u16 fn_8011EF4C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xA4;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x0);
}
/* 0x8011EF78 | 44 bytes | compound_getter */
u16 fn_8011EF78(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x98;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0xA);
}
/* 0x8011EFA4 | 44 bytes | compound_getter */
u16 fn_8011EFA4(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x98;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x8);
}
/* 0x8011EFD0 | 44 bytes | compound_getter */
u16 fn_8011EFD0(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x98;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x6);
}
/* 0x8011EFFC | 44 bytes | compound_getter */
u16 fn_8011EFFC(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x98;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x4);
}
/* 0x8011F028 | 44 bytes | compound_getter */
u16 fn_8011F028(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x98;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x2);
}
/* 0x8011F054 | 44 bytes | compound_getter */
u16 fn_8011F054(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x98;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x0);
}
/* 0x8011F080 | 44 bytes | compound_getter */
u16 fn_8011F080(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0xA);
}
/* 0x8011F0AC | 44 bytes | compound_getter */
u16 fn_8011F0AC(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x8);
}
/* 0x8011F0D8 | 44 bytes | compound_getter */
u16 fn_8011F0D8(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x6);
}
/* 0x8011F104 | 44 bytes | compound_getter */
u16 fn_8011F104(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x4);
}
/* 0x8011F130 | 44 bytes | compound_getter */
u16 fn_8011F130(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x2);
}
/* 0x8011F15C | 44 bytes | compound_getter */
u16 fn_8011F15C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x0);
}
/* 0x8011F1B8 | 0x38 */
u8 fn_8011F1B8(void* ctx, u32 p1) {
    extern void* fn_8011F260();
    void* result = fn_8011F260(ctx, p1, 1);
    if (result == 0) return 0;
    return *((u8*)result + 0x3);
}
/* 0x8011F1F0 | 0x38 */
u8 fn_8011F1F0(void* ctx, u32 p1) {
    extern void* fn_8011F260();
    void* result = fn_8011F260(ctx, p1, 1);
    if (result == 0) return 0;
    return *((u8*)result + 0x2);
}
/* 0x8011F228 | 0x38 */
u16 fn_8011F228(void* ctx, u32 p1) {
    extern void* fn_8011F260();
    void* result = fn_8011F260(ctx, p1, 1);
    if (result == 0) return 0;
    return *(u16*)result;
}
/* 0x8011F260 | 0x1FC */
extern u8 lbl_80478B58[4];
extern u8 lbl_80478B5C[4];
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8011F260(void);
/* 0x8011F474 | 0x34 */
void* fn_8011F474(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 1) { return NULL; }
    return ptr + (u32)idx * 16 + 0x64;
}
/* 0x8011F4D8 | 24 bytes | nc_addi_ptr */
void* fn_8011F4D8(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x44;
}
/* 0x8011F4F0 | 24 bytes | nc_addi_ptr */
void* fn_8011F4F0(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x2E;
}
/* 0x8011F508 | 24 bytes | nc_addi_ptr */
void* fn_8011F508(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x18;
}
/* 0x8011F598 | 24 bytes | nc_addi_ptr */
void* fn_8011F598(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x8;
}
/* 0x8011F5E0 | 0x1C */
void fn_8011F5E0(u32* dst, u32* src) {
    if (dst == NULL) { return; }
    if (src == NULL) { return; }
    *dst = *src;
}
#if 0
asm void fn_8011E1D4(void) {
#include "src/game/gs_field_world_fn_8011E1D4.inc"
}
#else
#pragma optimization_level 4
u8 fn_8011E1D4(u8* ptr, s32 idx) {
    u8* elem;

    if (ptr == NULL) {
        elem = NULL;
    } else if ((u16)idx >= 2) {
        elem = NULL;
    } else {
        elem = ptr + 0x10C + ((u16)idx << 3);
    }
    if (elem == NULL) {
        return 0;
    }
    return *(u8*)elem;
}
#endif
#if 0
asm void fn_8011E21C(void) {
#include "src/game/gs_field_world_fn_8011E21C.inc"
}
#else
#pragma optimization_level 4
u16 fn_8011E21C(u8* ptr, s32 idx) {
    u8* elem;

    if (ptr == NULL) {
        elem = NULL;
    } else if ((u16)idx >= 2) {
        elem = NULL;
    } else {
        elem = ptr + 0x10C + ((u16)idx << 3);
    }
    if (elem == NULL) {
        return 0;
    }
    return *(u16*)(elem + 2);
}
#endif
#if 0
asm void fn_8011E264(void) {
#include "src/game/gs_field_world_fn_8011E264.inc"
}
#else
#pragma optimization_level 4
u32 fn_8011E264(u8* ptr, s32 idx) {
    u8* elem;

    if (ptr == NULL) {
        elem = NULL;
    } else if ((u16)idx >= 2) {
        elem = NULL;
    } else {
        elem = ptr + 0x10C + ((u16)idx << 3);
    }
    if (elem == NULL) {
        return 0;
    }
    return *(u32*)(elem + 4);
}
#endif
#if 0
asm void fn_8011E2DC(void) {
#include "src/game/gs_field_world_fn_8011E2DC.inc"
}
#else
#pragma optimization_level 4
u16 fn_8011E2DC(u8* ptr, s32 idx) {
    u8* elem;

    if (ptr == NULL) {
        elem = NULL;
    } else if ((u16)idx >= 0x14) {
        elem = NULL;
    } else {
        elem = ptr + 0xBA + ((u16)idx << 2);
    }
    if (elem == NULL) {
        return 0;
    }
    return *(u16*)(elem + 2);
}
#endif
#if 0
asm void fn_8011E324(void) {
#include "src/game/gs_field_world_fn_8011E324.inc"
}
#else
#pragma optimization_level 4
u8 fn_8011E324(u8* ptr, s32 idx) {
    u8* elem;

    if (ptr == NULL) {
        elem = NULL;
    } else if ((u16)idx >= 0x14) {
        elem = NULL;
    } else {
        elem = ptr + 0xBA + ((u16)idx << 2);
    }
    if (elem == NULL) {
        return 0;
    }
    return *(u8*)elem;
}
#endif
#if 0
asm void fn_8011E36C(void) {
#include "src/game/gs_field_world_fn_8011E36C.inc"
}
#else
#pragma optimization_level 4
u16 fn_8011E36C(u8* ptr, u16 idx) {
    u8* elem;

    if (ptr == NULL) {
        elem = NULL;
    } else if ((u16)idx >= 5) {
        elem = NULL;
    } else {
        elem = ptr + 0x9C + (u16)idx * 6;
    }
    if (elem == NULL) {
        return 0;
    }
    return *(u16*)(elem + 4);
}
#endif
#if 0
asm void fn_8011E3B4(void) {
#include "src/game/gs_field_world_fn_8011E3B4.inc"
}
#else
#pragma optimization_level 4
u16 fn_8011E3B4(u8* ptr, u16 idx) {
    u8* elem;

    if (ptr == NULL) {
        elem = NULL;
    } else if ((u16)idx >= 5) {
        elem = NULL;
    } else {
        elem = ptr + 0x9C + (u16)idx * 6;
    }
    if (elem == NULL) {
        return 0;
    }
    return *(u16*)(elem + 2);
}
#endif
#if 0
asm void fn_8011E3FC(void) {
#include "src/game/gs_field_world_fn_8011E3FC.inc"
}
#else
#pragma optimization_level 4
u8 fn_8011E3FC(u8* ptr, u16 idx) {
    u8* elem;

    if (ptr == NULL) {
        elem = NULL;
    } else if ((u16)idx >= 5) {
        elem = NULL;
    } else {
        elem = ptr + 0x9C + (u16)idx * 6;
    }
    if (elem == NULL) {
        return 0;
    }
    return *(u8*)elem;
}
#endif
#if 0
asm void fn_8011E550(void) {
#include "src/game/gs_field_world_fn_8011E550.inc"
}
#else
#pragma optimization_level 4
u16 fn_8011E550(u8* ptr) {
    u8* sub;

    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0x90;
    }
    if (sub == NULL) {
        return 0;
    }
    return *(u16*)(sub + 0xA);
}
#endif
extern u32 lbl_80478F90;  /* obj header ptr (SDA) */
extern u32 lbl_80478F94;  /* obj data base (SDA) */
#if 0
asm void fn_8011E868(void) {
#include "src/game/gs_field_world_fn_8011E868.inc"
}
#else
#pragma optimization_level 4
u8 fn_8011E868(u8* ptr) {
    u16 idx; u8* entry; u8 flag;
    if (ptr == NULL) { return 0; }
    if (ptr == NULL) { idx = 0; } else { idx = *(u16*)ptr; }
    if ((u32)idx >= *(u32*)lbl_80478F90) {
        entry = NULL;
    } else {
        entry = (u8*)lbl_80478F94 + (u32)idx * 0x11C;
    }
    if (entry == NULL) { flag = 0; } else { flag = entry[0x33]; }
    if ((u8)flag == 0) { return 0; }
    return ptr[0xcc];
}
#endif
#if 0
asm void fn_8011ED18(void) {
#include "src/game/gs_field_world_fn_8011ED18.inc"
}
#else
#pragma optimization_level 4
void* fn_8011ED18(u8* ptr) {
    u32 val;

    if (ptr == NULL) {
        return NULL;
    }
    val = (ptr == NULL) ? 0 : *(u16*)(ptr + 0xD8);
    if ((u16)val == 0) {
        return NULL;
    }
    return fn_801EEEB8(val);
}
#endif
#if 0
asm void fn_8011ED68(void) {
#include "src/game/gs_field_world_fn_8011ED68.inc"
}
#else
#pragma optimization_level 4
u32 fn_8011ED68(u8* ptr) {
    u16 val;
    s32 val2;

    if (ptr == NULL) {
        return 0;
    }
    val = (ptr == NULL) ? 0 : *(u16*)(ptr + 0xD8);
    if ((u16)val == 0) {
        return 0;
    }
    val2 = (ptr == NULL) ? 0 : *(s32*)(ptr + 0xDC);
    if (val2 < 0) {
        return 0;
    }
    return 1;
}
#endif
