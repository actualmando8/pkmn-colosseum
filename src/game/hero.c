/**
 * @file hero.c
 * @brief Residual savedata candidate range, 0x80128E38 - 0x80129280.
 */
#include "dolphin/types.h"

extern void GScharMakeFromSJIS(u32* output, const u8* sjis);
extern void gamedataCreate(void* data, u8 a, u8 b, u8 c, u8 d);
extern void gamedatasaveSetStatus(void* data, u16 kind, u32 value);
extern void heroCreate(void* data, u32 trainerId, u8 sex);
extern void heroPokemonGetBlacky(void* data, u32 index);
extern void heroPokemonGetEifie(void* data, u32 index);
extern void memoDataSet(u32 index, void* pokemon);
extern void* heroBiosGetPokemonPtr(void* data, u32 index);
extern void heroSetStatus(void* data, u32 kind, u32 value);
extern void heroItemAddItemDataId(void* data, u32 item, u32 count, s32 slot);
extern s32 fn_800057A0(void);
extern const u8 lbl_8047D028[8];
extern s32 fn_80128DEC(void);
extern s32 fn_80128E04(void);
extern u32 fn_80128E24(void);
void savedataInit(u32 slot);

/* Create the initial game and hero records for a saved-data slot. */
void savedataCreate(u32 slot, const u32* trainerId)
{
    void* gameData;
    void* heroData;
    u32 defaultTrainerId;
    s32 version;

    savedataInit(slot);
    gameData = slot == 0 && fn_80128E24() == 0
                   ? NULL
                   : (void*)fn_80128E04();

    version = fn_800057A0();
    switch (version) {
    case 0:
        gamedataCreate(gameData, 0xB, 3, 1, 1);
        break;
    case 1:
        gamedataCreate(gameData, 0xB, 3, 2, 2);
        break;
    case 2:
        gamedataCreate(gameData, 0xB, 3, 3, 8);
        break;
    }

    heroData = slot == 0 && fn_80128E24() == 0
                   ? NULL
                   : (void*)fn_80128DEC();
    if (trainerId == NULL) {
        GScharMakeFromSJIS(&defaultTrainerId, lbl_8047D028);
        trainerId = &defaultTrainerId;
    }
    heroCreate(heroData, (u32)trainerId, 0);
    gamedatasaveSetStatus(gameData, 5, 2);
    gamedatasaveSetStatus(gameData, 7, 1);
    gamedatasaveSetStatus(gameData, 8, 1);
    heroPokemonGetBlacky(heroData, 0);
    heroPokemonGetEifie(heroData, 0);
    memoDataSet(0, heroBiosGetPokemonPtr(heroData, 0));
    memoDataSet(0, heroBiosGetPokemonPtr(heroData, 1));
    heroSetStatus(heroData, 0xC, 0x2710);
    heroItemAddItemDataId(heroData, 0x16, 2, -1);
    heroItemAddItemDataId(heroData, 0xD, 5, -1);
    heroItemAddItemDataId(heroData, 0xE, 2, -1);
    heroItemAddItemDataId(heroData, 0xF, 2, -1);
    heroItemAddItemDataId(heroData, 0x10, 2, -1);
    heroItemAddItemDataId(heroData, 0x12, 2, -1);
    heroItemAddItemDataId(heroData, 0x11, 2, -1);
    heroItemAddItemDataId(heroData, 0x17, 2, -1);
}

extern void GSflagClear(u32 flag);
extern void gamedataInit(s32 data);
extern void heroInit(s32 data);
extern void pcboxInit(s32 data);
extern void mailInitMailbox(s32 data);
extern void sodateyaInit(s32 data);
extern void memoInit(s32 data);
extern void exribbonInit(s32 data);
extern void fn_8006B6B4(s32 data);
extern void fn_80083CBC(s32 data);
extern void fn_801EF128(s32 data);
extern s32 fn_80128CC0(void);
extern s32 fn_80128CDC(void);
extern s32 fn_80128CF8(void);
extern s32 fn_80128D14(void);
extern s32 fn_80128D30(void);
extern s32 fn_80128D4C(void);
extern s32 fn_80128D68(void);
extern s32 fn_80128DD4(void);
extern s32 fn_80128DEC(void);
extern s32 fn_80128E04(void);
extern u32 fn_80128E24(void);

/* Initialize each saved-data subsystem from the selected slot. */
void savedataInit(u32 slot)
{
    s32 data;

    data = slot == 0 && fn_80128E24() == 0 ? 0 : fn_80128E04();
    gamedataInit(data);
    data = slot == 0 && fn_80128E24() == 0 ? 0 : fn_80128DEC();
    heroInit(data);
    data = slot == 0 && fn_80128E24() == 0 ? 0 : fn_80128DD4();
    pcboxInit(data);

    if (slot == 0 || slot == fn_80128E24()) {
        GSflagClear(1);
        GSflagClear(2);
        GSflagClear(3);
    }

    data = slot == 0 && fn_80128E24() == 0 ? 0 : fn_80128D68();
    mailInitMailbox(data);
    data = slot == 0 && fn_80128E24() == 0 ? 0 : fn_80128D4C();
    sodateyaInit(data);
    data = slot == 0 && fn_80128E24() == 0 ? 0 : fn_80128CF8();
    fn_8006B6B4(data);
    data = slot == 0 && fn_80128E24() == 0 ? 0 : fn_80128D30();
    memoInit(data);
    data = slot == 0 && fn_80128E24() == 0 ? 0 : fn_80128D14();
    fn_80083CBC(data);
    data = slot == 0 && fn_80128E24() == 0 ? 0 : fn_80128CDC();
    fn_801EF128(data);
    data = slot == 0 && fn_80128E24() == 0 ? 0 : fn_80128CC0();
    exribbonInit(data);
}
