/**
 * @file gba_misc.c
 * @brief GBA miscellaneous communication support (0x800895A4-0x80092C90)
 *
 * Address range: 0x800895A4 - 0x80092C90
 * Total functions: 69
 */

#include "dolphin/types.h"

typedef struct GbaMiscContext {
    u8 unk_0000[0x4000];
    u8 state_4000;       /* 0x4000 */
    u8 unk_4001[0x135];
    u8 tableKey_4136;    /* 0x4136 */
} GbaMiscContext;

/*
 * Source-level names only: the macros preserve the original fn_* linker
 * symbols for objdiff while documenting the recovered behavior.
 */
#define GbaMisc_GetMappedContextByte fn_80089B8C
#define GbaMisc_HasActiveContextState fn_80089C10
#define GbaMisc_PollEntryStatusA fn_80089CA8
#define GbaMisc_ResetEntryStatusA fn_80089D30
#define GbaMisc_GetEntryStatus fn_8008A9E4
#define GbaMisc_SendPackedEntryStatus fn_8008AB20
#define GbaMisc_SetEntryState gbaCommandSetKeyState
#define GbaMisc_RunFlagDispatch fn_8008C700

#define GBA_MISC_ENTRY_WORD_OFFSET(idx) ((idx) << 2)
#define GBA_MISC_ENTRY_HALF_OFFSET(idx) ((idx) << 1)
#define GbaMisc_EntryStateAtWordOffset(offset) \
    (*(s32*)((u8*)&lbl_803FB318 + (offset) + (-4)))
#define GbaMisc_EntryCachedStatusAtWordOffset(offset) \
    (*(s32*)((u8*)&lbl_803FB308 + (offset) + (-4)))
#define GbaMisc_EntryCounterAAtHalfOffset(offset) \
    (*(u16*)((u8*)&lbl_8047A684 + (offset) + (-2)))
#define GbaMisc_EntryCounterBAtHalfOffset(offset) \
    (*(u16*)((u8*)&lbl_8047A67C + (offset) + (-2)))
#define GbaMisc_EntryState(idx) \
    GbaMisc_EntryStateAtWordOffset(GBA_MISC_ENTRY_WORD_OFFSET(idx))
#define GbaMisc_EntryCachedStatus(idx) \
    GbaMisc_EntryCachedStatusAtWordOffset(GBA_MISC_ENTRY_WORD_OFFSET(idx))
#define GbaMisc_EntryCounterA(idx) \
    GbaMisc_EntryCounterAAtHalfOffset(GBA_MISC_ENTRY_HALF_OFFSET(idx))
#define GbaMisc_EntryCounterB(idx) \
    GbaMisc_EntryCounterBAtHalfOffset(GBA_MISC_ENTRY_HALF_OFFSET(idx))

/* ===== External function declarations ===== */
extern void fn_8001E184();
extern void fn_80071700();
extern void fn_800719A8();
extern s32 fn_80071AE4();
extern void fn_800722A0();
extern void fn_80072548();
extern s32 fn_800726A8();
extern void fn_80072A00();
extern void fn_80072C74();
extern void fn_80072D58();
extern s32 _AGB_EntryGetStatus__FlPUl(s32, u32*);
extern void fn_800730F8();
extern void fn_800733D0();
extern void fn_80073990();
extern void fn_80073A44();
extern void fn_800830A4();
extern void fn_80083BF8();
extern GbaMiscContext* fn_80083CFC();
extern void fn_80083D30();
extern void fn_80083ECC();
extern void __cvt_fp2unsigned();
extern void memmove();
extern void fn_800D3088();
extern void fn_800D37CC();
extern void GSmodelSetGSparticleLinkAttachMode();
extern void GSmodelLinkToGSparticleBank();
extern void GSmodelSetShadowTextureSize();
extern void GSmodelSetShadowLight();
extern void GSmodelSetShadowSurface();
extern void GSmodelSetShadowFlags();
extern void GSmodelGetFrameCount();
extern void GSmodelStartAnimation();
extern void GSmodelSetAnimFrame();
extern void GSmodelSetAnimType();
extern void GSmodelSetAnimIndex();
extern void _threadSwitch();
extern void GSresGetResource();
extern void fn_800F9AEC();
extern void fn_800F9C04();
extern void fn_800FF58C();
extern void fn_8011288C();
extern void fn_80113F48();
extern void fn_80118874();
extern void pokemonBiosSetEventGetFlag();
extern void pokemonBiosSetFightTrainerPokemonDataId();
extern void pokemonBiosSetPara1Amari();
extern void pokemonBiosSetAmari();
extern void pokemonBiosSetMailId();
extern void pokemonBiosSetPcboxMark();
extern void pokemonBiosSetFlagAmari();
extern void pokemonBiosSetFuseiFlag();
extern void pokemonBiosSetTokuseiFlag();
extern void pokemonBiosSetTamagoFlag();
extern void pokemonBiosSetPokerus();
extern void pokemonBiosSetAmariRibbon();
extern void pokemonBiosSetWorldRibbon();
extern void pokemonBiosSetEarthRibbon();
extern void pokemonBiosSetNationalRibbon();
extern void pokemonBiosSetCountryRibbon();
extern void pokemonBiosSetSkyRibbon();
/* ... and 210 more external functions */
extern void* memset(void* dst, int val, u32 size);
extern void* memcpy(void* dst, const void* src, u32 size);

/* ===== SDA globals ===== */
extern u8 lbl_80478960;
extern u8 lbl_8047A670;
extern u8 lbl_8047A674;
extern u8 lbl_8047A678;
extern u8 lbl_8047A67C;
extern u8 lbl_8047A684;
extern u8 lbl_8047A690;
extern u8 lbl_8047A694;
extern u8 lbl_8047C1D0;
extern u8 lbl_8047C1D4;
extern u8 lbl_8047C1D8;
extern u8 lbl_8047C1DC;
extern u8 lbl_8047C1E0;

/* ===== Rodata / data labels ===== */
extern u8 jumptable_802EEBB8[];
extern u8 jumptable_802EEBE0[];
extern u8 jumptable_802EEC10[];
extern u8 jumptable_802EEC30[];
extern u8 lbl_802EEB98[];
extern u8 lbl_802EEC70[];
extern u8 lbl_803FB308[];
extern u8 lbl_803FB318[];

/* ===== Forward declarations ===== */
void fn_800895A4(void);
u32 fn_800896B8(void);
u32 fn_800896C0(void);
u32 fn_800896C8(void);
void fn_800896D0(u32 v);
void fn_800896D8(u32 v);
void fn_800896E0(u32 v);
void fn_800896E8(void);
void fn_80089978(void);
u8 GbaMisc_GetMappedContextByte(void);
s32 GbaMisc_HasActiveContextState(void);
s32 fn_80089C54(void);
void fn_80089C84(s32 param);
s32 GbaMisc_PollEntryStatusA(s32 r31);
s32 GbaMisc_ResetEntryStatusA(s32 param);
void fn_80089D74(s32 param);
s32 fn_80089D98(s32 r31);
s32 fn_80089E20(s32 idx, void* obj, u32 packedStatus, u32 highHalf);
u32 fn_80089F58(u32 v);
u32 fn_80089F60(u32 v);
u32 fn_80089F68(u32 v);
u32 fn_80089F70(u32 v);
void fn_80089F78(void);
s32 fn_8008A99C(void);
int gbaCommandEntryPokemon(u32 r3, u8* r4);
s32 GbaMisc_GetEntryStatus(s32 idx, u32* out);
void GbaMisc_SendPackedEntryStatus(s32 param0, u32 param1, u32 param2);
void gbaCommandSendWazaText(s32 param0, s32 param1);
s32 fn_8008AB8C(s32 r3);
u8 fn_8008ABA0(s32 idx);
s32 GbaMisc_SetEntryState(s32 idx, s32 value);
void fn_8008AC34(void);
void fn_8008AE18(void);
void fn_8008BBDC(void);
void fn_8008C5D4(void);
void fn_8008C6FC(void);
void GbaMisc_RunFlagDispatch(void);
s32 fn_8008C78C(void);
void fn_8008C7B0(void);
void fn_8008CACC(void);
void fn_8008CDD8(void);
void fn_8008D0A0(void);
void fn_8008D348(void);
void fn_8008D938(void);
void fn_8008E320(void);
void fn_8008E7D4(void);
void fn_8008EC28(void);
void fn_8008EED0(void);
void fn_8008F190(void);
void fn_8008F524(void);
void fn_8008F91C(void);
void fn_8008FBF4(void);
void fn_8008FE94(void);
void fn_80090100(void);
void fn_80090720(void);
void fn_800909E4(void);
void fn_80090D34(void);
void fn_8009100C(void);
void fn_80091564(void);
void fn_80091774(void);
void fn_80091984(void);
void fn_80091B94(void);
void fn_80091DA4(void);
void fn_80091F48(void);
void fn_80092140(void);
void fn_80092498(void);
void fn_80092664(void);
void fn_800929BC(void);
void fn_80092B2C(void);

/* ===== Function implementations ===== */

/* 0x800895A4 | size: 0x114 */
void fn_800895A4(void) {
    extern void fn_8008BBDC();
    extern void heroBiosSetHomePlace();
    extern void heroBiosSetSexDataId();
    extern void heroBiosSetRnd();
    extern void heroBiosSetNamePtr();
    extern void heroBiosGetPokemonPtr();
    extern void fn_80135938();
    extern void exribbonSetNo();
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

    r30 = r3;
    r31 = r4;
    r0 = *(u8*)((u8*)r31 + 0x0);
    r0 = r0 & 0x00000004;
    if ((s32)r0 != 0x0) {
        r0 = 0x2;
    } else {

        r0 = 0x1;
    }
    r4 = r0 & 0xFF;
    heroBiosSetHomePlace();
    r3 = 0x0;
    r4 = 0x5;
    fn_80135938();
    r6 = r3;
    r3 = (u32)sp + 0x8;
    r4 = r31 + 0x4;
    r5 = 0x7;
    ((void(*)(void))fn_800F9C04)();
    r3 = r30;
    r4 = (u32)sp + 0x8;
    heroBiosSetNamePtr();
    r4 = *(u8*)((u8*)r31 + 0xC);
    r3 = r30;
    heroBiosSetSexDataId();
    r6 = *(u32*)((u8*)r31 + 0x10);
    r3 = r30;
    r0 = r6 & 0x0000FF00;
    r5 = r6 & 0x00FF0000;
    r4 = r6 << 24;
    r6 = (u32)r6 >> 24;
    r0 = r0 << 8;
    r5 = (u32)r5 >> 8;
    r0 = r4 | r0;
    r0 = r5 | r0;
    r4 = r6 | r0;
    heroBiosSetRnd();
    r29 = r31;
    r28 = 0x0;
    do {
        r3 = r30;
        r4 = r28 & 0xFFFF;
        heroBiosGetPokemonPtr();
        r4 = r29 + 0x14;
        r27 = r3;
        fn_8008BBDC();
        r3 = r27;
        r4 = r28 & 0xFFFF;
        ((void(*)(void))pokemonBiosSetFightTrainerPokemonDataId)();
        r29 = r29 + 0x64;
        r28 = r28 + 0x1;
    } while ((s32)r28 < 0x6);
    r29 = 0x0;
    do {
        r0 = r29 + 0x26c;
        r3 = r29;
        r4 = *(u8*)(r31 + r0);
        exribbonSetNo();
        r29 = r29 + 0x1;
    } while ((s32)r29 < 0xb);
    return;
}

/* 0x800896B8 | size: 0x8 */
u32 fn_800896B8(void) {
    return *(u32*)&lbl_80478960;
}

/* 0x800896C0 | size: 0x8 */
u32 fn_800896C0(void) {
    return *(u32*)&lbl_8047A674;
}

/* 0x800896C8 | size: 0x8 */
u32 fn_800896C8(void) {
    return *(u32*)&lbl_8047A670;
}

/* 0x800896D0 | size: 0x8 */
void fn_800896D0(u32 v) {
    *(u32*)&lbl_80478960 = v;
}

/* 0x800896D8 | size: 0x8 */
void fn_800896D8(u32 v) {
    *(u32*)&lbl_8047A674 = v;
}

/* 0x800896E0 | size: 0x8 */
void fn_800896E0(u32 v) {
    *(u32*)&lbl_8047A670 = v;
}

/* 0x800896E8 | size: 0x290 */
void fn_800896E8(void) {
    extern void fn_80089978();
    extern void msgctrlSetValue();
    extern void fn_80189990();
    extern void fn_8018C1E8();
    extern void fadeCheck();
    extern void fn_801CA5C4();
    extern void fn_801EEAD0();
    extern void fightTrainerPokemonDataBiosSetPokemonDataId();
    extern void fightTrainerPokemonDataBiosGetPtr();
    extern void fn_801FCAFC();
    extern void fn_801FCB40();
    extern void fn_801FCB84();
    extern void fn_801FCB94();
    extern void fn_801FCC3C();
    extern void fightTrainerDataBiosSetKindDataId();
    extern void fightTrainerDataBiosGetPtr();
    extern void fightEncountDataBiosGetPtr();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r30 = r3;
    r26 = r4;
    r3 = 0x0;
    ((void(*)(void))fn_80083CFC)();
    r31 = r3;
    if (r31 == 0) {
        r3 = 0x0;
        return;
    }
    tmp = *(u8*)((u8*)r31 + 0x4000);
    if (tmp == 1) {
        r4 = r31 + 0x4004;
        r3 = 0x4d;
        msgctrlSetValue();
        r3 = r30;
        r4 = r26;
        r5 = 0xe0;
        fn_80189990();

    } else {
        r4 = r31 + 0x4060;
        r3 = 0x4d;
        msgctrlSetValue();
        r3 = r30;
        r4 = r26;
        r5 = 0xe0;
        fn_80189990();
        ((void(*)(void))fn_8001E184)();
        tmp = (s8)r3;
        if ((s32)tmp != 0) {
            if ((s32)tmp < 0) {
            } else {

            }
            r3 = 0x0;
            return;
        }
    }
    tmp = 0x2;
    r3 = 0x231;
    *(u8*)((u8*)r31 + 0x4000) = tmp;
    fightEncountDataBiosGetPtr();
    r3 = 0x9;
    fightTrainerDataBiosGetPtr();
    r4 = *(u8*)((u8*)r31 + 0x4124);
    r28 = r3;
    fn_801FCB94();
    r4 = *(u8*)((u8*)r31 + 0x4125);
    r3 = r28;
    fightTrainerDataBiosSetKindDataId();
    r4 = *(u16*)((u8*)r31 + 0x4134);
    r3 = r28;
    fn_801FCB84();
    r4 = *(u8*)((u8*)r31 + 0x4136);
    r3 = r28;
    fn_801FCAFC();
    r27 = r31;
    r29 = 0x0;
    do {
        r5 = *(u16*)((u8*)r27 + 0x4126);
        r3 = r28;
        r4 = r29 & 0xFF;
        fn_801FCB40();
        r27 = r27 + 0x2;
        r29 = r29 + 0x1;
    } while ((s32)r29 < 4);
    r3 = r28;
    fn_801FCC3C();
    fightTrainerPokemonDataBiosGetPtr();
    r29 = 0x0;
    r26 = 0x0;
    tmp = r29 * 0x2a;
    r28 = r3;
    r27 = r31 + tmp;
    r27 = r27 + 0x4000;
    while ((s32)r26 < 4) {

        r3 = r28;
        r4 = r27 + 0x138;
        fn_80089978();
        tmp = r3 & 0xFF;
        if (tmp == 1) {
            r28 = r28 + 0x50;
            r29 = r29 + 0x1;
        }
        r27 = r27 + 0x2a;
        r26 = r26 + 0x1;

    }
    while ((s32)r29 < 6) {

        r3 = r28;
        r4 = 0x0;
        fightTrainerPokemonDataBiosSetPokemonDataId();
        r28 = r28 + 0x50;
        r29 = r29 + 0x1;

    }
    r3 = r31 + 0x4118;
    tmp = r31 + 0x40bc;
    r5 = 0x9;
    *(u32*)&lbl_8047A670 = r3;
    r3 = 0x231;
    r4 = 0x1;
    *(u32*)&lbl_80478960 = r5;
    r5 = 0x0;
    *(u32*)&lbl_8047A674 = tmp;
    fn_801CA5C4();
    r29 = r3;
    do {
        if (r29 != 2) break;
        r3 = *(u8*)((u8*)r31 + 0x41E1);
        do {
            if (r3 == 0) break;
            fn_801EEAD0();
            tmp = r3 & 0xFF;
            if (tmp != 1) break;
            tmp = 0x1;
            break;
        } while (0);

        tmp = 0x0;

        tmp = tmp & 0xFF;
        if (tmp != 1) break;
        r3 = r31;
        ((void(*)(void))fn_800830A4)();
        r3 = 0x0;
        ((void(*)(void))fn_80083CFC)();
        if (r3 != 0) {
            r4 = *(u8*)((u8*)r3 + 0x4136);
        } else {

            r4 = 0x0;
        }
        r3 = (u32)&lbl_802EEB98;
        r5 = 0x0;
        r3 = (u32)&lbl_802EEB98;
        tmp = 0x10;
        ctr_fn = (void(*)(void))tmp;
        do {
            tmp = *(u8*)((u8*)r3 + 0x1);
            if (r4 == tmp) {
                r3 = (u32)&lbl_802EEB98;
                tmp = r5 << 1;
                r3 = (u32)&lbl_802EEB98;
                r4 = *(u8*)(r3 + tmp);
                break;
            }
            r3 = r3 + 0x2;
            r5 = r5 + 0x1;
        } while (--ctr != 0);
        r3 = (u32)&lbl_802EEB98;
        r3 = (u32)&lbl_802EEB98;
        r4 = *(u8*)((u8*)r3 + 0x0);

        r3 = r30;
        r5 = 0x0;
        fn_8018C1E8();
    } while (0);

    r3 = 0x1;
    fadeCheck();
    r3 = r29;

    return;
}

/* 0x80089978 | size: 0x214 */
void fn_80089978(void) {
    extern void fn_801EEAD0();
    extern void fn_801EEE6C();
    extern void fightTrainerPokemonDataBiosSetPartDataId();
    extern void fightTrainerPokemonDataBiosSetKeyPlayerFlag();
    extern void fightTrainerPokemonDataBiosSetSeikakuDataId();
    extern void fightTrainerPokemonDataBiosSetSexDataId();
    extern void fightTrainerPokemonDataBiosSetFriend();
    extern void fightTrainerPokemonDataBiosSetWazaDataId();
    extern void fightTrainerPokemonDataBiosSetItemDataId();
    extern void fightTrainerPokemonDataBiosSetPokemonDataId();
    extern void fightTrainerPokemonDataBiosSetTokuseiFlag();
    extern void fightTrainerPokemonDataBiosSetDarkPokemonFlag();
    extern void fightTrainerPokemonDataBiosSetLevel();
    extern void fightTrainerPokemonDataBiosSetStatusEffort();
    extern void fightTrainerPokemonDataBiosSetStatusRnd();
    extern void fightTrainerPokemonDataBiosSetNickname();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r28 = r3;
    r29 = r4;
    r3 = *(u8*)((u8*)r29 + 0x2);
    do {
        if (r3 == 0) break;
        fn_801EEAD0();
        tmp = r3 & 0xFF;
        if (tmp != 1) break;
        tmp = 0x1;
        break;
    } while (0);

    tmp = 0x0;

    tmp = tmp & 0xFF;
    if (tmp == 1) {
        r3 = r28;
        r4 = 0x0;
        fightTrainerPokemonDataBiosSetPokemonDataId();
        r3 = 0x0;
    } else {

        r4 = *(u16*)((u8*)r29 + 0x0);
        r3 = r28;
        fightTrainerPokemonDataBiosSetPokemonDataId();
        r3 = r28;
        r4 = 0x0;
        fightTrainerPokemonDataBiosSetNickname();
        r4 = *(u8*)((u8*)r29 + 0x2);
        r3 = r28;
        fightTrainerPokemonDataBiosSetDarkPokemonFlag();
        r3 = *(u8*)((u8*)r29 + 0x2);
        if (r3 != 0) {
            r4 = *(u8*)((u8*)r29 + 0x28);
            fn_801EEE6C();
        }
        r4 = *(u8*)((u8*)r29 + 0x3);
        r3 = r28;
        fightTrainerPokemonDataBiosSetLevel();
        r31 = r29;
        r30 = 0x0;
        do {
            r5 = *(u16*)((u8*)r31 + 0x4);
            r3 = r28;
            r4 = r30 & 0xFF;
            fightTrainerPokemonDataBiosSetWazaDataId();
            r31 = r31 + 0x2;
            r30 = r30 + 0x1;
        } while ((s32)r30 < 4);
        r4 = *(u16*)((u8*)r29 + 0xC);
        r3 = r28;
        fightTrainerPokemonDataBiosSetItemDataId();
        r4 = *(u8*)((u8*)r29 + 0xE);
        r3 = r28;
        fightTrainerPokemonDataBiosSetTokuseiFlag();
        r5 = *(u8*)((u8*)r29 + 0xF);
        r3 = r28;
        r4 = 0x0;
        fightTrainerPokemonDataBiosSetStatusRnd();
        r5 = *(u8*)((u8*)r29 + 0x10);
        r3 = r28;
        r4 = 0x1;
        fightTrainerPokemonDataBiosSetStatusRnd();
        r5 = *(u8*)((u8*)r29 + 0x11);
        r3 = r28;
        r4 = 0x2;
        fightTrainerPokemonDataBiosSetStatusRnd();
        r5 = *(u8*)((u8*)r29 + 0x12);
        r3 = r28;
        r4 = 0x3;
        fightTrainerPokemonDataBiosSetStatusRnd();
        r5 = *(u8*)((u8*)r29 + 0x13);
        r3 = r28;
        r4 = 0x4;
        fightTrainerPokemonDataBiosSetStatusRnd();
        r5 = *(u8*)((u8*)r29 + 0x14);
        r3 = r28;
        r4 = 0x5;
        fightTrainerPokemonDataBiosSetStatusRnd();
        r5 = *(s16*)((u8*)r29 + 0x16);
        r3 = r28;
        r4 = 0x0;
        fightTrainerPokemonDataBiosSetStatusEffort();
        r5 = *(s16*)((u8*)r29 + 0x18);
        r3 = r28;
        r4 = 0x1;
        fightTrainerPokemonDataBiosSetStatusEffort();
        r5 = *(s16*)((u8*)r29 + 0x1A);
        r3 = r28;
        r4 = 0x2;
        fightTrainerPokemonDataBiosSetStatusEffort();
        r5 = *(s16*)((u8*)r29 + 0x1C);
        r3 = r28;
        r4 = 0x3;
        fightTrainerPokemonDataBiosSetStatusEffort();
        r5 = *(s16*)((u8*)r29 + 0x1E);
        r3 = r28;
        r4 = 0x4;
        fightTrainerPokemonDataBiosSetStatusEffort();
        r5 = *(s16*)((u8*)r29 + 0x20);
        r3 = r28;
        r4 = 0x5;
        fightTrainerPokemonDataBiosSetStatusEffort();
        r4 = *(s16*)((u8*)r29 + 0x22);
        r3 = r28;
        fightTrainerPokemonDataBiosSetFriend();
        tmp = *(u8*)((u8*)r29 + 0x24);
        r3 = r28;
        r4 = (s8)tmp;
        fightTrainerPokemonDataBiosSetSexDataId();
        r4 = *(u8*)((u8*)r29 + 0x25);
        r3 = r28;
        fightTrainerPokemonDataBiosSetSeikakuDataId();
        r4 = *(u8*)((u8*)r29 + 0x26);
        r3 = r28;
        fightTrainerPokemonDataBiosSetKeyPlayerFlag();
        r4 = *(u8*)((u8*)r29 + 0x27);
        r3 = r28;
        fightTrainerPokemonDataBiosSetPartDataId();
        r3 = 0x1;
    }
    return;
}

/* 0x80089B8C | size: 0x84 */
#pragma push
#pragma peephole off
u8 GbaMisc_GetMappedContextByte(void) {
    GbaMiscContext* ptr;
    u32 value;
    u8* table;
    u32 index;
    u32 count;

    ptr = fn_80083CFC(0);
    table = &ptr->tableKey_4136;
    if (ptr != 0) {
        value = *table;
    } else {
        value = 0;
    }
    for (table = lbl_802EEB98, index = 0, count = 0x10; count != 0; count--) {
        if (value == table[1]) {
            return lbl_802EEB98[index << 1];
        }
        table += 2;
        index++;
    }
    return lbl_802EEB98[0];
}
#pragma pop

/* 0x80089C10 | size: 0x44 */
#pragma push
#pragma scheduling off
s32 GbaMisc_HasActiveContextState(void) {
    GbaMiscContext* ptr;

    ptr = fn_80083CFC(0);
    if (ptr != 0) {
        if (ptr->state_4000 != 0) {
            return 1;
        }
    }
    return 0;
}
#pragma pop

/* 0x80089C54 | size: 0x30 */
#pragma push
#pragma scheduling off
s32 fn_80089C54(void) {
    extern s32 fn_80083BF8(s32);
    return fn_80083BF8(0) > 0;
}
#pragma pop

/* 0x80089C84 | size: 0x24 */
#pragma push
#pragma scheduling off
void fn_80089C84(s32 param) {
    extern void fn_80071700(s32);
    fn_80071700(param - 1);
}
#pragma pop

/* 0x80089CA8 | size: 0x88 */
s32 GbaMisc_PollEntryStatusA(s32 r31) {
    extern s32 fn_800719A8(s32);
    s32 n;

    n = fn_800719A8(r31 - 1);
    if (n < 0) {
        u16 *base = (u16*)&lbl_8047A684;
        base[r31 - 1] = 0;
    } else if (n == 1 || n == 2) {
        u16 *base = (u16*)&lbl_8047A684;
        u32 v = base[r31 - 1] + 1;
        base[r31 - 1] = v;
        if ((u16)v <= 0xa) {
            n = -1;
        }
    }
    return n;
}

