/**
 * @file effect_util.c
 * @brief Decompiled functions.
 *
 * Address range: 0x8013151C - 0x80137114
 */

#include "dolphin/types.h"

/* ===================================================================
 * Generated: 59 pattern-matched + 148 stubs
 * Range: 0x8013151C - 0x80137114
 * =================================================================== */

extern u32 lbl_803635D8;

extern u32 lbl_8047ADC8;
extern u32 lbl_8047ADCC;
extern u32 lbl_8047ADD0;
extern u32 lbl_8047ADE4;
extern u32 lbl_8047ADE8;
extern u32 lbl_8047ADEC;
extern u32 lbl_8047ADF0;
extern u32 lbl_8047ADF4;
extern u32 lbl_8047ADF8;
extern u32 lbl_8047ADFC;
extern u32 lbl_8047AE00;
extern u32 lbl_8047AE04;
extern u32 lbl_8047AE08;
extern u32 lbl_8047AE0C;
extern u32 lbl_8047AE20;
extern u32 lbl_8047AE24;
extern u32 lbl_8047AE28;
extern u32 lbl_8047AE2C;
extern u32 lbl_8047AE30;
extern u32 lbl_8047AE34;
extern u32 lbl_8047AE38;
extern u32 lbl_8047AE3C;
extern u32 lbl_8047AE40;
extern u32 lbl_8047AE5C;
extern u32 lbl_8047AE60;
extern u32 lbl_8047AE64;
extern u32 lbl_8047AE70;
extern u32 lbl_8047AE74;
extern u32 lbl_8047AE78;
extern u32 lbl_8047AE88;
extern u32 lbl_8047AE8C;
extern u32 lbl_8047AE98;
extern u32 lbl_8047AE9C;
extern u16 lbl_8047AEA2;
extern u8 lbl_8047AED0;

/* ===== Index lookup globals ===== */
extern u8 lbl_803635C0[];  /* effect table (BSS) */
extern u8 lbl_80363B88[];  /* trace fx table (BSS) */
extern u8 lbl_80363C00[];  /* trace table (BSS) */
extern u32 lbl_80478B98;  /* effect count (SDA) */
extern u32 lbl_80478BA0;  /* trace count (SDA) */

/* External function declarations */
extern u8   fn_80102620(u32 objID);
extern void fn_80102510(u32 arg1);

/* Forward declarations for converted functions */
void fn_80131CE8(u32 arg1, u32 arg2);
void fn_80133E6C(void);



/* 0x58 | fn_8013151C | leaf_multi_output */
void fn_8013151C(u32* out1) {
    if (out1 != NULL) { *out1 = *(u32*)((u8*)lbl_803635C0 + 0); }
}

/* 0x80131574 | 20 bytes | indexed_lookup */
u8 fn_80131574(u32 idx) {
    return ((u8*)lbl_803635D8)[idx];
}

/* 0x64 | fn_80131588 | guarded_call */
u32 fn_80131588(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    if (1 /* guard r0 != 0 */) { return 0; }
    if (1 /* guard r3 != 0 */) { return 0; }
    fn_801666BC();
    return 1;
}

/* 0x44 | fn_801315EC | guarded_call */
u32 fn_801315EC(void) {
    if (1 /* guard r0 != 0 */) { return 0; }
    if (0 /* guard r3 == 0 */) { return 0; }
    fn_80165A20();
    return 0;
}

/* 0x80131630 | 0x30 -- read byte from stream, store extsb to obj+0x43 if flag set */
u32 fn_80131630(void* obj) {
    u8* stream;
    if (*(u8*)((u8*)obj + 0x01) != 0) {
        stream = *(u8**)((u8*)obj + 0x30);
        *(u8*)((u8*)obj + 0x43) = (u8)(s8)*stream;
    }
    stream = *(u8**)((u8*)obj + 0x30);
    *(u32*)((u8*)obj + 0x30) = (u32)(stream + 1);
    return 0;
}

/* 0x80131660 | 0x30 -- read byte from stream, store extsb to obj+0x42 if flag set */
u32 fn_80131660(void* obj) {
    u8* stream;
    if (*(u8*)((u8*)obj + 0x01) != 0) {
        stream = *(u8**)((u8*)obj + 0x30);
        *(u8*)((u8*)obj + 0x42) = (u8)(s8)*stream;
    }
    stream = *(u8**)((u8*)obj + 0x30);
    *(u32*)((u8*)obj + 0x30) = (u32)(stream + 1);
    return 0;
}

/* 0x80131690 | 16 bytes | set_field_return */
u32 fn_80131690(void* obj) {
    *(u8*)((u8*)obj + 0x41) = 1;
    return 0;
}

/* 0x801316A0 | 0x8 | sda_getter */
u16 fn_801316A0(void) { return lbl_8047AEA2; }

/* 0x801316A8 | 0x28 -- calls fn_801FBD58 with lbl_8047AEA0 then fn_801FBD28 */
extern u16  lbl_8047AEA0;
extern void fn_801FBD58(u16 handle);
extern void fn_801FBD28(void);
void fn_801316A8(void) {
    fn_801FBD58(lbl_8047AEA0);
    fn_801FBD28();
}

/* 0x801316D0 | 0x8 | sda_getter */
u32 fn_801316D0(void) { return lbl_8047AE8C; }

/* 0x801316D8 | 0x8 | sda_getter */
u32 fn_801316D8(void) { return lbl_8047AE9C; }

/* 0x801316E0 | 0x8 | sda_getter */
u32 fn_801316E0(void) { return lbl_8047AE98; }

/* 0x801316E8 | 0x2C -- read byte from stream, store to obj+0x03 if flag clear */
u32 fn_801316E8(void* obj) {
    u8* stream;
    if (*(u8*)((u8*)obj + 0x01) == 0) {
        stream = *(u8**)((u8*)obj + 0x30);
        *(u8*)((u8*)obj + 0x03) = *stream;
    }
    stream = *(u8**)((u8*)obj + 0x30);
    *(u32*)((u8*)obj + 0x30) = (u32)(stream + 1);
    return 0;
}

/* 0x54 | fn_80131714 | generic */
u32 fn_80131714(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    fn_800FA064();
    return 0;
}

/* 0x80131768 | 0x2C -- read byte from stream, store to obj+0x02 if flag set */
extern void fn_80132834(void* table, u32 stride, u32 count, u32 type);
u32 fn_80131768(void* obj) {
    u8* stream;
    if (*(u8*)((u8*)obj + 0x01) != 0) {
        stream = *(u8**)((u8*)obj + 0x30);
        *(u8*)((u8*)obj + 0x02) = *stream;
    }
    stream = *(u8**)((u8*)obj + 0x30);
    *(u32*)((u8*)obj + 0x30) = (u32)(stream + 1);
    return 0;
}

/* 0x80131794 | 0x34 -- fn_80132834(lbl_80426FF0, 0x10, lbl_8047AE94, 4) */
extern u8  lbl_80426FF0[];
extern u32 lbl_8047AE94;
void fn_80131794(void) {
    fn_80132834(lbl_80426FF0, 0x10, lbl_8047AE94, 4);
}

/* 0x801317C8 | 0x34 -- fn_80132834(lbl_80427010, 0x10, lbl_8047AE68, 5) */
extern u8  lbl_80427010[];
extern u32 lbl_8047AE68;
void fn_801317C8(void) {
    fn_80132834(lbl_80427010, 0x10, lbl_8047AE68, 5);
}

/* 0x801317FC | 0x28 -- calls fn_8011E778(lbl_8047AE90) then fn_8011E760 */
extern u16  lbl_8047AE90;
extern void fn_8011E778(u16 handle);
extern void fn_8011E760(void);
void fn_801317FC(void) {
    fn_8011E778(lbl_8047AE90);
    fn_8011E760();
}

/* 0x80131824 | 0x8 | sda_getter */
u32 fn_80131824(void) { return lbl_8047AE88; }

/* 0x8013182C | 0x208 */
void fn_8013182C(void) {
    extern u8 lbl_80427030[];
    extern u8 lbl_80427050[];
    extern u8 lbl_8047AE84[];
    extern u8 lbl_8047AEA8[];
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = (0x91a3 << 16);
    r4 = *(u32*)lbl_8047AE84;
    /* subi r0, r3, 0x4c3b */;
    r3 = (0x8889 << 16);
    r0 = (u32)((u64)r0 * (u64)r4 >> 32);
    r29 = 0x0;
    /* subi r3, r3, 0x7777 */;
    r30 = (u32)r0 >> 11;
    r0 = r30 * 0xe10;
    r0 = r4 - r0;
    r0 = (u32)((u64)r3 * (u64)r0 >> 32);
    r31 = (u32)r0 >> 5;
    if ((u32)r30 >= (u32)0x64) {
        r3 = (0x51ec << 16);
        r5 = (u32)lbl_80427030;
        /* subi r0, r3, 0x7ae1 */;
        r4 = 0x10;
        r0 = (u32)((u64)r0 * (u64)r30 >> 32);
        r3 = (u32)lbl_80427030;
        r6 = 0x0;
        r5 = (u32)r0 >> 5;
        ((void(*)(void))fn_80132834)();
        r4 = (0x51ec << 16);
        *(u32*)lbl_8047AEA8 = r3;
        /* subi r0, r4, 0x7ae1 */;
        r5 = 0x0;
        r0 = (u32)((u64)r0 * (u64)r30 >> 32);
        r4 = (u32)lbl_80427050;
        r6 = *(u16*)((u8*)r3 + 0x0);
        r5 = r5 << 1;
        r3 = (u32)lbl_80427050;
        *(u16*)(r3 + r5) = r6;
        r0 = (u32)r0 >> 5;
        r29 = 0x1;
        r0 = r0 * 0x64;
        r30 = r30 - r0;
    }
    r3 = (0xcccd << 16);
    r5 = (u32)lbl_80427030;
    /* subi r0, r3, 0x3333 */;
    r4 = 0x10;
    r0 = (u32)((u64)r0 * (u64)r30 >> 32);
    r3 = (u32)lbl_80427030;
    r6 = 0x0;
    r5 = (u32)r0 >> 3;
    ((void(*)(void))fn_80132834)();
    r4 = (0xcccd << 16);
    r6 = r29;
    /* subi r0, r4, 0x3333 */;
    *(u32*)lbl_8047AEA8 = r3;
    r0 = (u32)((u64)r0 * (u64)r30 >> 32);
    r5 = (u32)lbl_80427050;
    r7 = *(u16*)((u8*)r3 + 0x0);
    r4 = (u32)lbl_80427030;
    r6 = r6 << 1;
    r5 = (u32)lbl_80427050;
    r0 = (u32)r0 >> 3;
    *(u16*)(r5 + r6) = r7;
    r0 = r0 * 0xa;
    r3 = (u32)lbl_80427030;
    r29 = r29 + 0x1;
    r4 = 0x10;
    r5 = r30 - r0;
    r6 = 0x0;
    ((void(*)(void))fn_80132834)();
    r4 = (0xcccd << 16);
    r6 = r29;
    *(u32*)lbl_8047AEA8 = r3;
    /* subi r0, r4, 0x3333 */;
    r0 = (u32)((u64)r0 * (u64)r31 >> 32);
    r29 = r29 + 0x1;
    r8 = *(u16*)((u8*)r3 + 0x0);
    r5 = (u32)lbl_80427050;
    r7 = r6 << 1;
    r6 = (u32)lbl_80427050;
    r4 = r29;
    r3 = (u32)lbl_80427030;
    *(u16*)(r6 + r7) = r8;
    r4 = r4 << 1;
    r5 = 0x3a;
    r3 = (u32)lbl_80427030;
    *(u16*)(r6 + r4) = r5;
    r29 = r29 + 0x1;
    r5 = (u32)r0 >> 3;
    r4 = 0x10;
    r6 = 0x0;
    ((void(*)(void))fn_80132834)();
    r4 = (0xcccd << 16);
    r6 = r29;
    /* subi r0, r4, 0x3333 */;
    *(u32*)lbl_8047AEA8 = r3;
    r0 = (u32)((u64)r0 * (u64)r31 >> 32);
    r5 = (u32)lbl_80427050;
    r7 = *(u16*)((u8*)r3 + 0x0);
    r4 = (u32)lbl_80427030;
    r6 = r6 << 1;
    r5 = (u32)lbl_80427050;
    r0 = (u32)r0 >> 3;
    *(u16*)(r5 + r6) = r7;
    r0 = r0 * 0xa;
    r3 = (u32)lbl_80427030;
    r29 = r29 + 0x1;
    r4 = 0x10;
    r5 = r31 - r0;
    r6 = 0x0;
    ((void(*)(void))fn_80132834)();
    *(u32*)lbl_8047AEA8 = r3;
    r0 = r29;
    r4 = (u32)lbl_80427050;
    r29 = r29 + 0x1;
    r5 = *(u16*)((u8*)r3 + 0x0);
    r3 = (u32)lbl_80427050;
    r4 = r0 << 1;
    r0 = r29 << 1;
    *(u16*)(r3 + r4) = r5;
    r4 = 0x0;
    *(u16*)(r3 + r0) = r4;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}

/* 0x80131A34 | 0x34 -- fn_80132834(lbl_80427070, 0x10, lbl_8047AE80, 4) */
extern u8 lbl_80427070[];
extern u32 lbl_8047AE80;
void fn_80131A34(void) {
    fn_80132834(lbl_80427070, 0x10, lbl_8047AE80, 4);
}

/* 0x80131A68 | 0x34 -- fn_80132834(lbl_80427090, 0x10, lbl_8047AE6C, 2) */
extern u8 lbl_80427090[];
extern u32 lbl_8047AE6C;
void fn_80131A68(void) {
    fn_80132834(lbl_80427090, 0x10, lbl_8047AE6C, 2);
}

/* 0x80131A9C | 0x34 -- fn_80132834(lbl_804270B0, 0x10, lbl_8047AE68, 2) */
extern u8 lbl_804270B0[];
void fn_80131A9C(void) {
    fn_80132834(lbl_804270B0, 0x10, lbl_8047AE68, 2);
}

/* 0x80131AD0 | 0x34 -- fn_80132834(lbl_804270D0, 0x10, lbl_8047AE68, 3) */
extern u8 lbl_804270D0[];
void fn_80131AD0(void) {
    fn_80132834(lbl_804270D0, 0x10, lbl_8047AE68, 3);
}

/* 0x80131B04 | 0x34 -- fn_80132834(lbl_804270F0, 0x10, lbl_8047AE68, 3) */
extern u8 lbl_804270F0[];
void fn_80131B04(void) {
    fn_80132834(lbl_804270F0, 0x10, lbl_8047AE68, 3);
}

/* 0x80131B38 | 0x34 -- fn_80132834(lbl_80427110, 0x10, lbl_8047AE6C, 1) */
extern u8 lbl_80427110[];
void fn_80131B38(void) {
    fn_80132834(lbl_80427110, 0x10, lbl_8047AE6C, 1);
}

/* 0x80131B6C | 0x34 -- fn_80132834(lbl_80427130, 0x10, lbl_8047AE68, 1) */
extern u8 lbl_80427130[];
void fn_80131B6C(void) {
    fn_80132834(lbl_80427130, 0x10, lbl_8047AE68, 1);
}

/* 0x80131BA0 | 0x8 | return_const */
u32 fn_80131BA0(void) { return 0; }

/* 0x80131BA8 | 0x8 | return_const */
u32 fn_80131BA8(void) { return 0; }

/* 0x80131BB0 | 0x8 | sda_getter */
u32 fn_80131BB0(void) { return lbl_8047AE40; }

/* 0x80131BB8 | 0x8 | sda_getter */
u32 fn_80131BB8(void) { return lbl_8047AE3C; }

/* 0x80131BC0 | 0x8 | sda_getter */
u32 fn_80131BC0(void) { return lbl_8047AE38; }

/* 0x80131BC8 | 0x8 | sda_getter */
u32 fn_80131BC8(void) { return lbl_8047AE34; }

/* 0x80131BD0 | 0x8 | sda_getter */
u32 fn_80131BD0(void) { return lbl_8047AE30; }

/* 0x80131BD8 | 0x8 | sda_getter */
u32 fn_80131BD8(void) { return lbl_8047AE2C; }

/* 0x80131BE0 | 0x8 | sda_getter */
u32 fn_80131BE0(void) { return lbl_8047AE28; }

/* 0x80131BE8 | 0x8 | sda_getter */
u32 fn_80131BE8(void) { return lbl_8047AE24; }

/* 0x80131BF0 | 0x8 | sda_getter */
u32 fn_80131BF0(void) { return lbl_8047AE20; }

/* 0x80131BF8 | 0x28 -- fn_80131CE8(lbl_8047AE4C, 2) */
extern u32 lbl_8047AE4C;
void fn_80131BF8(void) {
    fn_80131CE8(lbl_8047AE4C, 2);
}

/* 0x80131C20 | 0x28 -- fn_80131CE8(lbl_8047AE48, 1) */
extern u32 lbl_8047AE48;
void fn_80131C20(void) {
    fn_80131CE8(lbl_8047AE48, 1);
}

/* 0x80131C48 | 0x28 -- fn_80131CE8(lbl_8047AE44, 0) */
extern u32 lbl_8047AE44;
void fn_80131C48(void) {
    fn_80131CE8(lbl_8047AE44, 0);
}

/* 0x80131C70 | 0x28 -- fn_80131CE8(lbl_8047AE1C, 2) */
extern u32 lbl_8047AE1C;
void fn_80131C70(void) {
    fn_80131CE8(lbl_8047AE1C, 2);
}

/* 0x80131C98 | 0x28 -- fn_80131CE8(lbl_8047AE18, 1) */
extern u32 lbl_8047AE18;
void fn_80131C98(void) {
    fn_80131CE8(lbl_8047AE18, 1);
}

/* 0x80131CC0 | 0x28 -- fn_80131CE8(lbl_8047AE14, 0) */
extern u32 lbl_8047AE14;
void fn_80131CC0(void) {
    fn_80131CE8(lbl_8047AE14, 0);
}

/* 0x80131CE8 | 0x21C */
void fn_80131CE8(u32 arg1, u32 arg2) {
    extern void fn_800FA280();
    extern void fn_80132A38();
    extern void fn_801F0058();
    extern void fn_801F025C();
    extern void fn_801F18DC();
    extern void fn_801F4354();
    extern void fn_801F54A4();
    extern void fn_801F7258();
    extern void fn_801F8100();
    extern void fn_801FA524();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r24 = r3;
    r31 = r4;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r30 = r3 & 0xFFFF;
    r4 = r24;
    r3 = 0x2;
    fn_801F025C();
    r29 = r3;
    r4 = r24;
    r3 = 0x0;
    fn_801F4354();
    r26 = 0x0;
    r28 = r3;
    r25 = 0x0;
    goto L_80131DB4;
L_80131D44: ;
    r3 = r29;
    r4 = r25;
    fn_801F7258();
    r27 = r3;
    if ((u32)r27 == (u32)0x0) goto L_80131DB0;
    fn_801FA524();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80131DB0;
    r0 = r26 & 0xFFFF;
    if ((u32)r0 != (u32)0x0) goto L_80131D90;
    r3 = r27;
    fn_801F8100();
    r4 = r3;
    r3 = 0x4d;
    fn_80132A38();
    goto L_80131DAC;
L_80131D90: ;
    if ((u32)r0 != (u32)0x1) goto L_80131DAC;
    r3 = r27;
    fn_801F8100();
    r4 = r3;
    r3 = 0x57;
    fn_80132A38();
L_80131DAC: ;
    r26 = r26 + 0x1;
L_80131DB0: ;
    r25 = r25 + 0x1;
L_80131DB4: ;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_80131D44;
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if (((u32)r0 == (u32)0x1) && ((u32)r28 != (u32)0x0)) {

        r0 = r26 & 0xFFFF;
        if ((u32)r0 <= (u32)0x1) {
            r3 = r28;
            fn_801F8100();
            r4 = r3;
            r3 = 0x4d;
            fn_80132A38();
            r0 = r31 & 0xFF;
            if ((u32)r0 == (u32)0x0) {
                r3 = 0x7722;
                fn_800FA280();
                return;
            }
            if ((u32)r0 == (u32)0x1) {
                r3 = 0x7725;
                fn_800FA280();
                return;
            }
            r3 = 0x7727;
            fn_800FA280();
            return;
        }
        r0 = r31 & 0xFF;
        if ((u32)r0 == (u32)0x0) {
            r3 = 0x7724;
            fn_800FA280();
            return;
        }
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x7726;
            fn_800FA280();
            return;
        }
        r3 = 0x7728;
        fn_800FA280();
        return;
    }
    r3 = r24;
    r4 = r30;
    fn_801F0058();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = r31 & 0xFF;
        if ((u32)r0 == (u32)0x0) {
            r3 = 0x768a;
            fn_800FA280();
            return;
        }
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x768c;
            fn_800FA280();
            return;
        }
        r3 = 0x7688;
        fn_800FA280();
        return;
    }
    r0 = r31 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r3 = 0x7689;
        fn_800FA280();
        return;
    }
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x768b;
        fn_800FA280();
        return;
    }
    r3 = 0x7687;
    fn_800FA280();

    return;
}

/* 0x80131F04 | 0x98 */
void fn_80131F04(void) {
    extern u8 lbl_8047AE10[];
    extern void fn_800FA280(u32);
    extern void fn_80132A38(u32, u32);
    extern u32 fn_801F18DC(u32);
    extern u32 fn_801F4354(u32, u32);
    extern u32 fn_801F8100(u32);
    extern u32 fn_802037DC(u32);
    u32 val = *(u32*)lbl_8047AE10;
    u32 result = fn_801F4354(0, val);
    u32 flag = fn_801F18DC(0) & 0xFF;

    if (flag == 1 && result != 0) {
        u32 info = fn_801F8100(result);
        fn_80132A38(0x4D, info);
        info = fn_802037DC(val);
        fn_80132A38(0x57, info);
        fn_800FA280(0x7721);
    } else {
        fn_802037DC(val);
    }
}

/* 0x80131F9C | 0x8 | sda_getter */
u32 fn_80131F9C(void) { return lbl_8047AE0C; }

/* 0x80131FA4 | 0x8 | sda_getter */
u32 fn_80131FA4(void) { return lbl_8047AE08; }

/* 0x80131FAC | 0x8 | sda_getter */
u32 fn_80131FAC(void) { return lbl_8047AE04; }

/* 0x80131FB4 | 0x8 | sda_getter */
u32 fn_80131FB4(void) { return lbl_8047AE00; }

/* 0x80131FBC | 0x8 | sda_getter */
u32 fn_80131FBC(void) { return lbl_8047ADFC; }

/* 0x80131FC4 | 0x8 | sda_getter */
u32 fn_80131FC4(void) { return lbl_8047ADF8; }

/* 0x80131FCC | 0x8 | sda_getter */
u32 fn_80131FCC(void) { return lbl_8047ADF4; }

/* 0x80131FD4 | 0x8 | sda_getter */
u32 fn_80131FD4(void) { return lbl_8047ADF0; }

