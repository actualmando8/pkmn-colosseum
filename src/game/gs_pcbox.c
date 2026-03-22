/**
 * @file gs_pcbox.c
 * @brief GSpcbox -- PC Box Pokemon storage system UI.
 *
 * Address range: 0x800181C4 - 0x80020328 (~60 functions)
 *
 * This module implements the PC Box storage system where players can
 * deposit, withdraw, and organize their Pokemon collection. It handles:
 *   - Box list display with Pokemon icons
 *   - Deposit/withdraw operations
 *   - Move between boxes
 *   - Box wallpaper/name customization
 *   - Multi-select operations
 *   - Party <==> Box transfers
 *
 * Key functions:
 *   fn_800181C4  GSpcbox_SelectPokemon       -- 0x3D0 bytes, Pokemon selection in box
 *   fn_80018594  GSpcbox_DrawBoxGrid          -- 0x34C bytes, render box grid icons
 *   fn_800188E0  GSpcbox_ProcessInput         -- 0x188 bytes, D-pad/stick navigation
 *   fn_80018A68  GSpcbox_OperationDispatch    -- 0x4C8 bytes, deposit/withdraw/move
 *   fn_80018F30  GSpcbox_GetCurrentBox        -- 0x24 bytes, return active box index
 *   fn_80018F54  GSpcbox_SetCurrentBox        -- 0x34 bytes, switch active box
 *   fn_80018F88  GSpcbox_GetBoxPokemonCount   -- 0xDC bytes, count Pokemon in box
 *   fn_80019064  GSpcbox_IsBoxFull            -- 0x0C bytes, check if box at capacity
 *   fn_80019070  GSpcbox_GetEmptySlot         -- 0x68 bytes, find first empty slot
 *   fn_800190D8  GSpcbox_IsSlotOccupied       -- 0x40 bytes, check if slot has Pokemon
 *   fn_80019118  GSpcbox_Deposit              -- 0xEC bytes, move party -> box
 *   fn_80019204  GSpcbox_Withdraw             -- 0xA4 bytes, move box -> party
 *   fn_800192A8  GSpcbox_Move                 -- 0x228 bytes, move between boxes
 *   fn_800194D0  GSpcbox_GetSlotSpecies       -- 0x14 bytes, get species at box slot
 *   fn_800194E4  GSpcbox_GetSlotData          -- 0xFC bytes, get full data for slot
 *   fn_800195E0  GSpcbox_DrawSlotIcon         -- 0xA0 bytes, render Pokemon mini icon
 *   fn_80019680  GSpcbox_DrawCursor           -- 0xD4 bytes, render selection cursor
 *   fn_80019754  GSpcbox_AnimateCursor        -- 0x1E4 bytes, cursor movement animation
 *   fn_80019938  GSpcbox_DrawBoxLabel         -- 0xBC bytes, render box name/number
 *   fn_800199F4  GSpcbox_DrawOperationMenu    -- 0x128 bytes, "Deposit/Withdraw/Move" menu
 *   fn_80019B1C  GSpcbox_GetOperation         -- 0x2C bytes, return selected operation
 *   fn_80019B48  GSpcbox_DrawItemInfo         -- 0x214 bytes, show held item info
 *   fn_80019D5C  GSpcbox_DrawMarkings         -- 0x210 bytes, show/edit markings
 *   fn_80019F6C  GSpcbox_BoxSwitchAnimation   -- 0xA18 bytes, box change animation
 *   fn_8001A984  GSpcbox_DrawWallpaper        -- 0x114 bytes, render box wallpaper
 *   fn_8001AA98  GSpcbox_ChangeWallpaper      -- 0xD8 bytes, wallpaper selection
 *   fn_8001AB70  GSpcbox_RenameBox            -- 0x3D4 bytes, box name entry
 *   fn_8001AF44  GSpcbox_DrawPokemonList      -- 0x240 bytes, party list sidebar
 *   fn_8001B184  GSpcbox_GetPartySlotState    -- 0x68 bytes, get party slot info
 *   fn_8001B1EC  GSpcbox_MainStateMachine     -- 0x8D8 bytes, main PC box state machine
 *   fn_8001BAC4  GSpcbox_TransitionState      -- 0x228 bytes, state transition handler
 *   fn_8001BCEC  GSpcbox_CheckCanDeposit      -- 0x50 bytes, validate deposit allowed
 *   fn_8001BD3C  GSpcbox_CheckCanWithdraw     -- 0x44 bytes, validate withdraw allowed
 *   fn_8001BD80  GSpcbox_CheckPartySpace      -- 0x74 bytes, check party has room
 *   fn_8001BDF4  GSpcbox_CheckLastPokemon     -- 0x44 bytes, prevent depositing last mon
 *   fn_8001BE38  GSpcbox_ShowConfirmDialog    -- 0x84 bytes, confirmation prompt
 *   fn_8001BEBC  GSpcbox_Init                 -- 0x1A8 bytes, full initialization
 *
 * fn_800181C4 (GSpcbox_SelectPokemon) implements the core selection logic:
 *   - Takes party slot index, species ID, and box position as parameters
 *   - Looks up the species data via fn_801440A0 / fn_80143FFC
 *   - If the count (from fn_80143FFC) is 0, shows "no Pokemon here" message
 *     via fn_8002A0B8 (format text) and fn_80106ADC (display)
 *   - Otherwise, opens the detail view with move list, stats, etc.
 *   - References lbl_80266918 event table with 0x4C stride per entry
 *
 * fn_8001B1EC (GSpcbox_MainStateMachine) is the largest function (0x8D8 bytes):
 *   - Manages box browse, select, deposit, withdraw, and move states
 *   - Uses lbl_803A1D40 as the box data array base address
 *   - Uses lbl_802E4E58 as the Pokemon slot layout table
 *   - Iterates over 6 party slots with a 0x30-byte stride per slot
 *   - Calls fn_801080CC to set dialog/UI display states
 *
 * SDA globals:
 *   lbl_8047A2A8-A2F8: PC box state variables
 *   lbl_8047A308: Current cursor position (s16)
 *   lbl_80478898: Camera angle for box view (float)
 *
 * BSS globals:
 *   lbl_803A1D40: Box data array (30 Pokemon per box, 14 boxes)
 *   lbl_803A2688: Party data working copy
 *
 * Rodata:
 *   lbl_802E4E58: Pokemon slot position table for box grid rendering
 */

#include "dolphin/types.h"

/* =========================================================================
 * External declarations
 * ========================================================================= */

/* Pokemon data */
extern void  fn_80129BC8(void* pokeData, u8 fieldId, u16* outCount,
                          s32 p4, s32 p5, s32 p6);
extern void  fn_801297D8(void* pokeData, u16* outCount, s32 p3, s32 p4, s32 p5);
extern u8    fn_801429E8(void* fieldData);
extern u16   fn_80143C50(void* fieldData);
extern u16   fn_801440A0(u16 speciesId);
extern u16   fn_80143FFC(void);

/* Text formatting */
extern void  fn_8002A0B8(void* outBuf, void* fmt, s32 p3, s32 p4,
                          u16 p5, s32 p6, ...);
extern s32   fn_8012A5B0(void* partyData, s32 slot, s32 p3);

/* Dialog/rendering */
extern void  fn_80106ADC(s32 p1, void* text, s32 p3, s32 p4, u8 p5);
extern void  fn_801069FC(s32 slot);
extern void  fn_801080CC(void* ctx, s32 state);
extern void  fn_8005D95C(s16 npcId, u16* outX, u16* outY);
extern void  fn_8005D9AC(s16 x, s16 y, s16 z);
extern void* fn_8005DA18(void* data);

/* =========================================================================
 * BSS data references
 * ========================================================================= */

/* lbl_803A1D40: PC box storage array
 * 14 boxes * 30 slots = 420 Pokemon slots
 * Each slot is a Pokemon data structure */

/* lbl_803A2688: Working party data copy
 * Used during box operations to avoid corrupting the party until commit */

/* =========================================================================
 * Stubs for remaining GSpcbox functions (0x8001C064-0x8001FD48)
 * ========================================================================= */

/* 0x8001C064 | 0x754 */
void fn_8001C064(void) {
    extern u8 lbl_802E4EB8[];
    extern u8 lbl_803A1C20[];
    extern u8 lbl_803A1D40[];
    extern void fn_8001E074();
    extern void fn_8006AEEC();
    extern void fn_80106D3C();
    extern void fn_8011FDC8();
    extern void fn_80120FE0();
    extern void fn_801230E0();
    extern void fn_80123110();
    extern void fn_80123FBC();
    extern void fn_8012640C();
    extern void fn_80129280();
    extern void fn_801298B8();
    extern void fn_801299C8();
    extern void fn_80129A78();
    extern void fn_8012AC08();
    extern void fn_80132A38();
    extern void fn_801906A0();
    extern void fn_801F2A7C();
    extern void fn_801F986C();
    extern void fn_801FB1C0();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r28 = r4;
    r30 = r5;
    r29 = r6;
    r4 = (u32)lbl_803A1D40;
    r27 = 0x0;
    r31 = (u32)lbl_803A1D40;
    r5 = *(u32*)((u8*)r31 + 0x8);
    r4 = *(u32*)((u8*)r31 + 0xC);
    if ((s32)r5 != 1) {
        if ((s32)r5 < 1) {
            if ((s32)r5 < 0) {
                goto L_8001C1EC;
            }
            if ((s32)r5 >= 3) goto L_8001C1EC;
            goto L_8001C1D8;
            }
        r25 = (s8)r3;
        tmp = r25 & 0xFFFF;
        if (tmp >= 6) {
            goto L_8001C204;
        }
        if ((s32)r5 != 1) {
            if ((s32)r5 >= 1) goto L_8001C148;
            if ((s32)r5 < 0) {
                goto L_8001C148;
            }
            r3 = 0x8ae;
            fn_801906A0();
            if (r3 == 0) {
                r3 = 0x0;
                r4 = 0x2;
                fn_80129280();
                goto L_8001C14C;
            }
            fn_8006AEEC();
            goto L_8001C14C;
        }
        if (r4 == 0) {
            r3 = 0x0;
            fn_801F2A7C();
            r4 = r3;
        }
        if (r4 == 0) {
            r3 = 0x0;
            goto L_8001C14C;
        }
        r3 = r4;
        r4 = 0x0;
        r5 = 0x44;
        r6 = 0x0;
        fn_801FB1C0();
        goto L_8001C14C;
    L_8001C148:
        r3 = 0x0;
    L_8001C14C:
        if (r3 == 0) {
            r27 = 0x0;
            goto L_8001C204;
        }
        r4 = r25;
        fn_8012AC08();
        r27 = r3;
        goto L_8001C1EC;
    }
    r25 = (s8)r3;
    tmp = r25 & 0xFFFF;
    if (tmp >= 6) {
        goto L_8001C204;
    }
    if (r4 == 0) {
        r3 = 0x0;
        fn_801F2A7C();
        r4 = r3;
    }
    if (r4 == 0) {
        r27 = 0x0;
        goto L_8001C204;
    }
    r3 = r4;
    r4 = r25;
    fn_801F986C();
    if (r3 == 0) {
        r27 = 0x0;
        goto L_8001C204;
    }
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    fn_8012640C();
    r27 = r3;
    goto L_8001C1EC;
L_8001C1D8:
    tmp = (s8)r3;
    tmp = tmp & 0xFFFF;
    if (tmp >= 0x1e) {
        goto L_8001C204;
    }
L_8001C1EC:
    r3 = r27;
    fn_80123FBC();
    tmp = r3 & 0xFF;
    if (tmp != 0) goto L_8001C204;
    r27 = 0x0;
L_8001C204:
    r3 = (u32)lbl_803A1D40;
    r4 = (u32)lbl_803A1C20;
    r5 = (u32)lbl_803A1D40;
    r3 = 0x32;
    r5 = *(u8*)((u8*)r5 + 0x6);
    tmp = (u32)lbl_803A1C20;
    r4 = (s8)r5;
    r4 = r4 * 0x30;
    r4 = tmp + r4;
    fn_80132A38();
    r3 = r27;
    fn_801230E0();
    tmp = *(u32*)((u8*)r31 + 0x8);
    r26 = r3;
    r4 = *(u32*)((u8*)r31 + 0xC);
    if ((s32)tmp != 1) {
        if ((s32)tmp >= 1) goto L_8001C2BC;
        if ((s32)tmp < 0) {
            goto L_8001C2BC;
        }
        r3 = 0x8ae;
        fn_801906A0();
        if (r3 == 0) {
            r3 = 0x0;
            r4 = 0x2;
            fn_80129280();
            goto L_8001C2C0;
        }
        fn_8006AEEC();
        goto L_8001C2C0;
    }
    if (r4 == 0) {
        r3 = 0x0;
        fn_801F2A7C();
        r4 = r3;
    }
    if (r4 == 0) {
        r25 = 0x0;
        goto L_8001C2C4;
    }
    r3 = r4;
    r4 = 0x0;
    r5 = 0x44;
    r6 = 0x0;
    fn_801FB1C0();
    goto L_8001C2C0;
L_8001C2BC:
    r3 = 0x0;
L_8001C2C0:
    r25 = r3;
L_8001C2C4:
    if (r25 == 0) {
        r3 = -0x1;
        return;
    }
    tmp = r30 & 0xFFFF;
    if (tmp == 0) {
        tmp = r26 & 0xFFFF;
        if (tmp != 0) {
            r3 = r25;
            r4 = r26;
            fn_801298B8();
            if ((s32)r3 <= 0) {
                r3 = 0x2;
                r4 = 0x2b6b;
                r5 = 0x1;
                r6 = 0x1;
                fn_80106D3C();
                goto L_8001C518;
            }
            r3 = r25;
            r4 = r26;
            r5 = 0x1;
            r6 = -0x1;
            fn_80129A78();
            r3 = r27;
            r4 = 0x0;
            r5 = 0x0;
            fn_80123110();
            if (r29 != 0) {
                *(u16*)((u8*)r29 + 0x0) = r3;
            }
            r4 = r3 & 0xFFFF;
            r3 = 0x2d;
            fn_80132A38();
            r3 = 0x2;
            r4 = 0x2b69;
            r5 = 0x1;
            r6 = 0x1;
            fn_80106D3C();
            goto L_8001C518;
        }
        r3 = 0x2;
        r4 = 0x2b6a;
        r5 = 0x1;
        r6 = 0x1;
        fn_80106D3C();
        goto L_8001C518;
    }
    r4 = r26 & 0xFFFF;
    if (r4 != 0) {
        r3 = 0x2d;
        fn_80132A38();
        r3 = 0x2;
        r4 = 0x2b66;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        r3 = 0x0;
        r4 = -0x1;
        r5 = -0x1;
        r6 = 0x0;
        fn_8001E074();
        r24 = (s8)r3;
        r3 = 0x1;
        ((void(*)(void))fn_801069FC)();
        if ((s32)r24 != 0) {
            r3 = -0x1;
            return;
        }
        r3 = r25;
        r4 = r30;
        r6 = r28;
        r5 = 0x1;
        fn_801299C8();
        if ((s32)r3 != 0) {
            r3 = 0x2;
            r4 = 0x2b6b;
            r5 = 0x1;
            r6 = 0x1;
            fn_80106D3C();
            goto L_8001C518;
        }
        r3 = r25;
        r4 = r26;
        fn_801298B8();
        if ((s32)r3 <= 0) {
            r4 = (u32)lbl_803A1D40;
            r3 = r25;
            r5 = (u32)lbl_803A1D40;
            r4 = r30;
            r6 = *(u8*)((u8*)r5 + 0x11);
            r5 = 0x1;
            fn_80129A78();
            r3 = 0x2;
            r4 = 0x2b6b;
            r5 = 0x1;
            r6 = 0x1;
            fn_80106D3C();
            goto L_8001C518;
        }
        r3 = r25;
        r4 = r26;
        r5 = 0x1;
        r6 = -0x1;
        fn_80129A78();
        r3 = r27;
        r4 = 0x0;
        r5 = 0x0;
        fn_80123110();
        r26 = r3;
        if (r29 != 0) {
            *(u16*)((u8*)r29 + 0x0) = r26;
        }
        r3 = r27;
        r4 = r30;
        r5 = 0x1;
        fn_80123110();
        r4 = r26 & 0xFFFF;
        r3 = 0x2d;
        fn_80132A38();
        r4 = r30 & 0xFFFF;
        r3 = 0x2e;
        fn_80132A38();
        r3 = 0x2;
        r4 = 0x2b67;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        goto L_8001C518;
    }
    r3 = r25;
    r4 = r30;
    r6 = r28;
    r5 = 0x1;
    fn_801299C8();
    if ((s32)r3 != 0) goto L_8001C518;
    r3 = r27;
    r4 = r30;
    r5 = 0x1;
    fn_80123110();
    r4 = r30 & 0xFFFF;
    r3 = 0x2d;
    fn_80132A38();
    r3 = 0x2;
    r4 = 0x2b68;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
L_8001C518:
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r3 = (u32)lbl_803A1C20;
    r4 = 0x0;
    r3 = (u32)lbl_803A1C20;
    r5 = 0x120;
    memset((void*)r3, (int)r4, (u32)r5);
    r27 = 0x0;
    r3 = (u32)lbl_803A1C20;
    r28 = (u32)lbl_803A1C20;
    while (1) {
        tmp = r27 & 0xFFFF;
        if (tmp >= 6) break;
        r4 = r27 & 0xFFFF;
        r5 = *(u32*)((u8*)r31 + 0x8);
        tmp = r4 * 0x30;
        r3 = *(u32*)((u8*)r31 + 0xC);
        r25 = 0x0;
        r29 = r28 + tmp;
        if ((s32)r5 != 1) {
            if ((s32)r5 < 1) {
                if ((s32)r5 < 0) {
                    goto L_8001C68C;
                }
                if ((s32)r5 >= 3) goto L_8001C68C;
                goto L_8001C680;
                }
            if (r4 >= 6) {
                goto L_8001C6A4;
            }
            if ((s32)r5 != 1) {
                if ((s32)r5 >= 1) goto L_8001C600;
                if ((s32)r5 < 0) {
                    goto L_8001C600;
                }
                r3 = 0x8ae;
                fn_801906A0();
                if (r3 == 0) {
                    r3 = 0x0;
                    r4 = 0x2;
                    fn_80129280();
                    goto L_8001C604;
                }
                fn_8006AEEC();
                goto L_8001C604;
            }
            if (r3 == 0) {
                r3 = 0x0;
                fn_801F2A7C();
            }
            if (r3 == 0) {
                r3 = 0x0;
                goto L_8001C604;
            }
            r4 = 0x0;
            r5 = 0x44;
            r6 = 0x0;
            fn_801FB1C0();
            goto L_8001C604;
        L_8001C600:
            r3 = 0x0;
        L_8001C604:
            if (r3 == 0) {
                r25 = 0x0;
                goto L_8001C6A4;
            }
            r4 = r27;
            fn_8012AC08();
            r25 = r3;
            goto L_8001C68C;
        }
        if (r4 >= 6) {
            goto L_8001C6A4;
        }
        if (r3 == 0) {
            r3 = 0x0;
            fn_801F2A7C();
        }
        if (r3 == 0) {
            r25 = 0x0;
            goto L_8001C6A4;
        }
        r4 = r27;
        fn_801F986C();
        if (r3 == 0) {
            r25 = 0x0;
            goto L_8001C6A4;
        }
        r4 = 0x0;
        r5 = 0xcc;
        r6 = 0x0;
        fn_8012640C();
        r25 = r3;
        goto L_8001C68C;
    L_8001C680:
        if (r4 >= 0x1e) {
            goto L_8001C6A4;
        }
    L_8001C68C:
        r3 = r25;
        fn_80123FBC();
        tmp = r3 & 0xFF;
        if (tmp != 0) goto L_8001C6A4;
        r25 = 0x0;
    L_8001C6A4:
        if (r25 == 0) {
            tmp = 0x0;
            *(u16*)((u8*)r29 + 0x0) = tmp;
            goto L_8001C790;
        }
        r3 = r25;
        r4 = r29;
        fn_8011FDC8();
        r3 = r25;
        r4 = 0x0;
        r5 = 0x7b;
        r6 = 0x0;
        fn_8012640C();
        tmp = r3 & 0xFF;
        if (tmp == 1) {
            tmp = 0x0;
            *(u16*)((u8*)r29 + 0x1A) = tmp;
        }
        r3 = r25;
        r4 = 0x0;
        r5 = 0x7b;
        r6 = 0x0;
        fn_8012640C();
        tmp = r3 & 0xFF;
        if (tmp == 1) {
            tmp = 0x1;
            goto L_8001C778;
        }
        r3 = r25;
        fn_80120FE0();
        tmp = r3 & 0xFFFF;
        if ((s32)tmp != 0x3c) {
            if ((s32)tmp < 0x3c) {
                if ((s32)tmp != 0x3a) {
                    if ((s32)tmp < 0x3a) {
                        goto L_8001C774;
                    }
                    if ((s32)tmp != 0x3e) {
                        if ((s32)tmp >= 0x3e) goto L_8001C774;
                        goto L_8001C764;
                        }
                    tmp = 0x2;
                    goto L_8001C778;
                        }
                tmp = 0x3;
                goto L_8001C778;
            }
            tmp = 0x4;
            goto L_8001C778;
        L_8001C764:
            tmp = 0x5;
            goto L_8001C778;
                    }
        tmp = 0x6;
        goto L_8001C778;
    L_8001C774:
        tmp = 0x0;
    L_8001C778:
        tmp = tmp & 0xFFFF;
        r3 = (u32)lbl_802E4EB8;
        tmp = tmp << 1;
        r3 = (u32)lbl_802E4EB8;
        tmp = *(u16*)(r3 + tmp);
        *(u16*)((u8*)r29 + 0x24) = tmp;
    L_8001C790:
        r27 = r27 + 0x1;

    }
    r3 = -0x1;

    return;
}

