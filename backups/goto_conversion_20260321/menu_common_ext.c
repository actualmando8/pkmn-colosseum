/**
 * @file menu_common_ext.c
 * @brief Menu common extensions - helpers, draw, input (0x8007109C-0x80072A00)
 *
 * Address range: 0x8007109C - 0x80072A00
 * Total functions: 23
 */

#include "dolphin/types.h"

/* ===== External function declarations ===== */
extern void fn_8006A7E8();
extern void fn_8006B154();
extern void fn_80073A44();
extern void fn_80073C38();
extern void fn_8008ABA0();
extern void fn_8008ABE4();
extern void fn_800A13F8();
extern void fn_800A1F94();
extern void fn_800A221C();
extern void fn_800D0F44();
extern void fn_800E202C();
extern void fn_800E209C();
extern void fn_800E24B0();
extern void fn_800E27B0();
extern void fn_800E2C04();
extern void fn_800F7AF0();
extern void fn_800F7BC4();
extern void fn_80102568();
extern void fn_801026A4();
extern void fn_801046B8();
extern void fn_80104704();
extern void fn_80113828();
extern void fn_80129280();
extern void fn_80196E10();
extern void fn_8025F3F4();
extern void fn_8025F584();
extern void fn_8025F648();
extern void OSCreateAlarm();
extern void OSDisableInterrupts();
extern void OSGetTick();
extern void OSRestoreInterrupts();
extern void OSSetAlarm();

/* ===== SDA globals ===== */
extern u8 lbl_8047A600;
extern u8 lbl_8047C090;

/* ===== Rodata / data labels ===== */
extern u8 lbl_80268708[];
extern u8 lbl_80268718[];
extern u8 lbl_80268750[];
extern u8 lbl_803B6D88[];
extern u8 lbl_803B6DE0[];
extern u8 lbl_803B6E08[];
extern u8 lbl_803B6E18[];

/* ===== Forward declarations ===== */
void fn_8007109C(void);
s32 fn_80071104(void);
s32 fn_80071160(void);
void fn_80071208(void);
void fn_80071318(void);
void fn_80071344(void);
s32 fn_80071398(void);
s32 fn_800714C8(void);
void fn_800715BC(void);
void fn_8007162C(void);
void fn_80071644(void);
s32 fn_8007169C(void);
s32 fn_800716C8(void);
s32 fn_800716E8(void);
s32 fn_80071700(void);
void fn_800719A8(void);
s32 fn_80071AE4(void);
void fn_80071E34(void);
s32 fn_80071EA4(void);
s32 fn_800722A0(void);
void fn_80072548(void);
void fn_80072684(void);
s32 fn_800726A8(void);

/* ===== Function implementations ===== */


/* 0x8007109C | size: 0x68 */
void fn_8007109C(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    ((void(*)(void))fn_800E202C)();
    r31 = r3;
    r0 = r31 & 0xFFFF;
    if (r0 == 0x0) {
        r3 = (u32)&lbl_80268708;
        r4 = 0xde;
        r3 = (u32)&lbl_80268708;
        r5 = (u32)&lbl_8047C090;
        ((void(*)(void))fn_80196E10)();
    }
    r0 = r31 & 0xFFFF;
    if (r0 != 0x0) {
        r3 = r31;
        ((void(*)(void))fn_800E24B0)();
        r3 = r31;
        ((void(*)(void))fn_800E209C)();
    }
    return;
}

/* 0x80071104 | size: 0x5C */
s32 fn_80071104(void) {
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    tmp = r3 + 0x1f;
    r4 = 0x20;
    /* clrrwi r3, tmp, 5 */;
    ((void(*)(void))fn_800E2C04)();
    tmp = r3 & 0xFFFF;
    if (tmp != 0) {
        ((void(*)(void))fn_800E27B0)();
    } else {

        if (tmp == 0) {
            r3 = (u32)&lbl_80268708;
            r4 = 0xd5;
            r3 = (u32)&lbl_80268708;
            r5 = (u32)&lbl_8047C090;
            ((void(*)(void))fn_80196E10)();
        }
        r3 = 0x0;
    }
    return;
}

/* 0x80071160 | size: 0xA8 */
s32 fn_80071160(void) {
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = 0x0;
    r31 = 0x0;
    do {
        r3 = 0x0;
        r4 = 0xe;
        ((void(*)(void))fn_80129280)();
        tmp = r31 + 0x59cc;
        r29 = *(u32*)(r3 + tmp);
        if ((s32)r29 != 0) {
            r3 = 0x0;
            r4 = 0xe;
            ((void(*)(void))fn_80129280)();
            tmp = r31 + 0x59a8;
            r3 = r3 + tmp;
            ((void(*)(void))fn_8006A7E8)();
            if ((s32)r3 != 0) {
                r3 = r29;
                ((void(*)(void))fn_8008ABA0)();
                tmp = r3 & 0xFF;
                if (tmp == 0) {
                    r3 = r29;
                    return;
        }
        }
        }
        r31 = r31 + 0x1660;
        r30 = r30 + 0x1;
    } while (r30 < 4);
    r3 = 0x0;

    return;
}

/* 0x80071208 | size: 0x110 */
void fn_80071208(void) {
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    ((void(*)(void))fn_800F7AF0)();
    r31 = r3;
    r3 = r30;
    ((void(*)(void))fn_800F7BC4)();
    r31 = r3 & r31;
    if (r31 == 0) {
        r3 = r30;
        ((void(*)(void))fn_8008ABA0)();
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            r3 = r30;
            ((void(*)(void))fn_8006B154)();
            r4 = (u32)sp + 0x8;
            ((void(*)(void))fn_80073A44)();
            if ((s32)r3 == 0) {
                r3 = *(u16*)(sp + 0x8);
                tmp = r3 & 0x1;
                if ((s32)tmp != 0) {
                    r31 = r31 | 0x100;
                }
                tmp = r3 & 0x00000002;
                if ((s32)tmp != 0) {
                    r31 = r31 | 0x200;
                }
                tmp = r3 & 0x00000008;
                if ((s32)tmp != 0) {
                    r31 = r31 | 0x1000;
                }
                tmp = r3 & 0x00000010;
                if ((s32)tmp != 0) {
                    r31 = r31 | 0x2;
                }
                tmp = r3 & 0x00000020;
                if ((s32)tmp != 0) {
                    r31 = r31 | 0x1;
                }
                tmp = r3 & 0x00000040;
                if ((s32)tmp != 0) {
                    r31 = r31 | 0x8;
                }
                tmp = r3 & 0x00000080;
                if ((s32)tmp != 0) {
                    r31 = r31 | 0x4;
                }
                tmp = r3 & 0x00000100;
                if ((s32)tmp != 0) {
                    r31 = r31 | 0x20;
                }
                tmp = r3 & 0x00000200;
                if ((s32)tmp != 0) {
                    r31 = r31 | 0x40;
    }
    }
    }
    }
    r3 = r31;
    return;
}

