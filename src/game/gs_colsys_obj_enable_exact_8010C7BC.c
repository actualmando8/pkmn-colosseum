#include "dolphin/types.h"
#include "game/gs_colsys.h"

extern GSColSysState lbl_80404C68;

s32 GScolsys2GetObjEnable(s32 triIndex, u32* outResult)
{
    u8* base = (u8*)&lbl_80404C68;
    void* wzx = lbl_80404C68.wzxDataPtr;
    u8* entry;
    s32 result;

    if (wzx == NULL) {
        result = 1;
    } else if (triIndex < 0 || triIndex >= *(u32*)((u8*)wzx + 4)) {
        result = 2;
    } else {
        u8* triangle = base + lbl_80404C68.activeLayer * GSCOLSYS_LAYER_SIZE;
        triangle = triangle + 4;
        triangle = triangle + triIndex * GSCOLSYS_TRI_ENTRY_SIZE;
        entry = triangle;
        result = 0;
    }
    if (result != 0) {
        return result;
    }
    if (*(u16*)(entry + 0x24) & 1) {
        *outResult = 0;
    } else {
        *outResult = 1;
    }
    return 0;
}

s32 GScolsys2SetObjEnable(s32 triIndex, s32 visible)
{
    u8* base = (u8*)&lbl_80404C68;
    void* wzx = lbl_80404C68.wzxDataPtr;
    u8* entry;
    u16 flags;
    s32 result;

    if (wzx == NULL) {
        result = 1;
    } else if (triIndex < 0 || triIndex >= *(u32*)((u8*)wzx + 4)) {
        result = 2;
    } else {
        u8* triangle = base + lbl_80404C68.activeLayer * GSCOLSYS_LAYER_SIZE;
        triangle = triangle + 4;
        triangle = triangle + triIndex * GSCOLSYS_TRI_ENTRY_SIZE;
        entry = triangle;
        result = 0;
    }
    if (result != 0) {
        return result;
    }
    if (visible != 0) {
        flags = *(u16*)(entry + 0x24);
        flags &= 0xFFFE;
        *(u16*)(entry + 0x24) = flags;
    } else {
        flags = *(u16*)(entry + 0x24);
        flags |= 1;
        *(u16*)(entry + 0x24) = flags;
    }
    return 0;
}
