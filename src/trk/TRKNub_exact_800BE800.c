/**
 * @file TRKNub_exact_800BE800.c
 * @brief Exact pure-C MetroTRK message sender.
 */
#include "dolphin/types.h"

extern void MWTRACE(s32 level, const char* fmt, ...);
extern s32 fn_800C3588(void* dst, u32 val);
extern char lbl_8026F640[];

s32 MessageSend(u8* message)
{
    s32 result = fn_800C3588(message + 0x10, *(u32*)(message + 0x8));
    MWTRACE(1, lbl_8026F640, result);
    return 0;
}
