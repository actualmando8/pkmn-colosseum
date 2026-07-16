/**
 * @file menuCB_Battle.c
 * @brief menuCB_Battle.cpp, 0x80069A60 - 0x80069C0C.
 *
 * Split out of the former game/menu/menuCB_Battle.c bucket (2026-07-07) into
 * true XD source-unit segments; this final range keeps the original bucket
 * name since it is the only segment with direct filename proof. XD:
 * menuCB_Battle / menuCB_InitBattle / _menuCBBattle_DeleteAllItem /
 * _menuCB_Flash* locals (XD 0x8004D394-0x8004DE74).
 *
 * DIRECT filename proof: fn_80069A60 calls __assert with file string
 * 'menuCB_Battle.c' (.data 0x80267C94) and conds
 * 'FIGHT_ENCOUNT_DATA_null != nFightEncountID',
 * '_LENGTH(staColosseum)>p->m_eColosseum'; calls
 * fightEncountDataBiosGetPtr/SetFightTrainerDataId = battle-encounter setup.
 */
#include "dolphin/types.h"

typedef struct MenuCBBattleSetup {
    s32 battleMode;
    s32 selectedRule;
    u8 pad08[0x14];
    u8 active;
    u8 pad1D[7];
    u16 trainerDataId;
    u8 pad26[0x5982];
    u16 savedTrainerDataId;
} MenuCBBattleSetup;

extern u8 lbl_80267C94[];
extern u8 lbl_80267CA4[];
extern u8 lbl_8047C030;

extern u8* savedataGetStatus(u32 file, u32 index);
extern void* fightTrainerDataBiosGetPtr(u16 index);
extern void fightTrainerDataBiosSetKindDataId(void* trainer, u16 kind);
extern void* fightEncountDataBiosGetPtr();
extern void fightEncountDataBiosSetFightTrainerDataId(void* encounter, u32 slot,
                                                       u16 trainer);
extern void* memcpy(void* dst, const void* src, u32 size);
extern void __assert(const char* file, s32 line, const char* condition);
extern void fn_801903B0(u32 id);
extern u16 fn_8020DAD0();

#pragma push
#pragma peephole off
u32 menuCB_Battle(MenuCBBattleSetup* setup)
{
    MenuCBBattleSetup* p = setup;
    s32 encounter = 1;
    s32 current;
    void* trainer;
    void* source;

    savedataGetStatus(0, 14)[0x1C] = 1;

    switch (p->battleMode) {
    case 0:
    case 1:
        encounter = 0;
        source = fightTrainerDataBiosGetPtr(p->savedTrainerDataId);
        trainer = fightTrainerDataBiosGetPtr(0x333);
        memcpy(trainer, source, 0x34);
        fightTrainerDataBiosSetKindDataId(trainer, 1);
        p->trainerDataId = 0x333;

        switch (p->selectedRule) {
        case 0:
            encounter = 0x20A;
            break;
        case 1:
            encounter = 0x20B;
            break;
        default:
            __assert((const char*)lbl_80267C94, 0x2D7,
                     (const char*)&lbl_8047C030);
            break;
        }

        if ((u16)encounter == 0) {
            __assert((const char*)lbl_80267C94, 0x2DA,
                     (const char*)lbl_80267CA4);
        }
        fightEncountDataBiosSetFightTrainerDataId(
            fightEncountDataBiosGetPtr(encounter), 0, p->trainerDataId);
        break;
    }

    switch (p->selectedRule) {
    case 0:
        current = 0x20A;
        break;
    case 1:
        current = 0x20B;
        break;
    case 2:
        current = 0x20C;
        break;
    }

    fn_801903B0(0xE05);
    encounter = fn_8020DAD0(current);

    switch (p->battleMode) {
    case 0:
    case 1:
        p->trainerDataId =
            *(u16*)(savedataGetStatus(0, 14) + 0x59A8);
        break;
    }

    p->active = 0;
    return encounter;
}
#pragma pop