/* 0x8001C7B8 | 0xBC0 */
void fn_8001C7B8(void) {
    extern u8 lbl_802E4E58[];
    extern u8 lbl_802E4EB8[];
    extern u8 lbl_803A1C20[];
    extern u8 lbl_803A1D40[];
    extern u8 lbl_8047B7C0[];
    extern void fn_80018F54();
    extern void fn_80019064();
    extern void fn_8001C064();
    extern void fn_8001D718();
    extern void fn_8006AEEC();
    extern void fn_80097F08();
    extern void fn_800F0308();
    extern void fn_801022B8();
    extern void fn_80102510();
    extern void fn_80102568();
    extern void fn_8010264C();
    extern void fn_801026A4();
    extern void fn_80104704();
    extern void fn_801070F4();
    extern void fn_8011FDC8();
    extern void fn_80120FE0();
    extern void fn_80121BB4();
    extern void fn_80123FBC();
    extern void fn_8012640C();
    extern void fn_80129280();
    extern void fn_8012AC08();
    extern void fn_801906A0();
    extern void fn_801C40F0();
    extern void fn_801C41C8();
    extern void fn_801F2A7C();
    extern void fn_801F986C();
    extern void fn_801FB1C0();
    extern void fn_80019D5C();
    u8 sp[0x60];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
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

    r26 = r3;
    r3 = (u32)lbl_803A1D40;
    r31 = (u32)lbl_803A1D40;
    do {
        tmp = 0x1;
        r3 = 0x8ae;
        *(u8*)((u8*)r31 + 0x14) = tmp;
        fn_801906A0();
        if (r3 == 0) {
            r3 = 0x6b;
            r4 = 0x1;
            fn_8010264C();
            r23 = r3;
            r3 = 0x6b;
            fn_80102510();
        } else {

            r3 = 0x10f;
            r4 = 0x1;
            fn_8010264C();
            r23 = r3;
            r3 = 0x10f;
            fn_80102510();
        }
        do {
            if ((s32)r23 == 0x3d2) break;
            if ((s32)r23 < 0x3d2) {
                if ((s32)r23 == 0x3cf) goto L_8001C874;
                if ((s32)r23 < 0x3cf) {
                    if ((s32)r23 == (s32)-0x1) break;
                    goto L_8001D358;
                }
                if ((s32)r23 >= 0x3d1) goto L_8001D234;
                goto L_8001CA84;
            }
            if ((s32)r23 == 0x52b) goto L_8001CA84;
            if ((s32)r23 < 0x52b) {
                if ((s32)r23 >= 0x52a) goto L_8001C874;
                goto L_8001D358;
            }
            if ((s32)r23 >= 0x52d) goto L_8001D358;
            break;
        L_8001C874:
            r3 = 0x63;
            r4 = 0x0;
            r5 = 0x1;
            fn_80102568();
            r4 = *(u32*)((u8*)r31 + 0x8);
            r24 = 0x0;
            r3 = *(u32*)((u8*)r31 + 0xC);
            if ((s32)r4 != 1) {
                if ((s32)r4 < 1) {
                    if ((s32)r4 < 0) {
                        goto L_8001C9D8;
                    }
                    if ((s32)r4 >= 3) goto L_8001C9D8;
                    goto L_8001C9C4;
                    }
                r23 = (s8)r26;
                tmp = r23 & 0xFFFF;
                if (tmp >= 6) {
                    goto L_8001C9F0;
                }
                if ((s32)r4 != 1) {
                    if ((s32)r4 >= 1) goto L_8001C93C;
                    if ((s32)r4 < 0) {
                        goto L_8001C93C;
                    }
                    r3 = 0x8ae;
                    fn_801906A0();
                    if (r3 == 0) {
                        r3 = 0x0;
                        r4 = 0x2;
                        fn_80129280();
                        goto L_8001C940;
                    }
                    fn_8006AEEC();
                    goto L_8001C940;
                }
                if (r3 == 0) {
                    r3 = 0x0;
                    fn_801F2A7C();
                }
                if (r3 == 0) {
                    r3 = 0x0;
                    goto L_8001C940;
                }
                r4 = 0x0;
                r5 = 0x44;
                r6 = 0x0;
                fn_801FB1C0();
                goto L_8001C940;
            L_8001C93C:
                r3 = 0x0;
            L_8001C940:
                if (r3 == 0) {
                    r24 = 0x0;
                    goto L_8001C9F0;
                }
                r4 = r23;
                fn_8012AC08();
                r24 = r3;
                goto L_8001C9D8;
            }
            r23 = (s8)r26;
            tmp = r23 & 0xFFFF;
            if (tmp >= 6) {
                goto L_8001C9F0;
            }
            if (r3 == 0) {
                r3 = 0x0;
                fn_801F2A7C();
            }
            if (r3 == 0) {
                r24 = 0x0;
                goto L_8001C9F0;
            }
            r4 = r23;
            fn_801F986C();
            if (r3 == 0) {
                r24 = 0x0;
                goto L_8001C9F0;
            }
            r4 = 0x0;
            r5 = 0xcc;
            r6 = 0x0;
            fn_8012640C();
            r24 = r3;
            goto L_8001C9D8;
        L_8001C9C4:
            tmp = (s8)r26;
            tmp = tmp & 0xFFFF;
            if (tmp >= 0x1e) {
                goto L_8001C9F0;
            }
        L_8001C9D8:
            r3 = r24;
            fn_80123FBC();
            tmp = r3 & 0xFF;
            if (tmp != 0) goto L_8001C9F0;
            r24 = 0x0;
        L_8001C9F0:
            f1 = *(f32*)lbl_8047B7C0;
            r3 = 0x3;
            fn_801C41C8();
            r3 = 0x1;
            fn_801C40F0();
            r4 = (u32)fn_80019D5C;
            r3 = r24;
            r4 = (u32)fn_80019D5C;
            r5 = 0x0;
            fn_80097F08();
            tmp = *(u8*)((u8*)r31 + 0x6);
            r3 = (u32)lbl_803A1C20;
            r9 = (u32)lbl_803A1C20;
            r5 = (u32)sp + 0x28;
            tmp = (s8)tmp;
            r3 = 0x63;
            *(u32*)(sp + 0x28) = tmp;
            r4 = 0x0;
            r6 = 0x0;
            r7 = 0x0;
            r8 = 0x1;
            fn_801026A4();
            r3 = 0x63;
            fn_80104704();
            if (r3 != 0) {
                tmp = 0x1;
                *(u8*)((u8*)r3 + 0x98) = tmp;
            }
            f1 = *(f32*)lbl_8047B7C0;
            r3 = 0x2;
            fn_801C41C8();
            r3 = 0x1;
            fn_801C40F0();
            r26 = *(u8*)((u8*)r31 + 0x6);
            r23 = 0x0;
            goto L_8001D358;
        L_8001CA84:
            tmp = *(u8*)((u8*)r31 + 0x6);
            r4 = 0x2;
            r3 = (u32)lbl_803A1C20;
            *(u8*)((u8*)r31 + 0x14) = r4;
            tmp = (s8)tmp;
            r5 = (u32)sp + 0x2c;
            *(u8*)((u8*)r31 + 0x7) = r26;
            r9 = (u32)lbl_803A1C20;
            r3 = 0x63;
            r4 = 0x0;
            *(u32*)(sp + 0x2C) = tmp;
            r6 = 0x0;
            r7 = 0x1;
            r8 = 0x1;
            fn_801026A4();
            tmp = *(u8*)((u8*)r31 + 0x1);
            r3 = (s8)r3;
            if (tmp == 0) {
                r3 = -0x2;
            }
            tmp = -0x1;
            r28 = (s8)r3;
            *(u8*)((u8*)r31 + 0x7) = tmp;
            r3 = 0x63;
            fn_801022B8();
            do {
            if ((s32)r3 == 0x3b6 || (s32)r28 == (s32)-0x1) break;

                r4 = *(u32*)((u8*)r31 + 0x8);
                r27 = 0x0;
                r3 = *(u32*)((u8*)r31 + 0xC);
                if ((s32)r4 != 1) {
                    if ((s32)r4 < 1) {
                        if ((s32)r4 < 0) {
                            goto L_8001CC50;
                        }
                        if ((s32)r4 >= 3) goto L_8001CC50;
                        goto L_8001CC3C;
                        }
                    r23 = (s8)r26;
                    tmp = r23 & 0xFFFF;
                    if (tmp >= 6) {
                        goto L_8001CC68;
                    }
                    if ((s32)r4 != 1) {
                        if ((s32)r4 >= 1) goto L_8001CBB4;
                        if ((s32)r4 < 0) {
                            goto L_8001CBB4;
                        }
                        r3 = 0x8ae;
                        fn_801906A0();
                        if (r3 == 0) {
                            r3 = 0x0;
                            r4 = 0x2;
                            fn_80129280();
                            goto L_8001CBB8;
                        }
                        fn_8006AEEC();
                        goto L_8001CBB8;
                    }
                    if (r3 == 0) {
                        r3 = 0x0;
                        fn_801F2A7C();
                    }
                    if (r3 == 0) {
                        r3 = 0x0;
                        goto L_8001CBB8;
                    }
                    r4 = 0x0;
                    r5 = 0x44;
                    r6 = 0x0;
                    fn_801FB1C0();
                    goto L_8001CBB8;
                L_8001CBB4:
                    r3 = 0x0;
                L_8001CBB8:
                    if (r3 == 0) {
                        r27 = 0x0;
                        goto L_8001CC68;
                    }
                    r4 = r23;
                    fn_8012AC08();
                    r27 = r3;
                    goto L_8001CC50;
                }
                r23 = (s8)r26;
                tmp = r23 & 0xFFFF;
                if (tmp >= 6) {
                    goto L_8001CC68;
                }
                if (r3 == 0) {
                    r3 = 0x0;
                    fn_801F2A7C();
                }
                if (r3 == 0) {
                    r27 = 0x0;
                    goto L_8001CC68;
                }
                r4 = r23;
                fn_801F986C();
                if (r3 == 0) {
                    r27 = 0x0;
                    goto L_8001CC68;
                }
                r4 = 0x0;
                r5 = 0xcc;
                r6 = 0x0;
                fn_8012640C();
                r27 = r3;
                goto L_8001CC50;
            L_8001CC3C:
                tmp = (s8)r26;
                tmp = tmp & 0xFFFF;
                if (tmp >= 0x1e) {
                    goto L_8001CC68;
                }
            L_8001CC50:
                r3 = r27;
                fn_80123FBC();
                tmp = r3 & 0xFF;
                if (tmp != 0) goto L_8001CC68;
                r27 = 0x0;
            L_8001CC68:
                r4 = *(u32*)((u8*)r31 + 0x8);
                r29 = 0x0;
                r3 = *(u32*)((u8*)r31 + 0xC);
                if ((s32)r4 != 1) {
                    if ((s32)r4 < 1) {
                        if ((s32)r4 < 0) {
                            goto L_8001CDB0;
                        }
                        if ((s32)r4 >= 3) goto L_8001CDB0;
                        goto L_8001CDA0;
                        }
                    tmp = r28 & 0xFFFF;
                    if (tmp >= 6) {
                        goto L_8001CDC8;
                    }
                    if ((s32)r4 != 1) {
                        if ((s32)r4 >= 1) goto L_8001CD1C;
                        if ((s32)r4 < 0) {
                            goto L_8001CD1C;
                        }
                        r3 = 0x8ae;
                        fn_801906A0();
                        if (r3 == 0) {
                            r3 = 0x0;
                            r4 = 0x2;
                            fn_80129280();
                            goto L_8001CD20;
                        }
                        fn_8006AEEC();
                        goto L_8001CD20;
                    }
                    if (r3 == 0) {
                        r3 = 0x0;
                        fn_801F2A7C();
                    }
                    if (r3 == 0) {
                        r3 = 0x0;
                        goto L_8001CD20;
                    }
                    r4 = 0x0;
                    r5 = 0x44;
                    r6 = 0x0;
                    fn_801FB1C0();
                    goto L_8001CD20;
                L_8001CD1C:
                    r3 = 0x0;
                L_8001CD20:
                    if (r3 == 0) {
                        r29 = 0x0;
                        goto L_8001CDC8;
                    }
                    r4 = r28 & 0xFFFF;
                    fn_8012AC08();
                    r29 = r3;
                    goto L_8001CDB0;
                }
                tmp = r28 & 0xFFFF;
                if (tmp >= 6) {
                    goto L_8001CDC8;
                }
                if (r3 == 0) {
                    r3 = 0x0;
                    fn_801F2A7C();
                }
                if (r3 == 0) {
                    r29 = 0x0;
                    goto L_8001CDC8;
                }
                r4 = r28 & 0xFFFF;
                fn_801F986C();
                if (r3 == 0) {
                    r29 = 0x0;
                    goto L_8001CDC8;
                }
                r4 = 0x0;
                r5 = 0xcc;
                r6 = 0x0;
                fn_8012640C();
                r29 = r3;
                goto L_8001CDB0;
            L_8001CDA0:
                tmp = r28 & 0xFFFF;
                if (tmp >= 0x1e) {
                    goto L_8001CDC8;
                }
            L_8001CDB0:
                r3 = r29;
                fn_80123FBC();
                tmp = r3 & 0xFF;
                if (tmp != 0) goto L_8001CDC8;
                r29 = 0x0;
            L_8001CDC8:
                if (r27 == 0 || r27 == 0) break;

                tmp = *(u8*)((u8*)r31 + 0x4);
                r3 = (u32)lbl_802E4E58;
                r30 = (s8)r26;
                r4 = (u32)sp + 0x1e;
                r5 = (s8)tmp;
                tmp = (u32)lbl_802E4E58;
                r3 = r5 * 0x30;
                r24 = r30 << 3;
                r5 = (u32)sp + 0x20;
                r3 = tmp + r3;
                r25 = *(s16*)(r3 + r24);
                r3 = r25;
                ((void(*)(void))fn_8005D95C)();
                tmp = *(s16*)((u8*)(u32)sp + 0x1E);
                r3 = r25;
                if ((s32)tmp > 0xfa) {
                    r4 = 0x11a;
                } else {

                    r4 = 0x122;
                }
                ((void(*)(void))fn_801080CC)();
                r4 = *(u8*)((u8*)r31 + 0x4);
                r3 = (u32)lbl_802E4E58;
                tmp = (u32)lbl_802E4E58;
                r25 = r28 << 3;
                r3 = (s8)r4;
                r4 = (u32)sp + 0x1a;
                r3 = r3 * 0x30;
                r5 = (u32)sp + 0x1c;
                r3 = tmp + r3;
                r28 = *(s16*)(r3 + r25);
                r3 = r28;
                ((void(*)(void))fn_8005D95C)();
                tmp = *(s16*)((u8*)(u32)sp + 0x1A);
                r3 = r28;
                if ((s32)tmp > 0xfa) {
                    r4 = 0x11a;
                } else {

                    r4 = 0x122;
                }
                ((void(*)(void))fn_801080CC)();
                r3 = (u32)lbl_802E4E58;
                r4 = r30 << 3;
                tmp = (u32)lbl_802E4E58;
                r30 = tmp + r4;
                while (1) {
                    tmp = *(u8*)((u8*)r31 + 0x4);
                    r4 = (u32)sp + 0x16;
                    r5 = (u32)sp + 0x18;
                    tmp = (s8)tmp;
                    tmp = tmp * 0x30;
                    r28 = *(s16*)(r30 + tmp);
                    r3 = r28;
                    ((void(*)(void))fn_8005D95C)();
                    r3 = r28;
                    fn_801070F4();
                    tmp = r3 & 0xFF;
                    if (tmp == 0) break;
                    fn_800F0308();

                }
                r3 = r27;
                r4 = r29;
                fn_80121BB4();
                r3 = (u32)lbl_803A1C20;
                r4 = 0x0;
                r3 = (u32)lbl_803A1C20;
                r5 = 0x120;
                memset((void*)r3, (int)r4, (u32)r5);
                r29 = 0x0;
                r3 = (u32)lbl_803A1C20;
                r27 = (u32)lbl_803A1C20;
                while (1) {
                    tmp = r29 & 0xFFFF;
                    if (tmp >= 6) break;
                    r4 = r29 & 0xFFFF;
                    r5 = *(u32*)((u8*)r31 + 0x8);
                    tmp = r4 * 0x30;
                    r3 = *(u32*)((u8*)r31 + 0xC);
                    r23 = 0x0;
                    r28 = r27 + tmp;
                    if ((s32)r5 != 1) {
                        if ((s32)r5 < 1) {
                            if ((s32)r5 < 0) {
                                goto L_8001D03C;
                            }
                            if ((s32)r5 >= 3) goto L_8001D03C;
                            goto L_8001D030;
                            }
                        if (r4 >= 6) {
                            goto L_8001D054;
                        }
                        if ((s32)r5 != 1) {
                            if ((s32)r5 >= 1) goto L_8001CFB0;
                            if ((s32)r5 < 0) {
                                goto L_8001CFB0;
                            }
                            r3 = 0x8ae;
                            fn_801906A0();
                            if (r3 == 0) {
                                r3 = 0x0;
                                r4 = 0x2;
                                fn_80129280();
                                goto L_8001CFB4;
                            }
                            fn_8006AEEC();
                            goto L_8001CFB4;
                        }
                        if (r3 == 0) {
                            r3 = 0x0;
                            fn_801F2A7C();
                        }
                        if (r3 == 0) {
                            r3 = 0x0;
                            goto L_8001CFB4;
                        }
                        r4 = 0x0;
                        r5 = 0x44;
                        r6 = 0x0;
                        fn_801FB1C0();
                        goto L_8001CFB4;
                    L_8001CFB0:
                        r3 = 0x0;
                    L_8001CFB4:
                        if (r3 == 0) {
                            r23 = 0x0;
                            goto L_8001D054;
                        }
                        r4 = r29;
                        fn_8012AC08();
                        r23 = r3;
                        goto L_8001D03C;
                    }
                    if (r4 >= 6) {
                        goto L_8001D054;
                    }
                    if (r3 == 0) {
                        r3 = 0x0;
                        fn_801F2A7C();
                    }
                    if (r3 == 0) {
                        r23 = 0x0;
                        goto L_8001D054;
                    }
                    r4 = r29;
                    fn_801F986C();
                    if (r3 == 0) {
                        r23 = 0x0;
                        goto L_8001D054;
                    }
                    r4 = 0x0;
                    r5 = 0xcc;
                    r6 = 0x0;
                    fn_8012640C();
                    r23 = r3;
                    goto L_8001D03C;
                L_8001D030:
                    if (r4 >= 0x1e) {
                        goto L_8001D054;
                    }
                L_8001D03C:
                    r3 = r23;
                    fn_80123FBC();
                    tmp = r3 & 0xFF;
                    if (tmp != 0) goto L_8001D054;
                    r23 = 0x0;
                L_8001D054:
                    if (r23 == 0) {
                        tmp = 0x0;
                        *(u16*)((u8*)r28 + 0x0) = tmp;
                        goto L_8001D140;
                    }
                    r3 = r23;
                    r4 = r28;
                    fn_8011FDC8();
                    r3 = r23;
                    r4 = 0x0;
                    r5 = 0x7b;
                    r6 = 0x0;
                    fn_8012640C();
                    tmp = r3 & 0xFF;
                    if (tmp == 1) {
                        tmp = 0x0;
                        *(u16*)((u8*)r28 + 0x1A) = tmp;
                    }
                    r3 = r23;
                    r4 = 0x0;
                    r5 = 0x7b;
                    r6 = 0x0;
                    fn_8012640C();
                    tmp = r3 & 0xFF;
                    if (tmp == 1) {
                        tmp = 0x1;
                        goto L_8001D128;
                    }
                    r3 = r23;
                    fn_80120FE0();
                    tmp = r3 & 0xFFFF;
                    if ((s32)tmp != 0x3c) {
                        if ((s32)tmp < 0x3c) {
                            if ((s32)tmp != 0x3a) {
                                if ((s32)tmp < 0x3a) {
                                    goto L_8001D124;
                                }
                                if ((s32)tmp != 0x3e) {
                                    if ((s32)tmp >= 0x3e) goto L_8001D124;
                                    goto L_8001D114;
                                    }
                                tmp = 0x2;
                                goto L_8001D128;
                                    }
                            tmp = 0x3;
                            goto L_8001D128;
                        }
                        tmp = 0x4;
                        goto L_8001D128;
                    L_8001D114:
                        tmp = 0x5;
                        goto L_8001D128;
                                }
                    tmp = 0x6;
                    goto L_8001D128;
                L_8001D124:
                    tmp = 0x0;
                L_8001D128:
                    tmp = tmp & 0xFFFF;
                    r3 = (u32)lbl_802E4EB8;
                    tmp = tmp << 1;
                    r3 = (u32)lbl_802E4EB8;
                    tmp = *(u16*)(r3 + tmp);
                    *(u16*)((u8*)r28 + 0x24) = tmp;
                L_8001D140:
                    r29 = r29 + 0x1;

                }
                f1 = *(f32*)lbl_8047B7C0;
                fn_8001D718();
                r5 = *(u8*)((u8*)r31 + 0x4);
                r3 = (u32)lbl_802E4E58;
                tmp = (u32)lbl_802E4E58;
                r4 = (u32)sp + 0x12;
                r3 = (s8)r5;
                r5 = (u32)sp + 0x14;
                r3 = r3 * 0x30;
                r3 = tmp + r3;
                r24 = *(s16*)(r3 + r24);
                r3 = r24;
                ((void(*)(void))fn_8005D95C)();
                tmp = *(s16*)((u8*)(u32)sp + 0x12);
                r3 = r24;
                if ((s32)tmp > 0xfa) {
                    r4 = 0x116;
                } else {

                    r4 = 0x11e;
                }
                ((void(*)(void))fn_801080CC)();
                r5 = *(u8*)((u8*)r31 + 0x4);
                r3 = (u32)lbl_802E4E58;
                tmp = (u32)lbl_802E4E58;
                r4 = (u32)sp + 0xe;
                r3 = (s8)r5;
                r5 = (u32)sp + 0x10;
                r3 = r3 * 0x30;
                r3 = tmp + r3;
                r24 = *(s16*)(r3 + r25);
                r3 = r24;
                ((void(*)(void))fn_8005D95C)();
                tmp = *(s16*)((u8*)(u32)sp + 0xE);
                r3 = r24;
                if ((s32)tmp > 0xfa) {
                    r4 = 0x116;
                } else {

                    r4 = 0x11e;
                }
                ((void(*)(void))fn_801080CC)();
                while (1) {
                    tmp = *(u8*)((u8*)r31 + 0x4);
                    r4 = (u32)sp + 0xa;
                    r5 = (u32)sp + 0xc;
                    tmp = (s8)tmp;
                    tmp = tmp * 0x30;
                    r24 = *(s16*)(r30 + tmp);
                    r3 = r24;
                    ((void(*)(void))fn_8005D95C)();
                    r3 = r24;
                    fn_801070F4();
                    tmp = r3 & 0xFF;
                    if (tmp == 0) break;
                    fn_800F0308();

                }
            } while (0);
            r23 = -0x1;
            goto L_8001D358;
        L_8001D234:
            tmp = 0x3;
            r3 = 0x6c;
            *(u8*)((u8*)r31 + 0x14) = tmp;
            r4 = 0x1;
            fn_8010264C();
            r24 = r3;
            r3 = 0x6c;
            fn_80102510();
            if ((s32)r24 != 1) {
                if ((s32)r24 < 1) {
                    if ((s32)r24 == (s32)-0x1) goto L_8001D338;
                    if ((s32)r24 < (s32)-0x1) {
                        goto L_8001D33C;
                    }
                    if ((s32)r24 >= 3) goto L_8001D33C;
                    goto L_8001D338;
                    }
                r3 = 0x63;
                fn_80102510();
                r3 = 0x2;
                r4 = 0x0;
                r5 = 0x0;
                fn_80018F54();
                r25 = r3;
                fn_80019064();
                tmp = *(u8*)((u8*)r31 + 0x6);
                r4 = (u32)lbl_803A1C20;
                r9 = (u32)lbl_803A1C20;
                r24 = r3;
                tmp = (s8)tmp;
                r5 = (u32)sp + 0x24;
                *(u32*)(sp + 0x24) = tmp;
                r3 = 0x63;
                r4 = 0x0;
                r6 = 0x0;
                r7 = 0x0;
                r8 = 0x1;
                fn_801026A4();
                r3 = 0x63;
                fn_80104704();
                if (r3 != 0) {
                    tmp = 0x1;
                    *(u8*)((u8*)r3 + 0x98) = tmp;
                }
                tmp = r25 & 0xFFFF;
                if (tmp == 0) {
                    r24 = -0x1;
                    goto L_8001D33C;
                }
                r3 = r26;
                r5 = r25;
                r4 = r24 & 0xFF;
                r6 = (u32)sp + 0x8;
                fn_8001C064();
                r24 = 0x0;
                goto L_8001D33C;
            }
            r3 = r26;
            r6 = (u32)sp + 0x8;
            r4 = -0x1;
            r5 = 0x0;
            fn_8001C064();
            r24 = 0x0;
            goto L_8001D33C;
        L_8001D338:
            r24 = -0x1;
        L_8001D33C:
            if ((s32)r24 == (s32)-0x1) {
                r23 = 0x0;
                goto L_8001D358;
            }
            r23 = -0x1;
            goto L_8001D358;
        } while (0);
        r23 = -0x1;
    L_8001D358:
        ;
    } while ((s32)r23 != (s32)(-0x1));
    r3 = 0x0;
    return;
}

