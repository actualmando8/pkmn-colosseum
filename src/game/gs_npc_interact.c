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
void fn_800111C4(void) {
    extern void fn_80089F78();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r6 = 0;

    r6 = 0x0;
    fn_80089F78();
    return;
}

/* 0x800111E8 | 0x24 -- small accessor */
void fn_800111E8(void) {
    extern void fn_80089F78();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r6 = 0;

    r6 = 0x1;
    fn_80089F78();
    return;
}

/* 0x7C | fn_8001120C | nullcheck_call_flag */
u32 fn_8001120C(void* obj) {
    if (fn_80102620() == 0) { return 255; }
    fn_80102620(obj);
    return 0;
}

/* 0x80011288 | 0x21C */
void fn_80011288(void) {
    extern void fn_8005D8F8();
    extern void fn_80102138();
    extern void fn_801026A4();
    u8 sp[0x20];
    u32 tmp = 0;
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
    tmp = *(u8*)((u8*)r28 + 0x21);
    if (tmp == 0) {
    do {
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
        do {
        do {
            tmp = *(u32*)((u8*)r31 + 0x4);
            if ((s32)tmp != 0x47) {
                if ((s32)tmp < 0x47) {
                    if ((s32)tmp != 0x45) {
                        if ((s32)tmp < 0x45) {
                            goto L_8001133C;
                        }
                        if ((s32)tmp >= 0x49) goto L_8001133C;
                        goto L_80011334;
                        }
                    r3 = 0x125b;
                    break;
                        }
                r3 = 0x125a;
                break;
            }
            r3 = 0x1258;
            break;
        L_80011334:
            r3 = 0x1259;
            break;
        L_8001133C:
            r3 = 0x0;
        } while (0);
            if ((s32)r3 != 0) {
                r4 = 0x1;
                fn_8005D8F8();
            }
            r31 = r31 + 0x8;
            r30 = r30 + 0x1;
        } while ((s32)r30 < 4);
        r3 = 0xff;
        r4 = 0x125a;
        fn_80102138();
        if ((s32)r3 == (s32)-0x1) {
            r3 = 0xff;
            r4 = 0x125b;
            fn_80102138();
            if ((s32)r3 == (s32)-0x1) {
                tmp = 0x0;
                *(u32*)(sp + 0x8) = tmp;
        }
        }
        r7 = r29;
        r9 = r28;
        r5 = (u32)sp + 0x8;
        r3 = 0xff;
        r4 = 0x0;
        r6 = 0x0;
        r8 = 0x1;
        fn_801026A4();
        if ((s32)r3 != 0x125a) {
            if ((s32)r3 < 0x125a) {
                if ((s32)r3 != 0x1258) {
                    if ((s32)r3 < 0x1258) {
                        goto L_80011404;
                    }
                    if ((s32)r3 >= 0x125c) goto L_80011404;
                    goto L_800113FC;
                    }
                r4 = 0x47;
                break;
                    }
            r4 = 0x48;
            break;
        }
        r4 = 0x46;
        break;
    L_800113FC:
        r4 = 0x45;
        break;
    L_80011404:
        r4 = -0x1;
    } while (0);
        r3 = -0x1;
        tmp = *(u32*)((u8*)r28 + 0x4);
        if ((s32)r4 == (s32)tmp) {
            r3 = 0x0;
            return;
        }
        tmp = *(u32*)((u8*)r28 + 0xC);
        if ((s32)r4 == (s32)tmp) {
            r3 = 0x1;
            return;
        }
        tmp = *(u32*)((u8*)r28 + 0x14);
        if ((s32)r4 == (s32)tmp) {
            r3 = 0x2;
            return;
        }
        tmp = *(u32*)((u8*)r28 + 0x1C);
        if ((s32)r4 != (s32)tmp) return;
        r3 = 0x3;
        return;
        return;
    }
    r5 = r4;
    r7 = r29;
    r9 = r28;
    r3 = 0x104;
    r4 = 0x0;
    r6 = 0x0;
    r8 = 0x1;
    fn_801026A4();

    return;
}

/* 0x800114A4 | 0x25C */
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
    f32 f8 = 0.0f;

    r24 = r3;
    r25 = r4;
    r26 = r5;
    r27 = r6;
    r28 = r7;
    r3 = r26;
    fn_8020E204();
    fn_8020E1A4();
    tmp = r28 & 0xFF;
    r31 = r3;
    if (tmp == 0) {
        r4 = r27;
        r5 = r24;
        r6 = r25;
        fn_8001BD80();
        r29 = r3;
        r3 = r29;
        return;
    }
    r30 = r27 & 0xFF;
