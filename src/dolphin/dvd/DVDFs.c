#include "dolphin/dvd/dvd.h"

/*
 * DVDFs.c - DVD filesystem initialization.
 *
 * Reads the FST (File System Table) pointer from the disc header
 * and sets up the string table pointer and entry count.
 *
 * Matches: 0x800A4CF0 | size: 0x38
 */

/* Boot info / disc header at 0x80000000 */
#define BOOT_INFO       ((u32*)0x80000000)

/* SDA-relative globals */
static u32* BootInfo;           /* 0x8047A7C8 */
static u32* FstStart;           /* 0x8047A7CC */
static u32* FstStringStart;     /* 0x8047A7D0 */
static u32  MaxEntryNum;        /* 0x8047A7D4 */

/* SDA symbol aliases used by stub functions */
extern u32 FstStart_8047A7CC;
extern u32 FstStringStart_8047A7D0;
extern u32 __DVDLongFileNameFlag;

/*
 * __DVDFSInit - Initialize the DVD filesystem
 * 0x800A4CF0 | size: 0x38
 *
 * Reads the FST location from offset 0x38 in the boot info,
 * then extracts the root entry count and string table offset.
 *
 * FST entry format (12 bytes each):
 *   word 0: flags/name_offset
 *   word 1: file_offset or parent_dir
 *   word 2: file_length or num_entries (for root: total entries)
 */
void __DVDFSInit(void) {
    BootInfo = BOOT_INFO;

    /* FST start address is at offset 0x38 in boot info */
    FstStart = (u32*)BootInfo[0x38 / 4];

    if (FstStart == NULL) {
        return;
    }

    /* Root entry's third word contains the total number of entries */
    MaxEntryNum = FstStart[2];

    /* String table immediately follows the FST entries */
    FstStringStart = (u32*)((u8*)FstStart + MaxEntryNum * 12);
}

/* ========================================================== */
/* Stub functions for coverage - TODO: decompile              */
/* ========================================================== */

