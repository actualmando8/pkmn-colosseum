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
