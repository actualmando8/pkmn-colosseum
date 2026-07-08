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
 *   menuFightCtrlSecretPokemonTop  GSnpc_GetQuestState         -- 0x4C bytes
 *   fn_8000F35C  GSnpc_SetQuestState         -- 0xA4 bytes
 *   fn_8000F400  GSnpc_HealParty             -- 0x368 bytes, Pokemon Center heal
 *   fn_8000F768  GSnpc_FadeTransition        -- 0x1FC bytes, screen fade during dialog
 *   fn_8000F964  GSnpc_PokemonTrade          -- 0x474 bytes, in-game trade event
 *   menuFightCtrlSecretWazaTop  GSnpc_MoveTutor             -- 0x60 bytes, move tutor intro
 *   fn_8000FE38  GSnpc_MoveTutorTeach        -- 0x118 bytes, teach move
 *   menuFightCtrlSecretMain  GSnpc_NameRater             -- 0x58 bytes, name rater check
 *   fn_8000FFA8  GSnpc_NameRaterRename       -- 0x118 bytes, rename Pokemon
 *   menuFightCtrlBall  GSnpc_DaycareDeposit        -- 0x68 bytes, daycare deposit
 *   _menuFightIsUse__FP16MENU_WAZA_STATUSUs  GSnpc_DaycareWithdraw       -- 0x16C bytes, daycare withdraw
 *   fn_80010294  GSnpc_PurificationChamber   -- 0x1E8 bytes, purification setup
 *   fn_8001047C  GSnpc_ShadowGaugeCheck      -- 0x10C bytes, check purification ready
 *   fn_80010588  GSnpc_PurifyPokemon         -- 0x11C bytes, purify Shadow Pokemon
 *   fn_800106A4  GSnpc_SnagMachineSetup      -- 0x1A0 bytes, snag machine interaction
 *   fn_80010844  GSnpc_GBALinkPrompt         -- 0x15C bytes, GBA link cable prompt
 *   fn_800109A0  GSnpc_ColosseumSignup       -- 0x190 bytes, colosseum registration
 *   fn_80010B30  GSnpc_MtBattleEntry         -- 0x168 bytes, Mt. Battle entry
 *   menuPokemonCheckPokemonChange  GSnpc_WarpToLocation        -- 0x52C bytes, warp/teleport handler
 *
 * The dialog state machine (fn_8000D3AC) has 5 states:
 *   State 0: Init -- load NPC dialog data, set up text box
 *   State 1: Advance -- wait for player input to advance text
 *   State 2: Choice -- display yes/no or multi-choice prompt
 *   State 3: Close -- animate text box closing
 *   State 4: Cleanup -- restore camera, free resources
 *
 * fn_8000D298 (GSnpc_InitDialog) sets up the NPC sprite for dialog:
 *   - Calls winSpriteSetDisp to set NPC facing direction
 *   - Gets the NPC data from menuItemBiosGetPtr (lookup by ID)
 *   - Sets up the text viewport via fn_801040F0
 *   - If the NPC has a special marker (offset +0x4C), renders it
 *     using GSmsgGetRect/fn_800FB680 for the dialog portrait
 *
 * fn_8000E290 (GSnpc_TrainerBattle) is a key bridge function:
 *   - Gets the overworld data for the NPC
 *   - Looks up the trainer ID
 *   - Dispatches a battle event via pokemonGetStatus
 *   - On event completion, validates the battle result
 *   - Uses a jump table (jumptable_802E4BB8) to handle different
 *     battle outcome types (win, lose, flee, draw)
 *
 * menuPokemonCheckPokemonChange (GSnpc_WarpToLocation) handles map transitions:
 *   - Checks warp destination validity via fn_801F2020
 *   - Gets the destination map name via fightOutPokemonGetNicknamePtr
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

/* renamed symbols referenced by asm incs (symbolmap port) */
extern u32 _menuFightIsUse__FP16MENU_WAZA_STATUSUs();
extern void* menuSubCalcColor(void*, void*);

/* NPC/People system */
extern void  winSpriteSetDisp(void* npc, s32 direction);  /* Set NPC facing */
extern void* menuItemBiosGetPtr(s16 npcId);                  /* Lookup NPC data by ID */

/* Text/dialog system */
extern void  fn_801040F0();
extern void  fn_801080CC(void* ctx, s32 state);       /* Set dialog state */
extern void  fn_801081F8(void* ctx, s32 msgId, s32 flags); /* Display message */

/* Rendering */
extern u32   GSmsgGetRect();                           /* Get model dimensions */
extern void  fn_800FB680();

/* Map/warp */
extern u8    fn_801F2020(s32 p1, void* warpId, void* outDest);
extern void* fightOutPokemonGetNicknamePtr(void* mapData);              /* Get map name string */
extern void  fn_80106D3C(s32 slot, s32 msgId, s32 p3, s32 p4);
extern void  fn_801069FC(s32 slot);                   /* Close message box */

/* Input/frame */
extern u8    fn_801F18DC(s32 controller);             /* Check input ready */
extern u8    fn_801F1700(s32 controller);             /* Check button pressed */
extern u8    fn_80265924(void);                       /* Check A button */
extern u16   fn_801EF634(void);                       /* Get input state */
extern void  _threadSwitch(void);                       /* Frame advance */
extern u32   fn_800F7AF0(s32 slot);                   /* Get render flags */
extern u32   fn_800F7BC4(s32 slot);                   /* Get VSync flags */

/* Battle bridge */
extern void  fn_80132A38();                           /* Set battle parameter */
extern void  fn_801F4C14();                           /* Configure map object */
extern u32   windowGetParam();                           /* Get participant data */

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
 * then hands off to the battle system via pokemonGetStatus.
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
extern void fn_80089F78(u32, u32, u32, u32);
#if 0
asm void fn_800111C4(void) {
#include "src/game/gs_npc_interact_fn_800111C4.inc"
}
#else
#pragma peephole off
void fn_800111C4(u32 a, u32 b, u32 c) { fn_80089F78(a, b, c, 0); }
#pragma peephole on
#endif

/* 0x800111E8 | 0x24 -- small accessor */
#if 0
asm void fn_800111E8(void) {
#include "src/game/gs_npc_interact_fn_800111E8.inc"
}
#else
#pragma peephole off
void fn_800111E8(u32 a, u32 b, u32 c) { fn_80089F78(a, b, c, 1); }
#pragma peephole on
#endif

/* 0x7C | fn_8001120C | nullcheck_call_flag */
#pragma peephole off
u32 fn_8001120C(void* obj) {
    extern void fn_80102568();
    if ((u8)fn_80102620(0xff) != 0) fn_80102568(0xff, 0, obj);
    if ((u8)fn_80102620(0x104) != 0) fn_80102568(0x104, 0, obj);
    fn_80102620(0x100);
    return 0;
}
#pragma peephole on

