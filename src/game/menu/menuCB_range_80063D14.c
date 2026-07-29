/**
 * @file menuCB_range_80063D14.c
 * @brief Residual menuCB candidate range, 0x80063D14 - 0x80064378.
 */
#define MENUCB_RANGE_RESIDUAL_EMPTY_ONLY
#include "menuCB_range_80062948.c"

typedef struct MenuCBEntryPort {
    u32 unused;
    s32 enabled;
} MenuCBEntryPort;

s32 fn_80063D14(void* work)
{
    extern void fn_80165A20(s32, s32, s32);
    extern void* memcpy(void*, const void*, u32);
    extern MenuCBEntryPort* fn_8006B09C(s32);
    extern void* fn_8006A814(MenuCBEntryPort*);
    extern s32 fn_8006B0F8(s32);
    extern void gbaCommandSendWazaText(void*, s32);
    extern void fn_8008AB20(void*, u16, s32);
    extern void toolentryTaisenInitPokemonOrder(s32);
    extern void* fn_8006ACCC(s32);
    extern s32 menuOpen(s32, s32);
    extern void menuSetEnablePort(s32);
    s32 battleType;
    s32 playerCount;
    s32 player;

    fn_80165A20(0x1E, 0, 0xFF);
    memcpy(lbl_803A9F08 + 0x150, work, 0xCC2C);

    battleType = toolentryTaisenGetBattleType();
    playerCount = toolentryTaisenGetEntryPlayerNum();
    *(s32*)&lbl_803A9F08[0] = 0;
    *(s32*)&lbl_803A9F08[0xC] = 0;
    *(s32*)&lbl_803A9F08[0x2C] = 0;
    lbl_803A9F08[0xCE58] = 1;
    *(s32*)&lbl_803A9F08[0xCE5C] = -1;
    lbl_803A9F08[0xCE4C] = 0;

    for (player = 0; player < playerCount; player++) {
        MenuCBEntryPort* port = fn_8006B09C(player);
        void* command = fn_8006A814(port);
        lbl_803A9F08[player + 4] = 0;
        if (port->enabled != 0) {
            u16 count;
            gbaCommandSendWazaText(command, fn_8006B0F8(player));
            count = (u16)fn_8006B1D4();
            if ((u16)toolentryTaisenGetPokemonNum(player) < count) {
                count = (u16)toolentryTaisenGetPokemonNum(player);
            }
            fn_8008AB20(command, count, battleType == 1 ? 2 : 1);
        }
    }

    for (player = 0; player < 4; player++) {
        void* entry;

        toolentryTaisenInitPokemonOrder(player);
        entry = fn_8006ACCC(player);
        lbl_803A9F08[player + 8] =
            entry == NULL ? 0xFF : *(s8*)((u8*)entry + 0x28);
    }

    menuSetEnablePort(0);
    player = menuOpen(0xC6, 1);
    menuSetEnablePort(1);
    if (player == 0) {
        return 0xB3;
    }
    return player == 1 ? 0xB5 : -1;
}
