/**
 * @file gba_conv2.c
 * @brief Pokemon conversion GBA-GCN (0x80089048-0x800895A4)
 *
 * Address range: 0x80089048 - 0x800895A4
 * Total functions: 2
 */

#include "dolphin/types.h"

/* ===== External function declarations ===== */
extern void fn_8008AE18();
extern void fn_8011F5C8();
extern void fn_80123FBC();
extern void fn_80196E10();
extern void fn_80265F14();
extern void* memset(void* dst, int val, u32 size);

/* ===== SDA globals ===== */
extern u8 lbl_8047A660;
extern u8 lbl_8047A664;
extern u8 lbl_8047A668;
extern u8 lbl_8047A66C;

/* ===== Rodata / data labels ===== */
extern u8 lbl_8026F568[];
extern u8 lbl_8026F574[];

/* ===== Forward declarations ===== */
s32 fn_80089048(void);
s32 fn_80089380(void);

/* ===== Function implementations ===== */


/* 0x80089048 | size: 0x338 */
s32 fn_80089048(void) {
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
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r28 = r3;
    r29 = r4;
    r30 = r5;
    if (r30 != 0x0) {
        r3 = r30;
        ((void(*)(void))fn_80123FBC)();
        r0 = r3 & 0xFF;
        if (r0 == 0x0) {
            r3 = 0x0;
            return;
    }
    }
    r11 = *(u32*)((u8*)r29 + 0x0);
    r12 = *(u32*)((u8*)r29 + 0x4);
    r31 = *(u32*)((u8*)r29 + 0x8);
    r5 = r11 & 0x0000FF00;
    r3 = r12 & 0x0000FF00;
    r10 = r11 & 0x00FF0000;
    r0 = r31 & 0x0000FF00;
    r7 = r12 & 0x00FF0000;
    r4 = r31 & 0x00FF0000;
    r9 = r11 << 24;
    r8 = r5 << 8;
    r6 = r12 << 24;
    r5 = r3 << 8;
    r3 = r31 << 24;
    r0 = r0 << 8;
    r10 = (u32)r10 >> 8;
    r8 = r9 | r8;
    r7 = (u32)r7 >> 8;
    r5 = r6 | r5;
    r4 = (u32)r4 >> 8;
    r0 = r3 | r0;
    r9 = (u32)r11 >> 24;
    r8 = r10 | r8;
    r6 = (u32)r12 >> 24;
    r5 = r7 | r5;
    r3 = (u32)r31 >> 24;
    r0 = r4 | r0;
    r7 = r9 | r8;
    r4 = r6 | r5;
    *(u32*)((u8*)r28 + 0x0) = r7;
    r0 = r3 | r0;
    r31 = *(u16*)((u8*)r29 + 0xE);
    *(u32*)((u8*)r28 + 0x4) = r4;
    *(u32*)((u8*)r28 + 0x8) = r0;
    if (r30 != 0x0) {
        r3 = r30;
        ((void(*)(void))fn_8011F5C8)();
        r0 = r3 & 0xFFFF;
        r0 = r0 << 16;
        r31 = r31 | r0;
    }
    r0 = r31 & 0x0000FF00;
    r4 = r31 & 0x00FF0000;
    r3 = r31 << 24;
    r5 = *(u16*)((u8*)r29 + 0xE);
    r0 = r0 << 8;
    r4 = (u32)r4 >> 8;
    r0 = r3 | r0;
    r3 = (u32)r31 >> 24;
    r0 = r4 | r0;
    r0 = r3 | r0;
    r31 = r28 + 0x10;
    *(u32*)((u8*)r28 + 0xC) = r0;
    r0 = 0x0;
    if (r5 == 0x32) goto L_8008917C;
    if (r5 != 0x1e) goto L_80089180;
L_8008917C: ;
    r0 = 0x1;
L_80089180: ;
    if ((s32)r0 == 0x0) {
        r3 = (u32)&lbl_8026F568;
        r5 = (u32)&lbl_8026F574;
        r3 = (u32)&lbl_8026F568;
        r4 = 0xb7;
        r5 = (u32)&lbl_8026F574;
        ((void(*)(void))fn_80196E10)();
    }
    r10 = r29;
    r3 = *(u16*)((u8*)r29 + 0xE);
    if ((s32)r3 <= 0x0) goto L_80089314;
    r0 = (u32)r3 >> 2;
    ctr_fn = (void(*)(void))r0;
    if (r0 == 0x0) goto L_800892CC;
    do {
        r9 = *(u16*)((u8*)r10 + 0x12);
        r11 = *(u16*)((u8*)r10 + 0x10);
        r10 = r10 + 0x4;
        r0 = r9 << 16;
        r9 = *(u16*)((u8*)r10 + 0x12);
        r11 = r11 | r0;
        r7 = r11 & 0x00FF0000;
        r0 = r9 << 16;
        r4 = r11 & 0x0000FF00;
        r8 = (u32)r11 >> 24;
        r5 = r11 << 24;
        r11 = *(u16*)((u8*)r10 + 0x10);
        r4 = r4 << 8;
        r10 = r10 + 0x4;
        r11 = r11 | r0;
        r6 = (u32)r7 >> 8;
        r0 = r5 | r4;
        r9 = *(u16*)((u8*)r10 + 0x12);
        r0 = r6 | r0;
        r4 = r11 & 0x0000FF00;
        r0 = r8 | r0;
        r7 = r11 & 0x00FF0000;
        *(u32*)((u8*)r31 + 0x0) = r0;
        r8 = (u32)r11 >> 24;
        r5 = r11 << 24;
        r4 = r4 << 8;
        r11 = *(u16*)((u8*)r10 + 0x10);
        r0 = r9 << 16;
        r10 = r10 + 0x4;
        r6 = (u32)r7 >> 8;
        r11 = r11 | r0;
        r0 = r5 | r4;
        r0 = r6 | r0;
        r9 = *(u16*)((u8*)r10 + 0x12);
        r0 = r8 | r0;
        r7 = r11 & 0x00FF0000;
        r4 = r11 & 0x0000FF00;
        r31 = r31 + 0x4;
        *(u32*)((u8*)r31 + 0x0) = r0;
        r8 = (u32)r11 >> 24;
        r5 = r11 << 24;
        r11 = *(u16*)((u8*)r10 + 0x10);
        r0 = r9 << 16;
        r4 = r4 << 8;
        r11 = r11 | r0;
        r6 = (u32)r7 >> 8;
        r0 = r5 | r4;
        r31 = r31 + 0x4;
        r4 = r11 & 0x0000FF00;
        r7 = r11 & 0x00FF0000;
        r0 = r6 | r0;
        r5 = r11 << 24;
        r0 = r8 | r0;
        r4 = r4 << 8;
        *(u32*)((u8*)r31 + 0x0) = r0;
        r6 = (u32)r7 >> 8;
        r0 = r5 | r4;
        r8 = (u32)r11 >> 24;
        r0 = r6 | r0;
        r31 = r31 + 0x4;
        r0 = r8 | r0;
        r10 = r10 + 0x4;
        *(u32*)((u8*)r31 + 0x0) = r0;
        r31 = r31 + 0x4;
    } while (--ctr != 0);
    r3 = r3 & 0x3;
    if (r0 == 0x0) goto L_80089314;
L_800892CC: ;
    ctr_fn = (void(*)(void))r3;
    do {
        r9 = *(u16*)((u8*)r10 + 0x12);
        r11 = *(u16*)((u8*)r10 + 0x10);
        r10 = r10 + 0x4;
        r0 = r9 << 16;
        r11 = r11 | r0;
        r4 = r11 & 0x0000FF00;
        r7 = r11 & 0x00FF0000;
        r5 = r11 << 24;
        r4 = r4 << 8;
        r8 = (u32)r11 >> 24;
        r6 = (u32)r7 >> 8;
        r0 = r5 | r4;
        r0 = r6 | r0;
        r0 = r8 | r0;
        *(u32*)((u8*)r31 + 0x0) = r0;
        r31 = r31 + 0x4;
    } while (--ctr != 0);
L_80089314: ;
    if (r30 != 0x0) {
        r3 = r30;
        r4 = r31;
        ((void(*)(void))fn_8008AE18)();
        r3 = r31 + 0x64;
        r4 = 0x0;
        r5 = 0xc;
        memset((void*)r3, (int)r4, (u32)r5);
        r28 = r31 + 0x64;
        r29 = 0x0;
        do {
            r3 = r29;
            ((void(*)(void))fn_80265F14)();
            *(u8*)((u8*)r28 + 0x0) = r3;
            r28 = r28 + 0x1;
            r29 = r29 + 0x1;
        } while ((s32)r29 < 0xb);
    }
    r3 = 0x1;

    return;
}