/* fn_800A4D28 - 0x800A4D28 | size: 0x2F4 */
void fn_800A4D28(void) {
    extern u8 lbl_803118F0[];
    extern u8 lbl_804789C0[];
    extern u8 lbl_8047A7D8[];
    extern void fn_800060F0();
    extern void fn_800C7558();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r23 = r3;
    r3 = (u32)lbl_803118F0;
    r25 = r23 + 0x0;
    r31 = (u32)lbl_803118F0;
    r26 = *(u32*)lbl_8047A7D8;
L_800A4D4C:
    r3 = *(u8*)((u8*)r23 + 0x0);
    tmp = (s8)r3;
    if ((s32)tmp != 0) goto L_800A4D60;
    r3 = r26;
    goto L_800A5008;
L_800A4D60:
    tmp = (s8)r3;
    if ((s32)tmp != 0x2f) goto L_800A4D78;
    r26 = 0x0;
    r23 = r23 + 0x1;
    goto L_800A4D4C;
L_800A4D78:
    if ((s32)tmp != 0x2e) goto L_800A4DF0;
    r3 = *(u8*)((u8*)r23 + 0x1);
    tmp = (s8)r3;
    if ((s32)tmp != 0x2e) goto L_800A4DD0;
    r3 = *(u8*)((u8*)r23 + 0x2);
    if ((s32)r3 != 0x2f) goto L_800A4DB4;
    r3 = r26 * 0xc;
    r4 = *(u32*)FstStart_8047A7CC;
    tmp = r3 + 0x4;
    r26 = *(u32*)(r4 + tmp);
    r23 = r23 + 0x3;
    goto L_800A4D4C;
L_800A4DB4:
    tmp = (s8)r3;
    if ((s32)r3 != 0x2f) goto L_800A4DF0;
    tmp = r26 * 0xc;
    r3 = *(u32*)FstStart_8047A7CC;
    r3 = r3 + tmp;
    r3 = *(u32*)((u8*)r3 + 0x4);
    goto L_800A5008;
L_800A4DD0:
    if ((s32)tmp != 0x2f) goto L_800A4DE0;
    r23 = r23 + 0x2;
    goto L_800A4D4C;
L_800A4DE0:
    tmp = (s8)r3;
    if ((s32)tmp != 0x2f) goto L_800A4DF0;
    r3 = r26;
    goto L_800A5008;
L_800A4DF0:
    tmp = *(u32*)__DVDLongFileNameFlag;
    if (tmp != 0) goto L_800A4EA4;
    r28 = r23 + 0x0;
    r5 = 0x0;
    r4 = 0x0;
    goto L_800A4E50;
L_800A4E0C:
    tmp = (s8)r3;
    if ((s32)tmp != 0x2e) goto L_800A4E40;
    tmp = r28 - r23;
    if ((s32)tmp > 8) goto L_800A4E2C;
    if ((s32)r5 != 1) goto L_800A4E34;
L_800A4E2C:
    r4 = 0x1;
    goto L_800A4E68;
L_800A4E34:
    r24 = r28 + 0x1;
    r5 = 0x1;
    goto L_800A4E4C;
L_800A4E40:
    if ((s32)tmp != 0x20) goto L_800A4E4C;
    r4 = 0x1;
L_800A4E4C:
    r28 = r28 + 0x1;
L_800A4E50:
    r3 = *(u8*)((u8*)r28 + 0x0);
    tmp = (s8)r3;
    if ((s32)tmp == 0x20) goto L_800A4E68;
    tmp = (s8)r3;
    if ((s32)tmp != 0x2f) goto L_800A4E0C;
L_800A4E68:
    if ((s32)r5 != 1) goto L_800A4E80;
    tmp = r28 - r24;
    if ((s32)tmp <= 3) goto L_800A4E80;
    r4 = 0x1;
L_800A4E80:
    if ((s32)r4 == 0) goto L_800A4EC8;
    r5 = r31 + 0x0;
    r6 = r25 + 0x0;
    r3 = (u32)lbl_804789C0;
    r4 = 0x17b;
    fn_800060F0();
    goto L_800A4EC8;
L_800A4EA4:
    r28 = r23;
    goto L_800A4EB0;
L_800A4EAC:
    r28 = r28 + 0x1;
L_800A4EB0:
    r3 = *(u8*)((u8*)r28 + 0x0);
    tmp = (s8)r3;
    if ((s32)r4 == 0) goto L_800A4EC8;
    tmp = (s8)r3;
    if ((s32)tmp != 0x2f) goto L_800A4EAC;
L_800A4EC8:
    tmp = *(u8*)((u8*)r28 + 0x0);
    tmp = (s8)tmp;
    if ((s32)tmp != 0x2f) goto L_800A4EDC;
    r30 = 0x0;
    goto L_800A4EE0;
L_800A4EDC:
    r30 = 0x1;
L_800A4EE0:
    r29 = r26 * 0xc;
    r27 = r28 - r23;
    r26 = r26 + 0x1;
    goto L_800A4FD0;
L_800A4EF0:
    r28 = r26 * 0xc;
    r4 = *(u32*)(r3 + r28);
    /* clrrwi. tmp, r4, 24 */;
    if ((s32)tmp != 0x2f) goto L_800A4F08;
    tmp = 0x0;
    goto L_800A4F0C;
L_800A4F08:
    tmp = 0x1;
L_800A4F0C:
    if ((s32)tmp != 0) goto L_800A4F1C;
    if ((s32)r30 == 1) goto L_800A4F98;
L_800A4F1C:
    r3 = *(u32*)FstStringStart_8047A7D0;
    tmp = r4 & 0xFFFFFF;
    r21 = r23 + 0x0;
    r20 = r3 + tmp;
    goto L_800A4F64;
L_800A4F30:
    tmp = *(u8*)((u8*)r20 + 0x0);
    r20 = r20 + 0x1;
    r3 = (s8)tmp;
    fn_800C7558();
    tmp = *(u8*)((u8*)r21 + 0x0);
    r22 = r3 + 0x0;
    r21 = r21 + 0x1;
    r3 = (s8)tmp;
    fn_800C7558();
    if ((s32)r3 == (s32)r22) goto L_800A4F64;
    tmp = 0x0;
    goto L_800A4F90;
L_800A4F64:
    tmp = *(u8*)((u8*)r20 + 0x0);
    tmp = (s8)tmp;
    if ((s32)r3 != (s32)r22) goto L_800A4F30;
    r3 = *(u8*)((u8*)r21 + 0x0);
    if ((s32)r3 == 0x2f) goto L_800A4F84;
    tmp = (s8)r3;
    if ((s32)r3 != 0x2f) goto L_800A4F8C;
L_800A4F84:
    tmp = 0x1;
    goto L_800A4F90;
L_800A4F8C:
    tmp = 0x0;
L_800A4F90:
    if ((s32)tmp == 1) goto L_800A4FEC;
L_800A4F98:
    tmp = *(u32*)FstStart_8047A7CC;
    r3 = tmp + r28;
    tmp = *(u32*)((u8*)r3 + 0x0);
    /* clrrwi. tmp, tmp, 24 */;
    if ((s32)tmp != 1) goto L_800A4FB4;
    tmp = 0x0;
    goto L_800A4FB8;
L_800A4FB4:
    tmp = 0x1;
L_800A4FB8:
    if ((s32)tmp == 0) goto L_800A4FC8;
    tmp = *(u32*)((u8*)r3 + 0x8);
    goto L_800A4FCC;
L_800A4FC8:
    tmp = r26 + 0x1;
L_800A4FCC:
    r26 = tmp;
L_800A4FD0:
    r3 = *(u32*)FstStart_8047A7CC;
    tmp = r3 + 0x8;
    tmp = *(u32*)(r29 + tmp);
    if (r26 < tmp) goto L_800A4EF0;
    r3 = -0x1;
    goto L_800A5008;
L_800A4FEC:
    if ((s32)r30 != 0) goto L_800A4FFC;
    r3 = r26;
    goto L_800A5008;
L_800A4FFC:
    r23 = r27 + r23;
    r23 = r23 + 0x1;
    goto L_800A4D4C;
L_800A5008:
    return;
}