/* 0x8001D378 | 0x2AC */
void fn_8001D378(void) {
    extern u8 lbl_802E4EB8[];
    extern u8 lbl_803A1C20[];
    extern u8 lbl_803A1D40[];
    extern void fn_8006AEEC();
    extern void fn_8011FDC8();
    extern void fn_80120FE0();
    extern void fn_80123FBC();
    extern void fn_8012640C();
    extern void fn_80129280();
    extern void fn_8012AC08();
    extern void fn_801906A0();
    extern void fn_801F2A7C();
    extern void fn_801F986C();
    extern void fn_801FB1C0();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = (u32)lbl_803A1C20;
    r4 = 0x0;
    r3 = (u32)lbl_803A1C20;
    r5 = 0x120;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = (u32)lbl_803A1D40;
    r29 = 0x0;
    r30 = (u32)lbl_803A1D40;
    r3 = (u32)lbl_803A1C20;
    r31 = (u32)lbl_803A1C20;
    while (1) {
        tmp = r29 & 0xFFFF;
        if (tmp >= 6) break;
        r4 = r29 & 0xFFFF;
        r5 = *(u32*)((u8*)r30 + 0x8);
        tmp = r4 * 0x30;
        r3 = *(u32*)((u8*)r30 + 0xC);
        r27 = 0x0;
        r28 = r31 + tmp;
        if ((s32)r5 != 1) {
            if ((s32)r5 < 1) {
                if ((s32)r5 < 0) {
                    goto L_8001D4FC;
                }
                if ((s32)r5 >= 3) goto L_8001D4FC;
                goto L_8001D4F0;
                }
            if (r4 >= 6) {
                goto L_8001D514;
            }
            if ((s32)r5 != 1) {
                if ((s32)r5 >= 1) goto L_8001D470;
                if ((s32)r5 < 0) {
                    goto L_8001D470;
                }
                r3 = 0x8ae;
                fn_801906A0();
                if (r3 == 0) {
                    r3 = 0x0;
                    r4 = 0x2;
                    fn_80129280();
                    goto L_8001D474;
                }
                fn_8006AEEC();
                goto L_8001D474;
            }
            if (r3 == 0) {
                r3 = 0x0;
                fn_801F2A7C();
            }
            if (r3 == 0) {
                r3 = 0x0;
                goto L_8001D474;
            }
            r4 = 0x0;
            r5 = 0x44;
            r6 = 0x0;
            fn_801FB1C0();
            goto L_8001D474;
        L_8001D470:
            r3 = 0x0;
        L_8001D474:
            if (r3 == 0) {
                r27 = 0x0;
                goto L_8001D514;
            }
            r4 = r29;
            fn_8012AC08();
            r27 = r3;
            goto L_8001D4FC;
        }
        if (r4 >= 6) {
            goto L_8001D514;
        }
        if (r3 == 0) {
            r3 = 0x0;
            fn_801F2A7C();
        }
        if (r3 == 0) {
            r27 = 0x0;
            goto L_8001D514;
        }
        r4 = r29;
        fn_801F986C();
        if (r3 == 0) {
            r27 = 0x0;
            goto L_8001D514;
        }
        r4 = 0x0;
        r5 = 0xcc;
        r6 = 0x0;
        fn_8012640C();
        r27 = r3;
        goto L_8001D4FC;
    L_8001D4F0:
        if (r4 >= 0x1e) {
            goto L_8001D514;
        }
    L_8001D4FC:
        r3 = r27;
        fn_80123FBC();
        tmp = r3 & 0xFF;
        if (tmp != 0) goto L_8001D514;
        r27 = 0x0;
    L_8001D514:
        if (r27 == 0) {
            tmp = 0x0;
            *(u16*)((u8*)r28 + 0x0) = tmp;
            goto L_8001D600;
        }
        r3 = r27;
        r4 = r28;
        fn_8011FDC8();
        r3 = r27;
        r4 = 0x0;
        r5 = 0x7b;
        r6 = 0x0;
        fn_8012640C();
        tmp = r3 & 0xFF;
        if (tmp == 1) {
            tmp = 0x0;
            *(u16*)((u8*)r28 + 0x1A) = tmp;
        }
        r3 = r27;
        r4 = 0x0;
        r5 = 0x7b;
        r6 = 0x0;
        fn_8012640C();
        tmp = r3 & 0xFF;
        if (tmp == 1) {
            tmp = 0x1;
            goto L_8001D5E8;
        }
        r3 = r27;
        fn_80120FE0();
        tmp = r3 & 0xFFFF;
        if ((s32)tmp != 0x3c) {
            if ((s32)tmp < 0x3c) {
                if ((s32)tmp != 0x3a) {
                    if ((s32)tmp < 0x3a) {
                        goto L_8001D5E4;
                    }
                    if ((s32)tmp != 0x3e) {
                        if ((s32)tmp >= 0x3e) goto L_8001D5E4;
                        goto L_8001D5D4;
                        }
                    tmp = 0x2;
                    goto L_8001D5E8;
                        }
                tmp = 0x3;
                goto L_8001D5E8;
            }
            tmp = 0x4;
            goto L_8001D5E8;
        L_8001D5D4:
            tmp = 0x5;
            goto L_8001D5E8;
                    }
        tmp = 0x6;
        goto L_8001D5E8;
    L_8001D5E4:
        tmp = 0x0;
    L_8001D5E8:
        tmp = tmp & 0xFFFF;
        r3 = (u32)lbl_802E4EB8;
        tmp = tmp << 1;
        r3 = (u32)lbl_802E4EB8;
        tmp = *(u16*)(r3 + tmp);
        *(u16*)((u8*)r28 + 0x24) = tmp;
    L_8001D600:
        r29 = r29 + 0x1;

    }
    return;
}

