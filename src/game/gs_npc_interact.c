/**
 * @file gs_npc_interact.c
 * @brief GSnpcInteract -- Overworld NPC interaction and dialog system.
 *
 * Address range: 0x8000D298 - 0x80012858 (~70 functions)
 *
 * This module handles player-NPC interactions in the overworld, including:
 *   - Dialog initiation and text display
 *   - Item give/receive events
 *   - Trainer battle triggers
 *   - NPC sprite/model animation during dialog
 *   - Shop/mart interactions
 *   - Quest/story progression triggers
 *
 * Key functions:
 *   fn_8000D298  GSnpc_InitDialog            -- 0x114 bytes, start NPC dialog
 *   fn_8000D3AC  GSnpc_DialogStateMachine    -- 0x364 bytes, dialog state handler
 *   fn_8000D710  GSnpc_EventDispatch         -- 0x398 bytes, NPC event dispatcher
 *   fn_8000DAA8  GSnpc_Nop                   -- 8-byte stub
 *   fn_8000DAB0  GSnpc_SetAnimation          -- set NPC animation state
 *   fn_8000DAE8  GSnpc_ProcessItemEvent      -- 0x1A0 bytes, item give/take
 *   fn_8000DC88  GSnpc_ValidateItem          -- check item validity
 *   fn_8000DD0C  GSnpc_GetItemSlot           -- get bag slot for item
 *   fn_8000DD30  GSnpc_AddItem               -- add item to player bag
 *   fn_8000DD5C  GSnpc_RemoveItem            -- remove item from bag
 *   fn_8000DD98  GSnpc_GetMoney              -- get player money count
 *   fn_8000DDBC  GSnpc_AddMoney              -- add money to player
 *   fn_8000DDE8  GSnpc_RemoveMoney           -- subtract money from player
 *   fn_8000DE24  GSnpc_CheckFlag             -- check game flag for NPC
 *   fn_8000DEC4  GSnpc_SetFlag               -- set game flag from NPC event
 *   fn_8000DFF0  GSnpc_MartBuy               -- 0x214 bytes, Poke Mart buy logic
 *   fn_8000E204  GSnpc_MartSell              -- 0x88 bytes, Poke Mart sell logic
 *   fn_8000E28C  GSnpc_NopStub               -- 4 bytes, no-op
 *   fn_8000E290  GSnpc_TrainerBattle         -- 0x780 bytes, initiate trainer battle
 *   fn_8000EA10  GSnpc_PostBattleReward      -- 0x324 bytes, prize money & items
 *   fn_8000ED34  GSnpc_QuestUpdate           -- 0x5DC bytes, quest progression
 *   fn_8000F310  GSnpc_GetQuestState         -- 0x4C bytes
 *   fn_8000F35C  GSnpc_SetQuestState         -- 0xA4 bytes
 *   fn_8000F400  GSnpc_HealParty             -- 0x368 bytes, Pokemon Center heal
 *   fn_8000F768  GSnpc_FadeTransition        -- 0x1FC bytes, screen fade during dialog
 *   fn_8000F964  GSnpc_PokemonTrade          -- 0x474 bytes, in-game trade event
 *   fn_8000FDD8  GSnpc_MoveTutor             -- 0x60 bytes, move tutor intro
 *   fn_8000FE38  GSnpc_MoveTutorTeach        -- 0x118 bytes, teach move
 *   fn_8000FF50  GSnpc_NameRater             -- 0x58 bytes, name rater check
 *   fn_8000FFA8  GSnpc_NameRaterRename       -- 0x118 bytes, rename Pokemon
 *   fn_800100C0  GSnpc_DaycareDeposit        -- 0x68 bytes, daycare deposit
 *   fn_80010128  GSnpc_DaycareWithdraw       -- 0x16C bytes, daycare withdraw
 *   fn_80010294  GSnpc_PurificationChamber   -- 0x1E8 bytes, purification setup
 *   fn_8001047C  GSnpc_ShadowGaugeCheck      -- 0x10C bytes, check purification ready
 *   fn_80010588  GSnpc_PurifyPokemon         -- 0x11C bytes, purify Shadow Pokemon
 *   fn_800106A4  GSnpc_SnagMachineSetup      -- 0x1A0 bytes, snag machine interaction
 *   fn_80010844  GSnpc_GBALinkPrompt         -- 0x15C bytes, GBA link cable prompt
 *   fn_800109A0  GSnpc_ColosseumSignup       -- 0x190 bytes, colosseum registration
 *   fn_80010B30  GSnpc_MtBattleEntry         -- 0x168 bytes, Mt. Battle entry
 *   fn_80010C98  GSnpc_WarpToLocation        -- 0x52C bytes, warp/teleport handler
 *
 * The dialog state machine (fn_8000D3AC) has 5 states:
 *   State 0: Init -- load NPC dialog data, set up text box
 *   State 1: Advance -- wait for player input to advance text
 *   State 2: Choice -- display yes/no or multi-choice prompt
 *   State 3: Close -- animate text box closing
 *   State 4: Cleanup -- restore camera, free resources
 *
 * fn_8000D298 (GSnpc_InitDialog) sets up the NPC sprite for dialog:
 *   - Calls fn_80109220 to set NPC facing direction
 *   - Gets the NPC data from fn_8005D934 (lookup by ID)
 *   - Sets up the text viewport via fn_801040F0
 *   - If the NPC has a special marker (offset +0x4C), renders it
 *     using fn_800FA444/fn_800FB680 for the dialog portrait
 *
 * fn_8000E290 (GSnpc_TrainerBattle) is a key bridge function:
 *   - Gets the overworld data for the NPC
 *   - Looks up the trainer ID
 *   - Dispatches a battle event via fn_8012640C
 *   - On event completion, validates the battle result
 *   - Uses a jump table (jumptable_802E4BB8) to handle different
 *     battle outcome types (win, lose, flee, draw)
 *
 * fn_80010C98 (GSnpc_WarpToLocation) handles map transitions:
 *   - Checks warp destination validity via fn_801F2020
 *   - Gets the destination map name via fn_802037DC
 *   - Loads string message 0x76FB for the warp confirmation dialog
 *   - Waits for player input with a render loop
 *   - On confirmation, triggers the floor transition
 *
 * Rodata references:
 *   jumptable_802E4BB8: Battle outcome jump table (26 entries)
 *   Various Shift-JIS string constants for dialog templates
 *
 * SDA globals:
 *   Many NPC-specific state variables in 0x8047A280-0x8047A2A0 range
 */