/* fn_800A501C - 0x800A501C | size: 0xC8 */
void fn_800A501C(void) {
    extern u8 lbl_803119B8[];
    extern void fn_800A4D28();
    extern void fn_800A5268();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r4 + 0x0;
    r30 = r3 + 0x0;
    fn_800A4D28();
    if ((s32)r3 >= 0) goto L_800A5070;
    r3 = (u32)sp + 0x10;
    r4 = 0x80;
    fn_800A5268();
    r3 = (u32)lbl_803119B8;
    r3 = (u32)lbl_803119B8;
    r4 = r30 + 0x0;
    r5 = (u32)sp + 0x10;
    OSReport();
    r3 = 0x0;
    goto L_800A50CC;
L_800A5070:
    r5 = r3 * 0xc;
    r3 = *(u32*)FstStart_8047A7CC;
    tmp = *(u32*)(r3 + r5);
    /* clrrwi. tmp, tmp, 24 */;
    if ((s32)r3 != 0) goto L_800A508C;
    tmp = 0x0;
    goto L_800A5090;
L_800A508C:
    tmp = 0x1;
L_800A5090:
    if ((s32)tmp == 0) goto L_800A50A0;
    r3 = 0x0;
    goto L_800A50CC;
L_800A50A0:
    r3 = r3 + r5;
    r4 = *(u32*)((u8*)r3 + 0x4);
    tmp = 0x0;
    r3 = 0x1;
    *(u32*)((u8*)r31 + 0x30) = r4;
    r4 = *(u32*)FstStart_8047A7CC;
    r4 = r4 + r5;
    r4 = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r31 + 0x34) = r4;
    *(u32*)((u8*)r31 + 0x38) = tmp;
    *(u32*)((u8*)r31 + 0xC) = tmp;
L_800A50CC:
    return;
}