L_80011500:
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
    fn_801026A4();
    tmp = r3;
    r3 = 0xf8;
    r29 = tmp;
    r4 = 0x20;
    ((void(*)(void))fn_801080CC)();
    if ((s32)r29 == (s32)-0x1) {
        r3 = 0xf8;
        r4 = 0x0;
        r5 = 0x1;
        fn_80102568();
        r3 = -0x1;
        return;
    }
    r3 = r25;
    r4 = r24;
    r5 = r29;
    fn_80010C98();
    tmp = r3 & 0xFF;
    if (tmp == 0) goto L_80011500;
    tmp = r27 & 0xFF;
    do {
        if (tmp == 0) break;
        tmp = r31 & 0xFF;
        if (tmp < 2) break;
        r4 = r25;
        r5 = r26;
        r3 = 0xf;
        fn_801F02AC();
        r23 = r3;
        fn_802062FC();
        tmp = r3 & 0xFF;
        if (tmp == 1) {
            r3 = r23;
            r4 = r26;
            r5 = 0x0;
            fn_802656AC();
        } else {

            r3 = 0x0;
        }
        r4 = r25;
        r5 = r26;
        r3 = 0x10;
        fn_801F02AC();
        r23 = r3;
        fn_802062FC();
        tmp = r3 & 0xFF;
        if (tmp == 1) {
            r3 = r23;
            r4 = r26;
            r5 = 0x0;
            fn_802656AC();
        } else {

            r3 = 0x0;
        }
        r4 = r25;
        r5 = r26;
        r3 = 0xe;
        fn_801F02AC();
        r23 = r3;
        fn_802062FC();
        tmp = r3 & 0xFF;
        if (tmp == 1) {
            r3 = r23;
            r4 = r26;
            r5 = 0x0;
            fn_802656AC();
        } else {

            r3 = 0x0;
        }
        tmp = 0x0;
        r3 = (u32)sp + 0x10;
        r4 = 0x0;
        *(u32*)(sp + 0x2C) = tmp;
        r5 = 0x1;
        *(u8*)(sp + 0x31) = r28;
        fn_80011288();
        r23 = r3;
        r3 = 0xff;
        fn_80102620();
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            r3 = 0xff;
            r4 = 0x0;
            r5 = 0x1;
            fn_80102568();
        }
        r3 = 0x104;
        fn_80102620();
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            r3 = 0x104;
            r4 = 0x0;
            r5 = 0x1;
            fn_80102568();
        }
        r3 = 0x100;
        fn_80102620();
        if ((s32)r23 == (s32)-0x1) goto L_80011500;
    } while (0);

    r3 = 0xf8;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();

    r3 = r29;

    return;
}

/* 0x80011700 | 0xBC */
void fn_80011700(void) {
    extern void fn_80102568();
    extern void fn_80102620();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;
    f32 f7 = 0.0f;
    f32 f9 = 0.0f;

    r31 = r3;
    r3 = 0x4c;
    fn_80102620();
    tmp = r3 & 0xFF;
    if (tmp != 0) {
        r5 = r31;
        r3 = 0x4c;
        r4 = 0x0;
        fn_80102568();
    }
    r3 = 0xf9;
    fn_80102620();
    tmp = r3 & 0xFF;
    if (tmp != 0) {
        r5 = r31;
        r3 = 0xf9;
        r4 = 0x0;
        fn_80102568();
    }
    r3 = 0xfa;
    fn_80102620();
    tmp = r3 & 0xFF;
    if (tmp != 0) {
        r5 = r31;
        r3 = 0xfa;
        r4 = 0x0;
        fn_80102568();
    }
    r3 = 0xf7;
    fn_80102620();
    tmp = r3 & 0xFF;
    if (tmp != 0) {
        r5 = r31;
        r3 = 0xf7;
        r4 = 0x0;
        fn_80102568();
    }
    r3 = 0x0;
    return;
}