#include "dolphin/types.h"

/* =========================================================================
 * External declarations
 * ========================================================================= */

/* NPC/People system */
extern void  fn_80109220(void* npc, s32 direction);  /* Set NPC facing */
extern void* fn_8005D934(s16 npcId);                  /* Lookup NPC data by ID */

/* Text/dialog system */
extern void  fn_801040F0(s32 p1, s32 p2, void* textCtx, u16 msgId, s32 p5);
extern void  fn_801080CC(void* ctx, s32 state);       /* Set dialog state */
extern void  fn_801081F8(void* ctx, s32 msgId, s32 flags); /* Display message */

/* Rendering */
extern u32   fn_800FA444(void* model);                /* Get model dimensions */
extern void  fn_800FB680(s32 x, s32 y, s32 z, u32 flags, u16 modelId);

/* Map/warp */
extern u8    fn_801F2020(s32 p1, void* warpId, void* outDest);
extern void* fn_802037DC(void* mapData);              /* Get map name string */
extern void  fn_80106D3C(s32 slot, s32 msgId, s32 p3, s32 p4);
extern void  fn_801069FC(s32 slot);                   /* Close message box */

/* Input/frame */
extern u8    fn_801F18DC(s32 controller);             /* Check input ready */
extern u8    fn_801F1700(s32 controller);             /* Check button pressed */
extern u8    fn_80265924(void);                       /* Check A button */
extern u16   fn_801EF634(void);                       /* Get input state */
extern void  fn_800F0308(void);                       /* Frame advance */
extern u32   fn_800F7AF0(s32 slot);                   /* Get render flags */
extern u32   fn_800F7BC4(s32 slot);                   /* Get VSync flags */

/* Battle bridge */
extern void  fn_80132A38(s32 paramId, s32 value);     /* Set battle parameter */
extern void  fn_801F4C14(s32 p1, s32 p2, s32 p3, s32 p4, u16 p5); /* Configure map object */
extern void  fn_801040D0(void* ctx, s32 index);       /* Get participant data */

/* =========================================================================
 * SDA globals
 * ========================================================================= */

/* NPC interaction state variables are scattered across the SDA region.
 * The exact mapping is determined by the lbz/sth instructions that
 * reference r0+offset or r13+offset addressing modes. */

/* =========================================================================
 * Function: GSnpc_InitDialog
 * Address:  0x8000D298
 * Size:     0x114
 *
 * Initializes dialog with an NPC. Takes two parameters:
 *   r3: Dialog context pointer (contains camera/viewport state)
 *   r4: NPC instance pointer (contains NPC ID at +0x06, model at +0x4C)
 *
 * First sets the NPC facing direction toward the player, then looks up
 * the NPC data table entry. If the NPC has a portrait model (offset +0x4C),
 * renders it at a calculated screen position based on the dialog box location.
 * ========================================================================= */