/* fn_800A50E4 - 0x800A50E4 | size: 0x24 */
void fn_800A50E4(void) {
    extern void fn_800A7AFC();
    u32 tmp = 0;
    u32 r3 = 0;

    fn_800A7AFC();
    r3 = 0x1;
    return;
}

/* fn_800A5108 - 0x800A5108 | size: 0x160 */
void fn_800A5108(void) {
    extern void fn_800A5108();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r5 + 0x0;
    r29 = r4 + 0x0;
    if (r3 != 0) goto L_800A513C;
    r3 = 0x0;
    goto L_800A5248;
L_800A513C:
    r4 = *(u32*)FstStart_8047A7CC;
    r3 = r3 * 0xc;
    r6 = *(u32*)FstStringStart_8047A7D0;
    r5 = r4 + 0x4;
    tmp = *(u32*)(r4 + r3);
    r3 = *(u32*)(r5 + r3);
    tmp = tmp & 0xFFFFFF;
    r31 = r6 + tmp;
    if (r3 != 0) goto L_800A516C;
    r3 = 0x0;
    goto L_800A51E8;
L_800A516C:
    r3 = r3 * 0xc;
    tmp = *(u32*)(r4 + r3);
    r4 = r29;
    r3 = *(u32*)(r5 + r3);
    r5 = r30 + 0x0;
    tmp = tmp & 0xFFFFFF;
    r28 = r6 + tmp;
    fn_800A5108();
    if (r3 != r30) goto L_800A5198;
    goto L_800A51E8;
L_800A5198:
    tmp = r3 + 0x0;
    r3 = r3 + 0x1;
    r4 = 0x2f;
    r6 = r30 - r3;
    *(u8*)(r29 + tmp) = r4;
    r4 = r6 + 0x0;
    r5 = r29 + r3;
    goto L_800A51CC;
L_800A51B8:
    tmp = *(u8*)((u8*)r28 + 0x0);
    r28 = r28 + 0x1;
    *(u8*)((u8*)r5 + 0x0) = tmp;
    r5 = r5 + 0x1;
L_800A51CC:
    if (r4 == 0) goto L_800A51E0;
    tmp = *(u8*)((u8*)r28 + 0x0);
    tmp = (s8)tmp;
    if (r4 != 0) goto L_800A51B8;
L_800A51E0:
    tmp = r6 - r4;
    r3 = r3 + tmp;
L_800A51E8:
    if (r3 != r30) goto L_800A51F4;
    goto L_800A5248;
L_800A51F4:
    tmp = r3 + 0x0;
    r3 = r3 + 0x1;
    r4 = 0x2f;
    r7 = r30 - r3;
    *(u8*)(r29 + tmp) = r4;
    r6 = r31 + 0x0;
    r4 = r7 + 0x0;
    r5 = r29 + r3;
    goto L_800A522C;
L_800A5218:
    tmp = *(u8*)((u8*)r6 + 0x0);
    r6 = r6 + 0x1;
    *(u8*)((u8*)r5 + 0x0) = tmp;
    r5 = r5 + 0x1;
L_800A522C:
    if (r4 == 0) goto L_800A5240;
    tmp = *(u8*)((u8*)r6 + 0x0);
    tmp = (s8)tmp;
    if (r4 != 0) goto L_800A5218;
L_800A5240:
    tmp = r7 - r4;
    r3 = r3 + tmp;
L_800A5248:
    return;
}

