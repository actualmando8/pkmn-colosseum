/**
 * @file gs_range_8007FD64.c
 * @brief gs-engine code, 0x8007FD64 - 0x80088428 (28 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

/* ===== External function declarations (fn_80084A8C only) ===== */
extern void fn_8005CF2C();
extern void fn_800776E4();
extern void fn_8008ABA0();
extern void fn_80092E38();
extern void fn_80092FC8();
extern void fn_80093160();
extern void fn_800932F0();
extern void fn_800934E4();
extern void fn_80093610();
extern void fn_80093698();
extern void fn_800D3088();
extern void fn_800D37CC();
extern void menuCloseCustom();
extern void menuIsCheck();
extern void menuGetEnablePort();
extern void menuSetEnablePort();
extern void windowGetFreeWork();
extern void windowGetKeyInfo();
extern void winMsgOpen();
extern void winMsgClose();
extern void menuOpen();
extern void windowSearchID();
extern void _threadSwitch();

/* ===== SDA globals (fn_80084A8C only) ===== */
extern u8 lbl_80478950;
extern u8 lbl_80478954;
extern u8 lbl_8047C1A0;
extern u8 lbl_8047C1A8;
extern u8 lbl_8047C1AC;
extern u8 lbl_8047C1B0;
extern u8 lbl_8047C1B8;

/* ===== Rodata / data labels ===== */
extern u8 jumptable_802EEB78[];
extern u8 lbl_8026F2E8[];

/* 0x8007FD64 | size: 0x58
 * menuCardE_CompareEntryPtrs: qsort-style comparator for MenuCardEEntry*
 * elements.
 */
s32 menuCardE_CompareEntryPtrs(u32 r3, u32 r4) {
    u32 r0;
    u32 r5;

    r5 = *(u32*)((u8*)r3 + 0x0);
    r4 = *(u32*)((u8*)r4 + 0x0);
    r3 = *(u8*)((u8*)r5 + 0x1C);
    r0 = *(u8*)((u8*)r4 + 0x1C);
    r3 = (s8)r3;
    r0 = (s8)r0;
    if ((s32)r3 < (s32)r0) {
        return 0x1;
    }
    if ((s32)r3 > (s32)r0) {
        return -0x1;
    }
    r3 = *(u8*)((u8*)r5 + 0x1A);
    r0 = *(u8*)((u8*)r4 + 0x1A);
    if (r3 < r0) {
        return -0x1;
    }
    r0 = r0 - r3;
    r3 = (u32)r0 >> 31;
    return r3;
}

/* 0x80084034 | size: 0x4 */
void fn_80084034(void) {
}

