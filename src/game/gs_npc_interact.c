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
    extern void fn_80089F78();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r6 = 0;

    r6 = 0x0;
    fn_80089F78();
    return;
}
#pragma pop

/* 0x800111E8 | 0x24 -- small accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800111E8(void) {
    extern void fn_80089F78();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r6 = 0;

    r6 = 0x1;
    fn_80089F78();
    return;
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
    extern void fn_8005D8F8();
    extern void fn_80102138();
    extern void fn_801026A4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r28 = r3;
    r29 = r5;
    r0 = *(u8*)((u8*)r28 + 0x21);
    if ((u32)r0 != (u32)0x0) goto L_80011460;
    r3 = 0x1258;
    r4 = 0x0;
    fn_8005D8F8();
    r3 = 0x1259;
    r4 = 0x0;
    fn_8005D8F8();
    r3 = 0x125a;
    r4 = 0x0;
    fn_8005D8F8();
    r3 = 0x125b;
    r4 = 0x0;
    fn_8005D8F8();
    r31 = r28;
    r30 = 0x0;
L_800112F0: ;
    r0 = *(u32*)((u8*)r31 + 0x4);
    if ((s32)r0 == (s32)0x47) goto L_8001132C;
    if ((s32)r0 >= (s32)0x47) goto L_80011310;
    if ((s32)r0 == (s32)0x45) goto L_8001131C;
    if ((s32)r0 >= (s32)0x45) goto L_80011324;
    goto L_8001133C;
L_80011310: ;
    if ((s32)r0 >= (s32)0x49) goto L_8001133C;
    goto L_80011334;
L_8001131C: ;
    r3 = 0x125b;
    goto L_80011340;
L_80011324: ;
    r3 = 0x125a;
    goto L_80011340;
L_8001132C: ;
    r3 = 0x1258;
    goto L_80011340;
L_80011334: ;
    r3 = 0x1259;
    goto L_80011340;
L_8001133C: ;
    r3 = 0x0;
L_80011340: ;
    if ((s32)r3 == (s32)0x0) goto L_80011350;
    r4 = 0x1;
    fn_8005D8F8();
L_80011350: ;
    r31 = r31 + 0x8;
    r30 = r30 + 0x1;
    if ((s32)r30 < (s32)0x4) goto L_800112F0;
    r3 = 0xff;
    r4 = 0x125a;
    fn_80102138();
    if ((s32)r3 != (s32)-0x1) goto L_80011398;
    r3 = 0xff;
    r4 = 0x125b;
    fn_80102138();
    if ((s32)r3 != (s32)-0x1) goto L_80011398;
    r0 = 0x0;
    *(u32*)(sp + 0x8) = r0;
L_80011398: ;
    r7 = r29;
    r9 = r28;
    r5 = r1 + 0x8;
    r3 = 0xff;
    r4 = 0x0;
    r6 = 0x0;
    r8 = 0x1;
    /* crclr cr1eq */;
    fn_801026A4();
    if ((s32)r3 == (s32)0x125a) goto L_800113F4;
    if ((s32)r3 >= (s32)0x125a) goto L_800113D8;
    if ((s32)r3 == (s32)0x1258) goto L_800113E4;
    if ((s32)r3 >= (s32)0x1258) goto L_800113EC;
    goto L_80011404;
L_800113D8: ;
    if ((s32)r3 >= (s32)0x125c) goto L_80011404;
    goto L_800113FC;
L_800113E4: ;
    r4 = 0x47;
    goto L_80011408;
L_800113EC: ;
    r4 = 0x48;
    goto L_80011408;
L_800113F4: ;
    r4 = 0x46;
    goto L_80011408;
L_800113FC: ;
    r4 = 0x45;
    goto L_80011408;
L_80011404: ;
    r4 = -0x1;
L_80011408: ;
    r3 = -0x1;
    r0 = *(u32*)((u8*)r28 + 0x4);
    if ((s32)r4 != (s32)r0) goto L_80011420;
    r3 = 0x0;
    goto L_80011484;
L_80011420: ;
    r0 = *(u32*)((u8*)r28 + 0xC);
    if ((s32)r4 != (s32)r0) goto L_80011434;
    r3 = 0x1;
    goto L_80011484;
L_80011434: ;
    r0 = *(u32*)((u8*)r28 + 0x14);
    if ((s32)r4 != (s32)r0) goto L_80011448;
    r3 = 0x2;
    goto L_80011484;
L_80011448: ;
    r0 = *(u32*)((u8*)r28 + 0x1C);
    if ((s32)r4 != (s32)r0) goto L_80011484;
    r3 = 0x3;
    goto L_80011484;
    goto L_80011484;
L_80011460: ;
    r5 = r4;
    r7 = r29;
    r9 = r28;
    r3 = 0x104;
    r4 = 0x0;
    r6 = 0x0;
    r8 = 0x1;
    /* crclr cr1eq */;
    fn_801026A4();