/* fn_800A5268 - 0x800A5268 | size: 0xC4 */
void fn_800A5268(void) {
    extern u8 lbl_8047A7D8[];
    extern void fn_800A5108();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r4 + 0x0;
    r5 = r30 + 0x0;
    r29 = r3 + 0x0;
    r4 = r29 + 0x0;
    r31 = *(u32*)lbl_8047A7D8;
    r3 = r31 + 0x0;
    fn_800A5108();
    if (r3 != r30) goto L_800A52B4;
    tmp = 0x0;
    r3 = r29 + r30;
    *(u8*)((u8*)r3 + (-1)) = tmp;
    goto L_800A530C;
L_800A52B4:
    tmp = r31 * 0xc;
    r4 = *(u32*)FstStart_8047A7CC;
    tmp = *(u32*)(r4 + tmp);
    /* clrrwi. tmp, tmp, 24 */;
    if (r3 != r30) goto L_800A52D0;
    tmp = 0x0;
    goto L_800A52D4;
L_800A52D0:
    tmp = 0x1;
L_800A52D4:
    if ((s32)tmp == 0) goto L_800A5300;
    if (r3 != tmp) goto L_800A52F4;
    tmp = 0x0;
    *(u8*)(r29 + r3) = tmp;
    goto L_800A530C;
L_800A52F4:
    tmp = 0x2f;
    *(u8*)(r29 + r3) = tmp;
    r3 = r3 + 0x1;
L_800A5300:
    tmp = 0x0;
    *(u8*)(r29 + r3) = tmp;
    tmp = 0x1;
L_800A530C:
    r3 = tmp;
    return;
}

/* fn_800A532C - 0x800A532C | size: 0xC0 */
void fn_800A532C(void) {
    extern u8 lbl_803119F0[];
    extern u8 lbl_804789C0[];
    extern void fn_800060F0();
    extern void fn_800A720C();
    extern void fn_800A53EC();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* mr. r29, r6 */;
    r26 = r3 + 0x0;
    r27 = r4 + 0x0;
    r28 = r5 + 0x0;
    r30 = r7 + 0x0;
    r31 = r8 + 0x0;
    if ((s32)tmp < 0) goto L_800A5364;
    tmp = *(u32*)((u8*)r26 + 0x34);
    if (r29 < tmp) goto L_800A537C;
L_800A5364:
    r3 = (u32)lbl_803119F0;
    r5 = (u32)lbl_803119F0;
    r3 = (u32)lbl_804789C0;
    r4 = 0x2e6;
    fn_800060F0();
L_800A537C:
    /* add. r4, r29, r28 */;
    if (r29 < tmp) goto L_800A5394;
    r3 = *(u32*)((u8*)r26 + 0x34);
    tmp = r3 + 0x20;
    if (r4 < tmp) goto L_800A53AC;
L_800A5394:
    r3 = (u32)lbl_803119F0;
    r5 = (u32)lbl_803119F0;
    r3 = (u32)lbl_804789C0;
    r4 = 0x2ec;
    fn_800060F0();
L_800A53AC:
    *(u32*)((u8*)r26 + 0x38) = r30;
    r3 = (u32)fn_800A53EC;
    r7 = (u32)fn_800A53EC;
    tmp = *(u32*)((u8*)r26 + 0x30);
    r3 = r26 + 0x0;
    r4 = r27 + 0x0;
    r5 = r28 + 0x0;
    r8 = r31 + 0x0;
    r6 = tmp + r29;
    fn_800A720C();
    r3 = 0x1;
    return;
}

/* fn_800A53EC - 0x800A53EC | size: 0x30 */
void fn_800A53EC(void) {
    u32 tmp = 0;
    u32 r4 = 0;
    u32 r12 = 0;

    r12 = *(u32*)((u8*)r4 + 0x38);
    if (r12 == 0) goto L_800A540C;
    /* blrl  */;
L_800A540C:
    return;
}

