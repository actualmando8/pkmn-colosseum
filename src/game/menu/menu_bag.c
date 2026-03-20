/**
 * @file menu_bag.c
 * @brief Bag / item pocket management interface.
 *
 * Implements the bag screen where the player views and manages items across
 * different pockets (Items, Key Items, Poke Balls, TMs/HMs, Berries).
 * Handles item selection, use, give-to-Pokemon, and toss operations.
 *
 * Key behaviors:
 *   - Uses BSS lbl_803A6AB0 (0x2B38 = 11,064 bytes) as the bag state buffer.
 *     This is the largest BSS allocation in the gap -- it stores cached copies
 *     of all item data for fast display (553 species * 0x14 bytes = 0x2B24).
 *   - fn_8004EADC and fn_8004EC54 are initialization functions that copy
 *     item data from the save file (via fn_801FBFBC) into the local cache,
 *     iterating 0x229 (553) times with 0x14-byte entries.
 *   - fn_80050844 (0xECC = 3,788 bytes) is the main bag update loop with
 *     a massive switch statement dispatching on message IDs (0x5BC through
 *     0xFEC, with cases for each pocket and item action).
 *   - fn_80051710 (0x728 bytes) handles bag pocket switching
 *   - fn_80051E38 (0x122C = 4,652 bytes) processes item use/give/toss actions
 *   - fn_8004EDCC (0xA94 = 2,708 bytes) handles item sorting/filtering
 *   - fn_8004F860 (0x5DC bytes) manages scroll position and item list display
 *   - fn_8004FE3C (0xA08 = 2,568 bytes) processes item selection confirmation
 *   - Message IDs 0x5BC-0x5D3 map to pocket navigation, 0x766 and 0xFEC
 *     to special item actions (key items, registered items)
 *   - Scene model indices 0x89 and 0x8A used for bag UI elements
 *   - Checks scene model 0x44 for resource availability
 *
 * BSS usage:
 *   - lbl_803A6AB0 (0x2B38 bytes): Bag item cache (553 species * 20 bytes)
 *
 * Address range: 0x8004EADC - 0x80053110 (9 functions)
 */

#include "dolphin/types.h"

/* ===== GS Engine ===== */
extern void  fn_800F0308(void);           /* GSthread yield */
extern u32   fn_80102568(u32 a, u32 b, u32 c); /* scene load */
extern u32   fn_8010264C(u32 a, u32 b);        /* scene query */
extern void  fn_801026A4(u32 sceneId, u32 a, u32 b, u32 c,
                         u32 d, u32 e, ...);
extern u32   fn_801022B8(u32 a);                /* scene message get */
extern void  fn_80109220(u32 obj, u8 visible);  /* model visibility */
extern void  fn_800FB680(u32 a, u32 b, s32 c, u32 d); /* sound trigger */
extern void  fn_80132A38(u32 effectId, u32 param);

/* ===== Pokemon data ===== */
extern void* fn_801FBFBC(u16 species);    /* Species base data get */
extern void* fn_801FAA58(u32 slot);       /* Party Pokemon get */
extern u32   fn_801FB1C0(void* pkmn, u32 field);

/* ===== Text / Messages ===== */
extern void* fn_8001E224(u32 msgBank, u32 msgId);
extern u32   fn_8001E200(u32 msgBank, u32 msgId);

/* ===== BSS data ===== */
extern u8    lbl_803A6AB0[];   /* Bag item cache (0x2B38 bytes) */

/*
 * Functions in this translation unit (9 total):
 *
 * fn_8004EADC  0x178  Bag init A (copy item data, 553 species loop, scene 0x89)
 * fn_8004EC54  0x178  Bag init B (copy item data, 553 species loop, scene 0x8A)
 * fn_8004EDCC  0xA94  Item sort/filter handler (2708 bytes)
 * fn_8004F860  0x5DC  Scroll position / item list display (1500 bytes)
 * fn_8004FE3C  0xA08  Item selection confirmation (2568 bytes)
 * fn_80050844  0xECC  Bag main update loop (3788 bytes, msg switch 0x5BC-0xFEC)
 * fn_80051710  0x728  Pocket switching handler (1832 bytes)
 * fn_80051E38  0x122C Item use/give/toss processor (4652 bytes)
 * fn_80053064  0x0AC  Bag cleanup
 */

#pragma push
#pragma force_active on

/* 0x8004EADC | size: 0x178 */
asm void fn_8004EADC(void) { nofralloc
    #include "asm/GC6E01/nonmatching/menu_bag/fn_8004EADC.s"
}

/* 0x80050844 | size: 0xECC */
asm void fn_80050844(void) { nofralloc
    #include "asm/GC6E01/nonmatching/menu_bag/fn_80050844.s"
}

#pragma pop