/* 0x80089D30 | size: 0x44 */
#pragma push
#pragma peephole off
s32 GbaMisc_ResetEntryStatusA(s32 param) {
    extern s32 fn_80071AE4(s32);
    s32 ret;
    u32 tmp;
    u32 r4;

    ret = fn_80071AE4(param - 1);
    tmp = GBA_MISC_ENTRY_HALF_OFFSET(param);
    r4 = (u32)&lbl_8047A684;
    r4 = r4 + tmp;
    tmp = 0;
    *(u16*)((u8*)r4 + (-2)) = tmp;
    return ret;
}
#pragma pop

/* 0x80089D74 | size: 0x24 */
#pragma push
#pragma scheduling off
void fn_80089D74(s32 param) {
    extern void fn_800722A0(s32);
    fn_800722A0(param - 1);
}
#pragma pop

/* 0x80089D98 | size: 0x88 */
s32 fn_80089D98(s32 r31) {
    extern s32 fn_80072548(s32);
    s32 n;
    n = fn_80072548(r31 - 1);
    if (n < 0) {
        u16 *base = (u16*)&lbl_8047A684;
        base[r31 - 1] = 0;
    } else if (n == 1 || n == 2) {
        u16 *base = (u16*)&lbl_8047A684;
        u32 v = base[r31 - 1] + 1;
        base[r31 - 1] = v;
        if ((u16)v <= 0xa) {
            n = -1;
        }
    }
    return n;
}

/* 0x80089E20 | size: 0x138 */
#pragma push
#pragma peephole off
s32 fn_80089E20(s32 r30, void* r31, u32 r5, u32 r29) {
    extern void fn_8008AE18(void*, void*);
    extern u8 pokemonBiosGetTokuseiFlag(void*);
    extern u16 pokemonBiosGetPokemonDataId(void*);
    extern u8 exribbonGetNo(s32);
    u8 sp[0x78];
    u32 tmp;
    u32 r3;
    u32 r4;
    u32 r6;
    s32 ret;

    tmp = r5 & 0x0000FF00;
    r4 = r5 & 0x00FF0000;
    r3 = r5 << 24;
    r5 = (u32)r5 >> 24;
    tmp = tmp << 8;
    r4 = (u32)r4 >> 8;
    tmp = r3 | tmp;
    tmp = r4 | tmp;
    tmp = r5 | tmp;
    *(u32*)(sp + 0x0) = tmp;
    r3 = pokemonBiosGetPokemonDataId(r31);
    r3 = r3 & 0xFFFF;
    tmp = r29 << 16;
    r6 = tmp | r3;
    tmp = r6 & 0x0000FF00;
    r4 = r6 & 0x00FF0000;
    r5 = r6 << 24;
    tmp = tmp << 8;
    r6 = (u32)r6 >> 24;
    r4 = (u32)r4 >> 8;
    tmp = r5 | tmp;
    tmp = r4 | tmp;
    tmp = r6 | tmp;
    *(u32*)(sp + 0x4) = tmp;
    if (pokemonBiosGetPokemonDataId(r31) == 0x181) {
        tmp = pokemonBiosGetTokuseiFlag(r31);
        tmp = __cntlzw(tmp & 0xFF);
        tmp = (u32)tmp >> 5;
        r4 = tmp & 0xFF;
        pokemonBiosSetTokuseiFlag(r31, r4);
    }
    fn_8008AE18(r31, sp + 0x8);
    r31 = sp + 0x6C;
    memset(r31, 0, 0xc);
    for (r29 = 0; (s32)r29 < 0xb; r31 = (u8*)r31 + 1, r29++) {
        *(u8*)r31 = exribbonGetNo(r29);
    }
    ret = fn_800726A8(r30 - 1, sp + 0x0);
    r3 = r30 << 1;
    r4 = (u32)&lbl_8047A684;
    r4 = r4 + r3;
    r3 = 0;
    *(u16*)((u8*)r4 + (-2)) = r3;
    return ret;
}
#pragma pop

/* 0x80089F58 | size: 0x8 */
u32 fn_80089F58(u32 v) {
    return v & 0xFFFF;
}

/* 0x80089F60 | size: 0x8 */
u32 fn_80089F60(u32 v) {
    return (v >> 8) & 0xFF;
}

/* 0x80089F68 | size: 0x8 */
u32 fn_80089F68(u32 v) {
    return v & 0xFF;
}

/* 0x80089F70 | size: 0x8 */
u32 fn_80089F70(u32 v) {
    return v >> 16;
}

/* 0x80089F78 | size: 0xA24 */
void fn_80089F78(void) {
    extern void fn_8008C5D4();
    extern void pokemonBiosGetFightTrainerPokemonDataId();
    extern void pokemonBiosGetMailId();
    extern void pokemonBiosGetPokerus();
    extern void pokemonBiosGetHp();
    extern void pokemonBiosGetPokemonWazaPp();
    extern void pokemonBiosGetPokemonWazaDataId();
    extern void pokemonBiosGetLevel();
    extern void pokemonBiosGetNicknamePtr();
    extern void pokemonBiosGetAttest();
    extern void pokemonBiosGetPokemonDataId();
    extern void pokemonGetSex();
    extern void gamedataAttestBiosGetLangareaId();
    extern void fn_801EF634();
    extern void fn_801F02AC();
    extern void fn_801F1700();
    extern void fn_801F2020();
    extern void fn_801F54A4();
    extern void fightFloorBiosGetFightFloorPtr();
    extern void fightTrainerCheckCanIrekaeFightPokemon();
    extern void fightTrainerGetValidFightOutPokemonPtr();
    extern void fightTrainerGetValidFightPokemonPtr();
    extern void fightTrainer_GetControllerId();
    extern void fightOutPokemonBiosGetZokuseiDataId();
    extern void fightOutPokemonGetRndStatus();
    extern void fightOutPokemonCheckFightActionWazaSelect();
    extern void fightOutPokemonCheckCanOutOkWazaBanme();
    extern void fightPokemonGetSoubiItemDataId();
    extern void fightOutPokemonGetPokemonPtr();
    extern void fightPokemonGetPokemonPtr();
    extern void fightOutPokemonGetTokuseiDataId();
    extern void fightTypeDataBiosGetFightoutPokemonNum();
    extern void fightTypeDataBiosGetEntryPokemonNum();
    extern void fightTypeDataBiosGetTrainerNum();
    extern void fightTypeDataBiosGetPtr();
    extern void fn_8022B2CC();
    extern void fightTimerCommandIsOver();
    extern void fn_8008A99C();
    u8 sp[0x8D0];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r14 = 0;
    u32 r15 = 0;
    u32 r16 = 0;
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
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r15 = r4;
    r17 = r5;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r24 = r3 & 0xFFFF;
    r3 = r24;
    fightTypeDataBiosGetPtr();
    r18 = r3;
    fightTypeDataBiosGetTrainerNum();
    r19 = r3;
    r3 = r18;
    fightTypeDataBiosGetEntryPokemonNum();
    r16 = r3;
    r3 = r18;
    fightTypeDataBiosGetFightoutPokemonNum();
    r18 = r3;
    r3 = r19 & 0xFF;
    tmp = r18 & 0xFF;
    r4 = r15;
    tmp = r3 * tmp;
    r5 = r24;
    r3 = 0xb;
    tmp = tmp << 1;
    r23 = tmp & 0xFF;
    fn_801F02AC();
    r4 = 0x0;
    r20 = r3;
    fightTrainerGetValidFightOutPokemonPtr();
    tmp = r19 & 0xFF;
    r4 = 0x1;
    *(u8*)(sp + 0x24) = r4;
    if (tmp == 2) {
        r4 = r20;
        r5 = r24;
        r3 = 0x7;
        fn_801F02AC();
        r4 = 0x0;
        r19 = r3;
        fightTrainerGetValidFightOutPokemonPtr();
        r3 = r19;
        fightTrainer_GetControllerId();
        *(u8*)(sp + 0x25) = r3;
        r4 = r20;
        r5 = r24;
        r3 = 0x9;
        fn_801F02AC();
        r4 = 0x0;
        r19 = r3;
        fightTrainerGetValidFightOutPokemonPtr();
        r3 = r19;
        fightTrainer_GetControllerId();
        *(u8*)(sp + 0x26) = r3;
        r4 = r20;
        r5 = r24;
        r3 = 0xa;
        fn_801F02AC();
        r4 = 0x0;
        r19 = r3;
        fightTrainerGetValidFightOutPokemonPtr();
        r3 = r19;
        fightTrainer_GetControllerId();
        *(u8*)(sp + 0x27) = r3;

    } else {
        tmp = r18 & 0xFF;
        if (tmp == 2) {
            r3 = r20;
            r4 = 0x1;
            fightTrainerGetValidFightOutPokemonPtr();
            r4 = r20;
            r5 = r24;
            r3 = 0x9;
            fn_801F02AC();
            r4 = 0x0;
            r19 = r3;
            fightTrainerGetValidFightOutPokemonPtr();
            r3 = r19;
            r4 = 0x1;
            fightTrainerGetValidFightOutPokemonPtr();
            tmp = 0x2;
            r4 = 0x1;
            *(u8*)(sp + 0x25) = r4;
            *(u8*)(sp + 0x26) = tmp;
            *(u8*)(sp + 0x27) = tmp;

        } else {
            r4 = r20;
            r5 = r24;
            r3 = 0x9;
            fn_801F02AC();
            r4 = 0x0;
            fightTrainerGetValidFightOutPokemonPtr();
            tmp = 0x1;
            *(u8*)(sp + 0x25) = tmp;
        }
    }
    r3 = r15;
    r4 = r17 & 0xFFFF;
    fightTrainerGetValidFightOutPokemonPtr();
    tmp = r18 & 0xFF;
    r22 = r3;
    if (tmp == 2) {
        tmp = r17 + 0x1;
    } else {

        tmp = 0x0;
    }
    tmp = tmp & 0xFF;
    r4 = *(u8*)(sp + 0x3B);
    r4 = (r4 & ~0x0000007F) | (((tmp << 0) | (tmp >> 32)) & 0x0000007F);
    r25 = (u32)sp + 0x38;
    *(u8*)(sp + 0x3B) = r4;
    r3 = tmp & 0xFF;
    tmp = r4 & 0xFF;
    tmp = (tmp & ~0x00000080) | (((r3 << 7) | (r3 >> 25)) & 0x00000080);
    *(u8*)(sp + 0x3F) = r23;
    r27 = r25;
    r26 = r16 & 0xFF;
    *(u8*)(sp + 0x3B) = tmp;
    r19 = 0x0;
    r18 = 0x0;
    r21 = 0x0;
    r28 = 0x8;
    while (1) {
        if ((s32)r21 >= (s32)r26) break;
        r3 = r15;
        r4 = r21 & 0xFFFF;
        fightTrainerGetValidFightPokemonPtr();
        r29 = r3;
        if (r29 == 0) break;
        fightPokemonGetPokemonPtr();
        r17 = r3;
        pokemonBiosGetFightTrainerPokemonDataId();
        tmp = r3 & 0xFFFF;
        r3 = r15;
        tmp = tmp << r28;
        r4 = r29;
        r19 = r19 | tmp;
        fightTrainerCheckCanIrekaeFightPokemon();
        tmp = r3 & 0xFF;
        r3 = r17;
        tmp = tmp << r28;
        r18 = r18 | tmp;
        pokemonBiosGetPokerus();
        r16 = r3;
        r3 = r17;
        pokemonBiosGetHp();
        tmp = r3 & 0xFFFF;
        r3 = r17;
        r4 = tmp << 8;
        tmp = (s32)tmp >> 8;
        tmp = r4 | tmp;
        tmp = tmp & 0xFFFF;
        *(u16*)((u8*)r27 + 0x24) = tmp;
        fn_8008C5D4();
        r20 = r3 & 0xFFFF;
        r3 = r29;
        tmp = r20 & 0xFF;
        *(u8*)((u8*)r27 + 0x26) = tmp;
        fightPokemonGetSoubiItemDataId();
        r5 = r3 & 0xFFFF;
        tmp = *(u8*)((u8*)r27 + 0x27);
        r4 = -r5;
        r3 = r17;
        r4 = r4 | r5;
        r4 = (u32)r4 >> 31;
        tmp = (tmp & ~0x00000080) | (((r4 << 7) | (r4 >> 25)) & 0x00000080);
        *(u8*)((u8*)r27 + 0x27) = tmp;
        pokemonBiosGetMailId();
        r4 = r3 & 0xFF;
        r7 = r16 & 0xFF;
        r3 = 0xff - r4;
        r6 = *(u8*)((u8*)r27 + 0x27);
        r5 = r7 & 0xF;
        tmp = r3 | tmp;
        r3 = r7 & 0x000000F0;
        tmp = (u32)tmp >> 31;
        r4 = -r5;
        r6 = (r6 & ~0x00000040) | (((tmp << 6) | (tmp >> 26)) & 0x00000040);
        tmp = -r3;
        *(u8*)((u8*)r27 + 0x27) = r6;
        r4 = r4 | r5;
        r5 = (u32)r4 >> 31;
        r3 = tmp | r3;
        r4 = *(u8*)((u8*)r27 + 0x27);
        r4 = (r4 & ~0x00000010) | (((r5 << 4) | (r5 >> 28)) & 0x00000010);
        tmp = (s32)r20 >> 8;
        r5 = (u32)r3 >> 31;
        *(u8*)((u8*)r27 + 0x27) = r4;
        r4 = tmp & 0xFF;
        r3 = r29;
        tmp = *(u8*)((u8*)r27 + 0x27);
        tmp = (tmp & ~0x00000020) | (((r5 << 5) | (r5 >> 27)) & 0x00000020);
        *(u8*)((u8*)r27 + 0x27) = tmp;
        tmp = *(u8*)((u8*)r27 + 0x27);
        tmp = (tmp & ~0x0000000F) | (((r4 << 0) | (r4 >> 32)) & 0x0000000F);
        *(u8*)((u8*)r27 + 0x27) = tmp;
        fightPokemonGetSoubiItemDataId();
        tmp = r3 & 0xFFFF;
        r31 = r27;
        r3 = tmp << 8;
        r30 = r27;
        tmp = (s32)tmp >> 8;
        r29 = r27;
        tmp = r3 | tmp;
        r20 = 0x0;
        tmp = tmp & 0xFFFF;
        *(u16*)((u8*)r27 + 0x28) = tmp;
        do {
            r3 = r17;
            r4 = r20 & 0xFFFF;
            pokemonBiosGetPokemonWazaDataId();
            tmp = r3;
            r3 = r17;
            r16 = tmp;
            r4 = r20 & 0xFFFF;
            tmp = r16 & 0xFFFF;
            r5 = tmp << 8;
            tmp = (s32)tmp >> 8;
            tmp = r5 | tmp;
            tmp = tmp & 0xFFFF;
            *(u16*)((u8*)r31 + 0x2C) = tmp;
            pokemonBiosGetPokemonWazaPp();
            *(u8*)((u8*)r30 + 0x34) = r3;
            r4 = r16;
            r3 = r29 + 0x38;
            ((void(*)(void))fn_80083ECC)();
            r31 = r31 + 0x2;
            r30 = r30 + 0x1;
            r29 = r29 + 0x50;
            r20 = r20 + 0x1;
        } while ((s32)r20 < 4);
        r28 = r28 + 0x4;
        r27 = r27 + 0x154;
        r21 = r21 + 0x1;

    }

    r20 = r21 & 0xFF;
    tmp = r18 & 0x0000FF00;
    r3 = r20 << 2;
    r4 = r18 & 0x00FF0000;
    r3 = r3 + 0x8;
    r5 = 0x1;
    r5 = r5 << r3;
    r3 = r18 << 24;
    r5 = -r5;
    tmp = tmp << 8;
    r19 = r19 | r5;
    r4 = (u32)r4 >> 8;
    r5 = r19 & 0x0000FF00;
    tmp = r3 | tmp;
    r3 = r19 & 0x00FF0000;
    r6 = r19 << 24;
    r5 = r5 << 8;
    r8 = (u32)r19 >> 24;
    r7 = (u32)r3 >> 8;
    r3 = (u32)r18 >> 24;
    r5 = r6 | r5;
    tmp = r4 | tmp;
    r5 = r7 | r5;
    r5 = r8 | r5;
    r3 = r3 | tmp;
    r4 = (r4 & ~0xFFFFFF00) | (((r5 << 8) | (r5 >> 24)) & 0xFFFFFF00);
    tmp = (tmp & ~0xFFFFFF00) | (((r3 << 8) | (r3 >> 24)) & 0xFFFFFF00);
    r3 = r22;
    r26 = 0x0;
    *(u32*)(sp + 0x3C) = tmp;
    fightOutPokemonGetPokemonPtr();
    tmp = r3;
    r3 = r22;
    r21 = tmp;
    r4 = (u32)sp + 0x20;
    r5 = (u32)sp + 0x1c;
    fightOutPokemonGetRndStatus();
    r3 = r21;
    r4 = r9 & 0x0000FF00;
    r8 = r9 & 0x00FF0000;
    tmp = r10 & 0x0000FF00;
    r5 = r10 & 0x00FF0000;
    r7 = r9 << 24;
    r6 = r4 << 8;
    r4 = r10 << 24;
    tmp = tmp << 8;
    r8 = (u32)r8 >> 8;
    r6 = r7 | r6;
    r5 = (u32)r5 >> 8;
    tmp = r4 | tmp;
    r7 = (u32)r9 >> 24;
    r4 = r8 | r6;
    r6 = r7 | r4;
    r4 = (u32)r10 >> 24;
    tmp = r5 | tmp;
    tmp = r4 | tmp;
    *(u32*)(sp + 0x44) = tmp;
    pokemonBiosGetPokemonDataId();
    r4 = r3 & 0xFFFF;
    r3 = r4 << 8;
    tmp = (s32)r4 >> 8;
    tmp = r3 | tmp;
    tmp = tmp & 0xFFFF;
    *(u16*)(sp + 0x48) = tmp;
    if (r4 == 0x181) {
        r3 = r22;
        r4 = 0x0;
        fightOutPokemonBiosGetZokuseiDataId();
        tmp = r3 & 0xFFFF;
        if (tmp == 0xa) {
            r4 = 0x1;

        } else if (tmp == 0xb) {
            r4 = 0x2;

        } else if (tmp == 0xf) {
            r4 = 0x3;

        } else {
            r4 = 0x0;
        }
        tmp = r3 & 0xFFFF;
        if (tmp == 0xa) {
            tmp = 0x1;

        } else if (tmp == 0xb) {
            tmp = 0x2;

        } else if (tmp == 0xf) {
            tmp = 0x3;

        } else {
            tmp = 0x0;
        }
        r3 = tmp & 0xFFFF;
        tmp = r4 & 0xFFFF;
        r3 = r3 << 8;
        tmp = (s32)tmp >> 8;
        tmp = r3 | tmp;
        tmp = tmp & 0xFFFF;
        *(u16*)(sp + 0x4A) = tmp;
    } else {

        tmp = 0x0;
        *(u16*)(sp + 0x4A) = tmp;
    }
    if ((s32)tmp != 0) {
        r3 = 0x0;
        r6 = 0x0;

    } else {
        fightFloorBiosGetFightFloorPtr();
        r4 = r22;
        r5 = (u32)sp + 0x18;
        fn_801F2020();
        tmp = r3 & 0xFF;
        if (tmp == 2) {
            fightOutPokemonGetTokuseiDataId();
            tmp = r3 & 0xFFFF;
            if (tmp == 0x17) {
                tmp = 0x2;

            } else if (tmp == 0x2a) {
                tmp = 0x3;

            } else if (tmp == 0x47) {
                tmp = 0x4;

            } else {
                tmp = 0x0;
            }
            r3 = tmp & 0xFF;
        }
        r4 = (u32)sp + 0x28;
        tmp = r6 << 2;
        r4 = r4 + tmp;
        ctr_fn = (void(*)(void))r6;
        if ((s32)r6 > 0) {
            do {
                tmp = *(u32*)((u8*)r4 + 0x0);
                if (r5 == tmp) break;
            } while (--ctr != 0);
        }
    }
    tmp = *(u8*)(sp + 0x5B);
    tmp = (tmp & ~0x0000000F) | (((r3 << 0) | (r3 >> 32)) & 0x0000000F);
    r4 = r6 & 0xFF;
    r3 = r22;
    *(u8*)(sp + 0x5B) = tmp;
    tmp = tmp & 0xFF;
    tmp = (tmp & ~0x000000F0) | (((r4 << 4) | (r4 >> 28)) & 0x000000F0);
    r4 = 0x0;
    *(u8*)(sp + 0x5B) = tmp;
    fightOutPokemonCheckFightActionWazaSelect();
    tmp = *(u8*)(sp + 0x5A);
    tmp = (tmp & ~0x0000000F) | (((r3 << 0) | (r3 >> 32)) & 0x0000000F);
    r19 = 0x0;
    r16 = r25;
    *(u8*)(sp + 0x5A) = tmp;
    r17 = r25;
    r18 = r19;
    r3 = (u32)fn_8008A99C;
    r15 = (u32)fn_8008A99C;
    do {
        tmp = 0x0;
        r3 = r21;
        *(u8*)&lbl_8047A678 = tmp;
        r4 = r19 & 0xFFFF;
        pokemonBiosGetPokemonWazaDataId();
        tmp = r3;
        r3 = r21;
        r27 = tmp;
        r4 = r19 & 0xFFFF;
        tmp = r27 & 0xFFFF;
        r5 = tmp << 8;
        tmp = (s32)tmp >> 8;
        tmp = r5 | tmp;
        tmp = tmp & 0xFFFF;
        *(u16*)((u8*)r16 + 0x14) = tmp;
        pokemonBiosGetPokemonWazaPp();
        *(u8*)((u8*)r17 + 0x1C) = r3;
        r3 = r22;
        r4 = r27;
        r5 = r24;
        r6 = r15;
        r7 = 0x1;
        r8 = 0x0;
        r9 = -0x1;
        fn_8022B2CC();
        tmp = *(u8*)&lbl_8047A678;
        if (tmp != 0) {
            tmp = 0x8;
            tmp = tmp << r18;
            tmp = r26 | tmp;
            r26 = tmp & 0xFFFF;
        }
        r3 = r22;
        r4 = r19 & 0xFFFF;
        r6 = (u32)sp + 0x10;
        r5 = 0x1;
        fightOutPokemonCheckCanOutOkWazaBanme();
        r3 = r3 & 0xFF;
        tmp = r3 << r18;
        tmp = r26 | tmp;
        r26 = tmp & 0xFFFF;
        if (r3 == 5) {
            r14 = *(u16*)(sp + 0x10);
        }
        r16 = r16 + 0x2;
        r17 = r17 + 0x1;
        r18 = r18 + 0x4;
        r19 = r19 + 0x1;
    } while ((s32)r19 < 4);
    tmp = (s32)r26 >> 8;
    r3 = r26 << 8;
    tmp = r3 | tmp;
    r3 = r14 << 8;
    r4 = tmp & 0xFFFF;
    tmp = (s32)r14 >> 8;
    *(u16*)(sp + 0x58) = r4;
    tmp = r3 | tmp;
    r4 = 0x3;
    tmp = tmp & 0xFFFF;
    r3 = *(u16*)(sp + 0x52);
    if (r3 != tmp) {
        r4 = 0x2;
        r3 = *(u16*)(sp + 0x50);
        if (r3 != tmp) {
            r4 = 0x1;
            r3 = *(u16*)(sp + 0x4E);
            if (r3 != tmp) {
                r4 = 0x0;
    }
    }
    }
    r3 = r4 & 0xFF;
    tmp = *(u8*)(sp + 0x5A);
    tmp = (tmp & ~0x000000F0) | (((r3 << 4) | (r3 >> 28)) & 0x000000F0);
    r14 = (u32)sp + 0x28;
    *(u8*)(sp + 0x5A) = tmp;
    r15 = (u32)sp + 0x24;
    r16 = 0x0;
    while ((s32)r16 < (s32)r23) {

        r3 = r25 + 0x81c;
        r4 = 0x0;
        r5 = 0xc;
        memset((void*)r3, (int)r4, (u32)r5);
        r3 = *(u32*)((u8*)r14 + 0x0);
        if (r3 == r22) {
            tmp = 0xff;
            *(u8*)((u8*)r25 + 0x81C) = tmp;
        } else {

            fightOutPokemonGetPokemonPtr();
            r18 = r3;
            pokemonBiosGetAttest();
            r17 = r3;
            gamedataAttestBiosGetLangareaId();
            r19 = r3 & 0xFF;
            r3 = r18;
            pokemonBiosGetNicknamePtr();
            r4 = r3;
            r5 = r19;
            r3 = r25 + 0x81c;
            ((void(*)(void))fn_800F9AEC)();
            if ((s32)r3 < 0xa) {
                tmp = r3 + 0x81c;
                r3 = 0xff;
                *(u8*)(r25 + tmp) = r3;
            }
            r3 = r18;
            pokemonBiosGetPokemonDataId();
            r19 = r3;
            r3 = r18;
            pokemonGetSex();
            r5 = r19 & 0xFFFF;
            r6 = *(u8*)((u8*)r25 + 0x826);
            r6 = (r6 & ~0x000000C0) | (((r3 << 6) | (r3 >> 26)) & 0x000000C0);
            r4 = *(u8*)((u8*)r15 + 0x0);
            tmp = 0x20 - r5;
            *(u8*)((u8*)r25 + 0x826) = r6;
            r3 = __cntlzw(tmp);
            tmp = 0x1d - r5;
            r3 = (u32)r3 >> 5;
            r5 = *(u8*)((u8*)r25 + 0x826);
            r3 = r3 & 0xFF;
            tmp = __cntlzw(tmp);
            r5 = (r5 & ~0x00000020) | (((r3 << 5) | (r3 >> 27)) & 0x00000020);
            r3 = r17;
            *(u8*)((u8*)r25 + 0x826) = r5;
            tmp = (u32)tmp >> 5;
            r5 = tmp & 0xFF;
            tmp = *(u8*)((u8*)r25 + 0x826);
            tmp = (tmp & ~0x00000010) | (((r5 << 4) | (r5 >> 28)) & 0x00000010);
            *(u8*)((u8*)r25 + 0x826) = tmp;
            tmp = *(u8*)((u8*)r25 + 0x826);
            tmp = (tmp & ~0x00000007) | (((r4 << 0) | (r4 >> 32)) & 0x00000007);
            *(u8*)((u8*)r25 + 0x826) = tmp;
            gamedataAttestBiosGetLangareaId();
            tmp = r3 & 0xFF;
            if (tmp == 1) {
                tmp = *(u8*)((u8*)r25 + 0x826);
                r3 = 0x1;
                tmp = (tmp & ~0x00000008) | (((r3 << 3) | (r3 >> 29)) & 0x00000008);
                *(u8*)((u8*)r25 + 0x826) = tmp;
            } else {

                tmp = *(u8*)((u8*)r25 + 0x826);
                r3 = 0x0;
                tmp = (tmp & ~0x00000008) | (((r3 << 3) | (r3 >> 29)) & 0x00000008);
                *(u8*)((u8*)r25 + 0x826) = tmp;
            }
            r3 = r18;
            pokemonBiosGetLevel();
            *(u8*)((u8*)r25 + 0x827) = r3;
        }
        r25 = r25 + 0xc;
        r14 = r14 + 0x4;
        r15 = r15 + 0x1;
        r16 = r16 + 0x1;

    }
    r3 = r20 * 0x154;
    tmp = (u32)sp + 0x38;
    r4 = (u32)sp + 0x854;
    r14 = r23 * 0xc;
    r3 = r3 + 0x24;
    r5 = r14;
    r3 = tmp + r3;
    ((void(*)(void))memmove)();
    tmp = r20 * 0x154;
    r4 = (u32)sp + 0x38;
    r5 = tmp + r14;
    r3 = r15;
    r5 = r5 + 0x24;
    ((void(*)(void))fn_80072D58)();
    do {
        fn_801EF634();
        tmp = r3 & 0xFFFF;
        if (tmp == 1) {
            r3 = r15;
            ((void(*)(void))fn_80072A00)();
            r3 = 0x50000;
            return;
        }
        r3 = 0x0;
        fn_801F1700();
        tmp = r3 & 0xFF;
        if (tmp == 1) {
            fightTimerCommandIsOver();
            tmp = r3 & 0xFF;
            if (tmp == 1) {
                r3 = r15;
                ((void(*)(void))fn_80072A00)();
                r3 = 0x40000;
                return;
        }
        }
        r3 = r15;
        r4 = (u32)sp + 0x14;
        ((void(*)(void))fn_80072C74)();
        if ((s32)r3 > 0) {
            r3 = r3 | (0x5 << 16);
            return;
        }
        if ((s32)r3 == 0) {
            tmp = r5 & 0x0000FF00;
            r4 = r5 & 0x00FF0000;
            r3 = r5 << 24;
            r5 = (u32)r5 >> 24;
            tmp = tmp << 8;
            r4 = (u32)r4 >> 8;
            tmp = r3 | tmp;
            tmp = r4 | tmp;
            r3 = r5 | tmp;
            return;
        }
        ((void(*)(void))_threadSwitch)();
    } while (1);

    return;
}