/* 0x800117BC | 0x1EC */
void fn_800117BC(void) {
    extern void fn_80010128();
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_801026A4();
    extern void fn_80106080();
    extern void fn_80106394();
    u8 sp[0x20];
    u32 tmp = 0;
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

    r28 = r3;
    r29 = r4;
    r30 = r5;
    tmp = *(u8*)((u8*)r28 + 0x44);
    if (tmp == 0) {
        r31 = 0x4c;
        goto L_800117F0;
    }
    r31 = 0xf7;
L_800117F0:
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
    fn_801026A4();
    r27 = r3;
    do {
        if ((s32)r27 < 0) break;
        r3 = r28;
        r4 = r27 & 0xFFFF;
        fn_80010128();
        r26 = r3;
        if (r26 == 0) break;
        r3 = 0x4c;
        fn_80102620();
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            r3 = 0x4c;
            r4 = 0x0;
            r5 = 0x1;
            fn_80102568();
        }
        r3 = 0xf9;
        fn_80102620();
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            r3 = 0xf9;
            r4 = 0x0;
            r5 = 0x1;
            fn_80102568();
        }
        r3 = 0xfa;
        fn_80102620();
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            r3 = 0xfa;
            r4 = 0x0;
            r5 = 0x1;
            fn_80102568();
        }
        r3 = 0xf7;
        fn_80102620();
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            r3 = 0xf7;
            r4 = 0x0;
            r5 = 0x1;
            fn_80102568();
        }
        r3 = r26;
        r4 = 0x1;
        r5 = 0x1;
        fn_80106394();
        while (1) {
            r3 = 0x1;
            ((void(*)(void))fn_800F7AF0)();
            r27 = r3;
            r3 = 0x1;
            ((void(*)(void))fn_800F7BC4)();
            tmp = r3 & r27;
            tmp = tmp & 0x00000300;
            if (tmp != 0) break;
            r3 = 0x0;
            ((void(*)(void))fn_801F18DC)();
            tmp = r3 & 0xFF;
            do {
                if (tmp == 0) break;
                r3 = 0x0;
                ((void(*)(void))fn_801F1700)();
                tmp = r3 & 0xFF;
                do {
                    if (tmp != 1) break;
                    ((void(*)(void))fn_80265924)();
                    tmp = r3 & 0xFF;
                    if (tmp != 1) break;
                    tmp = 0x1;
                    break;
                } while (0);

                ((void(*)(void))fn_801EF634)();
                tmp = r3 & 0xFFFF;
                if (tmp != 1) break;
                tmp = 0x1;
                break;
            } while (0);

            tmp = 0x0;

            tmp = tmp & 0xFF;
            if (tmp != 0) break;
            ((void(*)(void))fn_800F0308)();

        }

        r3 = 0x1;
        fn_80106080();
        goto L_800117F0;
    } while (0);

    r3 = r31;
    r4 = 0x20;
    ((void(*)(void))fn_801080CC)();
    r3 = r27;
    return;
}

/* 0x74 | fn_800119A8 | nullcheck_call_flag */
u32 fn_800119A8(void* obj) {
    if (fn_80102620() == 0) { return 75; }
    fn_80102568(obj);
    return 0;
}

/* 0x80011A1C | 0x130 */
void fn_80011A1C(void) {
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_801026A4();
    u8 sp[0x30];
    u32 tmp = 0;
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

    r26 = r3;
    r27 = r4;
    r28 = r5;
    r29 = *(u8*)((u8*)r26 + 0x16);
    do {
        tmp = *(u8*)((u8*)r26 + 0x17);
        if (tmp != 0) {
            r5 = r27;
            r7 = r28;
            r9 = r26;
            r3 = 0xf6;
            r4 = 0x0;
            r6 = 0x0;
            r8 = 0x1;
            fn_801026A4();
        } else {

            r5 = r27;
            r7 = r28;
            r9 = r26;
            r3 = 0x4b;
            r4 = 0x0;
            r6 = 0x0;
            r8 = 0x1;
            fn_801026A4();
        }
        r30 = r3;
        if (r29 != 0) { r3 = r30; return; }
        if ((s32)r3 != 3) { r3 = r30; return; }
        tmp = 0x0;
        r3 = 0x4b;
        *(u32*)(sp + 0x8) = tmp;
        fn_80102620();
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            r3 = 0x4b;
            r4 = 0x0;
            r5 = 0x1;
            fn_80102568();
        }
        r3 = 0xf6;
        fn_80102620();
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            r3 = 0xf6;
            r4 = 0x0;
            r5 = 0x1;
            fn_80102568();
        }
        r5 = (u32)sp + 0x8;
        r3 = 0xfb;
        r4 = 0x0;
        r6 = 0x0;
        r7 = 0x1;
        r8 = 0x0;
        fn_801026A4();
        r31 = r3;
        r3 = 0xfb;
        r4 = 0x0;
        r5 = 0x1;
        fn_80102568();
    } while ((s32)r31 != 0x1207);

    r3 = r30;
    return;
}

