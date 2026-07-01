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
extern void  _threadSwitch(void);           /* GSthread yield */
extern u32   fn_80102568(u32 a, u32 b, u32 c); /* scene load */
extern s32   fn_8010264C(u32 a, u32 b);        /* scene query */
extern s32   fn_801026A4(u32 sceneId, u32 a, u32 b, u32 c,
                         u32 d, u32 e, ...);
extern u32   fn_801022B8(u32 a);                /* scene message get */
extern void  fn_80109220(u32 obj, u8 visible);  /* model visibility */
extern void  fn_800FB680(u32 a, u32 b, s32 c, u32 d); /* sound trigger */
extern void  fn_80132A38(u32 effectId, u32 param);

/* ===== Pokemon data ===== */
extern void* fn_801FBFBC(u32 species);    /* Species base data get */
extern void  fn_801FAA58(u32 a, u32 b, u32 c, u32 d, s32 e);
extern u32   fn_801FB1C0(u32 a, u32 b, u32 c, u32 d);

/* ===== Text / Messages ===== */
extern u8    fn_8001E224(u32 msgBank, s32* out, u32 a, u32 b, u32 c, u32 d);
extern void  fn_8001E200(void);

typedef struct BagCacheEntry {
    u32 word0;
    u32 word1;
    u32 word2;
    u32 word3;
    u32 word4;
} BagCacheEntry;

/* ===== BSS data ===== */
extern BagCacheEntry lbl_803A6AB0[];   /* Bag item cache (0x2B38 bytes) */

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


/* 0x8004EADC | size: 0x178 */
s32 fn_8004EADC(void) {
#pragma peephole off
    u32 index;
    BagCacheEntry* src;
    BagCacheEntry* dst;
    s32 result;

    index = 0;
    while ((u16)index < 0x229) {
        src = fn_801FBFBC(index);
        dst = &lbl_803A6AB0[(u16)index];
        index++;
        *dst = *src;
    }

    while (1) {
        result = fn_801026A4(0x89, 0, 0, 0, 1, 0);
        if (result == -1) {
            fn_80102568(0x89, 0, 1);

            index = 0;
            while ((u16)index < 0x229) {
                dst = fn_801FBFBC(index);
                src = &lbl_803A6AB0[(u16)index];
                index++;
                *dst = *src;
            }

            return -1;
        }

        if (result == -2) {
            if (fn_8010264C(0x44, 1) != 0) {
                fn_80102568(0x44, 0, 1);
            } else {
                fn_80102568(0x44, 0, 1);
                fn_80102568(0x89, 0, 1);
                return 1;
            }
        }
    }
}

/* 0x80050844 | size: 0xECC */
s32 fn_80050844(void) {
#pragma peephole off
    s32 dummy1;
    s32 dummy0;
    s32 value24;
    s32 value23;
    s32 value22;
    s32 value21;
    s32 value20;
    s32 value19;
    s32 value18;
    s32 value17;
    s32 value16;
    s32 value15;
    s32 value14;
    s32 value13;
    s32 value12;
    s32 value11;
    s32 value10;
    s32 value9;
    s32 value8;
    s32 value7;
    s32 value6;
    s32 value5;
    s32 value4;
    s32 value3;
    s32 value2;
    s32 value1;
    s32 value0;
    u32 index;
    BagCacheEntry* src;
    BagCacheEntry* dst;
    s32 result;
    u32 msg;

#define HANDLE_BAG_VALUE(slot_, value_)                                           \
        if (fn_8001E224(fn_801FB1C0(0, (slot_), 0x3E, 0),                         \
                        &(value_), 0, 0x32, 0x32, 0) == 1) {                      \
            if ((value_) > 0xC8) {                                                \
                (value_) = 0xC8;                                                  \
            }                                                                     \
            if ((value_) < -0xC8) {                                               \
                (value_) = -0xC8;                                                 \
            }                                                                     \
            fn_801FAA58(0, (slot_), 0x3E, 0, (value_));                           \
        }                                                                         \
        fn_8001E200()

    index = 0;
    while ((u16)index < 0x229) {
        src = fn_801FBFBC(index);
        dst = &lbl_803A6AB0[(u16)index];
        index++;
        *dst = *src;
    }

    while (1) {
        result = fn_801026A4(0x8A, 0, 0, 0, 1, 0);
        if (result == -1) {
            fn_80102568(0x8A, 0, 1);

            index = 0;
            while ((u16)index < 0x229) {
                dst = fn_801FBFBC(index);
                src = &lbl_803A6AB0[(u16)index];
                index++;
                *dst = *src;
            }

            return -1;
        }

        if (result == -2) {
            if (fn_8010264C(0x44, 1) != 0) {
                fn_80102568(0x44, 0, 1);
                continue;
            }

            fn_80102568(0x44, 0, 1);
            break;
        }

        msg = fn_801022B8(0x8A);
        switch (msg) {
        case 0x5BC:
            HANDLE_BAG_VALUE(1, value24);
            continue;
        case 0x5BD:
            HANDLE_BAG_VALUE(2, value23);
            continue;
        case 0x5BE:
            HANDLE_BAG_VALUE(3, value22);
            continue;
        case 0x5BF:
            HANDLE_BAG_VALUE(4, value21);
            continue;
        case 0x5C0:
            HANDLE_BAG_VALUE(5, value20);
            continue;
        case 0x5C1:
            HANDLE_BAG_VALUE(6, value19);
            continue;
        case 0xFEC:
            HANDLE_BAG_VALUE(7, value18);
            continue;
        case 0x766:
            HANDLE_BAG_VALUE(8, value17);
            continue;
        case 0x5C2:
            HANDLE_BAG_VALUE(0x19, value16);
            continue;
        case 0x5C3:
            HANDLE_BAG_VALUE(9, value15);
            continue;
        case 0x5C4:
            HANDLE_BAG_VALUE(0xA, value14);
            continue;
        case 0x5C5:
            HANDLE_BAG_VALUE(0xB, value13);
            continue;
        case 0x5C6:
            HANDLE_BAG_VALUE(0xC, value12);
            continue;
        case 0x5C7:
            HANDLE_BAG_VALUE(0xD, value11);
            continue;
        case 0x5C8:
            HANDLE_BAG_VALUE(0xE, value10);
            continue;
        case 0x5C9:
            HANDLE_BAG_VALUE(0xF, value9);
            continue;
        case 0x5CA:
            HANDLE_BAG_VALUE(0x10, value8);
            continue;
        case 0x5CB:
            HANDLE_BAG_VALUE(0x11, value7);
            continue;
        case 0x5CC:
            HANDLE_BAG_VALUE(0x12, value6);
            continue;
        case 0x5CD:
            HANDLE_BAG_VALUE(0x13, value5);
            continue;
        case 0x5CE:
            HANDLE_BAG_VALUE(0x14, value4);
            continue;
        case 0x5CF:
            HANDLE_BAG_VALUE(0x15, value3);
            continue;
        case 0x5D0:
            HANDLE_BAG_VALUE(0x16, value2);
            continue;
        case 0x5D1:
            HANDLE_BAG_VALUE(0x17, value1);
            continue;
        case 0x5D2:
            HANDLE_BAG_VALUE(0x18, value0);
            continue;
        default:
            continue;
        }
    }

#undef HANDLE_BAG_VALUE

    fn_80102568(0x8A, 0, 1);
    return 1;
}