/* 0x80089380 | size: 0x224 */
s32 fn_80089380(void) {
    u8 sp[0x20];
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
    u32 r12 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    r29 = *(u32*)((u8*)r4 + 0x0);
    r31 = r4 + 0x10;
    r28 = *(u32*)((u8*)r4 + 0x4);
    r5 = 0x0;
    r3 = r29 & 0x0000FF00;
    tmp = *(u32*)((u8*)r4 + 0x8);
    r10 = r29 & 0x00FF0000;
    r9 = r29 << 24;
    r7 = r3 << 8;
    r8 = r28 & 0x0000FF00;
    r3 = *(u32*)((u8*)r4 + 0xC);
    r11 = r28 & 0x00FF0000;
    r6 = tmp & 0x0000FF00;
    r12 = (u32)r10 >> 8;
    r10 = r9 | r7;
    r9 = tmp & 0x00FF0000;
    r4 = r3 & 0x0000FF00;
    r7 = r3 & 0x00FF0000;
    r29 = (u32)r29 >> 24;
    r10 = r12 | r10;
    r12 = r29 | r10;
    r10 = r28 << 24;
    r8 = r8 << 8;
    r11 = (u32)r11 >> 8;
    r10 = r10 | r8;
    *(u32*)((u8*)r30 + 0x0) = r12;
    r12 = (u32)r28 >> 24;
    r8 = tmp << 24;
    r10 = r11 | r10;
    r6 = r6 << 8;
    r10 = r12 | r10;
    r9 = (u32)r9 >> 8;
    r8 = r8 | r6;
    r6 = r3 << 24;
    r4 = r4 << 8;
    *(u32*)((u8*)r30 + 0x4) = r10;
    r10 = (u32)tmp >> 24;
    tmp = r9 | r8;
    r8 = r10 | tmp;
    r7 = (u32)r7 >> 8;
    tmp = r6 | r4;
    r3 = (u32)r3 >> 24;
    tmp = r7 | tmp;
    *(u32*)((u8*)r30 + 0x8) = r8;
    r3 = r3 | tmp;
    tmp = (u32)r3 >> 16;
    *(u16*)((u8*)r30 + 0xC) = tmp;
    tmp = r3 & 0xFFFF;
    *(u16*)((u8*)r30 + 0xE) = tmp;
    tmp = *(u16*)((u8*)r30 + 0xE);
    if (tmp == 0x32) goto L_80089478;
    if (tmp != 0x1e) goto L_8008947C;
L_80089478:
    r5 = 0x1;
L_8008947C:
    if ((s32)r5 == 0) {
        r3 = (u32)&lbl_8026F568;
        r5 = (u32)&lbl_8026F574;
        r3 = (u32)&lbl_8026F568;
        r4 = 0x6f;
        r5 = (u32)&lbl_8026F574;
        ((void(*)(void))fn_80196E10)();
    }
    r6 = r30;
    r7 = 0x0;
    goto L_800894EC;
L_800894A8:
    r5 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r7 = r7 + 0x1;
    tmp = r5 & 0x0000FF00;
    r4 = r5 & 0x00FF0000;
    r3 = r5 << 24;
    r5 = (u32)r5 >> 24;
    tmp = tmp << 8;
    r4 = (u32)r4 >> 8;
    tmp = r3 | tmp;
    tmp = r4 | tmp;
    r3 = r5 | tmp;
    tmp = r3 & 0xFFFF;
    *(u16*)((u8*)r6 + 0x10) = tmp;
    tmp = (u32)r3 >> 16;
    *(u16*)((u8*)r6 + 0x12) = tmp;
    r6 = r6 + 0x4;
L_800894EC:
    tmp = *(u16*)((u8*)r30 + 0xE);
    if ((s32)r7 < (s32)tmp) goto L_800894A8;
    tmp = *(u32*)&lbl_8047A664;
    if ((s32)tmp != 0) {
        tmp = 0x0;
        *(u32*)((u8*)r30 + 0x0) = tmp;
        *(u32*)((u8*)r30 + 0x4) = tmp;
        *(u32*)&lbl_8047A664 = tmp;
    }
    tmp = *(u32*)&lbl_8047A660;
    if ((s32)tmp != 0) {
        r4 = *(u32*)((u8*)r30 + 0x0);
        tmp = 0x0;
        r3 = *(u32*)&lbl_8047A660;
        r3 = r4 + r3;
        *(u32*)((u8*)r30 + 0x0) = r3;
        r4 = *(u32*)((u8*)r30 + 0x4);
        r3 = *(u32*)&lbl_8047A660;
        r3 = r4 + r3;
        *(u32*)((u8*)r30 + 0x4) = r3;
        *(u32*)&lbl_8047A660 = tmp;
    }
    tmp = *(u32*)&lbl_8047A66C;
    if ((s32)tmp != 0) {
        tmp = 0x0;
        *(u32*)((u8*)r30 + 0x8) = tmp;
        *(u32*)&lbl_8047A66C = tmp;
    }
    tmp = *(u32*)&lbl_8047A668;
    if ((s32)tmp != 0) {
        r3 = *(u32*)((u8*)r30 + 0x8);
        tmp = 0x0;
        r3 = r3 | 0x10;
        *(u32*)((u8*)r30 + 0x8) = r3;
        *(u32*)&lbl_8047A668 = tmp;
    }
    r3 = 0x1;
    return;
}