/* 0x78 | fn_80011B4C | generic */
u32 fn_80011B4C(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    fn_80104704();
    fn_801040A0();
    fn_800F0308();
    return 1;
}

/* 0x80011BC4 | 0xB4 */
void fn_80011BC4(void) {
    extern void fn_80103FE4();
    extern void fn_801040A0();
    extern void fn_80104704();
    extern void fn_80166A28();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r4;
    fn_80104704();
    r30 = r3;
    if (r30 != 0) {
        fn_80103FE4();
        r31 = r3;
        r3 = r30;
        fn_801040A0();
        tmp = *(u32*)((u8*)r31 + 0x20);
        if (r29 > tmp) {
            tmp = r29 - tmp;
        } else {

            tmp = tmp - r29;
        }
        r4 = tmp * 0x64;
        tmp = *(u32*)((u8*)r31 + 0x1C);
        tmp = (u32)r4 / (u32)tmp;
        tmp = (s16)tmp;
        *(u16*)((u8*)r3 + 0xC) = tmp;
        tmp = *(s16*)((u8*)r3 + 0xC);
        if ((s32)tmp < 0xf) {
            tmp = 0xf;
            *(u16*)((u8*)r3 + 0xC) = tmp;
        }
        r4 = *(u32*)((u8*)r31 + 0x20);
        tmp = 0x0;
        *(u32*)((u8*)r3 + 0x8) = r4;
        *(u32*)((u8*)r31 + 0x20) = r29;
        *(u16*)((u8*)r3 + 0xE) = tmp;
        r3 = 0x4d0;
        fn_80166A28();
    }
    return;
}

/* 0x78 | fn_80011C78 | generic */
u32 fn_80011C78(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    fn_80104704();
    fn_801040A0();
    fn_800F0308();
    return 1;
}

/* 0x80011CF0 | 0xAC */
void fn_80011CF0(void) {
    extern void fn_80103FE4();
    extern void fn_801040A0();
    extern void fn_80104704();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r4;
    fn_80104704();
    r30 = r3;
    if (r30 != 0) {
        fn_80103FE4();
        r31 = r3;
        r3 = r30;
        fn_801040A0();
        tmp = *(s16*)((u8*)r31 + 0x1A);
        r4 = (s16)r29;
        tmp = tmp - r4;
        if ((s32)tmp < 0) {
            tmp = -tmp;
        }
        r4 = tmp * 0x64;
        tmp = *(s16*)((u8*)r31 + 0x18);
        tmp = (s32)r4 / (s32)tmp;
        tmp = (s16)tmp;
        *(u16*)((u8*)r3 + 0x2) = tmp;
        tmp = *(s16*)((u8*)r3 + 0x2);
        if ((s32)tmp <= 0) {
            tmp = 0x1;
            *(u16*)((u8*)r3 + 0x2) = tmp;
        }
        r4 = *(s16*)((u8*)r31 + 0x1A);
        tmp = 0x0;
        *(u16*)((u8*)r3 + 0x0) = r4;
        *(u16*)((u8*)r31 + 0x1A) = r29;
        *(u16*)((u8*)r3 + 0x4) = tmp;
    }
    return;
}

/* 0x80011D9C | 0xCC */
void fn_80011D9C(void) {
    extern void fn_80103F74();
    extern void fn_80104704();
    u8 sp[0x20];
    u32 tmp = 0;
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
    if (r31 == 0) return;
    do {
        if ((s32)r28 == 0x49) break;
        if ((s32)r28 < 0x49) {
            if ((s32)r28 >= 0x47) break;
            if ((s32)r28 >= 0x45) break;
            break;
        }
        if ((s32)r28 >= 0x4b) break;
        break;
    } while (0);

    r30 = 0x538;
    if ((s32)r29 != 0) {
        r3 = r31;
        r4 = r30;
        r5 = 0x1;
        fn_80103F74();
        r3 = r31;
        r4 = r30;
        r5 = 0x2d;
        ((void(*)(void))fn_801081F8)();
        return;
    }
    r3 = r31;
    r4 = r30;
    r5 = 0x0;
    fn_80103F74();

    return;
}