/* 0x8001D718 | 0xCC */
void fn_8001D718(void) {
    extern u8 lbl_8047B7C8[];
    extern u8 lbl_8047B7D0[];
    extern u8 lbl_8047B7D8[];
    extern void fn_800D3088();
    extern void fn_800D37CC();
    extern void fn_800F0308();
    u8 sp[0x70];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    f27 = f1;
    f28 = *(f32*)lbl_8047B7C8;
    f29 = *(f64*)lbl_8047B7D0;
    r31 = 0x43300000;
    f31 = *(f64*)lbl_8047B7D8;
    while (f28 < f27) {

        fn_800F0308();
        fn_800D37CC();
        *(u32*)(sp + 0xC) = tmp;
        f30 = f0 - f29;
        fn_800D3088();
        f0 = f0 - f31;
        f0 = f0 / f30;
        f28 = f28 + f0;

    }
    return;
}

/* 0x50 | fn_8001D7E4 | multi_call_cond */
u32 fn_8001D7E4(void) {
    if (fn_800F0308() == 0) { return 1; }
    fn_800F7AF0();
    fn_800F7BC4();
    return 1;
}

/* 0x8001D834 | 0xB4 */
void fn_8001D834(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    f32 f7 = 0.0f;

    r6 = *(u8*)((u8*)r4 + 0x67);
    r5 = 0x80810000;
    tmp = *(u8*)((u8*)r3 + 0x8B);
    r8 = *(u8*)((u8*)r4 + 0x66);
    r7 = *(u8*)((u8*)r3 + 0x8A);
    r9 = r6 * tmp;
    r6 = *(u8*)((u8*)r4 + 0x64);
    r5 = *(u8*)((u8*)r3 + 0x88);
    r4 = *(u8*)((u8*)r4 + 0x65);
    tmp = *(u8*)((u8*)r3 + 0x89);
    r3 = r8 * r7;
    r5 = r6 * r5;
    tmp = r4 * tmp;
    r4 = (s32)((s64)r10 * (s64)r9 >> 32);
    r6 = (s32)((s64)r10 * (s64)r3 >> 32);
    r4 = r4 + r9;
    r7 = (s32)r4 >> 7;
    r4 = (s32)((s64)r10 * (s64)r5 >> 32);
    r8 = (u32)r7 >> 31;
    r3 = r6 + r3;
    r7 = r7 + r8;
    r6 = (s32)r3 >> 7;
    r7 = r7 & 0xFF;
    r3 = (s32)((s64)r10 * (s64)tmp >> 32);
    r4 = r4 + r5;
    r5 = (u32)r6 >> 31;
    r4 = (s32)r4 >> 7;
    r6 = r6 + r5;
    r5 = (u32)r4 >> 31;
    tmp = r3 + tmp;
    r4 = r4 + r5;
    tmp = (s32)tmp >> 7;
    r5 = r6 & 0xFF;
    r3 = (u32)tmp >> 31;
    r4 = r4 & 0xFF;
    tmp = tmp + r3;
    r5 = r5 << 8;
    tmp = tmp & 0xFF;
    r3 = r4 << 24;
    tmp = tmp << 16;
    tmp = r3 | tmp;
    tmp = r5 | tmp;
    r3 = r7 | tmp;
    return;
}

/* 0x8001D8E8 | 0xAC */
void fn_8001D8E8(void) {
    extern void fn_80102568();
    extern void fn_801026A4();
    extern void fn_80102868();
    extern void fn_801043A4();
    extern void fn_801045A8();
    extern void fn_801046B8();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r31 = r7;
    fn_801046B8();
    r4 = r3;
    r10 = r28;
    r5 = (u32)sp + 0x10;
    r9 = r27 & 0xFF;
    r3 = 0xe7;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x3;
    fn_801026A4();
    r4 = (s16)r30;
    r5 = (s16)r31;
    r3 = 0xe7;
    fn_80102868();
    r3 = 0xe7;
    r4 = 0x1;
    fn_801045A8();
    r3 = 0xe7;
    fn_801043A4();
    tmp = r3;
    r3 = 0xe7;
    r31 = tmp;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
    r3 = r31;
    return;
}

/* 0x8001D994 | 0xCC */
void fn_8001D994(void) {
    extern void fn_800F9EE4();
    extern void fn_800FA280();
    extern void fn_8011E760();
    extern void fn_8011E778();
    extern void fn_8011F5C8();
    extern void fn_801231A4();
    extern void fn_80123FBC();
    extern void fn_8012640C();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    fn_80123FBC();
    tmp = r3 & 0xFF;
    if (tmp == 0) {
        r3 = 0xff;
        return;
    }
    r3 = r30;
    fn_8011F5C8();
    tmp = r3 & 0xFFFF;

    do {
        if (tmp != 0x1d && tmp != 0x20) break;

        r3 = r30;
        r4 = 0x0;
        r5 = 0x6e;
        r6 = 0x0;
        fn_8012640C();
        r3 = r3 & 0xFFFF;
        fn_8011E778();
        if (r3 == 0) break;
        fn_8011E760();
        fn_800FA280();
        tmp = r3;
        r3 = r30;
        r31 = tmp;
        r4 = 0x0;
        r5 = 0x77;
        r6 = 0x0;
        fn_8012640C();
        r4 = r31;
        fn_800F9EE4();
        if ((s32)r3 != 0) break;
        r3 = 0x2;
        return;
    } while (0);
    r3 = r30;
    fn_801231A4();

    return;
}

/* 0x8001DACC | 0x4DC */
void fn_8001DACC(void) {
    extern u8 lbl_8047AD00[];
    extern u8 lbl_8047B7D0[];
    extern u8 lbl_8047B7D8[];
    extern u8 lbl_8047B7E0[];
    extern void fn_800D59B8();
    extern void fn_800D5CB8();
    extern void fn_800D61E4();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D848C();
    extern void fn_800D85D4();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    extern void fn_800DBFD4();
    extern void fn_800DC04C();
    extern void fn_800DC0D4();
    extern void fn_800DC14C();
    extern void fn_800DC1D4();
    extern void fn_800DC224();
    extern void fn_800EF4F4();
    extern void fn_800EF4FC();
    extern void fn_800EF590();
    extern void fn_800F92D4();
    u8 sp[0xF0];
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
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f4 = 0.0f;
    f32 f26 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    r29 = r4;
    f29 = f1;
    r3 = *(u32*)((u8*)r29 + 0x58);
    fn_800F92D4();
    r4 = 0xEF0000;
    r31 = r3;
    r3 = r4 + 0x1200;
    fn_800F92D4();
    r30 = r3;
    if ((r31 != 0) && (r30 != 0)) {

        r3 = r31;
        fn_800EF4FC();
        tmp = *(s16*)((u8*)r29 + 0x5C);
        r3 = r3 & 0xFFFF;
        r4 = 0x43300000;
        f2 = *(f64*)lbl_8047B7D8;
        r3 = r31;
        f1 = *(f64*)lbl_8047B7D0;
        *(u32*)(sp + 0x44) = tmp;
        f2 = f0 - f2;
        f0 = f0 - f1;
        f31 = f0 / f2;
        fn_800EF4FC();
        r4 = *(s16*)((u8*)r29 + 0x5C);
        r3 = r3 & 0xFFFF;
        tmp = *(s16*)((u8*)r29 + 0x60);
        r5 = 0x43300000;
        r3 = r31;
        tmp = r4 + tmp;
        f2 = *(f64*)lbl_8047B7D8;
        f1 = *(f64*)lbl_8047B7D0;
        *(u32*)(sp + 0x54) = tmp;
        f2 = f0 - f2;
        f0 = f0 - f1;
        f30 = f0 / f2;
        fn_800EF4F4();
        tmp = *(s16*)((u8*)r29 + 0x5E);
        r3 = r3 & 0xFFFF;
        r4 = 0x43300000;
        f2 = *(f64*)lbl_8047B7D8;
        r3 = r31;
        f1 = *(f64*)lbl_8047B7D0;
        *(u32*)(sp + 0x64) = tmp;
        f2 = f0 - f2;
        f0 = f0 - f1;
        f27 = f0 / f2;
        fn_800EF4F4();
        r4 = *(s16*)((u8*)r29 + 0x5E);
        r5 = 0x43300000;
        tmp = *(s16*)((u8*)r29 + 0x62);
        r6 = r3 & 0xFFFF;
        r3 = 0x80000000;
        tmp = r4 + tmp;
        f2 = *(f64*)lbl_8047B7D8;
        f1 = *(f64*)lbl_8047B7D0;
        r3 = r3 + 0x3;
        *(u32*)(sp + 0x74) = tmp;
        f2 = f0 - f2;
        f0 = f0 - f1;
        f26 = f0 / f2;
        fn_800D88DC();
        r3 = 0x4;
        fn_800D888C();
        r3 = r30;
        r4 = 0x1;
        r5 = 0x1;
        fn_800EF590();
        r6 = (u32)sp + 0x8;
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x4;
        fn_800D848C();
        r6 = (u32)sp + 0x8;
        r3 = 0x1;
        r4 = 0x0;
        r5 = 0x5;
        fn_800D848C();
        r6 = (u32)sp + 0x8;
        r3 = 0x2;
        r4 = 0x0;
        r5 = 0x6;
        fn_800D848C();
        r3 = 0x3;
        fn_800DC1D4();
        r4 = r31;
        r3 = 0x0;
        fn_800D85D4();
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x0;
        r7 = 0x0;
        fn_800DC224();
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x0;
        r7 = 0x0;
        r8 = 0x1;
        fn_800DC14C();
        r3 = 0x0;
        r4 = 0xf;
        r5 = 0x8;
        r6 = 0xa;
        r7 = 0xf;
        fn_800DC0D4();
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x0;
        r7 = 0x0;
        r8 = 0x1;
        fn_800DC04C();
        r3 = 0x0;
        r4 = 0x7;
        r5 = 0x4;
        r6 = 0x5;
        r7 = 0x7;
        fn_800DBFD4();
        r4 = r30;
        r3 = 0x1;
        fn_800D85D4();
        r3 = 0x1;
        r4 = 0x0;
        r5 = 0x1;
        r6 = 0x1;
        r7 = 0x0;
        fn_800DC224();
        r3 = 0x1;
        r4 = 0x1;
        r5 = 0x0;
        r6 = 0x0;
        r7 = 0x1;
        r8 = 0x0;
        fn_800DC14C();
        r3 = 0x1;
        r4 = 0xf;
        r5 = 0x8;
        r6 = 0xd;
        r7 = 0x2;
        fn_800DC0D4();
        r3 = 0x1;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x0;
        r7 = 0x1;
        r8 = 0x0;
        fn_800DC04C();
        r3 = 0x1;
        r4 = 0x7;
        r5 = 0x7;
        r6 = 0x7;
        r7 = 0x1;
        fn_800DBFD4();
        r4 = r30;
        r3 = 0x2;
        fn_800D85D4();
        r3 = 0x2;
        r4 = 0x0;
        r5 = 0x2;
        r6 = 0x2;
        r7 = 0x0;
        fn_800DC224();
        r3 = 0x2;
        r4 = 0x1;
        r5 = 0x0;
        r6 = 0x0;
        r7 = 0x1;
        r8 = 0x0;
        fn_800DC14C();
        r3 = 0x2;
        r4 = 0xf;
        r5 = 0x8;
        r6 = 0xd;
        r7 = 0x2;
        fn_800DC0D4();
        r3 = 0x2;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x0;
        r7 = 0x1;
        r8 = 0x0;
        fn_800DC04C();
        r3 = 0x2;
        r4 = 0x7;
        r5 = 0x7;
        r6 = 0x7;
        r7 = 0x1;
        fn_800DBFD4();
        r3 = *(u32*)lbl_8047AD00;
        fn_800D7820();
        r3 = 0x7;
        fn_800D6A00();
        r3 = 0x2;
        fn_800D67BC();
        r3 = 0x0;
        r4 = 0x0;
        fn_800D61E4();
        r3 = 0x0;
        r4 = 0xff;
        r5 = 0x40;
        r6 = 0x40;
        r7 = 0xff;
        fn_800D5CB8();
        f1 = f31;
        r3 = 0x0;
        f2 = f27;
        fn_800D59B8();
        f28 = f29 + f27;
        r3 = 0x1;
        f1 = f29 + f31;
        f2 = f28;
        fn_800D59B8();
        f0 = *(f32*)lbl_8047B7E0;
        f2 = f28;
        r3 = 0x2;
        f27 = f0 - f29;
        f1 = f27 + f31;
        fn_800D59B8();
        r3 = *(s16*)((u8*)r29 + 0x54);
        r4 = *(s16*)((u8*)r29 + 0x56);
        fn_800D61E4();
        r3 = 0x0;
        r4 = 0xff;
        r5 = 0x40;
        r6 = 0x40;
        r7 = 0xff;
        fn_800D5CB8();
        f1 = f30;
        r3 = 0x0;
        f2 = f26;
        fn_800D59B8();
        f28 = f29 + f26;
        r3 = 0x1;
        f1 = f29 + f30;
        f2 = f28;
        fn_800D59B8();
        f2 = f28;
        r3 = 0x2;
        f1 = f27 + f30;
        fn_800D59B8();
        fn_800D6728();
        r3 = 0x1;
        fn_800DC1D4();
        r3 = 0x80000000;
        fn_800D888C();
    }
    r3 = 0x0;
    return;
}

/* 0x64 | fn_8001DFA8 | generic_call_check_store */
void fn_8001DFA8(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    void* result = (void*)fn_8005D8B8(arg1);
    if (result == NULL) { return; }
    /* store to offset 0x66 */
    /* store to offset 0x65 */
    /* store to offset 0x64 */
    /* store to offset 0x66 */
    /* store to offset 0x65 */
    /* store to offset 0x64 */
}

/* 0x68 | fn_8001E00C | call_sequence */
void fn_8001E00C(void) {
    fn_801046B8();
    fn_801026A4();
    fn_80102568();
}

/* 0x8001E3E0 | 0xD4 */
void fn_8001E3E0(void) {
    extern void fn_80102510();
    extern void fn_801026A4();
    extern void fn_80102868();
    extern void fn_801043A4();
    extern void fn_801045A8();
    extern void fn_801046B8();
    extern void fn_80104704();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r3;
    r30 = r4;
    r31 = 0x0;
    fn_801046B8();
    tmp = 0x0;
    r4 = r3;
    *(u32*)(sp + 0x8) = tmp;
    r9 = r29;
    r3 = 0x2;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x3;
    r10 = 0x1;
    fn_801026A4();
    r3 = 0x2;
    r4 = 0x32;
    r5 = 0x3c;
    fn_80102868();
    r3 = 0x2;
    r4 = 0x1;
    fn_801045A8();
    r3 = 0x2;
    fn_801043A4();
    r3 = 0x2;
    fn_80104704();
    if (r3 != 0) {
        if (r30 != 0) {
            tmp = *(u32*)((u8*)r3 + 0x80);
            *(u32*)((u8*)r30 + 0x0) = tmp;
        }
        tmp = *(u8*)((u8*)r3 + 0x99);
        if (tmp == 0) {
            r31 = 0x1;
        }
        r3 = 0x2;
        fn_80102510();
    }
    r3 = r31;
    return;
}