/* 0x8008A99C | size: 0x10 */
s32 fn_8008A99C(void) {
    lbl_8047A678 = 1;
    return 0;
}

/* 0x8008A9AC | size: 0x38 */
int gbaCommandEntryPokemon(u32 r3, u8* r4) {
    r4[0] = r3 & 0xF;
    r4[1] = (r3 >> 4) & 0xF;
    r4[2] = (r3 >> 8) & 0xF;
    r4[3] = (r3 >> 12) & 0xF;
    r4[4] = (r3 >> 16) & 0xF;
    r4[5] = (r3 >> 20) & 0xF;
    return 0;
}

/* 0x8008A9E4 | size: 0x13C */
#pragma push
#pragma peephole off
s32 GbaMisc_GetEntryStatus(s32 idx, u32* out) {
    u32 status;
    u32 tmp;
    u32 r3;
    u32 r4;
    u32 r5;
    u32 offset32;
    u32 offset16;
    s32 ret;

    *out = 0x2000000;
    ret = _AGB_EntryGetStatus__FlPUl(idx - 1, &status);
    if (ret < 0) {
        status = 0x2000000;
        goto returnZero;
    }
    if (ret != 0) {
        *out = 0x3000000;
        offset32 = GBA_MISC_ENTRY_WORD_OFFSET(idx);
        offset16 = GBA_MISC_ENTRY_HALF_OFFSET(idx);
        GbaMisc_EntryStateAtWordOffset(offset32) = 1;
        GbaMisc_EntryCachedStatusAtWordOffset(offset32) = 0;
        GbaMisc_EntryCounterAAtHalfOffset(offset16) = 0;
        GbaMisc_EntryCounterBAtHalfOffset(offset16) = 0;
        return ret;
    }
    r5 = *(volatile u32*)&status;
    tmp = r5 & 0x0000FF00;
    r4 = r5 & 0x00FF0000;
    r3 = r5 << 24;
    r5 = (u32)r5 >> 24;
    tmp = tmp << 8;
    r4 = (u32)r4 >> 8;
    tmp = r3 | tmp;
    tmp = r4 | tmp;
    tmp = r5 | tmp;
    *out = tmp;
    tmp = *out >> 24;
    if (tmp != 0) {
        goto returnZero;
    }
    offset32 = GBA_MISC_ENTRY_WORD_OFFSET(idx);
    offset16 = GBA_MISC_ENTRY_HALF_OFFSET(idx);
    GbaMisc_EntryStateAtWordOffset(offset32) = 1;
    GbaMisc_EntryCachedStatusAtWordOffset(offset32) = 0;
    GbaMisc_EntryCounterAAtHalfOffset(offset16) = 0;
    GbaMisc_EntryCounterBAtHalfOffset(offset16) = 0;
returnZero:
    return 0;
}
#pragma pop

/* 0x8008AB20 | size: 0x2C */
#pragma push
#pragma peephole off
void GbaMisc_SendPackedEntryStatus(s32 param0, u32 param1, u32 param2) {
    u32 packed;

    packed = param2 << 24;
    param0--;
    fn_800730F8(param0, packed | param1);
}
#pragma pop

/* 0x8008AB4C | size: 0x40 */
void gbaCommandSendWazaText(s32 param0, s32 param1) {
    extern void fn_80083D30(s32, void*);
    extern void fn_800733D0(s32, void*);
    u8 buf[0x780];
    fn_80083D30(param1, buf);
    fn_800733D0(param0 - 1, buf);
}

/* 0x8008AB8C | size: 0x14 */
s32 fn_8008AB8C(s32 r3) {
    u16 *base = (u16*)&lbl_8047A67C;
    return base[r3 - 1];
}

/* 0x8008ABA0 | size: 0x44 */
u8 fn_8008ABA0(s32 idx) {
    u32 ret = 0;
    if (GbaMisc_EntryState(idx) != 0) {
        if (GbaMisc_EntryCachedStatus(idx) == 0) {
            ret = 1;
        }
    }
    return (u8)ret;
}

/* 0x8008ABE4 | size: 0x50 */
s32 GbaMisc_SetEntryState(s32 idx, s32 value) {
    u32 r0;
    u32 r5;
    u32 r6;
    u32 r7;
    u32 r8;
    u32 r9;
    u32 r10;
    s32 old;

    r6 = (u32)&lbl_803FB318;
    r7 = idx << 2;
    r0 = r6;
    r5 = (u32)&lbl_803FB308;
    r9 = r0 + r7;
    r10 = idx << 1;
    idx = r10;
    r9 = r9 - 4;
    r6 = r5;
    old = *(s32*)r9;
    r5 = (u32)&lbl_8047A684;
    r7 = r6 + r7;
    r8 = 0;
    r6 = r5 + idx;
    r0 = (u32)&lbl_8047A67C;
    r5 = r0 + idx;
    *(s32*)r9 = value;
    *(u32*)((u8*)r7 + (-4)) = r8;
    *(u16*)((u8*)r6 + (-2)) = r8;
    *(u16*)((u8*)r5 + (-2)) = r8;
    return old;
}

/* 0x8008AC34 | size: 0x1E4 */
void fn_8008AC34(void) {
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r3;
    r3 = (u32)&lbl_803FB318;
    r30 = r29 << 2;
    tmp = (u32)&lbl_803FB318;
    r31 = tmp + r30;
    tmp = *(u32*)((u8*)r31 + 0x0);
    if ((s32)tmp != 2) {
        if ((s32)tmp < 2) {
            if ((s32)tmp < 1 || (s32)tmp >= 4) {
                tmp = r29 << 1;
                r3 = (u32)&lbl_8047A67C;
                r3 = r3 + tmp;
                tmp = 0x0;
                *(u16*)((u8*)r3 + (-2)) = tmp;
                r3 = 0x1;
            } else {

            r4 = (u32)sp + 0x8;
            ((void(*)(void))fn_80073A44)();
            if ((s32)r3 == 0) {
                r7 = r29 << 1;
                r6 = (u32)&lbl_8047A684;
                tmp = (u32)&lbl_8047A67C;
                r5 = *(u16*)(sp + 0x8);
                r4 = tmp + r7;
                r6 = r6 + r7;
                tmp = 0x0;
                *(u16*)((u8*)r4 + (-2)) = r5;
                *(u16*)((u8*)r6 + (-2)) = tmp;
            } else {
            do {
                if ((s32)r3 > 2) break;
                tmp = r29 << 1;
                r4 = (u32)&lbl_8047A684;
                r5 = r4 + tmp;
                r4 = *(u16*)((u8*)r5 + (-2));
                r4 = r4 + 0x1;
                tmp = r4 & 0xFFFF;
                *(u16*)((u8*)r5 + (-2)) = r4;
                if (tmp > 0xa) break;
                r3 = 0x0;
            } while (0);

            r7 = r29 << 1;
            tmp = (u32)&lbl_8047A67C;
            r8 = tmp + r7;
            r6 = 0x0;
            r4 = (u32)&lbl_803FB308;
            *(u16*)((u8*)r8 + 0x0) = r6;
            r4 = (u32)&lbl_803FB308;
            tmp = (u32)&lbl_8047A684;
            r5 = r4 + r30;
            *(u32*)((u8*)r31 + 0x0) = r6;
            r4 = tmp + r7;
            *(u32*)((u8*)r5 + (-4)) = r6;
            *(u16*)((u8*)r4 + (-2)) = r6;
            *(u16*)((u8*)r8 + 0x0) = r6;
            }
            }
        } else {
        ((void(*)(void))fn_80073990)();
        if ((s32)r3 == 0) {
            tmp = r29 << 1;
            r4 = (u32)&lbl_8047A684;
            r4 = r4 + tmp;
            tmp = 0x0;
            *(u16*)((u8*)r4 + (-2)) = tmp;

        } else {
            do {
            if ((s32)r3 > 2) break;
                tmp = r29 << 1;
                r4 = (u32)&lbl_8047A684;
                r5 = r4 + tmp;
                r4 = *(u16*)((u8*)r5 + (-2));
                r4 = r4 + 0x1;
                tmp = r4 & 0xFFFF;
                *(u16*)((u8*)r5 + (-2)) = r4;
                if (tmp > 0xa) break;
                r3 = 0x0;
                break;
            } while (0);

            tmp = 0x0;
            *(u32*)((u8*)r31 + 0x0) = tmp;
        }
        tmp = r29 << 1;
        r4 = (u32)&lbl_8047A67C;
        r4 = r4 + tmp;
        tmp = 0x0;
        *(u16*)((u8*)r4 + (-2)) = tmp;
        }
    } else {
    tmp = r29 << 1;
    r3 = (u32)&lbl_8047A67C;
    r3 = r3 + tmp;
    tmp = 0x0;
    *(u16*)((u8*)r3 + (-2)) = tmp;
    r3 = 0x0;
    }
    r4 = (u32)&lbl_803FB308;
    tmp = (u32)&lbl_803FB308;
    r4 = tmp + r30;
    *(u32*)((u8*)r4 + (-4)) = r3;
    return;
}