/* 0x80011E68 | 0x3C */
/* Set an NPC's facing direction. */
void fn_80011E68(u32 npcId, u16 direction) {
    extern void* fn_80103FE4(void* obj);
    extern void* fn_80104704(u32 npcId);
    void* npc;

    npc = fn_80104704(npcId);
    if (npc != NULL) {
        u8* obj = (u8*)fn_80103FE4(npc);
        *(u16*)(obj + 0x1A) = direction;
    }
}

/* 0x80011EA4 | 0x9B4 -- GSnpc_WarpToLocation continued */
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

    r27 = r3;
    r28 = r4;
    fn_80103FE4();
    tmp = r3;
    r3 = r27;
    r30 = tmp;
    fn_801040A0();
    r31 = r3;
    r3 = r27;
    fn_80103FE4();
    r6 = *(s16*)((u8*)r28 + 0x6);
    r4 = 0x1;
    r5 = *(u8*)((u8*)r3 + 0x16);
    if ((s32)r6 < 0xa8) {
        if ((s32)r6 < 0x9f) {
            if ((s32)r6 == 0x9a) goto L_80011FC0;
            if ((s32)r6 < 0x9a) {
                if ((s32)r6 >= 0x99) goto L_80011FA8;
                goto L_80011FC0;
            }
            if ((s32)r6 >= 0x9d) goto L_80011FA8;
            goto L_80011F8C;
        }
        if ((s32)r6 == 0xa5) goto L_80011FC0;
        if ((s32)r6 >= 0xa5) goto L_80011F8C;
        if ((s32)r6 >= 0xa4) goto L_80011FA8;
        goto L_80011FC0;
    }
    if ((s32)r6 < 0x538) {
        if ((s32)r6 < 0x534) {
            if ((s32)r6 >= 0xaa) goto L_80011FC0;
            goto L_80011FA8;
        }
        if ((s32)r6 >= 0x536) goto L_80011FA8;
        goto L_80011F8C;
    }
    if ((s32)r6 < 0x53e) {
        if ((s32)r6 >= 0x53c) goto L_80011F8C;
        goto L_80011FC0;
    }
    if ((s32)r6 >= 0x540) goto L_80011FC0;
    goto L_80011FA8;
L_80011F8C:
    tmp = *(u8*)((u8*)r3 + 0x29);
    if (tmp != 0) {
        r4 = 0x0;
        goto L_80011FC0;
    }
    r4 = 0x1;
    goto L_80011FC0;
L_80011FA8:
    tmp = *(u8*)((u8*)r3 + 0x29);
    if (tmp != 0) {
        r4 = 0x1;
        goto L_80011FC0;
    }
    r4 = 0x0;