/* 0x80131FDC | 0x8 | sda_getter */
u32 fn_80131FDC(void) { return lbl_8047ADEC; }

/* 0x80131FE4 | 0x8 | sda_getter */
u32 fn_80131FE4(void) { return lbl_8047ADE8; }

/* 0x80131FEC | 0x8 | sda_getter */
u32 fn_80131FEC(void) { return lbl_8047ADE4; }

/* 0x80131FF4 | 0x98 */
void fn_80131FF4(void) {
    extern u8 lbl_8047ADE0[];
    extern void fn_800FA280(u32);
    extern void fn_80132A38(u32, u32);
    extern u32 fn_801F18DC(u32);
    extern u32 fn_801F4354(u32, u32);
    extern u32 fn_801F8100(u32);
    extern u32 fn_802037DC(u32);
    u32 val = *(u32*)lbl_8047ADE0;
    u32 result = fn_801F4354(0, val);
    u32 flag = fn_801F18DC(0) & 0xFF;

    if (flag == 1 && result != 0) {
        u32 info = fn_801F8100(result);
        fn_80132A38(0x4D, info);
        info = fn_802037DC(val);
        fn_80132A38(0x57, info);
        fn_800FA280(0x7721);
    } else {
        fn_802037DC(val);
    }
}

/* 0x8013208C | 0x98 */
void fn_8013208C(void) {
    extern u8 lbl_8047ADDC[];
    extern void fn_800FA280(u32);
    extern void fn_80132A38(u32, u32);
    extern u32 fn_801F18DC(u32);
    extern u32 fn_801F4354(u32, u32);
    extern u32 fn_801F8100(u32);
    extern u32 fn_802037DC(u32);
    u32 val = *(u32*)lbl_8047ADDC;
    u32 result = fn_801F4354(0, val);
    u32 flag = fn_801F18DC(0) & 0xFF;

    if (flag == 1 && result != 0) {
        u32 info = fn_801F8100(result);
        fn_80132A38(0x4D, info);
        info = fn_802037DC(val);
        fn_80132A38(0x57, info);
        fn_800FA280(0x7721);
    } else {
        fn_802037DC(val);
    }
}

/* 0x80132124 | 0x98 */
void fn_80132124(void) {
    extern u8 lbl_8047ADD8[];
    extern void fn_800FA280(u32);
    extern void fn_80132A38(u32, u32);
    extern u32 fn_801F18DC(u32);
    extern u32 fn_801F4354(u32, u32);
    extern u32 fn_801F8100(u32);
    extern u32 fn_802037DC(u32);
    u32 val = *(u32*)lbl_8047ADD8;
    u32 result = fn_801F4354(0, val);
    u32 flag = fn_801F18DC(0) & 0xFF;

    if (flag == 1 && result != 0) {
        u32 info = fn_801F8100(result);
        fn_80132A38(0x4D, info);
        info = fn_802037DC(val);
        fn_80132A38(0x57, info);
        fn_800FA280(0x7721);
    } else {
        fn_802037DC(val);
    }
}

/* 0x801321BC | 0x98 */
void fn_801321BC(void) {
    extern u8 lbl_8047ADD4[];
    extern void fn_800FA280(u32);
    extern void fn_80132A38(u32, u32);
    extern u32 fn_801F18DC(u32);
    extern u32 fn_801F4354(u32, u32);
    extern u32 fn_801F8100(u32);
    extern u32 fn_802037DC(u32);
    u32 val = *(u32*)lbl_8047ADD4;
    u32 result = fn_801F4354(0, val);
    u32 flag = fn_801F18DC(0) & 0xFF;

    if (flag == 1 && result != 0) {
        u32 info = fn_801F8100(result);
        fn_80132A38(0x4D, info);
        info = fn_802037DC(val);
        fn_80132A38(0x57, info);
        fn_800FA280(0x7721);
    } else {
        fn_802037DC(val);
    }
}

/* 0x80132254 | 0x8 | sda_getter */
u32 fn_80132254(void) { return lbl_8047ADD0; }

/* 0x8013225C | 0x8 | sda_getter */
u32 fn_8013225C(void) { return lbl_8047ADCC; }

/* 0x80132264 | 0x8 | sda_getter */
u32 fn_80132264(void) { return lbl_8047ADC8; }

/* 0x8013226C | 0x28 -- calls fn_8011CA34(lbl_8047AE7C) then fn_8011CA1C */
extern u16  lbl_8047AE7C;
extern void fn_8011CA34(u16 handle);
extern void fn_8011CA1C(void);
void fn_8013226C(void) {
    fn_8011CA34(lbl_8047AE7C);
    fn_8011CA1C();
}

/* 0x80132294 | 0x8 | sda_getter */
u32 fn_80132294(void) { return lbl_8047AE78; }

/* 0x8013229C | 0x8 | sda_getter */
u32 fn_8013229C(void) { return lbl_8047AE74; }

/* 0x801322A4 | 0x8 | sda_getter */
u32 fn_801322A4(void) { return lbl_8047AE70; }

/* 0x801322AC | 0x34 -- fn_80132834(lbl_80427150, 0x10, lbl_8047AE6C, 0) */
extern u8 lbl_80427150[];
void fn_801322AC(void) {
    fn_80132834(lbl_80427150, 0x10, lbl_8047AE6C, 0);
}

/* 0x801322E0 | 0x34 -- fn_80132834(lbl_80427170, 0x10, lbl_8047AE68, 0) */
extern u8 lbl_80427170[];
void fn_801322E0(void) {
    fn_80132834(lbl_80427170, 0x10, lbl_8047AE68, 0);
}

/* 0x80132314 | 0x8 | sda_getter */
u32 fn_80132314(void) { return lbl_8047AE64; }

/* 0x8013231C | 0x8 | sda_getter */
u32 fn_8013231C(void) { return lbl_8047AE60; }

/* 0x80132324 | 0x8 | sda_getter */
u32 fn_80132324(void) { return lbl_8047AE5C; }

/* 0x8013232C | 0x34 -- fn_80132834(lbl_80427190, 0x10, lbl_8047AE58, 0) */
extern u8  lbl_80427190[];
extern u32 lbl_8047AE58;
void fn_8013232C(void) {
    fn_80132834(lbl_80427190, 0x10, lbl_8047AE58, 0);
}

/* 0x80132360 | 0x34 -- fn_80132834(lbl_804271B0, 0x10, lbl_8047AE54, 0) */
extern u8  lbl_804271B0[];
extern u32 lbl_8047AE54;
void fn_80132360(void) {
    fn_80132834(lbl_804271B0, 0x10, lbl_8047AE54, 0);
}

/* 0x80132394 | 0x34 -- calls fn_801440A0(lbl_8047AE52) then fn_80144088, default to 0x2B6E */
extern u16  lbl_8047AE52;
extern void fn_801440A0(u16 handle);
extern u32  fn_80144088(void);
u32 fn_80132394(void) {
    u32 result;
    fn_801440A0(lbl_8047AE52);
    result = fn_80144088();
    if (result == 0) { result = 0x2B6E; }
    return result;
}

/* 0x801323C8 | 0x34 -- calls fn_801440A0(lbl_8047AE50) then fn_80144088, default to 0x2B6E */
extern u16 lbl_8047AE50;
u32 fn_801323C8(void) {
    u32 result;
    fn_801440A0(lbl_8047AE50);
    result = fn_80144088();
    if (result == 0) { result = 0x2B6E; }
    return result;
}

/* 0x801323FC | 0x2C -- fn_80129280(0, 2) then fn_8012A8D4 */
extern u32  fn_80129280(u32 side, u32 slotType);
extern u32  fn_8012A8D4(void);
u32 fn_801323FC(void) {
    fn_80129280(0, 2);
    return fn_8012A8D4();
}

/* 0x80132428 | 0x2C -- fn_80129280(0, 2) then fn_8012AC54 */
extern u32 fn_8012AC54(void);
u32 fn_80132428(void) {
    fn_80129280(0, 2);
    return fn_8012AC54();
}

/* 0x78 | fn_80132454 | generic */
u32 fn_80132454(void) {
    return 0;
}

/* 0x801324CC | 0xA4 -- read color index from stream, look up RGBA, apply */
void fn_801324CC(void* obj) {
    extern u8 lbl_80478E88[];
    extern u8 lbl_80478E8C[];
    extern void fn_800FA160(void*);
    u8* stream;
    u8 idx;
    u8* colorPtr;
    u32 maxIdx;

    if (*(u8*)((u8*)obj + 0x1) != 0) {
        stream = *(u8**)((u8*)obj + 0x30);
        idx = *stream;
        maxIdx = *(u32*)(*(u32*)lbl_80478E88);
        if (idx >= maxIdx) {
            idx = 0;
        }
        colorPtr = (u8*)(*(u32*)lbl_80478E8C + (u32)idx * 4);
        *(u32*)((u8*)obj + 0x24) = ((u32)colorPtr[0] << 24) |
                                    ((u32)colorPtr[1] << 16) |
                                    ((u32)colorPtr[2] << 8) |
                                    (u32)colorPtr[3];
        fn_800FA160(obj);
    }
    /* Advance stream pointer */
    *(u32*)((u8*)obj + 0x30) = *(u32*)((u8*)obj + 0x30) + 1;
}

/* 0x54 | fn_80132570 | generic */
u32 fn_80132570(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    fn_800FA160();
    return 0;
}

/* 0x68 | fn_801325C4 | two_call_arg_check */
void fn_801325C4(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    if (arg1 != 0) { return; }
    fn_800FA1BC();
    /* store u16 to offset 0x20 */
    /* store u16 to offset 0x20 */
    /* store u32 to offset 0x30 */
    fn_800FA1BC();
}

/* 0x8013262C | 16 bytes | set_field_return */
u32 fn_8013262C(void* obj) {
    *(u8*)((u8*)obj + 0x4B) = 0;
    return 0;
}

/* 0x8013263C | 16 bytes | set_field_return */
u32 fn_8013263C(void* obj) {
    *(u8*)((u8*)obj + 0x4B) = 2;
    return 0;
}

/* 0x44 | fn_8013264C | generic */
u32 fn_8013264C(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    fn_800FAA98();
    return 0;
}

/* 0x80132690 | 0xCC -- effect tick with flag-based logic */
u32 fn_80132690(void* obj) {
    extern void fn_80166A28(u32);
    u8* p = (u8*)obj;

    /* Check flag bit 1 at offset 0x44 */
    if (p[0x44] & 0x02) {
        p[0x45] = 1;
    }
    /* If scene object 0xA is active, clear the flag */
    if ((fn_80102620(0x0A) & 0xFF) != 0) {
        p[0x45] = 0;
    }

    if (p[0x01] == 0) {
        /* Not active */
        if (p[0x45] != 0) {
            p[0x45] = 0;
            *(u32*)(p + 0x2C) = *(u32*)(p + 0x30);
            if (!(p[0x44] & 0x02)) {
                fn_80166A28(0x24);
            }
        } else {
            /* No trigger: keep stream pos, mark done */
            *(u8*)(p + 0x46) = 1;
        }
    } else {
        /* Active: copy position floats */
        *(f32*)(p + 0x0C) = *(f32*)(p + 0x04);
        *(f32*)(p + 0x10) = *(f32*)(p + 0x08);
    }
    return 1;
}

/* 0x8013275C | 0x84 */
void fn_8013275C(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r31 = 0;

    r31 = r3;
    r0 = *(u8*)((u8*)r31 + 0x44);
    r0 = r0 & 0x00000002;
    if ((s32)r0 != (s32)0x0) {
        r0 = 0x1;
        *(u8*)((u8*)r31 + 0x45) = r0;
    }
    r3 = 0xa;
    ((void(*)(void))fn_80102620)();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r0 = 0x0;
        *(u8*)((u8*)r31 + 0x45) = r0;
    }
    r0 = *(u8*)((u8*)r31 + 0x45);
    if ((u32)r0 != (u32)0x0) {
        r0 = 0x0;
        *(u8*)((u8*)r31 + 0x45) = r0;
    } else {

        r3 = *(u32*)((u8*)r31 + 0x30);
        *(u32*)((u8*)r31 + 0x30) = r0;
    }
    r3 = 0x1;
    r31 = *(u32*)(sp + 0xC);
    return;
}

/* 0x54 | fn_801327E0 | framed_no_calls */
u32 fn_801327E0(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_8047D0E0 */
    return 0;
}

/* 0x80132834 | 0x204 */
void fn_80132834(void* table, u32 stride, u32 count, u32 type) {
    extern u8 lbl_803635F0[];
    extern u8 lbl_80363610[];
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
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r0 = r9 << 1;
    r4 = 0x0;
    *(u16*)(r3 + r0) = r4;
    r0 = 0x0;
    r4 = 0xa;
    r5 = 0x0;
    r7 = 0x1;
    if ((s32)r6 == (s32)0x3) goto L_801328B4;
    if ((s32)r6 >= (s32)0x3) goto L_80132884;
    if ((s32)r6 == (s32)0x1) goto L_801328BC;
    if ((s32)r6 >= (s32)0x1) goto L_801328AC;
    if ((s32)r6 >= (s32)0x0) goto L_80132890;
    goto L_801328BC;
L_80132884: ;
    if ((s32)r6 == (s32)0x5) goto L_80132890;
    goto L_801328BC;
L_80132890: ;
    r8 = *(u32*)(sp + 0x8);
    if ((s32)r8 >= (s32)0x0) goto L_801328BC;
    r8 = -r8;
    r0 = 0x1;
    goto L_801328BC;
L_801328AC: ;
    r7 = 0xa;
    goto L_801328BC;
L_801328B4: ;
    r4 = 0x10;
    r7 = 0x8;
L_801328BC: ;
    if ((s32)r6 == (s32)0x5) goto L_801328C8;
    goto L_801328D4;
L_801328C8: ;
    r8 = (u32)lbl_80363610;
    r8 = (u32)lbl_80363610;
    goto L_801328DC;
L_801328D4: ;
    r8 = (u32)lbl_803635F0;
    r8 = (u32)lbl_803635F0;
L_801328DC: ;
    r10 = r9 << 1;
    while (1) {
        r11 = *(u32*)(sp + 0x8);
        if ((u32)r11 == (u32)0x0) break;
        if ((s32)r6 == (s32)0x4) {
            if ((s32)r5 != (s32)0x0) {
                r11 = (0x5555 << 16);
                r11 = r11 + 0x5556;
                r12 = (s32)((s64)r11 * (s64)r5 >> 32);
                r11 = (u32)r12 >> 31;
                r11 = r12 + r11;
                r11 = r11 * 0x3;
                r11 = r5 - r11;
                if ((s32)r11 == (s32)0x0) {
                    r11 = 0x2c;
                    *(u16*)(r3 + r10) = r11;
        }
        }
        }
        r31 = *(u32*)(sp + 0x8);
        r5 = r5 + 0x1;
        r12 = (u32)r31 / (u32)r4;
        r11 = r12 * r4;
        r11 = r31 - r11;
        r11 = r11 << 1;
        r11 = *(u16*)(r8 + r11);
        *(u16*)(r3 + r10) = r11;


    }
    r6 = r9 << 1;
    r5 = r7 - r5;
    if ((s32)r5 >= (s32)r7) goto L_80132A08;
    r4 = (u32)r5 >> 3;
    ctr_fn = (void(*)(void))r4;
    if ((u32)r4 == (u32)0x0) goto L_801329F0;
    do {
        r4 = *(u16*)((u8*)r8 + 0x0);
        *(u16*)(r3 + r6) = r4;
        r4 = *(u16*)((u8*)r8 + 0x0);
        *(u16*)(r3 + r6) = r4;
        r4 = *(u16*)((u8*)r8 + 0x0);
        *(u16*)(r3 + r6) = r4;
        r4 = *(u16*)((u8*)r8 + 0x0);
        *(u16*)(r3 + r6) = r4;
        r4 = *(u16*)((u8*)r8 + 0x0);
        *(u16*)(r3 + r6) = r4;
        r4 = *(u16*)((u8*)r8 + 0x0);
        *(u16*)(r3 + r6) = r4;
        r4 = *(u16*)((u8*)r8 + 0x0);
        *(u16*)(r3 + r6) = r4;
        r4 = *(u16*)((u8*)r8 + 0x0);
        *(u16*)(r3 + r6) = r4;
    } while (--ctr != 0);
    r5 = r5 & 0x7;
    if ((u32)r4 == (u32)0x0) goto L_80132A08;
L_801329F0: ;
    ctr_fn = (void(*)(void))r5;
    do {
        r4 = *(u16*)((u8*)r8 + 0x0);
        *(u16*)(r3 + r6) = r4;
    } while (--ctr != 0);
L_80132A08: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r4 = 0x2d;
        r0 = r9 << 1;
        *(u16*)(r3 + r0) = r4;
    }
    r0 = r9 << 1;
    r3 = r3 + r0;
    r31 = *(u32*)(sp + 0x1C);
    return;
}

