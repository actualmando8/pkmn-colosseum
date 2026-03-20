/**
 * @file menu_msgbox.c
 * @brief Message box and text rendering system.
 *
 * Manages the in-game message box / text window rendering pipeline.
 * Handles character-by-character text display, text scrolling, cursor
 * positioning within text boxes, and text formatting for the game's
 * custom multi-byte character encoding.
 *
 * Key behaviors:
 *   - Uses BSS lbl_803A9768 (0x2A0 bytes) as the message box state,
 *     containing the current text buffer, cursor position, scroll state,
 *     rendering parameters, and animation timing
 *   - fn_80056C54 (0x440 bytes) is the main message box state machine
 *     that coordinates text display, calling fn_80057DE8 and fn_80057F94
 *     from the dialog system for window management
 *   - fn_80057270 is a key function called 37 times from the script
 *     callback system -- it returns the current text/message context
 *     pointer, querying lbl_803A9768 and calling fn_80055194, fn_80056A78,
 *     and fn_80058F08
 *   - fn_800576C4 (0x16C bytes) is the text character renderer, called
 *     21 times from various UI modules
 *   - fn_80057830 (0x118 bytes) handles text scroll animation
 *   - fn_80057948 (0xC0 bytes) handles cursor blink/wait
 *   - fn_80057538 (0x15C bytes) handles multi-byte character decoding
 *   - fn_80057094 is called from script callbacks for text sync
 *   - Functions fn_80057694/fn_800576A4/fn_800576B4 are small state accessors
 *     (0x10 bytes each) for text position queries
 *   - fn_80057400 and fn_800574A8/fn_800574E0/fn_800574FC are text format
 *     parameter setters used by menu_status for numeric displays
 *
 * BSS usage:
 *   - lbl_803A9768 (0x2A0 bytes): Message box state structure
 *
 * Address range: 0x80056C54 - 0x80057B34 (24 functions)
 */

#include "dolphin/types.h"

/* ===== GS Engine ===== */
extern void  fn_800E01D0(void* dst, void* src);
extern void  fn_800D61E4(void* obj);
extern void  fn_800D20CC(void* obj);
extern void  fn_80109220(u32 obj, u8 visible);

/* ===== Text / Messages ===== */
extern void* fn_8001E224(u32 msgBank, u32 msgId);
extern u32   fn_8001E200(u32 msgBank, u32 msgId);

/* ===== Dialog system (internal) ===== */
extern void  fn_80057DE8(void* ctx);
extern void  fn_80057F94(void* ctx);

/* ===== Status/script (internal) ===== */
extern void  fn_80055194(void);
extern void  fn_80056A78(void);
extern void  fn_80058F08(void);

/* ===== BSS data ===== */
extern u8    lbl_803A9768[];   /* Message box state (0x2A0 bytes) */

/*
 * Functions in this translation unit (24 total):
 *
 * fn_80056C54  0x440  Message box state machine (calls fn_80057DE8, fn_80057F94)
 * fn_80057094  0x03C  Text sync accessor
 * fn_800570D0  0x044  Text sub-handler A (calls fn_80056C54)
 * fn_80057114  0x030  Text sub-handler B (calls fn_80056C54)
 * fn_80057144  0x12C  Text line formatter
 * fn_80057270  0x150  Get text/message context (called 37x from script_callback)
 * fn_800573C0  0x040  Text reset
 * fn_80057400  0x028  Text format param setter A
 * fn_80057428  0x024  Text format param accessor A (no BSS)
 * fn_8005744C  0x00C  Text format param accessor B (no BSS)
 * fn_80057458  0x048  Text format param setter B
 * fn_800574A8  0x038  Text format param setter C
 * fn_800574E0  0x01C  Text format param setter D
 * fn_800574FC  0x03C  Text format param setter E
 * fn_80057538  0x15C  Multi-byte character decoder
 * fn_80057694  0x010  Text position query X
 * fn_800576A4  0x010  Text position query Y
 * fn_800576B4  0x010  Text position query Z
 * fn_800576C4  0x16C  Text character renderer (called 21x)
 * fn_80057830  0x118  Text scroll animation
 * fn_80057948  0x0C0  Cursor blink/wait handler
 * fn_80057A08  0x030  Text state query A (no BSS)
 * fn_80057A38  0x02C  Text state query B (no BSS)
 * fn_80057A64  0x0D0  Text finalize
 */

#pragma push
#pragma force_active on

/* 0x80056C54 | size: 0x440 */
asm void fn_80056C54(void) { nofralloc
    #include "asm/GC6E01/nonmatching/menu_msgbox/fn_80056C54.s"
}

/* 0x80057270 | size: 0x150 */
asm void fn_80057270(void) { nofralloc
    #include "asm/GC6E01/nonmatching/menu_msgbox/fn_80057270.s"
}

#pragma pop