L_80011FC0:
do {
    if ((s32)r6 < 0x534) {
        if ((s32)r6 == 0xa4) goto L_80011FFC;
        if ((s32)r6 >= 0xa4) break;
        if ((s32)r6 == 0x99) goto L_80011FFC;
        break;
    }
    if ((s32)r6 < 0x53c) {
        if ((s32)r6 >= 0x538) break;
        goto L_80011FFC;
    }
    if ((s32)r6 >= 0x540) break;
L_80011FFC:
    if (r5 == 1) {
        r4 = 0x0;
    }
} while (0);
    r3 = r28;
    ((void(*)(void))fn_80109220)();
    r3 = r28;
    fn_801091F4();
    tmp = r3 & 0xFF;
    if (tmp == 0) return;
    tmp = *(s16*)((u8*)r28 + 0x6);
    r3 = -0x100;
    r8 = *(u8*)((u8*)r27 + 0x8B);
    r29 = r8 | r3;
    if ((s32)tmp != 0xae) {
        if ((s32)tmp < 0xae) {
            if ((s32)tmp == 0xa3) goto L_8001230C;
            if ((s32)tmp < 0xa3) {
                if ((s32)tmp < 0x9f) {
                    if ((s32)tmp == 0x9a) goto L_8001215C;
                    if ((s32)tmp < 0x9a) return;
                    if ((s32)tmp >= 0x9d) goto L_80012110;
                    return;
                }
                if ((s32)tmp == 0xa1) goto L_80012170;
                if ((s32)tmp >= 0xa1) goto L_800122C8;
                return;
            }
            if ((s32)tmp < 0xaa) {
                if ((s32)tmp == 0xa5) goto L_8001215C;
                if ((s32)tmp < 0xa5) return;
                if ((s32)tmp >= 0xa8) goto L_80012110;
                return;
            }
            if ((s32)tmp == 0xac) goto L_80012170;
            if ((s32)tmp >= 0xac) goto L_800122C8;
            return;
        }
        if ((s32)tmp == 0x53a) goto L_800123A4;
        if ((s32)tmp < 0x53a) {
            if ((s32)tmp == 0x535) goto L_80012680;
            if ((s32)tmp < 0x535) {
                if ((s32)tmp == 0x533) goto L_800123CC;
                if ((s32)tmp >= 0x533) return;
                if ((s32)tmp >= 0x532) goto L_800123A4;
                return;
            }
            if ((s32)tmp == 0x537) goto L_80012758;
            return;
        }
        if ((s32)tmp == 0x53e) return;
        if ((s32)tmp < 0x53e) {
            if ((s32)tmp == 0x53c) return;
            if ((s32)tmp >= 0x53c) goto L_80012680;
            goto L_800123CC;
        }
        if ((s32)tmp >= 0x540) return;
        goto L_80012758;
    L_80012110:
        tmp = *(u8*)((u8*)r30 + 0x29);
        if (tmp != 2) return;
        r5 = *(u16*)((u8*)r31 + 0x6);
        tmp = 0x43300000;
        *(u32*)(sp + 0x8) = tmp;
        r3 = r27;
        f2 = *(f64*)lbl_8047B730;
        r4 = r28;
        f0 = *(f32*)lbl_8047B720;
        f1 = f1 - f2;
        f1 = f1 / f0;
        fn_8001DACC();
        r3 = r28;
        r4 = 0x0;
        ((void(*)(void))fn_80109220)();
        return;
    L_8001215C:
        r5 = *(u16*)((u8*)r30 + 0x26);
        r3 = r27;
        r4 = r28;
        fn_8010B9E8();
        return;
    L_80012170:
    do {
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x32;
        r6 = 0x0;
        fn_801F54A4();
        if ((s32)r3 != 0) {
            tmp = *(u8*)((u8*)r30 + 0x16);
            if (tmp != 0) return;
        }
        r3 = *(s16*)((u8*)r31 + 0x2);
        if ((s32)r3 != 0) {
            tmp = *(s16*)((u8*)r31 + 0x4);
            r5 = 0x43300000;
            r6 = *(s16*)((u8*)r31 + 0x0);
            r3 = *(s16*)((u8*)r30 + 0x1A);
            *(u32*)(sp + 0xC) = tmp;
            r3 = r3 - r6;
            f3 = *(f64*)lbl_8047B738;
            f1 = f0 - f3;
            f0 = f0 - f3;
            f2 = f1 / f0;
            *(u32*)(sp + 0x24) = tmp;
            f1 = f1 - f3;
            f0 = f0 - f3;
            f1 = f2 * f1 + f0;
            f0 = (f64)(s32)f1;
            tmp = (s16)r27;
            if ((s32)tmp != 0) break;
            f0 = *(f32*)lbl_8047B718;
            if (f1 <= f0) break;
            r27 = 0x1;
            break;
        }
        r27 = *(s16*)((u8*)r30 + 0x1A);
    } while (0);
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
        tmp = (u32)r3 >> 16;
        r5 = r29;
        tmp = (s16)tmp;
        r4 = -0x1;
        tmp = 0x18 - tmp;
        r6 = 0xcb;
        r3 = (s16)tmp;
        ((void(*)(void))fn_800FB680)();
        r4 = *(s16*)((u8*)r30 + 0x18);
        r3 = 0x34;
        ((void(*)(void))fn_80132A38)();
        r3 = 0xcb;
        ((void(*)(void))fn_800FA444)();
        r3 = (u32)r3 >> 16;
        tmp = *(s16*)((u8*)r28 + 0x54);
        r3 = (s16)r3;
        r5 = r29;
        tmp = tmp - r3;
        r4 = -0x1;
        r3 = (s16)tmp;
        r6 = 0xcb;
        ((void(*)(void))fn_800FB680)();
        return;
    L_800122C8:
        r4 = *(u8*)((u8*)r30 + 0x17);
        r3 = 0x34;
        ((void(*)(void))fn_80132A38)();
        r3 = 0xcb;
        ((void(*)(void))fn_800FA444)();
        r3 = (u32)r3 >> 16;
        tmp = *(s16*)((u8*)r28 + 0x54);
        r3 = (s16)r3;
        r5 = *(u8*)((u8*)r27 + 0x8B);
        r3 = tmp - r3;
        tmp = -0x100;
        r3 = (s16)r3;
        r4 = -0x1;
        r5 = r5 | tmp;
        r6 = 0xcb;
        ((void(*)(void))fn_800FB680)();
        return;
    }
