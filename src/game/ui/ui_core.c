/**
 * @file ui_core.c
 * @brief UI Core state machine and dispatch (0x80059BDC-0x80069A60)
 *
 * Address range: 0x80059BDC - 0x80069A60
 * Total functions: 101
 */

#include "dolphin/types.h"

/* ===== External function declarations ===== */
extern void fn_8001DA60();
extern void fn_8001E074();
extern void fn_8001E58C();
extern void fn_8002D91C();
extern void fn_800347B8();
extern void fn_800347C4();
extern void fn_800347E8();
extern void fn_8003480C();
extern void fn_80069C0C();
extern void fn_8006A76C();
extern void fn_8006A79C();
extern void fn_8006A7AC();
extern void fn_8006A7BC();
extern void fn_8006A7C8();
extern void fn_8006A7D0();
extern void fn_8006A7E0();
extern void fn_8006A7E8();
extern void fn_8006A7F0();
extern void fn_8006A814();
extern void fn_8006A81C();
extern void fn_8006A824();
extern void fn_8006AC28();
extern void fn_8006AC6C();
extern void fn_8006ACCC();
extern void fn_8006ADB4();
extern void fn_8006ADEC();
extern void fn_8006AF44();
extern void fn_8006AFC4();
extern void fn_8006AFE4();
extern void fn_8006B09C();
extern void fn_8006B0F8();
extern void fn_8006B1D4();
extern void fn_8006B1F4();
extern void fn_8006B2A4();
extern void fn_8006B354();
extern void fn_8006B3C8();
extern void fn_8006B420();
extern void fn_8006B4AC();
extern void fn_8006B51C();
extern void fn_8006B5D0();
extern void fn_8006B8E8();
extern void fn_8006B8F0();
extern void fn_8006B8FC();
extern void fn_8006E0CC();
extern void fn_80071160();
extern void fn_80071344();
extern void fn_80071398();
extern void fn_800714C8();
extern void fn_800715BC();
extern void fn_8007162C();
extern void fn_80076054();
extern void fn_800776E4();
extern void fn_80077E80();
extern void fn_80077EA4();
extern void fn_800849B4();
extern void fn_800886D0();
extern void fn_80088964();
extern void fn_800889A4();
extern void fn_800889E4();
extern void fn_80088C60();
/* ... and 173 more external functions */
extern void* memset(void* dst, int val, u32 size);
extern void* memcpy(void* dst, const void* src, u32 size);

/* ===== SDA globals ===== */
extern u8 lbl_80478848;
extern u8 lbl_80478900;
extern u8 lbl_80478908;
extern u8 lbl_80478910;
extern u8 lbl_80478918;
extern u8 lbl_8047891A;
extern u8 lbl_8047891C;
extern u8 lbl_80478920;
extern u8 lbl_80478922;
extern u8 lbl_80478968;
extern u8 lbl_80478BD8;
extern u8 lbl_80478E90;
extern u8 lbl_80478E94;
extern u8 lbl_8047A5A0;
extern u8 lbl_8047A5A8;
extern u8 lbl_8047A5B0;
extern u8 lbl_8047A5B4;
extern u8 lbl_8047A5B8;
extern u8 lbl_8047A5BC;
extern u8 lbl_8047A5C0;
extern u8 lbl_8047A5C4;
extern u8 lbl_8047A5C8;
extern u8 lbl_8047A5D0;
extern u8 lbl_8047BF18;
extern u8 lbl_8047BF1C;
extern u8 lbl_8047BF20;
extern u8 lbl_8047BF24;
extern u8 lbl_8047BF28;
extern u8 lbl_8047BF30;
extern u8 lbl_8047BF34;
extern u8 lbl_8047BF38;
extern u8 lbl_8047BF3C;
extern u8 lbl_8047BF40;
extern u8 lbl_8047BF48;
extern u8 lbl_8047BF50;
extern u8 lbl_8047BF54;
extern u8 lbl_8047BF58;
extern u8 lbl_8047BF5C;
extern u8 lbl_8047BF60;
extern u8 lbl_8047BF64;
extern u8 lbl_8047BF68;
extern u8 lbl_8047BF6C;
extern u8 lbl_8047BF70;
extern u8 lbl_8047BF74;
extern u8 lbl_8047BF78;
extern u8 lbl_8047BF7C;
extern u8 lbl_8047BF80;
extern u8 lbl_8047BF88;
extern u8 lbl_8047BF90;
extern u8 lbl_8047BF94;
extern u8 lbl_8047BF98;
extern u8 lbl_8047BF9C;
extern u8 lbl_8047BFA0;
extern u8 lbl_8047BFA4;
extern u8 lbl_8047BFA8;
extern u8 lbl_8047BFAC;
extern u8 lbl_8047BFB0;
extern u8 lbl_8047BFB4;
extern u8 lbl_8047BFB8;
extern u8 lbl_8047BFBC;
extern u8 lbl_8047BFC0;
extern u8 lbl_8047BFC4;
extern u8 lbl_8047BFC8;
extern u8 lbl_8047BFCC;
extern u8 lbl_8047BFD0;
extern u8 lbl_8047BFD4;
extern u8 lbl_8047BFD8;
extern u8 lbl_8047BFE0;
extern u8 lbl_8047BFE8;
extern u8 lbl_8047BFEC;
extern u8 lbl_8047BFF0;
extern u8 lbl_8047BFF8;
extern u8 lbl_8047BFFC;
extern u8 lbl_8047C000;
extern u8 lbl_8047C004;
extern u8 lbl_8047C008;
extern u8 lbl_8047C00C;
extern u8 lbl_8047C010;
extern u8 lbl_8047C014;
extern u8 lbl_8047C018;
extern u8 lbl_8047C020;

/* ===== Rodata / data labels ===== */
extern u8 jumptable_802E62B0[];
extern u8 jumptable_802ED9B8[];
extern u8 jumptable_802EDB7C[];
extern u8 lbl_80267840[];
extern u8 lbl_802678D8[];
extern u8 lbl_80267A20[];
extern u8 lbl_80267A80[];
extern u8 lbl_80267A98[];
extern u8 lbl_80267AB0[];
extern u8 lbl_80267AF8[];
extern u8 lbl_80267B88[];
extern u8 lbl_802E2DB8[];
extern u8 lbl_802E6428[];
extern u8 lbl_802E7CE8[];
extern u8 lbl_802ED958[];
extern u8 lbl_802ED978[];
extern u8 lbl_802ED9A0[];
extern u8 lbl_802ED9F0[];
extern u8 lbl_802ED9FC[];
extern u8 lbl_802EDA20[];
extern u8 lbl_802EDB40[];
extern u8 lbl_802EDB64[];
extern u8 lbl_802EF0A8[];
extern u8 lbl_80314E08[];
extern u8 lbl_80314F98[];
extern u8 lbl_803A9A60[];
extern u8 lbl_803A9E40[];
extern u8 lbl_803A9EA0[];
extern u8 lbl_803A9F08[];

/* ===== Forward declarations ===== */
s32 fn_80059BDC(void);
s32 fn_8005CCD0(void);
s32 fn_8005CD88(void);
s32 fn_8005CEE8(void);
s32 fn_8005CF2C(void);
s32 fn_8005D094(void);
s32 fn_8005D0B8(void);
s32 fn_8005D0DC(void);
s32 fn_8005D130(void);
s32 fn_8005D184(void);
s32 fn_8005D26C(void);
s32 fn_8005D2E8(void);
s32 fn_8005D3D0(void);
s32 fn_8005D4AC(void);
s32 fn_8005D4F4(void);
s32 fn_8005D53C(void);
s32 fn_8005D584(void);
s32 fn_8005D5CC(void);
s32 fn_8005D6A8(void);
s32 fn_8005D738(void);
void fn_8005D798(void);
s32 fn_8005D7F8(void);
s32 fn_8005D830(void);
s32 fn_8005D858(void);
s32 fn_8005D880(void);
s32 fn_8005D8B8(void);
s32 fn_8005D8F8(void);
s32 fn_8005D934(void);
s32 fn_8005D95C(void);
s32 fn_8005D9AC(void);
s32 fn_8005D9E4(void);
s32 fn_8005DA18(void);
s32 fn_8005DA48(void);
s32 fn_8005DBC4(void);
s32 fn_8005DC24(void);
s32 fn_8005DCC4(void);
s32 fn_8005DEE8(void);
s32 fn_8005DFC8(void);
void fn_8005E690(void);
void fn_8005E730(void);
s32 fn_8005E750(void);
void fn_8005E7F0(void);
s32 fn_8005FFE4(void);
s32 fn_80060434(void);
s32 fn_800608C4(void);
void fn_800609B4(void);
void fn_80060A28(void);
s32 fn_80060D70(void);
void fn_80060EF4(void);
void fn_80061018(void);
s32 fn_80061028(void);
s32 fn_8006106C(void);
s32 fn_80061240(void);
s32 fn_80061454(void);
s32 fn_800615F4(void);
s32 fn_800617E0(void);
s32 fn_80061A2C(void);
void fn_80061B74(void);
s32 fn_80061BBC(void);
void fn_80061D34(void);
s32 fn_80061F6C(void);
s32 fn_80062284(void);
void fn_80062334(void);
s32 fn_800626CC(void);
void fn_80062834(void);
s32 fn_80062948(void);
s32 fn_80062AB4(void);
s32 fn_80063060(void);
s32 fn_800637B0(void);
s32 fn_800638F4(void);
s32 fn_80063AD4(void);
void fn_80063D10(void);
s32 fn_80063D14(void);
s32 fn_80064378(void);
s32 fn_800643D4(void);
void fn_80065628(void);
void fn_80065730(void);
void fn_80065838(void);
void fn_80065940(void);
s32 fn_80065A48(void);
s32 fn_800676EC(void);
s32 fn_800679C0(void);
s32 fn_800681B4(void);
s32 fn_80068418(void);
s32 fn_80068738(void);
s32 fn_80068794(void);
s32 fn_800688C4(void);
void fn_800689FC(void);
s32 fn_80068BB0(void);
s32 fn_80068DBC(void);
void fn_80068F84(void);
void fn_80069048(void);
s32 fn_8006905C(void);
s32 fn_80069220(void);
s32 fn_800693A4(void);
s32 fn_80069504(void);
s32 fn_80069664(void);
s32 fn_800697C4(void);
void fn_800697F4(void);
void fn_80069944(void);
s32 fn_80069A08(void);

/* ===== Function implementations ===== */


/* 0x80059BDC | size: 0x30F4 */
s32 fn_80059BDC(void) {
    extern void fn_80062948();
    extern void fn_80088D84();
    extern void fn_80089028();
    extern void fn_8008ABA0();
    extern void fn_80092C90();
    extern void fn_80093574();
    extern void fn_800F0308();
    extern void fn_800F7EF8();
    extern void fn_800F9EE4();
    extern void fn_800FF540();
    extern void fn_801022B8();
    extern void fn_80102510();
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_8010264C();
    extern void fn_801026A4();
    extern void fn_80102868();
    extern void fn_801046B8();
    extern void fn_80104704();
    extern void fn_80105624();
    extern void fn_801069FC();
    extern void fn_80106D3C();
    extern void fn_80108518();
    extern void fn_80113778();
    extern void fn_80113828();
    extern void fn_80129280();
    extern void fn_80129384();
    extern void fn_801293FC();
    extern void fn_8012A248();
    extern void fn_8012A774();
    extern void fn_8012AC3C();
    extern void fn_8012AC54();
    extern void fn_8012AC64();
    extern void fn_80130054();
    extern void fn_80132A38();
    extern void fn_80135168();
    extern void fn_80166A28();
    extern void fn_801906A0();
    extern void fn_8019075C();
    extern void fn_80196E10();
    extern void fn_801D04E8();
    extern u8 jumptable_802E62B0[];
    u8 sp[0x220];
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
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    
    *(f64*)(sp + 0x210) = f31;
    /* psq_st f31, 0x218((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x200) = f30;
    /* psq_st f30, 0x208((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x1F0) = f29;
    /* psq_st f29, 0x1f8((u32)sp), 0, qr0 */;
    r4 = (u32)&lbl_80267840;
    r3 = 0x0;
    r31 = (u32)&lbl_80267840;
    r4 = 0xe;
    fn_80129280();
    r27 = 0x0;
    r28 = 0x0;
    ((void(*)(void))fn_8007162C)();
    r29 = 0x0;
    r22 = r3;
    fn_800FF540();
    if ((u32)r3 != (u32)0x0) {
        r3 = r31 + 0x98;
        r5 = r31 + 0xb0;
        r4 = 0x267;
        fn_80196E10();
    }
    r3 = 0x8ae;
    fn_801906A0();
    if ((u32)r3 == (u32)0x0) goto L_8005CBD8;
    r3 = r31 + 0x98;
    r5 = r31 + 0xd0;
    r4 = 0x268;
    fn_80196E10();
    goto L_8005CBD8;
    L_80059C6C: ;
    ((void(*)(void))fn_8007162C)();
    r30 = r3;
    ((void(*)(void))fn_8007162C)();
    r26 = r3;
    r9 = r30;
    r3 = 0xbe;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x10;
    r7 = 0x0;
    r8 = 0x1;
    /* crclr cr1eq */;
    fn_801026A4();
    ((void(*)(void))fn_8007162C)();
    /* subi r0, r3, 0xa8 */;
    if ((u32)r0 > (u32)0x5d) goto L_8005CAC0;
    r3 = (u32)jumptable_802E62B0;
    r0 = r0 << 2;
    r3 = (u32)jumptable_802E62B0;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    ((void(*)(void))fn_80071344)();
    if ((s32)r3 < (s32)0x0) {
        r26 = -0x1;
        goto L_8005CAC0;
    }
    r0 = r3 << 2;
    r3 = r31 + 0x0;
    r26 = *(u32*)(r3 + r0);
    if ((s32)r3 != (s32)0x1) {
        goto L_8005CAC0;
    }
    r29 = 0x1;
    goto L_8005CAC0;
    ((void(*)(void))fn_80071344)();
    if ((s32)r3 != (s32)0x1) {
        if ((s32)r3 < (s32)0x1) {
            if ((s32)r3 < (s32)0x0) {
                goto L_80059DC0;
            }
            goto L_80059DC0;
            }
        r26 = 0xac;
        r24 = 0x0;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x4) = r24;
        r24 = 0x0;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x10) = r24;
        r24 = 0x0;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x0) = r24;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r3 = r3 + 0x59a8;
        r4 = 0x0;
        ((void(*)(void))fn_8006A7E0)();
        goto L_8005CAC0;
    }
    r26 = 0xb3;
    r24 = 0x2;
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    *(u32*)((u8*)r3 + 0x4) = r24;
    r24 = 0x4;
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    *(u32*)((u8*)r3 + 0x10) = r24;
    r24 = 0x2;
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    *(u32*)((u8*)r3 + 0x0) = r24;
    goto L_8005CAC0;
    L_80059DC0: ;
    r26 = -0x1;
    goto L_8005CAC0;
    r3 = 0x0;
    ((void(*)(void))fn_8006B4AC)();
    ((void(*)(void))fn_80071344)();
    if ((s32)r3 < (s32)0x0) {
        r26 = -0x1;
        goto L_8005CAC0;
    }
    r0 = r3 << 2;
    r3 = r31 + 0x18;
    r22 = *(u32*)(r3 + r0);
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    ((void(*)(void))fn_8006AFC4)();
    if ((u32)r3 != (u32)0x0 || (s32)r22 != (s32)0xae && (s32)r22 != (s32)0xaf || (s32)r22 != (s32)0xae && (s32)r22 != (s32)0xaf || (s32)r22 != (s32)0xae && (s32)r22 != (s32)0xaf || (s32)r22 != (s32)0xae && (s32)r22 != (s32)0xaf || (s32)r22 != (s32)0xae && (s32)r22 != (s32)0xaf) goto L_80059E38;

    if ((s32)r22 != (s32)0xae && (s32)r22 != (s32)0xaf) goto L_80059E38;

    r3 = 0x2;
    r4 = 0x3bfe;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    r3 = 0x1;
    fn_801069FC();
    goto L_8005CAC0;
    L_80059E38: ;
    if ((s32)r22 != (s32)0xae) {
        if ((s32)r22 < (s32)0xae) {
            if ((s32)r22 < (s32)0xad) {
                goto L_80059F18;
            }
            if ((s32)r22 >= (s32)0xb0) goto L_80059F18;
            goto L_80059E78;
        }
        r24 = 0x0;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x0) = r24;
        r26 = r22;
        goto L_8005CAC0;
        L_80059E78: ;
        r24 = 0x1;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x0) = r24;
        r24 = 0x6;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0xC) = r24;
        r24 = 0x2;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x8) = r24;
        r26 = r22;
        goto L_8005CAC0;
            }
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    ((void(*)(void))fn_8006A7BC)();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        ((void(*)(void))fn_8006AFC4)();
        if ((u32)r3 == (u32)0x0) {
        }
        r3 = 0x2;
        r4 = 0x4415;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        r3 = 0x1;
        fn_801069FC();
        goto L_8005CAC0;
        }
    r26 = r22;
    goto L_8005CAC0;
    L_80059F18: ;
    r26 = r22;
    goto L_8005CAC0;
    r24 = 0x0;
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    ((void(*)(void))fn_8006A7BC)();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        ((void(*)(void))fn_8006AFC4)();
        if ((u32)r3 != (u32)0x0) {
            r24 = 0x1;
    }
    }
    if ((s32)r24 == (s32)0x0) {
        r3 = r31 + 0x98;
        r5 = r31 + 0x10c;
        r4 = 0x30f;
        fn_80196E10();
    }
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r26 = r3 + (0x1 << 16);
    r3 = 0x0;
    /* subi r26, r26, 0x3674 */;
    r4 = 0xe;
    fn_80129280();
    ((void(*)(void))fn_8006AFC4)();
    r25 = r3;
    r3 = 0xc8;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0xc8;
        fn_80102510();
        while (1) {
            r3 = 0xc8;
            fn_80102620();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x0) break;
            fn_800F0308();


        }
    }
    r3 = r25;
    ((void(*)(void))fn_8006A7E8)();
    r24 = r3;
    r3 = r25;
    ((void(*)(void))fn_8006A7C8)();
    r0 = 0x0;
    r9 = r3;
    *(u32*)(sp + 0x8) = r0;
    r10 = r24;
    r3 = 0xc8;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x10;
    r7 = 0x0;
    r8 = 0x4;
    /* crclr cr1eq */;
    fn_801026A4();
    r3 = 0xd6;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0xd6;
        fn_80102510();
        while (1) {
            r3 = 0xd6;
            fn_80102620();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x0) break;
            fn_800F0308();


        }
    }
    r0 = 0x1;
    r4 = 0x3d89;
    *(u32*)(sp + 0x24) = r0;
    r0 = 0x0;
    r5 = (u32)sp + 0x24;
    r3 = 0xd6;
    r4 = 0x0;
    r6 = 0x10;
    r7 = 0x1;
    *(u32*)(sp + 0xC) = r0;
    r8 = 0x4;
    r9 = 0x3db0;
    r10 = 0x3db1;
    /* crclr cr1eq */;
    fn_801026A4();
    r24 = r3;
    r3 = 0xd6;
    fn_80102510();
    if ((s32)r24 != (s32)0x0) {
        r3 = 0xc8;
        r4 = 0x0;
        r5 = 0x0;
        fn_80102568();
        ((void(*)(void))fn_8006E0CC)();
        r26 = -0x1;
        goto L_8005CAC0;
    }
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    ((void(*)(void))fn_8006AFC4)();
    r24 = r3;
    r3 = 0x1;
    ((void(*)(void))fn_8006AFE4)();
    r4 = r24;
    ((void(*)(void))fn_8006A7F0)();
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    ((void(*)(void))fn_8006A76C)();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        ((void(*)(void))fn_8006A79C)();
        goto L_8005A19C;
    }
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    ((void(*)(void))fn_8006A7BC)();
    r24 = r3;
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    ((void(*)(void))fn_8006A79C)();
    r3 = 0x2;
    r4 = 0x44d9;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    r3 = 0x1;
    fn_801069FC();
    ((void(*)(void))fn_80088C60)();
    if ((s32)r3 >= (s32)0x0) goto L_8005A19C;
    r0 = r24 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        ((void(*)(void))fn_8006A7AC)();
    }
    r3 = 0xc8;
    r4 = 0x0;
    r5 = 0x0;
    fn_80102568();
    ((void(*)(void))fn_8006E0CC)();
    r26 = -0x1;
    goto L_8005CAC0;
    L_8005A19C: ;
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r3 = r3 + (0x1 << 16);
    r0 = *(u32*)((u8*)r3 + (-13940));
    if ((s32)r0 != (s32)0x1) {
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r25 = r3 + (0x1 << 16);
        r3 = 0x0;
        r24 = *(u32*)((u8*)r25 + (-13940));
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x0) = r24;
        r3 = 0x0;
        r4 = 0xe;
        r24 = *(u32*)((u8*)r25 + (-13936));
        fn_80129280();
        *(u32*)((u8*)r3 + 0x4) = r24;
        r3 = 0x0;
        r4 = 0xe;
        r24 = *(u32*)((u8*)r25 + (-13932));
        fn_80129280();
        *(u32*)((u8*)r3 + 0x8) = r24;
        r3 = 0x0;
        r4 = 0xe;
        r24 = *(u32*)((u8*)r25 + (-13928));
        fn_80129280();
        *(u32*)((u8*)r3 + 0xC) = r24;
        r3 = 0x0;
        r4 = 0xe;
        r24 = *(u32*)((u8*)r25 + (-13924));
        fn_80129280();
        *(u32*)((u8*)r3 + 0x10) = r24;
        r3 = 0x0;
        r4 = 0xe;
        r24 = *(u32*)((u8*)r25 + (-13920));
        fn_80129280();
        *(u32*)((u8*)r3 + 0x14) = r24;
        r3 = 0x0;
        r4 = 0xe;
        r24 = *(u32*)((u8*)r25 + (-13916));
        fn_80129280();
        *(u32*)((u8*)r3 + 0x18) = r24;
        r0 = *(u32*)((u8*)r25 + (-13920));
        if ((u32)r0 == (u32)0x0) {
            r3 = 0x0;
            ((void(*)(void))fn_8006ADB4)();
        }
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r0 = *(u32*)((u8*)r3 + 0x0);
        if ((s32)r0 != (s32)0x0) {
            r3 = r31 + 0x98;
            r5 = r31 + 0x158;
            r4 = 0xab;
            fn_80196E10();
        }
        r0 = *(u32*)((u8*)r25 + (-13936));
        r3 = 0x8ae;
        if ((s32)r0 == (s32)0x0) {
            r4 = 0x1;
        } else {

            r4 = 0x2;
        }
        fn_8019075C();
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        ((void(*)(void))fn_80069C0C)();
        r26 = 0xd1;
    } else {

        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r25 = r3 + (0x1 << 16);
        r3 = 0x0;
        r24 = *(u32*)((u8*)r25 + (-13940));
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x0) = r24;
        r3 = 0x0;
        r4 = 0xe;
        r24 = *(u32*)((u8*)r25 + (-13936));
        fn_80129280();
        *(u32*)((u8*)r3 + 0x4) = r24;
        r3 = 0x0;
        r4 = 0xe;
        r24 = *(u32*)((u8*)r25 + (-13932));
        fn_80129280();
        *(u32*)((u8*)r3 + 0x8) = r24;
        r3 = 0x0;
        r4 = 0xe;
        r24 = *(u32*)((u8*)r25 + (-13928));
        fn_80129280();
        *(u32*)((u8*)r3 + 0xC) = r24;
        r3 = 0x0;
        r4 = 0xe;
        r24 = *(u32*)((u8*)r25 + (-13924));
        fn_80129280();
        *(u32*)((u8*)r3 + 0x10) = r24;
        r3 = 0x0;
        r4 = 0xe;
        r24 = *(u32*)((u8*)r25 + (-13920));
        fn_80129280();
        *(u32*)((u8*)r3 + 0x14) = r24;
        r3 = 0x0;
        r4 = 0xe;
        r24 = *(u32*)((u8*)r25 + (-13916));
        fn_80129280();
        *(u32*)((u8*)r3 + 0x18) = r24;
        r0 = *(u32*)((u8*)r25 + (-13920));
        if ((u32)r0 == (u32)0x0) {
            r3 = 0x0;
            ((void(*)(void))fn_8006ADB4)();
        }
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r0 = *(u32*)((u8*)r3 + 0xC);
        if ((s32)r0 != (s32)0x6) {
            r3 = r31 + 0x98;
            r5 = r31 + 0x184;
            r4 = 0x81;
            fn_80196E10();
        }
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r0 = *(u32*)((u8*)r3 + 0x0);
        if ((s32)r0 != (s32)0x1) {
            r3 = r31 + 0x98;
            r5 = r31 + 0x1b0;
            r4 = 0x82;
            fn_80196E10();
        }
        r0 = *(u32*)((u8*)r25 + (-13936));
        r3 = 0x8ae;
        if ((s32)r0 == (s32)0x0) {
            r4 = 0x1;
        } else {

            r4 = 0x2;
        }
        fn_8019075C();
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r4 = r3;
        r3 = 0xb59;
        r4 = *(u32*)((u8*)r4 + 0x14);
        fn_8019075C();
        r3 = 0xafc;
        r4 = 0x0;
        fn_8019075C();
        r3 = 0xb11;
        r4 = 0x0;
        fn_8019075C();
        r3 = 0xde1;
        r4 = 0x0;
        fn_8019075C();
        fn_80130054();
        r4 = *(u32*)((u8*)r25 + (-13876));
        r3 = 0xafc;
        fn_8019075C();
        r4 = *(u32*)((u8*)r25 + (-13868));
        r3 = 0xb11;
        fn_8019075C();
        r4 = *(u32*)((u8*)r25 + (-13864));
        r3 = 0xde1;
        fn_8019075C();
        r29 = *(u32*)((u8*)r25 + (-13908));
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r4 = r3 + (0x1 << 16);
        r3 = 0x0;
        f31 = *(f32*)((u8*)r4 + (-13900));
        r4 = 0xe;
        fn_80129280();
        r4 = r3 + (0x1 << 16);
        r3 = 0x0;
        f30 = *(f32*)((u8*)r4 + (-13896));
        r4 = 0xe;
        fn_80129280();
        r4 = r3 + (0x1 << 16);
        r3 = 0x0;
        f29 = *(f32*)((u8*)r4 + (-13892));
        r4 = 0xe;
        fn_80129280();
        r3 = r3 + (0x1 << 16);
        f0 = *(f32*)&lbl_8047BF18;
        f1 = *(f32*)((u8*)r3 + (-13884));
        r27 = 0x1;
        r26 = 0x105;
        f0 = f0 * f1;
        f0 = (f64)(s32)f0;
        *(f64*)(sp + 0x1C0) = f0;
    }
    r3 = 0xc8;
    r4 = 0x0;
    r5 = 0x0;
    fn_80102568();
    ((void(*)(void))fn_8006E0CC)();
    goto L_8005CAC0;
    r24 = 0x2;
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    *(u32*)((u8*)r3 + 0x8) = r24;
    r24 = 0x0;
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    *(u32*)((u8*)r3 + 0x0) = r24;
    ((void(*)(void))fn_80071344)();
    r24 = r3;
    if ((s32)r24 != (s32)0x6) {
        if ((s32)r24 >= (s32)0x6) goto L_8005A550;
        if ((s32)r24 < (s32)0x0) {
            goto L_8005A550;
        }
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r0 = r24 << 2;
        r4 = r31 + 0x30;
        *(u32*)((u8*)r3 + 0xC) = r24;
        r3 = *(u32*)(r4 + r0);
        ((void(*)(void))fn_8006B4AC)();
        r26 = 0xaf;
        goto L_8005CAC0;
    }
    r26 = 0xc0;
    goto L_8005CAC0;
    L_8005A550: ;
    r26 = -0x1;
    goto L_8005CAC0;
    ((void(*)(void))fn_80071344)();
    r24 = r3;
    if ((s32)r24 < (s32)0x0) {
        r26 = -0x1;

    } else if ((s32)r24 == (s32)0x2) {
        r26 = -0x1;

    }
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    *(u32*)((u8*)r3 + 0x4) = r24;
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((s32)r0 == (s32)0x3) {
        r0 = 0x1 - r24;
        r0 = __cntlzw(r0);
        r0 = (u32)r0 >> 5;
        r22 = r0 & 0xFF;
        ((void(*)(void))fn_8007162C)();
        r3 = r3 & 0xFFFF;
        fn_80104704();
        if ((u32)r3 != (u32)0x0) {
            r25 = *(u32*)((u8*)r3 + 0x20);
        } else {

            r25 = 0x0;
        }
        r3 = r25 + 0xc;
        r4 = 0x1ce;
        fn_80108518();
        r9 = r22;
        r3 = 0xbc;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x10;
        r7 = 0x1;
        r8 = 0x1;
        /* crclr cr1eq */;
        fn_801026A4();
        r24 = r3;
        if ((s32)r24 >= (s32)0x4) goto L_8005A670;
        if ((s32)r24 < (s32)0x0) {
            goto L_8005A670;
        }
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x10) = r24;
        r24 = 0x0;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x8) = r24;
        r24 = 0x0;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0xC) = r24;
        r26 = 0xcc;
        r3 = 0xbc;
        r4 = 0x0;
        r5 = 0x0;
        fn_80102568();
        goto L_8005CAC0;
        L_8005A670: ;
        r3 = 0xbc;
        r4 = 0x0;
        r5 = 0x1;
        fn_80102568();
        r3 = r25 + 0xc;
        r4 = 0x1ca;
        fn_80108518();
        goto L_8005CAC0;
    }
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r0 = *(u32*)((u8*)r3 + 0x10);
    if ((s32)r0 == (s32)0x4) {
        r26 = 0xbf;
        goto L_8005CAC0;
    }
    r26 = 0xb1;
    goto L_8005CAC0;
    r3 = r31 + 0x98;
    r4 = 0x3c4;
    r5 = (u32)&lbl_8047BF1C;
    fn_80196E10();
    goto L_8005CAC0;
    ((void(*)(void))fn_80071344)();
    if ((s32)r3 != (s32)0x1) {
        if ((s32)r3 >= (s32)0x1) goto L_8005A778;
        if ((s32)r3 < (s32)0x0) {
            goto L_8005A778;
        }
        fn_801D04E8();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x0) {
            r3 = 0x2;
            r4 = 0x44eb;
            r5 = 0x1;
            r6 = 0x0;
            fn_80106D3C();
            r3 = 0x1;
            fn_801069FC();
            goto L_8005CAC0;
        }
        r3 = 0x0;
        r4 = 0x4;
        fn_80135168();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x2;
            r4 = 0x444d;
            r5 = 0x1;
            r6 = 0x0;
            fn_80106D3C();
            r3 = 0x1;
            fn_801069FC();
            goto L_8005CAC0;
        }
        r3 = 0x0;
        r4 = 0x2;
        fn_80129280();
        r5 = *(u32*)&lbl_8047A5A0;
        r4 = r3;
        r3 = r5 + 0x1660;
        fn_8012AC64();
        r26 = 0xee;
        goto L_8005CAC0;
    }
    r26 = 0xed;
    goto L_8005CAC0;
    L_8005A778: ;
    r26 = -0x1;
    goto L_8005CAC0;
    r4 = *(u32*)&lbl_8047A5A0;
    r3 = 0x2;
    r22 = r4 + 0x1660;
    ((void(*)(void))fn_8006B4AC)();
    r3 = r22;
    ((void(*)(void))fn_800776E4)();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r3 = 0xbe;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x10;
        r7 = 0x0;
        r8 = 0x1;
        r9 = 0xf5;
        /* crclr cr1eq */;
        fn_801026A4();
        ((void(*)(void))fn_8006B420)();
        r0 = 0x0;
        r9 = r22;
        r3 = 0xda;
        *(u32*)(sp + 0xC) = r0;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x10;
        r7 = 0x0;
        r8 = 0x4;
        r10 = 0x0;
        /* crclr cr1eq */;
        fn_801026A4();
        r3 = 0xda;
        r4 = 0x0;
        r5 = -0x28;
        fn_80102868();
        ((void(*)(void))fn_8006B420)();
        r4 = r3;
        r3 = r22;
        ((void(*)(void))fn_80076054)();
        r24 = r3;
        r0 = r24 & 0xFFFF;
        if ((u32)r0 == (u32)0x0) {
            r3 = r31 + 0x98;
            r5 = r31 + 0x1e0;
            r4 = 0x1bb;
            fn_80196E10();
        }
        r3 = 0x26;
        fn_80166A28();
        r4 = r24 & 0xFFFF;
        r3 = 0x7;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        r3 = 0x7;
        r4 = 0x440a;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        r3 = 0xda;
        r4 = 0x0;
        r5 = 0x0;
        fn_80102568();
        r3 = 0xbe;
        r4 = 0x0;
        r5 = 0x1;
        fn_80102568();
        r3 = 0xda;
        r4 = 0x0;
        r5 = 0x1;
        fn_80102568();
        ((void(*)(void))fn_8006E0CC)();
        r3 = 0x0;
        ((void(*)(void))fn_8006B4AC)();
        r3 = 0x1;
        fn_801069FC();
        ((void(*)(void))fn_800714C8)();
        r26 = -0x1;
        goto L_8005CAC0;
    }
    r3 = 0x0;
    ((void(*)(void))fn_8006B4AC)();
    r3 = r22;
    r4 = 0x0;
    fn_8012A774();
    r3 = *(u32*)&lbl_8047A5A0;
    r4 = 0x0;
    ((void(*)(void))fn_8006AC28)();
    r3 = *(u32*)&lbl_8047A5A0;
    r4 = r22;
    ((void(*)(void))fn_8006A824)();
    ((void(*)(void))fn_800714C8)();
    r26 = 0xb2;
    goto L_8005CAC0;
    r24 = 0x0;
    r22 = r24;
    do {
        r0 = *(u32*)&lbl_8047A5A0;
        r3 = r22 + 0x1660;
        r4 = 0x0;
        r5 = 0xb18;
        r3 = r0 + r3;
        memset((void*)r3, (int)r4, (u32)r5);
        r0 = *(u32*)&lbl_8047A5A0;
        r3 = r22 + 0x1660;
        r3 = r0 + r3;
        fn_8012A248();
        r22 = r22 + 0xb18;
        r24 = r24 + 0x1;
    } while ((u32)r24 < (u32)0x4);
    r4 = *(u32*)&lbl_8047A5A0;
    r5 = 0x0;
    r3 = 0x2;
    r0 = r4 + 0x1660;
    *(u32*)(sp + 0x50) = r0;
    ((void(*)(void))fn_8006B4AC)();
    r5 = (u32)sp + 0x4c;
    r3 = 0x0;
    r4 = 0x2;
    r6 = 0x0;
    ((void(*)(void))fn_800849B4)();
    r24 = r3;
    r3 = 0x0;
    ((void(*)(void))fn_8006B4AC)();
    if ((s32)r24 < (s32)0x0) {
        ((void(*)(void))fn_800714C8)();
        r26 = -0x1;
        goto L_8005CAC0;
    }
    r3 = *(u32*)&lbl_8047A5A0;
    r4 = 0x0;
    ((void(*)(void))fn_8006AC28)();
    r3 = *(u32*)&lbl_8047A5A0;
    r4 = r3 + 0x1660;
    ((void(*)(void))fn_8006A824)();
    ((void(*)(void))fn_800714C8)();
    r26 = 0xb2;
    goto L_8005CAC0;
    r25 = *(u32*)&lbl_8047A5A0;
    r3 = 0xda;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0xda;
        fn_80102510();
        while (1) {
            r3 = 0xda;
            fn_80102620();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x0) break;
            fn_800F0308();


        }
    }
    r3 = r25;
    ((void(*)(void))fn_8006A7E8)();
    r24 = r3;
    r3 = r25;
    ((void(*)(void))fn_8006A7C8)();
    r0 = 0x0;
    r9 = r3;
    *(u32*)(sp + 0x8) = r0;
    r10 = r24;
    r3 = 0xda;
    r4 = 0x0;
    *(u32*)(sp + 0xC) = r0;
    r5 = 0x0;
    r6 = 0x10;
    r7 = 0x0;
    r8 = 0x4;
    /* crclr cr1eq */;
    fn_801026A4();
    r3 = 0xd6;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0xd6;
        fn_80102510();
        while (1) {
            r3 = 0xd6;
            fn_80102620();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x0) break;
            fn_800F0308();


        }
    }
    r6 = 0x0;
    r0 = -0x2a;
    r5 = (u32)sp + 0x20;
    r3 = 0xd6;
    r4 = 0x0;
    r6 = 0x10;
    r7 = 0x1;
    r8 = 0x4;
    *(u32*)(sp + 0xC) = r0;
    r9 = 0x3d47;
    r10 = 0x3d49;
    /* crclr cr1eq */;
    fn_801026A4();
    r24 = r3;
    r3 = 0xd6;
    fn_80102510();
    if ((s32)r24 != (s32)0x0) goto L_8005AEA8;
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    ((void(*)(void))fn_8006AFC4)();
    if ((u32)r3 != (u32)0x0) {
        r3 = 0xda;
        r4 = 0x0;
        r5 = 0x0;
        fn_80102568();
        ((void(*)(void))fn_8006E0CC)();
        r3 = 0xbe;
        r4 = 0x0;
        r5 = 0x1;
        fn_80102568();
        r3 = 0xbe;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x10;
        r7 = 0x0;
        r8 = 0x1;
        r9 = 0xeb;
        /* crclr cr1eq */;
        fn_801026A4();
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        ((void(*)(void))fn_8006AFC4)();
        r25 = r3;
        r3 = 0xda;
        fn_80102620();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x0) {
            r3 = 0xda;
            fn_80102510();
            while (1) {
                r3 = 0xda;
                fn_80102620();
                r0 = r3 & 0xFF;
                if ((u32)r0 == (u32)0x0) break;
                fn_800F0308();


            }
        }
        r3 = r25;
        ((void(*)(void))fn_8006A7E8)();
        r24 = r3;
        r3 = r25;
        ((void(*)(void))fn_8006A7C8)();
        r0 = 0x0;
        r9 = r3;
        *(u32*)(sp + 0x8) = r0;
        r10 = r24;
        r3 = 0xda;
        r4 = 0x0;
        *(u32*)(sp + 0xC) = r0;
        r5 = 0x0;
        r6 = 0x10;
        r7 = 0x0;
        r8 = 0x4;
        /* crclr cr1eq */;
        fn_801026A4();
        r3 = 0xd6;
        fn_80102620();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x0) {
            r3 = 0xd6;
            fn_80102510();
            while (1) {
                r3 = 0xd6;
                fn_80102620();
                r0 = r3 & 0xFF;
                if ((u32)r0 == (u32)0x0) break;
                fn_800F0308();


            }
        }
        r0 = 0x1;
        r4 = 0x3c54;
        *(u32*)(sp + 0x1C) = r0;
        r0 = -0x28;
        r5 = (u32)sp + 0x1c;
        r3 = 0xd6;
        r4 = 0x0;
        r6 = 0x10;
        r7 = 0x1;
        *(u32*)(sp + 0xC) = r0;
        r8 = 0x4;
        r9 = 0x3d47;
        r10 = 0x3d49;
        /* crclr cr1eq */;
        fn_801026A4();
        r24 = r3;
        r3 = 0xd6;
        fn_80102510();
        if ((s32)r24 != (s32)0x0) goto L_8005AEA8;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        ((void(*)(void))fn_8006A7BC)();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x0) {
            r3 = 0x2;
            r4 = 0x44c2;
            r5 = 0x1;
            r6 = 0x0;
            fn_80106D3C();
            r3 = 0x0;
            r4 = 0x3c;
            r5 = 0x9e;
            r6 = 0x1;
            ((void(*)(void))fn_8001E074)();
            r22 = (s8)r3;
            r3 = 0x1;
            fn_801069FC();
            if ((s32)r22 != (s32)0x0) goto L_8005AEA8;
        }
        r3 = 0xda;
        r4 = 0x0;
        r5 = 0x0;
        fn_80102568();
        ((void(*)(void))fn_8006E0CC)();
        r3 = 0xbe;
        r4 = 0x0;
        r5 = 0x1;
        fn_80102568();
        r9 = r30;
        r3 = 0xbe;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x10;
        r7 = 0x0;
        r8 = 0x1;
        /* crclr cr1eq */;
        fn_801026A4();
        r25 = *(u32*)&lbl_8047A5A0;
        r3 = 0xda;
        fn_80102620();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x0) {
            r3 = 0xda;
            fn_80102510();
            while (1) {
                r3 = 0xda;
                fn_80102620();
                r0 = r3 & 0xFF;
                if ((u32)r0 == (u32)0x0) break;
                fn_800F0308();


            }
        }
        r3 = r25;
        ((void(*)(void))fn_8006A7E8)();
        r24 = r3;
        r3 = r25;
        ((void(*)(void))fn_8006A7C8)();
        r0 = 0x0;
        r9 = r3;
        *(u32*)(sp + 0x8) = r0;
        r10 = r24;
        r3 = 0xda;
        r4 = 0x0;
        *(u32*)(sp + 0xC) = r0;
        r5 = 0x0;
        r6 = 0x10;
        r7 = 0x0;
        r8 = 0x4;
        /* crclr cr1eq */;
        fn_801026A4();
    }
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r6 = *(u32*)&lbl_8047A5A0;
    r5 = (0x1 << 16);
    r4 = r3;
    r3 = r6 + 0x4318;
    /* subi r5, r5, 0x33d4 */;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r4 = *(u32*)&lbl_8047A5A0;
    ((void(*)(void))fn_8006AF44)();
    fn_801D04E8();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r3 = 0x26;
        fn_80166A28();
        r3 = 0x2;
        r4 = 0x3c60;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        r3 = *(u32*)&lbl_8047A5A0;
        ((void(*)(void))fn_8006A7E8)();
        if ((s32)r3 == (s32)0x0) goto L_8005AE80;
        r3 = 0x2;
        r4 = 0x3d55;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        goto L_8005AE80;
    }
    ((void(*)(void))fn_800889A4)();
    if ((s32)r3 < (s32)0x0) {
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r6 = *(u32*)&lbl_8047A5A0;
        r4 = (0x1 << 16);
        /* subi r5, r4, 0x33d4 */;
        r4 = r6 + 0x4318;
        memcpy((void*)r3, (const void*)r4, (u32)r5);
        r3 = *(u32*)&lbl_8047A5A0;
        ((void(*)(void))fn_8006A7E8)();
        if ((s32)r3 == (s32)0x0) goto L_8005AE80;
        r3 = 0x2;
        r4 = 0x3d55;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        goto L_8005AE80;
    }
    r3 = *(u32*)&lbl_8047A5A0;
    ((void(*)(void))fn_8006A7E8)();
    if ((s32)r3 == (s32)0x0) {
        r3 = 0x2;
        r4 = 0x3c5e;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        goto L_8005AE80;
    }
    r3 = 0x2;
    r4 = 0x3d44;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    L_8005AE80: ;
    r3 = 0xda;
    r4 = 0x0;
    r5 = 0x0;
    fn_80102568();
    ((void(*)(void))fn_8006E0CC)();
    ((void(*)(void))fn_800714C8)();
    r3 = 0x1;
    fn_801069FC();
    r26 = -0x1;
    goto L_8005CAC0;
    L_8005AEA8: ;
    r3 = *(u32*)&lbl_8047A5A0;
    ((void(*)(void))fn_8006A7E8)();
    if ((s32)r3 != (s32)0x0) {
        r3 = 0x2;
        r4 = 0x3d55;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        r3 = 0xda;
        r4 = 0x0;
        r5 = 0x0;
        fn_80102568();
        ((void(*)(void))fn_8006E0CC)();
        r3 = 0x1;
        fn_801069FC();
    }
    r26 = -0x1;
    goto L_8005CAC0;
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    ((void(*)(void))fn_80069C0C)();
    r3 = 0x0;
    ((void(*)(void))fn_8006B09C)();
    r25 = r3;
    r3 = 0xda;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0xda;
        fn_80102510();
        while (1) {
            r3 = 0xda;
            fn_80102620();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x0) break;
            fn_800F0308();


        }
    }
    r3 = r25;
    ((void(*)(void))fn_8006A7E8)();
    r24 = r3;
    r3 = r25;
    ((void(*)(void))fn_8006A7C8)();
    r0 = 0x0;
    r9 = r3;
    *(u32*)(sp + 0x8) = r0;
    r10 = r24;
    r3 = 0xda;
    r4 = 0x0;
    *(u32*)(sp + 0xC) = r0;
    r5 = 0x0;
    r6 = 0x10;
    r7 = 0x0;
    r8 = 0x4;
    /* crclr cr1eq */;
    fn_801026A4();
    r3 = 0xd6;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0xd6;
        fn_80102510();
        while (1) {
            r3 = 0xd6;
            fn_80102620();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x0) break;
            fn_800F0308();


        }
    }
    r6 = 0x0;
    r0 = -0x2a;
    r5 = (u32)sp + 0x18;
    r3 = 0xd6;
    r4 = 0x0;
    r6 = 0x10;
    r7 = 0x1;
    r8 = 0x4;
    *(u32*)(sp + 0xC) = r0;
    r9 = 0x3d47;
    r10 = 0x3d49;
    /* crclr cr1eq */;
    fn_801026A4();
    r24 = r3;
    r3 = 0xd6;
    fn_80102510();
    r3 = 0xda;
    r4 = 0x0;
    r5 = 0x0;
    fn_80102568();
    ((void(*)(void))fn_8006E0CC)();
    if ((s32)r24 != (s32)0x0) {
        r26 = -0x1;
        goto L_8005CAC0;
    }
    r26 = 0xd1;
    goto L_8005CAC0;
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    ((void(*)(void))fn_8006AFC4)();
    r0 = r3;
    r3 = 0x0;
    r26 = r0;
    r4 = 0xe;
    fn_80129280();
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((s32)r0 == (s32)0x2) {
        r3 = r31 + 0x98;
        r5 = r31 + 0x1f4;
        r4 = 0x4b9;
        fn_80196E10();
    }
    if ((u32)r26 == (u32)0x0) {
        r3 = r31 + 0x98;
        r4 = 0x4ba;
        r5 = (u32)&lbl_8047BF20;
        fn_80196E10();
    }
    ((void(*)(void))fn_8006B420)();
    r25 = r3;
    r3 = 0xda;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0xda;
        fn_80102510();
        while (1) {
            r3 = 0xda;
            fn_80102620();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x0) break;
            fn_800F0308();


        }
    }
    r3 = r26;
    ((void(*)(void))fn_8006A7E8)();
    r24 = r3;
    r3 = r26;
    ((void(*)(void))fn_8006A7C8)();
    r0 = 0x0;
    r9 = r3;
    r10 = r24;
    *(u32*)(sp + 0xC) = r0;
    r3 = 0xda;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x10;
    r7 = 0x0;
    r8 = 0x4;
    /* crclr cr1eq */;
    fn_801026A4();
    ((void(*)(void))fn_8006B420)();
    r4 = r3;
    r3 = r26 + 0xb44;
    ((void(*)(void))fn_80076054)();
    r4 = r3 & 0xFFFF;
    if ((u32)r4 != (u32)0x0) {
        r3 = 0x1;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        r3 = 0x1;
        fn_801069FC();
        r3 = 0xda;
        r4 = 0x0;
        r5 = 0x0;
        fn_80102568();
        ((void(*)(void))fn_8006E0CC)();
        ((void(*)(void))fn_800714C8)();
        r26 = -0x1;
        goto L_8005CAC0;
    }
    r3 = 0xd6;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0xd6;
        fn_80102510();
        while (1) {
            r3 = 0xd6;
            fn_80102620();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x0) break;
            fn_800F0308();


        }
    }
    r6 = 0x0;
    r0 = -0x2a;
    r5 = (u32)sp + 0x14;
    r3 = 0xd6;
    r4 = 0x0;
    r6 = 0x10;
    r7 = 0x1;
    r8 = 0x4;
    *(u32*)(sp + 0xC) = r0;
    r9 = 0x3d47;
    r10 = 0x3d49;
    /* crclr cr1eq */;
    fn_801026A4();
    r24 = r3;
    r3 = 0xd6;
    fn_80102510();
    if ((s32)r24 != (s32)0x0) {
        r3 = 0xda;
        r4 = 0x0;
        r5 = 0x0;
        fn_80102568();
        ((void(*)(void))fn_8006E0CC)();
        r26 = -0x1;
        goto L_8005CAC0;
    }
    r5 = *(u32*)&lbl_8047A5A0;
    r3 = 0x0;
    r4 = 0xe;
    r25 = r5 + 0x4318;
    fn_80129280();
    r0 = 0x1985;
    /* subi r5, r25, 0x4 */;
    /* subi r4, r3, 0x4 */;
    ctr_fn = (void(*)(void))r0;
    do {
        r3 = *(u32*)((u8*)r4 + 0x4);
        r0 = *(u32*)((u8*)r4 + 0x8);
        *(u32*)((u8*)r5 + 0x4) = r3;
        r5 += 8; *(u32*)r5 = r0;
    } while (--ctr != 0);
    r0 = *(u32*)((u8*)r4 + 0x4);
    r3 = r25;
    *(u32*)((u8*)r5 + 0x4) = r0;
    ((void(*)(void))fn_8006AFC4)();
    r0 = r3;
    r3 = r25;
    r22 = r0;
    ((void(*)(void))fn_8006A7BC)();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0 || (u32)r22 == (u32)0x0) goto L_8005B4CC;

    r3 = 0xda;
    r4 = 0x0;
    r5 = 0x0;
    fn_80102568();
    ((void(*)(void))fn_8006E0CC)();
    r3 = 0xbe;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
    r3 = 0xbe;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x10;
    r7 = 0x0;
    r8 = 0x1;
    r9 = 0xd7;
    /* crclr cr1eq */;
    fn_801026A4();
    r3 = 0xc8;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0xc8;
        fn_80102510();
        while (1) {
            r3 = 0xc8;
            fn_80102620();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x0) break;
            fn_800F0308();


        }
    }
    r3 = r22;
    ((void(*)(void))fn_8006A7E8)();
    r24 = r3;
    r3 = r22;
    ((void(*)(void))fn_8006A7C8)();
    r0 = 0x0;
    r9 = r3;
    r4 = r25 + (0x1 << 16);
    *(u32*)(sp + 0x8) = r0;
    /* subi r0, r4, 0x3674 */;
    r10 = r24;
    *(u32*)(sp + 0xC) = r0;
    r3 = 0xc8;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x10;
    r7 = 0x0;
    r8 = 0x4;
    /* crclr cr1eq */;
    fn_801026A4();
    r3 = 0xd6;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0xd6;
        fn_80102510();
        while (1) {
            r3 = 0xd6;
            fn_80102620();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x0) break;
            fn_800F0308();


        }
    }
    r0 = 0x1;
    r4 = 0x44c8;
    *(u32*)(sp + 0x10) = r0;
    r0 = 0x0;
    r5 = (u32)sp + 0x10;
    r3 = 0xd6;
    r4 = 0x0;
    r6 = 0x10;
    r7 = 0x1;
    *(u32*)(sp + 0xC) = r0;
    r8 = 0x4;
    r9 = 0x3d47;
    r10 = 0x3d49;
    /* crclr cr1eq */;
    fn_801026A4();
    r24 = r3;
    r3 = 0xd6;
    fn_80102510();
    r3 = 0xc8;
    r4 = 0x0;
    r5 = 0x0;
    fn_80102568();
    ((void(*)(void))fn_8006E0CC)();
    r3 = 0xbe;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
    if ((s32)r24 != (s32)0x0) {
        r26 = -0x1;
        goto L_8005CAC0;
    }
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    ((void(*)(void))fn_8006A76C)();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8005B4CC;
    r9 = r30;
    r3 = 0xbe;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x10;
    r7 = 0x0;
    r8 = 0x1;
    /* crclr cr1eq */;
    fn_801026A4();
    ((void(*)(void))fn_8006B420)();
    r25 = r3;
    r3 = 0xda;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0xda;
        fn_80102510();
        while (1) {
            r3 = 0xda;
            fn_80102620();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x0) break;
            fn_800F0308();


        }
    }
    r3 = r26;
    ((void(*)(void))fn_8006A7E8)();
    r24 = r3;
    r3 = r26;
    ((void(*)(void))fn_8006A7C8)();
    r0 = 0x0;
    r9 = r3;
    r10 = r24;
    *(u32*)(sp + 0xC) = r0;
    r3 = 0xda;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x10;
    r7 = 0x0;
    r8 = 0x4;
    /* crclr cr1eq */;
    fn_801026A4();
    L_8005B4CC: ;
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    ((void(*)(void))fn_8006A76C)();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        ((void(*)(void))fn_8006A79C)();
        goto L_8005B564;
    }
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    ((void(*)(void))fn_8006A7BC)();
    r24 = r3;
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    ((void(*)(void))fn_8006A79C)();
    ((void(*)(void))fn_80088C60)();
    if ((s32)r3 >= (s32)0x0) goto L_8005B564;
    r0 = r24 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        ((void(*)(void))fn_8006A7AC)();
    }
    r3 = 0xda;
    r4 = 0x0;
    r5 = 0x0;
    fn_80102568();
    ((void(*)(void))fn_8006E0CC)();
    r26 = -0x1;
    goto L_8005CAC0;
    L_8005B564: ;
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r22 = *(u32*)((u8*)r3 + 0x10);
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r23 = *(u32*)((u8*)r3 + 0xC);
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r26 = *(u32*)((u8*)r3 + 0x8);
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r25 = *(u32*)((u8*)r3 + 0x4);
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r24 = *(u32*)((u8*)r3 + 0x0);
    r3 = (u32)sp + 0x98;
    r4 = 0x0;
    r5 = 0x50;
    memset((void*)r3, (int)r4, (u32)r5);
    r5 = 0x0;
    r0 = 0x5;
    r3 = 0x0;
    r4 = 0xe;
    *(u32*)(sp + 0xB0) = r0;
    fn_80129280();
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((s32)r0 == (s32)0x1) {
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x0) = r24;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x4) = r24;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x8) = r24;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0xC) = r24;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x10) = r24;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x14) = r24;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x18) = r24;
        if ((u32)r0 == (u32)0x0) {
            r3 = 0x0;
            ((void(*)(void))fn_8006ADB4)();
        }
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r0 = *(u32*)((u8*)r3 + 0xC);
        if ((s32)r0 != (s32)0x6) {
            r3 = r31 + 0x98;
            r5 = r31 + 0x184;
            r4 = 0x81;
            fn_80196E10();
        }
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r0 = *(u32*)((u8*)r3 + 0x0);
        if ((s32)r0 != (s32)0x1) {
            r3 = r31 + 0x98;
            r5 = r31 + 0x1b0;
            r4 = 0x82;
            fn_80196E10();
        }
        r3 = 0x8ae;
        if ((s32)r0 == (s32)0x0) {
            r4 = 0x1;
        } else {

            r4 = 0x2;
        }
        fn_8019075C();
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r4 = r3;
        r3 = 0xb59;
        r4 = *(u32*)((u8*)r4 + 0x14);
        fn_8019075C();
        r3 = 0xafc;
        r4 = 0x0;
        fn_8019075C();
        r3 = 0xb11;
        r4 = 0x0;
        fn_8019075C();
        r3 = 0xde1;
        r4 = 0x0;
        fn_8019075C();
        fn_80130054();
        r29 = 0x4c;
        r26 = 0x105;
    } else {

        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x0) = r24;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x4) = r24;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x8) = r24;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0xC) = r24;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x10) = r24;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x14) = r24;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x18) = r24;
        if ((u32)r0 == (u32)0x0) {
            r3 = 0x0;
            ((void(*)(void))fn_8006ADB4)();
        }
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r0 = *(u32*)((u8*)r3 + 0x0);
        if ((s32)r0 != (s32)0x0) {
            r3 = r31 + 0x98;
            r5 = r31 + 0x158;
            r4 = 0xab;
            fn_80196E10();
        }
        r3 = 0x8ae;
        if ((s32)r0 == (s32)0x0) {
            r4 = 0x1;
        } else {

            r4 = 0x2;
        }
        fn_8019075C();
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        ((void(*)(void))fn_80069C0C)();
        r26 = 0xd1;
    }
    r3 = 0xda;
    r4 = 0x0;
    r5 = 0x0;
    fn_80102568();
    ((void(*)(void))fn_8006E0CC)();
    goto L_8005CAC0;
    ((void(*)(void))fn_80071344)();
    if ((s32)r3 < (s32)0x0) {
        r26 = -0x1;
        goto L_8005CAC0;
    }
    if ((s32)r3 < (s32)0x4) {
        r0 = r3 << 4;
        r22 = r31 + 0x4c;
        r22 = r22 + r0;
        r23 = r22 + 0x8;
        r0 = *(u32*)((u8*)r23 + 0x0);
        if ((s32)r0 != (s32)0x0) goto L_8005B910;
        fn_801D04E8();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x0) {
            r3 = 0x2;
            r4 = 0x44ea;
            r5 = 0x1;
            r6 = 0x0;
            fn_80106D3C();
            r3 = 0x1;
            fn_801069FC();
            goto L_8005CAC0;
        }
        r3 = 0x0;
        r4 = 0x4;
        fn_80135168();
        if ((u32)r3 != (u32)0x0) goto L_8005B910;
        r3 = 0x2;
        r4 = 0x44db;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        r3 = 0x1;
        fn_801069FC();
        goto L_8005CAC0;
        L_8005B910: ;
        r24 = *(u32*)((u8*)r22 + 0x0);
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0x4) = r24;
        r3 = 0x0;
        r24 = *(u32*)((u8*)r22 + 0x4);
        r4 = 0xe;
        fn_80129280();
        *(u32*)((u8*)r3 + 0xC) = r24;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r4 = *(u32*)((u8*)r23 + 0x0);
        r3 = r3 + 0x59a8;
        ((void(*)(void))fn_8006A7E0)();
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r4 = *(u8*)((u8*)r22 + 0xC);
        r3 = r3 + 0x59a8;
        r4 = (s8)r4;
        ((void(*)(void))fn_8006A81C)();
        r23 = r22 + 0x1;
        r24 = 0x1;
        r22 = 0x1660;
        do {
            r3 = 0x0;
            r4 = 0xe;
            fn_80129280();
            r0 = r22 + 0x59a8;
            r4 = 0x1;
            r3 = r3 + r0;
            ((void(*)(void))fn_8006A7E0)();
            r3 = 0x0;
            r4 = 0xe;
            fn_80129280();
            r4 = *(u8*)((u8*)r23 + 0xC);
            r0 = r22 + 0x59a8;
            r3 = r3 + r0;
            r4 = (s8)r4;
            ((void(*)(void))fn_8006A81C)();
            r22 = r22 + 0x1660;
            r23 = r23 + 0x1;
            r24 = r24 + 0x1;
        } while ((u32)r24 < (u32)0x4);
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r0 = *(u32*)((u8*)r3 + 0x4);
        if ((s32)r0 == (s32)0x2) {
            r26 = 0xbf;
            goto L_8005CAC0;
        }
        r26 = 0xaf;
        goto L_8005CAC0;
    }
    r26 = -0x1;
    goto L_8005CAC0;
    r0 = 0x0;
    *(u32*)(sp + 0x28) = r0;
    if ((s32)r22 == (s32)0xc1) {
        r0 = 0x7;
        *(u32*)(sp + 0x28) = r0;
        goto L_8005BA28;
    }
    ((void(*)(void))fn_8006B420)();
    r5 = *(u32*)&lbl_8047A5A0;
    r4 = r3;
    r3 = r5 + 0x42c0;
    ((void(*)(void))fn_80077E80)();
    L_8005BA28: ;
    ((void(*)(void))fn_8007162C)();
    r7 = *(u32*)&lbl_8047A5A0;
    r5 = (u32)sp + 0x28;
    r4 = 0x0;
    r6 = 0x10;
    r9 = r7 + 0x42c0;
    r7 = 0x1;
    r8 = 0x1;
    /* crclr cr1eq */;
    fn_801026A4();
    if ((s32)r3 < (s32)0x0) {
        r26 = -0x1;
        goto L_8005CAC0;
    }
    if ((s32)r3 == (s32)0x8) goto L_8005BAF0;
    if ((s32)r3 >= (s32)0x8) goto L_8005CAC0;
    if ((s32)r3 != (s32)0x3) {
        goto L_8005CAC0;
    }
    r4 = *(u32*)&lbl_8047A5A0;
    r0 = *(u32*)((u8*)r4 + 0x42C8);
    if ((s32)r0 != (s32)0x2) goto L_8005CAC0;
    r3 = (u32)sp + 0x5c;
    r4 = r4 + 0x42d8;
    r5 = 0x3c;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r9 = (u32)sp + 0x5c;
    r3 = 0xb4;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x10;
    r7 = 0x1;
    r8 = 0x1;
    /* crclr cr1eq */;
    fn_801026A4();
    r24 = r3;
    r3 = 0xb4;
    fn_80102510();
    if ((s32)r24 >= (s32)0x0) {
        r3 = *(u32*)&lbl_8047A5A0;
        r4 = (u32)sp + 0x5c;
        r5 = 0x3c;
        r3 = r3 + 0x42d8;
        memcpy((void*)r3, (const void*)r4, (u32)r5);
    }
    r0 = 0x3;
    *(u32*)(sp + 0x28) = r0;
    goto L_8005BA28;
    L_8005BAF0: ;
    ((void(*)(void))fn_800714C8)();
    r26 = 0xc1;
    goto L_8005CAC0;
    ((void(*)(void))fn_8007162C)();
    r7 = *(u32*)&lbl_8047A5A0;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x10;
    r9 = r7 + 0x42c0;
    r7 = 0x1;
    r8 = 0x1;
    /* crclr cr1eq */;
    fn_801026A4();
    if ((s32)r3 < (s32)0x0) {
        r26 = -0x1;
        goto L_8005CAC0;
    }
    if ((s32)r3 != (s32)0x6) {
        if ((s32)r3 < (s32)0x6) {
            if ((s32)r3 < (s32)0x5) {
                goto L_8005CAC0;
            }
            if ((s32)r3 >= (s32)0x8) goto L_8005CAC0;
            goto L_8005BB9C;
            }
        ((void(*)(void))fn_800714C8)();
        r26 = 0xc0;
        goto L_8005CAC0;
    }
    ((void(*)(void))fn_8007162C)();
    fn_801022B8();
    if ((s32)r3 != (s32)0x9fc) goto L_8005BB9C;
    r3 = 0x0;
    ((void(*)(void))fn_8006B51C)();
    r5 = *(u32*)&lbl_8047A5A0;
    r4 = r3;
    r3 = r5 + 0x42c0;
    ((void(*)(void))fn_80077E80)();
    r3 = *(u32*)&lbl_8047A5A0;
    r0 = 0x6;
    *(u16*)((u8*)r3 + 0x42C6) = r0;
    goto L_8005CAC0;
    L_8005BB9C: ;
    ((void(*)(void))fn_8006B420)();
    r5 = *(u32*)&lbl_8047A5A0;
    r4 = r3;
    r3 = r5 + 0x42c0;
    ((void(*)(void))fn_80077EA4)();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r0 = *(u32*)((u8*)r3 + 0x8);
        r3 = 0x0;
        r4 = 0xe;
        r24 = r0 * 0x54;
        fn_80129280();
        r5 = r24 + (0x1 << 16);
        r4 = *(u32*)&lbl_8047A5A0;
        /* subi r5, r5, 0x3624 */;
        r3 = r3 + r5;
        r4 = r4 + 0x42c0;
        ((void(*)(void))fn_80077E80)();
        L_8005BBF4: ;
        fn_801D04E8();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x0) goto L_8005BC44;
        r3 = 0x2;
        r4 = 0x44b1;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        r3 = 0x0;
        r4 = 0x3c;
        r5 = 0x9e;
        r6 = 0x0;
        ((void(*)(void))fn_8001E074)();
        r0 = (s8)r3;
        if ((s32)r0 != (s32)0x0) goto L_8005BC44;
        ((void(*)(void))fn_80088964)();
        if ((s32)r3 < (s32)0x0) goto L_8005BBF4;
        L_8005BC44: ;
        r3 = 0x1;
        fn_801069FC();
    }
    r26 = -0x1;
    goto L_8005CAC0;
    r3 = r31 + 0x98;
    r4 = 0x5f5;
    r5 = (u32)&lbl_8047BF1C;
    fn_80196E10();
    goto L_8005CAC0;
    ((void(*)(void))fn_80071344)();
    r24 = r3;
    if ((s32)r24 >= (s32)0x6) goto L_8005BCB8;
    if ((s32)r24 < (s32)0x0) {
        goto L_8005BCB8;
    }
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    *(u32*)((u8*)r3 + 0x8) = r24;
    fn_80105624();
    r0 = *(u16*)((u8*)r3 + 0x4);
    r0 = r0 & 0x00000400;
    if ((s32)r0 != (s32)0x0) {
        r26 = 0xc0;
        goto L_8005CAC0;
    }
    r26 = 0xc2;
    goto L_8005CAC0;
    L_8005BCB8: ;
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((s32)r0 == (s32)0x2) {
        r3 = 0xb3;
        ((void(*)(void))fn_80071398)();
        r26 = r3;
        goto L_8005CAC0;
    }
    r3 = 0xaf;
    ((void(*)(void))fn_80071398)();
    r26 = r3;
    goto L_8005CAC0;
    ((void(*)(void))fn_80071344)();
    if ((s32)r3 == (s32)0x1 || (s32)r3 >= (s32)0x1) goto L_8005BD50;

    if ((s32)r3 < (s32)0x0) {
        goto L_8005BD50;
    }
    ((void(*)(void))fn_800714C8)();
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r0 = *(u32*)((u8*)r3 + 0x10);
    if ((s32)r0 != (s32)0x4) {
        r26 = 0xb1;
        goto L_8005CAC0;
    }
    fn_80089028();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r26 = 0xe4;
        goto L_8005CAC0;
    }
    r26 = 0xb6;
    goto L_8005CAC0;
    L_8005BD50: ;
    r3 = 0xb3;
    ((void(*)(void))fn_80071398)();
    r26 = r3;
    goto L_8005CAC0;
    r22 = 0x0;
    r23 = r22;
    do {
        r0 = *(u32*)&lbl_8047A5A0;
        r3 = r23 + 0x1660;
        r4 = 0x0;
        r5 = 0xb18;
        r3 = r0 + r3;
        memset((void*)r3, (int)r4, (u32)r5);
        r0 = *(u32*)&lbl_8047A5A0;
        r3 = r23 + 0x1660;
        r3 = r0 + r3;
        fn_8012A248();
        r23 = r23 + 0xb18;
        r22 = r22 + 0x1;
    } while ((u32)r22 < (u32)0x4);
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((s32)r0 == (s32)0x2 || (s32)r0 >= (s32)0x2) goto L_8005BE30;

    if ((s32)r0 < (s32)0x0) {
        goto L_8005BE30;
    }
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r0 = *(u32*)((u8*)r3 + 0x59AC);
    if ((s32)r0 == (s32)0x0) {
        r4 = *(u32*)&lbl_8047A5A0;
        r0 = 0x0;
        *(u32*)(sp + 0x44) = r0;
        r3 = 0x0;
        r5 = r4 + 0x1660;
        r4 = r4 + 0x2178;
        *(u32*)(sp + 0x48) = r0;
        goto L_8005BE78;
    }
    r5 = *(u32*)&lbl_8047A5A0;
    r6 = 0x0;
    r3 = 0x1;
    r4 = r5 + 0x1660;
    r0 = r5 + 0x2178;
    *(u32*)(sp + 0x44) = r0;
    goto L_8005BE78;
    L_8005BE30: ;
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r0 = *(u32*)((u8*)r3 + 0x59AC);
    if ((s32)r0 == (s32)0x0) {
        r3 = 0x2;
    } else {

        r3 = 0x3;
    }
    r7 = *(u32*)&lbl_8047A5A0;
    r6 = r7 + 0x1660;
    r5 = r7 + 0x2178;
    r4 = r7 + 0x2c90;
    r0 = r7 + 0x37a8;
    *(u32*)(sp + 0x48) = r0;
    L_8005BE78: ;
    r5 = (u32)sp + 0x3c;
    r4 = 0x1a;
    r6 = 0x0;
    ((void(*)(void))fn_800849B4)();
    if ((s32)r3 < (s32)0x0) {
        r3 = 0xb3;
        ((void(*)(void))fn_80071398)();
        r26 = r3;
        goto L_8005CAC0;
    }
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r0 = *(u32*)((u8*)r3 + 0x59AC);
    if ((s32)r0 == (s32)0x0) {
        r3 = *(u32*)&lbl_8047A5A0;
        r4 = 0x0;
        r3 = r3 + 0x1660;
        fn_8012A774();
    }
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((s32)r0 == (s32)0x2 || (s32)r0 >= (s32)0x2) goto L_8005BFE0;

    if ((s32)r0 < (s32)0x0) {
        goto L_8005BFE0;
    }
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r4 = *(u32*)&lbl_8047A5A0;
    r3 = r3 + 0x59a8;
    r4 = r4 + 0x1660;
    ((void(*)(void))fn_8006A824)();
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r4 = *(u32*)&lbl_8047A5A0;
    r3 = r3 + 0x7008;
    r4 = r4 + 0x2178;
    ((void(*)(void))fn_8006A824)();
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r0 = *(u32*)((u8*)r3 + 0x59AC);
    if ((s32)r0 == (s32)0x0) {
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r3 = r3 + 0x59a8;
        r4 = 0x1;
        ((void(*)(void))fn_8006A81C)();
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r3 = r3 + 0x7008;
        r4 = 0x2;
        ((void(*)(void))fn_8006A81C)();
    } else {

        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r3 = r3 + 0x59a8;
        r4 = 0x2;
        ((void(*)(void))fn_8006A81C)();
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r3 = r3 + 0x7008;
        r4 = 0x3;
        ((void(*)(void))fn_8006A81C)();
    }
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r3 = r3 + (0x1 << 16);
    r4 = 0x0;
    /* subi r3, r3, 0x7998 */;
    ((void(*)(void))fn_8006A81C)();
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r3 = r3 + (0x1 << 16);
    r4 = 0x0;
    /* subi r3, r3, 0x6338 */;
    ((void(*)(void))fn_8006A81C)();
    goto L_8005C0C0;
    L_8005BFE0: ;
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r4 = *(u32*)&lbl_8047A5A0;
    r3 = r3 + 0x59a8;
    r4 = r4 + 0x1660;
    ((void(*)(void))fn_8006A824)();
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r4 = *(u32*)&lbl_8047A5A0;
    r3 = r3 + 0x7008;
    r4 = r4 + 0x2178;
    ((void(*)(void))fn_8006A824)();
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r4 = *(u32*)&lbl_8047A5A0;
    r3 = r3 + (0x1 << 16);
    /* subi r3, r3, 0x7998 */;
    r4 = r4 + 0x2c90;
    ((void(*)(void))fn_8006A824)();
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r4 = *(u32*)&lbl_8047A5A0;
    r3 = r3 + (0x1 << 16);
    /* subi r3, r3, 0x6338 */;
    r4 = r4 + 0x37a8;
    ((void(*)(void))fn_8006A824)();
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r3 = r3 + 0x59a8;
    r4 = 0x1;
    ((void(*)(void))fn_8006A81C)();
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r3 = r3 + 0x7008;
    r4 = 0x2;
    ((void(*)(void))fn_8006A81C)();
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r3 = r3 + (0x1 << 16);
    r4 = 0x3;
    /* subi r3, r3, 0x7998 */;
    ((void(*)(void))fn_8006A81C)();
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r3 = r3 + (0x1 << 16);
    r4 = 0x4;
    /* subi r3, r3, 0x6338 */;
    ((void(*)(void))fn_8006A81C)();
    L_8005C0C0: ;
    r26 = 0xb6;
    goto L_8005CAC0;
    r23 = 0x0;
    r22 = 0x0;
    r24 = r22;
    do {
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        r0 = r22 + 0x7005;
        r22 = r22 + 0x1660;
        *(u8*)(r3 + r0) = r24;
        r23 = r23 + 0x1;
    } while ((u32)r23 < (u32)0x4);
    ((void(*)(void))fn_80071344)();
    if ((s32)r3 < (s32)0x0) {
        ((void(*)(void))fn_80071160)();
        if ((s32)r3 != (s32)0x1) {
            if ((s32)r3 >= (s32)0x1) goto L_8005C1B4;
            if ((s32)r3 < (s32)0x0) {
                goto L_8005C1B4;
            }
            r3 = 0x1;
            fn_8008ABA0();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x0) {
                r3 = 0x2;
                r4 = 0x4445;
                r5 = 0x1;
                r6 = 0x0;
                fn_80106D3C();
                while (1) {
                    r3 = 0x1;
                    fn_800F7EF8();
                    r0 = r3 & 0xFF;
                    if ((u32)r0 != (u32)0x0) break;
                    fn_800F0308();


                }
                goto L_8005C1D4;
            }
            r3 = 0x2;
            r4 = 0x3d55;
            r5 = 0x1;
            r6 = 0x0;
            fn_80106D3C();
            goto L_8005C1D4;
        }
        r3 = 0x2;
        r4 = 0x44c0;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        while (1) {
            r3 = 0x1;
            fn_800F7EF8();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x0) break;
            fn_800F0308();


        }
        goto L_8005C1D4;
        L_8005C1B4: ;
        r4 = r3;
        r3 = 0x2f;
        fn_80132A38();
        r3 = 0x2;
        r4 = 0x44b8;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        L_8005C1D4: ;
        r3 = 0x1;
        fn_801069FC();
        r3 = 0x1;
        fn_800F7EF8();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x0) {
            r3 = 0x1;
            fn_8008ABA0();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x0) {
                r4 = 0x4445;
            } else {

                r4 = 0x3c4f;
            }
            r3 = 0x2;
            r5 = 0x1;
            r6 = 0x0;
            fn_80106D3C();
            while (1) {
                r3 = 0x1;
                fn_800F7EF8();
                r0 = r3 & 0xFF;
                if ((u32)r0 != (u32)0x0) break;
                fn_800F0308();


            }
            r3 = 0x1;
            fn_801069FC();
        }
        r3 = 0xb3;
        ((void(*)(void))fn_80071398)();
        r26 = r3;
        goto L_8005CAC0;
    }
    r3 = 0xd0;
    r4 = 0x1;
    fn_8010264C();
    r24 = r3;
    r3 = 0xd0;
    fn_80102510();
    if ((s32)r24 < (s32)0x0) {
        ((void(*)(void))fn_80071160)();
        if ((s32)r3 != (s32)0x1) {
            if ((s32)r3 >= (s32)0x1) goto L_8005C324;
            if ((s32)r3 < (s32)0x0) {
                goto L_8005C324;
            }
            r3 = 0x1;
            fn_8008ABA0();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x0) {
                r3 = 0x2;
                r4 = 0x4445;
                r5 = 0x1;
                r6 = 0x0;
                fn_80106D3C();
                while (1) {
                    r3 = 0x1;
                    fn_800F7EF8();
                    r0 = r3 & 0xFF;
                    if ((u32)r0 != (u32)0x0) break;
                    fn_800F0308();


                }
                goto L_8005C344;
            }
            r3 = 0x2;
            r4 = 0x3d55;
            r5 = 0x1;
            r6 = 0x0;
            fn_80106D3C();
            goto L_8005C344;
        }
        r3 = 0x2;
        r4 = 0x44c0;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        while (1) {
            r3 = 0x1;
            fn_800F7EF8();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x0) break;
            fn_800F0308();


        }
        goto L_8005C344;
        L_8005C324: ;
        r4 = r3;
        r3 = 0x2f;
        fn_80132A38();
        r3 = 0x2;
        r4 = 0x44b8;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        L_8005C344: ;
        r3 = 0x1;
        fn_801069FC();
        r3 = 0x1;
        fn_800F7EF8();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x0) {
            r3 = 0x1;
            fn_8008ABA0();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x0) {
                r4 = 0x4445;
            } else {

                r4 = 0x3c4f;
            }
            r3 = 0x2;
            r5 = 0x1;
            r6 = 0x0;
            fn_80106D3C();
            while (1) {
                r3 = 0x1;
                fn_800F7EF8();
                r0 = r3 & 0xFF;
                if ((u32)r0 != (u32)0x0) break;
                fn_800F0308();


            }
            r3 = 0x1;
            fn_801069FC();
        }
        r3 = 0xb3;
        ((void(*)(void))fn_80071398)();
        r26 = r3;
        goto L_8005CAC0;
    }
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((s32)r0 != (s32)0x2) {
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        ((void(*)(void))fn_80069C0C)();
        r26 = 0xd1;
        goto L_8005CAC0;
    }
    r26 = 0xb5;
    goto L_8005CAC0;
    ((void(*)(void))fn_80071344)();
    r26 = 0xd1;
    if ((s32)r3 == (s32)0x3 || (s32)r3 >= (s32)0x3 || (s32)r3 == (s32)-0x1) goto L_8005C4A8;


    if ((s32)r3 < (s32)-0x1) {
        goto L_8005C4A8;
    }
    r0 = r3 << 2;
    r22 = r31 + 0x8c;
    r22 = r22 + r0;
    r3 = 0x0;
    r0 = *(u8*)((u8*)r22 + 0x0);
    r4 = 0xe;
    r24 = (s8)r0;
    fn_80129280();
    r0 = *(u8*)((u8*)r22 + 0x1);
    r4 = 0xe;
    *(u32*)((u8*)r3 + 0x59D0) = r24;
    r3 = 0x0;
    r24 = (s8)r0;
    fn_80129280();
    r0 = *(u8*)((u8*)r22 + 0x2);
    r4 = 0xe;
    *(u32*)((u8*)r3 + 0x7030) = r24;
    r3 = 0x0;
    r24 = (s8)r0;
    fn_80129280();
    r3 = r3 + (0x1 << 16);
    r0 = *(u8*)((u8*)r22 + 0x3);
    *(u32*)((u8*)r3 + (-31088)) = r24;
    r3 = 0x0;
    r24 = (s8)r0;
    r4 = 0xe;
    fn_80129280();
    r4 = r3 + (0x1 << 16);
    r3 = 0x0;
    *(u32*)((u8*)r4 + (-25360)) = r24;
    r4 = 0xe;
    fn_80129280();
    ((void(*)(void))fn_80069C0C)();
    goto L_8005CAC0;
    L_8005C4A8: ;
    ((void(*)(void))fn_80071160)();
    if ((s32)r3 != (s32)0x1) {
        if ((s32)r3 >= (s32)0x1) goto L_8005C558;
        if ((s32)r3 < (s32)0x0) {
            goto L_8005C558;
        }
        r3 = 0x1;
        fn_8008ABA0();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x0) {
            r3 = 0x2;
            r4 = 0x4445;
            r5 = 0x1;
            r6 = 0x0;
            fn_80106D3C();
            while (1) {
                r3 = 0x1;
                fn_800F7EF8();
                r0 = r3 & 0xFF;
                if ((u32)r0 != (u32)0x0) break;
                fn_800F0308();


            }
            goto L_8005C578;
        }
        r3 = 0x2;
        r4 = 0x3d55;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        goto L_8005C578;
    }
    r3 = 0x2;
    r4 = 0x44c0;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    while (1) {
        r3 = 0x1;
        fn_800F7EF8();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x0) break;
        fn_800F0308();


    }
    goto L_8005C578;
    L_8005C558: ;
    r4 = r3;
    r3 = 0x2f;
    fn_80132A38();
    r3 = 0x2;
    r4 = 0x44b8;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    L_8005C578: ;
    r3 = 0x1;
    fn_801069FC();
    r3 = 0x1;
    fn_800F7EF8();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r3 = 0x1;
        fn_8008ABA0();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x0) {
            r4 = 0x4445;
        } else {

            r4 = 0x3c4f;
        }
        r3 = 0x2;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        while (1) {
            r3 = 0x1;
            fn_800F7EF8();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x0) break;
            fn_800F0308();


        }
        r3 = 0x1;
        fn_801069FC();
    }
    r3 = 0xb3;
    ((void(*)(void))fn_80071398)();
    r26 = r3;
    goto L_8005CAC0;
    ((void(*)(void))fn_8006B8FC)();
    r29 = 0x397;
    goto L_8005CAC0;
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    fn_80062948();
    ((void(*)(void))fn_80071398)();
    r26 = r3;
    goto L_8005CAC0;
    r3 = 0xb9;
    r4 = 0x1;
    fn_8010264C();
    r24 = r3;
    r3 = 0xb9;
    r4 = 0x0;
    r5 = 0x0;
    fn_80102568();
    r3 = 0xbe;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
    if ((s32)r24 != (s32)0x0) {
        if ((s32)r24 < (s32)0x0) {
            goto L_8005C738;
        }
        if ((s32)r24 >= (s32)0x2) goto L_8005C738;
        goto L_8005C6E0;
    }
    fn_801D04E8();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r3 = 0x2;
        r4 = 0x44ea;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        r3 = 0x1;
        fn_801069FC();
        goto L_8005CAC0;
    }
    r3 = 0x0;
    r4 = 0x4;
    fn_80135168();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x2;
        r4 = 0x44db;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        r3 = 0x1;
        fn_801069FC();
        goto L_8005CAC0;
    }
    r3 = 0xb;
    ((void(*)(void))fn_8002D91C)();
    goto L_8005CAC0;
    L_8005C6E0: ;
    r3 = 0x1;
    fn_8008ABA0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0x2;
        r4 = 0x4445;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        while (1) {
            r3 = 0x1;
            fn_800F7EF8();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x0) break;
            fn_800F0308();


        }
        r3 = 0x1;
        fn_801069FC();
    }
    r3 = 0xc;
    ((void(*)(void))fn_8002D91C)();
    goto L_8005CAC0;
    L_8005C738: ;
    r26 = -0x1;
    goto L_8005CAC0;
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    ((void(*)(void))fn_8006A7BC)();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        ((void(*)(void))fn_8006ADEC)();
        r0 = r3;
        r3 = 0x0;
        r25 = r0;
        r4 = 0xe;
        fn_80129280();
        ((void(*)(void))fn_8006AFC4)();
        r26 = r3;
        if ((u32)r26 == (u32)0x0) {
            r3 = r31 + 0x98;
            r4 = 0x72c;
            r5 = (u32)&lbl_8047BF24;
            fn_80196E10();
        }
        if ((u32)r25 == (u32)0x0) goto L_8005CA74;
        r3 = 0x0;
        ((void(*)(void))fn_8006ADB4)();
        r3 = r26;
        ((void(*)(void))fn_8006A7E8)();
        if ((s32)r3 != (s32)0x0) goto L_8005C868;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        ((void(*)(void))fn_8006A76C)();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x0) goto L_8005CA74;
        r22 = r25;
        r4 = r25;
        r3 = 0x0;
        fn_801293FC();
        L_8005C7E0: ;
        r3 = 0x2;
        r4 = 0x3c03;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        r3 = 0x0;
        r4 = 0x3c;
        r5 = 0x9e;
        r6 = 0x0;
        ((void(*)(void))fn_8001E074)();
        r0 = (s8)r3;
        if ((s32)r0 != (s32)0x0) goto L_8005C824;
        fn_80088D84();
        if ((s32)r3 >= (s32)0x0) goto L_8005CA74;
        goto L_8005C7E0;
        L_8005C824: ;
        r3 = 0x2;
        r4 = 0x3d54;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        r3 = 0x0;
        r4 = 0x3c;
        r5 = 0x9e;
        r6 = 0x1;
        ((void(*)(void))fn_8001E074)();
        r0 = (s8)r3;
        if ((s32)r0 != (s32)0x0) goto L_8005C7E0;
        r4 = r22;
        r3 = 0x0;
        fn_80129384();
        goto L_8005CA74;
        L_8005C868: ;
        r3 = 0x7;
        r4 = 0x3c23;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        r3 = 0x0;
        r4 = 0x3c;
        r5 = 0x9e;
        r6 = 0x0;
        ((void(*)(void))fn_8001E074)();
        r0 = (s8)r3;
        if ((s32)r0 != (s32)0x0) goto L_8005C9E4;
        r3 = *(u32*)&lbl_8047A5A0;
        r7 = 0x0;
        r5 = (u32)sp + 0x2c;
        r0 = r3 + 0x1660;
        r6 = (u32)sp + 0xe8;
        *(u32*)(sp + 0x30) = r0;
        r3 = 0x0;
        r4 = 0x40;
        ((void(*)(void))fn_800849B4)();
        if ((s32)r3 < (s32)0x0) goto L_8005C868;
        r3 = r26 + 0xb44;
        fn_8012AC3C();
        r4 = *(u32*)&lbl_8047A5A0;
        r24 = r3;
        r3 = r4 + 0x1660;
        fn_8012AC3C();
        if ((u32)r3 != (u32)r24) goto L_8005C9CC;
        r3 = r26 + 0xb44;
        fn_8012AC54();
        r4 = *(u32*)&lbl_8047A5A0;
        r24 = r3;
        r3 = r4 + 0x1660;
        fn_8012AC54();
        r4 = r24;
        fn_800F9EE4();
        if ((s32)r3 != (s32)0x0) goto L_8005C9CC;
        r3 = 0x7;
        r4 = 0x3d51;
        r5 = 0x0;
        r6 = 0x1;
        fn_80106D3C();
        r3 = (0x99 << 16);
        /* subi r0, r3, 0x6981 */;
        r5 = r5 + r25;
        r3 = r4 + r25;
        if ((u32)r5 > (u32)r0) {
            *(u32*)(sp + 0xE8) = r0;
        }
        r3 = (0x99 << 16);
        /* subi r0, r3, 0x6981 */;
        if ((u32)r4 > (u32)r0) {
            *(u32*)(sp + 0xEC) = r0;
        }
        r3 = 0x1;
        fn_80093574();
        r4 = (u32)sp + 0xe8;
        r3 = 0x1;
        r5 = 0x0;
        fn_80092C90();
        r3 = 0x1;
        fn_80093574();
        if ((s32)r3 == (s32)0xc) {
            r3 = 0x7;
            r4 = 0x3d52;
            r5 = 0x1;
            r6 = 0x0;
            fn_80106D3C();
            goto L_8005CA18;
        }
        r3 = 0x7;
        r4 = 0x3d53;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        goto L_8005C868;
        L_8005C9CC: ;
        r3 = 0x7;
        r4 = 0x44da;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        goto L_8005C868;
        L_8005C9E4: ;
        r3 = 0x7;
        r4 = 0x3d54;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        r3 = 0x0;
        r4 = 0x3c;
        r5 = 0x9e;
        r6 = 0x1;
        ((void(*)(void))fn_8001E074)();
        r0 = (s8)r3;
        if ((s32)r0 != (s32)0x0) goto L_8005C868;
        L_8005CA18: ;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        ((void(*)(void))fn_8006A76C)();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x0) goto L_8005CA74;
        L_8005CA34: ;
        r3 = 0x2;
        r4 = 0x44ec;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        r3 = 0x0;
        r4 = 0x3c;
        r5 = 0x9e;
        r6 = 0x0;
        ((void(*)(void))fn_8001E074)();
        r0 = (s8)r3;
        if ((s32)r0 != (s32)0x0) goto L_8005CA74;
        ((void(*)(void))fn_80088C60)();
        if ((s32)r3 < (s32)0x0) goto L_8005CA34;
        L_8005CA74: ;
        r3 = 0x1;
        fn_801069FC();
    }
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((s32)r0 == (s32)0x1 || (s32)r0 >= (s32)0x1) goto L_8005CAB4;

    if ((s32)r0 < (s32)0x0) {
        goto L_8005CAB4;
    }
    r3 = 0xae;
    ((void(*)(void))fn_80071398)();
    r26 = r3;
    goto L_8005CAC0;
    L_8005CAB4: ;
    r3 = 0xac;
    ((void(*)(void))fn_80071398)();
    r26 = r3;
    L_8005CAC0: ;
    r3 = 0x1;
    fn_800F7EF8();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8005CB20;
    r3 = 0x1;
    fn_8008ABA0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8005CB20;
    r3 = 0x2;
    r4 = 0x3c4f;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    while (1) {
        r3 = 0x1;
        fn_800F7EF8();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x0) break;
        fn_800F0308();


    }
    r3 = 0x1;
    fn_801069FC();
    L_8005CB20: ;
    r22 = r30;
    if ((s32)r26 < (s32)0x0) {
        ((void(*)(void))fn_800714C8)();
        r25 = r3;
        r3 = 0xbe;
        r4 = 0x0;
        r5 = 0x1;
        fn_80102568();
        if ((s32)r25 < (s32)0x0) goto L_8005CBE4;
        goto L_8005CB98;
    }
    ((void(*)(void))fn_8007162C)();
    if ((s32)r26 == (s32)r3) goto L_8005CB98;
    ((void(*)(void))fn_8007162C)();
    r25 = r3;
    fn_801046B8();
    if ((s32)r3 == (s32)r25) {
        ((void(*)(void))fn_8007162C)();
        r4 = 0x0;
        r5 = 0x0;
        fn_80102568();
    }
    r3 = 0xbe;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
    r3 = r26;
    ((void(*)(void))fn_800715BC)();
    L_8005CB98: ;
    if ((u32)r29 == (u32)0x0) goto L_8005CBD8;
    ((void(*)(void))fn_8007162C)();
    r25 = r3;
    fn_801046B8();
    if ((s32)r3 != (s32)r25) goto L_8005CBE4;
    ((void(*)(void))fn_8007162C)();
    r4 = 0x0;
    r5 = 0x0;
    fn_80102568();
    r3 = 0xbe;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
    goto L_8005CBE4;
    L_8005CBD8: ;
    ((void(*)(void))fn_8007162C)();
    if ((s32)r3 > (s32)0x0) goto L_80059C6C;
    L_8005CBE4: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        ((void(*)(void))fn_8006B8FC)();
        f1 = f31;
        r3 = r29;
        f2 = f30;
        r4 = r28;
        f3 = f29;
        fn_80113778();
        goto L_8005CCA4;
    }
    if ((u32)r29 == (u32)0x0) {
        r29 = 0x3a1;
    }
    if ((u32)r29 == (u32)0x3a1) {
        ((void(*)(void))fn_8006B8F0)();
        r3 = 0x1;
        fn_800F7EF8();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x0) goto L_8005CC98;
        r3 = 0x1;
        fn_8008ABA0();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x0) {
            r4 = 0x4445;
        } else {

            r4 = 0x3c4f;
        }
        r3 = 0x2;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        while (1) {
            r3 = 0x1;
            fn_800F7EF8();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x0) break;
            fn_800F0308();


        }
        r3 = 0x1;
        fn_801069FC();
        goto L_8005CC98;
    }
    ((void(*)(void))fn_8006B8FC)();
    L_8005CC98: ;
    r3 = r29;
    r4 = 0x0;
    fn_80113828();
    L_8005CCA4: ;
    /* psq_l f31, 0x218((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x210);
    /* psq_l f30, 0x208((u32)sp), 0, qr0 */;
    f30 = *(f64*)(sp + 0x200);
    /* psq_l f29, 0x1f8((u32)sp), 0, qr0 */;
    f29 = *(f64*)(sp + 0x1F0);
    return;
}


/* 0x8005CCD0 | size: 0xB8 */
s32 fn_8005CCD0(void) {
    extern void fn_800E202C();
    extern void fn_800E209C();
    extern void fn_800E24B0();
    extern void fn_80102510();
    extern void fn_80129280();
    extern void fn_8019075C();
    extern void fn_80196E10();
    extern void fn_801C40F0();
    extern void fn_801CB9D8();
    extern void fn_8025CD64();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    
    r3 = 0x1;
    fn_801C40F0();
    ((void(*)(void))fn_8006B8E8)();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r31 = 0x0;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u8*)((u8*)r3 + 0x1C) = r31;
        r3 = 0x8ae;
        r4 = 0x0;
        fn_8019075C();
    }
    r3 = 0xd3;
    fn_80102510();
    fn_8025CD64();
    r3 = *(u32*)&lbl_8047A5A0;
    r3 = *(u32*)((u8*)r3 + 0x4314);
    fn_801CB9D8();
    r3 = *(u32*)&lbl_8047A5A0;
    fn_800E202C();
    r31 = r3;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 == (u32)0x0) {
        r3 = (u32)&lbl_802678D8;
        r4 = 0x252;
        r3 = (u32)&lbl_802678D8;
        r5 = (u32)&lbl_8047BF28;
        fn_80196E10();
    }
    r3 = r31;
    fn_800E24B0();
    r3 = r31;
    fn_800E209C();
    r0 = 0x0;
    *(u32*)&lbl_8047A5A0 = r0;
    return;
}


/* 0x8005CD88 | size: 0x160 */
s32 fn_8005CD88(void) {
    extern void fn_800E27B0();
    extern void fn_800E2C04();
    extern void fn_800F9318();
    extern void fn_800FF548();
    extern void fn_8010264C();
    extern void fn_80113F48();
    extern void fn_80129280();
    extern void fn_80165A20();
    extern void fn_80176E0C();
    extern void fn_80177A44();
    extern void fn_8019075C();
    extern void fn_80196E10();
    extern void fn_801CBA0C();
    extern void fn_8025CDB8();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    
    r4 = (u32)&lbl_80267840;
    r3 = 0x1e;
    r31 = (u32)&lbl_80267840;
    r4 = 0x0;
    r5 = 0xff;
    fn_80165A20();
    r3 = 0x8ae;
    r4 = 0x0;
    fn_8019075C();
    r0 = *(u32*)&lbl_8047A5A0;
    if ((u32)r0 != (u32)0x0) {
        r3 = r31 + 0x98;
        r5 = r31 + 0x21c;
        r4 = 0x20f;
        fn_80196E10();
    }
    r3 = (0x1 << 16);
    r4 = 0x20;
    r3 = r3 + 0xf60;
    fn_800E2C04();
    r30 = r3;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 == (u32)0x0) {
        r3 = r31 + 0x98;
        r4 = 0x212;
        r5 = (u32)&lbl_8047BF28;
        fn_80196E10();
    }
    r3 = r30;
    fn_800E27B0();
    *(u32*)&lbl_8047A5A0 = r3;
    if ((u32)r3 == (u32)0x0) {
        r3 = r31 + 0x98;
        r5 = r31 + 0x22c;
        r4 = 0x213;
        fn_80196E10();
    }
    fn_80113F48();
    r4 = (0xffe << 16);
    r30 = r3;
    r3 = r4 + 0x1000;
    fn_801CBA0C();
    r4 = *(u32*)&lbl_8047A5A0;
    *(u32*)((u8*)r4 + 0x4314) = r3;
    r3 = r30;
    r4 = *(u32*)&lbl_8047A5A0;
    r4 = *(u32*)((u8*)r4 + 0x4314);
    fn_800F9318();
    r4 = (0xfff << 16);
    r3 = 0x531;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x1;
    fn_80176E0C();
    r3 = 0x4;
    fn_80177A44();
    fn_800FF548();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        fn_8025CDB8();
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        ((void(*)(void))fn_8006B5D0)();
    }
    ((void(*)(void))fn_8006B8E8)();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r31 = 0x0;
        r3 = 0x0;
        r4 = 0xe;
        fn_80129280();
        *(u8*)((u8*)r3 + 0x1C) = r31;
    }
    r3 = 0xd3;
    r4 = 0x0;
    fn_8010264C();
    return;
}


/* 0x8005CEE8 | size: 0x44 */
s32 fn_8005CEE8(void) {
    extern void fn_800FF58C();
    extern void fn_80129280();
    u8 sp[0x10];
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    
    r31 = r3;
    ((void(*)(void))fn_8006B8F0)();
    r3 = 0x0;
    r4 = 0xe;
    fn_80129280();
    *(u32*)((u8*)r3 + 0x0) = r31;
    r3 = 0x395;
    fn_800FF58C();
    return;
}


/* 0x8005CF2C | size: 0x168 */
s32 fn_8005CF2C(void) {
    extern void fn_80102568();
    extern void fn_801026A4();
    extern void fn_80102868();
    extern void fn_80106D3C();
    extern void fn_80166A28();
    extern void fn_80196E10();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    
    r31 = r3;
    r30 = r4;
    r3 = 0xbe;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x10;
    r7 = 0x0;
    r8 = 0x1;
    r9 = 0xf5;
    /* crclr cr1eq */;
    fn_801026A4();
    ((void(*)(void))fn_8006B420)();
    r0 = 0x0;
    r9 = r31;
    r10 = r30;
    *(u32*)(sp + 0xC) = r0;
    r3 = 0xda;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x10;
    r7 = 0x0;
    r8 = 0x4;
    /* crclr cr1eq */;
    fn_801026A4();
    r3 = 0xda;
    r4 = 0x0;
    r5 = -0x28;
    fn_80102868();
    ((void(*)(void))fn_8006B420)();
    r4 = r3;
    r3 = r31;
    ((void(*)(void))fn_80076054)();
    r31 = r3;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 == (u32)0x0) {
        r3 = (u32)&lbl_802678D8;
        r5 = (u32)&lbl_80267A20;
        r3 = (u32)&lbl_802678D8;
        r4 = 0x1bb;
        r5 = (u32)&lbl_80267A20;
        fn_80196E10();
    }
    r3 = 0x26;
    fn_80166A28();
    r4 = r31 & 0xFFFF;
    r3 = 0x7;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    if ((s32)r30 != (s32)0x0) {
        if ((s32)r30 < (s32)0x0) goto L_8005D034;
        goto L_8005D034;
    }
    r3 = 0x7;
    r4 = 0x440a;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    goto L_8005D048;
    L_8005D034: ;
    r3 = 0x7;
    r4 = 0x3c4e;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    L_8005D048: ;
    r3 = 0xda;
    r4 = 0x0;
    r5 = 0x0;
    fn_80102568();
    r3 = 0xbe;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
    r3 = 0xda;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
    ((void(*)(void))fn_8006E0CC)();
    return;
}


/* 0x8005D094 | size: 0x24 */
s32 fn_8005D094(void) {
    fn_800347B8();
    return -1;
}

/* 0x8005D0B8 | size: 0x24 */
s32 fn_8005D0B8(void) {
    fn_800347C4();
    return 0;
}

/* 0x8005D0DC | size: 0x54 */
s32 fn_8005D0DC(void) {
    extern s32 fn_80102510(s32);
    extern s32 fn_8010264C(s32, s32);
    extern void fn_800347E8(s32);
    s32 result;

    result = fn_8010264C(2, 1);
    fn_80102510(2);
    if (result >= 0) {
        fn_800347E8(result);
    }
    return 0;
}


/* 0x8005D130 | size: 0x54 */
s32 fn_8005D130(void) {
    extern s32 fn_80102510(s32);
    extern s32 fn_8010264C(s32, s32);
    extern void fn_8003480C(s32);
    s32 result;

    result = fn_8010264C(2, 1);
    fn_80102510(2);
    if (result >= 0) {
        fn_8003480C(result);
    }
    return 0;
}


/* 0x8005D184 | size: 0xE8 */
s32 fn_8005D184(void) {
    extern void fn_80109220();
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
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    
    r5 = (u32)&lbl_80267A80;
    r11 = (u32)sp + 0x8;
    r10 = (u32)&lbl_80267A80;
    r12 = 0x0;
    r9 = *(u32*)((u8*)r10 + 0x0);
    r8 = *(u32*)((u8*)r10 + 0x4);
    r7 = *(u32*)((u8*)r10 + 0x8);
    r6 = *(u32*)((u8*)r10 + 0xC);
    r5 = *(u32*)((u8*)r10 + 0x10);
    r0 = *(u32*)((u8*)r10 + 0x14);
    r0 = 0x2;
    ctr_fn = (void(*)(void))r0;
    L_8005D1D8: ;
    r6 = 0x0;
    r5 = *(s16*)((u8*)r4 + 0x6);
    r0 = *(u32*)((u8*)r11 + 0x0);
    if ((s32)r5 != (s32)r0) {
        r6 = 0x1;
        r0 = *(u32*)((u8*)r11 + 0x4);
        if ((s32)r5 != (s32)r0) {
            r6 = 0x2;
            r0 = *(u32*)((u8*)r11 + 0x8);
            if ((s32)r5 != (s32)r0) {
                r6 = 0x3;
    }
    }
    }
    if ((s32)r6 < (s32)0x3) goto L_8005D224;
    r11 = r11 + 0xc;
    r12 = r12 + 0x1;
    if (--ctr != 0) goto L_8005D1D8;
    L_8005D224: ;
    if ((s32)r12 >= (s32)0x2) {
        r3 = 0x0;
    } else {

        r0 = *(u8*)((u8*)r3 + 0x95);
        r3 = r4;
        r0 = (s8)r0;
        if ((s32)r0 == (s32)r12) {
            r4 = 0x1;
        } else {

            r4 = 0x0;
        }
        fn_80109220();
        r3 = 0x0;
    }
    return;
}


/* 0x8005D26C | size: 0x7C */
s32 fn_8005D26C(void) {
    extern void fn_80102428();
    extern void fn_80102510();
    extern void fn_8010264C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    
    r5 = *(u32*)&lbl_8047BF30;
    r3 = 0x9e;
    r0 = *(u32*)&lbl_8047BF34;
    r4 = 0x1;
    *(u32*)(sp + 0xC) = r0;
    fn_8010264C();
    r31 = r3;
    r3 = 0x9e;
    fn_80102510();
    r3 = 0x9e;
    r4 = 0x1;
    fn_80102428();
    if ((s32)r31 < (s32)-0x1) { r3 = 0x1; return; }
    if ((s32)r31 >= (s32)0x2) {

        r3 = 0x1;
        return;
    }
    r0 = r31 << 2;
    r3 = (u32)sp + 0x8;
    r3 = *(u32*)(r3 + r0);

    return;
}


/* 0x8005D2E8 | size: 0xE8 */
s32 fn_8005D2E8(void) {
    extern void fn_80109220();
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
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    
    r5 = (u32)&lbl_80267A98;
    r11 = (u32)sp + 0x8;
    r10 = (u32)&lbl_80267A98;
    r12 = 0x0;
    r9 = *(u32*)((u8*)r10 + 0x0);
    r8 = *(u32*)((u8*)r10 + 0x4);
    r7 = *(u32*)((u8*)r10 + 0x8);
    r6 = *(u32*)((u8*)r10 + 0xC);
    r5 = *(u32*)((u8*)r10 + 0x10);
    r0 = *(u32*)((u8*)r10 + 0x14);
    r0 = 0x2;
    ctr_fn = (void(*)(void))r0;
    L_8005D33C: ;
    r6 = 0x0;
    r5 = *(s16*)((u8*)r4 + 0x6);
    r0 = *(u32*)((u8*)r11 + 0x0);
    if ((s32)r5 != (s32)r0) {
        r6 = 0x1;
        r0 = *(u32*)((u8*)r11 + 0x4);
        if ((s32)r5 != (s32)r0) {
            r6 = 0x2;
            r0 = *(u32*)((u8*)r11 + 0x8);
            if ((s32)r5 != (s32)r0) {
                r6 = 0x3;
    }
    }
    }
    if ((s32)r6 < (s32)0x3) goto L_8005D388;
    r11 = r11 + 0xc;
    r12 = r12 + 0x1;
    if (--ctr != 0) goto L_8005D33C;
    L_8005D388: ;
    if ((s32)r12 >= (s32)0x2) {
        r3 = 0x0;
    } else {

        r0 = *(u8*)((u8*)r3 + 0x95);
        r3 = r4;
        r0 = (s8)r0;
        if ((s32)r0 == (s32)r12) {
            r4 = 0x1;
        } else {

            r4 = 0x0;
        }
        fn_80109220();
        r3 = 0x0;
    }
    return;
}


/* 0x8005D3D0 | size: 0xDC */
s32 fn_8005D3D0(void) {
    extern void fn_80102428();
    extern void fn_80102510();
    extern void fn_801026A4();
    extern void fn_801046B8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r31 = 0;

    
    r5 = *(u32*)&lbl_8047BF38;
    r0 = 0x0;
    r4 = *(u32*)&lbl_8047BF3C;
    r6 = (u32)sp + 0xc;
    *(u32*)(sp + 0x8) = r0;
    while (1) {
        if ((s32)r4 >= (s32)0x2) break;
        r0 = *(u32*)((u8*)r6 + 0x0);
        if ((s32)r3 == (s32)r0) break;
        r6 = r6 + 0x4;
        r0 = r4 + 0x1;
        *(u32*)(sp + 0x8) = r0;

    }

    if ((s32)r4 >= (s32)0x2) {
        r0 = 0x0;
        *(u32*)(sp + 0x8) = r0;
    }
    fn_801046B8();
    r4 = r3;
    r5 = (u32)sp + 0x8;
    r3 = 0xa7;
    r6 = 0x0;
    r7 = 0x1;
    r8 = 0x0;
    /* crclr cr1eq */;
    fn_801026A4();
    r31 = r3;
    r3 = 0xa7;
    fn_80102510();
    r3 = 0xa7;
    r4 = 0x1;
    fn_80102428();
    if ((s32)r31 <= (s32)-0x1) { r3 = 0x1; return; }
    if ((s32)r31 >= (s32)0x2) {

        r3 = 0x1;
        return;
    }
    r0 = r31 << 2;
    r3 = (u32)sp + 0xc;
    r3 = *(u32*)(r3 + r0);

    return;
}


/* 0x8005D4AC | size: 0x48 */
s32 fn_8005D4AC(void) {
    extern void fn_80109220();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    
    r0 = *(u8*)&lbl_8047A5A8;
    r3 = r4;
    r0 = r0 & 0x00000008;
    if ((s32)r0 != (s32)0x0) {
        r0 = 0x1;
    } else {

        r0 = 0x0;
    }
    r4 = r0 & 0xFF;
    fn_80109220();
    r3 = 0x0;
    return;
}


/* 0x8005D4F4 | size: 0x48 */
s32 fn_8005D4F4(void) {
    extern void fn_80109220();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    
    r0 = *(u8*)&lbl_8047A5A8;
    r3 = r4;
    r0 = r0 & 0x00000004;
    if ((s32)r0 != (s32)0x0) {
        r0 = 0x1;
    } else {

        r0 = 0x0;
    }
    r4 = r0 & 0xFF;
    fn_80109220();
    r3 = 0x0;
    return;
}


/* 0x8005D53C | size: 0x48 */
s32 fn_8005D53C(void) {
    extern void fn_80109220();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    
    r0 = *(u8*)&lbl_8047A5A8;
    r3 = r4;
    r0 = r0 & 0x00000002;
    if ((s32)r0 != (s32)0x0) {
        r0 = 0x1;
    } else {

        r0 = 0x0;
    }
    r4 = r0 & 0xFF;
    fn_80109220();
    r3 = 0x0;
    return;
}


/* 0x8005D584 | size: 0x48 */
s32 fn_8005D584(void) {
    extern void fn_80109220();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    
    r0 = *(u8*)&lbl_8047A5A8;
    r3 = r4;
    r0 = r0 & 0x1;
    if ((s32)r0 != (s32)0x0) {
        r0 = 0x1;
    } else {

        r0 = 0x0;
    }
    r4 = r0 & 0xFF;
    fn_80109220();
    r3 = 0x0;
    return;
}


/* 0x8005D5CC | size: 0xDC */
s32 fn_8005D5CC(void) {
    extern void fn_80109220();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    
    r5 = (u32)&lbl_80267AB0;
    r0 = 0x9;
    r5 = (u32)&lbl_80267AB0;
    r7 = (u32)sp + 0x4;
    /* subi r6, r5, 0x4 */;
    ctr_fn = (void(*)(void))r0;
    do {
        r5 = *(u32*)((u8*)r6 + 0x4);
        r0 = *(u32*)((u8*)r6 + 0x8);
        *(u32*)((u8*)r7 + 0x4) = r5;
        r7 += 8; *(u32*)r7 = r0;
    } while (--ctr != 0);
    r6 = (u32)sp + 0x8;
    r8 = 0x0;
    r0 = 0x6;
    ctr_fn = (void(*)(void))r0;
    L_8005D614: ;
    r7 = 0x0;
    r5 = *(s16*)((u8*)r4 + 0x6);
    r0 = *(u32*)((u8*)r6 + 0x0);
    if ((s32)r5 != (s32)r0) {
        r7 = 0x1;
        r0 = *(u32*)((u8*)r6 + 0x4);
        if ((s32)r5 != (s32)r0) {
            r7 = 0x2;
            r0 = *(u32*)((u8*)r6 + 0x8);
            if ((s32)r5 != (s32)r0) {
                r7 = 0x3;
    }
    }
    }
    if ((s32)r7 < (s32)0x3) goto L_8005D660;
    r6 = r6 + 0xc;
    r8 = r8 + 0x1;
    if (--ctr != 0) goto L_8005D614;
    L_8005D660: ;
    if ((s32)r8 >= (s32)0x6) {
        r3 = 0x0;
    } else {

        r0 = *(u8*)((u8*)r3 + 0x95);
        r3 = r4;
        r0 = (s8)r0;
        if ((s32)r0 == (s32)r8) {
            r4 = 0x1;
        } else {

            r4 = 0x0;
        }
        fn_80109220();
        r3 = 0x0;
    }
    return;
}


/* 0x8005D6A8 | size: 0x90 */
s32 fn_8005D6A8(void) {
    extern void fn_80102ED4();
    extern void fn_80105624();
    extern void fn_80166A50();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    
    r31 = r3;
    fn_80105624();
    r4 = *(u8*)((u8*)r31 + 0x95);
    r0 = *(u32*)&lbl_8047BF40;
    r4 = (s8)r4;
    *(u32*)(sp + 0x8) = r0;
    if ((s32)r4 >= (s32)0x0) {
        if ((s32)r4 <= (s32)0x3) {
            r0 = *(u16*)((u8*)r3 + 0x4);
            r0 = r0 & 0x00000010;
            if ((s32)r0 != (s32)0x0) {
                r3 = (u32)sp + 0x8;
                r6 = *(u8*)&lbl_8047A5A8;
                r0 = *(u8*)(r3 + r4);
                r3 = 0x3c6;
                r4 = 0x0;
                r5 = 0xff;
                r0 = r6 ^ r0;
                r6 = 0x0;
                *(u8*)&lbl_8047A5A8 = r0;
                fn_80166A50();
                return;
    }
    }
    }
    r3 = r31;
    fn_80102ED4();

    return;
}


/* 0x8005D738 | size: 0x60 */
s32 fn_8005D738(void) {
    extern void fn_80102428();
    extern void fn_80102510();
    extern void fn_8010264C();
    u8 sp[0x10];
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    
    *(u8*)&lbl_8047A5A8 = r3;
    r3 = 0x9d;
    r4 = 0x1;
    fn_8010264C();
    r31 = r3;
    r3 = 0x9d;
    fn_80102510();
    r3 = 0x9d;
    r4 = 0x1;
    fn_80102428();
    if ((s32)r31 == (s32)0x73d) {
        r3 = *(u8*)&lbl_8047A5A8;
    } else {

        r3 = 0xff;
    }
    return;
}


/* 0x8005D798 | size: 0x60 */
void fn_8005D798(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    
    r0 = r4 & 0xFF;
    r4 = 0x0;
    if ((s32)r0 != (s32)0x3) {
        if ((s32)r0 < (s32)0x3) {
            if ((s32)r0 != (s32)0x1) {
                if ((s32)r0 < (s32)0x1) {
                    r3 = r4;
                    return;
                }
                if ((s32)r0 != (s32)0x5) {
                    if ((s32)r0 >= (s32)0x5) { r3 = r4; return; }
                    goto L_8005D7E4;
                    }
                r4 = *(u16*)((u8*)r3 + 0x4);
                r3 = r4;
                return;
                    }
            r4 = *(u16*)((u8*)r3 + 0x6);
            r3 = r4;
            return;
        }
        r4 = *(u16*)((u8*)r3 + 0x8);
        r3 = r4;
        return;
        L_8005D7E4: ;
        r4 = *(u16*)((u8*)r3 + 0x0);
        r3 = r4;
        return;
                }
    r4 = *(u16*)((u8*)r3 + 0x2);

    r3 = r4;
    return;
}


/* 0x8005D7F8 | size: 0x38 */
s32 fn_8005D7F8(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    
    r5 = *(u32*)&lbl_80478E94;
    if ((u32)r5 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r4 = *(u32*)&lbl_80478E90;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r3 >= (u32)r0) {
        r3 = 0x0;
        return;
    }
    r0 = r3 * 0xa;
    r3 = r5 + r0;
    return;
}


/* 0x8005D830 | size: 0x28 */
s32 fn_8005D830(void) {
    return 0;
}

/* 0x8005D858 | size: 0x28 */
s32 fn_8005D858(void) {
    return 0;
}

/* 0x8005D880 | size: 0x38 */
s32 fn_8005D880(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    
    r0 = *(u32*)&lbl_80478968;
    if ((u32)r3 >= (u32)r0) {
        r3 = 0x0;
    } else {

        r6 = r3 * 0x1c;
        r3 = (u32)&lbl_802EF0A8;
        r0 = (u32)&lbl_802EF0A8;
        r3 = r0 + r6;
    }
    if ((u32)r3 == (u32)0x0) return;
    *(u16*)((u8*)r3 + 0x2) = r4;
    *(u16*)((u8*)r3 + 0x4) = r5;
    return;
}


/* 0x8005D8B8 | size: 0x40 */
s32 fn_8005D8B8(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    
    r0 = *(u32*)&lbl_80478968;
    if ((u32)r3 >= (u32)r0) {
        r3 = 0x0;
    } else {

        r4 = r3 * 0x1c;
        r3 = (u32)&lbl_802EF0A8;
        r0 = (u32)&lbl_802EF0A8;
        r3 = r0 + r4;
    }
    if ((u32)r3 != (u32)0x0) {
        r0 = *(u8*)((u8*)r3 + 0x0);
        /* extrwi r3, r0, 1, 24 */;
        return;
    }
    r3 = 0x0;
    return;
}


/* 0x8005D8F8 | size: 0x3C */
s32 fn_8005D8F8(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    
    r0 = *(u32*)&lbl_80478968;
    if ((u32)r3 >= (u32)r0) {
        r3 = 0x0;
    } else {

        r5 = r3 * 0x1c;
        r3 = (u32)&lbl_802EF0A8;
        r0 = (u32)&lbl_802EF0A8;
        r3 = r0 + r5;
    }
    if ((u32)r3 == (u32)0x0) return;
    r0 = *(u8*)((u8*)r3 + 0x0);
    r0 = (r0 & ~0x00000080) | (((r4 << 7) | ((u32)r4 >> 25)) & 0x00000080);
    *(u8*)((u8*)r3 + 0x0) = r0;
    return;
}


/* 0x8005D934 | size: 0x28 */
s32 fn_8005D934(void) {
    return 0;
}

/* 0x8005D95C | size: 0x50 */
s32 fn_8005D95C(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    
    if ((s32)r3 < (s32)0x0) {
        r3 = 0x0;
    }
    r0 = *(u32*)&lbl_80478848;
    if ((u32)r3 >= (u32)r0) {
        r3 = 0x1;
    }
    r6 = r3 * 0x1c;
    r3 = (u32)&lbl_802E2DB8;
    r0 = (u32)&lbl_802E2DB8;
    r3 = r0 + r6;
    if ((u32)r4 != (u32)0x0) {
        r0 = *(s16*)((u8*)r3 + 0x6);
        *(u16*)((u8*)r4 + 0x0) = r0;
    }
    if ((u32)r5 == (u32)0x0) return;
    r0 = *(s16*)((u8*)r3 + 0x8);
    *(u16*)((u8*)r5 + 0x0) = r0;
    return;
}


/* 0x8005D9AC | size: 0x38 */
s32 fn_8005D9AC(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    
    if ((s32)r3 < (s32)0x0) {
        r3 = 0x0;
    }
    r0 = *(u32*)&lbl_80478848;
    if ((u32)r3 >= (u32)r0) {
        r3 = 0x1;
    }
    r6 = r3 * 0x1c;
    r3 = (u32)&lbl_802E2DB8;
    r0 = (u32)&lbl_802E2DB8;
    r3 = r0 + r6;
    *(u16*)((u8*)r3 + 0x6) = r4;
    *(u16*)((u8*)r3 + 0x8) = r5;
    return;
}


/* 0x8005D9E4 | size: 0x34 */
s32 fn_8005D9E4(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    
    if ((s32)r3 < (s32)0x0) {
        r3 = 0x0;
    }
    r0 = *(u32*)&lbl_80478848;
    if ((u32)r3 >= (u32)r0) {
        r3 = 0x1;
    }
    r4 = r3 * 0x1c;
    r3 = (u32)&lbl_802E2DB8;
    r0 = (u32)&lbl_802E2DB8;
    r3 = r0 + r4;
    r3 = *(u8*)((u8*)r3 + 0x2);
    return;
}


/* 0x8005DA18 | size: 0x30 */
s32 fn_8005DA18(void) {
    return 0;
}

/* 0x8005DA48 | size: 0x17C */
s32 fn_8005DA48(void) {
    extern void fn_800DA1E8();
    extern void fn_800DD270();
    extern void fn_800DD384();
    extern void fn_800FAEF8();
    extern void fn_801040B8();
    extern void fn_801040D0();
    extern void fn_80105624();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    
    r28 = r3;
    r0 = *(u32*)&lbl_8047BF48;
    *(u32*)(sp + 0xC) = r0;
    fn_80105624();
    r0 = *(u16*)((u8*)r3 + 0x4);
    r0 = r0 & 0x00000100;
    if ((s32)r0 != (s32)0x0) {
        r3 = r28;
        r4 = 0x0;
        fn_801040D0();
        r5 = r3 + 0x1;
        r3 = r28;
        r0 = r5 << 30;
        r4 = 0x0;
        r5 = (u32)r5 >> 31;
        r0 = r0 - r5;
        /* rotlwi r0, r0, 2 */;
        r5 = r0 + r5;
        fn_801040B8();
    }
    r3 = r28;
    r4 = 0x0;
    fn_801040D0();
    switch ((s32)r3) {
        case 1:
            r28 = 0xa;
            r29 = 0x82;
            r30 = 0x27;
            break;
        case 2:
            r28 = 0x20;
            r29 = 0x1a0;
            r30 = 0x27;
            break;
        case 3:
            r28 = 0x1;
            r29 = 0xd;
            r30 = 0x1ba;
            break;
        default:
            r28 = 0xa;
            r29 = 0x82;
            r30 = 0x145;
            break;
    }
    r3 = 0x0;
    r4 = 0x7;
    r5 = 0x2;
    fn_800DA1E8();
    /* subi r4, r30, 0x5 */;
    r6 = r29 + 0x12;
    r7 = (u32)sp + 0x8;
    *(u32*)(sp + 0x8) = r0;
    r3 = 0xf;
    r5 = 0x25d;
    ((void(*)(void))fn_8001E58C)();
    r29 = 0x0;
    r31 = (0xc0c1 << 16);
    while ((s32)r29 < (s32)r28) {

        fn_800DD384();
        r0 = r29 + r3;
        r3 = r0 - r28;
        if ((s32)r3 >= (s32)0x0) {
            fn_800DD270();
            r6 = r3;
            r4 = r30;
            /* subi r5, r31, 0x3f01 */;
            r3 = 0x14;
            /* crclr cr1eq */;
            fn_800FAEF8();
        }
        r30 = r30 + 0xd;
        r29 = r29 + 0x1;

    }
    r3 = 0x0;
    return;
}


/* 0x8005DBC4 | size: 0x60 */
s32 fn_8005DBC4(void) {
    extern void fn_80102510();
    extern void fn_80102620();
    extern void fn_801026A4();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;

    
    r3 = 0xbb;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0xbb;
        fn_80102510();
    } else {

        r3 = 0xbb;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x0;
        r7 = 0x1;
        r8 = 0x0;
        /* crclr cr1eq */;
        fn_801026A4();
    }
    r3 = 0x0;
    return;
}


/* 0x8005DC24 | size: 0xA0 */
s32 fn_8005DC24(void) {
    extern void fn_800FF56C();
    extern void fn_80102620();
    extern void fn_801026A4();
    extern void fn_80102868();
    extern void fn_801176C8();
    extern void fn_80117AD4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r31 = 0;

    
    r3 = 0xca;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        fn_800FF56C();
        r31 = r3;
        fn_80117AD4();
        if ((u32)r31 != (u32)r3) {
            r3 = r31;
            fn_801176C8();
            r0 = 0x0;
            *(u32*)&lbl_8047A5B0 = r0;
            *(u32*)&lbl_8047A5B4 = r0;
            *(u32*)&lbl_8047A5B8 = r0;
        }
        r0 = 0x0;
        r3 = 0xca;
        *(u8*)&lbl_8047A5BC = r0;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x0;
        r7 = 0x1;
        r8 = 0x0;
        /* crclr cr1eq */;
        fn_801026A4();
        r3 = 0xca;
        r4 = 0xc;
        r5 = 0xa;
        fn_80102868();
    }
    r3 = 0x0;
    return;
}


/* 0x8005DCC4 | size: 0x224 */
s32 fn_8005DCC4(void) {
    extern void fn_80105624();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    
    r31 = r3;
    fn_80105624();
    r0 = *(u16*)((u8*)r3 + 0x6);
    r0 = r0 & 0x1;
    if ((s32)r0 != (s32)0x0) {
        r4 = *(u8*)((u8*)r31 + 0x95);
        /* subi r4, r4, 0x1 */;
        r0 = (s8)r4;
        *(u8*)((u8*)r31 + 0x95) = r4;
        if ((s32)r0 < (s32)0x0) {
            r0 = 0x0;
            *(u8*)((u8*)r31 + 0x95) = r0;
    }
    }
    r0 = *(u16*)((u8*)r3 + 0x6);
    r0 = r0 & 0x00000002;
    if ((s32)r0 != (s32)0x0) {
        r3 = *(u8*)((u8*)r31 + 0x95);
        r3 = r3 + 0x1;
        r0 = (s8)r3;
        *(u8*)((u8*)r31 + 0x95) = r3;
        if ((s32)r0 > (s32)0x1) {
            r0 = 0x1;
            *(u8*)((u8*)r31 + 0x95) = r0;
    }
    }
    r0 = *(u8*)((u8*)r31 + 0x95);
    r0 = (s8)r0;
    if ((s32)r0 != (s32)0x1) {
        if ((s32)r0 >= (s32)0x1) { r3 = 0x0; return; }
        if ((s32)r0 < (s32)0x0) {
            r3 = 0x0;
            return;
        }
        fn_80105624();
        r0 = *(u16*)((u8*)r3 + 0x6);
        r0 = r0 & 0x00000008;
        if ((s32)r0 != (s32)0x0) {
            r4 = *(u32*)&lbl_8047A5C8;
            r5 = *(u32*)&lbl_80478BD8;
            r0 = r4 + 0x1;
            *(u32*)&lbl_8047A5C8 = r0;
            if ((s32)r0 >= (s32)r5) {
                /* subi r0, r5, 0x1 */;
                *(u32*)&lbl_8047A5C8 = r0;
        }
        }
        r0 = *(u16*)((u8*)r3 + 0x6);
        r0 = r0 & 0x00000400;
        if ((s32)r0 != (s32)0x0) {
            r4 = *(u32*)&lbl_8047A5C8;
            r5 = *(u32*)&lbl_80478BD8;
            r0 = r4 + 0xa;
            *(u32*)&lbl_8047A5C8 = r0;
            if ((s32)r0 >= (s32)r5) {
                /* subi r0, r5, 0x1 */;
                *(u32*)&lbl_8047A5C8 = r0;
        }
        }
        r0 = *(u16*)((u8*)r3 + 0x6);
        r0 = r0 & 0x00000004;
        if ((s32)r0 != (s32)0x0) {
            r4 = *(u32*)&lbl_8047A5C8;
            /* subi r0, r4, 0x1 */;
            *(u32*)&lbl_8047A5C8 = r0;
            if ((s32)r0 < (s32)0x0) {
                r0 = 0x0;
                *(u32*)&lbl_8047A5C8 = r0;
        }
        }
        r0 = *(u16*)((u8*)r3 + 0x6);
        r0 = r0 & 0x00000200;
        if ((s32)r0 == (s32)0x0) { r3 = 0x0; return; }
        r3 = *(u32*)&lbl_8047A5C8;
        /* subi r0, r3, 0xa */;
        *(u32*)&lbl_8047A5C8 = r0;
        if ((s32)r0 >= (s32)0x0) { r3 = 0x0; return; }
        r0 = 0x0;
        *(u32*)&lbl_8047A5C8 = r0;
        r3 = 0x0;
        return;
    }
    fn_80105624();
    r0 = *(u16*)((u8*)r3 + 0x6);
    r0 = r0 & 0x00000008;
    if ((s32)r0 != (s32)0x0) {
        r4 = *(u32*)&lbl_8047A5C4;
        r0 = r4 + 0x1;
        *(u32*)&lbl_8047A5C4 = r0;
        if ((s32)r0 > (s32)0x3e7) {
            r0 = 0x3e7;
            *(u32*)&lbl_8047A5C4 = r0;
    }
    }
    r0 = *(u16*)((u8*)r3 + 0x6);
    r0 = r0 & 0x00000400;
    if ((s32)r0 != (s32)0x0) {
        r4 = *(u32*)&lbl_8047A5C4;
        r0 = r4 + 0xa;
        *(u32*)&lbl_8047A5C4 = r0;
        if ((s32)r0 > (s32)0x3e7) {
            r0 = 0x3e7;
            *(u32*)&lbl_8047A5C4 = r0;
    }
    }
    r0 = *(u16*)((u8*)r3 + 0x6);
    r0 = r0 & 0x00000004;
    if ((s32)r0 != (s32)0x0) {
        r4 = *(u32*)&lbl_8047A5C4;
        /* subi r0, r4, 0x1 */;
        *(u32*)&lbl_8047A5C4 = r0;
        if ((s32)r0 < (s32)0x0) {
            r0 = 0x0;
            *(u32*)&lbl_8047A5C4 = r0;
    }
    }
    r0 = *(u16*)((u8*)r3 + 0x6);
    r0 = r0 & 0x00000200;
    if ((s32)r0 == (s32)0x0) { r3 = 0x0; return; }
    r3 = *(u32*)&lbl_8047A5C4;
    /* subi r0, r3, 0xa */;
    *(u32*)&lbl_8047A5C4 = r0;
    if ((s32)r0 >= (s32)0x0) { r3 = 0x0; return; }
    r0 = 0x0;
    *(u32*)&lbl_8047A5C4 = r0;

    r3 = 0x0;
    return;
}


/* 0x8005DEE8 | size: 0xE0 */
s32 fn_8005DEE8(void) {
    extern void fn_80102428();
    extern void fn_80102510();
    extern void fn_80102568();
    extern void fn_8010264C();
    extern void fn_80129A78();
    extern void fn_80142984();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    
    r0 = *(u32*)&lbl_8047A5C0;
    if ((s32)r0 == (s32)0x0) {
        r0 = 0x1;
        *(u32*)&lbl_8047A5C8 = r0;
        *(u32*)&lbl_8047A5C0 = r0;
    }
    r0 = 0x1;
    *(u32*)&lbl_8047A5C4 = r0;
    L_8005DF18: ;
    r3 = 0xcb;
    r4 = 0x1;
    fn_8010264C();
    if ((s32)r3 == (s32)-0x1) goto L_8005DFA0;
    r0 = *(u32*)&lbl_8047A5C8;
    r3 = r0 & 0xFFFF;
    fn_80142984();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8005DF18;
    r0 = *(u32*)&lbl_8047A5C4;
    if ((s32)r0 < (s32)0x1 || (s32)r0 > (s32)0x3e7) goto L_8005DF18;

    r3 = 0x44;
    r4 = 0x1;
    fn_8010264C();
    r31 = r3;
    r3 = 0x44;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
    if ((s32)r31 != (s32)0x0) goto L_8005DF18;
    r4 = *(u32*)&lbl_8047A5C8;
    r3 = 0x0;
    r0 = *(u32*)&lbl_8047A5C4;
    r6 = -0x1;
    r4 = r4 & 0xFFFF;
    r5 = r0 & 0xFFFF;
    fn_80129A78();
    goto L_8005DF18;
    L_8005DFA0: ;
    r3 = 0xcb;
    fn_80102510();
    r3 = 0xcb;
    r4 = 0x1;
    fn_80102428();
    return;
}


/* 0x8005DFC8 | size: 0x6C8 */
s32 fn_8005DFC8(void) {
    extern void fn_80060A28();
    extern void fn_80069048();
    extern void fn_800D3088();
    extern void fn_800D37CC();
    extern void fn_801080CC();
    extern void fn_801666BC();
    extern void fn_8017B000();
    extern void fn_8025DA88();
    extern void fn_800626CC();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    
    *(f64*)(sp + 0x20) = f31;
    /* psq_st f31, 0x28((u32)sp), 0, qr0 */;
    r30 = r3;
    fn_800D37CC();
    /* xoris r3, r3, 0x8000 */;
    r0 = (0x4330 << 16);
    f1 = *(f64*)&lbl_8047BF80;
    *(u32*)(sp + 0x8) = r0;
    f0 = *(f64*)(sp + 0x8);
    f31 = f0 - f1;
    fn_800D3088();
    r5 = (0x4330 << 16);
    r4 = (u32)&lbl_803A9A60;
    r31 = (u32)&lbl_803A9A60;
    r0 = *(u32*)((u8*)r31 + 0x38);
    f1 = *(f64*)&lbl_8047BF88;
    f0 = *(f64*)(sp + 0x10);
    f0 = f0 - f1;
    f2 = f0 / f31;
    *(f32*)((u8*)r31 + 0x3C) = f2;
    if ((s32)r0 == (s32)0x7) goto L_8005E620;
    if ((s32)r0 < (s32)0x7) {
        if ((s32)r0 == (s32)0x3) goto L_8005E398;
        if ((s32)r0 < (s32)0x3) {
            if ((s32)r0 != (s32)0x1) {
                if ((s32)r0 < (s32)0x1) {
                    if ((s32)r0 < (s32)0x0) {
                        goto L_8005E670;
                    }
                    if ((s32)r0 == (s32)0x5) goto L_8005E4F8;
                    if ((s32)r0 >= (s32)0x5) goto L_8005E520;
                    goto L_8005E3DC;
                }
                if ((s32)r0 == (s32)0xb) goto L_8005E654;
                if ((s32)r0 < (s32)0xb) {
                    if ((s32)r0 == (s32)0x9) goto L_8005E670;
                    if ((s32)r0 >= (s32)0x9) goto L_8005E3C0;
                    goto L_8005E648;
                }
                if ((s32)r0 == (s32)0x64) goto L_8005E670;
                goto L_8005E670;
                    }
            fn_80069048();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x0) goto L_8005E670;
            r3 = (u32)&lbl_803A9A60;
            r30 = (u32)&lbl_803A9A60;
            r0 = *(u8*)((u8*)r30 + 0x34);
            if ((u32)r0 == (u32)0x0) {
                r0 = 0x0;
                *(u8*)((u8*)r30 + 0x34) = r0;
                *(u32*)((u8*)r30 + 0x2C) = r0;
                fn_8025DA88();
                if ((s32)r3 == (s32)0x2) {
                    r3 = (u32)&lbl_803A9A60;
                    r0 = 0x4;
                    r3 = (u32)&lbl_803A9A60;
                    *(u32*)((u8*)r3 + 0x30) = r0;
                } else {

                    r3 = (u32)&lbl_803A9A60;
                    r0 = 0x2;
                    r3 = (u32)&lbl_803A9A60;
                    *(u32*)((u8*)r3 + 0x30) = r0;
                }
                r0 = 0x0;
                *(u32*)((u8*)r30 + 0x10) = r0;
                *(u32*)((u8*)r30 + 0xC) = r0;
                *(u32*)((u8*)r30 + 0x18) = r0;
                *(u32*)((u8*)r30 + 0x14) = r0;
                *(u32*)((u8*)r30 + 0x20) = r0;
                *(u32*)((u8*)r30 + 0x1C) = r0;
                *(u32*)((u8*)r30 + 0x28) = r0;
                *(u32*)((u8*)r30 + 0x24) = r0;
                fn_8025DA88();
                if ((s32)r3 == (s32)0x2) {
                    r3 = 0x0;
                } else {

                    r3 = 0x1;
                }
                r0 = 0x1;
                *(u8*)((u8*)r30 + 0x34) = r0;
                if ((s32)r3 == (s32)0x0) {
                    r3 = 0x5c4;
                } else {

                    r3 = 0x5c3;
                }
                r5 = (u32)fn_800626CC;
                r4 = 0x0;
                r5 = (u32)fn_800626CC;
                r6 = 0x0;
                r7 = 0x0;
                fn_8017B000();
                r0 = 0x1;
                *(u32*)((u8*)r31 + 0x38) = r0;
                goto L_8005E670;
            }
            r0 = 0x2;
            *(u32*)((u8*)r31 + 0x38) = r0;
            goto L_8005E670;
                }
        fn_80069048();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x0) goto L_8005E1B4;
        r3 = (u32)&lbl_803A9A60;
        r3 = (u32)&lbl_803A9A60;
        r0 = *(u8*)((u8*)r3 + 0x34);
        if ((u32)r0 == (u32)0x0) goto L_8005E1B4;
        r0 = 0x1;
        goto L_8005E1B8;
        L_8005E1B4: ;
        r0 = 0x0;
        L_8005E1B8: ;
        r0 = r0 & 0xFF;
        if ((u32)r0 == (u32)0x0) goto L_8005E670;
        r0 = 0x2;
        *(u32*)((u8*)r31 + 0x38) = r0;
        goto L_8005E670;
                }
    fn_80060A28();
    r3 = (u32)&lbl_803A9A60;
    r3 = (u32)&lbl_803A9A60;
    r5 = r3;
    r0 = 0x4;
    ctr_fn = (void(*)(void))r0;
                        do {
        r4 = r5 + 0x58;
        f1 = *(f32*)((u8*)r4 + 0x3C);
        f0 = *(f32*)((u8*)r4 + 0x54);
        if (f1 == f0) {


        r6 = r4 + 0x4;
        f1 = *(f32*)((u8*)r6 + 0x3C);
        f0 = *(f32*)((u8*)r6 + 0x54);
        if (f1 == f0) {


            r6 = r4 + 0x8;
            f1 = *(f32*)((u8*)r6 + 0x3C);
            f0 = *(f32*)((u8*)r6 + 0x54);
        if (f1 == f0) {


                r6 = r4 + 0xc;
                f1 = *(f32*)((u8*)r6 + 0x3C);
                f0 = *(f32*)((u8*)r6 + 0x54);
        if (f1 == f0) {


                    r6 = r4 + 0x10;
                    f1 = *(f32*)((u8*)r6 + 0x3C);
                    f0 = *(f32*)((u8*)r6 + 0x54);
        if (f1 == f0) {


                        r6 = r4 + 0x14;
                        f1 = *(f32*)((u8*)r6 + 0x3C);
                        f0 = *(f32*)((u8*)r6 + 0x54);
        if (f1 == f0) {


                            r5 = r5 + 0xb4;
                        } while (--ctr != 0);
                        r4 = r3 + 0x328;
                        f1 = *(f32*)((u8*)r4 + 0x4);
                        f0 = *(f32*)((u8*)r4 + 0x8);
    if (f1 == f0) {


                            r5 = r3 + 0xc;
                            r4 = r5 + 0x328;
                            f1 = *(f32*)((u8*)r4 + 0x4);
                            f0 = *(f32*)((u8*)r4 + 0x8);
    if (f1 == f0) {


                                r5 = r5 + 0xc;
                                r4 = r5 + 0x328;
                                f1 = *(f32*)((u8*)r4 + 0x4);
                                f0 = *(f32*)((u8*)r4 + 0x8);
    if (f1 == f0) {


                                    r5 = r5 + 0xc;
                                    r4 = r5 + 0x328;
                                    f1 = *(f32*)((u8*)r4 + 0x4);
                                    f0 = *(f32*)((u8*)r4 + 0x8);
    if (f1 == f0) {


                                        f1 = *(f32*)((u8*)r3 + 0x48);
                                        f0 = *(f32*)((u8*)r3 + 0x50);
    if (f1 == f0) {


                                            r3 = r3 + 0x4;
                                            f1 = *(f32*)((u8*)r3 + 0x48);
                                            f0 = *(f32*)((u8*)r3 + 0x50);
    if (f1 == f0) {


                                                r3 = (u32)&lbl_803A9A60;
                                                r0 = 0x1;
                                                r3 = (u32)&lbl_803A9A60;
                                                *(u8*)((u8*)r3 + 0x368) = r0;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8005E670;
    r3 = (u32)&lbl_803A9A60;
    r3 = (u32)&lbl_803A9A60;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((s32)r0 == (s32)0x0) {
        f0 = *(f32*)&lbl_8047BF60;
        r0 = 0x3;
        *(u32*)((u8*)r31 + 0x38) = r0;
        *(f32*)((u8*)r3 + 0x3B8) = f0;
        goto L_8005E670;
    }
    r0 = 0x4;
    *(u32*)((u8*)r31 + 0x38) = r0;
    goto L_8005E670;
    L_8005E398: ;
    f1 = *(f32*)((u8*)r31 + 0x3B8);
    f0 = *(f32*)&lbl_8047BF64;
    f1 = f1 + f2;
    *(f32*)((u8*)r31 + 0x3B8) = f1;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_8005E670;
    r0 = 0xa;
    *(u32*)((u8*)r31 + 0x38) = r0;
    goto L_8005E670;
    L_8005E3C0: ;
    r3 = *(u32*)((u8*)r31 + 0x3BC);
    fn_801666BC();
    if ((s32)r3 != (s32)0x0) goto L_8005E670;
    r0 = 0xb;
    *(u32*)((u8*)r31 + 0x38) = r0;
    goto L_8005E670;
    L_8005E3DC: ;
    r7 = r31;
    r3 = 0x1;
    r5 = 0x0;
    f1 = *(f32*)&lbl_8047BF68;
    do {
        r6 = r7 + 0x58;
        r4 = 0x0;
        r0 = 0x2;
        ctr_fn = (void(*)(void))r0;
        do {
            f0 = *(f32*)((u8*)r6 + 0x84);
            f3 = *(f32*)((u8*)r6 + 0x6C);
            if (f0 != f3) {
                f2 = *(f32*)((u8*)r6 + 0x9C);
                f0 = *(f32*)((u8*)r31 + 0x3C);
                f0 = f2 * f0;
                f0 = f0 * f1;
                f0 = f3 - f0;
                *(f32*)((u8*)r6 + 0x6C) = f0;
                f0 = *(f32*)((u8*)r6 + 0x6C);
                f2 = *(f32*)((u8*)r6 + 0x84);
                if (f0 < f2) {
                    *(f32*)((u8*)r6 + 0x6C) = f2;
                }
                r3 = 0x0;
            }
            r6 = r6 + 0x4;
            f0 = *(f32*)((u8*)r6 + 0x84);
            f3 = *(f32*)((u8*)r6 + 0x6C);
            if (f0 != f3) {
                f2 = *(f32*)((u8*)r6 + 0x9C);
                f0 = *(f32*)((u8*)r31 + 0x3C);
                f0 = f2 * f0;
                f0 = f0 * f1;
                f0 = f3 - f0;
                *(f32*)((u8*)r6 + 0x6C) = f0;
                f0 = *(f32*)((u8*)r6 + 0x6C);
                f2 = *(f32*)((u8*)r6 + 0x84);
                if (f0 < f2) {
                    *(f32*)((u8*)r6 + 0x6C) = f2;
                }
                r3 = 0x0;
            }
            r6 = r6 + 0x4;
            f0 = *(f32*)((u8*)r6 + 0x84);
            f3 = *(f32*)((u8*)r6 + 0x6C);
            if (f0 != f3) {
                f2 = *(f32*)((u8*)r6 + 0x9C);
                f0 = *(f32*)((u8*)r31 + 0x3C);
                f0 = f2 * f0;
                f0 = f0 * f1;
                f0 = f3 - f0;
                *(f32*)((u8*)r6 + 0x6C) = f0;
                f0 = *(f32*)((u8*)r6 + 0x6C);
                f2 = *(f32*)((u8*)r6 + 0x84);
                if (f0 < f2) {
                    *(f32*)((u8*)r6 + 0x6C) = f2;
                }
                r3 = 0x0;
            }
            r6 = r6 + 0x4;
            r4 = r4 + 0x2;
        } while (--ctr != 0);
        r7 = r7 + 0xb4;
        r5 = r5 + 0x1;
    } while ((s32)r5 < (s32)0x4);
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8005E670;
    r0 = 0x5;
    *(u32*)((u8*)r31 + 0x38) = r0;
    goto L_8005E670;
    L_8005E4F8: ;
    f1 = *(f32*)((u8*)r31 + 0x3B4);
    f0 = *(f32*)&lbl_8047BF6C;
    f1 = f1 + f2;
    *(f32*)((u8*)r31 + 0x3B4) = f1;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_8005E670;
    r0 = 0x6;
    *(u32*)((u8*)r31 + 0x38) = r0;
    goto L_8005E670;
    L_8005E520: ;
    r3 = r31;
    r0 = 0x2;
    ctr_fn = (void(*)(void))r0;
    do {
        r4 = r3 + 0x358;
        f0 = *(f32*)((u8*)r4 + 0x0);
        f1 = *(f32*)((u8*)r4 + 0x4);
        if (f0 != f1) {
            f1 = f1 - f0;
            f2 = *(f32*)&lbl_8047BF70;
            f0 = *(f32*)((u8*)r31 + 0x3C);
            f1 = f2 * f1;
            f3 = f1 * f0;
            if (f3 > f2) {
                f3 = f2;
            }
            f0 = *(f32*)&lbl_8047BF74;
            /* cror eq, lt, eq */;
            if (f3 == f0) {
                f3 = f0;
            }
            f1 = *(f32*)((u8*)r4 + 0x0);
            f0 = *(f32*)&lbl_8047BF60;
            f1 = f1 + f3;
            *(f32*)((u8*)r4 + 0x0) = f1;
            f2 = *(f32*)((u8*)r4 + 0x4);
            f0 = *(f32*)((u8*)r4 + 0x0);
            f1 = f2 - f0;
            if (f3 > f0) {
            } else {

                f3 = -f3;
            }
            f0 = *(f32*)&lbl_8047BF60;
            if (f1 > f0) {
            } else {

                f1 = -f1;
            }
            /* cror eq, lt, eq */;
            if (f1 == f3) {
                *(f32*)((u8*)r4 + 0x0) = f2;
        }
        }
        r3 = r3 + 0x8;
    } while (--ctr != 0);
    r3 = (u32)&lbl_803A9A60;
    f0 = *(f32*)&lbl_8047BF60;
    r3 = (u32)&lbl_803A9A60;
    f2 = *(f32*)((u8*)r3 + 0x358);
    f1 = *(f32*)((u8*)r3 + 0x35C);
    f1 = f2 - f1;
    if (f1 > f0) {
    } else {

        f1 = -f1;
    }
    f0 = *(f32*)&lbl_8047BF78;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_8005E670;
    r3 = (u32)&lbl_803A9A60;
    f0 = *(f32*)&lbl_8047BF60;
    r3 = (u32)&lbl_803A9A60;
    r0 = 0x7;
    *(u32*)((u8*)r31 + 0x38) = r0;
    *(f32*)((u8*)r3 + 0x3B8) = f0;
    goto L_8005E670;
    L_8005E620: ;
    f1 = *(f32*)((u8*)r31 + 0x3B8);
    f0 = *(f32*)&lbl_8047BF7C;
    f1 = f1 + f2;
    *(f32*)((u8*)r31 + 0x3B8) = f1;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_8005E670;
    r0 = 0x8;
    *(u32*)((u8*)r31 + 0x38) = r0;
    goto L_8005E670;
    L_8005E648: ;
    r0 = 0x9;
    *(u32*)((u8*)r31 + 0x38) = r0;
    goto L_8005E670;
    L_8005E654: ;
    r0 = 0x64;
    r4 = 0x1c6;
    *(u32*)((u8*)r31 + 0x38) = r0;
    r3 = *(u32*)((u8*)r30 + 0x4);
    fn_801080CC();
    r0 = 0x1;
    *(u8*)((u8*)r30 + 0x2) = r0;
    L_8005E670: ;
    /* psq_l f31, 0x28((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x20);
    return;
}


/* 0x8005E690 | size: 0xA0 */
void fn_8005E690(void) {
    extern void fn_80102ED4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    
    r30 = r3;
    r4 = (u32)&lbl_803A9A60;
    r31 = (u32)&lbl_803A9A60;
    r0 = *(u32*)((u8*)r31 + 0x4);
    if ((s32)r0 != (s32)0x1) {
        if ((s32)r0 >= (s32)0x1) return;
        if ((s32)r0 < (s32)0x0) {
            return;
        }
        r0 = *(u32*)((u8*)r31 + 0x38);
        if ((s32)r0 >= (s32)0x3) {
            fn_80102ED4();
        }
        r0 = *(u32*)((u8*)r31 + 0x38);
        if ((s32)r0 != (s32)0x64) return;
        r0 = 0x1;
        *(u8*)((u8*)r30 + 0x98) = r0;
        return;
    }
    r0 = *(u32*)((u8*)r31 + 0x38);
    if ((s32)r0 >= (s32)0x7) {
        fn_80102ED4();
    }
    r0 = *(u32*)((u8*)r31 + 0x38);
    if ((s32)r0 != (s32)0x9) return;
    r0 = 0x1;
    *(u8*)((u8*)r30 + 0x98) = r0;

    return;
}


/* 0x8005E730 | size: 0x20 */
void fn_8005E730(void) {
    fn_8005DFC8();
}

/* 0x8005E750 | size: 0xA0 */
s32 fn_8005E750(void) {
    extern void fn_80061F6C();
    extern void fn_80102568();
    extern void fn_8010264C();
    extern void fn_80103CC0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    
    r31 = r3;
    r4 = 0x0;
    fn_80061F6C();
    r3 = (u32)&lbl_803A9A60;
    r0 = 0xba;
    r4 = (u32)&lbl_803A9A60;
    r3 = 0x0;
    *(u16*)((u8*)r4 + 0x8) = r0;
    fn_80103CC0();
    r3 = 0xdf;
    r4 = 0x0;
    fn_8010264C();
    r3 = 0xba;
    r4 = 0x1;
    fn_8010264C();
    r3 = 0x1;
    fn_80103CC0();
    r3 = 0xba;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
    r3 = (u32)&lbl_803A9A60;
    r0 = 0x0;
    r3 = (u32)&lbl_803A9A60;
    *(u32*)((u8*)r3 + 0x4) = r0;
    r0 = *(u32*)((u8*)r31 + 0x4);
    if ((s32)r0 != (s32)0x2) {
        r3 = 0xc4;
    } else {

        r3 = 0xc6;
    }
    return;
}


/* 0x8005E7F0 | size: 0x17F4 */
void fn_8005E7F0(void) {
    extern void fn_80060434();
    extern void fn_800608C4();
    extern void fn_800609B4();
    extern void fn_80060D70();
    extern void fn_80060EF4();
    extern void fn_8006106C();
    extern void fn_80061454();
    extern void fn_800615F4();
    extern void fn_800617E0();
    extern void fn_80061A2C();
    extern void fn_80061B74();
    extern void fn_80061BBC();
    u32 r0 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    f32 f1 = 0.0f;

    
    r0 = *(s16*)((u8*)r4 + 0x6);
    r5 = (u32)&lbl_803A9E40;
    r5 = (u32)&lbl_803A9E40;
    if ((s32)r0 == (s32)0xc7f) goto L_8005F648;
    if ((s32)r0 < (s32)0xc7f) {
        if ((s32)r0 == (s32)0xc39) goto L_8005F210;
        if ((s32)r0 < (s32)0xc39) {
            if ((s32)r0 == (s32)0xc16) goto L_8005EFF4;
            if ((s32)r0 < (s32)0xc16) {
                if ((s32)r0 == (s32)0xc02) goto L_8005EEDC;
                if ((s32)r0 < (s32)0xc02) {
                    if ((s32)r0 < (s32)0xbf8) {
                        if ((s32)r0 < (s32)0xbf3) {
                            if ((s32)r0 == (s32)0x8a6) goto L_8005EEC0;
                            if ((s32)r0 < (s32)0x8a6) return;
                            if ((s32)r0 >= (s32)0xbf1) goto L_8005FE28;
                            return;
                        }
                        if ((s32)r0 == (s32)0xbf5) goto L_8005FE40;
                        if ((s32)r0 >= (s32)0xbf5) goto L_8005FE4C;
                        goto L_8005FE34;
                    }
                    if ((s32)r0 == (s32)0xbfe) goto L_8005FE7C;
                    if ((s32)r0 < (s32)0xbfe) {
                        if ((s32)r0 >= (s32)0xbfc) goto L_8005FE70;
                        if ((s32)r0 >= (s32)0xbfa) goto L_8005FE64;
                        goto L_8005FE58;
                    }
                    if ((s32)r0 == (s32)0xc00) goto L_8005FFBC;
                    if ((s32)r0 >= (s32)0xc00) goto L_8005EEC8;
                    goto L_8005FFA8;
                }
                if ((s32)r0 == (s32)0xc0f) goto L_8005EF68;
                if ((s32)r0 < (s32)0xc0f) {
                    if ((s32)r0 == (s32)0xc06) goto L_8005EF2C;
                    if ((s32)r0 < (s32)0xc06) {
                        if ((s32)r0 == (s32)0xc04) goto L_8005EF04;
                        if ((s32)r0 >= (s32)0xc04) goto L_8005EF18;
                        goto L_8005EEF0;
                    }
                    if ((s32)r0 == (s32)0xc0d) goto L_8005EF40;
                    if ((s32)r0 >= (s32)0xc0d) goto L_8005EF54;
                    return;
                }
                if ((s32)r0 == (s32)0xc13) goto L_8005EFB8;
                if ((s32)r0 < (s32)0xc13) {
                    if ((s32)r0 == (s32)0xc11) goto L_8005EF90;
                    if ((s32)r0 >= (s32)0xc11) goto L_8005EFA4;
                    goto L_8005EF7C;
                }
                if ((s32)r0 >= (s32)0xc15) goto L_8005EFE0;
                goto L_8005EFCC;
            }
            if ((s32)r0 == (s32)0xc25) goto L_8005FF68;
            if ((s32)r0 < (s32)0xc25) {
                if ((s32)r0 == (s32)0xc1e) goto L_8005F094;
                if ((s32)r0 < (s32)0xc1e) {
                    if ((s32)r0 == (s32)0xc1a) goto L_8005F044;
                    if ((s32)r0 < (s32)0xc1a) {
                        if ((s32)r0 == (s32)0xc18) goto L_8005F01C;
                        if ((s32)r0 >= (s32)0xc18) goto L_8005F030;
                        goto L_8005F008;
                    }
                    if ((s32)r0 == (s32)0xc1c) goto L_8005F06C;
                    if ((s32)r0 >= (s32)0xc1c) goto L_8005F080;
                    goto L_8005F058;
                }
                if ((s32)r0 == (s32)0xc22) goto L_8005F0F8;
                if ((s32)r0 < (s32)0xc22) {
                    if ((s32)r0 == (s32)0xc20) goto L_8005F0D0;
                    if ((s32)r0 >= (s32)0xc20) goto L_8005F0E4;
                    goto L_8005F0BC;
                }
                if ((s32)r0 >= (s32)0xc24) goto L_8005FF28;
                goto L_8005F10C;
            }
            if ((s32)r0 == (s32)0xc2c) goto L_8005F184;
            if ((s32)r0 < (s32)0xc2c) {
                if ((s32)r0 == (s32)0xc29) goto L_8005F148;
                if ((s32)r0 < (s32)0xc29) {
                    if ((s32)r0 == (s32)0xc27) goto L_8005F120;
                    if ((s32)r0 >= (s32)0xc27) goto L_8005F134;
                    goto L_8005FEE8;
                }
                if ((s32)r0 >= (s32)0xc2b) goto L_8005F170;
                goto L_8005F15C;
            }
            if ((s32)r0 == (s32)0xc35) goto L_8005F1C0;
            if ((s32)r0 < (s32)0xc35) {
                if ((s32)r0 == (s32)0xc33) goto L_8005F198;
                if ((s32)r0 >= (s32)0xc33) goto L_8005F1AC;
                return;
            }
            if ((s32)r0 == (s32)0xc37) goto L_8005F1E8;
            if ((s32)r0 >= (s32)0xc37) goto L_8005F1FC;
            goto L_8005F1D4;
        }
        if ((s32)r0 == (s32)0xc5c) goto L_8005F42C;
        if ((s32)r0 < (s32)0xc5c) {
            if ((s32)r0 == (s32)0xc48) goto L_8005F350;
            if ((s32)r0 < (s32)0xc48) {
                if ((s32)r0 == (s32)0xc41) goto L_8005F2B0;
                if ((s32)r0 < (s32)0xc41) {
                    if ((s32)r0 == (s32)0xc3d) goto L_8005F260;
                    if ((s32)r0 < (s32)0xc3d) {
                        if ((s32)r0 == (s32)0xc3b) goto L_8005F238;
                        if ((s32)r0 >= (s32)0xc3b) goto L_8005F24C;
                        goto L_8005F224;
                    }
                    if ((s32)r0 == (s32)0xc3f) goto L_8005F288;
                    if ((s32)r0 >= (s32)0xc3f) goto L_8005F29C;
                    goto L_8005F274;
                }
                if ((s32)r0 == (s32)0xc45) goto L_8005F314;
                if ((s32)r0 < (s32)0xc45) {
                    if ((s32)r0 == (s32)0xc43) goto L_8005F2D8;
                    if ((s32)r0 >= (s32)0xc43) goto L_8005F2EC;
                    goto L_8005F2C4;
                }
                if ((s32)r0 >= (s32)0xc47) goto L_8005F33C;
                goto L_8005F328;
            }
            if ((s32)r0 == (s32)0xc50) goto L_8005F3B4;
            if ((s32)r0 < (s32)0xc50) {
                if ((s32)r0 == (s32)0xc4c) goto L_8005FEF8;
                if ((s32)r0 < (s32)0xc4c) {
                    if ((s32)r0 == (s32)0xc4a) goto L_8005FF38;
                    if ((s32)r0 >= (s32)0xc4a) goto L_8005FF78;
                    goto L_8005F364;
                }
                if ((s32)r0 == (s32)0xc4e) goto L_8005F38C;
                if ((s32)r0 >= (s32)0xc4e) goto L_8005F3A0;
                goto L_8005F378;
            }
            if ((s32)r0 == (s32)0xc59) goto L_8005F3F0;
            if ((s32)r0 < (s32)0xc59) {
                if ((s32)r0 == (s32)0xc52) goto L_8005F3DC;
                if ((s32)r0 >= (s32)0xc52) return;
                goto L_8005F3C8;
            }
            if ((s32)r0 >= (s32)0xc5b) goto L_8005F418;
            goto L_8005F404;
        }
        if ((s32)r0 == (s32)0xc6b) goto L_8005F56C;
        if ((s32)r0 < (s32)0xc6b) {
            if ((s32)r0 == (s32)0xc64) goto L_8005F4CC;
            if ((s32)r0 < (s32)0xc64) {
                if ((s32)r0 == (s32)0xc60) goto L_8005F47C;
                if ((s32)r0 < (s32)0xc60) {
                    if ((s32)r0 == (s32)0xc5e) goto L_8005F454;
                    if ((s32)r0 >= (s32)0xc5e) goto L_8005F468;
                    goto L_8005F440;
                }
                if ((s32)r0 == (s32)0xc62) goto L_8005F4A4;
                if ((s32)r0 >= (s32)0xc62) goto L_8005F4B8;
                goto L_8005F490;
            }
            if ((s32)r0 == (s32)0xc68) goto L_8005F51C;
            if ((s32)r0 < (s32)0xc68) {
                if ((s32)r0 == (s32)0xc66) goto L_8005F4F4;
                if ((s32)r0 >= (s32)0xc66) goto L_8005F508;
                goto L_8005F4E0;
            }
            if ((s32)r0 >= (s32)0xc6a) goto L_8005F544;
            goto L_8005F530;
        }
        if ((s32)r0 == (s32)0xc73) goto L_8005F5D0;
        if ((s32)r0 < (s32)0xc73) {
            if ((s32)r0 == (s32)0xc6f) goto L_8005F5BC;
            if ((s32)r0 < (s32)0xc6f) {
                if ((s32)r0 == (s32)0xc6d) goto L_8005F594;
                if ((s32)r0 >= (s32)0xc6d) goto L_8005F5A8;
                goto L_8005F580;
            }
            if ((s32)r0 == (s32)0xc71) goto L_8005FF88;
            if ((s32)r0 >= (s32)0xc71) goto L_8005FF08;
            goto L_8005FF48;
        }
        if ((s32)r0 == (s32)0xc77) goto L_8005F620;
        if ((s32)r0 < (s32)0xc77) {
            if ((s32)r0 == (s32)0xc75) goto L_8005F5F8;
            if ((s32)r0 >= (s32)0xc75) goto L_8005F60C;
            goto L_8005F5E4;
        }
        if ((s32)r0 >= (s32)0xc79) return;
        goto L_8005F634;
    }
    if ((s32)r0 == (s32)0xdd2) goto L_8005FED8;
    if ((s32)r0 < (s32)0xdd2) {
        if ((s32)r0 == (s32)0xdb4) goto L_8005F300;
        if ((s32)r0 < (s32)0xdb4) {
            if ((s32)r0 == (s32)0xc8e) goto L_8005F774;
            if ((s32)r0 < (s32)0xc8e) {
                if ((s32)r0 == (s32)0xc87) goto L_8005F6E8;
                if ((s32)r0 < (s32)0xc87) {
                    if ((s32)r0 == (s32)0xc83) goto L_8005F698;
                    if ((s32)r0 < (s32)0xc83) {
                        if ((s32)r0 == (s32)0xc81) goto L_8005F670;
                        if ((s32)r0 >= (s32)0xc81) goto L_8005F684;
                        goto L_8005F65C;
                    }
                    if ((s32)r0 == (s32)0xc85) goto L_8005F6C0;
                    if ((s32)r0 >= (s32)0xc85) goto L_8005F6D4;
                    goto L_8005F6AC;
                }
                if ((s32)r0 == (s32)0xc8b) goto L_8005F738;
                if ((s32)r0 < (s32)0xc8b) {
                    if ((s32)r0 == (s32)0xc89) goto L_8005F710;
                    if ((s32)r0 >= (s32)0xc89) goto L_8005F724;
                    goto L_8005F6FC;
                }
                if ((s32)r0 >= (s32)0xc8d) goto L_8005F760;
                goto L_8005F74C;
            }
            if ((s32)r0 == (s32)0xc96) goto L_8005FF58;
            if ((s32)r0 < (s32)0xc96) {
                if ((s32)r0 == (s32)0xc92) goto L_8005F7D8;
                if ((s32)r0 < (s32)0xc92) {
                    if ((s32)r0 == (s32)0xc90) goto L_8005F79C;
                    if ((s32)r0 >= (s32)0xc90) goto L_8005F7C4;
                    goto L_8005F788;
                }
                if ((s32)r0 == (s32)0xc94) goto L_8005F800;
                if ((s32)r0 >= (s32)0xc94) goto L_8005F814;
                goto L_8005F7EC;
            }
            if ((s32)r0 == (s32)0xdaf) goto L_8005F558;
            if ((s32)r0 < (s32)0xdaf) {
                if ((s32)r0 == (s32)0xc98) goto L_8005FF18;
                if ((s32)r0 >= (s32)0xc98) return;
                goto L_8005FF98;
            }
            if ((s32)r0 >= (s32)0xdb3) goto L_8005F7B0;
            return;
        }
        if ((s32)r0 == (s32)0xdc3) goto L_8005F92C;
        if ((s32)r0 < (s32)0xdc3) {
            if ((s32)r0 == (s32)0xdbc) goto L_8005F8A0;
            if ((s32)r0 < (s32)0xdbc) {
                if ((s32)r0 == (s32)0xdb8) goto L_8005F850;
                if ((s32)r0 < (s32)0xdb8) {
                    if ((s32)r0 == (s32)0xdb6) goto L_8005F828;
                    if ((s32)r0 >= (s32)0xdb6) goto L_8005F83C;
                    goto L_8005F0A8;
                }
                if ((s32)r0 == (s32)0xdba) goto L_8005F878;
                if ((s32)r0 >= (s32)0xdba) goto L_8005F88C;
                goto L_8005F864;
            }
            if ((s32)r0 == (s32)0xdc0) goto L_8005F8F0;
            if ((s32)r0 < (s32)0xdc0) {
                if ((s32)r0 == (s32)0xdbe) goto L_8005F8C8;
                if ((s32)r0 >= (s32)0xdbe) goto L_8005F8DC;
                goto L_8005F8B4;
            }
            if ((s32)r0 >= (s32)0xdc2) goto L_8005F918;
            goto L_8005F904;
        }
        if ((s32)r0 == (s32)0xdcb) goto L_8005F9CC;
        if ((s32)r0 < (s32)0xdcb) {
            if ((s32)r0 == (s32)0xdc7) goto L_8005F97C;
            if ((s32)r0 < (s32)0xdc7) {
                if ((s32)r0 == (s32)0xdc5) goto L_8005F954;
                if ((s32)r0 >= (s32)0xdc5) goto L_8005F968;
                goto L_8005F940;
            }
            if ((s32)r0 == (s32)0xdc9) goto L_8005F9A4;
            if ((s32)r0 >= (s32)0xdc9) goto L_8005F9B8;
            goto L_8005F990;
        }
        if ((s32)r0 == (s32)0xdcf) goto L_8005FEA8;
        if ((s32)r0 < (s32)0xdcf) {
            if ((s32)r0 == (s32)0xdcd) goto L_8005F9F4;
            if ((s32)r0 >= (s32)0xdcd) goto L_8005FEB8;
            goto L_8005F9E0;
        }
        if ((s32)r0 >= (s32)0xdd1) goto L_8005FE98;
        goto L_8005FE88;
    }
    if ((s32)r0 == (s32)0xdf0) goto L_8005FCD8;
    if ((s32)r0 < (s32)0xdf0) {
        if ((s32)r0 == (s32)0xde1) goto L_8005FB70;
        if ((s32)r0 < (s32)0xde1) {
            if ((s32)r0 == (s32)0xdda) goto L_8005FA30;
            if ((s32)r0 < (s32)0xdda) {
                if ((s32)r0 == (s32)0xdd6) goto L_8005FDE8;
                if ((s32)r0 < (s32)0xdd6) {
                    if ((s32)r0 == (s32)0xdd4) goto L_8005FDC8;
                    if ((s32)r0 >= (s32)0xdd4) goto L_8005FDD8;
                    goto L_8005FEC8;
                }
                if ((s32)r0 == (s32)0xdd8) goto L_8005FA08;
                if ((s32)r0 >= (s32)0xdd8) goto L_8005FA1C;
                goto L_8005FDF8;
            }
            if ((s32)r0 == (s32)0xdde) goto L_8005FAF8;
            if ((s32)r0 < (s32)0xdde) {
                if ((s32)r0 == (s32)0xddc) goto L_8005FA94;
                if ((s32)r0 >= (s32)0xddc) goto L_8005FAA8;
                goto L_8005FA80;
            }
            if ((s32)r0 >= (s32)0xde0) goto L_8005FB20;
            goto L_8005FB0C;
        }
        if ((s32)r0 == (s32)0xde9) goto L_8005FAE4;
        if ((s32)r0 < (s32)0xde9) {
            if ((s32)r0 == (s32)0xde5) goto L_8005FA58;
            if ((s32)r0 < (s32)0xde5) {
                if ((s32)r0 == (s32)0xde3) goto L_8005FB98;
                if ((s32)r0 >= (s32)0xde3) goto L_8005FA44;
                goto L_8005FB84;
            }
            if ((s32)r0 == (s32)0xde7) goto L_8005FABC;
            if ((s32)r0 >= (s32)0xde7) goto L_8005FAD0;
            goto L_8005FA6C;
        }
        if ((s32)r0 == (s32)0xded) goto L_8005FBAC;
        if ((s32)r0 < (s32)0xded) {
            if ((s32)r0 == (s32)0xdeb) goto L_8005FB48;
            if ((s32)r0 >= (s32)0xdeb) goto L_8005FB5C;
            goto L_8005FB34;
        }
        if ((s32)r0 >= (s32)0xdef) goto L_8005FBD4;
        goto L_8005FBC0;
    }
    if ((s32)r0 == (s32)0xdff) goto L_8005FC24;
    if ((s32)r0 < (s32)0xdff) {
        if ((s32)r0 == (s32)0xdf8) goto L_8005FC88;
        if ((s32)r0 < (s32)0xdf8) {
            if ((s32)r0 == (s32)0xdf4) goto L_8005FBFC;
            if ((s32)r0 < (s32)0xdf4) {
                if ((s32)r0 == (s32)0xdf2) goto L_8005FD00;
                if ((s32)r0 >= (s32)0xdf2) goto L_8005FBE8;
                goto L_8005FCEC;
            }
            if ((s32)r0 == (s32)0xdf6) goto L_8005FC60;
            if ((s32)r0 >= (s32)0xdf6) goto L_8005FC74;
            goto L_8005FC10;
        }
        if ((s32)r0 == (s32)0xdfc) goto L_8005FD14;
        if ((s32)r0 < (s32)0xdfc) {
            if ((s32)r0 == (s32)0xdfa) goto L_8005FD64;
            if ((s32)r0 >= (s32)0xdfa) goto L_8005FD78;
            goto L_8005FD50;
        }
        if ((s32)r0 >= (s32)0xdfe) goto L_8005FD3C;
        goto L_8005FD28;
    }
    if ((s32)r0 == (s32)0xe07) goto L_8005FDB4;
    if ((s32)r0 < (s32)0xe07) {
        if ((s32)r0 == (s32)0xe03) goto L_8005FCB0;
        if ((s32)r0 < (s32)0xe03) {
            if ((s32)r0 == (s32)0xe01) goto L_8005FC4C;
            if ((s32)r0 >= (s32)0xe01) goto L_8005FC9C;
            goto L_8005FC38;
        }
        if ((s32)r0 == (s32)0xe05) goto L_8005FD8C;
        if ((s32)r0 >= (s32)0xe05) goto L_8005FDA0;
        goto L_8005FCC4;
    }
    if ((s32)r0 == (s32)0x102d) goto L_8005FE18;
    if ((s32)r0 < (s32)0x102d) {
        if ((s32)r0 >= (s32)0x102c) goto L_8005FE08;
        return;
    }
    if ((s32)r0 == (s32)0x1096) goto L_8005FFD0;
    return;
    L_8005EEC0: ;
    fn_800608C4();
    return;
    L_8005EEC8: ;
    r6 = *(u32*)((u8*)r5 + 0x0);
    r5 = 0x0;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005EEDC: ;
    r6 = *(u32*)((u8*)r5 + 0x4);
    r5 = 0x0;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005EEF0: ;
    r6 = *(u32*)((u8*)r5 + 0x8);
    r5 = 0x0;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005EF04: ;
    r6 = *(u32*)((u8*)r5 + 0xC);
    r5 = 0x0;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005EF18: ;
    r6 = *(u32*)((u8*)r5 + 0x10);
    r5 = 0x0;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005EF2C: ;
    r6 = *(u32*)((u8*)r5 + 0x14);
    r5 = 0x0;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005EF40: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005EF54: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005EF68: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005EF7C: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005EF90: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005EFA4: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005EFB8: ;
    r6 = *(u32*)((u8*)r5 + 0x0);
    r5 = 0x0;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005EFCC: ;
    r6 = *(u32*)((u8*)r5 + 0x4);
    r5 = 0x0;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005EFE0: ;
    r6 = *(u32*)((u8*)r5 + 0x8);
    r5 = 0x0;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005EFF4: ;
    r6 = *(u32*)((u8*)r5 + 0xC);
    r5 = 0x0;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005F008: ;
    r6 = *(u32*)((u8*)r5 + 0x10);
    r5 = 0x0;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005F01C: ;
    r6 = *(u32*)((u8*)r5 + 0x14);
    r5 = 0x0;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005F030: ;
    r6 = *(u32*)((u8*)r5 + 0x0);
    r5 = 0x0;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F044: ;
    r6 = *(u32*)((u8*)r5 + 0x4);
    r5 = 0x0;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F058: ;
    r6 = *(u32*)((u8*)r5 + 0x8);
    r5 = 0x0;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F06C: ;
    r6 = *(u32*)((u8*)r5 + 0xC);
    r5 = 0x0;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F080: ;
    r6 = *(u32*)((u8*)r5 + 0x10);
    r5 = 0x0;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F094: ;
    r6 = *(u32*)((u8*)r5 + 0x14);
    r5 = 0x0;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F0A8: ;
    r6 = *(u32*)((u8*)r5 + 0x0);
    r5 = 0x0;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F0BC: ;
    r6 = *(u32*)((u8*)r5 + 0x4);
    r5 = 0x0;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F0D0: ;
    r6 = *(u32*)((u8*)r5 + 0x8);
    r5 = 0x0;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F0E4: ;
    r6 = *(u32*)((u8*)r5 + 0xC);
    r5 = 0x0;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F0F8: ;
    r6 = *(u32*)((u8*)r5 + 0x10);
    r5 = 0x0;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F10C: ;
    r6 = *(u32*)((u8*)r5 + 0x14);
    r5 = 0x0;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F120: ;
    r6 = *(u32*)((u8*)r5 + 0x18);
    r5 = 0x1;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005F134: ;
    r6 = *(u32*)((u8*)r5 + 0x1C);
    r5 = 0x1;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005F148: ;
    r6 = *(u32*)((u8*)r5 + 0x20);
    r5 = 0x1;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005F15C: ;
    r6 = *(u32*)((u8*)r5 + 0x24);
    r5 = 0x1;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005F170: ;
    r6 = *(u32*)((u8*)r5 + 0x28);
    r5 = 0x1;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005F184: ;
    r6 = *(u32*)((u8*)r5 + 0x2C);
    r5 = 0x1;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005F198: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F1AC: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F1C0: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F1D4: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F1E8: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F1FC: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F210: ;
    r6 = *(u32*)((u8*)r5 + 0x18);
    r5 = 0x1;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005F224: ;
    r6 = *(u32*)((u8*)r5 + 0x1C);
    r5 = 0x1;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005F238: ;
    r6 = *(u32*)((u8*)r5 + 0x20);
    r5 = 0x1;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005F24C: ;
    r6 = *(u32*)((u8*)r5 + 0x24);
    r5 = 0x1;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005F260: ;
    r6 = *(u32*)((u8*)r5 + 0x28);
    r5 = 0x1;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005F274: ;
    r6 = *(u32*)((u8*)r5 + 0x2C);
    r5 = 0x1;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005F288: ;
    r6 = *(u32*)((u8*)r5 + 0x18);
    r5 = 0x1;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F29C: ;
    r6 = *(u32*)((u8*)r5 + 0x1C);
    r5 = 0x1;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F2B0: ;
    r6 = *(u32*)((u8*)r5 + 0x20);
    r5 = 0x1;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F2C4: ;
    r6 = *(u32*)((u8*)r5 + 0x24);
    r5 = 0x1;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F2D8: ;
    r6 = *(u32*)((u8*)r5 + 0x28);
    r5 = 0x1;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F2EC: ;
    r6 = *(u32*)((u8*)r5 + 0x2C);
    r5 = 0x1;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F300: ;
    r6 = *(u32*)((u8*)r5 + 0x18);
    r5 = 0x1;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F314: ;
    r6 = *(u32*)((u8*)r5 + 0x1C);
    r5 = 0x1;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F328: ;
    r6 = *(u32*)((u8*)r5 + 0x20);
    r5 = 0x1;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F33C: ;
    r6 = *(u32*)((u8*)r5 + 0x24);
    r5 = 0x1;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F350: ;
    r6 = *(u32*)((u8*)r5 + 0x28);
    r5 = 0x1;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F364: ;
    r6 = *(u32*)((u8*)r5 + 0x2C);
    r5 = 0x1;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F378: ;
    r6 = *(u32*)((u8*)r5 + 0x30);
    r5 = 0x2;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005F38C: ;
    r6 = *(u32*)((u8*)r5 + 0x34);
    r5 = 0x2;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005F3A0: ;
    r6 = *(u32*)((u8*)r5 + 0x38);
    r5 = 0x2;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005F3B4: ;
    r6 = *(u32*)((u8*)r5 + 0x3C);
    r5 = 0x2;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005F3C8: ;
    r6 = *(u32*)((u8*)r5 + 0x40);
    r5 = 0x2;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005F3DC: ;
    r6 = *(u32*)((u8*)r5 + 0x44);
    r5 = 0x2;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005F3F0: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F404: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F418: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F42C: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F440: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F454: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F468: ;
    r6 = *(u32*)((u8*)r5 + 0x30);
    r5 = 0x2;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005F47C: ;
    r6 = *(u32*)((u8*)r5 + 0x34);
    r5 = 0x2;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005F490: ;
    r6 = *(u32*)((u8*)r5 + 0x38);
    r5 = 0x2;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005F4A4: ;
    r6 = *(u32*)((u8*)r5 + 0x3C);
    r5 = 0x2;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005F4B8: ;
    r6 = *(u32*)((u8*)r5 + 0x40);
    r5 = 0x2;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005F4CC: ;
    r6 = *(u32*)((u8*)r5 + 0x44);
    r5 = 0x2;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005F4E0: ;
    r6 = *(u32*)((u8*)r5 + 0x30);
    r5 = 0x2;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F4F4: ;
    r6 = *(u32*)((u8*)r5 + 0x34);
    r5 = 0x2;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F508: ;
    r6 = *(u32*)((u8*)r5 + 0x38);
    r5 = 0x2;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F51C: ;
    r6 = *(u32*)((u8*)r5 + 0x3C);
    r5 = 0x2;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F530: ;
    r6 = *(u32*)((u8*)r5 + 0x40);
    r5 = 0x2;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F544: ;
    r6 = *(u32*)((u8*)r5 + 0x44);
    r5 = 0x2;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F558: ;
    r6 = *(u32*)((u8*)r5 + 0x30);
    r5 = 0x2;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F56C: ;
    r6 = *(u32*)((u8*)r5 + 0x34);
    r5 = 0x2;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F580: ;
    r6 = *(u32*)((u8*)r5 + 0x38);
    r5 = 0x2;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F594: ;
    r6 = *(u32*)((u8*)r5 + 0x3C);
    r5 = 0x2;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F5A8: ;
    r6 = *(u32*)((u8*)r5 + 0x40);
    r5 = 0x2;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F5BC: ;
    r6 = *(u32*)((u8*)r5 + 0x44);
    r5 = 0x2;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F5D0: ;
    r6 = *(u32*)((u8*)r5 + 0x48);
    r5 = 0x3;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005F5E4: ;
    r6 = *(u32*)((u8*)r5 + 0x4C);
    r5 = 0x3;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005F5F8: ;
    r6 = *(u32*)((u8*)r5 + 0x50);
    r5 = 0x3;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005F60C: ;
    r6 = *(u32*)((u8*)r5 + 0x54);
    r5 = 0x3;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005F620: ;
    r6 = *(u32*)((u8*)r5 + 0x58);
    r5 = 0x3;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005F634: ;
    r6 = *(u32*)((u8*)r5 + 0x5C);
    r5 = 0x3;
    r7 = 0x2;
    fn_8006106C();
    return;
    L_8005F648: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F65C: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F670: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F684: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F698: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F6AC: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F6C0: ;
    r6 = *(u32*)((u8*)r5 + 0x48);
    r5 = 0x3;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005F6D4: ;
    r6 = *(u32*)((u8*)r5 + 0x4C);
    r5 = 0x3;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005F6E8: ;
    r6 = *(u32*)((u8*)r5 + 0x50);
    r5 = 0x3;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005F6FC: ;
    r6 = *(u32*)((u8*)r5 + 0x54);
    r5 = 0x3;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005F710: ;
    r6 = *(u32*)((u8*)r5 + 0x58);
    r5 = 0x3;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005F724: ;
    r6 = *(u32*)((u8*)r5 + 0x5C);
    r5 = 0x3;
    r7 = 0x2;
    fn_80061A2C();
    return;
    L_8005F738: ;
    r6 = *(u32*)((u8*)r5 + 0x48);
    r5 = 0x3;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F74C: ;
    r6 = *(u32*)((u8*)r5 + 0x4C);
    r5 = 0x3;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F760: ;
    r6 = *(u32*)((u8*)r5 + 0x50);
    r5 = 0x3;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F774: ;
    r6 = *(u32*)((u8*)r5 + 0x54);
    r5 = 0x3;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F788: ;
    r6 = *(u32*)((u8*)r5 + 0x58);
    r5 = 0x3;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F79C: ;
    r6 = *(u32*)((u8*)r5 + 0x5C);
    r5 = 0x3;
    r7 = 0x2;
    fn_80061BBC();
    return;
    L_8005F7B0: ;
    r6 = *(u32*)((u8*)r5 + 0x48);
    r5 = 0x3;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F7C4: ;
    r6 = *(u32*)((u8*)r5 + 0x4C);
    r5 = 0x3;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F7D8: ;
    r6 = *(u32*)((u8*)r5 + 0x50);
    r5 = 0x3;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F7EC: ;
    r6 = *(u32*)((u8*)r5 + 0x54);
    r5 = 0x3;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F800: ;
    r6 = *(u32*)((u8*)r5 + 0x58);
    r5 = 0x3;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F814: ;
    r6 = *(u32*)((u8*)r5 + 0x5C);
    r5 = 0x3;
    r7 = 0x2;
    fn_80061B74();
    return;
    L_8005F828: ;
    r6 = *(u32*)((u8*)r5 + 0x0);
    r5 = 0x0;
    r7 = 0x0;
    fn_8006106C();
    return;
    L_8005F83C: ;
    r6 = *(u32*)((u8*)r5 + 0x4);
    r5 = 0x0;
    r7 = 0x0;
    fn_8006106C();
    return;
    L_8005F850: ;
    r6 = *(u32*)((u8*)r5 + 0x8);
    r5 = 0x0;
    r7 = 0x0;
    fn_8006106C();
    return;
    L_8005F864: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F878: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F88C: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F8A0: ;
    r6 = *(u32*)((u8*)r5 + 0x0);
    r5 = 0x0;
    r7 = 0x0;
    fn_80061A2C();
    return;
    L_8005F8B4: ;
    r6 = *(u32*)((u8*)r5 + 0x4);
    r5 = 0x0;
    r7 = 0x0;
    fn_80061A2C();
    return;
    L_8005F8C8: ;
    r6 = *(u32*)((u8*)r5 + 0x8);
    r5 = 0x0;
    r7 = 0x0;
    fn_80061A2C();
    return;
    L_8005F8DC: ;
    r6 = *(u32*)((u8*)r5 + 0x0);
    r5 = 0x0;
    r7 = 0x0;
    fn_80061BBC();
    return;
    L_8005F8F0: ;
    r6 = *(u32*)((u8*)r5 + 0x4);
    r5 = 0x0;
    r7 = 0x0;
    fn_80061BBC();
    return;
    L_8005F904: ;
    r6 = *(u32*)((u8*)r5 + 0x8);
    r5 = 0x0;
    r7 = 0x0;
    fn_80061BBC();
    return;
    L_8005F918: ;
    r6 = *(u32*)((u8*)r5 + 0x18);
    r5 = 0x1;
    r7 = 0x0;
    fn_8006106C();
    return;
    L_8005F92C: ;
    r6 = *(u32*)((u8*)r5 + 0x1C);
    r5 = 0x1;
    r7 = 0x0;
    fn_8006106C();
    return;
    L_8005F940: ;
    r6 = *(u32*)((u8*)r5 + 0x20);
    r5 = 0x1;
    r7 = 0x0;
    fn_8006106C();
    return;
    L_8005F954: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F968: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F97C: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005F990: ;
    r6 = *(u32*)((u8*)r5 + 0x18);
    r5 = 0x1;
    r7 = 0x0;
    fn_80061A2C();
    return;
    L_8005F9A4: ;
    r6 = *(u32*)((u8*)r5 + 0x1C);
    r5 = 0x1;
    r7 = 0x0;
    fn_80061A2C();
    return;
    L_8005F9B8: ;
    r6 = *(u32*)((u8*)r5 + 0x20);
    r5 = 0x1;
    r7 = 0x0;
    fn_80061A2C();
    return;
    L_8005F9CC: ;
    r6 = *(u32*)((u8*)r5 + 0x18);
    r5 = 0x1;
    r7 = 0x0;
    fn_80061BBC();
    return;
    L_8005F9E0: ;
    r6 = *(u32*)((u8*)r5 + 0x1C);
    r5 = 0x1;
    r7 = 0x0;
    fn_80061BBC();
    return;
    L_8005F9F4: ;
    r6 = *(u32*)((u8*)r5 + 0x20);
    r5 = 0x1;
    r7 = 0x0;
    fn_80061BBC();
    return;
    L_8005FA08: ;
    r6 = *(u32*)((u8*)r5 + 0x0);
    r5 = 0x0;
    r7 = 0x1;
    fn_8006106C();
    return;
    L_8005FA1C: ;
    r6 = *(u32*)((u8*)r5 + 0x4);
    r5 = 0x0;
    r7 = 0x1;
    fn_8006106C();
    return;
    L_8005FA30: ;
    r6 = *(u32*)((u8*)r5 + 0x8);
    r5 = 0x0;
    r7 = 0x1;
    fn_8006106C();
    return;
    L_8005FA44: ;
    r6 = *(u32*)((u8*)r5 + 0xC);
    r5 = 0x0;
    r7 = 0x1;
    fn_8006106C();
    return;
    L_8005FA58: ;
    r6 = *(u32*)((u8*)r5 + 0x10);
    r5 = 0x0;
    r7 = 0x1;
    fn_8006106C();
    return;
    L_8005FA6C: ;
    r6 = *(u32*)((u8*)r5 + 0x14);
    r5 = 0x0;
    r7 = 0x1;
    fn_8006106C();
    return;
    L_8005FA80: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005FA94: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005FAA8: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005FABC: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005FAD0: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005FAE4: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005FAF8: ;
    r6 = *(u32*)((u8*)r5 + 0x0);
    r5 = 0x0;
    r7 = 0x1;
    fn_80061A2C();
    return;
    L_8005FB0C: ;
    r6 = *(u32*)((u8*)r5 + 0x4);
    r5 = 0x0;
    r7 = 0x1;
    fn_80061A2C();
    return;
    L_8005FB20: ;
    r6 = *(u32*)((u8*)r5 + 0x8);
    r5 = 0x0;
    r7 = 0x1;
    fn_80061A2C();
    return;
    L_8005FB34: ;
    r6 = *(u32*)((u8*)r5 + 0xC);
    r5 = 0x0;
    r7 = 0x1;
    fn_80061A2C();
    return;
    L_8005FB48: ;
    r6 = *(u32*)((u8*)r5 + 0x10);
    r5 = 0x0;
    r7 = 0x1;
    fn_80061A2C();
    return;
    L_8005FB5C: ;
    r6 = *(u32*)((u8*)r5 + 0x14);
    r5 = 0x0;
    r7 = 0x1;
    fn_80061A2C();
    return;
    L_8005FB70: ;
    r6 = *(u32*)((u8*)r5 + 0x0);
    r5 = 0x0;
    r7 = 0x1;
    fn_80061BBC();
    return;
    L_8005FB84: ;
    r6 = *(u32*)((u8*)r5 + 0x4);
    r5 = 0x0;
    r7 = 0x1;
    fn_80061BBC();
    return;
    L_8005FB98: ;
    r6 = *(u32*)((u8*)r5 + 0x8);
    r5 = 0x0;
    r7 = 0x1;
    fn_80061BBC();
    return;
    L_8005FBAC: ;
    r6 = *(u32*)((u8*)r5 + 0xC);
    r5 = 0x0;
    r7 = 0x1;
    fn_80061BBC();
    return;
    L_8005FBC0: ;
    r6 = *(u32*)((u8*)r5 + 0x10);
    r5 = 0x0;
    r7 = 0x1;
    fn_80061BBC();
    return;
    L_8005FBD4: ;
    r6 = *(u32*)((u8*)r5 + 0x14);
    r5 = 0x0;
    r7 = 0x1;
    fn_80061BBC();
    return;
    L_8005FBE8: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005FBFC: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005FC10: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005FC24: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005FC38: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005FC4C: ;
    r0 = *(u8*)((u8*)r4 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    return;
    L_8005FC60: ;
    r6 = *(u32*)((u8*)r5 + 0x18);
    r5 = 0x1;
    r7 = 0x1;
    fn_80061A2C();
    return;
    L_8005FC74: ;
    r6 = *(u32*)((u8*)r5 + 0x1C);
    r5 = 0x1;
    r7 = 0x1;
    fn_80061A2C();
    return;
    L_8005FC88: ;
    r6 = *(u32*)((u8*)r5 + 0x20);
    r5 = 0x1;
    r7 = 0x1;
    fn_80061A2C();
    return;
    L_8005FC9C: ;
    r6 = *(u32*)((u8*)r5 + 0x24);
    r5 = 0x1;
    r7 = 0x1;
    fn_80061A2C();
    return;
    L_8005FCB0: ;
    r6 = *(u32*)((u8*)r5 + 0x28);
    r5 = 0x1;
    r7 = 0x1;
    fn_80061A2C();
    return;
    L_8005FCC4: ;
    r6 = *(u32*)((u8*)r5 + 0x2C);
    r5 = 0x1;
    r7 = 0x1;
    fn_80061A2C();
    return;
    L_8005FCD8: ;
    r6 = *(u32*)((u8*)r5 + 0x18);
    r5 = 0x1;
    r7 = 0x1;
    fn_8006106C();
    return;
    L_8005FCEC: ;
    r6 = *(u32*)((u8*)r5 + 0x1C);
    r5 = 0x1;
    r7 = 0x1;
    fn_8006106C();
    return;
    L_8005FD00: ;
    r6 = *(u32*)((u8*)r5 + 0x20);
    r5 = 0x1;
    r7 = 0x1;
    fn_8006106C();
    return;
    L_8005FD14: ;
    r6 = *(u32*)((u8*)r5 + 0x24);
    r5 = 0x1;
    r7 = 0x1;
    fn_8006106C();
    return;
    L_8005FD28: ;
    r6 = *(u32*)((u8*)r5 + 0x28);
    r5 = 0x1;
    r7 = 0x1;
    fn_8006106C();
    return;
    L_8005FD3C: ;
    r6 = *(u32*)((u8*)r5 + 0x2C);
    r5 = 0x1;
    r7 = 0x1;
    fn_8006106C();
    return;
    L_8005FD50: ;
    r6 = *(u32*)((u8*)r5 + 0x18);
    r5 = 0x1;
    r7 = 0x1;
    fn_80061BBC();
    return;
    L_8005FD64: ;
    r6 = *(u32*)((u8*)r5 + 0x1C);
    r5 = 0x1;
    r7 = 0x1;
    fn_80061BBC();
    return;
    L_8005FD78: ;
    r6 = *(u32*)((u8*)r5 + 0x20);
    r5 = 0x1;
    r7 = 0x1;
    fn_80061BBC();
    return;
    L_8005FD8C: ;
    r6 = *(u32*)((u8*)r5 + 0x24);
    r5 = 0x1;
    r7 = 0x1;
    fn_80061BBC();
    return;
    L_8005FDA0: ;
    r6 = *(u32*)((u8*)r5 + 0x28);
    r5 = 0x1;
    r7 = 0x1;
    fn_80061BBC();
    return;
    L_8005FDB4: ;
    r6 = *(u32*)((u8*)r5 + 0x2C);
    r5 = 0x1;
    r7 = 0x1;
    fn_80061BBC();
    return;
    L_8005FDC8: ;
    r5 = 0x0;
    r6 = 0x0;
    fn_80060D70();
    return;
    L_8005FDD8: ;
    r5 = 0x0;
    r6 = 0x1;
    fn_80060D70();
    return;
    L_8005FDE8: ;
    r5 = 0x1;
    r6 = 0x0;
    fn_80060D70();
    return;
    L_8005FDF8: ;
    r5 = 0x1;
    r6 = 0x1;
    fn_80060D70();
    return;
    L_8005FE08: ;
    r5 = 0x1;
    r6 = 0x2;
    fn_80060D70();
    return;
    L_8005FE18: ;
    r5 = 0x1;
    r6 = 0x2;
    fn_80060D70();
    return;
    L_8005FE28: ;
    r5 = 0x6;
    fn_80060EF4();
    return;
    L_8005FE34: ;
    r5 = 0x6;
    fn_80060EF4();
    return;
    L_8005FE40: ;
    r5 = -0x1;
    fn_80060EF4();
    return;
    L_8005FE4C: ;
    r5 = 0x3;
    fn_80060EF4();
    return;
    L_8005FE58: ;
    r5 = 0x4;
    fn_80060EF4();
    return;
    L_8005FE64: ;
    r5 = 0x2;
    fn_80060EF4();
    return;
    L_8005FE70: ;
    r5 = 0x1;
    fn_80060EF4();
    return;
    L_8005FE7C: ;
    r5 = 0x0;
    fn_80060EF4();
    return;
    L_8005FE88: ;
    r5 = 0x0;
    r6 = 0x0;
    fn_800617E0();
    return;
    L_8005FE98: ;
    r5 = 0x1;
    r6 = 0x0;
    fn_800617E0();
    return;
    L_8005FEA8: ;
    r5 = 0x0;
    r6 = 0x0;
    fn_800615F4();
    return;
    L_8005FEB8: ;
    r5 = 0x1;
    r6 = 0x0;
    fn_800615F4();
    return;
    L_8005FEC8: ;
    r5 = 0x0;
    r6 = 0x0;
    fn_80061454();
    return;
    L_8005FED8: ;
    r5 = 0x1;
    r6 = 0x0;
    fn_80061454();
    return;
    L_8005FEE8: ;
    r5 = 0x0;
    r6 = 0x2;
    fn_800617E0();
    return;
    L_8005FEF8: ;
    r5 = 0x1;
    r6 = 0x2;
    fn_800617E0();
    return;
    L_8005FF08: ;
    r5 = 0x2;
    r6 = 0x2;
    fn_800617E0();
    return;
    L_8005FF18: ;
    r5 = 0x3;
    r6 = 0x2;
    fn_800617E0();
    return;
    L_8005FF28: ;
    r5 = 0x0;
    r6 = 0x2;
    fn_80061454();
    return;
    L_8005FF38: ;
    r5 = 0x1;
    r6 = 0x2;
    fn_80061454();
    return;
    L_8005FF48: ;
    r5 = 0x2;
    r6 = 0x2;
    fn_80061454();
    return;
    L_8005FF58: ;
    r5 = 0x3;
    r6 = 0x2;
    fn_80061454();
    return;
    L_8005FF68: ;
    r5 = 0x0;
    r6 = 0x2;
    fn_800615F4();
    return;
    L_8005FF78: ;
    r5 = 0x1;
    r6 = 0x2;
    fn_800615F4();
    return;
    L_8005FF88: ;
    r5 = 0x2;
    r6 = 0x2;
    fn_800615F4();
    return;
    L_8005FF98: ;
    r5 = 0x3;
    r6 = 0x2;
    fn_800615F4();
    return;
    L_8005FFA8: ;
    r5 = (u32)&lbl_803A9A60;
    r5 = (u32)&lbl_803A9A60;
    f1 = *(f32*)((u8*)r5 + 0x48);
    fn_800609B4();
    return;
    L_8005FFBC: ;
    r5 = (u32)&lbl_803A9A60;
    r5 = (u32)&lbl_803A9A60;
    f1 = *(f32*)((u8*)r5 + 0x4C);
    fn_800609B4();
    return;
    L_8005FFD0: ;
    fn_80060434();

    return;
}


/* 0x8005FFE4 | size: 0x450 */
s32 fn_8005FFE4(void) {
    extern void fn_8025D808();
    extern void fn_8025D89C();
    extern void fn_8025DA88();
    u8 sp[0x140];
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

    
    fn_8025DA88();
    r5 = *(u32*)&lbl_8047BF58;
    r4 = (u32)&lbl_803A9E40;
    r0 = *(u32*)&lbl_8047BF5C;
    r31 = r3;
    r7 = (u32)&lbl_803A9E40;
    *(u32*)(sp + 0xC) = r0;
    r29 = 0x0;
    r6 = 0x1;
    r5 = 0x2;
    r4 = 0x3;
    r3 = 0x4;
    r0 = 0x5;
    *(u32*)((u8*)r7 + 0x0) = r29;
    r8 = r7 + 0x18;
    *(u32*)((u8*)r7 + 0x4) = r6;
    *(u32*)((u8*)r7 + 0x8) = r5;
    *(u32*)((u8*)r7 + 0xC) = r4;
    *(u32*)((u8*)r7 + 0x10) = r3;
    *(u32*)((u8*)r7 + 0x14) = r0;
    *(u32*)((u8*)r8 + 0x0) = r29;
    *(u32*)((u8*)r8 + 0x4) = r6;
    *(u32*)((u8*)r8 + 0x8) = r5;
    *(u32*)((u8*)r8 + 0xC) = r4;
    *(u32*)((u8*)r8 + 0x10) = r3;
    *(u32*)((u8*)r8 + 0x14) = r0;
    r8 = r8 + 0x18;
    *(u32*)((u8*)r8 + 0x0) = r29;
    *(u32*)((u8*)r8 + 0x4) = r6;
    *(u32*)((u8*)r8 + 0x8) = r5;
    *(u32*)((u8*)r8 + 0xC) = r4;
    *(u32*)((u8*)r8 + 0x10) = r3;
    *(u32*)((u8*)r8 + 0x14) = r0;
    r8 = r8 + 0x18;
    *(u32*)((u8*)r8 + 0x0) = r29;
    *(u32*)((u8*)r8 + 0x4) = r6;
    *(u32*)((u8*)r8 + 0x8) = r5;
    *(u32*)((u8*)r8 + 0xC) = r4;
    *(u32*)((u8*)r8 + 0x10) = r3;
    *(u32*)((u8*)r8 + 0x14) = r0;
    r3 = (u32)&lbl_803A9A60;
    r3 = (u32)&lbl_803A9A60;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((s32)r0 != (s32)0x1) {
        if ((s32)r0 >= (s32)0x1) goto L_8006010C;
        if ((s32)r0 < (s32)0x0) {
            goto L_8006010C;
        }
        r30 = (u32)sp + 0x8;
        r29 = 0x0;
        do {
            r3 = r29;
            fn_8025D89C();
            *(u16*)((u8*)r30 + 0x0) = r3;
            r30 = r30 + 0x2;
            r29 = r29 + 0x1;
        } while ((s32)r29 < (s32)0x4);
        goto L_8006010C;
    }
    r30 = (u32)sp + 0x8;
    do {
        r3 = r29;
        fn_8025D808();
        *(u16*)((u8*)r30 + 0x0) = r3;
        r30 = r30 + 0x2;
        r29 = r29 + 0x1;
    } while ((s32)r29 < (s32)0x4);
    L_8006010C: ;
    if ((s32)r31 != (s32)0x1) {
        if ((s32)r31 < (s32)0x1) {
            if ((s32)r31 < (s32)0x0) {
                return;
            }
            if ((s32)r31 >= (s32)0x3) return;
            goto L_80060260;
            }
        r3 = (u32)&lbl_803A9E40;
        r8 = (u32)sp + 0x8;
        r7 = (u32)&lbl_803A9E40;
        r9 = 0x0;
        r3 = (u32)&lbl_80267AF8;
        r4 = (u32)&lbl_80267AF8;
        do {
            r0 = 0x12;
            r6 = (u32)sp + 0x9c;
            /* subi r5, r4, 0x4 */;
            ctr_fn = (void(*)(void))r0;
            do {
                r3 = *(u32*)((u8*)r5 + 0x4);
                r0 = *(u32*)((u8*)r5 + 0x8);
                *(u32*)((u8*)r6 + 0x4) = r3;
                r6 += 8; *(u32*)r6 = r0;
            } while (--ctr != 0);
            r3 = *(u16*)((u8*)r8 + 0x0);
            r5 = (u32)sp + 0xa0;
            r8 = r8 + 0x2;
            r9 = r9 + 0x1;
            /* subi r0, r3, 0x1 */;
            r0 = r0 * 0x18;
            r5 = r5 + r0;
            r3 = *(u32*)((u8*)r5 + 0x0);
            r0 = *(u32*)((u8*)r5 + 0x4);
            *(u32*)((u8*)r7 + 0x0) = r3;
            r3 = *(u32*)((u8*)r5 + 0x8);
            *(u32*)((u8*)r7 + 0x4) = r0;
            r0 = *(u32*)((u8*)r5 + 0xC);
            *(u32*)((u8*)r7 + 0x8) = r3;
            r3 = *(u32*)((u8*)r5 + 0x10);
            *(u32*)((u8*)r7 + 0xC) = r0;
            r0 = *(u32*)((u8*)r5 + 0x14);
            *(u32*)((u8*)r7 + 0x10) = r3;
            *(u32*)((u8*)r7 + 0x14) = r0;
            r7 = r7 + 0x18;
        } while ((s32)r9 < (s32)0x2);
        return;
    }
    r3 = (u32)&lbl_803A9E40;
    r8 = (u32)sp + 0x8;
    r7 = (u32)&lbl_803A9E40;
    r9 = 0x0;
    r3 = (u32)&lbl_80267B88;
    r4 = (u32)&lbl_80267B88;
    do {
        r0 = 0x12;
        r6 = (u32)sp + 0xc;
        /* subi r5, r4, 0x4 */;
        ctr_fn = (void(*)(void))r0;
        do {
            r3 = *(u32*)((u8*)r5 + 0x4);
            r0 = *(u32*)((u8*)r5 + 0x8);
            *(u32*)((u8*)r6 + 0x4) = r3;
            r6 += 8; *(u32*)r6 = r0;
        } while (--ctr != 0);
        r3 = *(u16*)((u8*)r8 + 0x0);
        r5 = (u32)sp + 0x10;
        r8 = r8 + 0x2;
        r9 = r9 + 0x1;
        /* subi r0, r3, 0x1 */;
        r0 = r0 * 0x18;
        r5 = r5 + r0;
        r3 = *(u32*)((u8*)r5 + 0x0);
        r0 = *(u32*)((u8*)r5 + 0x4);
        *(u32*)((u8*)r7 + 0x0) = r3;
        r3 = *(u32*)((u8*)r5 + 0x8);
        *(u32*)((u8*)r7 + 0x4) = r0;
        r0 = *(u32*)((u8*)r5 + 0xC);
        *(u32*)((u8*)r7 + 0x8) = r3;
        r3 = *(u32*)((u8*)r5 + 0x10);
        *(u32*)((u8*)r7 + 0xC) = r0;
        r0 = *(u32*)((u8*)r5 + 0x14);
        *(u32*)((u8*)r7 + 0x10) = r3;
        *(u32*)((u8*)r7 + 0x14) = r0;
        r7 = r7 + 0x18;
    } while ((s32)r9 < (s32)0x2);
    return;
    L_80060260: ;
    r3 = (u32)&lbl_803A9E40;
    r4 = (u32)sp + 0x8;
    r5 = (u32)&lbl_803A9E40;
    r0 = 0x4;
    ctr_fn = (void(*)(void))r0;
    do {
        r3 = *(u16*)((u8*)r4 + 0x0);
        r12 = 0x0;
        if ((u32)r3 > (u32)0x0) {
            /* subi r0, r3, 0x8 */;
            if ((u32)r3 <= (u32)0x8) goto L_80060320;
            r0 = r0 & 0xFFFF;
            while (1) {
                r6 = r12 & 0xFFFF;
                if ((u32)r6 >= (u32)r0) break;
                r9 = r12 & 0xFFFF;
                r7 = r12 + 0x1;
                r8 = r9 << 2;
                r6 = r12 + 0x2;
                r30 = r5 + r8;
                r10 = r12 + 0x3;
                *(u32*)((u8*)r30 + 0x0) = r9;
                r7 = r7 & 0xFFFF;
                r9 = r12 + 0x4;
                r8 = r12 + 0x5;
                *(u32*)((u8*)r30 + 0x4) = r7;
                r11 = r6 & 0xFFFF;
                r7 = r12 + 0x6;
                r6 = r12 + 0x7;
                *(u32*)((u8*)r30 + 0x8) = r11;
                r10 = r10 & 0xFFFF;
                r9 = r9 & 0xFFFF;
                r8 = r8 & 0xFFFF;
                *(u32*)((u8*)r30 + 0xC) = r10;
                r7 = r7 & 0xFFFF;
                r6 = r6 & 0xFFFF;
                r12 = r12 + 0x8;
                *(u32*)((u8*)r30 + 0x10) = r9;
                *(u32*)((u8*)r30 + 0x14) = r8;
                *(u32*)((u8*)r30 + 0x18) = r7;
                *(u32*)((u8*)r30 + 0x1C) = r6;


            }
            goto L_80060320;
            L_80060310: ;
            r6 = r12 & 0xFFFF;
            r12 = r12 + 0x1;
            r0 = r6 << 2;
            *(u32*)(r5 + r0) = r6;
            L_80060320: ;
            r0 = r12 & 0xFFFF;
            if ((u32)r0 < (u32)r3) goto L_80060310;
        }
        r6 = r3;
        if ((u32)r3 < (u32)0x6) {
            r0 = 0x6 - r3;
            r0 = r0 & 0xFFFF;
            if ((u32)r0 <= (u32)0x8) goto L_80060400;
            while (1) {
                r0 = r6 & 0xFFFF;
                if ((u32)r0 >= (u32)0xfffe) break;
                r0 = r6 & 0xFFFF;
                r7 = r3;
                r0 = r0 << 2;
                r3 = r3 + 0x1;
                r8 = r7 & 0xFFFF;
                r6 = r6 + 0x8;
                r7 = r3;
                r9 = r5 + r0;
                r3 = r3 + 0x1;
                *(u32*)((u8*)r9 + 0x0) = r8;
                r0 = r3;
                r7 = r7 & 0xFFFF;
                *(u32*)((u8*)r9 + 0x4) = r7;
                r3 = r3 + 0x1;
                r7 = r3;
                r0 = r0 & 0xFFFF;
                *(u32*)((u8*)r9 + 0x8) = r0;
                r3 = r3 + 0x1;
                r0 = r3;
                r7 = r7 & 0xFFFF;
                *(u32*)((u8*)r9 + 0xC) = r7;
                r3 = r3 + 0x1;
                r7 = r3;
                r0 = r0 & 0xFFFF;
                *(u32*)((u8*)r9 + 0x10) = r0;
                r3 = r3 + 0x1;
                r0 = r3;
                r7 = r7 & 0xFFFF;
                *(u32*)((u8*)r9 + 0x14) = r7;
                r0 = r0 & 0xFFFF;
                r3 = r3 + 0x1;
                *(u32*)((u8*)r9 + 0x18) = r0;
                r0 = r3 & 0xFFFF;
                r3 = r3 + 0x1;
                *(u32*)((u8*)r9 + 0x1C) = r0;


            }
            goto L_80060400;
            L_800603E8: ;
            r0 = r6 & 0xFFFF;
            r7 = r3 & 0xFFFF;
            r0 = r0 << 2;
            r3 = r3 + 0x1;
            *(u32*)(r5 + r0) = r7;
            r6 = r6 + 0x1;
            L_80060400: ;
            r0 = r6 & 0xFFFF;
            if ((u32)r0 < (u32)0x6) goto L_800603E8;
        }
        r4 = r4 + 0x2;
        r5 = r5 + 0x18;
    } while (--ctr != 0);

    return;
}


/* 0x80060434 | size: 0x490 */
s32 fn_80060434(void) {
    extern void fn_800FA444();
    extern void fn_800FB680();
    extern void fn_80132A38();
    extern void fn_8025D9A8();
    extern void fn_8025DA88();
    extern void fn_8025DAD0();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    
    r29 = r3;
    fn_8025DA88();
    fn_8025D9A8();
    r27 = r3;
    fn_8025DAD0();
    r5 = (u32)&lbl_803A9A60;
    r4 = (u32)&lbl_802EF0A8;
    r5 = (u32)&lbl_803A9A60;
    r0 = *(u32*)((u8*)r5 + 0x4);
    r4 = (u32)&lbl_802EF0A8;
    r31 = *(u32*)((u8*)r5 + 0x3C0);
    r30 = r4 + (0x2 << 16);
    if ((s32)r0 != (s32)0x1) {
        if ((s32)r0 >= (s32)0x1) return;
        if ((s32)r0 < (s32)0x0) {
            return;
        }
        if ((s32)r27 != (s32)0x2) {
            if ((s32)r27 >= (s32)0x2) return;
            if ((s32)r27 != (s32)0x0) {
                if ((s32)r27 < (s32)0x0) {
                    return;

                    return;
                }
                if ((s32)r31 <= (s32)0x5) {
                    r4 = r31 + 0x1;
                    r3 = 0x2f;
                    fn_80132A38();
                    r3 = 0x3f39;
                    fn_800FA444();
                    r4 = *(s16*)((u8*)r30 + (-12178));
                    r5 = (u32)r3 >> 16;
                    r3 = *(u8*)((u8*)r29 + 0x8B);
                    r0 = -0x100;
                    r7 = r4 - r5;
                    r4 = 0x0;
                    r6 = (u32)r7 >> 31;
                    r5 = r3 | r0;
                    r0 = r6 + r7;
                    r6 = 0x3f39;
                    r3 = (s32)r0 >> 1;
                    fn_800FB680();
                    return;
                }
                if ((s32)r31 == (s32)0x6) {
                    r3 = 0x3f3a;
                    fn_800FA444();
                    r4 = *(s16*)((u8*)r30 + (-12178));
                    r5 = (u32)r3 >> 16;
                    r3 = *(u8*)((u8*)r29 + 0x8B);
                    r0 = -0x100;
                    r7 = r4 - r5;
                    r4 = 0x0;
                    r6 = (u32)r7 >> 31;
                    r5 = r3 | r0;
                    r0 = r6 + r7;
                    r6 = 0x3f3a;
                    r3 = (s32)r0 >> 1;
                    fn_800FB680();
                    return;
                }
                if ((s32)r31 != (s32)0x7) return;
                r3 = 0x3f3b;
                fn_800FA444();
                r4 = *(s16*)((u8*)r30 + (-12178));
                r5 = (u32)r3 >> 16;
                r3 = *(u8*)((u8*)r29 + 0x8B);
                r0 = -0x100;
                r7 = r4 - r5;
                r4 = 0x0;
                r6 = (u32)r7 >> 31;
                r5 = r3 | r0;
                r0 = r6 + r7;
                r6 = 0x3f3b;
                r3 = (s32)r0 >> 1;
                fn_800FB680();
                return;
                }
            r4 = r31 + 0x1;
            r3 = 0x2f;
            fn_80132A38();
            r3 = 0x3f3c;
            fn_800FA444();
            r4 = *(s16*)((u8*)r30 + (-12178));
            r5 = (u32)r3 >> 16;
            r3 = *(u8*)((u8*)r29 + 0x8B);
            r0 = -0x100;
            r7 = r4 - r5;
            r4 = 0x0;
            r6 = (u32)r7 >> 31;
            r5 = r3 | r0;
            r0 = r6 + r7;
            r6 = 0x3f3c;
            r3 = (s32)r0 >> 1;
            fn_800FB680();
            return;
        }
        r4 = (u32)&lbl_802ED9A0;
        r28 = r3 << 2;
        r27 = (u32)&lbl_802ED9A0;
        r3 = *(u32*)(r27 + r28);
        fn_800FA444();
        r4 = *(s16*)((u8*)r30 + (-12178));
        r5 = (u32)r3 >> 16;
        r3 = *(u8*)((u8*)r29 + 0x8B);
        r0 = -0x100;
        r7 = r4 - r5;
        r6 = *(u32*)(r27 + r28);
        r4 = (u32)r7 >> 31;
        r5 = r3 | r0;
        r0 = r4 + r7;
        r4 = 0x0;
        r3 = (s32)r0 >> 1;
        fn_800FB680();
        return;
    }
    if ((s32)r27 != (s32)0x2) {
        if ((s32)r27 >= (s32)0x2) return;
        if ((s32)r27 != (s32)0x0) {
            if ((s32)r27 < (s32)0x0) {
                return;

                return;
            }
            if ((s32)r31 <= (s32)0x5) {
                r4 = r31 + 0x1;
                r3 = 0x2f;
                fn_80132A38();
                r3 = 0x3f39;
                fn_800FA444();
                r4 = (u32)r3 >> 16;
                r3 = 0x3f3d;
                r26 = r4 + 0xb;
                fn_800FA444();
                r3 = (u32)r3 >> 16;
                r0 = *(s16*)((u8*)r30 + (-12178));
                r4 = r26 + r3;
                r3 = *(u8*)((u8*)r29 + 0x8B);
                r6 = r0 - r4;
                r0 = -0x100;
                r5 = (u32)r6 >> 31;
                r4 = 0x0;
                r6 = r5 + r6;
                r5 = r3 | r0;
                r27 = (s32)r6 >> 1;
                r6 = 0x3f39;
                r3 = r27;
                fn_800FB680();
                r5 = *(u8*)((u8*)r29 + 0x8B);
                r0 = -0x100;
                r3 = r27 + r26;
                r4 = 0x0;
                r5 = r5 | r0;
                r6 = 0x3f3d;
                fn_800FB680();
                return;
            }
            if ((s32)r31 == (s32)0x6) {
                r3 = 0x3f3a;
                fn_800FA444();
                r4 = (u32)r3 >> 16;
                r3 = 0x3f3d;
                r26 = r4 + 0x9;
                fn_800FA444();
                r3 = (u32)r3 >> 16;
                r0 = *(s16*)((u8*)r30 + (-12178));
                r4 = r26 + r3;
                r3 = *(u8*)((u8*)r29 + 0x8B);
                r6 = r0 - r4;
                r0 = -0x100;
                r5 = (u32)r6 >> 31;
                r4 = 0x0;
                r6 = r5 + r6;
                r5 = r3 | r0;
                r27 = (s32)r6 >> 1;
                r6 = 0x3f3a;
                r3 = r27;
                fn_800FB680();
                r5 = *(u8*)((u8*)r29 + 0x8B);
                r0 = -0x100;
                r3 = r27 + r26;
                r4 = 0x0;
                r5 = r5 | r0;
                r6 = 0x3f3d;
                fn_800FB680();
                return;
            }
            if ((s32)r31 != (s32)0x7) return;
            r3 = 0x3f3b;
            fn_800FA444();
            r4 = (u32)r3 >> 16;
            r3 = 0x3f3d;
            r26 = r4 + 0x9;
            fn_800FA444();
            r3 = (u32)r3 >> 16;
            r0 = *(s16*)((u8*)r30 + (-12178));
            r4 = r26 + r3;
            r3 = *(u8*)((u8*)r29 + 0x8B);
            r6 = r0 - r4;
            r0 = -0x100;
            r5 = (u32)r6 >> 31;
            r4 = 0x0;
            r6 = r5 + r6;
            r5 = r3 | r0;
            r27 = (s32)r6 >> 1;
            r6 = 0x3f3b;
            r3 = r27;
            fn_800FB680();
            r5 = *(u8*)((u8*)r29 + 0x8B);
            r0 = -0x100;
            r3 = r26 + r27;
            r4 = 0x0;
            r5 = r5 | r0;
            r6 = 0x3f3d;
            fn_800FB680();
            return;
            }
        r3 = 0x3f3c;
        fn_800FA444();
        r4 = (u32)r3 >> 16;
        r3 = 0x3f3d;
        r26 = r4 + 0xb;
        fn_800FA444();
        r3 = (u32)r3 >> 16;
        r0 = *(s16*)((u8*)r30 + (-12178));
        r3 = r26 + r3;
        r4 = r31 + 0x1;
        r5 = r0 - r3;
        r3 = 0x2f;
        r0 = (u32)r5 >> 31;
        r0 = r0 + r5;
        r27 = (s32)r0 >> 1;
        fn_80132A38();
        r5 = *(u8*)((u8*)r29 + 0x8B);
        r0 = -0x100;
        r3 = r27;
        r4 = 0x0;
        r5 = r5 | r0;
        r6 = 0x3f3c;
        fn_800FB680();
        r5 = *(u8*)((u8*)r29 + 0x8B);
        r0 = -0x100;
        r3 = r26 + r27;
        r4 = 0x0;
        r5 = r5 | r0;
        r6 = 0x3f3d;
        fn_800FB680();
        return;
    }
    r4 = (u32)&lbl_802ED9A0;
    r27 = r3 << 2;
    r28 = (u32)&lbl_802ED9A0;
    r3 = *(u32*)(r28 + r27);
    fn_800FA444();
    r4 = (u32)r3 >> 16;
    r3 = 0x3f3d;
    r26 = r4 + 0xb;
    fn_800FA444();
    r3 = (u32)r3 >> 16;
    r0 = *(s16*)((u8*)r30 + (-12178));
    r3 = r26 + r3;
    r4 = r31;
    r5 = r0 - r3;
    r3 = 0x2f;
    r0 = (u32)r5 >> 31;
    r0 = r0 + r5;
    r30 = (s32)r0 >> 1;
    fn_80132A38();
    r4 = *(u8*)((u8*)r29 + 0x8B);
    r0 = -0x100;
    r6 = *(u32*)(r28 + r27);
    r3 = r30;
    r5 = r4 | r0;
    r4 = 0x0;
    fn_800FB680();
    r5 = *(u8*)((u8*)r29 + 0x8B);
    r0 = -0x100;
    r3 = r26 + r30;
    r4 = 0x0;
    r5 = r5 | r0;
    r6 = 0x3f3d;
    fn_800FB680();

    return;
}


/* 0x800608C4 | size: 0xF0 */
s32 fn_800608C4(void) {
    extern void fn_800D59B8();
    extern void fn_800D5CB8();
    extern void fn_800D61E4();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D85D4();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    extern void fn_80109934();
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r30 = r4;
    r3 = (u32)&lbl_803A9A60;
    r3 = r3 + 0x36c;
    fn_80109934();
    r31 = r3;
    if (r31 != 0) {
        r3 = 0x3;
        fn_800D88DC();
        r3 = 0x4;
        fn_800D888C();
        r3 = 0x7;
        fn_800D6A00();
        r3 = (u32)&lbl_80314F98;
        fn_800D7820();
        r4 = r31;
        r3 = 0x0;
        fn_800D85D4();
        r3 = 0x2;
        fn_800D67BC();
        r3 = 0x0;
        r4 = 0x0;
        fn_800D61E4();
        r3 = 0x0;
        r4 = 0xff;
        r5 = 0xff;
        r6 = 0xff;
        r7 = 0xff;
        fn_800D5CB8();
        f1 = *(f32*)&lbl_8047BF60;
        r3 = 0x0;
        f2 = f1;
        fn_800D59B8();
        r3 = *(s16*)((u8*)r30 + 0x54);
        r4 = *(s16*)((u8*)r30 + 0x56);
        fn_800D61E4();
        r3 = 0x0;
        r4 = 0xff;
        r5 = 0xff;
        r6 = 0xff;
        r7 = 0xff;
        fn_800D5CB8();
        f1 = *(f32*)&lbl_8047BF90;
        r3 = 0x0;
        f2 = f1;
        fn_800D59B8();
        fn_800D6728();
    }
    return 0;
}


/* 0x800609B4 | size: 0x74 */
void fn_800609B4(void) {
    extern void fn_800FE4D4();
    extern void fn_800FE6D0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    
    r0 = *(s16*)((u8*)r4 + 0x6);
    f0 = (f64)(s32)f1;
    r5 = (u32)&lbl_802EF0A8;
    r6 = r0 * 0x1c;
    *(f64*)(sp + 0x8) = f0;
    r0 = (u32)&lbl_802EF0A8;
    r5 = r0 + r6;
    r5 = *(s16*)((u8*)r5 + 0x2);
    r0 = r5 + r0;
    r0 = (s16)r0;
    *(u16*)((u8*)r4 + 0x50) = r0;
    r7 = *(s16*)((u8*)r3 + 0x84);
    r6 = *(s16*)((u8*)r4 + 0x50);
    r5 = *(s16*)((u8*)r3 + 0x86);
    r0 = *(s16*)((u8*)r4 + 0x52);
    r3 = r7 + r6;
    r3 = (s16)r3;
    r0 = r5 + r0;
    r4 = (s16)r0;
    fn_800FE6D0();
    fn_800FE4D4();
    return;
}


/* 0x80060A28 | size: 0x348 */
void fn_80060A28(void) {
    u32 r0 = 0;
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
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    
    r3 = (u32)&lbl_803A9A60;
    r5 = 0x0;
    r6 = (u32)&lbl_803A9A60;
    r4 = r6;
    f3 = *(f32*)&lbl_8047BF60;
    do {
        r3 = r4 + 0x58;
        r7 = 0x0;
        r0 = 0x6;
        ctr_fn = (void(*)(void))r0;
        do {
            f1 = *(f32*)((u8*)r3 + 0x24);
            if (f3 != f1) {
                f0 = *(f32*)((u8*)r6 + 0x3C);
                f0 = f1 - f0;
                *(f32*)((u8*)r3 + 0x24) = f0;
                f0 = *(f32*)((u8*)r3 + 0x24);
                if (f0 >= f3) goto L_80060B34;
                *(f32*)((u8*)r3 + 0x24) = f3;
                goto L_80060B34;
            }
            f0 = *(f32*)((u8*)r3 + 0x3C);
            f1 = *(f32*)((u8*)r3 + 0x54);
            if (f0 == f1) goto L_80060B34;
            f4 = f1 - f0;
            f2 = *(f32*)&lbl_8047BF94;
            f1 = *(f32*)((u8*)r6 + 0x3C);
            f0 = *(f32*)&lbl_8047BF98;
            f2 = f2 * f4;
            f4 = f2 * f1;
            if (f4 > f0) {
                f4 = f0;
            }
            f0 = *(f32*)&lbl_8047BF9C;
            /* cror eq, lt, eq */;
            if (f4 == f0) {
                f4 = f0;
            }
            f1 = *(f32*)((u8*)r3 + 0x3C);
            f0 = *(f32*)&lbl_8047BF60;
            f1 = f1 + f4;
            *(f32*)((u8*)r3 + 0x3C) = f1;
            f2 = *(f32*)((u8*)r3 + 0x54);
            f0 = *(f32*)((u8*)r3 + 0x3C);
            f1 = f2 - f0;
            if (f4 > f0) {
            } else {

                f4 = -f4;
            }
            f0 = *(f32*)&lbl_8047BF60;
            if (f1 > f0) {
                f0 = f1;
            } else {

                f0 = -f1;
            }
            /* cror eq, lt, eq */;
            if (f0 != f4) {
                f0 = *(f32*)&lbl_8047BF60;
                if (f1 > f0) {
                } else {

                    f1 = -f1;
                }
                f0 = *(f32*)&lbl_8047BF90;
                if (f1 >= f0) goto L_80060B34;
            }
            *(f32*)((u8*)r3 + 0x3C) = f2;
            L_80060B34: ;
            r3 = r3 + 0x4;
            r7 = r7 + 0x1;
        } while (--ctr != 0);
        r4 = r4 + 0xb4;
        r5 = r5 + 0x1;
    } while ((s32)r5 < (s32)0x4);
    r3 = r6;
    r0 = 0x4;
    f3 = *(f32*)&lbl_8047BF60;
    ctr_fn = (void(*)(void))r0;
    do {
        r4 = r3 + 0x328;
        f1 = *(f32*)((u8*)r4 + 0x0);
        if (f3 != f1) {
            f0 = *(f32*)((u8*)r6 + 0x3C);
            f0 = f1 - f0;
            *(f32*)((u8*)r4 + 0x0) = f0;
            f0 = *(f32*)((u8*)r4 + 0x0);
            if (f0 >= f3) goto L_80060C4C;
            *(f32*)((u8*)r4 + 0x0) = f3;
            goto L_80060C4C;
        }
        f0 = *(f32*)((u8*)r4 + 0x4);
        f1 = *(f32*)((u8*)r4 + 0x8);
        if (f0 == f1) goto L_80060C4C;
        f4 = f1 - f0;
        f2 = *(f32*)&lbl_8047BF94;
        f1 = *(f32*)((u8*)r6 + 0x3C);
        f0 = *(f32*)&lbl_8047BF98;
        f2 = f2 * f4;
        f4 = f2 * f1;
        if (f4 > f0) {
            f4 = f0;
        }
        f0 = *(f32*)&lbl_8047BF9C;
        /* cror eq, lt, eq */;
        if (f4 == f0) {
            f4 = f0;
        }
        f1 = *(f32*)((u8*)r4 + 0x4);
        f0 = *(f32*)&lbl_8047BF60;
        f1 = f1 + f4;
        *(f32*)((u8*)r4 + 0x4) = f1;
        f2 = *(f32*)((u8*)r4 + 0x8);
        f0 = *(f32*)((u8*)r4 + 0x4);
        f1 = f2 - f0;
        if (f4 > f0) {
        } else {

            f4 = -f4;
        }
        f0 = *(f32*)&lbl_8047BF60;
        if (f1 > f0) {
            f0 = f1;
        } else {

            f0 = -f1;
        }
        /* cror eq, lt, eq */;
        if (f0 != f4) {
            f0 = *(f32*)&lbl_8047BF60;
            if (f1 > f0) {
            } else {

                f1 = -f1;
            }
            f0 = *(f32*)&lbl_8047BF90;
            if (f1 >= f0) goto L_80060C4C;
        }
        *(f32*)((u8*)r4 + 0x4) = f2;
        L_80060C4C: ;
        r3 = r3 + 0xc;
    } while (--ctr != 0);
    r3 = (u32)&lbl_803A9A60;
    r4 = r7 << 2;
    r0 = (u32)&lbl_803A9A60;
    r5 = r6;
    r3 = r0 + r4;
    r3 = r3 + 0x40;
    r0 = 0x2;
    f3 = *(f32*)&lbl_8047BF60;
    ctr_fn = (void(*)(void))r0;
    do {
        f0 = *(f32*)((u8*)r5 + 0x40);
        if (f3 != f0) {
            f1 = *(f32*)((u8*)r3 + 0x0);
            f0 = *(f32*)((u8*)r6 + 0x3C);
            f0 = f1 - f0;
            *(f32*)((u8*)r3 + 0x0) = f0;
            f0 = *(f32*)((u8*)r3 + 0x0);
            if (f0 >= f3) goto L_80060D64;
            *(f32*)((u8*)r3 + 0x0) = f3;
            goto L_80060D64;
        }
        f0 = *(f32*)((u8*)r5 + 0x48);
        f1 = *(f32*)((u8*)r5 + 0x50);
        if (f0 == f1) goto L_80060D64;
        f4 = f1 - f0;
        f2 = *(f32*)&lbl_8047BF94;
        f1 = *(f32*)((u8*)r6 + 0x3C);
        f0 = *(f32*)&lbl_8047BF98;
        f2 = f2 * f4;
        f4 = f2 * f1;
        if (f4 > f0) {
            f4 = f0;
        }
        f0 = *(f32*)&lbl_8047BF9C;
        /* cror eq, lt, eq */;
        if (f4 == f0) {
            f4 = f0;
        }
        f1 = *(f32*)((u8*)r5 + 0x48);
        f0 = *(f32*)&lbl_8047BF60;
        f1 = f1 + f4;
        *(f32*)((u8*)r5 + 0x48) = f1;
        f2 = *(f32*)((u8*)r5 + 0x50);
        f0 = *(f32*)((u8*)r5 + 0x48);
        f1 = f2 - f0;
        if (f4 > f0) {
        } else {

            f4 = -f4;
        }
        f0 = *(f32*)&lbl_8047BF60;
        if (f1 > f0) {
            f0 = f1;
        } else {

            f0 = -f1;
        }
        /* cror eq, lt, eq */;
        if (f0 != f4) {
            f0 = *(f32*)&lbl_8047BF60;
            if (f1 > f0) {
            } else {

                f1 = -f1;
            }
            f0 = *(f32*)&lbl_8047BF90;
            if (f1 >= f0) goto L_80060D64;
        }
        *(f32*)((u8*)r5 + 0x48) = f2;
        L_80060D64: ;
        r5 = r5 + 0x4;
    } while (--ctr != 0);
    return;
}


/* 0x80060D70 | size: 0x184 */
s32 fn_80060D70(void) {
    extern void fn_801EF634();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    
    r29 = r4;
    r30 = r5;
    r31 = r6;
    r3 = (u32)&lbl_803A9A60;
    r5 = *(u32*)&lbl_8047BF50;
    r3 = (u32)&lbl_803A9A60;
    r4 = *(u32*)&lbl_8047BF54;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((s32)r0 == (s32)0x1) {
        fn_801EF634();
        r0 = r3 & 0xFFFF;
        switch ((s32)r0) {
            case 2:
            case 5:
                r3 = 0x0;
                r0 = 0x1;
                *(u32*)(sp + 0xC) = r0;
                break;
            case 3:
            case 4:
                r3 = 0x1;
                r0 = 0x0;
                *(u32*)(sp + 0xC) = r0;
                break;
            case 6:
            case 7:
                r0 = 0x2;
                *(u32*)(sp + 0x8) = r0;
                *(u32*)(sp + 0xC) = r0;
                break;
            default:
                r0 = 0x2;
                *(u32*)(sp + 0x8) = r0;
                *(u32*)(sp + 0xC) = r0;
                break;
        }
        r3 = (u32)&lbl_803A9A60;
        r4 = (u32)&lbl_803A9A60;
        r0 = *(u32*)((u8*)r4 + 0x38);
        if ((s32)r0 >= (s32)0x6) {
            r0 = r30 << 2;
            r3 = (u32)sp + 0x8;
            r0 = *(u32*)(r3 + r0);
            if ((s32)r31 == (s32)r0) {
                r0 = r30 << 3;
                f2 = *(f32*)&lbl_8047BF90;
                r3 = r4 + r0;
                f0 = *(f32*)&lbl_8047BFA0;
                f3 = *(f32*)((u8*)r3 + 0x358);
                f1 = f3 - f2;
                f1 = f2 - f1;
                f0 = f0 * f1;
                f0 = (f64)(s32)f0;
                *(f64*)(sp + 0x10) = f0;
                *(u8*)((u8*)r29 + 0x67) = r0;
                *(f32*)((u8*)r29 + 0x68) = f3;
                *(f32*)((u8*)r29 + 0x6C) = f3;
                r0 = *(u8*)((u8*)r29 + 0x4);
                r0 = r0 | 0x2;
                r0 = (s8)r0;
                *(u8*)((u8*)r29 + 0x4) = r0;
                return;
            }
            r0 = *(u8*)((u8*)r29 + 0x4);
            r0 = r0 & 0xFFFFFFFD;
            r0 = (s8)r0;
            *(u8*)((u8*)r29 + 0x4) = r0;
            return;
        }
        r0 = *(u8*)((u8*)r29 + 0x4);
        r0 = r0 & 0xFFFFFFFD;
        r0 = (s8)r0;
        *(u8*)((u8*)r29 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r29 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r29 + 0x4) = r0;

    return;
}


/* 0x80060EF4 | size: 0x124 */
void fn_80060EF4(void) {
    extern void fn_8025D9A8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    
    r31 = r4;
    r29 = r5;
    r3 = (u32)&lbl_803A9A60;
    r3 = (u32)&lbl_803A9A60;
    r3 = *(u32*)((u8*)r3 + 0x0);
    r30 = *(u32*)((u8*)r3 + 0xC);
    fn_8025D9A8();
    if ((s32)r29 < (s32)0x0) {
        if ((s32)r3 == (s32)0x1) {
            r0 = *(u8*)((u8*)r31 + 0x4);
            r0 = r0 | 0x2;
            r0 = (s8)r0;
            *(u8*)((u8*)r31 + 0x4) = r0;
            return;
        }
        if ((s32)r29 == (s32)r30) {
            r0 = *(u8*)((u8*)r31 + 0x4);
            r0 = r0 | 0x2;
            r0 = (s8)r0;
            *(u8*)((u8*)r31 + 0x4) = r0;
            return;
        }
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 & 0xFFFFFFFD;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    if ((s32)r3 == (s32)0x1) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 & 0xFFFFFFFD;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    if ((s32)r30 == (s32)0x5) {
        if ((s32)r29 == (s32)0x3) {
            r0 = *(u8*)((u8*)r31 + 0x4);
            r0 = r0 | 0x2;
            r0 = (s8)r0;
            *(u8*)((u8*)r31 + 0x4) = r0;
            return;
        }
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 & 0xFFFFFFFD;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    if ((s32)r29 == (s32)r30) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;

    return;
}


/* 0x80061018 | size: 0x10 */
void fn_80061018(void) {
}

/* 0x80061028 | size: 0x44 */
s32 fn_80061028(void) {
    extern void fn_80102568();
    u8 sp[0x10];
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    
    r31 = r3;
    r3 = 0xba;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
    r3 = (u32)&lbl_803A9A60;
    r3 = (u32)&lbl_803A9A60;
    *(u32*)((u8*)r3 + 0x4) = r31;
    return;
}


/* 0x8006106C | size: 0x1D4 */
s32 fn_8006106C(void) {
    extern void fn_80061D34();
    extern void fn_80069A08();
    extern void fn_800FE4D4();
    extern void fn_800FE6D0();
    extern void fn_801040F0();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    
    r25 = r3;
    r26 = r4;
    r27 = r5;
    r28 = r6;
    r29 = r7;
    fn_80061D34();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_800611F0;
    r6 = r27 * 0xb4;
    r5 = (u32)&lbl_803A9A60;
    r4 = *(s16*)((u8*)r26 + 0x6);
    r3 = (u32)&lbl_802EF0A8;
    r0 = (u32)&lbl_803A9A60;
    r5 = r0 + r6;
    r31 = r5 + 0x58;
    r30 = r28 << 2;
    r5 = r31 + r30;
    r0 = (u32)&lbl_802EF0A8;
    f0 = *(f32*)((u8*)r5 + 0x3C);
    r3 = r4 * 0x1c;
    f0 = (f64)(s32)f0;
    r3 = r0 + r3;
    r3 = *(s16*)((u8*)r3 + 0x2);
    *(f64*)(sp + 0x8) = f0;
    r0 = r3 + r0;
    r0 = (s16)r0;
    *(u16*)((u8*)r26 + 0x50) = r0;
    r5 = *(s16*)((u8*)r25 + 0x84);
    r3 = *(s16*)((u8*)r26 + 0x50);
    r4 = *(s16*)((u8*)r25 + 0x86);
    r0 = *(s16*)((u8*)r26 + 0x52);
    r3 = r5 + r3;
    r3 = (s16)r3;
    r0 = r4 + r0;
    r4 = (s16)r0;
    fn_800FE6D0();
    fn_800FE4D4();
    r3 = r25;
    r4 = r26;
    r5 = r27;
    r6 = r28;
    fn_80069A08();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r24 = r28 << 1;
        r0 = *(s16*)(r31 + r24);
        if ((s32)r0 == (s32)0x0) goto L_800611F0;
        r0 = r0 << 1;
        r3 = (u32)&lbl_80478910;
        r6 = *(u16*)(r3 + r0);
        r5 = r25;
        r3 = 0x0;
        r4 = 0x0;
        r7 = 0x0;
        fn_801040F0();
        r3 = (u32)&lbl_803A9A60;
        r3 = (u32)&lbl_803A9A60;
        r0 = *(u32*)((u8*)r3 + 0x38);
        if ((s32)r0 != (s32)0x3) goto L_800611F0;
        r4 = r31 + r30;
        f1 = *(f32*)((u8*)r3 + 0x3C);
        r4 = r4 + 0xc;
        f0 = *(f32*)&lbl_8047BFA4;
        f2 = *(f32*)((u8*)r4 + 0x0);
        f1 = f2 + f1;
        *(f32*)((u8*)r4 + 0x0) = f1;
        f1 = *(f32*)((u8*)r4 + 0x0);
        /* cror eq, gt, eq */;
        if (f1 != f0) goto L_800611F0;
        f0 = *(f32*)&lbl_8047BF60;
        *(f32*)((u8*)r4 + 0x0) = f0;
        r3 = *(s16*)(r31 + r24);
        /* subi r0, r3, 0x1 */;
        *(u16*)(r31 + r24) = r0;
        goto L_800611F0;
    }
    r0 = r28 << 1;
    r0 = *(s16*)(r31 + r0);
    if ((s32)r0 == (s32)0x0) goto L_800611F0;
    r0 = r0 << 1;
    r3 = (u32)&lbl_80478910;
    r6 = *(u16*)(r3 + r0);
    r5 = r25;
    r3 = 0x0;
    r4 = 0x0;
    r7 = 0x0;
    fn_801040F0();
    L_800611F0: ;
    r3 = r25;
    r4 = r26;
    r5 = r27;
    r6 = r28;
    r7 = r29;
    fn_80061D34();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r5 = r25;
        r3 = -0x8;
        r4 = -0x8;
        r6 = 0x40;
        r7 = 0x0;
        fn_801040F0();
    }
    return;
}


/* 0x80061240 | size: 0x214 */
s32 fn_80061240(void) {
    extern void fn_800D5BA0();
    extern void fn_800D61E4();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    u8 sp[0x60];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f31 = 0.0f;

    
    *(f64*)(sp + 0x50) = f31;
    /* psq_st f31, 0x58((u32)sp), 0, qr0 */;
    r27 = r4;
    r5 = r5 * 0xb4;
    r4 = (u32)&lbl_803A9A60;
    r6 = r6 << 2;
    f0 = *(f32*)&lbl_8047BFA8;
    r0 = (u32)&lbl_803A9A60;
    r26 = *(u8*)((u8*)r3 + 0x8B);
    r3 = r0 + r5;
    r0 = r3 + 0x58;
    r3 = r0 + r6;
    f2 = *(f32*)((u8*)r3 + 0x6C);
    f1 = *(f32*)((u8*)r3 + 0x9C);
    f1 = f2 / f1;
    *(f32*)&lbl_8047891C = f1;
    /* cror eq, lt, eq */;
    if (f1 == f0) {
        r28 = 0xa7;
        r29 = 0x23;
        r25 = 0x13;
        goto L_800612D8;
    }
    f0 = *(f32*)&lbl_8047BF68;
    /* cror eq, lt, eq */;
    if (f1 == f0) {
        r28 = 0xc1;
        r29 = 0xbd;
        r25 = 0x16;
        goto L_800612D8;
    }
    r28 = 0x5;
    r29 = 0xb3;
    r25 = 0x11;
    L_800612D8: ;
    f0 = *(f32*)&lbl_8047BF60;
    if (f0 != f1) {
        r3 = 0x1;
        fn_800D88DC();
        r3 = 0x6;
        fn_800D888C();
        r3 = (u32)&lbl_80314E08;
        r3 = (u32)&lbl_80314E08;
        fn_800D7820();
        r3 = 0x4;
        fn_800D6A00();
        r3 = 0x4;
        fn_800D67BC();
        /* lha r3, lbl_80478918@sda21(r0) */;
        /* lha r4, lbl_8047891A@sda21(r0) */;
        fn_800D61E4();
        r30 = r28 & 0xFF;
        r31 = r29 & 0xFF;
        r29 = r25 & 0xFF;
        r28 = r26 & 0xFF;
        /* subi r4, r30, 0x2 */;
        /* subi r0, r31, 0x2 */;
        /* subi r5, r29, 0x2 */;
        r3 = 0x0;
        r4 = r4 & 0xFF;
        r0 = r0 & 0xFF;
        r5 = r5 & 0xFF;
        r4 = r4 << 24;
        r0 = r0 << 16;
        r5 = r5 << 8;
        r0 = r4 | r0;
        r0 = r5 | r0;
        r26 = r28 | r0;
        r4 = r26;
        fn_800D5BA0();
        /* lha r5, lbl_80478918@sda21(r0) */;
        r3 = (0x4330 << 16);
        r4 = *(s16*)((u8*)r27 + 0x54);
        /* xoris r0, r5, 0x8000 */;
        r4 = r4 - r5;
        f2 = *(f64*)&lbl_8047BF80;
        /* xoris r4, r4, 0x8000 */;
        *(u32*)(sp + 0x14) = r0;
        f3 = *(f32*)&lbl_8047891C;
        /* lha r4, lbl_8047891A@sda21(r0) */;
        f1 = *(f64*)(sp + 0x8);
        f0 = *(f64*)(sp + 0x10);
        f1 = f1 - f2;
        f0 = f0 - f2;
        f31 = f3 * f1 + f0;
        f0 = (f64)(s32)f31;
        *(f64*)(sp + 0x18) = f0;
        fn_800D61E4();
        r4 = r26;
        r3 = 0x0;
        fn_800D5BA0();
        /* lha r4, lbl_8047891A@sda21(r0) */;
        r0 = *(s16*)((u8*)r27 + 0x56);
        /* lha r3, lbl_80478918@sda21(r0) */;
        r0 = r0 - r4;
        r4 = (s16)r0;
        fn_800D61E4();
        r4 = r30 << 24;
        r0 = r31 << 16;
        r5 = r29 << 8;
        r3 = 0x0;
        r0 = r4 | r0;
        r0 = r5 | r0;
        r26 = r28 | r0;
        r4 = r26;
        fn_800D5BA0();
        f0 = (f64)(s32)f31;
        /* lha r3, lbl_8047891A@sda21(r0) */;
        r0 = *(s16*)((u8*)r27 + 0x56);
        *(f64*)(sp + 0x20) = f0;
        r0 = r0 - r3;
        r4 = (s16)r0;
        fn_800D61E4();
        r4 = r26;
        r3 = 0x0;
        fn_800D5BA0();
        fn_800D6728();
    }
    /* psq_l f31, 0x58((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x50);
    return;
}


/* 0x80061454 | size: 0x1A0 */
s32 fn_80061454(void) {
    extern void fn_800D59B8();
    extern void fn_800D5CB8();
    extern void fn_800D61E4();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D85D4();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    extern void fn_800FE4D4();
    extern void fn_800FE6D0();
    extern void fn_8025DA88();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    
    r28 = r3;
    r29 = r4;
    r25 = r5;
    r26 = r6;
    r3 = (u32)&lbl_803A9A60;
    r4 = r25 << 3;
    r0 = (u32)&lbl_803A9A60;
    r31 = *(u8*)((u8*)r28 + 0x8B);
    r3 = r0 + r4;
    r27 = 0x1;
    r30 = r3 + 0xc;
    fn_8025DA88();
    if ((s32)r26 == (s32)0x2) {
        if ((s32)r3 == (s32)0x2) goto L_800614B8;
        r27 = 0x0;
        goto L_800614B8;
    }
    if ((s32)r3 != (s32)0x2) goto L_800614B8;
    r27 = 0x0;
    L_800614B8: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r4 = r25 * 0xc;
        r3 = (u32)&lbl_803A9A60;
        r6 = *(s16*)((u8*)r29 + 0x6);
        r5 = (u32)&lbl_802EF0A8;
        r0 = (u32)&lbl_803A9A60;
        r3 = r0 + r4;
        f0 = *(f32*)((u8*)r3 + 0x32C);
        r3 = r6 * 0x1c;
        r0 = (u32)&lbl_802EF0A8;
        f0 = (f64)(s32)f0;
        r3 = r0 + r3;
        r3 = *(s16*)((u8*)r3 + 0x2);
        *(f64*)(sp + 0x8) = f0;
        r0 = r3 + r0;
        r0 = (s16)r0;
        *(u16*)((u8*)r29 + 0x50) = r0;
        r5 = *(s16*)((u8*)r28 + 0x84);
        r3 = *(s16*)((u8*)r29 + 0x50);
        r4 = *(s16*)((u8*)r28 + 0x86);
        r0 = *(s16*)((u8*)r29 + 0x52);
        r3 = r5 + r3;
        r3 = (s16)r3;
        r0 = r4 + r0;
        r4 = (s16)r0;
        fn_800FE6D0();
        fn_800FE4D4();
        r27 = *(u32*)((u8*)r30 + 0x0);
        if ((u32)r27 != (u32)0x0) {
            r3 = 0x3;
            fn_800D88DC();
            r3 = 0x4;
            fn_800D888C();
            r3 = 0x7;
            fn_800D6A00();
            r3 = (u32)&lbl_80314F98;
            r3 = (u32)&lbl_80314F98;
            fn_800D7820();
            r4 = r27;
            r3 = 0x0;
            fn_800D85D4();
            r3 = 0x2;
            fn_800D67BC();
            r3 = 0x0;
            r4 = 0x0;
            fn_800D61E4();
            r7 = r31;
            r3 = 0x0;
            r4 = 0xff;
            r5 = 0xff;
            r6 = 0xff;
            fn_800D5CB8();
            f1 = *(f32*)&lbl_8047BF60;
            r3 = 0x0;
            f2 = f1;
            fn_800D59B8();
            r3 = *(s16*)((u8*)r29 + 0x54);
            r4 = *(s16*)((u8*)r29 + 0x56);
            fn_800D61E4();
            r7 = r31;
            r3 = 0x0;
            r4 = 0xff;
            r5 = 0xff;
            r6 = 0xff;
            fn_800D5CB8();
            f1 = *(f32*)&lbl_8047BF90;
            r3 = 0x0;
            f2 = f1;
            fn_800D59B8();
            fn_800D6728();
    }
    }
    return;
}


/* 0x800615F4 | size: 0x1EC */
s32 fn_800615F4(void) {
    extern void fn_800FA280();
    extern void fn_800FB680();
    extern void fn_800FBB34();
    extern void fn_800FE4D4();
    extern void fn_800FE6D0();
    extern void fn_80132A38();
    extern void fn_801FBD28();
    extern void fn_801FBD58();
    extern void fn_801FCC64();
    extern void fn_801FCCC4();
    extern void fn_8025D28C();
    extern void fn_8025D9CC();
    extern void fn_8025DA18();
    extern void fn_8025DA88();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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
    f32 f0 = 0.0f;

    
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    fn_8025D9CC();
    r31 = r3;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r30 == (s32)0x2) {
        if ((s32)r3 == (s32)0x2) goto L_80061648;
        r26 = 0x0;
        goto L_80061648;
    }
    if ((s32)r3 != (s32)0x2) goto L_80061648;
    r26 = 0x0;
    L_80061648: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r29 * 0xc;
    r3 = (u32)&lbl_803A9A60;
    r6 = *(s16*)((u8*)r28 + 0x6);
    r5 = (u32)&lbl_802EF0A8;
    r0 = (u32)&lbl_803A9A60;
    r3 = r0 + r4;
    f0 = *(f32*)((u8*)r3 + 0x32C);
    r3 = r6 * 0x1c;
    r0 = (u32)&lbl_802EF0A8;
    f0 = (f64)(s32)f0;
    r3 = r0 + r3;
    r3 = *(s16*)((u8*)r3 + 0x2);
    *(f64*)(sp + 0x8) = f0;
    r0 = r3 + r0;
    r0 = (s16)r0;
    *(u16*)((u8*)r28 + 0x50) = r0;
    r5 = *(s16*)((u8*)r27 + 0x84);
    r3 = *(s16*)((u8*)r28 + 0x50);
    r4 = *(s16*)((u8*)r27 + 0x86);
    r0 = *(s16*)((u8*)r28 + 0x52);
    r3 = r5 + r3;
    r3 = (s16)r3;
    r0 = r4 + r0;
    r4 = (s16)r0;
    fn_800FE6D0();
    fn_800FE4D4();
    if ((s32)r31 == (s32)0x4) {
        if ((s32)r30 != (s32)0x0) return;
        r3 = r29;
        fn_8025DA18();
        r4 = r3 & 0xFFFF;
        r3 = 0x34;
        r4 = r4 + 0x1;
        fn_80132A38();
        if ((s32)r29 == (s32)0x0) {
            r4 = *(u8*)((u8*)r27 + 0x8B);
            r0 = -0x100;
            r5 = *(s16*)((u8*)r28 + 0x54);
            r3 = 0x0;
            r6 = *(s16*)((u8*)r28 + 0x56);
            r7 = r4 | r0;
            r4 = 0x0;
            r8 = 0x30e9;
            fn_800FBB34();
            return;
        }
        r5 = *(u8*)((u8*)r27 + 0x8B);
        r0 = -0x100;
        r3 = 0x0;
        r4 = 0x0;
        r5 = r5 | r0;
        r6 = 0x30e5;
        fn_800FB680();
        return;
    }
    r3 = r29;
    fn_8025D28C();
    r28 = r3;
    r3 = r28 & 0xFFFF;
    fn_801FCCC4();
    fn_801FCC64();
    fn_801FBD58();
    fn_801FBD28();
    r0 = r28 & 0xFFFF;
    r3 = (u32)&lbl_803A9A60;
    r3 = (u32)&lbl_803A9A60;
    r3 = *(u32*)((u8*)r3 + 0x3DC);
    if ((u32)r0 == (u32)0x0) {
        r3 = 0x1;
        fn_800FA280();
        r28 = r3;
    } else {

        fn_800FA280();
        r28 = r3;
    }
    r4 = r28;
    r3 = 0x37;
    fn_80132A38();
    r4 = r28;
    r3 = 0x4d;
    fn_80132A38();
    if ((s32)r30 != (s32)0x0) return;
    if ((s32)r29 == (s32)0x0) return;
    r5 = *(u8*)((u8*)r27 + 0x8B);
    r0 = -0x100;
    r3 = 0x0;
    r4 = 0x0;
    r5 = r5 | r0;
    r6 = 0xcf;
    fn_800FB680();

    return;
}


/* 0x800617E0 | size: 0x24C */
s32 fn_800617E0(void) {
    extern void fn_800FA280();
    extern void fn_800FB680();
    extern void fn_800FBB34();
    extern void fn_800FE4D4();
    extern void fn_800FE6D0();
    extern void fn_8012AC54();
    extern void fn_80132A38();
    extern void fn_8025D914();
    extern void fn_8025D9CC();
    extern void fn_8025DA18();
    extern void fn_8025DA88();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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
    f32 f0 = 0.0f;

    
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r31 = r6;
    r27 = 0x1;
    fn_8025DA88();
    if ((s32)r31 == (s32)0x2) {
        if ((s32)r3 == (s32)0x2) goto L_8006182C;
        r27 = 0x0;
        goto L_8006182C;
    }
    if ((s32)r3 != (s32)0x2) goto L_8006182C;
    r27 = 0x0;
    L_8006182C: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r30 * 0xc;
    r3 = (u32)&lbl_803A9A60;
    r6 = *(s16*)((u8*)r29 + 0x6);
    r5 = (u32)&lbl_802EF0A8;
    r0 = (u32)&lbl_803A9A60;
    r3 = r0 + r4;
    f0 = *(f32*)((u8*)r3 + 0x32C);
    r3 = r6 * 0x1c;
    r0 = (u32)&lbl_802EF0A8;
    f0 = (f64)(s32)f0;
    r3 = r0 + r3;
    r3 = *(s16*)((u8*)r3 + 0x2);
    *(f64*)(sp + 0x8) = f0;
    r0 = r3 + r0;
    r0 = (s16)r0;
    *(u16*)((u8*)r29 + 0x50) = r0;
    r5 = *(s16*)((u8*)r28 + 0x84);
    r3 = *(s16*)((u8*)r29 + 0x50);
    r4 = *(s16*)((u8*)r28 + 0x86);
    r0 = *(s16*)((u8*)r29 + 0x52);
    r3 = r5 + r3;
    r3 = (s16)r3;
    r0 = r4 + r0;
    r4 = (s16)r0;
    fn_800FE6D0();
    fn_800FE4D4();
    r3 = r30;
    fn_8025D914();
    fn_8012AC54();
    r27 = r3;
    if ((u32)r27 == (u32)0x0) {
        r3 = 0x1;
        fn_800FA280();
        r27 = r3;
    }
    fn_8025D9CC();
    if ((s32)r3 == (s32)0x4) {
        r4 = r27;
        r3 = 0x37;
        fn_80132A38();
        r4 = r27;
        r3 = 0x4d;
        fn_80132A38();

    } else if ((s32)r30 == (s32)0x0) {
        r4 = r27;
        r3 = 0x37;
        fn_80132A38();
        r4 = r27;
        r3 = 0x4d;
        fn_80132A38();

    } else {
        r4 = (u32)&lbl_803A9A60;
        r3 = 0x37;
        r4 = (u32)&lbl_803A9A60;
        r27 = r4 + 0x3c4;
        r4 = r27;
        fn_80132A38();
        r4 = r27;
        r3 = 0x4d;
        fn_80132A38();
    }
    if ((s32)r31 == (s32)0x0) {
        if ((s32)r30 == (s32)0x0) {
            r4 = *(u8*)((u8*)r28 + 0x8B);
            r0 = -0x100;
            r5 = *(s16*)((u8*)r29 + 0x54);
            r3 = 0x0;
            r6 = *(s16*)((u8*)r29 + 0x56);
            r7 = r4 | r0;
            r4 = 0x0;
            r8 = 0x30e2;
            fn_800FBB34();
            return;
        }
        r5 = *(u8*)((u8*)r28 + 0x8B);
        r0 = -0x100;
        r3 = 0x0;
        r4 = 0x0;
        r5 = r5 | r0;
        r6 = 0xce;
        fn_800FB680();
        return;
    }
    r3 = r30;
    fn_8025DA18();
    r4 = r3 & 0xFFFF;
    r3 = 0x34;
    r4 = r4 + 0x1;
    fn_80132A38();
    if ((s32)r30 < (s32)0x2) {
        r4 = *(u8*)((u8*)r28 + 0x8B);
        r0 = -0x100;
        r5 = *(s16*)((u8*)r29 + 0x54);
        r3 = 0x0;
        r6 = *(s16*)((u8*)r29 + 0x56);
        r7 = r4 | r0;
        r4 = 0x0;
        r8 = 0x30e9;
        fn_800FBB34();
        r4 = *(u8*)((u8*)r28 + 0x8B);
        r0 = -0x100;
        r5 = *(s16*)((u8*)r29 + 0x54);
        r3 = 0x0;
        r6 = *(s16*)((u8*)r29 + 0x56);
        r7 = r4 | r0;
        r4 = 0x16;
        r8 = 0x30e8;
        fn_800FBB34();
        return;
    }
    r5 = *(u8*)((u8*)r28 + 0x8B);
    r0 = -0x100;
    r3 = 0x0;
    r4 = 0x0;
    r5 = r5 | r0;
    r6 = 0x30e7;
    fn_800FB680();

    return;
}


/* 0x80061A2C | size: 0x148 */
s32 fn_80061A2C(void) {
    extern void fn_80061240();
    extern void fn_80061D34();
    extern void fn_800FE4D4();
    extern void fn_800FE6D0();
    extern void fn_801040F0();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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
    f32 f0 = 0.0f;

    
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r31 = r7;
    r8 = (u32)&lbl_803A9A60;
    r8 = (u32)&lbl_803A9A60;
    r0 = *(u32*)((u8*)r8 + 0x4);
    if ((s32)r0 != (s32)0x1) {
        if ((s32)r0 >= (s32)0x1) return;
        if ((s32)r0 < (s32)0x0) {
            return;
        }
        r0 = *(u8*)((u8*)r28 + 0x4);
        r0 = r0 & 0xFFFFFFFD;
        r0 = (s8)r0;
        *(u8*)((u8*)r28 + 0x4) = r0;
        return;
    }
    fn_80061D34();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r4 = r29 * 0xb4;
        r3 = (u32)&lbl_803A9A60;
        r6 = *(s16*)((u8*)r28 + 0x6);
        r5 = (u32)&lbl_802EF0A8;
        r0 = (u32)&lbl_803A9A60;
        r3 = r0 + r4;
        r0 = r30 << 2;
        r4 = (u32)&lbl_802EF0A8;
        r3 = r3 + r0;
        f0 = *(f32*)((u8*)r3 + 0x94);
        r0 = r6 * 0x1c;
        f0 = (f64)(s32)f0;
        r3 = r4 + r0;
        r3 = *(s16*)((u8*)r3 + 0x2);
        *(f64*)(sp + 0x8) = f0;
        r0 = r3 + r0;
        r0 = (s16)r0;
        *(u16*)((u8*)r28 + 0x50) = r0;
        r5 = *(s16*)((u8*)r27 + 0x84);
        r3 = *(s16*)((u8*)r28 + 0x50);
        r4 = *(s16*)((u8*)r27 + 0x86);
        r0 = *(s16*)((u8*)r28 + 0x52);
        r3 = r5 + r3;
        r3 = (s16)r3;
        r0 = r4 + r0;
        r4 = (s16)r0;
        fn_800FE6D0();
        fn_800FE4D4();
        r0 = *(u8*)((u8*)r28 + 0x4);
        r5 = r27;
        r3 = 0x0;
        r4 = 0x0;
        r0 = r0 & 0xFFFFFFFD;
        r6 = 0x314;
        r0 = (s8)r0;
        r7 = 0x0;
        *(u8*)((u8*)r28 + 0x4) = r0;
        fn_801040F0();
        r3 = r27;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        r7 = r31;
        fn_80061240();
        return;
    }
    r0 = *(u8*)((u8*)r28 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r28 + 0x4) = r0;

    return;
}


/* 0x80061B74 | size: 0x48 */
void fn_80061B74(void) {
    s32 state;
    u8 *base;

    base = (u8 *)&lbl_803A9A60;
    state = *(s32 *)(base + 0x4);
    if (state == 0 || state == 1) {
        *(u8 *)(base + 0x4) = (s8)(*(u8 *)(base + 0x4) & ~0x02);
    }
}


/* 0x80061BBC | size: 0x178 */
s32 fn_80061BBC(void) {
    extern void fn_80061D34();
    extern void fn_800FE4D4();
    extern void fn_800FE6D0();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    
    r25 = r3;
    r26 = r4;
    r27 = r5;
    r28 = r6;
    r29 = r7;
    r3 = (u32)&lbl_803A9A60;
    r5 = (u32)&lbl_803A9A60;
    r0 = *(u32*)((u8*)r5 + 0x4);
    if ((s32)r0 != (s32)0x1) {
        if ((s32)r0 >= (s32)0x1) return;
        if ((s32)r0 < (s32)0x0) {
            return;
        }
        r0 = *(u8*)((u8*)r26 + 0x4);
        r0 = r0 & 0xFFFFFFFD;
        r0 = (s8)r0;
        *(u8*)((u8*)r26 + 0x4) = r0;
        return;
    }
    r0 = r27 * 0xb4;
    r4 = *(s16*)((u8*)r26 + 0x6);
    r3 = (u32)&lbl_802EF0A8;
    r31 = r28 << 2;
    r5 = r5 + r0;
    r0 = (u32)&lbl_802EF0A8;
    r30 = r5 + 0x58;
    r3 = r30 + r31;
    f0 = *(f32*)((u8*)r3 + 0x3C);
    r3 = r4 * 0x1c;
    f0 = (f64)(s32)f0;
    r3 = r0 + r3;
    r3 = *(s16*)((u8*)r3 + 0x2);
    *(f64*)(sp + 0x8) = f0;
    r0 = r3 + r0;
    r0 = (s16)r0;
    *(u16*)((u8*)r26 + 0x50) = r0;
    r5 = *(s16*)((u8*)r25 + 0x84);
    r3 = *(s16*)((u8*)r26 + 0x50);
    r4 = *(s16*)((u8*)r25 + 0x86);
    r0 = *(s16*)((u8*)r26 + 0x52);
    r3 = r5 + r3;
    r3 = (s16)r3;
    r0 = r4 + r0;
    r4 = (s16)r0;
    fn_800FE6D0();
    fn_800FE4D4();
    r3 = (u32)&lbl_803A9A60;
    r3 = (u32)&lbl_803A9A60;
    r0 = *(u32*)((u8*)r3 + 0x38);
    if ((s32)r0 >= (s32)0x5) {
        r3 = r25;
        r4 = r26;
        r5 = r27;
        r6 = r28;
        r7 = r29;
        fn_80061D34();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x0) {
            r3 = r30 + r31;
            f1 = *(f32*)&lbl_8047BF60;
            f0 = *(f32*)((u8*)r3 + 0x84);
            if (f1 == f0) {
                r0 = *(u8*)((u8*)r26 + 0x4);
                r0 = r0 | 0x2;
                r0 = (s8)r0;
                *(u8*)((u8*)r26 + 0x4) = r0;
                return;
            }
            r0 = *(u8*)((u8*)r26 + 0x4);
            r0 = r0 & 0xFFFFFFFD;
            r0 = (s8)r0;
            *(u8*)((u8*)r26 + 0x4) = r0;
            return;
        }
        r0 = *(u8*)((u8*)r26 + 0x4);
        r0 = r0 & 0xFFFFFFFD;
        r0 = (s8)r0;
        *(u8*)((u8*)r26 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r26 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r26 + 0x4) = r0;

    return;
}


/* 0x80061D34 | size: 0x238 */
void fn_80061D34(void) {
    extern void fn_8025D808();
    extern void fn_8025D89C();
    extern void fn_8025DA88();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    
    r30 = r4;
    r24 = r5;
    r25 = r6;
    r26 = r7;
    fn_8025DA88();
    r4 = (u32)&lbl_803A9A60;
    r28 = r3;
    r29 = (u32)&lbl_803A9A60;
    r31 = 0x1;
    r0 = *(u32*)((u8*)r29 + 0x4);
    if ((s32)r0 != (s32)0x1) {
        if ((s32)r0 >= (s32)0x1) goto L_80061DA0;
        if ((s32)r0 < (s32)0x0) {
            goto L_80061DA0;
        }
        r3 = r24;
        fn_8025D89C();
        r27 = r3 & 0xFFFF;
        goto L_80061DA0;
    }
    r3 = r24;
    fn_8025D808();
    r27 = r3 & 0xFFFF;
    L_80061DA0: ;
    r0 = *(u32*)((u8*)r29 + 0x4);
    if ((s32)r0 == (s32)0x0) {
        if ((s32)r26 == (s32)0x2) {
            if ((s32)r28 == (s32)0x2) { r3 = r31; return; }
            r0 = *(u8*)((u8*)r30 + 0x4);
            r31 = 0x0;
            r0 = r0 & 0xFFFFFFFD;
            r0 = (s8)r0;
            *(u8*)((u8*)r30 + 0x4) = r0;
            r3 = r31;
            return;
        }
        if ((s32)r26 == (s32)0x0) {
            if ((s32)r27 >= (s32)0x4) {
                r0 = *(u8*)((u8*)r30 + 0x4);
                r31 = 0x0;
                r0 = r0 & 0xFFFFFFFD;
                r0 = (s8)r0;
                *(u8*)((u8*)r30 + 0x4) = r0;
                r3 = r31;
                return;
            }
            if ((s32)r28 != (s32)0x2) { r3 = r31; return; }
            r0 = *(u8*)((u8*)r30 + 0x4);
            r31 = 0x0;
            r0 = r0 & 0xFFFFFFFD;
            r0 = (s8)r0;
            *(u8*)((u8*)r30 + 0x4) = r0;
            r3 = r31;
            return;
        }
        if ((s32)r27 < (s32)0x4) {
            r0 = *(u8*)((u8*)r30 + 0x4);
            r31 = 0x0;
            r0 = r0 & 0xFFFFFFFD;
            r0 = (s8)r0;
            *(u8*)((u8*)r30 + 0x4) = r0;
            r3 = r31;
            return;
        }
        if ((s32)r28 != (s32)0x2) { r3 = r31; return; }
        r0 = *(u8*)((u8*)r30 + 0x4);
        r31 = 0x0;
        r0 = r0 & 0xFFFFFFFD;
        r0 = (s8)r0;
        *(u8*)((u8*)r30 + 0x4) = r0;
        r3 = r31;
        return;
    }
    if ((s32)r26 == (s32)0x2) {
        if ((s32)r28 != (s32)0x2) {
            r0 = *(u8*)((u8*)r30 + 0x4);
            r31 = 0x0;
            r0 = r0 & 0xFFFFFFFD;
            r0 = (s8)r0;
            *(u8*)((u8*)r30 + 0x4) = r0;
        }
        if ((s32)r27 > (s32)r25) { r3 = r31; return; }
        r31 = 0x0;
        r3 = r31;
        return;
    }
    if ((s32)r26 == (s32)0x0) {
        if ((s32)r27 >= (s32)0x4) {
            r0 = *(u8*)((u8*)r30 + 0x4);
            r31 = 0x0;
            r0 = r0 & 0xFFFFFFFD;
            r0 = (s8)r0;
            *(u8*)((u8*)r30 + 0x4) = r0;
            r3 = r31;
            return;
        }
        if ((s32)r28 == (s32)0x2) {
            r0 = *(u8*)((u8*)r30 + 0x4);
            r31 = 0x0;
            r0 = r0 & 0xFFFFFFFD;
            r0 = (s8)r0;
            *(u8*)((u8*)r30 + 0x4) = r0;
            r3 = r31;
            return;
        }
        if ((s32)r27 > (s32)r25) { r3 = r31; return; }
        r0 = *(u8*)((u8*)r30 + 0x4);
        r31 = 0x0;
        r0 = r0 & 0xFFFFFFFD;
        r0 = (s8)r0;
        *(u8*)((u8*)r30 + 0x4) = r0;
        r3 = r31;
        return;
    }
    if ((s32)r27 < (s32)0x4) {
        r0 = *(u8*)((u8*)r30 + 0x4);
        r31 = 0x0;
        r0 = r0 & 0xFFFFFFFD;
        r0 = (s8)r0;
        *(u8*)((u8*)r30 + 0x4) = r0;
        r3 = r31;
        return;
    }
    if ((s32)r28 == (s32)0x2) {
        r0 = *(u8*)((u8*)r30 + 0x4);
        r31 = 0x0;
        r0 = r0 & 0xFFFFFFFD;
        r0 = (s8)r0;
        *(u8*)((u8*)r30 + 0x4) = r0;
        r3 = r31;
        return;
    }
    if ((s32)r27 > (s32)r25) { r3 = r31; return; }
    r0 = *(u8*)((u8*)r30 + 0x4);
    r31 = 0x0;
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r30 + 0x4) = r0;

    r3 = r31;
    return;
}


/* 0x80061F6C | size: 0x318 */
s32 fn_80061F6C(void) {
    extern void fn_8005FFE4();
    extern void fn_80062334();
    extern void fn_80068F84();
    extern void fn_800697C4();
    extern void fn_800F9E70();
    extern void fn_800FA280();
    extern void fn_8011F15C();
    extern void fn_8011F188();
    extern void fn_8012AC54();
    extern void fn_80165A20();
    extern void fn_801FBD28();
    extern void fn_801FBD58();
    extern void fn_801FCC64();
    extern void fn_801FCCC4();
    extern void fn_8025D28C();
    extern void fn_8025D914();
    extern void fn_8025D938();
    extern void fn_8025D9A8();
    extern void fn_8025DA88();
    extern void fn_8025DBB0();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    
    r30 = r3;
    r28 = r4;
    fn_8025DA88();
    r31 = r3;
    r29 = 0x1;
    fn_8025DA88();
    if ((s32)r31 == (s32)0x2) {
        if ((s32)r3 == (s32)0x2) goto L_80061FB8;
        r29 = 0x0;
        goto L_80061FB8;
    }
    if ((s32)r3 != (s32)0x2) goto L_80061FB8;
    r29 = 0x0;
    L_80061FB8: ;
    r0 = r29 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0x1;
        fn_8025D914();
        fn_8012AC54();
        r4 = (u32)&lbl_803A9A60;
        r4 = (u32)&lbl_803A9A60;
        r27 = r4 + 0x3c4;
        r4 = r3;
        if ((u32)r27 != (u32)0x0) {
            r3 = r27;
            fn_800F9E70();
        } else {

            r3 = 0x1;
            fn_800FA280();
            r4 = r3;
            r3 = r27;
            fn_800F9E70();
        }
        r3 = 0x1;
        fn_8025D28C();
        r3 = r3 & 0xFFFF;
        fn_801FCCC4();
        fn_801FCC64();
        fn_801FBD58();
        fn_801FBD28();
        r4 = (u32)&lbl_803A9A60;
        r4 = (u32)&lbl_803A9A60;
        *(u32*)((u8*)r4 + 0x3DC) = r3;
    }
    fn_8025DBB0();
    r4 = (u32)&lbl_803A9A60;
    r0 = 0x0;
    r4 = (u32)&lbl_803A9A60;
    *(u32*)((u8*)r4 + 0x3C0) = r3;
    *(u32*)((u8*)r4 + 0x0) = r30;
    *(u32*)((u8*)r4 + 0x4) = r28;
    *(u32*)((u8*)r4 + 0x38) = r0;
    fn_8005FFE4();
    fn_80062334();
    r3 = (u32)&lbl_803A9A60;
    f0 = *(f32*)&lbl_8047BF60;
    r3 = (u32)&lbl_803A9A60;
    *(f32*)((u8*)r3 + 0x3B4) = f0;
    if ((s32)r28 != (s32)0x1) {
        if ((s32)r28 >= (s32)0x1) return;
        if ((s32)r28 < (s32)0x0) {
            return;
        }
        fn_80068F84();
        fn_800697C4();
        fn_8025DBB0();
        r31 = r3;
        fn_8025D9A8();
        if ((s32)r3 != (s32)0x2) {
            if ((s32)r3 < (s32)0x2) {
                if ((s32)r3 != (s32)0x0) {
                    if ((s32)r3 < (s32)0x0) {
                        goto L_80062144;
                    }
                    if ((s32)r3 >= (s32)0x4) goto L_80062144;
                    goto L_80062130;
                    }
                r4 = (u32)&lbl_802ED958;
                r3 = (u32)&lbl_803A9A60;
                r0 = r31 << 2;
                r4 = (u32)&lbl_802ED958;
                r3 = (u32)&lbl_803A9A60;
                r0 = *(u32*)(r4 + r0);
                *(u32*)((u8*)r3 + 0x3BC) = r0;
                goto L_80062154;
                    }
            r3 = (0x6666 << 16);
            r4 = (u32)&lbl_802ED978;
            r0 = r3 + 0x6667;
            r3 = (u32)&lbl_803A9A60;
            r0 = (s32)((s64)r0 * (s64)r31 >> 32);
            r4 = (u32)&lbl_802ED978;
            r3 = (u32)&lbl_803A9A60;
            r0 = (s32)r0 >> 2;
            r5 = (u32)r0 >> 31;
            r0 = r0 + r5;
            r0 = r0 * 0xa;
            r0 = r31 - r0;
            r0 = r0 << 2;
            r0 = *(u32*)(r4 + r0);
            *(u32*)((u8*)r3 + 0x3BC) = r0;
            goto L_80062154;
        }
        r3 = (u32)&lbl_803A9A60;
        r0 = 0x3cd;
        r3 = (u32)&lbl_803A9A60;
        *(u32*)((u8*)r3 + 0x3BC) = r0;
        goto L_80062154;
        L_80062130: ;
        r3 = (u32)&lbl_803A9A60;
        r0 = 0x3cd;
        r3 = (u32)&lbl_803A9A60;
        *(u32*)((u8*)r3 + 0x3BC) = r0;
        goto L_80062154;
        L_80062144: ;
        r3 = (u32)&lbl_803A9A60;
        r0 = 0x3cd;
        r3 = (u32)&lbl_803A9A60;
        *(u32*)((u8*)r3 + 0x3BC) = r0;
        L_80062154: ;
        r3 = (u32)&lbl_803A9A60;
        r4 = 0x0;
        r3 = (u32)&lbl_803A9A60;
        r5 = 0xff;
        r3 = *(u32*)((u8*)r3 + 0x3BC);
        fn_80165A20();
        return;
    }
    fn_80068F84();
    fn_800697C4();
    r3 = (u32)&lbl_803A9A60;
    r29 = 0x0;
    r27 = (u32)&lbl_803A9A60;
    do {
        r28 = r27 + 0x58;
        r30 = 0x0;
        do {
            r3 = r29;
            r4 = r30;
            fn_8025D938();
            r31 = r3;
            if ((u32)r31 != (u32)0x0) {
                fn_8011F15C();
                r3 = r3 & 0xFFFF;
                r0 = (0x4330 << 16);
                r3 = r31;
                f1 = *(f64*)&lbl_8047BF88;
                *(u32*)(sp + 0x8) = r0;
                f0 = *(f64*)(sp + 0x8);
                f0 = f0 - f1;
                *(f32*)((u8*)r28 + 0x6C) = f0;
                fn_8011F188();
                r3 = r3 & 0xFFFF;
                r0 = (0x4330 << 16);
                r3 = r31;
                f1 = *(f64*)&lbl_8047BF88;
                *(u32*)(sp + 0x10) = r0;
                f0 = *(f64*)(sp + 0x10);
                f0 = f0 - f1;
                *(f32*)((u8*)r28 + 0x84) = f0;
                fn_8011F15C();
                r3 = r3 & 0xFFFF;
                r0 = (0x4330 << 16);
                f1 = *(f64*)&lbl_8047BF88;
                *(u32*)(sp + 0x18) = r0;
                f0 = *(f64*)(sp + 0x18);
                f0 = f0 - f1;
                *(f32*)((u8*)r28 + 0x9C) = f0;
            } else {

                f1 = *(f32*)&lbl_8047BF60;
                f0 = *(f32*)&lbl_8047BFAC;
                *(f32*)((u8*)r28 + 0x6C) = f1;
                *(f32*)((u8*)r28 + 0x84) = f1;
                *(f32*)((u8*)r28 + 0x9C) = f0;
            }
            r28 = r28 + 0x4;
            r30 = r30 + 0x1;
        } while ((s32)r30 < (s32)0x6);
        r27 = r27 + 0xb4;
        r29 = r29 + 0x1;
    } while ((s32)r29 < (s32)0x4);
    r3 = (u32)&lbl_803A9A60;
    r0 = 0x1e;
    r4 = (u32)&lbl_803A9A60;
    r3 = 0x1e;
    *(u32*)((u8*)r4 + 0x3BC) = r0;
    r4 = 0x0;
    r5 = 0xff;
    fn_80165A20();

    return;
}


/* 0x80062284 | size: 0xB0 */
s32 fn_80062284(void) {
    extern void fn_80123FBC();
    extern void fn_8012640C();
    extern void fn_8025D808();
    extern void fn_8025D938();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    
    r28 = r3;
    fn_8025D808();
    r29 = r3 & 0xFFFF;
    r31 = 0x0;
    while ((s32)r31 < (s32)r29) {

        r3 = r28;
        r4 = r31;
        fn_8025D938();
        r30 = r3;
        if ((u32)r30 != (u32)0x0) {
            fn_80123FBC();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x0) {
                r3 = r30;
                r4 = 0x0;
                r5 = 0x7b;
                r6 = 0x0;
                fn_8012640C();
                r0 = r3 & 0xFF;
                if ((u32)r0 == (u32)0x1) {
                    r3 = 0x0;
                    return;
        }
        }
        }
        r31 = r31 + 0x1;

    }
    r3 = 0x1;

    return;
}


/* 0x80062334 | size: 0x398 */
void fn_80062334(void) {
    extern void fn_8025DA88();
    u8 sp[0x70];
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
    u32 r26 = 0;
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
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    
    fn_8025DA88();
    r4 = (u32)&lbl_803A9E40;
    f5 = *(f32*)&lbl_8047BFA4;
    r7 = (u32)&lbl_803A9E40;
    f4 = *(f32*)&lbl_8047BFA8;
    f3 = *(f32*)&lbl_8047BFB0;
    r6 = r7 + 0x18;
    f2 = *(f32*)&lbl_8047BFB4;
    r5 = r7 + 0x30;
    f1 = *(f32*)&lbl_8047BF68;
    r0 = r7 + 0x48;
    f0 = *(f32*)&lbl_8047BFB8;
    r4 = (u32)&lbl_803A9A60;
    r11 = (u32)&lbl_803A9A60;
    r8 = (u32)sp + 0x8;
    r9 = (u32)sp + 0x34;
    r7 = r11;
    r10 = (u32)sp + 0x18;
    r31 = 0x0;
    *(u32*)(sp + 0x14) = r0;
    *(f32*)(sp + 0x34) = f5;
    *(f32*)(sp + 0x38) = f4;
    *(f32*)(sp + 0x3C) = f3;
    *(f32*)(sp + 0x40) = f2;
    *(f32*)(sp + 0x44) = f1;
    *(f32*)(sp + 0x48) = f0;
    *(f32*)(sp + 0x18) = f0;
    *(f32*)(sp + 0x1C) = f1;
    *(f32*)(sp + 0x20) = f2;
    *(f32*)(sp + 0x24) = f3;
    *(f32*)(sp + 0x28) = f4;
    *(f32*)(sp + 0x2C) = f5;
    do {
        r30 = r7 + 0x58;
        if ((s32)r3 == (s32)0x2) {
            r4 = r30;
            r5 = r30;
            r6 = r9;
            r12 = r10;
            r0 = 0x6;
            ctr_fn = (void(*)(void))r0;
            do {
                r0 = *(u32*)((u8*)r11 + 0x4);
                if ((s32)r0 != (s32)0x1) {
                    if ((s32)r0 >= (s32)0x1) goto L_80062434;
                    if ((s32)r0 < (s32)0x0) {
                        goto L_80062434;
                    }
                    r0 = 0x3;
                    f0 = *(f32*)&lbl_8047BF60;
                    *(u16*)((u8*)r4 + 0x0) = r0;
                    *(f32*)((u8*)r5 + 0xC) = f0;
                    goto L_80062434;
                }
                r0 = 0x0;
                f0 = *(f32*)&lbl_8047BF60;
                *(u16*)((u8*)r4 + 0x0) = r0;
                *(f32*)((u8*)r5 + 0xC) = f0;
                L_80062434: ;
                if ((s32)r31 < (s32)0x2) {
                    f0 = *(f32*)&lbl_8047BFBC;
                    f1 = *(f32*)&lbl_8047BF60;
                    *(f32*)((u8*)r5 + 0x3C) = f0;
                    f0 = *(f32*)((u8*)r6 + 0x0);
                    *(f32*)((u8*)r5 + 0x54) = f1;
                    *(f32*)((u8*)r5 + 0x24) = f0;
                } else {

                    f0 = *(f32*)&lbl_8047BFC0;
                    f1 = *(f32*)&lbl_8047BF60;
                    *(f32*)((u8*)r5 + 0x3C) = f0;
                    f0 = *(f32*)((u8*)r12 + 0x0);
                    *(f32*)((u8*)r5 + 0x54) = f1;
                    *(f32*)((u8*)r5 + 0x24) = f0;
                }
                r4 = r4 + 0x2;
                r5 = r5 + 0x4;
                r6 = r6 + 0x4;
                r12 = r12 + 0x4;
            } while (--ctr != 0);
            goto L_800625A4;
        }
        r26 = (u32)r31 >> 31;
        r0 = r31 & 0x1;
        r0 = r0 ^ r26;
        r12 = 0x0;
        r4 = r12;
        r5 = r30;
        r6 = r30;
        r0 = r0 - r26;
        r29 = 0x6;
        ctr_fn = (void(*)(void))r29;
        do {
            r26 = *(u32*)((u8*)r11 + 0x4);
            r27 = *(u32*)((u8*)r8 + 0x0);
            r28 = *(u32*)(r27 + r4);
            if ((s32)r26 != (s32)0x1) {
                if ((s32)r26 >= (s32)0x1) goto L_800624F8;
                if ((s32)r26 < (s32)0x0) {
                    goto L_800624F8;
                }
                r26 = 0x3;
                f0 = *(f32*)&lbl_8047BF60;
                *(u16*)((u8*)r5 + 0x0) = r26;
                *(f32*)((u8*)r6 + 0xC) = f0;
                goto L_800624F8;
            }
            r26 = 0x0;
            f0 = *(f32*)&lbl_8047BF60;
            *(u16*)((u8*)r5 + 0x0) = r26;
            *(f32*)((u8*)r6 + 0xC) = f0;
            L_800624F8: ;
            if ((s32)r0 != (s32)0x0) {
                r26 = (0x5555 << 16);
                f0 = *(f32*)&lbl_8047BFC0;
                r27 = r26 + 0x5556;
                r26 = r28 << 2;
                r29 = (s32)((s64)r27 * (s64)r12 >> 32);
                *(f32*)((u8*)r6 + 0x3C) = f0;
                f0 = *(f32*)&lbl_8047BF60;
                r28 = (u32)sp + 0x18;
                r26 = r26 + 0x24;
                *(f32*)((u8*)r6 + 0x54) = f0;
                r27 = (u32)r29 >> 31;
                r27 = r29 + r27;
                r27 = r27 * 0x3;
                r27 = r12 - r27;
                r27 = r27 << 2;
                r27 = r27 + 0xc;
                f0 = *(f32*)(r28 + r27);
                *(f32*)(r30 + r26) = f0;
            } else {

                r27 = (0x5555 << 16);
                f0 = *(f32*)&lbl_8047BFBC;
                r27 = r27 + 0x5556;
                r29 = r28 << 2;
                r26 = (s32)((s64)r27 * (s64)r12 >> 32);
                *(f32*)((u8*)r6 + 0x3C) = f0;
                f0 = *(f32*)&lbl_8047BF60;
                r28 = (u32)sp + 0x34;
                r29 = r29 + 0x24;
                *(f32*)((u8*)r6 + 0x54) = f0;
                r27 = (u32)r26 >> 31;
                r27 = r26 + r27;
                r27 = r27 * 0x3;
                r27 = r12 - r27;
                r27 = r27 << 2;
                f0 = *(f32*)(r28 + r27);
                *(f32*)(r30 + r29) = f0;
            }
            r4 = r4 + 0x4;
            r5 = r5 + 0x2;
            r6 = r6 + 0x4;
            r12 = r12 + 0x1;
        } while (--ctr != 0);
        L_800625A4: ;
        r7 = r7 + 0xb4;
        r8 = r8 + 0x4;
        r31 = r31 + 0x1;
    } while ((s32)r31 < (s32)0x4);
    r5 = r11;
    r6 = 0x0;
    r0 = 0x4;
    ctr_fn = (void(*)(void))r0;
    do {
        r7 = r5 + 0x328;
        if ((s32)r3 == (s32)0x2) {
            if ((s32)r6 < (s32)0x2) {
                f0 = *(f32*)&lbl_8047BFBC;
                f1 = *(f32*)&lbl_8047BF60;
                *(f32*)((u8*)r7 + 0x4) = f0;
                f0 = *(f32*)&lbl_8047BFC4;
                *(f32*)((u8*)r7 + 0x8) = f1;
                *(f32*)((u8*)r7 + 0x0) = f0;
                goto L_80062660;
            }
            f0 = *(f32*)&lbl_8047BFC0;
            f1 = *(f32*)&lbl_8047BF60;
            *(f32*)((u8*)r7 + 0x4) = f0;
            f0 = *(f32*)&lbl_8047BFC4;
            *(f32*)((u8*)r7 + 0x8) = f1;
            *(f32*)((u8*)r7 + 0x0) = f0;
            goto L_80062660;
        }
        r4 = (u32)r6 >> 31;
        r0 = r6 & 0x1;
        r0 = r0 ^ r4;
        r0 = r0 - r4;
        if ((s32)r0 != (s32)0x0) {
            f0 = *(f32*)&lbl_8047BFC0;
            f1 = *(f32*)&lbl_8047BF60;
            *(f32*)((u8*)r7 + 0x4) = f0;
            f0 = *(f32*)&lbl_8047BFC4;
            *(f32*)((u8*)r7 + 0x8) = f1;
            *(f32*)((u8*)r7 + 0x0) = f0;
            goto L_80062660;
        }
        f0 = *(f32*)&lbl_8047BFBC;
        f1 = *(f32*)&lbl_8047BF60;
        *(f32*)((u8*)r7 + 0x4) = f0;
        f0 = *(f32*)&lbl_8047BFC4;
        *(f32*)((u8*)r7 + 0x8) = f1;
        *(f32*)((u8*)r7 + 0x0) = f0;
        L_80062660: ;
        r5 = r5 + 0xc;
        r6 = r6 + 0x1;
    } while (--ctr != 0);
    f4 = *(f32*)&lbl_8047BF70;
    r3 = (u32)&lbl_803A9A60;
    f3 = *(f32*)&lbl_8047BF90;
    r3 = (u32)&lbl_803A9A60;
    f2 = *(f32*)&lbl_8047BF60;
    r0 = 0x0;
    f1 = *(f32*)&lbl_8047BFBC;
    f0 = *(f32*)&lbl_8047BFC0;
    *(u8*)((u8*)r3 + 0x368) = r0;
    *(f32*)((u8*)r11 + 0x358) = f4;
    *(f32*)((u8*)r11 + 0x35C) = f3;
    *(f32*)((u8*)r3 + 0x360) = f4;
    *(f32*)((u8*)r3 + 0x364) = f3;
    *(f32*)((u8*)r3 + 0x54) = f2;
    *(f32*)((u8*)r3 + 0x4C) = f1;
    *(f32*)((u8*)r3 + 0x44) = f2;
    *(f32*)((u8*)r3 + 0x50) = f2;
    *(f32*)((u8*)r3 + 0x48) = f0;
    *(f32*)((u8*)r3 + 0x40) = f2;
    return;
}


/* 0x800626CC | size: 0x168 */
s32 fn_800626CC(void) {
    extern void fn_800F92D4();
    extern void fn_8017B000();
    extern void fn_8025D2D4();
    extern void fn_8025D364();
    extern void fn_8025DA88();
    extern void fn_800626CC();
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

    
    r27 = r4;
    r30 = 0x1;
    if ((u32)r27 != (u32)0x0) {
        r3 = *(u32*)((u8*)r27 + 0x4);
        fn_800F92D4();
        *(u32*)((u8*)r27 + 0x0) = r3;
    }
    r3 = (u32)&lbl_803A9A60;
    r31 = (u32)&lbl_803A9A60;
    do {
        r3 = *(u32*)((u8*)r31 + 0x2C);
        r0 = *(u32*)((u8*)r31 + 0x30);
        if ((s32)r3 == (s32)r0) {
            r30 = 0x0;
            goto L_80062818;
        }
        fn_8025DA88();
        if ((s32)r3 != (s32)0x2) {
            r0 = *(u32*)((u8*)r31 + 0x2C);
            r3 = (u32)r0 >> 31;
            r0 = r0 & 0x1;
            r0 = r0 ^ r3;
            r0 = r0 - r3;
            if ((s32)r0 != (s32)0x0) {
                r3 = 0x1;
                r4 = 0x0;
                fn_8025D364();
                r28 = r3;
                goto L_80062790;
            }
            r3 = 0x0;
            r4 = 0x1;
            fn_8025D364();
            r28 = r3;
            goto L_80062790;
        }
        r3 = *(u32*)((u8*)r31 + 0x2C);
        if ((s32)r3 < (s32)0x2) {
            r4 = 0x1;
            fn_8025D2D4();
            r28 = r3;
            goto L_80062790;
        }
        r4 = 0x0;
        fn_8025D2D4();
        r28 = r3;
        L_80062790: ;
        fn_8025DA88();
        if ((s32)r3 != (s32)0x2) {
            r27 = 0x5c3;
        } else {

            r27 = 0x5c4;
        }
        r4 = *(u32*)((u8*)r31 + 0x2C);
        r3 = (u32)&lbl_803A9A60;
        r0 = (u32)&lbl_803A9A60;
        r3 = r4 << 3;
        r3 = r0 + r3;
        r29 = r3 + 0xc;
        *(u32*)((u8*)r29 + 0x4) = r28;
        r3 = *(u32*)((u8*)r29 + 0x4);
        fn_800F92D4();
        *(u32*)((u8*)r29 + 0x0) = r3;
        r0 = *(u32*)((u8*)r29 + 0x0);
        if ((u32)r0 != (u32)0x0) {
            r3 = *(u32*)((u8*)r31 + 0x2C);
            r0 = r3 + 0x1;
            *(u32*)((u8*)r31 + 0x2C) = r0;
            goto L_80062818;
        }
        r4 = (u32)fn_800626CC;
        r3 = r27;
        r5 = (u32)fn_800626CC;
        r6 = r29;
        r4 = r28;
        r7 = r28;
        fn_8017B000();
        r3 = *(u32*)((u8*)r31 + 0x2C);
        r30 = 0x0;
        r0 = r3 + 0x1;
        *(u32*)((u8*)r31 + 0x2C) = r0;
        L_80062818: ;
    } while ((s32)r30 != (s32)0x0);
    return;
}


/* 0x80062834 | size: 0x114 */
void fn_80062834(void) {
    extern void fn_800F915C();
    extern void fn_800F9210();
    extern void fn_8017B1CC();
    extern void fn_8025DA88();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    
    fn_8025DA88();
    if ((s32)r3 != (s32)0x2) {
        r29 = 0x5c3;
    } else {

        r29 = 0x5c4;
    }
    r3 = r29;
    fn_8017B1CC();
    r3 = r29;
    fn_800F915C();
    r3 = (u32)&lbl_803A9A60;
    r3 = (u32)&lbl_803A9A60;
    r30 = r3 + 0xc;
    r28 = r30;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2) {
        r3 = (u32)&lbl_803A9A60;
        r0 = 0x4;
        r3 = (u32)&lbl_803A9A60;
        *(u32*)((u8*)r3 + 0x30) = r0;
    } else {

        r3 = (u32)&lbl_803A9A60;
        r0 = 0x2;
        r3 = (u32)&lbl_803A9A60;
        *(u32*)((u8*)r3 + 0x30) = r0;
    }
    r3 = (u32)&lbl_803A9A60;
    r27 = 0x0;
    r31 = (u32)&lbl_803A9A60;
    while (1) {
        r0 = *(u32*)((u8*)r31 + 0x30);
        if ((s32)r27 >= (s32)r0) break;
        r4 = *(u32*)((u8*)r28 + 0x4);
        r3 = r29;
        fn_800F9210();
        r28 = r28 + 0x8;
        r27 = r27 + 0x1;


    }
    r3 = (u32)&lbl_803A9A60;
    r0 = 0x0;
    r3 = (u32)&lbl_803A9A60;
    *(u8*)((u8*)r3 + 0x34) = r0;
    *(u32*)((u8*)r3 + 0x2C) = r0;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2) {
        r0 = 0x4;
        *(u32*)((u8*)r31 + 0x30) = r0;
    } else {

        r0 = 0x2;
        *(u32*)((u8*)r31 + 0x30) = r0;
    }
    r0 = 0x0;
    *(u32*)((u8*)r30 + 0x4) = r0;
    *(u32*)((u8*)r30 + 0x0) = r0;
    *(u32*)((u8*)r30 + 0xC) = r0;
    *(u32*)((u8*)r30 + 0x8) = r0;
    *(u32*)((u8*)r30 + 0x14) = r0;
    *(u32*)((u8*)r30 + 0x10) = r0;
    *(u32*)((u8*)r30 + 0x1C) = r0;
    *(u32*)((u8*)r30 + 0x18) = r0;
    return;
}


/* 0x80062948 | size: 0x16C */
s32 fn_80062948(void) {
    extern void fn_80061028();
    extern void fn_80061F6C();
    extern void fn_80062834();
    extern void fn_80062AB4();
    extern void fn_80063060();
    extern void fn_80069944();
    extern void fn_800F0308();
    extern void fn_80102568();
    extern void fn_8010264C();
    extern void fn_801070F4();
    extern void fn_801080CC();
    extern void fn_8025D788();
    extern void fn_8025D9A8();
    extern void fn_8025DA88();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    
    r30 = r3;
    r4 = 0x1;
    fn_80061F6C();
    fn_8025D9A8();
    r31 = r3;
    fn_8025DA88();

    /* Switch on r31 */
    if ((s32)r31 == 0 || (s32)r31 == 1) {
        /* Case 0/1: process via fn_80063060 */
        r3 = r30;
        fn_80063060();
        r31 = r3;
    } else if ((s32)r31 == 3) {
        /* Case 3: special processing */
        r3 = 0xdf;
        r4 = 0x0;
        fn_8010264C();
        r3 = 0xba;
        r4 = 0x1;
        fn_8010264C();
        r3 = 0x106;
        r4 = 0x1;
        fn_8010264C();
        r0 = *(u32*)((u8*)r30 + 0x4);
        if (((s32)r0 != (s32)0x2) && ((s32)r3 > (s32)0x0)) {
            r3 = r3 + 0x1;
        }
        if ((s32)r3 == 0) {
            fn_8025D788();
            r31 = 0xd1;
        } else {
            r31 = -0x1;
        }
        r3 = 0x106;
        r4 = 0x0;
        r5 = 0x1;
        fn_80102568();
    } else {
        /* Case 2 and default: process via fn_80062AB4 */
        r3 = r30;
        fn_80062AB4();
        r31 = r3;
    }

    /* Common path: wait for animations */
    r3 = 0xdf;
    r4 = 0x1c6;
    fn_801080CC();
    r3 = 0xba;
    r4 = 0x1c6;
    fn_801080CC();

    /* Wait for 0xdf animation to finish */
    do {
        r3 = 0xdf;
        fn_801070F4();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x0) {
            fn_800F0308();
        }
    } while ((u32)r0 != (u32)0x0);

    /* Wait for 0xba animation to finish */
    do {
        r3 = 0xba;
        fn_801070F4();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x0) {
            fn_800F0308();
        }
    } while ((u32)r0 != (u32)0x0);
    fn_80069944();
    fn_80062834();
    r3 = 0x1;
    fn_80061028();
    r3 = 0xdf;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
    r3 = r31;
    return;
}


/* 0x80062AB4 | size: 0x5AC */
s32 fn_80062AB4(void) {
    extern void fn_8008ABA0();
    extern void fn_800F0308();
    extern void fn_800F7C28();
    extern void fn_800F7EF8();
    extern void fn_80102568();
    extern void fn_8010264C();
    extern void fn_801026A4();
    extern void fn_80103CC0();
    extern void fn_801046B8();
    extern void fn_801069FC();
    extern void fn_80106D3C();
    extern void fn_80132A38();
    extern void fn_801EF634();
    extern void fn_8025D2B0();
    extern void fn_8025D788();
    extern void fn_8025D9A8();
    extern void fn_8025D9F0();
    extern void fn_8025DA3C();
    extern void fn_8025DA88();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    
    r28 = r3;
    fn_801EF634();
    r27 = r3;
    r25 = 0x0;
    r3 = 0x0;
    fn_80103CC0();
    r3 = 0xdf;
    r4 = 0x0;
    fn_8010264C();
    r3 = 0xba;
    r4 = 0x1;
    fn_8010264C();
    r0 = r27 & 0xFFFF;
    if ((s32)r0 != (s32)0x1) {
        goto L_80062CC8;
    }
    r3 = 0x1;
    fn_80103CC0();
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2) {
        r3 = 0x0;
        fn_8025D9F0();
        r0 = r3 & 0xFFFF;
        if ((s32)r0 != (s32)0x0) {
            r0 = 0x1;
            goto L_80062B40;
        }
        r0 = 0x0;
        goto L_80062B40;
    }
    r0 = 0x0;
    L_80062B40: ;
    r0 = r0 & 0xFFFF;
    if ((u32)r0 == (u32)0x0) {
        fn_8025DA3C();
        r29 = r3;
        fn_8025D9A8();
        r30 = 0x0;
        while (1) {
            if ((s32)r30 >= (s32)r29) break;
            r3 = r30;
            fn_8025D9F0();
            r27 = r3;
            r3 = r30;
            fn_8025D2B0();
            r31 = r3;
            if ((s32)r31 == (s32)0x0) goto L_80062BAC;
            r0 = r27 & 0xFFFF;

            if ((u32)r0 != (u32)0x1 && (u32)r0 != (u32)0x2) goto L_80062BAC;

            r3 = r31;
            fn_8008ABA0();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x0) goto L_80062BAC;
            goto L_80062BBC;
            L_80062BAC: ;
            r30 = r30 + 0x1;

        }
        r31 = 0x2;
        L_80062BBC: ;
        r4 = r31;
        r3 = 0x30;
        fn_80132A38();
        r3 = 0x2;
        r4 = 0x44dc;
        r5 = 0x1;
        r6 = 0x1;
        fn_80106D3C();
        r3 = 0x1;
        fn_801069FC();
        r25 = 0x1;
        goto L_80062CC8;
    }
    fn_8025DA3C();
    r29 = r3;
    fn_8025D9A8();
    r30 = 0x0;
    while (1) {
        if ((s32)r30 >= (s32)r29) break;
        r3 = r30;
        fn_8025D9F0();
        r27 = r3;
        r3 = r30;
        fn_8025D2B0();
        if ((s32)r3 == (s32)0x0) goto L_80062C40;
        r0 = r27 & 0xFFFF;

        if ((u32)r0 != (u32)0x1 && (u32)r0 != (u32)0x2) goto L_80062C40;

        fn_8008ABA0();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x0) break;
        L_80062C40: ;
        r30 = r30 + 0x1;

    }

    r3 = 0x2;
    r4 = 0x44e7;
    r5 = 0x1;
    r6 = 0x1;
    fn_80106D3C();
    r27 = 0x1;
    do {
        r3 = 0x1;
        fn_800F7EF8();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x0) {
            r3 = 0x1;
            fn_800F7C28();
            if ((s32)r3 == (s32)0x0) {
                r0 = 0x1;
                goto L_80062C9C;
            }
            r0 = 0x0;
            goto L_80062C9C;
        }
        r0 = 0x0;
        L_80062C9C: ;
        r0 = r0 & 0xFF;
        if ((u32)r0 != (u32)0x0) {
            r27 = 0x0;
        } else {

            fn_800F0308();
        }
    } while ((s32)r27 != (s32)0x0);
    r3 = 0x1;
    fn_801069FC();
    r25 = 0x1;
    L_80062CC8: ;
    r0 = r25 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0xb3;
        return;
    }
    r0 = *(u32*)((u8*)r28 + 0x4);
    if ((s32)r0 != (s32)0x2) {
        r25 = 0xd4;
    } else {

        r25 = 0xd5;
    }
    r3 = 0x1;
    fn_80103CC0();
    r3 = 0x2;
    r4 = 0x3c20;
    r5 = 0x1;
    r6 = 0x1;
    fn_80106D3C();
    fn_801046B8();
    r31 = r25 & 0xFFFF;
    r4 = r3;
    r3 = r31;
    r5 = 0x0;
    r6 = 0x8;
    r7 = 0x1;
    r8 = 0x0;
    /* crclr cr1eq */;
    fn_801026A4();
    r0 = *(u32*)((u8*)r28 + 0x4);
    if (((s32)r0 != (s32)0x2) && ((s32)r3 > (s32)0x0)) {

        r3 = r3 + 0x1;
    }
    if ((s32)r3 != (s32)0x1) {
        if ((s32)r3 < (s32)0x1) {
            if ((s32)r3 == (s32)-0x1) goto L_80062D94;
            if ((s32)r3 < (s32)-0x1) {
                goto L_80062D94;
            }
            if ((s32)r3 >= (s32)0x3) goto L_80062D94;
            goto L_80062D8C;
            }
        fn_8025D788();
        r30 = 0xd1;
        goto L_80062D98;
    }
    r30 = 0xb5;
    goto L_80062D98;
    L_80062D8C: ;
    r30 = 0xb3;
    goto L_80062D98;
    L_80062D94: ;
    r30 = -0x1;
    L_80062D98: ;
    r3 = 0x1;
    fn_801069FC();

    if ((s32)r30 != (s32)-0x1 && (s32)r30 != (s32)0xb3) goto L_80063040;

    r25 = 0x0;
    r29 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2) {
        r3 = 0x0;
        fn_8025D9F0();
        r0 = r3 & 0xFFFF;
        if ((s32)r0 != (s32)0x0) {
            r0 = 0x1;
            goto L_80062DEC;
        }
        r0 = 0x0;
        goto L_80062DEC;
    }
    r0 = 0x0;
    L_80062DEC: ;
    r28 = r0 & 0xFFFF;
    do {
        if ((s32)r25 != (s32)0x2) {
            if ((s32)r25 < (s32)0x2) {
                if ((s32)r25 != (s32)0x0) {
                    if ((s32)r25 < (s32)0x0) {
                        goto L_80063038;
                    }
                    if ((s32)r25 != (s32)0x4) {
                        if ((s32)r25 >= (s32)0x4) goto L_80063038;
                        goto L_80062FA4;
                        }
                    if ((s32)r28 == (s32)0x0) {
                        r25 = 0x1;
                        goto L_80063038;
                    }
                    r25 = 0x2;
                    goto L_80063038;
                        }
                r3 = r31;
                r4 = 0x0;
                r5 = 0x1;
                fn_80102568();
                r3 = 0x2;
                r4 = 0x4446;
                r5 = 0x1;
                r6 = 0x1;
                fn_80106D3C();
                r25 = 0x1;
                do {
                    r26 = 0x0;
                    L_80062E60: ;
                    r3 = r26;
                    fn_8025D9F0();
                    r27 = r3;
                    r3 = r26;
                    fn_8025D2B0();
                    if ((s32)r3 == (s32)0x0) goto L_80062EA8;
                    r0 = r27 & 0xFFFF;

                    if ((u32)r0 != (u32)0x1 && (u32)r0 != (u32)0x2) goto L_80062EA8;

                    fn_8008ABA0();
                    r0 = r3 & 0xFF;
                    if ((u32)r0 == (u32)0x0) goto L_80062EA8;
                    r0 = 0x0;
                    goto L_80062EB8;
                    L_80062EA8: ;
                    r26 = r26 + 0x1;
                    if ((s32)r26 < (s32)0x4) goto L_80062E60;
                    r0 = 0x1;
                    L_80062EB8: ;
                    r0 = r0 & 0xFF;
                    if ((u32)r0 != (u32)0x0) {
                        r25 = 0x0;
                    }
                    if ((s32)r25 != (s32)0x0) {
                        fn_800F0308();
                    }
                } while ((s32)r25 != (s32)0x0);
                r3 = 0x1;
                fn_801069FC();
                r25 = 0x4;
                goto L_80063038;
            }
            r3 = r31;
            r4 = 0x0;
            r5 = 0x1;
            fn_80102568();
            r3 = 0x2;
            r4 = 0x4445;
            r5 = 0x1;
            r6 = 0x1;
            fn_80106D3C();
            r25 = 0x1;
            do {
                r26 = 0x0;
                L_80062F18: ;
                r3 = r26;
                fn_8025D9F0();
                r27 = r3;
                r3 = r26;
                fn_8025D2B0();
                if ((s32)r3 == (s32)0x0) goto L_80062F60;
                r0 = r27 & 0xFFFF;

                if ((u32)r0 != (u32)0x1 && (u32)r0 != (u32)0x2) goto L_80062F60;

                fn_8008ABA0();
                r0 = r3 & 0xFF;
                if ((u32)r0 == (u32)0x0) goto L_80062F60;
                r0 = 0x0;
                goto L_80062F70;
                L_80062F60: ;
                r26 = r26 + 0x1;
                if ((s32)r26 < (s32)0x4) goto L_80062F18;
                r0 = 0x1;
                L_80062F70: ;
                r0 = r0 & 0xFF;
                if ((u32)r0 != (u32)0x0) {
                    r25 = 0x0;
                }
                if ((s32)r25 != (s32)0x0) {
                    fn_800F0308();
                }
            } while ((s32)r25 != (s32)0x0);
            r3 = 0x1;
            fn_801069FC();
            r25 = 0x3;
            goto L_80063038;
            L_80062FA4: ;
            r3 = r31;
            r4 = 0x0;
            r5 = 0x1;
            fn_80102568();
            r3 = 0x2;
            r4 = 0x44e2;
            r5 = 0x1;
            r6 = 0x1;
            fn_80106D3C();
            r26 = 0x1;
            do {
                r3 = 0x1;
                fn_800F7EF8();
                r0 = r3 & 0xFF;
                if ((u32)r0 != (u32)0x0) {
                    r3 = 0x1;
                    fn_800F7C28();
                    if ((s32)r3 == (s32)0x0) {
                        r0 = 0x1;
                        goto L_80063004;
                    }
                    r0 = 0x0;
                    goto L_80063004;
                }
                r0 = 0x0;
                L_80063004: ;
                r0 = r0 & 0xFF;
                if ((u32)r0 != (u32)0x0) {
                    r26 = 0x0;
                } else {

                    fn_800F0308();
                }
            } while ((s32)r26 != (s32)0x0);
            r3 = 0x1;
            fn_801069FC();
            r25 = 0x4;
            goto L_80063038;
                    }
        r29 = 0x0;
        L_80063038: ;
    } while ((s32)r29 != (s32)0x0);
    L_80063040: ;
    r0 = 0x0;
    r3 = r30;
    *(u32*)&lbl_8047A5D0 = r0;

    return;
}


/* 0x80063060 | size: 0x750 */
s32 fn_80063060(void) {
    extern void fn_80061028();
    extern void fn_80062284();
    extern void fn_80062834();
    extern void fn_800637B0();
    extern void fn_80069944();
    extern void fn_80088D84();
    extern void fn_800F0308();
    extern void fn_80102568();
    extern void fn_8010264C();
    extern void fn_801026A4();
    extern void fn_80102868();
    extern void fn_801043A4();
    extern void fn_801045A8();
    extern void fn_801046B8();
    extern void fn_801069FC();
    extern void fn_80106D3C();
    extern void fn_801070F4();
    extern void fn_801080CC();
    extern void fn_80129280();
    extern void fn_8012A7C4();
    extern void fn_8012A7DC();
    extern void fn_8012A80C();
    extern void fn_8012A824();
    extern void fn_80132A38();
    extern void fn_80166AB8();
    extern void fn_801906A0();
    extern void fn_801EE398();
    extern void fn_801EF634();
    extern void fn_8025D06C();
    extern void fn_8025D164();
    extern void fn_8025D9A8();
    extern void fn_8025DAF4();
    extern void fn_8025DB2C();
    extern void fn_8025DB5C();
    extern void fn_8025DB80();
    extern void fn_8025DBB0();
    extern u8 jumptable_802ED9B8[];
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
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
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    
    r31 = r3;
    r16 = 0x0;
    fn_801EF634();
    r18 = r3;
    r27 = -0x1;
    r26 = 0x1;
    fn_8025D9A8();
    r29 = r3;
    fn_8025DBB0();
    r30 = r3;
    r25 = 0x0;
    r24 = 0x0;
    r23 = 0x0;
    r22 = 0x1;
    r20 = 0x1;
    r21 = 0x0;
    r3 = 0x0;
    r4 = 0x2;
    fn_80129280();
    r17 = r3;
    fn_8012A80C();
    r0 = r3;
    r3 = r17;
    r17 = r0;
    fn_8012A7C4();
    r4 = (0x6666 << 16);
    r5 = r30 + 0x1;
    r0 = r4 + 0x6667;
    r19 = r3;
    r0 = (s32)((s64)r0 * (s64)r5 >> 32);
    r28 = r18 & 0xFFFF;
    r0 = (s32)r0 >> 2;
    r3 = (u32)r0 >> 31;
    r0 = r0 + r3;
    r0 = r0 * 0xa;
    r18 = r5 - r0;
    do {
        if ((u32)r16 > (u32)0xc) goto L_80063788;
        r3 = (u32)jumptable_802ED9B8;
        r0 = r16 << 2;
        r3 = (u32)jumptable_802ED9B8;
        r0 = *(u32*)(r3 + r0);
        ctr_fn = (void(*)(void))r0;
        /* indirect jump via ctr */;
        r3 = 0xdf;
        r4 = 0x0;
        fn_8010264C();
        r3 = 0xba;
        r4 = 0x1;
        fn_8010264C();
        if ((s32)r28 == (s32)0x5) goto L_80063160;
        if ((s32)r28 < (s32)0x5) {
            if ((s32)r28 == (s32)0x2) goto L_80063160;
            if ((s32)r28 >= (s32)0x2) goto L_80063260;
            goto L_80063290;
        }
        if ((s32)r28 >= (s32)0x8) goto L_80063290;
        goto L_80063260;
        L_80063160: ;
        if ((s32)r29 != (s32)0x1) {
            if ((s32)r29 >= (s32)0x1) goto L_800631C0;
            if ((s32)r29 < (s32)0x0) {
                goto L_800631C0;
            }
            if ((s32)r30 != (s32)0x7) goto L_800631C0;
            fn_8025D164();
            ((void(*)(void))fn_8006ADB4)();
            r25 = 0x1;
            fn_800637B0();
            goto L_800631C0;
        }
        if ((s32)r18 == (s32)0x0) {
            fn_8025D164();
            r16 = r3;
            ((void(*)(void))fn_8006ADEC)();
            r3 = r16 + r3;
            ((void(*)(void))fn_8006ADB4)();
        }
        r0 = r30 + 0x1;
        if ((s32)r0 != (s32)0x64) goto L_800631C0;
        r25 = 0x1;
        L_800631C0: ;
        r0 = r25 & 0xFF;
        if ((u32)r0 != (u32)0x0) {
            if ((s32)r29 == (s32)0x1) {
                r3 = 0xafd;
                fn_801906A0();
                if ((u32)r3 == (u32)0x0) {
                    fn_801EE398();
                    r21 = r3;
            }
            }
            r16 = 0x5;
            goto L_80063788;
        }
        r3 = 0x0;
        fn_80062284();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x0) {
            r3 = 0x2;
            r4 = 0x3c10;
            r5 = 0x1;
            r6 = 0x1;
            fn_80106D3C();
            r3 = 0x1;
            fn_801069FC();
            fn_8025DB2C();
            r3 = 0x2;
            r4 = 0x30dd;
            r5 = 0x0;
            r6 = 0x1;
            fn_80106D3C();
            r16 = 0x1;
            goto L_80063788;
        }
        r3 = 0x2;
        r4 = 0x30dd;
        r5 = 0x0;
        r6 = 0x1;
        fn_80106D3C();
        r16 = 0x1;
        goto L_80063788;
        L_80063260: ;
        fn_8025DB5C();
        if ((s32)r3 == (s32)0x0) {
            if ((s32)r29 == (s32)0x1) {
                r27 = 0x105;
            } else {

                r27 = 0xac;
            }
            r16 = 0x9;
            goto L_80063788;
        }
        r16 = 0x2;
        goto L_80063788;
        L_80063290: ;
        r3 = 0x2;
        r4 = 0x3da4;
        r5 = 0x0;
        r6 = 0x1;
        fn_80106D3C();
        r16 = 0x9;
        goto L_80063788;
        fn_801046B8();
        r4 = r3;
        r3 = 0xec;
        r5 = 0x0;
        r6 = 0x8;
        r7 = 0x0;
        r8 = 0x0;
        /* crclr cr1eq */;
        fn_801026A4();
        /* lha r4, lbl_80478920@sda21(r0) */;
        r3 = 0xec;
        /* lha r5, lbl_80478922@sda21(r0) */;
        fn_80102868();
        r3 = 0xec;
        r4 = 0x1;
        fn_801045A8();
        r3 = 0xec;
        fn_801043A4();
        r16 = (s8)r3;
        r3 = 0xec;
        r4 = 0x0;
        r5 = 0x1;
        fn_80102568();
        r3 = 0x1;
        fn_801069FC();
        if ((s32)r16 == (s32)0x0) {
            r0 = r22 & 0xFF;
            r24 = 0x1;
            if ((u32)r0 != (u32)0x0) {
                fn_8025DB80();
            }
            r27 = 0xd1;
            r23 = 0x0;
            r16 = 0x9;
            goto L_80063788;
        }
        r0 = r22 & 0xFF;
        r24 = 0x1;
        if ((u32)r0 != (u32)0x0) {
            fn_8025DB80();
        }
        r16 = 0x9;
        r23 = 0x1;
        r27 = 0xac;
        goto L_80063788;
        r3 = 0x2;
        r4 = 0x44e3;
        r5 = 0x0;
        r6 = 0x1;
        fn_80106D3C();
        /* lha r4, lbl_80478920@sda21(r0) */;
        r3 = 0x0;
        /* lha r5, lbl_80478922@sda21(r0) */;
        r6 = 0x1;
        ((void(*)(void))fn_8001E074)();
        r16 = (s8)r3;
        r3 = 0x1;
        fn_801069FC();
        if ((s32)r16 == (s32)0x0) {
            r16 = 0xc;
            goto L_80063788;
        }
        r16 = 0x1;
        goto L_80063788;
        fn_8025DB5C();
        r4 = r3;
        r3 = 0x30;
        fn_80132A38();
        r3 = 0x2;
        r4 = 0x3c13;
        r5 = 0x0;
        r6 = 0x1;
        fn_80106D3C();
        /* lha r4, lbl_80478920@sda21(r0) */;
        r3 = 0x0;
        /* lha r5, lbl_80478922@sda21(r0) */;
        r6 = 0x0;
        ((void(*)(void))fn_8001E074)();
        r16 = (s8)r3;
        r3 = 0x1;
        fn_801069FC();
        if ((s32)r16 == (s32)0x0) {
            fn_8025DAF4();
            r16 = 0x9;
            r27 = 0xd1;
            goto L_80063788;
        }
        r16 = 0x3;
        goto L_80063788;
        fn_8025DB5C();
        r4 = r3;
        r3 = 0x30;
        fn_80132A38();
        r3 = 0x2;
        r4 = 0x44df;
        r5 = 0x0;
        r6 = 0x1;
        fn_80106D3C();
        /* lha r4, lbl_80478920@sda21(r0) */;
        r3 = 0x0;
        /* lha r5, lbl_80478922@sda21(r0) */;
        r6 = 0x1;
        ((void(*)(void))fn_8001E074)();
        r16 = (s8)r3;
        r3 = 0x1;
        fn_801069FC();
        if ((s32)r16 == (s32)0x0) {
            r16 = 0x9;
            r27 = 0xac;
            goto L_80063788;
        }
        r16 = 0x2;
        goto L_80063788;
        ((void(*)(void))fn_8006ADEC)();
        r0 = r3;
        r3 = 0x30;
        r4 = r0;
        fn_80132A38();
        r3 = 0x3cc;
        r4 = 0x0;
        r5 = 0x0;
        fn_80166AB8();
        r3 = 0x2;
        r4 = 0x3c11;
        r5 = 0x1;
        r6 = 0x1;
        fn_80106D3C();
        r3 = 0x0;
        ((void(*)(void))fn_8006B09C)();
        ((void(*)(void))fn_8006A7D0)();
        ((void(*)(void))fn_8006AC6C)();
        r0 = r21 & 0xFF;
        r3 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0x0) {
            r16 = 0x8;
            goto L_80063788;
        }
        if ((s32)r3 != (s32)0x0) {
            if ((s32)r3 >= (s32)0x0 || (s32)r3 >= (s32)0x3) {

                goto L_800634F0;
            }
            r16 = 0x6;
            goto L_80063788;
            L_800634F0: ;
            r27 = 0x105;
            r16 = 0xc;
            goto L_80063788;
            }
        r16 = 0x6;
        goto L_80063788;
        r3 = 0x2;
        r4 = 0x3c23;
        r5 = 0x0;
        r6 = 0x1;
        fn_80106D3C();
        /* lha r4, lbl_80478920@sda21(r0) */;
        r3 = 0x0;
        /* lha r5, lbl_80478922@sda21(r0) */;
        r6 = 0x0;
        ((void(*)(void))fn_8001E074)();
        r0 = (s8)r3;
        if ((s32)r0 == (s32)0x0) {
            r27 = 0x105;
            r16 = 0xc;
            goto L_80063788;
        }
        r16 = 0xb;
        goto L_80063788;
        r3 = 0x2;
        r4 = 0x3c0f;
        r5 = 0x0;
        r6 = 0x1;
        fn_80106D3C();
        /* lha r4, lbl_80478920@sda21(r0) */;
        r3 = 0x0;
        /* lha r5, lbl_80478922@sda21(r0) */;
        r6 = 0x1;
        ((void(*)(void))fn_8001E074)();
        r0 = (s8)r3;
        if ((s32)r0 == (s32)0x0) {
            r27 = 0xac;
            r16 = 0xc;
            goto L_80063788;
        }
        r16 = 0xa;
        goto L_80063788;
        r3 = 0x2;
        r4 = 0x3c03;
        r5 = 0x0;
        r6 = 0x1;
        fn_80106D3C();
        /* lha r4, lbl_80478920@sda21(r0) */;
        r3 = 0x0;
        /* lha r5, lbl_80478922@sda21(r0) */;
        r6 = 0x0;
        ((void(*)(void))fn_8001E074)();
        r16 = (s8)r3;
        r3 = 0x1;
        fn_801069FC();
        if ((s32)r16 == (s32)0x0) {
            r0 = r20 & 0xFF;
            r27 = 0xac;
            r16 = 0x9;
            if ((u32)r0 != (u32)0x0) {
                fn_8025D06C();
            }
            r23 = 0x1;
            goto L_80063788;
        }
        r16 = 0x7;
        goto L_80063788;
        r3 = 0x2;
        r4 = 0x3c41;
        r5 = 0x0;
        r6 = 0x1;
        fn_80106D3C();
        /* lha r4, lbl_80478920@sda21(r0) */;
        r3 = 0x0;
        /* lha r5, lbl_80478922@sda21(r0) */;
        r6 = 0x1;
        ((void(*)(void))fn_8001E074)();
        r0 = (s8)r3;
        if ((s32)r0 == (s32)0x0) {
            r3 = 0x0;
            r4 = 0x2;
            fn_80129280();
            r4 = r17;
            r16 = r3;
            fn_8012A824();
            r3 = r16;
            r4 = r19;
            fn_8012A7DC();
            r16 = 0xc;
            goto L_80063788;
        }
        r16 = 0x6;
        goto L_80063788;
        r3 = 0x2;
        r4 = 0x3c12;
        r5 = 0x0;
        r6 = 0x1;
        fn_80106D3C();
        r3 = 0x1;
        fn_801069FC();
        r3 = 0xdf;
        r4 = 0x1c6;
        fn_801080CC();
        r3 = 0xba;
        r4 = 0x1c6;
        fn_801080CC();
        while (1) {
            r3 = 0xdf;
            fn_801070F4();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x0) break;
            fn_800F0308();


        }
        while (1) {
            r3 = 0xba;
            fn_801070F4();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x0) break;
            fn_800F0308();


        }
        fn_80069944();
        fn_80062834();
        r3 = 0x1;
        fn_80061028();
        r3 = 0xdf;
        r4 = 0x0;
        r5 = 0x1;
        fn_80102568();
        ((void(*)(void))fn_800886D0)();
        r27 = 0x105;
        r16 = 0x9;
        goto L_80063788;
        r0 = r23 & 0xFF;
        if ((u32)r0 != (u32)0x0) {
            r0 = r24 & 0xFF;
            if ((u32)r0 != (u32)0x0) {
                r3 = 0x0;
                ((void(*)(void))fn_800889E4)();
                if ((s32)r3 < (s32)0x0) {
                    r16 = 0x4;
                    r22 = 0x0;
                    goto L_80063788;
                }
                r3 = r31;
                r26 = 0x0;
                ((void(*)(void))fn_80069C0C)();
                goto L_80063788;
            }
            fn_80088D84();
            if ((s32)r3 < (s32)0x0) {
                r16 = 0x6;
                r20 = 0x0;
                goto L_80063788;
            }
            r3 = r31;
            r26 = 0x0;
            ((void(*)(void))fn_80069C0C)();
            goto L_80063788;
        }
        r0 = r24 & 0xFF;
        r26 = 0x0;
        if ((u32)r0 == (u32)0x0) goto L_80063788;
        r3 = r31;
        ((void(*)(void))fn_80069C0C)();
        goto L_80063788;
        r26 = 0x0;
        L_80063788: ;
    } while ((s32)r26 != (s32)0x0);
    r3 = 0x1;
    fn_801069FC();
    r3 = r27;
    return;
}


/* 0x800637B0 | size: 0x144 */
s32 fn_800637B0(void) {
    extern void fn_8025DA88();
    extern void fn_8025DAAC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    
    fn_8025DA88();
    r30 = r3;
    fn_8025DAAC();
    r0 = r3;
    r4 = r30;
    r31 = r0;
    ((void(*)(void))fn_8006B1F4)();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r3 = r31;
        r4 = r30;
        ((void(*)(void))fn_8006B2A4)();
    }
    r3 = 0x3;
    ((void(*)(void))fn_8006B3C8)();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r30 = 0x1;
        for (r31 = 0x0; (s32)r31 <= (s32)0x2; r31 = r31 + 0x1) {
            r3 = r31;
            r4 = 0x0;
            ((void(*)(void))fn_8006B1F4)();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x0) {
                r30 = 0x0;
                break;
            }
            r3 = r31;
            r4 = 0x1;
            ((void(*)(void))fn_8006B1F4)();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x0) {
                r30 = 0x0;
                break;
            }
        }
        r0 = r30 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x3;
            ((void(*)(void))fn_8006B354)();
        }
    }
    r3 = 0x5;
    ((void(*)(void))fn_8006B3C8)();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) return;
    r31 = 0x1;
    r3 = 0x4;
    r4 = 0x0;
    ((void(*)(void))fn_8006B1F4)();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0x4;
        r4 = 0x1;
        ((void(*)(void))fn_8006B1F4)();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x0) {
            r31 = 0x0;
        }
    } else {
        r31 = 0x0;
    }
    r0 = r31 & 0xFF;
    if ((u32)r0 != (u32)0x1) return;
    r3 = 0x5;
    ((void(*)(void))fn_8006B354)();

    return;
}


/* 0x800638F4 | size: 0x1E0 */
s32 fn_800638F4(void) {
    extern void fn_80063AD4();
    extern void fn_800FB680();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    
    r0 = *(s16*)((u8*)r4 + 0x6);

    /* Switch on message ID */
    if ((s32)r0 == 0xe08 || (s32)r0 == 0xe17 || (s32)r0 == 0x1264 || (s32)r0 == 0x1123) {
        /* These IDs call fn_80063AD4 */
        fn_80063AD4();
        return;
    }

    if ((s32)r0 == 0xe14 || (s32)r0 == 0xe24 || (s32)r0 == 0x126f) {
        r6 = 0x3c21;
    } else if ((s32)r0 == 0xe15 || (s32)r0 == 0xe26) {
        r6 = 0x3db2;
    } else if ((s32)r0 == 0xe16 || (s32)r0 == 0xe27 || (s32)r0 == 0x1270) {
        r6 = 0x3db3;
    } else if ((s32)r0 == 0xe25) {
        r6 = 0x3dae;
    } else {
        return;
    }

    r5 = *(u8*)((u8*)r3 + 0x8B);
    r0 = -0x100;
    r3 = 0x0;
    r4 = 0x0;
    r5 = r5 | r0;
    fn_800FB680();

    return;
}


/* 0x80063AD4 | size: 0x23C */
s32 fn_80063AD4(void) {
    extern void fn_800D5648();
    extern void fn_800D5BA0();
    extern void fn_800D61E4();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    extern void fn_800FE35C();
    extern void fn_800FE38C();
    u8 sp[0x60];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f31 = 0.0f;

    
    *(f64*)(sp + 0x50) = f31;
    /* psq_st f31, 0x58((u32)sp), 0, qr0 */;
    r31 = r4;
    r4 = *(u8*)((u8*)r3 + 0x8B);
    r5 = (0x4330 << 16);
    r0 = *(u8*)((u8*)r31 + 0x67);
    r3 = (0x8102 << 16);
    r8 = *(u32*)&lbl_8047BFC8;
    /* subi r7, r3, 0x7dfd */;
    r6 = r4 * r0;
    r0 = *(u32*)&lbl_8047BFCC;
    r3 = 0x1;
    f4 = *(f64*)&lbl_8047BFD8;
    *(u32*)(sp + 0xC) = r0;
    r7 = (s32)((s64)r7 * (s64)r6 >> 32);
    r4 = *(u8*)(sp + 0xB);
    r0 = *(u8*)(sp + 0xF);
    f2 = *(f64*)&lbl_8047BFE0;
    r4 = r7 + r6;
    r4 = (s32)r4 >> 15;
    r6 = (u32)r4 >> 31;
    *(u32*)(sp + 0x2C) = r0;
    r0 = r4 + r6;
    f0 = *(f64*)(sp + 0x18);
    /* xoris r0, r0, 0x8000 */;
    f1 = f0 - f2;
    *(u32*)(sp + 0x14) = r0;
    f0 = *(f64*)(sp + 0x28);
    f3 = *(f64*)(sp + 0x10);
    f0 = f0 - f2;
    f31 = f3 - f4;
    f1 = f1 * f31;
    f0 = f0 * f31;
    f1 = (f64)(s32)f1;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x20) = f1;
    *(f64*)(sp + 0x30) = f0;
    *(u8*)(sp + 0xB) = r4;
    *(u8*)(sp + 0xF) = r0;
    fn_800D88DC();
    r3 = 0x6;
    fn_800D888C();
    r3 = 0x6;
    fn_800D6A00();
    r3 = (u32)&lbl_80314E08;
    r3 = (u32)&lbl_80314E08;
    fn_800D7820();
    r3 = 0x4;
    fn_800D67BC();
    r3 = 0x0;
    r4 = 0x0;
    fn_800D61E4();
    r3 = 0x0;
    fn_800D5BA0();
    r3 = *(s16*)((u8*)r31 + 0x54);
    r4 = 0x0;
    fn_800D61E4();
    r3 = 0x0;
    fn_800D5BA0();
    r3 = *(s16*)((u8*)r31 + 0x54);
    r4 = *(s16*)((u8*)r31 + 0x56);
    fn_800D61E4();
    r3 = 0x0;
    fn_800D5BA0();
    r4 = *(s16*)((u8*)r31 + 0x56);
    r3 = 0x0;
    fn_800D61E4();
    r3 = 0x0;
    fn_800D5BA0();
    fn_800D6728();
    r5 = *(s16*)((u8*)r31 + 0x54);
    r3 = 0x0;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r4 = 0x0;
    fn_800FE38C();
    r3 = 0x1;
    fn_800D88DC();
    r3 = 0x6;
    fn_800D888C();
    f1 = *(f32*)&lbl_8047BFD0;
    fn_800D5648();
    r3 = 0x1;
    fn_800D6A00();
    r3 = (u32)&lbl_80314E08;
    r3 = (u32)&lbl_80314E08;
    fn_800D7820();
    f0 = *(f32*)&lbl_8047BFD4;
    r0 = 0xff;
    *(u8*)(sp + 0x8) = r0;
    r30 = 0x0;
    f0 = f0 * f31;
    *(u8*)(sp + 0x9) = r0;
    f0 = (f64)(s32)f0;
    *(u8*)(sp + 0xA) = r0;
    *(f64*)(sp + 0x38) = f0;
    *(u8*)(sp + 0xB) = r0;

    while ((s32)r3 < (s32)r0) {
        r3 = 0x2;
        fn_800D67BC();
        r4 = r30;
        r3 = 0x0;
        fn_800D61E4();
        r3 = 0x0;
        fn_800D5BA0();
        r3 = *(s16*)((u8*)r31 + 0x54);
        r4 = r30;
        fn_800D61E4();
        r3 = 0x0;
        fn_800D5BA0();
        fn_800D6728();
        r30 = r30 + 0x4;

    r0 = *(s16*)((u8*)r31 + 0x56);
    r3 = (s16)r30;
    }
    fn_800FE35C();
    /* psq_l f31, 0x58((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x50);
    return;
}


/* 0x80063D10 | size: 0x4 */
void fn_80063D10(void) {
}

/* 0x80063D14 | size: 0x664 */
s32 fn_80063D14(void) {
    extern void fn_80062834();
    extern void fn_8008AB20();
    extern void fn_8008AB4C();
    extern void fn_800F0308();
    extern void fn_800F7C28();
    extern void fn_800F7EF8();
    extern void fn_80102568();
    extern void fn_8010264C();
    extern void fn_80103CC0();
    extern void fn_801069FC();
    extern void fn_80106D3C();
    extern void fn_80132A38();
    extern void fn_80165A20();
    extern void fn_8025D3F4();
    extern void fn_8025D744();
    extern void fn_8025D89C();
    extern void fn_8025D9CC();
    extern void fn_8025D9F0();
    extern void fn_8025DA3C();
    extern void fn_8025DA88();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
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
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    
    r22 = r3;
    r25 = 0x1;
    r3 = 0x1e;
    r4 = 0x0;
    r5 = 0xff;
    fn_80165A20();
    r3 = (u32)&lbl_803A9F08;
    r5 = (0x1 << 16);
    r3 = (u32)&lbl_803A9F08;
    r4 = r22;
    r3 = r3 + 0x150;
    /* subi r5, r5, 0x33d4 */;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    fn_8025DA88();
    r30 = r3;
    fn_8025DA3C();
    r4 = (u32)&lbl_803A9F08;
    r0 = -0x1;
    r27 = (u32)&lbl_803A9F08;
    f0 = *(f32*)&lbl_8047BFE8;
    r4 = 0x1;
    r29 = 0x0;
    r26 = r27 + (0x1 << 16);
    r31 = r3;
    *(u8*)((u8*)r26 + (-12712)) = r4;
    r28 = r27;
    *(u32*)((u8*)r26 + (-12708)) = r0;
    *(f32*)((u8*)r26 + (-12724)) = f0;
    *(u32*)((u8*)r26 + (-12720)) = r29;
    *(u32*)((u8*)r27 + 0x2C) = r29;
    *(u32*)((u8*)r26 + (-12928)) = r29;
    *(u32*)((u8*)r27 + 0x0) = r29;
    *(u32*)((u8*)r27 + 0xC) = r29;
    while ((s32)r29 < (s32)r31) {

        r3 = r29;
        ((void(*)(void))fn_8006B09C)();
        r24 = r3;
        ((void(*)(void))fn_8006A814)();
        r22 = r3;
        r3 = r29;
        ((void(*)(void))fn_8006B0F8)();
        r0 = 0x0;
        r4 = r3;
        *(u8*)((u8*)r28 + 0x4) = r0;
        r0 = *(u32*)((u8*)r24 + 0x4);
        if ((s32)r0 != (s32)0x0) {
            r3 = r22;
            fn_8008AB4C();
            if ((s32)r30 == (s32)0x1) {
                r24 = 0x2;
            } else {

                r24 = 0x1;
            }
            ((void(*)(void))fn_8006B1D4)();
            r23 = r3 & 0xFFFF;
            r3 = r29;
            fn_8025D89C();
            r0 = r3 & 0xFFFF;
            r3 = r22;
            if ((u32)r0 < (u32)r23) {
            } else {

                r0 = r23;
            }
            r5 = r24;
            r4 = r0 & 0xFFFF;
            fn_8008AB20();
        }
        r28 = r28 + 0x1;
        r29 = r29 + 0x1;

    }
    r22 = 0x0;
    do {
        r3 = r22;
        fn_8025D744();
        r22 = r22 + 0x1;
    } while ((s32)r22 < (s32)0x4);
    r29 = r27;
    r24 = (u32)sp + 0x8;
    r23 = 0x0;
    r28 = -0x1;
    do {
        r3 = r23;
        ((void(*)(void))fn_8006ACCC)();
        *(u32*)((u8*)r24 + 0x0) = r3;
        r3 = *(u32*)((u8*)r24 + 0x0);
        if ((u32)r3 != (u32)0x0) {
            r0 = *(u32*)((u8*)r3 + 0x28);
            r0 = (s8)r0;
            *(u8*)((u8*)r29 + 0x8) = r0;
        } else {

            *(u8*)((u8*)r29 + 0x8) = r28;
        }
        r24 = r24 + 0x4;
        r29 = r29 + 0x1;
        r23 = r23 + 0x1;
    } while ((s32)r23 < (s32)0x4);
    fn_8025DA88();
    if ((s32)r3 != (s32)0x2) {
        if ((s32)r3 >= (s32)0x2) goto L_80063EDC;
        if ((s32)r3 < (s32)0x0) {
            goto L_80063EDC;
        }
        r0 = 0x136;
        *(u32*)((u8*)r26 + (-12928)) = r0;
        goto L_80063EE4;
    }
    r0 = 0x0;
    *(u32*)((u8*)r26 + (-12928)) = r0;
    goto L_80063EE4;
    L_80063EDC: ;
    r0 = 0x0;
    *(u32*)((u8*)r26 + (-12928)) = r0;
    L_80063EE4: ;
    r3 = (u32)&lbl_803A9F08;
    r31 = (u32)&lbl_803A9F08;
    do {
        r0 = *(u32*)((u8*)r31 + 0x0);
        if ((s32)r0 != (s32)0x2) {
            if ((s32)r0 < (s32)0x2) {
                if ((s32)r0 != (s32)0x0) {
                    if ((s32)r0 < (s32)0x0) {
                        goto L_80064070;
                    }
                    if ((s32)r0 >= (s32)0x4) goto L_80064070;
                    goto L_8006404C;
                    }
                r3 = 0x0;
                fn_80103CC0();
                r3 = 0xc6;
                r4 = 0x1;
                fn_8010264C();
                r28 = r3;
                r3 = 0x1;
                fn_80103CC0();
                if ((s32)r28 == (s32)0x0) {
                    r3 = (u32)&lbl_803A9F08;
                    r0 = 0x2;
                    r3 = (u32)&lbl_803A9F08;
                    *(u32*)((u8*)r3 + 0x0) = r0;

                } else if ((s32)r28 == (s32)0x1) {
                    r3 = (u32)&lbl_803A9F08;
                    r0 = 0x1;
                    r3 = (u32)&lbl_803A9F08;
                    *(u32*)((u8*)r3 + 0x0) = r0;

                }
                r3 = (u32)&lbl_803A9F08;
                r0 = 0x3;
                r3 = (u32)&lbl_803A9F08;
                *(u32*)((u8*)r3 + 0x0) = r0;
                goto L_80064070;
                    }
            r3 = 0xc5;
            r4 = 0x1;
            fn_8010264C();
            if ((s32)r3 == (s32)0x0) {
                r0 = 0x1;
                r3 = 0xc5;
                *(u8*)((u8*)r31 + 0x4) = r0;
                r4 = 0x0;
                r5 = 0x1;
                fn_80102568();
                r3 = (u32)&lbl_803A9F08;
                r0 = 0x0;
                r3 = (u32)&lbl_803A9F08;
                *(u32*)((u8*)r3 + 0x0) = r0;
                goto L_80064070;
            }
            r3 = 0x0;
            fn_8025D744();
            r3 = 0xc5;
            r4 = 0x0;
            r5 = 0x1;
            fn_80102568();
            r3 = (u32)&lbl_803A9F08;
            r0 = 0x0;
            r3 = (u32)&lbl_803A9F08;
            *(u32*)((u8*)r3 + 0x0) = r0;
            goto L_80064070;
        }
        r3 = 0xc7;
        r4 = 0x1;
        fn_8010264C();
        if ((s32)r3 >= (s32)0x0) {
            r3 = 0xc7;
            r4 = 0x0;
            r5 = 0x1;
            fn_80102568();
            r3 = (u32)&lbl_803A9F08;
            r0 = 0x0;
            r3 = (u32)&lbl_803A9F08;
            *(u32*)((u8*)r3 + 0x0) = r0;
            goto L_80064070;
        }
        r3 = 0xc7;
        r4 = 0x0;
        r5 = 0x1;
        fn_80102568();
        r3 = (u32)&lbl_803A9F08;
        r0 = 0x0;
        r3 = (u32)&lbl_803A9F08;
        *(u32*)((u8*)r3 + 0x0) = r0;
        goto L_80064070;
        L_8006404C: ;
        r25 = 0x0;
        r3 = 0xc6;
        r4 = 0x0;
        r5 = 0x1;
        fn_80102568();
        r3 = 0xdf;
        r4 = 0x0;
        r5 = 0x1;
        fn_80102568();
        L_80064070: ;
    } while ((s32)r25 != (s32)0x0);
    r0 = *(u8*)((u8*)r26 + (-12712));
    if ((u32)r0 == (u32)0x0) {
        fn_8025DA88();
        if ((s32)r3 == (s32)0x2) {
            r3 = 0x0;
            fn_8025D9F0();
            r0 = r3 & 0xFFFF;
            if ((s32)r0 != (s32)0x0) {
                r0 = 0x1;
                goto L_800640B8;
            }
            r0 = 0x0;
            goto L_800640B8;
        }
        r0 = 0x0;
        L_800640B8: ;
        r0 = r0 & 0xFFFF;
        if ((u32)r0 == (u32)0x0) {
            r4 = *(u32*)((u8*)r26 + (-12708));
            r3 = 0x30;
            fn_80132A38();
            r3 = 0x2;
            r4 = 0x44dc;
            r5 = 0x1;
            r6 = 0x1;
            fn_80106D3C();
            goto L_80064160;
        }
        r3 = 0x2;
        r4 = 0x44e7;
        r5 = 0x1;
        r6 = 0x1;
        fn_80106D3C();
        r24 = 0x1;
        do {
            r3 = 0x1;
            fn_800F7EF8();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x0) {
                r3 = 0x1;
                fn_800F7C28();
                if ((s32)r3 == (s32)0x0) {
                    r0 = 0x1;
                    goto L_80064138;
                }
                r0 = 0x0;
                goto L_80064138;
            }
            r0 = 0x0;
            L_80064138: ;
            r0 = r0 & 0xFF;
            if ((u32)r0 != (u32)0x0) {
                r24 = 0x0;
            } else {

                fn_800F0308();
            }
        } while ((s32)r24 != (s32)0x0);
        r3 = 0x1;
        fn_801069FC();
        L_80064160: ;
        r0 = 0x0;
        r3 = (u32)&lbl_803A9F08;
        r3 = (u32)&lbl_803A9F08;
        *(u32*)((u8*)r27 + 0x2C) = r0;
        r3 = r3 + (0x1 << 16);
        *(u8*)((u8*)r3 + (-12924)) = r0;
        r0 = 0x2;
        ctr_fn = (void(*)(void))r0;
        do {
            r3 = r27 + 0x30;
            r0 = 0x0;
            *(u8*)((u8*)r3 + 0x0) = r0;
            r8 = r3 + 0xc;
            r7 = r3 + 0x18;
            r6 = r3 + 0x24;
            *(u32*)((u8*)r3 + 0x4) = r0;
            r5 = r3 + 0x30;
            r4 = r3 + 0x3c;
            r27 = r27 + 0x48;
            *(u8*)((u8*)r8 + 0x0) = r0;
            r3 = r27 + 0x30;
            r27 = r27 + 0x48;
            *(u32*)((u8*)r8 + 0x4) = r0;
            r8 = r3 + 0xc;
            *(u8*)((u8*)r7 + 0x0) = r0;
            *(u32*)((u8*)r7 + 0x4) = r0;
            r7 = r3 + 0x18;
            *(u8*)((u8*)r6 + 0x0) = r0;
            *(u32*)((u8*)r6 + 0x4) = r0;
            r6 = r3 + 0x24;
            *(u8*)((u8*)r5 + 0x0) = r0;
            *(u32*)((u8*)r5 + 0x4) = r0;
            r5 = r3 + 0x30;
            *(u8*)((u8*)r4 + 0x0) = r0;
            *(u32*)((u8*)r4 + 0x4) = r0;
            r4 = r3 + 0x3c;
            *(u8*)((u8*)r3 + 0x0) = r0;
            *(u32*)((u8*)r3 + 0x4) = r0;
            *(u8*)((u8*)r8 + 0x0) = r0;
            *(u32*)((u8*)r8 + 0x4) = r0;
            *(u8*)((u8*)r7 + 0x0) = r0;
            *(u32*)((u8*)r7 + 0x4) = r0;
            *(u8*)((u8*)r6 + 0x0) = r0;
            *(u32*)((u8*)r6 + 0x4) = r0;
            *(u8*)((u8*)r5 + 0x0) = r0;
            *(u32*)((u8*)r5 + 0x4) = r0;
            *(u8*)((u8*)r4 + 0x0) = r0;
            *(u32*)((u8*)r4 + 0x4) = r0;
        } while (--ctr != 0);
        fn_80062834();
        r3 = 0xb3;
        return;
    }
    fn_8025DA88();
    r25 = r3;
    fn_8025D9CC();
    if ((s32)r3 == (s32)0x4) {
        if ((s32)r25 >= (s32)0x2) goto L_8006425C;
        if ((s32)r25 < (s32)0x0) {
            goto L_8006425C;
        }
        r23 = 0x2;
        goto L_80064284;
        L_8006425C: ;
        r23 = 0x4;
        goto L_80064284;
    }
    if ((s32)r25 >= (s32)0x2) goto L_80064280;
    if ((s32)r25 < (s32)0x0) {
        goto L_80064280;
    }
    r23 = 0x2;
    goto L_80064284;
    L_80064280: ;
    r23 = 0x1;
    L_80064284: ;
    r22 = 0x0;
    while ((s32)r22 < (s32)r23) {

        r3 = r22;
        fn_8025D3F4();
        r22 = r22 + 0x1;

    }
    r0 = 0x0;
    r3 = (u32)&lbl_803A9F08;
    r3 = (u32)&lbl_803A9F08;
    *(u32*)((u8*)r27 + 0x2C) = r0;
    r3 = r3 + (0x1 << 16);
    *(u8*)((u8*)r3 + (-12924)) = r0;
    r0 = 0x2;
    ctr_fn = (void(*)(void))r0;
    do {
        r3 = r27 + 0x30;
        r0 = 0x0;
        *(u8*)((u8*)r3 + 0x0) = r0;
        r8 = r3 + 0xc;
        r7 = r3 + 0x18;
        r6 = r3 + 0x24;
        *(u32*)((u8*)r3 + 0x4) = r0;
        r5 = r3 + 0x30;
        r4 = r3 + 0x3c;
        r27 = r27 + 0x48;
        *(u8*)((u8*)r8 + 0x0) = r0;
        r3 = r27 + 0x30;
        r27 = r27 + 0x48;
        *(u32*)((u8*)r8 + 0x4) = r0;
        r8 = r3 + 0xc;
        *(u8*)((u8*)r7 + 0x0) = r0;
        *(u32*)((u8*)r7 + 0x4) = r0;
        r7 = r3 + 0x18;
        *(u8*)((u8*)r6 + 0x0) = r0;
        *(u32*)((u8*)r6 + 0x4) = r0;
        r6 = r3 + 0x24;
        *(u8*)((u8*)r5 + 0x0) = r0;
        *(u32*)((u8*)r5 + 0x4) = r0;
        r5 = r3 + 0x30;
        *(u8*)((u8*)r4 + 0x0) = r0;
        *(u32*)((u8*)r4 + 0x4) = r0;
        r4 = r3 + 0x3c;
        *(u8*)((u8*)r3 + 0x0) = r0;
        *(u32*)((u8*)r3 + 0x4) = r0;
        *(u8*)((u8*)r8 + 0x0) = r0;
        *(u32*)((u8*)r8 + 0x4) = r0;
        *(u8*)((u8*)r7 + 0x0) = r0;
        *(u32*)((u8*)r7 + 0x4) = r0;
        *(u8*)((u8*)r6 + 0x0) = r0;
        *(u32*)((u8*)r6 + 0x4) = r0;
        *(u8*)((u8*)r5 + 0x0) = r0;
        *(u32*)((u8*)r5 + 0x4) = r0;
        *(u8*)((u8*)r4 + 0x0) = r0;
        *(u32*)((u8*)r4 + 0x4) = r0;
    } while (--ctr != 0);
    r3 = 0xb8;

    return;
}


/* 0x80064378 | size: 0x5C */
s32 fn_80064378(void) {
    extern void fn_80063AD4();
    extern void fn_800FB680();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    
    r0 = *(s16*)((u8*)r4 + 0x6);
    if ((s32)r0 != (s32)0xa9e) {
        if ((s32)r0 >= (s32)0xa9e) return;
        if ((s32)r0 != (s32)0xa88) {
            return;
        }
        r5 = *(u8*)((u8*)r3 + 0x8B);
        r0 = -0x100;
        r3 = 0x0;
        r4 = 0x0;
        r5 = r5 | r0;
        r6 = 0x3c1a;
        fn_800FB680();
        return;
        }
    fn_80063AD4();

    return;
}


/* 0x800643D4 | size: 0x1254 */
s32 fn_800643D4(void) {
    extern void fn_80060EF4();
    extern void fn_800FA280();
    extern void fn_800FB680();
    extern void fn_800FBB34();
    extern void fn_801040F0();
    extern void fn_8010B9E8();
    extern void fn_8011BEB4();
    extern void fn_8011C9EC();
    extern void fn_8011CA34();
    extern void fn_8011F188();
    extern void fn_8011F4F0();
    extern void fn_801230E0();
    extern void fn_80123CD4();
    extern void fn_8012640C();
    extern void fn_80132A38();
    extern void fn_8025D970();
    extern void fn_8025DA88();
    extern void fn_8025DAD0();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    
    r30 = r3;
    r31 = r4;
    r0 = *(s16*)((u8*)r31 + 0x6);
    if ((s32)r0 == (s32)0xb34) goto L_80064E1C;
    if ((s32)r0 < (s32)0xb34) {
        if ((s32)r0 == (s32)0xb2a) goto L_800653D8;
        if ((s32)r0 < (s32)0xb2a) {
            if ((s32)r0 == (s32)0xb21) goto L_8006526C;
            if ((s32)r0 < (s32)0xb21) {
                if ((s32)r0 == (s32)0xb1f) goto L_8006516C;
                if ((s32)r0 >= (s32)0xb1f) goto L_800651D0;
                if ((s32)r0 >= (s32)0xb1e) goto L_80065490;
                return;
            }
            if ((s32)r0 == (s32)0xb28) goto L_80065320;
            if ((s32)r0 >= (s32)0xb28) goto L_8006537C;
            if ((s32)r0 >= (s32)0xb27) goto L_800652C4;
            return;
        }
        if ((s32)r0 == (s32)0xb2f) goto L_800649A4;
        if ((s32)r0 < (s32)0xb2f) {
            if ((s32)r0 != (s32)0xb2d) {
                if ((s32)r0 >= (s32)0xb2d) goto L_8006489C;
                if ((s32)r0 < (s32)0xb2c) {
                    goto L_80065434;
                }
                if ((s32)r0 == (s32)0xb32) goto L_80064C64;
                if ((s32)r0 >= (s32)0xb32) goto L_80064D40;
                if ((s32)r0 >= (s32)0xb31) goto L_80064B88;
                goto L_80064AAC;
            }
            if ((s32)r0 < (s32)0x1099) {
                if ((s32)r0 != (s32)0xb3a) {
                    if ((s32)r0 < (s32)0xb3a) {
                        if ((s32)r0 == (s32)0xb37) goto L_80065098;
                        if ((s32)r0 < (s32)0xb37) {
                            if ((s32)r0 >= (s32)0xb36) goto L_80064FC4;
                            goto L_80064EF0;
                        }
                        if ((s32)r0 < (s32)0xb39) {
                            goto L_80064520;
                        }
                        if ((s32)r0 == (s32)0xe32) goto L_800654D4;
                        if ((s32)r0 < (s32)0xe32) return;
                        if ((s32)r0 >= (s32)0x1097) goto L_80065528;
                        return;
                    }
                    if ((s32)r0 < (s32)0x10a2) {
                        if ((s32)r0 < (s32)0x109e) {
                            if ((s32)r0 == (s32)0x109b) goto L_80065540;
                            if ((s32)r0 >= (s32)0x109b) goto L_8006554C;
                            goto L_80065534;
                        }
                        if ((s32)r0 >= (s32)0x10a0) goto L_80065564;
                        goto L_80065558;
                    }
                    if ((s32)r0 == (s32)0x10a5) goto L_80065588;
                    if ((s32)r0 >= (s32)0x10a5) return;
                    if ((s32)r0 >= (s32)0x10a4) goto L_8006557C;
                    goto L_80065570;
                    L_80064520: ;
                    r4 = (u32)&lbl_803A9F08;
                    r3 = 0x0;
                    r4 = (u32)&lbl_803A9F08;
                    r4 = *(u32*)((u8*)r4 + 0xC);
                    fn_8025D970();
                    r31 = r3;
                    fn_8011F4F0();
                    if ((u32)r3 == (u32)0x0) {
                        r3 = 0x1;
                        fn_800FA280();
                    }
                    r4 = r3;
                    r3 = 0x37;
                    fn_80132A38();
                    r5 = *(u8*)((u8*)r30 + 0x8B);
                    r0 = -0x100;
                    r3 = 0x0;
                    r4 = 0x0;
                    r5 = r5 | r0;
                    r6 = 0xe7;
                    fn_800FB680();
                    r3 = r31;
                    ((void(*)(void))fn_8001DA60)();
                    r0 = r3 & 0xFF;
                    if ((s32)r0 != (s32)0x1) {
                        if ((s32)r0 < (s32)0x1) {
                            if ((s32)r0 < (s32)0x0) {
                                goto L_800645B0;
                            }
                            goto L_800645B0;
                            }
                        r3 = 0xd67;
                        goto L_800645B4;
                    }
                    r3 = 0xd68;
                    goto L_800645B4;
                    L_800645B0: ;
                    r3 = 0x0;
                    L_800645B4: ;
                    if ((u32)r3 == (u32)0x0) return;
                    fn_800FA280();
                    r4 = r3;
                    r3 = 0x37;
                    fn_80132A38();
                    r5 = *(u8*)((u8*)r30 + 0x8B);
                    r0 = -0x100;
                    r3 = 0x5a;
                    r4 = 0x0;
                    r5 = r5 | r0;
                    r6 = 0xcf;
                    fn_800FB680();
                    return;
                        }
                r4 = (u32)&lbl_803A9F08;
                r3 = 0x0;
                r4 = (u32)&lbl_803A9F08;
                r4 = *(u32*)((u8*)r4 + 0xC);
                fn_8025D970();
                if ((u32)r3 == (u32)0x0) return;
                r4 = 0x0;
                r5 = 0x7a;
                r6 = 0x0;
                fn_8012640C();
                r4 = r3 & 0xFF;
                r3 = 0x34;
                fn_80132A38();
                r5 = *(u8*)((u8*)r30 + 0x8B);
                r0 = -0x100;
                r3 = 0x0;
                r4 = 0x0;
                r5 = r5 | r0;
                r6 = 0xd3;
                fn_800FB680();
                return;
                }
            r4 = (u32)&lbl_803A9F08;
            r3 = 0x0;
            r4 = (u32)&lbl_803A9F08;
            r4 = *(u32*)((u8*)r4 + 0xC);
            fn_8025D970();
            fn_8011F188();
            r0 = r3;
            r3 = 0x34;
            r4 = r0 & 0xFFFF;
            fn_80132A38();
            r5 = *(u8*)((u8*)r30 + 0x8B);
            r0 = -0x100;
            r3 = 0x0;
            r4 = 0x0;
            r5 = r5 | r0;
            r6 = 0xd3;
            fn_800FB680();
            return;
                }
        r4 = (u32)&lbl_803A9F08;
        r3 = 0x0;
        r4 = (u32)&lbl_803A9F08;
        r4 = *(u32*)((u8*)r4 + 0xC);
        fn_8025D970();
        r4 = 0x0;
        r29 = r3;
        r5 = 0x7f;
        r6 = 0x0;
        fn_8012640C();
        r28 = r3 & 0xFFFF;
        r3 = r29;
        r4 = 0x0;
        fn_80123CD4();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x0) {
            r28 = 0x0;
            goto L_80064700;
        }
        r3 = (0x1 << 16);
        /* subi r0, r3, 0x2 */;
        if ((s32)r28 < (s32)r0) {
            if ((s32)r28 != (s32)0x0) {
                goto L_80064700;
            }
            if ((s32)r28 >= (s32)r3) goto L_80064700;
            }
        r28 = 0x0;
        L_80064700: ;
        r3 = (0x1 << 16);
        r4 = r28 & 0xFFFF;
        /* subi r0, r3, 0x2 */;
        if ((s32)r4 != (s32)r0) {
            if ((s32)r4 < (s32)r0) {
                if ((s32)r4 == (s32)0x0) goto L_80064750;
                goto L_8006473C;
            }
            if ((s32)r4 >= (s32)r3) goto L_8006473C;
            r4 = 0x933;
            goto L_80064750;
        }
        r4 = 0x934;
        goto L_80064750;
        L_8006473C: ;
        r3 = 0x0;
        r5 = 0x1;
        r6 = 0x0;
        fn_8011BEB4();
        r4 = r3;
        L_80064750: ;
        if ((u32)r4 == (u32)0x0) return;
        r3 = r4;
        fn_800FA280();
        r4 = r3;
        r3 = 0x37;
        fn_80132A38();
        r4 = *(u8*)((u8*)r30 + 0x8B);
        r0 = -0x100;
        r5 = *(s16*)((u8*)r31 + 0x54);
        r3 = 0x0;
        r6 = *(s16*)((u8*)r31 + 0x56);
        r7 = r4 | r0;
        r4 = 0x0;
        r8 = 0xe9;
        fn_800FBB34();
        return;
            }
    r4 = (u32)&lbl_803A9F08;
    r3 = 0x0;
    r4 = (u32)&lbl_803A9F08;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_8025D970();
    r4 = 0x0;
    r29 = r3;
    r5 = 0x7f;
    r6 = 0x1;
    fn_8012640C();
    r28 = r3 & 0xFFFF;
    r3 = r29;
    r4 = 0x1;
    fn_80123CD4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r28 = 0x0;
        goto L_80064808;
    }
    r3 = (0x1 << 16);
    /* subi r0, r3, 0x2 */;
    if ((s32)r28 < (s32)r0) {
        if ((s32)r28 != (s32)0x0) {
            goto L_80064808;
        }
        if ((s32)r28 >= (s32)r3) goto L_80064808;
        }
    r28 = 0x0;
    L_80064808: ;
    r3 = (0x1 << 16);
    r4 = r28 & 0xFFFF;
    /* subi r0, r3, 0x2 */;
    if ((s32)r4 != (s32)r0) {
        if ((s32)r4 < (s32)r0) {
            if ((s32)r4 == (s32)0x0) goto L_80064858;
            goto L_80064844;
        }
        if ((s32)r4 >= (s32)r3) goto L_80064844;
        r4 = 0x933;
        goto L_80064858;
    }
    r4 = 0x934;
    goto L_80064858;
    L_80064844: ;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x0;
    fn_8011BEB4();
    r4 = r3;
    L_80064858: ;
    if ((u32)r4 == (u32)0x0) return;
    r3 = r4;
    fn_800FA280();
    r4 = r3;
    r3 = 0x37;
    fn_80132A38();
    r4 = *(u8*)((u8*)r30 + 0x8B);
    r0 = -0x100;
    r5 = *(s16*)((u8*)r31 + 0x54);
    r3 = 0x0;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r7 = r4 | r0;
    r4 = 0x0;
    r8 = 0xe9;
    fn_800FBB34();
    return;
    L_8006489C: ;
    r4 = (u32)&lbl_803A9F08;
    r3 = 0x0;
    r4 = (u32)&lbl_803A9F08;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_8025D970();
    r4 = 0x0;
    r29 = r3;
    r5 = 0x7f;
    r6 = 0x2;
    fn_8012640C();
    r28 = r3 & 0xFFFF;
    r3 = r29;
    r4 = 0x2;
    fn_80123CD4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r28 = 0x0;
        goto L_80064910;
    }
    r3 = (0x1 << 16);
    /* subi r0, r3, 0x2 */;
    if ((s32)r28 < (s32)r0) {
        if ((s32)r28 != (s32)0x0) {
            goto L_80064910;
        }
        if ((s32)r28 >= (s32)r3) goto L_80064910;
        }
    r28 = 0x0;
    L_80064910: ;
    r3 = (0x1 << 16);
    r4 = r28 & 0xFFFF;
    /* subi r0, r3, 0x2 */;
    if ((s32)r4 != (s32)r0) {
        if ((s32)r4 < (s32)r0) {
            if ((s32)r4 == (s32)0x0) goto L_80064960;
            goto L_8006494C;
        }
        if ((s32)r4 >= (s32)r3) goto L_8006494C;
        r4 = 0x933;
        goto L_80064960;
    }
    r4 = 0x934;
    goto L_80064960;
    L_8006494C: ;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x0;
    fn_8011BEB4();
    r4 = r3;
    L_80064960: ;
    if ((u32)r4 == (u32)0x0) return;
    r3 = r4;
    fn_800FA280();
    r4 = r3;
    r3 = 0x37;
    fn_80132A38();
    r4 = *(u8*)((u8*)r30 + 0x8B);
    r0 = -0x100;
    r5 = *(s16*)((u8*)r31 + 0x54);
    r3 = 0x0;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r7 = r4 | r0;
    r4 = 0x0;
    r8 = 0xe9;
    fn_800FBB34();
    return;
    L_800649A4: ;
    r4 = (u32)&lbl_803A9F08;
    r3 = 0x0;
    r4 = (u32)&lbl_803A9F08;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_8025D970();
    r4 = 0x0;
    r29 = r3;
    r5 = 0x7f;
    r6 = 0x3;
    fn_8012640C();
    r28 = r3 & 0xFFFF;
    r3 = r29;
    r4 = 0x3;
    fn_80123CD4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r28 = 0x0;
        goto L_80064A18;
    }
    r3 = (0x1 << 16);
    /* subi r0, r3, 0x2 */;
    if ((s32)r28 < (s32)r0) {
        if ((s32)r28 != (s32)0x0) {
            goto L_80064A18;
        }
        if ((s32)r28 >= (s32)r3) goto L_80064A18;
        }
    r28 = 0x0;
    L_80064A18: ;
    r3 = (0x1 << 16);
    r4 = r28 & 0xFFFF;
    /* subi r0, r3, 0x2 */;
    if ((s32)r4 != (s32)r0) {
        if ((s32)r4 < (s32)r0) {
            if ((s32)r4 == (s32)0x0) goto L_80064A68;
            goto L_80064A54;
        }
        if ((s32)r4 >= (s32)r3) goto L_80064A54;
        r4 = 0x933;
        goto L_80064A68;
    }
    r4 = 0x934;
    goto L_80064A68;
    L_80064A54: ;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x0;
    fn_8011BEB4();
    r4 = r3;
    L_80064A68: ;
    if ((u32)r4 == (u32)0x0) return;
    r3 = r4;
    fn_800FA280();
    r4 = r3;
    r3 = 0x37;
    fn_80132A38();
    r4 = *(u8*)((u8*)r30 + 0x8B);
    r0 = -0x100;
    r5 = *(s16*)((u8*)r31 + 0x54);
    r3 = 0x0;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r7 = r4 | r0;
    r4 = 0x0;
    r8 = 0xe9;
    fn_800FBB34();
    return;
    L_80064AAC: ;
    r4 = (u32)&lbl_803A9F08;
    r3 = 0x0;
    r4 = (u32)&lbl_803A9F08;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_8025D970();
    r4 = 0x0;
    r29 = r3;
    r5 = 0x7f;
    r6 = 0x0;
    fn_8012640C();
    r28 = r3 & 0xFFFF;
    r3 = r29;
    r4 = 0x0;
    fn_80123CD4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r28 = 0x0;
        goto L_80064B20;
    }
    r3 = (0x1 << 16);
    /* subi r0, r3, 0x2 */;
    if ((s32)r28 < (s32)r0) {
        if ((s32)r28 != (s32)0x0) {
            goto L_80064B20;
        }
        if ((s32)r28 >= (s32)r3) goto L_80064B20;
        }
    r28 = 0x0;
    L_80064B20: ;
    r3 = (0x1 << 16);
    r4 = r28 & 0xFFFF;
    /* subi r0, r3, 0x2 */;
    if ((s32)r4 == (s32)r0) return;
    if ((s32)r4 < (s32)r0) {
        if ((s32)r4 == (s32)0x0) return;
    }
    r3 = r29;
    r4 = 0x0;
    r5 = 0x80;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3;
    r3 = 0x34;
    fn_80132A38();
    r4 = *(u8*)((u8*)r30 + 0x8B);
    r0 = -0x100;
    r5 = *(s16*)((u8*)r31 + 0x54);
    r3 = 0x0;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r7 = r4 | r0;
    r4 = 0x0;
    r8 = 0xdf;
    fn_800FBB34();
    return;
    L_80064B88: ;
    r4 = (u32)&lbl_803A9F08;
    r3 = 0x0;
    r4 = (u32)&lbl_803A9F08;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_8025D970();
    r4 = 0x0;
    r29 = r3;
    r5 = 0x7f;
    r6 = 0x1;
    fn_8012640C();
    r28 = r3 & 0xFFFF;
    r3 = r29;
    r4 = 0x1;
    fn_80123CD4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r28 = 0x0;
        goto L_80064BFC;
    }
    r3 = (0x1 << 16);
    /* subi r0, r3, 0x2 */;
    if ((s32)r28 < (s32)r0) {
        if ((s32)r28 != (s32)0x0) {
            goto L_80064BFC;
        }
        if ((s32)r28 >= (s32)r3) goto L_80064BFC;
        }
    r28 = 0x0;
    L_80064BFC: ;
    r3 = (0x1 << 16);
    r4 = r28 & 0xFFFF;
    /* subi r0, r3, 0x2 */;
    if ((s32)r4 == (s32)r0) return;
    if ((s32)r4 < (s32)r0) {
        if ((s32)r4 == (s32)0x0) return;
    }
    r3 = r29;
    r4 = 0x0;
    r5 = 0x80;
    r6 = 0x1;
    fn_8012640C();
    r4 = r3;
    r3 = 0x34;
    fn_80132A38();
    r4 = *(u8*)((u8*)r30 + 0x8B);
    r0 = -0x100;
    r5 = *(s16*)((u8*)r31 + 0x54);
    r3 = 0x0;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r7 = r4 | r0;
    r4 = 0x0;
    r8 = 0xdf;
    fn_800FBB34();
    return;
    L_80064C64: ;
    r4 = (u32)&lbl_803A9F08;
    r3 = 0x0;
    r4 = (u32)&lbl_803A9F08;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_8025D970();
    r4 = 0x0;
    r29 = r3;
    r5 = 0x7f;
    r6 = 0x2;
    fn_8012640C();
    r28 = r3 & 0xFFFF;
    r3 = r29;
    r4 = 0x2;
    fn_80123CD4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r28 = 0x0;
        goto L_80064CD8;
    }
    r3 = (0x1 << 16);
    /* subi r0, r3, 0x2 */;
    if ((s32)r28 < (s32)r0) {
        if ((s32)r28 != (s32)0x0) {
            goto L_80064CD8;
        }
        if ((s32)r28 >= (s32)r3) goto L_80064CD8;
        }
    r28 = 0x0;
    L_80064CD8: ;
    r3 = (0x1 << 16);
    r4 = r28 & 0xFFFF;
    /* subi r0, r3, 0x2 */;
    if ((s32)r4 == (s32)r0) return;
    if ((s32)r4 < (s32)r0) {
        if ((s32)r4 == (s32)0x0) return;
    }
    r3 = r29;
    r4 = 0x0;
    r5 = 0x80;
    r6 = 0x2;
    fn_8012640C();
    r4 = r3;
    r3 = 0x34;
    fn_80132A38();
    r4 = *(u8*)((u8*)r30 + 0x8B);
    r0 = -0x100;
    r5 = *(s16*)((u8*)r31 + 0x54);
    r3 = 0x0;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r7 = r4 | r0;
    r4 = 0x0;
    r8 = 0xdf;
    fn_800FBB34();
    return;
    L_80064D40: ;
    r4 = (u32)&lbl_803A9F08;
    r3 = 0x0;
    r4 = (u32)&lbl_803A9F08;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_8025D970();
    r4 = 0x0;
    r29 = r3;
    r5 = 0x7f;
    r6 = 0x3;
    fn_8012640C();
    r28 = r3 & 0xFFFF;
    r3 = r29;
    r4 = 0x3;
    fn_80123CD4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r28 = 0x0;
        goto L_80064DB4;
    }
    r3 = (0x1 << 16);
    /* subi r0, r3, 0x2 */;
    if ((s32)r28 < (s32)r0) {
        if ((s32)r28 != (s32)0x0) {
            goto L_80064DB4;
        }
        if ((s32)r28 >= (s32)r3) goto L_80064DB4;
        }
    r28 = 0x0;
    L_80064DB4: ;
    r3 = (0x1 << 16);
    r4 = r28 & 0xFFFF;
    /* subi r0, r3, 0x2 */;
    if ((s32)r4 == (s32)r0) return;
    if ((s32)r4 < (s32)r0) {
        if ((s32)r4 == (s32)0x0) return;
    }
    r3 = r29;
    r4 = 0x0;
    r5 = 0x80;
    r6 = 0x3;
    fn_8012640C();
    r4 = r3;
    r3 = 0x34;
    fn_80132A38();
    r4 = *(u8*)((u8*)r30 + 0x8B);
    r0 = -0x100;
    r5 = *(s16*)((u8*)r31 + 0x54);
    r3 = 0x0;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r7 = r4 | r0;
    r4 = 0x0;
    r8 = 0xdf;
    fn_800FBB34();
    return;
    L_80064E1C: ;
    r4 = (u32)&lbl_803A9F08;
    r3 = 0x0;
    r4 = (u32)&lbl_803A9F08;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_8025D970();
    r4 = 0x0;
    r29 = r3;
    r5 = 0x7f;
    r6 = 0x0;
    fn_8012640C();
    r28 = r3 & 0xFFFF;
    r3 = r29;
    r4 = 0x0;
    fn_80123CD4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r28 = 0x0;
        goto L_80064E90;
    }
    r3 = (0x1 << 16);
    /* subi r0, r3, 0x2 */;
    if ((s32)r28 < (s32)r0) {
        if ((s32)r28 != (s32)0x0) {
            goto L_80064E90;
        }
        if ((s32)r28 >= (s32)r3) goto L_80064E90;
        }
    r28 = 0x0;
    L_80064E90: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 == (u32)0xffff) {
        r0 = 0xa5;
    }
    if ((u32)r0 == (u32)0x0) return;
    r3 = r0 & 0xFFFF;
    fn_8011CA34();
    fn_8011C9EC();
    r4 = (0x1 << 16);
    r5 = r3 & 0xFF;
    /* subi r0, r4, 0x2 */;
    if ((s32)r5 == (s32)r0) return;
    r3 = (u32)&lbl_802EDB40;
    r0 = r5 << 1;
    r3 = (u32)&lbl_802EDB40;
    r5 = r30;
    r6 = *(u16*)(r3 + r0);
    r3 = 0x0;
    r4 = 0x0;
    r7 = 0x0;
    fn_801040F0();
    return;
    L_80064EF0: ;
    r4 = (u32)&lbl_803A9F08;
    r3 = 0x0;
    r4 = (u32)&lbl_803A9F08;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_8025D970();
    r4 = 0x0;
    r29 = r3;
    r5 = 0x7f;
    r6 = 0x1;
    fn_8012640C();
    r28 = r3 & 0xFFFF;
    r3 = r29;
    r4 = 0x1;
    fn_80123CD4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r28 = 0x0;
        goto L_80064F64;
    }
    r3 = (0x1 << 16);
    /* subi r0, r3, 0x2 */;
    if ((s32)r28 < (s32)r0) {
        if ((s32)r28 != (s32)0x0) {
            goto L_80064F64;
        }
        if ((s32)r28 >= (s32)r3) goto L_80064F64;
        }
    r28 = 0x0;
    L_80064F64: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 == (u32)0xffff) {
        r0 = 0xa5;
    }
    if ((u32)r0 == (u32)0x0) return;
    r3 = r0 & 0xFFFF;
    fn_8011CA34();
    fn_8011C9EC();
    r4 = (0x1 << 16);
    r5 = r3 & 0xFF;
    /* subi r0, r4, 0x2 */;
    if ((s32)r5 == (s32)r0) return;
    r3 = (u32)&lbl_802EDB40;
    r0 = r5 << 1;
    r3 = (u32)&lbl_802EDB40;
    r5 = r30;
    r6 = *(u16*)(r3 + r0);
    r3 = 0x0;
    r4 = 0x0;
    r7 = 0x0;
    fn_801040F0();
    return;
    L_80064FC4: ;
    r4 = (u32)&lbl_803A9F08;
    r3 = 0x0;
    r4 = (u32)&lbl_803A9F08;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_8025D970();
    r4 = 0x0;
    r29 = r3;
    r5 = 0x7f;
    r6 = 0x2;
    fn_8012640C();
    r28 = r3 & 0xFFFF;
    r3 = r29;
    r4 = 0x2;
    fn_80123CD4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r28 = 0x0;
        goto L_80065038;
    }
    r3 = (0x1 << 16);
    /* subi r0, r3, 0x2 */;
    if ((s32)r28 < (s32)r0) {
        if ((s32)r28 != (s32)0x0) {
            goto L_80065038;
        }
        if ((s32)r28 >= (s32)r3) goto L_80065038;
        }
    r28 = 0x0;
    L_80065038: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 == (u32)0xffff) {
        r0 = 0xa5;
    }
    if ((u32)r0 == (u32)0x0) return;
    r3 = r0 & 0xFFFF;
    fn_8011CA34();
    fn_8011C9EC();
    r4 = (0x1 << 16);
    r5 = r3 & 0xFF;
    /* subi r0, r4, 0x2 */;
    if ((s32)r5 == (s32)r0) return;
    r3 = (u32)&lbl_802EDB40;
    r0 = r5 << 1;
    r3 = (u32)&lbl_802EDB40;
    r5 = r30;
    r6 = *(u16*)(r3 + r0);
    r3 = 0x0;
    r4 = 0x0;
    r7 = 0x0;
    fn_801040F0();
    return;
    L_80065098: ;
    r4 = (u32)&lbl_803A9F08;
    r3 = 0x0;
    r4 = (u32)&lbl_803A9F08;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_8025D970();
    r4 = 0x0;
    r29 = r3;
    r5 = 0x7f;
    r6 = 0x3;
    fn_8012640C();
    r28 = r3 & 0xFFFF;
    r3 = r29;
    r4 = 0x3;
    fn_80123CD4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r28 = 0x0;
        goto L_8006510C;
    }
    r3 = (0x1 << 16);
    /* subi r0, r3, 0x2 */;
    if ((s32)r28 < (s32)r0) {
        if ((s32)r28 != (s32)0x0) {
            goto L_8006510C;
        }
        if ((s32)r28 >= (s32)r3) goto L_8006510C;
        }
    r28 = 0x0;
    L_8006510C: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 == (u32)0xffff) {
        r0 = 0xa5;
    }
    if ((u32)r0 == (u32)0x0) return;
    r3 = r0 & 0xFFFF;
    fn_8011CA34();
    fn_8011C9EC();
    r4 = (0x1 << 16);
    r5 = r3 & 0xFF;
    /* subi r0, r4, 0x2 */;
    if ((s32)r5 == (s32)r0) return;
    r3 = (u32)&lbl_802EDB40;
    r0 = r5 << 1;
    r3 = (u32)&lbl_802EDB40;
    r5 = r30;
    r6 = *(u16*)(r3 + r0);
    r3 = 0x0;
    r4 = 0x0;
    r7 = 0x0;
    fn_801040F0();
    return;
    L_8006516C: ;
    r4 = (u32)&lbl_803A9F08;
    r3 = 0x0;
    r4 = (u32)&lbl_803A9F08;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_8025D970();
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x16;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFFFF;
    r3 = (u32)&lbl_802ED9FC;
    r0 = r0 << 1;
    r5 = r30;
    r4 = (u32)&lbl_802ED9FC;
    r3 = 0x0;
    r6 = *(u16*)(r4 + r0);
    r4 = 0x0;
    r7 = 0x0;
    fn_801040F0();
    return;
    L_800651D0: ;
    r4 = (u32)&lbl_803A9F08;
    r3 = 0x0;
    r4 = (u32)&lbl_803A9F08;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_8025D970();
    r4 = 0x0;
    r29 = r3;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x16;
    r6 = 0x0;
    fn_8012640C();
    r31 = r3 & 0xFFFF;
    r3 = r29;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x16;
    r6 = 0x1;
    fn_8012640C();
    r0 = r3 & 0xFFFF;
    if ((u32)r31 == (u32)r0) return;
    r3 = (u32)&lbl_802ED9FC;
    r0 = r0 << 1;
    r3 = (u32)&lbl_802ED9FC;
    r5 = r30;
    r6 = *(u16*)(r3 + r0);
    r3 = 0x0;
    r4 = 0x0;
    r7 = 0x0;
    fn_801040F0();
    return;
    L_8006526C: ;
    r4 = (u32)&lbl_803A9F08;
    r3 = 0x0;
    r4 = (u32)&lbl_803A9F08;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_8025D970();
    r4 = *(u8*)((u8*)r30 + 0x8B);
    r0 = -0x100;
    r29 = r4 | r0;
    fn_801230E0();
    r4 = r3 & 0xFFFF;
    if ((u32)r4 == (u32)0x0) return;
    r3 = 0x2d;
    fn_80132A38();
    r5 = *(s16*)((u8*)r31 + 0x54);
    r7 = r29;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r3 = 0x0;
    r4 = 0x0;
    r8 = 0x30da;
    fn_800FBB34();
    return;
    L_800652C4: ;
    r4 = (u32)&lbl_803A9F08;
    r3 = 0x0;
    r4 = (u32)&lbl_803A9F08;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_8025D970();
    r6 = *(u8*)((u8*)r30 + 0x8B);
    r0 = -0x100;
    r4 = 0x0;
    r5 = 0x88;
    r29 = r6 | r0;
    r6 = 0x0;
    fn_8012640C();
    r4 = (s16)r3;
    r3 = 0x34;
    fn_80132A38();
    r5 = *(s16*)((u8*)r31 + 0x54);
    r7 = r29;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r3 = 0x0;
    r4 = 0x0;
    r8 = 0xdf;
    fn_800FBB34();
    return;
    L_80065320: ;
    r4 = (u32)&lbl_803A9F08;
    r3 = 0x0;
    r4 = (u32)&lbl_803A9F08;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_8025D970();
    r6 = *(u8*)((u8*)r30 + 0x8B);
    r0 = -0x100;
    r4 = 0x0;
    r5 = 0x89;
    r29 = r6 | r0;
    r6 = 0x0;
    fn_8012640C();
    r4 = (s16)r3;
    r3 = 0x34;
    fn_80132A38();
    r5 = *(s16*)((u8*)r31 + 0x54);
    r7 = r29;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r3 = 0x0;
    r4 = 0x0;
    r8 = 0xdf;
    fn_800FBB34();
    return;
    L_8006537C: ;
    r4 = (u32)&lbl_803A9F08;
    r3 = 0x0;
    r4 = (u32)&lbl_803A9F08;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_8025D970();
    r6 = *(u8*)((u8*)r30 + 0x8B);
    r0 = -0x100;
    r4 = 0x0;
    r5 = 0x8a;
    r29 = r6 | r0;
    r6 = 0x0;
    fn_8012640C();
    r4 = (s16)r3;
    r3 = 0x34;
    fn_80132A38();
    r5 = *(s16*)((u8*)r31 + 0x54);
    r7 = r29;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r3 = 0x0;
    r4 = 0x0;
    r8 = 0xdf;
    fn_800FBB34();
    return;
    L_800653D8: ;
    r4 = (u32)&lbl_803A9F08;
    r3 = 0x0;
    r4 = (u32)&lbl_803A9F08;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_8025D970();
    r6 = *(u8*)((u8*)r30 + 0x8B);
    r0 = -0x100;
    r4 = 0x0;
    r5 = 0x8b;
    r29 = r6 | r0;
    r6 = 0x0;
    fn_8012640C();
    r4 = (s16)r3;
    r3 = 0x34;
    fn_80132A38();
    r5 = *(s16*)((u8*)r31 + 0x54);
    r7 = r29;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r3 = 0x0;
    r4 = 0x0;
    r8 = 0xdf;
    fn_800FBB34();
    return;
    L_80065434: ;
    r4 = (u32)&lbl_803A9F08;
    r3 = 0x0;
    r4 = (u32)&lbl_803A9F08;
    r4 = *(u32*)((u8*)r4 + 0xC);
    fn_8025D970();
    r6 = *(u8*)((u8*)r30 + 0x8B);
    r0 = -0x100;
    r4 = 0x0;
    r5 = 0x8c;
    r29 = r6 | r0;
    r6 = 0x0;
    fn_8012640C();
    r4 = (s16)r3;
    r3 = 0x34;
    fn_80132A38();
    r5 = *(s16*)((u8*)r31 + 0x54);
    r7 = r29;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r3 = 0x0;
    r4 = 0x0;
    r8 = 0xdf;
    fn_800FBB34();
    return;
    L_80065490: ;
    r3 = (u32)&lbl_803A9F08;
    r3 = (u32)&lbl_803A9F08;
    r29 = *(u32*)((u8*)r3 + 0xC);
    fn_8025DA88();
    r0 = r29 * 0xc;
    r3 = (u32)&lbl_803A9F08;
    r3 = (u32)&lbl_803A9F08;
    r3 = r3 + r0;
    r3 = r3 + 0x30;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r30;
    r4 = r31;
    fn_8010B9E8();
    return;
    L_800654D4: ;
    r3 = (u32)&lbl_803A9F08;
    r3 = (u32)&lbl_803A9F08;
    r29 = *(u32*)((u8*)r3 + 0xC);
    fn_8025DA88();
    r4 = r29;
    r3 = 0x0;
    fn_8025D970();
    fn_801230E0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x0) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    L_80065528: ;
    r5 = 0x6;
    fn_80060EF4();
    return;
    L_80065534: ;
    r5 = 0x6;
    fn_80060EF4();
    return;
    L_80065540: ;
    r5 = -0x1;
    fn_80060EF4();
    return;
    L_8006554C: ;
    r5 = 0x3;
    fn_80060EF4();
    return;
    L_80065558: ;
    r5 = 0x4;
    fn_80060EF4();
    return;
    L_80065564: ;
    r5 = 0x2;
    fn_80060EF4();
    return;
    L_80065570: ;
    r5 = 0x1;
    fn_80060EF4();
    return;
    L_8006557C: ;
    r5 = 0x0;
    fn_80060EF4();
    return;
    L_80065588: ;
    r3 = (u32)&lbl_802EF0A8;
    r3 = (u32)&lbl_802EF0A8;
    r28 = r3 + (0x2 << 16);
    fn_8025DAD0();
    if ((s32)r3 == (s32)0x0) {
        r3 = 0x3db4;
        fn_800FA280();
    } else {

        ((void(*)(void))fn_8006B1D4)();
        r4 = r3;
        r3 = 0x2f;
        fn_80132A38();
        r3 = 0x3c1e;
        fn_800FA280();
    }
    r4 = r3;
    r3 = 0x37;
    fn_80132A38();
    r5 = *(s16*)((u8*)r31 + 0x50);
    r0 = -0x100;
    r3 = *(s16*)((u8*)r28 + (-11762));
    r8 = 0xcf;
    r4 = *(u8*)((u8*)r30 + 0x8B);
    r3 = r3 - r5;
    r10 = *(s16*)((u8*)r31 + 0x52);
    r9 = *(s16*)((u8*)r28 + (-11760));
    /* subi r3, r3, 0x12 */;
    r5 = *(s16*)((u8*)r28 + (-11758));
    r7 = r4 | r0;
    r6 = *(s16*)((u8*)r28 + (-11756));
    r4 = r9 - r10;
    fn_800FBB34();

    return;
}


/* 0x80065628 | size: 0x108 */
void fn_80065628(void) {
    extern void fn_80065A48();
    extern void fn_8025DA18();
    extern void fn_8025DA88();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;


    r29 = r3;
    r30 = r4;
    fn_8025DA88();
    switch ((s32)r3) {
        case 0:
        case 1:
            r31 = 0x3;
            break;
        case 2:
            r31 = 0x3;
            break;
        default:
            r31 = 0x3;
            break;
    }
    r3 = (u32)&lbl_803A9F08;
    r3 = (u32)&lbl_803A9F08;
    r0 = *(u32*)((u8*)r3 + 0x154);
    if ((s32)r0 != (s32)0x2) {
        r0 = *(u8*)((u8*)r30 + 0x4);
        r0 = r0 & 0xFFFFFFFD;
        r0 = (s8)r0;
        *(u8*)((u8*)r30 + 0x4) = r0;
    }
    r3 = r29;
    r4 = r30;
    r5 = 0x3;
    fn_80065A48();
    r3 = r31;
    fn_8025DA18();
    r3 = r3 & 0xFFFF;
    r0 = *(s16*)((u8*)r30 + 0x6);
    r4 = r3 * 0x3;
    r3 = (u32)&lbl_802ED9F0;
    r3 = (u32)&lbl_802ED9F0;
    r3 = r3 + r4;
    if ((s32)r0 == (s32)0xb73 || (s32)r0 == (s32)0xb92 ||
        (s32)r0 == (s32)0xbb1 || (s32)r0 == (s32)0xbd0) {
        r0 = *(u8*)((u8*)r3 + 0x0);
        *(u8*)((u8*)r30 + 0x64) = r0;
        r0 = *(u8*)((u8*)r3 + 0x1);
        *(u8*)((u8*)r30 + 0x65) = r0;
        r0 = *(u8*)((u8*)r3 + 0x2);
        *(u8*)((u8*)r30 + 0x66) = r0;
    }

    return;
}


/* 0x80065730 | size: 0x108 */
void fn_80065730(void) {
    extern void fn_80065A48();
    extern void fn_8025DA18();
    extern void fn_8025DA88();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;


    r29 = r3;
    r30 = r4;
    fn_8025DA88();
    switch ((s32)r3) {
        case 0:
        case 1:
            r31 = 0x1;
            break;
        case 2:
            r31 = 0x2;
            break;
        default:
            r31 = 0x2;
            break;
    }
    r3 = (u32)&lbl_803A9F08;
    r3 = (u32)&lbl_803A9F08;
    r0 = *(u32*)((u8*)r3 + 0x154);
    if ((s32)r0 != (s32)0x2) {
        r0 = *(u8*)((u8*)r30 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r30 + 0x4) = r0;
    }
    r3 = r29;
    r4 = r30;
    r5 = 0x2;
    fn_80065A48();
    r3 = r31;
    fn_8025DA18();
    r3 = r3 & 0xFFFF;
    r0 = *(s16*)((u8*)r30 + 0x6);
    r4 = r3 * 0x3;
    r3 = (u32)&lbl_802ED9F0;
    r3 = (u32)&lbl_802ED9F0;
    r3 = r3 + r4;
    if ((s32)r0 == (s32)0xb73 || (s32)r0 == (s32)0xb92 ||
        (s32)r0 == (s32)0xbb1 || (s32)r0 == (s32)0xbd0) {
        r0 = *(u8*)((u8*)r3 + 0x0);
        *(u8*)((u8*)r30 + 0x64) = r0;
        r0 = *(u8*)((u8*)r3 + 0x1);
        *(u8*)((u8*)r30 + 0x65) = r0;
        r0 = *(u8*)((u8*)r3 + 0x2);
        *(u8*)((u8*)r30 + 0x66) = r0;
    }

    return;
}


/* 0x80065838 | size: 0x108 */
void fn_80065838(void) {
    extern void fn_80065A48();
    extern void fn_8025DA18();
    extern void fn_8025DA88();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;


    r29 = r3;
    r30 = r4;
    fn_8025DA88();
    switch ((s32)r3) {
        case 0:
        case 1:
            r31 = 0x2;
            break;
        case 2:
            r31 = 0x1;
            break;
        default:
            r31 = 0x1;
            break;
    }
    r3 = (u32)&lbl_803A9F08;
    r3 = (u32)&lbl_803A9F08;
    r0 = *(u32*)((u8*)r3 + 0x154);
    if ((s32)r0 != (s32)0x2) {
        r0 = *(u8*)((u8*)r30 + 0x4);
        r0 = r0 & 0xFFFFFFFD;
        r0 = (s8)r0;
        *(u8*)((u8*)r30 + 0x4) = r0;
    }
    r3 = r29;
    r4 = r30;
    r5 = 0x1;
    fn_80065A48();
    r3 = r31;
    fn_8025DA18();
    r3 = r3 & 0xFFFF;
    r0 = *(s16*)((u8*)r30 + 0x6);
    r4 = r3 * 0x3;
    r3 = (u32)&lbl_802ED9F0;
    r3 = (u32)&lbl_802ED9F0;
    r3 = r3 + r4;
    if ((s32)r0 == (s32)0xb73 || (s32)r0 == (s32)0xb92 ||
        (s32)r0 == (s32)0xbb1 || (s32)r0 == (s32)0xbd0) {
        r0 = *(u8*)((u8*)r3 + 0x0);
        *(u8*)((u8*)r30 + 0x64) = r0;
        r0 = *(u8*)((u8*)r3 + 0x1);
        *(u8*)((u8*)r30 + 0x65) = r0;
        r0 = *(u8*)((u8*)r3 + 0x2);
        *(u8*)((u8*)r30 + 0x66) = r0;
    }

    return;
}


/* 0x80065940 | size: 0x108 */
void fn_80065940(void) {
    extern void fn_80065A48();
    extern void fn_8025DA18();
    extern void fn_8025DA88();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;


    r29 = r3;
    r30 = r4;
    fn_8025DA88();
    switch ((s32)r3) {
        case 0:
        case 1:
            r31 = 0x0;
            break;
        case 2:
            r31 = 0x0;
            break;
        default:
            r31 = 0x0;
            break;
    }
    r3 = (u32)&lbl_803A9F08;
    r3 = (u32)&lbl_803A9F08;
    r0 = *(u32*)((u8*)r3 + 0x154);
    if ((s32)r0 != (s32)0x2) {
        r0 = *(u8*)((u8*)r30 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r30 + 0x4) = r0;
    }
    r3 = r29;
    r4 = r30;
    r5 = 0x0;
    fn_80065A48();
    r3 = r31;
    fn_8025DA18();
    r3 = r3 & 0xFFFF;
    r0 = *(s16*)((u8*)r30 + 0x6);
    r4 = r3 * 0x3;
    r3 = (u32)&lbl_802ED9F0;
    r3 = (u32)&lbl_802ED9F0;
    r3 = r3 + r4;
    if ((s32)r0 == (s32)0xb73 || (s32)r0 == (s32)0xb92 ||
        (s32)r0 == (s32)0xbb1 || (s32)r0 == (s32)0xbd0) {
        r0 = *(u8*)((u8*)r3 + 0x0);
        *(u8*)((u8*)r30 + 0x64) = r0;
        r0 = *(u8*)((u8*)r3 + 0x1);
        *(u8*)((u8*)r30 + 0x65) = r0;
        r0 = *(u8*)((u8*)r3 + 0x2);
        *(u8*)((u8*)r30 + 0x66) = r0;
    }

    return;
}


/* 0x80065A48 | size: 0x1CA4 */
s32 fn_80065A48(void) {
    extern void fn_80068794();
    extern void fn_800688C4();
    extern void fn_800689FC();
    extern void fn_80068BB0();
    extern void fn_80068DBC();
    extern void fn_8010B9E8();
    extern void fn_8025D89C();
    extern void fn_8025D9F0();
    extern void fn_8025DA88();
    extern u8 jumptable_802EDB7C[];
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
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    
    r26 = r3;
    r31 = r4;
    fn_8025DA88();
    if ((s32)r3 != (s32)0x2) {
        if ((s32)r3 >= (s32)0x2) goto L_80065AA4;
        if ((s32)r3 < (s32)0x0) {
            goto L_80065AA4;
        }
        r30 = 0x0;
        r29 = 0x1;
        r28 = 0x2;
        r27 = 0x3;
        goto L_80065AB4;
    }
    r30 = 0x0;
    r28 = 0x1;
    r29 = 0x2;
    r27 = 0x3;
    goto L_80065AB4;
    L_80065AA4: ;
    r30 = 0x0;
    r28 = 0x1;
    r29 = 0x2;
    r27 = 0x3;
    L_80065AB4: ;
    r3 = *(s16*)((u8*)r31 + 0x6);
    /* subi r0, r3, 0xb3b */;
    if ((u32)r0 > (u32)0xb5) return;
    r3 = (u32)jumptable_802EDB7C;
    r0 = r0 << 2;
    r3 = (u32)jumptable_802EDB7C;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r27 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80065B08;

    if ((s32)r3 < (s32)0x0) {
        goto L_80065B08;
    }
    if ((s32)r30 < (s32)0x2) goto L_80065B08;
    r27 = 0x0;
    L_80065B08: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r30 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x30;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r27 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80065B74;

    if ((s32)r3 < (s32)0x0) {
        goto L_80065B74;
    }
    if ((s32)r30 < (s32)0x2) goto L_80065B74;
    r27 = 0x0;
    L_80065B74: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r30 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x3c;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r27 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80065BE0;

    if ((s32)r3 < (s32)0x0) {
        goto L_80065BE0;
    }
    if ((s32)r30 < (s32)0x2) goto L_80065BE0;
    r27 = 0x0;
    L_80065BE0: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r30 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x48;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r27 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80065C4C;

    if ((s32)r3 < (s32)0x0) {
        goto L_80065C4C;
    }
    if ((s32)r30 < (s32)0x2) goto L_80065C4C;
    r27 = 0x0;
    L_80065C4C: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r30 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x54;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r27 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80065CB8;

    if ((s32)r3 < (s32)0x0) {
        goto L_80065CB8;
    }
    if ((s32)r30 < (s32)0x2) goto L_80065CB8;
    r27 = 0x0;
    L_80065CB8: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r30 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x60;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r27 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80065D24;

    if ((s32)r3 < (s32)0x0) {
        goto L_80065D24;
    }
    if ((s32)r30 < (s32)0x2) goto L_80065D24;
    r27 = 0x0;
    L_80065D24: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r30 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x6c;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r27 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80065D90;

    if ((s32)r3 < (s32)0x0) {
        goto L_80065D90;
    }
    if ((s32)r28 < (s32)0x2) goto L_80065D90;
    r27 = 0x0;
    L_80065D90: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r28 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x30;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r27 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80065DFC;

    if ((s32)r3 < (s32)0x0) {
        goto L_80065DFC;
    }
    if ((s32)r28 < (s32)0x2) goto L_80065DFC;
    r27 = 0x0;
    L_80065DFC: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r28 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x3c;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r27 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80065E68;

    if ((s32)r3 < (s32)0x0) {
        goto L_80065E68;
    }
    if ((s32)r28 < (s32)0x2) goto L_80065E68;
    r27 = 0x0;
    L_80065E68: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r28 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x48;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r27 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80065ED4;

    if ((s32)r3 < (s32)0x0) {
        goto L_80065ED4;
    }
    if ((s32)r28 < (s32)0x2) goto L_80065ED4;
    r27 = 0x0;
    L_80065ED4: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r28 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x54;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r27 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80065F40;

    if ((s32)r3 < (s32)0x0) {
        goto L_80065F40;
    }
    if ((s32)r28 < (s32)0x2) goto L_80065F40;
    r27 = 0x0;
    L_80065F40: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r28 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x60;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r27 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80065FAC;

    if ((s32)r3 < (s32)0x0) {
        goto L_80065FAC;
    }
    if ((s32)r28 < (s32)0x2) goto L_80065FAC;
    r27 = 0x0;
    L_80065FAC: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r28 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x6c;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r27 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80066018;

    if ((s32)r3 < (s32)0x0) {
        goto L_80066018;
    }
    if ((s32)r29 < (s32)0x2) goto L_80066018;
    r27 = 0x0;
    L_80066018: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r29 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x30;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r27 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80066084;

    if ((s32)r3 < (s32)0x0) {
        goto L_80066084;
    }
    if ((s32)r29 < (s32)0x2) goto L_80066084;
    r27 = 0x0;
    L_80066084: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r29 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x3c;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r27 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_800660F0;

    if ((s32)r3 < (s32)0x0) {
        goto L_800660F0;
    }
    if ((s32)r29 < (s32)0x2) goto L_800660F0;
    r27 = 0x0;
    L_800660F0: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r29 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x48;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r27 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_8006615C;

    if ((s32)r3 < (s32)0x0) {
        goto L_8006615C;
    }
    if ((s32)r29 < (s32)0x2) goto L_8006615C;
    r27 = 0x0;
    L_8006615C: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r29 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x54;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r27 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_800661C8;

    if ((s32)r3 < (s32)0x0) {
        goto L_800661C8;
    }
    if ((s32)r29 < (s32)0x2) goto L_800661C8;
    r27 = 0x0;
    L_800661C8: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r29 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x60;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r27 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80066234;

    if ((s32)r3 < (s32)0x0) {
        goto L_80066234;
    }
    if ((s32)r29 < (s32)0x2) goto L_80066234;
    r27 = 0x0;
    L_80066234: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r29 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x6c;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r28 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_800662A0;

    if ((s32)r3 < (s32)0x0) {
        goto L_800662A0;
    }
    if ((s32)r27 < (s32)0x2) goto L_800662A0;
    r28 = 0x0;
    L_800662A0: ;
    r0 = r28 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r27 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x30;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r28 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_8006630C;

    if ((s32)r3 < (s32)0x0) {
        goto L_8006630C;
    }
    if ((s32)r27 < (s32)0x2) goto L_8006630C;
    r28 = 0x0;
    L_8006630C: ;
    r0 = r28 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r27 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x3c;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r28 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80066378;

    if ((s32)r3 < (s32)0x0) {
        goto L_80066378;
    }
    if ((s32)r27 < (s32)0x2) goto L_80066378;
    r28 = 0x0;
    L_80066378: ;
    r0 = r28 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r27 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x48;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r28 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_800663E4;

    if ((s32)r3 < (s32)0x0) {
        goto L_800663E4;
    }
    if ((s32)r27 < (s32)0x2) goto L_800663E4;
    r28 = 0x0;
    L_800663E4: ;
    r0 = r28 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r27 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x54;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r28 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80066450;

    if ((s32)r3 < (s32)0x0) {
        goto L_80066450;
    }
    if ((s32)r27 < (s32)0x2) goto L_80066450;
    r28 = 0x0;
    L_80066450: ;
    r0 = r28 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r27 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x60;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r28 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_800664BC;

    if ((s32)r3 < (s32)0x0) {
        goto L_800664BC;
    }
    if ((s32)r27 < (s32)0x2) goto L_800664BC;
    r28 = 0x0;
    L_800664BC: ;
    r0 = r28 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r4 = r27 * 0x48;
    r3 = (u32)&lbl_803A9F08;
    r0 = (u32)&lbl_803A9F08;
    r3 = r0 + r4;
    r3 = r3 + 0x6c;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) return;
    r5 = *(u16*)((u8*)r3 + 0x2);
    r3 = r26;
    r4 = r31;
    fn_8010B9E8();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r30;
    fn_80068DBC();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r30;
    r6 = 0x0;
    fn_80068BB0();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r30;
    fn_800689FC();
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80066568;

    if ((s32)r3 < (s32)0x0) {
        goto L_80066568;
    }
    if ((s32)r30 < (s32)0x2) goto L_80066568;
    r26 = 0x0;
    L_80066568: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r30;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x0) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_800665F8;

    if ((s32)r3 < (s32)0x0) {
        goto L_800665F8;
    }
    if ((s32)r30 < (s32)0x2) goto L_800665F8;
    r26 = 0x0;
    L_800665F8: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r30;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x1) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80066688;

    if ((s32)r3 < (s32)0x0) {
        goto L_80066688;
    }
    if ((s32)r30 < (s32)0x2) goto L_80066688;
    r26 = 0x0;
    L_80066688: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r30;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x2) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80066718;

    if ((s32)r3 < (s32)0x0) {
        goto L_80066718;
    }
    if ((s32)r30 < (s32)0x2) goto L_80066718;
    r26 = 0x0;
    L_80066718: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r30;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x3) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_800667A8;

    if ((s32)r3 < (s32)0x0) {
        goto L_800667A8;
    }
    if ((s32)r30 < (s32)0x2) goto L_800667A8;
    r26 = 0x0;
    L_800667A8: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r30;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x4) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80066838;

    if ((s32)r3 < (s32)0x0) {
        goto L_80066838;
    }
    if ((s32)r30 < (s32)0x2) goto L_80066838;
    r26 = 0x0;
    L_80066838: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r30;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x5) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r3 = r26;
    r4 = r31;
    r5 = r30;
    r6 = 0x0;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r30;
    r6 = 0x1;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r30;
    r6 = 0x2;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r30;
    r6 = 0x3;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r30;
    r6 = 0x4;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r30;
    r6 = 0x5;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r28;
    fn_80068DBC();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r28;
    r6 = 0x0;
    fn_80068BB0();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r28;
    fn_800689FC();
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80066998;

    if ((s32)r3 < (s32)0x0) {
        goto L_80066998;
    }
    if ((s32)r28 < (s32)0x2) goto L_80066998;
    r26 = 0x0;
    L_80066998: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r28;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x0) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80066A28;

    if ((s32)r3 < (s32)0x0) {
        goto L_80066A28;
    }
    if ((s32)r28 < (s32)0x2) goto L_80066A28;
    r26 = 0x0;
    L_80066A28: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r28;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x1) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80066AB8;

    if ((s32)r3 < (s32)0x0) {
        goto L_80066AB8;
    }
    if ((s32)r28 < (s32)0x2) goto L_80066AB8;
    r26 = 0x0;
    L_80066AB8: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r28;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x2) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80066B48;

    if ((s32)r3 < (s32)0x0) {
        goto L_80066B48;
    }
    if ((s32)r28 < (s32)0x2) goto L_80066B48;
    r26 = 0x0;
    L_80066B48: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r28;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x3) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80066BD8;

    if ((s32)r3 < (s32)0x0) {
        goto L_80066BD8;
    }
    if ((s32)r28 < (s32)0x2) goto L_80066BD8;
    r26 = 0x0;
    L_80066BD8: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r28;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x4) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80066C68;

    if ((s32)r3 < (s32)0x0) {
        goto L_80066C68;
    }
    if ((s32)r28 < (s32)0x2) goto L_80066C68;
    r26 = 0x0;
    L_80066C68: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r28;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x5) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r3 = r26;
    r4 = r31;
    r5 = r28;
    r6 = 0x0;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r28;
    r6 = 0x1;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r28;
    r6 = 0x2;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r28;
    r6 = 0x3;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r28;
    r6 = 0x4;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r28;
    r6 = 0x5;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r29;
    fn_80068DBC();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r29;
    r6 = 0x0;
    fn_80068BB0();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r29;
    fn_800689FC();
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80066DC8;

    if ((s32)r3 < (s32)0x0) {
        goto L_80066DC8;
    }
    if ((s32)r29 < (s32)0x2) goto L_80066DC8;
    r26 = 0x0;
    L_80066DC8: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r29;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x0) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80066E58;

    if ((s32)r3 < (s32)0x0) {
        goto L_80066E58;
    }
    if ((s32)r29 < (s32)0x2) goto L_80066E58;
    r26 = 0x0;
    L_80066E58: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r29;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x1) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80066EE8;

    if ((s32)r3 < (s32)0x0) {
        goto L_80066EE8;
    }
    if ((s32)r29 < (s32)0x2) goto L_80066EE8;
    r26 = 0x0;
    L_80066EE8: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r29;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x2) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80066F78;

    if ((s32)r3 < (s32)0x0) {
        goto L_80066F78;
    }
    if ((s32)r29 < (s32)0x2) goto L_80066F78;
    r26 = 0x0;
    L_80066F78: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r29;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x3) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80067008;

    if ((s32)r3 < (s32)0x0) {
        goto L_80067008;
    }
    if ((s32)r29 < (s32)0x2) goto L_80067008;
    r26 = 0x0;
    L_80067008: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r29;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x4) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80067098;

    if ((s32)r3 < (s32)0x0) {
        goto L_80067098;
    }
    if ((s32)r29 < (s32)0x2) goto L_80067098;
    r26 = 0x0;
    L_80067098: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r29;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x5) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r3 = r26;
    r4 = r31;
    r5 = r29;
    r6 = 0x0;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r29;
    r6 = 0x1;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r29;
    r6 = 0x2;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r29;
    r6 = 0x3;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r29;
    r6 = 0x4;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r29;
    r6 = 0x5;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r27;
    fn_80068DBC();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r27;
    r6 = 0x0;
    fn_80068BB0();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r27;
    fn_800689FC();
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_800671F8;

    if ((s32)r3 < (s32)0x0) {
        goto L_800671F8;
    }
    if ((s32)r27 < (s32)0x2) goto L_800671F8;
    r26 = 0x0;
    L_800671F8: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r27;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x0) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80067288;

    if ((s32)r3 < (s32)0x0) {
        goto L_80067288;
    }
    if ((s32)r27 < (s32)0x2) goto L_80067288;
    r26 = 0x0;
    L_80067288: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r27;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x1) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80067318;

    if ((s32)r3 < (s32)0x0) {
        goto L_80067318;
    }
    if ((s32)r27 < (s32)0x2) goto L_80067318;
    r26 = 0x0;
    L_80067318: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r27;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x2) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_800673A8;

    if ((s32)r3 < (s32)0x0) {
        goto L_800673A8;
    }
    if ((s32)r27 < (s32)0x2) goto L_800673A8;
    r26 = 0x0;
    L_800673A8: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r27;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x3) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80067438;

    if ((s32)r3 < (s32)0x0) {
        goto L_80067438;
    }
    if ((s32)r27 < (s32)0x2) goto L_80067438;
    r26 = 0x0;
    L_80067438: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r27;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x4) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r26 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_800674C8;

    if ((s32)r3 < (s32)0x0) {
        goto L_800674C8;
    }
    if ((s32)r27 < (s32)0x2) goto L_800674C8;
    r26 = 0x0;
    L_800674C8: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    ((void(*)(void))fn_8006B1D4)();
    r26 = r3 & 0xFFFF;
    r3 = r27;
    fn_8025D89C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r26) {
    } else {

        r0 = r26;
    }
    r0 = r0 & 0xFFFF;
    if ((s32)r0 > (s32)0x5) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r3 = r26;
    r4 = r31;
    r5 = r27;
    r6 = 0x0;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r27;
    r6 = 0x1;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r27;
    r6 = 0x2;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r27;
    r6 = 0x3;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r27;
    r6 = 0x4;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r27;
    r6 = 0x5;
    fn_80068794();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r30;
    r6 = 0x0;
    fn_800688C4();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r28;
    r6 = 0x1;
    fn_800688C4();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r29;
    r6 = 0x2;
    fn_800688C4();
    return;
    r3 = r26;
    r4 = r31;
    r5 = r27;
    r6 = 0x3;
    fn_800688C4();
    return;
    r3 = 0x0;
    fn_8025D9F0();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0x0) {
    } else {

        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r3 = 0x0;
    fn_8025D9F0();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0x0) {
    } else {

        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;
    return;
    r3 = 0x0;
    fn_8025D9F0();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0x0) {
    } else {

        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;

    return;
}


/* 0x800676EC | size: 0x2D4 */
s32 fn_800676EC(void) {
    extern void fn_800679C0();
    extern void fn_8006905C();
    extern void fn_800F7BC4();
    extern void fn_80102ED4();
    extern void fn_8025D2B0();
    extern void fn_8025D89C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    
    r31 = r3;
    r3 = 0x1;
    fn_800F7BC4();
    r0 = r3 & 0x00000020;
    if ((u32)r0 != (u32)0x0) {
        r3 = 0x0;
        fn_8025D2B0();
        if ((s32)r3 != (s32)0x1) goto L_80067948;
        r3 = (u32)&lbl_803A9F08;
        r3 = (u32)&lbl_803A9F08;
        r0 = *(u32*)((u8*)r3 + 0x0);
        if ((s32)r0 != (s32)0x2) {
            if ((s32)r0 < (s32)0x2) {
                if ((s32)r0 != (s32)0x0) {
                    if ((s32)r0 < (s32)0x0) {
                        goto L_80067948;
                    }
                    goto L_80067948;
                    }
                r0 = *(u8*)((u8*)r3 + 0x4);
                if ((u32)r0 == (u32)0x0) {
                    r3 = 0x1;
                    fn_800F7BC4();
                    r30 = r3;
                    r29 = -0x1;
                    r3 = 0x0;
                    fn_8025D89C();
                    r0 = r30 & 0x1;
                    r3 = r3 & 0xFFFF;
                    if ((u32)r0 != (u32)0x0) {
                        r29 = 0x0;
                    }
                    r0 = r30 & 0x00000008;
                    if ((u32)r0 != (u32)0x0) {
                        r29 = 0x1;
                    }
                    r0 = r30 & 0x00000800;
                    if ((u32)r0 != (u32)0x0) {
                        r29 = 0x2;
                    }
                    r0 = r30 & 0x00000004;
                    if ((u32)r0 != (u32)0x0) {
                        r29 = 0x3;
                    }
                    r0 = r30 & 0x00000002;
                    if ((u32)r0 != (u32)0x0) {
                        r29 = 0x4;
                    }
                    r0 = r30 & 0x00000400;
                    if ((u32)r0 != (u32)0x0) {
                        r29 = 0x5;
                    }
                    if ((s32)r3 <= (s32)r29) {
                        r29 = -0x1;
                    }
                    if ((s32)r29 >= (s32)0x0) {
                        r3 = 0x0;
                        fn_8025D89C();
                        r0 = r3 & 0xFFFF;
                        if ((s32)r29 < (s32)r0) {
                            r0 = 0x0;
                            r3 = (u32)&lbl_803A9F08;
                            *(u8*)((u8*)r31 + 0x95) = r0;
                            r0 = 0x1;
                            r3 = (u32)&lbl_803A9F08;
                            *(u8*)((u8*)r31 + 0x98) = r0;
                            *(u32*)((u8*)r3 + 0xC) = r29;
                }
                }
                }
                r3 = r31;
                r4 = 0x1;
                fn_800679C0();
                goto L_80067948;
                    }
            r3 = r31;
            r4 = 0x1;
            fn_800679C0();
            goto L_80067948;
        }
        r3 = 0x1;
        fn_800F7BC4();
        r4 = (u32)&lbl_803A9F08;
        r5 = 0x0;
        r4 = (u32)&lbl_803A9F08;
        r0 = *(u32*)((u8*)r4 + 0xC);
        if ((s32)r0 != (s32)0x3) {
            if ((s32)r0 < (s32)0x3) {
                if ((s32)r0 != (s32)0x1) {
                    if ((s32)r0 < (s32)0x1) {
                        if ((s32)r0 < (s32)0x0) {
                            goto L_800678C0;
                        }
                        if ((s32)r0 != (s32)0x5) {
                            if ((s32)r0 >= (s32)0x5) goto L_800678C0;
                            goto L_800678B4;
                            }
                        r5 = 0x1;
                        goto L_800678C0;
                        }
                    r5 = 0x8;
                    goto L_800678C0;
                        }
                r5 = 0x800;
                goto L_800678C0;
            }
            r5 = 0x4;
            goto L_800678C0;
            L_800678B4: ;
            r5 = 0x2;
            goto L_800678C0;
                        }
        r5 = 0x400;
        L_800678C0: ;
        r0 = r3 & r5;
        if ((u32)r0 == (u32)0x0) {
            r0 = 0x1;
            *(u8*)((u8*)r31 + 0x98) = r0;
        }
        r3 = r31;
        r4 = 0x1;
        fn_800679C0();
        goto L_80067948;
    }
    r3 = (u32)&lbl_803A9F08;
    r3 = (u32)&lbl_803A9F08;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((s32)r0 != (s32)0x2) {
        if ((s32)r0 < (s32)0x2) {
            if ((s32)r0 != (s32)0x0) {
                if ((s32)r0 < (s32)0x0) {
                    goto L_80067948;
                }
                goto L_80067948;
                }
            r3 = r31;
            r4 = 0x0;
            fn_800679C0();
            goto L_80067948;
                }
        r3 = r31;
        r4 = 0x1;
        fn_800679C0();
        goto L_80067948;
    }
    r3 = r31;
    r4 = 0x1;
    fn_800679C0();
    r0 = 0x1;
    *(u8*)((u8*)r31 + 0x98) = r0;
    L_80067948: ;
    fn_8006905C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) {
        r0 = 0x1;
        *(u8*)((u8*)r31 + 0x98) = r0;
        *(u8*)((u8*)r31 + 0x99) = r0;
    }
    r3 = (u32)&lbl_803A9F08;
    r3 = (u32)&lbl_803A9F08;
    r3 = r3 + (0x1 << 16);
    r0 = *(u8*)((u8*)r3 + (-12712));
    if ((u32)r0 == (u32)0x0) {
        r0 = 0x1;
        *(u8*)((u8*)r31 + 0x98) = r0;
        *(u8*)((u8*)r31 + 0x99) = r0;
    }
    r3 = (u32)&lbl_803A9F08;
    r3 = (u32)&lbl_803A9F08;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((s32)r0 == (s32)0x1) {
        r3 = r31;
        fn_80102ED4();
    }
    return;
}


/* 0x800679C0 | size: 0x7F4 */
s32 fn_800679C0(void) {
    extern void fn_800681B4();
    extern void fn_8008ABA0();
    extern void fn_800F7AF0();
    extern void fn_800F7BC4();
    extern void fn_80166AB8();
    extern void fn_8025D2B0();
    extern void fn_8025D560();
    extern void fn_8025D584();
    extern void fn_8025D644();
    extern void fn_8025D89C();
    extern void fn_8025D9CC();
    extern void fn_8025D9F0();
    extern void fn_8025DA18();
    extern void fn_8025DA3C();
    extern void fn_8025DA88();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    
    r27 = r3;
    r24 = r4;
    fn_8025DA3C();
    r30 = r3;
    fn_8025DA88();
    r3 = 0x0;
    fn_8025D9F0();
    r3 = 0x1;
    fn_8025D9F0();
    r3 = 0x2;
    fn_8025D9F0();
    r3 = 0x3;
    fn_8025D9F0();
    if ((s32)r24 != (s32)0x0) {
        r28 = 0x1;
    } else {

        r28 = 0x0;
    }
    r3 = (u32)&lbl_803A9F08;
    r31 = (u32)&lbl_803A9F08;
    r29 = r31 + (0x1 << 16);
    r26 = r28 * 0x30;
    while (1) {
        if ((s32)r28 >= (s32)r30) break;
        fn_8025D9CC();
        if ((s32)r3 != (s32)0x4) goto L_80067AA8;
        r3 = (u32)&lbl_803A9F08;
        r24 = r28;
        r0 = (u32)&lbl_803A9F08;
        r3 = r0 + r28;
        r0 = *(u8*)((u8*)r3 + 0x4);
        if ((u32)r0 == (u32)0x0) goto L_80067AA8;
        r3 = r28;
        fn_8025D2B0();
        ((void(*)(void))fn_8006AFE4)();
        r0 = *(u32*)((u8*)r3 + 0x4);

        if ((s32)r0 != (s32)0x1 && (s32)r0 != (s32)0x2) goto L_80067AA8;

        r3 = r24;
        fn_8025D2B0();
        r25 = r3;
        fn_8008ABA0();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x0) goto L_80067AA8;
        r0 = 0x0;
        *(u8*)((u8*)r29 + (-12712)) = r0;
        r0 = *(u32*)((u8*)r29 + (-12708));
        if ((s32)r0 >= (s32)0x0) goto L_80067AA8;
        *(u32*)((u8*)r29 + (-12708)) = r25;
        L_80067AA8: ;
        r3 = r31 + r28;
        r0 = *(u8*)((u8*)r3 + 0x4);
        if ((u32)r0 != (u32)0x0) goto L_80068190;
        fn_8025D9CC();
        if ((s32)r3 == (s32)0x4) {
            r3 = r28;
            fn_8025D2B0();
            ((void(*)(void))fn_8006AFE4)();
            r0 = *(u32*)((u8*)r3 + 0x4);

            if ((s32)r0 != (s32)0x1 && (s32)r0 != (s32)0x2) goto L_80067AF8;

            r3 = r28;
            fn_8025D2B0();
            r4 = r28;
            fn_800681B4();
            goto L_800680EC;
            L_80067AF8: ;
            r3 = r28;
            fn_8025D2B0();
            r24 = r3;
            fn_800F7AF0();
            r25 = r3;
            r3 = r24;
            fn_800F7BC4();
            r25 = r3 & r25;
            r3 = r28;
            fn_8025DA18();
            r0 = r25 & 0x00000040;
            if ((u32)r0 != (u32)0x0) {
                r3 = r28;
                fn_8025D560();
                r25 = r3;
                r3 = r28;
                fn_8025D584();
                if ((s32)r25 == (s32)r3) goto L_800680EC;
                r3 = 0x25;
                r4 = 0x0;
                r5 = 0x0;
                fn_80166AB8();
                goto L_800680EC;
            }
            r0 = r25 & 0xc0f;
            if ((u32)r0 == (u32)0x0) goto L_800680EC;
            r3 = r28;
            r24 = -0x1;
            fn_8025D89C();
            r0 = r25 & 0x1;
            r3 = r3 & 0xFFFF;
            if ((u32)r0 != (u32)0x0) {
                r24 = 0x0;
            }
            r0 = r25 & 0x00000008;
            if ((u32)r0 != (u32)0x0) {
                r24 = 0x1;
            }
            r0 = r25 & 0x00000800;
            if ((u32)r0 != (u32)0x0) {
                r24 = 0x2;
            }
            r0 = r25 & 0x00000004;
            if ((u32)r0 != (u32)0x0) {
                r24 = 0x3;
            }
            r0 = r25 & 0x00000002;
            if ((u32)r0 != (u32)0x0) {
                r24 = 0x4;
            }
            r0 = r25 & 0x00000400;
            if ((u32)r0 != (u32)0x0) {
                r24 = 0x5;
            }
            if ((s32)r3 <= (s32)r24) {
                r24 = -0x1;
            }
            if ((s32)r24 < (s32)0x0) goto L_800680EC;
            r3 = r28;
            r4 = r24;
            fn_8025D644();
            r25 = r3;
            if ((s32)r25 < (s32)0x0) goto L_800680EC;
            r3 = 0x3c3;
            r4 = 0x0;
            r5 = 0x0;
            fn_80166AB8();
            r3 = 0x5 - r25;
            r0 = (0x4330 << 16);
            r4 = r3 * 0x18;
            r3 = (u32)&lbl_803A9F08;
            *(u32*)(sp + 0x8) = r0;
            r0 = r25 << 2;
            r3 = (u32)&lbl_803A9F08;
            f2 = *(f64*)&lbl_8047BFF0;
            /* xoris r4, r4, 0x8000 */;
            f0 = *(f32*)&lbl_8047BFE8;
            r4 = r28 * 0x30;
            f1 = *(f64*)(sp + 0x8);
            r3 = r3 + r4;
            r3 = r3 + (0x1 << 16);
            f1 = f1 - f2;
            r3 = r3 + r0;
            /* subi r3, r3, 0x3274 */;
            *(f32*)((u8*)r3 + 0x0) = f1;
            *(f32*)((u8*)r3 + 0x18) = f0;
            goto L_800680EC;
        }
        if ((s32)r28 == (s32)0x1) {
            r3 = r28;
            fn_8025D2B0();
            r3 = r28;
            fn_8025DA18();
            f2 = *(f32*)((u8*)r29 + (-12724));
            f1 = *(f32*)((u8*)r29 + (-12920));
            f0 = *(f32*)&lbl_8047BFEC;
            f1 = f2 + f1;
            *(f32*)((u8*)r29 + (-12724)) = f1;
            f1 = *(f32*)((u8*)r29 + (-12724));
            /* cror eq, gt, eq */;
            if (f1 == f0) {
                f0 = *(f32*)&lbl_8047BFE8;
                r3 = (u32)&lbl_802EDB64;
                r3 = (u32)&lbl_802EDB64;
                *(f32*)((u8*)r29 + (-12724)) = f0;
                r5 = *(u32*)((u8*)r29 + (-12720));
                r4 = r5 << 2;
                r0 = r5 + 0x1;
                r24 = *(u32*)(r3 + r4);
                *(u32*)((u8*)r29 + (-12720)) = r0;
            } else {

                r24 = 0x0;
            }
            r0 = r24 & 0xc0f;
            if ((u32)r0 == (u32)0x0) goto L_800680EC;
            r3 = r28;
            r25 = -0x1;
            fn_8025D89C();
            r0 = r24 & 0x1;
            r3 = r3 & 0xFFFF;
            if ((u32)r0 != (u32)0x0) {
                r25 = 0x0;
            }
            r0 = r24 & 0x00000008;
            if ((u32)r0 != (u32)0x0) {
                r25 = 0x1;
            }
            r0 = r24 & 0x00000800;
            if ((u32)r0 != (u32)0x0) {
                r25 = 0x2;
            }
            r0 = r24 & 0x00000004;
            if ((u32)r0 != (u32)0x0) {
                r25 = 0x3;
            }
            r0 = r24 & 0x00000002;
            if ((u32)r0 != (u32)0x0) {
                r25 = 0x4;
            }
            r0 = r24 & 0x00000400;
            if ((u32)r0 != (u32)0x0) {
                r25 = 0x5;
            }
            r3 = r28;
            if ((s32)r3 <= (s32)r25) {
                r4 = -0x1;
            } else {

                r4 = r25;
            }
            fn_8025D644();
            r25 = r3;
            if ((s32)r25 < (s32)0x0) goto L_800680EC;
            r3 = 0x3c3;
            r4 = 0x0;
            r5 = 0x0;
            fn_80166AB8();
            r3 = 0x5 - r25;
            r0 = (0x4330 << 16);
            r4 = r3 * 0x18;
            r3 = (u32)&lbl_803A9F08;
            *(u32*)(sp + 0x8) = r0;
            r0 = r25 << 2;
            r3 = (u32)&lbl_803A9F08;
            f2 = *(f64*)&lbl_8047BFF0;
            /* xoris r4, r4, 0x8000 */;
            f0 = *(f32*)&lbl_8047BFE8;
            r4 = r28 * 0x30;
            f1 = *(f64*)(sp + 0x8);
            r3 = r3 + r4;
            r3 = r3 + (0x1 << 16);
            f1 = f1 - f2;
            r3 = r3 + r0;
            /* subi r3, r3, 0x3274 */;
            *(f32*)((u8*)r3 + 0x0) = f1;
            *(f32*)((u8*)r3 + 0x18) = f0;
            goto L_800680EC;
        }
        r3 = r28;
        fn_8025D9F0();
        r0 = r3 & 0xFFFF;
        if ((s32)r0 != (s32)0x0) {
            if ((s32)r0 >= (s32)0x0 || (s32)r0 >= (s32)0x3) {

                goto L_80067F6C;
            }
            r3 = r28;
            fn_8025D2B0();
            r24 = r3;
            fn_800F7AF0();
            r25 = r3;
            r3 = r24;
            fn_800F7BC4();
            r24 = r3 & r25;
            r3 = r28;
            fn_8025DA18();
            r0 = r24 & 0x00000040;
            if ((u32)r0 != (u32)0x0) {
                r3 = r28;
                fn_8025D560();
                r25 = r3;
                r3 = r28;
                fn_8025D584();
                if ((s32)r25 == (s32)r3) goto L_800680EC;
                r3 = 0x25;
                r4 = 0x0;
                r5 = 0x0;
                fn_80166AB8();
                goto L_800680EC;
            }
            r0 = r24 & 0xc0f;
            if ((u32)r0 == (u32)0x0) goto L_800680EC;
            r3 = r28;
            r25 = -0x1;
            fn_8025D89C();
            r0 = r24 & 0x1;
            r3 = r3 & 0xFFFF;
            if ((u32)r0 != (u32)0x0) {
                r25 = 0x0;
            }
            r0 = r24 & 0x00000008;
            if ((u32)r0 != (u32)0x0) {
                r25 = 0x1;
            }
            r0 = r24 & 0x00000800;
            if ((u32)r0 != (u32)0x0) {
                r25 = 0x2;
            }
            r0 = r24 & 0x00000004;
            if ((u32)r0 != (u32)0x0) {
                r25 = 0x3;
            }
            r0 = r24 & 0x00000002;
            if ((u32)r0 != (u32)0x0) {
                r25 = 0x4;
            }
            r0 = r24 & 0x00000400;
            if ((u32)r0 != (u32)0x0) {
                r25 = 0x5;
            }
            if ((s32)r3 <= (s32)r25) {
                r25 = -0x1;
            }
            if ((s32)r25 < (s32)0x0) goto L_800680EC;
            r3 = r28;
            r4 = r25;
            fn_8025D644();
            r25 = r3;
            if ((s32)r25 < (s32)0x0) goto L_800680EC;
            r3 = 0x3c3;
            r4 = 0x0;
            r5 = 0x0;
            fn_80166AB8();
            r3 = 0x5 - r25;
            r0 = (0x4330 << 16);
            r4 = r3 * 0x18;
            r3 = (u32)&lbl_803A9F08;
            *(u32*)(sp + 0x8) = r0;
            r0 = r25 << 2;
            r3 = (u32)&lbl_803A9F08;
            f2 = *(f64*)&lbl_8047BFF0;
            /* xoris r4, r4, 0x8000 */;
            f0 = *(f32*)&lbl_8047BFE8;
            r4 = r28 * 0x30;
            f1 = *(f64*)(sp + 0x8);
            r3 = r3 + r4;
            r3 = r3 + (0x1 << 16);
            f1 = f1 - f2;
            r3 = r3 + r0;
            /* subi r3, r3, 0x3274 */;
            *(f32*)((u8*)r3 + 0x0) = f1;
            *(f32*)((u8*)r3 + 0x18) = f0;
            goto L_800680EC;
            L_80067F6C: ;
            r3 = r28;
            fn_8025D2B0();
            r4 = r28;
            fn_800681B4();
            goto L_800680EC;
            }
        r3 = r28;
        fn_8025D2B0();
        r24 = r3;
        fn_800F7AF0();
        r25 = r3;
        r3 = r24;
        fn_800F7BC4();
        r24 = r3 & r25;
        r3 = r28;
        fn_8025DA18();
        r0 = r24 & 0x00000040;
        if ((u32)r0 != (u32)0x0) {
            r3 = r28;
            fn_8025D560();
            r25 = r3;
            r3 = r28;
            fn_8025D584();
            if ((s32)r25 == (s32)r3) goto L_800680EC;
            r3 = 0x25;
            r4 = 0x0;
            r5 = 0x0;
            fn_80166AB8();
            goto L_800680EC;
        }
        r0 = r24 & 0xc0f;
        if ((u32)r0 == (u32)0x0) goto L_800680EC;
        r3 = r28;
        r25 = -0x1;
        fn_8025D89C();
        r0 = r24 & 0x1;
        r3 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0x0) {
            r25 = 0x0;
        }
        r0 = r24 & 0x00000008;
        if ((u32)r0 != (u32)0x0) {
            r25 = 0x1;
        }
        r0 = r24 & 0x00000800;
        if ((u32)r0 != (u32)0x0) {
            r25 = 0x2;
        }
        r0 = r24 & 0x00000004;
        if ((u32)r0 != (u32)0x0) {
            r25 = 0x3;
        }
        r0 = r24 & 0x00000002;
        if ((u32)r0 != (u32)0x0) {
            r25 = 0x4;
        }
        r0 = r24 & 0x00000400;
        if ((u32)r0 != (u32)0x0) {
            r25 = 0x5;
        }
        if ((s32)r3 <= (s32)r25) {
            r25 = -0x1;
        }
        if ((s32)r25 < (s32)0x0) goto L_800680EC;
        r3 = r28;
        r4 = r25;
        fn_8025D644();
        r25 = r3;
        if ((s32)r25 < (s32)0x0) goto L_800680EC;
        r3 = 0x3c3;
        r4 = 0x0;
        r5 = 0x0;
        fn_80166AB8();
        r3 = 0x5 - r25;
        r0 = (0x4330 << 16);
        r4 = r3 * 0x18;
        r3 = (u32)&lbl_803A9F08;
        *(u32*)(sp + 0x8) = r0;
        r0 = r25 << 2;
        r3 = (u32)&lbl_803A9F08;
        f2 = *(f64*)&lbl_8047BFF0;
        /* xoris r4, r4, 0x8000 */;
        f0 = *(f32*)&lbl_8047BFE8;
        r4 = r28 * 0x30;
        f1 = *(f64*)(sp + 0x8);
        r3 = r3 + r4;
        r3 = r3 + (0x1 << 16);
        f1 = f1 - f2;
        r3 = r3 + r0;
        /* subi r3, r3, 0x3274 */;
        *(f32*)((u8*)r3 + 0x0) = f1;
        *(f32*)((u8*)r3 + 0x18) = f0;
        L_800680EC: ;
        r3 = r28;
        fn_8025D2B0();
        if ((s32)r3 != (s32)0x1) goto L_80068190;
        r3 = r28;
        fn_8025D9F0();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0x0) goto L_80068190;
        r3 = r28;
        fn_8025D560();
        r25 = r3;
        ((void(*)(void))fn_8006B1D4)();
        r24 = r3 & 0xFFFF;
        r3 = r28;
        fn_8025D89C();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 < (u32)r24) {
        } else {

            r0 = r24;
        }
        r0 = r0 & 0xFFFF;
        if ((s32)r25 != (s32)r0) goto L_80068190;
        /* subi r0, r25, 0x1 */;
        if ((s32)r0 < (s32)0x0) {
            r0 = 0x0;
        }
        r3 = (u32)&lbl_803A9F08;
        r0 = r0 << 2;
        r3 = (u32)&lbl_803A9F08;
        f1 = *(f32*)&lbl_8047BFE8;
        r3 = r3 + r26;
        r3 = r3 + r0;
        r3 = r3 + (0x1 << 16);
        f0 = *(f32*)((u8*)r3 + (-12916));
        if (f1 != f0) goto L_80068190;
        r0 = 0x1;
        *(u8*)((u8*)r27 + 0x95) = r0;
        *(u8*)((u8*)r27 + 0x98) = r0;
        L_80068190: ;
        r28 = r28 + 0x1;
        r26 = r26 + 0x30;

    }
    return;
}


/* 0x800681B4 | size: 0x264 */
s32 fn_800681B4(void) {
    extern void fn_8008A9AC();
    extern void fn_8008A9E4();
    extern void fn_80166AB8();
    extern void fn_8025D560();
    extern void fn_8025D584();
    extern void fn_8025D5E0();
    extern void fn_8025D644();
    extern void fn_8025D808();
    extern void fn_8025DA18();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    
    r31 = r3;
    r30 = r4;
    r3 = r30;
    fn_8025DA18();
    r3 = r31;
    r4 = (u32)sp + 0x8;
    fn_8008A9E4();
    r0 = (0x100 << 16);
    /* clrrwi r3, r3, 24 */;
    if ((s32)r3 != (s32)r0) {
        if ((s32)r3 < (s32)r0) {
            if ((s32)r3 != (s32)0x0) {
                if ((s32)r3 >= (s32)0x0) return;
                r0 = (0xff00 << 16);
                if ((s32)r3 != (s32)r0) {
                    return;
                }
                r0 = (0x300 << 16);
                if ((s32)r3 != (s32)r0) {
                    return;
                }
                r3 = r30;
                fn_8025D560();
                r0 = r3;
                r3 = r30;
                r4 = r0;
                fn_8025D644();
                r31 = r3;
                if ((s32)r31 < (s32)0x0) return;
                r3 = 0x3c3;
                r4 = 0x0;
                r5 = 0x0;
                fn_80166AB8();
                r3 = 0x5 - r31;
                r0 = (0x4330 << 16);
                r3 = r3 * 0x18;
                r4 = (u32)&lbl_803A9F08;
                *(u32*)(sp + 0x30) = r0;
                r5 = r31 << 2;
                f2 = *(f64*)&lbl_8047BFF0;
                r4 = (u32)&lbl_803A9F08;
                /* xoris r0, r3, 0x8000 */;
                f0 = *(f32*)&lbl_8047BFE8;
                *(u32*)(sp + 0x34) = r0;
                r0 = r30 * 0x30;
                f1 = *(f64*)(sp + 0x30);
                r3 = r4 + r0;
                r4 = r3 + (0x1 << 16);
                f1 = f1 - f2;
                /* subi r4, r4, 0x3274 */;
                r3 = r4 + r5;
                *(f32*)(r4 + r5) = f1;
                *(f32*)((u8*)r3 + 0x18) = f0;
                return;
                    }
            r3 = r30;
            fn_8025D560();
            r31 = r3;
            r3 = r30;
            fn_8025D584();
            if ((s32)r31 == (s32)r3) return;
            r3 = 0x25;
            r4 = 0x0;
            r5 = 0x0;
            fn_80166AB8();
            return;
                }
        r3 = r30;
        fn_8025D808();
        r31 = r3 & 0xFFFF;
        r4 = (u32)sp + 0xc;
        fn_8008A9AC();
        r6 = 0x0;
        if ((s32)r6 < (s32)r31) {
            /* subi r3, r31, 0x8 */;
            if ((s32)r31 > (s32)0x8) {
                r4 = (u32)sp + 0xc;
                r5 = (u32)sp + 0x14;
                r0 = r3 + 0x7;
                r0 = (u32)r0 >> 3;
                ctr_fn = (void(*)(void))r0;
                if ((s32)r3 > (s32)0x0) {
                    do {
                        r0 = *(u8*)((u8*)r4 + 0x0);
                        r6 = r6 + 0x8;
                        *(u32*)((u8*)r5 + 0x0) = r0;
                        r0 = *(u8*)((u8*)r4 + 0x1);
                        *(u32*)((u8*)r5 + 0x4) = r0;
                        r0 = *(u8*)((u8*)r4 + 0x2);
                        *(u32*)((u8*)r5 + 0x8) = r0;
                        r0 = *(u8*)((u8*)r4 + 0x3);
                        *(u32*)((u8*)r5 + 0xC) = r0;
                        r0 = *(u8*)((u8*)r4 + 0x4);
                        *(u32*)((u8*)r5 + 0x10) = r0;
                        r0 = *(u8*)((u8*)r4 + 0x5);
                        *(u32*)((u8*)r5 + 0x14) = r0;
                        r0 = *(u8*)((u8*)r4 + 0x6);
                        *(u32*)((u8*)r5 + 0x18) = r0;
                        r0 = *(u8*)((u8*)r4 + 0x7);
                        r4 = r4 + 0x8;
                        *(u32*)((u8*)r5 + 0x1C) = r0;
                        r5 = r5 + 0x20;
                    } while (--ctr != 0);
            }
            }
            r3 = (u32)sp + 0xc;
            r0 = r6 << 2;
            r4 = (u32)sp + 0x14;
            r3 = r3 + r6;
            r4 = r4 + r0;
            r0 = r31 - r6;
            ctr_fn = (void(*)(void))r0;
            if ((s32)r6 < (s32)r31) {
                do {
                    r0 = *(u8*)((u8*)r3 + 0x0);
                    r3 = r3 + 0x1;
                    *(u32*)((u8*)r4 + 0x0) = r0;
                    r4 = r4 + 0x4;
                } while (--ctr != 0);
        }
        }
        r3 = r30;
        r4 = r31;
        r5 = (u32)sp + 0x14;
        fn_8025D5E0();
        r3 = (u32)&lbl_803A9F08;
        r4 = 0x1;
        r0 = (u32)&lbl_803A9F08;
        r3 = r0 + r30;
        *(u8*)((u8*)r3 + 0x4) = r4;
        return;
                }
    r3 = (u32)&lbl_803A9F08;
    r0 = 0x0;
    r3 = (u32)&lbl_803A9F08;
    r3 = r3 + (0x1 << 16);
    *(u8*)((u8*)r3 + (-12712)) = r0;
    r0 = *(u32*)((u8*)r3 + (-12708));
    if ((s32)r0 >= (s32)0x0) return;
    *(u32*)((u8*)r3 + (-12708)) = r31;

    return;
}


/* 0x80068418 | size: 0x320 */
s32 fn_80068418(void) {
    extern void fn_800CE2D8();
    extern void fn_800D3088();
    extern void fn_800F7A08();
    extern void fn_800F7A7C();
    extern void fn_800F7BC4();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    
    r26 = r3;
    r0 = *(u16*)((u8*)r26 + 0x0);
    r31 = r4;
    r3 = r31;
    r29 = 0x0;
    *(u16*)((u8*)r26 + 0x2) = r0;
    r4 = 0x0;
    fn_800F7A08();
    r27 = r3;
    r3 = r31;
    r4 = 0x0;
    fn_800F7A7C();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0x0) {
        r0 = -r0;
    }
    if ((s32)r0 <= (s32)0x20) {
        r0 = (s8)r27;
        if ((s32)r0 < (s32)0x0) {
            r0 = -r0;
        }
        if ((s32)r0 <= (s32)0x20) goto L_8006858C;
    }
    r4 = (s8)r3;
    r0 = (s8)r27;
    r3 = (0x4330 << 16);
    f2 = *(f64*)&lbl_8047BFF0;
    /* xoris r4, r4, 0x8000 */;
    /* xoris r0, r0, 0x8000 */;
    f0 = *(f64*)(sp + 0x8);
    *(u32*)(sp + 0x14) = r0;
    f1 = f0 - f2;
    f0 = *(f64*)(sp + 0x10);
    f2 = f0 - f2;
    fn_800CE2D8();
    f2 = (f32)f1;
    f0 = *(f32*)&lbl_8047BFE8;
    if (f2 > f0) {
        f1 = f2;
    } else {

        f1 = -f2;
    }
    f0 = *(f32*)&lbl_8047BFF8;
    if (f1 < f0) {
        r0 = r29 | 0x2;
        r29 = r0 & 0xFFFF;
        goto L_80068524;
    }
    f0 = *(f32*)&lbl_8047BFE8;
    if (f2 > f0) {
        f1 = f2;
    } else {

        f1 = -f2;
    }
    f0 = *(f32*)&lbl_8047BFFC;
    if (f1 <= f0) goto L_80068524;
    r0 = r29 | 0x1;
    r29 = r0 & 0xFFFF;
    L_80068524: ;
    f0 = *(f32*)&lbl_8047BFE8;
    f1 = *(f32*)&lbl_8047C000;
    if (f2 > f0) {
        f0 = f2;
    } else {

        f0 = -f2;
    }
    if (f1 >= f0) goto L_8006858C;
    f0 = *(f32*)&lbl_8047BFE8;
    if (f2 > f0) {
        f1 = f2;
    } else {

        f1 = -f2;
    }
    f0 = *(f32*)&lbl_8047C004;
    if (f1 >= f0) goto L_8006858C;
    f0 = *(f32*)&lbl_8047BFE8;
    if (f2 < f0) {
        r0 = r29 | 0x4;
        r29 = r0 & 0xFFFF;
        goto L_8006858C;
    }
    r0 = r29 | 0x8;
    r29 = r0 & 0xFFFF;
    L_8006858C: ;
    r3 = r31;
    fn_800F7BC4();
    r0 = r3 & 0x00000008;
    if ((u32)r0 != (u32)0x0) {
        r0 = r29 | 0x1;
        r29 = r0 & 0xFFFF;
    }
    r0 = r3 & 0x00000004;
    if ((u32)r0 != (u32)0x0) {
        r0 = r29 | 0x2;
        r29 = r0 & 0xFFFF;
    }
    r0 = r3 & 0x1;
    if ((u32)r0 != (u32)0x0) {
        r0 = r29 | 0x4;
        r29 = r0 & 0xFFFF;
    }
    r0 = r3 & 0x00000002;
    if ((u32)r0 != (u32)0x0) {
        r0 = r29 | 0x8;
        r29 = r0 & 0xFFFF;
    }
    r0 = r3 & 0x00000100;
    if ((u32)r0 != (u32)0x0) {
        r0 = r29 | 0x10;
        r29 = r0 & 0xFFFF;
    }
    r0 = r3 & 0x00000200;
    if ((u32)r0 != (u32)0x0) {
        r0 = r29 | 0x20;
        r29 = r0 & 0xFFFF;
    }
    r0 = r3 & 0x00000400;
    if ((u32)r0 != (u32)0x0) {
        r0 = r29 | 0x40;
        r29 = r0 & 0xFFFF;
    }
    r0 = r3 & 0x00000800;
    if ((u32)r0 != (u32)0x0) {
        r0 = r29 | 0x80;
        r29 = r0 & 0xFFFF;
    }
    r0 = r3 & 0x00000010;
    if ((u32)r0 != (u32)0x0) {
        r0 = r29 | 0x100;
        r29 = r0 & 0xFFFF;
    }
    r0 = r3 & 0x00000040;
    if ((u32)r0 != (u32)0x0) {
        r0 = r29 | 0x200;
        r29 = r0 & 0xFFFF;
    }
    r0 = r3 & 0x00000020;
    if ((u32)r0 != (u32)0x0) {
        r0 = r29 | 0x400;
        r29 = r0 & 0xFFFF;
    }
    r0 = r3 & 0x00001000;
    if ((u32)r0 != (u32)0x0) {
        r0 = r29 | 0x800;
        r29 = r0 & 0xFFFF;
    }
    r0 = *(u16*)((u8*)r26 + 0x2);
    r31 = r29 & 0xFFFF;
    r27 = 0x0;
    r30 = 0x0;
    r0 = r0 ^ 0xffff;
    r0 = r0 & r31;
    r28 = r0 & 0xFFFF;
    do {
        r0 = 0x1;
        r0 = r0 << r30;
        r25 = r0 & 0xFFFF;
        r0 = r28 & r25;
        if ((s32)r0 != (s32)0x0) {
            r0 = r30 + 0xa;
            r3 = 0xf;
            *(u8*)(r26 + r0) = r3;
            r27 = r27 | r25;
            goto L_8006870C;
        }
        r0 = r31 & r25;
        if ((s32)r0 == (s32)0x0) goto L_8006870C;
        fn_800D3088();
        r4 = r30 + 0xa;
        r0 = *(u8*)(r26 + r4);
        r0 = r0 - r3;
        r0 = (s8)r0;
        *(u8*)(r26 + r4) = r0;
        r0 = *(u8*)(r26 + r4);
        r0 = (s8)r0;
        if ((s32)r0 > (s32)0x0) goto L_8006870C;
        r0 = 0x5;
        r27 = r27 | r25;
        *(u8*)(r26 + r4) = r0;
        L_8006870C: ;
        r30 = r30 + 0x1;
    } while ((s32)r30 < (s32)0x10);
    *(u16*)((u8*)r26 + 0x0) = r29;
    *(u16*)((u8*)r26 + 0x4) = r28;
    *(u16*)((u8*)r26 + 0x6) = r27;
    return;
}


/* 0x80068738 | size: 0x5C */
s32 fn_80068738(void) {
    extern void fn_80068418(u8 *, s32);
    extern void fn_80105624(void);
    u8 *ptr;
    s32 i;

    fn_80105624();
    ptr = (u8 *)&lbl_803A9EA0;
    for (i = 0; i < 4; i++) {
        fn_80068418(ptr, i + 1);
        ptr += 0x1A;
    }
    return 0;
}


/* 0x80068794 | size: 0x130 */
s32 fn_80068794(void) {
    extern void fn_8025D560();
    extern void fn_8025DA88();
    u8 sp[0x30];
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

    
    r31 = r4;
    r27 = r5;
    r28 = r6;
    r3 = r27;
    fn_8025D560();
    r30 = r3;
    r29 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_800687E8;

    if ((s32)r3 < (s32)0x0) {
        goto L_800687E8;
    }
    if ((s32)r27 < (s32)0x2) goto L_800687E8;
    r29 = 0x0;
    L_800687E8: ;
    r0 = r29 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    if ((s32)r30 > (s32)r28) {
        r6 = r27 * 0x30;
        r5 = (u32)&lbl_803A9F08;
        r4 = *(s16*)((u8*)r31 + 0x6);
        r3 = (u32)&lbl_802EF0A8;
        r0 = (u32)&lbl_803A9F08;
        f0 = *(f32*)&lbl_8047BFE8;
        r5 = r0 + r6;
        r6 = r28 << 2;
        r5 = r5 + (0x1 << 16);
        r0 = (u32)&lbl_802EF0A8;
        /* subi r5, r5, 0x3274 */;
        r5 = r5 + r6;
        f1 = *(f32*)((u8*)r5 + 0x0);
        r3 = r4 * 0x1c;
        f1 = (f64)(s32)f1;
        r3 = r0 + r3;
        r3 = *(s16*)((u8*)r3 + 0x2);
        *(f64*)(sp + 0x8) = f1;
        r0 = r3 + r0;
        r0 = (s16)r0;
        *(u16*)((u8*)r31 + 0x50) = r0;
        f2 = *(f32*)((u8*)r5 + 0x18);
        f1 = *(f32*)((u8*)r5 + 0x0);
        f2 = f2 - f1;
        if (f2 > f0) {
        } else {

            f2 = -f2;
        }
        f1 = *(f32*)&lbl_8047C00C;
        f0 = *(f32*)&lbl_8047C008;
        f0 = -(f1 * f2 - f0);
        f0 = (f64)(s32)f0;
        *(f64*)(sp + 0x10) = f0;
        *(u8*)((u8*)r31 + 0x67) = r0;
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;

    return;
}


/* 0x800688C4 | size: 0x138 */
s32 fn_800688C4(void) {
    extern void fn_800FA280();
    extern void fn_800FB680();
    extern void fn_8012AC54();
    extern void fn_80132A38();
    extern void fn_8025D914();
    extern void fn_8025D9CC();
    extern void fn_8025DA18();
    extern void fn_8025DA88();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    
    r28 = r3;
    r29 = r5;
    r30 = r6;
    r31 = 0x1;
    fn_8025DA88();
    if ((s32)r3 == (s32)0x2 || (s32)r3 >= (s32)0x2) goto L_80068918;

    if ((s32)r3 < (s32)0x0) {
        goto L_80068918;
    }
    if ((s32)r29 < (s32)0x2) goto L_80068918;
    r31 = 0x0;
    L_80068918: ;
    r0 = r31 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;
    r3 = r29;
    fn_8025D914();
    fn_8012AC54();
    r31 = r3;
    if ((u32)r31 == (u32)0x0) {
        r3 = 0x1;
        fn_800FA280();
        r31 = r3;
    }
    r3 = r29;
    fn_8025DA18();
    r4 = r3 & 0xFFFF;
    r3 = 0x34;
    r4 = r4 + 0x1;
    fn_80132A38();
    r4 = r31;
    r3 = 0x37;
    fn_80132A38();
    fn_8025D9CC();
    if ((s32)r3 == (s32)0x4) {
        r5 = *(u8*)((u8*)r28 + 0x8B);
        r0 = -0x100;
        r3 = 0x0;
        r4 = 0x0;
        r5 = r5 | r0;
        r6 = 0x30dc;
        fn_800FB680();
        return;
    }
    if ((s32)r30 == (s32)0x2) {
        r5 = *(u8*)((u8*)r28 + 0x8B);
        r0 = -0x100;
        r3 = 0x0;
        r4 = 0x0;
        r5 = r5 | r0;
        r6 = 0x30e6;
        fn_800FB680();
        return;
    }
    r5 = *(u8*)((u8*)r28 + 0x8B);
    r0 = -0x100;
    r3 = 0x0;
    r4 = 0x0;
    r5 = r5 | r0;
    r6 = 0x30dc;
    fn_800FB680();

    return;
}


/* 0x800689FC | size: 0x1B4 */
void fn_800689FC(void) {
    extern void fn_801230E0();
    extern void fn_8025D970();
    extern void fn_8025DA88();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    
    r31 = r4;
    r29 = r5;
    r30 = 0x1;
    fn_8025DA88();

    /* Check if processing should proceed */
    if ((s32)r3 == 0 || (s32)r3 == 1) {
        if ((s32)r29 >= (s32)0x2) {
            r30 = 0x0;
        }
    }

    r0 = r30 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;

    /* Search lookup table for matching key */
    r0 = *(s16*)((u8*)r31 + 0x6);
    r3 = (u32)&lbl_802EDA20;
    r3 = (u32)&lbl_802EDA20;
    r4 = 0x0;
    r5 = r0 & 0xFFFF;
    r0 = 0x0;
    ctr = 0x8;
    while (ctr != 0) {
        /* Unrolled search: check 9 entries per iteration */
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r4 = r4 + 0x8;
        ctr--;
        if (ctr == 0) { r0 = 0x0; }
    }

    r4 = r0 & 0xFFFF;
    r3 = r29;
    fn_8025D970();
    fn_801230E0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x0) {
        r0 = *(u8*)((u8*)r31 + 0x4);
        r0 = r0 | 0x2;
        r0 = (s8)r0;
        *(u8*)((u8*)r31 + 0x4) = r0;
        return;
    }
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r31 + 0x4) = r0;

    return;
}


/* 0x80068BB0 | size: 0x20C */
s32 fn_80068BB0(void) {
    extern void fn_800FB680();
    extern void fn_80123FBC();
    extern void fn_8012640C();
    extern void fn_80132A38();
    extern void fn_8025D970();
    extern void fn_8025DA88();
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

    
    r29 = r3;
    r30 = r4;
    r27 = r5;
    r31 = r6;
    r28 = 0x1;
    fn_8025DA88();

    /* Check if processing should proceed */
    if ((s32)r3 == 0 || (s32)r3 == 1) {
        if ((s32)r27 >= (s32)0x2) {
            r28 = 0x0;
        }
    }

    r0 = r28 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;

    /* Search lookup table for matching key */
    r0 = *(s16*)((u8*)r30 + 0x6);
    r3 = (u32)&lbl_802EDA20;
    r3 = (u32)&lbl_802EDA20;
    r4 = 0x0;
    r5 = r0 & 0xFFFF;
    r0 = 0x0;
    ctr = 0x8;
    while (ctr != 0) {
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r4 = r4 + 0x8;
        ctr--;
        if (ctr == 0) { r0 = 0x0; }
    }

    r4 = r0 & 0xFFFF;
    r3 = r27;
    fn_8025D970();
    r28 = r3;
    if ((u32)r28 == (u32)0x0) return;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) {
        r0 = *(u8*)((u8*)r30 + 0x4);
        r0 = r0 & 0xFFFFFFFD;
        r0 = (s8)r0;
        *(u8*)((u8*)r30 + 0x4) = r0;
        return;
    }
    r3 = r28;
    r4 = 0x0;
    r5 = 0x7a;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3 & 0xFF;
    r3 = 0x34;
    fn_80132A38();
    if ((s32)r31 == (s32)0x0) {
        r5 = *(u8*)((u8*)r29 + 0x8B);
        r0 = -0x100;
        r3 = 0x0;
        r4 = 0x0;
        r5 = r5 | r0;
        r6 = 0x30d4;
        fn_800FB680();
        return;
    }
    r5 = *(u8*)((u8*)r29 + 0x8B);
    r0 = -0x100;
    r3 = 0x0;
    r4 = 0x0;
    r5 = r5 | r0;
    r6 = 0xd3;
    fn_800FB680();

    return;
}


/* 0x80068DBC | size: 0x1C8 */
s32 fn_80068DBC(void) {
    extern void fn_800FA280();
    extern void fn_800FB680();
    extern void fn_8011F4F0();
    extern void fn_80132A38();
    extern void fn_8025D970();
    extern void fn_8025DA88();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    
    r31 = r3;
    r28 = r4;
    r29 = r5;
    r30 = 0x1;
    fn_8025DA88();

    /* Check if processing should proceed */
    if ((s32)r3 == 0 || (s32)r3 == 1) {
        if ((s32)r29 >= (s32)0x2) {
            r30 = 0x0;
        }
    }

    r0 = r30 & 0xFF;
    if ((u32)r0 == (u32)0x0) return;

    /* Search lookup table for matching key */
    r0 = *(s16*)((u8*)r28 + 0x6);
    r3 = (u32)&lbl_802EDA20;
    r3 = (u32)&lbl_802EDA20;
    r4 = 0x0;
    r5 = r0 & 0xFFFF;
    r0 = 0x0;
    ctr = 0x8;
    while (ctr != 0) {
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r0 = *(u16*)((u8*)r3 + 0x0);
        if ((u32)r0 == (u32)r5) { r0 = *(u16*)((u8*)r3 + 0x2); break; }
        r3 = r3 + 0x4;
        r4 = r4 + 0x8;
        ctr--;
        if (ctr == 0) { r0 = 0x0; }
    }

    r4 = r0 & 0xFFFF;
    r3 = r29;
    fn_8025D970();
    fn_8011F4F0();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x1;
        fn_800FA280();
    }
    r4 = r3;
    r3 = 0x37;
    fn_80132A38();
    r5 = *(u8*)((u8*)r31 + 0x8B);
    r0 = -0x100;
    r3 = 0x0;
    r4 = 0x0;
    r5 = r5 | r0;
    r6 = 0xe9;
    fn_800FB680();

    return;
}


/* 0x80068F84 | size: 0xC4 */
void fn_80068F84(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 ctr = 0;

    r4 = (u32)&lbl_803A9F08;
    *(u32*)((u8*)r4 + 0x2C) = 0;
    r3 = r4 + (0x1 << 16);
    *(u8*)((u8*)r3 + (-12924)) = 0;
    ctr = 2;
    do {
        r9 = r4 + 0x30;
        r0 = 0x0;
        *(u8*)((u8*)r9 + 0x0) = r0;
        r8 = r9 + 0xc;
        r7 = r9 + 0x18;
        r6 = r9 + 0x24;
        *(u32*)((u8*)r9 + 0x4) = r0;
        r5 = r9 + 0x30;
        r3 = r9 + 0x3c;
        r4 = r4 + 0x48;
        *(u8*)((u8*)r8 + 0x0) = r0;
        r9 = r4 + 0x30;
        r4 = r4 + 0x48;
        *(u32*)((u8*)r8 + 0x4) = r0;
        r8 = r9 + 0xc;
        *(u8*)((u8*)r7 + 0x0) = r0;
        *(u32*)((u8*)r7 + 0x4) = r0;
        r7 = r9 + 0x18;
        *(u8*)((u8*)r6 + 0x0) = r0;
        *(u32*)((u8*)r6 + 0x4) = r0;
        r6 = r9 + 0x24;
        *(u8*)((u8*)r5 + 0x0) = r0;
        *(u32*)((u8*)r5 + 0x4) = r0;
        r5 = r9 + 0x30;
        *(u8*)((u8*)r3 + 0x0) = r0;
        *(u32*)((u8*)r3 + 0x4) = r0;
        r3 = r9 + 0x3c;
        *(u8*)((u8*)r9 + 0x0) = r0;
        *(u32*)((u8*)r9 + 0x4) = r0;
        *(u8*)((u8*)r8 + 0x0) = r0;
        *(u32*)((u8*)r8 + 0x4) = r0;
        *(u8*)((u8*)r7 + 0x0) = r0;
        *(u32*)((u8*)r7 + 0x4) = r0;
        *(u8*)((u8*)r6 + 0x0) = r0;
        *(u32*)((u8*)r6 + 0x4) = r0;
        *(u8*)((u8*)r5 + 0x0) = r0;
        *(u32*)((u8*)r5 + 0x4) = r0;
        *(u8*)((u8*)r3 + 0x0) = r0;
        *(u32*)((u8*)r3 + 0x4) = r0;
    } while (--ctr != 0);
}


/* 0x80069048 | size: 0x14 */
void fn_80069048(void) {
}

/* 0x8006905C | size: 0x1C4 */
s32 fn_8006905C(void) {
    extern void fn_8025D560();
    extern void fn_8025D89C();
    extern void fn_8025D9CC();
    extern void fn_8025DA88();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    
    r31 = 0x1;
    fn_8025D9CC();
    r30 = r3;
    fn_8025DA88();
    if ((s32)r3 != (s32)0x2) {
        if ((s32)r3 >= (s32)0x2) goto L_800690E8;
        if ((s32)r3 < (s32)0x0) {

        } else if ((s32)r30 == (s32)0x4) {
            r3 = (u32)&lbl_803A9F08;
            r0 = 0x4;
            r3 = (u32)&lbl_803A9F08;
            r31 = 0x2;
            r3 = r3 + (0x1 << 16);
            *(u32*)((u8*)r3 + (-12932)) = r0;

        }
        r3 = (u32)&lbl_803A9F08;
        r31 = 0x2;
        r3 = (u32)&lbl_803A9F08;
        r3 = r3 + (0x1 << 16);
        *(u32*)((u8*)r3 + (-12932)) = r30;
        goto L_800690E8;
    }
    r3 = (u32)&lbl_803A9F08;
    r0 = 0x4;
    r3 = (u32)&lbl_803A9F08;
    r31 = 0x4;
    r3 = r3 + (0x1 << 16);
    *(u32*)((u8*)r3 + (-12932)) = r0;
    L_800690E8: ;
    r3 = (u32)&lbl_803A9F08;
    r4 = (u32)&lbl_803A9F08;
    r3 = r4 + (0x1 << 16);
    r0 = *(u32*)((u8*)r3 + (-12932));
    if ((s32)r0 != (s32)0x4) {
        r28 = r4 + 0x1;
        r27 = r4 + 0x30;
        r26 = 0x1;
        while ((s32)r26 < (s32)r31) {

            r0 = *(u8*)((u8*)r28 + 0x4);
            if ((u32)r0 == (u32)0x0) {
                ((void(*)(void))fn_8006B1D4)();
                r30 = r3 & 0xFFFF;
                r3 = r26;
                fn_8025D89C();
                r29 = r3 & 0xFFFF;
                if ((u32)r29 < (u32)r30) {
                } else {

                    r29 = r30;
                }
                r3 = r26;
                fn_8025D560();
                r0 = r29 & 0xFFFF;
                if ((s32)r3 == (s32)r0) {
                    r3 = r26;
                    fn_8025D560();
                    r30 = r3;
                    ((void(*)(void))fn_8006B1D4)();
                    r29 = r3 & 0xFFFF;
                    r3 = r26;
                    fn_8025D89C();
                    r0 = r3 & 0xFFFF;
                    if ((u32)r0 < (u32)r29) {
                    } else {

                        r0 = r29;
                    }
                    r0 = r0 & 0xFFFF;
                    if ((s32)r30 == (s32)r0) {
                        /* subi r0, r30, 0x1 */;
                        if ((s32)r0 < (s32)0x0) {
                            r0 = 0x0;
                        }
                        r3 = r0 << 2;
                        f1 = *(f32*)&lbl_8047BFE8;
                        r3 = r3 + (0x1 << 16);
                        /* subi r3, r3, 0x3274 */;
                        f0 = *(f32*)(r27 + r3);
                        if (f1 == f0) {
                            r0 = 0x1;
                            *(u8*)((u8*)r28 + 0x4) = r0;
            }
            }
            }
            }
            r28 = r28 + 0x1;
            r27 = r27 + 0x30;
            r26 = r26 + 0x1;

        }
    }
    r3 = (u32)&lbl_803A9F08;
    r3 = (u32)&lbl_803A9F08;
    ctr_fn = (void(*)(void))r31;
    if ((s32)r31 > (s32)0x0) {
        do {
            r0 = *(u8*)((u8*)r3 + 0x4);
            if ((u32)r0 == (u32)0x0) {
                r3 = 0x0;
                return;
            }
            r3 = r3 + 0x1;
        } while (--ctr != 0);
    }
    r3 = 0x1;

    return;
}


/* 0x80069220 | size: 0x184 */
s32 fn_80069220(void) {
    extern void fn_800D3088();
    extern void fn_800D37CC();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    
    *(f64*)(sp + 0x20) = f31;
    /* psq_st f31, 0x28((u32)sp), 0, qr0 */;
    r31 = r3;
    fn_800D37CC();
    /* xoris r3, r3, 0x8000 */;
    r0 = (0x4330 << 16);
    f1 = *(f64*)&lbl_8047BFF0;
    *(u32*)(sp + 0x8) = r0;
    f0 = *(f64*)(sp + 0x8);
    f31 = f0 - f1;
    fn_800D3088();
    r0 = (0x4330 << 16);
    r4 = (u32)&lbl_803A9F08;
    f1 = *(f64*)&lbl_8047C020;
    *(u32*)(sp + 0x10) = r0;
    r5 = (u32)&lbl_803A9F08;
    r6 = r5 + (0x1 << 16);
    r3 = 0x0;
    f0 = *(f64*)(sp + 0x10);
    f0 = f0 - f1;
    f0 = f0 / f31;
    *(f32*)((u8*)r6 + (-12920)) = f0;
    do {
        r4 = r5 + (0x1 << 16);
        /* subi r4, r4, 0x3274 */;
        r0 = 0x6;
        ctr_fn = (void(*)(void))r0;
        do {
            f0 = *(f32*)((u8*)r4 + 0x0);
            f1 = *(f32*)((u8*)r4 + 0x18);
            if (f0 == f1) goto L_80069358;
            f1 = f1 - f0;
            f2 = *(f32*)&lbl_8047C010;
            f0 = *(f32*)((u8*)r6 + (-12920));
            f1 = f2 * f1;
            f3 = f1 * f0;
            if (f3 > f2) {
                f3 = f2;
            }
            f0 = *(f32*)&lbl_8047C014;
            /* cror eq, lt, eq */;
            if (f3 == f0) {
                f3 = f0;
            }
            f1 = *(f32*)((u8*)r4 + 0x0);
            f0 = *(f32*)&lbl_8047BFE8;
            f1 = f1 + f3;
            *(f32*)((u8*)r4 + 0x0) = f1;
            f2 = *(f32*)((u8*)r4 + 0x18);
            f0 = *(f32*)((u8*)r4 + 0x0);
            f1 = f2 - f0;
            if (f3 > f0) {
            } else {

                f3 = -f3;
            }
            f0 = *(f32*)&lbl_8047BFE8;
            if (f1 > f0) {
                f0 = f1;
            } else {

                f0 = -f1;
            }
            /* cror eq, lt, eq */;
            if (f0 != f3) {
                f0 = *(f32*)&lbl_8047BFE8;
                if (f1 > f0) {
                } else {

                    f1 = -f1;
                }
                f0 = *(f32*)&lbl_8047C018;
                if (f1 >= f0) goto L_80069358;
            }
            *(f32*)((u8*)r4 + 0x0) = f2;
            L_80069358: ;
            r4 = r4 + 0x4;
        } while (--ctr != 0);
        r5 = r5 + 0x30;
        r3 = r3 + 0x1;
    } while ((s32)r3 < (s32)0x4);
    r3 = (u32)&lbl_803A9F08;
    r3 = (u32)&lbl_803A9F08;
    r3 = r3 + (0x1 << 16);
    r0 = *(u32*)((u8*)r3 + (-12928));
    r0 = (s16)r0;
    *(u16*)((u8*)r31 + 0x84) = r0;
    /* psq_l f31, 0x28((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x20);
    return;
}


/* 0x800693A4 | size: 0x160 */
s32 fn_800693A4(void) {
    extern void fn_800D3088();
    extern void fn_800D37CC();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    
    *(f64*)(sp + 0x20) = f31;
    /* psq_st f31, 0x28((u32)sp), 0, qr0 */;
    fn_800D37CC();
    /* xoris r3, r3, 0x8000 */;
    r0 = (0x4330 << 16);
    f1 = *(f64*)&lbl_8047BFF0;
    *(u32*)(sp + 0x8) = r0;
    f0 = *(f64*)(sp + 0x8);
    f31 = f0 - f1;
    fn_800D3088();
    r0 = (0x4330 << 16);
    r4 = (u32)&lbl_803A9F08;
    f1 = *(f64*)&lbl_8047C020;
    *(u32*)(sp + 0x10) = r0;
    r5 = (u32)&lbl_803A9F08;
    r6 = r5 + (0x1 << 16);
    r3 = 0x0;
    f0 = *(f64*)(sp + 0x10);
    f0 = f0 - f1;
    f0 = f0 / f31;
    *(f32*)((u8*)r6 + (-12920)) = f0;
    do {
        r4 = r5 + (0x1 << 16);
        /* subi r4, r4, 0x3274 */;
        r0 = 0x6;
        ctr_fn = (void(*)(void))r0;
        do {
            f0 = *(f32*)((u8*)r4 + 0x0);
            f1 = *(f32*)((u8*)r4 + 0x18);
            if (f0 == f1) goto L_800694D4;
            f1 = f1 - f0;
            f2 = *(f32*)&lbl_8047C010;
            f0 = *(f32*)((u8*)r6 + (-12920));
            f1 = f2 * f1;
            f3 = f1 * f0;
            if (f3 > f2) {
                f3 = f2;
            }
            f0 = *(f32*)&lbl_8047C014;
            /* cror eq, lt, eq */;
            if (f3 == f0) {
                f3 = f0;
            }
            f1 = *(f32*)((u8*)r4 + 0x0);
            f0 = *(f32*)&lbl_8047BFE8;
            f1 = f1 + f3;
            *(f32*)((u8*)r4 + 0x0) = f1;
            f2 = *(f32*)((u8*)r4 + 0x18);
            f0 = *(f32*)((u8*)r4 + 0x0);
            f1 = f2 - f0;
            if (f3 > f0) {
            } else {

                f3 = -f3;
            }
            f0 = *(f32*)&lbl_8047BFE8;
            if (f1 > f0) {
                f0 = f1;
            } else {

                f0 = -f1;
            }
            /* cror eq, lt, eq */;
            if (f0 != f3) {
                f0 = *(f32*)&lbl_8047BFE8;
                if (f1 > f0) {
                } else {

                    f1 = -f1;
                }
                f0 = *(f32*)&lbl_8047C018;
                if (f1 >= f0) goto L_800694D4;
            }
            *(f32*)((u8*)r4 + 0x0) = f2;
            L_800694D4: ;
            r4 = r4 + 0x4;
        } while (--ctr != 0);
        r5 = r5 + 0x30;
        r3 = r3 + 0x1;
    } while ((s32)r3 < (s32)0x4);
    /* psq_l f31, 0x28((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x20);
    return;
}


/* 0x80069504 | size: 0x160 */
s32 fn_80069504(void) {
    extern void fn_800D3088();
    extern void fn_800D37CC();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    
    *(f64*)(sp + 0x20) = f31;
    /* psq_st f31, 0x28((u32)sp), 0, qr0 */;
    fn_800D37CC();
    /* xoris r3, r3, 0x8000 */;
    r0 = (0x4330 << 16);
    f1 = *(f64*)&lbl_8047BFF0;
    *(u32*)(sp + 0x8) = r0;
    f0 = *(f64*)(sp + 0x8);
    f31 = f0 - f1;
    fn_800D3088();
    r0 = (0x4330 << 16);
    r4 = (u32)&lbl_803A9F08;
    f1 = *(f64*)&lbl_8047C020;
    *(u32*)(sp + 0x10) = r0;
    r5 = (u32)&lbl_803A9F08;
    r6 = r5 + (0x1 << 16);
    r3 = 0x0;
    f0 = *(f64*)(sp + 0x10);
    f0 = f0 - f1;
    f0 = f0 / f31;
    *(f32*)((u8*)r6 + (-12920)) = f0;
    do {
        r4 = r5 + (0x1 << 16);
        /* subi r4, r4, 0x3274 */;
        r0 = 0x6;
        ctr_fn = (void(*)(void))r0;
        do {
            f0 = *(f32*)((u8*)r4 + 0x0);
            f1 = *(f32*)((u8*)r4 + 0x18);
            if (f0 == f1) goto L_80069634;
            f1 = f1 - f0;
            f2 = *(f32*)&lbl_8047C010;
            f0 = *(f32*)((u8*)r6 + (-12920));
            f1 = f2 * f1;
            f3 = f1 * f0;
            if (f3 > f2) {
                f3 = f2;
            }
            f0 = *(f32*)&lbl_8047C014;
            /* cror eq, lt, eq */;
            if (f3 == f0) {
                f3 = f0;
            }
            f1 = *(f32*)((u8*)r4 + 0x0);
            f0 = *(f32*)&lbl_8047BFE8;
            f1 = f1 + f3;
            *(f32*)((u8*)r4 + 0x0) = f1;
            f2 = *(f32*)((u8*)r4 + 0x18);
            f0 = *(f32*)((u8*)r4 + 0x0);
            f1 = f2 - f0;
            if (f3 > f0) {
            } else {

                f3 = -f3;
            }
            f0 = *(f32*)&lbl_8047BFE8;
            if (f1 > f0) {
                f0 = f1;
            } else {

                f0 = -f1;
            }
            /* cror eq, lt, eq */;
            if (f0 != f3) {
                f0 = *(f32*)&lbl_8047BFE8;
                if (f1 > f0) {
                } else {

                    f1 = -f1;
                }
                f0 = *(f32*)&lbl_8047C018;
                if (f1 >= f0) goto L_80069634;
            }
            *(f32*)((u8*)r4 + 0x0) = f2;
            L_80069634: ;
            r4 = r4 + 0x4;
        } while (--ctr != 0);
        r5 = r5 + 0x30;
        r3 = r3 + 0x1;
    } while ((s32)r3 < (s32)0x4);
    /* psq_l f31, 0x28((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x20);
    return;
}


/* 0x80069664 | size: 0x160 */
s32 fn_80069664(void) {
    extern void fn_800D3088();
    extern void fn_800D37CC();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    
    *(f64*)(sp + 0x20) = f31;
    /* psq_st f31, 0x28((u32)sp), 0, qr0 */;
    fn_800D37CC();
    /* xoris r3, r3, 0x8000 */;
    r0 = (0x4330 << 16);
    f1 = *(f64*)&lbl_8047BFF0;
    *(u32*)(sp + 0x8) = r0;
    f0 = *(f64*)(sp + 0x8);
    f31 = f0 - f1;
    fn_800D3088();
    r0 = (0x4330 << 16);
    r4 = (u32)&lbl_803A9F08;
    f1 = *(f64*)&lbl_8047C020;
    *(u32*)(sp + 0x10) = r0;
    r5 = (u32)&lbl_803A9F08;
    r6 = r5 + (0x1 << 16);
    r3 = 0x0;
    f0 = *(f64*)(sp + 0x10);
    f0 = f0 - f1;
    f0 = f0 / f31;
    *(f32*)((u8*)r6 + (-12920)) = f0;
    do {
        r4 = r5 + (0x1 << 16);
        /* subi r4, r4, 0x3274 */;
        r0 = 0x6;
        ctr_fn = (void(*)(void))r0;
        do {
            f0 = *(f32*)((u8*)r4 + 0x0);
            f1 = *(f32*)((u8*)r4 + 0x18);
            if (f0 == f1) goto L_80069794;
            f1 = f1 - f0;
            f2 = *(f32*)&lbl_8047C010;
            f0 = *(f32*)((u8*)r6 + (-12920));
            f1 = f2 * f1;
            f3 = f1 * f0;
            if (f3 > f2) {
                f3 = f2;
            }
            f0 = *(f32*)&lbl_8047C014;
            /* cror eq, lt, eq */;
            if (f3 == f0) {
                f3 = f0;
            }
            f1 = *(f32*)((u8*)r4 + 0x0);
            f0 = *(f32*)&lbl_8047BFE8;
            f1 = f1 + f3;
            *(f32*)((u8*)r4 + 0x0) = f1;
            f2 = *(f32*)((u8*)r4 + 0x18);
            f0 = *(f32*)((u8*)r4 + 0x0);
            f1 = f2 - f0;
            if (f3 > f0) {
            } else {

                f3 = -f3;
            }
            f0 = *(f32*)&lbl_8047BFE8;
            if (f1 > f0) {
                f0 = f1;
            } else {

                f0 = -f1;
            }
            /* cror eq, lt, eq */;
            if (f0 != f3) {
                f0 = *(f32*)&lbl_8047BFE8;
                if (f1 > f0) {
                } else {

                    f1 = -f1;
                }
                f0 = *(f32*)&lbl_8047C018;
                if (f1 >= f0) goto L_80069794;
            }
            *(f32*)((u8*)r4 + 0x0) = f2;
            L_80069794: ;
            r4 = r4 + 0x4;
        } while (--ctr != 0);
        r5 = r5 + 0x30;
        r3 = r3 + 0x1;
    } while ((s32)r3 < (s32)0x4);
    /* psq_l f31, 0x28((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x20);
    return;
}


/* 0x800697C4 | size: 0x30 */
s32 fn_800697C4(void) {
    fn_8010B01C();
    return 0;
}

/* 0x800697F4 | size: 0x150 */
void fn_800697F4(void) {
    extern void fn_80061018();
    extern void fn_8010BBB8();
    extern void fn_80123FBC();
    extern void fn_8025D808();
    extern void fn_8025D938();
    extern void fn_8025D970();
    extern void fn_8025DA3C();
    extern void fn_8025DA88();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    
    r26 = r3;
    r29 = 0x1;
    fn_8025DA3C();
    fn_8025DA88();
    if ((u32)r26 != (u32)0x0) {
        r0 = 0x1;
        *(u8*)((u8*)r26 + 0x0) = r0;
    }
    r3 = (u32)&lbl_803A9F08;
    r31 = (u32)&lbl_803A9F08;
    r30 = r31 + (0x1 << 16);
    do {
        r4 = *(u32*)((u8*)r31 + 0x2C);
        if ((s32)r4 == (s32)0x18) {
            r0 = 0x1;
            r29 = 0x0;
            *(u8*)((u8*)r30 + (-12924)) = r0;
            r27 = 0x0;
            goto L_80069924;
        }
        r3 = (0x2aab << 16);
        /* subi r0, r3, 0x5555 */;
        r3 = (s32)((s64)r0 * (s64)r4 >> 32);
        r0 = (u32)r3 >> 31;
        r28 = r3 + r0;
        r0 = r28 * 0x6;
        r26 = r4 - r0;
        fn_80061018();
        if ((s32)r3 == (s32)0x0) {
            r3 = r28;
            r4 = r26;
            fn_8025D970();
            r27 = r3;
            goto L_800698B8;
        }
        r3 = r28;
        fn_8025D808();
        r0 = r3 & 0xFFFF;
        if ((s32)r0 <= (s32)r26) {
            r27 = 0x0;
            goto L_800698B8;
        }
        r3 = r28;
        r4 = r26;
        fn_8025D938();
        r27 = r3;
        L_800698B8: ;
        r5 = r28 * 0x48;
        r4 = (u32)&lbl_803A9F08;
        r3 = r27;
        r4 = (u32)&lbl_803A9F08;
        r0 = r26 * 0xc;
        r4 = r4 + r5;
        r28 = r4 + r0;
        r28 = r28 + 0x30;
        fn_80123FBC();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x0) {
            r3 = r27;
            fn_8010BBB8();
            r0 = 0x1;
            r29 = 0x0;
            *(u8*)((u8*)r28 + 0x0) = r0;
            *(u16*)((u8*)r28 + 0x2) = r3;
            r3 = *(u32*)((u8*)r31 + 0x2C);
            r0 = r3 + 0x1;
            *(u32*)((u8*)r31 + 0x2C) = r0;
            goto L_80069924;
        }
        r0 = 0x0;
        *(u8*)((u8*)r28 + 0x0) = r0;
        r3 = *(u32*)((u8*)r31 + 0x2C);
        r0 = r3 + 0x1;
        *(u32*)((u8*)r31 + 0x2C) = r0;
        L_80069924: ;
    } while ((s32)r29 != (s32)0x0);
    r3 = r27;
    return;
}


/* 0x80069944 | size: 0xC4 */
void fn_80069944(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 ctr = 0;

    r4 = (u32)&lbl_803A9F08;
    *(u32*)((u8*)r4 + 0x2C) = 0;
    r3 = r4 + (0x1 << 16);
    *(u8*)((u8*)r3 + (-12924)) = 0;
    ctr = 2;
    do {
        r3 = r4 + 0x30;
        r0 = 0x0;
        *(u8*)((u8*)r3 + 0x0) = r0;
        r9 = r3 + 0xc;
        r8 = r3 + 0x18;
        r7 = r3 + 0x24;
        *(u32*)((u8*)r3 + 0x4) = r0;
        r6 = r3 + 0x30;
        r5 = r3 + 0x3c;
        r4 = r4 + 0x48;
        *(u8*)((u8*)r9 + 0x0) = r0;
        r3 = r4 + 0x30;
        r4 = r4 + 0x48;
        *(u32*)((u8*)r9 + 0x4) = r0;
        r9 = r3 + 0xc;
        *(u8*)((u8*)r8 + 0x0) = r0;
        *(u32*)((u8*)r8 + 0x4) = r0;
        r8 = r3 + 0x18;
        *(u8*)((u8*)r7 + 0x0) = r0;
        *(u32*)((u8*)r7 + 0x4) = r0;
        r7 = r3 + 0x24;
        *(u8*)((u8*)r6 + 0x0) = r0;
        *(u32*)((u8*)r6 + 0x4) = r0;
        r6 = r3 + 0x30;
        *(u8*)((u8*)r5 + 0x0) = r0;
        *(u32*)((u8*)r5 + 0x4) = r0;
        r5 = r3 + 0x3c;
        *(u8*)((u8*)r3 + 0x0) = r0;
        *(u32*)((u8*)r3 + 0x4) = r0;
        *(u8*)((u8*)r9 + 0x0) = r0;
        *(u32*)((u8*)r9 + 0x4) = r0;
        *(u8*)((u8*)r8 + 0x0) = r0;
        *(u32*)((u8*)r8 + 0x4) = r0;
        *(u8*)((u8*)r7 + 0x0) = r0;
        *(u32*)((u8*)r7 + 0x4) = r0;
        *(u8*)((u8*)r6 + 0x0) = r0;
        *(u32*)((u8*)r6 + 0x4) = r0;
        *(u8*)((u8*)r5 + 0x0) = r0;
        *(u32*)((u8*)r5 + 0x4) = r0;
    } while (--ctr != 0);
}


/* 0x80069A08 | size: 0x58 */
s32 fn_80069A08(void) {
    extern void fn_8010B9E8();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    
    r7 = r5 * 0x48;
    r5 = (u32)&lbl_803A9F08;
    r5 = (u32)&lbl_803A9F08;
    r0 = r6 * 0xc;
    r5 = r5 + r7;
    r5 = r5 + r0;
    r5 = r5 + 0x30;
    r0 = *(u8*)((u8*)r5 + 0x0);
    if ((u32)r0 != (u32)0x0) {
        r5 = *(u16*)((u8*)r5 + 0x2);
        fn_8010B9E8();
        r3 = 0x1;
    } else {

        r3 = 0x0;
    }
    return;
}