/* 0x8001E4B4 | 0xD8 */
void fn_8001E4B4(void) {
    extern u8 lbl_80314E08[];
    extern void fn_800D5CB8();
    extern void fn_800D61E4();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r26 = r3;
    r27 = r4;
    r28 = r5;
    r29 = r6;
    r30 = r7;
    r31 = r8;
    r3 = 0x1;
    fn_800D88DC();
    r3 = 0x6;
    fn_800D888C();
    r3 = 0x3;
    fn_800D6A00();
    r3 = (u32)lbl_80314E08;
    r3 = (u32)lbl_80314E08;
    fn_800D7820();
    r3 = 0x3;
    fn_800D67BC();
    r3 = (s16)r26;
    r4 = (s16)r27;
    fn_800D61E4();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    r7 = 0xff;
    fn_800D5CB8();
    r3 = (s16)r28;
    r4 = (s16)r29;
    fn_800D61E4();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    r7 = 0xff;
    fn_800D5CB8();
    r3 = (s16)r30;
    r4 = (s16)r31;
    fn_800D61E4();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    r7 = 0xff;
    fn_800D5CB8();
    fn_800D6728();
    return;
}

/* 0x8001E644 | 0x454 */
void fn_8001E644(void) {
    extern u8 lbl_80266C20[];
    extern u8 lbl_80314E08[];
    extern u8 lbl_8047B7D8[];
    extern u8 lbl_8047B7E0[];
    extern u8 lbl_8047B7E4[];
    extern void fn_8005D858();
    extern void fn_8005D934();
    extern void fn_800D5648();
    extern void fn_800D5BA0();
    extern void fn_800D61E4();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    extern void fn_801040F0();
    extern void fn_80104160();
    u8 sp[0x80];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
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
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;

    r28 = r3;
    r25 = r4;
    r27 = r5;
    r26 = r6;
    r6 = 0x43300000;
    tmp = r7 & 0xFF;
    *(u32*)(sp + 0x1C) = tmp;
    r3 = (u32)lbl_80266C20;
    r7 = (u32)lbl_80266C20;
    f3 = *(f64*)lbl_8047B7D8;
    r3 = 0x1;
    r5 = *(u32*)((u8*)r7 + 0x0);
    f0 = *(f32*)lbl_8047B7E4;
    f1 = f1 - f3;
    r4 = *(u32*)((u8*)r7 + 0x4);
    tmp = *(u32*)((u8*)r7 + 0x8);
    f4 = f1 / f0;
    r5 = *(u8*)(sp + 0xB);
    *(u32*)(sp + 0x10) = tmp;
    r4 = *(u8*)(sp + 0xF);
    tmp = *(u8*)(sp + 0x13);
    f1 = f0 - f3;
    f2 = f1 * f4;
    *(u32*)(sp + 0x44) = tmp;
    f1 = f0 - f3;
    f2 = (f64)(s32)f2;
    f1 = f1 * f4;
    f0 = f0 - f3;
    f1 = (f64)(s32)f1;
    f0 = f0 * f4;
    *(u8*)(sp + 0xB) = tmp;
    f0 = (f64)(s32)f0;
    *(u8*)(sp + 0xF) = r4;
    *(u8*)(sp + 0x13) = tmp;
    fn_800D88DC();
    r3 = 0x6;
    fn_800D888C();
    r3 = 0x6;
    fn_800D6A00();
    r3 = (u32)lbl_80314E08;
    r3 = (u32)lbl_80314E08;
    fn_800D7820();
    r31 = (s16)r3;
    r3 = 0x4;
    r23 = (s16)tmp;
    fn_800D67BC();
    r3 = r31;
    r4 = r23;
    fn_800D61E4();
    r3 = 0x0;
    fn_800D5BA0();
    tmp = r27 + 0x14;
    r4 = r23;
    tmp = (s16)tmp;
    r30 = r31 + tmp;
    r3 = (s16)r30;
    fn_800D61E4();
    r3 = 0x0;
    fn_800D5BA0();
    tmp = r26 + 0x14;
    r3 = (s16)r30;
    r24 = (s16)tmp;
    r29 = r23 + r24;
    r4 = (s16)r29;
    fn_800D61E4();
    r3 = 0x0;
    fn_800D5BA0();
    r3 = r31;
    r4 = (s16)r29;
    fn_800D61E4();
    r3 = 0x0;
    fn_800D5BA0();
    fn_800D6728();
    r3 = 0x1;
    fn_800D6A00();
    r3 = (u32)lbl_80314E08;
    r3 = (u32)lbl_80314E08;
    fn_800D7820();
    f1 = *(f32*)lbl_8047B7E0;
    fn_800D5648();
    r29 = r23;
    r23 = 0x0;
    while ((s32)r23 < (s32)r24) {

        r3 = 0x2;
        fn_800D67BC();
        r3 = r31;
        r4 = (s16)r29;
        fn_800D61E4();
        r3 = 0x0;
        fn_800D5BA0();
        r3 = (s16)r30;
        r4 = (s16)r29;
        fn_800D61E4();
        r3 = 0x0;
        fn_800D5BA0();
        fn_800D6728();
        r29 = r29 + 0x4;
        r23 = r23 + 0x4;

    }
    r3 = 0xbb;
    fn_8005D858();
    r6 = *(s16*)((u8*)r3 + 0xC);
    r5 = r28 + r27;
    r4 = *(s16*)((u8*)r3 + 0xE);
    r3 = 0xb8;
    tmp = (u32)r6 >> 31;
    r6 = tmp + r6;
    tmp = (u32)r4 >> 31;
    r8 = (s32)r6 >> 1;
    tmp = tmp + r4;
    r9 = (s32)tmp >> 1;
    r7 = r28 - r8;
    r4 = r25 - r9;
    r6 = r28 + r8;
    r5 = r5 - r8;
    r31 = (s16)r7;
    tmp = r25 + tmp;
    r30 = (s16)r6;
    r29 = (s16)r5;
    r28 = (s16)r4;
    r27 = (s16)tmp;
    fn_8005D858();
    r7 = *(s16*)((u8*)r3 + 0xE);
    tmp = r25 + r26;
    r3 = r31;
    r4 = r28;
    r6 = (u32)r7 >> 31;
    r5 = 0x0;
    r7 = r6 + r7;
    r6 = 0xbb;
    r8 = (s32)r7 >> 1;
    r7 = 0x0;
    r8 = tmp - r8;
    tmp = r8 + 0xa;
    r26 = (s16)tmp;
    fn_801040F0();
    r3 = r29;
    r4 = r28;
    r5 = 0x0;
    r6 = 0xbb;
    r7 = 0x1;
    fn_801040F0();
    r3 = r31;
    r4 = r26;
    r5 = 0x0;
    r6 = 0xb8;
    r7 = 0x0;
    fn_801040F0();
    r3 = r29;
    r4 = r26;
    r5 = 0x0;
    r6 = 0xb8;
    r7 = 0x1;
    fn_801040F0();
    r3 = 0xba;
    fn_8005D858();
    r24 = r3;
    r3 = 0x84;
    fn_8005D934();
    r25 = r3;
    r3 = 0x87;
    fn_8005D934();
    r7 = *(s16*)((u8*)r25 + 0x2);
    r23 = r26 - r27;
    tmp = *(s16*)((u8*)r3 + 0x2);
    r4 = r27;
    r5 = *(s16*)((u8*)r24 + 0xC);
    r6 = (s16)r23;
    tmp = tmp - r7;
    r7 = -0x1;
    tmp = (s16)tmp;
    r8 = 0x0;
    tmp = r31 + tmp;
    r9 = 0xba;
    r3 = (s16)tmp;
    r10 = 0x0;
    fn_80104160();
    r3 = 0x85;
    fn_8005D934();
    r25 = r3;
    r3 = 0x86;
    fn_8005D934();
    r8 = *(s16*)((u8*)r25 + 0x2);
    r4 = r27;
    tmp = *(s16*)((u8*)r3 + 0x2);
    r6 = (s16)r23;
    r5 = *(s16*)((u8*)r24 + 0xC);
    r7 = -0x1;
    tmp = tmp - r8;
    r8 = 0x0;
    tmp = (s16)tmp;
    r9 = 0xba;
    tmp = r29 + tmp;
    r10 = 0x0;
    r3 = (s16)tmp;
    fn_80104160();
    r3 = 0xb7;
    fn_8005D858();
    r27 = r3;
    r3 = 0x84;
    fn_8005D934();
    r25 = r3;
    r3 = 0x8b;
    fn_8005D934();
    r5 = r3;
    r23 = r29 - r30;
    r4 = *(s16*)((u8*)r25 + 0x4);
    r3 = r30;
    tmp = *(s16*)((u8*)r5 + 0x4);
    r5 = (s16)r23;
    r6 = *(s16*)((u8*)r27 + 0xE);
    r7 = -0x1;
    tmp = tmp - r4;
    r8 = 0x0;
    tmp = (s16)tmp;
    r9 = 0xb7;
    tmp = r28 + tmp;
    r10 = 0x0;
    r4 = (s16)tmp;
    fn_80104160();
    r3 = 0x88;
    fn_8005D934();
    r25 = r3;
    r3 = 0x8a;
    fn_8005D934();
    tmp = *(s16*)((u8*)r3 + 0x4);
    r3 = r30;
    r4 = *(s16*)((u8*)r25 + 0x4);
    r5 = (s16)r23;
    r6 = *(s16*)((u8*)r27 + 0xE);
    r7 = -0x1;
    tmp = tmp - r4;
    r8 = 0x0;
    tmp = (s16)tmp;
    r9 = 0xb7;
    tmp = r26 + tmp;
    r10 = 0x0;
    r4 = (s16)tmp;
    fn_80104160();
    return;
}

/* 0x8001EA98 | 0x170 */
void fn_8001EA98(void) {
    extern u8 lbl_80314E08[];
    extern u8 lbl_8047B7E0[];
    extern void fn_800D5648();
    extern void fn_800D5BA0();
    extern void fn_800D61E4();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    r28 = r3;
    r29 = r4;
    r31 = r5;
    r30 = r6;
    r3 = 0x1;
    fn_800D88DC();
    r3 = 0x6;
    fn_800D888C();
    r3 = 0x7;
    fn_800D6A00();
    r3 = (u32)lbl_80314E08;
    r3 = (u32)lbl_80314E08;
    fn_800D7820();
    r3 = 0x2;
    fn_800D67BC();
    r3 = (s16)r3;
    r4 = (s16)tmp;
    fn_800D61E4();
    r3 = 0x0;
    r4 = 0xc0;
    fn_800D5BA0();
    r31 = r31 + 0xa;
    r30 = r30 + 0xa;
    r31 = r28 + r31;
    r30 = r29 + r30;
    r3 = (s16)r31;
    r4 = (s16)r30;
    fn_800D61E4();
    r3 = 0x0;
    r4 = 0xc0;
    fn_800D5BA0();
    fn_800D6728();
    f1 = *(f32*)lbl_8047B7E0;
    fn_800D5648();
    r3 = 0x2;
    fn_800D6A00();
    r3 = 0x5;
    fn_800D67BC();
    r3 = (s16)r3;
    r4 = (s16)tmp;
    fn_800D61E4();
    r3 = 0x0;
    r4 = -0x1;
    fn_800D5BA0();
    r3 = (s16)r31;
    r4 = (s16)tmp;
    fn_800D61E4();
    r3 = 0x0;
    r4 = -0x1;
    fn_800D5BA0();
    r3 = (s16)r31;
    r4 = (s16)r30;
    fn_800D61E4();
    r3 = 0x0;
    r4 = -0x1;
    fn_800D5BA0();
    r4 = (s16)r30;
    r3 = (s16)tmp;
    fn_800D61E4();
    r3 = 0x0;
    r4 = -0x1;
    fn_800D5BA0();
    r3 = (s16)r3;
    r4 = (s16)tmp;
    fn_800D61E4();
    r3 = 0x0;
    r4 = -0x1;
    fn_800D5BA0();
    fn_800D6728();
    return;
}

/* 0x8001EC08 | 0x370 */
void fn_8001EC08(void) {
    extern u8 lbl_80314E08[];
    extern u8 lbl_803A1D60[];
    extern u8 lbl_8047B7D0[];
    extern u8 lbl_8047B7E8[];
    extern u8 lbl_8047B7F0[];
    extern u8 lbl_8047B7F8[];
    extern u8 lbl_8047B800[];
    extern void fn_800CDBE0();
    extern void fn_800CE148();
    extern void fn_800D5BA0();
    extern void fn_800D61E4();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    u8 sp[0xB0];
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
    f32 f26 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    r11 = (u32)sp + 0xb0;
    tmp = r8 & 0xFF;
    r8 = (u32)lbl_803A1D60;
    r27 = (u32)lbl_803A1D60;
    if (tmp != 0) {
        r7 = r7 & 0xFF;
        tmp = (u32)r7 >> 31;
        tmp = tmp + r7;
        tmp = (s32)tmp >> 1;
        r7 = tmp & 0xFF;
    }
    r8 = (u32)r5 >> 31;
    tmp = (u32)r6 >> 31;
    r8 = r8 + r5;
    r9 = r6 + r4;
    r8 = (s32)r8 >> 1;
    tmp = tmp + r6;
    tmp = (s32)tmp >> 1;
    r6 = r5 + r3;
    r9 = r9 + 0x2;
    r5 = r8 + r3;
    tmp = tmp + r4;
    r10 = (s16)r6;
    r28 = r27 + 0x0;
    r9 = (s16)r9;
    r8 = (s16)r4;
    r6 = (s16)r3;
    r5 = (s16)r5;
    tmp = (s16)tmp;
    r24 = r7 & 0xFF;
    r23 = r27 + 0x128;
    r22 = r27 + 0x28;
    *(u16*)((u8*)r27 + 0x0) = r10;
    r26 = r23;
    r21 = 0x0;
    *(u16*)((u8*)r28 + 0x2) = r9;
    r25 = r22;
    *(u16*)((u8*)r28 + 0x8) = r10;
    *(u16*)((u8*)r28 + 0xA) = r8;
    *(u16*)((u8*)r28 + 0x10) = r6;
    *(u16*)((u8*)r28 + 0x12) = r8;
    *(u16*)((u8*)r28 + 0x18) = r6;
    *(u16*)((u8*)r28 + 0x1A) = r9;
    *(u16*)((u8*)r28 + 0x20) = r5;
    *(u16*)((u8*)r28 + 0x22) = tmp;
    *(u32*)((u8*)r28 + 0x24) = r24;
    f28 = *(f64*)lbl_8047B7F0;
    f29 = *(f64*)lbl_8047B7D0;
    f30 = *(f64*)lbl_8047B7F8;
    r29 = 0x43300000;
    f31 = *(f64*)lbl_8047B7E8;
    f27 = *(f64*)lbl_8047B800;
    do {
        *(u32*)(sp + 0xC) = tmp;
        f0 = f0 - f29;
        f0 = f28 * f0;
        f26 = f0 * f30;
        f1 = f26;
        fn_800CE148();
        f0 = f0 - f29;
        f0 = f31 * f1 + f0;
        f1 = f26;
        f0 = (f64)(s32)f0;
        *(u16*)((u8*)r26 + 0x0) = tmp;
        fn_800CDBE0();
        tmp = 0x0;
        f0 = f0 - f29;
        f0 = f31 * f1 + f0;
        f1 = f26;
        f0 = (f64)(s32)f0;
        *(u16*)((u8*)r26 + 0x2) = r3;
        *(u32*)((u8*)r26 + 0x4) = tmp;
        fn_800CE148();
        f0 = f0 - f29;
        f0 = f27 * f1 + f0;
        f1 = f26;
        f0 = (f64)(s32)f0;
        *(u16*)((u8*)r25 + 0x0) = tmp;
        fn_800CDBE0();
        tmp = (s32)r21 >> 3;
        r3 = r27 + 0x0;
        tmp = tmp << 3;
        r3 = r3 + tmp;
        tmp = *(s16*)((u8*)r26 + 0x0);
        r5 = r3 + 0x2;
        r4 = *(s16*)((u8*)r3 + 0x0);
        r21 = r21 + 0x1;
        f0 = f0 - f29;
        r3 = *(s16*)((u8*)r5 + 0x0);
        tmp = tmp + r4;
        *(u16*)((u8*)r26 + 0x0) = tmp;
        f0 = f27 * f1 + f0;
        tmp = *(s16*)((u8*)r26 + 0x2);
        tmp = tmp + r3;
        f0 = (f64)(s32)f0;
        *(u16*)((u8*)r26 + 0x2) = tmp;
        r26 = r26 + 0x8;
        *(u16*)((u8*)r25 + 0x2) = tmp;
        *(u32*)((u8*)r25 + 0x4) = r24;
        tmp = *(s16*)((u8*)r25 + 0x0);
        tmp = tmp + r4;
        *(u16*)((u8*)r25 + 0x0) = tmp;
        tmp = *(s16*)((u8*)r25 + 0x2);
        tmp = tmp + r3;
        *(u16*)((u8*)r25 + 0x2) = tmp;
        r25 = r25 + 0x8;
    } while ((s32)r21 < 0x20);
    r3 = 0x1;
    fn_800D88DC();
    r3 = 0x6;
    fn_800D888C();
    r3 = 0x4;
    fn_800D6A00();
    r3 = (u32)lbl_80314E08;
    r3 = (u32)lbl_80314E08;
    fn_800D7820();
    r3 = 0x42;
    fn_800D67BC();
    r21 = r22;
    r24 = 0x0;
    do {
        r3 = *(s16*)((u8*)r23 + 0x0);
        r4 = *(s16*)((u8*)r23 + 0x2);
        fn_800D61E4();
        r4 = *(u32*)((u8*)r23 + 0x4);
        r3 = 0x0;
        fn_800D5BA0();
        r3 = *(s16*)((u8*)r21 + 0x0);
        r4 = *(s16*)((u8*)r21 + 0x2);
        fn_800D61E4();
        r4 = *(u32*)((u8*)r21 + 0x4);
        r3 = 0x0;
        fn_800D5BA0();
        r23 = r23 + 0x8;
        r21 = r21 + 0x8;
        r24 = r24 + 0x1;
    } while ((s32)r24 < 0x20);
    r4 = r27 + 0x128;
    r3 = *(s16*)((u8*)r27 + 0x128);
    r4 = *(s16*)((u8*)r4 + 0x2);
    fn_800D61E4();
    r4 = r27 + 0x128;
    r3 = 0x0;
    r4 = *(u32*)((u8*)r4 + 0x4);
    fn_800D5BA0();
    r21 = r27 + 0x28;
    r3 = *(s16*)((u8*)r27 + 0x28);
    r4 = *(s16*)((u8*)r21 + 0x2);
    fn_800D61E4();
    r23 = r21;
    r3 = 0x0;
    r4 = *(u32*)((u8*)r23 + 0x4);
    fn_800D5BA0();
    fn_800D6728();
    r3 = 0x5;
    fn_800D6A00();
    r3 = 0x22;
    fn_800D67BC();
    r3 = *(s16*)((u8*)r28 + 0x20);
    r4 = *(s16*)((u8*)r28 + 0x22);
    fn_800D61E4();
    r4 = *(u32*)((u8*)r28 + 0x24);
    r3 = 0x0;
    fn_800D5BA0();
    r24 = 0x0;
    do {
        r3 = *(s16*)((u8*)r22 + 0x0);
        r4 = *(s16*)((u8*)r22 + 0x2);
        fn_800D61E4();
        r4 = *(u32*)((u8*)r22 + 0x4);
        r3 = 0x0;
        fn_800D5BA0();
        r22 = r22 + 0x8;
        r24 = r24 + 0x1;
    } while ((s32)r24 < 0x20);
    r3 = *(s16*)((u8*)r27 + 0x28);
    r4 = *(s16*)((u8*)r21 + 0x2);
    fn_800D61E4();
    r4 = *(u32*)((u8*)r23 + 0x4);
    r3 = 0x0;
    fn_800D5BA0();
    fn_800D6728();
    r11 = (u32)sp + 0xb0;
    return;
}