/* fn_800A541C - 0x800A541C | size: 0x118 */
void fn_800A541C(void) {
    extern u8 lbl_80311A24[];
    extern u8 lbl_804789C0[];
    extern void fn_800060F0();
    extern void fn_800A238C();
    extern void fn_800A720C();
    extern void fn_800A5534();
    extern u8 __DVDThreadQueue[];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* mr. r29, r6 */;
    r31 = r3 + 0x0;
    r27 = r4 + 0x0;
    r28 = r5 + 0x0;
    r30 = r7 + 0x0;
    if ((s32)tmp < 0) goto L_800A5450;
    tmp = *(u32*)((u8*)r31 + 0x34);
    if (r29 < tmp) goto L_800A5468;
L_800A5450:
    r3 = (u32)lbl_80311A24;
    r5 = (u32)lbl_80311A24;
    r3 = (u32)lbl_804789C0;
    r4 = 0x32c;
    fn_800060F0();
L_800A5468:
    /* add. r4, r29, r28 */;
    if (r29 < tmp) goto L_800A5480;
    r3 = *(u32*)((u8*)r31 + 0x34);
    tmp = r3 + 0x20;
    if (r4 < tmp) goto L_800A5498;
L_800A5480:
    r3 = (u32)lbl_80311A24;
    r5 = (u32)lbl_80311A24;
    r3 = (u32)lbl_804789C0;
    r4 = 0x332;
    fn_800060F0();
L_800A5498:
    tmp = *(u32*)((u8*)r31 + 0x30);
    r4 = (u32)fn_800A5534;
    r7 = (u32)fn_800A5534;
    r3 = r31 + 0x0;
    r4 = r27 + 0x0;
    r5 = r28 + 0x0;
    r8 = r30 + 0x0;
    r6 = tmp + r29;
    fn_800A720C();
    if ((s32)r3 != 0) goto L_800A54CC;
    r3 = -0x1;
    goto L_800A5520;
L_800A54CC:
    OSDisableInterrupts();
    r30 = r3;
L_800A54D4:
    tmp = *(u32*)((u8*)r31 + 0xC);
    if ((s32)tmp != 0) goto L_800A54E8;
    r31 = *(u32*)((u8*)r31 + 0x20);
    goto L_800A5514;
L_800A54E8:
    if ((s32)tmp != (s32)-0x1) goto L_800A54F8;
    r31 = -0x1;
    goto L_800A5514;
L_800A54F8:
    if ((s32)tmp != 0xa) goto L_800A5508;
    r31 = -0x3;
    goto L_800A5514;
L_800A5508:
    r3 = (u32)__DVDThreadQueue;
    fn_800A238C();
    goto L_800A54D4;
L_800A5514:
    r3 = r30;
    OSRestoreInterrupts();
    r3 = r31;
L_800A5520:
    return;
}

/* fn_800A5534 - 0x800A5534 | size: 0x24 */
void fn_800A5534(void) {
    extern void fn_800A2478();
    extern u8 __DVDThreadQueue[];
    u32 tmp = 0;
    u32 r3 = 0;

    r3 = (u32)__DVDThreadQueue;
    fn_800A2478();
    return;
}

/* fn_800A5558 - 0x800A5558 | size: 0x98 */
void fn_800A5558(void) {
    extern u8 lbl_80311A54[];
    extern u8 lbl_804789C0[];
    extern void fn_800060F0();
    extern void fn_800A72E8();
    extern void fn_800A55F0();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r6 + 0x0;
    r30 = r5 + 0x0;
    /* mr. r29, r4 */;
    r28 = r3 + 0x0;
    if ((s32)tmp < 0) goto L_800A5594;
    tmp = *(u32*)((u8*)r28 + 0x34);
    if (r29 < tmp) goto L_800A55AC;
L_800A5594:
    r3 = (u32)lbl_80311A54;
    r5 = (u32)lbl_80311A54;
    r3 = (u32)lbl_804789C0;
    r4 = 0x383;
    fn_800060F0();
L_800A55AC:
    *(u32*)((u8*)r28 + 0x38) = r30;
    r3 = (u32)fn_800A55F0;
    r5 = (u32)fn_800A55F0;
    tmp = *(u32*)((u8*)r28 + 0x30);
    r3 = r28 + 0x0;
    r6 = r31 + 0x0;
    r4 = tmp + r29;
    fn_800A72E8();
    r3 = 0x1;
    return;
}

/* fn_800A55F0 - 0x800A55F0 | size: 0x30 */
void fn_800A55F0(void) {
    u32 tmp = 0;
    u32 r4 = 0;
    u32 r12 = 0;

    r12 = *(u32*)((u8*)r4 + 0x38);
    if (r12 == 0) goto L_800A5610;
    /* blrl  */;
L_800A5610:
    return;
}

/* fn_800A5620 - 0x800A5620 | size: 0x4 */
/* Empty function (blr) - no-op placeholder */
void fn_800A5620(void) {
}