L_8001230C:
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
    tmp = *(u8*)((u8*)r30 + 0x28);
    r3 = (u32)r3 >> 16;
    r27 = (s16)r3;
    do {
        if ((s32)tmp != 1) {
            if ((s32)tmp < 1) {
                if ((s32)tmp < 0) {
                    goto L_80012370;
                }
                goto L_80012370;
                }
            r3 = 0xd67;
            break;
        }
        r3 = 0xd68;
        break;
    L_80012370:
        r3 = 0x0;
    } while (0);

    if (r3 == 0) return;
    fn_800FA280();
    r4 = r3;
    r3 = 0x37;
    ((void(*)(void))fn_80132A38)();
    r3 = r27;
    r5 = r29;
    r4 = -0x1;
    r6 = 0xd0;
    ((void(*)(void))fn_800FB680)();
    return;
L_800123A4:
    r9 = *(u16*)((u8*)r30 + 0x24);
    r7 = r29;
    r5 = *(s16*)((u8*)r28 + 0x54);
    r8 = r27;
    r6 = *(s16*)((u8*)r28 + 0x56);
    r3 = 0x0;
    r4 = 0x0;
    r10 = 0x0;
    fn_80104160();
    return;
L_800123CC:
    r6 = *(s16*)((u8*)r31 + 0x2);
    if ((s32)r6 != 0) {
        tmp = *(s16*)((u8*)r31 + 0x4);
        r5 = 0x43300000;
        r7 = *(s16*)((u8*)r31 + 0x0);
        r3 = *(s16*)((u8*)r30 + 0x1A);
        *(u32*)(sp + 0x2C) = tmp;
        r3 = r3 - r7;
        f3 = *(f64*)lbl_8047B738;
        f1 = f0 - f3;
        f0 = f0 - f3;
        f2 = f1 / f0;
        *(u32*)(sp + 0x14) = tmp;
        f1 = f1 - f3;
        f0 = f0 - f3;
        f31 = f2 * f1 + f0;
    } else {

        r3 = *(s16*)((u8*)r30 + 0x1A);
        tmp = 0x43300000;
        *(u32*)(sp + 0x8) = tmp;
        f1 = *(f64*)lbl_8047B738;
        *(u32*)(sp + 0xC) = tmp;
        f31 = f0 - f1;
    }
    tmp = *(s16*)((u8*)r30 + 0x18);
    r3 = 0x43300000;
    f3 = *(f64*)lbl_8047B738;
    *(u32*)(sp + 0x34) = tmp;
    f30 = f0 - f3;
    if ((s32)r6 != 0) {
        r4 = *(s16*)((u8*)r31 + 0x0);
        do {
            if ((s32)r4 <= 0) {
                r31 = 0x0;
                break;
            }
            f0 = *(f32*)lbl_8047B724;
            f1 = *(f32*)lbl_8047B71C;
            f0 = f0 * f30;
            *(u32*)(sp + 0x34) = tmp;
            f0 = f0 / f1;
            f2 = f2 - f3;
            /* cror eq, lt, eq */;
            if (f2 == f0) {
                r31 = 0x80000000;
                break;
            }
            f0 = *(f32*)lbl_8047B728;
            *(u32*)(sp + 0x34) = tmp;
            f0 = f0 * f30;
            f0 = f0 / f1;
            f1 = f1 - f3;
            /* cror eq, lt, eq */;
            if (f1 == f0) {
                r31 = 0x64640000;
                break;
            }
            r31 = 0x800000;
        } while (0);

        r3 = *(s16*)((u8*)r28 + 0x54);
        tmp = 0x43300000;
        f0 = *(f32*)lbl_8047B72C;
        r31 = r31 | r8;
        r4 = r4 * r3;
        *(u32*)(sp + 0x30) = tmp;
        f1 = *(f64*)lbl_8047B738;
        f2 = f30 - f0;
        r3 = 0x1;
        *(u32*)(sp + 0x34) = tmp;
        f0 = f0 - f1;
        f0 = f2 + f0;
        f0 = f0 / f30;
        f0 = (f64)(s32)f0;
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
    }
    f0 = *(f32*)lbl_8047B718;
    /* cror eq, lt, eq */;
    do {
        if (f31 == f0) {
            r9 = 0x0;
            break;
        }
        f0 = *(f32*)lbl_8047B724;
        f1 = *(f32*)lbl_8047B71C;
        f0 = f0 * f30;
        f0 = f0 / f1;
        /* cror eq, lt, eq */;
        if (f31 == f0) {
            r9 = 0x1ad;
            break;
        }
        f0 = *(f32*)lbl_8047B728;
        f0 = f0 * f30;
        f0 = f0 / f1;
        /* cror eq, lt, eq */;
        if (f31 == f0) {
            r9 = 0x1b0;
            break;
        }
        r9 = 0x1b1;
    } while (0);

    r4 = *(s16*)((u8*)r28 + 0x54);
    r3 = 0x43300000;
    f0 = *(f32*)lbl_8047B72C;
    tmp = r9 & 0xFFFF;
    f2 = *(f64*)lbl_8047B738;
    f0 = f30 - f0;
    f1 = f1 - f2;
    f0 = f31 * f1 + f0;
    f0 = f0 / f30;
    f0 = (f64)(s32)f0;
    if (tmp == 0) return;
    r6 = *(s16*)((u8*)r28 + 0x56);
    r7 = r29;
    r8 = r27;
    r3 = 0x0;
    r4 = 0x0;
    r10 = 0x0;
    fn_80104160();
    return;
