/**
 * @file hero_bios_exact_8012A450.c
 * @brief Exact hero status and BIOS range, 0x8012A450 - 0x8012AC9C.
 */
#include "dolphin/types.h"

typedef struct HeroPokemonData {
    u8 bytes[0x138];
} HeroPokemonData;

typedef struct HeroItemSlot {
    u16 id;
    u16 count;
} HeroItemSlot;

typedef struct HeroSaveData {
    u8 name[0x2C];
    u32 rnd;
    HeroPokemonData pokemon[6];
    HeroItemSlot normalItems[0x14];
    HeroItemSlot extraItems[0x2B];
    HeroItemSlot ballItems[0x10];
    HeroItemSlot skillItems[0x40];
    HeroItemSlot seedItems[0x2E];
    HeroItemSlot koronItems[3];
    u8 sexDataId;
    u8 homePlace;
    u8 padA82[2];
    s32 pokedoru;
    s32 pokecoupon;
    s32 pokecouponAll;
    u8 badge01Flag;
    u8 badge02Flag;
    u8 badge03Flag;
    u8 badge04Flag;
    u8 badge05Flag;
    u8 badge06Flag;
    u8 badge07Flag;
    u8 badge08Flag;
    u8 hizukiFlag;
    u8 padA99;
    HeroItemSlot hizukiItems[0xA];
    u8 hizukiName[0x56];
} HeroSaveData;

extern void GScharLenCpy();

void heroBiosSetHomePlace(u8* ptr, u8 val);
void heroBiosSetHizukiFlag(u8* ptr, u8 val);
void heroBiosSetBadge08Flag(u8* ptr, u8 val);
void heroBiosSetBadge07Flag(u8* ptr, u8 val);
void heroBiosSetBadge06Flag(u8* ptr, u8 val);
void heroBiosSetBadge05Flag(u8* ptr, u8 val);
void heroBiosSetBadge04Flag(u8* ptr, u8 val);
void heroBiosSetBadge03Flag(u8* ptr, u8 val);
void heroBiosSetBadge02Flag(u8* ptr, u8 val);
void heroBiosSetBadge01Flag(u8* ptr, u8 val);
void heroBiosSetSexDataId(u8* ptr, u8 val);
void heroBiosSetRnd(u8* ptr, u32 val);

extern void heroBiosSetPokecouponAll();
extern void heroBiosSetPokecoupon();
extern void heroBiosSetPokedoru();
extern void heroBiosSetHizukiNamePtr();
extern void heroBiosSetNamePtr();

/* 0x8012A450 | 0x160 */
void heroSetStatus(u8* ptr, u32 selector, u32 value) {
    extern u32 savedataGetStatus(u8*, u16);
    u16 status = (u16)selector;

    if (status == 0 || status >= 0x1A) { return; }
    if (ptr == NULL) {
        ptr = (u8*)savedataGetStatus(NULL, 0);
        if (ptr == NULL) { goto done; }
        ptr = (u8*)savedataGetStatus(ptr, 2);
        if (ptr == NULL) { goto done; }
    }

    switch ((u16)selector) {
    case 1:
        heroBiosSetNamePtr(ptr, (void*)value);
        break;
    case 2:
        heroBiosSetRnd(ptr, value);
        break;
    case 11:
        heroBiosSetSexDataId(ptr, (u8)value);
        break;
    case 12:
        heroBiosSetPokedoru(ptr, value);
        break;
    case 13:
        heroBiosSetPokecoupon(ptr, value);
        break;
    case 14:
        heroBiosSetPokecouponAll(ptr, value);
        break;
    case 15:
        heroBiosSetBadge01Flag(ptr, (u8)value);
        break;
    case 16:
        heroBiosSetBadge02Flag(ptr, (u8)value);
        break;
    case 17:
        heroBiosSetBadge03Flag(ptr, (u8)value);
        break;
    case 18:
        heroBiosSetBadge04Flag(ptr, (u8)value);
        break;
    case 19:
        heroBiosSetBadge05Flag(ptr, (u8)value);
        break;
    case 20:
        heroBiosSetBadge06Flag(ptr, (u8)value);
        break;
    case 21:
        heroBiosSetBadge07Flag(ptr, (u8)value);
        break;
    case 22:
        heroBiosSetBadge08Flag(ptr, (u8)value);
        break;
    case 23:
        heroBiosSetHizukiNamePtr(ptr, (void*)value);
        break;
    case 24:
        heroBiosSetHizukiFlag(ptr, (u8)value);
        break;
    case 25:
        heroBiosSetHomePlace(ptr, (u8)value);
        break;
    }
done:
    return;
}