/* 0x8001EF78 | 0x270 */
void fn_8001EF78(void) {
    extern u8 lbl_803A1F88[];
    extern u8 lbl_8047A31C[];
    extern u8 lbl_8047A334[];
    extern u8 lbl_8047A338[];
    extern u8 lbl_8047A344[];
    extern u8 lbl_8047B810[];
    extern u8 lbl_8047B814[];
    extern u8 lbl_8047B818[];
    extern u8 lbl_8047B81C[];
    extern u8 lbl_8047B820[];
    extern u8 lbl_8047B824[];
    extern u8 lbl_8047B828[];
    extern u8 lbl_8047B830[];
    extern void fn_800D3088();
    extern void fn_800D37CC();
    extern void fn_801666BC();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    fn_800D37CC();
    tmp = 0x43300000;
    f1 = *(f64*)lbl_8047B828;
    *(u32*)(sp + 0x8) = tmp;
    f31 = f0 - f1;
    fn_800D3088();
    tmp = 0x43300000;
    f3 = *(f64*)lbl_8047B830;
    *(u32*)(sp + 0x10) = tmp;
    f2 = *(f32*)lbl_8047B810;
    f1 = *(f32*)lbl_8047A338;
    f3 = f0 - f3;
    f0 = *(f32*)lbl_8047B818;
    f3 = f3 / f31;
    f1 = f3 * f2 + f1;
    *(f32*)lbl_8047A344 = f3;
    *(f32*)lbl_8047A338 = f1;
    /* cror eq, gt, eq */;
    if (f1 == f0) {
        f0 = *(f32*)lbl_8047B814;
        *(f32*)lbl_8047A338 = f0;
    }
    tmp = *(u32*)lbl_8047A31C;
    if ((s32)tmp < 0x1e) return;
    if ((s32)tmp == 0xc8) return;
    f0 = *(f32*)lbl_8047A334;
    r3 = 0x46a;
    f0 = f0 + f3;
    *(f32*)lbl_8047A334 = f0;
    fn_801666BC();
    if ((s32)r3 == 0) {
        tmp = *(u32*)lbl_8047A31C;
        f0 = *(f32*)lbl_8047B814;
        *(f32*)lbl_8047A334 = f0;
        if ((s32)tmp != 0x3e8) {
            tmp = 0x28;
            *(u32*)lbl_8047A31C = tmp;
    }
    }
    r3 = (u32)lbl_803A1F88;
    f0 = *(f32*)lbl_8047A344;
    r3 = (u32)lbl_803A1F88;
    tmp = 0x3;
    ctr_fn = (void(*)(void))tmp;
    do {
        f1 = *(f32*)((u8*)r3 + 0x10);
        f2 = *(f32*)((u8*)r3 + 0x28);
        do {
            if (f1 == f2) break;
            f3 = f2 - f1;
            f2 = *(f32*)lbl_8047B81C;
            f1 = *(f32*)lbl_8047B820;
            f2 = f2 * f3;
            f4 = f2 * f0;
            if (f4 > f1) {
                f4 = f1;
            }
            f1 = *(f32*)lbl_8047B824;
            /* cror eq, lt, eq */;
            if (f4 == f1) {
                f4 = f1;
            }
            f2 = *(f32*)((u8*)r3 + 0x10);
            f1 = *(f32*)lbl_8047B814;
            f2 = f2 + f4;
            *(f32*)((u8*)r3 + 0x10) = f2;
            f3 = *(f32*)((u8*)r3 + 0x28);
            f1 = *(f32*)((u8*)r3 + 0x10);
            f2 = f3 - f1;
            if (f4 > f1) {
            } else {

                f4 = -f4;
            }
            f1 = *(f32*)lbl_8047B814;
            if (f2 > f1) {
                f1 = f2;
            } else {

                f1 = -f2;
            }
            /* cror eq, lt, eq */;
            if (f1 != f4) {
                f1 = *(f32*)lbl_8047B814;
                if (f2 > f1) {
                } else {

                    f2 = -f2;
                }
                f1 = *(f32*)lbl_8047B818;
                if (f2 >= f1) break;
            }
            *(f32*)((u8*)r3 + 0x10) = f3;
        } while (0);
        f1 = *(f32*)((u8*)r3 + 0x1C);
        f2 = *(f32*)((u8*)r3 + 0x34);
        do {
            if (f1 == f2) break;
            f3 = f2 - f1;
            f2 = *(f32*)lbl_8047B81C;
            f1 = *(f32*)lbl_8047B820;
            f2 = f2 * f3;
            f4 = f2 * f0;
            if (f4 > f1) {
                f4 = f1;
            }
            f1 = *(f32*)lbl_8047B824;
            /* cror eq, lt, eq */;
            if (f4 == f1) {
                f4 = f1;
            }
            f2 = *(f32*)((u8*)r3 + 0x1C);
            f1 = *(f32*)lbl_8047B814;
            f2 = f2 + f4;
            *(f32*)((u8*)r3 + 0x1C) = f2;
            f3 = *(f32*)((u8*)r3 + 0x34);
            f1 = *(f32*)((u8*)r3 + 0x1C);
            f2 = f3 - f1;
            if (f4 > f1) {
            } else {

                f4 = -f4;
            }
            f1 = *(f32*)lbl_8047B814;
            if (f2 > f1) {
                f1 = f2;
            } else {

                f1 = -f2;
            }
            /* cror eq, lt, eq */;
            if (f1 != f4) {
                f1 = *(f32*)lbl_8047B814;
                if (f2 > f1) {
                } else {

                    f2 = -f2;
                }
                f1 = *(f32*)lbl_8047B818;
                if (f2 >= f1) break;
            }
            *(f32*)((u8*)r3 + 0x1C) = f3;
        } while (0);
        r3 = r3 + 0x4;
    } while (--ctr != 0);

    return;
}

/* 0x8001F1E8 | 0x11C */
void fn_8001F1E8(void) {
    extern u8 lbl_8047A31C[];
    extern u8 lbl_8047A328[];
    extern void fn_800F7AF0();
    extern void fn_800F7BC4();
    extern void fn_80105624();
    extern void fn_801337A8();
    extern void fn_801669E4();
    extern void fn_80166AB8();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    r31 = r3;
    tmp = *(u32*)lbl_8047A31C;
    if ((s32)tmp < 4) {
        r3 = 0x0;
        fn_801337A8();
        if (r31 == 0) return;
        r3 = *(u32*)((u8*)r31 + 0x4);
        ((void(*)(void))fn_8005DA18)();
        r3 = 0x1;
        fn_800F7AF0();
        r31 = r3;
        r3 = 0x1;
        fn_800F7BC4();
        tmp = r3 & r31;
        tmp = tmp & 0x1100;
        if (tmp != 0) {
            tmp = 0x4;
            r3 = 0x46e;
            *(u32*)lbl_8047A31C = tmp;
            r4 = 0x0;
            r5 = 0x0;
            fn_801669E4();
            tmp = 0x1;
            *(u32*)lbl_8047A328 = tmp;
        }
        r3 = 0x1;
        fn_800F7AF0();
        r31 = r3;
        r3 = 0x1;
        fn_800F7BC4();
        tmp = r3 & r31;
        tmp = tmp & 0x00000200;
        if (tmp == 0) return;
        tmp = 0x4;
        r3 = 0x46e;
        *(u32*)lbl_8047A31C = tmp;
        r4 = 0x0;
        r5 = 0x0;
        fn_801669E4();
        tmp = 0x1;
        *(u32*)lbl_8047A328 = tmp;
        return;
    }
    r3 = 0x1;
    fn_801337A8();
    if (r31 == 0) return;
    r3 = *(u32*)((u8*)r31 + 0x4);
    ((void(*)(void))fn_8005DA18)();
    fn_80105624();
    tmp = *(u16*)((u8*)r3 + 0x4);
    tmp = tmp & 0x810;
    if ((s32)tmp == 0) return;
    tmp = 0x1;
    r3 = 0x4c2;
    *(u8*)((u8*)r31 + 0x98) = tmp;
    r4 = 0x0;
    r5 = 0x0;
    fn_80166AB8();

    return;
}