/* 0x80071318 | size: 0x2C */
void fn_80071318(void) {
}

/* 0x80071344 | size: 0x54 */
void fn_80071344(void) {
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;

    r3 = (u32)&lbl_803B6D88;
    r4 = 0x0;
    r3 = (u32)&lbl_803B6D88;
    r6 = 0x10;
    tmp = *(u32*)((u8*)r3 + 0x40);
    r7 = 0x1;
    r8 = 0x0;
    r9 = 0x0;
    tmp = tmp << 3;
    r5 = r3 + tmp;
    r3 = *(u32*)((u8*)r5 + 0x0);
    r5 = r5 + 0x4;
    ((void(*)(void))fn_801026A4)();
    return;
}

/* 0x80071398 | size: 0x130 */
s32 fn_80071398(void) {
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    r3 = (u32)&lbl_803B6D88;
    r31 = (u32)&lbl_803B6D88;
    tmp = *(u32*)((u8*)r31 + 0x40);
    tmp = tmp << 3;
    r29 = *(u32*)(r31 + tmp);
    ((void(*)(void))fn_801046B8)();
    if ((s32)r3 == (s32)r29) {
        tmp = *(u32*)((u8*)r31 + 0x40);
        r3 = (u32)&lbl_803B6D88;
        r3 = (u32)&lbl_803B6D88;
        r4 = 0x0;
        tmp = tmp << 3;
        r5 = 0x0;
        r3 = *(u32*)(r3 + tmp);
        ((void(*)(void))fn_80102568)();
    }
    r3 = 0xbe;
    ((void(*)(void))fn_80104704)();
    if (r3 != 0) {
        r3 = 0xbe;
        r4 = 0x0;
        r5 = 0x1;
        ((void(*)(void))fn_80102568)();
    }
    tmp = *(u32*)((u8*)r31 + 0x40);
    r3 = (u32)&lbl_803B6D88;
    r3 = (u32)&lbl_803B6D88;
    r4 = 0x0;
    tmp = tmp << 3;
    r3 = r3 + tmp;
    *(u32*)((u8*)r3 + 0x4) = r4;
    tmp = *(u32*)((u8*)r31 + 0x40);
    if ((s32)tmp != 0) {
        if ((s32)tmp <= 0) {
            r3 = (u32)&lbl_80268708;
            r5 = (u32)&lbl_80268718;
            r3 = (u32)&lbl_80268708;
            r4 = 0x5c;
            r5 = (u32)&lbl_80268718;
            ((void(*)(void))fn_80196E10)();
        }
        r3 = *(u32*)((u8*)r31 + 0x40);
        *(u32*)((u8*)r31 + 0x40) = tmp;
    }
    r3 = (u32)&lbl_803B6D88;
    r3 = (u32)&lbl_803B6D88;
    goto L_80071484;
L_80071470:
    if ((s32)r4 == 0) goto L_80071498;
    r4 = *(u32*)((u8*)r31 + 0x40);
    *(u32*)((u8*)r31 + 0x40) = tmp;
L_80071484:
    r4 = *(u32*)((u8*)r31 + 0x40);
    tmp = r4 << 3;
    tmp = *(u32*)(r3 + tmp);
    if ((s32)r30 != (s32)tmp) goto L_80071470;
L_80071498:
    tmp = *(u32*)((u8*)r31 + 0x40);
    r3 = (u32)&lbl_803B6D88;
    r3 = (u32)&lbl_803B6D88;
    tmp = tmp << 3;
    r3 = *(u32*)(r3 + tmp);
    return;
}

/* 0x800714C8 | size: 0xF4 */
s32 fn_800714C8(void) {
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = (u32)&lbl_803B6D88;
    r31 = (u32)&lbl_803B6D88;
    tmp = *(u32*)((u8*)r31 + 0x40);
    tmp = tmp << 3;
    r30 = *(u32*)(r31 + tmp);
    ((void(*)(void))fn_801046B8)();
    if ((s32)r3 == (s32)r30) {
        tmp = *(u32*)((u8*)r31 + 0x40);
        r3 = (u32)&lbl_803B6D88;
        r3 = (u32)&lbl_803B6D88;
        r4 = 0x0;
        tmp = tmp << 3;
        r5 = 0x0;
        r3 = *(u32*)(r3 + tmp);
        ((void(*)(void))fn_80102568)();
    }
    r3 = 0xbe;
    ((void(*)(void))fn_80104704)();
    if (r3 != 0) {
        r3 = 0xbe;
        r4 = 0x0;
        r5 = 0x1;
        ((void(*)(void))fn_80102568)();
    }
    tmp = *(u32*)((u8*)r31 + 0x40);
    r3 = (u32)&lbl_803B6D88;
    r3 = (u32)&lbl_803B6D88;
    r4 = 0x0;
    tmp = tmp << 3;
    r3 = r3 + tmp;
    *(u32*)((u8*)r3 + 0x4) = r4;
    tmp = *(u32*)((u8*)r31 + 0x40);
    if ((s32)tmp == 0) {
        r3 = -0x1;
    } else {

        if ((s32)tmp <= 0) {
            r3 = (u32)&lbl_80268708;
            r5 = (u32)&lbl_80268718;
            r3 = (u32)&lbl_80268708;
            r4 = 0x5c;
            r5 = (u32)&lbl_80268718;
            ((void(*)(void))fn_80196E10)();
        }
        r4 = *(u32*)((u8*)r31 + 0x40);
        r3 = (u32)&lbl_803B6D88;
        r3 = (u32)&lbl_803B6D88;
        *(u32*)((u8*)r31 + 0x40) = tmp;
        tmp = tmp << 3;
        r3 = *(u32*)(r3 + tmp);
    }
    return;
}