/* 0x80132A38 | 0x210 */
void fn_80132A38(void) {
    extern u8 lbl_8047ADD4[];
    extern u8 lbl_8047ADD8[];
    extern u8 lbl_8047ADDC[];
    extern u8 lbl_8047ADE0[];
    extern u8 lbl_8047AE10[];
    extern u8 lbl_8047AE84[];
    extern u8 lbl_8047AEA4[];
    extern u8 jumptable_80363630[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    void (*ctr_fn)(void) = 0;

    /* subi r0, r3, 0xd */;
    if ((u32)r0 > (u32)0x50) return;
    r3 = (u32)jumptable_80363630;
    r0 = r0 << 2;
    r3 = (u32)jumptable_80363630;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r0 = r4 & 0xFFFF;
    *(u16*)&lbl_8047AE50 = r0;
    return;
    r0 = r4 & 0xFFFF;
    *(u16*)&lbl_8047AE52 = r0;
    return;
    *(u32*)&lbl_8047AE54 = r4;
    return;
    *(u32*)&lbl_8047AE58 = r4;
    return;
    *(u32*)&lbl_8047AE5C = r4;
    return;
    *(u32*)&lbl_8047AE60 = r4;
    return;
    *(u32*)&lbl_8047AE64 = r4;
    return;
    *(u32*)&lbl_8047AE68 = r4;
    return;
    *(u32*)&lbl_8047AE6C = r4;
    return;
    *(u32*)&lbl_8047AE70 = r4;
    return;
    *(u32*)&lbl_8047AE74 = r4;
    return;
    *(u32*)&lbl_8047AE78 = r4;
    return;
    r0 = r4 & 0xFFFF;
    *(u16*)&lbl_8047AE7C = r0;
    return;
    *(u32*)&lbl_8047AE88 = r4;
    return;
    *(u32*)&lbl_8047AE8C = r4;
    return;
    r0 = r4 & 0xFFFF;
    *(u16*)&lbl_8047AE90 = r0;
    return;
    *(u32*)&lbl_8047AE80 = r4;
    return;
    *(u32*)lbl_8047AE84 = r4;
    return;
    *(u32*)&lbl_8047AE94 = r4;
    return;
    *(u32*)&lbl_8047AE98 = r4;
    return;
    *(u32*)&lbl_8047AE9C = r4;
    return;
    r0 = r4 & 0xFFFF;
    *(u16*)&lbl_8047AEA0 = r0;
    return;
    r0 = r4 & 0xFFFF;
    *(u16*)&lbl_8047AEA2 = r0;
    return;
    r0 = r4 & 0xFFFF;
    *(u16*)lbl_8047AEA4 = r0;
    return;
    *(u32*)&lbl_8047ADC8 = r4;
    return;
    *(u32*)&lbl_8047ADCC = r4;
    return;
    *(u32*)&lbl_8047ADD0 = r4;
    return;
    *(u32*)lbl_8047ADD4 = r4;
    return;
    *(u32*)lbl_8047ADD8 = r4;
    return;
    *(u32*)lbl_8047ADDC = r4;
    return;
    *(u32*)lbl_8047ADE0 = r4;
    return;
    *(u32*)&lbl_8047ADE4 = r4;
    return;
    *(u32*)&lbl_8047ADE8 = r4;
    return;
    *(u32*)&lbl_8047ADEC = r4;
    return;
    *(u32*)&lbl_8047ADF0 = r4;
    return;
    *(u32*)&lbl_8047ADF4 = r4;
    return;
    *(u32*)&lbl_8047ADF8 = r4;
    return;
    *(u32*)&lbl_8047ADFC = r4;
    return;
    *(u32*)&lbl_8047AE00 = r4;
    return;
    *(u32*)&lbl_8047AE04 = r4;
    return;
    *(u32*)&lbl_8047AE08 = r4;
    return;
    *(u32*)&lbl_8047AE0C = r4;
    return;
    *(u32*)lbl_8047AE10 = r4;
    return;
    *(u32*)&lbl_8047AE14 = r4;
    return;
    *(u32*)&lbl_8047AE18 = r4;
    return;
    *(u32*)&lbl_8047AE1C = r4;
    return;
    *(u32*)&lbl_8047AE20 = r4;
    return;
    *(u32*)&lbl_8047AE24 = r4;
    return;
    *(u32*)&lbl_8047AE28 = r4;
    return;
    *(u32*)&lbl_8047AE2C = r4;
    return;
    *(u32*)&lbl_8047AE30 = r4;
    return;
    *(u32*)&lbl_8047AE34 = r4;
    return;
    *(u32*)&lbl_8047AE38 = r4;
    return;
    *(u32*)&lbl_8047AE3C = r4;
    return;
    *(u32*)&lbl_8047AE40 = r4;
    return;
    *(u32*)&lbl_8047AE44 = r4;
    return;
    *(u32*)&lbl_8047AE48 = r4;
    return;
    *(u32*)&lbl_8047AE4C = r4;
    return;
}

/* 0x80132C48 | 36 bytes | multi_sda_store */
void fn_80132C48(void) {
    lbl_8047AE70 = 0;
    lbl_8047AE74 = 0;
    lbl_8047AE78 = 0;
    lbl_8047AE60 = 0;
    lbl_8047AE64 = 0;
    lbl_8047AE88 = 0;
    lbl_8047AE8C = 0;
}

/* 0x80132C6C | 0x310 */
void fn_80132C6C(void) {
    extern u8 lbl_8047AEB0[];
    extern u8 lbl_8047AEB4[];
    extern u8 lbl_8047AEB8[];
    extern u8 lbl_8047AEBC[];
    extern u8 lbl_8047AEC0[];
    extern u8 lbl_8047AEC4[];
    extern u8 lbl_8047AEC8[];
    extern u8 lbl_8047AECC[];
    extern void fn_800E27B0();
    extern void fn_800E3534();
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
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* mr. r0, r3 */;
    if ((s32)r0 == (s32)0) goto L_80132F60;
    if ((u32)r4 != (u32)0x0) goto L_80132C98;
    goto L_80132F60;
L_80132C98: ;
    r3 = r0 * 0x18;
    *(u32*)lbl_8047AEB4 = r0;
    *(u32*)lbl_8047AEC0 = r4;
    *(u32*)lbl_8047AECC = r6;
    *(u32*)lbl_8047AEC8 = r5;
    fn_800E3534();
    r0 = r3 & 0xFFFF;
    *(u16*)lbl_8047AEB8 = r3;
    if ((u32)r4 == (u32)0x0) goto L_80132F60;
    r3 = r0;
    fn_800E27B0();
    r10 = 0x0;
    *(u32*)lbl_8047AEB0 = r3;
    r8 = r10;
    r11 = 0x0;
    r7 = r10;
    r6 = r10;
    r5 = r10;
    r4 = r10;
    r3 = r10;
    r0 = r10;
    while (1) {
        r9 = *(u32*)lbl_8047AEB4;
        if ((u32)r11 >= (u32)r9) break;
        r9 = *(u32*)lbl_8047AEB0;
        r11 = r11 + 0x1;
        r9 = r9 + r10;
        r10 = r10 + 0x18;
        *(u32*)((u8*)r9 + 0x0) = r8;
        *(u32*)((u8*)r9 + 0x4) = r7;
        *(u32*)((u8*)r9 + 0x8) = r6;
        *(u32*)((u8*)r9 + 0xC) = r5;
        *(u32*)((u8*)r9 + 0x10) = r4;
        *(u8*)((u8*)r9 + 0x14) = r3;
        *(u8*)((u8*)r9 + 0x15) = r0;


    }
    r0 = *(u32*)lbl_8047AEC0;
    r31 = r9 * r0;
    r3 = r31 << 5;
    fn_800E3534();
    r0 = r3 & 0xFFFF;
    *(u16*)lbl_8047AEC4 = r3;
    if ((u32)r11 == (u32)r9) goto L_80132F60;
    r3 = r0;
    fn_800E27B0();
    *(u32*)lbl_8047AEBC = r3;
    r7 = 0x0;
    if ((u32)r31 <= (u32)0x0) goto L_80132F60;
    if ((u32)r31 > (u32)0x8) {
        r0 = r3 + 0x7;
        r6 = r7;
        r0 = (u32)r0 >> 3;
        ctr_fn = (void(*)(void))r0;
        if ((u32)r3 > (u32)0x0) {
            do {
                r0 = *(u32*)lbl_8047AEBC;
                r4 = -0x1;
                r3 = 0x0;
                r29 = r6 + 0x20;
                r5 = r0 + r6;
                r30 = r6 + 0x40;
                *(u32*)((u8*)r5 + 0x0) = r4;
                r12 = r6 + 0x60;
                r11 = r6 + 0x80;
                r10 = r6 + 0xa0;
                *(u16*)((u8*)r5 + 0x4) = r3;
                r9 = r6 + 0xc0;
                r8 = r6 + 0xe0;
                r6 = r6 + 0x100;
                *(u16*)((u8*)r5 + 0x6) = r3;
                r7 = r7 + 0x8;
                *(u32*)((u8*)r5 + 0x8) = r3;
                *(u32*)((u8*)r5 + 0xC) = r3;
                *(u32*)((u8*)r5 + 0x10) = r4;
                *(u8*)((u8*)r5 + 0x14) = r3;
                *(u32*)((u8*)r5 + 0x18) = r3;
                *(u32*)((u8*)r5 + 0x1C) = r3;
                r0 = *(u32*)lbl_8047AEBC;
                r29 = r0 + r29;
                *(u32*)((u8*)r29 + 0x0) = r4;
                *(u16*)((u8*)r29 + 0x4) = r3;
                *(u16*)((u8*)r29 + 0x6) = r3;
                *(u32*)((u8*)r29 + 0x8) = r3;
                *(u32*)((u8*)r29 + 0xC) = r3;
                *(u32*)((u8*)r29 + 0x10) = r4;
                *(u8*)((u8*)r29 + 0x14) = r3;
                *(u32*)((u8*)r29 + 0x18) = r3;
                *(u32*)((u8*)r29 + 0x1C) = r3;
                r0 = *(u32*)lbl_8047AEBC;
                r30 = r0 + r30;
                *(u32*)((u8*)r30 + 0x0) = r4;
                *(u16*)((u8*)r30 + 0x4) = r3;
                *(u16*)((u8*)r30 + 0x6) = r3;
                *(u32*)((u8*)r30 + 0x8) = r3;
                *(u32*)((u8*)r30 + 0xC) = r3;
                *(u32*)((u8*)r30 + 0x10) = r4;
                *(u8*)((u8*)r30 + 0x14) = r3;
                *(u32*)((u8*)r30 + 0x18) = r3;
                *(u32*)((u8*)r30 + 0x1C) = r3;
                r0 = *(u32*)lbl_8047AEBC;
                r12 = r0 + r12;
                *(u32*)((u8*)r12 + 0x0) = r4;
                *(u16*)((u8*)r12 + 0x4) = r3;
                *(u16*)((u8*)r12 + 0x6) = r3;
                *(u32*)((u8*)r12 + 0x8) = r3;
                *(u32*)((u8*)r12 + 0xC) = r3;
                *(u32*)((u8*)r12 + 0x10) = r4;
                *(u8*)((u8*)r12 + 0x14) = r3;
                *(u32*)((u8*)r12 + 0x18) = r3;
                *(u32*)((u8*)r12 + 0x1C) = r3;
                r0 = *(u32*)lbl_8047AEBC;
                r11 = r0 + r11;
                *(u32*)((u8*)r11 + 0x0) = r4;
                *(u16*)((u8*)r11 + 0x4) = r3;
                *(u16*)((u8*)r11 + 0x6) = r3;
                *(u32*)((u8*)r11 + 0x8) = r3;
                *(u32*)((u8*)r11 + 0xC) = r3;
                *(u32*)((u8*)r11 + 0x10) = r4;
                *(u8*)((u8*)r11 + 0x14) = r3;
                *(u32*)((u8*)r11 + 0x18) = r3;
                *(u32*)((u8*)r11 + 0x1C) = r3;
                r0 = *(u32*)lbl_8047AEBC;
                r10 = r0 + r10;
                *(u32*)((u8*)r10 + 0x0) = r4;
                *(u16*)((u8*)r10 + 0x4) = r3;
                *(u16*)((u8*)r10 + 0x6) = r3;
                *(u32*)((u8*)r10 + 0x8) = r3;
                *(u32*)((u8*)r10 + 0xC) = r3;
                *(u32*)((u8*)r10 + 0x10) = r4;
                *(u8*)((u8*)r10 + 0x14) = r3;
                *(u32*)((u8*)r10 + 0x18) = r3;
                *(u32*)((u8*)r10 + 0x1C) = r3;
                r0 = *(u32*)lbl_8047AEBC;
                r9 = r0 + r9;
                *(u32*)((u8*)r9 + 0x0) = r4;
                *(u16*)((u8*)r9 + 0x4) = r3;
                *(u16*)((u8*)r9 + 0x6) = r3;
                *(u32*)((u8*)r9 + 0x8) = r3;
                *(u32*)((u8*)r9 + 0xC) = r3;
                *(u32*)((u8*)r9 + 0x10) = r4;
                *(u8*)((u8*)r9 + 0x14) = r3;
                *(u32*)((u8*)r9 + 0x18) = r3;
                *(u32*)((u8*)r9 + 0x1C) = r3;
                r0 = *(u32*)lbl_8047AEBC;
                r8 = r0 + r8;
                *(u32*)((u8*)r8 + 0x0) = r4;
                *(u16*)((u8*)r8 + 0x4) = r3;
                *(u16*)((u8*)r8 + 0x6) = r3;
                *(u32*)((u8*)r8 + 0x8) = r3;
                *(u32*)((u8*)r8 + 0xC) = r3;
                *(u32*)((u8*)r8 + 0x10) = r4;
                *(u8*)((u8*)r8 + 0x14) = r3;
                *(u32*)((u8*)r8 + 0x18) = r3;
                *(u32*)((u8*)r8 + 0x1C) = r3;
            } while (--ctr != 0);
    }
    }
    r0 = r31 - r7;
    r5 = r7 << 5;
    r4 = -0x1;
    r3 = 0x0;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r7 >= (u32)r31) goto L_80132F60;
    do {
        r0 = *(u32*)lbl_8047AEBC;
        r6 = r0 + r5;
        r5 = r5 + 0x20;
        *(u32*)((u8*)r6 + 0x0) = r4;
        *(u16*)((u8*)r6 + 0x4) = r3;
        *(u16*)((u8*)r6 + 0x6) = r3;
        *(u32*)((u8*)r6 + 0x8) = r3;
        *(u32*)((u8*)r6 + 0xC) = r3;
        *(u32*)((u8*)r6 + 0x10) = r4;
        *(u8*)((u8*)r6 + 0x14) = r3;
        *(u32*)((u8*)r6 + 0x18) = r3;
        *(u32*)((u8*)r6 + 0x1C) = r3;
    } while (--ctr != 0);
L_80132F60: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}

/* 0x5C | fn_80132F7C | multi_call_cond */
u32 fn_80132F7C(void) {
    if (fn_80102620(0) == 0) { return 171; }
    fn_80102510(0);
    fn_801026A4();
    return 0;
}

/* 0x78 | fn_80132FD8 | generic */
u32 fn_80132FD8(void) {
    /* refs: lbl_80272AA8 */
    fn_801E1810();
    fn_800F0308();
    fn_801E1874();
    fn_8010264C();
    fn_800C8520();
    fn_801E189C();
    return 0;
}

/* 0x80133050 | 0x3C -- fn_800D37D4(3, 2, 0, 2, 0, 0), return 0 */
extern void fn_800D37D4(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
u32 fn_80133050(void) {
    fn_800D37D4(3, 2, 0, 2, 0, 0);
    return 0;
}

/* 0x8013308C | 0x3C -- fn_800D37D4(2, 2, 0, 2, 0, 0), return 0 */
u32 fn_8013308C(void) {
    fn_800D37D4(2, 2, 0, 2, 0, 0);
    return 0;
}

/* 0x801330C8 | 0x150 */
void fn_801330C8(void) {
    extern u8 lbl_80478AC0[];
    extern u8 lbl_80478AC4[];
    extern u8 lbl_8047D0F0[];
    extern u8 lbl_8047D0F4[];
    extern u8 lbl_8047D0F8[];
    extern u8 lbl_8047D0FC[];
    extern u8 lbl_8047D100[];
    extern u8 lbl_8047D104[];
    extern void fn_800D5CB8();
    extern void fn_800D6680();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    extern void fn_800D9B58();
    extern void fn_800DA028();
    extern void fn_800DA1E8();
    extern void fn_800DA2BC();
    extern void fn_800DA4C4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;

    r3 = 0x1;
    fn_800D88DC();
    r3 = 0x6;
    fn_800D888C();
    f1 = *(f32*)lbl_8047D0F0;
    f3 = *(f32*)lbl_8047D0F4;
    f2 = f1;
    f4 = *(f32*)lbl_8047D0F8;
    fn_800D9B58();
    r3 = 0x0;
    r4 = 0x6;
    r5 = 0x7;
    fn_800DA4C4();
    r3 = 0x1;
    r4 = 0x1;
    r5 = 0x0;
    fn_800DA2BC();
    r3 = 0x0;
    r4 = 0x1;
    r5 = 0x1;
    fn_800DA1E8();
    r3 = 0x0;
    fn_800DA028();
    r3 = 0x4;
    fn_800D6A00();
    r3 = 0x0;
    fn_800D7820();
    r3 = 0x3;
    fn_800D67BC();
    r3 = (u32)lbl_80478AC0;
    f1 = *(f32*)lbl_8047D0FC;
    f0 = *(f32*)lbl_80478AC0;
    f3 = *(f32*)lbl_8047D0F0;
    f2 = -f0;
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0x80;
    r6 = 0x40;
    r7 = 0x0;
    fn_800D5CB8();
    r3 = (u32)lbl_80478AC4;
    f2 = *(f32*)lbl_8047D100;
    f3 = *(f32*)lbl_8047D0F0;
    f1 = *(f32*)lbl_80478AC4;
    fn_800D6680();
    r3 = 0x0;
    r4 = 0x40;
    r5 = 0xff;
    r6 = 0x0;
    r7 = 0x0;
    fn_800D5CB8();
    r3 = (u32)lbl_80478AC4;
    f1 = *(f32*)lbl_8047D104;
    f3 = *(f32*)lbl_8047D0F0;
    f2 = *(f32*)lbl_80478AC4;
    fn_800D6680();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x40;
    r6 = 0xff;
    r7 = 0x0;
    fn_800D5CB8();
    r3 = (u32)lbl_80478AC0;
    f1 = *(f32*)lbl_8047D0FC;
    f0 = *(f32*)lbl_80478AC0;
    f3 = *(f32*)lbl_8047D0F0;
    f2 = -f0;
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0x80;
    r6 = 0x40;
    r7 = 0x0;
    fn_800D5CB8();
    fn_800D6728();
    r3 = 0x0;
    return;
}

/* 0x80133218 | 0x38 -- fn_800E1544() then print result, return 0 */
extern u32  fn_800E1544(void);
extern void fn_800DD970(const char* fmt, ...);
extern const char lbl_80272AB8[];
u32 fn_80133218(void) {
    u32 val = fn_800E1544();
    fn_800DD970(lbl_80272AB8, val);
    return 0;
}

/* 0x80133250 | 0x2C -- fn_800E0E14(1, 1), return 0 */
extern u8 fn_800E0E14(u32 a, u32 b);
u32 fn_80133250(void) {
    fn_800E0E14(1, 1);
    return 0;
}

/* 0x5C | fn_8013327C -- fn_800E0E14(1,0) check then print */
extern const char lbl_80272AE0[];
extern const char lbl_80272AF0[];
u32 fn_8013327C(void) {
    u8 result = fn_800E0E14(1, 0);
    if ((result & 0xFF) == 1) {
        fn_800DD970(lbl_80272AE0);
    } else {
        fn_800DD970(lbl_80272AF0);
    }
    return 0;
}

/* 0x801332D8 | 0x28 -- fn_800D3074(2), return 0 */
extern void fn_800D3074(u32 mode);
u32 fn_801332D8(void) {
    fn_800D3074(2);
    return 0;
}

/* 0x80133300 | 0x28 -- fn_800D3074(1), return 0 */
u32 fn_80133300(void) {
    fn_800D3074(1);
    return 0;
}

/* 0x80133328 | 36 bytes | call_return_const2 */
u32 fn_80133328(void) {
    fn_801D216C();
    return 0;
}

/* 0x60 | fn_8013334C | generic */
u32 fn_8013334C(void) {
    fn_801D1CC4();
    fn_801D1D58();
    fn_801D268C();
    fn_8010264C();
    return 0;
}

/* 0x801333AC | 0xA4 */
void fn_801333AC(void) {
    extern void fn_800F9318();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r4 = 0x2;
    r31 = r3;
    r3 = 0x0;
    fn_800F9318();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        r31 = *(u32*)(sp + 0xC);
        return;
    }
    if ((s32)r31 == (s32)0xb9) goto L_80133418;
    if ((s32)r31 >= (s32)0xb9) goto L_80133438;
    if ((s32)r31 >= (s32)0xb8) goto L_801333F4;
    goto L_80133438;
L_801333F4: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_8013340C;
    r0 = 0x0;
    *(u8*)((u8*)r3 + 0x0) = r0;
    goto L_80133438;
L_8013340C: ;
    r0 = 0x1;
    *(u8*)((u8*)r3 + 0x0) = r0;
    goto L_80133438;
L_80133418: ;
    r0 = *(u8*)((u8*)r3 + 0x1);
    if ((u32)r0 == (u32)0x0) goto L_80133430;
    r0 = 0x0;
    *(u8*)((u8*)r3 + 0x1) = r0;
    goto L_80133438;
L_80133430: ;
    r0 = 0x1;
    *(u8*)((u8*)r3 + 0x1) = r0;
L_80133438: ;
    r3 = 0x0;

    r31 = *(u32*)(sp + 0xC);
    return;
}

/* 0x58 | fn_80133450 | call_sequence */
void fn_80133450(void) {
    fn_801026A4();
    fn_80102510(0);
}

/* 0x801334A8 | 0x34 -- toggle lbl_8047AED9 (cntlzw/extrwi), call fn_800E8F80 */
extern u8   lbl_8047AED9;
extern void fn_800E8F80(u32 val);
u32 fn_801334A8(void) {
    u32 clz = __cntlzw(lbl_8047AED9);
    u8 val = (u8)((clz >> 19) & 0xFF);
    lbl_8047AED9 = val;
    fn_800E8F80(val);
    return 0;
}

/* 0x801334DC | 0x34 -- toggle lbl_8047AED8 (cntlzw/extrwi), call fn_800D4610 */
extern u8   lbl_8047AED8;
extern void fn_800D4610(u32 val);
u32 fn_801334DC(void) {
    u32 clz = __cntlzw(lbl_8047AED8);
    u8 val = (u8)((clz >> 19) & 0xFF);
    lbl_8047AED8 = val;
    fn_800D4610(val);
    return 0;
}

/* 0x5C | fn_80133510 | multi_call_cond */
u32 fn_80133510(void) {
    if (fn_80102620(0) == 0) { return 7; }
    fn_80102510(0);
    fn_801026A4();
    return 0;
}

/* 0x68 | fn_8013356C | generic */
u32 fn_8013356C(void) {
    fn_8012F11C();
    fn_8012F150();
    fn_8012F1FC();
    return 0;
}

/* 0x5C | fn_801335D4 | multi_call_cond */
u32 fn_801335D4(void) {
    if (fn_80102620(0) == 0) { return 4; }
    fn_80102510(0);
    fn_801026A4();
    return 0;
}

/* 0x80133630 | 0x34 -- toggle lbl_8047AED4 (cntlzw>>5), call fn_80101B88 */
extern u32  lbl_8047AED4;
u32 fn_80133630(void) {
    u32 clz = __cntlzw(lbl_8047AED4);
    lbl_8047AED4 = clz >> 5;
    fn_80101B88(clz >> 5);
    return 0;
}

/* 0x80133664 | 0x13C */
void fn_80133664(void) {
    extern void fn_8005D9E4();
    extern void fn_80105624();
    extern void fn_80133B50();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    fn_80105624();
    r29 = *(u16*)((u8*)r3 + 0x6);
    r3 = r30;
    r4 = 0x0;
    fn_80133B50();
    r31 = (s8)r3;
    r3 = *(u32*)((u8*)r30 + 0x4);
    fn_8005D9E4();
    r5 = (s8)r3;
    if ((s32)r31 < (s32)r5) {
        r5 = r31;
    }
    r3 = *(u16*)((u8*)r30 + 0x94);
    r0 = r29 & 0x1;
    r0 = r29 & 0xFFFF;
    *(u16*)(sp + 0x8) = r3;
    if ((s32)r31 == (s32)r5) goto L_801336D4;
    r3 = *(u8*)(sp + 0x9);
    *(u8*)(sp + 0x9) = r0;
    goto L_801336E8;
L_801336D4: ;
    r0 = r0 & 0x00000002;
    if ((s32)r31 == (s32)r5) goto L_801336E8;
    r3 = *(u8*)(sp + 0x9);
    r0 = r3 + 0x1;
    *(u8*)(sp + 0x9) = r0;
L_801336E8: ;
    r4 = *(u8*)(sp + 0x9);
    r0 = (s8)r4;
    if ((s32)r31 >= (s32)r5) goto L_8013372C;
    r3 = *(u8*)(sp + 0x8);
    r0 = 0x0;
    *(u8*)(sp + 0x9) = r0;
    r3 = r3 + r4;
    r0 = r3 & 0xFF;
    *(u8*)(sp + 0x8) = r3;
    r0 = (s8)r0;
    if ((s32)r31 >= (s32)r5) goto L_8013377C;
    r4 = (s8)r5;
    r0 = r31 - r4;
    *(u8*)(sp + 0x9) = r3;
    *(u8*)(sp + 0x8) = r0;
    goto L_8013377C;
L_8013372C: ;
    r0 = (s8)r4;
    r3 = (s8)r5;
    if ((s32)r0 < (s32)r3) goto L_8013377C;
    r3 = *(u8*)(sp + 0x8);
    r0 = r4 - r5;
    *(u8*)(sp + 0x9) = r5;
    r4 = r3 + r0;
    r0 = r5 & 0xFF;
    *(u8*)(sp + 0x8) = r4;
    r3 = r4 & 0xFF;
    r3 = (s8)r3;
    r0 = (s8)r0;
    r0 = r3 + r0;
    if ((s32)r0 < (s32)r31) goto L_8013377C;
    r0 = 0x0;
    *(u8*)(sp + 0x8) = r0;
    *(u8*)(sp + 0x9) = r0;
L_8013377C: ;
    r0 = *(u16*)(sp + 0x8);
    *(u16*)((u8*)r30 + 0x94) = r0;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}

/* 0x801337A0 | 0x8 | sda_getter */
u8 fn_801337A0(void) { return lbl_8047AED0; }

/* 0x801337A8 | 0x8 | sda_setter */
void fn_801337A8(u8 val) { lbl_8047AED0 = val; }

/* 0x801337B0 | 0x34 -- check fn_80102620(lbl_80478848) != 0, return 0 or 1 */
extern u32  lbl_80478848;
u32 fn_801337B0(void) {
    u8 result = fn_80102620(lbl_80478848);
    return (result != 0) ? 0 : 1;
}

/* 0x801337E4 | 0x2C -- set lbl_8047AED1 = 0, call fn_80102510(lbl_80478848) */
extern u8   lbl_8047AED1;
void fn_801337E4(void) {
    lbl_8047AED1 = 0;
    fn_80102510(lbl_80478848);
}