L_80011484: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x800114A4 | 0x25C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800114A4(void) {
    extern void fn_80010C98();
    extern void fn_80011288();
    extern void fn_8001BD80();
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_801026A4();
    extern void fn_801F02AC();
    extern void fn_802062FC();
    extern void fn_8020E1A4();
    extern void fn_8020E204();
    extern void fn_802656AC();
    u8 sp[0x60];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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
    f32 f8 = 0.0f;

    /* stmw r23, 0x3c(r1) */;
    r24 = r3;
    r25 = r4;
    r26 = r5;
    r27 = r6;
    r28 = r7;
    r3 = r26;
    fn_8020E204();
    fn_8020E1A4();
    r0 = r28 & 0xFF;
    r31 = r3;
    if ((u32)r0 != (u32)0x0) goto L_800114FC;
    r4 = r27;
    r5 = r24;
    r6 = r25;
    fn_8001BD80();
    r29 = r3;
    goto L_800116E8;
L_800114FC: ;
    r30 = r27 & 0xFF;
L_80011500: ;
    r3 = 0xf8;
    r4 = 0x1e;
    ((void(*)(void))fn_801080CC)();
    r9 = r24;
    r10 = r25;
    r3 = 0xf8;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    r8 = 0x3;
    /* crclr cr1eq */;
    fn_801026A4();
    r0 = r3;
    r3 = 0xf8;
    r29 = r0;
    r4 = 0x20;
    ((void(*)(void))fn_801080CC)();
    if ((s32)r29 != (s32)-0x1) goto L_8001156C;
    r3 = 0xf8;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
    r3 = -0x1;
    goto L_800116EC;
L_8001156C: ;
    r3 = r25;
    r4 = r24;
    r5 = r29;
    fn_80010C98();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80011500;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_800116D8;
    r0 = r31 & 0xFF;
    if ((u32)r0 < (u32)0x2) goto L_800116D8;
    r4 = r25;
    r5 = r26;
    r3 = 0xf;
    fn_801F02AC();
    r23 = r3;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_800115D8;
    r3 = r23;
    r4 = r26;
    r5 = 0x0;
    fn_802656AC();
    goto L_800115DC;
L_800115D8: ;
    r3 = 0x0;
L_800115DC: ;
    r4 = r25;
    r5 = r26;
    r3 = 0x10;
    fn_801F02AC();
    r23 = r3;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80011618;
    r3 = r23;
    r4 = r26;
    r5 = 0x0;
    fn_802656AC();
    goto L_8001161C;
L_80011618: ;
    r3 = 0x0;
L_8001161C: ;
    r4 = r25;
    r5 = r26;
    r3 = 0xe;
    fn_801F02AC();
    r23 = r3;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80011658;
    r3 = r23;
    r4 = r26;
    r5 = 0x0;
    fn_802656AC();
    goto L_8001165C;
L_80011658: ;
    r3 = 0x0;
L_8001165C: ;
    r0 = 0x0;
    r3 = r1 + 0x10;
    r4 = 0x0;
    *(u32*)(sp + 0x2C) = r0;
    r5 = 0x1;
    *(u8*)(sp + 0x31) = r28;
    fn_80011288();
    r23 = r3;
    r3 = 0xff;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_800116A4;
    r3 = 0xff;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
L_800116A4: ;
    r3 = 0x104;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_800116C8;
    r3 = 0x104;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
L_800116C8: ;
    r3 = 0x100;
    fn_80102620();
    if ((s32)r23 == (s32)-0x1) goto L_80011500;
L_800116D8: ;
    r3 = 0xf8;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
L_800116E8: ;
    r3 = r29;
L_800116EC: ;
    /* lmw r23, 0x3c(r1) */;
    return;
}
#pragma pop

/* 0x80011700 | 0xBC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80011700(void) {
    extern void fn_80102568();
    extern void fn_80102620();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;
    f32 f7 = 0.0f;
    f32 f9 = 0.0f;

    r31 = r3;
    r3 = 0x4c;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80011738;
    r5 = r31;
    r3 = 0x4c;
    r4 = 0x0;
    fn_80102568();
L_80011738: ;
    r3 = 0xf9;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8001175C;
    r5 = r31;
    r3 = 0xf9;
    r4 = 0x0;
    fn_80102568();
L_8001175C: ;
    r3 = 0xfa;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80011780;
    r5 = r31;
    r3 = 0xfa;
    r4 = 0x0;
    fn_80102568();
L_80011780: ;
    r3 = 0xf7;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_800117A4;
    r5 = r31;
    r3 = 0xf7;
    r4 = 0x0;
    fn_80102568();
L_800117A4: ;
    r3 = 0x0;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x800117BC | 0x1EC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800117BC(void) {
    extern void fn_80010128();
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_801026A4();
    extern void fn_80106080();
    extern void fn_80106394();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f7 = 0.0f;
    f32 f9 = 0.0f;

    /* stmw r26, 0x8(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r0 = *(u8*)((u8*)r28 + 0x44);
    if ((u32)r0 != (u32)0x0) goto L_800117EC;
    r31 = 0x4c;
    goto L_800117F0;
L_800117EC: ;
    r31 = 0xf7;
L_800117F0: ;
    r3 = r31;
    r4 = 0x1e;
    ((void(*)(void))fn_801080CC)();
    r3 = r31;
    r5 = r29;
    r7 = r30;
    r9 = r28;
    r4 = 0x0;
    r6 = 0x0;
    r8 = 0x1;
    /* crclr cr1eq */;
    fn_801026A4();
    r27 = r3;
    if ((s32)r27 < (s32)0x0) goto L_80011984;
    r3 = r28;
    r4 = r27 & 0xFFFF;
    fn_80010128();
    r26 = r3;
    if ((u32)r26 == (u32)0x0) goto L_80011984;
    r3 = 0x4c;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80011868;
    r3 = 0x4c;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
L_80011868: ;
    r3 = 0xf9;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8001188C;
    r3 = 0xf9;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
L_8001188C: ;
    r3 = 0xfa;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_800118B0;
    r3 = 0xfa;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