/* 0x8008AE18 | size: 0xDC4 */
void fn_8008AE18(void) {
    extern void pokemonBiosGetEventGetFlag();
    extern void pokemonBiosGetPara1Amari();
    extern void pokemonBiosGetAmari();
    extern void pokemonBiosGetMailId();
    extern void pokemonBiosGetPcboxMark();
    extern void pokemonBiosGetFlagAmari();
    extern void pokemonBiosGetFuseiFlag();
    extern void pokemonBiosGetTokuseiFlag();
    extern void pokemonBiosGetTamagoFlag();
    extern void pokemonBiosGetPokerus();
    extern void pokemonBiosGetAmariRibbon();
    extern void pokemonBiosGetWorldRibbon();
    extern void pokemonBiosGetEarthRibbon();
    extern void pokemonBiosGetNationalRibbon();
    extern void pokemonBiosGetCountryRibbon();
    extern void pokemonBiosGetSkyRibbon();
    extern void pokemonBiosGetLandRibbon();
    extern void pokemonBiosGetMarineRibbon();
    extern void pokemonBiosGetGanbaRibbon();
    extern void pokemonBiosGetBromideRibbon();
    extern void pokemonBiosGetVictoryRibbon();
    extern void pokemonBiosGetWinningRibbon();
    extern void pokemonBiosGetChampRibbon();
    extern void pokemonBiosGetFur();
    extern void pokemonBiosGetStrongMedal();
    extern void pokemonBiosGetCleverMedal();
    extern void pokemonBiosGetCuteMedal();
    extern void pokemonBiosGetBeautifulMedal();
    extern void pokemonBiosGetStyleMedal();
    extern void pokemonBiosGetStrong();
    extern void pokemonBiosGetClever();
    extern void pokemonBiosGetCute();
    extern void pokemonBiosGetBeautiful();
    extern void pokemonBiosGetStyle();
    extern void pokemonBiosGetFriend();
    extern void pokemonBiosGetNimblenessRnd();
    extern void pokemonBiosGetSpeDefRnd();
    extern void pokemonBiosGetSpeAtkRnd();
    extern void pokemonBiosGetPhyDefRnd();
    extern void pokemonBiosGetPhyAtkRnd();
    extern void pokemonBiosGetMaxHpRnd();
    extern void pokemonBiosGetNimblenessEffort();
    extern void pokemonBiosGetSpeDefEffort();
    extern void pokemonBiosGetSpeAtkEffort();
    extern void pokemonBiosGetPhyDefEffort();
    extern void pokemonBiosGetPhyAtkEffort();
    extern void pokemonBiosGetMaxHpEffort();
    extern void pokemonBiosGetNimbleness();
    extern void pokemonBiosGetSpeDef();
    extern void pokemonBiosGetSpeAtk();
    extern void pokemonBiosGetPhyDef();
    extern void pokemonBiosGetPhyAtk();
    extern void pokemonBiosGetMaxHp();
    extern void pokemonBiosGetHp();
    extern void pokemonBiosGetItemDataId();
    extern void pokemonBiosGetPokemonWazaPpCount();
    extern void pokemonBiosGetPokemonWazaPp();
    extern void pokemonBiosGetPokemonWazaDataId();
    extern void pokemonBiosGetConditionAmari();
    extern void pokemonBiosGetLevel();
    extern void pokemonBiosGetExp();
    extern void pokemonBiosGetNicknameOrgPtr();
    extern void pokemonBiosGetCatchTrainerNamePtr();
    extern void pokemonBiosGetCatchTrainerRnd();
    extern void pokemonBiosGetCatchTrainerSex();
    extern void pokemonBiosGetCatchBallId();
    extern void pokemonBiosGetCatchLevel();
    extern void pokemonBiosGetCatchFloorId();
    extern void pokemonBiosGetAttest();
    extern void pokemonBiosGetRnd();
    extern void pokemonBiosGetPokemonDataId();
    extern void fn_8012189C();
    extern void fn_80121984();
    extern void fn_80121ADC();
    extern void pokemonCheckValid();
    extern void gamedataAttestBiosGetLangareaId();
    extern void gamedataAttestBiosGetVerId();
    extern u8 jumptable_802EEBB8[];
    extern u8 jumptable_802EEBE0[];
    u8 sp[0x40];
    u32 tmp = 0;
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
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r28 = r3;
    r31 = r4;
    r3 = r4;
    r29 = 0x0;
    r4 = 0x0;
    r5 = 0x64;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = r28;
    pokemonCheckValid();
    tmp = r3 & 0xFF;
    if (tmp == 0) return;
    r3 = r28;
    pokemonBiosGetRnd();
    tmp = r3 & 0x0000FF00;
    r5 = r3 & 0x00FF0000;
    r4 = r3 << 24;
    r6 = (u32)r3 >> 24;
    tmp = tmp << 8;
    r5 = (u32)r5 >> 8;
    tmp = r4 | tmp;
    r3 = r28;
    tmp = r5 | tmp;
    tmp = r6 | tmp;
    *(u32*)((u8*)r31 + 0x0) = tmp;
    pokemonBiosGetCatchTrainerRnd();
    tmp = r3 & 0x0000FF00;
    r5 = r3 & 0x00FF0000;
    r4 = r3 << 24;
    r6 = (u32)r3 >> 24;
    tmp = tmp << 8;
    r5 = (u32)r5 >> 8;
    tmp = r4 | tmp;
    r3 = r28;
    tmp = r5 | tmp;
    tmp = r6 | tmp;
    *(u32*)((u8*)r31 + 0x4) = tmp;
    pokemonBiosGetAttest();
    r30 = r3;
    gamedataAttestBiosGetVerId();
    tmp = r3 & 0xFF;
    do {
        if (tmp <= 0xb) {
            r3 = (u32)jumptable_802EEBE0;
            tmp = tmp << 2;
            r3 = (u32)jumptable_802EEBE0;
            tmp = *(u32*)(r3 + tmp);
            ctr_fn = (void(*)(void))tmp;
            tmp = *(u16*)(sp + 0x10);
            r3 = 0x1;
            tmp = (tmp & ~0x00000780) | (((r3 << 7) | (r3 >> 25)) & 0x00000780);
            *(u16*)(sp + 0x10) = tmp;
            break;
            tmp = *(u16*)(sp + 0x10);
            r3 = 0x2;
            tmp = (tmp & ~0x00000780) | (((r3 << 7) | (r3 >> 25)) & 0x00000780);
            *(u16*)(sp + 0x10) = tmp;
            break;
            tmp = *(u16*)(sp + 0x10);
            r3 = 0x3;
            tmp = (tmp & ~0x00000780) | (((r3 << 7) | (r3 >> 25)) & 0x00000780);
            *(u16*)(sp + 0x10) = tmp;
            break;
            tmp = *(u16*)(sp + 0x10);
            r3 = 0x4;
            tmp = (tmp & ~0x00000780) | (((r3 << 7) | (r3 >> 25)) & 0x00000780);
            *(u16*)(sp + 0x10) = tmp;
            break;
            tmp = *(u16*)(sp + 0x10);
            r3 = 0x5;
            tmp = (tmp & ~0x00000780) | (((r3 << 7) | (r3 >> 25)) & 0x00000780);
            *(u16*)(sp + 0x10) = tmp;
            break;
            tmp = *(u16*)(sp + 0x10);
            r3 = 0xf;
            tmp = (tmp & ~0x00000780) | (((r3 << 7) | (r3 >> 25)) & 0x00000780);
            *(u16*)(sp + 0x10) = tmp;
            break;
        }
        tmp = *(u16*)(sp + 0x10);
        r3 = 0x0;
        tmp = (tmp & ~0x00000780) | (((r3 << 7) | (r3 >> 25)) & 0x00000780);
        *(u16*)(sp + 0x10) = tmp;
    } while (0);

    r3 = r30;
    gamedataAttestBiosGetLangareaId();
    tmp = r3 & 0xFF;
    do {
        if (tmp <= 9) {
            r3 = (u32)jumptable_802EEBB8;
            tmp = tmp << 2;
            r3 = (u32)jumptable_802EEBB8;
            tmp = *(u32*)(r3 + tmp);
            ctr_fn = (void(*)(void))tmp;
            tmp = 0x1;
            *(u8*)((u8*)r31 + 0x12) = tmp;
            break;
            tmp = 0x2;
            *(u8*)((u8*)r31 + 0x12) = tmp;
            break;
            tmp = 0x5;
            *(u8*)((u8*)r31 + 0x12) = tmp;
            break;
            tmp = 0x3;
            *(u8*)((u8*)r31 + 0x12) = tmp;
            break;
            tmp = 0x4;
            *(u8*)((u8*)r31 + 0x12) = tmp;
            break;
            tmp = 0x7;
            *(u8*)((u8*)r31 + 0x12) = tmp;
            break;
            tmp = 0x2;
            *(u8*)((u8*)r31 + 0x12) = tmp;
            break;
            tmp = 0x2;
            *(u8*)((u8*)r31 + 0x12) = tmp;
            break;
        }
        tmp = 0x0;
        *(u8*)((u8*)r31 + 0x12) = tmp;
    } while (0);

    tmp = *(u8*)((u8*)r31 + 0x13);
    r3 = 0x1;
    tmp = (tmp & ~0x00000002) | (((r3 << 1) | (r3 >> 31)) & 0x00000002);
    r3 = r28;
    *(u8*)((u8*)r31 + 0x13) = tmp;
    pokemonBiosGetTamagoFlag();
    r5 = r3 & 0xFF;
    tmp = *(u8*)((u8*)r31 + 0x13);
    r4 = -r5;
    r3 = r28;
    r4 = r4 | r5;
    r4 = (u32)r4 >> 31;
    tmp = (tmp & ~0x00000004) | (((r4 << 2) | (r4 >> 30)) & 0x00000004);
    *(u8*)((u8*)r31 + 0x13) = tmp;
    pokemonBiosGetFlagAmari();
    tmp = *(u8*)((u8*)r31 + 0x13);
    tmp = (tmp & ~0x000000F8) | (((r3 << 3) | (r3 >> 29)) & 0x000000F8);
    r3 = r28;
    *(u8*)((u8*)r31 + 0x13) = tmp;
    pokemonBiosGetFuseiFlag();
    r5 = r3 & 0xFF;
    tmp = *(u8*)((u8*)r31 + 0x13);
    r4 = -r5;
    r3 = r30;
    r4 = r4 | r5;
    r4 = (u32)r4 >> 31;
    tmp = (tmp & ~0x00000001) | (((r4 << 0) | (r4 >> 32)) & 0x00000001);
    *(u8*)((u8*)r31 + 0x13) = tmp;
    gamedataAttestBiosGetLangareaId();
    r27 = r3 & 0xFF;
    r3 = r28;
    pokemonBiosGetNicknameOrgPtr();
    r4 = r3;
    r5 = r27;
    r3 = r31 + 0x8;
    ((void(*)(void))fn_800F9AEC)();
    r5 = r3;
    if ((s32)r5 < 0xa) {
        r4 = r31 + r5;
        tmp = 0xff;
        r3 = r5 + 0x9;
        *(u8*)((u8*)r4 + 0x8) = tmp;
        r3 = r31 + r3;
        r5 = 0x9 - r5;
        r4 = 0x0;
        memset((void*)r3, (int)r4, (u32)r5);
    }
    r3 = r30;
    gamedataAttestBiosGetLangareaId();
    r30 = r3 & 0xFF;
    r3 = r28;
    pokemonBiosGetCatchTrainerNamePtr();
    r4 = r3;
    r5 = r30;
    r3 = r31 + 0x14;
    ((void(*)(void))fn_800F9AEC)();
    r5 = r3;
    if ((s32)r5 < 7) {
        r4 = r31 + r5;
        tmp = 0xff;
        r3 = r5 + 0x15;
        *(u8*)((u8*)r4 + 0x14) = tmp;
        r3 = r31 + r3;
        r5 = 0x6 - r5;
        r4 = 0x0;
        memset((void*)r3, (int)r4, (u32)r5);
    }
    r3 = r28;
    pokemonBiosGetPcboxMark();
    *(u8*)((u8*)r31 + 0x1B) = r3;
    r3 = r28;
    pokemonBiosGetAmari();
    tmp = r3 & 0xFFFF;
    r3 = r28;
    r4 = tmp << 8;
    tmp = (s32)tmp >> 8;
    tmp = r4 | tmp;
    tmp = tmp & 0xFFFF;
    *(u16*)((u8*)r31 + 0x1E) = tmp;
    pokemonBiosGetPokemonDataId();
    tmp = r3 & 0xFFFF;
    r3 = r28;
    r4 = tmp << 8;
    tmp = (s32)tmp >> 8;
    tmp = r4 | tmp;
    tmp = tmp & 0xFFFF;
    *(u16*)((u8*)r31 + 0x20) = tmp;
    pokemonBiosGetItemDataId();
    tmp = r3 & 0xFFFF;
    r3 = r28;
    r4 = tmp << 8;
    tmp = (s32)tmp >> 8;
    tmp = r4 | tmp;
    tmp = tmp & 0xFFFF;
    *(u16*)((u8*)r31 + 0x22) = tmp;
    pokemonBiosGetExp();
    tmp = r3 & 0x0000FF00;
    r5 = r3 & 0x00FF0000;
    r4 = r3 << 24;
    r6 = (u32)r3 >> 24;
    tmp = tmp << 8;
    r5 = (u32)r5 >> 8;
    tmp = r4 | tmp;
    r3 = r28;
    tmp = r5 | tmp;
    tmp = r6 | tmp;
    *(u32*)((u8*)r31 + 0x24) = tmp;
    pokemonBiosGetFriend();
    tmp = r3 & 0xFF;
    r3 = r28;
    *(u8*)((u8*)r31 + 0x29) = tmp;
    pokemonBiosGetPara1Amari();
    *(u16*)((u8*)r31 + 0x2A) = r3;
    r26 = 0x0;
    r27 = r31;
    r30 = 0x0;
    *(u8*)((u8*)r31 + 0x28) = r26;
    do {
        r3 = r28;
        r4 = r30 & 0xFFFF;
        pokemonBiosGetPokemonWazaDataId();
        tmp = r3 & 0xFFFF;
        r3 = r28;
        r5 = tmp << 8;
        r4 = r30 & 0xFFFF;
        tmp = (s32)tmp >> 8;
        tmp = r5 | tmp;
        tmp = tmp & 0xFFFF;
        *(u16*)((u8*)r27 + 0x2C) = tmp;
        pokemonBiosGetPokemonWazaPpCount();
        r3 = r3 & 0xFF;
        tmp = *(u8*)((u8*)r31 + 0x28);
        r4 = r3 << r26;
        r3 = r28;
        tmp = tmp | r4;
        r4 = r30 & 0xFFFF;
        tmp = tmp & 0xFF;
        *(u8*)((u8*)r31 + 0x28) = tmp;
        pokemonBiosGetPokemonWazaPp();
        tmp = r30 + 0x34;
        r27 = r27 + 0x2;
        *(u8*)(r31 + tmp) = r3;
        r26 = r26 + 0x2;
        r30 = r30 + 0x1;
    } while ((s32)r30 < 4);
    r3 = r28;
    pokemonBiosGetMaxHpEffort();
    tmp = r3 & 0xFF;
    r3 = r28;
    *(u8*)((u8*)r31 + 0x38) = tmp;
    pokemonBiosGetPhyAtkEffort();
    tmp = r3 & 0xFF;
    r3 = r28;
    *(u8*)((u8*)r31 + 0x39) = tmp;
    pokemonBiosGetPhyDefEffort();
    tmp = r3 & 0xFF;
    r3 = r28;
    *(u8*)((u8*)r31 + 0x3A) = tmp;
    pokemonBiosGetNimblenessEffort();
    tmp = r3 & 0xFF;
    r3 = r28;
    *(u8*)((u8*)r31 + 0x3B) = tmp;
    pokemonBiosGetSpeAtkEffort();
    tmp = r3 & 0xFF;
    r3 = r28;
    *(u8*)((u8*)r31 + 0x3C) = tmp;
    pokemonBiosGetSpeDefEffort();
    tmp = r3 & 0xFF;
    r3 = r28;
    *(u8*)((u8*)r31 + 0x3D) = tmp;
    pokemonBiosGetStyle();
    *(u8*)((u8*)r31 + 0x3E) = r3;
    r3 = r28;
    pokemonBiosGetBeautiful();
    *(u8*)((u8*)r31 + 0x3F) = r3;
    r3 = r28;
    pokemonBiosGetCute();
    *(u8*)((u8*)r31 + 0x40) = r3;
    r3 = r28;
    pokemonBiosGetClever();
    *(u8*)((u8*)r31 + 0x41) = r3;
    r3 = r28;
    pokemonBiosGetStrong();
    *(u8*)((u8*)r31 + 0x42) = r3;
    r3 = r28;
    pokemonBiosGetFur();
    *(u8*)((u8*)r31 + 0x43) = r3;
    r3 = r28;
    pokemonBiosGetPokerus();
    *(u8*)(sp + 0x13) = r3;
    r3 = r28;
    pokemonBiosGetCatchFloorId();
    tmp = r3 & 0xFF;
    r3 = r28;
    *(u8*)(sp + 0x12) = tmp;
    pokemonBiosGetCatchLevel();
    tmp = *(u8*)(sp + 0x11);
    tmp = (tmp & ~0x0000007F) | (((r3 << 0) | (r3 >> 32)) & 0x0000007F);
    r3 = r28;
    *(u8*)(sp + 0x11) = tmp;
    pokemonBiosGetCatchBallId();
    tmp = *(u8*)(sp + 0x10);
    tmp = (tmp & ~0x00000078) | (((r3 << 3) | (r3 >> 29)) & 0x00000078);
    r3 = r28;
    *(u8*)(sp + 0x10) = tmp;
    pokemonBiosGetCatchTrainerSex();
    tmp = *(u8*)(sp + 0x10);
    tmp = (tmp & ~0x00000080) | (((r3 << 7) | (r3 >> 25)) & 0x00000080);
    r3 = r28;
    *(u8*)(sp + 0x10) = tmp;
    pokemonBiosGetMaxHpRnd();
    r3 = r3 & 0xFF;
    tmp = *(u8*)(sp + 0xF);
    tmp = (tmp & ~0x0000001F) | (((r3 << 0) | (r3 >> 32)) & 0x0000001F);
    r3 = r28;
    *(u8*)(sp + 0xF) = tmp;
    pokemonBiosGetPhyAtkRnd();
    tmp = *(u16*)(sp + 0xE);
    tmp = (tmp & ~0x000003E0) | (((r3 << 5) | (r3 >> 27)) & 0x000003E0);
    r3 = r28;
    *(u16*)(sp + 0xE) = tmp;
    pokemonBiosGetPhyDefRnd();
    r3 = r3 & 0xFF;
    tmp = *(u8*)(sp + 0xE);
    tmp = (tmp & ~0x0000007C) | (((r3 << 2) | (r3 >> 30)) & 0x0000007C);
    r3 = r28;
    *(u8*)(sp + 0xE) = tmp;
    pokemonBiosGetNimblenessRnd();
    r3 = r3 & 0xFFFF;
    tmp = (tmp & ~0x000F8000) | (((r3 << 15) | (r3 >> 17)) & 0x000F8000);
    r3 = r28;
    *(u32*)(sp + 0xC) = tmp;
    pokemonBiosGetSpeAtkRnd();
    tmp = *(u16*)(sp + 0xC);
    tmp = (tmp & ~0x000001F0) | (((r3 << 4) | (r3 >> 28)) & 0x000001F0);
    r3 = r28;
    *(u16*)(sp + 0xC) = tmp;
    pokemonBiosGetSpeDefRnd();
    r3 = r3 & 0xFF;
    tmp = *(u8*)(sp + 0xC);
    tmp = (tmp & ~0x0000003E) | (((r3 << 1) | (r3 >> 31)) & 0x0000003E);
    r3 = r28;
    *(u8*)(sp + 0xC) = tmp;
    pokemonBiosGetTamagoFlag();
    r5 = r3 & 0xFF;
    tmp = *(u8*)(sp + 0xC);
    r4 = -r5;
    r3 = r28;
    r4 = r4 | r5;
    r4 = (u32)r4 >> 31;
    tmp = (tmp & ~0x00000040) | (((r4 << 6) | (r4 >> 26)) & 0x00000040);
    *(u8*)(sp + 0xC) = tmp;
    pokemonBiosGetTokuseiFlag();
    r5 = r3 & 0xFF;
    tmp = *(u8*)(sp + 0xC);
    r4 = -r5;
    r3 = r28;
    r4 = r4 | r5;
    r4 = (u32)r4 >> 31;
    tmp = (tmp & ~0x00000080) | (((r4 << 7) | (r4 >> 25)) & 0x00000080);
    *(u8*)(sp + 0xC) = tmp;
    pokemonBiosGetStyleMedal();
    tmp = *(u8*)(sp + 0xB);
    tmp = (tmp & ~0x00000007) | (((r3 << 0) | (r3 >> 32)) & 0x00000007);
    r3 = r28;
    *(u8*)(sp + 0xB) = tmp;
    pokemonBiosGetBeautifulMedal();
    tmp = *(u8*)(sp + 0xB);
    tmp = (tmp & ~0x00000038) | (((r3 << 3) | (r3 >> 29)) & 0x00000038);
    r3 = r28;
    *(u8*)(sp + 0xB) = tmp;
    pokemonBiosGetCuteMedal();
    r3 = r3 & 0xFF;
    tmp = *(u16*)(sp + 0xA);
    tmp = (tmp & ~0x000001C0) | (((r3 << 6) | (r3 >> 26)) & 0x000001C0);
    r3 = r28;
    *(u16*)(sp + 0xA) = tmp;
    pokemonBiosGetCleverMedal();
    tmp = *(u8*)(sp + 0xA);
    tmp = (tmp & ~0x0000000E) | (((r3 << 1) | (r3 >> 31)) & 0x0000000E);
    r3 = r28;
    *(u8*)(sp + 0xA) = tmp;
    pokemonBiosGetStrongMedal();
    tmp = *(u8*)(sp + 0xA);
    tmp = (tmp & ~0x00000070) | (((r3 << 4) | (r3 >> 28)) & 0x00000070);
    r3 = r28;
    *(u8*)(sp + 0xA) = tmp;
    pokemonBiosGetChampRibbon();
    r5 = r3 & 0xFF;
    tmp = *(u8*)(sp + 0xA);
    r4 = -r5;
    r3 = r28;
    r4 = r4 | r5;
    r4 = (u32)r4 >> 31;
    tmp = (tmp & ~0x00000080) | (((r4 << 7) | (r4 >> 25)) & 0x00000080);
    *(u8*)(sp + 0xA) = tmp;
    pokemonBiosGetWinningRibbon();
    r5 = r3 & 0xFF;
    tmp = *(u8*)(sp + 0x9);
    r4 = -r5;
    r3 = r28;
    r4 = r4 | r5;
    r4 = (u32)r4 >> 31;
    tmp = (tmp & ~0x00000001) | (((r4 << 0) | (r4 >> 32)) & 0x00000001);
    *(u8*)(sp + 0x9) = tmp;
    pokemonBiosGetVictoryRibbon();
    r5 = r3 & 0xFF;
    tmp = *(u8*)(sp + 0x9);
    r4 = -r5;
    r3 = r28;
    r4 = r4 | r5;
    r4 = (u32)r4 >> 31;
    tmp = (tmp & ~0x00000002) | (((r4 << 1) | (r4 >> 31)) & 0x00000002);
    *(u8*)(sp + 0x9) = tmp;
    pokemonBiosGetBromideRibbon();
    r5 = r3 & 0xFF;
    tmp = *(u8*)(sp + 0x9);
    r4 = -r5;
    r3 = r28;
    r4 = r4 | r5;
    r4 = (u32)r4 >> 31;
    tmp = (tmp & ~0x00000004) | (((r4 << 2) | (r4 >> 30)) & 0x00000004);
    *(u8*)(sp + 0x9) = tmp;
    pokemonBiosGetGanbaRibbon();
    r5 = r3 & 0xFF;
    tmp = *(u8*)(sp + 0x9);
    r4 = -r5;
    r3 = r28;
    r4 = r4 | r5;
    r4 = (u32)r4 >> 31;
    tmp = (tmp & ~0x00000008) | (((r4 << 3) | (r4 >> 29)) & 0x00000008);
    *(u8*)(sp + 0x9) = tmp;
    pokemonBiosGetMarineRibbon();
    r5 = r3 & 0xFF;
    tmp = *(u8*)(sp + 0x9);
    r4 = -r5;
    r3 = r28;
    r4 = r4 | r5;
    r4 = (u32)r4 >> 31;
    tmp = (tmp & ~0x00000010) | (((r4 << 4) | (r4 >> 28)) & 0x00000010);
    *(u8*)(sp + 0x9) = tmp;
    pokemonBiosGetLandRibbon();
    r5 = r3 & 0xFF;
    tmp = *(u8*)(sp + 0x9);
    r4 = -r5;
    r3 = r28;
    r4 = r4 | r5;
    r4 = (u32)r4 >> 31;
    tmp = (tmp & ~0x00000020) | (((r4 << 5) | (r4 >> 27)) & 0x00000020);
    *(u8*)(sp + 0x9) = tmp;
    pokemonBiosGetSkyRibbon();
    r5 = r3 & 0xFF;
    tmp = *(u8*)(sp + 0x9);
    r4 = -r5;
    r3 = r28;
    r4 = r4 | r5;
    r4 = (u32)r4 >> 31;
    tmp = (tmp & ~0x00000040) | (((r4 << 6) | (r4 >> 26)) & 0x00000040);
    *(u8*)(sp + 0x9) = tmp;
    pokemonBiosGetCountryRibbon();
    r5 = r3 & 0xFF;
    tmp = *(u8*)(sp + 0x9);
    r4 = -r5;
    r3 = r28;
    r4 = r4 | r5;
    r4 = (u32)r4 >> 31;
    tmp = (tmp & ~0x00000080) | (((r4 << 7) | (r4 >> 25)) & 0x00000080);
    *(u8*)(sp + 0x9) = tmp;
    pokemonBiosGetNationalRibbon();
    r5 = r3 & 0xFF;
    tmp = *(u8*)(sp + 0x8);
    r4 = -r5;
    r3 = r28;
    r4 = r4 | r5;
    r4 = (u32)r4 >> 31;
    tmp = (tmp & ~0x00000001) | (((r4 << 0) | (r4 >> 32)) & 0x00000001);
    *(u8*)(sp + 0x8) = tmp;
    pokemonBiosGetEarthRibbon();
    r5 = r3 & 0xFF;
    tmp = *(u8*)(sp + 0x8);
    r4 = -r5;
    r3 = r28;
    r4 = r4 | r5;
    r4 = (u32)r4 >> 31;
    tmp = (tmp & ~0x00000002) | (((r4 << 1) | (r4 >> 31)) & 0x00000002);
    *(u8*)(sp + 0x8) = tmp;
    pokemonBiosGetWorldRibbon();
    r5 = r3 & 0xFF;
    tmp = *(u8*)(sp + 0x8);
    r4 = -r5;
    r3 = r28;
    r4 = r4 | r5;
    r4 = (u32)r4 >> 31;
    tmp = (tmp & ~0x00000004) | (((r4 << 2) | (r4 >> 30)) & 0x00000004);
    *(u8*)(sp + 0x8) = tmp;
    pokemonBiosGetAmariRibbon();
    tmp = *(u8*)(sp + 0x8);
    tmp = (tmp & ~0x00000078) | (((r3 << 3) | (r3 >> 29)) & 0x00000078);
    r3 = r28;
    *(u8*)(sp + 0x8) = tmp;
    pokemonBiosGetEventGetFlag();
    tmp = *(u8*)(sp + 0x8);
    tmp = (tmp & ~0x00000080) | (((r3 << 7) | (r3 >> 25)) & 0x00000080);
    r3 = r28;
    r27 = 0x0;
    *(u8*)(sp + 0x8) = tmp;
    r4 = 0x4;
    fn_80121ADC();
    tmp = r3 & 0xFF;
    if (tmp != 0) {
        r3 = r28;
        r4 = 0x4;
        fn_80121984();
        tmp = (s16)r3;
        tmp = tmp << 8;
        tmp = tmp | 0x80;
        r27 = tmp & 0xFFFF;

    } else {
        r3 = r28;
        r4 = 0x5;
        fn_80121ADC();
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            tmp = r27 | 0x40;
            r27 = tmp & 0xFFFF;
        } else {
        r3 = r28;
        r4 = 0x7;
        fn_80121ADC();
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            tmp = r27 | 0x20;
            r27 = tmp & 0xFFFF;
        } else {
        r3 = r28;
        r4 = 0x6;
        fn_80121ADC();
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            tmp = r27 | 0x10;
            r27 = tmp & 0xFFFF;
        } else {
        r3 = r28;
        r4 = 0x3;
        fn_80121ADC();
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            tmp = r27 | 0x8;
            r27 = tmp & 0xFFFF;

        } else {
            r3 = r28;
            r4 = 0x8;
            fn_80121ADC();
            tmp = r3 & 0xFF;
            if (tmp != 0) {
                r3 = r28;
                r4 = 0x8;
                fn_8012189C();
                tmp = (s8)r3;
                r27 = tmp & 0xFFFF;
            }
        }
        }
        }
        }
    }
    r3 = r28;
    r27 = r27 & 0xFFFF;
    pokemonBiosGetConditionAmari();
    tmp = r27 & 0x0000FF00;
    r5 = r27 & 0x00FF0000;
    r4 = r27 << 24;
    /* clrrwi r6, r3, 12 */;
    tmp = tmp << 8;
    r3 = (u32)r5 >> 8;
    tmp = r4 | tmp;
    r4 = (u32)r27 >> 24;
    tmp = r3 | tmp;
    r3 = r28;
    tmp = r4 | tmp;
    tmp = tmp | r6;
    *(u32*)((u8*)r31 + 0x50) = tmp;
    pokemonBiosGetLevel();
    *(u8*)((u8*)r31 + 0x54) = r3;
    r3 = r28;
    pokemonBiosGetMailId();
    *(u8*)((u8*)r31 + 0x55) = r3;
    r3 = r28;
    pokemonBiosGetMaxHp();
    tmp = r3 & 0xFFFF;
    r3 = r28;
    r4 = tmp << 8;
    tmp = (s32)tmp >> 8;
    tmp = r4 | tmp;
    tmp = tmp & 0xFFFF;
    *(u16*)((u8*)r31 + 0x58) = tmp;
    pokemonBiosGetHp();
    tmp = r3 & 0xFFFF;
    r3 = r28;
    r4 = tmp << 8;
    tmp = (s32)tmp >> 8;
    tmp = r4 | tmp;
    tmp = tmp & 0xFFFF;
    *(u16*)((u8*)r31 + 0x56) = tmp;
    pokemonBiosGetPhyAtk();
    tmp = r3 & 0xFFFF;
    r3 = r28;
    r4 = tmp << 8;
    tmp = (s32)tmp >> 8;
    tmp = r4 | tmp;
    tmp = tmp & 0xFFFF;
    *(u16*)((u8*)r31 + 0x5A) = tmp;
    pokemonBiosGetPhyDef();
    tmp = r3 & 0xFFFF;
    r3 = r28;
    r4 = tmp << 8;
    tmp = (s32)tmp >> 8;
    tmp = r4 | tmp;
    tmp = tmp & 0xFFFF;
    *(u16*)((u8*)r31 + 0x5C) = tmp;
    pokemonBiosGetNimbleness();
    tmp = r3 & 0xFFFF;
    r3 = r28;
    r4 = tmp << 8;
    tmp = (s32)tmp >> 8;
    tmp = r4 | tmp;
    tmp = tmp & 0xFFFF;
    *(u16*)((u8*)r31 + 0x5E) = tmp;
    pokemonBiosGetSpeAtk();
    tmp = r3 & 0xFFFF;
    r3 = r28;
    r4 = tmp << 8;
    tmp = (s32)tmp >> 8;
    tmp = r4 | tmp;
    tmp = tmp & 0xFFFF;
    *(u16*)((u8*)r31 + 0x60) = tmp;
    pokemonBiosGetSpeDef();
    tmp = r3 & 0xFFFF;
    r4 = r31 + 0x20;
    r7 = r11 & 0x0000FF00;
    r5 = tmp << 8;
    r3 = (s32)tmp >> 8;
    r10 = r11 & 0x00FF0000;
    r3 = r5 | r3;
    r27 = r3 & 0xFFFF;
    r5 = r12 & 0x0000FF00;
    r8 = r12 & 0x00FF0000;
    r3 = tmp & 0x0000FF00;
    r6 = tmp & 0x00FF0000;
    r9 = r11 << 24;
    r7 = r7 << 8;
    r10 = (u32)r10 >> 8;
    r9 = r9 | r7;
    *(u16*)((u8*)r31 + 0x62) = r27;
    r11 = (u32)r11 >> 24;
    r7 = r12 << 24;
    r9 = r10 | r9;
    r5 = r5 << 8;
    r9 = r11 | r9;
    r8 = (u32)r8 >> 8;
    r7 = r7 | r5;
    *(u32*)((u8*)r31 + 0x44) = r9;
    r9 = (u32)r12 >> 24;
    r5 = tmp << 24;
    r7 = r8 | r7;
    r3 = r3 << 8;
    r7 = r9 | r7;
    r6 = (u32)r6 >> 8;
    r5 = r5 | r3;
    *(u32*)((u8*)r31 + 0x48) = r7;
    r7 = (u32)tmp >> 24;
    r3 = r4;
    tmp = r6 | r5;
    tmp = r7 | tmp;
    *(u32*)((u8*)r31 + 0x4C) = tmp;
    tmp = 0x3;
    ctr_fn = (void(*)(void))tmp;
    do {
        r5 = *(u16*)((u8*)r3 + 0x0);
        r7 = *(u16*)((u8*)r3 + 0x2);
        tmp = (s32)r5 >> 8;
        r5 = r5 << 8;
        r8 = *(u16*)((u8*)r3 + 0x4);
        r6 = r5 | tmp;
        tmp = (s32)r7 >> 8;
        r5 = r7 << 8;
        r7 = *(u16*)((u8*)r3 + 0x6);
        r11 = r6 & 0xFFFF;
        r10 = r5 | tmp;
        tmp = (s32)r8 >> 8;
        r5 = r8 << 8;
        r8 = *(u16*)((u8*)r3 + 0x8);
        r6 = r5 | tmp;
        tmp = (s32)r7 >> 8;
        r5 = r7 << 8;
        r7 = *(u16*)((u8*)r3 + 0xA);
        r9 = r5 | tmp;
        tmp = (s32)r8 >> 8;
        r5 = r8 << 8;
        r26 = *(u16*)((u8*)r3 + 0xC);
        r12 = *(u16*)((u8*)r3 + 0xE);
        r8 = r5 | tmp;
        tmp = (s32)r7 >> 8;
        r5 = r7 << 8;
        r7 = r5 | tmp;
        tmp = (s32)r26 >> 8;
        r5 = r26 << 8;
        r29 = r29 + r11;
        r11 = r10 & 0xFFFF;
        r10 = r6 & 0xFFFF;
        r29 = r29 + r11;
        r6 = r5 | tmp;
        r29 = r29 + r10;
        tmp = r9 & 0xFFFF;
        r29 = r29 + tmp;
        tmp = r8 & 0xFFFF;
        r29 = r29 + tmp;
        r7 = r7 & 0xFFFF;
        r5 = r12 << 8;
        tmp = (s32)r12 >> 8;
        tmp = r5 | tmp;
        r29 = r29 + r7;
        r5 = r6 & 0xFFFF;
        r3 = r3 + 0x10;
        r29 = r29 + r5;
        tmp = tmp & 0xFFFF;
        r29 = r29 + tmp;
    } while (--ctr != 0);
    tmp = r29 & 0xFFFF;
    r3 = tmp << 8;
    tmp = (s32)tmp >> 8;
    tmp = r3 | tmp;
    tmp = tmp & 0xFFFF;
    *(u16*)((u8*)r31 + 0x1C) = tmp;
    tmp = 0x2;
    ctr_fn = (void(*)(void))tmp;
    do {
        r3 = *(u32*)((u8*)r31 + 0x0);
        tmp = *(u32*)((u8*)r31 + 0x4);
        r5 = *(u32*)((u8*)r4 + 0x0);
        tmp = r3 ^ tmp;
        tmp = r5 ^ tmp;
        *(u32*)((u8*)r4 + 0x0) = tmp;
        r3 = *(u32*)((u8*)r31 + 0x0);
        tmp = *(u32*)((u8*)r31 + 0x4);
        r5 = *(u32*)((u8*)r4 + 0x4);
        tmp = r3 ^ tmp;
        tmp = r5 ^ tmp;
        *(u32*)((u8*)r4 + 0x4) = tmp;
        r3 = *(u32*)((u8*)r31 + 0x0);
        tmp = *(u32*)((u8*)r31 + 0x4);
        r5 = *(u32*)((u8*)r4 + 0x8);
        tmp = r3 ^ tmp;
        tmp = r5 ^ tmp;
        *(u32*)((u8*)r4 + 0x8) = tmp;
        r3 = *(u32*)((u8*)r31 + 0x0);
        tmp = *(u32*)((u8*)r31 + 0x4);
        r5 = *(u32*)((u8*)r4 + 0xC);
        tmp = r3 ^ tmp;
        tmp = r5 ^ tmp;
        *(u32*)((u8*)r4 + 0xC) = tmp;
        r3 = *(u32*)((u8*)r31 + 0x0);
        tmp = *(u32*)((u8*)r31 + 0x4);
        r5 = *(u32*)((u8*)r4 + 0x10);
        tmp = r3 ^ tmp;
        tmp = r5 ^ tmp;
        *(u32*)((u8*)r4 + 0x10) = tmp;
        r3 = *(u32*)((u8*)r31 + 0x0);
        tmp = *(u32*)((u8*)r31 + 0x4);
        r5 = *(u32*)((u8*)r4 + 0x14);
        tmp = r3 ^ tmp;
        tmp = r5 ^ tmp;
        *(u32*)((u8*)r4 + 0x14) = tmp;
        r4 = r4 + 0x18;
    } while (--ctr != 0);
    r6 = *(u32*)((u8*)r31 + 0x0);
    r3 = 0xAAAB0000;
    tmp = r6 & 0x0000FF00;
    r5 = r6 & 0x00FF0000;
    r4 = r6 << 24;
    r6 = (u32)r6 >> 24;
    tmp = tmp << 8;
    r5 = (u32)r5 >> 8;
    tmp = r4 | tmp;
    tmp = r5 | tmp;
    r27 = r6 | tmp;
    tmp = (u32)((u64)r3 * (u64)r27 >> 32);
    tmp = (u32)tmp >> 4;
    tmp = tmp * 0x18;
    r27 = r27 - tmp;
    tmp = (u32)((u64)r3 * (u64)r27 >> 32);
    tmp = (u32)tmp >> 2;
    if (tmp != 0) {
        r26 = tmp * 0xc;
        r3 = (u32)sp + 0x14;
        r5 = 0xc;
        r4 = r31 + r26;
        r4 = r4 + 0x20;
        memcpy((void*)r3, (const void*)r4, (u32)r5);
        r5 = r26;
        r3 = r31 + 0x2c;
        r4 = r31 + 0x20;
        ((void(*)(void))memmove)();
        r3 = r31 + 0x20;
        r4 = (u32)sp + 0x14;
        r5 = 0xc;
        memcpy((void*)r3, (const void*)r4, (u32)r5);
    }
    r3 = 0xAAAB0000;
    tmp = (u32)((u64)tmp * (u64)r27 >> 32);
    tmp = (u32)tmp >> 2;
    tmp = tmp * 0x6;
    r27 = r27 - tmp;
    tmp = (u32)r27 >> 1;
    if (tmp != 0) {
        r26 = tmp * 0xc;
        r3 = (u32)sp + 0x14;
        r5 = 0xc;
        r4 = r31 + r26;
        r4 = r4 + 0x2c;
        memcpy((void*)r3, (const void*)r4, (u32)r5);
        r28 = r31 + 0x2c;
        r5 = r26;
        r4 = r28;
        r3 = r31 + 0x38;
        ((void(*)(void))memmove)();
        r3 = r28;
        r4 = (u32)sp + 0x14;
        r5 = 0xc;
        memcpy((void*)r3, (const void*)r4, (u32)r5);
    }
    tmp = r27 & 0x1;
    if (tmp == 0) return;
    r26 = r31 + 0x44;
    r3 = (u32)sp + 0x14;
    r4 = r26;
    r5 = 0xc;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r27 = r31 + 0x38;
    r3 = r26;
    r4 = r27;
    r5 = 0xc;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r3 = r27;
    r4 = (u32)sp + 0x14;
    r5 = 0xc;
    memcpy((void*)r3, (const void*)r4, (u32)r5);

    return;
}