/* 0x8001F304 | 0xA44 */
void fn_8001F304(void) {
    extern u8 lbl_802EF0A8[];
    extern u8 lbl_803A1F88[];
    extern u8 lbl_80478878[];
    extern u8 lbl_8047A31C[];
    extern u8 lbl_8047A338[];
    extern u8 lbl_8047A34C[];
    extern u8 lbl_8047B814[];
    extern u8 lbl_8047B81C[];
    extern u8 lbl_8047B838[];
    extern u8 lbl_8047B83C[];
    extern u8 lbl_8047B840[];
    extern u8 lbl_8047B844[];
    extern u8 lbl_8047B848[];
    extern void fn_800CE148();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f8 = 0.0f;
    f32 f9 = 0.0f;

    r31 = r4;
    tmp = *(s16*)((u8*)r31 + 0x6);
    if ((s32)tmp != 0xf03) {
        if ((s32)tmp < 0xf03) {
            if ((s32)tmp != 0xefd) {
                if ((s32)tmp < 0xefd) {
                    if ((s32)tmp != 0xefa) {
                        if ((s32)tmp < 0xefa) {
                            if ((s32)tmp != 0x2c4) {
                                goto L_8001FD18;
                            }
                            if ((s32)tmp < 0xefc) {
                                goto L_8001F4C8;
                            }
                            if ((s32)tmp == 0xf00) goto L_8001F6F8;
                            if ((s32)tmp < 0xf00) {
                                if ((s32)tmp >= 0xeff) goto L_8001F688;
                                goto L_8001F618;
                            }
                            if ((s32)tmp >= 0xf02) goto L_8001F7D8;
                            goto L_8001F768;
                        }
                        if ((s32)tmp == 0xf09) goto L_8001FAE8;
                        if ((s32)tmp < 0xf09) {
                            if ((s32)tmp == 0xf06) goto L_8001F998;
                            if ((s32)tmp < 0xf06) {
                                if ((s32)tmp >= 0xf05) goto L_8001F928;
                                goto L_8001F8B8;
                            }
                            if ((s32)tmp >= 0xf08) goto L_8001FA78;
                            goto L_8001FA08;
                        }
                        if ((s32)tmp == 0xf0c) goto L_8001FC38;
                        if ((s32)tmp < 0xf0c) {
                            if ((s32)tmp >= 0xf0b) goto L_8001FBC8;
                            goto L_8001FB58;
                        }
                        if ((s32)tmp >= 0xf0e) goto L_8001FD18;
                        goto L_8001FCA8;
                            }
                    tmp = *(u32*)lbl_8047A31C;
                    if ((s32)tmp >= 0x1e) {
                        if ((s32)tmp > 0x20) {
                        }
                        tmp = *(u8*)((u8*)r31 + 0x4);
                        r3 = 0x0;
                        tmp = tmp & 0xFFFFFFFD;
                        tmp = (s8)tmp;
                        *(u8*)((u8*)r31 + 0x4) = tmp;
                        goto L_8001F410;
                        }
                    tmp = *(u8*)((u8*)r31 + 0x4);
                    r3 = 0x1;
                    tmp = tmp | 0x2;
                    tmp = (s8)tmp;
                    *(u8*)((u8*)r31 + 0x4) = tmp;
                L_8001F410:
                    tmp = r3 & 0xFF;
                    if (tmp == 0) return;
                    tmp = *(u32*)lbl_8047A31C;
                    f2 = *(f32*)lbl_8047B838;
                    f1 = *(f32*)lbl_8047A338;
                    if ((s32)tmp == 0x1f) {
                        f0 = *(f32*)lbl_8047B81C;
                    } else {

                        f0 = *(f32*)lbl_8047B83C;
                    }
                    f0 = f1 * f0;
                    f1 = f2 * f0;
                    fn_800CE148();
                    f3 = (f32)f1;
                    f2 = *(f32*)lbl_8047B844;
                    f1 = *(f32*)lbl_8047B840;
                    f0 = *(f32*)lbl_8047B848;
                    f1 = f2 * f3 + f1;
                    if (f1 > f0) {
                        f1 = f0;
                    }
                    f0 = *(f32*)lbl_8047B814;
                    if (f1 < f0) {
                        f1 = f0;
                    }
                    f0 = (f64)(s32)f1;
                    *(u8*)((u8*)r31 + 0x67) = tmp;
                    return;
                                }
                tmp = *(u32*)lbl_8047A31C;
                if ((s32)tmp >= 0x1e) {
                    if ((s32)tmp > 0x20) {
                    }
                    tmp = *(u8*)((u8*)r31 + 0x4);
                    tmp = tmp & 0xFFFFFFFD;
                    tmp = (s8)tmp;
                    *(u8*)((u8*)r31 + 0x4) = tmp;
                    return;
                    }
                tmp = *(u8*)((u8*)r31 + 0x4);
                tmp = tmp | 0x2;
                tmp = (s8)tmp;
                *(u8*)((u8*)r31 + 0x4) = tmp;
                return;
            L_8001F4C8:
                tmp = *(u32*)lbl_80478878;
                if ((s32)tmp == 0) {
                    tmp = *(u8*)((u8*)r31 + 0x4);
                    r3 = (u32)lbl_803A1F88;
                    r4 = (u32)lbl_802EF0A8;
                    tmp = tmp | 0x2;
                    r3 = (u32)lbl_803A1F88;
                    r5 = (s8)tmp;
                    tmp = (u32)lbl_802EF0A8;
                    *(u8*)((u8*)r31 + 0x4) = r5;
                    f0 = *(f32*)((u8*)r3 + 0x1C);
                    r3 = *(s16*)((u8*)r31 + 0x6);
                    f0 = (f64)(s32)f0;
                    r3 = r3 * 0x1c;
                    r3 = tmp + r3;
                    r3 = *(s16*)((u8*)r3 + 0x2);
                    tmp = r3 + tmp;
                    tmp = (s16)tmp;
                    *(u16*)((u8*)r31 + 0x50) = tmp;
                    return;
                }
                tmp = *(u8*)((u8*)r31 + 0x4);
                tmp = tmp & 0xFFFFFFFD;
                tmp = (s8)tmp;
                *(u8*)((u8*)r31 + 0x4) = tmp;
                return;
                            }
            tmp = *(u32*)lbl_80478878;
            if ((s32)tmp == 0) {
                tmp = *(u8*)((u8*)r31 + 0x4);
                r3 = (u32)lbl_803A1F88;
                r4 = (u32)lbl_802EF0A8;
                tmp = tmp | 0x2;
                r3 = (u32)lbl_803A1F88;
                r5 = (s8)tmp;
                tmp = (u32)lbl_802EF0A8;
                *(u8*)((u8*)r31 + 0x4) = r5;
                f0 = *(f32*)((u8*)r3 + 0x20);
                r3 = *(s16*)((u8*)r31 + 0x6);
                f0 = (f64)(s32)f0;
                r3 = r3 * 0x1c;
                r3 = tmp + r3;
                r3 = *(s16*)((u8*)r3 + 0x2);
                tmp = r3 + tmp;
                tmp = (s16)tmp;
                *(u16*)((u8*)r31 + 0x50) = tmp;
                return;
            }
            tmp = *(u8*)((u8*)r31 + 0x4);
            tmp = tmp & 0xFFFFFFFD;
            tmp = (s8)tmp;
            *(u8*)((u8*)r31 + 0x4) = tmp;
            return;
            }
        tmp = *(u32*)lbl_80478878;
        if ((s32)tmp == 0) {
            tmp = *(u8*)((u8*)r31 + 0x4);
            r3 = (u32)lbl_803A1F88;
            r4 = (u32)lbl_802EF0A8;
            tmp = tmp | 0x2;
            r3 = (u32)lbl_803A1F88;
            r5 = (s8)tmp;
            tmp = (u32)lbl_802EF0A8;
            *(u8*)((u8*)r31 + 0x4) = r5;
            f0 = *(f32*)((u8*)r3 + 0x10);
            r3 = *(s16*)((u8*)r31 + 0x6);
            f0 = (f64)(s32)f0;
            r3 = r3 * 0x1c;
            r3 = tmp + r3;
            r3 = *(s16*)((u8*)r3 + 0x2);
            tmp = r3 + tmp;
            tmp = (s16)tmp;
            *(u16*)((u8*)r31 + 0x50) = tmp;
            return;
        }
        tmp = *(u8*)((u8*)r31 + 0x4);
        tmp = tmp & 0xFFFFFFFD;
        tmp = (s8)tmp;
        *(u8*)((u8*)r31 + 0x4) = tmp;
        return;
    L_8001F618:
        tmp = *(u32*)lbl_80478878;
        if ((s32)tmp == 0) {
            tmp = *(u8*)((u8*)r31 + 0x4);
            r3 = (u32)lbl_803A1F88;
            r4 = (u32)lbl_802EF0A8;
            tmp = tmp | 0x2;
            r3 = (u32)lbl_803A1F88;
            r5 = (s8)tmp;
            tmp = (u32)lbl_802EF0A8;
            *(u8*)((u8*)r31 + 0x4) = r5;
            f0 = *(f32*)((u8*)r3 + 0x14);
            r3 = *(s16*)((u8*)r31 + 0x6);
            f0 = (f64)(s32)f0;
            r3 = r3 * 0x1c;
            r3 = tmp + r3;
            r3 = *(s16*)((u8*)r3 + 0x2);
            tmp = r3 + tmp;
            tmp = (s16)tmp;
            *(u16*)((u8*)r31 + 0x50) = tmp;
            return;
        }
        tmp = *(u8*)((u8*)r31 + 0x4);
        tmp = tmp & 0xFFFFFFFD;
        tmp = (s8)tmp;
        *(u8*)((u8*)r31 + 0x4) = tmp;
        return;
    L_8001F688:
        tmp = *(u32*)lbl_80478878;
        if ((s32)tmp == 1) {
            tmp = *(u8*)((u8*)r31 + 0x4);
            r3 = (u32)lbl_803A1F88;
            r4 = (u32)lbl_802EF0A8;
            tmp = tmp | 0x2;
            r3 = (u32)lbl_803A1F88;
            r5 = (s8)tmp;
            tmp = (u32)lbl_802EF0A8;
            *(u8*)((u8*)r31 + 0x4) = r5;
            f0 = *(f32*)((u8*)r3 + 0x1C);
            r3 = *(s16*)((u8*)r31 + 0x6);
            f0 = (f64)(s32)f0;
            r3 = r3 * 0x1c;
            r3 = tmp + r3;
            r3 = *(s16*)((u8*)r3 + 0x2);
            tmp = r3 + tmp;
            tmp = (s16)tmp;
            *(u16*)((u8*)r31 + 0x50) = tmp;
            return;
        }
        tmp = *(u8*)((u8*)r31 + 0x4);
        tmp = tmp & 0xFFFFFFFD;
        tmp = (s8)tmp;
        *(u8*)((u8*)r31 + 0x4) = tmp;
        return;
    L_8001F6F8:
        tmp = *(u32*)lbl_80478878;
        if ((s32)tmp == 1) {
            tmp = *(u8*)((u8*)r31 + 0x4);
            r3 = (u32)lbl_803A1F88;
            r4 = (u32)lbl_802EF0A8;
            tmp = tmp | 0x2;
            r3 = (u32)lbl_803A1F88;
            r5 = (s8)tmp;
            tmp = (u32)lbl_802EF0A8;
            *(u8*)((u8*)r31 + 0x4) = r5;
            f0 = *(f32*)((u8*)r3 + 0x20);
            r3 = *(s16*)((u8*)r31 + 0x6);
            f0 = (f64)(s32)f0;
            r3 = r3 * 0x1c;
            r3 = tmp + r3;
            r3 = *(s16*)((u8*)r3 + 0x2);
            tmp = r3 + tmp;
            tmp = (s16)tmp;
            *(u16*)((u8*)r31 + 0x50) = tmp;
            return;
        }
        tmp = *(u8*)((u8*)r31 + 0x4);
        tmp = tmp & 0xFFFFFFFD;
        tmp = (s8)tmp;
        *(u8*)((u8*)r31 + 0x4) = tmp;
        return;
    L_8001F768:
        tmp = *(u32*)lbl_80478878;
        if ((s32)tmp == 1) {
            tmp = *(u8*)((u8*)r31 + 0x4);
            r3 = (u32)lbl_803A1F88;
            r4 = (u32)lbl_802EF0A8;
            tmp = tmp | 0x2;
            r3 = (u32)lbl_803A1F88;
            r5 = (s8)tmp;
            tmp = (u32)lbl_802EF0A8;
            *(u8*)((u8*)r31 + 0x4) = r5;
            f0 = *(f32*)((u8*)r3 + 0x24);
            r3 = *(s16*)((u8*)r31 + 0x6);
            f0 = (f64)(s32)f0;
            r3 = r3 * 0x1c;
            r3 = tmp + r3;
            r3 = *(s16*)((u8*)r3 + 0x2);
            tmp = r3 + tmp;
            tmp = (s16)tmp;
            *(u16*)((u8*)r31 + 0x50) = tmp;
            return;
        }
        tmp = *(u8*)((u8*)r31 + 0x4);
        tmp = tmp & 0xFFFFFFFD;
        tmp = (s8)tmp;
        *(u8*)((u8*)r31 + 0x4) = tmp;
        return;
    L_8001F7D8:
        tmp = *(u32*)lbl_80478878;
        if ((s32)tmp == 1) {
            tmp = *(u8*)((u8*)r31 + 0x4);
            r3 = (u32)lbl_803A1F88;
            r4 = (u32)lbl_802EF0A8;
            tmp = tmp | 0x2;
            r3 = (u32)lbl_803A1F88;
            r5 = (s8)tmp;
            tmp = (u32)lbl_802EF0A8;
            *(u8*)((u8*)r31 + 0x4) = r5;
            f0 = *(f32*)((u8*)r3 + 0x10);
            r3 = *(s16*)((u8*)r31 + 0x6);
            f0 = (f64)(s32)f0;
            r3 = r3 * 0x1c;
            r3 = tmp + r3;
            r3 = *(s16*)((u8*)r3 + 0x2);
            tmp = r3 + tmp;
            tmp = (s16)tmp;
            *(u16*)((u8*)r31 + 0x50) = tmp;
            return;
        }
        tmp = *(u8*)((u8*)r31 + 0x4);
        tmp = tmp & 0xFFFFFFFD;
        tmp = (s8)tmp;
        *(u8*)((u8*)r31 + 0x4) = tmp;
        return;
    }
    tmp = *(u32*)lbl_80478878;
    if ((s32)tmp == 1) {
        tmp = *(u8*)((u8*)r31 + 0x4);
        r3 = (u32)lbl_803A1F88;
        r4 = (u32)lbl_802EF0A8;
        tmp = tmp | 0x2;
        r3 = (u32)lbl_803A1F88;
        r5 = (s8)tmp;
        tmp = (u32)lbl_802EF0A8;
        *(u8*)((u8*)r31 + 0x4) = r5;
        f0 = *(f32*)((u8*)r3 + 0x14);
        r3 = *(s16*)((u8*)r31 + 0x6);
        f0 = (f64)(s32)f0;
        r3 = r3 * 0x1c;
        r3 = tmp + r3;
        r3 = *(s16*)((u8*)r3 + 0x2);
        tmp = r3 + tmp;
        tmp = (s16)tmp;
        *(u16*)((u8*)r31 + 0x50) = tmp;
        return;
    }
    tmp = *(u8*)((u8*)r31 + 0x4);
    tmp = tmp & 0xFFFFFFFD;
    tmp = (s8)tmp;
    *(u8*)((u8*)r31 + 0x4) = tmp;
    return;
L_8001F8B8:
    tmp = *(u32*)lbl_80478878;
    if ((s32)tmp == 1) {
        tmp = *(u8*)((u8*)r31 + 0x4);
        r3 = (u32)lbl_803A1F88;
        r4 = (u32)lbl_802EF0A8;
        tmp = tmp | 0x2;
        r3 = (u32)lbl_803A1F88;
        r5 = (s8)tmp;
        tmp = (u32)lbl_802EF0A8;
        *(u8*)((u8*)r31 + 0x4) = r5;
        f0 = *(f32*)((u8*)r3 + 0x18);
        r3 = *(s16*)((u8*)r31 + 0x6);
        f0 = (f64)(s32)f0;
        r3 = r3 * 0x1c;
        r3 = tmp + r3;
        r3 = *(s16*)((u8*)r3 + 0x2);
        tmp = r3 + tmp;
        tmp = (s16)tmp;
        *(u16*)((u8*)r31 + 0x50) = tmp;
        return;
    }
    tmp = *(u8*)((u8*)r31 + 0x4);
    tmp = tmp & 0xFFFFFFFD;
    tmp = (s8)tmp;
    *(u8*)((u8*)r31 + 0x4) = tmp;
    return;
L_8001F928:
    tmp = *(u32*)lbl_80478878;
    if ((s32)tmp == 2) {
        tmp = *(u8*)((u8*)r31 + 0x4);
        r3 = (u32)lbl_803A1F88;
        r4 = (u32)lbl_802EF0A8;
        tmp = tmp | 0x2;
        r3 = (u32)lbl_803A1F88;
        r5 = (s8)tmp;
        tmp = (u32)lbl_802EF0A8;
        *(u8*)((u8*)r31 + 0x4) = r5;
        f0 = *(f32*)((u8*)r3 + 0x1C);
        r3 = *(s16*)((u8*)r31 + 0x6);
        f0 = (f64)(s32)f0;
        r3 = r3 * 0x1c;
        r3 = tmp + r3;
        r3 = *(s16*)((u8*)r3 + 0x2);
        tmp = r3 + tmp;
        tmp = (s16)tmp;
        *(u16*)((u8*)r31 + 0x50) = tmp;
        return;
    }
    tmp = *(u8*)((u8*)r31 + 0x4);
    tmp = tmp & 0xFFFFFFFD;
    tmp = (s8)tmp;
    *(u8*)((u8*)r31 + 0x4) = tmp;
    return;
L_8001F998:
    tmp = *(u32*)lbl_80478878;
    if ((s32)tmp == 2) {
        tmp = *(u8*)((u8*)r31 + 0x4);
        r3 = (u32)lbl_803A1F88;
        r4 = (u32)lbl_802EF0A8;
        tmp = tmp | 0x2;
        r3 = (u32)lbl_803A1F88;
        r5 = (s8)tmp;
        tmp = (u32)lbl_802EF0A8;
        *(u8*)((u8*)r31 + 0x4) = r5;
        f0 = *(f32*)((u8*)r3 + 0x20);
        r3 = *(s16*)((u8*)r31 + 0x6);
        f0 = (f64)(s32)f0;
        r3 = r3 * 0x1c;
        r3 = tmp + r3;
        r3 = *(s16*)((u8*)r3 + 0x2);
        tmp = r3 + tmp;
        tmp = (s16)tmp;
        *(u16*)((u8*)r31 + 0x50) = tmp;
        return;
    }
    tmp = *(u8*)((u8*)r31 + 0x4);
    tmp = tmp & 0xFFFFFFFD;
    tmp = (s8)tmp;
    *(u8*)((u8*)r31 + 0x4) = tmp;
    return;
L_8001FA08:
    tmp = *(u32*)lbl_80478878;
    if ((s32)tmp == 2) {
        tmp = *(u8*)((u8*)r31 + 0x4);
        r3 = (u32)lbl_803A1F88;
        r4 = (u32)lbl_802EF0A8;
        tmp = tmp | 0x2;
        r3 = (u32)lbl_803A1F88;
        r5 = (s8)tmp;
        tmp = (u32)lbl_802EF0A8;
        *(u8*)((u8*)r31 + 0x4) = r5;
        f0 = *(f32*)((u8*)r3 + 0x10);
        r3 = *(s16*)((u8*)r31 + 0x6);
        f0 = (f64)(s32)f0;
        r3 = r3 * 0x1c;
        r3 = tmp + r3;
        r3 = *(s16*)((u8*)r3 + 0x2);
        tmp = r3 + tmp;
        tmp = (s16)tmp;
        *(u16*)((u8*)r31 + 0x50) = tmp;
        return;
    }
    tmp = *(u8*)((u8*)r31 + 0x4);
    tmp = tmp & 0xFFFFFFFD;
    tmp = (s8)tmp;
    *(u8*)((u8*)r31 + 0x4) = tmp;
    return;
L_8001FA78:
    tmp = *(u32*)lbl_80478878;
    if ((s32)tmp == 2) {
        tmp = *(u8*)((u8*)r31 + 0x4);
        r3 = (u32)lbl_803A1F88;
        r4 = (u32)lbl_802EF0A8;
        tmp = tmp | 0x2;
        r3 = (u32)lbl_803A1F88;
        r5 = (s8)tmp;
        tmp = (u32)lbl_802EF0A8;
        *(u8*)((u8*)r31 + 0x4) = r5;
        f0 = *(f32*)((u8*)r3 + 0x14);
        r3 = *(s16*)((u8*)r31 + 0x6);
        f0 = (f64)(s32)f0;
        r3 = r3 * 0x1c;
        r3 = tmp + r3;
        r3 = *(s16*)((u8*)r3 + 0x2);
        tmp = r3 + tmp;
        tmp = (s16)tmp;
        *(u16*)((u8*)r31 + 0x50) = tmp;
        return;
    }
    tmp = *(u8*)((u8*)r31 + 0x4);
    tmp = tmp & 0xFFFFFFFD;
    tmp = (s8)tmp;
    *(u8*)((u8*)r31 + 0x4) = tmp;
    return;
L_8001FAE8:
    tmp = *(u32*)lbl_80478878;
    if ((s32)tmp == 2) {
        tmp = *(u8*)((u8*)r31 + 0x4);
        r3 = (u32)lbl_803A1F88;
        r4 = (u32)lbl_802EF0A8;
        tmp = tmp | 0x2;
        r3 = (u32)lbl_803A1F88;
        r5 = (s8)tmp;
        tmp = (u32)lbl_802EF0A8;
        *(u8*)((u8*)r31 + 0x4) = r5;
        f0 = *(f32*)((u8*)r3 + 0x18);
        r3 = *(s16*)((u8*)r31 + 0x6);
        f0 = (f64)(s32)f0;
        r3 = r3 * 0x1c;
        r3 = tmp + r3;
        r3 = *(s16*)((u8*)r3 + 0x2);
        tmp = r3 + tmp;
        tmp = (s16)tmp;
        *(u16*)((u8*)r31 + 0x50) = tmp;
        return;
    }
    tmp = *(u8*)((u8*)r31 + 0x4);
    tmp = tmp & 0xFFFFFFFD;
    tmp = (s8)tmp;
    *(u8*)((u8*)r31 + 0x4) = tmp;
    return;
L_8001FB58:
    tmp = *(u32*)lbl_80478878;
    if ((s32)tmp == 3) {
        tmp = *(u8*)((u8*)r31 + 0x4);
        r3 = (u32)lbl_803A1F88;
        r4 = (u32)lbl_802EF0A8;
        tmp = tmp | 0x2;
        r3 = (u32)lbl_803A1F88;
        r5 = (s8)tmp;
        tmp = (u32)lbl_802EF0A8;
        *(u8*)((u8*)r31 + 0x4) = r5;
        f0 = *(f32*)((u8*)r3 + 0x10);
        r3 = *(s16*)((u8*)r31 + 0x6);
        f0 = (f64)(s32)f0;
        r3 = r3 * 0x1c;
        r3 = tmp + r3;
        r3 = *(s16*)((u8*)r3 + 0x2);
        tmp = r3 + tmp;
        tmp = (s16)tmp;
        *(u16*)((u8*)r31 + 0x50) = tmp;
        return;
    }
    tmp = *(u8*)((u8*)r31 + 0x4);
    tmp = tmp & 0xFFFFFFFD;
    tmp = (s8)tmp;
    *(u8*)((u8*)r31 + 0x4) = tmp;
    return;
L_8001FBC8:
    tmp = *(u32*)lbl_80478878;
    if ((s32)tmp == 3) {
        tmp = *(u8*)((u8*)r31 + 0x4);
        r3 = (u32)lbl_803A1F88;
        r4 = (u32)lbl_802EF0A8;
        tmp = tmp | 0x2;
        r3 = (u32)lbl_803A1F88;
        r5 = (s8)tmp;
        tmp = (u32)lbl_802EF0A8;
        *(u8*)((u8*)r31 + 0x4) = r5;
        f0 = *(f32*)((u8*)r3 + 0x14);
        r3 = *(s16*)((u8*)r31 + 0x6);
        f0 = (f64)(s32)f0;
        r3 = r3 * 0x1c;
        r3 = tmp + r3;
        r3 = *(s16*)((u8*)r3 + 0x2);
        tmp = r3 + tmp;
        tmp = (s16)tmp;
        *(u16*)((u8*)r31 + 0x50) = tmp;
        return;
    }
    tmp = *(u8*)((u8*)r31 + 0x4);
    tmp = tmp & 0xFFFFFFFD;
    tmp = (s8)tmp;
    *(u8*)((u8*)r31 + 0x4) = tmp;
    return;
L_8001FC38:
    tmp = *(u32*)lbl_80478878;
    if ((s32)tmp == 3) {
        tmp = *(u8*)((u8*)r31 + 0x4);
        r3 = (u32)lbl_803A1F88;
        r4 = (u32)lbl_802EF0A8;
        tmp = tmp | 0x2;
        r3 = (u32)lbl_803A1F88;
        r5 = (s8)tmp;
        tmp = (u32)lbl_802EF0A8;
        *(u8*)((u8*)r31 + 0x4) = r5;
        f0 = *(f32*)((u8*)r3 + 0x1C);
        r3 = *(s16*)((u8*)r31 + 0x6);
        f0 = (f64)(s32)f0;
        r3 = r3 * 0x1c;
        r3 = tmp + r3;
        r3 = *(s16*)((u8*)r3 + 0x2);
        tmp = r3 + tmp;
        tmp = (s16)tmp;
        *(u16*)((u8*)r31 + 0x50) = tmp;
        return;
    }
    tmp = *(u8*)((u8*)r31 + 0x4);
    tmp = tmp & 0xFFFFFFFD;
    tmp = (s8)tmp;
    *(u8*)((u8*)r31 + 0x4) = tmp;
    return;
L_8001FCA8:
    tmp = *(u32*)lbl_80478878;
    if ((s32)tmp == 3) {
        tmp = *(u8*)((u8*)r31 + 0x4);
        r3 = (u32)lbl_803A1F88;
        r4 = (u32)lbl_802EF0A8;
        tmp = tmp | 0x2;
        r3 = (u32)lbl_803A1F88;
        r5 = (s8)tmp;
        tmp = (u32)lbl_802EF0A8;
        *(u8*)((u8*)r31 + 0x4) = r5;
        f0 = *(f32*)((u8*)r3 + 0x20);
        r3 = *(s16*)((u8*)r31 + 0x6);
        f0 = (f64)(s32)f0;
        r3 = r3 * 0x1c;
        r3 = tmp + r3;
        r3 = *(s16*)((u8*)r3 + 0x2);
        tmp = r3 + tmp;
        tmp = (s16)tmp;
        *(u16*)((u8*)r31 + 0x50) = tmp;
        return;
    }
    tmp = *(u8*)((u8*)r31 + 0x4);
    tmp = tmp & 0xFFFFFFFD;
    tmp = (s8)tmp;
    *(u8*)((u8*)r31 + 0x4) = tmp;
    return;
L_8001FD18:
    tmp = *(u8*)lbl_8047A34C;
    if (tmp != 0) return;
    tmp = *(u8*)((u8*)r31 + 0x4);
    tmp = tmp & 0xFFFFFFFD;
    tmp = (s8)tmp;
    *(u8*)((u8*)r31 + 0x4) = tmp;

    return;
}