/* 0x80133810 | 0x94 */
void fn_80133810(void) {
    extern u8 lbl_80478F88[];
    extern void fn_801338A4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r12 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r31 = r3;
    r12 = *(u32*)lbl_80478F88;
    if ((u32)r12 == (u32)0x0) {
        r3 = 0x0;
    } else {

        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    if ((u32)r3 == (u32)0x0) { r31 = *(u32*)(sp + 0xC); return; }
    r0 = *(u8*)&lbl_8047AED0;
    if ((u32)r0 == (u32)0x0) { r31 = *(u32*)(sp + 0xC); return; }
    r3 = *(u32*)&lbl_80478848;
    ((void(*)(void))fn_80102620)();
    r3 = r3 & 0xFF;
    r0 = -r3;
    r0 = r0 | r3;
    /* srwi. r0, r0, 31 */;
    if ((u32)r0 != (u32)0x0) { r31 = *(u32*)(sp + 0xC); return; }
    *(u8*)&lbl_8047AED1 = r31;
    do {
        r3 = 0x0;
        fn_801338A4();
        if ((s32)r3 < (s32)0x0) { r31 = *(u32*)(sp + 0xC); return; }
        r0 = *(u8*)&lbl_8047AED1;
    } while ((u32)r0 == (u32)0x1);

    r31 = *(u32*)(sp + 0xC);
    return;
}

/* 0x801338A4 | 0x2AC */
void fn_801338A4(void) {
    extern u8 lbl_80478F88[];
    extern u8 lbl_80478F8C[];
    extern u8 lbl_8047AEDC[];
    extern void fn_801026A4();
    extern void fn_801338A4();
    extern void fn_80133C3C();
    extern void fn_80133E6C();
    extern void fn_80104704();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r12 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r29 = r3;
    r26 = 0x0;
    r0 = *(u32*)&lbl_80478848;
    r28 = r0 + r29;
L_801338C8: ;
    r0 = *(u32*)&lbl_80478848;
    r3 = (s32)r29 >> 31;
    r0 = r0 + r27;
    r25 = r0 & ~r3;
    r3 = r25;
    fn_80133C3C();
    r0 = *(u32*)lbl_8047AEDC;
    r5 = r3 << 2;
    r3 = r28;
    r4 = r25;
    r30 = r0 + r5;
    r6 = 0x0;
    r5 = r30;
    r7 = 0x1;
    r8 = 0x0;
    fn_801026A4();
    r25 = r3;
    r3 = r28;
    ((void(*)(void))fn_80104704)();
    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) {
        r3 = *(u8*)((u8*)r31 + 0x94);
        r0 = *(u8*)((u8*)r31 + 0x95);
        r3 = (s8)r3;
        r0 = (s8)r0;
        r0 = r3 + r0;
        *(u32*)((u8*)r30 + 0x0) = r0;
    } else {

        r0 = 0x0;
        *(u32*)((u8*)r30 + 0x0) = r0;
    }
    if ((s32)r25 != (s32)-0x1) goto L_8013395C;
    if ((s32)r29 != (s32)0x0) goto L_80133B30;
    r26 = -0x1;
    goto L_80133B30;
L_8013395C: ;
    r4 = *(u8*)((u8*)r31 + 0x94);
    r3 = r31;
    r0 = *(u8*)((u8*)r31 + 0x95);
    r4 = (s8)r4;
    r0 = (s8)r0;
    r4 = r4 + r0;
    fn_80133E6C();
    /* mr. r30, r3 */;
    if ((s32)r29 <= (s32)0x0) goto L_801339A4;
    r12 = *(u32*)lbl_80478F88;
    if ((u32)r12 == (u32)0x0) {
        r3 = 0x0;
    } else {

        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    if ((s32)r3 > (s32)r30) goto L_801339AC;
L_801339A4: ;
    r25 = 0x0;
    goto L_80133A14;
L_801339AC: ;
    r12 = *(u32*)lbl_80478F8C;
    if ((u32)r12 == (u32)0x0) {
        r3 = 0x0;
    } else {

        r3 = r30;
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    if ((u32)r3 == (u32)0x0) {
        r25 = 0x0;
    } else {

        r25 = *(s16*)((u8*)r3 + 0x2);
    }
    r0 = (s16)r25;
    if ((u32)r3 <= (u32)0x0) goto L_80133A10;
    r12 = *(u32*)lbl_80478F88;
    if ((u32)r12 == (u32)0x0) {
        r3 = 0x0;
    } else {

        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    r0 = (s16)r25;
    if ((s32)r3 > (s32)r0) goto L_80133A14;
L_80133A10: ;
    r25 = 0x0;
L_80133A14: ;
    r0 = (s16)r25;
    if ((s32)r3 == (s32)r0) goto L_80133AC4;
    r12 = *(u32*)lbl_80478F8C;
    if ((u32)r12 == (u32)0x0) {
        r3 = 0x0;
    } else {

        r3 = r30;
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    if ((u32)r3 == (u32)0x0) {
        r12 = 0x0;
    } else {

        r12 = *(u32*)((u8*)r3 + 0x8);
    }
    if ((u32)r12 != (u32)0x0) {
        r4 = *(u8*)((u8*)r31 + 0x94);
        r3 = r30;
        r0 = *(u8*)((u8*)r31 + 0x95);
        r4 = (s8)r4;
        r0 = (s8)r0;
        r4 = r4 + r0;
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    } else {

        r3 = 0x1;
    }
    if ((s32)r3 == (s32)0x0) {
        r3 = *(u32*)&lbl_80478848;
        ((void(*)(void))fn_80102510)();
        r3 = 0x1;
        return;
    }
    if ((s32)r3 == (s32)-0x1) goto L_801338C8;
    r0 = (s16)r25;
    if ((s32)r0 == (s32)0x1) goto L_801338C8;
    r3 = r29 + 0x1;
    fn_801338A4();
    if ((s32)r3 != (s32)0x1) goto L_801338C8;
    r3 = 0x1;
    return;
L_80133AC4: ;
    r3 = *(u32*)&lbl_80478848;
    ((void(*)(void))fn_80102510)();
    r12 = *(u32*)lbl_80478F8C;
    if ((u32)r12 == (u32)0x0) {
        r3 = 0x0;
    } else {

        r3 = r30;
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    if ((u32)r3 == (u32)0x0) {
        r12 = 0x0;
    } else {

        r12 = *(u32*)((u8*)r3 + 0x8);
    }
    if ((u32)r12 != (u32)0x0) {
        r4 = *(u8*)((u8*)r31 + 0x94);
        r3 = r30;
        r0 = *(u8*)((u8*)r31 + 0x95);
        r4 = (s8)r4;
        r0 = (s8)r0;
        r4 = r4 + r0;
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    r3 = 0x1;
    return;
L_80133B30: ;
    r3 = r28;
    ((void(*)(void))fn_80102510)();
    r3 = r26;

    return;
}

/* 0x80133B50 | 0x94 */
void fn_80133B50(void) {
    extern void fn_800FA444();
    extern void fn_80133BE4();
    extern void fn_80133E1C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* mr. r30, r4 */;
    r29 = r3;
    if ((s32)r0 != (s32)0) {
        r0 = 0x0;
        *(u32*)((u8*)r30 + 0x0) = r0;
    }
    r31 = 0x0;
    do {
        r3 = r29;
        r4 = r31;
        fn_80133E1C();
        if ((u32)r30 != (u32)0x0) {
            fn_800FA444();
            r0 = *(u32*)((u8*)r30 + 0x0);
            r3 = (u32)r3 >> 16;
            if ((s32)r0 < (s32)r3) {
                *(u32*)((u8*)r30 + 0x0) = r3;
        }
        }
        r4 = r31;
        r3 = r29;
        r31 = r31 + 0x1;
        fn_80133BE4();
    } while ((s32)r3 == (s32)0x0);
    r3 = r31;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}

/* 0x58 | fn_80133BE4 | generic */
u32 fn_80133BE4(void) {
    /* refs: lbl_80478F8C */
    fn_80133E6C();
    return 0;
}

/* 0x80133C3C | 0x1E0 */
void fn_80133C3C(void) {
    extern u8 lbl_80478F88[];
    extern u8 lbl_80478F8C[];
    extern void fn_800057A8();
    extern void fn_80133E6C();
    extern void fn_80104704();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r12 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    ((void(*)(void))fn_80104704)();
    if ((u32)r3 == (u32)0x0) goto L_80133D78;
    r4 = *(u8*)((u8*)r3 + 0x94);
    r0 = *(u8*)((u8*)r3 + 0x95);
    r4 = (s8)r4;
    r0 = (s8)r0;
    r4 = r4 + r0;
    fn_80133E6C();
    /* mr. r31, r3 */;
    if ((u32)r3 <= (u32)0x0) goto L_80133CA4;
    r12 = *(u32*)lbl_80478F88;
    if ((u32)r12 == (u32)0x0) {
        r3 = 0x0;
    } else {

        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    if ((s32)r3 > (s32)r31) goto L_80133CAC;
L_80133CA4: ;
    r31 = 0x0;
    goto L_80133D14;
L_80133CAC: ;
    r12 = *(u32*)lbl_80478F8C;
    if ((u32)r12 == (u32)0x0) {
        r3 = 0x0;
    } else {

        r3 = r31;
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    if ((u32)r3 == (u32)0x0) {
        r31 = 0x0;
    } else {

        r31 = *(s16*)((u8*)r3 + 0x2);
    }
    r0 = (s16)r31;
    if ((u32)r3 <= (u32)0x0) goto L_80133D10;
    r12 = *(u32*)lbl_80478F88;
    if ((u32)r12 == (u32)0x0) {
        r3 = 0x0;
    } else {

        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    r0 = (s16)r31;
    if ((s32)r3 > (s32)r0) goto L_80133D14;
L_80133D10: ;
    r31 = 0x0;
L_80133D14: ;
    r31 = (s16)r31;
    r29 = 0x0;
    r30 = 0x0;
    while ((s32)r30 < (s32)r31) {

        r12 = *(u32*)lbl_80478F8C;
        if ((u32)r12 == (u32)0x0) {
            r3 = 0x0;
        } else {

            r3 = r30;
            ctr_fn = (void(*)(void))r12;
            ctr_fn();
        }
        if ((u32)r3 == (u32)0x0) {
            r0 = 0x1;
        } else {

            r0 = *(u8*)((u8*)r3 + 0x0);
        }
        r0 = r0 & 0xFF;
        if ((u32)r3 != (u32)0x0) {
            r29 = r29 + 0x1;
        }
        r30 = r30 + 0x1;

    }
    goto L_80133DFC;
L_80133D78: ;
    r29 = 0x0;
    r31 = 0x0;
    goto L_80133DCC;
L_80133D84: ;
    r12 = *(u32*)lbl_80478F8C;
    if ((u32)r12 == (u32)0x0) {
        r3 = 0x0;
    } else {

        r3 = r31;
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    if ((u32)r3 == (u32)0x0) {
        r0 = 0x1;
    } else {

        r0 = *(u8*)((u8*)r3 + 0x0);
    }
    r0 = r0 & 0xFF;
    if ((u32)r3 != (u32)0x0) {
        r29 = r29 + 0x1;
    }
    r31 = r31 + 0x1;
L_80133DCC: ;
    fn_800057A8();
    if ((s32)r3 == (s32)0x1) goto L_80133DF0;
    if ((s32)r3 >= (s32)0x1) goto L_80133DE0;
    goto L_80133DF0;
L_80133DE0: ;
    if ((s32)r3 >= (s32)0x3) goto L_80133DF0;
    r0 = 0x115;
    goto L_80133DF4;
L_80133DF0: ;
    r0 = 0x2;
L_80133DF4: ;
    if ((s32)r31 < (s32)r0) goto L_80133D84;
L_80133DFC: ;
    r3 = r29;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}

/* 0x50 | fn_80133E1C | call_then_multi_check */
u32 fn_80133E1C(void) {
    fn_80133E6C();
    /* multi-branch on result */
    return 0;
}

/* 0x80133E6C | 0x2F8 */
void fn_80133E6C(void) {
    extern u8 lbl_80478F88[];
    extern u8 lbl_80478F8C[];
    extern void fn_800057A8();
    extern void fn_80133E6C();
    extern void fn_80134164();
    extern void fn_80134228();
    extern void fn_80134258();
    extern void fn_80134274();
    extern void fn_801342B8();
    extern void fn_80134304();
    extern void fn_80104704();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r12 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r31 = r4;
    r0 = *(u32*)((u8*)r3 + 0x4);
    r3 = -0x1;
    r4 = *(u32*)&lbl_80478848;
    if ((s32)r0 >= (s32)r4) {
        r3 = r0 - r4;
    }
    if ((s32)r3 >= (s32)0x0) goto L_80133EB0;
    r3 = 0x0;
    goto L_80134148;
L_80133EB0: ;
    if ((s32)r3 != (s32)0x0) goto L_80133EE0;
    fn_800057A8();
    if ((s32)r3 == (s32)0x1) goto L_80133ED8;
    if ((s32)r3 >= (s32)0x1) goto L_80133EC8;
    goto L_80133ED8;
L_80133EC8: ;
    if ((s32)r3 >= (s32)0x3) goto L_80133ED8;
    r0 = 0x115;
    goto L_80134144;
L_80133ED8: ;
    r0 = 0x2;
    goto L_80134144;
L_80133EE0: ;
    r3 = r3 + r4;
    r4 = (s32)r0 >> 31;
    r3 = r0 & ~r4;
    ((void(*)(void))fn_80104704)();
    if ((u32)r3 != (u32)0x0) goto L_80133F30;
    fn_800057A8();
    if ((s32)r3 == (s32)0x1) goto L_80133F24;
    if ((s32)r3 >= (s32)0x1) goto L_80133F14;
    goto L_80133F24;
L_80133F14: ;
    if ((s32)r3 >= (s32)0x3) goto L_80133F24;
    r0 = 0x115;
    goto L_80133F28;
L_80133F24: ;
    r0 = 0x2;
L_80133F28: ;
    r29 = r0;
    goto L_801340A4;
L_80133F30: ;
    r7 = *(u32*)((u8*)r3 + 0x4);
    r4 = -0x1;
    r6 = *(u32*)&lbl_80478848;
    r5 = *(u8*)((u8*)r3 + 0x94);
    r0 = *(u8*)((u8*)r3 + 0x95);
    r3 = (s8)r5;
    r0 = (s8)r0;
    r30 = r3 + r0;
    if ((s32)r7 >= (s32)r6) {
        r4 = r7 - r6;
    }
    if ((s32)r4 >= (s32)0x0) goto L_80133F6C;
    r0 = 0x0;
    goto L_801340A0;
L_80133F6C: ;
    if ((s32)r4 != (s32)0x0) goto L_80133F9C;
    fn_800057A8();
    if ((s32)r3 == (s32)0x1) goto L_80133F94;
    if ((s32)r3 >= (s32)0x1) goto L_80133F84;
    goto L_80133F94;
L_80133F84: ;
    if ((s32)r3 >= (s32)0x3) goto L_80133F94;
    r0 = 0x115;
    goto L_8013409C;
L_80133F94: ;
    r0 = 0x2;
    goto L_8013409C;
L_80133F9C: ;
    r3 = (s32)r0 >> 31;
    r0 = r6 + r0;
    r3 = r0 & ~r3;
    ((void(*)(void))fn_80104704)();
    if ((u32)r3 != (u32)0x0) goto L_80133FE4;
    fn_800057A8();
    if ((s32)r3 == (s32)0x1) goto L_80133FDC;
    if ((s32)r3 >= (s32)0x1) goto L_80133FCC;
    goto L_80133FDC;
L_80133FCC: ;
    if ((s32)r3 >= (s32)0x3) goto L_80133FDC;
    r29 = 0x115;
    goto L_80134054;
L_80133FDC: ;
    r29 = 0x2;
    goto L_80134054;
L_80133FE4: ;
    r4 = *(u8*)((u8*)r3 + 0x94);
    r0 = *(u8*)((u8*)r3 + 0x95);
    r4 = (s8)r4;
    r0 = (s8)r0;
    r29 = r4 + r0;
    fn_80134258();
    if ((s32)r3 >= (s32)0x0) goto L_8013400C;
    r29 = 0x0;
    goto L_80134054;
L_8013400C: ;
    if ((s32)r3 == (s32)0x0) {
        fn_80134274();
    } else {

        fn_80134228();
        if ((u32)r3 == (u32)0x0) {
            fn_80134274();
        } else {

            r4 = *(u8*)((u8*)r3 + 0x94);
            r0 = *(u8*)((u8*)r3 + 0x95);
            r4 = (s8)r4;
            r0 = (s8)r0;
            r4 = r4 + r0;
            fn_80133E6C();
        }
        fn_80134164();
        r3 = (s16)r3;
    }
    r29 = r3 + r29;
L_80134054: ;
    if ((s32)r29 <= (s32)0x0) goto L_80134068;
    fn_80134304();
    if ((s32)r3 > (s32)r29) goto L_80134070;
L_80134068: ;
    r29 = 0x0;
    goto L_80134098;
L_80134070: ;
    r3 = r29;
    fn_801342B8();
    r29 = r3;
    r0 = (s16)r29;
    if ((s32)r3 <= (s32)r29) goto L_80134094;
    fn_80134304();
    r0 = (s16)r29;
    if ((s32)r3 > (s32)r0) goto L_80134098;
L_80134094: ;
    r29 = 0x0;
L_80134098: ;
    r0 = (s16)r29;
L_8013409C: ;
    r0 = r0 + r30;
L_801340A0: ;
    r29 = r0;
L_801340A4: ;
    if ((s32)r29 <= (s32)0x0) goto L_801340D0;
    r12 = *(u32*)lbl_80478F88;
    if ((u32)r12 == (u32)0x0) {
        r3 = 0x0;
    } else {

        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    if ((s32)r3 > (s32)r29) goto L_801340D8;
L_801340D0: ;
    r30 = 0x0;
    goto L_80134140;
L_801340D8: ;
    r12 = *(u32*)lbl_80478F8C;
    if ((u32)r12 == (u32)0x0) {
        r3 = 0x0;
    } else {

        r3 = r29;
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    if ((u32)r3 == (u32)0x0) {
        r30 = 0x0;
    } else {

        r30 = *(s16*)((u8*)r3 + 0x2);
    }
    r0 = (s16)r30;
    if ((u32)r3 <= (u32)0x0) goto L_8013413C;
    r12 = *(u32*)lbl_80478F88;
    if ((u32)r12 == (u32)0x0) {
        r3 = 0x0;
    } else {

        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    r0 = (s16)r30;
    if ((s32)r3 > (s32)r0) goto L_80134140;
L_8013413C: ;
    r30 = 0x0;
L_80134140: ;
    r0 = (s16)r30;
L_80134144: ;
    r3 = r0 + r31;
L_80134148: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}

/* 0x80134164 | 0xC4 */
void fn_80134164(void) {
    extern u8 lbl_80478F88[];
    extern u8 lbl_80478F8C[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r12 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 <= (s32)0) goto L_801341A0;
    r12 = *(u32*)lbl_80478F88;
    if ((u32)r12 == (u32)0x0) {
        r3 = 0x0;
    } else {

        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    if ((s32)r3 > (s32)r31) goto L_801341A8;
L_801341A0: ;
    r3 = 0x0;
    r31 = *(u32*)(sp + 0xC);
    return;
L_801341A8: ;
    r12 = *(u32*)lbl_80478F8C;
    if ((u32)r12 == (u32)0x0) {
        r3 = 0x0;
    } else {

        r3 = r31;
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    if ((u32)r3 == (u32)0x0) {
        r31 = 0x0;
    } else {

        r31 = *(s16*)((u8*)r3 + 0x2);
    }
    r0 = (s16)r31;
    if ((u32)r3 <= (u32)0x0) goto L_8013420C;
    r12 = *(u32*)lbl_80478F88;
    if ((u32)r12 == (u32)0x0) {
        r3 = 0x0;
    } else {

        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    r0 = (s16)r31;
    if ((s32)r3 > (s32)r0) goto L_80134210;
L_8013420C: ;
    r31 = 0x0;
L_80134210: ;
    r3 = r31;

    r31 = *(u32*)(sp + 0xC);
    return;
}

/* 0x80134228 | 0x30 -- saturate add: max(0, lbl_80478848 + arg) then fn_80104704 */
extern void* fn_80104704(s32 key);
void* fn_80134228(s32 offset) {
    s32 key = (s32)lbl_80478848 + offset;
    if (key < 0) key = 0;
    return fn_80104704(key);
}

/* 0x80134258 | 0x1C -- get relative key from obj->0x04, return (key - lbl_80478848), or -1 */
s32 fn_80134258(void* obj) {
    u32 val = *(u32*)((u8*)obj + 0x04);
    if ((s32)val < (s32)lbl_80478848) return -1;
    return (s32)(val - lbl_80478848);
}

/* 0x44 | fn_80134274 | call_then_multi_check */
u32 fn_80134274(void) {
    u32 result = fn_800057A8();
    /* multi-branch on result */
    return result;
}

/* 0x4C | fn_801342B8 | framed_no_calls */
u32 fn_801342B8(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_80478F8C */
    return 0;
}

/* 0x80134304 | 0x38 */
void fn_80134304(void) {
    extern u8 lbl_80478F88[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r12 = 0;
    void (*ctr_fn)(void) = 0;

    r12 = *(u32*)lbl_80478F88;
    if ((u32)r12 == (u32)0x0) {
        r3 = 0x0;
    } else {

        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    return;
}

/* 0x8013433C | 0xE4 */
void fn_8013433C(void) {
    extern void fn_80140A9C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r4;
    r30 = r5;
    /* mr. r29, r3 */;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        r4 = 0x3;
        ((void(*)(void))fn_80129280)();
    }
    r0 = (s16)r31;
    if ((s32)r0 < (s32)0) goto L_80134384;
    r0 = (s16)r31;
    if ((s32)r0 < (s32)0xeb) goto L_8013438C;
L_80134384: ;
    r31 = 0x0;
    goto L_80134398;
L_8013438C: ;
    r4 = r0 << 2;
    r31 = r4 + 0x6dec;
    r31 = r3 + r31;
L_80134398: ;
    if ((u32)r31 != (u32)0x0) goto L_801343A8;
    r3 = 0x0;
    goto L_80134404;
L_801343A8: ;
    r3 = r29;
    if ((u32)r29 == (u32)0x0) {
        r3 = 0x0;
        r4 = 0x3;
        ((void(*)(void))fn_80129280)();
    }
    r0 = (s16)r30;
    if ((u32)r29 < (u32)0x0) goto L_801343D4;
    r0 = (s16)r30;
    if ((s32)r0 < (s32)0xeb) goto L_801343DC;
L_801343D4: ;
    r4 = 0x0;
    goto L_801343E8;
L_801343DC: ;
    r4 = r0 << 2;
    r4 = r4 + 0x6dec;
    r4 = r3 + r4;
L_801343E8: ;
    if ((u32)r4 != (u32)0x0) goto L_801343F8;
    r3 = 0x0;
    goto L_80134404;
L_801343F8: ;
    r3 = r31;
    fn_80140A9C();
    r3 = 0x1;
L_80134404: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}

/* 0x80134420 | 0x164 */
void fn_80134420(void) {
    extern void fn_801429E8();
    extern void fn_80142CF4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r3;
    r28 = r4;
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        r4 = 0x3;
        ((void(*)(void))fn_80129280)();
        r31 = r3;
    }
    r3 = r28;
    ((void(*)(void))fn_801440A0)();
    if ((u32)r3 != (u32)0x0) goto L_80134474;
    r3 = 0x0;
    goto L_80134564;
L_80134474: ;
    r29 = r31;
    r28 = r28 & 0xFFFF;
    r30 = 0x0;
L_80134480: ;
    r3 = r29 + 0x6dec;
    fn_801429E8();
    r0 = r3 & 0xFF;
    if ((u32)r3 == (u32)0x0) goto L_801344AC;
    r3 = r29 + 0x6dec;
    r4 = 0x0;
    r5 = 0x1b;
    r6 = 0x0;
    fn_80142CF4();
    if ((s32)r28 == (s32)r3) goto L_801344BC;
L_801344AC: ;
    r30 = r30 + 0x1;
    r29 = r29 + 0x4;
    if ((s32)r30 < (s32)0xeb) goto L_80134480;
L_801344BC: ;
    r3 = -0x1;
    if ((s32)r30 < (s32)0xeb) {
        r3 = (s16)r30;
    }
    r0 = (s16)r3;
    if ((s32)r30 < (s32)0xeb) goto L_80134558;
    r0 = (s16)r3;
    if ((s32)r30 < (s32)0xeb) goto L_801344E8;
    r0 = (s16)r3;
    if ((s32)r0 < (s32)0xeb) goto L_801344F0;
L_801344E8: ;
    r30 = 0x0;
    goto L_801344FC;
L_801344F0: ;
    r3 = r0 << 2;
    r30 = r3 + 0x6dec;
    r30 = r31 + r30;
L_801344FC: ;
    if ((u32)r30 != (u32)0x0) goto L_80134510;
    r3 = (0x1 << 16);
    goto L_80134544;
L_80134510: ;
    r3 = r30;
    fn_801429E8();
    r0 = r3 & 0xFF;
    if ((u32)r30 != (u32)0x0) goto L_8013452C;
    r3 = (0x1 << 16);
    goto L_80134544;
L_8013452C: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x1c;
    r6 = 0x0;
    fn_80142CF4();
    r3 = r3 & 0xFFFF;
L_80134544: ;
    r0 = r3 & 0xFFFF;
    if ((u32)r0 <= (u32)0x3e7) goto L_8013455C;
    r3 = 0x0;
    goto L_8013455C;
L_80134558: ;
    r3 = 0x0;
L_8013455C: ;
    r0 = 0x3e7 - r3;
    r3 = r0 & 0xFFFF;
L_80134564: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}

/* 0x80134584 | 0xF8 */
void fn_80134584(void) {
    extern void fn_80140ACC();
    extern void fn_801429E8();
    extern void fn_80142CF4();
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

    r26 = r4;
    r27 = r5;
    r31 = r3;
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        r4 = 0x3;
        ((void(*)(void))fn_80129280)();
        r31 = r3;
    }
    r0 = r27 & 0xFFFF;
    if ((u32)r3 == (u32)0x0) {
        r3 = r27;
        return;
    }
    r3 = r26;
    ((void(*)(void))fn_801440A0)();
    if ((u32)r3 == (u32)0x0) {
        r3 = r27;
        return;
    }
    r29 = r31;
    r28 = r26 & 0xFFFF;
    r30 = 0x0;
L_801345EC: ;
    r3 = r29 + 0x6dec;
    fn_801429E8();
    r0 = r3 & 0xFF;
    if ((u32)r3 == (u32)0x0) goto L_80134618;
    r3 = r29 + 0x6dec;
    r4 = 0x0;
    r5 = 0x1b;
    r6 = 0x0;
    fn_80142CF4();
    if ((s32)r28 == (s32)r3) goto L_80134628;
L_80134618: ;
    r30 = r30 + 0x1;
    r29 = r29 + 0x4;
    if ((s32)r30 < (s32)0xeb) goto L_801345EC;
L_80134628: ;
    r7 = -0x1;
    if ((s32)r30 < (s32)0xeb) {
        r7 = (s16)r30;
    }
    r0 = (s16)r7;
    if ((s32)r30 < (s32)0xeb) {
        r3 = r27;
        return;
    }
    r5 = r26;
    r6 = r27;
    r3 = r31 + 0x6dec;
    r4 = 0xeb;
    r8 = 0x3e7;
    r9 = 0x0;
    fn_80140ACC();
    r3 = r3 & 0xFFFF;

    return;
}

/* 0x8013467C | 0xEC */
void fn_8013467C(void) {
    extern void fn_80141308();
    extern void fn_801429E8();
    extern void fn_80142CF4();
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
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r26 = r4;
    r27 = r5;
    r31 = r3;
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        r4 = 0x3;
        ((void(*)(void))fn_80129280)();
        r31 = r3;
    }
    r0 = r27 & 0xFFFF;
    if ((u32)r3 == (u32)0x0) {
        r3 = r27;
        return;
    }
    r3 = r26;
    ((void(*)(void))fn_801440A0)();
    if ((u32)r3 == (u32)0x0) {
        r3 = r27;
        return;
    }
    r29 = r31;
    r28 = r26 & 0xFFFF;
    r30 = 0x0;
L_801346E4: ;
    r3 = r29 + 0x6dec;
    fn_801429E8();
    r0 = r3 & 0xFF;
    if ((u32)r3 == (u32)0x0) goto L_80134710;
    r3 = r29 + 0x6dec;
    r4 = 0x0;
    r5 = 0x1b;
    r6 = 0x0;
    fn_80142CF4();
    if ((s32)r28 == (s32)r3) goto L_80134720;
L_80134710: ;
    r30 = r30 + 0x1;
    r29 = r29 + 0x4;
    if ((s32)r30 < (s32)0xeb) goto L_801346E4;
L_80134720: ;
    r5 = r26;
    r6 = r27;
    r3 = r31 + 0x6dec;
    r4 = 0xeb;
    r7 = -0x1;
    if ((s32)r30 < (s32)0xeb) {
        r7 = (s16)r30;
    }
    r8 = 0x3e7;
    r9 = 0x0;
    r10 = 0x0;
    fn_80141308();
    r3 = r3 & 0xFFFF;

    return;
}

/* 0x68 | fn_80134768 | generic */
u32 fn_80134768(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_80129280(0, 0);
    return 0;
}

/* 0x801347D0 | 0x8 | return_const */
u32 fn_801347D0(void) { return 235; }

/* 0x801347D8 | 0x8 | return_const */
u32 fn_801347D8(void) { return 30; }

/* 0x801347E0 | 0x8 | return_const */
u32 fn_801347E0(void) { return 3; }

/* 0x801347E8 | 0x104 */
void fn_801347E8(void) {
    extern void fn_80123FBC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = 0x0;
    r28 = r4;
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        r4 = 0x3;
        ((void(*)(void))fn_80129280)();
    }
    r0 = (s8)r28;
    if ((u32)r3 < (u32)0x0) goto L_80134834;
    r0 = (s8)r28;
    if ((s32)r0 < (s32)0x3) goto L_8013483C;
L_80134834: ;
    r30 = -0x1;
    goto L_801348B4;
L_8013483C: ;
    r0 = r0 * 0x24a4;
    r31 = 0x0;
    r29 = r3 + r0;
    goto L_801348A8;
L_8013484C: ;
    r0 = (s8)r28;
    if ((s32)r0 < (s32)0x3) goto L_80134860;
    r0 = (s8)r28;
    if ((s32)r0 < (s32)0x3) goto L_80134868;
L_80134860: ;
    r3 = 0x0;
    goto L_80134888;
L_80134868: ;
    r0 = (s8)r31;
    if ((s32)r0 < (s32)0x3) goto L_8013487C;
    r0 = (s8)r31;
    if ((s32)r0 < (s32)0x1e) goto L_80134884;
L_8013487C: ;
    r3 = 0x0;
    goto L_80134888;
L_80134884: ;
    r3 = r29 + 0x14;
L_80134888: ;
    if ((u32)r3 != (u32)0x0) {
        fn_80123FBC();
        r0 = r3 & 0xFF;
        if ((u32)r3 != (u32)0x0) {
            r30 = r30 + 0x1;
    }
    }
    r29 = r29 + 0x138;
    r31 = r31 + 0x1;
L_801348A8: ;
    r0 = (s8)r31;
    if ((s32)r0 < (s32)0x1e) goto L_8013484C;
L_801348B4: ;
    r0 = (s8)r30;
    if ((s32)r0 < (s32)0x1e) {
        r3 = -0x1;
    } else {

        r0 = 0x1e - r30;
        r3 = (s8)r0;
    }
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}

/* 0x801348EC | 0xF0 */
void fn_801348EC(void) {
    extern void fn_80123FBC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = 0x0;
    r28 = r4;
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        r4 = 0x3;
        ((void(*)(void))fn_80129280)();
    }
    r0 = (s8)r28;
    if ((u32)r3 < (u32)0x0) goto L_80134938;
    r0 = (s8)r28;
    if ((s32)r0 < (s32)0x3) goto L_80134940;
L_80134938: ;
    r3 = -0x1;
    goto L_801349BC;
L_80134940: ;
    r0 = r0 * 0x24a4;
    r29 = 0x0;
    r31 = r3 + r0;
    goto L_801349AC;
L_80134950: ;
    r0 = (s8)r28;
    if ((s32)r0 < (s32)0x3) goto L_80134964;
    r0 = (s8)r28;
    if ((s32)r0 < (s32)0x3) goto L_8013496C;
L_80134964: ;
    r3 = 0x0;
    goto L_8013498C;
L_8013496C: ;
    r0 = (s8)r29;
    if ((s32)r0 < (s32)0x3) goto L_80134980;
    r0 = (s8)r29;
    if ((s32)r0 < (s32)0x1e) goto L_80134988;
L_80134980: ;
    r3 = 0x0;
    goto L_8013498C;
L_80134988: ;
    r3 = r31 + 0x14;
L_8013498C: ;
    if ((u32)r3 != (u32)0x0) {
        fn_80123FBC();
        r0 = r3 & 0xFF;
        if ((u32)r3 != (u32)0x0) {
            r30 = r30 + 0x1;
    }
    }
    r31 = r31 + 0x138;
    r29 = r29 + 0x1;
L_801349AC: ;
    r0 = (s8)r29;
    if ((s32)r0 < (s32)0x1e) goto L_80134950;
    r3 = r30;
L_801349BC: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}

/* 0x801349DC | 0xBC */
void fn_801349DC(void) {
    extern void fn_800F9E70();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r5;
    r30 = r4;
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        r4 = 0x3;
        ((void(*)(void))fn_80129280)();
    }
    r0 = (s8)r30;
    if ((u32)r3 < (u32)0x0) goto L_80134A20;
    r0 = (s8)r30;
    if ((s32)r0 < (s32)0x3) goto L_80134A28;
L_80134A20: ;
    r3 = 0x0;
    goto L_80134A80;
L_80134A28: ;
    if ((u32)r31 != (u32)0x0) goto L_80134A38;
    r3 = 0x0;
    goto L_80134A80;
L_80134A38: ;
    r5 = r31;
    r4 = 0x0;
    while (1) {
        r0 = *(u16*)((u8*)r5 + 0x0);
        if ((u32)r0 == (u32)0x0) break;
        r5 = r5 + 0x2;
        r4 = r4 + 0x1;


    }
    if ((s32)r4 <= (s32)0x8) goto L_80134A68;
    r3 = 0x0;
    goto L_80134A80;
L_80134A68: ;
    r0 = (s8)r30;
    r4 = r31;
    r0 = r0 * 0x24a4;
    r3 = r3 + r0;
    fn_800F9E70();
    r3 = 0x1;
L_80134A80: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}

/* 0x60 | fn_80134A98 | generic */
u32 fn_80134A98(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_80129280(0, 0);
    return 0;
}

/* 0x80134AF8 | 0xC8 */
void fn_80134AF8(void) {
    extern void fn_80123FBC();
    extern void fn_80124A60();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r5;
    r30 = r4;
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        r4 = 0x3;
        ((void(*)(void))fn_80129280)();
    }
    r0 = (s8)r30;
    if ((u32)r3 < (u32)0x0) goto L_80134B3C;
    r4 = (s8)r30;
    if ((s32)r4 < (s32)0x3) goto L_80134B44;
L_80134B3C: ;
    r31 = 0x0;
    goto L_80134B74;
L_80134B44: ;
    r0 = (s8)r31;
    if ((s32)r4 < (s32)0x3) goto L_80134B58;
    r0 = (s8)r31;
    if ((s32)r0 < (s32)0x1e) goto L_80134B60;
L_80134B58: ;
    r31 = 0x0;
    goto L_80134B74;
L_80134B60: ;
    r4 = r4 * 0x24a4;
    r0 = r0 * 0x138;
    r4 = r4 + r0;
    r31 = r4 + 0x14;
    r31 = r3 + r31;
L_80134B74: ;
    if ((u32)r31 != (u32)0x0) goto L_80134B84;
    r3 = 0x0;
    goto L_80134BA8;
L_80134B84: ;
    r3 = r31;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r31 != (u32)0x0) goto L_80134B9C;
    r3 = 0x0;
    goto L_80134BA8;
L_80134B9C: ;
    r3 = r31;
    fn_80124A60();
    r3 = 0x1;
L_80134BA8: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}

/* 0x80134BC0 | 0x250 */
void fn_80134BC0(void) {
    extern void fn_8012086C();
    extern void fn_80123FBC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r29 = r4;
    r30 = r5;
    r31 = r3;
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        r4 = 0x3;
        ((void(*)(void))fn_80129280)();
        r31 = r3;
    }
    r0 = (s8)r30;
    if ((s32)r0 < (s32)-0x1) { r3 = 0x0; return; }
    if ((s32)r0 >= (s32)0x3) {

        r3 = 0x0;
        return;
    }
    if ((u32)r29 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r0 != (s32)-0x1) goto L_80134CE0;
    r27 = r31;
    r30 = 0x0;
    goto L_80134CC0;
L_80134C34: ;
    r28 = 0x0;
L_80134C38: ;
    r0 = (s8)r30;
    r3 = (s8)r28;
    if ((s32)r0 < (s32)-0x1) goto L_80134C50;
    r0 = (s8)r30;
    if ((s32)r0 < (s32)0x3) goto L_80134C58;
L_80134C50: ;
    r3 = 0x0;
    goto L_80134C7C;
L_80134C58: ;
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0x3) goto L_80134C68;
    if ((s32)r3 < (s32)0x1e) goto L_80134C70;
L_80134C68: ;
    r3 = 0x0;
    goto L_80134C7C;
L_80134C70: ;
    r3 = r3 * 0x138;
    r3 = r3 + 0x14;
    r3 = r27 + r3;
L_80134C7C: ;
    if ((u32)r3 == (u32)0x0) goto L_80134C90;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 == (u32)0x0) goto L_80134C9C;
L_80134C90: ;
    r28 = r28 + 0x1;
    if ((s32)r28 < (s32)0x1e) goto L_80134C38;
L_80134C9C: ;
    r3 = -0x1;
    if ((s32)r28 < (s32)0x1e) {
        r3 = (s8)r28;
    }
    r0 = (s8)r3;
    r28 = r3;
    if ((s32)r28 >= (s32)0x1e) goto L_80134CCC;
    r27 = r27 + 0x24a4;
    r30 = r30 + 0x1;
L_80134CC0: ;
    r0 = (s8)r30;
    if ((s32)r0 < (s32)0x3) goto L_80134C34;
L_80134CCC: ;
    r0 = (s8)r30;
    if ((s32)r0 < (s32)0x3) goto L_80134D74;
    r3 = 0x0;
    return;
L_80134CE0: ;
    r0 = r0 * 0x24a4;
    r28 = 0x0;
    r27 = r31 + r0;
L_80134CEC: ;
    r0 = (s8)r30;
    r3 = (s8)r28;
    if ((s32)r0 < (s32)0x3) goto L_80134D04;
    r0 = (s8)r30;
    if ((s32)r0 < (s32)0x3) goto L_80134D0C;
L_80134D04: ;
    r3 = 0x0;
    goto L_80134D30;
L_80134D0C: ;
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0x3) goto L_80134D1C;
    if ((s32)r3 < (s32)0x1e) goto L_80134D24;
L_80134D1C: ;
    r3 = 0x0;
    goto L_80134D30;
L_80134D24: ;
    r3 = r3 * 0x138;
    r3 = r3 + 0x14;
    r3 = r27 + r3;
L_80134D30: ;
    if ((u32)r3 == (u32)0x0) goto L_80134D44;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 == (u32)0x0) goto L_80134D50;
L_80134D44: ;
    r28 = r28 + 0x1;
    if ((s32)r28 < (s32)0x1e) goto L_80134CEC;
L_80134D50: ;
    r3 = -0x1;
    if ((s32)r28 < (s32)0x1e) {
        r3 = (s8)r28;
    }
    r0 = (s8)r3;
    r28 = r3;
    if ((s32)r28 >= (s32)0x1e) goto L_80134D74;
    r3 = 0x0;
    return;
L_80134D74: ;
    r0 = (s8)r30;
    if ((s32)r28 < (s32)0x1e) goto L_80134D88;
    r3 = (s8)r30;
    if ((s32)r3 < (s32)0x3) goto L_80134D90;
L_80134D88: ;
    r3 = 0x0;
    goto L_80134DC0;
L_80134D90: ;
    r0 = (s8)r28;
    if ((s32)r3 < (s32)0x3) goto L_80134DA4;
    r0 = (s8)r28;
    if ((s32)r0 < (s32)0x1e) goto L_80134DAC;
L_80134DA4: ;
    r3 = 0x0;
    goto L_80134DC0;
L_80134DAC: ;
    r3 = r3 * 0x24a4;
    r0 = r0 * 0x138;
    r3 = r3 + r0;
    r3 = r3 + 0x14;
    r3 = r31 + r3;
L_80134DC0: ;
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r0 = 0x27;
    ctr_fn = (void(*)(void))r0;
    do {
        r4 = *(u32*)((u8*)r5 + 0x4);
        r0 = *(u32*)((u8*)r5 + 0x8);
        *(u32*)((u8*)r6 + 0x4) = r4;
        r6 += 8; *(u32*)r6 = r0;
    } while (--ctr != 0);
    fn_8012086C();
    r3 = 0x1;

    return;
}

/* 0x80134E10 | 0xE0 */
void fn_80134E10(void) {
    extern void fn_8012086C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r31 = r6;
    r30 = r5;
    r29 = r4;
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        r4 = 0x3;
        ((void(*)(void))fn_80129280)();
    }
    r0 = (s8)r30;
    if ((u32)r3 < (u32)0x0) goto L_80134E5C;
    r4 = (s8)r30;
    if ((s32)r4 < (s32)0x3) goto L_80134E64;
L_80134E5C: ;
    r6 = 0x0;
    goto L_80134E94;
L_80134E64: ;
    r0 = (s8)r31;
    if ((s32)r4 < (s32)0x3) goto L_80134E78;
    r0 = (s8)r31;
    if ((s32)r0 < (s32)0x1e) goto L_80134E80;
L_80134E78: ;
    r6 = 0x0;
    goto L_80134E94;
L_80134E80: ;
    r4 = r4 * 0x24a4;
    r0 = r0 * 0x138;
    r4 = r4 + r0;
    r6 = r4 + 0x14;
    r6 = r3 + r6;
L_80134E94: ;
    if ((u32)r6 == (u32)0x0) {
        r3 = 0x0;
    } else {

        r0 = 0x27;
        ctr_fn = (void(*)(void))r0;
        do {
            r3 = *(u32*)((u8*)r4 + 0x4);
            r0 = *(u32*)((u8*)r4 + 0x8);
            *(u32*)((u8*)r5 + 0x4) = r3;
            r5 += 8; *(u32*)r5 = r0;
        } while (--ctr != 0);
        r3 = r6;
        fn_8012086C();
        r3 = 0x1;
    }
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}

/* 0x80134EF0 | 0x98 */
void fn_80134EF0(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r5;
    r30 = r4;
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        r4 = 0x3;
        ((void(*)(void))fn_80129280)();
    }
    r0 = (s8)r30;
    if ((u32)r3 < (u32)0x0) goto L_80134F34;
    r4 = (s8)r30;
    if ((s32)r4 < (s32)0x3) goto L_80134F3C;
L_80134F34: ;
    r0 = 0x0;
    goto L_80134F6C;
L_80134F3C: ;
    r0 = (s8)r31;
    if ((s32)r4 < (s32)0x3) goto L_80134F50;
    r0 = (s8)r31;
    if ((s32)r0 < (s32)0x1e) goto L_80134F58;
L_80134F50: ;
    r0 = 0x0;
    goto L_80134F6C;
L_80134F58: ;
    r4 = r4 * 0x24a4;
    r0 = r0 * 0x138;
    r4 = r4 + r0;
    r0 = r4 + 0x14;
    r0 = r3 + r0;
L_80134F6C: ;
    r3 = r0;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}

/* 0x80134F88 | 0x9C */
void fn_80134F88(void) {
    extern void fn_800F96E4();
    extern void fn_801249F8();
    extern void fn_80132A38();
    extern void fn_80142A88();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r3;
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        r4 = 0x3;
        ((void(*)(void))fn_80129280)();
        r31 = r3;
    }
    r30 = 0x0;
    r29 = r31;
    do {
        r4 = r30 + 0x1;
        r3 = 0x34;
        fn_80132A38();
        r3 = r29;
        r4 = 0x9;
        r5 = 0x32c9;
        fn_800F96E4();
        r3 = r29 + 0x14;
        r4 = 0x1e;
        fn_801249F8();
        r30 = r30 + 0x1;
        r29 = r29 + 0x24a4;
    } while ((s32)r30 < (s32)0x3);
    r3 = r31 + 0x6dec;
    r4 = 0xeb;
    fn_80142A88();
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}

/* 0x80135024 | 0x4 | void_stub */
void fn_80135024(void) {
}

/* 0x80135028 | 0x8 | return_const */
u32 fn_80135028(void) { return 0; }

/* 0x80135030 | 0x138 */
void fn_80135030(void) {
    extern u8 lbl_8047D108[];
    extern void fn_80135B1C();
    extern void fn_80135B2C();
    extern void fn_80135B3C();
    extern void fn_80135B4C();
    extern void fn_80135B5C();
    extern void fn_80135B6C();
    extern void fn_80135B7C();
    extern void fn_80135BA0();
    extern void fn_80135C90();
    extern void fn_80135CD0();
    extern u8 jumptable_80363A70[];
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r0 = r4 & 0xFFFF;
    r31 = r5;
    r30 = r4;
    r29 = r3;
    if ((s32)r0 == (s32)0) goto L_8013514C;
    if ((u32)r0 < (u32)0xb) goto L_80135068;
    goto L_8013514C;
L_80135068: ;
    if ((u32)r29 != (u32)0x0) goto L_80135094;
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 == (u32)0x0) goto L_8013514C;
    r4 = 0x1;
    ((void(*)(void))fn_80129280)();
    /* mr. r29, r3 */;
    if ((u32)r3 == (u32)0x0) goto L_8013514C;
L_80135094: ;
    r3 = r29;
    fn_80135CD0();
    if ((u32)r3 == (u32)0x0) goto L_8013514C;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 > (u32)0xa) goto L_8013514C;
    r4 = (u32)jumptable_80363A70;
    r0 = r0 << 2;
    r4 = (u32)jumptable_80363A70;
    r0 = *(u32*)(r4 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r3 = r29;
    r4 = r31;
    fn_80135C90();
    goto L_8013514C;
    r4 = r31;
    fn_80135BA0();
    goto L_8013514C;
    r4 = r31;
    fn_80135B7C();
    goto L_8013514C;
    r4 = r31;
    fn_80135B6C();
    goto L_8013514C;
    r0 = (0x4330 << 16);
    f1 = *(f64*)lbl_8047D108;
    *(u32*)(sp + 0x8) = r0;
    f0 = *(f64*)(sp + 0x8);
    f1 = f0 - f1;
    fn_80135B5C();
    goto L_8013514C;
    r4 = r31;
    fn_80135B4C();
    goto L_8013514C;
    r4 = r31 & 0xFF;
    fn_80135B3C();
    goto L_8013514C;
    r4 = r31 & 0xFF;
    fn_80135B2C();
    goto L_8013514C;
    r4 = r31 & 0xFF;
    fn_80135B1C();
L_8013514C: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}