/* 0x8008BBDC | size: 0x9F8 */
void fn_8008BBDC(void) {
    extern void pokemonBiosSetLandRibbon();
    extern void pokemonBiosSetMarineRibbon();
    extern void pokemonBiosSetGanbaRibbon();
    extern void pokemonBiosSetBromideRibbon();
    extern void pokemonBiosSetVictoryRibbon();
    extern void pokemonBiosSetWinningRibbon();
    extern void pokemonBiosSetChampRibbon();
    extern void pokemonBiosSetFur();
    extern void pokemonBiosSetStrongMedal();
    extern void pokemonBiosSetCleverMedal();
    extern void pokemonBiosSetCuteMedal();
    extern void pokemonBiosSetBeautifulMedal();
    extern void pokemonBiosSetStyleMedal();
    extern void pokemonBiosSetStrong();
    extern void pokemonBiosSetClever();
    extern void pokemonBiosSetCute();
    extern void pokemonBiosSetBeautiful();
    extern void pokemonBiosSetStyle();
    extern void pokemonBiosSetFriend();
    extern void pokemonBiosSetNimblenessRnd();
    extern void pokemonBiosSetSpeDefRnd();
    extern void pokemonBiosSetSpeAtkRnd();
    extern void pokemonBiosSetPhyDefRnd();
    extern void pokemonBiosSetPhyAtkRnd();
    extern void pokemonBiosSetMaxHpRnd();
    extern void pokemonBiosSetNimblenessEffort();
    extern void pokemonBiosSetSpeDefEffort();
    extern void pokemonBiosSetSpeAtkEffort();
    extern void pokemonBiosSetPhyDefEffort();
    extern void pokemonBiosSetPhyAtkEffort();
    extern void pokemonBiosSetMaxHpEffort();
    extern void pokemonBiosSetNimbleness();
    extern void pokemonBiosSetSpeDef();
    extern void pokemonBiosSetSpeAtk();
    extern void pokemonBiosSetPhyDef();
    extern void pokemonBiosSetPhyAtk();
    extern void pokemonBiosSetMaxHp();
    extern void pokemonBiosSetHp();
    extern void pokemonBiosSetItemDataId();
    extern void pokemonBiosSetPokemonWazaPpCount();
    extern void pokemonBiosSetPokemonWazaPp();
    extern void pokemonBiosSetPokemonWazaDataId();
    extern void pokemonBiosSetConditionAmari();
    extern void pokemonBiosSetLevel();
    extern void pokemonBiosSetExp();
    extern void pokemonBiosSetNicknamePtr();
    extern void pokemonBiosSetCatchTrainerRnd();
    extern void pokemonBiosSetCatchTrainerSex();
    extern void pokemonBiosSetCatchBallId();
    extern void pokemonBiosSetCatchLevel();
    extern void pokemonBiosSetCatchFloorId();
    extern void pokemonBiosSetRnd();
    extern void pokemonBiosSetPokemonDataId();
    extern void pokemonBiosGetCatchTrainerNamePtr();
    extern void pokemonBiosGetAttest();
    extern void fn_8012173C();
    extern void fn_8012190C();
    extern void fn_801219F4();
    extern void pokemonInit();
    extern void fn_801353C0();
    extern u8 jumptable_802EEC10[];
    extern u8 jumptable_802EEC30[];
    u8 sp[0x60];
    u32 tmp = 0;
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
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r30 = r3;
    r31 = r4;
    pokemonInit();
    r3 = *(u8*)((u8*)r31 + 0x13);
    /* extrwi tmp, r3, 1, 30 */;
    if (tmp == 0) {
        tmp = r3 & 0x1;
        if (tmp == 0) return;
    }
    r3 = r31 + 0x20;
    tmp = 0x2;
    ctr_fn = (void(*)(void))tmp;
    do {
        r4 = *(u32*)((u8*)r31 + 0x0);
        tmp = *(u32*)((u8*)r31 + 0x4);
        r5 = *(u32*)((u8*)r3 + 0x0);
        tmp = r4 ^ tmp;
        tmp = r5 ^ tmp;
        *(u32*)((u8*)r3 + 0x0) = tmp;
        r4 = *(u32*)((u8*)r31 + 0x0);
        tmp = *(u32*)((u8*)r31 + 0x4);
        r5 = *(u32*)((u8*)r3 + 0x4);
        tmp = r4 ^ tmp;
        tmp = r5 ^ tmp;
        *(u32*)((u8*)r3 + 0x4) = tmp;
        r4 = *(u32*)((u8*)r31 + 0x0);
        tmp = *(u32*)((u8*)r31 + 0x4);
        r5 = *(u32*)((u8*)r3 + 0x8);
        tmp = r4 ^ tmp;
        tmp = r5 ^ tmp;
        *(u32*)((u8*)r3 + 0x8) = tmp;
        r4 = *(u32*)((u8*)r31 + 0x0);
        tmp = *(u32*)((u8*)r31 + 0x4);
        r5 = *(u32*)((u8*)r3 + 0xC);
        tmp = r4 ^ tmp;
        tmp = r5 ^ tmp;
        *(u32*)((u8*)r3 + 0xC) = tmp;
        r4 = *(u32*)((u8*)r31 + 0x0);
        tmp = *(u32*)((u8*)r31 + 0x4);
        r5 = *(u32*)((u8*)r3 + 0x10);
        tmp = r4 ^ tmp;
        tmp = r5 ^ tmp;
        *(u32*)((u8*)r3 + 0x10) = tmp;
        r4 = *(u32*)((u8*)r31 + 0x0);
        tmp = *(u32*)((u8*)r31 + 0x4);
        r5 = *(u32*)((u8*)r3 + 0x14);
        tmp = r4 ^ tmp;
        tmp = r5 ^ tmp;
        *(u32*)((u8*)r3 + 0x14) = tmp;
        r3 = r3 + 0x18;
    } while (--ctr != 0);
    r5 = *(u32*)((u8*)r31 + 0x0);
    tmp = r5 & 0x0000FF00;
    r4 = r5 & 0x00FF0000;
    r3 = r5 << 24;
    r5 = (u32)r5 >> 24;
    tmp = tmp << 8;
    r4 = (u32)r4 >> 8;
    tmp = r3 | tmp;
    tmp = r4 | tmp;
    r25 = r5 | tmp;
    tmp = r25 & 0x1;
    if (tmp != 0) {
        r26 = r31 + 0x38;
        r3 = (u32)sp + 0x14;
        r4 = r26;
        r5 = 0xc;
        memcpy((void*)r3, (const void*)r4, (u32)r5);
        r27 = r31 + 0x44;
        r3 = r26;
        r4 = r27;
        r5 = 0xc;
        memcpy((void*)r3, (const void*)r4, (u32)r5);
        r3 = r27;
        r4 = (u32)sp + 0x14;
        r5 = 0xc;
        memcpy((void*)r3, (const void*)r4, (u32)r5);
    }
    r3 = 0xAAAB0000;
    r4 = (u32)r25 >> 1;
    tmp = (u32)((u64)tmp * (u64)r4 >> 32);
    tmp = (u32)tmp >> 1;
    tmp = tmp * 0x3;
    r26 = r4 - tmp;
    if (r26 != 0) {
        r27 = r31 + 0x2c;
        r3 = (u32)sp + 0x14;
        r4 = r27;
        r5 = 0xc;
        memcpy((void*)r3, (const void*)r4, (u32)r5);
        r26 = r26 * 0xc;
        r3 = r27;
        r4 = r31 + 0x38;
        r5 = r26;
        ((void(*)(void))memmove)();
        r3 = r31 + r26;
        r4 = (u32)sp + 0x14;
        r3 = r3 + 0x2c;
        r5 = 0xc;
        memcpy((void*)r3, (const void*)r4, (u32)r5);
    }
    r3 = 0xAAAB0000;
    tmp = (u32)((u64)tmp * (u64)r25 >> 32);
    tmp = (u32)tmp >> 2;
    r25 = tmp & 0x3;
    if (r25 != 0) {
        r3 = (u32)sp + 0x14;
        r4 = r31 + 0x20;
        r5 = 0xc;
        memcpy((void*)r3, (const void*)r4, (u32)r5);
        r25 = r25 * 0xc;
        r3 = r31 + 0x20;
        r4 = r31 + 0x2c;
        r5 = r25;
        ((void(*)(void))memmove)();
        r3 = r31 + r25;
        r4 = (u32)sp + 0x14;
        r3 = r3 + 0x20;
        r5 = 0xc;
        memcpy((void*)r3, (const void*)r4, (u32)r5);
    }
    r25 = *(u32*)((u8*)r31 + 0x44);
    r3 = r30;
    tmp = *(u32*)((u8*)r31 + 0x48);
    r4 = *(u32*)((u8*)r31 + 0x4C);
    r11 = r25 & 0x0000FF00;
    r5 = *(u32*)((u8*)r31 + 0x0);
    r9 = tmp & 0x0000FF00;
    r8 = r4 & 0x0000FF00;
    r26 = r25 & 0x00FF0000;
    r29 = tmp & 0x00FF0000;
    r10 = r4 & 0x00FF0000;
    r6 = r5 & 0x0000FF00;
    r7 = r5 & 0x00FF0000;
    r27 = r25 << 24;
    r28 = r11 << 8;
    r12 = tmp << 24;
    r11 = r9 << 8;
    r9 = r4 << 24;
    r8 = r8 << 8;
    r26 = (u32)r26 >> 8;
    r28 = r27 | r28;
    r29 = (u32)r29 >> 8;
    r11 = r12 | r11;
    r10 = (u32)r10 >> 8;
    r8 = r9 | r8;
    r12 = (u32)r25 >> 24;
    r9 = r26 | r28;
    r28 = r12 | r9;
    r12 = (u32)tmp >> 24;
    r11 = r29 | r11;
    r9 = (u32)r4 >> 24;
    r8 = r10 | r8;
    r4 = r5 << 24;
    tmp = r6 << 8;
    r10 = r12 | r11;
    r8 = r9 | r8;
    r6 = (u32)r7 >> 8;
    tmp = r4 | tmp;
    r4 = (u32)r5 >> 24;
    tmp = r6 | tmp;
    r4 = r4 | tmp;
    pokemonBiosSetRnd();
    r6 = *(u32*)((u8*)r31 + 0x4);
    r3 = r30;
    tmp = r6 & 0x0000FF00;
    r5 = r6 & 0x00FF0000;
    r4 = r6 << 24;
    r6 = (u32)r6 >> 24;
    tmp = tmp << 8;
    r5 = (u32)r5 >> 8;
    tmp = r4 | tmp;
    tmp = r5 | tmp;
    r4 = r6 | tmp;
    pokemonBiosSetCatchTrainerRnd();
    tmp = *(u16*)(sp + 0x10);
    /* extrwi tmp, tmp, 4, 21 */;
    do {
        if (tmp <= 0xf) {
            r3 = (u32)jumptable_802EEC30;
            tmp = tmp << 2;
            r3 = (u32)jumptable_802EEC30;
            tmp = *(u32*)(r3 + tmp);
            ctr_fn = (void(*)(void))tmp;
            r25 = 0x8;
            break;
            r25 = 0x9;
            break;
            r25 = 0xa;
            break;
            r25 = 0x1;
            break;
            r25 = 0x2;
            break;
            r25 = 0xb;
            break;
        }
        r25 = 0x0;
    } while (0);

    tmp = *(u8*)((u8*)r31 + 0x12);
    do {
        if (tmp <= 7) {
            r3 = (u32)jumptable_802EEC10;
            tmp = tmp << 2;
            r3 = (u32)jumptable_802EEC10;
            tmp = *(u32*)(r3 + tmp);
            ctr_fn = (void(*)(void))tmp;
            r26 = 0x1;
            r27 = 0x1;
            break;
            r26 = 0x2;
            r27 = 0x2;
            break;
            r26 = 0x3;
            r27 = 0x4;
            break;
            r26 = 0x3;
            r27 = 0x5;
            break;
            r26 = 0x3;
            r27 = 0x3;
            break;
            r26 = 0x3;
            r27 = 0x6;
            break;
        }
        r26 = 0x0;
        r27 = 0x0;
    } while (0);

    r3 = r30;
    pokemonBiosGetAttest();
    r4 = r25;
    r6 = r26;
    r7 = r27;
    r5 = 0x3;
    fn_801353C0();
    r25 = r27 & 0xFF;
    r3 = (u32)sp + 0x20;
    r6 = r25;
    r4 = r31 + 0x8;
    r5 = 0xa;
    ((void(*)(void))fn_800F9C04)();
    r3 = r30;
    r4 = (u32)sp + 0x20;
    pokemonBiosSetNicknamePtr();
    tmp = *(u8*)((u8*)r31 + 0x13);
    r3 = r30;
    /* extrwi r4, tmp, 5, 24 */;
    ((void(*)(void))pokemonBiosSetFlagAmari)();
    tmp = *(u8*)((u8*)r31 + 0x13);
    r3 = r30;
    r4 = tmp & 0x1;
    ((void(*)(void))pokemonBiosSetFuseiFlag)();
    r3 = r30;
    pokemonBiosGetCatchTrainerNamePtr();
    r6 = r25;
    r4 = r31 + 0x14;
    r5 = 0x7;
    ((void(*)(void))fn_800F9C04)();
    r4 = *(u8*)((u8*)r31 + 0x1B);
    r3 = r30;
    ((void(*)(void))pokemonBiosSetPcboxMark)();
    tmp = *(u16*)((u8*)r31 + 0x1E);
    r3 = r30;
    r4 = tmp << 8;
    tmp = (s32)tmp >> 8;
    tmp = r4 | tmp;
    r4 = tmp & 0xFFFF;
    ((void(*)(void))pokemonBiosSetAmari)();
    tmp = *(u16*)((u8*)r31 + 0x20);
    r3 = r30;
    r4 = tmp << 8;
    tmp = (s32)tmp >> 8;
    tmp = r4 | tmp;
    r4 = tmp & 0xFFFF;
    pokemonBiosSetPokemonDataId();
    tmp = *(u16*)((u8*)r31 + 0x22);
    r3 = r30;
    r4 = tmp << 8;
    tmp = (s32)tmp >> 8;
    tmp = r4 | tmp;
    r4 = tmp & 0xFFFF;
    pokemonBiosSetItemDataId();
    r6 = *(u32*)((u8*)r31 + 0x24);
    r3 = r30;
    tmp = r6 & 0x0000FF00;
    r5 = r6 & 0x00FF0000;
    r4 = r6 << 24;
    r6 = (u32)r6 >> 24;
    tmp = tmp << 8;
    r5 = (u32)r5 >> 8;
    tmp = r4 | tmp;
    tmp = r5 | tmp;
    r4 = r6 | tmp;
    pokemonBiosSetExp();
    r4 = *(u8*)((u8*)r31 + 0x29);
    r3 = r30;
    pokemonBiosSetFriend();
    r4 = *(u16*)((u8*)r31 + 0x2A);
    r3 = r30;
    ((void(*)(void))pokemonBiosSetPara1Amari)();
    r27 = 0x0;
    r25 = r31;
    r26 = r27;
    do {
        tmp = *(u16*)((u8*)r25 + 0x2C);
        r3 = r30;
        r4 = r27 & 0xFFFF;
        r5 = tmp << 8;
        tmp = (s32)tmp >> 8;
        tmp = r5 | tmp;
        r5 = tmp & 0xFFFF;
        pokemonBiosSetPokemonWazaDataId();
        tmp = *(u8*)((u8*)r31 + 0x28);
        r3 = r30;
        r4 = r27 & 0xFFFF;
        tmp = (s32)tmp >> r26;
        r5 = tmp & 0x3;
        pokemonBiosSetPokemonWazaPpCount();
        tmp = r27 + 0x34;
        r3 = r30;
        r5 = *(u8*)(r31 + tmp);
        r4 = r27 & 0xFFFF;
        pokemonBiosSetPokemonWazaPp();
        r25 = r25 + 0x2;
        r26 = r26 + 0x2;
        r27 = r27 + 0x1;
    } while ((s32)r27 < 4);
    r4 = *(u8*)((u8*)r31 + 0x38);
    r3 = r30;
    pokemonBiosSetMaxHpEffort();
    r4 = *(u8*)((u8*)r31 + 0x39);
    r3 = r30;
    pokemonBiosSetPhyAtkEffort();
    r4 = *(u8*)((u8*)r31 + 0x3A);
    r3 = r30;
    pokemonBiosSetPhyDefEffort();
    r4 = *(u8*)((u8*)r31 + 0x3B);
    r3 = r30;
    pokemonBiosSetNimblenessEffort();
    r4 = *(u8*)((u8*)r31 + 0x3C);
    r3 = r30;
    pokemonBiosSetSpeAtkEffort();
    r4 = *(u8*)((u8*)r31 + 0x3D);
    r3 = r30;
    pokemonBiosSetSpeDefEffort();
    r4 = *(u8*)((u8*)r31 + 0x3E);
    r3 = r30;
    pokemonBiosSetStyle();
    r4 = *(u8*)((u8*)r31 + 0x3F);
    r3 = r30;
    pokemonBiosSetBeautiful();
    r4 = *(u8*)((u8*)r31 + 0x40);
    r3 = r30;
    pokemonBiosSetCute();
    r4 = *(u8*)((u8*)r31 + 0x41);
    r3 = r30;
    pokemonBiosSetClever();
    r4 = *(u8*)((u8*)r31 + 0x42);
    r3 = r30;
    pokemonBiosSetStrong();
    r4 = *(u8*)((u8*)r31 + 0x43);
    r3 = r30;
    pokemonBiosSetFur();
    r4 = *(u8*)(sp + 0x13);
    r3 = r30;
    ((void(*)(void))pokemonBiosSetPokerus)();
    r4 = *(u8*)(sp + 0x12);
    r3 = r30;
    pokemonBiosSetCatchFloorId();
    tmp = *(u8*)(sp + 0x11);
    r3 = r30;
    r4 = tmp & 0x7F;
    pokemonBiosSetCatchLevel();
    tmp = *(u8*)(sp + 0x10);
    r3 = r30;
    /* extrwi r4, tmp, 4, 25 */;
    pokemonBiosSetCatchBallId();
    tmp = *(u8*)(sp + 0x10);
    r3 = r30;
    /* extrwi r4, tmp, 1, 24 */;
    pokemonBiosSetCatchTrainerSex();
    tmp = *(u8*)(sp + 0xF);
    r3 = r30;
    r4 = tmp & 0x1F;
    pokemonBiosSetMaxHpRnd();
    tmp = *(u16*)(sp + 0xE);
    r3 = r30;
    /* extrwi r4, tmp, 5, 22 */;
    pokemonBiosSetPhyAtkRnd();
    tmp = *(u8*)(sp + 0xE);
    r3 = r30;
    /* extrwi r4, tmp, 5, 25 */;
    pokemonBiosSetPhyDefRnd();
    r3 = r30;
    /* extrwi r4, tmp, 5, 12 */;
    pokemonBiosSetNimblenessRnd();
    tmp = *(u16*)(sp + 0xC);
    r3 = r30;
    /* extrwi r4, tmp, 5, 23 */;
    pokemonBiosSetSpeAtkRnd();
    tmp = *(u8*)(sp + 0xC);
    r3 = r30;
    /* extrwi r4, tmp, 5, 26 */;
    pokemonBiosSetSpeDefRnd();
    tmp = *(u8*)(sp + 0xC);
    r3 = r30;
    /* extrwi r4, tmp, 1, 25 */;
    ((void(*)(void))pokemonBiosSetTamagoFlag)();
    tmp = *(u8*)(sp + 0xC);
    r3 = r30;
    /* extrwi r4, tmp, 1, 24 */;
    ((void(*)(void))pokemonBiosSetTokuseiFlag)();
    tmp = *(u8*)(sp + 0xB);
    r3 = r30;
    r4 = tmp & 0x7;
    pokemonBiosSetStyleMedal();
    tmp = *(u8*)(sp + 0xB);
    r3 = r30;
    /* extrwi r4, tmp, 3, 26 */;
    pokemonBiosSetBeautifulMedal();
    tmp = *(u16*)(sp + 0xA);
    r3 = r30;
    /* extrwi r4, tmp, 3, 23 */;
    pokemonBiosSetCuteMedal();
    tmp = *(u8*)(sp + 0xA);
    r3 = r30;
    /* extrwi r4, tmp, 3, 28 */;
    pokemonBiosSetCleverMedal();
    tmp = *(u8*)(sp + 0xA);
    r3 = r30;
    /* extrwi r4, tmp, 3, 25 */;
    pokemonBiosSetStrongMedal();
    tmp = *(u8*)(sp + 0xA);
    r3 = r30;
    /* extrwi r4, tmp, 1, 24 */;
    pokemonBiosSetChampRibbon();
    tmp = *(u8*)(sp + 0x9);
    r3 = r30;
    r4 = tmp & 0x1;
    pokemonBiosSetWinningRibbon();
    tmp = *(u8*)(sp + 0x9);
    r3 = r30;
    /* extrwi r4, tmp, 1, 30 */;
    pokemonBiosSetVictoryRibbon();
    tmp = *(u8*)(sp + 0x9);
    r3 = r30;
    /* extrwi r4, tmp, 1, 29 */;
    pokemonBiosSetBromideRibbon();
    tmp = *(u8*)(sp + 0x9);
    r3 = r30;
    /* extrwi r4, tmp, 1, 28 */;
    pokemonBiosSetGanbaRibbon();
    tmp = *(u8*)(sp + 0x9);
    r3 = r30;
    /* extrwi r4, tmp, 1, 27 */;
    pokemonBiosSetMarineRibbon();
    tmp = *(u8*)(sp + 0x9);
    r3 = r30;
    /* extrwi r4, tmp, 1, 26 */;
    pokemonBiosSetLandRibbon();
    tmp = *(u8*)(sp + 0x9);
    r3 = r30;
    /* extrwi r4, tmp, 1, 25 */;
    ((void(*)(void))pokemonBiosSetSkyRibbon)();
    tmp = *(u8*)(sp + 0x9);
    r3 = r30;
    /* extrwi r4, tmp, 1, 24 */;
    ((void(*)(void))pokemonBiosSetCountryRibbon)();
    tmp = *(u8*)(sp + 0x8);
    r3 = r30;
    r4 = tmp & 0x1;
    ((void(*)(void))pokemonBiosSetNationalRibbon)();
    tmp = *(u8*)(sp + 0x8);
    r3 = r30;
    /* extrwi r4, tmp, 1, 30 */;
    ((void(*)(void))pokemonBiosSetEarthRibbon)();
    tmp = *(u8*)(sp + 0x8);
    r3 = r30;
    /* extrwi r4, tmp, 1, 29 */;
    ((void(*)(void))pokemonBiosSetWorldRibbon)();
    tmp = *(u8*)(sp + 0x8);
    r3 = r30;
    /* extrwi r4, tmp, 4, 25 */;
    ((void(*)(void))pokemonBiosSetAmariRibbon)();
    tmp = *(u8*)(sp + 0x8);
    r3 = r30;
    /* extrwi r4, tmp, 1, 24 */;
    ((void(*)(void))pokemonBiosSetEventGetFlag)();
    r5 = *(u32*)((u8*)r31 + 0x50);
    tmp = r5 & 0x0000FF00;
    r4 = r5 & 0x00FF0000;
    r3 = r5 << 24;
    r5 = (u32)r5 >> 24;
    tmp = tmp << 8;
    r4 = (u32)r4 >> 8;
    tmp = r3 | tmp;
    tmp = r4 | tmp;
    tmp = r5 | tmp;
    r29 = tmp & 0xFFFF;
    tmp = r29 & 0x00000080;
    if ((s32)tmp != 0) {
        r3 = r30;
        r4 = 0x4;
        r5 = 0x0;
        fn_801219F4();
        tmp = r29 & 0x00000F00;
        r3 = r30;
        tmp = (s32)tmp >> 8;
        r4 = 0x4;
        r5 = (s16)tmp;
        fn_8012190C();

    } else {
        tmp = r29 & 0x00000040;
        if ((s32)tmp != 0) {
            r3 = r30;
            r4 = 0x5;
            r5 = 0x0;
            fn_801219F4();
        } else {
        tmp = r29 & 0x00000020;
        if ((s32)tmp != 0) {
            r3 = r30;
            r4 = 0x7;
            r5 = 0x0;
            fn_801219F4();
        } else {
        tmp = r29 & 0x00000010;
        if ((s32)tmp != 0) {
            r3 = r30;
            r4 = 0x6;
            r5 = 0x0;
            fn_801219F4();
        } else {
        tmp = r29 & 0x00000008;
        if ((s32)tmp != 0) {
            r3 = r30;
            r4 = 0x3;
            r5 = 0x0;
            fn_801219F4();

        } else {
            r25 = r29 & 0x7;
            if ((s32)r25 != 0) {
                r3 = r30;
                r4 = 0x8;
                r5 = 0x0;
                fn_801219F4();
                r3 = r30;
                r5 = (s8)r25;
                r4 = 0x8;
                fn_8012173C();
            }
        }
        }
        }
        }
    }
    r6 = *(u32*)((u8*)r31 + 0x50);
    r3 = r30;
    tmp = r6 & 0x0000FF00;
    r5 = r6 & 0x00FF0000;
    r4 = r6 << 24;
    r6 = (u32)r6 >> 24;
    tmp = tmp << 8;
    r5 = (u32)r5 >> 8;
    tmp = r4 | tmp;
    tmp = r5 | tmp;
    tmp = r6 | tmp;
    /* clrrwi r4, tmp, 12 */;
    pokemonBiosSetConditionAmari();
    r4 = *(u8*)((u8*)r31 + 0x54);
    r3 = r30;
    pokemonBiosSetLevel();
    r4 = *(u8*)((u8*)r31 + 0x55);
    r3 = r30;
    ((void(*)(void))pokemonBiosSetMailId)();
    tmp = *(u16*)((u8*)r31 + 0x58);
    r3 = r30;
    r4 = tmp << 8;
    tmp = (s32)tmp >> 8;
    tmp = r4 | tmp;
    r4 = tmp & 0xFFFF;
    pokemonBiosSetMaxHp();
    tmp = *(u16*)((u8*)r31 + 0x56);
    r3 = r30;
    r4 = tmp << 8;
    tmp = (s32)tmp >> 8;
    tmp = r4 | tmp;
    r4 = tmp & 0xFFFF;
    pokemonBiosSetHp();
    tmp = *(u16*)((u8*)r31 + 0x5A);
    r3 = r30;
    r4 = tmp << 8;
    tmp = (s32)tmp >> 8;
    tmp = r4 | tmp;
    r4 = tmp & 0xFFFF;
    pokemonBiosSetPhyAtk();
    tmp = *(u16*)((u8*)r31 + 0x5C);
    r3 = r30;
    r4 = tmp << 8;
    tmp = (s32)tmp >> 8;
    tmp = r4 | tmp;
    r4 = tmp & 0xFFFF;
    pokemonBiosSetPhyDef();
    tmp = *(u16*)((u8*)r31 + 0x5E);
    r3 = r30;
    r4 = tmp << 8;
    tmp = (s32)tmp >> 8;
    tmp = r4 | tmp;
    r4 = tmp & 0xFFFF;
    pokemonBiosSetNimbleness();
    tmp = *(u16*)((u8*)r31 + 0x60);
    r3 = r30;
    r4 = tmp << 8;
    tmp = (s32)tmp >> 8;
    tmp = r4 | tmp;
    r4 = tmp & 0xFFFF;
    pokemonBiosSetSpeAtk();
    tmp = *(u16*)((u8*)r31 + 0x62);
    r3 = r30;
    r4 = tmp << 8;
    tmp = (s32)tmp >> 8;
    tmp = r4 | tmp;
    r4 = tmp & 0xFFFF;
    pokemonBiosSetSpeDef();

    return;
}

