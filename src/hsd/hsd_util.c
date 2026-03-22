/**
 * @file hsd_util.c
 * @brief HSD internal functions (0x8019C3C4-0x8019C690).
 *
 * Stub coverage for 1 functions.
 */

#include "dolphin/types.h"
#include "hsd/hsd_debug.h"

/* 0x8019C3C4 | 0x2CC */
void fn_8019C3C4(void) {
    extern u8 lbl_80274818[];
    extern u8 lbl_80478C74[];
    extern u8 lbl_80478C7C[];
    extern u8 lbl_80478C80[];
    extern u8 lbl_8047B270[];
    extern u8 lbl_8047B274[];
    extern u8 lbl_8047B280[];
    extern u8 lbl_8047B284[];
    extern u8 lbl_8047B288[];
    extern u8 lbl_8047B28C[];
    extern void fn_8009F3D4();
    extern void fn_801A69C0();
    extern u8 jumptable_8036C8C0[];
    u8 sp[0x90];
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
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    if ((s32)tmp == 0) {
    }
    tmp = *(u32*)lbl_8047B280;
    r11 = (u32)lbl_80274818;
    r31 = (u32)lbl_80274818;
    r30 = 0x0;
    if ((s32)tmp != 0) {
        tmp = *(u32*)lbl_8047B28C;
        if ((s32)tmp == 0) {
            r3 = r31 + 0x8c;
            OSReport();
            tmp = 0x1;
            *(u32*)lbl_8047B28C = tmp;
        }
        r3 = r30;
        return;
    }
    r4 = (u32)sp + 0x98;
    tmp = (u32)sp + 0x8;
    r5 = 0x1000000;
    *(u32*)(sp + 0x70) = tmp;
    if (r3 > 7) { r3 = r30; return; }
    r4 = (u32)jumptable_8036C8C0;
    tmp = r3 << 2;
    r3 = (u32)jumptable_8036C8C0;
    r3 = *(u32*)(r3 + tmp);
    ctr_fn = (void(*)(void))r3;
    r3 = (u32)sp + 0x68;
    r4 = 0x1;
    __va_arg();
    tmp = *(u32*)((u8*)r3 + 0x0);
    if (tmp == 0) { r3 = r30; return; }
    *(u32*)lbl_80478C7C = tmp;
    r30 = 0x1;
    r3 = r30;
    return;
    r3 = (u32)sp + 0x68;
    r4 = 0x1;
    __va_arg();
    tmp = *(u32*)((u8*)r3 + 0x0);
    if (tmp == 0) { r3 = r30; return; }
    *(u32*)lbl_80478C80 = tmp;
    r30 = 0x1;
    r3 = r30;
    return;
    r3 = (u32)sp + 0x68;
    r4 = 0x1;
    __va_arg();
    r31 = *(u32*)((u8*)r3 + 0x0);
    fn_8009F3D4();
    if (r31 >= r3) { r3 = r30; return; }
    *(u32*)lbl_8047B284 = r31;
    r30 = 0x1;
    r3 = r30;
    return;
    r3 = (u32)sp + 0x68;
    r4 = 0x1;
    __va_arg();
    r4 = r3;
    r3 = (u32)sp + 0x68;
    r31 = *(u32*)((u8*)r4 + 0x0);
    r4 = 0x1;
    __va_arg();
    tmp = *(u32*)((u8*)r3 + 0x0);
    r30 = 0x1;
    *(u32*)lbl_8047B270 = r31;
    *(u32*)lbl_8047B274 = tmp;
    r3 = r30;
    return;
    r3 = (u32)sp + 0x68;
    r4 = 0x1;
    __va_arg();
    r4 = r3;
    r3 = (u32)sp + 0x68;
    tmp = *(u32*)((u8*)r4 + 0x0);
    r4 = 0x1;
    *(u32*)(sp + 0x74) = tmp;
    __va_arg();
    r4 = r3;
    r3 = (u32)sp + 0x68;
    tmp = *(u32*)((u8*)r4 + 0x0);
    r4 = 0x1;
    *(u32*)(sp + 0x78) = tmp;
    __va_arg();
    r4 = r3;
    r3 = (u32)sp + 0x68;
    tmp = *(u32*)((u8*)r4 + 0x0);
    r4 = 0x1;
    *(u32*)(sp + 0x7C) = tmp;
    __va_arg();
    r4 = r3;
    r3 = (u32)sp + 0x68;
    tmp = *(u32*)((u8*)r4 + 0x0);
    r4 = 0x1;
    *(u32*)(sp + 0x80) = tmp;
    __va_arg();
    r4 = r3;
    r3 = (u32)sp + 0x68;
    tmp = *(u32*)((u8*)r4 + 0x0);
    r4 = 0x1;
    *(u32*)(sp + 0x84) = tmp;
    __va_arg();
    tmp = *(u32*)((u8*)r3 + 0x0);
    if (tmp != 0) {
        r3 = r31 + 0xc8;
        OSReport();
        r3 = r31 + 0xec;
        OSReport();
        r3 = r30;
        return;
    }
    r3 = (u32)sp + 0x74;
    r4 = 0x14;
    fn_801A69C0();
    tmp = 0x1;
    r30 = 0x1;
    *(u32*)lbl_8047B288 = tmp;
    r3 = r30;
    return;
    r3 = (u32)sp + 0x68;
    r4 = 0x1;
    __va_arg();
    tmp = *(u32*)((u8*)r3 + 0x0);
    if (tmp == 0) { r3 = r30; return; }
    *(u32*)lbl_80478C74 = tmp;
    r30 = 0x1;
    r3 = r30;
    return;
    r3 = r31 + 0xc8;
    OSReport();
    r3 = r31 + 0x128;
    OSReport();
    r3 = (u32)sp + 0x68;
    r4 = 0x1;
    __va_arg();
    tmp = *(u32*)((u8*)r3 + 0x0);
    if ((s32)tmp != 0) { r3 = r30; return; }
    r30 = 0x1;
    r3 = r30;
    return;
    r3 = r31 + 0xc8;
    OSReport();
    r3 = r31 + 0x160;
    OSReport();

    r3 = r30;

    return;
}