L_800118B0: ;
    r3 = 0xf7;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_800118D4;
    r3 = 0xf7;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
L_800118D4: ;
    r3 = r26;
    r4 = 0x1;
    r5 = 0x1;
    fn_80106394();
    goto L_80011954;
L_800118E8: ;
    r3 = 0x0;
    ((void(*)(void))fn_801F18DC)();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80011940;
    r3 = 0x0;
    ((void(*)(void))fn_801F1700)();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80011928;
    ((void(*)(void))fn_80265924)();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80011928;
    r0 = 0x1;
    goto L_80011944;
L_80011928: ;
    ((void(*)(void))fn_801EF634)();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_80011940;
    r0 = 0x1;
    goto L_80011944;
L_80011940: ;
    r0 = 0x0;
L_80011944: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_80011978;
    ((void(*)(void))fn_800F0308)();
L_80011954: ;
    r3 = 0x1;
    ((void(*)(void))fn_800F7AF0)();
    r27 = r3;
    r3 = 0x1;
    ((void(*)(void))fn_800F7BC4)();
    r0 = r3 & r27;
    r0 = r0 & 0x00000300;
    if ((u32)r0 == (u32)0x0) goto L_800118E8;
L_80011978: ;
    r3 = 0x1;
    fn_80106080();
    goto L_800117F0;
L_80011984: ;
    r3 = r31;
    r4 = 0x20;
    ((void(*)(void))fn_801080CC)();
    r3 = r27;
    /* lmw r26, 0x8(r1) */;
    return;
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
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_801026A4();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f6 = 0.0f;

    /* stmw r26, 0x18(r1) */;
    r26 = r3;
    r27 = r4;
    r28 = r5;
    r29 = *(u8*)((u8*)r26 + 0x16);
L_80011A3C: ;
    r0 = *(u8*)((u8*)r26 + 0x17);
    if ((u32)r0 == (u32)0x0) goto L_80011A70;
    r5 = r27;
    r7 = r28;
    r9 = r26;
    r3 = 0xf6;
    r4 = 0x0;
    r6 = 0x0;
    r8 = 0x1;
    /* crclr cr1eq */;
    fn_801026A4();
    goto L_80011A94;
L_80011A70: ;
    r5 = r27;
    r7 = r28;
    r9 = r26;
    r3 = 0x4b;
    r4 = 0x0;
    r6 = 0x0;
    r8 = 0x1;
    /* crclr cr1eq */;
    fn_801026A4();
L_80011A94: ;
    r30 = r3;
    if ((u32)r29 != (u32)0x0) goto L_80011B34;
    if ((s32)r3 != (s32)0x3) goto L_80011B34;
    r0 = 0x0;
    r3 = 0x4b;
    *(u32*)(sp + 0x8) = r0;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80011AD4;
    r3 = 0x4b;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
L_80011AD4: ;
    r3 = 0xf6;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80011AF8;
    r3 = 0xf6;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
L_80011AF8: ;
    r5 = r1 + 0x8;
    r3 = 0xfb;
    r4 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    r8 = 0x0;
    /* crclr cr1eq */;
    fn_801026A4();
    r31 = r3;
    r3 = 0xfb;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
    if ((s32)r31 != (s32)0x1207) goto L_80011A3C;
L_80011B34: ;
    r3 = r30;
    /* lmw r26, 0x18(r1) */;
    return;
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
    extern void fn_80103FE4();
    extern void fn_801040A0();
    extern void fn_80104704();
    extern void fn_80166A28();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r4;
    fn_80104704();
    r30 = r3;
    if ((u32)r30 == (u32)0x0) goto L_80011C5C;
    fn_80103FE4();
    r31 = r3;
    r3 = r30;
    fn_801040A0();
    r0 = *(u32*)((u8*)r31 + 0x20);
    if ((u32)r29 <= (u32)r0) goto L_80011C14;
    r0 = r29 - r0;
    goto L_80011C18;
L_80011C14: ;
    r0 = r0 - r29;
L_80011C18: ;
    r4 = r0 * 0x64;
    r0 = *(u32*)((u8*)r31 + 0x1C);
    r0 = (u32)r4 / (u32)r0;
    r0 = (s16)r0;
    *(u16*)((u8*)r3 + 0xC) = r0;
    r0 = *(s16*)((u8*)r3 + 0xC);
    if ((s32)r0 >= (s32)0xf) goto L_80011C40;
    r0 = 0xf;
    *(u16*)((u8*)r3 + 0xC) = r0;
L_80011C40: ;
    r4 = *(u32*)((u8*)r31 + 0x20);
    r0 = 0x0;
    *(u32*)((u8*)r3 + 0x8) = r4;
    *(u32*)((u8*)r31 + 0x20) = r29;
    *(u16*)((u8*)r3 + 0xE) = r0;
    r3 = 0x4d0;
    fn_80166A28();
L_80011C5C: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
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
    extern void fn_80103FE4();
    extern void fn_801040A0();
    extern void fn_80104704();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r4;
    fn_80104704();
    r30 = r3;
    if ((u32)r30 == (u32)0x0) goto L_80011D80;
    fn_80103FE4();
    r31 = r3;
    r3 = r30;
    fn_801040A0();
    r0 = *(s16*)((u8*)r31 + 0x1A);
    r4 = (s16)r29;
    r0 = r0 - r4;
    if ((s32)r0 >= (s32)0x0) goto L_80011D44;
    r0 = -r0;