/* 0x8008C5D4 | size: 0x128 */
void fn_8008C5D4(void) {
    extern void fn_8012189C();
    extern void fn_80121984();
    extern void fn_80121ADC();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    r31 = 0x0;
    r4 = 0x4;
    fn_80121ADC();
    tmp = r3 & 0xFF;
    if (tmp != 0) {
        r3 = r30;
        r4 = 0x4;
        fn_80121984();
        tmp = (s16)r3;
        tmp = tmp << 8;
        tmp = tmp | 0x80;
        r31 = tmp & 0xFFFF;
        r3 = r31;
        return;
    }
    r3 = r30;
    r4 = 0x5;
    fn_80121ADC();
    tmp = r3 & 0xFF;
    if (tmp != 0) {
        tmp = r31 | 0x40;
        r31 = tmp & 0xFFFF;
        r3 = r31;
        return;
    }
    r3 = r30;
    r4 = 0x7;
    fn_80121ADC();
    tmp = r3 & 0xFF;
    if (tmp != 0) {
        tmp = r31 | 0x20;
        r31 = tmp & 0xFFFF;
        r3 = r31;
        return;
    }
    r3 = r30;
    r4 = 0x6;
    fn_80121ADC();
    tmp = r3 & 0xFF;
    if (tmp != 0) {
        tmp = r31 | 0x10;
        r31 = tmp & 0xFFFF;
        r3 = r31;
        return;
    }
    r3 = r30;
    r4 = 0x3;
    fn_80121ADC();
    tmp = r3 & 0xFF;
    if (tmp != 0) {
        tmp = r31 | 0x8;
        r31 = tmp & 0xFFFF;
        r3 = r31;
        return;
    }
    r3 = r30;
    r4 = 0x8;
    fn_80121ADC();
    tmp = r3 & 0xFF;
    if (tmp == 0) { r3 = r31; return; }
    r3 = r30;
    r4 = 0x8;
    fn_8012189C();
    tmp = (s8)r3;
    r31 = tmp & 0xFFFF;

    r3 = r31;
    return;
}

/* 0x8008C6FC | size: 0x4 */
void fn_8008C6FC(void) {
}

/* 0x8008C700 | size: 0x8C */
#pragma push
#pragma peephole off
void GbaMisc_RunFlagDispatch(void) {
    extern s32 fn_80113F48(void);
    extern s32 fn_801906A0(s32);
    extern void _flagSet(s32, s32);
    s32 arg;
    u32 state;
    u32 offset;
    void (*handler)(s32);
    s32 nextState;

    *(u32*)&lbl_8047A694 = 0;
    *(u32*)((u8*)&lbl_8047A694 + 0x4) = 0;
    *(u32*)&lbl_8047A690 = 0;
    arg = fn_80113F48();
    state = fn_801906A0(0xb5d);
    offset = state << 2;
    handler = *(void (**)(s32))((u8*)lbl_802EEC70 + offset);
    handler(arg);
    nextState = state + 1;
    if ((u32)nextState >= 0x1f) {
        nextState = 0;
    }
    _flagSet(0xb5d, nextState);
}
#pragma pop

/* 0x8008C78C | size: 0x24 */
#pragma push
#pragma scheduling off
s32 fn_8008C78C(void) {
    extern s32 fn_801906A0(s32);
    return fn_801906A0(0xb5d);
}
#pragma pop

/* 0x8008C7B0 | size: 0x31C */
void fn_8008C7B0(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void fn_80190528();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x30];
    u32 tmp = 0;
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
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r31 = r3;
    r4 = 0xCE60000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xCE60000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r29 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r29 = r3;
        if (r29 < 1) {
            r29 = 0x1;
    }
    }
    r30 = 0x0;
    while (r30 < r29) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r30 = r30 + r3;

    }
    r3 = 0xCE60000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r30 = r3;
    r4 = 0x1;
    r3 = *(u32*)((u8*)r30 + 0x144);
    ((void(*)(void))fn_80118874)();
    tmp = 0x0;
    r4 = 0xCE60000;
    *(u32*)((u8*)r30 + 0x144) = tmp;
    r3 = r31;
    f0 = *(f32*)&lbl_8047C1D4;
    r4 = r4 + 0x1004;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x0;
    r29 = r3;
    ((void(*)(void))GSmodelSetAnimIndex)();
    r3 = r29;
    r4 = (u32)sp + 0x8;
    r5 = 0x0;
    ((void(*)(void))GSmodelGetFrameCount)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    f0 = *(f32*)&lbl_8047C1D8;
    r4 = 0x0;
    f0 = f1 - f0;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSmodelSetAnimIndex)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    ((void(*)(void))GSmodelSetAnimFrame)();
    r3 = r29;
    r4 = 0x0;
    ((void(*)(void))GSmodelSetAnimType)();
    r3 = r29;
    ((void(*)(void))GSmodelStartAnimation)();
    r3 = 0x6AF0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0xB720000;
    r28 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r27 = tmp;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r27;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0x11510000;
    r3 = r31;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r30 = r3;
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = r30;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r4 = 0xD040000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0xD0D0000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    r4 = 0xD0D0000;
    r29 = r3;
    r3 = r4 + 0x1001;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r30 = tmp;
    r4 = r28;
    r5 = r31;
    r6 = r29;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r27;
    r5 = r31;
    r6 = r30;
    r7 = 0x0;
    fn_801845E4();
    r3 = r28;
    r4 = 0x9;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r27;
    r4 = 0x6;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x8d0;
    fn_80190528();
    r3 = 0x1;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x8008CACC | size: 0x30C */
void fn_8008CACC(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r31 = r3;
    r4 = 0xCE60000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xCE60000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r29 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r29 = r3;
        if (r29 < 1) {
            r29 = 0x1;
    }
    }
    r30 = 0x0;
    while (r30 < r29) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r30 = r30 + r3;

    }
    r3 = 0xCE60000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r30 = r3;
    r4 = 0x1;
    r3 = *(u32*)((u8*)r30 + 0x144);
    ((void(*)(void))fn_80118874)();
    tmp = 0x0;
    r4 = 0xCE60000;
    *(u32*)((u8*)r30 + 0x144) = tmp;
    r3 = r31;
    f0 = *(f32*)&lbl_8047C1D4;
    r4 = r4 + 0x1004;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x0;
    r29 = r3;
    ((void(*)(void))GSmodelSetAnimIndex)();
    r3 = r29;
    r4 = (u32)sp + 0x8;
    r5 = 0x0;
    ((void(*)(void))GSmodelGetFrameCount)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    f0 = *(f32*)&lbl_8047C1D8;
    r4 = 0x0;
    f0 = f1 - f0;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSmodelSetAnimIndex)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    ((void(*)(void))GSmodelSetAnimFrame)();
    r3 = r29;
    r4 = 0x0;
    ((void(*)(void))GSmodelSetAnimType)();
    r3 = r29;
    ((void(*)(void))GSmodelStartAnimation)();
    r4 = 0x11210000;
    r3 = r31;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r4 = 0xCE60000;
    r30 = r3;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = r30;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r3 = 0xCE60000;
    r4 = 0x2;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r30 = 0x64;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1DC;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0x6BC0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r28 = tmp;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0xD020000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0xD0C0000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r6 = tmp;
    r4 = r28;
    r5 = r31;
    r7 = 0x0;
    fn_801845E4();
    r3 = r28;
    r4 = 0x4;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x89;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x8008CDD8 | size: 0x2C8 */
void fn_8008CDD8(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void scriptWaitSyncMotion();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r31 = r3;
    r4 = 0xCE60000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xCE60000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r29 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r29 = r3;
        if (r29 < 1) {
            r29 = 0x1;
    }
    }
    r30 = 0x0;
    while (r30 < r29) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r30 = r30 + r3;

    }
    r3 = 0xCE60000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r30 = r3;
    r4 = 0x1;
    r3 = *(u32*)((u8*)r30 + 0x144);
    ((void(*)(void))fn_80118874)();
    tmp = 0x0;
    r4 = 0xCE60000;
    *(u32*)((u8*)r30 + 0x144) = tmp;
    r3 = r31;
    f0 = *(f32*)&lbl_8047C1D4;
    r4 = r4 + 0x1004;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x0;
    r29 = r3;
    ((void(*)(void))GSmodelSetAnimIndex)();
    r3 = r29;
    r4 = (u32)sp + 0x8;
    r5 = 0x0;
    ((void(*)(void))GSmodelGetFrameCount)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    f0 = *(f32*)&lbl_8047C1D8;
    r4 = 0x0;
    f0 = f1 - f0;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSmodelSetAnimIndex)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    ((void(*)(void))GSmodelSetAnimFrame)();
    r3 = r29;
    r4 = 0x0;
    ((void(*)(void))GSmodelSetAnimType)();
    r3 = r29;
    ((void(*)(void))GSmodelStartAnimation)();
    r3 = 0x6AF0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r28 = tmp;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0x11510000;
    r3 = r31;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r30 = r3;
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = r30;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r4 = 0xD010000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0xD0B0000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r6 = tmp;
    r4 = r28;
    r5 = r31;
    r7 = 0x0;
    fn_801845E4();
    r3 = r28;
    r4 = 0xb;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r28;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r28;
    r4 = 0xc;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x89;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x8008D0A0 | size: 0x2A8 */
void fn_8008D0A0(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r31 = r3;
    r4 = 0xCE60000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xCE60000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r29 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r29 = r3;
        if (r29 < 1) {
            r29 = 0x1;
    }
    }
    r30 = 0x0;
    while (r30 < r29) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r30 = r30 + r3;

    }
    r3 = 0xCE60000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r30 = r3;
    r4 = 0x1;
    r3 = *(u32*)((u8*)r30 + 0x144);
    ((void(*)(void))fn_80118874)();
    tmp = 0x0;
    r4 = 0xCE60000;
    *(u32*)((u8*)r30 + 0x144) = tmp;
    r3 = r31;
    f0 = *(f32*)&lbl_8047C1D4;
    r4 = r4 + 0x1004;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x0;
    r29 = r3;
    ((void(*)(void))GSmodelSetAnimIndex)();
    r3 = r29;
    r4 = (u32)sp + 0x8;
    r5 = 0x0;
    ((void(*)(void))GSmodelGetFrameCount)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    f0 = *(f32*)&lbl_8047C1D8;
    r4 = 0x0;
    f0 = f1 - f0;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSmodelSetAnimIndex)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    ((void(*)(void))GSmodelSetAnimFrame)();
    r3 = r29;
    r4 = 0x0;
    ((void(*)(void))GSmodelSetAnimType)();
    r3 = r29;
    ((void(*)(void))GSmodelStartAnimation)();
    r3 = 0x6AF0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r28 = tmp;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0x11510000;
    r3 = r31;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r30 = r3;
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = r30;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r4 = 0xD000000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0xD0A0000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r6 = tmp;
    r4 = r28;
    r5 = r31;
    r7 = 0x0;
    fn_801845E4();
    r3 = r28;
    r4 = 0x9;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x89;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x8008D348 | size: 0x5F0 */
void fn_8008D348(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x50];
    u32 tmp = 0;
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
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r31 = r3;
    r4 = 0xCE60000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xCE60000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r22 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r22 = r3;
        if (r22 < 1) {
            r22 = 0x1;
    }
    }
    r21 = 0x0;
    while (r21 < r22) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r21 = r21 + r3;

    }
    r3 = 0xCE60000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r21 = r3;
    r4 = 0x1;
    r3 = *(u32*)((u8*)r21 + 0x144);
    ((void(*)(void))fn_80118874)();
    tmp = 0x0;
    r4 = 0xCE60000;
    *(u32*)((u8*)r21 + 0x144) = tmp;
    r3 = r31;
    f0 = *(f32*)&lbl_8047C1D4;
    r4 = r4 + 0x1004;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x0;
    r21 = r3;
    ((void(*)(void))GSmodelSetAnimIndex)();
    r3 = r21;
    r4 = (u32)sp + 0x8;
    r5 = 0x0;
    ((void(*)(void))GSmodelGetFrameCount)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r21;
    f0 = *(f32*)&lbl_8047C1D8;
    r4 = 0x0;
    f0 = f1 - f0;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSmodelSetAnimIndex)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r21;
    ((void(*)(void))GSmodelSetAnimFrame)();
    r3 = r21;
    r4 = 0x0;
    ((void(*)(void))GSmodelSetAnimType)();
    r3 = r21;
    ((void(*)(void))GSmodelStartAnimation)();
    r4 = 0x11200000;
    r3 = r31;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r4 = 0xCE60000;
    r21 = r3;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = r21;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r3 = 0x6AF0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BC0000;
    r30 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BE0000;
    r24 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BE0000;
    r25 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BE0000;
    r26 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BE0000;
    r27 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BE0000;
    r29 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r28 = tmp;
    r4 = r30;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r21 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r21;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r21;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r24;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r21 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r21;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r21;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r25;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r21 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r21;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r21;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r26;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r21 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r21;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r21;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r27;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r21 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r21;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r21;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r29;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r21 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r21;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r21;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r21 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r21;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r21;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0x11510000;
    r3 = r31;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r21 = r3;
    r3 = r31;
    r4 = r30;
    ((void(*)(void))GSresGetResource)();
    r4 = r21;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r3 = r31;
    r4 = r30;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r4 = 0xCFF0000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r22 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r22 = r3;
        if (r22 < 1) {
            r22 = 0x1;
    }
    }
    r21 = 0x0;
    while (r21 < r22) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r21 = r21 + r3;

    }
    r3 = 0xD090000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    r4 = 0xD090000;
    r23 = r3;
    r3 = r4 + 0x1001;
    fn_801CBA0C();
    r4 = 0xD090000;
    r22 = r3;
    r3 = r4 + 0x1006;
    fn_801CBA0C();
    r4 = 0xD090000;
    r21 = r3;
    r3 = r4 + 0x1002;
    fn_801CBA0C();
    r4 = 0xD090000;
    r20 = r3;
    r3 = r4 + 0x1003;
    fn_801CBA0C();
    r4 = 0xD090000;
    r19 = r3;
    r3 = r4 + 0x1004;
    fn_801CBA0C();
    r4 = 0xD090000;
    r18 = r3;
    r3 = r4 + 0x1005;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r17 = tmp;
    r4 = r30;
    r5 = r31;
    r6 = r23;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r24;
    r5 = r31;
    r6 = r22;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r25;
    r5 = r31;
    r6 = r21;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r26;
    r5 = r31;
    r6 = r20;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r27;
    r5 = r31;
    r6 = r19;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r29;
    r5 = r31;
    r6 = r18;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r28;
    r5 = r31;
    r6 = r17;
    r7 = 0x0;
    fn_801845E4();
    r3 = 0xCE60000;
    r4 = 0x1;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r30;
    r4 = 0x9;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r24;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r25;
    r4 = 0xa;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r26;
    r4 = 0xa;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r27;
    r4 = 0xa;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r29;
    r4 = 0xa;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r28;
    r4 = 0xa;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x89;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x8008D938 | size: 0x9E8 */
void fn_8008D938(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void scriptWaitSyncMotion();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x60];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r14 = 0;
    u32 r15 = 0;
    u32 r16 = 0;
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
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r15 = r3;
    r4 = 0xCE60000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xCE60000;
    r3 = r15;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xCE60000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r16 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r16 = r3;
        if (r16 < 1) {
            r16 = 0x1;
    }
    }
    r14 = 0x0;
    while (r14 < r16) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r14 = r14 + r3;

    }
    r3 = 0xCE60000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0xCE60000;
    r3 = r15;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r14 = r3;
    r4 = 0x1;
    r3 = *(u32*)((u8*)r14 + 0x144);
    ((void(*)(void))fn_80118874)();
    tmp = 0x0;
    r4 = 0xCE60000;
    *(u32*)((u8*)r14 + 0x144) = tmp;
    r3 = r15;
    f0 = *(f32*)&lbl_8047C1D4;
    r4 = r4 + 0x1004;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x0;
    r14 = r3;
    ((void(*)(void))GSmodelSetAnimIndex)();
    r3 = r14;
    r4 = (u32)sp + 0x8;
    r5 = 0x0;
    ((void(*)(void))GSmodelGetFrameCount)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r14;
    f0 = *(f32*)&lbl_8047C1D8;
    r4 = 0x0;
    f0 = f1 - f0;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSmodelSetAnimIndex)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r14;
    ((void(*)(void))GSmodelSetAnimFrame)();
    r3 = r14;
    r4 = 0x0;
    ((void(*)(void))GSmodelSetAnimType)();
    r3 = r14;
    ((void(*)(void))GSmodelStartAnimation)();
    r4 = 0x111B0000;
    r3 = r15;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r4 = 0xCE60000;
    r14 = r3;
    r3 = r15;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = r14;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r4 = 0xCE60000;
    r3 = r15;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r3 = 0xCE60000;
    r4 = 0x3;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r16 = 0x32;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1E0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r16 = r3;
        if (r16 < 1) {
            r16 = 0x1;
    }
    }
    r14 = 0x0;
    while (r14 < r16) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r14 = r14 + r3;

    }
    r4 = 0x111F0000;
    r3 = r15;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r4 = 0xCE60000;
    r14 = r3;
    r3 = r15;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = r14;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r4 = 0xCE60000;
    r3 = r15;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r3 = 0x6BC0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BE0000;
    r31 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0xD290000;
    r30 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BE0000;
    r29 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0xD240000;
    r28 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0xD290000;
    r27 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0xD240000;
    r26 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BE0000;
    r25 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0xD240000;
    r24 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    tmp = r3;
    r3 = r15;
    r23 = tmp;
    r4 = r31;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r14 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r14;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r14;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r15;
    r4 = r30;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r14 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r14;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r14;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r15;
    r4 = r29;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r14 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r14;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r14;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r15;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r14 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r14;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r14;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r15;
    r4 = r27;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r14 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r14;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r14;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r15;
    r4 = r26;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r14 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r14;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r14;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r15;
    r4 = r25;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r14 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r14;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r14;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r15;
    r4 = r24;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r14 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r14;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r14;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r15;
    r4 = r23;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r14 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r14;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r14;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0xCFE0000;
    r3 = r15;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r16 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r16 = r3;
        if (r16 < 1) {
            r16 = 0x1;
    }
    }
    r14 = 0x0;
    while (r14 < r16) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r14 = r14 + r3;

    }
    r3 = 0xD080000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    r4 = 0xD080000;
    r3 = r4 + 0x1008;
    fn_801CBA0C();
    r4 = 0xD080000;
    r14 = r3;
    r3 = r4 + 0x1001;
    fn_801CBA0C();
    r4 = 0xD080000;
    r22 = r3;
    r3 = r4 + 0x1002;
    fn_801CBA0C();
    r4 = 0xD080000;
    r21 = r3;
    r3 = r4 + 0x1003;
    fn_801CBA0C();
    r4 = 0xD080000;
    r20 = r3;
    r3 = r4 + 0x1004;
    fn_801CBA0C();
    r4 = 0xD080000;
    r19 = r3;
    r3 = r4 + 0x1005;
    fn_801CBA0C();
    r4 = 0xD080000;
    r18 = r3;
    r3 = r4 + 0x1006;
    fn_801CBA0C();
    r4 = 0xD080000;
    r17 = r3;
    r3 = r4 + 0x1007;
    fn_801CBA0C();
    tmp = r3;
    r3 = r15;
    r4 = r31;
    r16 = tmp;
    r5 = r15;
    r7 = 0x0;
    fn_801845E4();
    r3 = r15;
    r4 = r30;
    r5 = r15;
    r6 = r14;
    r7 = 0x0;
    fn_801845E4();
    r3 = r15;
    r4 = r29;
    r5 = r15;
    r6 = r22;
    r7 = 0x0;
    fn_801845E4();
    r3 = r15;
    r4 = r28;
    r5 = r15;
    r6 = r21;
    r7 = 0x0;
    fn_801845E4();
    r3 = r15;
    r4 = r27;
    r5 = r15;
    r6 = r20;
    r7 = 0x0;
    fn_801845E4();
    r3 = r15;
    r4 = r26;
    r5 = r15;
    r6 = r19;
    r7 = 0x0;
    fn_801845E4();
    r3 = r15;
    r4 = r25;
    r5 = r15;
    r6 = r18;
    r7 = 0x0;
    fn_801845E4();
    r3 = r15;
    r4 = r24;
    r5 = r15;
    r6 = r17;
    r7 = 0x0;
    fn_801845E4();
    r3 = r15;
    r4 = r23;
    r5 = r15;
    r6 = r16;
    r7 = 0x0;
    fn_801845E4();
    r3 = 0xCE60000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r31;
    r4 = 0x6;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r30;
    r4 = 0x9;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r29;
    r4 = 0xb;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r28;
    r4 = 0x9;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r27;
    r4 = 0xb;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r26;
    r4 = 0x9;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r25;
    r4 = 0xb;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r24;
    r4 = 0x9;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r24;
    r4 = 0xb;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r23;
    r4 = 0xb;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r31;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r30;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r29;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r28;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r27;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r26;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r25;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r24;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r24;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r23;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r31;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r30;
    r4 = 0xa;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r29;
    r4 = 0xc;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r28;
    r4 = 0xa;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r26;
    r4 = 0xa;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r27;
    r4 = 0xc;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r25;
    r4 = 0xc;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r24;
    r4 = 0xa;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r24;
    r4 = 0xc;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r23;
    r4 = 0xc;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r31;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r30;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r29;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r28;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r27;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r26;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r25;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r24;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r24;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r23;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r31;
    r4 = 0x6;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r30;
    r4 = 0x9;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r29;
    r4 = 0xb;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r28;
    r4 = 0x9;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r27;
    r4 = 0xb;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r26;
    r4 = 0x9;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r25;
    r4 = 0xb;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r24;
    r4 = 0x9;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r24;
    r4 = 0xb;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r23;
    r4 = 0xb;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x89;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x8008E320 | size: 0x4B4 */
