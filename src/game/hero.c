/**
 * @file hero.c
 * @brief GSfield world segment -- split from gs_field_world.c.
 *
 * XD source unit: hero / heroBios (hero module)
 * Address range: 0x80128E38 - 0x8012AC9C (~67 functions)
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
extern void fn_80260070(void);
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

/* Address: 0x8012A774 | Size: 0x10 | Pattern: nullcheck_setter */
void heroBiosSetHomePlace(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA81]) = val;
}
/* Address: 0x8012A784 | Size: 0x18 | Pattern: nullcheck_getter */
u8 heroBiosGetHomePlace(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA81]);
}
/* Address: 0x8012A79C | Size: 0x18 | Pattern: nullcheck_getter */
u8 heroBiosGetHizukiFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA98]);
}
/* Address: 0x8012A7B4 | Size: 0x10 | Pattern: nullcheck_setter */
void heroBiosSetHizukiFlag(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA98]) = val;
}
/* Address: 0x8012A7C4 | Size: 0x18 | Pattern: nullcheck_getter */
u32 heroBiosGetPokecouponAll(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xA8C]);
}
/* Address: 0x8012A80C | Size: 0x18 | Pattern: nullcheck_getter */
u32 heroBiosGetPokecoupon(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xA88]);
}
/* Address: 0x8012A854 | Size: 0x18 | Pattern: nullcheck_getter */
u32 heroBiosGetPokedoru(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xA84]);
}
/* Address: 0x8012A8EC | Size: 0x18 | Pattern: nullcheck_getter */
u8 heroBiosGetBadge08Flag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA97]);
}
/* Address: 0x8012A904 | Size: 0x18 | Pattern: nullcheck_getter */
u8 heroBiosGetBadge07Flag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA96]);
}
/* Address: 0x8012A91C | Size: 0x18 | Pattern: nullcheck_getter */
u8 heroBiosGetBadge06Flag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA95]);
}
/* Address: 0x8012A934 | Size: 0x18 | Pattern: nullcheck_getter */
u8 heroBiosGetBadge05Flag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA94]);
}
/* Address: 0x8012A94C | Size: 0x18 | Pattern: nullcheck_getter */
u8 heroBiosGetBadge04Flag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA93]);
}
/* Address: 0x8012A964 | Size: 0x18 | Pattern: nullcheck_getter */
u8 heroBiosGetBadge03Flag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA92]);
}
/* Address: 0x8012A97C | Size: 0x18 | Pattern: nullcheck_getter */
u8 heroBiosGetBadge02Flag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA91]);
}
/* Address: 0x8012A994 | Size: 0x18 | Pattern: nullcheck_getter */
u8 heroBiosGetBadge01Flag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA90]);
}
/* Address: 0x8012A9AC | Size: 0x10 | Pattern: nullcheck_setter */
void heroBiosSetBadge08Flag(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA97]) = val;
}
/* Address: 0x8012A9BC | Size: 0x10 | Pattern: nullcheck_setter */
void heroBiosSetBadge07Flag(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA96]) = val;
}
/* Address: 0x8012A9CC | Size: 0x10 | Pattern: nullcheck_setter */
void heroBiosSetBadge06Flag(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA95]) = val;
}
/* Address: 0x8012A9DC | Size: 0x10 | Pattern: nullcheck_setter */
void heroBiosSetBadge05Flag(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA94]) = val;
}
/* Address: 0x8012A9EC | Size: 0x10 | Pattern: nullcheck_setter */
void heroBiosSetBadge04Flag(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA93]) = val;
}
/* Address: 0x8012A9FC | Size: 0x10 | Pattern: nullcheck_setter */
void heroBiosSetBadge03Flag(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA92]) = val;
}
/* Address: 0x8012AA0C | Size: 0x10 | Pattern: nullcheck_setter */
void heroBiosSetBadge02Flag(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA91]) = val;
}
/* Address: 0x8012AA1C | Size: 0x10 | Pattern: nullcheck_setter */
void heroBiosSetBadge01Flag(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA90]) = val;
}
/* Address: 0x8012AA2C | Size: 0x18 | Pattern: nullcheck_getter */
u8 heroBiosGetSexDataId(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA80]);
}
/* Address: 0x8012AA44 | Size: 0x10 | Pattern: nullcheck_setter */
void heroBiosSetSexDataId(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA80]) = val;
}
/* Address: 0x8012AA54 | Size: 0x10 | Pattern: nullcheck_setter */
void heroBiosSetRnd(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x2C]) = val;
}
/* Address: 0x8012AC3C | Size: 0x18 | Pattern: nullcheck_getter */
u32 heroBiosGetRnd(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x2C]);
}
/* 0x80128E38 | 0x25C */
extern void fn_8013528C(void);
extern void fn_800F9D04(void);
extern void gamedatasaveSetStatus(void);
extern u8 lbl_8047D028[8];
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void savedataCreate(void);
/* 0x80129094 | 0x1EC */
extern void gamedataInit(void);
extern void pcboxInit(void);
extern void fn_801908D4(void);
extern void mailInitMailbox(void);
extern void sodateyaInit(void);
extern void fn_8006B6B4(void);
extern void fn_80260070(void);
extern void fn_80083CBC(void);
extern void fn_801EF128(void);
extern void exribbonInit(void);
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void savedataInit(void);
/* 0x80129280 | 0x104 */
extern void jumptable_803634A8();
#if 0
asm void savedataGetStatus(void) {
#include "src/game/gs_field_world_fn_80129280.inc"
}
#else
u32 savedataGetStatus(u8* arg1, u16 arg2) {
    extern u8* fn_80128E24(u8* ptr);
    extern u32 fn_80128E04(u8* ptr);
    extern u32 fn_80128DEC(u8* ptr);
    extern u32 fn_80128DD4(u8* ptr);
    extern u32 fn_80128DB8(u8* ptr);
    extern u32 fn_80128D9C(u8* ptr);
    extern u32 fn_80128D80(u8* ptr);
    extern u32 fn_80128D68(u8* ptr);
    extern u32 fn_80128D4C(u8* ptr);
    extern u32 fn_80128D30(u8* ptr);
    extern u32 fn_80128D14(u8* ptr);
    extern u32 fn_80128CF8(u8* ptr);
    extern u32 fn_80128CDC(u8* ptr);
    extern u32 fn_80128CC0(u8* ptr);
    if ((u32)arg2 >= 0x11) { return 0; }
    if (arg1 == NULL) {
        arg1 = fn_80128E24(arg1);
        if (arg1 == NULL) { return 0; }
    }
    switch (arg2) {
        case 0x0: return (u32)arg1;
        case 0x1: return fn_80128E04(arg1);
        case 0x2: return fn_80128DEC(arg1);
        case 0x3: return fn_80128DD4(arg1);
        case 0x4: return fn_80128DB8(arg1);
        case 0x5: return fn_80128D9C(arg1);
        case 0x6: return fn_80128D80(arg1);
        case 0x7: return 0x8;
        case 0x8: return 0x20;
        case 0x9: return 0x180;
        case 0xa: return fn_80128D68(arg1);
        case 0xb: return fn_80128D4C(arg1);
        case 0xc: return fn_80128D30(arg1);
        case 0xd: return fn_80128D14(arg1);
        case 0xe: return fn_80128CF8(arg1);
        case 0xf: return fn_80128CDC(arg1);
        case 0x10: return fn_80128CC0(arg1);
        default: return 0;
    }
}
#endif
/* 0x78 | heroDecPokecoupon | multi_call_guarded */
void heroDecPokecoupon(u8* ptr, s32 offset) {
    extern u32 heroGetStatus(u8* ptr, u32 a, u32 b);
    extern void heroSetStatus(u8* ptr, u32 a, u32 b);
    heroSetStatus(ptr, 0xd, heroGetStatus(ptr, 0xd, 0) - offset);
    if (offset <= 0) {
        heroSetStatus(ptr, 0xe, heroGetStatus(ptr, 0xe, 0) - offset);
    }
}
/* 0x78 | heroAddPokecoupon | multi_call_guarded */
void heroAddPokecoupon(u8* ptr, s32 offset) {
    extern u32 heroGetStatus(u8* ptr, u32 a, u32 b);
    extern void heroSetStatus(u8* ptr, u32 a, u32 b);
    u32 val;
    val = heroGetStatus(ptr, 0xd, 0);
    val += offset;
    heroSetStatus(ptr, 0xd, val);
    if (offset >= 0) {
        val = heroGetStatus(ptr, 0xe, 0);
        val += offset;
        heroSetStatus(ptr, 0xe, val);
    }
}
/* 0x50 | heroDecPokedoru | call_sequence */
#if 0
asm void heroDecPokedoru(void) {
#include "src/game/gs_field_world_fn_80129474.inc"
}
#else
void heroDecPokedoru(u8* ptr, u32 offset) {
    extern u32 heroGetStatus(u8* ptr, u32 a, u32 b);
    extern void heroSetStatus(u8* ptr, u32 a, u32 b);
    heroSetStatus(ptr, 0xc, heroGetStatus(ptr, 0xc, 0) - offset);
}
#endif
/* 0x50 | heroAddPokedoru | call_sequence */
#if 0
asm void heroAddPokedoru(void) {
#include "src/game/gs_field_world_fn_801294C4.inc"
}
#else
void heroAddPokedoru(u8* ptr, u32 offset) {
    extern u32 heroGetStatus(u8* ptr, u32 a, u32 b);
    extern void heroSetStatus(u8* ptr, u32 a, u32 b);
    u32 val = heroGetStatus(ptr, 0xc, 0);
    val += offset;
    heroSetStatus(ptr, 0xc, val);
}
#endif
/* 0x80129514 | 0x88 */
extern void fn_80140A9C(void);
#if 0
asm void fn_80129514(void) {
#include "src/game/gs_field_world_fn_80129514.inc"
}
#else
void fn_80129514(u8* ptr, s32 arg2, s32 arg3) {
    extern void* heroGetStatus(u8* a, u32 b, u32 c);
    extern void fn_80140A9C(u8* a, u8* b);
    u16 local;
    u8* val;
    if (&local != NULL) { local = 0xa; }
    val = (u8*)heroGetStatus(ptr, 0xa, 0);
    if (val == NULL) { return; }
    if ((u16)arg2 >= local) { return; }
    if ((u16)arg3 >= local) { return; }
    fn_80140A9C(val + (u16)arg2 * 4, val + (u16)arg3 * 4);
}
#endif
/* 0x8012959C | 0xB4 */
extern void fn_80140ACC(void);
#if 0
asm void fn_8012959C(void) {
#include "src/game/gs_field_world_fn_8012959C.inc"
}
#else
s32 fn_8012959C(u8* ptr, u32 arg2, u32 arg3, u32 arg4) {
    extern void* heroGetStatus(u8* a, u32 b, u32 c);
    extern s32 fn_80140ACC(void* a, u16 b, u32 c, u32 d, u32 e, u16 f, u8 g);
    u16 local_c = 0;
    u16 local_a = 0;
    u8 local_8;
    void* val;
    if (&local_c != NULL) { local_c = 0xa; }
    if (&local_a != NULL) { local_a = 1; }
    if (&local_8 != NULL) { local_8 = 0; }
    val = heroGetStatus(ptr, 0xa, 0);
    if (val == NULL) { return -1; }
    return fn_80140ACC(val, local_c, arg2, arg3, arg4, local_a, local_8);
}
#endif
/* 0x80129650 | 0xC8 */
extern void fn_80141308(void);
#if 0
asm void fn_80129650(void) {
#include "src/game/gs_field_world_fn_80129650.inc"
}
#else
s32 fn_80129650(u8* ptr, u32 arg2, u32 arg3, u32 arg4) {
    extern void* heroGetStatus(u8* a, u32 b, u32 c);
    extern s32 fn_80141308(void* a, u16 b, u32 c, u32 d, u32 e, u16 f, u8 g, u8 h);
    u16 local_c = 0;
    u16 local_a = 0;
    u8 local_9;
    u8 local_8;
    void* val;
    if (&local_c != NULL) { local_c = 0xa; }
    if (&local_a != NULL) { local_a = 1; }
    if (&local_9 != NULL) { local_9 = 0; }
    if (&local_8 != NULL) { local_8 = 1; }
    val = heroGetStatus(ptr, 0xa, 0);
    if (val == NULL) { return -1; }
    return fn_80141308(val, local_c, arg2, arg3, arg4, local_a, local_9, local_8);
}
#endif
/* 0x80129718 | 0xC0 */
extern void fn_80142368(void);
#if 0
asm void fn_80129718(void) {
#include "src/game/gs_field_world_fn_80129718.inc"
}
#else
u32 fn_80129718(u8* ptr, u32 arg2) {
    extern void* heroGetStatus(u8* a, u32 b, u32 c);
    extern s32 fn_80142368(void* a, u16 b, u32 c, u32 d, u16 e);
    u16 local_a = 0;
    u16 local_8 = 0;
    void* val;
    if (&local_a != NULL) { local_a = 0xa; }
    if (&local_8 != NULL) { local_8 = 1; }
    val = heroGetStatus(ptr, 0xa, 0);
    if (val == NULL) { return 0; }
    if ((u32)fn_80142368(val, local_a, arg2, 1, local_8) != 0) { return 1; }
    return fn_80142368(val, local_a, arg2, 2, local_8) != 0;
}
#endif
/* 0x68 | heroHizukiItemGetItemAryPtr | guarded_call */
#if 0
asm void heroHizukiItemGetItemAryPtr(void) {
#include "src/game/gs_field_world_fn_801297D8.inc"
}
#else
u32 heroHizukiItemGetItemAryPtr(u8* ptr, u16* out_a, u16* out_b, u8* out_c, u8* out_d) {
    extern u32 heroGetStatus(u8* ptr, u32 a, u32 b);
    if (out_a != NULL) { *out_a = 0xa; }
    if (out_b != NULL) { *out_b = 1; }
    if (out_c != NULL) { *out_c = 0; }
    if (out_d != NULL) { *out_d = 1; }
    return heroGetStatus(ptr, 0xa, 0);
}
#endif
/* 0x78 | heroCheckSetMonohiroiAllTemotiPokemon | generic */
void heroCheckSetMonohiroiAllTemotiPokemon(u8* arg1) {
    extern u32 heroGetStatus(u8* ptr, u32 a, u32 b);
    extern u8 pokemonCheckValid(u32 a);
    extern void pokemonCheckSetMonohiroi(u32 a);
    u32 result;
    u32 i;
    for (i = 0; (u16)i < 6; i++) {
        result = heroGetStatus(arg1, 3, i);
        if (pokemonCheckValid(result)) {
            pokemonCheckSetMonohiroi(result);
        }
    }
}
/* 0x801298B8 | 0x90 */
extern void fn_80140588(void);
#if 0
asm void heroItemCheckAddItemDataId(void) {
#include "src/game/gs_field_world_fn_801298B8.inc"
}
#else
s32 heroItemCheckAddItemDataId(u8* ptr, u32 arg2) {
    extern u32 itemGetStatus(u32 a, u32 b, u32 c, u32 d);
    extern void* heroItemGetItemKindToItemAryPtr(u8* ptr, u8 a, u16* b, u16* c, u32 d, u8* e);
    extern s32 fn_80140588(void* a, u16 b, u32 c, u16 d, u8 e);
    u16 local_c = 0;
    u16 local_a = 0;
    u8 local_8;
    void* result;
    result = heroItemGetItemKindToItemAryPtr(ptr, (u8)itemGetStatus(0, arg2, 2, 0), &local_c, &local_a, 0, &local_8);
    if (result == NULL) { return -1; }
    return fn_80140588(result, local_c, arg2, local_a, local_8);
}
#endif
/* 0x80 | fn_80129948 | generic */
void fn_80129948(u8* arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, u32 arg7) {
    extern void* heroItemGetItemKindToItemAryPtr(u8* a, u32 b, u16* c, u32 d, u32 e, u32 f);
    extern void fn_80140A9C(u8* a, u8* b);
    u16 local_8;
    u8* result;
    result = (u8*)heroItemGetItemKindToItemAryPtr(arg1, arg2, &local_8, 0, 0, 0);
    if (result == NULL) { return; }
    if ((u16)arg3 >= local_8) { return; }
    if ((u16)arg4 >= local_8) { return; }
    fn_80140A9C(result + (u16)arg3 * 4, result + (u16)arg4 * 4);
}
/* 0x801299C8 | 0xB0 */
#if 0
asm void heroItemDecItemDataId(void) {
#include "src/game/gs_field_world_fn_801299C8.inc"
}
#else
s32 heroItemDecItemDataId(u8* ptr, u32 arg2, u32 arg3, u32 arg4) {
    extern u32 itemGetStatus(u32 a, u32 b, u32 c, u32 d);
    extern void* heroItemGetItemKindToItemAryPtr(u8* a, u8 b, u16* c, u16* d, u8* e, u32 f);
    extern s32 fn_80140ACC(void* a, u16 b, u32 c, u32 d, u32 e, u16 f, u8 g);
    u16 local_c = 0;
    u16 local_a = 0;
    u8 local_8;
    u8 tmp;
    void* result;
    tmp = (u8)itemGetStatus(0, arg2, 2, 0);
    result = heroItemGetItemKindToItemAryPtr(ptr, tmp, &local_c, &local_a, &local_8, 0);
    if (result == NULL) { return -1; }
    return fn_80140ACC(result, local_c, arg2, arg3, arg4, local_a, local_8);
}
#endif
/* 0x80129A78 | 0xB4 */
#if 0
asm void heroItemAddItemDataId(void) {
#include "src/game/gs_field_world_fn_80129A78.inc"
}
#else
s32 heroItemAddItemDataId(u8* ptr, u32 arg2, u32 arg3, u32 arg4) {
    extern u32 itemGetStatus(u32 a, u32 b, u32 c, u32 d);
    extern void* heroItemGetItemKindToItemAryPtr(u8* a, u8 b, u16* c, u16* d, u8* e, u8* f);
    extern s32 fn_80141308(void* a, u16 b, u32 c, u32 d, u32 e, u16 f, u8 g, u8 h);
    u16 local_c = 0;
    u16 local_a = 0;
    u8 local_9;
    u8 local_8;
    u8 tmp;
    void* result;
    tmp = (u8)itemGetStatus(0, arg2, 2, 0);
    result = heroItemGetItemKindToItemAryPtr(ptr, tmp, &local_c, &local_a, &local_9, &local_8);
    if (result == NULL) { return -1; }
    return fn_80141308(result, local_c, arg2, arg3, arg4, local_a, local_9, local_8);
}
#endif
/* 0x80129B2C | 0x9C */
#if 0
asm void heroItemCheckHaveItemDataId(void) {
#include "src/game/gs_field_world_fn_80129B2C.inc"
}
#else
u32 heroItemCheckHaveItemDataId(u8* ptr, u32 arg2) {
    extern u32 itemGetStatus(u32 a, u32 b, u32 c, u32 d);
    extern void* heroItemGetItemKindToItemAryPtr(u8* ptr, u8 a, u16* b, u16* c, u32 d, u32 e);
    extern s32 fn_80142368(void* a, u16 b, u32 c, u32 d, u16 e);
    u16 local_a = 0;
    u16 local_8 = 0;
    void* result;
    result = heroItemGetItemKindToItemAryPtr(ptr, (u8)itemGetStatus(0, arg2, 2, 0), &local_a, &local_8, 0, 0);
    if (result == NULL) { return 0; }
    return fn_80142368(result, local_a, arg2, 0, local_8) != 0;
}
#endif
/* 0x80129BC8 | 0x19C */
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void heroItemGetItemKindToItemAryPtr(void);
/* 0x80129D64 | 0xBC */
#if 0
asm void fn_80129D64(void) {
#include "src/game/gs_field_world_fn_80129D64.inc"
}
#else
u32 fn_80129D64(u8* ptr, u8* arg2) {
    extern u32 heroGetStatus(u8* a, u32 b, u32 c);
    extern u32 pokemonGetStatus(u8* a, u32 b, u32 c, u32 d);
    extern u32 fn_800F9EE4(u32 a, u32 b);
    u32 val1, temp, val2, result2;
    val1 = heroGetStatus(ptr, 2, 0);
    temp = heroGetStatus(ptr, 1, 0);
    val2 = pokemonGetStatus(arg2, 0, 0x75, 0);
    result2 = pokemonGetStatus(arg2, 0, 0x76, 0);
    if (val1 != val2) { return 0; }
    return fn_800F9EE4(temp, result2) == 0;
}
#endif
/* 0x80129E20 | 0x100 */
extern void fn_80134BC0(void);
#if 0
asm void heroGetPokemon(void) {
#include "src/game/gs_field_world_fn_80129E20.inc"
}
#else
s32 heroGetPokemon(u8* ptr, void* buf, u8 flag) {
    extern void* heroGetStatus(u8* a, u32 b, u32 c);
    extern u32 pokemonCheckValid(void* val);
    extern void pokemonBiosCopy(void* a, void* b);
    extern u32 fn_80134BC0(u32 a, void* b, s32 c);
    u8 local_buf[0x138];
    u8 i;
    s16 ret;
    void* val;

    if (buf == NULL) { return 6; }
    pokemonBiosCopy(local_buf, buf);
    if (&local_buf == NULL) { i = 6; goto after_loop; }
    for (i = 0; i < 6; i++) {
        val = heroGetStatus(ptr, 3, i);
        if ((u8)pokemonCheckValid(val) != 1) {
            pokemonBiosCopy(val, local_buf);
            goto after_loop;
        }
    }
    i = 6;
after_loop:
    ret = (s16)(u8)i;
    if ((u8)i >= 6) {
        if ((u8)flag == 0) { return -2; }
        return (fn_80134BC0(0, local_buf, -1) == 1) ? -1 : -2;
    }
    return ret;
}
#endif
/* 0x80129F20 | 0x16C */
#if 0
asm void heroCatchPokemon(void) {
#include "src/game/gs_field_world_fn_80129F20.inc"
}
#else
s32 heroCatchPokemon(u8* ptr, u8* buf, u32 arg3, u16 arg4, u8 flag) {
    extern u32 pokemonGetStatus(u8* a, u32 b, u32 c, u32 d);
    extern u32 heroGetStatus(u8* a, u32 b, u32 c);
    extern u32 pokemonCheckValid(u32 val);
    extern void pokemonBiosCopy(void* a, void* b);
    extern void pokemonSetCatchStatus(u8* a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g);
    extern u32 fn_80134BC0(u32 a, void* b, s32 c);
    u8 local_buf[0x138];
    u8 field7a;
    u8 status;
    u32 val2;
    u32 val1;
    u8 i;
    void* slot;

    if (buf == NULL) { return 6; }
    field7a = (u8)pokemonGetStatus(buf, 0, 0x7a, 0);
    status = (u8)heroGetStatus(ptr, 0xb, 0);
    val2 = heroGetStatus(ptr, 2, 0);
    val1 = heroGetStatus(ptr, 1, 0);
    pokemonBiosCopy(local_buf, buf);
    pokemonSetCatchStatus(local_buf, arg3, field7a, arg4, status, val2, val1);

    if (&local_buf == NULL) {
        i = 6;
    } else {
        i = 0;
        while ((u8)i < 6) {
            slot = (void*)heroGetStatus(ptr, 3, (u8)i);
            if ((u8)pokemonCheckValid((u32)slot) != 1) {
                pokemonBiosCopy(slot, local_buf);
                break;
            }
            i++;
        }
        if ((u8)i >= 6) {
            i = 6;
        }
    }
    if ((u8)i >= 6) {
        if ((u8)flag == 0) { return -2; }
        return (fn_80134BC0(0, local_buf, -1) == 1) ? -1 : -2;
    }
    return (s16)(u8)i;
}
#endif
/* 0x8012A08C | 0xA4 */
#if 0
asm void heroAddPokemon(void) {
#include "src/game/gs_field_world_fn_8012A08C.inc"
}
#else
u32 heroAddPokemon(u8* ptr, void* arg2) {
    extern u32 heroGetStatus(u8* a, u32 b, u32 c);
    extern u32 pokemonCheckValid(u32 val);
    extern void pokemonBiosCopy(u32 a, void* b);
    u32 val;
    u32 i;
    if (arg2 == NULL) { return 6; }
    i = 0;
    while ((u8)i < 6) {
        val = heroGetStatus(ptr, 3, i & 0xFF);
        if ((u8)pokemonCheckValid(val) != 1) {
            pokemonBiosCopy(val, arg2);
            return i;
        }
        i++;
    }
    return 6;
}
#endif
/* 0x8012A1A4 | 0xA4 */
#if 0
asm void heroCreate(void) {
#include "src/game/gs_field_world_fn_8012A1A4.inc"
}
#else
void heroCreate(u8* ptr, u32 arg2, u8 arg3) {
    extern void heroInit(u8* ptr);
    extern u32 fn_800E0C54(void);
    extern void heroSetStatus(u8* ptr, u32 a, u32 b);
    extern u32 fn_800FA280(u32 val);
    u32 lo;
    u32 hi;
    heroInit(ptr);
    lo = fn_800E0C54() & 0xFFFF;
    hi = fn_800E0C54() << 16;
    heroSetStatus(ptr, 2, hi | lo);
    heroSetStatus(ptr, 1, arg2);
    heroSetStatus(ptr, 0xb, arg3);
    heroSetStatus(ptr, 0x17, fn_800FA280(0xfa2));
}
#endif
/* 0x8012A248 | 0x208 */
extern void fn_80142A88(void);
#if 0
asm void heroInit(void) {
#include "src/game/gs_field_world_fn_8012A248.inc"
}
#else
void heroInit(u8* ptr) {
    extern void heroSetStatus(u8* p, u32 a, u32 b);
    extern u8* heroGetStatus(u8* p, u32 a, u32 b);
    extern void pokemonInitAry(u8* p, u16 count);
    extern void fn_80142A88(u8* p, u32 v);
    u16 local = 0;

    heroSetStatus(ptr, 1, (u32)&local);
    heroSetStatus(ptr, 2, 0);
    pokemonInitAry(heroGetStatus(ptr, 3, 0), 6);
    fn_80142A88(heroGetStatus(ptr, 4, 0), 0x14);
    fn_80142A88(heroGetStatus(ptr, 5, 0), 0x2b);
    fn_80142A88(heroGetStatus(ptr, 6, 0), 0x10);
    fn_80142A88(heroGetStatus(ptr, 7, 0), 0x40);
    fn_80142A88(heroGetStatus(ptr, 8, 0), 0x2e);
    fn_80142A88(heroGetStatus(ptr, 9, 0), 0x3);
    heroSetStatus(ptr, 0xb, 2);
    heroSetStatus(ptr, 0xc, 0);
    heroSetStatus(ptr, 0xd, 0);
    heroSetStatus(ptr, 0xe, 0);
    heroSetStatus(ptr, 0xf, 1);
    heroSetStatus(ptr, 0x10, 1);
    heroSetStatus(ptr, 0x11, 1);
    heroSetStatus(ptr, 0x12, 1);
    heroSetStatus(ptr, 0x13, 1);
    heroSetStatus(ptr, 0x14, 1);
    heroSetStatus(ptr, 0x15, 1);
    heroSetStatus(ptr, 0x16, 1);
    heroSetStatus(ptr, 0x17, (u32)&local);
    heroSetStatus(ptr, 0x18, 0);
    fn_80142A88(heroGetStatus(ptr, 0xa, 0), 0xa);
    heroSetStatus(ptr, 0x19, 0);
}
#endif
/* 0x8012A450 | 0x160 */
extern void jumptable_803634F0();
#if 0
asm void heroSetStatus(void) {
#include "src/game/gs_field_world_fn_8012A450.inc"
}
#else
void heroSetStatus(u8* ptr, u32 selector, u32 value) {
    extern u32 savedataGetStatus(u8*, u16);
    u16 sel = (u16)selector;

    if (sel == 0) { return; }
    if (sel >= 0x1A) { return; }

    if (ptr == NULL) {
        ptr = (u8*)savedataGetStatus(NULL, 0);
        if (ptr == NULL) { return; }
        ptr = (u8*)savedataGetStatus(ptr, 2);
        if (ptr == NULL) { return; }
    }

    switch (sel) {
    case 1:
        heroBiosSetNamePtr(ptr, (void*)value);
        break;
    case 2:
        heroBiosSetRnd(ptr, value);
        break;
    case 3:
        heroBiosSetSexDataId(ptr, (u8)value);
        break;
    case 4:
        heroBiosSetPokedoru(ptr, value);
        break;
    case 5:
        heroBiosSetPokecoupon(ptr, value);
        break;
    case 6:
        heroBiosSetPokecouponAll(ptr, value);
        break;
    case 7:
        heroBiosSetBadge01Flag(ptr, (u8)value);
        break;
    case 8:
        heroBiosSetBadge02Flag(ptr, (u8)value);
        break;
    case 9:
        heroBiosSetBadge03Flag(ptr, (u8)value);
        break;
    case 10:
        heroBiosSetBadge04Flag(ptr, (u8)value);
        break;
    case 11:
        heroBiosSetBadge05Flag(ptr, (u8)value);
        break;
    case 12:
        heroBiosSetBadge06Flag(ptr, (u8)value);
        break;
    case 13:
        heroBiosSetBadge07Flag(ptr, (u8)value);
        break;
    case 14:
        heroBiosSetBadge08Flag(ptr, (u8)value);
        break;
    case 15:
        heroBiosSetHizukiNamePtr(ptr, (void*)value);
        break;
    case 16:
        heroBiosSetHizukiFlag(ptr, (u8)value);
        break;
    case 17:
        heroBiosSetHomePlace(ptr, (u8)value);
        break;
    }
}
#endif
/* 0x8012A5B0 | 0x1C4 */
extern void jumptable_80363558();
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void heroGetStatus(void);
/* 0x8012A7DC | 0x30 */
void heroBiosSetPokecouponAll(u8* ptr, s32 val) {
    if (ptr == NULL) { return; }
    if (val < 0) { val = 0; }
    if (val > 0x98967F) { val = 0x98967F; }
    *(s32*)(&ptr[0xA8C]) = val;
}
/* 0x8012A824 | 0x30 */
void heroBiosSetPokecoupon(u8* ptr, s32 val) {
    if (ptr == NULL) { return; }
    if (val < 0) { val = 0; }
    if (val > 0x98967F) { val = 0x98967F; }
    *(s32*)(&ptr[0xA88]) = val;
}
/* 0x8012A86C | 0x30 */
void heroBiosSetPokedoru(u8* ptr, s32 val) {
    if (ptr == NULL) { return; }
    if (val < 0) { val = 0; }
    if (val > 0x98967F) { val = 0x98967F; }
    *(s32*)(&ptr[0xA84]) = val;
}
/* 0x8012A89C | 0x38 */
void heroBiosSetHizukiNamePtr(u8* ptr, void* src) {
    if (ptr == NULL) { return; }
    if (src == NULL) { return; }
    GScharLenCpy(ptr + 0xAC2, src, 0xB);
}
/* 0x8012A8D4 | 24 bytes | nc_addi_ptr */
void* heroBiosGetHizukiNamePtr(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0xAC2;
}
/* 0x8012AA64 | 0x38 */
void heroBiosSetNamePtr(void* dst, void* src) {
    extern void GScharLenCpy();
    if (dst == NULL) { return; }
    if (src == NULL) { return; }
    GScharLenCpy(dst, src, 0xB);
}
/* 0x8012AA9C | 0x34 */
void* heroBiosGetHizukiItemPtr(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 0xA) { return NULL; }
    return ptr + (u32)idx * 4 + 0xA9A;
}
/* 0x8012AAD0 | 0x34 */
void* heroBiosGetItemKoronPtr(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 0x3) { return NULL; }
    return ptr + (u32)idx * 4 + 0xA74;
}
/* 0x8012AB04 | 0x34 */
void* heroBiosGetItemSeedPtr(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 0x2E) { return NULL; }
    return ptr + (u32)idx * 4 + 0x9BC;
}
/* 0x8012AB38 | 0x34 */
void* heroBiosGetItemSkillPtr(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 0x40) { return NULL; }
    return ptr + (u32)idx * 4 + 0x8BC;
}
/* 0x8012AB6C | 0x34 */
void* heroBiosGetItemBallPtr(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 0x10) { return NULL; }
    return ptr + (u32)idx * 4 + 0x87C;
}
/* 0x8012ABA0 | 0x34 */
void* heroBiosGetExtraItemPtr(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 0x2B) { return NULL; }
    return ptr + (u32)idx * 4 + 0x7D0;
}
/* 0x8012ABD4 | 0x34 */
void* heroBiosGetItemNormalPtr(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 0x14) { return NULL; }
    return ptr + (u32)idx * 4 + 0x780;
}
/* 0x8012AC08 | 0x34 */
void* heroBiosGetPokemonPtr(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 6) { return NULL; }
    return ptr + (u32)idx * 0x138 + 0x30;
}
/* 0x8012AC54 | 16 bytes | nc_bnelr */
u32 heroBiosGetNamePtr(void* ptr) {
    if (ptr != NULL) { return (u32)ptr; }
    return 0;
}
/* 0x8012AC64 | 0x38 */
#ifndef PCPORT
typedef struct { u32 data[0x2C6]; } GfwBuf0xB18;
#endif
void heroBiosCopy(u32* dst, u32* src) {
#ifdef PCPORT
    u32 i;
#endif
    if (dst == NULL) { return; }
    if (src == NULL) { return; }
#ifdef PCPORT
    for (i = 0; i < 0x2C6; i++) {
        dst[i] = src[i];
    }
#else
    *(GfwBuf0xB18*)dst = *(GfwBuf0xB18*)src;
#endif
}
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
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void pokemonSetStatus(void);
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
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
u32 pokemonGetStatus(void);
#if 0
asm void heroCheckValid(void) {
#include "src/game/gs_field_world_fn_8012A130.inc"
}
#else
u32 heroCheckValid(u8* ptr) {
    extern u32 heroGetStatus(u8* ptr, u32 a, u32 b);
    extern s32 GScharCmp(u32 val, u16* out);
    u16 local = 0;
    if (GScharCmp(heroGetStatus(ptr, 1, 0), &local) == 0) { return 0; }
    return heroGetStatus(ptr, 0xb, 0) != 2;
}
#endif