L_80011D44: ;
    r4 = r0 * 0x64;
    r0 = *(s16*)((u8*)r31 + 0x18);
    r0 = (s32)r4 / (s32)r0;
    r0 = (s16)r0;
    *(u16*)((u8*)r3 + 0x2) = r0;
    r0 = *(s16*)((u8*)r3 + 0x2);
    if ((s32)r0 > (s32)0x0) goto L_80011D6C;
    r0 = 0x1;
    *(u16*)((u8*)r3 + 0x2) = r0;
L_80011D6C: ;
    r4 = *(s16*)((u8*)r31 + 0x1A);
    r0 = 0x0;
    *(u16*)((u8*)r3 + 0x0) = r4;
    *(u16*)((u8*)r31 + 0x1A) = r29;
    *(u16*)((u8*)r3 + 0x4) = r0;
L_80011D80: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}
#pragma pop

/* 0x80011D9C | 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80011D9C(void) {
    extern void fn_80103F74();
    extern void fn_80104704();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r28 = r3;
    r29 = r4;
    r30 = 0x0;
    fn_80104704();
    r31 = r3;
    if ((u32)r31 == (u32)0x0) goto L_80011E48;
    if ((s32)r28 == (s32)0x49) goto L_80011E00;
    if ((s32)r28 >= (s32)0x49) goto L_80011DF4;
    if ((s32)r28 >= (s32)0x47) goto L_80011E08;
    if ((s32)r28 >= (s32)0x45) goto L_80011E00;
    goto L_80011E0C;
L_80011DF4: ;
    if ((s32)r28 >= (s32)0x4b) goto L_80011E0C;
    goto L_80011E08;
L_80011E00: ;
    r30 = 0x538;
    goto L_80011E0C;
L_80011E08: ;
    r30 = 0x540;
L_80011E0C: ;
    if ((s32)r29 == (s32)0x0) goto L_80011E38;
    r3 = r31;
    r4 = r30;
    r5 = 0x1;
    fn_80103F74();
    r3 = r31;
    r4 = r30;
    r5 = 0x2d;
    ((void(*)(void))fn_801081F8)();
    goto L_80011E48;
L_80011E38: ;
    r3 = r31;
    r4 = r30;
    r5 = 0x0;
    fn_80103F74();
L_80011E48: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x80011E68 | 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80011E68(void) {
    extern void fn_80103FE4();
    extern void fn_80104704();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r31 = r4;
    fn_80104704();
    if ((u32)r3 == (u32)0x0) goto L_80011E90;
    fn_80103FE4();
    *(u16*)((u8*)r3 + 0x1A) = r31;
L_80011E90: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80011EA4 | 0x9B4 -- GSnpc_WarpToLocation continued */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80011EA4(void) {
    extern u8 lbl_80314E08[];
    extern u8 lbl_8047B718[];
    extern u8 lbl_8047B71C[];
    extern u8 lbl_8047B720[];
    extern u8 lbl_8047B724[];
    extern u8 lbl_8047B728[];
    extern u8 lbl_8047B72C[];
    extern u8 lbl_8047B730[];
    extern u8 lbl_8047B738[];
    extern void fn_8001DACC();
    extern void fn_800C46B0();
    extern void fn_800D5BA0();
    extern void fn_800D61E4();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    extern void fn_800FA280();
    extern void fn_80103FE4();
    extern void fn_801040A0();
    extern void fn_80104160();
    extern void fn_801091F4();
    extern void fn_8010B9E8();
    extern void fn_801F54A4();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
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
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x60) = f31;
    /* psq_st f31, 0x68(r1), 0, qr0 */;
    *(f64*)(sp + 0x50) = f30;
    /* psq_st f30, 0x58(r1), 0, qr0 */;
    /* stmw r27, 0x3c(r1) */;
    r27 = r3;
    r28 = r4;
    fn_80103FE4();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    fn_801040A0();
    r31 = r3;
    r3 = r27;
    fn_80103FE4();
    r6 = *(s16*)((u8*)r28 + 0x6);
    r4 = 0x1;
    r5 = *(u8*)((u8*)r3 + 0x16);
    if ((s32)r6 >= (s32)0xa8) goto L_80011F44;
    if ((s32)r6 >= (s32)0x9f) goto L_80011F2C;
    if ((s32)r6 == (s32)0x9a) goto L_80011FC0;
    if ((s32)r6 >= (s32)0x9a) goto L_80011F20;
    if ((s32)r6 >= (s32)0x99) goto L_80011FA8;
    goto L_80011FC0;
L_80011F20: ;
    if ((s32)r6 >= (s32)0x9d) goto L_80011FA8;
    goto L_80011F8C;
L_80011F2C: ;
    if ((s32)r6 == (s32)0xa5) goto L_80011FC0;
    if ((s32)r6 >= (s32)0xa5) goto L_80011F8C;
    if ((s32)r6 >= (s32)0xa4) goto L_80011FA8;
    goto L_80011FC0;
L_80011F44: ;
    if ((s32)r6 >= (s32)0x538) goto L_80011F6C;
    if ((s32)r6 >= (s32)0x534) goto L_80011F60;
    if ((s32)r6 >= (s32)0xaa) goto L_80011FC0;
    goto L_80011FA8;
L_80011F60: ;
    if ((s32)r6 >= (s32)0x536) goto L_80011FA8;
    goto L_80011F8C;
L_80011F6C: ;
    if ((s32)r6 >= (s32)0x53e) goto L_80011F80;
    if ((s32)r6 >= (s32)0x53c) goto L_80011F8C;
    goto L_80011FC0;