/* 0x8012A5B0 | 0x1C4 */
u32 heroGetStatus(u8* ptr, u32 selector, u32 index) {
    extern u32 savedataGetStatus(u8*, u16);
    extern u32 heroBiosGetNamePtr(u8*);
    extern u32 heroBiosGetRnd(u8*);
    extern u32 heroBiosGetPokemonPtr(u8*, u32);
    extern u32 heroBiosGetItemNormalPtr(u8*, u32);
    extern u32 heroBiosGetExtraItemPtr(u8*, u32);
    extern u32 heroBiosGetItemBallPtr(u8*, u32);
    extern u32 heroBiosGetItemSkillPtr(u8*, u32);
    extern u32 heroBiosGetItemSeedPtr(u8*, u32);
    extern u32 heroBiosGetItemKoronPtr(u8*, u32);
    extern u32 heroBiosGetHizukiItemPtr(u8*, u32);
    extern u8 heroBiosGetSexDataId(u8*);
    extern u32 heroBiosGetPokedoru(u8*);
    extern u32 heroBiosGetPokecoupon(u8*);
    extern u32 heroBiosGetPokecouponAll(u8*);
    extern u8 heroBiosGetBadge01Flag(u8*);
    extern u8 heroBiosGetBadge02Flag(u8*);
    extern u8 heroBiosGetBadge03Flag(u8*);
    extern u8 heroBiosGetBadge04Flag(u8*);
    extern u8 heroBiosGetBadge05Flag(u8*);
    extern u8 heroBiosGetBadge06Flag(u8*);
    extern u8 heroBiosGetBadge07Flag(u8*);
    extern u8 heroBiosGetBadge08Flag(u8*);
    extern u32 heroBiosGetHizukiNamePtr(u8*);
    extern u8 heroBiosGetHizukiFlag(u8*);
    extern u8 heroBiosGetHomePlace(u8*);

    if ((u16)selector == 0) { goto invalid; }
    if ((u16)selector < 0x1A) { goto valid; }

invalid:
    return 0;

valid:
    if (ptr == NULL) {
        ptr = (u8*)savedataGetStatus(NULL, 0);
        if (ptr == NULL) { return 0; }
        ptr = (u8*)savedataGetStatus(ptr, 2);
        if (ptr == NULL) { return 0; }
    }

    switch ((u16)selector) {
    case 1:
        return heroBiosGetNamePtr(ptr);
    case 2:
        return heroBiosGetRnd(ptr);
    case 3:
        return heroBiosGetPokemonPtr(ptr, index);
    case 4:
        return heroBiosGetItemNormalPtr(ptr, index);
    case 5:
        return heroBiosGetExtraItemPtr(ptr, index);
    case 6:
        return heroBiosGetItemBallPtr(ptr, index);
    case 7:
        return heroBiosGetItemSkillPtr(ptr, index);
    case 8:
        return heroBiosGetItemSeedPtr(ptr, index);
    case 9:
        return heroBiosGetItemKoronPtr(ptr, index);
    case 10:
        return heroBiosGetHizukiItemPtr(ptr, index);
    case 11:
        return heroBiosGetSexDataId(ptr);
    case 12:
        return heroBiosGetPokedoru(ptr);
    case 13:
        return heroBiosGetPokecoupon(ptr);
    case 14:
        return heroBiosGetPokecouponAll(ptr);
    case 15:
        return heroBiosGetBadge01Flag(ptr);
    case 16:
        return heroBiosGetBadge02Flag(ptr);
    case 17:
        return heroBiosGetBadge03Flag(ptr);
    case 18:
        return heroBiosGetBadge04Flag(ptr);
    case 19:
        return heroBiosGetBadge05Flag(ptr);
    case 20:
        return heroBiosGetBadge06Flag(ptr);
    case 21:
        return heroBiosGetBadge07Flag(ptr);
    case 22:
        return heroBiosGetBadge08Flag(ptr);
    case 23:
        return heroBiosGetHizukiNamePtr(ptr);
    case 24:
        return heroBiosGetHizukiFlag(ptr);
    case 25:
        return heroBiosGetHomePlace(ptr);
    }
    return 0;
}

/* 0x8012A774 | 0x10 */
void heroBiosSetHomePlace(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ptr[0xA81] = val;
}

/* 0x8012A784 | 0x18 */
u8 heroBiosGetHomePlace(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0xA81];
}

/* 0x8012A79C | 0x18 */
u8 heroBiosGetHizukiFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0xA98];
}

/* 0x8012A7B4 | 0x10 */
void heroBiosSetHizukiFlag(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ptr[0xA98] = val;
}

/* 0x8012A7C4 | 0x18 */
u32 heroBiosGetPokecouponAll(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)&ptr[0xA8C];
}