L_80012680:
    r3 = *(s16*)((u8*)r31 + 0xC);
    if ((s32)r3 != 0) {
        tmp = *(s16*)((u8*)r31 + 0xE);
        r4 = 0x43300000;
        r6 = *(u32*)((u8*)r31 + 0x8);
        tmp = *(u32*)((u8*)r30 + 0x20);
        tmp = tmp - r6;
        f3 = *(f64*)lbl_8047B738;
        f2 = *(f64*)lbl_8047B730;
        f1 = f0 - f3;
        *(u32*)(sp + 0x24) = tmp;
        f0 = f0 - f3;
        f3 = f1 / f0;
        f1 = f1 - f2;
        f0 = f0 - f2;
        f1 = f3 * f1 + f0;
        fn_800C46B0();
    } else {

        r3 = *(u32*)((u8*)r30 + 0x20);
    }
    r11 = *(u32*)((u8*)r30 + 0x1C);
    if (r3 > r11) {
        r3 = r11;
    }
    if (r11 == 0) return;
    tmp = *(s16*)((u8*)r28 + 0x54);
    r7 = r29;
    r6 = *(s16*)((u8*)r28 + 0x56);
    r8 = r27;
    tmp = r3 * tmp;
    r3 = 0x0;
    r4 = 0x0;
    r9 = 0x1ac;
    r10 = 0x0;
    r5 = r11 + tmp;
    tmp = (u32)tmp / (u32)r11;
    r5 = (s16)tmp;
    fn_80104160();
    return;
L_80012758:
    tmp = *(s16*)((u8*)r31 + 0xC);
    if ((s32)tmp != 0) {
        r4 = *(s16*)((u8*)r31 + 0xE);
        r3 = 0x43300000;
        f3 = *(f64*)lbl_8047B738;
        f4 = *(f32*)((u8*)r31 + 0x8);
        *(u32*)(sp + 0x2C) = tmp;
        f2 = f0 - f3;
        f0 = *(f32*)((u8*)r30 + 0x20);
        f0 = f0 - f4;
        f1 = f1 - f3;
        f1 = f2 / f1;
        f2 = f1 * f0 + f4;
    } else {

        f2 = *(f32*)((u8*)r30 + 0x20);
    }
    f3 = *(f32*)((u8*)r30 + 0x1C);
    if (f2 > f3) {
        f2 = f3;
    }
    r3 = *(s16*)((u8*)r28 + 0x54);
    tmp = 0x43300000;
    f0 = *(f32*)lbl_8047B718;
    *(u32*)(sp + 0x20) = tmp;
    f1 = *(f64*)lbl_8047B738;
    f0 = f0 - f1;
    f0 = f2 * f0;
    f0 = f0 / f3;
    f0 = (f64)(s32)f0;
    if (f2 > f0) {
        tmp = (s16)r5;
        if ((s32)tmp == 0) {
            r5 = 0x1;
    }
    }
    r6 = *(s16*)((u8*)r28 + 0x56);
    r7 = r29;
    r8 = r27;
    r3 = 0x0;
    r4 = 0x0;
    r9 = 0x1ab;
    r10 = 0x0;
    fn_80104160();

    return;
}