L_80011F80: ;
    if ((s32)r6 >= (s32)0x540) goto L_80011FC0;
    goto L_80011FA8;
L_80011F8C: ;
    r0 = *(u8*)((u8*)r3 + 0x29);
    if ((u32)r0 == (u32)0x0) goto L_80011FA0;
    r4 = 0x0;
    goto L_80011FC0;
L_80011FA0: ;
    r4 = 0x1;
    goto L_80011FC0;
L_80011FA8: ;
    r0 = *(u8*)((u8*)r3 + 0x29);
    if ((u32)r0 == (u32)0x0) goto L_80011FBC;
    r4 = 0x1;
    goto L_80011FC0;
L_80011FBC: ;
    r4 = 0x0;
L_80011FC0: ;
    if ((s32)r6 >= (s32)0x534) goto L_80011FE0;
    if ((s32)r6 == (s32)0xa4) goto L_80011FFC;
    if ((s32)r6 >= (s32)0xa4) goto L_80012008;
    if ((s32)r6 == (s32)0x99) goto L_80011FFC;
    goto L_80012008;
L_80011FE0: ;
    if ((s32)r6 >= (s32)0x53c) goto L_80011FF4;
    if ((s32)r6 >= (s32)0x538) goto L_80012008;
    goto L_80011FFC;
L_80011FF4: ;
    if ((s32)r6 >= (s32)0x540) goto L_80012008;
L_80011FFC: ;
    if ((u32)r5 != (u32)0x1) goto L_80012008;
    r4 = 0x0;
L_80012008: ;
    r3 = r28;
    ((void(*)(void))fn_80109220)();
    r3 = r28;
    fn_801091F4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80012834;
    r0 = *(s16*)((u8*)r28 + 0x6);
    r3 = -0x100;
    r8 = *(u8*)((u8*)r27 + 0x8B);
    r29 = r8 | r3;
    if ((s32)r0 == (s32)0xae) goto L_8001230C;
    if ((s32)r0 >= (s32)0xae) goto L_800120AC;
    if ((s32)r0 == (s32)0xa3) goto L_8001230C;
    if ((s32)r0 >= (s32)0xa3) goto L_8001207C;
    if ((s32)r0 >= (s32)0x9f) goto L_8001206C;
    if ((s32)r0 == (s32)0x9a) goto L_8001215C;
    if ((s32)r0 < (s32)0x9a) goto L_80012834;
    if ((s32)r0 >= (s32)0x9d) goto L_80012110;
    goto L_80012834;
L_8001206C: ;
    if ((s32)r0 == (s32)0xa1) goto L_80012170;
    if ((s32)r0 >= (s32)0xa1) goto L_800122C8;
    goto L_80012834;
L_8001207C: ;
    if ((s32)r0 >= (s32)0xaa) goto L_8001209C;
    if ((s32)r0 == (s32)0xa5) goto L_8001215C;
    if ((s32)r0 < (s32)0xa5) goto L_80012834;
    if ((s32)r0 >= (s32)0xa8) goto L_80012110;
    goto L_80012834;
L_8001209C: ;
    if ((s32)r0 == (s32)0xac) goto L_80012170;
    if ((s32)r0 >= (s32)0xac) goto L_800122C8;
    goto L_80012834;
L_800120AC: ;
    if ((s32)r0 == (s32)0x53a) goto L_800123A4;
    if ((s32)r0 >= (s32)0x53a) goto L_800120E8;
    if ((s32)r0 == (s32)0x535) goto L_80012680;
    if ((s32)r0 >= (s32)0x535) goto L_800120DC;
    if ((s32)r0 == (s32)0x533) goto L_800123CC;
    if ((s32)r0 >= (s32)0x533) goto L_80012834;
    if ((s32)r0 >= (s32)0x532) goto L_800123A4;
    goto L_80012834;
L_800120DC: ;
    if ((s32)r0 == (s32)0x537) goto L_80012758;
    goto L_80012834;
L_800120E8: ;
    if ((s32)r0 == (s32)0x53e) goto L_80012834;
    if ((s32)r0 >= (s32)0x53e) goto L_80012104;
    if ((s32)r0 == (s32)0x53c) goto L_80012834;
    if ((s32)r0 >= (s32)0x53c) goto L_80012680;
    goto L_800123CC;
L_80012104: ;
    if ((s32)r0 >= (s32)0x540) goto L_80012834;
    goto L_80012758;
L_80012110: ;
    r0 = *(u8*)((u8*)r30 + 0x29);
    if ((u32)r0 != (u32)0x2) goto L_80012834;
    r5 = *(u16*)((u8*)r31 + 0x6);
    r0 = (0x4330 << 16);
    *(u32*)(sp + 0x8) = r0;
    r3 = r27;
    f2 = *(f64*)lbl_8047B730;
    r4 = r28;
    f0 = *(f32*)lbl_8047B720;
    f1 = *(f64*)(sp + 0x8);
    f1 = f1 - f2;
    f1 = f1 / f0;
    fn_8001DACC();
    r3 = r28;
    r4 = 0x0;
    ((void(*)(void))fn_80109220)();
    goto L_80012834;
L_8001215C: ;
    r5 = *(u16*)((u8*)r30 + 0x26);
    r3 = r27;
    r4 = r28;
    fn_8010B9E8();
    goto L_80012834;