void fn_8008E320(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x40];
    u32 tmp = 0;
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
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r31 = r3;
    r4 = 0xCE60000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xCE60000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r27 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r27 = r3;
        if (r27 < 1) {
            r27 = 0x1;
    }
    }
    r26 = 0x0;
    while (r26 < r27) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r26 = r26 + r3;

    }
    r3 = 0xCE60000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r26 = r3;
    r4 = 0x1;
    r3 = *(u32*)((u8*)r26 + 0x144);
    ((void(*)(void))fn_80118874)();
    tmp = 0x0;
    r4 = 0xCE60000;
    *(u32*)((u8*)r26 + 0x144) = tmp;
    r3 = r31;
    f0 = *(f32*)&lbl_8047C1D4;
    r4 = r4 + 0x1004;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x0;
    r26 = r3;
    ((void(*)(void))GSmodelSetAnimIndex)();
    r3 = r26;
    r4 = (u32)sp + 0x8;
    r5 = 0x0;
    ((void(*)(void))GSmodelGetFrameCount)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r26;
    f0 = *(f32*)&lbl_8047C1D8;
    r4 = 0x0;
    f0 = f1 - f0;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSmodelSetAnimIndex)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r26;
    ((void(*)(void))GSmodelSetAnimFrame)();
    r3 = r26;
    r4 = 0x0;
    ((void(*)(void))GSmodelSetAnimType)();
    r3 = r26;
    ((void(*)(void))GSmodelStartAnimation)();
    r4 = 0x111B0000;
    r3 = r31;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r4 = 0xCE60000;
    r26 = r3;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = r26;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r3 = 0xCE60000;
    r4 = 0x3;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r27 = 0x32;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1E0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r27 = r3;
        if (r27 < 1) {
            r27 = 0x1;
    }
    }
    r26 = 0x0;
    while (r26 < r27) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r26 = r26 + r3;

    }
    r3 = 0x6AF0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BC0000;
    r30 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BE0000;
    r29 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BE0000;
    r28 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r27 = tmp;
    r4 = r30;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r26 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r26;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r26;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r29;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r26 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r26;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r26;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r26 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r26;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r26;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r27;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r26 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r26;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r26;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0x11510000;
    r3 = r31;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r26 = r3;
    r3 = r31;
    r4 = r30;
    ((void(*)(void))GSresGetResource)();
    r4 = r26;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r3 = r31;
    r4 = r30;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r4 = 0xCFD0000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r25 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r25 = r3;
        if (r25 < 1) {
            r25 = 0x1;
    }
    }
    r26 = 0x0;
    while (r26 < r25) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r26 = r26 + r3;

    }
    r3 = 0xD070000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    r4 = 0xD070000;
    r25 = r3;
    r3 = r4 + 0x1001;
    fn_801CBA0C();
    r4 = 0xD070000;
    r26 = r3;
    r3 = r4 + 0x1003;
    fn_801CBA0C();
    r4 = 0xD070000;
    r24 = r3;
    r3 = r4 + 0x1002;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r23 = tmp;
    r4 = r30;
    r5 = r31;
    r6 = r25;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r29;
    r5 = r31;
    r6 = r26;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r28;
    r5 = r31;
    r6 = r24;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r27;
    r5 = r31;
    r6 = r23;
    r7 = 0x0;
    fn_801845E4();
    r3 = r30;
    r4 = 0xa;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r29;
    r4 = 0x5;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r28;
    r4 = 0xe;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r27;
    r4 = 0xe;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x89;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x8008E7D4 | size: 0x454 */
void fn_8008E7D4(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void scriptWaitSyncMotion();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x30];
    u32 tmp = 0;
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
    f32 f1 = 0.0f;

    r28 = r3;
    r4 = 0xCE60000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xCE60000;
    r3 = r28;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xCE60000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r29 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r29 = r3;
        if (r29 < 1) {
            r29 = 0x1;
    }
    }
    r27 = 0x0;
    while (r27 < r29) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r27 = r27 + r3;

    }
    r3 = 0xCE60000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0xCE60000;
    r3 = r28;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r27 = r3;
    r4 = 0x1;
    r3 = *(u32*)((u8*)r27 + 0x144);
    ((void(*)(void))fn_80118874)();
    tmp = 0x0;
    r4 = 0xCE60000;
    *(u32*)((u8*)r27 + 0x144) = tmp;
    r3 = r28;
    f0 = *(f32*)&lbl_8047C1D4;
    r4 = r4 + 0x1004;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x0;
    r27 = r3;
    ((void(*)(void))GSmodelSetAnimIndex)();
    r3 = r27;
    r4 = (u32)sp + 0x8;
    r5 = 0x0;
    ((void(*)(void))GSmodelGetFrameCount)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r27;
    f0 = *(f32*)&lbl_8047C1D8;
    r4 = 0x0;
    f0 = f1 - f0;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSmodelSetAnimIndex)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r27;
    ((void(*)(void))GSmodelSetAnimFrame)();
    r3 = r27;
    r4 = 0x0;
    ((void(*)(void))GSmodelSetAnimType)();
    r3 = r27;
    ((void(*)(void))GSmodelStartAnimation)();
    r4 = 0x111B0000;
    r3 = r28;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r4 = 0xCE60000;
    r27 = r3;
    r3 = r28;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = r27;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r4 = 0xCE60000;
    r3 = r28;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r3 = 0xCE60000;
    r4 = 0x3;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r29 = 0x32;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1E0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r29 = r3;
        if (r29 < 1) {
            r29 = 0x1;
    }
    }
    r27 = 0x0;
    while (r27 < r29) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r27 = r27 + r3;

    }
    r3 = 0x6AF0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BC0000;
    r31 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BE0000;
    r30 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    tmp = r3;
    r3 = r28;
    r29 = tmp;
    r4 = r31;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r27 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r27;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r27;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r28;
    r4 = r30;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r27 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r27;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r27;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r28;
    r4 = r29;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r27 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r27;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r27;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0x11510000;
    r3 = r28;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r27 = r3;
    r3 = r28;
    r4 = r31;
    ((void(*)(void))GSresGetResource)();
    r4 = r27;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r3 = r28;
    r4 = r31;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r4 = 0xCFC0000;
    r3 = r28;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r26 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r26 = r3;
        if (r26 < 1) {
            r26 = 0x1;
    }
    }
    r27 = 0x0;
    while (r27 < r26) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r27 = r27 + r3;

    }
    r3 = 0xD060000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    r4 = 0xD060000;
    r26 = r3;
    r3 = r4 + 0x1001;
    fn_801CBA0C();
    r4 = 0xD060000;
    r27 = r3;
    r3 = r4 + 0x1002;
    fn_801CBA0C();
    tmp = r3;
    r3 = r28;
    r25 = tmp;
    r4 = r31;
    r5 = r28;
    r6 = r26;
    r7 = 0x2;
    fn_801845E4();
    r3 = r28;
    r4 = r30;
    r5 = r28;
    r6 = r27;
    r7 = 0x0;
    fn_801845E4();
    r3 = r28;
    r4 = r29;
    r5 = r28;
    r6 = r25;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r31;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r31;
    r4 = 0x8;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r30;
    r4 = 0x5;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r29;
    r4 = 0xe;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x89;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x8008EC28 | size: 0x2A8 */
void fn_8008EC28(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r31 = r3;
    r4 = 0xCE60000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xCE60000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r29 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r29 = r3;
        if (r29 < 1) {
            r29 = 0x1;
    }
    }
    r30 = 0x0;
    while (r30 < r29) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r30 = r30 + r3;

    }
    r3 = 0xCE60000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r30 = r3;
    r4 = 0x1;
    r3 = *(u32*)((u8*)r30 + 0x144);
    ((void(*)(void))fn_80118874)();
    tmp = 0x0;
    r4 = 0xCE60000;
    *(u32*)((u8*)r30 + 0x144) = tmp;
    r3 = r31;
    f0 = *(f32*)&lbl_8047C1D4;
    r4 = r4 + 0x1004;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x0;
    r29 = r3;
    ((void(*)(void))GSmodelSetAnimIndex)();
    r3 = r29;
    r4 = (u32)sp + 0x8;
    r5 = 0x0;
    ((void(*)(void))GSmodelGetFrameCount)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    f0 = *(f32*)&lbl_8047C1D8;
    r4 = 0x0;
    f0 = f1 - f0;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSmodelSetAnimIndex)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    ((void(*)(void))GSmodelSetAnimFrame)();
    r3 = r29;
    r4 = 0x0;
    ((void(*)(void))GSmodelSetAnimType)();
    r3 = r29;
    ((void(*)(void))GSmodelStartAnimation)();
    r3 = 0x6AF0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r28 = tmp;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0x11510000;
    r3 = r31;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r30 = r3;
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = r30;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r4 = 0xCFB0000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0xD050000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r6 = tmp;
    r4 = r28;
    r5 = r31;
    r7 = 0x0;
    fn_801845E4();
    r3 = r28;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x89;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x8008EED0 | size: 0x2C0 */
void fn_8008EED0(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void scriptWaitSyncMotion();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r31 = r3;
    r4 = 0xCE60000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xCE60000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r29 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r29 = r3;
        if (r29 < 1) {
            r29 = 0x1;
    }
    }
    r30 = 0x0;
    while (r30 < r29) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r30 = r30 + r3;

    }
    r3 = 0xCE60000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r30 = r3;
    r4 = 0x1;
    r3 = *(u32*)((u8*)r30 + 0x144);
    ((void(*)(void))fn_80118874)();
    tmp = 0x0;
    r4 = 0xCE60000;
    *(u32*)((u8*)r30 + 0x144) = tmp;
    r3 = r31;
    f0 = *(f32*)&lbl_8047C1D4;
    r4 = r4 + 0x1004;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x0;
    r29 = r3;
    ((void(*)(void))GSmodelSetAnimIndex)();
    r3 = r29;
    r4 = (u32)sp + 0x8;
    r5 = 0x0;
    ((void(*)(void))GSmodelGetFrameCount)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    f0 = *(f32*)&lbl_8047C1D8;
    r4 = 0x0;
    f0 = f1 - f0;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSmodelSetAnimIndex)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    ((void(*)(void))GSmodelSetAnimFrame)();
    r3 = r29;
    r4 = 0x0;
    ((void(*)(void))GSmodelSetAnimType)();
    r3 = r29;
    ((void(*)(void))GSmodelStartAnimation)();
    r3 = 0x6AF0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0x4;
    r28 = r3;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0x11510000;
    r3 = r31;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r30 = r3;
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = r30;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r4 = 0xCFA0000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0xD030000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r6 = tmp;
    r4 = r28;
    r5 = r31;
    r7 = 0x0;
    fn_801845E4();
    r3 = r28;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r28;
    r4 = 0x5;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x89;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x8008F190 | size: 0x394 */
void fn_8008F190(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void scriptWaitSyncMotion();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f8 = 0.0f;
    f32 f9 = 0.0f;

    r31 = r3;
    r4 = 0xCE60000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xCE60000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r29 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r29 = r3;
        if (r29 < 1) {
            r29 = 0x1;
    }
    }
    r30 = 0x0;
    while (r30 < r29) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r30 = r30 + r3;

    }
    r3 = 0xCE60000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r30 = r3;
    r4 = 0x1;
    r3 = *(u32*)((u8*)r30 + 0x144);
    ((void(*)(void))fn_80118874)();
    tmp = 0x0;
    r4 = 0xCE60000;
    *(u32*)((u8*)r30 + 0x144) = tmp;
    r3 = r31;
    f0 = *(f32*)&lbl_8047C1D4;
    r4 = r4 + 0x1004;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x0;
    r29 = r3;
    ((void(*)(void))GSmodelSetAnimIndex)();
    r3 = r29;
    r4 = (u32)sp + 0x8;
    r5 = 0x0;
    ((void(*)(void))GSmodelGetFrameCount)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    f0 = *(f32*)&lbl_8047C1D8;
    r4 = 0x0;
    f0 = f1 - f0;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSmodelSetAnimIndex)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    ((void(*)(void))GSmodelSetAnimFrame)();
    r3 = r29;
    r4 = 0x0;
    ((void(*)(void))GSmodelSetAnimType)();
    r3 = r29;
    ((void(*)(void))GSmodelStartAnimation)();
    r4 = 0x111B0000;
    r3 = r31;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r4 = 0xCE60000;
    r30 = r3;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = r30;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r3 = 0xCE60000;
    r4 = 0x3;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r30 = 0x32;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1E0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0x6AF0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0x0;
    r28 = r3;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0x11510000;
    r3 = r31;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r30 = r3;
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = r30;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r4 = 0xCF90000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0xCF80000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r6 = tmp;
    r4 = r28;
    r5 = r31;
    r7 = 0x0;
    fn_801845E4();
    r3 = r28;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r28;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r28;
    r4 = 0x2;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r28;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r28;
    r4 = 0x3;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x89;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x8008F524 | size: 0x3F8 */
void fn_8008F524(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x30];
    u32 tmp = 0;
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
    f32 f1 = 0.0f;
    f32 f7 = 0.0f;

    r28 = r3;
    r4 = 0xCE60000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xCE60000;
    r3 = r28;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xCE60000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0xCE60000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0xCE60000;
    r3 = r28;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r29 = r3;
    r4 = 0x1;
    r3 = *(u32*)((u8*)r29 + 0x144);
    ((void(*)(void))fn_80118874)();
    tmp = 0x0;
    r4 = 0xCE60000;
    *(u32*)((u8*)r29 + 0x144) = tmp;
    r3 = r28;
    f0 = *(f32*)&lbl_8047C1D4;
    r4 = r4 + 0x1004;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x0;
    r29 = r3;
    ((void(*)(void))GSmodelSetAnimIndex)();
    r3 = r29;
    r4 = (u32)sp + 0x8;
    r5 = 0x0;
    ((void(*)(void))GSmodelGetFrameCount)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    f0 = *(f32*)&lbl_8047C1D8;
    r4 = 0x0;
    f0 = f1 - f0;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSmodelSetAnimIndex)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    ((void(*)(void))GSmodelSetAnimFrame)();
    r3 = r29;
    r4 = 0x0;
    ((void(*)(void))GSmodelSetAnimType)();
    r3 = r29;
    ((void(*)(void))GSmodelStartAnimation)();
    r4 = 0x111B0000;
    r3 = r28;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r4 = 0xCE60000;
    r29 = r3;
    r3 = r28;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = r29;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r4 = 0xCE60000;
    r3 = r28;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r3 = 0xCE60000;
    r4 = 0x3;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r30 = 0x32;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1E0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0x6BC0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BE0000;
    r31 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BE0000;
    r30 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    tmp = r3;
    r3 = r28;
    r29 = tmp;
    r4 = r31;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r27 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r27;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r27;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r28;
    r4 = r30;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r27 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r27;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r27;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r28;
    r4 = r29;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r27 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r27;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r27;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0xCF70000;
    r3 = r28;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r26 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r26 = r3;
        if (r26 < 1) {
            r26 = 0x1;
    }
    }
    r27 = 0x0;
    while (r27 < r26) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r27 = r27 + r3;

    }
    r3 = 0xCEE0000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    r4 = 0xCEE0000;
    r26 = r3;
    r3 = r4 + 0x1002;
    fn_801CBA0C();
    r4 = 0xCEE0000;
    r27 = r3;
    r3 = r4 + 0x1001;
    fn_801CBA0C();
    tmp = r3;
    r3 = r28;
    r25 = tmp;
    r4 = r31;
    r5 = r28;
    r6 = r26;
    r7 = 0x0;
    fn_801845E4();
    r3 = r28;
    r4 = r30;
    r5 = r28;
    r6 = r27;
    r7 = 0x0;
    fn_801845E4();
    r3 = r28;
    r4 = r29;
    r5 = r28;
    r6 = r25;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = 0x3;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r30;
    r4 = 0x4;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r29;
    r4 = 0x4;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x89;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x8008F91C | size: 0x2D8 */
void fn_8008F91C(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x30];
    u32 tmp = 0;
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
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f6 = 0.0f;

    r31 = r3;
    r4 = 0xCE60000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xCE60000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r29 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r29 = r3;
        if (r29 < 1) {
            r29 = 0x1;
    }
    }
    r30 = 0x0;
    while (r30 < r29) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r30 = r30 + r3;

    }
    r3 = 0xCE60000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r30 = r3;
    r4 = 0x1;
    r3 = *(u32*)((u8*)r30 + 0x144);
    ((void(*)(void))fn_80118874)();
    tmp = 0x0;
    r4 = 0xCE60000;
    *(u32*)((u8*)r30 + 0x144) = tmp;
    r3 = r31;
    f0 = *(f32*)&lbl_8047C1D4;
    r4 = r4 + 0x1004;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x0;
    r29 = r3;
    ((void(*)(void))GSmodelSetAnimIndex)();
    r3 = r29;
    r4 = (u32)sp + 0x8;
    r5 = 0x0;
    ((void(*)(void))GSmodelGetFrameCount)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    f0 = *(f32*)&lbl_8047C1D8;
    r4 = 0x0;
    f0 = f1 - f0;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSmodelSetAnimIndex)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    ((void(*)(void))GSmodelSetAnimFrame)();
    r3 = r29;
    r4 = 0x0;
    ((void(*)(void))GSmodelSetAnimType)();
    r3 = r29;
    ((void(*)(void))GSmodelStartAnimation)();
    r3 = 0x6BD0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BA0000;
    r28 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r27 = tmp;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r27;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0xCF60000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0xCED0000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    r4 = 0xCED0000;
    r30 = r3;
    r3 = r4 + 0x1001;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r29 = tmp;
    r4 = r27;
    r5 = r31;
    r6 = r30;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r28;
    r5 = r31;
    r6 = r29;
    r7 = 0x0;
    fn_801845E4();
    r3 = r27;
    r4 = 0x2;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r28;
    r4 = 0x2;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x89;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x8008FBF4 | size: 0x2A0 */
void fn_8008FBF4(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f5 = 0.0f;

    r31 = r3;
    r4 = 0xCE60000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xCE60000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r29 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r29 = r3;
        if (r29 < 1) {
            r29 = 0x1;
    }
    }
    r30 = 0x0;
    while (r30 < r29) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r30 = r30 + r3;

    }
    r3 = 0xCE60000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r30 = r3;
    r4 = 0x1;
    r3 = *(u32*)((u8*)r30 + 0x144);
    ((void(*)(void))fn_80118874)();
    tmp = 0x0;
    r4 = 0xCE60000;
    *(u32*)((u8*)r30 + 0x144) = tmp;
    r3 = r31;
    f0 = *(f32*)&lbl_8047C1D4;
    r4 = r4 + 0x1004;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x0;
    r29 = r3;
    ((void(*)(void))GSmodelSetAnimIndex)();
    r3 = r29;
    r4 = (u32)sp + 0x8;
    r5 = 0x0;
    ((void(*)(void))GSmodelGetFrameCount)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    f0 = *(f32*)&lbl_8047C1D8;
    r4 = 0x0;
    f0 = f1 - f0;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSmodelSetAnimIndex)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    ((void(*)(void))GSmodelSetAnimFrame)();
    r3 = r29;
    r4 = 0x0;
    ((void(*)(void))GSmodelSetAnimType)();
    r3 = r29;
    ((void(*)(void))GSmodelStartAnimation)();
    r3 = 0x6AF0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0x0;
    r28 = r3;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0x11510000;
    r3 = r31;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r30 = r3;
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = r30;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r4 = 0xCF50000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0xCEC0000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r6 = tmp;
    r4 = r28;
    r5 = r31;
    r7 = 0x0;
    fn_801845E4();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x89;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x8008FE94 | size: 0x26C */
void fn_8008FE94(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f4 = 0.0f;

    r31 = r3;
    r4 = 0xCE60000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xCE60000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r29 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r29 = r3;
        if (r29 < 1) {
            r29 = 0x1;
    }
    }
    r30 = 0x0;
    while (r30 < r29) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r30 = r30 + r3;

    }
    r3 = 0xCE60000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r30 = r3;
    r4 = 0x1;
    r3 = *(u32*)((u8*)r30 + 0x144);
    ((void(*)(void))fn_80118874)();
    tmp = 0x0;
    r4 = 0xCE60000;
    *(u32*)((u8*)r30 + 0x144) = tmp;
    r3 = r31;
    f0 = *(f32*)&lbl_8047C1D4;
    r4 = r4 + 0x1004;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x0;
    r29 = r3;
    ((void(*)(void))GSmodelSetAnimIndex)();
    r3 = r29;
    r4 = (u32)sp + 0x8;
    r5 = 0x0;
    ((void(*)(void))GSmodelGetFrameCount)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    f0 = *(f32*)&lbl_8047C1D8;
    r4 = 0x0;
    f0 = f1 - f0;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSmodelSetAnimIndex)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r29;
    ((void(*)(void))GSmodelSetAnimFrame)();
    r3 = r29;
    r4 = 0x0;
    ((void(*)(void))GSmodelSetAnimType)();
    r3 = r29;
    ((void(*)(void))GSmodelStartAnimation)();
    r3 = 0x6BD0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r28 = tmp;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0xCF40000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0xCEB0000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r6 = tmp;
    r4 = r28;
    r5 = r31;
    r7 = 0x0;
    fn_801845E4();
    r3 = r28;
    r4 = 0x2;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x89;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x80090100 | size: 0x620 */
void fn_80090100(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x60];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r15 = 0;
    u32 r16 = 0;
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
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f3 = 0.0f;

    r31 = r3;
    r4 = 0xCE60000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xCE60000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r18 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r18 = r3;
        if (r18 < 1) {
            r18 = 0x1;
    }
    }
    r17 = 0x0;
    while (r17 < r18) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r17 = r17 + r3;

    }
    r3 = 0xCE60000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r18 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r18 = r3;
        if (r18 < 1) {
            r18 = 0x1;
    }
    }
    r17 = 0x0;
    while (r17 < r18) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r17 = r17 + r3;

    }
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r17 = r3;
    r4 = 0x1;
    r3 = *(u32*)((u8*)r17 + 0x144);
    ((void(*)(void))fn_80118874)();
    tmp = 0x0;
    r4 = 0xCE60000;
    *(u32*)((u8*)r17 + 0x144) = tmp;
    r3 = r31;
    f0 = *(f32*)&lbl_8047C1D4;
    r4 = r4 + 0x1004;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x0;
    r17 = r3;
    ((void(*)(void))GSmodelSetAnimIndex)();
    r3 = r17;
    r4 = (u32)sp + 0x8;
    r5 = 0x0;
    ((void(*)(void))GSmodelGetFrameCount)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r17;
    f0 = *(f32*)&lbl_8047C1D8;
    r4 = 0x0;
    f0 = f1 - f0;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSmodelSetAnimIndex)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r17;
    ((void(*)(void))GSmodelSetAnimFrame)();
    r3 = r17;
    r4 = 0x0;
    ((void(*)(void))GSmodelSetAnimType)();
    r3 = r17;
    ((void(*)(void))GSmodelStartAnimation)();
    r3 = 0x6BC0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0xCEA0000;
    r23 = r3;
    r3 = r4 + 0x1000;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r22 = tmp;
    r4 = r23;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r17 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r17;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r17;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = 0xD290000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BE0000;
    r24 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0xD240000;
    r25 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BE0000;
    r26 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0xD240000;
    r27 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0xD290000;
    r30 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0xD240000;
    r29 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r28 = tmp;
    r4 = r24;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r17 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r17;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r17;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r25;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r17 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r17;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r17;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r26;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r17 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r17;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r17;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r27;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r17 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r17;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r17;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r30;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r17 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r17;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r17;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r29;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r17 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r17;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r17;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r17 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r17;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r17;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0xCF30000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r18 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r18 = r3;
        if (r18 < 1) {
            r18 = 0x1;
    }
    }
    r17 = 0x0;
    while (r17 < r18) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r17 = r17 + r3;

    }
    r3 = 0xCEA0000;
    r3 = r3 + 0x1006;
    fn_801CBA0C();
    r4 = 0xCEA0000;
    r21 = r3;
    r3 = r4 + 0x1007;
    fn_801CBA0C();
    r4 = 0xCEA0000;
    r20 = r3;
    r3 = r4 + 0x1001;
    fn_801CBA0C();
    r4 = 0xCEA0000;
    r19 = r3;
    r3 = r4 + 0x1002;
    fn_801CBA0C();
    r4 = 0xCEA0000;
    r18 = r3;
    r3 = r4 + 0x1003;
    fn_801CBA0C();
    r4 = 0xCEA0000;
    r17 = r3;
    r3 = r4 + 0x1004;
    fn_801CBA0C();
    r4 = 0xCEA0000;
    r16 = r3;
    r3 = r4 + 0x1005;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r15 = tmp;
    r4 = r23;
    r5 = r31;
    r6 = r22;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r24;
    r5 = r31;
    r6 = r21;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r25;
    r5 = r31;
    r6 = r20;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r26;
    r5 = r31;
    r6 = r19;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r27;
    r5 = r31;
    r6 = r18;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r30;
    r5 = r31;
    r6 = r17;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r29;
    r5 = r31;
    r6 = r16;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r28;
    r5 = r31;
    r6 = r15;
    r7 = 0x0;
    fn_801845E4();
    r3 = r23;
    r4 = 0x3;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r24;
    r4 = 0x4;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r25;
    r4 = 0x3;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r26;
    r4 = 0x4;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r27;
    r4 = 0x3;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r30;
    r4 = 0x5;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r29;
    r4 = 0x4;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r28;
    r4 = 0x4;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x89;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x80090720 | size: 0x2C4 */