/* 0x800715BC | size: 0x70 */
void fn_800715BC(void) {
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r4 = (u32)&lbl_803B6D88;
    r5 = (u32)&lbl_803B6D88;
    r4 = *(u32*)((u8*)r5 + 0x40);
    if (r4 >= 8) {
        r3 = (u32)&lbl_80268708;
        r5 = (u32)&lbl_80268750;
        r3 = (u32)&lbl_80268708;
        r4 = 0x41;
        r5 = (u32)&lbl_80268750;
        ((void(*)(void))fn_80196E10)();
    } else {

        tmp = r4 + 0x1;
        r4 = 0x0;
        *(u32*)((u8*)r5 + 0x40) = tmp;
        tmp = tmp << 3;
        *(u32*)(r5 + tmp) = r3;
        tmp = *(u32*)((u8*)r5 + 0x40);
        tmp = tmp << 3;
        r3 = r5 + tmp;
        *(u32*)((u8*)r3 + 0x4) = r4;
    }
    return;
}

/* 0x8007162C | size: 0x18 */
void fn_8007162C(void) {
}

/* 0x80071644 | size: 0x58 */
void fn_80071644(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r4 = (u32)&lbl_803B6D88;
    tmp = 0x0;
    r4 = (u32)&lbl_803B6D88;
    *(u32*)((u8*)r4 + 0x0) = tmp;
    *(u32*)((u8*)r4 + 0x4) = tmp;
    *(u32*)((u8*)r4 + 0x8) = tmp;
    *(u32*)((u8*)r4 + 0xC) = tmp;
    *(u32*)((u8*)r4 + 0x10) = tmp;
    *(u32*)((u8*)r4 + 0x14) = tmp;
    *(u32*)((u8*)r4 + 0x18) = tmp;
    *(u32*)((u8*)r4 + 0x1C) = tmp;
    *(u32*)((u8*)r4 + 0x20) = tmp;
    *(u32*)((u8*)r4 + 0x24) = tmp;
    *(u32*)((u8*)r4 + 0x28) = tmp;
    *(u32*)((u8*)r4 + 0x2C) = tmp;
    *(u32*)((u8*)r4 + 0x30) = tmp;
    *(u32*)((u8*)r4 + 0x34) = tmp;
    *(u32*)((u8*)r4 + 0x38) = tmp;
    *(u32*)((u8*)r4 + 0x3C) = tmp;
    *(u32*)((u8*)r4 + 0x0) = r3;
    *(u32*)((u8*)r4 + 0x40) = tmp;
    return;
}

/* 0x8007169C | size: 0x2C */
s32 fn_8007169C(void) {
    fn_80113828();
    return 0;
}

/* 0x800716C8 | size: 0x20 */
s32 fn_800716C8(void) {
    return 0;
}

/* 0x800716E8 | size: 0x18 */
s32 fn_800716E8(void) {
    return 0;
}

/* 0x80071700 | size: 0x2A8 */
s32 fn_80071700(void) {
    u8 sp[0x40];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r30 = r3;
    r31 = r30 + 0x1;
    r4 = 0x2;
    r3 = r31;
    ((void(*)(void))fn_8008ABE4)();
    r3 = r30;
    ((void(*)(void))fn_80073C38)();
    if ((s32)r3 != 0) {
        r27 = r3;
        goto L_80071984;
    }
    tmp = 0x44;
    r3 = r30;
    *(u32*)(sp + 0xC) = tmp;
    r4 = (u32)sp + 0xc;
    r5 = (u32)sp + 0xa;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == 0) goto L_80071764;
    r27 = 0xb;
    goto L_80071858;
L_80071764:
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r26 = tmp * 0x64;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r30 << 3;
    r29 = r3;
    tmp = (u32)&lbl_803B6E18;
    r28 = r30 << 2;
    r24 = tmp + r6;
    r27 = (u32)&lbl_803B6E08;
    r25 = r24 + 0x4;
L_800717AC:
    OSGetTick();
    tmp = r3 - r29;
    if (tmp <= r26) goto L_800717C4;
    r3 = 0x1;
    goto L_80071844;
L_800717C4:
    r3 = r30;
    r4 = (u32)sp + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_800717E0;
    r3 = 0x2;
    goto L_80071844;