L_80012170: ;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x32;
    r6 = 0x0;
    fn_801F54A4();
    if ((s32)r3 == (s32)0x0) goto L_80012198;
    r0 = *(u8*)((u8*)r30 + 0x16);
    if ((u32)r0 != (u32)0x0) goto L_80012834;
L_80012198: ;
    r3 = *(s16*)((u8*)r31 + 0x2);
    if ((s32)r3 == (s32)0x0) goto L_80012240;
    r0 = *(s16*)((u8*)r31 + 0x4);
    r5 = (0x4330 << 16);
    /* xoris r4, r3, 0x8000 */;
    r6 = *(s16*)((u8*)r31 + 0x0);
    /* xoris r0, r0, 0x8000 */;
    r3 = *(s16*)((u8*)r30 + 0x1A);
    *(u32*)(sp + 0xC) = r0;
    /* xoris r0, r6, 0x8000 */;
    r3 = r3 - r6;
    f3 = *(f64*)lbl_8047B738;
    /* xoris r3, r3, 0x8000 */;
    f0 = *(f64*)(sp + 0x8);
    f1 = f0 - f3;
    f0 = *(f64*)(sp + 0x10);
    f0 = f0 - f3;
    f2 = f1 / f0;
    f1 = *(f64*)(sp + 0x18);
    *(u32*)(sp + 0x24) = r0;
    f0 = *(f64*)(sp + 0x20);
    f1 = f1 - f3;
    f0 = f0 - f3;
    f1 = f2 * f1 + f0;
    f0 = (f64)(s32)f1;
    *(f64*)(sp + 0x28) = f0;
    r27 = *(u32*)(sp + 0x2C);
    r0 = (s16)r27;
    if ((s32)r0 != (s32)0x0) goto L_80012244;
    f0 = *(f32*)lbl_8047B718;
    if (f1 <= f0) goto L_80012244;
    r27 = 0x1;
    goto L_80012244;
L_80012240: ;
    r27 = *(s16*)((u8*)r30 + 0x1A);
L_80012244: ;
    r5 = r29;
    r3 = 0x20;
    r4 = -0x2;
    r6 = 0x195;
    ((void(*)(void))fn_800FB680)();
    r4 = (s16)r27;
    r3 = 0x34;
    ((void(*)(void))fn_80132A38)();
    r3 = 0xcb;
    ((void(*)(void))fn_800FA444)();
    r0 = (u32)r3 >> 16;
    r5 = r29;
    r0 = (s16)r0;
    r4 = -0x1;
    r0 = 0x18 - r0;
    r6 = 0xcb;
    r3 = (s16)r0;
    ((void(*)(void))fn_800FB680)();
    r4 = *(s16*)((u8*)r30 + 0x18);
    r3 = 0x34;
    ((void(*)(void))fn_80132A38)();
    r3 = 0xcb;
    ((void(*)(void))fn_800FA444)();
    r3 = (u32)r3 >> 16;
    r0 = *(s16*)((u8*)r28 + 0x54);
    r3 = (s16)r3;
    r5 = r29;
    r0 = r0 - r3;
    r4 = -0x1;
    r3 = (s16)r0;
    r6 = 0xcb;
    ((void(*)(void))fn_800FB680)();
    goto L_80012834;
L_800122C8: ;
    r4 = *(u8*)((u8*)r30 + 0x17);
    r3 = 0x34;
    ((void(*)(void))fn_80132A38)();
    r3 = 0xcb;
    ((void(*)(void))fn_800FA444)();
    r3 = (u32)r3 >> 16;
    r0 = *(s16*)((u8*)r28 + 0x54);
    r3 = (s16)r3;
    r5 = *(u8*)((u8*)r27 + 0x8B);
    r3 = r0 - r3;
    r0 = -0x100;
    r3 = (s16)r3;
    r4 = -0x1;
    r5 = r5 | r0;
    r6 = 0xcb;
    ((void(*)(void))fn_800FB680)();
    goto L_80012834;
L_8001230C: ;
    r4 = r30;
    r3 = 0x37;
    ((void(*)(void))fn_80132A38)();
    r5 = r29;
    r3 = 0x0;
    r4 = -0x1;
    r6 = 0xe9;
    ((void(*)(void))fn_800FB680)();
    r3 = 0xe9;
    ((void(*)(void))fn_800FA444)();
    r0 = *(u8*)((u8*)r30 + 0x28);
    r3 = (u32)r3 >> 16;
    r27 = (s16)r3;
    if ((s32)r0 == (s32)0x1) goto L_80012368;
    if ((s32)r0 >= (s32)0x1) goto L_80012358;
    if ((s32)r0 >= (s32)0x0) goto L_80012360;
    goto L_80012370;
L_80012358: ;
    goto L_80012370;
L_80012360: ;
    r3 = 0xd67;
    goto L_80012374;
L_80012368: ;
    r3 = 0xd68;
    goto L_80012374;
L_80012370: ;
    r3 = 0x0;
L_80012374: ;
    if ((u32)r3 == (u32)0x0) goto L_80012834;
    fn_800FA280();
    r4 = r3;
    r3 = 0x37;
    ((void(*)(void))fn_80132A38)();
    r3 = r27;
    r5 = r29;
    r4 = -0x1;
    r6 = 0xd0;
    ((void(*)(void))fn_800FB680)();
    goto L_80012834;