/* 0x80084A8C | size: 0x305C */
void fn_80084A8C(void) {
    extern void fn_80087AE8();
    extern void fn_80128E04();
    extern void fn_80128E24();
    extern void savedataGetStatus();
    extern void heroInit();
    extern void heroBiosCopy();
    extern void msgctrlSetValue();
    extern void gamedataAttestBiosGetLangareaId();
    extern void gamedataBiosGetGamedataAtttestPtr();
    extern void fn_80166A28();
    extern void __assert();
    extern u8 jumptable_802EEB78[];
    u8 sp[0xBF0];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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
    f32 f0 = 0.0f;
    f32 f4 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r3 = (u32)&lbl_8026F2E8;
    r15 = 0x0;
    tmp = (u32)&lbl_8026F2E8;
    *(u32*)(sp + 0xC34) = tmp;
    r3 = 0x1;
    ((void(*)(void))fn_80093698)();
    while (1) {
        r5 = 0x0;
        r4 = r3 + 0x190;
        r3 = 0x1;
        ((void(*)(void))fn_800932F0)();
        if ((s32)r3 != 0) break;
        ((void(*)(void))_threadSwitch)();


    }
    r3 = 0xe4;
    r4 = 0x0;
    ((void(*)(void))menuOpen)();
    r3 = 0xe4;
    ((void(*)(void))windowSearchID)();
    r16 = r3;
    if (r16 == 0) {
        r4 = 0x1f4;
        r5 = (u32)&lbl_8047C1A0;
        r3 = r3 + 0x184;
        __assert();
    }
    if (r16 == 0) {
        r3 = 0xa6;
        ((void(*)(void))windowSearchID)();
        r16 = r3;
    }
    r3 = r16;
    ((void(*)(void))windowGetFreeWork)();
    r5 = 0x0;
    r16 = *(u32*)((u8*)r3 + 0x0);
    r30 = (u32)&lbl_80478954;
    r4 = tmp << 4;
    *(u32*)((u8*)r16 + 0x24) = tmp;
    r29 = r3 + 0x30;
    r29 = r29 + r4;
    *(u32*)((u8*)r16 + 0x2C) = tmp;
    tmp = *(u8*)&lbl_80478954;
    *(u8*)((u8*)r16 + 0x21) = tmp;
    tmp = 0x5;
    r3 = *(u8*)((u8*)r30 + 0x0);
    r3 = (s8)r3;
    r5 = r3 << 2;
    r3 = *(u32*)(r29 + r5);
    *(u32*)(r16 + r5) = r3;
    if ((s32)r5 < 0) {
        r3 = *(u32*)(r16 + r5);
        if ((s32)r3 == 1) {
            *(u32*)(r16 + r5) = tmp;
    }
    }
    r4 = r30 + 0x1;
    r5 = 0x1;
    r3 = *(u8*)((u8*)r4 + 0x0);
    r3 = (s8)r3;
    r5 = r3 << 2;
    r3 = *(u32*)(r29 + r5);
    *(u32*)(r16 + r5) = r3;
    if ((s32)r5 < 0) {
        r3 = *(u32*)(r16 + r5);
        if ((s32)r3 == 1) {
            *(u32*)(r16 + r5) = tmp;
    }
    }
    r4 = r4 + 0x1;
    r5 = 0x2;
    r3 = *(u8*)((u8*)r4 + 0x0);
    r3 = (s8)r3;
    r5 = r3 << 2;
    r3 = *(u32*)(r29 + r5);
    *(u32*)(r16 + r5) = r3;
    if ((s32)r5 < 0) {
        r3 = *(u32*)(r16 + r5);
        if ((s32)r3 == 1) {
            *(u32*)(r16 + r5) = tmp;
    }
    }
    r4 = r4 + 0x1;
    r5 = 0x3;
    r3 = *(u8*)((u8*)r4 + 0x0);
    r3 = (s8)r3;
    r5 = r3 << 2;
    r3 = *(u32*)(r29 + r5);
    *(u32*)(r16 + r5) = r3;
    if ((s32)r5 < 0) {
        r3 = *(u32*)(r16 + r5);
        if ((s32)r3 == 1) {
            *(u32*)(r16 + r5) = tmp;
    }
    }
    r24 = r16;
    tmp = tmp & 0x00000010;
    if (tmp != 0) {
        if ((s32)tmp != 0) {
        }
        if ((s32)tmp == 2) {
            }
        r3 = 0x0;
        r4 = 0x2;
        savedataGetStatus();
        r4 = 0x0;
        *(u8*)((u8*)r16 + 0x21) = r4;
        r4 = 0x8;
        tmp = tmp & 0x00000002;
        r18 = r3;
        *(u32*)((u8*)r16 + 0x0) = r4;
        if (tmp != 0) {
            r3 = 0x2f;
            r4 = 0x1;
            msgctrlSetValue();
            r3 = 0x7;
            r4 = 0x3d88;
            r5 = 0x0;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
            f27 = *(f32*)&lbl_8047C1A8;
            f31 = *(f64*)&lbl_8047C1B0;
            r17 = 0x43300000;
            f29 = *(f64*)&lbl_8047C1B8;
            f28 = *(f32*)&lbl_8047C1AC;
            while (f27 < f28) {

                ((void(*)(void))_threadSwitch)();
                ((void(*)(void))fn_800D37CC)();
                *(u32*)(sp + 0xC14) = tmp;
                f30 = f0 - f31;
                ((void(*)(void))fn_800D3088)();
                f0 = f0 - f29;
                f0 = f0 / f30;
                f27 = f27 + f0;

            }
            r3 = r18;
            ((void(*)(void))fn_800776E4)();
            tmp = r3 & 0xFF;
            if (tmp == 0) {
                r3 = 0xe4;
                r4 = 0x0;
                r5 = 0x1;
                ((void(*)(void))menuCloseCustom)();
                r3 = r18;
                r4 = 0x0;
                ((void(*)(void))fn_8005CF2C)();
                r3 = 0x0;
                return;
        }
        }
        if (tmp != 0) {
            r3 = tmp;
            r3 = *(u32*)((u8*)r3 + 0x0);
            if (r3 != 0) {
                r4 = r18;
                heroBiosCopy();
        }
        }
        tmp = 0xa;
        *(u32*)((u8*)r16 + 0x0) = tmp;
        }
    r25 = 0x0;
    tmp = tmp & 0x00000040;
    *(u32*)(sp + 0xC2C) = tmp;
    tmp = tmp & 0x1;
    *(u32*)(sp + 0xC28) = tmp;
    tmp = tmp & 0x00000002;
    *(u32*)(sp + 0xC24) = tmp;
    tmp = tmp & 0x00000008;
    *(u32*)(sp + 0xC20) = tmp;
    r31 = tmp & 0x00000020;
    do {
        r23 = *(u8*)((u8*)r3 + 0x0);
        *(u8*)((u8*)r24 + 0x21) = r23;
        if (tmp != 0) {
            tmp = (s8)r23;
            tmp = tmp << 2;
            tmp = *(u32*)(r3 + tmp);
            if (tmp != 0) {
                r22 = tmp;
                goto L_80084E14;
            }
        }
        r22 = (u32)sp + 0xf4;
    L_80084E14:
        if (tmp != 0) {
            tmp = (s8)r23;
            if ((s32)tmp == 1) {
                goto L_80084E38;
            }
        }
        r21 = (u32)sp + 0x1c;
    L_80084E38:
        r28 = (s8)r23;
        r26 = r28 + 0x1;
        r27 = r28 << 2;
    L_80084E44:
        ((void(*)(void))menuGetEnablePort)();
        r4 = (u32)&lbl_80478950;
        tmp = *(u8*)(r4 + r28);
        tmp = r3 & ~tmp;
        r3 = tmp & 0xFF;
        ((void(*)(void))menuSetEnablePort)();
        tmp = *(u32*)((u8*)r24 + 0x28);
        if (tmp != 4) {
            tmp = 0x2;
            r4 = r26;
            *(u32*)(r27 + r24) = tmp;
            r3 = 0x2f;
            msgctrlSetValue();
            r3 = 0x7;
            r4 = 0x3c42;
            r5 = 0x0;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
            r3 = r24;
            r4 = 0x6;
            fn_80087AE8();
            tmp = r3 & 0xFF;
            if (tmp == 0) {
                r3 = 0x1;
                ((void(*)(void))menuSetEnablePort)();
                r15 = 0x0;
            L_80084EB4:
                r16 = 0x0;
                r18 = r24;
                r17 = r16;
                r19 = (u32)&lbl_80478950;
                do {
                    tmp = *(u32*)((u8*)r18 + 0x0);
                    if ((s32)tmp != 5) {
                        if ((s32)tmp == 4) {
                        }
                        r3 = r17 + 0x1;
                        ((void(*)(void))fn_8008ABA0)();
                        tmp = r3 & 0xFF;
                        if (tmp == 0) {
                            ((void(*)(void))menuGetEnablePort)();
                            tmp = *(u8*)((u8*)r19 + 0x0);
                            tmp = r3 & ~tmp;
                            r3 = tmp & 0xFF;
                            ((void(*)(void))menuSetEnablePort)();
                            r3 = 0x7;
                            tmp = 0x8;
                            *(u32*)((u8*)r18 + 0x0) = r3;
                            *(u32*)((u8*)r24 + 0x28) = tmp;
                    }
                        }
                    tmp = *(u32*)((u8*)r18 + 0x0);
                    if ((s32)tmp == 7) {
                        r16 = 0x1;
                    }
                    r18 = r18 + 0x4;
                    r19 = r19 + 0x1;
                    r17 = r17 + 0x1;
                } while ((s32)r17 <= 3);
                tmp = r16 & 0xFF;
                if (tmp == 0) {
                    ((void(*)(void))_threadSwitch)();
                    r15 = r15 + 0x1;
                    if ((s32)r15 < 0xf) goto L_80084EB4;
                }
                r3 = 0x26;
                fn_80166A28();
                r15 = 0x0;
                tmp = *(u32*)((u8*)r24 + 0x0);
                if ((s32)tmp != 7) {
                    r15 = 0x1;
                    tmp = *(u32*)((u8*)r24 + 0x4);
                    if ((s32)tmp != 7) {
                        r15 = 0x2;
                        tmp = *(u32*)((u8*)r24 + 0x8);
                        if ((s32)tmp != 7) {
                            r15 = 0x3;
                            tmp = *(u32*)((u8*)r24 + 0xC);
                            if ((s32)tmp != 7) {
                                r15 = 0x4;
                }
                }
                }
                }
                if ((s32)r15 > 3) {
                    tmp = *(u32*)((u8*)r24 + 0x28);
                    tmp = tmp & 0x00000008;
                    if (tmp == 0) {
                        r3 = 0x0;
                        goto L_80085010;
                }
                }
                r3 = 0x1;
                ((void(*)(void))menuSetEnablePort)();
                r4 = r15 + 0x1;
                r3 = 0x2f;
                msgctrlSetValue();
                if ((s32)r15 == 0) {
                    r3 = 0x7;
                    r4 = 0x44c0;
                    r5 = 0x1;
                    r6 = 0x0;
                    ((void(*)(void))winMsgOpen)();
                } else {

                    r3 = 0x7;
                    r4 = 0x44b8;
                    r5 = 0x1;
                    r6 = 0x0;
                    ((void(*)(void))winMsgOpen)();
                }
                tmp = 0x8;
                r3 = 0x1;
                *(u32*)((u8*)r24 + 0x28) = tmp;
            L_80085010:
                tmp = r3 & 0xFF;
                if (tmp == 0) {
                    tmp = *(u8*)((u8*)r24 + 0x21);
                    r3 = 0x6;
                    tmp = (s8)tmp;
                    tmp = tmp << 2;
                    *(u32*)(r24 + tmp) = r3;
                }
                tmp = *(u32*)((u8*)r24 + 0x2C);
                if ((s32)tmp == 3) {
                    r3 = 0x7;
                    r4 = 0x44e7;
                    r5 = 0x1;
                    r6 = 0x0;
                    ((void(*)(void))winMsgOpen)();
                } else {

                    r3 = 0x7;
                    r4 = 0x44e6;
                    r5 = 0x1;
                    r6 = 0x0;
                    ((void(*)(void))winMsgOpen)();
                }
                r3 = *(u8*)((u8*)r24 + 0x21);
                r3 = (s8)r3;
                ((void(*)(void))fn_80093698)();
                r3 = 0x0;
                return;
            }
        }
        r4 = r26;
        r3 = 0x2f;
        msgctrlSetValue();
        r3 = 0x7;
        r4 = 0x3c43;
        r5 = 0x0;
        r6 = 0x0;
        ((void(*)(void))winMsgOpen)();
        tmp = r15 & 0xFF;
        r3 = 0x3;
        *(u32*)(r27 + r24) = r3;
        if (tmp == 0) {
            tmp = 0x0;
            *(u32*)((u8*)r24 + 0x28) = tmp;
            goto L_80085114;
        L_800850BC:
            r3 = 0x10c;
            ((void(*)(void))menuIsCheck)();
            tmp = r3 & 0xFF;
            if (tmp != 0) {
                ((void(*)(void))_threadSwitch)();
                goto L_80085114;
            }
            ((void(*)(void))windowGetKeyInfo)();
            tmp = *(u16*)((u8*)r3 + 0x4);
            tmp = tmp & 0x00000020;
            if ((s32)tmp != 0) {
                tmp = 0x2;
                r3 = 0xe;
                *(u32*)((u8*)r24 + 0x28) = tmp;
                goto L_8008513C;
            }
            tmp = *(u32*)((u8*)r24 + 0x28);
            if (tmp == 8) {
                r3 = 0xe;
                goto L_8008513C;
            }
            ((void(*)(void))_threadSwitch)();
        L_80085114:
            r3 = *(u8*)((u8*)r24 + 0x21);
            r3 = (s8)r3;
            ((void(*)(void))fn_800934E4)();
            if ((s32)r3 == 0) goto L_800850BC;
            tmp = 0x0;
            *(u32*)((u8*)r24 + 0x28) = tmp;
            r3 = *(u8*)((u8*)r24 + 0x21);
            r3 = (s8)r3;
            ((void(*)(void))fn_80093610)();
        L_8008513C:
            if ((s32)r3 == 0xe) {
                r3 = 0x1;
                ((void(*)(void))menuSetEnablePort)();
                r15 = 0x0;
            L_80085150:
                r16 = 0x0;
                r18 = r24;
                r17 = r16;
                r19 = (u32)&lbl_80478950;
                do {
                    tmp = *(u32*)((u8*)r18 + 0x0);
                    if ((s32)tmp != 5) {
                        if ((s32)tmp == 4) {
                        }
                        r3 = r17 + 0x1;
                        ((void(*)(void))fn_8008ABA0)();
                        tmp = r3 & 0xFF;
                        if (tmp == 0) {
                            ((void(*)(void))menuGetEnablePort)();
                            tmp = *(u8*)((u8*)r19 + 0x0);
                            tmp = r3 & ~tmp;
                            r3 = tmp & 0xFF;
                            ((void(*)(void))menuSetEnablePort)();
                            r3 = 0x7;
                            tmp = 0x8;
                            *(u32*)((u8*)r18 + 0x0) = r3;
                            *(u32*)((u8*)r24 + 0x28) = tmp;
                        }
                        }
                    tmp = *(u32*)((u8*)r18 + 0x0);
                    if ((s32)tmp == 7) {
                        r16 = 0x1;
                    }
                    r18 = r18 + 0x4;
                    r19 = r19 + 0x1;
                    r17 = r17 + 0x1;
                } while ((s32)r17 <= 3);
                tmp = r16 & 0xFF;
                if (tmp == 0) {
                    ((void(*)(void))_threadSwitch)();
                    r15 = r15 + 0x1;
                    if ((s32)r15 < 0xf) goto L_80085150;
                }
                r3 = 0x26;
                fn_80166A28();
                r15 = 0x0;
                tmp = *(u32*)((u8*)r24 + 0x0);
                if ((s32)tmp != 7) {
                    r15 = 0x1;
                    tmp = *(u32*)((u8*)r24 + 0x4);
                    if ((s32)tmp != 7) {
                        r15 = 0x2;
                        tmp = *(u32*)((u8*)r24 + 0x8);
                        if ((s32)tmp != 7) {
                            r15 = 0x3;
                            tmp = *(u32*)((u8*)r24 + 0xC);
                            if ((s32)tmp != 7) {
                                r15 = 0x4;
                }
                }
                }
                }
                if ((s32)r15 > 3) {
                    tmp = *(u32*)((u8*)r24 + 0x28);
                    tmp = tmp & 0x00000008;
                    if (tmp == 0) {
                        r3 = 0x0;
                        goto L_800852AC;
                    }
                }
                r3 = 0x1;
                ((void(*)(void))menuSetEnablePort)();
                r4 = r15 + 0x1;
                r3 = 0x2f;
                msgctrlSetValue();
                if ((s32)r15 == 0) {
                    r3 = 0x7;
                    r4 = 0x44c0;
                    r5 = 0x1;
                    r6 = 0x0;
                    ((void(*)(void))winMsgOpen)();
                } else {

                    r3 = 0x7;
                    r4 = 0x44b8;
                    r5 = 0x1;
                    r6 = 0x0;
                    ((void(*)(void))winMsgOpen)();
                }
                tmp = 0x8;
                r3 = 0x1;
                *(u32*)((u8*)r24 + 0x28) = tmp;
            L_800852AC:
                tmp = r3 & 0xFF;
                if (tmp == 0) {
                    tmp = *(u8*)((u8*)r24 + 0x21);
                    r3 = 0x6;
                    tmp = (s8)tmp;
                    tmp = tmp << 2;
                    *(u32*)(r24 + tmp) = r3;
                }
                tmp = *(u32*)((u8*)r24 + 0x2C);
                if ((s32)tmp == 3) {
                    r3 = 0x7;
                    r4 = 0x44e7;
                    r5 = 0x1;
                    r6 = 0x0;
                    ((void(*)(void))winMsgOpen)();
                } else {

                    r3 = 0x7;
                    r4 = 0x44e6;
                    r5 = 0x1;
                    r6 = 0x0;
                    ((void(*)(void))winMsgOpen)();
                }
                r3 = *(u8*)((u8*)r24 + 0x21);
                r3 = (s8)r3;
                ((void(*)(void))fn_80093698)();
                r3 = 0x0;
                return;
            }
            r15 = 0x1;
        }
        r3 = r28;
        r4 = 0x0;
        ((void(*)(void))fn_80093160)();
        tmp = 0x0;
        *(u32*)((u8*)r24 + 0x28) = tmp;
        goto L_8008538C;
    L_80085334:
        r3 = 0x10c;
        ((void(*)(void))menuIsCheck)();
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            ((void(*)(void))_threadSwitch)();
            goto L_8008538C;
        }
        ((void(*)(void))windowGetKeyInfo)();
        tmp = *(u16*)((u8*)r3 + 0x4);
        tmp = tmp & 0x00000020;
        if ((s32)tmp != 0) {
            tmp = 0x2;
            r3 = 0xe;
            *(u32*)((u8*)r24 + 0x28) = tmp;
            goto L_800853B4;
        }
        tmp = *(u32*)((u8*)r24 + 0x28);
        if (tmp == 8) {
            r3 = 0xe;
            goto L_800853B4;
        }
        ((void(*)(void))_threadSwitch)();
    L_8008538C:
        r3 = *(u8*)((u8*)r24 + 0x21);
        r3 = (s8)r3;
        ((void(*)(void))fn_800934E4)();
        if ((s32)r3 == 0) goto L_80085334;
        tmp = 0x0;
        *(u32*)((u8*)r24 + 0x28) = tmp;
        r3 = *(u8*)((u8*)r24 + 0x21);
        r3 = (s8)r3;
        ((void(*)(void))fn_80093610)();
    L_800853B4:
        if ((s32)r3 != 0xe) {
        do {
            if ((s32)r3 < 0xe) {
                if ((s32)r3 != 2) {
                    break;
                }
                r4 = 0x20000;
                tmp = r4 + 0x2;
                if ((s32)r3 == (s32)tmp) break;
                break;
            }
            r3 = 0x1;
            ((void(*)(void))menuSetEnablePort)();
            r15 = 0x0;
        L_800853EC:
            r16 = 0x0;
            r18 = r24;
            r17 = r16;
            r19 = (u32)&lbl_80478950;
            do {
                tmp = *(u32*)((u8*)r18 + 0x0);
                if ((s32)tmp != 5) {
                    if ((s32)tmp == 4) {
                    }
                    r3 = r17 + 0x1;
                    ((void(*)(void))fn_8008ABA0)();
                    tmp = r3 & 0xFF;
                    if (tmp == 0) {
                        ((void(*)(void))menuGetEnablePort)();
                        tmp = *(u8*)((u8*)r19 + 0x0);
                        tmp = r3 & ~tmp;
                        r3 = tmp & 0xFF;
                        ((void(*)(void))menuSetEnablePort)();
                        r3 = 0x7;
                        tmp = 0x8;
                        *(u32*)((u8*)r18 + 0x0) = r3;
                        *(u32*)((u8*)r24 + 0x28) = tmp;
                    }
                    }
                tmp = *(u32*)((u8*)r18 + 0x0);
                if ((s32)tmp == 7) {
                    r16 = 0x1;
                }
                r18 = r18 + 0x4;
                r19 = r19 + 0x1;
                r17 = r17 + 0x1;
            } while ((s32)r17 <= 3);
            tmp = r16 & 0xFF;
            if (tmp == 0) {
                ((void(*)(void))_threadSwitch)();
                r15 = r15 + 0x1;
                if ((s32)r15 < 0xf) goto L_800853EC;
            }
            r3 = 0x26;
            fn_80166A28();
            r15 = 0x0;
            tmp = *(u32*)((u8*)r24 + 0x0);
            if ((s32)tmp != 7) {
                r15 = 0x1;
                tmp = *(u32*)((u8*)r24 + 0x4);
                if ((s32)tmp != 7) {
                    r15 = 0x2;
                    tmp = *(u32*)((u8*)r24 + 0x8);
                    if ((s32)tmp != 7) {
                        r15 = 0x3;
                        tmp = *(u32*)((u8*)r24 + 0xC);
                        if ((s32)tmp != 7) {
                            r15 = 0x4;
            }
            }
            }
            }
            if ((s32)r15 > 3) {
                tmp = *(u32*)((u8*)r24 + 0x28);
                tmp = tmp & 0x00000008;
                if (tmp == 0) {
                    r3 = 0x0;
                    goto L_80085548;
                }
            }
            r3 = 0x1;
            ((void(*)(void))menuSetEnablePort)();
            r4 = r15 + 0x1;
            r3 = 0x2f;
            msgctrlSetValue();
            if ((s32)r15 == 0) {
                r3 = 0x7;
                r4 = 0x44c0;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            } else {

                r3 = 0x7;
                r4 = 0x44b8;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            }
            tmp = 0x8;
            r3 = 0x1;
            *(u32*)((u8*)r24 + 0x28) = tmp;
        L_80085548:
            tmp = r3 & 0xFF;
            if (tmp == 0) {
                tmp = *(u8*)((u8*)r24 + 0x21);
                r3 = 0x6;
                tmp = (s8)tmp;
                tmp = tmp << 2;
                *(u32*)(r24 + tmp) = r3;
            }
            tmp = *(u32*)((u8*)r24 + 0x2C);
            if ((s32)tmp == 3) {
                r3 = 0x7;
                r4 = 0x44e7;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            } else {

                r3 = 0x7;
                r4 = 0x44e6;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            }
            r3 = *(u8*)((u8*)r24 + 0x21);
            r3 = (s8)r3;
            ((void(*)(void))fn_80093698)();
            r3 = 0x0;
            return;
        } while (0);
            r20 = 0x0;
        L_800855B8:
            r19 = 0x0;
            r17 = r24;
            r18 = r19;
            r16 = (u32)&lbl_80478950;
            do {
                tmp = *(u32*)((u8*)r17 + 0x0);
                if ((s32)tmp != 5) {
                    if ((s32)tmp == 4) {
                    }
                    r3 = r18 + 0x1;
                    ((void(*)(void))fn_8008ABA0)();
                    tmp = r3 & 0xFF;
                    if (tmp == 0) {
                        ((void(*)(void))menuGetEnablePort)();
                        tmp = *(u8*)((u8*)r16 + 0x0);
                        tmp = r3 & ~tmp;
                        r3 = tmp & 0xFF;
                        ((void(*)(void))menuSetEnablePort)();
                        r3 = 0x7;
                        tmp = 0x8;
                        *(u32*)((u8*)r17 + 0x0) = r3;
                        *(u32*)((u8*)r24 + 0x28) = tmp;
                    }
                    }
                tmp = *(u32*)((u8*)r17 + 0x0);
                if ((s32)tmp == 7) {
                    r19 = 0x1;
                }
                r17 = r17 + 0x4;
                r16 = r16 + 0x1;
                r18 = r18 + 0x1;
            } while ((s32)r18 <= 3);
            tmp = r19 & 0xFF;
            if (tmp == 0) {
                ((void(*)(void))_threadSwitch)();
                r20 = r20 + 0x1;
                if ((s32)r20 < 0xf) goto L_800855B8;
            }
            r3 = 0x26;
            fn_80166A28();
            r16 = 0x0;
            tmp = *(u32*)((u8*)r24 + 0x0);
            if ((s32)tmp != 7) {
                r16 = 0x1;
                tmp = *(u32*)((u8*)r24 + 0x4);
                if ((s32)tmp != 7) {
                    r16 = 0x2;
                    tmp = *(u32*)((u8*)r24 + 0x8);
                    if ((s32)tmp != 7) {
                        r16 = 0x3;
                        tmp = *(u32*)((u8*)r24 + 0xC);
                        if ((s32)tmp != 7) {
                            r16 = 0x4;
            }
            }
            }
            }
            if ((s32)r16 > 3) {
                tmp = *(u32*)((u8*)r24 + 0x28);
                tmp = tmp & 0x00000008;
                if (tmp == 0) {
                    r3 = 0x0;
                    goto L_80085714;
                }
            }
            r3 = 0x1;
            ((void(*)(void))menuSetEnablePort)();
            r4 = r16 + 0x1;
            r3 = 0x2f;
            msgctrlSetValue();
            if ((s32)r16 == 0) {
                r3 = 0x7;
                r4 = 0x44c0;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            } else {

                r3 = 0x7;
                r4 = 0x44b8;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            }
            tmp = 0x8;
            r3 = 0x1;
            *(u32*)((u8*)r24 + 0x28) = tmp;
        L_80085714:
        do {
            tmp = r3 & 0xFF;
            if (tmp != 0) {
                tmp = *(u32*)((u8*)r24 + 0x24);
                tmp = tmp & 0x00000008;
                if (tmp != 0) {
                    tmp = 0x1;
                    break;
                }
                tmp = 0x0;
                break;
            }
            tmp = *(u8*)((u8*)r24 + 0x21);
            r4 = 0x6;
            r3 = 0x2f;
            tmp = (s8)tmp;
            tmp = tmp << 2;
            *(u32*)(r24 + tmp) = r4;
            tmp = *(u8*)((u8*)r24 + 0x21);
            r4 = (s8)tmp;
            r4 = r4 + 0x1;
            msgctrlSetValue();
            r3 = 0x7;
            r4 = 0x3c47;
            r5 = 0x0;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
            tmp = *(u32*)((u8*)r24 + 0x24);
            tmp = tmp & 0x00000008;
            if (tmp == 0) {
                r3 = r24;
                r4 = 0x1;
                fn_80087AE8();

            } else {
            r3 = r24;
            r4 = 0x7;
            fn_80087AE8();
            tmp = r3 & 0xFF;
            if (tmp != 0) {
                tmp = 0x1;
                break;
            }
            }
            r3 = *(u8*)((u8*)r24 + 0x21);
            r3 = (s8)r3;
            ((void(*)(void))fn_80093698)();
            tmp = 0x0;
        } while (0);
            tmp = tmp & 0xFF;
            if (tmp != 0) goto L_80084E44;
            r3 = 0x0;
            return;
                }
        tmp = 0x4;
        r16 = 0x0;
        *(u32*)(r27 + r24) = tmp;
        do {
            r3 = r26;
            ((void(*)(void))fn_8008ABA0)();
            tmp = r3 & 0xFF;
            if (tmp != 0) {
                ((void(*)(void))menuGetEnablePort)();
                r4 = (u32)&lbl_80478950;
                tmp = *(u8*)(r4 + r28);
                tmp = tmp | r3;
                r3 = tmp & 0xFF;
                ((void(*)(void))menuSetEnablePort)();
                break;
            }
            ((void(*)(void))_threadSwitch)();
            r16 = r16 + 0x1;
        } while ((s32)r16 < 0x12c);

        if (tmp == 0) {
            r4 = r26;
            r3 = 0x2f;
            msgctrlSetValue();
            r3 = 0x7;
            r4 = 0x3c4d;
            r5 = 0x0;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
            f27 = *(f32*)&lbl_8047C1A8;
            f28 = *(f64*)&lbl_8047C1B0;
            r16 = 0x43300000;
            f30 = *(f64*)&lbl_8047C1B8;
            f31 = *(f32*)&lbl_8047C1AC;
            while (f27 < f31) {

                ((void(*)(void))_threadSwitch)();
                ((void(*)(void))fn_800D37CC)();
                *(u32*)(sp + 0xC1C) = tmp;
                f29 = f0 - f28;
                ((void(*)(void))fn_800D3088)();
                f0 = f0 - f30;
                f0 = f0 / f29;
                f27 = f27 + f0;

            }
        }
        r3 = r22;
        heroInit();
        tmp = 0x0;
        r3 = r28;
        *(u32*)(sp + 0x18) = tmp;
        r4 = r22;
        r5 = (u32)sp + 0x18;
        ((void(*)(void))fn_80092FC8)();
        tmp = 0x0;
        *(u32*)((u8*)r24 + 0x28) = tmp;
        goto L_80085934;
    L_800858DC:
        r3 = 0x10c;
        ((void(*)(void))menuIsCheck)();
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            ((void(*)(void))_threadSwitch)();
            goto L_80085934;
        }
        ((void(*)(void))windowGetKeyInfo)();
        tmp = *(u16*)((u8*)r3 + 0x4);
        tmp = tmp & 0x00000020;
        if ((s32)tmp != 0) {
            tmp = 0x2;
            r16 = 0xe;
            *(u32*)((u8*)r24 + 0x28) = tmp;
            goto L_80085960;
        }
        tmp = *(u32*)((u8*)r24 + 0x28);
        if (tmp == 8) {
            r16 = 0xe;
            goto L_80085960;
        }
        ((void(*)(void))_threadSwitch)();
    L_80085934:
        r3 = *(u8*)((u8*)r24 + 0x21);
        r3 = (s8)r3;
        ((void(*)(void))fn_800934E4)();
        if ((s32)r3 == 0) goto L_800858DC;
        tmp = 0x0;
        *(u32*)((u8*)r24 + 0x28) = tmp;
        r3 = *(u8*)((u8*)r24 + 0x21);
        r3 = (s8)r3;
        ((void(*)(void))fn_80093610)();
        r16 = r3;
    L_80085960:
        if ((s32)r16 != 0xe) {
            goto L_80085B40;
        }
        r3 = 0x1;
        ((void(*)(void))menuSetEnablePort)();
        r15 = 0x0;
    L_80085978:
        r16 = 0x0;
        r18 = r24;
        r17 = r16;
        r19 = (u32)&lbl_80478950;
        do {
            tmp = *(u32*)((u8*)r18 + 0x0);
            if ((s32)tmp != 5) {
                if ((s32)tmp == 4) {
                }
                r3 = r17 + 0x1;
                ((void(*)(void))fn_8008ABA0)();
                tmp = r3 & 0xFF;
                if (tmp == 0) {
                    ((void(*)(void))menuGetEnablePort)();
                    tmp = *(u8*)((u8*)r19 + 0x0);
                    tmp = r3 & ~tmp;
                    r3 = tmp & 0xFF;
                    ((void(*)(void))menuSetEnablePort)();
                    r3 = 0x7;
                    tmp = 0x8;
                    *(u32*)((u8*)r18 + 0x0) = r3;
                    *(u32*)((u8*)r24 + 0x28) = tmp;
                }
                }
            tmp = *(u32*)((u8*)r18 + 0x0);
            if ((s32)tmp == 7) {
                r16 = 0x1;
            }
            r18 = r18 + 0x4;
            r19 = r19 + 0x1;
            r17 = r17 + 0x1;
        } while ((s32)r17 <= 3);
        tmp = r16 & 0xFF;
        if (tmp == 0) {
            ((void(*)(void))_threadSwitch)();
            r15 = r15 + 0x1;
            if ((s32)r15 < 0xf) goto L_80085978;
        }
        r3 = 0x26;
        fn_80166A28();
        r15 = 0x0;
        tmp = *(u32*)((u8*)r24 + 0x0);
        if ((s32)tmp != 7) {
            r15 = 0x1;
            tmp = *(u32*)((u8*)r24 + 0x4);
            if ((s32)tmp != 7) {
                r15 = 0x2;
                tmp = *(u32*)((u8*)r24 + 0x8);
                if ((s32)tmp != 7) {
                    r15 = 0x3;
                    tmp = *(u32*)((u8*)r24 + 0xC);
                    if ((s32)tmp != 7) {
                        r15 = 0x4;
        }
        }
        }
        }
        if ((s32)r15 > 3) {
            tmp = *(u32*)((u8*)r24 + 0x28);
            tmp = tmp & 0x00000008;
            if (tmp == 0) {
                r3 = 0x0;
                goto L_80085AD4;
            }
        }
        r3 = 0x1;
        ((void(*)(void))menuSetEnablePort)();
        r4 = r15 + 0x1;
        r3 = 0x2f;
        msgctrlSetValue();
        if ((s32)r15 == 0) {
            r3 = 0x7;
            r4 = 0x44c0;
            r5 = 0x1;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
        } else {

            r3 = 0x7;
            r4 = 0x44b8;
            r5 = 0x1;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
        }
        tmp = 0x8;
        r3 = 0x1;
        *(u32*)((u8*)r24 + 0x28) = tmp;
    L_80085AD4:
        tmp = r3 & 0xFF;
        if (tmp == 0) {
            tmp = *(u8*)((u8*)r24 + 0x21);
            r3 = 0x6;
            tmp = (s8)tmp;
            tmp = tmp << 2;
            *(u32*)(r24 + tmp) = r3;
        }
        tmp = *(u32*)((u8*)r24 + 0x2C);
        if ((s32)tmp == 3) {
            r3 = 0x7;
            r4 = 0x44e7;
            r5 = 0x1;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
        } else {

            r3 = 0x7;
            r4 = 0x44e6;
            r5 = 0x1;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
        }
        r3 = *(u8*)((u8*)r24 + 0x21);
        r3 = (s8)r3;
        ((void(*)(void))fn_80093698)();
        r3 = 0x0;
        return;
    L_80085B40:
        /* extrwi tmp, r3, 2, 22 */;
        if (tmp != 0) {
            r20 = 0x0;
        L_80085B54:
            r18 = 0x0;
            r16 = r24;
            r19 = r18;
            r17 = (u32)&lbl_80478950;
            do {
                tmp = *(u32*)((u8*)r16 + 0x0);
                if ((s32)tmp != 5) {
                    if ((s32)tmp == 4) {
                    }
                    r3 = r19 + 0x1;
                    ((void(*)(void))fn_8008ABA0)();
                    tmp = r3 & 0xFF;
                    if (tmp == 0) {
                        ((void(*)(void))menuGetEnablePort)();
                        tmp = *(u8*)((u8*)r17 + 0x0);
                        tmp = r3 & ~tmp;
                        r3 = tmp & 0xFF;
                        ((void(*)(void))menuSetEnablePort)();
                        r3 = 0x7;
                        tmp = 0x8;
                        *(u32*)((u8*)r16 + 0x0) = r3;
                        *(u32*)((u8*)r24 + 0x28) = tmp;
                    }
                    }
                tmp = *(u32*)((u8*)r16 + 0x0);
                if ((s32)tmp == 7) {
                    r18 = 0x1;
                }
                r16 = r16 + 0x4;
                r17 = r17 + 0x1;
                r19 = r19 + 0x1;
            } while ((s32)r19 <= 3);
            tmp = r18 & 0xFF;
            if (tmp == 0) {
                ((void(*)(void))_threadSwitch)();
                r20 = r20 + 0x1;
                if ((s32)r20 < 0xf) goto L_80085B54;
            }
            r3 = 0x26;
            fn_80166A28();
            r16 = 0x0;
            tmp = *(u32*)((u8*)r24 + 0x0);
            if ((s32)tmp != 7) {
                r16 = 0x1;
                tmp = *(u32*)((u8*)r24 + 0x4);
                if ((s32)tmp != 7) {
                    r16 = 0x2;
                    tmp = *(u32*)((u8*)r24 + 0x8);
                    if ((s32)tmp != 7) {
                        r16 = 0x3;
                        tmp = *(u32*)((u8*)r24 + 0xC);
                        if ((s32)tmp != 7) {
                            r16 = 0x4;
            }
            }
            }
            }
            if ((s32)r16 > 3) {
                tmp = *(u32*)((u8*)r24 + 0x28);
                tmp = tmp & 0x00000008;
                if (tmp == 0) {
                    r3 = 0x0;
                    goto L_80085CB0;
                }
            }
            r3 = 0x1;
            ((void(*)(void))menuSetEnablePort)();
            r4 = r16 + 0x1;
            r3 = 0x2f;
            msgctrlSetValue();
            if ((s32)r16 == 0) {
                r3 = 0x7;
                r4 = 0x44c0;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            } else {

                r3 = 0x7;
                r4 = 0x44b8;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            }
            tmp = 0x8;
            r3 = 0x1;
            *(u32*)((u8*)r24 + 0x28) = tmp;
        L_80085CB0:
        do {
            tmp = r3 & 0xFF;
            if (tmp != 0) {
                tmp = *(u32*)((u8*)r24 + 0x24);
                tmp = tmp & 0x00000008;
                if (tmp != 0) {
                    tmp = 0x1;
                    break;
                }
                tmp = 0x0;
                break;
            }
            tmp = *(u8*)((u8*)r24 + 0x21);
            r4 = 0x6;
            r3 = 0x2f;
            tmp = (s8)tmp;
            tmp = tmp << 2;
            *(u32*)(r24 + tmp) = r4;
            tmp = *(u8*)((u8*)r24 + 0x21);
            r4 = (s8)tmp;
            r4 = r4 + 0x1;
            msgctrlSetValue();
            r3 = 0x7;
            r4 = 0x3c49;
            r5 = 0x0;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
            tmp = *(u32*)((u8*)r24 + 0x24);
            tmp = tmp & 0x00000008;
            if (tmp == 0) {
                r3 = r24;
                r4 = 0x1;
                fn_80087AE8();

            } else {
            r3 = r24;
            r4 = 0x7;
            fn_80087AE8();
            tmp = r3 & 0xFF;
            if (tmp != 0) {
                tmp = 0x1;
                break;
            }
            }
            r3 = *(u8*)((u8*)r24 + 0x21);
            r3 = (s8)r3;
            ((void(*)(void))fn_80093698)();
            tmp = 0x0;
        } while (0);
            tmp = tmp & 0xFF;
            if (tmp != 0) goto L_80084E44;
            r3 = 0x0;
            return;
        }
        /* extrwi tmp, r3, 4, 24 */;
        if (tmp <= 7) {
            r3 = (u32)jumptable_802EEB78;
            tmp = tmp << 2;
            r3 = (u32)jumptable_802EEB78;
            tmp = *(u32*)(r3 + tmp);
            ctr_fn = (void(*)(void))tmp;
            r17 = 0x1;


        } else {
        tmp = 0x0;
        goto L_80085E1C;
        }
        fn_80128E24();
        if (r3 != 0) {
            fn_80128E04();
            if (r3 != 0) {
                gamedataBiosGetGamedataAtttestPtr();
                if (r3 != 0) {
                    gamedataAttestBiosGetLangareaId();
                    r3 = r3 & 0xFF;
                    tmp = r17 & 0xFF;
                    if (r3 == tmp) {
                        tmp = 0x1;
                        goto L_80085E1C;
            }
            }
            }
        }
        tmp = 0x0;
    L_80085E1C:
        tmp = tmp & 0xFF;
        if (tmp == 0) {
            r20 = 0x0;
        L_80085E2C:
            r18 = 0x0;
            r16 = r24;
            r19 = r18;
            r17 = (u32)&lbl_80478950;
            do {
                tmp = *(u32*)((u8*)r16 + 0x0);
                if ((s32)tmp != 5) {
                    if ((s32)tmp == 4) {
                    }
                    r3 = r19 + 0x1;
                    ((void(*)(void))fn_8008ABA0)();
                    tmp = r3 & 0xFF;
                    if (tmp == 0) {
                        ((void(*)(void))menuGetEnablePort)();
                        tmp = *(u8*)((u8*)r17 + 0x0);
                        tmp = r3 & ~tmp;
                        r3 = tmp & 0xFF;
                        ((void(*)(void))menuSetEnablePort)();
                        r3 = 0x7;
                        tmp = 0x8;
                        *(u32*)((u8*)r16 + 0x0) = r3;
                        *(u32*)((u8*)r24 + 0x28) = tmp;
                    }
                    }
                tmp = *(u32*)((u8*)r16 + 0x0);
                if ((s32)tmp == 7) {
                    r18 = 0x1;
                }
                r16 = r16 + 0x4;
                r17 = r17 + 0x1;
                r19 = r19 + 0x1;
            } while ((s32)r19 <= 3);
            tmp = r18 & 0xFF;
            if (tmp == 0) {
                ((void(*)(void))_threadSwitch)();
                r20 = r20 + 0x1;
                if ((s32)r20 < 0xf) goto L_80085E2C;
            }
            r3 = 0x26;
            fn_80166A28();
            r16 = 0x0;
            tmp = *(u32*)((u8*)r24 + 0x0);
            if ((s32)tmp != 7) {
                r16 = 0x1;
                tmp = *(u32*)((u8*)r24 + 0x4);
                if ((s32)tmp != 7) {
                    r16 = 0x2;
                    tmp = *(u32*)((u8*)r24 + 0x8);
                    if ((s32)tmp != 7) {
                        r16 = 0x3;
                        tmp = *(u32*)((u8*)r24 + 0xC);
                        if ((s32)tmp != 7) {
                            r16 = 0x4;
            }
            }
            }
            }
            if ((s32)r16 > 3) {
                tmp = *(u32*)((u8*)r24 + 0x28);
                tmp = tmp & 0x00000008;
                if (tmp == 0) {
                    r3 = 0x0;
                    goto L_80085F88;
                }
            }
            r3 = 0x1;
            ((void(*)(void))menuSetEnablePort)();
            r4 = r16 + 0x1;
            r3 = 0x2f;
            msgctrlSetValue();
            if ((s32)r16 == 0) {
                r3 = 0x7;
                r4 = 0x44c0;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            } else {

                r3 = 0x7;
                r4 = 0x44b8;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            }
            tmp = 0x8;
            r3 = 0x1;
            *(u32*)((u8*)r24 + 0x28) = tmp;
        L_80085F88:
        do {
            tmp = r3 & 0xFF;
            if (tmp != 0) {
                tmp = *(u32*)((u8*)r24 + 0x24);
                tmp = tmp & 0x00000008;
                if (tmp != 0) {
                    tmp = 0x1;
                    break;
                }
                tmp = 0x0;
                break;
            }
            tmp = *(u8*)((u8*)r24 + 0x21);
            r4 = 0x6;
            r3 = 0x2f;
            tmp = (s8)tmp;
            tmp = tmp << 2;
            *(u32*)(r24 + tmp) = r4;
            tmp = *(u8*)((u8*)r24 + 0x21);
            r4 = (s8)tmp;
            r4 = r4 + 0x1;
            msgctrlSetValue();
            r3 = 0x7;
            r4 = 0x44f0;
            r5 = 0x0;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
            tmp = *(u32*)((u8*)r24 + 0x24);
            tmp = tmp & 0x00000008;
            if (tmp == 0) {
                r3 = r24;
                r4 = 0x1;
                fn_80087AE8();

            } else {
            r3 = r24;
            r4 = 0x7;
            fn_80087AE8();
            tmp = r3 & 0xFF;
            if (tmp != 0) {
                tmp = 0x1;
                break;
            }
            }
            r3 = *(u8*)((u8*)r24 + 0x21);
            r3 = (s8)r3;
            ((void(*)(void))fn_80093698)();
            tmp = 0x0;
        } while (0);
            tmp = tmp & 0xFF;
            if (tmp != 0) goto L_80084E44;
            r3 = 0x0;
            return;
        }
        if ((s32)r16 != 4) {

        } else {
        tmp = r3 & 0x00000002;
        if (tmp != 0) goto L_8008629C;
        }
        r20 = 0x0;
    L_80086074:
        r18 = 0x0;
        r16 = r24;
        r19 = r18;
        r17 = (u32)&lbl_80478950;
        do {
            tmp = *(u32*)((u8*)r16 + 0x0);
            if ((s32)tmp != 5) {
                if ((s32)tmp == 4) {
                }
                r3 = r19 + 0x1;
                ((void(*)(void))fn_8008ABA0)();
                tmp = r3 & 0xFF;
                if (tmp == 0) {
                    ((void(*)(void))menuGetEnablePort)();
                    tmp = *(u8*)((u8*)r17 + 0x0);
                    tmp = r3 & ~tmp;
                    r3 = tmp & 0xFF;
                    ((void(*)(void))menuSetEnablePort)();
                    r3 = 0x7;
                    tmp = 0x8;
                    *(u32*)((u8*)r16 + 0x0) = r3;
                    *(u32*)((u8*)r24 + 0x28) = tmp;
                }
                }
            tmp = *(u32*)((u8*)r16 + 0x0);
            if ((s32)tmp == 7) {
                r18 = 0x1;
            }
            r16 = r16 + 0x4;
            r17 = r17 + 0x1;
            r19 = r19 + 0x1;
        } while ((s32)r19 <= 3);
        tmp = r18 & 0xFF;
        if (tmp == 0) {
            ((void(*)(void))_threadSwitch)();
            r20 = r20 + 0x1;
            if ((s32)r20 < 0xf) goto L_80086074;
        }
        r3 = 0x26;
        fn_80166A28();
        r16 = 0x0;
        tmp = *(u32*)((u8*)r24 + 0x0);
        if ((s32)tmp != 7) {
            r16 = 0x1;
            tmp = *(u32*)((u8*)r24 + 0x4);
            if ((s32)tmp != 7) {
                r16 = 0x2;
                tmp = *(u32*)((u8*)r24 + 0x8);
                if ((s32)tmp != 7) {
                    r16 = 0x3;
                    tmp = *(u32*)((u8*)r24 + 0xC);
                    if ((s32)tmp != 7) {
                        r16 = 0x4;
        }
        }
        }
        }
        if ((s32)r16 > 3) {
            tmp = *(u32*)((u8*)r24 + 0x28);
            tmp = tmp & 0x00000008;
            if (tmp == 0) {
                r3 = 0x0;
                goto L_800861D0;
            }
        }
        r3 = 0x1;
        ((void(*)(void))menuSetEnablePort)();
        r4 = r16 + 0x1;
        r3 = 0x2f;
        msgctrlSetValue();
        if ((s32)r16 == 0) {
            r3 = 0x7;
            r4 = 0x44c0;
            r5 = 0x1;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
        } else {

            r3 = 0x7;
            r4 = 0x44b8;
            r5 = 0x1;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
        }
        tmp = 0x8;
        r3 = 0x1;
        *(u32*)((u8*)r24 + 0x28) = tmp;
    L_800861D0:
    do {
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            tmp = *(u32*)((u8*)r24 + 0x24);
            tmp = tmp & 0x00000008;
            if (tmp != 0) {
                tmp = 0x1;
                break;
            }
            tmp = 0x0;
            break;
        }
        tmp = *(u8*)((u8*)r24 + 0x21);
        r4 = 0x6;
        r3 = 0x2f;
        tmp = (s8)tmp;
        tmp = tmp << 2;
        *(u32*)(r24 + tmp) = r4;
        tmp = *(u8*)((u8*)r24 + 0x21);
        r4 = (s8)tmp;
        r4 = r4 + 0x1;
        msgctrlSetValue();
        r3 = 0x7;
        r4 = 0x3c49;
        r5 = 0x0;
        r6 = 0x0;
        ((void(*)(void))winMsgOpen)();
        tmp = *(u32*)((u8*)r24 + 0x24);
        tmp = tmp & 0x00000008;
        if (tmp == 0) {
            r3 = r24;
            r4 = 0x1;
            fn_80087AE8();

        } else {
        r3 = r24;
        r4 = 0x7;
        fn_80087AE8();
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            tmp = 0x1;
            break;
        }
        }
        r3 = *(u8*)((u8*)r24 + 0x21);
        r3 = (s8)r3;
        ((void(*)(void))fn_80093698)();
        tmp = 0x0;
    } while (0);
        tmp = tmp & 0xFF;
        if (tmp != 0) goto L_80084E44;
        r3 = 0x0;
        return;
    L_8008629C:
        if (tmp != 0) {
            tmp = r3 & 0x00000004;
            r4 = 0x1;
            if (tmp != 0) {
                tmp = r3 & 0x00000008;
                if (tmp == 0) {
                    r4 = 0x0;
                }

            } else {
            tmp = r3 & 0x1;
            if (tmp == 0) {
                r4 = 0x0;
        }
            }
            tmp = r4 & 0xFF;
        }
        if (tmp == 0) {
            r20 = 0x0;
        L_800862EC:
            r18 = 0x0;
            r16 = r24;
            r19 = r18;
            r17 = (u32)&lbl_80478950;
            do {
                tmp = *(u32*)((u8*)r16 + 0x0);
                if ((s32)tmp != 5) {
                    if ((s32)tmp == 4) {
                    }
                    r3 = r19 + 0x1;
                    ((void(*)(void))fn_8008ABA0)();
                    tmp = r3 & 0xFF;
                    if (tmp == 0) {
                        ((void(*)(void))menuGetEnablePort)();
                        tmp = *(u8*)((u8*)r17 + 0x0);
                        tmp = r3 & ~tmp;
                        r3 = tmp & 0xFF;
                        ((void(*)(void))menuSetEnablePort)();
                        r3 = 0x7;
                        tmp = 0x8;
                        *(u32*)((u8*)r16 + 0x0) = r3;
                        *(u32*)((u8*)r24 + 0x28) = tmp;
                    }
                    }
                tmp = *(u32*)((u8*)r16 + 0x0);
                if ((s32)tmp == 7) {
                    r18 = 0x1;
                }
                r16 = r16 + 0x4;
                r17 = r17 + 0x1;
                r19 = r19 + 0x1;
            } while ((s32)r19 <= 3);
            tmp = r18 & 0xFF;
            if (tmp == 0) {
                ((void(*)(void))_threadSwitch)();
                r20 = r20 + 0x1;
                if ((s32)r20 < 0xf) goto L_800862EC;
            }
            r3 = 0x26;
            fn_80166A28();
            r16 = 0x0;
            tmp = *(u32*)((u8*)r24 + 0x0);
            if ((s32)tmp != 7) {
                r16 = 0x1;
                tmp = *(u32*)((u8*)r24 + 0x4);
                if ((s32)tmp != 7) {
                    r16 = 0x2;
                    tmp = *(u32*)((u8*)r24 + 0x8);
                    if ((s32)tmp != 7) {
                        r16 = 0x3;
                        tmp = *(u32*)((u8*)r24 + 0xC);
                        if ((s32)tmp != 7) {
                            r16 = 0x4;
            }
            }
            }
            }
            if ((s32)r16 > 3) {
                tmp = *(u32*)((u8*)r24 + 0x28);
                tmp = tmp & 0x00000008;
                if (tmp == 0) {
                    r3 = 0x0;
                    goto L_80086448;
                }
            }
            r3 = 0x1;
            ((void(*)(void))menuSetEnablePort)();
            r4 = r16 + 0x1;
            r3 = 0x2f;
            msgctrlSetValue();
            if ((s32)r16 == 0) {
                r3 = 0x7;
                r4 = 0x44c0;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            } else {

                r3 = 0x7;
                r4 = 0x44b8;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            }
            tmp = 0x8;
            r3 = 0x1;
            *(u32*)((u8*)r24 + 0x28) = tmp;
        L_80086448:
        do {
            tmp = r3 & 0xFF;
            if (tmp != 0) {
                tmp = *(u32*)((u8*)r24 + 0x24);
                tmp = tmp & 0x00000008;
                if (tmp != 0) {
                    tmp = 0x1;
                    break;
                }
                tmp = 0x0;
                break;
            }
            tmp = *(u8*)((u8*)r24 + 0x21);
            r4 = 0x6;
            r3 = 0x2f;
            tmp = (s8)tmp;
            tmp = tmp << 2;
            *(u32*)(r24 + tmp) = r4;
            tmp = *(u8*)((u8*)r24 + 0x21);
            r4 = (s8)tmp;
            r4 = r4 + 0x1;
            msgctrlSetValue();
            r3 = 0x7;
            r4 = 0x44c3;
            r5 = 0x0;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
            tmp = *(u32*)((u8*)r24 + 0x24);
            tmp = tmp & 0x00000008;
            if (tmp == 0) {
                r3 = r24;
                r4 = 0x1;
                fn_80087AE8();

            } else {
            r3 = r24;
            r4 = 0x7;
            fn_80087AE8();
            tmp = r3 & 0xFF;
            if (tmp != 0) {
                tmp = 0x1;
                break;
            }
            }
            r3 = *(u8*)((u8*)r24 + 0x21);
            r3 = (s8)r3;
            ((void(*)(void))fn_80093698)();
            tmp = 0x0;
        } while (0);
            tmp = tmp & 0xFF;
            if (tmp != 0) goto L_80084E44;
            r3 = 0x0;
            return;
        }
        do {
            if (tmp == 0) break;
            tmp = r3 & 0x1;
            if (tmp == 0) {
                r20 = 0x0;
            L_80086530:
                r18 = 0x0;
                r16 = r24;
                r19 = r18;
                r17 = (u32)&lbl_80478950;
                do {
                    tmp = *(u32*)((u8*)r16 + 0x0);
                    if ((s32)tmp != 5) {
                        if ((s32)tmp == 4) {
                        }
                        r3 = r19 + 0x1;
                        ((void(*)(void))fn_8008ABA0)();
                        tmp = r3 & 0xFF;
                        if (tmp == 0) {
                            ((void(*)(void))menuGetEnablePort)();
                            tmp = *(u8*)((u8*)r17 + 0x0);
                            tmp = r3 & ~tmp;
                            r3 = tmp & 0xFF;
                            ((void(*)(void))menuSetEnablePort)();
                            r3 = 0x7;
                            tmp = 0x8;
                            *(u32*)((u8*)r16 + 0x0) = r3;
                            *(u32*)((u8*)r24 + 0x28) = tmp;
                        }
                        }
                    tmp = *(u32*)((u8*)r16 + 0x0);
                    if ((s32)tmp == 7) {
                        r18 = 0x1;
                    }
                    r16 = r16 + 0x4;
                    r17 = r17 + 0x1;
                    r19 = r19 + 0x1;
                } while ((s32)r19 <= 3);
                tmp = r18 & 0xFF;
                if (tmp == 0) {
                    ((void(*)(void))_threadSwitch)();
                    r20 = r20 + 0x1;
                    if ((s32)r20 < 0xf) goto L_80086530;
                }
                r3 = 0x26;
                fn_80166A28();
                r16 = 0x0;
                tmp = *(u32*)((u8*)r24 + 0x0);
                if ((s32)tmp != 7) {
                    r16 = 0x1;
                    tmp = *(u32*)((u8*)r24 + 0x4);
                    if ((s32)tmp != 7) {
                        r16 = 0x2;
                        tmp = *(u32*)((u8*)r24 + 0x8);
                        if ((s32)tmp != 7) {
                            r16 = 0x3;
                            tmp = *(u32*)((u8*)r24 + 0xC);
                            if ((s32)tmp != 7) {
                                r16 = 0x4;
                }
                }
                }
                }
                if ((s32)r16 > 3) {
                    tmp = *(u32*)((u8*)r24 + 0x28);
                    tmp = tmp & 0x00000008;
                    if (tmp == 0) {
                        r3 = 0x0;
                        goto L_8008668C;
                    }
                }
                r3 = 0x1;
                ((void(*)(void))menuSetEnablePort)();
                r4 = r16 + 0x1;
                r3 = 0x2f;
                msgctrlSetValue();
                if ((s32)r16 == 0) {
                    r3 = 0x7;
                    r4 = 0x44c0;
                    r5 = 0x1;
                    r6 = 0x0;
                    ((void(*)(void))winMsgOpen)();
                } else {

                    r3 = 0x7;
                    r4 = 0x44b8;
                    r5 = 0x1;
                    r6 = 0x0;
                    ((void(*)(void))winMsgOpen)();
                }
                tmp = 0x8;
                r3 = 0x1;
                *(u32*)((u8*)r24 + 0x28) = tmp;
            L_8008668C:
            do {
                tmp = r3 & 0xFF;
                if (tmp != 0) {
                    tmp = *(u32*)((u8*)r24 + 0x24);
                    tmp = tmp & 0x00000008;
                    if (tmp != 0) {
                        tmp = 0x1;
                        break;
                    }
                    tmp = 0x0;
                    break;
                }
                tmp = *(u8*)((u8*)r24 + 0x21);
                r4 = 0x6;
                r3 = 0x2f;
                tmp = (s8)tmp;
                tmp = tmp << 2;
                *(u32*)(r24 + tmp) = r4;
                tmp = *(u8*)((u8*)r24 + 0x21);
                r4 = (s8)tmp;
                r4 = r4 + 0x1;
                msgctrlSetValue();
                r3 = 0x7;
                r4 = 0x44c3;
                r5 = 0x0;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
                tmp = *(u32*)((u8*)r24 + 0x24);
                tmp = tmp & 0x00000008;
                if (tmp == 0) {
                    r3 = r24;
                    r4 = 0x1;
                    fn_80087AE8();

                } else {
                r3 = r24;
                r4 = 0x7;
                fn_80087AE8();
                tmp = r3 & 0xFF;
                if (tmp != 0) {
                    tmp = 0x1;
                    break;
                }
                }
                r3 = *(u8*)((u8*)r24 + 0x21);
                r3 = (s8)r3;
                ((void(*)(void))fn_80093698)();
                tmp = 0x0;
            } while (0);
                tmp = tmp & 0xFF;
                if (tmp != 0) goto L_80084E44;
                r3 = 0x0;
                return;
            }
            r3 = r22;
            ((void(*)(void))fn_800776E4)();
            tmp = r3 & 0xFF;
            if (tmp != 0) break;
            r3 = 0xe4;
            r4 = 0x0;
            r5 = 0x1;
            ((void(*)(void))menuCloseCustom)();
            r3 = r22;
            r4 = 0x1;
            ((void(*)(void))fn_8005CF2C)();
            if (tmp == 0) {
                r3 = 0x0;
                return;
            }
            r3 = 0xe4;
            r4 = 0x0;
            ((void(*)(void))menuOpen)();
            r3 = 0xe4;
            ((void(*)(void))windowSearchID)();
            r16 = r3;
            if (r16 == 0) {
                r4 = 0x1f4;
                r5 = (u32)&lbl_8047C1A0;
                r3 = r3 + 0x184;
                __assert();
            }
            if (r16 == 0) {
                r3 = 0xa6;
                ((void(*)(void))windowSearchID)();
                r16 = r3;
            }
            r3 = r16;
            ((void(*)(void))windowGetFreeWork)();
            r5 = *(u32*)((u8*)r3 + 0x0);
            r4 = 0x0;
            *(u32*)((u8*)r5 + 0x24) = tmp;
            *(u32*)((u8*)r5 + 0x2C) = tmp;
            tmp = *(u8*)((u8*)r3 + 0x0);
            *(u8*)((u8*)r5 + 0x21) = tmp;
            tmp = 0x5;
            r3 = *(u8*)((u8*)r30 + 0x0);
            r3 = (s8)r3;
            r4 = r3 << 2;
            r3 = *(u32*)(r29 + r4);
            *(u32*)(r5 + r4) = r3;
            if ((s32)r4 < (s32)r25) {
                r3 = *(u32*)(r5 + r4);
                if ((s32)r3 == 1) {
                    *(u32*)(r5 + r4) = tmp;
            }
            }
            r6 = r30 + 0x1;
            r4 = 0x1;
            r3 = *(u8*)((u8*)r6 + 0x0);
            r3 = (s8)r3;
            r4 = r3 << 2;
            r3 = *(u32*)(r29 + r4);
            *(u32*)(r5 + r4) = r3;
            if ((s32)r4 < (s32)r25) {
                r3 = *(u32*)(r5 + r4);
                if ((s32)r3 == 1) {
                    *(u32*)(r5 + r4) = tmp;
            }
            }
            r6 = r6 + 0x1;
            r4 = 0x2;
            r3 = *(u8*)((u8*)r6 + 0x0);
            r3 = (s8)r3;
            r4 = r3 << 2;
            r3 = *(u32*)(r29 + r4);
            *(u32*)(r5 + r4) = r3;
            if ((s32)r4 < (s32)r25) {
                r3 = *(u32*)(r5 + r4);
                if ((s32)r3 == 1) {
                    *(u32*)(r5 + r4) = tmp;
            }
            }
            r6 = r6 + 0x1;
            r4 = 0x3;
            r3 = *(u8*)((u8*)r6 + 0x0);
            r3 = (s8)r3;
            r4 = r3 << 2;
            r3 = *(u32*)(r29 + r4);
            *(u32*)(r5 + r4) = r3;
            if ((s32)r4 < (s32)r25) {
                r3 = *(u32*)(r5 + r4);
                if ((s32)r3 == 1) {
                    *(u32*)(r5 + r4) = tmp;
            }
            }
            tmp = 0x6;
            r24 = r5;
            *(u32*)(r27 + r5) = tmp;
            goto L_80084E44;
        } while (0);
        if (tmp != 0) {
            tmp = (s8)r23;
            if ((s32)tmp != 1) {
            }
            if (r31 == 0) goto L_80086DBC;
            }
        r3 = r28;
        r4 = r21;
        ((void(*)(void))fn_80092E38)();
        tmp = 0x0;
        *(u32*)((u8*)r24 + 0x28) = tmp;
        goto L_8008697C;
    L_80086924:
        r3 = 0x10c;
        ((void(*)(void))menuIsCheck)();
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            ((void(*)(void))_threadSwitch)();
            goto L_8008697C;
        }
        ((void(*)(void))windowGetKeyInfo)();
        tmp = *(u16*)((u8*)r3 + 0x4);
        tmp = tmp & 0x00000020;
        if ((s32)tmp != 0) {
            tmp = 0x2;
            r3 = 0xe;
            *(u32*)((u8*)r24 + 0x28) = tmp;
            goto L_800869A4;
        }
        tmp = *(u32*)((u8*)r24 + 0x28);
        if (tmp == 8) {
            r3 = 0xe;
            goto L_800869A4;
        }
        ((void(*)(void))_threadSwitch)();
    L_8008697C:
        r3 = *(u8*)((u8*)r24 + 0x21);
        r3 = (s8)r3;
        ((void(*)(void))fn_800934E4)();
        if ((s32)r3 == 0) goto L_80086924;
        tmp = 0x0;
        *(u32*)((u8*)r24 + 0x28) = tmp;
        r3 = *(u8*)((u8*)r24 + 0x21);
        r3 = (s8)r3;
        ((void(*)(void))fn_80093610)();
    L_800869A4:
        if ((s32)r3 != 0xe) {
            if ((s32)r3 < 0xe) {
                if ((s32)r3 == 0xb) goto L_80086DBC;
            }
            goto L_80086B90;
        }
        r3 = 0x1;
        ((void(*)(void))menuSetEnablePort)();
        r15 = 0x0;
    L_800869C8:
        r16 = 0x0;
        r18 = r24;
        r17 = r16;
        r19 = (u32)&lbl_80478950;
        do {
            tmp = *(u32*)((u8*)r18 + 0x0);
            if ((s32)tmp != 5) {
                if ((s32)tmp == 4) {
                }
                r3 = r17 + 0x1;
                ((void(*)(void))fn_8008ABA0)();
                tmp = r3 & 0xFF;
                if (tmp == 0) {
                    ((void(*)(void))menuGetEnablePort)();
                    tmp = *(u8*)((u8*)r19 + 0x0);
                    tmp = r3 & ~tmp;
                    r3 = tmp & 0xFF;
                    ((void(*)(void))menuSetEnablePort)();
                    r3 = 0x7;
                    tmp = 0x8;
                    *(u32*)((u8*)r18 + 0x0) = r3;
                    *(u32*)((u8*)r24 + 0x28) = tmp;
                }
                }
            tmp = *(u32*)((u8*)r18 + 0x0);
            if ((s32)tmp == 7) {
                r16 = 0x1;
            }
            r18 = r18 + 0x4;
            r19 = r19 + 0x1;
            r17 = r17 + 0x1;
        } while ((s32)r17 <= 3);
        tmp = r16 & 0xFF;
        if (tmp == 0) {
            ((void(*)(void))_threadSwitch)();
            r15 = r15 + 0x1;
            if ((s32)r15 < 0xf) goto L_800869C8;
        }
        r3 = 0x26;
        fn_80166A28();
        r15 = 0x0;
        tmp = *(u32*)((u8*)r24 + 0x0);
        if ((s32)tmp != 7) {
            r15 = 0x1;
            tmp = *(u32*)((u8*)r24 + 0x4);
            if ((s32)tmp != 7) {
                r15 = 0x2;
                tmp = *(u32*)((u8*)r24 + 0x8);
                if ((s32)tmp != 7) {
                    r15 = 0x3;
                    tmp = *(u32*)((u8*)r24 + 0xC);
                    if ((s32)tmp != 7) {
                        r15 = 0x4;
        }
        }
        }
        }
        if ((s32)r15 > 3) {
            tmp = *(u32*)((u8*)r24 + 0x28);
            tmp = tmp & 0x00000008;
            if (tmp == 0) {
                r3 = 0x0;
                goto L_80086B24;
            }
        }
        r3 = 0x1;
        ((void(*)(void))menuSetEnablePort)();
        r4 = r15 + 0x1;
        r3 = 0x2f;
        msgctrlSetValue();
        if ((s32)r15 == 0) {
            r3 = 0x7;
            r4 = 0x44c0;
            r5 = 0x1;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
        } else {

            r3 = 0x7;
            r4 = 0x44b8;
            r5 = 0x1;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
        }
        tmp = 0x8;
        r3 = 0x1;
        *(u32*)((u8*)r24 + 0x28) = tmp;
    L_80086B24:
        tmp = r3 & 0xFF;
        if (tmp == 0) {
            tmp = *(u8*)((u8*)r24 + 0x21);
            r3 = 0x6;
            tmp = (s8)tmp;
            tmp = tmp << 2;
            *(u32*)(r24 + tmp) = r3;
        }
        tmp = *(u32*)((u8*)r24 + 0x2C);
        if ((s32)tmp == 3) {
            r3 = 0x7;
            r4 = 0x44e7;
            r5 = 0x1;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
        } else {

            r3 = 0x7;
            r4 = 0x44e6;
            r5 = 0x1;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
        }
        r3 = *(u8*)((u8*)r24 + 0x21);
        r3 = (s8)r3;
        ((void(*)(void))fn_80093698)();
        r3 = 0x0;
        return;
    L_80086B90:
        r20 = 0x0;
    L_80086B94:
        r18 = 0x0;
        r16 = r24;
        r19 = r18;
        r17 = (u32)&lbl_80478950;
        do {
            tmp = *(u32*)((u8*)r16 + 0x0);
            if ((s32)tmp != 5) {
                if ((s32)tmp == 4) {
                }
                r3 = r19 + 0x1;
                ((void(*)(void))fn_8008ABA0)();
                tmp = r3 & 0xFF;
                if (tmp == 0) {
                    ((void(*)(void))menuGetEnablePort)();
                    tmp = *(u8*)((u8*)r17 + 0x0);
                    tmp = r3 & ~tmp;
                    r3 = tmp & 0xFF;
                    ((void(*)(void))menuSetEnablePort)();
                    r3 = 0x7;
                    tmp = 0x8;
                    *(u32*)((u8*)r16 + 0x0) = r3;
                    *(u32*)((u8*)r24 + 0x28) = tmp;
                }
                }
            tmp = *(u32*)((u8*)r16 + 0x0);
            if ((s32)tmp == 7) {
                r18 = 0x1;
            }
            r16 = r16 + 0x4;
            r17 = r17 + 0x1;
            r19 = r19 + 0x1;
        } while ((s32)r19 <= 3);
        tmp = r18 & 0xFF;
        if (tmp == 0) {
            ((void(*)(void))_threadSwitch)();
            r20 = r20 + 0x1;
            if ((s32)r20 < 0xf) goto L_80086B94;
        }
        r3 = 0x26;
        fn_80166A28();
        r16 = 0x0;
        tmp = *(u32*)((u8*)r24 + 0x0);
        if ((s32)tmp != 7) {
            r16 = 0x1;
            tmp = *(u32*)((u8*)r24 + 0x4);
            if ((s32)tmp != 7) {
                r16 = 0x2;
                tmp = *(u32*)((u8*)r24 + 0x8);
                if ((s32)tmp != 7) {
                    r16 = 0x3;
                    tmp = *(u32*)((u8*)r24 + 0xC);
                    if ((s32)tmp != 7) {
                        r16 = 0x4;
        }
        }
        }
        }
        if ((s32)r16 > 3) {
            tmp = *(u32*)((u8*)r24 + 0x28);
            tmp = tmp & 0x00000008;
            if (tmp == 0) {
                r3 = 0x0;
                goto L_80086CF0;
            }
        }
        r3 = 0x1;
        ((void(*)(void))menuSetEnablePort)();
        r4 = r16 + 0x1;
        r3 = 0x2f;
        msgctrlSetValue();
        if ((s32)r16 == 0) {
            r3 = 0x7;
            r4 = 0x44c0;
            r5 = 0x1;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
        } else {

            r3 = 0x7;
            r4 = 0x44b8;
            r5 = 0x1;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
        }
        tmp = 0x8;
        r3 = 0x1;
        *(u32*)((u8*)r24 + 0x28) = tmp;
    L_80086CF0:
    do {
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            tmp = *(u32*)((u8*)r24 + 0x24);
            tmp = tmp & 0x00000008;
            if (tmp != 0) {
                tmp = 0x1;
                break;
            }
            tmp = 0x0;
            break;
        }
        tmp = *(u8*)((u8*)r24 + 0x21);
        r4 = 0x6;
        r3 = 0x2f;
        tmp = (s8)tmp;
        tmp = tmp << 2;
        *(u32*)(r24 + tmp) = r4;
        tmp = *(u8*)((u8*)r24 + 0x21);
        r4 = (s8)tmp;
        r4 = r4 + 0x1;
        msgctrlSetValue();
        r3 = 0x7;
        r4 = 0x3c47;
        r5 = 0x0;
        r6 = 0x0;
        ((void(*)(void))winMsgOpen)();
        tmp = *(u32*)((u8*)r24 + 0x24);
        tmp = tmp & 0x00000008;
        if (tmp == 0) {
            r3 = r24;
            r4 = 0x1;
            fn_80087AE8();

        } else {
        r3 = r24;
        r4 = 0x7;
        fn_80087AE8();
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            tmp = 0x1;
            break;
        }
        }
        r3 = *(u8*)((u8*)r24 + 0x21);
        r3 = (s8)r3;
        ((void(*)(void))fn_80093698)();
        tmp = 0x0;
    } while (0);
        tmp = tmp & 0xFF;
        if (tmp != 0) goto L_80084E44;
        r3 = 0x0;
        return;
    L_80086DBC:
        if (r31 != 0) {
            tmp = *(u32*)((u8*)r21 + 0x8);
            tmp = tmp & 0x00000010;
        }
        if (tmp == 0) {
            r20 = 0x0;
        L_80086DD8:
            r18 = 0x0;
            r16 = r24;
            r19 = r18;
            r17 = (u32)&lbl_80478950;
            do {
                tmp = *(u32*)((u8*)r16 + 0x0);
                if ((s32)tmp != 5) {
                    if ((s32)tmp == 4) {
                    }
                    r3 = r19 + 0x1;
                    ((void(*)(void))fn_8008ABA0)();
                    tmp = r3 & 0xFF;
                    if (tmp == 0) {
                        ((void(*)(void))menuGetEnablePort)();
                        tmp = *(u8*)((u8*)r17 + 0x0);
                        tmp = r3 & ~tmp;
                        r3 = tmp & 0xFF;
                        ((void(*)(void))menuSetEnablePort)();
                        r3 = 0x7;
                        tmp = 0x8;
                        *(u32*)((u8*)r16 + 0x0) = r3;
                        *(u32*)((u8*)r24 + 0x28) = tmp;
                    }
                    }
                tmp = *(u32*)((u8*)r16 + 0x0);
                if ((s32)tmp == 7) {
                    r18 = 0x1;
                }
                r16 = r16 + 0x4;
                r17 = r17 + 0x1;
                r19 = r19 + 0x1;
            } while ((s32)r19 <= 3);
            tmp = r18 & 0xFF;
            if (tmp == 0) {
                ((void(*)(void))_threadSwitch)();
                r20 = r20 + 0x1;
                if ((s32)r20 < 0xf) goto L_80086DD8;
            }
            r3 = 0x26;
            fn_80166A28();
            r16 = 0x0;
            tmp = *(u32*)((u8*)r24 + 0x0);
            if ((s32)tmp != 7) {
                r16 = 0x1;
                tmp = *(u32*)((u8*)r24 + 0x4);
                if ((s32)tmp != 7) {
                    r16 = 0x2;
                    tmp = *(u32*)((u8*)r24 + 0x8);
                    if ((s32)tmp != 7) {
                        r16 = 0x3;
                        tmp = *(u32*)((u8*)r24 + 0xC);
                        if ((s32)tmp != 7) {
                            r16 = 0x4;
            }
            }
            }
            }
            if ((s32)r16 > 3) {
                tmp = *(u32*)((u8*)r24 + 0x28);
                tmp = tmp & 0x00000008;
                if (tmp == 0) {
                    r3 = 0x0;
                    goto L_80086F34;
                }
            }
            r3 = 0x1;
            ((void(*)(void))menuSetEnablePort)();
            r4 = r16 + 0x1;
            r3 = 0x2f;
            msgctrlSetValue();
            if ((s32)r16 == 0) {
                r3 = 0x7;
                r4 = 0x44c0;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            } else {

                r3 = 0x7;
                r4 = 0x44b8;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            }
            tmp = 0x8;
            r3 = 0x1;
            *(u32*)((u8*)r24 + 0x28) = tmp;
        L_80086F34:
        do {
            tmp = r3 & 0xFF;
            if (tmp != 0) {
                tmp = *(u32*)((u8*)r24 + 0x24);
                tmp = tmp & 0x00000008;
                if (tmp != 0) {
                    tmp = 0x1;
                    break;
                }
                tmp = 0x0;
                break;
            }
            tmp = *(u8*)((u8*)r24 + 0x21);
            r4 = 0x6;
            r3 = 0x2f;
            tmp = (s8)tmp;
            tmp = tmp << 2;
            *(u32*)(r24 + tmp) = r4;
            tmp = *(u8*)((u8*)r24 + 0x21);
            r4 = (s8)tmp;
            r4 = r4 + 0x1;
            msgctrlSetValue();
            r3 = 0x7;
            r4 = 0x4417;
            r5 = 0x0;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
            tmp = *(u32*)((u8*)r24 + 0x24);
            tmp = tmp & 0x00000008;
            if (tmp == 0) {
                r3 = r24;
                r4 = 0x1;
                fn_80087AE8();

            } else {
            r3 = r24;
            r4 = 0x7;
            fn_80087AE8();
            tmp = r3 & 0xFF;
            if (tmp != 0) {
                tmp = 0x1;
                break;
            }
            }
            r3 = *(u8*)((u8*)r24 + 0x21);
            r3 = (s8)r3;
            ((void(*)(void))fn_80093698)();
            tmp = 0x0;
        } while (0);
            tmp = tmp & 0xFF;
            if (tmp != 0) goto L_80084E44;
            r3 = 0x2f;
            r4 = 0x0;
            msgctrlSetValue();
            r3 = 0x7;
            r4 = 0x44cf;
            r5 = 0x0;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
            r3 = r24;
            r4 = 0x1;
            fn_80087AE8();
            r3 = 0x0;
            return;
        }
    do {
        r3 = r28;
        ((void(*)(void))fn_80093698)();
        r3 = 0x3cc;
        fn_80166A28();
        tmp = 0x5;
        *(u32*)(r24 + r27) = tmp;
        ((void(*)(void))menuGetEnablePort)();
        r4 = (u32)&lbl_80478950;
        tmp = *(u8*)(r4 + r28);
        tmp = tmp | r3;
        r3 = tmp & 0xFF;
        ((void(*)(void))menuSetEnablePort)();
        r4 = r26;
        r3 = 0x2f;
        msgctrlSetValue();
        r3 = 0x7;
        r4 = 0x3c4b;
        r5 = 0x0;
        r6 = 0x0;
        ((void(*)(void))winMsgOpen)();
        if ((s32)tmp != 2) {
            if ((s32)tmp < 2) {
                if ((s32)tmp != 0) {
                    if ((s32)tmp < 0) {
                        break;
                    }
                    if ((s32)tmp >= 4) break;
                    goto L_8008769C;
                    }
                r3 = r24;
                r4 = 0x3;
                fn_80087AE8();
                tmp = r3 & 0xFF;
                if (tmp != 0) {
                    r3 = 0x1;
                    return;
                }
                r3 = 0x1;
                ((void(*)(void))menuSetEnablePort)();
                r15 = 0x0;
            L_800870D4:
                r16 = 0x0;
                r18 = r24;
                r17 = r16;
                r19 = (u32)&lbl_80478950;
                do {
                    tmp = *(u32*)((u8*)r18 + 0x0);
                    if ((s32)tmp != 5) {
                        if ((s32)tmp == 4) {
                        }
                        r3 = r17 + 0x1;
                        ((void(*)(void))fn_8008ABA0)();
                        tmp = r3 & 0xFF;
                        if (tmp == 0) {
                            ((void(*)(void))menuGetEnablePort)();
                            tmp = *(u8*)((u8*)r19 + 0x0);
                            tmp = r3 & ~tmp;
                            r3 = tmp & 0xFF;
                            ((void(*)(void))menuSetEnablePort)();
                            r3 = 0x7;
                            tmp = 0x8;
                            *(u32*)((u8*)r18 + 0x0) = r3;
                            *(u32*)((u8*)r24 + 0x28) = tmp;
                        }
                        }
                    tmp = *(u32*)((u8*)r18 + 0x0);
                    if ((s32)tmp == 7) {
                        r16 = 0x1;
                    }
                    r18 = r18 + 0x4;
                    r19 = r19 + 0x1;
                    r17 = r17 + 0x1;
                } while ((s32)r17 <= 3);
                tmp = r16 & 0xFF;
                if (tmp == 0) {
                    ((void(*)(void))_threadSwitch)();
                    r15 = r15 + 0x1;
                    if ((s32)r15 < 0xf) goto L_800870D4;
                }
                r3 = 0x26;
                fn_80166A28();
                r15 = 0x0;
                tmp = *(u32*)((u8*)r24 + 0x0);
                if ((s32)tmp != 7) {
                    r15 = 0x1;
                    tmp = *(u32*)((u8*)r24 + 0x4);
                    if ((s32)tmp != 7) {
                        r15 = 0x2;
                        tmp = *(u32*)((u8*)r24 + 0x8);
                        if ((s32)tmp != 7) {
                            r15 = 0x3;
                            tmp = *(u32*)((u8*)r24 + 0xC);
                            if ((s32)tmp != 7) {
                                r15 = 0x4;
                }
                }
                }
                }
                if ((s32)r15 > 3) {
                    tmp = *(u32*)((u8*)r24 + 0x28);
                    tmp = tmp & 0x00000008;
                    if (tmp == 0) {
                        r3 = 0x0;
                        goto L_80087230;
                    }
                }
                r3 = 0x1;
                ((void(*)(void))menuSetEnablePort)();
                r4 = r15 + 0x1;
                r3 = 0x2f;
                msgctrlSetValue();
                if ((s32)r15 == 0) {
                    r3 = 0x7;
                    r4 = 0x44c0;
                    r5 = 0x1;
                    r6 = 0x0;
                    ((void(*)(void))winMsgOpen)();
                } else {

                    r3 = 0x7;
                    r4 = 0x44b8;
                    r5 = 0x1;
                    r6 = 0x0;
                    ((void(*)(void))winMsgOpen)();
                }
                tmp = 0x8;
                r3 = 0x1;
                *(u32*)((u8*)r24 + 0x28) = tmp;
            L_80087230:
                tmp = r3 & 0xFF;
                if (tmp == 0) {
                    tmp = *(u8*)((u8*)r24 + 0x21);
                    r3 = 0x6;
                    tmp = (s8)tmp;
                    tmp = tmp << 2;
                    *(u32*)(r24 + tmp) = r3;
                }
                tmp = *(u32*)((u8*)r24 + 0x2C);
                if ((s32)tmp == 3) {
                    r3 = 0x7;
                    r4 = 0x44e7;
                    r5 = 0x1;
                    r6 = 0x0;
                    ((void(*)(void))winMsgOpen)();
                } else {

                    r3 = 0x7;
                    r4 = 0x44e6;
                    r5 = 0x1;
                    r6 = 0x0;
                    ((void(*)(void))winMsgOpen)();
                }
                r3 = *(u8*)((u8*)r24 + 0x21);
                r3 = (s8)r3;
                ((void(*)(void))fn_80093698)();
                r3 = 0x0;
                return;
                    }
            tmp = (s8)r23;
            if ((s32)tmp != 2) break;
            r3 = r24;
            r4 = 0x3;
            fn_80087AE8();
            tmp = r3 & 0xFF;
            if (tmp != 0) {
                r3 = 0x1;
                return;
            }
            r3 = 0x1;
            ((void(*)(void))menuSetEnablePort)();
            r15 = 0x0;
        L_800872D4:
            r16 = 0x0;
            r18 = r24;
            r17 = r16;
            r19 = (u32)&lbl_80478950;
            do {
                tmp = *(u32*)((u8*)r18 + 0x0);
                if ((s32)tmp != 5) {
                    if ((s32)tmp == 4) {
                    }
                    r3 = r17 + 0x1;
                    ((void(*)(void))fn_8008ABA0)();
                    tmp = r3 & 0xFF;
                    if (tmp == 0) {
                        ((void(*)(void))menuGetEnablePort)();
                        tmp = *(u8*)((u8*)r19 + 0x0);
                        tmp = r3 & ~tmp;
                        r3 = tmp & 0xFF;
                        ((void(*)(void))menuSetEnablePort)();
                        r3 = 0x7;
                        tmp = 0x8;
                        *(u32*)((u8*)r18 + 0x0) = r3;
                        *(u32*)((u8*)r24 + 0x28) = tmp;
                    }
                    }
                tmp = *(u32*)((u8*)r18 + 0x0);
                if ((s32)tmp == 7) {
                    r16 = 0x1;
                }
                r18 = r18 + 0x4;
                r19 = r19 + 0x1;
                r17 = r17 + 0x1;
            } while ((s32)r17 <= 3);
            tmp = r16 & 0xFF;
            if (tmp == 0) {
                ((void(*)(void))_threadSwitch)();
                r15 = r15 + 0x1;
                if ((s32)r15 < 0xf) goto L_800872D4;
            }
            r3 = 0x26;
            fn_80166A28();
            r15 = 0x0;
            tmp = *(u32*)((u8*)r24 + 0x0);
            if ((s32)tmp != 7) {
                r15 = 0x1;
                tmp = *(u32*)((u8*)r24 + 0x4);
                if ((s32)tmp != 7) {
                    r15 = 0x2;
                    tmp = *(u32*)((u8*)r24 + 0x8);
                    if ((s32)tmp != 7) {
                        r15 = 0x3;
                        tmp = *(u32*)((u8*)r24 + 0xC);
                        if ((s32)tmp != 7) {
                            r15 = 0x4;
            }
            }
            }
            }
            if ((s32)r15 > 3) {
                tmp = *(u32*)((u8*)r24 + 0x28);
                tmp = tmp & 0x00000008;
                if (tmp == 0) {
                    r3 = 0x0;
                    goto L_80087430;
                }
            }
            r3 = 0x1;
            ((void(*)(void))menuSetEnablePort)();
            r4 = r15 + 0x1;
            r3 = 0x2f;
            msgctrlSetValue();
            if ((s32)r15 == 0) {
                r3 = 0x7;
                r4 = 0x44c0;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            } else {

                r3 = 0x7;
                r4 = 0x44b8;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            }
            tmp = 0x8;
            r3 = 0x1;
            *(u32*)((u8*)r24 + 0x28) = tmp;
        L_80087430:
            tmp = r3 & 0xFF;
            if (tmp == 0) {
                tmp = *(u8*)((u8*)r24 + 0x21);
                r3 = 0x6;
                tmp = (s8)tmp;
                tmp = tmp << 2;
                *(u32*)(r24 + tmp) = r3;
            }
            tmp = *(u32*)((u8*)r24 + 0x2C);
            if ((s32)tmp == 3) {
                r3 = 0x7;
                r4 = 0x44e7;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            } else {

                r3 = 0x7;
                r4 = 0x44e6;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            }
            r3 = *(u8*)((u8*)r24 + 0x21);
            r3 = (s8)r3;
            ((void(*)(void))fn_80093698)();
            r3 = 0x0;
            return;
        }
        tmp = (s8)r23;
        if ((s32)tmp != 3) break;
        r3 = r24;
        r4 = 0x3;
        fn_80087AE8();
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            r3 = 0x1;
            return;
        }
        r3 = 0x1;
        ((void(*)(void))menuSetEnablePort)();
        r15 = 0x0;
    L_800874D4:
        r16 = 0x0;
        r18 = r24;
        r17 = r16;
        r19 = (u32)&lbl_80478950;
        do {
            tmp = *(u32*)((u8*)r18 + 0x0);
            if ((s32)tmp != 5) {
                if ((s32)tmp == 4) {
                }
                r3 = r17 + 0x1;
                ((void(*)(void))fn_8008ABA0)();
                tmp = r3 & 0xFF;
                if (tmp == 0) {
                    ((void(*)(void))menuGetEnablePort)();
                    tmp = *(u8*)((u8*)r19 + 0x0);
                    tmp = r3 & ~tmp;
                    r3 = tmp & 0xFF;
                    ((void(*)(void))menuSetEnablePort)();
                    r3 = 0x7;
                    tmp = 0x8;
                    *(u32*)((u8*)r18 + 0x0) = r3;
                    *(u32*)((u8*)r24 + 0x28) = tmp;
                }
                }
            tmp = *(u32*)((u8*)r18 + 0x0);
            if ((s32)tmp == 7) {
                r16 = 0x1;
            }
            r18 = r18 + 0x4;
            r19 = r19 + 0x1;
            r17 = r17 + 0x1;
        } while ((s32)r17 <= 3);
        tmp = r16 & 0xFF;
        if (tmp == 0) {
            ((void(*)(void))_threadSwitch)();
            r15 = r15 + 0x1;
            if ((s32)r15 < 0xf) goto L_800874D4;
        }
        r3 = 0x26;
        fn_80166A28();
        r15 = 0x0;
        tmp = *(u32*)((u8*)r24 + 0x0);
        if ((s32)tmp != 7) {
            r15 = 0x1;
            tmp = *(u32*)((u8*)r24 + 0x4);
            if ((s32)tmp != 7) {
                r15 = 0x2;
                tmp = *(u32*)((u8*)r24 + 0x8);
                if ((s32)tmp != 7) {
                    r15 = 0x3;
                    tmp = *(u32*)((u8*)r24 + 0xC);
                    if ((s32)tmp != 7) {
                        r15 = 0x4;
        }
        }
        }
        }
        if ((s32)r15 > 3) {
            tmp = *(u32*)((u8*)r24 + 0x28);
            tmp = tmp & 0x00000008;
            if (tmp == 0) {
                r3 = 0x0;
                goto L_80087630;
            }
        }
        r3 = 0x1;
        ((void(*)(void))menuSetEnablePort)();
        r4 = r15 + 0x1;
        r3 = 0x2f;
        msgctrlSetValue();
        if ((s32)r15 == 0) {
            r3 = 0x7;
            r4 = 0x44c0;
            r5 = 0x1;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
        } else {

            r3 = 0x7;
            r4 = 0x44b8;
            r5 = 0x1;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
        }
        tmp = 0x8;
        r3 = 0x1;
        *(u32*)((u8*)r24 + 0x28) = tmp;
    L_80087630:
        tmp = r3 & 0xFF;
        if (tmp == 0) {
            tmp = *(u8*)((u8*)r24 + 0x21);
            r3 = 0x6;
            tmp = (s8)tmp;
            tmp = tmp << 2;
            *(u32*)(r24 + tmp) = r3;
        }
        tmp = *(u32*)((u8*)r24 + 0x2C);
        if ((s32)tmp == 3) {
            r3 = 0x7;
            r4 = 0x44e7;
            r5 = 0x1;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
        } else {

            r3 = 0x7;
            r4 = 0x44e6;
            r5 = 0x1;
            r6 = 0x0;
            ((void(*)(void))winMsgOpen)();
        }
        r3 = *(u8*)((u8*)r24 + 0x21);
        r3 = (s8)r3;
        ((void(*)(void))fn_80093698)();
        r3 = 0x0;
        return;
    L_8008769C:
        tmp = (s8)r23;
        if ((s32)tmp == 0) {
            r3 = r24;
            r4 = 0x3;
            fn_80087AE8();
            tmp = r3 & 0xFF;
            if (tmp != 0) {
                r3 = 0x1;
                return;
            }
            r3 = 0x1;
            ((void(*)(void))menuSetEnablePort)();
            r15 = 0x0;
        L_800876D4:
            r16 = 0x0;
            r18 = r24;
            r17 = r16;
            r19 = (u32)&lbl_80478950;
            do {
                tmp = *(u32*)((u8*)r18 + 0x0);
                if ((s32)tmp != 5) {
                    if ((s32)tmp == 4) {
                    }
                    r3 = r17 + 0x1;
                    ((void(*)(void))fn_8008ABA0)();
                    tmp = r3 & 0xFF;
                    if (tmp == 0) {
                        ((void(*)(void))menuGetEnablePort)();
                        tmp = *(u8*)((u8*)r19 + 0x0);
                        tmp = r3 & ~tmp;
                        r3 = tmp & 0xFF;
                        ((void(*)(void))menuSetEnablePort)();
                        r3 = 0x7;
                        tmp = 0x8;
                        *(u32*)((u8*)r18 + 0x0) = r3;
                        *(u32*)((u8*)r24 + 0x28) = tmp;
                }
                    }
                tmp = *(u32*)((u8*)r18 + 0x0);
                if ((s32)tmp == 7) {
                    r16 = 0x1;
                }
                r18 = r18 + 0x4;
                r19 = r19 + 0x1;
                r17 = r17 + 0x1;
            } while ((s32)r17 <= 3);
            tmp = r16 & 0xFF;
            if (tmp == 0) {
                ((void(*)(void))_threadSwitch)();
                r15 = r15 + 0x1;
                if ((s32)r15 < 0xf) goto L_800876D4;
            }
            r3 = 0x26;
            fn_80166A28();
            r15 = 0x0;
            tmp = *(u32*)((u8*)r24 + 0x0);
            if ((s32)tmp != 7) {
                r15 = 0x1;
                tmp = *(u32*)((u8*)r24 + 0x4);
                if ((s32)tmp != 7) {
                    r15 = 0x2;
                    tmp = *(u32*)((u8*)r24 + 0x8);
                    if ((s32)tmp != 7) {
                        r15 = 0x3;
                        tmp = *(u32*)((u8*)r24 + 0xC);
                        if ((s32)tmp != 7) {
                            r15 = 0x4;
            }
            }
            }
            }
            if ((s32)r15 > 3) {
                tmp = *(u32*)((u8*)r24 + 0x28);
                tmp = tmp & 0x00000008;
                if (tmp == 0) {
                    r3 = 0x0;
                    goto L_80087830;
            }
            }
            r3 = 0x1;
            ((void(*)(void))menuSetEnablePort)();
            r4 = r15 + 0x1;
            r3 = 0x2f;
            msgctrlSetValue();
            if ((s32)r15 == 0) {
                r3 = 0x7;
                r4 = 0x44c0;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            } else {

                r3 = 0x7;
                r4 = 0x44b8;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            }
            tmp = 0x8;
            r3 = 0x1;
            *(u32*)((u8*)r24 + 0x28) = tmp;
        L_80087830:
            tmp = r3 & 0xFF;
            if (tmp == 0) {
                tmp = *(u8*)((u8*)r24 + 0x21);
                r3 = 0x6;
                tmp = (s8)tmp;
                tmp = tmp << 2;
                *(u32*)(r24 + tmp) = r3;
            }
            tmp = *(u32*)((u8*)r24 + 0x2C);
            if ((s32)tmp == 3) {
                r3 = 0x7;
                r4 = 0x44e7;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            } else {

                r3 = 0x7;
                r4 = 0x44e6;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            }
            r3 = *(u8*)((u8*)r24 + 0x21);
            r3 = (s8)r3;
            ((void(*)(void))fn_80093698)();
            r3 = 0x0;
            return;
        }
    } while (0);
        r4 = 0x7;
        tmp = *(u8*)((u8*)r3 + 0x1);
        r3 = r24;
        *(u8*)((u8*)r24 + 0x21) = tmp;
        fn_80087AE8();
        tmp = r3 & 0xFF;
        if (tmp == 0) {
            r3 = 0x1;
            ((void(*)(void))menuSetEnablePort)();
            r15 = 0x0;
        L_800878CC:
            r16 = 0x0;
            r18 = r24;
            r17 = r16;
            r19 = (u32)&lbl_80478950;
            do {
                tmp = *(u32*)((u8*)r18 + 0x0);
                if ((s32)tmp != 5) {
                    if ((s32)tmp == 4) {
                    }
                    r3 = r17 + 0x1;
                    ((void(*)(void))fn_8008ABA0)();
                    tmp = r3 & 0xFF;
                    if (tmp == 0) {
                        ((void(*)(void))menuGetEnablePort)();
                        tmp = *(u8*)((u8*)r19 + 0x0);
                        tmp = r3 & ~tmp;
                        r3 = tmp & 0xFF;
                        ((void(*)(void))menuSetEnablePort)();
                        r3 = 0x7;
                        tmp = 0x8;
                        *(u32*)((u8*)r18 + 0x0) = r3;
                        *(u32*)((u8*)r24 + 0x28) = tmp;
                    }
                    }
                tmp = *(u32*)((u8*)r18 + 0x0);
                if ((s32)tmp == 7) {
                    r16 = 0x1;
                }
                r18 = r18 + 0x4;
                r19 = r19 + 0x1;
                r17 = r17 + 0x1;
            } while ((s32)r17 <= 3);
            tmp = r16 & 0xFF;
            if (tmp == 0) {
                ((void(*)(void))_threadSwitch)();
                r15 = r15 + 0x1;
                if ((s32)r15 < 0xf) goto L_800878CC;
            }
            r3 = 0x26;
            fn_80166A28();
            r15 = 0x0;
            tmp = *(u32*)((u8*)r24 + 0x0);
            if ((s32)tmp != 7) {
                r15 = 0x1;
                tmp = *(u32*)((u8*)r24 + 0x4);
                if ((s32)tmp != 7) {
                    r15 = 0x2;
                    tmp = *(u32*)((u8*)r24 + 0x8);
                    if ((s32)tmp != 7) {
                        r15 = 0x3;
                        tmp = *(u32*)((u8*)r24 + 0xC);
                        if ((s32)tmp != 7) {
                            r15 = 0x4;
            }
            }
            }
            }
            if ((s32)r15 > 3) {
                tmp = *(u32*)((u8*)r24 + 0x28);
                tmp = tmp & 0x00000008;
                if (tmp == 0) {
                    r3 = 0x0;
                    goto L_80087A28;
                }
            }
            r3 = 0x1;
            ((void(*)(void))menuSetEnablePort)();
            r4 = r15 + 0x1;
            r3 = 0x2f;
            msgctrlSetValue();
            if ((s32)r15 == 0) {
                r3 = 0x7;
                r4 = 0x44c0;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            } else {

                r3 = 0x7;
                r4 = 0x44b8;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            }
            tmp = 0x8;
            r3 = 0x1;
            *(u32*)((u8*)r24 + 0x28) = tmp;
        L_80087A28:
            tmp = r3 & 0xFF;
            if (tmp == 0) {
                tmp = *(u8*)((u8*)r24 + 0x21);
                r3 = 0x6;
                tmp = (s8)tmp;
                tmp = tmp << 2;
                *(u32*)(r24 + tmp) = r3;
            }
            tmp = *(u32*)((u8*)r24 + 0x2C);
            if ((s32)tmp == 3) {
                r3 = 0x7;
                r4 = 0x44e7;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            } else {

                r3 = 0x7;
                r4 = 0x44e6;
                r5 = 0x1;
                r6 = 0x0;
                ((void(*)(void))winMsgOpen)();
            }
            r3 = *(u8*)((u8*)r24 + 0x21);
            r3 = (s8)r3;
            ((void(*)(void))fn_80093698)();
            r3 = 0x0;
            return;
        }
        r25 = r25 + 0x1;
        r4 = r4 + 0x1;
    } while (r25 < 4);

    return;
}