/* 0x80135168 | 0x124 */
void fn_80135168(void) {
    extern void fn_80135BB0();
    extern void fn_80135BC8();
    extern void fn_80135BE0();
    extern void fn_80135BF8();
    extern void fn_80135C10();
    extern void fn_80135C28();
    extern void fn_80135C40();
    extern void fn_80135C78();
    extern void fn_80135CD0();
    extern u8 jumptable_80363A9C[];
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r0 = r4 & 0xFFFF;
    r31 = 0x0;
    r30 = r4;
    if ((s32)r0 == (s32)0) goto L_80135194;
    if ((u32)r0 < (u32)0xb) goto L_8013519C;
L_80135194: ;
    r3 = 0x0;
    goto L_80135274;
L_8013519C: ;
    if ((u32)r3 != (u32)0x0) goto L_801351D8;
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    /* mr. r31, r3 */;
    if ((u32)r3 != (u32)0x0) goto L_801351C0;
    r3 = 0x0;
    goto L_80135274;
L_801351C0: ;
    r4 = 0x1;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 != (u32)0x0) goto L_801351D8;
    r3 = 0x0;
    goto L_80135274;
L_801351D8: ;
    fn_80135CD0();
    if ((u32)r3 != (u32)0x0) goto L_801351EC;
    r3 = 0x0;
    goto L_80135274;