L_800123A4: ;
    r9 = *(u16*)((u8*)r30 + 0x24);
    r7 = r29;
    r5 = *(s16*)((u8*)r28 + 0x54);
    r8 = r27;
    r6 = *(s16*)((u8*)r28 + 0x56);
    r3 = 0x0;
    r4 = 0x0;
    r10 = 0x0;
    fn_80104160();
    goto L_80012834;
L_800123CC: ;
    r6 = *(s16*)((u8*)r31 + 0x2);
    if ((s32)r6 == (s32)0x0) goto L_8001244C;
    r0 = *(s16*)((u8*)r31 + 0x4);
    r5 = (0x4330 << 16);
    /* xoris r4, r6, 0x8000 */;
    r7 = *(s16*)((u8*)r31 + 0x0);
    /* xoris r0, r0, 0x8000 */;
    r3 = *(s16*)((u8*)r30 + 0x1A);
    *(u32*)(sp + 0x2C) = r0;
    /* xoris r0, r7, 0x8000 */;
    r3 = r3 - r7;
    f3 = *(f64*)lbl_8047B738;
    /* xoris r3, r3, 0x8000 */;
    f0 = *(f64*)(sp + 0x28);
    f1 = f0 - f3;
    f0 = *(f64*)(sp + 0x20);
    f0 = f0 - f3;
    f2 = f1 / f0;
    f1 = *(f64*)(sp + 0x18);
    *(u32*)(sp + 0x14) = r0;
    f0 = *(f64*)(sp + 0x10);
    f1 = f1 - f3;
    f0 = f0 - f3;
    f31 = f2 * f1 + f0;
    goto L_8001246C;
L_8001244C: ;
    r3 = *(s16*)((u8*)r30 + 0x1A);
    r0 = (0x4330 << 16);
    *(u32*)(sp + 0x8) = r0;
    /* xoris r0, r3, 0x8000 */;
    f1 = *(f64*)lbl_8047B738;
    *(u32*)(sp + 0xC) = r0;
    f0 = *(f64*)(sp + 0x8);
    f31 = f0 - f1;
L_8001246C: ;
    r0 = *(s16*)((u8*)r30 + 0x18);
    r3 = (0x4330 << 16);
    /* xoris r0, r0, 0x8000 */;
    f3 = *(f64*)lbl_8047B738;
    *(u32*)(sp + 0x34) = r0;
    f0 = *(f64*)(sp + 0x30);
    f30 = f0 - f3;
    if ((s32)r6 == (s32)0x0) goto L_800125B8;
    r4 = *(s16*)((u8*)r31 + 0x0);
    if ((s32)r4 > (s32)0x0) goto L_800124A8;
    r31 = 0x0;
    goto L_80012514;
L_800124A8: ;
    f0 = *(f32*)lbl_8047B724;
    /* xoris r0, r4, 0x8000 */;
    f1 = *(f32*)lbl_8047B71C;
    f0 = f0 * f30;
    *(u32*)(sp + 0x34) = r0;
    f0 = f0 / f1;
    f2 = *(f64*)(sp + 0x30);
    f2 = f2 - f3;
    /* cror eq, lt, eq */;
    if (f2 != f0) goto L_800124E0;
    r31 = (0x8000 << 16);
    goto L_80012514;
L_800124E0: ;
    f0 = *(f32*)lbl_8047B728;
    *(u32*)(sp + 0x34) = r0;
    f0 = f0 * f30;
    f0 = f0 / f1;
    f1 = *(f64*)(sp + 0x30);
    f1 = f1 - f3;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_80012510;
    r31 = (0x6464 << 16);
    goto L_80012514;
L_80012510: ;
    r31 = (0x80 << 16);
L_80012514: ;
    r3 = *(s16*)((u8*)r28 + 0x54);
    r0 = (0x4330 << 16);
    f0 = *(f32*)lbl_8047B72C;
    r31 = r31 | r8;
    r4 = r4 * r3;
    *(u32*)(sp + 0x30) = r0;
    f1 = *(f64*)lbl_8047B738;
    f2 = f30 - f0;
    r3 = 0x1;
    /* xoris r0, r4, 0x8000 */;
    *(u32*)(sp + 0x34) = r0;
    f0 = *(f64*)(sp + 0x30);
    f0 = f0 - f1;
    f0 = f2 + f0;
    f0 = f0 / f30;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x28) = f0;
    r30 = *(u32*)(sp + 0x2C);
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
    r3 = 0x0;
    r4 = 0x0;
    fn_800D61E4();
    r4 = r31;
    r3 = 0x0;
    fn_800D5BA0();
    r4 = *(s16*)((u8*)r28 + 0x56);
    r3 = r30;
    fn_800D61E4();
    r4 = r31;
    r3 = 0x0;
    fn_800D5BA0();
    fn_800D6728();
L_800125B8: ;
    f0 = *(f32*)lbl_8047B718;
    /* cror eq, lt, eq */;
    if (f31 != f0) goto L_800125D0;
    r9 = 0x0;
    goto L_80012618;
L_800125D0: ;
    f0 = *(f32*)lbl_8047B724;
    f1 = *(f32*)lbl_8047B71C;
    f0 = f0 * f30;
    f0 = f0 / f1;
    /* cror eq, lt, eq */;
    if (f31 != f0) goto L_800125F4;
    r9 = 0x1ad;
    goto L_80012618;
L_800125F4: ;
    f0 = *(f32*)lbl_8047B728;
    f0 = f0 * f30;
    f0 = f0 / f1;
    /* cror eq, lt, eq */;
    if (f31 != f0) goto L_80012614;
    r9 = 0x1b0;
    goto L_80012618;
