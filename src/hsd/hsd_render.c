
/* Forward declarations for converted functions */
void fn_80197650(void);

/**
 * @file hsd_render.c
 * @brief HSD internal functions (0x80197344-0x80197A64).
 *
 * Stub coverage for 7 functions.
 */

#include "dolphin/types.h"
#include "hsd/hsd_debug.h"

/* 0x80197344 | 0xBC */
void fn_80197344(void) {
    extern u32 lbl_8047B240;
    extern void fn_80197784();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* mr. r31, r3 */;
    if ((s32)tmp == 0) return;
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x4020;
    tmp = __cntlzw(tmp);
    /* srwi. tmp, tmp, 5 */;
    if ((s32)tmp != 0) {
        r3 = r31;
        fn_80197784();
        return;
    }
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x00000020;
    if ((s32)tmp == 0) return;
    tmp = lbl_8047B240;
    if (tmp == 0) return;
    r30 = *(u32*)((u8*)r31 + 0x18);
    while (r30 != 0) {

        tmp = *(u32*)((u8*)r30 + 0x4);
        /* clrrwi. tmp, tmp, 31 */;
        if (tmp != 0) {
            tmp = *(u32*)((u8*)r30 + 0x4);
            r6 = r31;
            r4 = *(u32*)((u8*)r30 + 0x4);
            r3 = 0x0;
            r12 = lbl_8047B240;
            /* extrwi r5, tmp, 24, 2 */;
            r4 = r4 & 0x3F;
            ctr_fn = (void(*)(void))r12;
            ctr_fn();
        }
        tmp = *(u32*)((u8*)r30 + 0x4);
        tmp = tmp & 0x7FFFFFFF;
        *(u32*)((u8*)r30 + 0x4) = tmp;
        r30 = *(u32*)((u8*)r30 + 0x0);

    }

    return;
}

/* 0x80197400 | 0xA8 */
void fn_80197400(void) {
    extern u8 lbl_80465348[];
    extern u32 lbl_80478C64;
    extern u32 lbl_80478C68;
    extern u32 lbl_80478C6C;
    extern u8 lbl_8047B24C[];
    extern u8 lbl_8047B250[];
    extern u32 lbl_8047B254;
    extern u8 lbl_8047B258[];
    extern u32 lbl_8047B25C;
    extern void fn_801A84F0();
    extern void fn_801AA498();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = *(u32*)lbl_8047B24C;
    while (r31 != 0) {

        tmp = *(u32*)((u8*)r31 + 0x30);
        r30 = *(u32*)((u8*)r31 + 0x44);
        if (tmp != 0) {
            r3 = *(u32*)((u8*)r31 + 0x30);
            fn_801A84F0();
        }
        r3 = (u32)lbl_80465348;
        r4 = r31;
        r3 = (u32)lbl_80465348;
        fn_801AA498();
        r31 = r30;

    }
    r9 = 0x0;
    r8 = (u32)lbl_8047B24C;
    r7 = 0x0;
    r6 = (u32)lbl_8047B250;
    r5 = 0x0;
    r4 = 0x0;
    r3 = (u32)lbl_8047B258;
    tmp = 0x0;
    *(u32*)lbl_8047B24C = r9;
    lbl_80478C64 = r8;
    *(u32*)lbl_8047B250 = r7;
    lbl_80478C68 = r6;
    lbl_8047B254 = r5;
    *(u32*)lbl_8047B258 = r4;
    lbl_80478C6C = r3;
    lbl_8047B25C = tmp;
    return;
}