L_801351EC: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 > (u32)0xa) goto L_80135270;
    r4 = (u32)jumptable_80363A9C;
    r0 = r0 << 2;
    r4 = (u32)jumptable_80363A9C;
    r0 = *(u32*)(r4 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r3 = r31;
    goto L_80135274;
    fn_80135C78();
    goto L_80135274;
    fn_80135C40();
    goto L_80135274;
    fn_80135C28();
    goto L_80135274;
    fn_80135C10();
    f0 = (f64)(s32)f1;
    *(f64*)(sp + 0x8) = f0;
    r3 = *(u32*)(sp + 0xC);
    goto L_80135274;
    fn_80135BF8();
    goto L_80135274;
    fn_80135BE0();
    r3 = r3 & 0xFF;
    goto L_80135274;
    fn_80135BC8();
    r3 = r3 & 0xFF;
    goto L_80135274;
    fn_80135BB0();
    r3 = r3 & 0xFF;
    goto L_80135274;
L_80135270: ;
    r3 = 0x0;
L_80135274: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    return;
}

/* 0x8013528C | 0xAC */
void fn_8013528C(void) {
    extern void fn_80135338();
    extern void fn_801353C0();
    extern void fn_80135B0C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* mr. r27, r3 */;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r31 = r7;
    if ((s32)r0 == (s32)0) return;
    fn_80135338();
    r3 = r27;
    if ((u32)r27 != (u32)0x0) goto L_801352F8;
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 != (u32)0x0) goto L_801352E0;
    r3 = 0x0;
    goto L_80135308;
L_801352E0: ;
    r4 = 0x1;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 != (u32)0x0) goto L_801352F8;
    r3 = 0x0;
    goto L_80135308;
L_801352F8: ;
    fn_80135B0C();
    if ((u32)r3 != (u32)0x0) goto L_80135308;
    r3 = 0x0;
L_80135308: ;
    if ((u32)r3 == (u32)0x0) return;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = r31;
    fn_801353C0();

    return;
}

/* 0x80135338 | 0x88 */
void fn_80135338(void) {
    extern void fn_80135708();
    extern void fn_80135B0C();
    extern void fn_80135CE8();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r31 = r3;
    if ((u32)r3 != (u32)0x0) goto L_80135388;
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 != (u32)0x0) goto L_80135370;
    r3 = 0x0;
    goto L_80135398;
L_80135370: ;
    r4 = 0x1;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 != (u32)0x0) goto L_80135388;
    r3 = 0x0;
    goto L_80135398;
L_80135388: ;
    fn_80135B0C();
    if ((u32)r3 != (u32)0x0) goto L_80135398;
    r3 = 0x0;
L_80135398: ;
    if ((u32)r3 != (u32)0x0) {
        fn_80135708();
        r3 = r31;
        fn_80135CE8();
    }
    r31 = *(u32*)(sp + 0xC);
    return;
}

/* 0x801353C0 | 0x170 */
void fn_801353C0(void) {
    extern void fn_80135708();
    extern void fn_80135A30();
    extern void fn_80135A40();
    extern void fn_80135A50();
    extern void fn_80135A60();
    extern void fn_80135B0C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* mr. r27, r3 */;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r31 = r7;
    if ((s32)r0 == (s32)0) return;
    r0 = r28 & 0xFF;
    if ((s32)r0 == (s32)0) return;
    r0 = r29 & 0xFF;
    if ((s32)r0 == (s32)0) return;
    r0 = r30 & 0xFF;
    if ((s32)r0 == (s32)0) return;
    r0 = r31 & 0xFF;
    if ((s32)r0 == (s32)0) return;
    fn_80135708();
    r3 = r27;
    if ((u32)r27 != (u32)0x0) goto L_8013543C;
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 == (u32)0x0) goto L_80135450;
    r4 = 0x1;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 == (u32)0x0) goto L_80135450;
L_8013543C: ;
    fn_80135B0C();
    if ((u32)r3 == (u32)0x0) goto L_80135450;
    r4 = r28 & 0xFF;
    fn_80135A60();
L_80135450: ;
    r3 = r27;
    if ((u32)r27 != (u32)0x0) goto L_80135480;
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 == (u32)0x0) goto L_80135494;
    r4 = 0x1;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 == (u32)0x0) goto L_80135494;
L_80135480: ;
    fn_80135B0C();
    if ((u32)r3 == (u32)0x0) goto L_80135494;
    r4 = r29 & 0xFF;
    fn_80135A50();
L_80135494: ;
    r3 = r27;
    if ((u32)r27 != (u32)0x0) goto L_801354C4;
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 == (u32)0x0) goto L_801354D8;
    r4 = 0x1;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 == (u32)0x0) goto L_801354D8;
L_801354C4: ;
    fn_80135B0C();
    if ((u32)r3 == (u32)0x0) goto L_801354D8;
    r4 = r30 & 0xFF;
    fn_80135A40();
L_801354D8: ;
    r3 = r27;
    if ((u32)r27 == (u32)0x0) {
        r3 = 0x0;
        r4 = 0x0;
        ((void(*)(void))fn_80129280)();
        if ((u32)r3 == (u32)0x0) return;
        r4 = 0x1;
        ((void(*)(void))fn_80129280)();
        if ((u32)r3 == (u32)0x0) return;
    }
    fn_80135B0C();
    if ((u32)r3 == (u32)0x0) return;
    r4 = r31 & 0xFF;
    fn_80135A30();

    return;
}

/* 0x80135530 | 0x1D8 */
void fn_80135530(void) {
    extern void fn_80135A70();
    extern void fn_80135A88();
    extern void fn_80135AA0();
    extern void fn_80135AB8();
    extern void fn_80135B0C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        r31 = *(u32*)(sp + 0xC);
        return;
    }
    if ((s32)r0 != (s32)0) goto L_80135588;
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 != (u32)0x0) goto L_80135570;
    r0 = 0x0;
    goto L_801355A4;
L_80135570: ;
    r4 = 0x1;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 != (u32)0x0) goto L_80135588;
    r0 = 0x0;
    goto L_801355A4;
L_80135588: ;
    fn_80135B0C();
    if ((u32)r3 != (u32)0x0) goto L_8013559C;
    r0 = 0x0;
    goto L_801355A4;
L_8013559C: ;
    fn_80135AB8();
    r0 = r3 & 0xFF;
L_801355A4: ;
    if ((s32)r0 == (s32)0x0) {
        r3 = 0x0;
        r31 = *(u32*)(sp + 0xC);
        return;
    }
    r3 = r31;
    if ((u32)r31 != (u32)0x0) goto L_801355F4;
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 != (u32)0x0) goto L_801355DC;
    r0 = 0x0;
    goto L_80135610;
L_801355DC: ;
    r4 = 0x1;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 != (u32)0x0) goto L_801355F4;
    r0 = 0x0;
    goto L_80135610;
L_801355F4: ;
    fn_80135B0C();
    if ((u32)r3 != (u32)0x0) goto L_80135608;
    r0 = 0x0;
    goto L_80135610;
L_80135608: ;
    fn_80135AA0();
    r0 = r3 & 0xFF;
L_80135610: ;
    if ((s32)r0 == (s32)0x0) {
        r3 = 0x0;
        r31 = *(u32*)(sp + 0xC);
        return;
    }
    r3 = r31;
    if ((u32)r31 != (u32)0x0) goto L_80135660;
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 != (u32)0x0) goto L_80135648;
    r0 = 0x0;
    goto L_8013567C;
L_80135648: ;
    r4 = 0x1;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 != (u32)0x0) goto L_80135660;
    r0 = 0x0;
    goto L_8013567C;
L_80135660: ;
    fn_80135B0C();
    if ((u32)r3 != (u32)0x0) goto L_80135674;
    r0 = 0x0;
    goto L_8013567C;
L_80135674: ;
    fn_80135A88();
    r0 = r3 & 0xFF;
L_8013567C: ;
    if ((s32)r0 == (s32)0x0) {
        r3 = 0x0;
        r31 = *(u32*)(sp + 0xC);
        return;
    }
    r3 = r31;
    if ((u32)r31 != (u32)0x0) goto L_801356CC;
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 != (u32)0x0) goto L_801356B4;
    r3 = 0x0;
    goto L_801356E8;
L_801356B4: ;
    r4 = 0x1;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 != (u32)0x0) goto L_801356CC;
    r3 = 0x0;
    goto L_801356E8;
L_801356CC: ;
    fn_80135B0C();
    if ((u32)r3 != (u32)0x0) goto L_801356E0;
    r3 = 0x0;
    goto L_801356E8;
L_801356E0: ;
    fn_80135A70();
    r3 = r3 & 0xFF;
L_801356E8: ;
    r0 = -r3;
    r0 = r0 | r3;
    r3 = (u32)r0 >> 31;

    r31 = *(u32*)(sp + 0xC);
    return;
}

/* 0x80135708 | 0x134 */
void fn_80135708(void) {
    extern void fn_80135A30();
    extern void fn_80135A40();
    extern void fn_80135A50();
    extern void fn_80135A60();
    extern void fn_80135B0C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) { r31 = *(u32*)(sp + 0xC); return; }
    if ((s32)r0 != (s32)0) goto L_80135748;
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 == (u32)0x0) goto L_8013575C;
    r4 = 0x1;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 == (u32)0x0) goto L_8013575C;
L_80135748: ;
    fn_80135B0C();
    if ((u32)r3 == (u32)0x0) goto L_8013575C;
    r4 = 0x0;
    fn_80135A60();
L_8013575C: ;
    r3 = r31;
    if ((u32)r31 != (u32)0x0) goto L_8013578C;
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 == (u32)0x0) goto L_801357A0;
    r4 = 0x1;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 == (u32)0x0) goto L_801357A0;
L_8013578C: ;
    fn_80135B0C();
    if ((u32)r3 == (u32)0x0) goto L_801357A0;
    r4 = 0x0;
    fn_80135A50();
L_801357A0: ;
    r3 = r31;
    if ((u32)r31 != (u32)0x0) goto L_801357D0;
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 == (u32)0x0) goto L_801357E4;
    r4 = 0x1;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 == (u32)0x0) goto L_801357E4;
L_801357D0: ;
    fn_80135B0C();
    if ((u32)r3 == (u32)0x0) goto L_801357E4;
    r4 = 0x0;
    fn_80135A40();
L_801357E4: ;
    r3 = r31;
    if ((u32)r31 == (u32)0x0) {
        r3 = 0x0;
        r4 = 0x0;
        ((void(*)(void))fn_80129280)();
        if ((u32)r3 == (u32)0x0) { r31 = *(u32*)(sp + 0xC); return; }
        r4 = 0x1;
        ((void(*)(void))fn_80129280)();
        if ((u32)r3 == (u32)0x0) { r31 = *(u32*)(sp + 0xC); return; }
    }
    fn_80135B0C();
    if ((u32)r3 == (u32)0x0) { r31 = *(u32*)(sp + 0xC); return; }
    r4 = 0x0;
    fn_80135A30();

    r31 = *(u32*)(sp + 0xC);
    return;
}

/* 0x8013583C | 0xFC */
void fn_8013583C(void) {
    extern void fn_80135A30();
    extern void fn_80135A40();
    extern void fn_80135A50();
    extern void fn_80135A60();
    extern void fn_80135AEC();
    extern void fn_80135B0C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r0 = r4 & 0xFFFF;
    r31 = r5;
    r30 = r4;
    r29 = r3;
    if ((s32)r0 == (s32)0) goto L_8013591C;
    if ((u32)r0 < (u32)0x7) goto L_80135874;
    goto L_8013591C;
L_80135874: ;
    if ((u32)r29 != (u32)0x0) goto L_801358A0;
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    if ((u32)r3 == (u32)0x0) goto L_8013591C;
    r4 = 0x1;
    ((void(*)(void))fn_80129280)();
    /* mr. r29, r3 */;
    if ((u32)r3 == (u32)0x0) goto L_8013591C;
L_801358A0: ;
    r3 = r29;
    fn_80135B0C();
    if ((u32)r3 == (u32)0x0) goto L_8013591C;
    r0 = r30 & 0xFFFF;
    if ((s32)r0 == (s32)0x3) goto L_801358FC;
    if ((s32)r0 >= (s32)0x3) goto L_801358D0;
    if ((s32)r0 == (s32)0x1) goto L_801358E0;
    if ((s32)r0 >= (s32)0x1) goto L_801358F0;
    goto L_8013591C;
L_801358D0: ;
    if ((s32)r0 == (s32)0x5) goto L_80135914;
    if ((s32)r0 >= (s32)0x5) goto L_8013591C;
    goto L_80135908;
L_801358E0: ;
    r3 = r29;
    r4 = r31;
    fn_80135AEC();
    goto L_8013591C;
L_801358F0: ;
    r4 = r31 & 0xFF;
    fn_80135A60();
    goto L_8013591C;
L_801358FC: ;
    r4 = r31 & 0xFF;
    fn_80135A50();
    goto L_8013591C;
L_80135908: ;
    r4 = r31 & 0xFF;
    fn_80135A40();
    goto L_8013591C;
L_80135914: ;
    r4 = r31 & 0xFF;
    fn_80135A30();
L_8013591C: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}

/* 0x80135938 | 0xF8 */
void fn_80135938(void) {
    extern void fn_80135A70();
    extern void fn_80135A88();
    extern void fn_80135AA0();
    extern void fn_80135AB8();
    extern void fn_80135B0C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r0 = r4 & 0xFFFF;
    r31 = r4;
    if ((s32)r0 == (s32)0) goto L_8013595C;
    if ((u32)r0 < (u32)0x7) goto L_80135964;
L_8013595C: ;
    r3 = 0x0;
    r31 = *(u32*)(sp + 0xC);
    return;
L_80135964: ;
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        r4 = 0x0;
        ((void(*)(void))fn_80129280)();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            r31 = *(u32*)(sp + 0xC);
            return;
        }
        r4 = 0x1;
        ((void(*)(void))fn_80129280)();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            r31 = *(u32*)(sp + 0xC);
            return;
    }
    }
    fn_80135B0C();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        r31 = *(u32*)(sp + 0xC);
        return;
    }
    r0 = r31 & 0xFFFF;
    if ((s32)r0 == (s32)0x3) goto L_801359F4;
    if ((s32)r0 >= (s32)0x3) goto L_801359D4;
    if ((s32)r0 == (s32)0x1) { r31 = *(u32*)(sp + 0xC); return; }
    if ((s32)r0 >= (s32)0x1) goto L_801359E8;
    goto L_80135A18;
L_801359D4: ;
    if ((s32)r0 == (s32)0x5) goto L_80135A0C;
    if ((s32)r0 >= (s32)0x5) goto L_80135A18;
    goto L_80135A00;
    r31 = *(u32*)(sp + 0xC);
    return;
L_801359E8: ;
    fn_80135AB8();
    r3 = r3 & 0xFF;
    r31 = *(u32*)(sp + 0xC);
    return;
L_801359F4: ;
    fn_80135AA0();
    r3 = r3 & 0xFF;
    r31 = *(u32*)(sp + 0xC);
    return;
L_80135A00: ;
    fn_80135A88();
    r3 = r3 & 0xFF;
    r31 = *(u32*)(sp + 0xC);
    return;
L_80135A0C: ;
    fn_80135A70();
    r3 = r3 & 0xFF;
    r31 = *(u32*)(sp + 0xC);
    return;
L_80135A18: ;
    r3 = 0x0;

    r31 = *(u32*)(sp + 0xC);
    return;
}

/* 0x80135A30 | 0x10 | nc_setter */
void fn_80135A30(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)((u8*)ptr + 0x3) = val;
}

/* 0x80135A40 | 0x10 | nc_setter */
void fn_80135A40(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)((u8*)ptr + 0x2) = val;
}

/* 0x80135A50 | 0x10 | nc_setter */
void fn_80135A50(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)((u8*)ptr + 0x1) = val;
}

/* 0x80135A60 | 0x10 | nc_setter */
void fn_80135A60(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)((u8*)ptr + 0x0) = val;
}

/* 0x80135A70 | 0x18 | nc_getter */
u8 fn_80135A70(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)((u8*)ptr + 0x3);
}

/* 0x80135A88 | 0x18 | nc_getter */
u8 fn_80135A88(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)((u8*)ptr + 0x2);
}

/* 0x80135AA0 | 0x18 | nc_getter */
u8 fn_80135AA0(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)((u8*)ptr + 0x1);
}

/* 0x80135AB8 | 0x18 | nc_getter */
u8 fn_80135AB8(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)((u8*)ptr + 0x0);
}

/* 0x80135AD0 | 0x1C */
void fn_80135AD0(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    if ((u32)r3 == (u32)0x0) return;
    if ((u32)r4 == (u32)0x0) return;
    r0 = *(u32*)((u8*)r4 + 0x0);
    *(u32*)((u8*)r3 + 0x0) = r0;
    return;
}

/* 0x80135AEC | 0x20 */
/* Copy the first u32 from src to dst, if both are non-NULL. */
void fn_80135AEC(u32* dst, u32* src) {
    if (dst == NULL || src == NULL) {
        return;
    }
    *dst = *src;
}

/* 0x80135B0C | 16 bytes | nc_bnelr */
u32 fn_80135B0C(void* ptr) {
    if (ptr != NULL) { return (u32)ptr; }
    return 0;
}

/* 0x80135B1C | 0x10 | nc_setter */
void fn_80135B1C(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)((u8*)ptr + 0x22) = val;
}

/* 0x80135B2C | 0x10 | nc_setter */
void fn_80135B2C(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)((u8*)ptr + 0x21) = val;
}

/* 0x80135B3C | 0x10 | nc_setter */
void fn_80135B3C(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)((u8*)ptr + 0x20) = val;
}

/* 0x80135B4C | 0x10 | nc_setter */
void fn_80135B4C(void* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0x1C) = val;
}

/* 0x80135B5C | 0x10 | nc_setter_f */
void fn_80135B5C(void* ptr, f32 val) {
    if (ptr == NULL) { return; }
    *(f32*)((u8*)ptr + 0x18) = val;
}

/* 0x80135B6C | 0x10 | nc_setter */
void fn_80135B6C(void* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0x14) = val;
}

/* 0x80135B7C | 0x10 | nc_setter */
void fn_80135B7C(void* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0x8) = val;
}

/* 0x80135B8C | 0x14 */
void fn_80135B8C(void) {
    u32 r3 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    if ((u32)r3 == (u32)0x0) return;
    *(u32*)((u8*)r3 + 0x4) = r6;
    *(u32*)((u8*)r3 + 0x0) = r5;
    return;
}

/* 0x80135BA0 | 0x10 | nc_setter */
void fn_80135BA0(void* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0xC) = val;
}

/* 0x80135BB0 | 24 bytes | beq_default_getter */
u8 fn_80135BB0(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)((u8*)ptr + 0x22);
}

/* 0x80135BC8 | 24 bytes | beq_default_getter */
u8 fn_80135BC8(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)((u8*)ptr + 0x21);
}

/* 0x80135BE0 | 24 bytes | beq_default_getter */
u8 fn_80135BE0(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)((u8*)ptr + 0x20);
}

/* 0x80135BF8 | 24 bytes | beq_default_getter */
u32 fn_80135BF8(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0x1C);
}

/* 0x80135C10 | 0x18 */
void fn_80135C10(void) {
    extern u8 lbl_8047D110[];
    u32 r0 = 0;
    u32 r3 = 0;
    f32 f1 = 0.0f;

    if ((u32)r3 != (u32)0x0) {
        f1 = *(f32*)((u8*)r3 + 0x18);
        return;
    }
    f1 = *(f32*)lbl_8047D110;
    return;
}

/* 0x80135C28 | 24 bytes | beq_default_getter */
u32 fn_80135C28(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0x14);
}

/* 0x80135C40 | 24 bytes | beq_default_getter */
u32 fn_80135C40(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0x8);
}

/* 0x80135C58 | 0x20 */
void fn_80135C58(void) {
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r0 = 0;

    /* mr. r4, r3 */;
    if ((s32)r0 != (s32)0) {
        r3 = *(u32*)((u8*)r4 + 0x0);
        r4 = *(u32*)((u8*)r4 + 0x4);
        return;
    }
    r4 = 0x0;
    r3 = 0x0;
    return;
}

/* 0x80135C78 | 24 bytes | beq_default_getter */
u32 fn_80135C78(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0xC);
}

/* 0x40 | fn_80135C90 | generic */
void fn_80135C90(u32 arg1, u32 arg2) {

}

/* 0x80135CD0 | 24 bytes | nc_addi_ptr */
void* fn_80135CD0(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x8;
}

/* 0x80135CE8 | 0x28 */
void fn_80135CE8(void) {
    u32 r0 = 0;
    u32 r3 = 0;

    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
    } else {

        r3 = r3 + 0x8;
    }
    if ((u32)r3 == (u32)0x0) return;
    r0 = 0x0;
    *(u32*)((u8*)r3 + 0x8) = r0;
    return;
}