L_80012614: ;
    r9 = 0x1b1;
L_80012618: ;
    r4 = *(s16*)((u8*)r28 + 0x54);
    r3 = (0x4330 << 16);
    f0 = *(f32*)lbl_8047B72C;
    r0 = r9 & 0xFFFF;
    /* xoris r4, r4, 0x8000 */;
    f2 = *(f64*)lbl_8047B738;
    f0 = f30 - f0;
    f1 = *(f64*)(sp + 0x30);
    f1 = f1 - f2;
    f0 = f31 * f1 + f0;
    f0 = f0 / f30;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x28) = f0;
    r5 = *(u32*)(sp + 0x2C);
    if ((u32)r0 == (u32)0x0) goto L_80012834;
    r6 = *(s16*)((u8*)r28 + 0x56);
    r7 = r29;
    r8 = r27;
    r3 = 0x0;
    r4 = 0x0;
    r10 = 0x0;
    fn_80104160();
    goto L_80012834;
L_80012680: ;
    r3 = *(s16*)((u8*)r31 + 0xC);
    if ((s32)r3 == (s32)0x0) goto L_80012700;
    r0 = *(s16*)((u8*)r31 + 0xE);
    r4 = (0x4330 << 16);
    /* xoris r3, r3, 0x8000 */;
    r6 = *(u32*)((u8*)r31 + 0x8);
    /* xoris r5, r0, 0x8000 */;
    r0 = *(u32*)((u8*)r30 + 0x20);
    r0 = r0 - r6;
    f3 = *(f64*)lbl_8047B738;
    f2 = *(f64*)lbl_8047B730;
    f0 = *(f64*)(sp + 0x30);
    f1 = f0 - f3;
    f0 = *(f64*)(sp + 0x28);
    *(u32*)(sp + 0x24) = r0;
    f0 = f0 - f3;
    f3 = f1 / f0;
    f1 = *(f64*)(sp + 0x20);
    f0 = *(f64*)(sp + 0x18);
    f1 = f1 - f2;
    f0 = f0 - f2;
    f1 = f3 * f1 + f0;
    fn_800C46B0();
    goto L_80012704;
L_80012700: ;
    r3 = *(u32*)((u8*)r30 + 0x20);
L_80012704: ;
    r11 = *(u32*)((u8*)r30 + 0x1C);
    if ((u32)r3 <= (u32)r11) goto L_80012714;
    r3 = r11;
L_80012714: ;
    if ((u32)r11 == (u32)0x0) goto L_80012834;
    r0 = *(s16*)((u8*)r28 + 0x54);
    r7 = r29;
    r6 = *(s16*)((u8*)r28 + 0x56);
    r8 = r27;
    r0 = r3 * r0;
    r3 = 0x0;
    r4 = 0x0;
    r9 = 0x1ac;
    r10 = 0x0;
    r5 = r11 + r0;
    /* subi r0, r5, 0x1 */;
    r0 = (u32)r0 / (u32)r11;
    r5 = (s16)r0;
    fn_80104160();
    goto L_80012834;
L_80012758: ;
    r0 = *(s16*)((u8*)r31 + 0xC);
    if ((s32)r0 == (s32)0x0) goto L_800127B0;
    r4 = *(s16*)((u8*)r31 + 0xE);
    r3 = (0x4330 << 16);
    /* xoris r0, r0, 0x8000 */;
    /* xoris r4, r4, 0x8000 */;
    f3 = *(f64*)lbl_8047B738;
    f4 = *(f32*)((u8*)r31 + 0x8);
    f0 = *(f64*)(sp + 0x30);
    *(u32*)(sp + 0x2C) = r0;
    f2 = f0 - f3;
    f0 = *(f32*)((u8*)r30 + 0x20);
    f0 = f0 - f4;
    f1 = *(f64*)(sp + 0x28);
    f1 = f1 - f3;
    f1 = f2 / f1;
    f2 = f1 * f0 + f4;
    goto L_800127B4;
L_800127B0: ;
    f2 = *(f32*)((u8*)r30 + 0x20);
L_800127B4: ;
    f3 = *(f32*)((u8*)r30 + 0x1C);
    if (f2 <= f3) goto L_800127C4;
    f2 = f3;
L_800127C4: ;
    r3 = *(s16*)((u8*)r28 + 0x54);
    r0 = (0x4330 << 16);
    f0 = *(f32*)lbl_8047B718;
    /* xoris r3, r3, 0x8000 */;
    *(u32*)(sp + 0x20) = r0;
    f1 = *(f64*)lbl_8047B738;
    f0 = *(f64*)(sp + 0x20);
    f0 = f0 - f1;
    f0 = f2 * f0;
    f0 = f0 / f3;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x18) = f0;
    r5 = *(u32*)(sp + 0x1C);
    if (f2 <= f0) goto L_80012814;
    r0 = (s16)r5;
    if ((s32)r0 != (s32)0x0) goto L_80012814;
    r5 = 0x1;
L_80012814: ;
    r6 = *(s16*)((u8*)r28 + 0x56);
    r7 = r29;
    r8 = r27;
    r3 = 0x0;
    r4 = 0x0;
    r9 = 0x1ab;
    r10 = 0x0;
    fn_80104160();
L_80012834: ;
    /* psq_l f31, 0x68(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0x60);
    /* psq_l f30, 0x58(r1), 0, qr0 */;
    f30 = *(f64*)(sp + 0x50);
    /* lmw r27, 0x3c(r1) */;
    return;
}
#pragma pop