/* 0x80011288 | 0x21C */
extern void menuItemBiosSetSelectFlag();
extern void menuGetCursorFromItemID();
extern s32 fn_801026A4(s32, ...);
#if 0
asm void fn_80011288(void) {
#include "src/game/gs_npc_interact_fn_80011288.inc"
}
#else
void fn_80011288(void) {
    extern void menuItemBiosSetSelectFlag();
    extern void menuGetCursorFromItemID();
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
        menuItemBiosSetSelectFlag();
        r3 = 0x1259;
        r4 = 0x0;
        menuItemBiosSetSelectFlag();
        r3 = 0x125a;
        r4 = 0x0;
        menuItemBiosSetSelectFlag();
        r3 = 0x125b;
        r4 = 0x0;
        menuItemBiosSetSelectFlag();
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
                menuItemBiosSetSelectFlag();
            }
            r31 = r31 + 0x8;
            r30 = r30 + 0x1;
        } while ((s32)r30 < 4);
        r3 = 0xff;
        r4 = 0x125a;
        menuGetCursorFromItemID();
        if ((s32)r3 == (s32)-0x1) {
            r3 = 0xff;
            r4 = 0x125b;
            menuGetCursorFromItemID();
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
#endif

/* 0x800114A4 | 0x25C */
extern void fightTypeDataBiosGetPtr();
extern void fightTypeDataBiosGetFightoutPokemonNum();
extern void menuPokemonOpenFight();
extern void fn_80102568();
extern void fn_801F02AC();
extern void fightOutPokemonCheckFightOut();
extern void fn_802656AC();
extern u32 menuPokemonCheckPokemonChange();
extern void fn_80011288();
extern s32 fn_801026A4(s32, ...);
#if 0
asm void fn_800114A4(void) {
#include "src/game/gs_npc_interact_fn_800114A4.inc"
}
#else
void fn_800114A4(void) {
    extern u32 menuPokemonCheckPokemonChange();
    extern void fn_80011288();
    extern void menuPokemonOpenFight();
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_801026A4();
    extern void fn_801F02AC();
    extern void fightOutPokemonCheckFightOut();
    extern void fightTypeDataBiosGetFightoutPokemonNum();
    extern void fightTypeDataBiosGetPtr();
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
    fightTypeDataBiosGetPtr();
    fightTypeDataBiosGetFightoutPokemonNum();
    tmp = r28 & 0xFF;
    r31 = r3;
    if (tmp == 0) {
        r4 = r27;
        r5 = r24;
        r6 = r25;
        menuPokemonOpenFight();
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
    menuPokemonCheckPokemonChange();
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
        fightOutPokemonCheckFightOut();
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
        fightOutPokemonCheckFightOut();
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
        fightOutPokemonCheckFightOut();
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
#endif

/* 0x80011700 | 0xBC — clear 4 event flags referenced by input arg */
#if 0
asm void menuFightCloseWaza(void) {
#include "src/game/gs_npc_interact_menuFightCloseWaza.inc"
}
#else
#pragma push
#pragma peephole off
s32 menuFightCloseWaza(s32 arg) {
    if ((u8)fn_80102620(0x4c) != 0) fn_80102568(0x4c, 0, arg);
    if ((u8)fn_80102620(0xf9) != 0) fn_80102568(0xf9, 0, arg);
    if ((u8)fn_80102620(0xfa) != 0) fn_80102568(0xfa, 0, arg);
    if ((u8)fn_80102620(0xf7) != 0) fn_80102568(0xf7, 0, arg);
    return 0;
}
#pragma pop
#endif

/* 0x800117BC | 0x1EC */
extern u32 _menuFightIsUse__FP16MENU_WAZA_STATUSUs();
extern void fn_80106394();
extern void fn_80106080();
#if 0
asm void fn_800117BC(void) {
#include "src/game/gs_npc_interact_fn_800117BC.inc"
}
#else
void fn_800117BC(void) {
    extern u32 _menuFightIsUse__FP16MENU_WAZA_STATUSUs();
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

    } else {
        r31 = 0xf7;
    }
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
        _menuFightIsUse__FP16MENU_WAZA_STATUSUs();
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
            ((void(*)(void))_threadSwitch)();

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
#endif

/* 0x74 | fn_800119A8 | nullcheck_call_flag */
#pragma peephole off
#pragma peephole off
u32 fn_800119A8(void* obj) {
    if ((u8)fn_80102620(0x4b) != 0) fn_80102568(0x4b, 0, obj);
    if ((u8)fn_80102620(0xf6) != 0) fn_80102568(0xf6, 0, obj);
    return 0;
}
#pragma peephole on
#pragma peephole on

/* 0x80011A1C | 0x130 */
#if 0
asm void fn_80011A1C(void) {
#include "src/game/gs_npc_interact_fn_80011A1C.inc"
}
#else
#pragma push
#pragma peephole off
s32 fn_80011A1C(u8* obj, s32 a1, s32 a2) {
    u32 sp8;
    s32 ret;    /* r30 */
    u8 locked;  /* r29 */
    s32 ret2;   /* r31 */

    locked = obj[0x16];
    do {
        if (obj[0x17] != 0) {
            ret = fn_801026A4(0xF6, 0, a1, 0, a2, 1, obj);
        } else {
            ret = fn_801026A4(0x4B, 0, a1, 0, a2, 1, obj);
        }
        if (locked != 0) break;
        if (ret != 3) break;
        sp8 = 0;
        if ((u8)fn_80102620(0x4B) != 0) {
            fn_80102568(0x4B, 0, 1);
        }
        if ((u8)fn_80102620(0xF6) != 0) {
            fn_80102568(0xF6, 0, 1);
        }
        ret2 = fn_801026A4(0xFB, 0, &sp8, 0, 1, 0);
        fn_80102568(0xFB, 0, 1);
    } while (ret2 != 0x1207);

    return ret;
}
#pragma pop
#endif

/* 0x78 | fn_80011B4C | generic */
extern u32 windowSearchID();
extern u8* windowGetFreeWork();
#pragma peephole off
#pragma peephole off
u32 fn_80011B4C(u32 arg1, u8 arg2) {
    u32 r;
    while (1) {
        if (!(r = windowSearchID(arg1))) return 0;
        if (*(s16*)((u8*)windowGetFreeWork(r) + 0xc) == 0) return 0;
        if (arg2 != 0) {
            _threadSwitch();
        } else {
            return 1;
        }
    }
}
#pragma peephole on
#pragma peephole on

/* 0x80011BC4 | 0xB4 */
extern u8* windowGetAllocPtr();
extern void fn_80166A28(u32 val);
#if 0
asm void fn_80011BC4(void) {
#include "src/game/gs_npc_interact_fn_80011BC4.inc"
}
#else
#pragma peephole off
#pragma peephole off
#pragma peephole off
void fn_80011BC4(u32 arg1, u32 target) {
    u32 ptr;
    u32 state;
    u32 data;
    u32 diff;
    s16 score;
    ptr = windowSearchID(arg1);
    if (!ptr) { return; }
    state = (u32)windowGetAllocPtr(ptr);
    data = (u32)windowGetFreeWork(ptr);
    if (target > *(u32*)(state + 0x20)) {
        diff = target - *(u32*)(state + 0x20);
    } else {
        diff = *(u32*)(state + 0x20) - target;
    }
    score = diff * 100 / *(u32*)(state + 0x1c);
    *(s16*)(data + 0xc) = score;
    if (*(s16*)(data + 0xc) < 0xf) { *(s16*)(data + 0xc) = 0xf; }
    *(u32*)(data + 0x8) = *(u32*)(state + 0x20);
    *(u32*)(state + 0x20) = target;
    *(s16*)(data + 0xe) = 0;
    fn_80166A28(0x4d0);
}
#pragma peephole on
#pragma peephole on
#pragma peephole on
#endif

/* 0x78 | fn_80011C78 | generic */
#pragma peephole off
#pragma peephole off
u32 fn_80011C78(u32 arg1, u8 arg2) {
    u32 r;
    while (1) {
        if (!(r = windowSearchID(arg1))) return 0;
        if (*(s16*)((u8*)windowGetFreeWork(r) + 0x2) == 0) return 0;
        if (arg2 != 0) {
            _threadSwitch();
        } else {
            return 1;
        }
    }
}
#pragma peephole on
#pragma peephole on

/* 0x80011CF0 | 0xAC */
#if 0
asm void menuFightStatusStartAnimHP(void) {
#include "src/game/gs_npc_interact_fn_80011CF0.inc"
}
#else
#pragma peephole off
void menuFightStatusStartAnimHP(u32 arg1, s16 target) {
    u32 ptr;
    u32 state;
    u32 data;
    s32 diff;

    ptr = windowSearchID(arg1);
    if (ptr == 0) {
        return;
    }
    state = (u32)windowGetAllocPtr(ptr);
    data = (u32)windowGetFreeWork(ptr);
    diff = *(s16*)(state + 0x1a) - (s16)target;
    if (diff < 0) {
        diff = -diff;
    }
    *(s16*)(data + 2) = (s16)((diff * 100) / *(s16*)(state + 0x18));
    if (*(s16*)(data + 2) <= 0) {
        *(s16*)(data + 2) = 1;
    }
    *(s16*)(data + 0) = *(s16*)(state + 0x1a);
    *(s16*)(state + 0x1a) = target;
    *(s16*)(data + 4) = 0;
}
#pragma peephole on
#endif

/* 0x80011D9C | 0xCC — item-kind resolver + dispatch (same switch as fn_800129A8) */
#if 0
asm void fn_80011D9C(void) {
#include "src/game/gs_npc_interact_fn_80011D9C.inc"
}
#else
#pragma peephole off
#pragma peephole off
void fn_80011D9C(s32 id, s32 do_extra) {
    u32 resolved;
    s32 kind;
    kind = 0;
    resolved = windowSearchID(id);
    if (resolved == 0) return;
    switch (id) {
    case 0x45: case 0x46: case 0x49:
        kind = 0x538;
        break;
    case 0x47: case 0x48: case 0x4a:
        kind = 0x540;
        break;
    }
    if (do_extra != 0) {
        fn_80103F74(resolved, kind, 1);
        fn_801081F8((void*)resolved, kind, 0x2d);
    } else {
        fn_80103F74(resolved, kind, 0);
    }
}
#pragma peephole on
#pragma peephole on
#endif

/* 0x80011E68 | 0x3C */
/* Set an NPC's facing direction. */
void menuFightStatusSetHP(u32 npcId, u16 direction) {
    extern void* windowGetAllocPtr(void* obj);
    extern void* windowSearchID(u32 npcId);
    void* npc;

    npc = windowSearchID(npcId);
    if (npc != NULL) {
        u8* obj = (u8*)windowGetAllocPtr(npc);
        *(u16*)(obj + 0x1A) = direction;
    }
}

/* 0x80011EA4 | 0x9B4 -- GSnpc_WarpToLocation continued */
extern void winSpriteGetDisp();
extern void fn_8001DACC();
extern void fn_8010B9E8();
extern void fn_801F54A4();
extern void fn_800FA280();
extern void fn_80104160();
extern void fn_800D88DC();
extern void fn_800D888C();
extern void fn_800D6A00();
extern void fn_800D7820();
extern void fn_800D67BC();
extern void fn_800D61E4();
extern void fn_800D5BA0();
extern void fn_800D6728();
extern void __cvt_fp2unsigned();
extern f64 lbl_8047B730;
extern f32 lbl_8047B720;
extern f64 lbl_8047B738;
extern f32 lbl_8047B718;
extern f32 lbl_8047B724;
extern f32 lbl_8047B71C;
extern f32 lbl_8047B728;
extern f32 lbl_8047B72C;
extern u8 lbl_80314E08[];
#if 0
asm void fn_80011EA4(void) {
#include "src/game/gs_npc_interact_fn_80011EA4.inc"
}
#else
void fn_80011EA4(void) {
    extern u8 lbl_80314E08[];
    extern f32 lbl_8047B718;
    extern f32 lbl_8047B71C;
    extern f32 lbl_8047B720;
    extern f32 lbl_8047B724;
    extern f32 lbl_8047B728;
    extern f32 lbl_8047B72C;
    extern f64 lbl_8047B730;
    extern f64 lbl_8047B738;
    extern void fn_8001DACC();
    extern void __cvt_fp2unsigned();
    extern void fn_800D5BA0();
    extern void fn_800D61E4();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    extern void fn_800FA280();
    extern void windowGetAllocPtr();
    extern void windowGetFreeWork();
    extern void fn_80104160();
    extern void winSpriteGetDisp();
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
    windowGetAllocPtr();
    tmp = r3;
    r3 = r27;
    r30 = tmp;
    windowGetFreeWork();
    r31 = r3;
    r3 = r27;
    windowGetAllocPtr();
    r6 = *(s16*)((u8*)r28 + 0x6);
    r4 = 0x1;
    r5 = *(u8*)((u8*)r3 + 0x16);
    if ((s32)r6 < 0xa8) {
        if ((s32)r6 < 0x9f) {
            if ((s32)r6 == 0x9a) goto L_80011FC0;
            if ((s32)r6 < 0x9a) {
                if ((s32)r6 < 0x99) {
                    goto L_80011FC0;
                }
                if ((s32)r6 < 0x9d) {
                    goto L_80011F8C;
                }
                if ((s32)r6 != 0xa5) {
                    if ((s32)r6 < 0xa5) {
            }
                }
                if ((s32)r6 < 0xa4) {
                }
                goto L_80011FC0;
            }
            if ((s32)r6 < 0x538) {
                if ((s32)r6 < 0x534) {
                    if ((s32)r6 >= 0xaa) goto L_80011FC0;
                }
                goto L_80011FA8;
            }
            if ((s32)r6 >= 0x536) goto L_80011FA8;
        }

    } else {
        if ((s32)r6 < 0x53e) {
            if ((s32)r6 >= 0x53c) goto L_80011F8C;
            goto L_80011FC0;
        }
        if ((s32)r6 >= 0x540) goto L_80011FC0;
        goto L_80011FA8;
    }
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

    } else {
        r4 = 0x0;
    }
L_80011FC0:
do {
    if ((s32)r6 < 0x534) {
        if ((s32)r6 != 0xa4) {
            if ((s32)r6 >= 0xa4) break;
            if ((s32)r6 != 0x99) {
                break;
            }
            if ((s32)r6 < 0x53c) {
                if ((s32)r6 >= 0x538) break;
        }
        }

    } else {
        if ((s32)r6 >= 0x540) break;
    }
    if (r5 == 1) {
        r4 = 0x0;
    }
} while (0);
    r3 = r28;
    ((void(*)(void))winSpriteSetDisp)();
    r3 = r28;
    winSpriteGetDisp();
    tmp = r3 & 0xFF;
    if (tmp == 0) return;
    tmp = *(s16*)((u8*)r28 + 0x6);
    r3 = -0x100;
    r8 = *(u8*)((u8*)r27 + 0x8B);
    r29 = r8 | r3;
    if ((s32)tmp != 0xae) {
        if ((s32)tmp < 0xae) {
            if ((s32)tmp != 0xa3) {
                if ((s32)tmp < 0xa3) {
                    if ((s32)tmp < 0x9f) {
                        if ((s32)tmp == 0x9a) goto L_8001215C;
                        if ((s32)tmp < 0x9a) return;
                        if ((s32)tmp < 0x9d) return;

                    }
                    if ((s32)tmp == 0xa1) goto L_80012170;
                    if ((s32)tmp < 0xa1) return;

                }
                if ((s32)tmp < 0xaa) {
                    if ((s32)tmp == 0xa5) goto L_8001215C;
                    if ((s32)tmp < 0xa5) return;
                    if ((s32)tmp < 0xa8) return;

                }
                if ((s32)tmp == 0xac) goto L_80012170;
                if ((s32)tmp < 0xac) return;

            }
            if ((s32)tmp == 0x53a) goto L_800123A4;
            if ((s32)tmp < 0x53a) {
                if ((s32)tmp != 0x535) {
                    if ((s32)tmp < 0x535) {
                        if ((s32)tmp == 0x533) goto L_800123CC;
                        if ((s32)tmp >= 0x533) return;
                        if ((s32)tmp < 0x532) return;

                    }
                    if ((s32)tmp != 0x537) return;

                }
                if ((s32)tmp == 0x53e) return;
                if ((s32)tmp < 0x53e) {
                    if ((s32)tmp == 0x53c) return;
                }
                if ((s32)tmp >= 0x53c) goto L_80012680;
                goto L_800123CC;
            }
            if ((s32)tmp >= 0x540) return;
            goto L_80012758;

            tmp = *(u8*)((u8*)r30 + 0x29);
            if (tmp != 2) return;
            r5 = *(u16*)((u8*)r31 + 0x6);
            tmp = 0x43300000;
            *(u32*)(sp + 0x8) = tmp;
            r3 = r27;
            f2 = lbl_8047B730;
            r4 = r28;
            f0 = lbl_8047B720;
            f1 = f1 - f2;
            f1 = f1 / f0;
            fn_8001DACC();
            r3 = r28;
            r4 = 0x0;
            ((void(*)(void))winSpriteSetDisp)();
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
                f3 = lbl_8047B738;
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
                f0 = lbl_8047B718;
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
            ((void(*)(void))GSmsgGetRect)();
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
            ((void(*)(void))GSmsgGetRect)();
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

            r4 = *(u8*)((u8*)r30 + 0x17);
            r3 = 0x34;
            ((void(*)(void))fn_80132A38)();
            r3 = 0xcb;
            ((void(*)(void))GSmsgGetRect)();
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
            }
    r4 = r30;
    r3 = 0x37;
    ((void(*)(void))fn_80132A38)();
    r5 = r29;
    r3 = 0x0;
    r4 = -0x1;
    r6 = 0xe9;
    ((void(*)(void))fn_800FB680)();
    r3 = 0xe9;
    ((void(*)(void))GSmsgGetRect)();
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
        f3 = lbl_8047B738;
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
        f1 = lbl_8047B738;
        *(u32*)(sp + 0xC) = tmp;
        f31 = f0 - f1;
    }
    tmp = *(s16*)((u8*)r30 + 0x18);
    r3 = 0x43300000;
    f3 = lbl_8047B738;
    *(u32*)(sp + 0x34) = tmp;
    f30 = f0 - f3;
    if ((s32)r6 != 0) {
        r4 = *(s16*)((u8*)r31 + 0x0);
        do {
            if ((s32)r4 <= 0) {
                r31 = 0x0;
                break;
            }
            f0 = lbl_8047B724;
            f1 = lbl_8047B71C;
            f0 = f0 * f30;
            *(u32*)(sp + 0x34) = tmp;
            f0 = f0 / f1;
            f2 = f2 - f3;
            /* cror eq, lt, eq */;
            if (f2 == f0) {
                r31 = 0x80000000;
                break;
            }
            f0 = lbl_8047B728;
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
        f0 = lbl_8047B72C;
        r31 = r31 | r8;
        r4 = r4 * r3;
        *(u32*)(sp + 0x30) = tmp;
        f1 = lbl_8047B738;
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
    f0 = lbl_8047B718;
    /* cror eq, lt, eq */;
    do {
        if (f31 == f0) {
            r9 = 0x0;
            break;
        }
        f0 = lbl_8047B724;
        f1 = lbl_8047B71C;
        f0 = f0 * f30;
        f0 = f0 / f1;
        /* cror eq, lt, eq */;
        if (f31 == f0) {
            r9 = 0x1ad;
            break;
        }
        f0 = lbl_8047B728;
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
    f0 = lbl_8047B72C;
    tmp = r9 & 0xFFFF;
    f2 = lbl_8047B738;
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
        f3 = lbl_8047B738;
        f2 = lbl_8047B730;
        f1 = f0 - f3;
        *(u32*)(sp + 0x24) = tmp;
        f0 = f0 - f3;
        f3 = f1 / f0;
        f1 = f1 - f2;
        f0 = f0 - f2;
        f1 = f3 * f1 + f0;
        __cvt_fp2unsigned();
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
        f3 = lbl_8047B738;
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
    f0 = lbl_8047B718;
    *(u32*)(sp + 0x20) = tmp;
    f1 = lbl_8047B738;
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
#endif

/* ===== Phase 2 recovery stubs ===== */

/* fn_8000D710 - 0x8000D710 | size: 0x398 */
extern s32 heroMoveCheckEvent();
extern void fn_80116D30(void);
extern void heroMoveInitEvent(void);
extern void mailMainReceiveTerminate(void);
extern void GSmodelAllPauseAnimation(void);
extern s32 fn_800D37CC();
extern void fn_8010206C(void);
extern u8 fn_80102014();
extern void fn_801661D0(void);
extern void fn_800D3074(void);
extern void* fn_801906A0();
extern u32 fn_800FF560();
extern void fn_80130CD8(void);
extern void GSgappBlock(void);
extern void GSthreadBlockGroup(void);
extern void GSthreadCreate(void);
extern void GSgappUnblock(void);
extern void fn_80102510();
extern void GSmodelAllUnpauseAnimation(void);
extern void menuPokemonOpen(void);
extern void menuPdaOpen(void);
extern u32 fn_80018F88();
extern u32 fn_80019070();
extern void heroMoveAddAutoEvent(void);
extern s32 fn_801CBAB8();
extern void fn_80109764(void);
extern void fn_801660D8(void);
extern void menuReleaseOffScreen(void);
extern void menuCloseSync(void);
extern void heroMoveTermEvent(void);
extern u8 lbl_8047A2A0;
extern u32 lbl_8047B6F8;
extern f32 lbl_8047B6F0;
extern u32 lbl_8047A2A4;
extern u32 lbl_8047A2A8;
extern u32 lbl_8047A2AC;
extern u32 lbl_8047A2B0;
u32 fn_8000DAB0(void); /* forward decl: referenced by fn_8000D710 asm inc */
#if 0
asm void fn_8000D710(void) {
#include "src/game/gs_npc_interact_fn_8000D710.inc"
}
#else
#pragma push
#pragma peephole off
u32 fn_8000D710(u32 mode) {
    extern s32 heroMoveCheckEvent(void* out);
    extern void fn_80116D30(s32 kind, u16 value);
    extern void heroMoveInitEvent(void);
    extern void mailMainReceiveTerminate(void);
    extern void GSmodelAllPauseAnimation(void);
    extern s32 fn_800D37CC(void);
    extern void fn_8010206C(f32 value);
    extern u8 fn_80102014(void);
    extern void fn_801661D0(s32, s32, s32, s32);
    extern void fn_800D3074(s32);
    extern void* fn_801906A0(s32);
    extern u32 fn_800FF560(void);
    extern void fn_80130CD8(void);
    extern void GSgappBlock(void);
    extern void GSthreadBlockGroup(u32);
    extern void GSthreadCreate(s32, void*, u32, s32, s32, void*);
    extern void GSgappUnblock(void);
    extern void fn_80102510(s32);
    extern void GSmodelAllUnpauseAnimation(void);
    extern void menuPokemonOpen(s32, s32, s32);
    extern void menuPdaOpen(void);
    extern u32 fn_80018F88(s32, void*, s32);
    extern u32 fn_80019070(u32);
    extern void heroMoveAddAutoEvent(u32, u32, u32, s32, s32);
    extern s32 fn_801CBAB8(void);
    extern void fn_80109764(void);
    extern void fn_801660D8(s32, s32, s32);
    extern void menuReleaseOffScreen(f32 value);
    extern void menuCloseSync(s32, s32);
    extern void heroMoveTermEvent(void);
    extern f32 lbl_8047B6F0;
    u8 stackData[0xD8];
    void* threadEntry;
    s32 count;
    s32 i;
    s32 dialogId;
    u32 result;
    u32 work;
    u32 tmp;

    count = heroMoveCheckEvent(stackData);
    for (i = 0; i < count; i++) {
        fn_80116D30(4, *(u16*)(stackData + 0x30 + (i * 0x34)));
    }

    heroMoveInitEvent();
    mailMainReceiveTerminate();
    _threadSwitch();
    GSmodelAllPauseAnimation();
    lbl_8047A2A0 = 1;
    tmp = fn_800D37CC();
    if (tmp != 0) {
        fn_8010206C(lbl_8047B6F0 / (f32)(s32)tmp);
    }
    while ((u8)fn_80102014() == 0) {
        _threadSwitch();
    }
    fn_801661D0(0x55, 0x1F4, 1, 1);
    fn_800D3074(1);
    dialogId = (fn_801906A0(0x8AE) == NULL) ? 0x41 : 0x109;
    result = 0;

    switch ((u8)mode) {
    case 0:
        threadEntry = (void*)fn_8000DAB0;
        do {
            fn_800D3074(1);
            lbl_8047A2A4 = dialogId;
            lbl_8047A2A8 = fn_800FF560();
            fn_80130CD8();
            GSgappBlock();
            GSthreadBlockGroup(lbl_8047A2A8);
            lbl_8047A2AC = lbl_8047A2A8 - 0x20;
            GSthreadCreate(0xF, (void*)lbl_8047A2AC, 0x4000, 1, 1, threadEntry);
            _threadSwitch();
            fn_80130CD8();
            GSgappUnblock();

            if (lbl_8047A2B0 == 0) {
                fn_80102510(dialogId);
                GSmodelAllUnpauseAnimation();
                menuPokemonOpen(1, 0, 0);
                GSmodelAllPauseAnimation();
            } else if (lbl_8047A2B0 == 1) {
                fn_80102510(dialogId);
                GSmodelAllUnpauseAnimation();
                menuPdaOpen();
                GSmodelAllPauseAnimation();
            } else if (lbl_8047A2B0 == 2) {
                fn_80102510(dialogId);
                GSmodelAllUnpauseAnimation();
                result = fn_80018F88(0, &work, 0);
                GSmodelAllPauseAnimation();
                if (((u16)result != 0) && (fn_80019070(result) != 0xFFFFFFFF)) {
                    heroMoveAddAutoEvent(fn_80019070(result), (u16)result, work, 0, 0);
                    break;
                }
            } else {
                break;
            }
        } while (1);
        break;
    case 1:
        result = fn_80018F88(0, &work, 0);
        if (((u16)result != 0) && (fn_80019070(result) != 0xFFFFFFFF)) {
            heroMoveAddAutoEvent(fn_80019070(result), (u16)result, work, 0, 0);
        }
        break;
    default:
        break;
    }

    fn_80019070(result);
    fn_801660D8(0x1F4, 1, 1);
    fn_80102510(dialogId);
    tmp = fn_800D37CC();
    if (tmp != 0) {
        menuReleaseOffScreen(lbl_8047B6F0 / (f32)(s32)tmp);
    }
    menuCloseSync(dialogId, 1);
    fn_800D3074(2);
    heroMoveTermEvent();
    GSmodelAllUnpauseAnimation();
    lbl_8047A2A0 = 0;
    return 0;
}
#pragma pop
#endif

/* fn_8000DAB0 - 0x8000DAB0 | size: 0x38 */
extern u32 menuOpen(u32, u32);
extern void GSthreadUnblockGroup(u32);
extern u32 lbl_8047A2A4;
extern u32 lbl_8047A2A8;
extern u32 lbl_8047A2B0;
#if 0
asm void fn_8000DAB0(void) {
#include "src/game/gs_npc_interact_fn_8000DAB0.inc"
}
#else
#pragma peephole off
u32 fn_8000DAB0(void) {
    lbl_8047A2B0 = menuOpen(lbl_8047A2A4, 1);
    GSthreadUnblockGroup(lbl_8047A2A8);
    return lbl_8047A2B0;
}
#pragma peephole on
#endif

/* fn_8000DAE8 - 0x8000DAE8 | size: 0x1a0 */
extern void* fn_8001D834(void*, void*);
extern void fn_800FBB34(void);
extern u16 lbl_802E4B98[];
extern u16 lbl_803A1B80[];
#if 0
asm void fn_8000DAE8(void) {
#include "src/game/gs_npc_interact_fn_8000DAE8.inc"
}
#else
#pragma push
#pragma peephole off
void fn_8000DAE8(u8* ctx, u8* npc) {
    extern s32 windowGetParam(u8* a, s32 b);
    extern void fn_80132A38(s32 a, s32 b);
    extern void fn_800FB680();
    extern void fn_800FBB34();
    s32 value;
    s32 hour;
    s32 minute;
    s32 digit;
    s16 npcId;

    npcId = *(s16*)(npc + 6);
    if (npcId == 0x12AD) {
        value = windowGetParam(ctx, 0);
        minute = value % 60;
        hour = value / 60;
        lbl_803A1B80[2] = 0x3A;
        lbl_803A1B80[5] = 0;
        digit = hour / 10;
        lbl_803A1B80[0] = lbl_802E4B98[digit];
        digit = hour - (digit * 10);
        lbl_803A1B80[1] = lbl_802E4B98[digit];
        digit = minute / 10;
        lbl_803A1B80[3] = lbl_802E4B98[digit];
        digit = minute - (digit * 10);
        lbl_803A1B80[4] = lbl_802E4B98[digit];
        fn_80132A38(0x37, value);
        fn_800FB680(0, 0, (s32)menuSubCalcColor(ctx, npc), 0xCF);
    } else if (npcId == 0x12AF) {
        value = windowGetParam(ctx, 0);
        fn_80132A38(0x34, value);
        fn_800FBB34(0, 0, *(s16*)(npc + 0x54), *(s16*)(npc + 0x56),
                    (s32)menuSubCalcColor(ctx, npc), 0xDE);
    }
}
#pragma pop
#endif

/* fn_8000DC88 - 0x8000DC88 | size: 0x84 */
extern void fn_80265B74(void);
extern void windowSetParam(void);
extern void fn_8026595C(void);
#if 0
asm void fn_8000DC88(void) {
#include "src/game/gs_npc_interact_fn_8000DC88.inc"
}
#else
#pragma peephole off
u32 fn_8000DC88(u8* ptr) {
    extern f64 fn_80265B74(void);
    extern f64 fn_8026595C(void);
    extern void windowSetParam(u8* a, u32 b, s32 c);
    switch (*(s32*)(ptr + 4)) {
        case 0x10a:
            windowSetParam(ptr, 0, (s32)fn_80265B74());
            break;
        case 0x10b:
            windowSetParam(ptr, 0, (s32)fn_8026595C());
            break;
    }
    return 0;
}
#pragma peephole on
#endif

/* fn_8000DD0C - 0x8000DD0C | size: 0x24 */
#if 0
asm void fn_8000DD0C(void) {
#include "src/game/gs_npc_interact_fn_8000DD0C.inc"
}
#else
#pragma peephole off
u32 fn_8000DD0C(void) { return fn_80102620(0x10a); }
#pragma peephole on
#endif

/* fn_8000DD30 - 0x8000DD30 | size: 0x2c */
#if 0
asm void fn_8000DD30(void) {
#include "src/game/gs_npc_interact_fn_8000DD30.inc"
}
#else
#pragma peephole off
void fn_8000DD30(void) { fn_80102568(0x10a, 0, 0); }
#pragma peephole on
#endif

/* fn_8000DD5C - 0x8000DD5C | size: 0x3c */
#if 0
asm void fn_8000DD5C(void) {
#include "src/game/gs_npc_interact_fn_8000DD5C.inc"
}
#else
#pragma peephole off
void fn_8000DD5C(void) { fn_801026A4(0x10a, -1, 0, 0, 0, 0); }
#pragma peephole on
#endif

/* fn_8000DD98 - 0x8000DD98 | size: 0x24 */
#if 0
asm void fn_8000DD98(void) {
#include "src/game/gs_npc_interact_fn_8000DD98.inc"
}
#else
#pragma peephole off
u32 fn_8000DD98(void) { return fn_80102620(0x10b); }
#pragma peephole on
#endif

/* fn_8000DDBC - 0x8000DDBC | size: 0x2c */
#if 0
asm void fn_8000DDBC(void) {
#include "src/game/gs_npc_interact_fn_8000DDBC.inc"
}
#else
#pragma peephole off
void fn_8000DDBC(void) { fn_80102568(0x10b, 0, 0); }
#pragma peephole on
#endif

/* fn_8000DDE8 - 0x8000DDE8 | size: 0x3c */
#if 0
asm void fn_8000DDE8(void) {
#include "src/game/gs_npc_interact_fn_8000DDE8.inc"
}
#else
#pragma peephole off
void fn_8000DDE8(void) { fn_801026A4(0x10b, -1, 0, 0, 0, 0); }
#pragma peephole on
#endif

/* fn_8000DE24 - 0x8000DE24 | size: 0xa0 */
extern void menuButtonNormal(void);
#if 0
asm void fn_8000DE24(void) {
#include "src/game/gs_npc_interact_fn_8000DE24.inc"
}
#else
#pragma push
#pragma peephole off
#pragma peephole off
#pragma peephole off
#pragma peephole off
void fn_8000DE24(u8* ptr) {
    extern void menuButtonNormal(u8* a);
    extern u8 fn_801F18DC(s32 a);
    extern u8 fn_801F1700(s32 a);
    extern u8 fn_80265924(void);
    extern u16 fn_801EF634(void);
    u8 flag;
    menuButtonNormal(ptr);
    if (!(u8)fn_801F18DC(0)) goto _zero;
    if ((u8)fn_801F1700(0) == 1 && (u8)fn_80265924() == 1) { flag = 1; goto _check; }
    if ((u16)fn_801EF634() == 1) { flag = 1; goto _check; }
    _zero:
    flag = 0;
    _check:
    if (flag) {
        ptr[0x98] = 1;
        ptr[0x99] = 1;
    }
}
#pragma peephole on
#pragma peephole on
#pragma peephole on
#pragma pop
#endif

/* fn_8000DEC4 - 0x8000DEC4 | size: 0x12c */
#if 0
asm void fn_8000DEC4(void) {
#include "src/game/gs_npc_interact_fn_8000DEC4.inc"
}
#else
#pragma push
#pragma peephole off
void fn_8000DEC4(u8* arg1, u8* arg2) {
    extern void* windowGetParam(u8* a, u32 b);
    extern void winSpriteSetDisp(u8* a, u32 b);
    u8* entry;
    s32 i;
    entry = (u8*)windowGetParam(arg1, 0);
    for (i = 0; i < 2; i++) {
        s32 value;
        s16 npc_id;
        switch (*(s32*)(entry + 4)) {
            case 0x45: value = 0x125C; break;
            case 0x46: value = 0x125E; break;
            case 0x47: value = 0x12BE; break;
            case 0x48: value = 0x125D; break;
            default: value = 0; break;
        }
        npc_id = *(s16*)(arg2 + 6);
        if (value == npc_id) {
            winSpriteSetDisp(arg2, 1);
            return;
        }
        switch (*(s32*)(entry + 0xC)) {
            case 0x45: value = 0x125C; break;
            case 0x46: value = 0x125E; break;
            case 0x47: value = 0x12BE; break;
            case 0x48: value = 0x125D; break;
            default: value = 0; break;
        }
        if (value == npc_id) {
            winSpriteSetDisp(arg2, 1);
            return;
        }
        entry += 0x10;
    }
    winSpriteSetDisp(arg2, 0);
}
#pragma pop
#endif

/* fn_8000DFF0 - 0x8000DFF0 | size: 0x214 */
extern u8* fn_80105624();
#if 0
asm void fn_8000DFF0(void) {
#include "src/game/gs_npc_interact_fn_8000DFF0.inc"
}
#else
#pragma push
#pragma peephole off
void fn_8000DFF0(u8* ctx) {
    extern u8* fn_80105624(void);
    extern u32 windowGetParam(u8* a, s32 b);
    u8* flags;
    u8* entry;
    u16 bits;
    s32 selected;
    s32 i;
    s32 mode;
    s32 field;
    u8 pressed;

    flags = fn_80105624();
    entry = (u8*)windowGetParam(ctx, 0);
    selected = -1;
    bits = *(u16*)(flags + 4);
    if (bits & 1) {
        selected = 0;
    } else if (bits & 8) {
        selected = 1;
    } else if (bits & 4) {
        selected = 2;
    } else if (bits & 0x400) {
        ctx[0x98] = 1;
        ctx[0x99] = 1;
        *(s32*)(ctx + 0x80) = -1;
    }

    if (selected >= 0) {
        for (i = 0; i < 2; i++, entry += 0x10) {
            field = *(s32*)(entry + 4);
            if (field == 0x45) {
                mode = 1;
            } else if (field == 0x46) {
                mode = 0;
            } else if (field == 0x47 || field == 0x48) {
                mode = 2;
            } else {
                mode = -1;
            }
            if (selected == mode) {
                ctx[0x98] = 1;
                *(s32*)(ctx + 0x80) = selected;
                return;
            }
            field = *(s32*)(entry + 0xC);
            if (field == 0x45) {
                mode = 1;
            } else if (field == 0x46) {
                mode = 0;
            } else if (field == 0x47 || field == 0x48) {
                mode = 2;
            } else {
                mode = -1;
            }
            if (selected == mode) {
                ctx[0x98] = 1;
                *(s32*)(ctx + 0x80) = selected;
                return;
            }
        }
    }

    pressed = 0;
    if ((u8)fn_801F18DC(0) != 0) {
        if (((u8)fn_801F1700(0) == 1) && ((u8)fn_80265924() == 1)) {
            pressed = 1;
        } else if ((u16)fn_801EF634() == 1) {
            pressed = 1;
        }
    }
    if (pressed != 0) {
        ctx[0x98] = 1;
        ctx[0x99] = 1;
    }
}
#pragma pop
#endif

/* fn_8000E204 - 0x8000E204 | size: 0x88 */
extern void menuItemBiosGetSelectFlag(void);
extern void fn_801022B8(void);
#if 0
asm void fn_8000E204(void) {
#include "src/game/gs_npc_interact_fn_8000E204.inc"
}
#else
#pragma push
#pragma peephole off
void fn_8000E204(u8* arg1, u8* arg2) {
    extern u32 menuItemBiosGetSelectFlag(s16 val);
    extern s32 fn_801022B8(u32 val);
    extern void winSpriteSetDisp(u8* a, u32 b);
    if ((u8)menuItemBiosGetSelectFlag(*(s16*)(arg2 + 6)) != 0) {
        if (*(s16*)(arg2 + 6) == fn_801022B8(*(u32*)(arg1 + 4))) {
            winSpriteSetDisp(arg2, 1);
        } else {
            winSpriteSetDisp(arg2, 0);
        }
    } else {
        winSpriteSetDisp(arg2, 0);
    }
}
#pragma pop
#endif

/* fn_8000E28C - 0x8000E28C | size: 0x4 */
#if 0
asm void fn_8000E28C(void) {
#include "src/game/gs_npc_interact_fn_8000E28C.inc"
}
#else
void fn_8000E28C(void) {}
#endif

/* fn_8000EA10 - 0x8000EA10 | size: 0x324 */
extern u32 fn_801F2A7C(s32 arg);
extern u32 fn_801F986C(u32 warpId, u16 variant);
extern u32 pokemonGetStatus();
extern u32 pokemonCheckValid();
extern u32 fn_8001D624();
extern void jumptable_802E4C20();
#if 0
asm void fn_8000EA10(void) {
#include "src/game/gs_npc_interact_fn_8000EA10.inc"
}
#else
#pragma push
#pragma peephole off
void fn_8000EA10(u8* ctx, u8* npc) {
    extern u32 windowGetParam(u8* a, s32 b);
    extern u32 pokemonGetStatus();
    extern u32 pokemonCheckValid(void);
    extern u32 fn_8001D624(u32, s32);
    extern void fn_800FBB34();
    extern void fn_800FB680();
    s32 idx;
    s32 slot;
    u32 handle;
    s32 color;
    s32 npcId;
    s32 y;
    s32 top;
    s32 delta;

    handle = windowGetParam(ctx, 0);
    npcId = *(s16*)(npc + 6);
    idx = npcId - 0x1215;
    slot = -1;
    if ((u32)idx <= 0x17) {
        slot = idx % 6;
    }
    if (slot < 0) return;
    if ((u32)slot >= 6) return;
    if (handle == 0) {
        handle = fn_801F2A7C(0);
    }
    if (handle == 0) return;
    handle = fn_801F986C(handle, (u16)slot);
    if (handle == 0) return;
    handle = pokemonGetStatus(handle, 0, 0xCC, 0);
    if ((u8)pokemonCheckValid() == 0) return;

    color = (s32)menuSubCalcColor(ctx, npc);
    if (npcId >= 0x1215 && npcId < 0x121B) {
        fn_801040F0(0, 0, ctx, (u16)fn_8001D624(handle, 1), 0);
    } else if (npcId >= 0x121B && npcId < 0x1221) {
        y = (s16)(GSmsgGetRect(0x1A8) >> 16) + 2;
        fn_800FB680(0, 0, color, 0x1A8);
        top = (s16)(GSmsgGetRect(0x197) >> 16);
        delta = *(s16*)(npc + 0x54) - y - top;
        y = y + ((delta + ((u32)delta >> 31)) >> 1);
        fn_800FB680(y, 0, color, 0x197);
        fn_80132A38(0x34, (s16)pokemonGetStatus(handle, 0, 0x83, 0));
        fn_800FBB34(0, 0, (s16)y, *(s16*)(npc + 0x56), color, 0xDE);
        fn_80132A38(0x34, (s16)pokemonGetStatus(handle, 0, 0x87, 0));
        fn_800FBB34(0, 0, *(s16*)(npc + 0x54), *(s16*)(npc + 0x56), color, 0xDE);
    } else if (npcId >= 0x1221 && npcId < 0x1227) {
        y = (s16)(GSmsgGetRect(0x1A7) >> 16);
        fn_800FB680(0, 0, color, 0x1A7);
        fn_80132A38(0x34, (u8)pokemonGetStatus(handle, 0, 0x7A, 0));
        fn_800FBB34(y, 0, *(s16*)(npc + 0x54), *(s16*)(npc + 0x56), color, 0xD2);
    } else if (npcId >= 0x1227 && npcId < 0x122D) {
        fn_80132A38(0x37, pokemonGetStatus(handle, 0, 0x77, 0));
        fn_800FBB34(0, 0, *(s16*)(npc + 0x54), *(s16*)(npc + 0x56), color, 0xE7);
    }
}
#pragma pop
#endif

/* fn_8000ED34 - 0x8000ED34 | size: 0x5dc */
extern void menuSetDisp(void);
extern void fn_800F7920(void);
extern void fn_800F7994(void);
extern void atan2(void);
extern u32 lbl_8047B710;
extern u32 lbl_8047B700;
extern u32 lbl_8047B704;
extern u32 lbl_8047B708;
extern u8 lbl_8047885C[4];
#if 0
asm void fn_8000ED34(void) {
#include "src/game/gs_npc_interact_fn_8000ED34.inc"
}
#else
#pragma push
#pragma peephole off
void fn_8000ED34(u8* ctx) {
    extern u8* fn_80105624(void);
    extern u32 windowGetParam(u8* a, s32 b);
    extern void menuSetDisp(u32, s32);
    extern u8 fn_80102620(s32);
    extern s8 fn_800F7920(s32, s32);
    extern s8 fn_800F7994(s32, s32);
    extern u32 pokemonGetStatus();
    extern u32 pokemonCheckValid(void);
    extern s32 fn_801026A4(s32, ...);
    u8* flags;
    u32 base;
    u32 aux;
    s32 activeMenu;
    s32 targetMenu;
    s32 selected;
    s32 x;
    s32 y;
    s32 absX;
    s32 absY;
    u16 bits;
    u8 pressed;
    u32 handle;
    s32 stored;

    flags = fn_80105624();
    base = windowGetParam(ctx, 0);
    windowGetParam(ctx, 1);
    aux = windowGetParam(ctx, 2);
    menuSetDisp(*(u32*)(ctx + 4), 1);
    activeMenu = 0;
    targetMenu = 0;
    selected = -1;
    if ((u8)fn_80102620(0xFC) != 0) {
        activeMenu = 0xFC;
    } else if ((u8)fn_80102620(0xFD) != 0) {
        activeMenu = 0xFD;
    }

    x = (s8)fn_800F7920(1, 0);
    y = (s8)fn_800F7994(1, 0);
    absX = (x < 0) ? -x : x;
    absY = (y < 0) ? -y : y;
    if (absX > 0x20 || absY > 0x20) {
        if (absX < absY) {
            selected = (y < 0) ? 2 : 3;
        } else {
            selected = (x < 0) ? 0 : 1;
        }
    }
    if (selected < 0) {
        bits = *(u16*)(flags + 4);
        if (bits & 0x80) {
            selected = 4;
        } else if (bits & 0x40) {
            selected = 5;
        }
    }

    if (selected >= 0) {
        handle = base;
        if ((u32)(u16)selected < 6) {
            if (handle == 0) {
                handle = fn_801F2A7C(0);
            }
            if (handle != 0) {
                handle = fn_801F986C(handle, (u16)selected);
                if (handle != 0) {
                    handle = pokemonGetStatus(handle, 0, 0xCC, 0);
                    if ((u8)pokemonCheckValid() == 0) {
                        handle = 0;
                    }
                }
            }
        } else {
            handle = 0;
        }
        if (handle != 0) {
            ctx[0x98] = 1;
            *(s32*)(ctx + 0x80) = selected;
        } else {
            selected = -1;
        }
    }

    if (selected < 0) {
        bits = *(u16*)(flags + 4);
        if ((bits & 0x400) && ((u8)aux != 0)) {
            ctx[0x98] = 1;
            ctx[0x99] = 1;
            *(s32*)(ctx + 0x80) = -1;
        } else if (*(u16*)flags & 0x800) {
            menuSetDisp(*(u32*)(ctx + 4), 0);
            stored = -1;
            if (*(u16*)flags & 1) {
                if (*(u16*)flags & 8) {
                    stored = 5;
                } else if (*(u16*)flags & 4) {
                    stored = 4;
                } else {
                    stored = 0;
                }
            } else if (*(u16*)flags & 2) {
                stored = 2;
            } else if (*(u16*)flags & 8) {
                stored = 1;
            } else if (*(u16*)flags & 4) {
                stored = 3;
            }
            *(s32*)lbl_8047885C = stored;
            if (stored < 0) {
                targetMenu = 0xFC;
            } else {
                targetMenu = 0xFD;
            }
        }
    }

    pressed = 0;
    if ((u8)fn_801F18DC(0) != 0) {
        if (((u8)fn_801F1700(0) == 1) && ((u8)fn_80265924() == 1)) {
            pressed = 1;
        } else if ((u16)fn_801EF634() == 1) {
            pressed = 1;
        }
    }
    if (pressed != 0) {
        ctx[0x98] = 1;
        ctx[0x99] = 1;
    }

    if (activeMenu != targetMenu) {
        if (activeMenu != 0) {
            fn_80102510(activeMenu);
        }
        if (ctx[0x98] == 0) {
            if (targetMenu == 0xFC) {
                fn_801026A4(0xFC, *(u32*)(ctx + 4), 0, 0, 0, 1, base);
                menuSetDisp(0xFC, 1);
                menuSetDisp(0xFD, 0);
            } else if (targetMenu == 0xFD) {
                fn_801026A4(0xFD, *(u32*)(ctx + 4), 0, 0, 0, 2, base, lbl_8047885C);
                menuSetDisp(0xFD, 1);
                menuSetDisp(0xFC, 0);
            }
        }
    }
}
#pragma pop
#endif

/* menuFightCtrlSecretPokemonTop - 0x8000F310 | size: 0x4c */
#if 0
asm void menuFightCtrlSecretPokemonTop(void) {
#include "src/game/gs_npc_interact_fn_8000F310.inc"
}
#else
#pragma peephole off
u32 menuFightCtrlSecretPokemonTop(u32 arg) {
    windowGetParam((void*)arg, 0);
    windowGetParam((void*)arg, 1);
    windowGetParam((void*)arg, 2);
    return 0;
}
#pragma peephole on
#endif

/* fn_8000F35C - 0x8000F35C | size: 0xa4 */
extern void jumptable_802E4C80();
#if 0
asm void fn_8000F35C(void) {
#include "src/game/gs_npc_interact_fn_8000F35C.inc"
}
#else
#pragma push
#pragma peephole off
void fn_8000F35C(u8* ctx, u8* npc) {
    s32 visible;
    s32 id;
    s32 idx;

    visible = 1;
    if (*(s32*)(ctx + 4) == 0xF8) {
        if ((u8)windowGetParam(ctx, 2) == 0) {
            visible = 0;
        }
    }
    id = *(s16*)(npc + 6);
    idx = id - 0x11CE;
    if ((u32)idx <= 9) {
        if (idx == 0) {
            winSpriteSetDisp(npc, 1);
        } else {
            winSpriteSetDisp(npc, visible);
        }
    }
}
#pragma pop
#endif

/* fn_8000F400 - 0x8000F400 | size: 0x368 */
extern u32 fightOutPokemonGetPokemonPtr();
extern u32 wazaGetStatus();
extern void fn_800FB8C8(void);
extern void jumptable_802E4CA8();
#if 0
asm void fn_8000F400(void) {
#include "src/game/gs_npc_interact_fn_8000F400.inc"
}
#else
#pragma push
#pragma peephole off
void fn_8000F400(u8* ctx, u8* npc) {
    extern u32 windowSearchID(void);
    extern u8* windowGetAllocPtr(void);
    extern u32 windowGetParam(u8* a, s32 b);
    extern u32 fightOutPokemonGetPokemonPtr(void*);
    extern u32 pokemonGetStatus();
    extern u32 pokemonCheckValid(void);
    extern u32 wazaGetStatus();
    extern void fn_800FBB34();
    extern void fn_800FB8C8();
    extern void fn_800FB680();
    u8* party;
    u8* state;
    u32 battle;
    u32 result;
    s32 selected;
    s32 id;
    s32 color;
    s32 value;
    s32 y0;
    s32 y1;
    s32 delta;

    windowGetParam(ctx, 0);
    if (windowSearchID() == 0) return;
    party = windowGetAllocPtr();
    if (party == NULL) return;
    state = (u8*)windowGetParam(ctx, 1);
    selected = *(s32*)state;
    if (selected < 0) return;
    battle = fightOutPokemonGetPokemonPtr(*(void**)(party + 0x40));
    if (battle == 0) return;
    result = pokemonGetStatus(battle, 0, 0x7F, (u16)selected);
    color = (s32)menuSubCalcColor(ctx, npc);
    id = *(s16*)(npc + 6);

    if (id >= 0x11F4 && id < 0x11F8) {
        if (id == 0x11F4) value = 1;
        else if (id == 0x11F5) value = 3;
        else if (id == 0x11F6) value = 2;
        else value = 0;
        winSpriteSetDisp(npc, value == selected);
    } else if (id == 0x11F8) {
        fn_801040F0(0, 0, ctx, *(u16*)(party + (selected * 0xC) + 0xC), 0);
    } else if (id == 0x11F9) {
        value = wazaGetStatus(0, (u16)result, 0x22, 0);
        fn_800FBB34(0, 0, *(s16*)(npc + 0x54), *(s16*)(npc + 0x56), color, value);
    } else if (id == 0x11FA || id == 0x11FB) {
        value = wazaGetStatus(0, (u16)result, (id == 0x11FA) ? 6 : 7, 0);
        if ((u32)value <= 1) {
            fn_800FB8C8(0, 0, *(s16*)(npc + 0x54), *(s16*)(npc + 0x56), color, 0x2BE2);
        } else {
            fn_80132A38(0x34, value);
            fn_800FB8C8(0, 0, *(s16*)(npc + 0x54), *(s16*)(npc + 0x56), color, 0xD2);
        }
    } else if (id == 0x11FC) {
        fn_80132A38(0x37, *(u32*)(party + (selected * 0xC) + 4));
        fn_800FB680(0, 0, color, 0xCF);
    } else if (id >= 0x11FD && id < 0x1200) {
        y0 = (s16)(GSmsgGetRect(0x1A4) >> 16);
        delta = *(s16*)(npc + 0x54) - y0;
        fn_800FB680(0, 0, color, 0x1A4);
        y1 = (s16)(GSmsgGetRect(0x197) >> 16);
        y1 = (s16)((delta - y1 + ((u32)(delta - y1) >> 31)) >> 1);
        fn_800FB680(y0 + y1, 0, color, 0x197);
        fn_80132A38(0x34, *(u8*)(party + (selected * 0xC) + 0xF));
        fn_800FBB34(y0, y1, *(s16*)(npc + 0x56), color, color, 0xDE);
        fn_80132A38(0x34, *(u8*)(party + (selected * 0xC) + 0xE));
        fn_800FBB34(y0, delta, *(s16*)(npc + 0x56), color, color, 0xDE);
    }
}
#pragma pop
#endif

/* fn_8000F768 - 0x8000F768 | size: 0x1fc */
extern void jumptable_802E4CD8();
#if 0
asm void fn_8000F768(void) {
#include "src/game/gs_npc_interact_fn_8000F768.inc"
}
#else
#pragma push
#pragma peephole off
void fn_8000F768(u8* ctx, u8* npc) {
    extern u32 windowSearchID(void);
    extern u8* windowGetAllocPtr(void);
    extern u32 windowGetParam(u8* a, s32 b);
    extern void fn_800FBB34();
    extern void fn_800FB680();
    u8* entries;
    u8* entry;
    s32 slot;
    s32 idx;
    s32 id;
    s32 color;
    s32 y;
    s32 delta;

    windowGetParam(ctx, 0);
    if (windowSearchID() == 0) return;
    entries = windowGetAllocPtr();
    if (entries == NULL) return;
    color = (s32)menuSubCalcColor(ctx, npc);
    id = *(s16*)(npc + 6);
    idx = id - 0x11D9;
    if ((u32)idx > 0x14) return;
    slot = idx & 3;
    entry = entries + (slot * 0xC);
    if (*(u32*)(entry + 4) != 0) {
        winSpriteSetDisp(npc, 1);
    } else {
        winSpriteSetDisp(npc, 0);
        return;
    }

    if (id >= 0x11E2 && id < 0x11E6) {
        fn_801040F0(0, 0, ctx, *(u16*)(entry + 0xC), 0);
    } else if (id >= 0x11E6 && id < 0x11EA) {
        y = (s16)(GSmsgGetRect(0x197) >> 16);
        delta = *(s16*)(npc + 0x54) - y;
        y = (s16)((delta + ((u32)delta >> 31)) >> 1);
        fn_800FB680(y, 0, color, 0x197);
        fn_80132A38(0x34, *(u8*)(entry + 0xF));
        fn_800FBB34(0, 0, y, *(s16*)(npc + 0x56), color, 0xDE);
        fn_80132A38(0x34, *(u8*)(entry + 0xE));
        fn_800FBB34(0, 0, *(s16*)(npc + 0x54), *(s16*)(npc + 0x56), color, 0xDE);
    } else if (id >= 0x11EA && id < 0x11EE) {
        fn_80132A38(0x37, *(u32*)(entry + 4));
        fn_800FB680(0, 0, color, 0xE7);
    }
}
#pragma pop
#endif

/* fn_8000F964 - 0x8000F964 | size: 0x474 */
extern u32 lbl_8047B710;
extern u32 lbl_8047B700;
extern u32 lbl_8047B704;
extern u32 lbl_8047B708;
extern u8 lbl_80478858[4];
#if 0
asm void fn_8000F964(void) {
#include "src/game/gs_npc_interact_fn_8000F964.inc"
}
#else
#pragma push
#pragma peephole off
void fn_8000F964(u8* ctx) {
    extern u8* fn_80105624(void);
    extern u8* windowGetAllocPtr(u8* a);
    extern void menuSetDisp(u32, s32);
    extern u8 fn_80102620(s32);
    extern s8 fn_800F7920(s32, s32);
    extern s8 fn_800F7994(s32, s32);
    extern s32 fn_801026A4(s32, ...);
    u8* flags;
    u8* entries;
    s32 activeMenu;
    s32 targetMenu;
    s32 selected;
    s32 x;
    s32 y;
    s32 absX;
    s32 absY;
    s32 stored;
    u8 pressed;

    flags = fn_80105624();
    entries = windowGetAllocPtr(ctx);
    selected = -1;
    activeMenu = 0;
    targetMenu = 0;
    menuSetDisp(*(u32*)(ctx + 4), 1);
    if ((u8)fn_80102620(0xF9) != 0) {
        activeMenu = 0xF9;
    } else if ((u8)fn_80102620(0xFA) != 0) {
        activeMenu = 0xFA;
    }

    x = (s8)fn_800F7920(1, 0);
    y = (s8)fn_800F7994(1, 0);
    absX = (x < 0) ? -x : x;
    absY = (y < 0) ? -y : y;
    if (absX > 0x20 || absY > 0x20) {
        if (absX < absY) {
            selected = (y < 0) ? 2 : 3;
        } else {
            selected = (x < 0) ? 0 : 1;
        }
    }
    if (selected >= 0 && *(u32*)(entries + (selected * 0xC) + 4) == 0) {
        selected = -1;
    }
    if (selected >= 0) {
        ctx[0x98] = 1;
        *(s32*)(ctx + 0x80) = selected;
    } else if (*(u16*)(flags + 4) & 0x400) {
        ctx[0x98] = 1;
        ctx[0x99] = 1;
        *(s32*)(ctx + 0x80) = -1;
    } else if (*(u16*)flags & 0x800) {
        menuSetDisp(*(u32*)(ctx + 4), 0);
        if (*(u16*)flags & 1) {
            stored = 0;
        } else if (*(u16*)flags & 8) {
            stored = 1;
        } else if (*(u16*)flags & 2) {
            stored = 2;
        } else if (*(u16*)flags & 4) {
            stored = 3;
        } else {
            stored = -1;
        }
        *(s32*)lbl_80478858 = stored;
        if (stored >= 0 && *(u32*)(entries + (stored * 0xC) + 4) == 0) {
            *(s32*)lbl_80478858 = -1;
            stored = -1;
        }
        targetMenu = (stored < 0) ? 0xF9 : 0xFA;
    }

    pressed = 0;
    if ((u8)fn_801F18DC(0) != 0) {
        if (((u8)fn_801F1700(0) == 1) && ((u8)fn_80265924() == 1)) {
            pressed = 1;
        } else if ((u16)fn_801EF634() == 1) {
            pressed = 1;
        }
    }
    if (pressed != 0) {
        ctx[0x98] = 1;
        ctx[0x99] = 1;
    }

    if (activeMenu != targetMenu) {
        if (activeMenu != 0) {
            fn_80102510(activeMenu);
        }
        if (ctx[0x98] == 0) {
            if (targetMenu == 0xF9) {
                fn_801026A4(0xF9, *(u32*)(ctx + 4), 0, 0, 0, 1, *(u32*)(ctx + 4));
                menuSetDisp(0xF9, 1);
                menuSetDisp(0xFA, 0);
            } else if (targetMenu == 0xFA) {
                fn_801026A4(0xFA, *(u32*)(ctx + 4), 0, 0, 0, 2, *(u32*)(ctx + 4), lbl_80478858);
                menuSetDisp(0xFA, 1);
                menuSetDisp(0xF9, 0);
            }
        }
    }
}
#pragma pop
#endif

/* menuFightCtrlSecretWazaTop - 0x8000FDD8 | size: 0x60 */
extern void fn_80103FFC(void);
extern void* memcpy(void* dst, const void* src, u32 n);
#if 0
asm void menuFightCtrlSecretWazaTop(void) {
#include "src/game/gs_npc_interact_fn_8000FDD8.inc"
}
#else
#pragma peephole off
u32 menuFightCtrlSecretWazaTop(u8* ptr) {
    extern void* fn_80103FFC(u8* a, u32 size);
    extern void windowGetAllocPtr(u8* a);
    if ((s8)ptr[1] == 0) {
        void* dst = fn_80103FFC(ptr, 0x48);
        if (dst != NULL) {
            memcpy(dst, *(void**)(ptr + 0x60), 0x48);
        }
    }
    windowGetAllocPtr(ptr);
    return 0;
}
#pragma peephole on
#endif

/* fn_8000FE38 - 0x8000FE38 | size: 0x118 */
#if 0
asm void fn_8000FE38(void) {
#include "src/game/gs_npc_interact_fn_8000FE38.inc"
}
#else
#pragma push
#pragma peephole off
void fn_8000FE38(u8* arg1) {
    extern void* fn_80105624(void);
    extern u8 fn_801F18DC(s32 a);
    extern u8 fn_801F1700(s32 a);
    extern u8 fn_80265924(void);
    extern u16 fn_801EF634(void);
    void* data;
    u16 flags;
    s32 flag_val;
    u8 flag;
    data = fn_80105624();
    flags = *(u16*)((u8*)data + 4);
    flag_val = -1;
    if (flags & (1 << 4)) {
        flag_val = 0;
    } else if (flags & (1 << 5)) {
        flag_val = 2;
    } else if (flags & (1 << 11)) {
        flag_val = 3;
    } else if (flags & (1 << 9)) {
        arg1[0x98] = 1;
        arg1[0x99] = 1;
        *(s32*)(arg1 + 0x80) = -1;
    }
    if (flag_val >= 0) {
        arg1[0x98] = 1;
        *(s32*)(arg1 + 0x80) = flag_val;
    }
    if ((u8)fn_801F18DC(0) != 0) {
        if ((u8)fn_801F1700(0) == 1) {
            if ((u8)fn_80265924() == 1) {
                flag = 1;
                goto got_flag;
            }
        }
        if ((u16)fn_801EF634() == 1) {
            flag = 1;
            goto got_flag;
        }
    }
    flag = 0;
got_flag:
    if (flag) {
        arg1[0x98] = 1;
        arg1[0x99] = 1;
    }
}
#pragma pop
#endif

/* menuFightCtrlSecretMain - 0x8000FF50 | size: 0x58 */
#if 0
asm void menuFightCtrlSecretMain(void) {
#include "src/game/gs_npc_interact_fn_8000FF50.inc"
}
#else
#pragma peephole off
u32 menuFightCtrlSecretMain(u8* ptr) {
    extern void* fn_80103FFC(u8* a, u32 size);
    if ((s8)ptr[1] == 0) {
        void* dst = fn_80103FFC(ptr, 0x18);
        if (dst != NULL) {
            memcpy(dst, *(void**)(ptr + 0x60), 0x18);
        }
    }
    return 0;
}
#pragma peephole on
#endif

/* fn_8000FFA8 - 0x8000FFA8 | size: 0x118 */
extern void jumptable_802E4D2C();
#if 0
asm void fn_8000FFA8(void) {
#include "src/game/gs_npc_interact_fn_8000FFA8.inc"
}
#else
#pragma push
#pragma peephole off
void fn_8000FFA8(u8* ctx, u8* npc) {
    extern u8* windowGetFreeWork(u8* a);
    u8* state;
    s32 idx;
    s32 slot;
    s32 msg;

    state = windowGetFreeWork(ctx);
    idx = *(s16*)(npc + 6) - 0x11AA;
    slot = -1;
    msg = 0;
    if ((u32)idx <= 0x17 && idx < 6) {
        slot = idx;
    }
    if (slot >= 0) {
        switch (state[slot]) {
        case 0:
            msg = 0x1B4;
            break;
        case 1:
            msg = 0x3AC;
            break;
        case 2:
            msg = 0x3AE;
            break;
        case 3:
            msg = 0x3AD;
            break;
        }
        if ((u16)msg != 0) {
            fn_801040F0(0, 0, ctx, (u16)msg, 0);
        }
    }
}
#pragma pop
#endif

/* menuFightCtrlBall - 0x800100C0 | size: 0x68 */
#if 0
asm void menuFightCtrlBall(void) {
#include "src/game/gs_npc_interact_fn_800100C0.inc"
}
#else
#pragma peephole off
u32 menuFightCtrlBall(u8* ptr) {
    extern void* windowGetFreeWork(u8* a);
    extern void* windowGetParam(u8* a, u32 b);
    void* dst = windowGetFreeWork(ptr);
    if ((s8)ptr[1] == 0) {
        memcpy(dst, windowGetParam(ptr, 0), 6);
    }
    return 0;
}
#pragma peephole on
#endif

/* _menuFightIsUse__FP16MENU_WAZA_STATUSUs - 0x80010128 | size: 0x16c */
extern u32 fn_801FFEC8();
extern u32 fightOutPokemonGetSoubiItemDataId();
#if 0
asm void _menuFightIsUse__FP16MENU_WAZA_STATUSUs(void) {
#include "src/game/gs_npc_interact_fn_80010128.inc"
}
#else
#pragma push
#pragma peephole off
u32 _menuFightIsUse__FP16MENU_WAZA_STATUSUs(ctx, arg)
u8* ctx;
u16 arg;
{
    extern u32 fightOutPokemonGetPokemonPtr(void*);
    extern u32 fn_801FFEC8(void*, u16, s32, void*);
    extern u32 pokemonGetStatus();
    extern u32 wazaGetStatus();
    extern u32 fn_800FA280(void);
    extern u32 fightOutPokemonGetSoubiItemDataId(void*);
    u16 stackValue;
    u8* obj;
    u32 battle;
    u32 state;
    u32 result;
    u32 msg;

    obj = *(u8**)(ctx + 0x40);
    battle = fightOutPokemonGetPokemonPtr(obj);
    state = fn_801FFEC8(obj, arg, 1, &stackValue);
    result = pokemonGetStatus(battle, 0, 0x7F, arg);
    msg = 0;
    if ((u8)state != 0) {
        fn_80132A38(0x11, (s32)obj);
        msg = wazaGetStatus(0, (u16)result, 1, 0);
        fn_80132A38(0x28, fn_800FA280());
        fn_801F4C14(0, 0, 0x56, 0, (u16)fightOutPokemonGetSoubiItemDataId(obj));
    }
    switch ((u8)state) {
    case 6:
        msg = 0x7661;
        break;
    case 5:
        wazaGetStatus(0, stackValue, 1, 0);
        fn_80132A38(0x28, fn_800FA280());
        msg = 0x76BB;
        break;
    case 4:
        msg = 0x7600;
        break;
    case 3:
        msg = 0x75FF;
        break;
    case 2:
        msg = 0x75FE;
        break;
    case 1:
        msg = 0x75FD;
        break;
    }
    if ((u8)state == 0) {
        return 0;
    }
    return msg;
}
#pragma pop
#endif

/* fn_80010294 - 0x80010294 | size: 0x1e8 */
extern u32 fn_80104530();
#if 0
asm void fn_80010294(void) {
#include "src/game/gs_npc_interact_fn_80010294.inc"
}
#else
#pragma push
#pragma peephole off
void fn_80010294(u8* ctx, u8* npc) {
    extern u8* windowGetAllocPtr(u8* a);
    extern u32 fn_80104530(u32 val);
    extern u32 fightOutPokemonGetPokemonPtr(void*);
    extern u32 pokemonGetStatus();
    extern void fn_800FB8C8();
    u8* entries;
    u32 color;
    s32 idx;
    s32 offset;
    s32 y;
    u32 result;

    entries = windowGetAllocPtr(ctx);
    color = (u32)menuSubCalcColor(ctx, npc);
    idx = (s8)((fn_80104530(*(u32*)(ctx + 4)) >> 16) & 0xFF);
    y = *(s16*)(npc + 0x54) - (s16)(GSmsgGetRect(0x197) >> 16);
    y = (s16)((y + ((u32)y >> 31)) >> 1);
    fn_800FB680(y, 0, color, 0x197);

    if (idx < 0 || idx >= 4) return;
    offset = idx * 0xC;
    if (*(u32*)(entries + offset + 4) == 0) return;

    result = 0;
    if (fightOutPokemonGetPokemonPtr(*(void**)(entries + 0x40)) != 0) {
        result = (u16)pokemonGetStatus(fightOutPokemonGetPokemonPtr(*(void**)(entries + 0x40)), 0, 0x7F,
                                  (s8)ctx[0x95]);
    }
    if (result == 0 || result == 0x164) {
        fn_800FB8C8(0, 0, y, *(s16*)(npc + 0x56), color, 0x2BE1);
        fn_800FB8C8(0, 0, *(s16*)(npc + 0x54), *(s16*)(npc + 0x56), color, 0x2BE1);
    } else if (result == 0x165) {
        fn_800FB8C8(0, 0, y, *(s16*)(npc + 0x56), color, 0x2B6D);
        fn_800FB8C8(0, 0, *(s16*)(npc + 0x54), *(s16*)(npc + 0x56), color, 0x2B6D);
    } else {
        fn_80132A38(0x34, *(u8*)(entries + offset + 0xF));
        fn_800FB8C8(0, 0, y, *(s16*)(npc + 0x56), color, 0xD2);
        fn_80132A38(0x34, *(u8*)(entries + offset + 0xE));
        fn_800FB8C8(0, 0, *(s16*)(npc + 0x54), *(s16*)(npc + 0x56), color, 0xD2);
    }
}
#pragma pop
#endif

/* fn_8001047C - 0x8001047C | size: 0x10c */
#if 0
asm void fn_8001047C(void) {
#include "src/game/gs_npc_interact_fn_8001047C.inc"
}
#else
#pragma push
#pragma peephole off
void fn_8001047C(u8* arg1) {
    extern void* windowGetAllocPtr(u8* a);
    extern u32 fn_80104530(u32 val);
    extern void* fightOutPokemonGetPokemonPtr(void* obj);
    extern u32 pokemonGetStatus(s32 p1, s32 p2, s32 p3, s32 p4, u16 p5, s32 p6);
    extern void fn_80132A38(s32 p1, s32 val);
    extern void fn_801040F0(s32 p1, s32 p2, u8* p3, u16 p4, s32 p5);
    void* participant;
    u32 npc_data;
    s16 idx;
    s32 r30;
    u16 battle_result;
    u16 val;
    s32 temp;
    participant = windowGetAllocPtr(arg1);
    npc_data = fn_80104530(*(u32*)(arg1 + 4));
    idx = (s16)(npc_data >> 16);
    temp = (s8)(idx & 0xFF);
    if (temp < 0 || temp >= 4) return;
    r30 = temp * 0xc;
    if (*(u32*)((u8*)participant + r30 + 4) == 0) return;
    battle_result = 0;
    if (fightOutPokemonGetPokemonPtr(*(void**)((u8*)participant + 0x40)) != 0) {
        battle_result = (u16)pokemonGetStatus(0, 0x7f, 0, 0, (s8)(*(u8*)(arg1 + 0x95)), 0);
    }
    val = 0;
    if (battle_result == 0 || battle_result == 0x164) {
        val = 0;
    } else if (battle_result < 0x166) {
        val = 0x5d;
    } else {
        val = *(u16*)((u8*)participant + r30 + 0xc);
    }
    if ((u16)val != 0) {
        fn_801040F0(0, 2, arg1, val, 0);
    }
}
#pragma pop
#endif

/* fn_80010588 - 0x80010588 | size: 0x11c */
#if 0
asm void fn_80010588(void) {
#include "src/game/gs_npc_interact_fn_80010588.inc"
}
#else
#pragma push
#pragma peephole off
void fn_80010588(u8* arg1, u8* arg2) {
    typedef struct {
        u32 unk_00;
        u32 value;
        u32 unk_08;
    } NpcInteractEntry;
    extern NpcInteractEntry* windowGetAllocPtr(u8* a);
    extern void* windowGetFreeWork(u8* a);
    extern void fn_800FB680(s32 a, s32 b, s32 c, u32 d);
    NpcInteractEntry* participant;
    void* npc_data;
    s32 idx;
    s16 npc_id;
    s32 offset;
    u32 result;
    participant = windowGetAllocPtr(arg1);
    npc_data = windowGetFreeWork(arg1);
    npc_id = *(s16*)(arg2 + 6);
    idx = 0;
    switch (npc_id) {
    case 0xC4:
        idx = 0;
        break;
    case 0xC5:
        idx = 1;
        break;
    case 0xC6:
        idx = 2;
        break;
    case 0xC7:
        idx = 3;
        break;
    }
    if ((s8)*((u8*)npc_data + 2) == idx) {
        fn_801040F0(0, 0, arg1, 0x49, 0);
        fn_801040F0(0, 0, arg1, 0x4A, 0);
    }
    offset = idx * 0xc;
    participant = (NpcInteractEntry*)((u32)participant + offset);
    result = participant->value;
    if (result != 0) {
        fn_80132A38(0x37, result);
        fn_800FB680(0, 0, (s32)menuSubCalcColor(arg1, arg2), 0xE7);
    }
}
#pragma pop
#endif

/* fn_800106A4 - 0x800106A4 | size: 0x1a0 */
#if 0
asm void fn_800106A4(void) {
#include "src/game/gs_npc_interact_fn_800106A4.inc"
}
#else
#pragma push
#pragma peephole off
void fn_800106A4(u8* arg1, u8* arg2) {
    extern u32 windowGetAllocPtr(u8* a);
    extern u32 windowGetFreeWork(u8* a);
    extern u32 windowGetParam(u8* a, s32 b);
    extern void fn_80132A38(s32 p1, s32 val);
    extern void* fn_8001D834(u8* a, u8* b);
    extern void fn_800FB680(s32 a, s32 b, s32 c, u32 d);
    extern void* fightOutPokemonGetNicknamePtr(void* a);
    void* participant;
    void* npc_data;
    u32 r30;
    s16 npc_id;
    s32 sub;
    s32 val;
    u8* p1;
    u32 t1;
    u32 t2;
    npc_id = *(s16*)(arg2 + 6);
    r30 = 0;
    switch (npc_id) {
    case 0xb4:
        t1 = windowGetAllocPtr(arg1);
        fn_80132A38(0x37, (s32)t1);
        r30 = 0xcf;
        break;
    case 0x11cd:
        t2 = windowGetAllocPtr(arg1);
        fn_80132A38(0x36, (s32)t2);
        r30 = 0x196;
        break;
    case 0xc1:
        participant = (void*)windowGetAllocPtr(arg1);
        npc_data = (void*)windowGetFreeWork(arg1);
        sub = (s32)*(u8*)npc_data;
        switch (sub) {
        case 0:
            fn_80132A38(0x36, *(s32*)participant);
            r30 = 0x196;
            break;
        case 1:
            r30 = 0x199;
            break;
        }
        break;
    case 0x11d8:
        r30 = 0x196;
        val = (s32)*(u32*)(arg1 + 4);
        if (val == 0xf7) {
            fn_80132A38(0x36, *(s32*)windowGetAllocPtr(arg1));
        } else if (val == 0xf8) {
            p1 = (u8*)windowGetParam(arg1, 1);
            if ((u8)windowGetParam(arg1, 2) != 0) {
                fn_80132A38(0x36, (s32)fightOutPokemonGetNicknamePtr(p1));
            } else {
                r30 = 0x1a9;
            }
        } else {
            return;
        }
        break;
    }
    if (r30 != 0) {
        fn_800FB680(0, -2, (s32)menuSubCalcColor(arg1, arg2), r30);
    }
}
#pragma pop
#endif

/* fn_80010844 - 0x80010844 | size: 0x15c */
extern void pokemonWazaReplace(void);
extern void pokemonToMenuWazaStatus(void);
#if 0
asm void fn_80010844(void) {
#include "src/game/gs_npc_interact_fn_80010844.inc"
}
#else
#pragma push
#pragma peephole off
u32 fn_80010844(u8* ctx) {
    extern u8* windowGetAllocPtr(u8* a);
    extern u8* windowGetFreeWork(u8* a);
    extern u8* fn_80105624(void);
    extern u32 fightOutPokemonGetPokemonPtr(void*);
    extern u32 pokemonGetStatus();
    extern void menuButtonNormal(u8* a);
    extern void pokemonWazaReplace(void*, s32, s32);
    extern void pokemonToMenuWazaStatus(void*, void*);
    u8* participant;
    u8* state;
    u8* flagsObj;
    u32 model;
    u16 flags;
    u32 saved;

    participant = windowGetAllocPtr(ctx);
    state = windowGetFreeWork(ctx);
    model = fightOutPokemonGetPokemonPtr(*(void**)(participant + 0x40));
    flagsObj = fn_80105624();
    flags = *(u16*)(flagsObj + 4);

    if (state[0] == 0) {
        if (model != 0 &&
            ((u16)pokemonGetStatus(model, 0, 0x7F, (s8)ctx[0x95]) == 0x165) &&
            (*(u16*)(fn_80105624() + 4) & 0x40)) {
            ctx[0x98] = 1;
            ctx[0x99] = 1;
        } else {
            menuButtonNormal(ctx);
        }
    } else if (state[0] == 1) {
        if ((flags & 0xD0) != 0) {
            if (model != 0) {
                pokemonWazaReplace((void*)model, (s8)state[2], (s8)ctx[0x95]);
                saved = *(u32*)(participant + 0x40);
                pokemonToMenuWazaStatus((void*)model, participant);
                *(u32*)(participant + 0x40) = saved;
            }
            state[2] = 0xFF;
            state[0] = 0;
        } else if ((flags & 0x40) != 0) {
            state[2] = 0xFF;
            state[0] = 0;
        }
    }
    return 0;
}
#pragma pop
#endif

/* fn_800109A0 - 0x800109A0 | size: 0x190 */
extern u8 lbl_80478850[8];
#if 0
asm void fn_800109A0(void) {
#include "src/game/gs_npc_interact_fn_800109A0.inc"
}
#else
#pragma push
#pragma peephole off
u32 fn_800109A0(u8* ctx) {
    extern void* fn_80103FFC(u8* a, u32 size);
    extern u32 windowGetParam(u8* a, s32 b);
    extern u8* windowGetFreeWork(u8* a);
    extern u8* windowGetAllocPtr(u8* a);
    extern s32 fn_801022B8(u32 val);
    void* dst;
    u8* state;
    u8* party;
    s32 i;
    s32 id;

    if ((s8)ctx[1] == 0) {
        dst = fn_80103FFC(ctx, 0x48);
        if (dst != NULL) {
            memcpy(dst, (void*)windowGetParam(ctx, 0), 0x48);
        }
        state = windowGetFreeWork(ctx);
        state[0] = 0;
        state[1] = 0;
        state[2] = 0xFF;
    }

    party = windowGetAllocPtr(ctx);
    if ((s8)ctx[1] == 5) {
        for (i = 0; i < 4; i++) {
            menuItemBiosSetSelectFlag(*(u16*)(lbl_80478850 + (i * 2)), 1);
        }
    } else {
        for (i = 0; i < 4; i++) {
            menuItemBiosSetSelectFlag(*(u16*)(lbl_80478850 + (i * 2)),
                         (*(u32*)(party + (i * 0xC) + 4) != 0) ? 1 : 0);
        }
    }

    id = fn_801022B8(*(u32*)(ctx + 4));
    switch (id) {
    case 0xC4:
        *(s32*)(ctx + 0x80) = 0;
        break;
    case 0xC5:
        *(s32*)(ctx + 0x80) = 1;
        break;
    case 0xC6:
        *(s32*)(ctx + 0x80) = 2;
        break;
    case 0xC7:
        *(s32*)(ctx + 0x80) = 3;
        break;
    default:
        *(s32*)(ctx + 0x80) = -1;
        break;
    }
    return 0;
}
#pragma pop
#endif

/* fn_80010B30 - 0x80010B30 | size: 0x168 */
#pragma push
#pragma peephole off
#pragma peephole off
u32 fn_80010B30(u8* arg) {
    extern void* fn_80103FFC(u8* a, u32 size);
    extern void* windowGetAllocPtr(u8* a);
    extern s32 fn_801022B8(u32 val);
    extern u8* windowSearchItemID(u8* a, s32 id);
    extern void menuItemBiosSetSelectFlag(s32 id, s32 flag);
    void* entry;
    void* participant;
    s32 trainer_id;
    u8* r;
    if ((s8)arg[1] == 0) {
        entry = fn_80103FFC(arg, 0x18);
        if (entry != NULL) {
            memcpy(entry, *(void**)(arg + 0x60), 0x18);
        }
        if (*(u8*)((u8*)entry + 0x16) != 0) {
            r = windowSearchItemID(arg, 0xB6);
            *(s32*)(r + 0x4C) = 0x13D;
            r = windowSearchItemID(arg, 0xB8);
            *(s32*)(r + 0x4C) = 0x140;
            menuItemBiosSetSelectFlag(0xB8, 1);
        } else {
            r = windowSearchItemID(arg, 0xB6);
            *(s32*)(r + 0x4C) = 0x13F;
            r = windowSearchItemID(arg, 0xB8);
            *(s32*)(r + 0x4C) = 0;
            menuItemBiosSetSelectFlag(0xB8, 0);
        }
    }
    participant = windowGetAllocPtr(arg);
    trainer_id = fn_801022B8(*(u32*)(arg + 4));
    switch (trainer_id) {
    case 0xB5:
        *(s32*)(arg + 0x80) = 0;
        break;
    case 0xB6:
        if (*(u8*)((u8*)participant + 0x16) != 0) {
            *(s32*)(arg + 0x80) = 1;
        } else {
            *(s32*)(arg + 0x80) = 3;
        }
        break;
    case 0xB7:
        *(s32*)(arg + 0x80) = 2;
        break;
    case 0xB8:
        *(s32*)(arg + 0x80) = 3;
        break;
    default:
        *(s32*)(arg + 0x80) = -1;
        break;
    }
    return 0;
}
#pragma peephole on
#pragma pop

/* menuPokemonCheckPokemonChange - 0x80010C98 | size: 0x52c */
extern u32 fightOutPokemonGetTokuseiDataId(void* arg);
extern u8 fn_801F8C00(u32 warpId, u32 arg);
extern s32 fightPokemonGetNicknamePtr(u32 arg);
extern void GSlogWrite(const char* fmt, ...);
extern u8 lbl_80266788[];
extern u8 lbl_802E4B78[];
#if 0
asm void menuPokemonCheckPokemonChange(void) {
#include "src/game/gs_npc_interact_fn_80010C98.inc"
}
#else
#pragma peephole off
u32 menuPokemonCheckPokemonChange(void* npc, u32 warpId, u32 variant) {
#define WAIT_FOR_DIALOG(waitLabel, checkLabel, haveLabel, doneLabel) \
    goto checkLabel; \
waitLabel: \
    advance = fn_801F18DC(0); \
    if (advance != 0) { \
        if ((fn_801F1700(0) == 1) && (fn_80265924() == 1)) { \
            advance = 1; \
            goto haveLabel; \
        } else if (fn_801EF634() == 1) { \
            advance = 1; \
            goto haveLabel; \
        } \
    } \
    advance = 0; \
haveLabel: \
    if (advance != 0) { \
        goto doneLabel; \
    } \
    _threadSwitch(); \
checkLabel: \
    inputFlags = fn_800F7AF0(1); \
    maskedFlags = fn_800F7BC4(1); \
    maskedFlags &= inputFlags; \
    if ((maskedFlags & 0x300) == 0) { \
        goto waitLabel; \
    } \
doneLabel:

    void* linkedNpc;
    u32 inputFlags;
    u32 maskedFlags;
    u8 relation;
    u8 advance;
    u8 kind;

    relation = fn_801F2020(0, npc, &linkedNpc);
    if (relation == 1) {
        fn_80132A38(0xD, (s32)fightOutPokemonGetNicknamePtr(npc));
        fn_80106D3C(1, 0x76FB, 1, 0);
        WAIT_FOR_DIALOG(waitRelationOne, checkRelationOne, haveRelationOne, doneRelationOne);
        fn_801069FC(1);
        return 0;
    }
    if (relation == 2) {
        fn_801F4C14(0, 0, 0x57, 0, (u16)fightOutPokemonGetTokuseiDataId(linkedNpc));
        fn_80132A38(0xD, (s32)fightOutPokemonGetNicknamePtr(linkedNpc));
        fn_80132A38(0xE, (s32)fightOutPokemonGetNicknamePtr(npc));
        fn_80106D3C(1, 0x761F, 1, 0);
        WAIT_FOR_DIALOG(waitRelationTwo, checkRelationTwo, haveRelationTwo, doneRelationTwo);
        fn_801069FC(1);
        return 0;
    }
    if (warpId == 0) {
        warpId = fn_801F2A7C(0);
    }
    if (warpId == 0) {
        return 0;
    }
    variant = fn_801F986C(warpId, (u16)variant);
    if (variant == 0) {
        return 0;
    }
    kind = fn_801F8C00(warpId, variant);
    if (kind == 1) {
        fn_80132A38(0xD, fightPokemonGetNicknamePtr(variant));
        fn_80106D3C(1, 0x76FE, 1, 0);
        WAIT_FOR_DIALOG(waitKindOne, checkKindOne, haveKindOne, doneKindOne);
        fn_801069FC(1);
        return 0;
    }
    if (kind == 2) {
        fn_80132A38(0xD, fightPokemonGetNicknamePtr(variant));
        fn_80106D3C(1, 0x76FC, 1, 0);
        WAIT_FOR_DIALOG(waitKindTwo, checkKindTwo, haveKindTwo, doneKindTwo);
        fn_801069FC(1);
        return 0;
    }
    if (kind == 3) {
        fn_80132A38(0xD, fightPokemonGetNicknamePtr(variant));
        fn_80106D3C(1, 0x76FD, 1, 0);
        WAIT_FOR_DIALOG(waitKindThree, checkKindThree, haveKindThree, doneKindThree);
        fn_801069FC(1);
        return 0;
    }
    if (kind == 0) {
        return 1;
    }
    GSlogWrite((const char*)lbl_80266788, (const char*)lbl_802E4B78);
#undef WAIT_FOR_DIALOG
    return 0;
}
#pragma peephole on
#endif

/* fn_8001120C - 0x8001120C | size: 0x7c -- already decompiled above */
/* fn_800119A8 - 0x800119A8 | size: 0x74 -- already decompiled above */
/* fn_80011B4C - 0x80011B4C | size: 0x78 -- already decompiled above */
/* fn_80011C78 - 0x80011C78 | size: 0x78 -- already decompiled above */