/* 0x801974A8 | 0x154 */
void fn_801974A8(void) {
    extern u8 lbl_80465348[];
    extern u32 lbl_80478C64;
    extern u32 lbl_80478C68;
    extern u32 lbl_80478C6C;
    extern u8 lbl_8047B24C[];
    extern u8 lbl_8047B250[];
    extern u32 lbl_8047B254;
    extern u8 lbl_8047B258[];
    extern u32 lbl_8047B25C;
    extern void fn_801942B8();
    extern void fn_801A84F0();
    extern void fn_801AA498();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    fn_801942B8();
    tmp = r3 + 0x54;
    r30 = *(u32*)lbl_8047B250;
    r31 = tmp;
    while (r30 != 0) {

        tmp = *(u32*)((u8*)r30 + 0x30);
        r3 = *(u32*)((u8*)r30 + 0x34);
        if (tmp != 0) {
            r4 = *(u32*)((u8*)r30 + 0x30);
        } else {

            r4 = r31;
        }
        r8 = *(u32*)((u8*)r30 + 0x34);
        r5 = r30;
        r7 = *(u32*)((u8*)r30 + 0x38);
        r6 = 0x4;
        r8 = *(u32*)((u8*)r8 + 0x0);
        r12 = *(u32*)((u8*)r8 + 0x48);
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
        r30 = *(u32*)((u8*)r30 + 0x3C);

    }
    r30 = *(u32*)lbl_8047B258;
    while (r30 != 0) {

        tmp = *(u32*)((u8*)r30 + 0x30);
        r3 = *(u32*)((u8*)r30 + 0x34);
        if (tmp != 0) {
            r4 = *(u32*)((u8*)r30 + 0x30);
        } else {

            r4 = r31;
        }
        r8 = *(u32*)((u8*)r30 + 0x34);
        r5 = r30;
        r7 = *(u32*)((u8*)r30 + 0x38);
        r6 = 0x2;
        r8 = *(u32*)((u8*)r8 + 0x0);
        r12 = *(u32*)((u8*)r8 + 0x48);
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
        r30 = *(u32*)((u8*)r30 + 0x40);

    }
    r30 = *(u32*)lbl_8047B24C;
    while (r30 != 0) {

        tmp = *(u32*)((u8*)r30 + 0x30);
        r31 = *(u32*)((u8*)r30 + 0x44);
        if (tmp != 0) {
            r3 = *(u32*)((u8*)r30 + 0x30);
            fn_801A84F0();
        }
        r3 = (u32)lbl_80465348;
        r4 = r30;
        r3 = (u32)lbl_80465348;
        fn_801AA498();
        r30 = r31;

    }
    r9 = 0x0;
    r8 = (u32)lbl_8047B24C;
    r7 = 0x0;
    r6 = (u32)lbl_8047B250;
    r5 = 0x0;
    r4 = 0x0;
    r3 = (u32)lbl_8047B258;
    tmp = 0x0;
    *(u32*)lbl_8047B24C = r9;
    lbl_80478C64 = r8;
    *(u32*)lbl_8047B250 = r7;
    lbl_80478C68 = r6;
    lbl_8047B254 = r5;
    *(u32*)lbl_8047B258 = r4;
    lbl_80478C6C = r3;
    lbl_8047B25C = tmp;
    return;
}

/* 0x54 | fn_801975FC | two_call_arg_check */
void fn_801975FC(u32 arg1) {
    if (arg1 == 0) { return; }
    fn_80197650();
    fn_80197650();
}

