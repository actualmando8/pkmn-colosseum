    nofralloc
    stwu r1, -0xa0(r1)
    mflr r0
    stw r0, 0xa4(r1)
    stfd f31, 0x90(r1)
    psq_st f31, 0x98(r1), 0, 0
    stfd f30, 0x80(r1)
    psq_st f30, 0x88(r1), 0, 0
    stfd f29, 0x70(r1)
    psq_st f29, 0x78(r1), 0, 0
    stfd f28, 0x60(r1)
    psq_st f28, 0x68(r1), 0, 0
    stmw r23, 0x3c(r1)
    fmr f31, f1
    mr r31, r3
    fmr f28, f2
    mr r23, r4
    fmr f30, f3
    fmr f29, f4
    lbz r0, -21616(r13)
    cmplwi r0, 0x1
    bne @L_801C58F8
    li r0, 0x0
    stb r0, -21616(r13)
    bl fn_801C5D60
@L_801C58F8:
    fdivs f28, f31, f28
    lis r3, -32698
    addi r29, r3, 28720
    lhz r3, 0x8(r29)
    lfs f1, 0x14(r29)
    lfs f0, 0x10(r29)
    fadds f0, f1, f0
    lfs f1, 0x18(r29)
    addi r0, r3, 0x1
    fdivs f29, f30, f29
    sth r0, 0x8(r29)
    stfs f0, 0x14(r29)
    fcmpo cr0, f0, f1
    cror eq, gt, eq
    bne @L_801C5938
    stfs f1, 0x14(r29)
@L_801C5938:
    fmr f1, f28
    bl fn_801C6688
    cmplwi r23, 0x0
    bne @L_801C5950
    mr r3, r31
    b @L_801C5B2C
@L_801C5950:
    lfs f2, -22208(r2)
    fmr f3, f28
    lfs f1, -22168(r2)
    fmr f4, f29
    lfs f0, -22164(r2)
    mr r3, r23
    stfs f1, 0x18(r1)
    lfs f1, -22204(r2)
    addi r4, r1, 0x18
    stfs f0, 0x1c(r1)
    stfs f2, 0x20(r1)
    bl fn_801C63C0
    lis r3, -32698
    addi r3, r3, 28720
    lwz r3, 0x0(r3)
    bl fn_801C680C
    lfs f4, 0x14(r29)
    lis r3, -32698
    lfs f1, -22096(r2)
    addi r27, r3, 28240
    lfs f0, -22204(r2)
    fneg f3, f4
    fdivs f1, f4, f1
    lfs f2, -22100(r2)
    stfs f4, 0x10(r1)
    addi r26, r1, 0x8
    addi r25, r1, 0x10
    stfs f3, 0x14(r1)
    fmuls f1, f2, f1
    li r24, 0x0
    fsubs f31, f0, f28
    fneg f0, f1
    stfs f1, 0x8(r1)
    stfs f0, 0xc(r1)
    lfd f28, -22080(r2)
    lis r30, 0x4330
    lfs f29, -22208(r2)
    lfs f30, -22092(r2)
@L_801C59E8:
    addi r28, r27, 0xdc
    li r23, 0xb
@L_801C59F0:
    lhz r0, 0x8(r29)
    rlwinm r0, r0, 0, 30, 31
    cmpwi r0, 0x0
    bne @L_801C5A20
    lfs f0, -0x14(r28)
    stfs f0, 0x0(r28)
    lfs f0, -0x10(r28)
    stfs f0, 0x4(r28)
    lfs f0, -0xc(r28)
    stfs f0, 0x8(r28)
    lfs f0, -0x8(r28)
    stfs f0, 0xc(r28)
@L_801C5A20:
    lwz r0, 0x10(r28)
    mr r3, r28
    stw r30, 0x28(r1)
    xoris r0, r0, 0x8000
    lfs f1, -22204(r2)
    stw r0, 0x2c(r1)
    lfs f2, 0xc(r28)
    lfd f0, 0x28(r1)
    fsubs f0, f0, f28
    fmuls f0, f31, f0
    fctiwz f0, f0
    stfd f0, 0x30(r1)
    lwz r4, 0x34(r1)
    bl fn_801C5B60
    subi r28, r28, 0x14
    subi r23, r23, 0x1
    cmpwi r23, 0x0
    bgt @L_801C59F0
    lfs f1, 0xc(r27)
    lfs f0, 0x0(r26)
    fadds f1, f1, f0
    fcmpo cr0, f1, f29
    cror eq, lt, eq
    bne @L_801C5A8C
    lfs f0, -22100(r2)
    fadds f1, f1, f0
    b @L_801C5AA0
@L_801C5A8C:
    lfs f0, -22100(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801C5AA0
    fsubs f1, f1, f0
@L_801C5AA0:
    stfs f1, 0xc(r27)
    lfs f0, 0x0(r25)
    lfs f1, 0x0(r27)
    fadds f1, f1, f0
    fcmpo cr0, f1, f30
    ble @L_801C5AC0
    fmr f1, f30
    b @L_801C5AD0
@L_801C5AC0:
    lfs f0, -22088(r2)
    fcmpo cr0, f1, f0
    bge @L_801C5AD0
    fmr f1, f0
@L_801C5AD0:
    stfs f1, 0x0(r27)
    mr r3, r27
    lfs f1, -22204(r2)
    lwz r0, 0x10(r27)
    stw r30, 0x30(r1)
    xoris r0, r0, 0x8000
    lfs f2, 0xc(r27)
    stw r0, 0x34(r1)
    lfd f0, 0x30(r1)
    fsubs f0, f0, f28
    fmuls f0, f31, f0
    fctiwz f0, f0
    stfd f0, 0x28(r1)
    lwz r4, 0x2c(r1)
    bl fn_801C5B60
    addi r27, r27, 0xf0
    addi r26, r26, 0x4
    addi r25, r25, 0x4
    addi r24, r24, 0x1
    cmpwi r24, 0x2
    blt @L_801C59E8
    bl fn_801C673C
    mr r3, r31
@L_801C5B2C:
    psq_l f31, 0x98(r1), 0, 0
    lfd f31, 0x90(r1)
    psq_l f30, 0x88(r1), 0, 0
    lfd f30, 0x80(r1)
    psq_l f29, 0x78(r1), 0, 0
    lfd f29, 0x70(r1)
    psq_l f28, 0x68(r1), 0, 0
    lfd f28, 0x60(r1)
    lmw r23, 0x3c(r1)
    lwz r0, 0xa4(r1)
    mtlr r0
    addi r1, r1, 0xa0
    blr