L_800717E0:
    tmp = *(u8*)(sp + 0x8);
    tmp = tmp & 0xa;
    if ((s32)tmp == 8) goto L_80071820;
    r12 = *(u32*)((u8*)r24 + 0x0);
    if (r12 != 0) {
        r3 = r30;
        r4 = *(u32*)((u8*)r25 + 0x0);
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    tmp = *(u32*)(r27 + r28);
    if ((s32)tmp == 0) goto L_800717AC;
    r3 = 0x3e8;
    goto L_80071844;
L_80071820:
    r3 = r30;
    r4 = (u32)sp + 0x10;
    r5 = (u32)sp + 0xa;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_80071840;
    r3 = 0x3;
    goto L_80071844;
L_80071840:
    r3 = 0x0;
L_80071844:
    if ((s32)r3 == 0) goto L_80071854;
    r27 = r3 + 0xb;
    goto L_80071858;
L_80071854:
    r27 = 0x0;
L_80071858:
    if ((s32)r27 != 0) {
        goto L_80071984;
    }
    tmp = (u32)tmp >> 24;
    if (tmp != 0x44) {
        r27 = 0xf;
        goto L_80071984;
    }
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r26 = tmp * 0x7530;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r30 << 3;
    r27 = r3;
    tmp = (u32)&lbl_803B6E18;
    r28 = r30 << 2;
    r24 = tmp + r6;
    r29 = (u32)&lbl_803B6E08;
    r25 = r24 + 0x4;
L_800718C4:
    OSGetTick();
    tmp = r3 - r27;
    if (tmp <= r26) goto L_800718DC;
    r3 = 0x1;
    goto L_8007195C;
L_800718DC:
    r3 = r30;
    r4 = (u32)sp + 0x9;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_800718F8;
    r3 = 0x2;
    goto L_8007195C;
L_800718F8:
    tmp = *(u8*)(sp + 0x9);
    tmp = tmp & 0xa;
    if ((s32)tmp == 8) goto L_80071938;
    r12 = *(u32*)((u8*)r24 + 0x0);
    if (r12 != 0) {
        r3 = r30;
        r4 = *(u32*)((u8*)r25 + 0x0);
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    tmp = *(u32*)(r29 + r28);
    if ((s32)tmp == 0) goto L_800718C4;
    r3 = 0x3e8;
    goto L_8007195C;
L_80071938:
    r3 = r30;
    r4 = (u32)sp + 0x10;
    r5 = (u32)sp + 0xb;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_80071958;
    r3 = 0x3;
    goto L_8007195C;
L_80071958:
    r3 = 0x0;
L_8007195C:
    if ((s32)r3 != 0) {
        r27 = r3 + 0xf;

    } else if (tmp != 0) {
        r27 = 0x13;

    } else {
        r27 = 0x0;
    }
L_80071984:
    r3 = r31;
    r4 = 0x1;
    ((void(*)(void))fn_8008ABE4)();
    r3 = r27;
    return;
}

/* 0x800719A8 | size: 0x13C */
void fn_800719A8(void) {
    extern void fn_80072684();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;

    r31 = r3;
    ((void(*)(void))fn_800A13F8)();
    r4 = (u32)&lbl_803B6DE0;
    *(u32*)&lbl_8047A600 = r3;
    r3 = (u32)&lbl_803B6DE0;
    OSCreateAlarm();
    OSDisableInterrupts();
    r4 = 0x80000000;
    r5 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r4 = (u32)fn_80072684;
    r5 = r5 + 0x4dd3;
    r6 = (u32)&lbl_803B6DE0;
    tmp = (u32)tmp >> 2;
    r7 = (u32)fn_80072684;
    tmp = (u32)((u64)r5 * (u64)tmp >> 32);
    r5 = (u32)&lbl_803B6DE0;
    r30 = r3;
    r3 = r5;
    r5 = 0x0;
    r6 = (u32)tmp >> 6;
    OSSetAlarm();
    r3 = *(u32*)&lbl_8047A600;
    ((void(*)(void))fn_800A221C)();
    r3 = r30;
    OSRestoreInterrupts();
    r3 = r31;
    ((void(*)(void))fn_800D0F44)();
    /* subis tmp, r3, 0x4 */;
    if (tmp != 0) {
        r30 = 0x1;
        goto L_80071AAC;
    }
    r3 = r31;
    r4 = (u32)sp + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 != 0) {
        r30 = 0x2;
        goto L_80071AAC;
    }
    tmp = *(u8*)(sp + 0x8);
    tmp = tmp & 0x00000008;
    if ((s32)tmp == 0) {
        r30 = -0x1;
        goto L_80071AAC;
    }
    r3 = r31;
    r4 = (u32)sp + 0xc;
    r5 = (u32)sp + 0x8;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 != 0) {
        r30 = 0x3;

    } else if (tmp != 0) {
        r30 = 0x4;

    } else {
        r30 = 0x0;
    }
L_80071AAC:
    if ((s32)r30 != 0) {
        if ((s32)r30 < 3) { r3 = r30; return; }
    }
    r3 = r31 + 0x1;
    r4 = 0x1;
    ((void(*)(void))fn_8008ABE4)();

    r3 = r30;
    return;
}

/* 0x80071AE4 | size: 0x350 */
s32 fn_80071AE4(void) {
    u8 sp[0x70];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r14 = 0;
    u32 r15 = 0;
    u32 r16 = 0;
    u32 r17 = 0;
    u32 r18 = 0;
    u32 r19 = 0;
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
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r15 = r3;
    r16 = r4;
    tmp = r15 + 0x1;
    r4 = 0x2;
    *(u32*)(sp + 0x18) = tmp;
    r3 = tmp;
    ((void(*)(void))fn_8008ABE4)();
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r18 = tmp * 0x64;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r15 << 3;
    r25 = r3;
    tmp = (u32)&lbl_803B6E18;
    r26 = r15 << 2;
    r19 = tmp + r6;
    r27 = (u32)&lbl_803B6E08;
    r20 = r19 + 0x4;
    r5 = *(u32*)((u8*)r16 + 0xC);
    r3 = (u32)r5 >> 24;
    tmp = ((r5 << 24) | ((u32)r5 >> 8)) & 0x0000FF00;
    r4 = ((r5 << 8) | ((u32)r5 >> 24)) & 0x00FF0000;
    r5 = r5 << 24;
    tmp = r3 | tmp;
    tmp = r4 | tmp;
    tmp = r5 | tmp;
    r14 = (u32)tmp >> 16;
    r23 = tmp & 0xFFFF;
L_80071B80:
    OSGetTick();
    r4 = r3 - r25;
    r3 = r15;
    tmp = r4 ^ r18;
    tmp = __cntlzw(tmp);
    tmp = r4 << tmp;
    r17 = (u32)tmp >> 31;
    ((void(*)(void))fn_80073C38)();
    if ((s32)r3 == 0) goto L_80071BB0;
    r21 = r3;
    goto L_80071DF0;
L_80071BB0:
    tmp = 0x22;
    r3 = r15;
    *(u32*)(sp + 0x10) = tmp;
    r4 = (u32)sp + 0x10;
    r5 = (u32)sp + 0xa;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == 0) goto L_80071BD8;
    r21 = 0xb;
    goto L_80071CAC;
L_80071BD8:
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r22 = tmp * 0x64;
    OSGetTick();
    r21 = r3;
L_80071C00:
    OSGetTick();
    tmp = r3 - r21;
    if (tmp <= r22) goto L_80071C18;
    r3 = 0x1;
    goto L_80071C98;
L_80071C18:
    r3 = r15;
    r4 = (u32)sp + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80071C34;
    r3 = 0x2;
    goto L_80071C98;