/* 0x80197650 | 0x134 */
void fn_80197650(void) {
    extern void fn_80197650();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r28 = r5;
    if ((s32)r4 <= 1) {
        if (tmp != 0) {
            tmp = 0x0;
            *(u32*)(r3 + r28) = tmp;
        }
        return;
    }
    tmp = (u32)r4 >> 31;
    tmp = tmp + r4;
    r3 = 0x0;
    tmp = (s32)tmp >> 1;
    r29 = r4 - tmp;
    while ((s32)r3 < (s32)tmp) {

        r30 = *(u32*)(r30 + r28);
        r3 = r3 + 0x1;

    }
    r4 = tmp;
    r5 = r28;
    fn_80197650();
    tmp = r3;
    r3 = r30;
    r31 = tmp;
    r4 = r29;
    r5 = r28;
    fn_80197650();
    tmp = 0x0;
    r30 = r3;
    *(u32*)(sp + 0x8) = tmp;
    r3 = (u32)sp + 0x8;
    while (r31 != 0 && r30 != 0) {
        f1 = *(f32*)((u8*)r31 + 0x2C);
        f0 = *(f32*)((u8*)r30 + 0x2C);
        /* cror eq, lt, eq */;
        if (f1 == f0) {
            *(u32*)((u8*)r3 + 0x0) = r31;
            r31 = *(u32*)(r31 + r28);
        } else {
            *(u32*)((u8*)r3 + 0x0) = r30;
            r30 = *(u32*)(r30 + r28);
        }
        tmp = *(u32*)((u8*)r3 + 0x0);
        r3 = tmp + r28;
    }
    if (r31 != 0) {
        *(u32*)((u8*)r3 + 0x0) = r31;
        return;
    }
    if (r30 == 0) return;
    *(u32*)((u8*)r3 + 0x0) = r30;


    return;
}