/* 0x8001FD48 | 0x5E0 */
void fn_8001FD48(void) {
    extern u8 lbl_8047A310[];
    extern u8 lbl_8047A314[];
    extern u8 lbl_8047A318[];
    extern u8 lbl_8047A31C[];
    extern u8 lbl_8047A320[];
    extern u8 lbl_8047A324[];
    extern u8 lbl_8047A328[];
    extern u8 lbl_8047A33C[];
    extern u8 lbl_8047A340[];
    extern u8 lbl_8047A344[];
    extern u8 lbl_8047A348[];
    extern u8 lbl_8047B810[];
    extern u8 lbl_8047B814[];
    extern u8 lbl_8047B818[];
    extern void fn_800216E0();
    extern void fn_800EC8DC();
    extern void fn_800EC918();
    extern void fn_800EC960();
    extern void fn_800EC990();
    extern void fn_800EC9DC();
    extern void fn_800ECA78();
    extern void fn_800ECB74();
    extern void fn_800ECCA8();
    extern void fn_800F0308();
    extern void fn_800F92D4();
    extern void fn_8010264C();
    extern void fn_801026A4();
    extern void fn_801046B8();
    extern void fn_80113828();
    extern void fn_80128E38();
    extern void fn_80135168();
    extern void fn_80165548();
    extern void fn_8016557C();
    extern void fn_80165A20();
    extern void fn_801662E8();
    extern void fn_80176A44();
    extern void fn_80176A94();
    extern void fn_80176E0C();
    extern void fn_8017B1AC();
    extern void fn_801C40F0();
    extern void fn_801C41C8();
    extern void fn_801D0748();
    u8 sp[0x30];
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
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f31 = 0.0f;

    f0 = *(f32*)lbl_8047B814;
    tmp = 0x0;
    r3 = 0xB540000;
    *(u32*)lbl_8047A31C = tmp;
    r3 = r3 + 0x1000;
    r31 = 0x1;
    *(f32*)lbl_8047A348 = f0;
    fn_800F92D4();
    r4 = r3;
    f0 = *(f32*)lbl_8047B814;
    tmp = 0x0;
    r3 = 0x0;
    r30 = r4;
    *(f32*)lbl_8047A340 = f0;
    r4 = 0x406;
    *(u32*)lbl_8047A324 = r30;
    *(u32*)lbl_8047A33C = tmp;
    *(u32*)lbl_8047A328 = tmp;
    fn_801662E8();
    r3 = 0x46a;
    r4 = 0x0;
    r5 = 0xff;
    fn_80165A20();
    tmp = *(u32*)lbl_8047A310;
    if ((s32)tmp == 1) {
        r4 = 0x7;
        r3 = 0x0;
        tmp = 0x1;
        *(u32*)lbl_8047A31C = r4;
        *(u32*)lbl_8047A310 = r3;
        *(u32*)lbl_8047A320 = tmp;
    } else {

        tmp = 0x0;
        *(u32*)lbl_8047A320 = tmp;
    }
    r3 = 0x15;
    r4 = 0x0;
    fn_8010264C();
    do {
        fn_8017B1AC();
        r29 = r3;

        if ((s32)r29 == 0xb || (s32)r29 == 4) {

            tmp = *(u32*)lbl_8047A318;
            if ((s32)tmp == 0) {
                fn_800EC918();
                fn_80176A94();
                fn_8016557C();
                tmp = 0x1;
                *(u32*)lbl_8047A318 = tmp;
            }
            fn_800F0308();
            goto L_800202FC;
        }
        tmp = *(u32*)lbl_8047A318;
        if ((s32)tmp == 1) {
            fn_80176A44();
            r3 = 0x0;
            r4 = 0x406;
            fn_801662E8();
            fn_80165548();
            fn_800EC8DC();
            tmp = 0x0;
            *(u32*)lbl_8047A318 = tmp;
        }
        if ((s32)r29 == 5) {
            tmp = *(u32*)lbl_8047A314;
            if ((s32)tmp == 0) {
                fn_800EC918();
                fn_80176A94();
                fn_8016557C();
                tmp = 0x1;
                *(u32*)lbl_8047A314 = tmp;
            }
            fn_800F0308();
            goto L_800202FC;
        }
        tmp = *(u32*)lbl_8047A314;
        if ((s32)tmp == 1) {
            fn_80176A44();
            r3 = 0x0;
            r4 = 0x406;
            fn_801662E8();
            fn_80165548();
            fn_800EC8DC();
            tmp = 0x0;
            *(u32*)lbl_8047A314 = tmp;
        }
        tmp = *(u32*)lbl_8047A31C;
        if ((s32)tmp != 0x1e) {
            if ((s32)tmp < 0x1e) {
                if ((s32)tmp != 4) {
                    if ((s32)tmp < 4) {
                        if ((s32)tmp != 1) {
                            if ((s32)tmp < 1) {
                                if ((s32)tmp < 0) {
                                    goto L_800202FC;
                                }
                                if ((s32)tmp < 3) {
                                    goto L_80020018;
                                }
                                if ((s32)tmp == 7) goto L_8002010C;
                                if ((s32)tmp < 7) {
                                    if ((s32)tmp >= 6) goto L_800200D8;
                                    goto L_800200B0;
                                }
                                if ((s32)tmp >= 9) goto L_800202FC;
                                goto L_80020184;
                            }
                            if ((s32)tmp == 0xc8) goto L_80020274;
                            if ((s32)tmp < 0xc8) {
                                if ((s32)tmp == 0x28) goto L_800201E8;
                                if ((s32)tmp < 0x28) {
                                    if ((s32)tmp == 0x20) goto L_80020200;
                                    if ((s32)tmp >= 0x20) goto L_800202FC;
                                    goto L_80020240;
                                }
                                if ((s32)tmp >= 0x2a) goto L_800202FC;
                                goto L_800202E0;
                            }
                            if ((s32)tmp == 0x3e8) goto L_800202F8;
                            if ((s32)tmp >= 0x3e8) goto L_800202FC;
                            goto L_800202FC;
                                    }
                        if (r30 != 0) {
                            r3 = r30;
                            r4 = 0x0;
                            fn_800ECCA8();
                            f1 = *(f32*)lbl_8047B814;
                            r3 = r30;
                            fn_800ECA78();
                            f1 = *(f32*)lbl_8047B810;
                            r3 = r30;
                            fn_800EC9DC();
                            r3 = r30;
                            fn_800EC990();
                        }
                        r3 = r30;
                        r4 = 0x0;
                        fn_800ECB74();
                        r4 = 0xB550000;
                        r3 = 0x12;
                        r4 = r4 + 0x1800;
                        r5 = 0x0;
                        r6 = 0x0;
                        fn_80176E0C();
                        f1 = *(f32*)lbl_8047B810;
                        r3 = 0x4;
                        fn_801C41C8();
                        tmp = *(u32*)lbl_8047A31C;
                        if ((s32)tmp != 0) goto L_800202FC;
                        tmp = 0x1;
                        *(u32*)lbl_8047A31C = tmp;
                        goto L_800202FC;
                            }
                    r3 = r30;
                    fn_800EC960();
                    tmp = r3 & 0xFF;
                    if (tmp == 0) {
                        tmp = 0x2;
                        *(u32*)lbl_8047A31C = tmp;
                        goto L_800202FC;
                    }
                    fn_800F0308();
                    goto L_800202FC;
                L_80020018:
                    if (r30 != 0) {
                        r3 = r30;
                        r4 = 0x1;
                        fn_800ECCA8();
                        f1 = *(f32*)lbl_8047B814;
                        r3 = r30;
                        fn_800ECA78();
                        f1 = *(f32*)lbl_8047B810;
                        r3 = r30;
                        fn_800EC9DC();
                        r3 = r30;
                        fn_800EC990();
                    }
                    r3 = r30;
                    r4 = 0x0;
                    fn_800ECB74();
                    tmp = 0x3;
                    *(u32*)lbl_8047A31C = tmp;
                    goto L_800202FC;
                                }
                if (r30 != 0) goto L_80020078;
                goto L_8002008C;
                goto L_80020078;
            do {
                    fn_800F0308();
                L_80020078:
                    r3 = r30;
                    fn_800EC960();
                    tmp = r3 & 0xFF;
            } while (tmp != 0);
            L_8002008C:
                tmp = 0x8;
                *(u32*)lbl_8047A31C = tmp;
                goto L_800202FC;
                }
            f1 = *(f32*)lbl_8047B810;
            r3 = 0x5;
            fn_801C41C8();
            tmp = 0x6;
            *(u32*)lbl_8047A31C = tmp;
            goto L_800202FC;
        L_800200B0:
            r3 = 0x0;
            fn_801C40F0();
            tmp = (s8)r3;
            if ((s32)tmp == 0) {
                tmp = 0x6;
                *(u32*)lbl_8047A31C = tmp;
                goto L_800202FC;
            }
            fn_800F0308();
            goto L_800202FC;
        L_800200D8:
            f31 = *(f32*)lbl_8047B810;
            while (1) {
                f0 = *(f32*)lbl_8047A348;
                if (f0 >= f31) break;
                f1 = *(f32*)lbl_8047A348;
                f0 = *(f32*)lbl_8047A344;
                f0 = f1 + f0;
                *(f32*)lbl_8047A348 = f0;
                fn_800F0308();

            }
            tmp = 0x7;
            *(u32*)lbl_8047A31C = tmp;
            goto L_800202FC;
        L_8002010C:
            r4 = 0xB560000;
            r3 = 0x12;
            r4 = r4 + 0x1800;
            r5 = 0x0;
            r6 = 0x0;
            fn_80176E0C();
            if (r30 != 0) {
                r3 = r30;
                r4 = 0x2;
                fn_800ECCA8();
                f1 = *(f32*)lbl_8047B814;
                r3 = r30;
                fn_800ECA78();
                f1 = *(f32*)lbl_8047B810;
                r3 = r30;
                fn_800EC9DC();
                r3 = r30;
                fn_800EC990();
            }
            r3 = r30;
            r4 = 0x1;
            fn_800ECB74();
            f1 = *(f32*)lbl_8047B810;
            r3 = 0x4;
            fn_801C41C8();
            r3 = 0x1;
            fn_801C40F0();
            tmp = 0x1e;
            *(u32*)lbl_8047A31C = tmp;
            goto L_800202FC;
        L_80020184:
            r4 = 0xB560000;
            r3 = 0x12;
            r4 = r4 + 0x1800;
            r5 = 0x0;
            r6 = 0x0;
            fn_80176E0C();
            if (r30 != 0) {
                r3 = r30;
                r4 = 0x2;
                fn_800ECCA8();
                f1 = *(f32*)lbl_8047B814;
                r3 = r30;
                fn_800ECA78();
                f1 = *(f32*)lbl_8047B810;
                r3 = r30;
                fn_800EC9DC();
                r3 = r30;
                fn_800EC990();
            }
            r3 = r30;
            r4 = 0x1;
            fn_800ECB74();
            tmp = 0x1e;
            *(u32*)lbl_8047A31C = tmp;
            goto L_800202FC;
        L_800201E8:
            tmp = 0x3e8;
            *(u32*)lbl_8047A31C = tmp;
            goto L_800202FC;
        }
        tmp = 0x20;
        *(u32*)lbl_8047A31C = tmp;
        goto L_800202FC;
    L_80020200:
        fn_801046B8();
        r4 = r3;
        r3 = 0x13;
        r5 = 0x0;
        r6 = 0x0;
        r7 = 0x1;
        r8 = 0x0;
        fn_801026A4();
        if ((s32)r3 < 0) goto L_800202FC;
        f0 = *(f32*)lbl_8047B814;
        tmp = 0x1f;
        *(u32*)lbl_8047A31C = tmp;
        *(f32*)lbl_8047A348 = f0;
        goto L_800202FC;
    L_80020240:
        f31 = *(f32*)lbl_8047B818;
        while (1) {
            f0 = *(f32*)lbl_8047A348;
            if (f0 >= f31) break;
            f1 = *(f32*)lbl_8047A348;
            f0 = *(f32*)lbl_8047A344;
            f0 = f1 + f0;
            *(f32*)lbl_8047A348 = f0;
            fn_800F0308();

        }
        tmp = 0xc8;
        *(u32*)lbl_8047A31C = tmp;
        goto L_800202FC;
    L_80020274:
        r3 = 0x1;
        r4 = 0x2;
        r5 = 0x0;
        fn_801D0748();
        if ((s32)r3 == (s32)-0x1) {
            tmp = 0x20;
            *(u32*)lbl_8047A31C = tmp;
            goto L_800202FC;
        }
        if ((s32)r3 != 3) {
            r3 = 0x0;
            r4 = 0x0;
            fn_80128E38();
        }
        r3 = 0x0;
        r4 = 0x4;
        fn_80135168();
        if (r3 != 0) {
            r3 = 0x1;
            fn_800216E0();
        } else {

            r3 = 0x0;
            fn_800216E0();
        }
        tmp = 0x29;
        *(u32*)lbl_8047A31C = tmp;
        goto L_800202FC;
    L_800202E0:
        r3 = 0x3a1;
        r4 = 0x0;
        fn_80113828();
        tmp = 0x3e8;
        *(u32*)lbl_8047A31C = tmp;
        goto L_800202FC;
    L_800202F8:
        r31 = 0x0;
    L_800202FC:
        ;
    } while ((s32)r31 != 0);
    return;
}