void fn_80090720(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void scriptWaitSyncMotion();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r31 = r3;
    r4 = 0xCE60000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    f0 = *(f32*)&lbl_8047C1D4;
    r4 = 0xCE60000;
    r3 = r31;
    *(f32*)(sp + 0x8) = f0;
    r4 = r4 + 0x1004;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x1;
    r30 = r3;
    ((void(*)(void))GSmodelSetAnimIndex)();
    r3 = r30;
    r4 = (u32)sp + 0x8;
    r5 = 0x0;
    ((void(*)(void))GSmodelGetFrameCount)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r30;
    f0 = *(f32*)&lbl_8047C1D8;
    r4 = 0x1;
    f0 = f1 - f0;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSmodelSetAnimIndex)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r30;
    ((void(*)(void))GSmodelSetAnimFrame)();
    r3 = r30;
    r4 = 0x0;
    ((void(*)(void))GSmodelSetAnimType)();
    r3 = r30;
    ((void(*)(void))GSmodelStartAnimation)();
    r3 = 0xCE60000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r29 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r29 = r3;
        if (r29 < 1) {
            r29 = 0x1;
    }
    }
    r30 = 0x0;
    while (r30 < r29) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r30 = r30 + r3;

    }
    r3 = 0xCE60000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r30 = r3;
    r4 = 0x1;
    r3 = *(u32*)((u8*)r30 + 0x144);
    ((void(*)(void))fn_80118874)();
    tmp = 0x0;
    r3 = 0x6BC0000;
    *(u32*)((u8*)r30 + 0x144) = tmp;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r28 = tmp;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0xCF20000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0xCE90000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r6 = tmp;
    r4 = r28;
    r5 = r31;
    r7 = 0x0;
    fn_801845E4();
    r3 = r28;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = 0xCE60000;
    r4 = 0x0;
    r3 = r3 + 0x1004;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r28;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r28;
    r4 = 0x2;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r28;
    r4 = 0x1;
    scriptWaitSyncMotion();
    r3 = r28;
    r4 = 0x3;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x89;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x800909E4 | size: 0x350 */
void fn_800909E4(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x30];
    u32 tmp = 0;
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
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r31 = r3;
    r4 = 0xCE60000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    f0 = *(f32*)&lbl_8047C1D4;
    r4 = 0xCE60000;
    r3 = r31;
    *(f32*)(sp + 0x8) = f0;
    r4 = r4 + 0x1004;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x1;
    r30 = r3;
    ((void(*)(void))GSmodelSetAnimIndex)();
    r3 = r30;
    r4 = (u32)sp + 0x8;
    r5 = 0x0;
    ((void(*)(void))GSmodelGetFrameCount)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r30;
    f0 = *(f32*)&lbl_8047C1D8;
    r4 = 0x1;
    f0 = f1 - f0;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSmodelSetAnimIndex)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r30;
    ((void(*)(void))GSmodelSetAnimFrame)();
    r3 = r30;
    r4 = 0x0;
    ((void(*)(void))GSmodelSetAnimType)();
    r3 = r30;
    ((void(*)(void))GSmodelStartAnimation)();
    r3 = 0xCE60000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r29 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r29 = r3;
        if (r29 < 1) {
            r29 = 0x1;
    }
    }
    r30 = 0x0;
    while (r30 < r29) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r30 = r30 + r3;

    }
    r3 = 0xCE60000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0x111B0000;
    r3 = r31;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r4 = 0xCE60000;
    r30 = r3;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = r30;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r3 = 0xCE60000;
    r4 = 0x3;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r30 = 0x32;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1E0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0x6BD0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BA0000;
    r28 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r27 = tmp;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r27;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0xCF10000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0xCE80000;
    r3 = r3 + 0x1001;
    fn_801CBA0C();
    r4 = 0xCE80000;
    r29 = r3;
    r3 = r4 + 0x1000;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r30 = tmp;
    r4 = r28;
    r5 = r31;
    r6 = r29;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r27;
    r5 = r31;
    r6 = r30;
    r7 = 0x0;
    fn_801845E4();
    r3 = r28;
    r4 = 0x2;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r27;
    r4 = 0x2;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x89;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x80090D34 | size: 0x2D8 */
void fn_80090D34(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x30];
    u32 tmp = 0;
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
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r31 = r3;
    r4 = 0xCE60000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    f0 = *(f32*)&lbl_8047C1D4;
    r4 = 0xCE60000;
    r3 = r31;
    *(f32*)(sp + 0x8) = f0;
    r4 = r4 + 0x1004;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x1;
    r30 = r3;
    ((void(*)(void))GSmodelSetAnimIndex)();
    r3 = r30;
    r4 = (u32)sp + 0x8;
    r5 = 0x0;
    ((void(*)(void))GSmodelGetFrameCount)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r30;
    f0 = *(f32*)&lbl_8047C1D8;
    r4 = 0x1;
    f0 = f1 - f0;
    *(f32*)(sp + 0x8) = f0;
    ((void(*)(void))GSmodelSetAnimIndex)();
    f1 = *(f32*)(sp + 0x8);
    r3 = r30;
    ((void(*)(void))GSmodelSetAnimFrame)();
    r3 = r30;
    r4 = 0x0;
    ((void(*)(void))GSmodelSetAnimType)();
    r3 = r30;
    ((void(*)(void))GSmodelStartAnimation)();
    r3 = 0xCE60000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r29 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r29 = r3;
        if (r29 < 1) {
            r29 = 0x1;
    }
    }
    r30 = 0x0;
    while (r30 < r29) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r30 = r30 + r3;

    }
    r3 = 0xCE60000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0xCE60000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r30 = r3;
    r4 = 0x1;
    r3 = *(u32*)((u8*)r30 + 0x144);
    ((void(*)(void))fn_80118874)();
    tmp = 0x0;
    r3 = 0x6BD0000;
    *(u32*)((u8*)r30 + 0x144) = tmp;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BA0000;
    r28 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r27 = tmp;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r27;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0xCF00000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0xCE70000;
    r3 = r3 + 0x1001;
    fn_801CBA0C();
    r4 = 0xCE70000;
    r29 = r3;
    r3 = r4 + 0x1000;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r30 = tmp;
    r4 = r28;
    r5 = r31;
    r6 = r29;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r27;
    r5 = r31;
    r6 = r30;
    r7 = 0x0;
    fn_801845E4();
    r3 = r28;
    r4 = 0x2;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r27;
    r4 = 0x2;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x89;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x8009100C | size: 0x558 */
void fn_8009100C(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x60];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r14 = 0;
    u32 r15 = 0;
    u32 r16 = 0;
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
    f32 f1 = 0.0f;

    r15 = r3;
    r4 = 0x6DB0000;
    r4 = r4 + 0x1604;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0x6DB0000;
    r3 = r15;
    r4 = r4 + 0x1001;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0x6BC0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BE0000;
    r24 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0xD240000;
    r23 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0xD290000;
    r22 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BE0000;
    r21 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0xD240000;
    r20 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0xD290000;
    r19 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BE0000;
    r18 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0x6BE0000;
    r17 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    tmp = r3;
    r3 = r15;
    r16 = tmp;
    r4 = r24;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r14 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r14;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r14;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r15;
    r4 = r23;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r14 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r14;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r14;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r15;
    r4 = r22;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r14 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r14;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r14;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r15;
    r4 = r21;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r14 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r14;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r14;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r15;
    r4 = r20;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r14 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r14;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r14;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r15;
    r4 = r19;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r14 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r14;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r14;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r15;
    r4 = r18;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r14 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r14;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r14;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r15;
    r4 = r17;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r14 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r14;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r14;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r15;
    r4 = r16;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r14 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r14;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r14;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0xC390000;
    r3 = r15;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r25 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r25 = r3;
        if (r25 < 1) {
            r25 = 0x1;
    }
    }
    r14 = 0x0;
    while (r14 < r25) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r14 = r14 + r3;

    }
    r3 = 0xC380000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    r4 = 0xC380000;
    r3 = r4 + 0x1008;
    fn_801CBA0C();
    r4 = 0xC380000;
    r14 = r3;
    r3 = r4 + 0x1001;
    fn_801CBA0C();
    r4 = 0xC380000;
    r31 = r3;
    r3 = r4 + 0x1002;
    fn_801CBA0C();
    r4 = 0xC380000;
    r30 = r3;
    r3 = r4 + 0x1003;
    fn_801CBA0C();
    r4 = 0xC380000;
    r29 = r3;
    r3 = r4 + 0x1004;
    fn_801CBA0C();
    r4 = 0xC380000;
    r28 = r3;
    r3 = r4 + 0x1005;
    fn_801CBA0C();
    r4 = 0xC380000;
    r27 = r3;
    r3 = r4 + 0x1006;
    fn_801CBA0C();
    r4 = 0xC380000;
    r26 = r3;
    r3 = r4 + 0x1007;
    fn_801CBA0C();
    tmp = r3;
    r3 = r15;
    r4 = r24;
    r25 = tmp;
    r5 = r15;
    r7 = 0x0;
    fn_801845E4();
    r3 = r15;
    r4 = r23;
    r5 = r15;
    r6 = r14;
    r7 = 0x0;
    fn_801845E4();
    r3 = r15;
    r4 = r22;
    r5 = r15;
    r6 = r31;
    r7 = 0x0;
    fn_801845E4();
    r3 = r15;
    r4 = r21;
    r5 = r15;
    r6 = r30;
    r7 = 0x0;
    fn_801845E4();
    r3 = r15;
    r4 = r20;
    r5 = r15;
    r6 = r29;
    r7 = 0x0;
    fn_801845E4();
    r3 = r15;
    r4 = r19;
    r5 = r15;
    r6 = r28;
    r7 = 0x0;
    fn_801845E4();
    r3 = r15;
    r4 = r18;
    r5 = r15;
    r6 = r27;
    r7 = 0x0;
    fn_801845E4();
    r3 = r15;
    r4 = r17;
    r5 = r15;
    r6 = r26;
    r7 = 0x0;
    fn_801845E4();
    r3 = r15;
    r4 = r16;
    r5 = r15;
    r6 = r25;
    r7 = 0x0;
    fn_801845E4();
    r3 = r24;
    r4 = 0x3;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r23;
    r4 = 0x5;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r22;
    r4 = 0xe;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r21;
    r4 = 0xf;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r20;
    r4 = 0x4;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r19;
    r4 = 0x4;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r18;
    r4 = 0xe;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r17;
    r4 = 0xe;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r16;
    r4 = 0xe;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x89;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x80091564 | size: 0x210 */
void fn_80091564(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    r31 = r3;
    r4 = 0x6DC0000;
    r4 = r4 + 0x1605;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0x6DC0000;
    r3 = r31;
    r4 = r4 + 0x1001;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0x6DC0000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0x11260000;
    r3 = r31;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x6DC0000;
    r30 = r3;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = r30;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r4 = 0x6DC0000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r3 = 0x6DC0000;
    r4 = 0x4;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r29 = 0x64;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1DC;
        ((void(*)(void))__cvt_fp2unsigned)();
        r29 = r3;
        if (r29 < 1) {
            r29 = 0x1;
    }
    }
    r30 = 0x0;
    while (r30 < r29) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r30 = r30 + r3;

    }
    r3 = 0x6BD0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0x7;
    r28 = r3;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0xC420000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0xC3D0000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r6 = tmp;
    r4 = r28;
    r5 = r31;
    r7 = 0x0;
    fn_801845E4();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x81;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x80091774 | size: 0x210 */
void fn_80091774(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    r31 = r3;
    r4 = 0x6DC0000;
    r4 = r4 + 0x1605;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0x6DC0000;
    r3 = r31;
    r4 = r4 + 0x1001;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0x6DC0000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0x11250000;
    r3 = r31;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x6DC0000;
    r30 = r3;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = r30;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r4 = 0x6DC0000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r3 = 0x6DC0000;
    r4 = 0x3;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r29 = 0x64;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1DC;
        ((void(*)(void))__cvt_fp2unsigned)();
        r29 = r3;
        if (r29 < 1) {
            r29 = 0x1;
    }
    }
    r30 = 0x0;
    while (r30 < r29) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r30 = r30 + r3;

    }
    r3 = 0x6BD0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0x6;
    r28 = r3;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0xC410000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0xC3C0000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r6 = tmp;
    r4 = r28;
    r5 = r31;
    r7 = 0x0;
    fn_801845E4();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x82;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x80091984 | size: 0x210 */
void fn_80091984(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    r31 = r3;
    r4 = 0x6DC0000;
    r4 = r4 + 0x1605;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0x6DC0000;
    r3 = r31;
    r4 = r4 + 0x1001;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0x6DC0000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0x11240000;
    r3 = r31;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x6DC0000;
    r30 = r3;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = r30;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r4 = 0x6DC0000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r3 = 0x6DC0000;
    r4 = 0x2;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r29 = 0x64;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1DC;
        ((void(*)(void))__cvt_fp2unsigned)();
        r29 = r3;
        if (r29 < 1) {
            r29 = 0x1;
    }
    }
    r30 = 0x0;
    while (r30 < r29) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r30 = r30 + r3;

    }
    r3 = 0x6BD0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0x5;
    r28 = r3;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0xC400000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0xC3B0000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r6 = tmp;
    r4 = r28;
    r5 = r31;
    r7 = 0x0;
    fn_801845E4();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x82;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x80091B94 | size: 0x210 */
void fn_80091B94(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    r31 = r3;
    r4 = 0x6DC0000;
    r4 = r4 + 0x1605;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0x6DC0000;
    r3 = r31;
    r4 = r4 + 0x1001;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0x6DC0000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0x11220000;
    r3 = r31;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x6DC0000;
    r30 = r3;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = r30;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r4 = 0x6DC0000;
    r3 = r31;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r3 = 0x6DC0000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r29 = 0x64;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1DC;
        ((void(*)(void))__cvt_fp2unsigned)();
        r29 = r3;
        if (r29 < 1) {
            r29 = 0x1;
    }
    }
    r30 = 0x0;
    while (r30 < r29) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r30 = r30 + r3;

    }
    r3 = 0x6BD0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0x1;
    r28 = r3;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0xC3E0000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0x10490000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r6 = tmp;
    r4 = r28;
    r5 = r31;
    r7 = 0x0;
    fn_801845E4();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x82;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x80091DA4 | size: 0x1A4 */
void fn_80091DA4(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    r29 = r3;
    r4 = 0xCE60000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xCE60000;
    r3 = r29;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xCE60000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r3 = 0xCE60000;
    r4 = 0x1;
    r3 = r3 + 0x1004;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r4 = 0x111B0000;
    r3 = r29;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r4 = 0xCE60000;
    r31 = r3;
    r3 = r29;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = r31;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r4 = 0xCE60000;
    r3 = r29;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r3 = 0xCE60000;
    r4 = 0x3;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r30 = 0x32;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1E0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r31 = 0x0;
    while (r31 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r31 = r31 + r3;

    }
    r4 = 0xCEF0000;
    r3 = r29;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r31 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r31 = r3;
        if (r31 < 1) {
            r31 = 0x1;
    }
    }
    r30 = 0x0;
    while (r30 < r31) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r30 = r30 + r3;

    }
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x82;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x80091F48 | size: 0x1F8 */
void fn_80091F48(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void scriptWaitSyncMotion();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x20];
    u32 tmp = 0;
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
    f32 f1 = 0.0f;

    r30 = r3;
    r4 = 0x6DD0000;
    r4 = r4 + 0x1604;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0x6DD0000;
    r3 = r30;
    r4 = r4 + 0x1001;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0x6BB0000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0xD240000;
    r31 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    tmp = r3;
    r3 = r30;
    r27 = tmp;
    r4 = r31;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r30;
    r4 = r27;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r29 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r29;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r29;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0xB890000;
    r3 = r30;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r28 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r28 = r3;
        if (r28 < 1) {
            r28 = 0x1;
    }
    }
    r29 = 0x0;
    while (r29 < r28) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r29 = r29 + r3;

    }
    r3 = 0xB860000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    r4 = 0xB860000;
    r29 = r3;
    r3 = r4 + 0x1001;
    fn_801CBA0C();
    tmp = r3;
    r3 = r30;
    r28 = tmp;
    r4 = r31;
    r5 = r30;
    r6 = r29;
    r7 = 0x0;
    fn_801845E4();
    r3 = r30;
    r4 = r27;
    r5 = r30;
    r6 = r28;
    r7 = 0x0;
    fn_801845E4();
    r3 = r27;
    r4 = 0x8;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r31;
    r4 = 0x8;
    r5 = 0x32;
    r6 = 0x0;
    fn_801CB834();
    r3 = r31;
    r4 = 0x0;
    scriptWaitSyncMotion();
    r3 = r31;
    r4 = 0x9;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x89;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x80092140 | size: 0x358 */
void fn_80092140(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x40];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
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
    f32 f1 = 0.0f;

    r31 = r3;
    r4 = 0x6DD0000;
    r4 = r4 + 0x1604;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0x6DD0000;
    r3 = r31;
    r4 = r4 + 0x1001;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xD240000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0xD240000;
    r30 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0xD240000;
    r29 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0xD240000;
    r28 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0xD240000;
    r27 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r26 = tmp;
    r4 = r30;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r25 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r25;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r25;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r29;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r25 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r25;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r25;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r25 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r25;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r25;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r27;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r25 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r25;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r25;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r26;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r25 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r25;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r25;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0xB880000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r24 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r24 = r3;
        if (r24 < 1) {
            r24 = 0x1;
    }
    }
    r25 = 0x0;
    while (r25 < r24) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r25 = r25 + r3;

    }
    r3 = 0xB850000;
    r3 = r3 + 0x1004;
    fn_801CBA0C();
    r4 = 0xB850000;
    r24 = r3;
    r3 = r4 + 0x1003;
    fn_801CBA0C();
    r4 = 0xB850000;
    r25 = r3;
    r3 = r4 + 0x1001;
    fn_801CBA0C();
    r4 = 0xB850000;
    r23 = r3;
    r3 = r4 + 0x1002;
    fn_801CBA0C();
    r4 = 0xB850000;
    r22 = r3;
    r3 = r4 + 0x1003;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r21 = tmp;
    r4 = r30;
    r5 = r31;
    r6 = r24;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r29;
    r5 = r31;
    r6 = r25;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r28;
    r5 = r31;
    r6 = r23;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r27;
    r5 = r31;
    r6 = r22;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r26;
    r5 = r31;
    r6 = r21;
    r7 = 0x0;
    fn_801845E4();
    r3 = r30;
    r4 = 0x6;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r29;
    r4 = 0x6;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r28;
    r4 = 0x8;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r27;
    r4 = 0x8;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r26;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x83;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x80092498 | size: 0x1CC */
void fn_80092498(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    r29 = r3;
    r4 = 0xB630000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xB630000;
    r3 = r29;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xB630000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r31 = 0x0;
    while (r31 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r31 = r31 + r3;

    }
    r3 = 0xB630000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0xB630000;
    r3 = r29;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r31 = r3;
    r4 = 0x1;
    r3 = *(u32*)((u8*)r31 + 0x144);
    ((void(*)(void))fn_80118874)();
    tmp = 0x0;
    r4 = 0x112B0000;
    *(u32*)((u8*)r31 + 0x144) = tmp;
    r3 = r29;
    r4 = r4 + 0x1400;
    ((void(*)(void))GSresGetResource)();
    r4 = 0xB630000;
    r31 = r3;
    r3 = r29;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = r31;
    ((void(*)(void))GSmodelLinkToGSparticleBank)();
    r4 = 0xB630000;
    r3 = r29;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x4;
    ((void(*)(void))GSmodelSetGSparticleLinkAttachMode)();
    r4 = 0xB830000;
    r3 = r29;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r31 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r31 = r3;
        if (r31 < 1) {
            r31 = 0x1;
    }
    }
    r30 = 0x0;
    while (r30 < r31) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r30 = r30 + r3;

    }
    r3 = 0xB630000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x83;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x80092664 | size: 0x358 */
void fn_80092664(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x40];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
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
    f32 f1 = 0.0f;

    r31 = r3;
    r4 = 0x6DD0000;
    r4 = r4 + 0x1604;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0x6DD0000;
    r3 = r31;
    r4 = r4 + 0x1001;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xD240000;
    r3 = r3 + 0x400;
    fn_801CBA0C();
    r4 = 0xD240000;
    r30 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0xD240000;
    r29 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0xD240000;
    r28 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    r4 = 0xD240000;
    r27 = r3;
    r3 = r4 + 0x400;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r26 = tmp;
    r4 = r30;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r25 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r25;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r25;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r29;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r25 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r25;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r25;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r28;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r25 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r25;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r25;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r27;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r25 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r25;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r25;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r3 = r31;
    r4 = r26;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r25 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r25;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r25;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    r4 = 0xB870000;
    r3 = r31;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    cameraPlayAnime();
    r24 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r24 = r3;
        if (r24 < 1) {
            r24 = 0x1;
    }
    }
    r25 = 0x0;
    while (r25 < r24) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r25 = r25 + r3;

    }
    r3 = 0xB840000;
    r3 = r3 + 0x1004;
    fn_801CBA0C();
    r4 = 0xB840000;
    r24 = r3;
    r3 = r4 + 0x1000;
    fn_801CBA0C();
    r4 = 0xB840000;
    r25 = r3;
    r3 = r4 + 0x1001;
    fn_801CBA0C();
    r4 = 0xB840000;
    r23 = r3;
    r3 = r4 + 0x1002;
    fn_801CBA0C();
    r4 = 0xB840000;
    r22 = r3;
    r3 = r4 + 0x1003;
    fn_801CBA0C();
    tmp = r3;
    r3 = r31;
    r21 = tmp;
    r4 = r30;
    r5 = r31;
    r6 = r24;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r29;
    r5 = r31;
    r6 = r25;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r28;
    r5 = r31;
    r6 = r23;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r27;
    r5 = r31;
    r6 = r22;
    r7 = 0x0;
    fn_801845E4();
    r3 = r31;
    r4 = r26;
    r5 = r31;
    r6 = r21;
    r7 = 0x0;
    fn_801845E4();
    r3 = r30;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r29;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r28;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r27;
    r4 = 0x2;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r26;
    r4 = 0x2;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x87;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x800929BC | size: 0x170 */
void fn_800929BC(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801CB7C4();
    extern void fn_801CB834();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    r29 = r3;
    r4 = 0xB630000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xB630000;
    r3 = r29;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xB630000;
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    fn_801CB834();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r31 = 0x0;
    while (r31 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r31 = r31 + r3;

    }
    r3 = 0xB630000;
    r3 = r3 + 0x1000;
    fn_801CB7C4();
    r4 = 0xB630000;
    r3 = r29;
    r4 = r4 + 0x1000;
    ((void(*)(void))GSresGetResource)();
    r31 = r3;
    r4 = 0x1;
    r3 = *(u32*)((u8*)r31 + 0x144);
    ((void(*)(void))fn_80118874)();
    tmp = 0x0;
    *(u32*)((u8*)r31 + 0x144) = tmp;
    ((void(*)(void))fn_80113F48)();
    r4 = 0xB660000;
    r5 = 0x0;
    r4 = r4 + 0x1800;
    r6 = 0x0;
    cameraPlayAnime();
    r31 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r31 = r3;
        if (r31 < 1) {
            r31 = 0x1;
    }
    }
    r30 = 0x0;
    while (r30 < r31) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r30 = r30 + r3;

    }
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x83;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}

/* 0x80092B2C | size: 0x164 */
void fn_80092B2C(void) {
    extern void fn_80176B48();
    extern void cameraPlayAnime();
    extern void fn_801845E4();
    extern void fn_801CB834();
    extern void fn_801CBA0C();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    r28 = r3;
    r4 = 0xB630000;
    r4 = r4 + 0x1602;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A690 = r3;
    r4 = 0xB630000;
    r3 = r28;
    r4 = r4 + 0x1002;
    ((void(*)(void))GSresGetResource)();
    *(u32*)&lbl_8047A694 = r3;
    r3 = 0x280;
    r4 = 0x1e0;
    ((void(*)(void))GSmodelSetShadowTextureSize)();
    r3 = 0xB720000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    r4 = 0x6;
    r29 = r3;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = r28;
    r4 = r29;
    ((void(*)(void))GSresGetResource)();
    r4 = 0x2;
    r31 = r3;
    ((void(*)(void))GSmodelSetShadowFlags)();
    r4 = *(u32*)&lbl_8047A690;
    r3 = r31;
    ((void(*)(void))GSmodelSetShadowLight)();
    r3 = r31;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A694;
    ((void(*)(void))GSmodelSetShadowSurface)();
    ((void(*)(void))fn_80113F48)();
    r4 = 0xB650000;
    r5 = 0x0;
    r4 = r4 + 0x1800;
    r6 = 0x0;
    cameraPlayAnime();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 == 0x32) {
        f1 = *(f32*)&lbl_8047C1D0;
        ((void(*)(void))__cvt_fp2unsigned)();
        r30 = r3;
        if (r30 < 1) {
            r30 = 0x1;
    }
    }
    r31 = 0x0;
    while (r31 < r30) {

        ((void(*)(void))_threadSwitch)();
        ((void(*)(void))fn_800D3088)();
        r31 = r31 + r3;

    }
    r3 = 0xB730000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    tmp = r3;
    r3 = r28;
    r6 = tmp;
    r4 = r29;
    r5 = r28;
    r7 = 0x0;
    fn_801845E4();
    r3 = 0x1;
    fn_80176B48();
    r3 = 0x87;
    ((void(*)(void))fn_800FF58C)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_8011288C)();
    return;
}
