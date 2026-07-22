/**
 * @file people_item_tables_setters_exact_80143A94.c
 * @brief Strict item-table lookups and setters, 0x80143A94 - 0x80143C50.
 */
#include "dolphin/types.h"

extern u32 lbl_80478BE0;
extern u8 lbl_80368630[];
extern u32 lbl_80478BC8;
extern u8 lbl_80367F78[];
extern u32 lbl_80478BC0;
extern u8 lbl_80367EF0[];

u8* itemParamGetPtr(u8 idx)
{
    u8* result = &lbl_80368630[idx * 16];
    if (idx < lbl_80478BE0) {
        return result;
    }
    return NULL;
}

s8 tasteDataGetAisyou(u8* p, u16 idx)
{
    if (p == NULL) {
        return 0;
    }
    if (idx >= 0x19) {
        return 0;
    }
    return (s8)p[idx + 4];
}

u32 tasteDataGetNigateMsgDataId(u8* p)
{
    if (p == NULL) {
        return 0;
    }
    return *(u32*)p;
}

u8* tasteDataGetPtr(u16 idx)
{
    u8* result = &lbl_80367F78[idx * 32];
    if (idx < lbl_80478BC8) {
        return result;
    }
    return NULL;
}

u16 itemSoubiDataBiosGetFightKoukaDataId(u8* p)
{
    if (p == NULL) {
        return 0;
    }
    return *(u16*)p;
}

u8* itemSoubiDataBiosGetPtr(u16 idx)
{
    u8* result = &lbl_80367EF0[idx * 2];
    if (idx < lbl_80478BC0) {
        return result;
    }
    return NULL;
}

void itemBiosSetNum(u8* p, u16 val)
{
    if (p == NULL) {
        return;
    }
    *(u16*)(p + 0x2) = val;
}

void itemBiosSetItemDataId(u8* p, u16 val)
{
    if (p == NULL) {
        return;
    }
    *(u16*)(p + 0x0) = val;
}

void itemBallDataBiosSetFightKoukaDataId(u8* p, u16 val)
{
    if (p == NULL) {
        return;
    }
    *(u16*)(p + 0x0) = val;
}

void itemDataBiosSetBuff(u8* p, u32 val)
{
    if (p == NULL) {
        return;
    }
    *(u32*)(p + 0x18) = val;
}

void itemDataBiosSetUseFriend(u8* p, u16 idx, u8 val)
{
    if (p == NULL) {
        return;
    }
    if (idx >= 3) {
        return;
    }
    p[idx + 0x24] = val;
}

void itemDataBiosSetFightUseKoukaDataId(u8* p, u16 val)
{
    if (p == NULL) {
        return;
    }
    *(u16*)(p + 0xC) = val;
}

void itemDataBiosSetItemSoubiDataId(u8* p, u16 val)
{
    if (p == NULL) {
        return;
    }
    *(u16*)(p + 0xA) = val;
}

void itemDataBiosSetDoc(u8* p, u32 val)
{
    if (p == NULL) {
        return;
    }
    *(u32*)(p + 0x14) = val;
}

void itemDataBiosSetUseful(u8* p, u8 val)
{
    if (p == NULL) {
        return;
    }
    p[0x2] = val;
}

void itemDataBiosSetImportant(u8* p, u8 val)
{
    if (p == NULL) {
        return;
    }
    p[0x1] = val;
}

void itemDataBiosSetPrice(u8* p, u16 val)
{
    if (p == NULL) {
        return;
    }
    *(u16*)(p + 0x6) = val;
}

void itemDataBiosSetKind(u8* p, u8 val)
{
    if (p == NULL) {
        return;
    }
    p[0x0] = val;
}

void itemDataBiosSetName(u8* p, u32 val)
{
    if (p == NULL) {
        return;
    }
    *(u32*)(p + 0x10) = val;
}