/* 0x80135D10 | 0x134 */
void fn_80135D10(void) {
    extern void fn_8011BBD8();
    extern void fn_801254B4();
    extern void fn_8012A450();
    extern void fn_80135024();
    extern void fn_8013583C();
    extern void fn_80142B24();
    extern void fn_801F4C14();
    extern void fn_801F75F8();
    extern void fn_801FAA58();
    extern u8 jumptable_80363AC8[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r0 = r3 & 0xFF;
    r31 = 0x0;
    if ((u32)r0 > (u32)0x9) goto L_80135E2C;
    r3 = (u32)jumptable_80363AC8;
    r0 = r0 << 2;
    r3 = (u32)jumptable_80363AC8;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r3 = r4;
    r4 = r6;
    r5 = r8;
    fn_8013583C();
    goto L_80135E2C;
    r3 = r4;
    r4 = r6;
    r5 = r8;
    fn_80135024();
    goto L_80135E2C;
    r3 = r4;
    r4 = r6;
    r5 = r8;
    fn_8012A450();
    goto L_80135E2C;
    r3 = r4;
    r4 = r5;
    r5 = r6;
    r6 = r7;
    r7 = r8;
    fn_80142B24();
    goto L_80135E2C;
    r3 = r4;
    r4 = r5;
    r5 = r6;
    r6 = r7;
    r7 = r8;
    fn_801254B4();
    goto L_80135E2C;
    r3 = r4;
    r4 = r5;
    r5 = r6;
    r6 = r7;
    r7 = r8;
    fn_8011BBD8();
    goto L_80135E2C;
    r3 = r4;
    r4 = r5;
    r5 = r6;
    r6 = r7;
    r7 = r8;
    fn_801F4C14();
    r31 = r3;
    goto L_80135E2C;
    r3 = r4;
    r4 = r5;
    r5 = r6;
    r6 = r7;
    r7 = r8;
    fn_801F75F8();
    goto L_80135E2C;
    r3 = r4;
    r4 = r5;
    r5 = r6;
    r6 = r7;
    r7 = r8;
    fn_801FAA58();
L_80135E2C: ;
    r3 = r31;
    r31 = *(u32*)(sp + 0xC);
    return;
}

/* 0x80135E44 | 0x114 */
void fn_80135E44(void) {
    extern void fn_8011BEB4();
    extern void fn_8012640C();
    extern void fn_8012A5B0();
    extern void fn_80135028();
    extern void fn_80135938();
    extern void fn_80142CF4();
    extern void fn_801F54A4();
    extern void fn_801F76B8();
    extern void fn_801FB1C0();
    extern u8 jumptable_80363AF0[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    void (*ctr_fn)(void) = 0;

    r0 = r3 & 0xFF;
    if ((u32)r0 <= (u32)0x9) {
        r3 = (u32)jumptable_80363AF0;
        r0 = r0 << 2;
        r3 = (u32)jumptable_80363AF0;
        r0 = *(u32*)(r3 + r0);
        ctr_fn = (void(*)(void))r0;
        /* indirect jump via ctr */;
        r3 = 0x0;
        return;
        r3 = r4;
        r4 = r6;
        fn_80135938();
        return;
        r3 = r4;
        r4 = r6;
        r5 = r7;
        fn_80135028();
        return;
        r3 = r4;
        r4 = r6;
        r5 = r7;
        fn_8012A5B0();
        return;
        r3 = r4;
        r4 = r5;
        r5 = r6;
        r6 = r7;
        fn_80142CF4();
        return;
        r3 = r4;
        r4 = r5;
        r5 = r6;
        r6 = r7;
        fn_8012640C();
        return;
        r3 = r4;
        r4 = r5;
        r5 = r6;
        r6 = r7;
        fn_8011BEB4();
        return;
        r3 = r4;
        r4 = r5;
        r5 = r6;
        r6 = r7;
        fn_801F54A4();
        return;
        r3 = r4;
        r4 = r5;
        r5 = r6;
        r6 = r7;
        fn_801F76B8();
        return;
        r3 = r4;
        r4 = r5;
        r5 = r6;
        r6 = r7;
        fn_801FB1C0();
        return;
    }
    r3 = 0x0;

    return;
}

/* 0x80135F58 | 0x38 */
void fn_80135F58(void) {
    extern u8 lbl_80363B78[];
    extern u8 lbl_80478B90[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r0 = *(u32*)lbl_80478B90;
    if ((u32)r3 > (u32)r0) { r3 = 0x0; return; }
    if ((u32)r4 > (u32)0x8) {

        r3 = 0x0;
        return;
    }
    r5 = (u32)lbl_80363B78;
    r6 = r3 << 4;
    r3 = (u32)lbl_80363B78;
    r0 = r4 << 1;
    r3 = r3 + r6;
    r3 = *(u16*)(r3 + r0);
    return;
}

/* 0x80135F90 | 0x2C */
/* Get the u16 at offset 0x4 in an effect table entry (stride 0xA). */
u32 fn_80135F90(u32 index) {
    extern u8 lbl_80363B18[];
    extern u8 lbl_80478B88[];

    if (index > *(u32*)lbl_80478B88) {
        return 0;
    }
    return *(u16*)(lbl_80363B18 + index * 0xA + 0x4);
}

/* 0x80135FBC | 0x3C
 * Get a s16 value from the effect table at offset 0x6 + subIndex*2.
 * Returns 0 if index or subIndex is out of range.
 */
s32 fn_80135FBC(u32 index, u32 subIndex) {
    extern u8 lbl_80363B18[];
    extern u8 lbl_80478B88[];

    if (index > *(u32*)lbl_80478B88 || subIndex > 2) {
        return 0;
    }
    return *(s16*)(lbl_80363B18 + index * 0xA + subIndex * 2 + 0x6);
}

/* 0x80135FF8 | 0x2C
 * Get the u8 at offset 0x1 in an effect table entry (stride 0xA).
 */
u32 fn_80135FF8(u32 index) {
    extern u8 lbl_80363B18[];
    extern u8 lbl_80478B88[];

    if (index > *(u32*)lbl_80478B88) {
        return 0;
    }
    return *(u8*)(lbl_80363B18 + index * 0xA + 0x1);
}

/* 0x80136024 | 0x2C
 * Get the u16 at offset 0x2 in an effect table entry (stride 0xA).
 */
u32 fn_80136024(u32 index) {
    extern u8 lbl_80363B18[];
    extern u8 lbl_80478B88[];

    if (index > *(u32*)lbl_80478B88) {
        return 0;
    }
    return *(u16*)(lbl_80363B18 + index * 0xA + 0x2);
}

/* 0x80136050 | 0x28
 * Get the u8 at offset 0x0 in an effect table entry (stride 0xA).
 */
u32 fn_80136050(u32 index) {
    extern u8 lbl_80363B18[];
    extern u8 lbl_80478B88[];

    if (index > *(u32*)lbl_80478B88) {
        return 0;
    }
    return *(u8*)(lbl_80363B18 + index * 0xA);
}

/* 0x80136078 | 0xC4 */
void fn_80136078(void) {
    extern void fn_80135F58();
    extern void fn_80135F90();
    extern void fn_8013613C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* mr. r29, r6 */;
    r26 = r3;
    r27 = r4;
    r28 = r5;
    if ((s32)r0 != (s32)0) {
        fn_8013613C();
    } else {

        r6 = 0x0;
        fn_8013613C();
    }
    r3 = r26;
    fn_80135F90();
    r31 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) return;
    r30 = 0x0;
    goto L_8013611C;
L_801360C4: ;
    r3 = r31;
    r4 = r30 & 0xFFFF;
    fn_80135F58();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) goto L_80136118;
    if ((u32)r29 == (u32)0x0) goto L_80136104;
    r4 = r30 & 0xFFFF;
    r3 = r26;
    r0 = r4 + 0x1;
    r4 = r27;
    r0 = r0 << 2;
    r5 = r28;
    r6 = r29 + r0;
    fn_8013613C();
    goto L_80136118;
L_80136104: ;
    r3 = r26;
    r4 = r27;
    r5 = r28;
    r6 = 0x0;
    fn_8013613C();
L_80136118: ;
    r30 = r30 + 0x1;
L_8013611C: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 < (u32)0x8) goto L_801360C4;

    return;
}

/* 0x8013613C | 0x22C */
void fn_8013613C(void) {
    extern void fn_80135D10();
    extern void fn_80135E44();
    extern void fn_80135FBC();
    extern void fn_80135FF8();
    extern void fn_80136024();
    extern void fn_80136050();
    extern void fn_801F0134();
    extern void fn_801F54A4();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* mr. r23, r3 */;
    r26 = r4;
    r27 = r5;
    r28 = r6;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    fn_80136050();
    r0 = r3;
    r3 = r23;
    r29 = r0;
    fn_80136024();
    r0 = r3;
    r3 = r23;
    r30 = r0;
    r4 = 0x0;
    fn_80135FBC();
    r24 = r3;
    r3 = r23;
    r4 = 0x1;
    fn_80135FBC();
    r25 = r3;
    r3 = r23;
    fn_80135FF8();
    r23 = r3 & 0xFF;
    r3 = r29;
    r4 = r26;
    r6 = r30;
    r7 = r24 & 0xFFFF;
    r5 = 0x0;
    fn_80135E44();
    r31 = r3;
    if ((u32)r23 == (u32)0x0 || (u32)r23 == (u32)0x2) goto L_801361E4;

    if ((u32)r23 != (u32)0x3) goto L_8013625C;
L_801361E4: ;
    r0 = (s16)r24;
    if ((s32)r0 == (s32)-0x1) {
        r3 = r29;
        r4 = r26;
        r6 = r25 & 0xFFFF;
        r5 = 0x0;
        r7 = 0x0;
        fn_80135E44();
        r0 = (u32)r3 >> 31;
        r0 = r0 + r3;
        r0 = (s32)r0 >> 1;
        r24 = (s16)r0;

    } else if ((s32)r0 == (s32)-0x2) {
    if ((s32)r0 != (s32)-0x2) goto L_80136244;
        r3 = r29;
        r4 = r26;
        r6 = r25 & 0xFFFF;
        r5 = 0x0;
        r7 = 0x0;
        fn_80135E44();
        r24 = (s16)r3;

    } else {
        r0 = (s16)r24;
        if ((s32)r0 < (s32)-0x2) { r3 = 0x0; return; }
        r0 = (s16)r25;
        if ((s32)r0 >= (s32)-0x2) goto L_8013625C;

        r3 = 0x0;
        return;
    }
    if ((s32)r23 == (s32)0x3) goto L_801362B8;
    if ((s32)r23 >= (s32)0x3) goto L_80136280;
    if ((s32)r23 == (s32)0x1) goto L_80136298;
    if ((s32)r23 >= (s32)0x1) goto L_801362AC;
    if ((s32)r23 >= (s32)0x0) goto L_80136290;
    r3 = 0x0;
    return;
L_80136280: ;
    if ((s32)r23 == (s32)0x5) goto L_801362DC;
    if ((s32)r23 >= (s32)0x5) { r3 = 0x0; return; }
    goto L_801362C4;
L_80136290: ;
    r23 = (s16)r24;
    goto L_801362FC;
L_80136298: ;
    r3 = (s16)r24;
    r0 = (s16)r25;
    r3 = r31 * r3;
    r23 = (s32)r3 / (s32)r0;
    goto L_801362FC;
L_801362AC: ;
    r0 = (s16)r24;
    r23 = r31 + r0;
    goto L_801362FC;
L_801362B8: ;
    r0 = (s16)r24;
    r23 = r31 - r0;
    goto L_801362FC;
L_801362C4: ;
    r3 = (s16)r24;
    r0 = (s16)r25;
    r3 = r31 * r3;
    r0 = (s32)r3 / (s32)r0;
    r23 = r31 + r0;
    goto L_801362FC;
L_801362DC: ;
    r3 = (s16)r24;
    r0 = (s16)r25;
    r3 = r31 * r3;
    r0 = (s32)r3 / (s32)r0;
    r23 = r31 - r0;
    goto L_801362FC;

    r3 = 0x0;
    return;
L_801362FC: ;
    if ((u32)r27 != (u32)0x0) {
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x14;
        r6 = 0x0;
        fn_801F54A4();
        r4 = r3 & 0xFFFF;
        r3 = r27;
        fn_801F0134();
        r7 = r3;
    } else {

        r7 = 0x0;
    }
    r3 = r29;
    r4 = r26;
    r6 = r30;
    r8 = r23;
    r5 = 0x0;
    fn_80135D10();
    if ((u32)r28 == (u32)0x0) return;
    *(u32*)((u8*)r28 + 0x0) = r23;

    return;
}

/* 0x40 | fn_80136368 | index_lookup */
u32 fn_80136368(u16 idx) {
    void* entry;
    if (idx > lbl_80478B98) { return 0; }
    entry = (u8*)lbl_80363B88 + idx * 0x18;
    if (entry == NULL) { return 0; }
    return *(u32*)((u8*)entry + 0x8);
}

/* 0x40 | fn_801363A8 | index_lookup */
u8 fn_801363A8(u16 idx) {
    void* entry;
    if (idx > lbl_80478B98) { return 0; }
    entry = (u8*)lbl_80363B88 + idx * 0x18;
    if (entry == NULL) { return 0; }
    return *(u8*)((u8*)entry + 0x0);
}

/* 0x40 | fn_801363E8 | index_lookup */
u16 fn_801363E8(u16 idx) {
    void* entry;
    if (idx > lbl_80478BA0) { return 0; }
    entry = (u8*)lbl_80363C00 + idx * 0xC;
    if (entry == NULL) { return 0; }
    return *(u16*)((u8*)entry + 0x4);
}

/* 0x40 | fn_80136428 | index_lookup */
u8 fn_80136428(u16 idx) {
    void* entry;
    if (idx > lbl_80478BA0) { return 0; }
    entry = (u8*)lbl_80363C00 + idx * 0xC;
    if (entry == NULL) { return 0; }
    return *(u8*)((u8*)entry + 0x0);
}

/* 0x40 | fn_80136468 | index_lookup */
u16 fn_80136468(u16 idx) {
    void* entry;
    if (idx > lbl_80478BA0) { return 0; }
    entry = (u8*)lbl_80363C00 + idx * 0xC;
    if (entry == NULL) { return 0; }
    return *(u16*)((u8*)entry + 0x2);
}