/* =========================================================================
 * Function: GSnpc_DialogStateMachine
 * Address:  0x8000D3AC
 * Size:     0x364
 *
 * The main dialog state machine with 5 states (0-4).
 * State 0 initializes the text box; states 1-2 handle input; state 3-4 close.
 *
 * At state 0, checks byte at offset +0x01 of the context struct. If 0x41,
 * this is a standard dialog; if 0x109, this is a special event dialog.
 * Each dialog type uses different message IDs (0x65, 0xDB0-0xDB2).
 * ========================================================================= */

/* =========================================================================
 * Function: GSnpc_TrainerBattle
 * Address:  0x8000E290
 * Size:     0x780
 *
 * Initiates a trainer battle from an NPC interaction. This is the bridge
 * between the NPC system and the battle system.
 *
 * Uses jumptable_802E4BB8 (26 entries) for dispatching on the NPC's
 * event type ID (0x1235-0x124E). Different event types correspond to:
 *   0x1235: Standard trainer
 *   0x1236: Shadow Pokemon encounter
 *   0x1237: Double battle trainer
 *   0x1238: Colosseum opponent
 *   0x123A: Boss/admin battle
 *
 * For each type, configures the battle participants, field, and rules,
 * then hands off to the battle system via fn_8012640C.
 * ========================================================================= */

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 1 functions matched
 * =================================================================== */

extern u8 lbl_8047A2A0;

/* Address: 0x8000DAA8 | Size: 0x8 | Pattern: sda_getter */
u8 fn_8000DAA8(void) {
    return lbl_8047A2A0;
}

/* =========================================================================
 * Stubs for remaining GSnpcInteract functions (0x800111C4-0x80011EA4)
 * ========================================================================= */

/* 0x800111C4 | 0x24 -- small accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800111C4(void) {
    /* TODO: match -- 0x24 bytes at 0x800111C4 */
}
#pragma pop

/* 0x800111E8 | 0x24 -- small accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800111E8(void) {
    /* TODO: match -- 0x24 bytes at 0x800111E8 */
}
#pragma pop

/* 0x7C | fn_8001120C | nullcheck_call_flag */
u32 fn_8001120C(void* obj) {
    if (fn_80102620() == 0) { return 255; }
    fn_80102620(obj);
    return 0;
}

/* 0x80011288 | 0x21C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80011288(void) {
    /* TODO: match -- 0x21C bytes at 0x80011288 */
}
#pragma pop

/* 0x800114A4 | 0x25C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800114A4(void) {
    /* TODO: match -- 0x25C bytes at 0x800114A4 */
}
#pragma pop

/* 0x80011700 | 0xBC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80011700(void) {
    /* TODO: match -- 0xBC bytes at 0x80011700 */
}
#pragma pop

/* 0x800117BC | 0x1EC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800117BC(void) {
    /* TODO: match -- 0x1EC bytes at 0x800117BC */
}
#pragma pop

/* 0x74 | fn_800119A8 | nullcheck_call_flag */
u32 fn_800119A8(void* obj) {
    if (fn_80102620() == 0) { return 75; }
    fn_80102568(obj);
    return 0;
}

/* 0x80011A1C | 0x130 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80011A1C(void) {
    /* TODO: match -- 0x130 bytes at 0x80011A1C */
}
#pragma pop

/* 0x78 | fn_80011B4C | generic */
u32 fn_80011B4C(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    fn_80104704();
    fn_801040A0();
    fn_800F0308();
    return 1;
}

/* 0x80011BC4 | 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80011BC4(void) {
    /* TODO: match -- 0xB4 bytes at 0x80011BC4 */
}
#pragma pop

/* 0x78 | fn_80011C78 | generic */
u32 fn_80011C78(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    fn_80104704();
    fn_801040A0();
    fn_800F0308();
    return 1;
}

/* 0x80011CF0 | 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80011CF0(void) {
    /* TODO: match -- 0xAC bytes at 0x80011CF0 */
}
#pragma pop

/* 0x80011D9C | 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80011D9C(void) {
    /* TODO: match -- 0xCC bytes at 0x80011D9C */
}
#pragma pop

/* 0x80011E68 | 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80011E68(void) {
    /* TODO: match -- 0x3C bytes at 0x80011E68 */
}
#pragma pop

/* 0x80011EA4 | 0x9B4 -- GSnpc_WarpToLocation continued */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80011EA4(void) {
    /* TODO: match -- 0x9B4 bytes at 0x80011EA4 */
}
#pragma pop