L_80071C34:
    tmp = *(u8*)(sp + 0x8);
    tmp = tmp & 0xa;
    if ((s32)tmp == 8) goto L_80071C74;
    r12 = *(u32*)((u8*)r19 + 0x0);
    if (r12 != 0) {
        r3 = r15;
        r4 = *(u32*)((u8*)r20 + 0x0);
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    tmp = *(u32*)(r27 + r26);
    if ((s32)tmp == 0) goto L_80071C00;
    r3 = 0x3e8;
    goto L_80071C98;
L_80071C74:
    r3 = r15;
    r4 = (u32)sp + 0x14;
    r5 = (u32)sp + 0xa;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_80071C94;
    r3 = 0x3;
    goto L_80071C98;
L_80071C94:
    r3 = 0x0;
L_80071C98:
    if ((s32)r3 == 0) goto L_80071CA8;
    r21 = r3 + 0xb;
    goto L_80071CAC;
L_80071CA8:
    r21 = 0x0;
L_80071CAC:
    if ((s32)r21 == 0) goto L_80071CB8;
    goto L_80071DF0;
L_80071CB8:
    tmp = (u32)tmp >> 24;
    if (tmp == 0x22) goto L_80071CD0;
    r21 = 0xf;
    goto L_80071DF0;
L_80071CD0:
    tmp = r14 & 0xFFFF;
    r3 = r23 << 2;
    r24 = r3 + 0x10;
    if (tmp != 0) {
        r24 = r24 + 0x70;
    }
    r22 = 0x0;
    r3 = 0x10620000;
    r31 = r16;
    r28 = r3 + 0x4dd3;
    r29 = 0x80000000;
    goto L_80071DD4;
L_80071D00:
    tmp = *(u32*)((u8*)r31 + 0x0);
    r3 = r15;
    r4 = (u32)sp + 0xc;
    r5 = (u32)sp + 0x9;
    *(u32*)(sp + 0xC) = tmp;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == 0) goto L_80071D28;
    r21 = 0x10;
    goto L_80071DE0;
L_80071D28:
    tmp = *(u32*)((u8*)r29 + 0xF8);
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r28 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r21 = tmp * 0x64;
    OSGetTick();
    r30 = r3;
L_80071D44:
    OSGetTick();
    tmp = r3 - r30;
    if (tmp <= r21) goto L_80071D5C;
    r21 = 0x11;
    goto L_80071DE0;
L_80071D5C:
    r3 = r15;
    r4 = (u32)sp + 0x9;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80071D78;
    r21 = 0x12;
    goto L_80071DE0;
L_80071D78:
    tmp = *(u8*)(sp + 0x9);
    tmp = tmp & 0x00000002;
    if ((s32)tmp == 0) goto L_80071DB8;
    r12 = *(u32*)((u8*)r19 + 0x0);
    if (r12 != 0) {
        r3 = r15;
        r4 = *(u32*)((u8*)r20 + 0x0);
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    tmp = *(u32*)(r27 + r26);
    if ((s32)tmp == 0) goto L_80071D44;
    r21 = 0x3e8;
    goto L_80071DE0;
L_80071DB8:
    tmp = *(u32*)(r27 + r26);
    if ((s32)tmp == 0) goto L_80071DCC;
    r21 = 0x3e8;
    goto L_80071DE0;
L_80071DCC:
    r22 = r22 + 0x4;
    r31 = r31 + 0x4;
L_80071DD4:
    if ((s32)r22 < (s32)r24) goto L_80071D00;
    r21 = 0x0;
L_80071DE0:
    if ((s32)r21 == 0) goto L_80071DEC;
    goto L_80071DF0;
L_80071DEC:
    r21 = 0x0;
L_80071DF0:
    if ((s32)r21 != 1) goto L_80071E00;
    if ((s32)r17 == 0) goto L_80071B80;
L_80071E00:
    if ((s32)r21 != 0) {
        r4 = 0x1;
    } else {

        r4 = 0x3;
    }
    ((void(*)(void))fn_8008ABE4)();
    r3 = r21;
    return;
}

/* 0x80071E34 | size: 0x70 */
void fn_80071E34(void) {
    extern void fn_80071EA4();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r3;
    r30 = r4;
    r31 = r29 + 0x1;
    r4 = 0x2;
    r3 = r31;
    ((void(*)(void))fn_8008ABE4)();
    r3 = r29;
    r4 = r30;
    fn_80071EA4();
    tmp = r3;
    r3 = r31;
    r31 = tmp;
    r4 = 0x1;
    ((void(*)(void))fn_8008ABE4)();
    r3 = r31;
    return;
}

/* 0x80071EA4 | size: 0x3FC */
s32 fn_80071EA4(void) {
    u8 sp[0x50];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
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
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r28 = r3;
    r29 = r4;
    ((void(*)(void))fn_80073C38)();
    if ((s32)r3 != 0) {
        return;
    }
    tmp = 0x33;
    r3 = r28;
    *(u32*)(sp + 0x18) = tmp;
    r4 = (u32)sp + 0x18;
    r5 = (u32)sp + 0xd;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == 0) goto L_80071EF4;
    r3 = 0xb;
    goto L_80071FE8;
L_80071EF4:
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r24 = tmp * 0x64;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r28 << 3;
    r23 = r3;
    tmp = (u32)&lbl_803B6E18;
    r22 = r28 << 2;
    r25 = tmp + r6;
    r21 = (u32)&lbl_803B6E08;
    r20 = r25 + 0x4;
L_80071F3C:
    OSGetTick();
    tmp = r3 - r23;
    if (tmp <= r24) goto L_80071F54;
    r3 = 0x1;
    goto L_80071FD4;
L_80071F54:
    r3 = r28;
    r4 = (u32)sp + 0xa;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80071F70;
    r3 = 0x2;
    goto L_80071FD4;