/* 0x801364A8 | 0xC6C */
void fn_801364A8(void) {
    extern u8 lbl_80314638[];
    extern u8 lbl_80314AE8[];
    extern u8 lbl_8047D118[];
    extern u8 lbl_8047D11C[];
    extern u8 lbl_8047D120[];
    extern u8 lbl_8047D128[];
    extern void fn_800D37CC();
    extern void fn_800E01D0();
    extern void fn_800E27B0();
    extern void fn_800E2C04();
    extern void fn_800E4014();
    extern void fn_800EF590();
    extern void fn_800EFD14();
    extern void fn_800EFD3C();
    extern void fn_800F9318();
    extern void fn_801013A0();
    extern void fn_8010147C();
    extern void fn_80137114();
    extern void fn_8013735C();
    extern void fn_8013757C();
    extern void fn_80137780();
    extern void fn_8013D604();
    extern void fn_801DB060();
    extern u8 jumptable_80363C70[];
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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
    f32 f5 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;

    *(f64*)(sp + 0x60) = f31;
    *(f64*)(sp + 0x50) = f30;
    r29 = r4;
    r28 = r3;
    r30 = *(u32*)((u8*)r29 + 0x4);
    r27 = r29;
    r4 = 0x0;
    r5 = 0xd8;
    r29 = r29 + 0xc;
    memset((void*)r3, (int)r4, (u32)r5);
    r0 = *(u32*)((u8*)r27 + 0x0);
    *(u32*)((u8*)r28 + 0x0) = r0;
    fn_800D37CC();
    r4 = (0x4330 << 16);
    f3 = *(f64*)lbl_8047D128;
    f0 = *(f32*)lbl_8047D118;
    f1 = *(f64*)(sp + 0x10);
    *(u32*)(sp + 0x1C) = r0;
    f2 = f1 - f3;
    f1 = *(f64*)(sp + 0x18);
    f1 = f1 - f3;
    f1 = f1 * f2;
    f0 = f1 / f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x20) = f0;
    r0 = *(u32*)(sp + 0x24);
    *(u32*)((u8*)r28 + 0x4) = r0;
    r0 = *(u32*)((u8*)r27 + 0x0);
    if ((u32)r0 > (u32)0xc) goto L_801370EC;
    r3 = (u32)jumptable_80363C70;
    r0 = r0 << 2;
    r3 = (u32)jumptable_80363C70;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r4 = r29;
    r5 = r30;
    r3 = r28 + 0x8;
    fn_80137780();
    r29 = r3;
    goto L_801370EC;
    r3 = r28 + 0x8;
    r4 = 0x0;
    r5 = 0x60;
    memset((void*)r3, (int)r4, (u32)r5);
    fn_800D37CC();
    r5 = (0x4330 << 16);
    f3 = *(f64*)lbl_8047D128;
    r4 = r29;
    r3 = r28 + 0x10;
    f0 = *(f32*)lbl_8047D118;
    f1 = *(f64*)(sp + 0x20);
    *(u32*)(sp + 0x1C) = r0;
    f2 = f1 - f3;
    f1 = *(f64*)(sp + 0x18);
    f1 = f1 - f3;
    f1 = f1 * f2;
    f0 = f1 / f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x10) = f0;
    r0 = *(u32*)(sp + 0x14);
    *(u16*)((u8*)r28 + 0x52) = r0;
    r0 = *(u32*)((u8*)r29 + 0x3C);
    *(u16*)((u8*)r28 + 0x4C) = r0;
    r0 = *(u32*)((u8*)r29 + 0x40);
    *(u16*)((u8*)r28 + 0x4E) = r0;
    f0 = *(f32*)((u8*)r29 + 0x38);
    *(f32*)((u8*)r28 + 0x48) = f0;
    r0 = *(u32*)((u8*)r29 + 0x44);
    r0 = (u32)r0 >> 24;
    *(u8*)((u8*)r28 + 0x67) = r0;
    r0 = *(u32*)((u8*)r29 + 0x44);
    *(u8*)((u8*)r28 + 0x66) = r0;
    r0 = *(u32*)((u8*)r29 + 0x44);
    *(u8*)((u8*)r28 + 0x65) = r0;
    r0 = *(u32*)((u8*)r29 + 0x44);
    *(u8*)((u8*)r28 + 0x64) = r0;
    r0 = *(u32*)((u8*)r29 + 0x48);
    *(u32*)((u8*)r28 + 0x58) = r0;
    f0 = *(f32*)((u8*)r29 + 0x34);
    *(f32*)((u8*)r28 + 0x44) = f0;
    f0 = *(f32*)((u8*)r29 + 0x30);
    *(f32*)((u8*)r28 + 0x40) = f0;
    fn_800E01D0();
    r3 = r28 + 0x34;
    r4 = r29 + 0x24;
    fn_800E01D0();
    r3 = r28 + 0x1c;
    r4 = r29 + 0xc;
    fn_800E01D0();
    r3 = r28 + 0x28;
    r4 = r29 + 0x18;
    fn_800E01D0();
    r3 = *(u32*)((u8*)r29 + 0x4C);
    r0 = r29 + 0x73;
    /* clrrwi r27, r0, 5 */;
    r4 = 0x20;
    r0 = r3 + 0x1f;
    /* clrrwi r3, r0, 5 */;
    fn_800E2C04();
    r0 = r3 & 0xFFFF;
    r31 = r3;
    if ((u32)r0 != (u32)0xc) {
        fn_800E27B0();
        r5 = *(u32*)((u8*)r29 + 0x4C);
        r30 = r3;
        r4 = r27;
        r0 = r5 + 0x1f;
        /* clrrwi r5, r0, 5 */;
        memcpy((void*)r3, (const void*)r4, (u32)r5);
        r3 = r30;
        fn_800EFD3C();
        *(u32*)((u8*)r28 + 0x60) = r3;
        r4 = r31;
        r3 = *(u32*)((u8*)r28 + 0x60);
        fn_800EFD14();
    } else {

        r0 = 0x0;
        *(u32*)((u8*)r28 + 0x60) = r0;
    }
    r4 = *(u32*)((u8*)r29 + 0x4C);
    r3 = (u32)lbl_80314AE8;
    r0 = (u32)lbl_80314AE8;
    r3 = r4 + 0x1f;
    *(u32*)((u8*)r28 + 0x5C) = r0;
    /* clrrwi r0, r3, 5 */;
    r27 = r27 + r0;
    r29 = r27;
    goto L_801370EC;
    r3 = r28 + 0x8;
    r4 = 0x0;
    r5 = 0x24;
    memset((void*)r3, (int)r4, (u32)r5);
    f0 = *(f32*)((u8*)r29 + 0x4);
    r4 = (0x4330 << 16);
    r3 = (u32)lbl_80314638;
    r0 = (u32)lbl_80314638;
    f1 = *(f64*)lbl_8047D128;
    *(f32*)((u8*)r28 + 0x18) = f0;
    f0 = *(f32*)((u8*)r29 + 0x8);
    *(f32*)((u8*)r28 + 0x1C) = f0;
    r3 = *(u32*)((u8*)r29 + 0xC);
    f0 = *(f64*)(sp + 0x20);
    f0 = f0 - f1;
    *(f32*)((u8*)r28 + 0x20) = f0;
    r3 = *(u32*)((u8*)r29 + 0x10);
    f0 = *(f64*)(sp + 0x18);
    f0 = f0 - f1;
    *(f32*)((u8*)r28 + 0x24) = f0;
    f0 = *(f32*)((u8*)r29 + 0x14);
    *(f32*)((u8*)r28 + 0x28) = f0;
    r3 = *(u32*)((u8*)r29 + 0x0);
    r3 = (u32)r3 >> 24;
    *(u8*)((u8*)r28 + 0xB) = r3;
    r3 = *(u32*)((u8*)r29 + 0x0);
    *(u8*)((u8*)r28 + 0xA) = r3;
    r3 = *(u32*)((u8*)r29 + 0x0);
    *(u8*)((u8*)r28 + 0x9) = r3;
    r3 = *(u32*)((u8*)r29 + 0x0);
    *(u8*)((u8*)r28 + 0x8) = r3;
    *(u32*)((u8*)r28 + 0xC) = r0;
    fn_800D37CC();
    r4 = (0x4330 << 16);
    f3 = *(f64*)lbl_8047D128;
    r29 = r29 + 0x1c;
    f0 = *(f32*)lbl_8047D118;
    f1 = *(f64*)(sp + 0x10);
    *(u32*)(sp + 0x2C) = r0;
    f2 = f1 - f3;
    f1 = *(f64*)(sp + 0x28);
    f1 = f1 - f3;
    f1 = f1 * f2;
    f0 = f1 / f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x30) = f0;
    r0 = *(u32*)(sp + 0x34);
    *(u16*)((u8*)r28 + 0x12) = r0;
    goto L_801370EC;
    r4 = r29;
    r5 = r30;
    r3 = r28 + 0x8;
    fn_8013757C();
    r29 = r3;
    goto L_801370EC;
    r3 = r28 + 0x8;
    r4 = 0x0;
    r5 = 0x2c;
    memset((void*)r3, (int)r4, (u32)r5);
    fn_800D37CC();
    r5 = (0x4330 << 16);
    *(u32*)(sp + 0x34) = r0;
    r0 = r29 + 0x37;
    f3 = *(f64*)lbl_8047D128;
    /* clrrwi r31, r0, 5 */;
    f0 = *(f32*)lbl_8047D118;
    r4 = 0x20;
    f1 = *(f64*)(sp + 0x30);
    f2 = f1 - f3;
    f1 = *(f64*)(sp + 0x28);
    f1 = f1 - f3;
    f1 = f1 * f2;
    f0 = f1 / f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x20) = f0;
    r0 = *(u32*)(sp + 0x24);
    *(u16*)((u8*)r28 + 0x28) = r0;
    r0 = *(u32*)((u8*)r29 + 0x0);
    r0 = (u32)r0 >> 24;
    *(u8*)((u8*)r28 + 0x23) = r0;
    r0 = *(u32*)((u8*)r29 + 0x0);
    *(u8*)((u8*)r28 + 0x22) = r0;
    r0 = *(u32*)((u8*)r29 + 0x0);
    *(u8*)((u8*)r28 + 0x21) = r0;
    r0 = *(u32*)((u8*)r29 + 0x0);
    *(u8*)((u8*)r28 + 0x20) = r0;
    r0 = *(u32*)((u8*)r29 + 0x4);
    *(u16*)((u8*)r28 + 0x2A) = r0;
    r0 = *(u32*)((u8*)r29 + 0x8);
    *(u16*)((u8*)r28 + 0x30) = r0;
    r0 = *(u32*)((u8*)r29 + 0xC);
    *(u16*)((u8*)r28 + 0x32) = r0;
    r3 = *(u32*)((u8*)r29 + 0x10);
    r0 = r3 + 0x1f;
    /* clrrwi r3, r0, 5 */;
    fn_800E2C04();
    r0 = r3 & 0xFFFF;
    r30 = r3;
    if ((u32)r0 != (u32)0xc) {
        fn_800E27B0();
        r5 = *(u32*)((u8*)r29 + 0x10);
        r27 = r3;
        r4 = r31;
        r0 = r5 + 0x1f;
        /* clrrwi r5, r0, 5 */;
        memcpy((void*)r3, (const void*)r4, (u32)r5);
        r3 = r27;
        fn_800EFD3C();
        *(u32*)((u8*)r28 + 0x1C) = r3;
        r4 = r30;
        r3 = *(u32*)((u8*)r28 + 0x1C);
        fn_800EFD14();
    } else {

        r0 = 0x0;
        *(u32*)((u8*)r28 + 0x1C) = r0;
    }
    r4 = *(u32*)((u8*)r29 + 0x10);
    r3 = *(u32*)((u8*)r28 + 0x1C);
    r0 = r4 + 0x1f;
    /* clrrwi r0, r0, 5 */;
    r31 = r31 + r0;
    if ((u32)r3 != (u32)0x0) {
        r4 = 0x0;
        r5 = 0x0;
        fn_800EF590();
    }
    r3 = (u32)lbl_80314AE8;
    r29 = r31;
    r0 = (u32)lbl_80314AE8;
    *(u32*)((u8*)r28 + 0x18) = r0;
    goto L_801370EC;
    r3 = r28 + 0x8;
    r4 = 0x0;
    r5 = 0x4c;
    memset((void*)r3, (int)r4, (u32)r5);
    fn_800D37CC();
    r5 = (0x4330 << 16);
    *(u32*)(sp + 0x34) = r0;
    r3 = r29 + 0x5b;
    f3 = *(f64*)lbl_8047D128;
    /* clrrwi r27, r3, 5 */;
    f0 = *(f32*)lbl_8047D118;
    r0 = 0x4e20;
    f1 = *(f64*)(sp + 0x30);
    f2 = f1 - f3;
    f1 = *(f64*)(sp + 0x28);
    f1 = f1 - f3;
    f1 = f1 * f2;
    f0 = f1 / f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x20) = f0;
    r3 = *(u32*)(sp + 0x24);
    *(u16*)((u8*)r28 + 0x46) = r3;
    f0 = *(f32*)((u8*)r29 + 0x0);
    *(f32*)((u8*)r28 + 0x18) = f0;
    f0 = *(f32*)((u8*)r29 + 0x4);
    *(f32*)((u8*)r28 + 0x1C) = f0;
    f0 = *(f32*)((u8*)r29 + 0x8);
    *(f32*)((u8*)r28 + 0x20) = f0;
    f0 = *(f32*)((u8*)r29 + 0xC);
    *(f32*)((u8*)r28 + 0x24) = f0;
    f0 = *(f32*)((u8*)r29 + 0x10);
    *(f32*)((u8*)r28 + 0x28) = f0;
    f0 = *(f32*)((u8*)r29 + 0x14);
    *(f32*)((u8*)r28 + 0x2C) = f0;
    f0 = *(f32*)((u8*)r29 + 0x18);
    *(f32*)((u8*)r28 + 0x30) = f0;
    f0 = *(f32*)((u8*)r29 + 0x1C);
    *(f32*)((u8*)r28 + 0x34) = f0;
    f0 = *(f32*)((u8*)r29 + 0x20);
    *(f32*)((u8*)r28 + 0x38) = f0;
    f0 = *(f32*)((u8*)r29 + 0x24);
    *(f32*)((u8*)r28 + 0x3C) = f0;
    r3 = *(u32*)((u8*)r29 + 0x28);
    *(u16*)((u8*)r28 + 0x40) = r3;
    r3 = *(u32*)((u8*)r29 + 0x2C);
    *(u16*)((u8*)r28 + 0x42) = r3;
    r3 = *(u32*)((u8*)r29 + 0x30);
    *(u16*)((u8*)r28 + 0x52) = r3;
    *(u16*)((u8*)r28 + 0x48) = r0;
    fn_801DB060();
    *(u16*)((u8*)r28 + 0x4A) = r3;
    fn_801DB060();
    *(u16*)((u8*)r28 + 0x4C) = r3;
    r3 = r27;
    r5 = 0x4e20;
    r4 = *(u32*)((u8*)r29 + 0x34);
    r6 = *(u16*)((u8*)r28 + 0x4A);
    fn_8010147C();
    r4 = *(u16*)((u8*)r28 + 0x4A);
    r3 = 0x4e20;
    fn_800F9318();
    r6 = *(u16*)((u8*)r28 + 0x4C);
    r4 = 0x4e20;
    r5 = 0x0;
    fn_801013A0();
    r4 = *(u16*)((u8*)r28 + 0x4C);
    r3 = 0x4e20;
    fn_800F9318();
    if ((u32)r3 != (u32)0x0) {
        r4 = 0x0;
        fn_800E4014();
    }
    r3 = *(u32*)((u8*)r29 + 0x34);
    r0 = r3 + 0x1f;
    /* clrrwi r0, r0, 5 */;
    r0 = r27 + r0;
    r29 = r0;
    goto L_801370EC;
    r4 = r29;
    r5 = r30;
    r3 = r28 + 0x8;
    fn_8013735C();
    r29 = r3;
    goto L_801370EC;
    r3 = r28 + 0x8;
    r4 = 0x0;
    r5 = 0xd0;
    memset((void*)r3, (int)r4, (u32)r5);
    fn_800D37CC();
    r5 = (0x4330 << 16);
    *(u32*)(sp + 0x34) = r0;
    r3 = r29 + 0x53;
    f3 = *(f64*)lbl_8047D128;
    /* clrrwi r31, r3, 5 */;
    f0 = *(f32*)lbl_8047D118;
    r0 = 0x4e20;
    f1 = *(f64*)(sp + 0x30);
    f2 = f1 - f3;
    f1 = *(f64*)(sp + 0x28);
    f1 = f1 - f3;
    f1 = f1 * f2;
    f0 = f1 / f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x20) = f0;
    r3 = *(u32*)(sp + 0x24);
    *(u16*)((u8*)r28 + 0xD6) = r3;
    f0 = *(f32*)((u8*)r29 + 0x0);
    *(f32*)((u8*)r28 + 0x2C) = f0;
    f0 = *(f32*)((u8*)r29 + 0x4);
    *(f32*)((u8*)r28 + 0x30) = f0;
    f0 = *(f32*)((u8*)r29 + 0x8);
    *(f32*)((u8*)r28 + 0x34) = f0;
    r3 = *(u32*)((u8*)r29 + 0xC);
    r3 = (s32)r3 >> 24;
    *(u8*)((u8*)r28 + 0x47) = r3;
    r3 = *(u32*)((u8*)r29 + 0xC);
    *(u8*)((u8*)r28 + 0x46) = r3;
    r3 = *(u32*)((u8*)r29 + 0xC);
    *(u8*)((u8*)r28 + 0x45) = r3;
    r3 = *(u32*)((u8*)r29 + 0xC);
    *(u8*)((u8*)r28 + 0x44) = r3;
    f0 = *(f32*)((u8*)r29 + 0x10);
    *(f32*)((u8*)r28 + 0x48) = f0;
    f0 = *(f32*)((u8*)r29 + 0x14);
    *(f32*)((u8*)r28 + 0x4C) = f0;
    f0 = *(f32*)((u8*)r29 + 0x18);
    *(f32*)((u8*)r28 + 0x50) = f0;
    r3 = *(u32*)((u8*)r29 + 0x1C);
    *(u16*)((u8*)r28 + 0x54) = r3;
    r3 = *(u32*)((u8*)r29 + 0x20);
    *(u16*)((u8*)r28 + 0x56) = r3;
    f0 = *(f32*)((u8*)r29 + 0x24);
    *(f32*)((u8*)r28 + 0xCC) = f0;
    f0 = *(f32*)((u8*)r29 + 0x28);
    *(f32*)((u8*)r28 + 0xD0) = f0;
    *(u32*)((u8*)r28 + 0x58) = r0;
    fn_801DB060();
    *(u32*)((u8*)r28 + 0x60) = r3;
    fn_801DB060();
    *(u32*)((u8*)r28 + 0x5C) = r3;
    r3 = r31;
    r5 = 0x4e20;
    r4 = *(u32*)((u8*)r29 + 0x2C);
    r6 = *(u32*)((u8*)r28 + 0x60);
    fn_8010147C();
    r4 = *(u32*)((u8*)r28 + 0x60);
    r3 = 0x4e20;
    fn_800F9318();
    r6 = *(u32*)((u8*)r28 + 0x5C);
    r4 = 0x4e20;
    r5 = 0x0;
    fn_801013A0();
    r4 = *(u32*)((u8*)r28 + 0x5C);
    r3 = 0x4e20;
    fn_800F9318();
    if ((u32)r3 != (u32)0x0) {
        r4 = 0x0;
        fn_800E4014();
    }
    r3 = *(u32*)((u8*)r29 + 0x2C);
    r0 = r3 + 0x1f;
    /* clrrwi r0, r0, 5 */;
    r0 = r31 + r0;
    r29 = r0;
    goto L_801370EC;
    r27 = *(u32*)((u8*)r29 + 0x8);
    r3 = r28 + 0x8;
    r4 = 0x0;
    r5 = 0x18;
    memset((void*)r3, (int)r4, (u32)r5);
    r0 = *(u32*)((u8*)r29 + 0x0);
    *(u32*)((u8*)r28 + 0xC) = r0;
    r0 = *(u32*)((u8*)r29 + 0x4);
    *(u32*)((u8*)r28 + 0x10) = r0;
    r0 = *(u32*)((u8*)r29 + 0xC);
    if ((s32)r0 == (s32)0x2) goto L_80136C50;
    if ((s32)r0 >= (s32)0x2) goto L_80136D78;
    if ((s32)r0 >= (s32)0x1) goto L_80136CD0;
    goto L_80136D78;
L_80136C50: ;
    f30 = *(f64*)lbl_8047D128;
    r31 = r29 + 0x10;
    f31 = *(f32*)lbl_8047D118;
    r30 = 0x0;
    r29 = (0x4330 << 16);
    while ((s32)r30 < (s32)r27) {

        fn_800D37CC();
        r0 = *(u32*)((u8*)r31 + 0x8);
        r3 = r28 + 0x8;
        f1 = *(f32*)((u8*)r31 + 0x0);
        f2 = *(f32*)((u8*)r31 + 0x4);
        f0 = *(f64*)(sp + 0x30);
        *(u32*)(sp + 0x2C) = r0;
        f3 = f0 - f30;
        f0 = *(f64*)(sp + 0x28);
        f0 = f0 - f30;
        f0 = f0 * f3;
        f0 = f0 / f31;
        f0 = (f64)(s32)f0;
        *(f64*)(sp + 0x20) = f0;
        r4 = *(u32*)(sp + 0x24);
        fn_8013D604();
        r30 = r30 + 0x1;
        r31 = r31 + 0x10;

    }
    goto L_80136D78;
L_80136CD0: ;
    r0 = *(u32*)((u8*)r29 + 0x8);
    *(u32*)(sp + 0x8) = r0;
    f30 = *(f32*)(sp + 0x8);
    fn_800D37CC();
    r5 = (0x4330 << 16);
    *(u32*)(sp + 0x34) = r0;
    r3 = (0x5555 << 16);
    f5 = *(f64*)lbl_8047D128;
    r0 = r3 + 0x5556;
    f0 = *(f32*)lbl_8047D118;
    f2 = f30;
    f1 = *(f64*)(sp + 0x30);
    r3 = r28 + 0x8;
    f4 = f1 - f5;
    f1 = *(f32*)lbl_8047D11C;
    f3 = *(f64*)(sp + 0x28);
    f3 = f3 - f5;
    f3 = f3 * f4;
    f0 = f3 / f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x20) = f0;
    r4 = *(u32*)(sp + 0x24);
    r4 = (s32)((s64)r0 * (s64)r4 >> 32);
    r0 = (u32)r4 >> 31;
    r27 = r4 + r0;
    r4 = r27;
    fn_8013D604();
    f1 = f30;
    r4 = r27;
    f2 = f30;
    r3 = r28 + 0x8;
    fn_8013D604();
    f1 = *(f32*)lbl_8047D11C;
    r4 = r27;
    r3 = r28 + 0x8;
    f2 = f1;
    fn_8013D604();
L_80136D78: ;
    r29 = r31;
    goto L_801370EC;
    r4 = r29;
    r5 = r30;
    r3 = r28 + 0x8;
    fn_80137114();
    r29 = r3;
    goto L_801370EC;
    r3 = r28 + 0x8;
    r4 = 0x0;
    r5 = 0x34;
    memset((void*)r3, (int)r4, (u32)r5);
    fn_800D37CC();
    r4 = (0x4330 << 16);
    *(u32*)(sp + 0x34) = r0;
    f3 = *(f64*)lbl_8047D128;
    r0 = 0x0;
    f0 = *(f32*)lbl_8047D118;
    f1 = *(f64*)(sp + 0x30);
    f2 = f1 - f3;
    f1 = *(f64*)(sp + 0x28);
    f1 = f1 - f3;
    f1 = f1 * f2;
    f0 = f1 / f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x20) = f0;
    r3 = *(u32*)(sp + 0x24);
    *(u16*)((u8*)r28 + 0x3A) = r3;
    f0 = *(f32*)((u8*)r29 + 0x18);
    *(f32*)((u8*)r28 + 0x24) = f0;
    f0 = *(f32*)((u8*)r29 + 0x1C);
    *(f32*)((u8*)r28 + 0x28) = f0;
    f0 = *(f32*)((u8*)r29 + 0x20);
    *(f32*)((u8*)r28 + 0x2C) = f0;
    r3 = *(u32*)((u8*)r29 + 0x4);
    *(u8*)((u8*)r28 + 0x20) = r3;
    *(u8*)((u8*)r28 + 0x21) = r0;
    r0 = *(u32*)((u8*)r29 + 0xC);
    if ((s32)r0 != (s32)0x0) {
        r0 = *(u8*)((u8*)r28 + 0x21);
        r0 = r0 | 0x2;
        *(u8*)((u8*)r28 + 0x21) = r0;
    }
    r0 = *(u32*)((u8*)r29 + 0x14);
    if ((s32)r0 != (s32)0x0) {
        r0 = *(u8*)((u8*)r28 + 0x21);
        r0 = r0 | 0x8;
        *(u8*)((u8*)r28 + 0x21) = r0;
    }
    r0 = *(u32*)((u8*)r29 + 0x8);
    if ((s32)r0 != (s32)0x0) {
        r0 = *(u8*)((u8*)r28 + 0x21);
        r0 = r0 | 0x4;
        *(u8*)((u8*)r28 + 0x21) = r0;
    }
    r0 = *(u32*)((u8*)r29 + 0x10);
    if ((s32)r0 != (s32)0x0) {
        r0 = *(u8*)((u8*)r28 + 0x21);
        r0 = r0 | 0x1;
        *(u8*)((u8*)r28 + 0x21) = r0;
    }
    r0 = *(u32*)((u8*)r29 + 0x0);
    r0 = (s32)r0 >> 24;
    *(u8*)((u8*)r28 + 0x13) = r0;
    r0 = *(u32*)((u8*)r29 + 0x0);
    *(u8*)((u8*)r28 + 0x12) = r0;
    r0 = *(u32*)((u8*)r29 + 0x0);
    *(u8*)((u8*)r28 + 0x11) = r0;
    r0 = *(u32*)((u8*)r29 + 0x0);
    r29 = r29 + 0x28;
    *(u8*)((u8*)r28 + 0x10) = r0;
    goto L_801370EC;
    r3 = r28 + 0x8;
    r4 = 0x0;
    r5 = 0x24;
    memset((void*)r3, (int)r4, (u32)r5);
    r0 = *(u32*)((u8*)r29 + 0x8);
    *(u16*)((u8*)r28 + 0x24) = r0;
    r0 = *(u32*)((u8*)r29 + 0x10);
    if ((s32)r0 == (s32)0x1) goto L_80136EF4;
    if ((s32)r0 >= (s32)0x1) goto L_80136EFC;
    if ((s32)r0 >= (s32)0x0) goto L_80136EE8;
    goto L_80136EFC;
L_80136EE8: ;
    r0 = 0x0;
    *(u32*)((u8*)r28 + 0x20) = r0;
    goto L_80136EFC;
L_80136EF4: ;
    r0 = 0x1;
    *(u32*)((u8*)r28 + 0x20) = r0;
L_80136EFC: ;
    r4 = *(u32*)((u8*)r29 + 0xC);
    r3 = r29 + 0x37;
    r0 = 0x4e20;
    *(u16*)((u8*)r28 + 0x26) = r4;
    /* clrrwi r27, r3, 5 */;
    *(u32*)((u8*)r28 + 0x8) = r0;
    r0 = *(u32*)((u8*)r29 + 0x0);
    if ((s32)r0 != (s32)0x0) {
        fn_801DB060();
        *(u32*)((u8*)r28 + 0xC) = r3;
        fn_801DB060();
        *(u32*)((u8*)r28 + 0x10) = r3;
        r3 = r27;
        r5 = 0x4e20;
        r4 = *(u32*)((u8*)r29 + 0x0);
        r6 = *(u32*)((u8*)r28 + 0xC);
        fn_8010147C();
        r4 = *(u32*)((u8*)r28 + 0xC);
        r3 = 0x4e20;
        fn_800F9318();
        r6 = *(u32*)((u8*)r28 + 0x10);
        r4 = 0x4e20;
        r5 = 0x0;
        fn_801013A0();
        r4 = *(u32*)((u8*)r28 + 0x10);
        r3 = 0x4e20;
        fn_800F9318();
        if ((u32)r3 != (u32)0x0) {
            r4 = 0x0;
            fn_800E4014();
        }
        r3 = *(u32*)((u8*)r29 + 0x0);
        r0 = r3 + 0x1f;
        /* clrrwi r0, r0, 5 */;
        r27 = r27 + r0;
    } else {

        r0 = 0x0;
        *(u32*)((u8*)r28 + 0xC) = r0;
        *(u32*)((u8*)r28 + 0x10) = r0;
    }
    r0 = 0x0;
    r29 = r27;
    *(u32*)((u8*)r28 + 0x14) = r0;
    goto L_801370EC;
    r3 = r28 + 0x8;
    r27 = 0x0;
    r4 = 0x0;
    r5 = 0xb4;
    memset((void*)r3, (int)r4, (u32)r5);
    r0 = *(u32*)((u8*)r29 + 0x14);
    if ((s32)r0 == (s32)0x2) goto L_80136FEC;
    if ((s32)r0 >= (s32)0x2) goto L_80136FF4;
    if ((s32)r0 >= (s32)0x1) goto L_80136FDC;
    goto L_80136FF4;
L_80136FDC: ;
    f0 = *(f32*)lbl_8047D120;
    r27 = -0x4;
    *(f32*)((u8*)r28 + 0x24) = f0;
    goto L_80136FF4;
L_80136FEC: ;
    f0 = *(f32*)((u8*)r29 + 0x18);
    *(f32*)((u8*)r28 + 0x24) = f0;
L_80136FF4: ;
    fn_800D37CC();
    r5 = (0x4330 << 16);
    *(u32*)(sp + 0x34) = r0;
    r4 = r27 + r29;
    f3 = *(f64*)lbl_8047D128;
    r0 = r4 + 0x3b;
    f0 = *(f32*)lbl_8047D118;
    /* clrrwi r31, r0, 5 */;
    f1 = *(f64*)(sp + 0x30);
    r4 = 0x20;
    f2 = f1 - f3;
    f1 = *(f64*)(sp + 0x28);
    f1 = f1 - f3;
    f1 = f1 * f2;
    f0 = f1 / f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x20) = f0;
    r0 = *(u32*)(sp + 0x24);
    *(u16*)((u8*)r28 + 0xBA) = r0;
    f0 = *(f32*)((u8*)r29 + 0x8);
    *(f32*)((u8*)r28 + 0x20) = f0;
    f0 = *(f32*)((u8*)r28 + 0x20);
    *(f32*)((u8*)r28 + 0x1C) = f0;
    f0 = *(f32*)((u8*)r29 + 0xC);
    *(f32*)((u8*)r28 + 0x2C) = f0;
    r0 = *(u32*)((u8*)r29 + 0x0);
    *(u16*)((u8*)r28 + 0x30) = r0;
    r0 = *(u32*)((u8*)r29 + 0x4);
    *(u16*)((u8*)r28 + 0x32) = r0;
    r3 = *(u32*)((u8*)r29 + 0x10);
    r0 = r3 + 0x1f;
    /* clrrwi r3, r0, 5 */;
    fn_800E2C04();
    r0 = r3 & 0xFFFF;
    r30 = r3;
    if ((s32)r0 != (s32)0x1) {
        fn_800E27B0();
        r5 = *(u32*)((u8*)r29 + 0x10);
        r27 = r3;
        r4 = r31;
        r0 = r5 + 0x1f;
        /* clrrwi r5, r0, 5 */;
        memcpy((void*)r3, (const void*)r4, (u32)r5);
        r3 = r27;
        fn_800EFD3C();
        *(u32*)((u8*)r28 + 0xC) = r3;
        r4 = r30;
        r3 = *(u32*)((u8*)r28 + 0xC);
        fn_800EFD14();
    } else {

        r0 = 0x0;
        *(u32*)((u8*)r28 + 0xC) = r0;
    }
    r3 = *(u32*)((u8*)r29 + 0x10);
    r0 = r3 + 0x1f;
    /* clrrwi r0, r0, 5 */;
    r31 = r31 + r0;
    r29 = r31;
L_801370EC: ;
    r3 = r29;
    f31 = *(f64*)(sp + 0x60);
    f30 = *(f64*)(sp + 0x50);
    return;
}