/* 0x8012A7DC | 0x30 */
void heroBiosSetPokecouponAll(u8* ptr, s32 val) {
    if (ptr == NULL) { return; }
    if (val < 0) { val = 0; }
    if (val > 0x98967F) { val = 0x98967F; }
    *(s32*)&ptr[0xA8C] = val;
}

/* 0x8012A80C | 0x18 */
u32 heroBiosGetPokecoupon(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)&ptr[0xA88];
}

/* 0x8012A824 | 0x30 */
void heroBiosSetPokecoupon(u8* ptr, s32 val) {
    if (ptr == NULL) { return; }
    if (val < 0) { val = 0; }
    if (val > 0x98967F) { val = 0x98967F; }
    *(s32*)&ptr[0xA88] = val;
}

/* 0x8012A854 | 0x18 */
u32 heroBiosGetPokedoru(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)&ptr[0xA84];
}

/* 0x8012A86C | 0x30 */
void heroBiosSetPokedoru(u8* ptr, s32 val) {
    if (ptr == NULL) { return; }
    if (val < 0) { val = 0; }
    if (val > 0x98967F) { val = 0x98967F; }
    *(s32*)&ptr[0xA84] = val;
}

/* 0x8012A89C | 0x38 */
void heroBiosSetHizukiNamePtr(u8* ptr, void* src) {
    if (ptr == NULL) { return; }
    if (src == NULL) { return; }
    GScharLenCpy(ptr + 0xAC2, src, 0xB);
}

/* 0x8012A8D4 | 0x18 */
void* heroBiosGetHizukiNamePtr(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0xAC2;
}

/* 0x8012A8EC | 0x18 */
u8 heroBiosGetBadge08Flag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0xA97];
}

/* 0x8012A904 | 0x18 */
u8 heroBiosGetBadge07Flag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0xA96];
}

/* 0x8012A91C | 0x18 */
u8 heroBiosGetBadge06Flag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0xA95];
}

/* 0x8012A934 | 0x18 */
u8 heroBiosGetBadge05Flag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0xA94];
}

/* 0x8012A94C | 0x18 */
u8 heroBiosGetBadge04Flag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0xA93];
}

/* 0x8012A964 | 0x18 */
u8 heroBiosGetBadge03Flag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0xA92];
}

/* 0x8012A97C | 0x18 */
u8 heroBiosGetBadge02Flag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0xA91];
}

/* 0x8012A994 | 0x18 */
u8 heroBiosGetBadge01Flag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0xA90];
}

/* 0x8012A9AC | 0x10 */
void heroBiosSetBadge08Flag(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ptr[0xA97] = val;
}

/* 0x8012A9BC | 0x10 */
void heroBiosSetBadge07Flag(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ptr[0xA96] = val;
}

/* 0x8012A9CC | 0x10 */
void heroBiosSetBadge06Flag(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ptr[0xA95] = val;
}

/* 0x8012A9DC | 0x10 */
void heroBiosSetBadge05Flag(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ptr[0xA94] = val;
}

/* 0x8012A9EC | 0x10 */
void heroBiosSetBadge04Flag(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ptr[0xA93] = val;
}

/* 0x8012A9FC | 0x10 */
void heroBiosSetBadge03Flag(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ptr[0xA92] = val;
}

/* 0x8012AA0C | 0x10 */
void heroBiosSetBadge02Flag(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ptr[0xA91] = val;
}

/* 0x8012AA1C | 0x10 */
void heroBiosSetBadge01Flag(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ptr[0xA90] = val;
}

/* 0x8012AA2C | 0x18 */
u8 heroBiosGetSexDataId(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return ptr[0xA80];
}

/* 0x8012AA44 | 0x10 */
void heroBiosSetSexDataId(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ptr[0xA80] = val;
}

/* 0x8012AA54 | 0x10 */
void heroBiosSetRnd(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)&ptr[0x2C] = val;
}

/* 0x8012AA64 | 0x38 */
void heroBiosSetNamePtr(HeroSaveData* hero, void* src) {
    if (hero == NULL || src == NULL) { return; }
    GScharLenCpy(hero->name, src, 0xB);
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
    if (idx >= 3) { return NULL; }
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

/* 0x8012AC3C | 0x18 */
u32 heroBiosGetRnd(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)&ptr[0x2C];
}

/* 0x8012AC54 | 0x10 */
u32 heroBiosGetNamePtr(void* ptr) {
    if (ptr != NULL) { return (u32)ptr; }
    return 0;
}

/* 0x8012AC64 | 0x38 */
typedef struct HeroCopyData {
    u32 data[0x2C6];
} HeroCopyData;

void heroBiosCopy(u32* dst, u32* src) {
    if (dst == NULL) { return; }
    if (src == NULL) { return; }
    *(HeroCopyData*)dst = *(HeroCopyData*)src;
}