L_80071F70:
    tmp = *(u8*)(sp + 0xA);
    tmp = tmp & 0xa;
    if ((s32)tmp == 8) goto L_80071FB0;
    r12 = *(u32*)((u8*)r25 + 0x0);
    if (r12 != 0) {
        r3 = r28;
        r4 = *(u32*)((u8*)r20 + 0x0);
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    tmp = *(u32*)(r21 + r22);
    if ((s32)tmp == 0) goto L_80071F3C;
    r3 = 0x3e8;
    goto L_80071FD4;
L_80071FB0:
    r3 = r28;
    r4 = (u32)sp + 0x1c;
    r5 = (u32)sp + 0xd;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_80071FD0;
    r3 = 0x3;
    goto L_80071FD4;
L_80071FD0:
    r3 = 0x0;
L_80071FD4:
    if ((s32)r3 == 0) goto L_80071FE4;
    r3 = r3 + 0xb;
    goto L_80071FE8;
L_80071FE4:
    r3 = 0x0;
L_80071FE8:
    if ((s32)r3 != 0) {
        return;
    }
    tmp = (u32)tmp >> 24;
    if (tmp != 0x33) {
        r3 = 0xf;
        return;
    }
    r4 = (u32)&lbl_803B6E18;
    r3 = (u32)&lbl_803B6E08;
    r5 = r28 << 3;
    r27 = r28 << 2;
    tmp = (u32)&lbl_803B6E18;
    r26 = (u32)&lbl_803B6E08;
    r31 = tmp + r5;
    r23 = 0x0;
    r30 = r31 + 0x4;
    r3 = 0x10620000;
    r22 = r29;
    r25 = r3 + 0x4dd3;
    r24 = 0x80000000;
L_80072040:
    tmp = *(u32*)((u8*)r24 + 0xF8);
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r25 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r20 = tmp * 0x64;
    OSGetTick();
    r21 = r3;
L_8007205C:
    OSGetTick();
    tmp = r3 - r21;
    if (tmp <= r20) goto L_80072074;
    r3 = 0x1;
    goto L_800720F4;
L_80072074:
    r3 = r28;
    r4 = (u32)sp + 0x9;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80072090;
    r3 = 0x2;
    goto L_800720F4;
L_80072090:
    tmp = *(u8*)(sp + 0x9);
    tmp = tmp & 0xa;
    if ((s32)tmp == 8) goto L_800720D0;
    r12 = *(u32*)((u8*)r31 + 0x0);
    if (r12 != 0) {
        r3 = r28;
        r4 = *(u32*)((u8*)r30 + 0x0);
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    tmp = *(u32*)(r26 + r27);
    if ((s32)tmp == 0) goto L_8007205C;
    r3 = 0x3e8;
    goto L_800720F4;
L_800720D0:
    r3 = r28;
    r4 = (u32)sp + 0x14;
    r5 = (u32)sp + 0xc;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_800720F0;
    r3 = 0x3;
    goto L_800720F4;
L_800720F0:
    r3 = 0x0;
L_800720F4:
    if ((s32)r3 == 0) goto L_80072100;
    goto L_80072130;
L_80072100:
    *(u32*)((u8*)r22 + 0x0) = tmp;
    tmp = *(u32*)(r26 + r27);
    if ((s32)tmp == 0) goto L_8007211C;
    r3 = 0x3e8;
    goto L_80072130;
L_8007211C:
    r23 = r23 + 0x4;
    r22 = r22 + 0x4;
    if ((s32)r23 < 0x10) goto L_80072040;
    r3 = 0x0;
L_80072130:
    if ((s32)r3 != 0) {
        r3 = r3 + 0xf;
        return;
    }
    r5 = *(u32*)((u8*)r29 + 0xC);
    r6 = (u32)&lbl_803B6E08;
    r24 = r28 << 2;
    r23 = 0x0;
    r3 = (u32)r5 >> 24;
    tmp = ((r5 << 24) | ((u32)r5 >> 8)) & 0x0000FF00;
    r4 = ((r5 << 8) | ((u32)r5 >> 24)) & 0x00FF0000;
    r5 = r5 << 24;
    tmp = r3 | tmp;
    r25 = (u32)&lbl_803B6E08;
    tmp = r4 | tmp;
    tmp = r5 | tmp;
    tmp = tmp & 0xFFFF;
    r22 = tmp << 2;
    r3 = 0x10620000;
    r27 = 0x80000000;
    r26 = r3 + 0x4dd3;
    goto L_8007226C;
L_80072188:
    tmp = *(u32*)((u8*)r27 + 0xF8);
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r26 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r20 = tmp * 0x64;
    OSGetTick();
    r21 = r3;
L_800721A4:
    OSGetTick();
    tmp = r3 - r21;
    if (tmp <= r20) goto L_800721BC;
    r3 = 0x1;
    goto L_8007223C;
L_800721BC:
    r3 = r28;
    r4 = (u32)sp + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_800721D8;
    r3 = 0x2;
    goto L_8007223C;
L_800721D8:
    tmp = *(u8*)(sp + 0x8);
    tmp = tmp & 0xa;
    if ((s32)tmp == 8) goto L_80072218;
    r12 = *(u32*)((u8*)r31 + 0x0);
    if (r12 != 0) {
        r3 = r28;
        r4 = *(u32*)((u8*)r30 + 0x0);
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    tmp = *(u32*)(r25 + r24);
    if ((s32)tmp == 0) goto L_800721A4;
    r3 = 0x3e8;
    goto L_8007223C;
L_80072218:
    r3 = r28;
    r4 = (u32)sp + 0x10;
    r5 = (u32)sp + 0xb;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_80072238;
    r3 = 0x3;
    goto L_8007223C;
L_80072238:
    r3 = 0x0;
L_8007223C:
    if ((s32)r3 == 0) goto L_80072248;
    goto L_80072278;
L_80072248:
    r3 = r29 + r23;
    *(u32*)((u8*)r3 + 0x10) = tmp;
    tmp = *(u32*)(r25 + r24);
    if ((s32)tmp == 0) goto L_80072268;
    r3 = 0x3e8;
    goto L_80072278;
L_80072268:
    r23 = r23 + 0x4;
L_8007226C:
    if ((s32)r23 < (s32)r22) goto L_80072188;
    r3 = 0x0;
L_80072278:
    if ((s32)r3 != 0) {
        r3 = r3 + 0x12;
        return;
    }
    r3 = 0x0;

    return;
}

/* 0x800722A0 | size: 0x2A8 */
s32 fn_800722A0(void) {
    u8 sp[0x40];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r30 = r3;
    r31 = r30 + 0x1;
    r4 = 0x2;
    r3 = r31;
    ((void(*)(void))fn_8008ABE4)();
    r3 = r30;
    ((void(*)(void))fn_80073C38)();
    if ((s32)r3 != 0) {
        r27 = r3;
        goto L_80072524;
    }
    tmp = 0x44;
    r3 = r30;
    *(u32*)(sp + 0xC) = tmp;
    r4 = (u32)sp + 0xc;
    r5 = (u32)sp + 0xa;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == 0) goto L_80072304;
    r27 = 0xb;
    goto L_800723F8;
L_80072304:
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r26 = tmp * 0x64;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r30 << 3;
    r29 = r3;
    tmp = (u32)&lbl_803B6E18;
    r28 = r30 << 2;
    r24 = tmp + r6;
    r27 = (u32)&lbl_803B6E08;
    r25 = r24 + 0x4;
L_8007234C:
    OSGetTick();
    tmp = r3 - r29;
    if (tmp <= r26) goto L_80072364;
    r3 = 0x1;
    goto L_800723E4;
L_80072364:
    r3 = r30;
    r4 = (u32)sp + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80072380;
    r3 = 0x2;
    goto L_800723E4;
L_80072380:
    tmp = *(u8*)(sp + 0x8);
    tmp = tmp & 0xa;
    if ((s32)tmp == 8) goto L_800723C0;
    r12 = *(u32*)((u8*)r24 + 0x0);
    if (r12 != 0) {
        r3 = r30;
        r4 = *(u32*)((u8*)r25 + 0x0);
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    tmp = *(u32*)(r27 + r28);
    if ((s32)tmp == 0) goto L_8007234C;
    r3 = 0x3e8;
    goto L_800723E4;
L_800723C0:
    r3 = r30;
    r4 = (u32)sp + 0x10;
    r5 = (u32)sp + 0xa;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_800723E0;
    r3 = 0x3;
    goto L_800723E4;
L_800723E0:
    r3 = 0x0;
L_800723E4:
    if ((s32)r3 == 0) goto L_800723F4;
    r27 = r3 + 0xb;
    goto L_800723F8;
L_800723F4:
    r27 = 0x0;
L_800723F8:
    if ((s32)r27 != 0) {
        goto L_80072524;
    }
    tmp = (u32)tmp >> 24;
    if (tmp != 0x44) {
        r27 = 0xf;
        goto L_80072524;
    }
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r26 = tmp * 0x7530;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r30 << 3;
    r27 = r3;
    tmp = (u32)&lbl_803B6E18;
    r28 = r30 << 2;
    r24 = tmp + r6;
    r29 = (u32)&lbl_803B6E08;
    r25 = r24 + 0x4;
L_80072464:
    OSGetTick();
    tmp = r3 - r27;
    if (tmp <= r26) goto L_8007247C;
    r3 = 0x1;
    goto L_800724FC;
L_8007247C:
    r3 = r30;
    r4 = (u32)sp + 0x9;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80072498;
    r3 = 0x2;
    goto L_800724FC;
L_80072498:
    tmp = *(u8*)(sp + 0x9);
    tmp = tmp & 0xa;
    if ((s32)tmp == 8) goto L_800724D8;
    r12 = *(u32*)((u8*)r24 + 0x0);
    if (r12 != 0) {
        r3 = r30;
        r4 = *(u32*)((u8*)r25 + 0x0);
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    tmp = *(u32*)(r29 + r28);
    if ((s32)tmp == 0) goto L_80072464;
    r3 = 0x3e8;
    goto L_800724FC;
L_800724D8:
    r3 = r30;
    r4 = (u32)sp + 0x10;
    r5 = (u32)sp + 0xb;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_800724F8;
    r3 = 0x3;
    goto L_800724FC;
L_800724F8:
    r3 = 0x0;
L_800724FC:
    if ((s32)r3 != 0) {
        r27 = r3 + 0xf;

    } else if (tmp != 0) {
        r27 = 0x13;

    } else {
        r27 = 0x0;
    }
L_80072524:
    r3 = r31;
    r4 = 0x1;
    ((void(*)(void))fn_8008ABE4)();
    r3 = r27;
    return;
}

/* 0x80072548 | size: 0x13C */
void fn_80072548(void) {
    extern void fn_80072684();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;

    r31 = r3;
    ((void(*)(void))fn_800A13F8)();
    r4 = (u32)&lbl_803B6DE0;
    *(u32*)&lbl_8047A600 = r3;
    r3 = (u32)&lbl_803B6DE0;
    OSCreateAlarm();
    OSDisableInterrupts();
    r4 = 0x80000000;
    r5 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r4 = (u32)fn_80072684;
    r5 = r5 + 0x4dd3;
    r6 = (u32)&lbl_803B6DE0;
    tmp = (u32)tmp >> 2;
    r7 = (u32)fn_80072684;
    tmp = (u32)((u64)r5 * (u64)tmp >> 32);
    r5 = (u32)&lbl_803B6DE0;
    r30 = r3;
    r3 = r5;
    r5 = 0x0;
    r6 = (u32)tmp >> 6;
    OSSetAlarm();
    r3 = *(u32*)&lbl_8047A600;
    ((void(*)(void))fn_800A221C)();
    r3 = r30;
    OSRestoreInterrupts();
    r3 = r31;
    ((void(*)(void))fn_800D0F44)();
    /* subis tmp, r3, 0x4 */;
    if (tmp != 0) {
        r30 = 0x1;
        goto L_8007264C;
    }
    r3 = r31;
    r4 = (u32)sp + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 != 0) {
        r30 = 0x2;
        goto L_8007264C;
    }
    tmp = *(u8*)(sp + 0x8);
    tmp = tmp & 0x00000008;
    if ((s32)tmp == 0) {
        r30 = -0x1;
        goto L_8007264C;
    }
    r3 = r31;
    r4 = (u32)sp + 0xc;
    r5 = (u32)sp + 0x8;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 != 0) {
        r30 = 0x3;

    } else if (tmp != 0) {
        r30 = 0x4;

    } else {
        r30 = 0x0;
    }
L_8007264C:
    if ((s32)r30 != 0) {
        if ((s32)r30 < 3) { r3 = r30; return; }
    }
    r3 = r31 + 0x1;
    r4 = 0x1;
    ((void(*)(void))fn_8008ABE4)();

    r3 = r30;
    return;
}

/* 0x80072684 | size: 0x24 */
void fn_80072684(void) {
    fn_800A1F94();
}

/* 0x800726A8 | size: 0x358 */
s32 fn_800726A8(void) {
    u8 sp[0x60];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r14 = 0;
    u32 r15 = 0;
    u32 r16 = 0;
    u32 r17 = 0;
    u32 r18 = 0;
    u32 r19 = 0;
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
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r22 = r3;
    r23 = r4;
    r14 = r22 + 0x1;
    r4 = 0x2;
    r3 = r14;
    ((void(*)(void))fn_8008ABE4)();
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r25 = tmp * 0x64;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r22 << 3;
    r28 = r3;
    tmp = (u32)&lbl_803B6E18;
    r29 = r22 << 2;
    r26 = tmp + r6;
    r30 = (u32)&lbl_803B6E08;
    r27 = r26 + 0x4;
    r3 = 0x10620000;
    r15 = 0x80000000;
    r31 = r3 + 0x4dd3;
L_80072724:
    OSGetTick();
    tmp = *(u32*)((u8*)r15 + 0xF8);
    r4 = r3 - r28;
    r3 = r4 ^ r25;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r31 * (u64)tmp >> 32);
    r3 = __cntlzw(r3);
    r3 = r4 << r3;
    r24 = (u32)r3 >> 31;
    tmp = (u32)tmp >> 6;
    r17 = tmp * 0x64;
    OSGetTick();
    r16 = r3;
L_80072758:
    OSGetTick();
    tmp = r3 - r16;
    if (tmp <= r17) goto L_80072770;
    r16 = 0x1;
    goto L_800729BC;
L_80072770:
    r3 = r22;
    ((void(*)(void))fn_800D0F44)();
    /* subis tmp, r3, 0x4 */;
    if (tmp == 0) goto L_800727B4;
    r12 = *(u32*)((u8*)r26 + 0x0);
    if (r12 != 0) {
        r3 = r22;
        r4 = *(u32*)((u8*)r27 + 0x0);
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    tmp = *(u32*)(r30 + r29);
    if ((s32)tmp == 0) goto L_80072758;
    r16 = 0x3e8;
    goto L_800729BC;
L_800727B4:
    r3 = r22;
    ((void(*)(void))fn_80073C38)();
    if ((s32)r3 == 0) goto L_800727CC;
    r16 = r3;
    goto L_800729BC;
L_800727CC:
    tmp = 0x55;
    r3 = r22;
    *(u32*)(sp + 0x10) = tmp;
    r4 = (u32)sp + 0x10;
    r5 = (u32)sp + 0xa;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == 0) goto L_800727F4;
    r16 = 0xb;
    goto L_800728C8;
L_800727F4:
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r17 = tmp * 0x64;
    OSGetTick();
    r16 = r3;
L_8007281C:
    OSGetTick();
    tmp = r3 - r16;
    if (tmp <= r17) goto L_80072834;
    r3 = 0x1;
    goto L_800728B4;
L_80072834:
    r3 = r22;
    r4 = (u32)sp + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80072850;
    r3 = 0x2;
    goto L_800728B4;
L_80072850:
    tmp = *(u8*)(sp + 0x8);
    tmp = tmp & 0xa;
    if ((s32)tmp == 8) goto L_80072890;
    r12 = *(u32*)((u8*)r26 + 0x0);
    if (r12 != 0) {
        r3 = r22;
        r4 = *(u32*)((u8*)r27 + 0x0);
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    tmp = *(u32*)(r30 + r29);
    if ((s32)tmp == 0) goto L_8007281C;
    r3 = 0x3e8;
    goto L_800728B4;
L_80072890:
    r3 = r22;
    r4 = (u32)sp + 0x14;
    r5 = (u32)sp + 0xa;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_800728B0;
    r3 = 0x3;
    goto L_800728B4;
L_800728B0:
    r3 = 0x0;
L_800728B4:
    if ((s32)r3 == 0) goto L_800728C4;
    r16 = r3 + 0xb;
    goto L_800728C8;
L_800728C4:
    r16 = 0x0;
L_800728C8:
    if ((s32)r16 == 0) goto L_800728D4;
    goto L_800729BC;
L_800728D4:
    tmp = (u32)tmp >> 24;
    if (tmp == 0x55) goto L_800728EC;
    r16 = 0xf;
    goto L_800729BC;
L_800728EC:
    r16 = r23;
    r18 = 0x0;
    r3 = 0x10620000;
    r20 = 0x80000000;
    r19 = r3 + 0x4dd3;
L_80072900:
    tmp = *(u32*)((u8*)r16 + 0x0);
    r3 = r22;
    r4 = (u32)sp + 0xc;
    r5 = (u32)sp + 0x9;
    *(u32*)(sp + 0xC) = tmp;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 != 0) goto L_800729B8;
    tmp = *(u32*)((u8*)r20 + 0xF8);
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r19 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r17 = tmp * 0x64;
    OSGetTick();
    r21 = r3;
L_8007293C:
    OSGetTick();
    tmp = r3 - r21;
    if (tmp > r17) goto L_800729B8;
    r3 = r22;
    r4 = (u32)sp + 0x9;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 != 0) goto L_800729B8;
    tmp = *(u8*)(sp + 0x9);
    tmp = tmp & 0x00000002;
    if ((s32)tmp == 0) goto L_8007299C;
    r12 = *(u32*)((u8*)r26 + 0x0);
    if (r12 != 0) {
        r3 = r22;
        r4 = *(u32*)((u8*)r27 + 0x0);
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    tmp = *(u32*)(r30 + r29);
    if ((s32)tmp == 0) goto L_8007293C;
    goto L_800729B8;
L_8007299C:
    tmp = *(u32*)(r30 + r29);
    if ((s32)tmp != 0) goto L_800729B8;
    r16 = r16 + 0x4;
    r18 = r18 + 0x4;
    if ((s32)r18 < 0x78) goto L_80072900;
L_800729B8:
    r16 = 0x0;
L_800729BC:
    if ((s32)r16 != 1) goto L_800729CC;
    if ((s32)r24 == 0) goto L_80072724;
L_800729CC:
    r3 = r14;
    if ((s32)r16 != 0) {
        r4 = 0x1;
    } else {

        r4 = 0x3;
    }
    ((void(*)(void))fn_8008ABE4)();
    r3 = r16;
    return;
}

