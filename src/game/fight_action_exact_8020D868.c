#include "game/colosseum.h"

void fightActionBiosSetMotoFightActionDataPtr(u8* ptr, u32 val)
{
    if (ptr == NULL) {
        return;
    }
    *(u32*)(&ptr[0x18]) = val;
}

void fightActionBiosSetBuffDataId(u8* ptr, u32 val)
{
    if (ptr == NULL) {
        return;
    }
    *(u32*)(&ptr[0x10]) = val;
}

void fightActionBiosSetBuffDataPtr(u8* ptr, u32 val)
{
    if (ptr == NULL) {
        return;
    }
    *(u32*)(&ptr[0xC]) = val;
}

void fightActionBiosSetActorFightTargetPtr(u8* ptr, u32 val)
{
    if (ptr == NULL) {
        return;
    }
    *(u32*)(&ptr[0x14]) = val;
}

void fightActionBiosSetFightActionDataPtr(u8* ptr, u32 val)
{
    if (ptr == NULL) {
        return;
    }
    *(u32*)(&ptr[0x8]) = val;
}

void fightActionBiosSetBuff(u8* ptr, u32 val)
{
    if (ptr == NULL) {
        return;
    }
    *(u32*)(&ptr[0x4]) = val;
}

void fightActionBiosSetKind(u8* ptr, u16 val)
{
    if (ptr == NULL) {
        return;
    }
    *(u16*)(&ptr[0x0]) = val;
}

u32 fightActionBiosGetBuffDataId(u8* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return *(u32*)(&ptr[0x10]);
}

u32 fightActionBiosGetBuffDataPtr(u8* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return *(u32*)(&ptr[0xC]);
}

u32 fightActionBiosGetActorFightTargetPtr(u8* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return *(u32*)(&ptr[0x14]);
}

u32 fightActionBiosGetFightActionDataPtr(u8* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return *(u32*)(&ptr[0x8]);
}

u32 fightActionBiosGetBuff(u8* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return *(u32*)(&ptr[0x4]);
}

u16 fightActionBiosGetKind(u8* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return *(u16*)(&ptr[0x0]);
}