/* 0x80197784 | 0x214 */
void fn_80197784(void) {
    extern u8 lbl_80465348[];
    extern u32 lbl_80478C64;
    extern u32 lbl_80478C68;
    extern u32 lbl_80478C6C;
    extern u32 lbl_8047B244;
    extern u32 lbl_8047B254;
    extern u32 lbl_8047B25C;
    extern u8 lbl_8047D9E0[];
    extern u8 lbl_8047D9E8[];
    extern void fn_800A2D64();
    extern void fn_801942B8();
    extern void fn_80196E10();
    extern void fn_8019D9DC();
    extern void fn_801A8524();
    extern void fn_801AA4CC();
    u8 sp[0x50];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r12 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r28 = r3;
    r29 = r4;
    r30 = r6;
    tmp = *(u32*)((u8*)r3 + 0x14);
    tmp = tmp & 0x00000010;
    if ((s32)tmp != 0) return;
    r3 = *(u32*)((u8*)r28 + 0x14);
    tmp = r5 << 18;
    /* and. r31, r3, tmp */;
    if ((s32)tmp == 0) return;
    if (r28 != 0) {
        if (r28 == 0) {
            r3 = (u32)lbl_8047D9E0;
            r4 = 0x25d;
            r5 = (u32)lbl_8047D9E8;
            fn_80196E10();
        }
        tmp = *(u32*)((u8*)r28 + 0x14);
        r3 = 0x0;
        tmp = tmp & 0x00800000;
        if (r28 == 0) {
            tmp = *(u32*)((u8*)r28 + 0x14);
            tmp = tmp & 0x00000040;
            if (r28 != 0) {
                r3 = 0x1;
        }
        }
        if ((s32)r3 != 0) {
            r3 = r28;
            fn_8019D9DC();
    }
    }
    if (r29 == 0) {
        fn_801942B8();
        tmp = r3 + 0x54;
        r29 = tmp;
    }
    r6 = *(u32*)((u8*)r28 + 0x0);
    r3 = r28;
    r4 = r29;
    r5 = (u32)sp + 0x8;
    r12 = *(u32*)((u8*)r6 + 0x44);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    tmp = r31 & 0x00040000;
    if (r29 != 0) {
        r5 = *(u32*)((u8*)r28 + 0x0);
        r3 = r28;
        r4 = r29;
        r7 = r30;
        r12 = *(u32*)((u8*)r5 + 0x48);
        r5 = (u32)sp + 0x8;
        r6 = 0x1;
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    tmp = lbl_8047B244;
    if ((s32)tmp == 0) {
        tmp = r31 & 0x00100000;
        if ((s32)tmp != 0) {
            r5 = *(u32*)((u8*)r28 + 0x0);
            r3 = r28;
            r4 = r29;
            r7 = r30;
            r12 = *(u32*)((u8*)r5 + 0x48);
            r5 = (u32)sp + 0x8;
            r6 = 0x4;
            ctr_fn = (void(*)(void))r12;
            ctr_fn();
        }
        tmp = r31 & 0x00080000;
        if ((s32)tmp == 0) return;
        r5 = *(u32*)((u8*)r28 + 0x0);
        r3 = r28;
        r4 = r29;
        r7 = r30;
        r12 = *(u32*)((u8*)r5 + 0x48);
        r5 = (u32)sp + 0x8;
        r6 = 0x2;
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
        return;
    }
    tmp = r31 & 0x00180000;
    if ((s32)tmp == 0) return;
    r3 = (u32)lbl_80465348;
    r3 = (u32)lbl_80465348;
    fn_801AA4CC();
    r27 = r3;
    r4 = 0x0;
    r5 = 0x18;
    r3 = r27 + 0x30;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = (u32)sp + 0x8;
    r4 = r27;
    fn_800A2D64();
    if (r29 != 0) {
        fn_801A8524();
        tmp = r3;
        r3 = r29;
        *(u32*)((u8*)r27 + 0x30) = tmp;
        r4 = *(u32*)((u8*)r27 + 0x30);
        fn_800A2D64();
    }
    *(u32*)((u8*)r27 + 0x34) = r28;
    tmp = r31 & 0x00100000;
    tmp = r27 + 0x44;
    *(u32*)((u8*)r27 + 0x38) = r30;
    r3 = lbl_80478C64;
    *(u32*)((u8*)r3 + 0x0) = r27;
    lbl_80478C64 = tmp;
    if (r29 != 0) {
        r3 = lbl_80478C68;
        tmp = r27 + 0x3c;
        *(u32*)((u8*)r3 + 0x0) = r27;
        r3 = lbl_8047B254;
        lbl_80478C68 = tmp;
        tmp = r3 + 0x1;
        lbl_8047B254 = tmp;
    }
    tmp = r31 & 0x00080000;
    if (r29 == 0) return;
    r3 = lbl_80478C6C;
    tmp = r27 + 0x40;
    *(u32*)((u8*)r3 + 0x0) = r27;
    r3 = lbl_8047B25C;
    lbl_80478C6C = tmp;
    tmp = r3 + 0x1;
    lbl_8047B25C = tmp;

    return;
}

/* 0x80197998 | 0xCC */
void fn_80197998(void) {
    extern void fn_80199704();
    extern void fn_8019F024();
    extern void fn_801A5DCC();
    extern void fn_801AB63C();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r12 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r31 = r3;
    r27 = r4;
    r28 = r5;
    r30 = r6;
    r29 = r7;
    fn_8019F024();
    tmp = r29 & 0x04000000;
    r30 = r30 << 1;
    if ((s32)tmp == 0) {
        tmp = *(u32*)((u8*)r31 + 0x14);
        tmp = tmp & 0x00010000;
        if ((s32)tmp != 0) {
            r3 = r28;
            fn_801A5DCC();
    }
    }
    r3 = 0x0;
    r4 = 0x0;
    fn_801AB63C();
    r31 = *(u32*)((u8*)r31 + 0x18);
    while (r31 != 0) {

        tmp = *(u32*)((u8*)r31 + 0x14);
        tmp = tmp & 0x1;
        if ((s32)tmp == 0) {
            tmp = *(u32*)((u8*)r31 + 0x14);
            /* and. tmp, tmp, r30 */;
            if ((s32)tmp != 0) {
                r3 = r31;
                fn_80199704();
                r6 = *(u32*)((u8*)r31 + 0x0);
                r3 = r31;
                r4 = r27;
                r5 = r28;
                r12 = *(u32*)((u8*)r6 + 0x3C);
                r6 = r29;
                ctr_fn = (void(*)(void))r12;
                ctr_fn();
        }
        }
        r31 = *(u32*)((u8*)r31 + 0x4);

    }
    r3 = 0x0;
    fn_80199704();
    r3 = 0x0;
    fn_8019F024();
    return;
}

