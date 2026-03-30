    nofralloc
    stwu r1, -0xb0(r1)
    mflr r0
    stw r0, 0xb4(r1)
    stfd f31, 0xa0(r1)
    psq_st f31, 0xa8(r1), 0, 0
    stfd f30, 0x90(r1)
    psq_st f30, 0x98(r1), 0, 0
    stfd f29, 0x80(r1)
    psq_st f29, 0x88(r1), 0, 0
    stfd f28, 0x70(r1)
    psq_st f28, 0x78(r1), 0, 0
    stfd f27, 0x60(r1)
    psq_st f27, 0x68(r1), 0, 0
    stmw r23, 0x3c(r1)
    fmr f31, f1
    mr r30, r4
    mr r23, r5
    mr r29, r3
    mr r31, r6
    addi r3, r1, 0x8
    mr r24, r23
    addi r4, r30, 0x14
    bl fn_800E01D0
    cmplwi r31, 0x0
    lhz r26, 0x12e2(r29)
    beq @L_8013A24C
    lfs f1, lbl_8047D1B4(r2)
    lfs f0, 0x4(r23)
    fmuls f27, f1, f0
    b @L_8013A250
    @L_8013A24C:
    lfs f27, lbl_8047D190(r2)
    @L_8013A250:
    addi r0, r31, 0x1
    lis r3, 0x4330
    stw r0, 0x1c(r1)
    mulli r0, r26, 0x18
    lfd f1, lbl_8047D1A0(r2)
    mr r27, r26
    stw r3, 0x18(r1)
    add r28, r29, r0
    lfs f2, lbl_8047D1A8(r2)
    lfd f0, 0x18(r1)
    mr r23, r28
    fsubs f0, f0, f1
    fdivs f28, f2, f0
    b @L_8013A328
    @L_8013A288:
    stw r24, 0xc(r23)
    mr r25, r24
    mr r24, r23
    bl fn_800E0BA0
    lfs f2, 0x10(r1)
    lfs f0, 0x34(r30)
    fmadds f29, f2, f1, f0
    bl fn_800E0BA0
    lfs f2, 0xc(r1)
    lfs f0, 0x30(r30)
    fmadds f30, f2, f1, f0
    bl fn_800E0BA0
    lfs f4, 0x8(r1)
    fmr f2, f30
    lfs f0, 0x2c(r30)
    fmr f3, f29
    mr r3, r23
    fmadds f1, f4, f1, f0
    bl fn_800E01F4
    mr r3, r23
    mr r4, r23
    mr r5, r25
    bl fn_800E019C
    lhz r3, 0x12e2(r29)
    addi r0, r3, 0x1
    sth r0, 0x12e2(r29)
    lfs f0, 0x4(r23)
    fcmpo cr0, f0, f27
    blt @L_8013A308
    bl fn_800E0BE4
    fcmpo cr0, f28, f1
    bge @L_8013A320
    @L_8013A308:
    cmplwi r31, 0x0
    bne @L_8013A330
    mr r4, r23
    addi r3, r29, 0x12cc
    bl fn_800E01D0
    b @L_8013A330
    @L_8013A320:
    addi r27, r27, 0x1
    addi r23, r23, 0x18
    @L_8013A328:
    cmplwi r27, 0xc8
    blt @L_8013A288
    @L_8013A330:
    lhz r4, 0x46(r30)
    cmplw r31, r4
    bge @L_8013A3F0
    lis r3, 0x4330
    subf r0, r31, r4
    stw r0, 0x1c(r1)
    cmplwi r31, 0x0
    lfd f2, lbl_8047D1A0(r2)
    stw r3, 0x18(r1)
    lfs f3, 0x40(r30)
    lfd f0, 0x18(r1)
    stw r4, 0x24(r1)
    fsubs f1, f0, f2
    lhz r23, 0x12e2(r29)
    stw r3, 0x20(r1)
    lfd f0, 0x20(r1)
    fmuls f1, f3, f1
    fsubs f0, f0, f2
    fdivs f28, f1, f0
    beq @L_8013A39C
    subf r0, r26, r23
    stw r3, 0x28(r1)
    stw r0, 0x2c(r1)
    lfd f0, 0x28(r1)
    fsubs f0, f0, f2
    fdivs f27, f31, f0
    b @L_8013A3A0
    @L_8013A39C:
    lfs f27, lbl_8047D190(r2)
    @L_8013A3A0:
    lfs f30, lbl_8047D1AC(r2)
    mr r27, r26
    b @L_8013A3E8
    @L_8013A3AC:
    fsubs f0, f31, f27
    stfs f31, 0x10(r28)
    fmr f31, f0
    stfs f0, 0x14(r28)
    bl fn_800E0BE4
    fcmpo cr0, f28, f1
    ble @L_8013A3E0
    fmuls f1, f30, f31
    lwz r5, 0xc(r28)
    mr r3, r29
    mr r4, r30
    addi r6, r31, 0x1
    bl fn_8013A1D4
    @L_8013A3E0:
    addi r27, r27, 0x1
    addi r28, r28, 0x18
    @L_8013A3E8:
    cmplw r27, r23
    blt @L_8013A3AC
    @L_8013A3F0:
    psq_l f31, 0xa8(r1), 0, 0
    lfd f31, 0xa0(r1)
    psq_l f30, 0x98(r1), 0, 0
    lfd f30, 0x90(r1)
    psq_l f29, 0x88(r1), 0, 0
    lfd f29, 0x80(r1)
    psq_l f28, 0x78(r1), 0, 0
    lfd f28, 0x70(r1)
    psq_l f27, 0x68(r1), 0, 0
    lfd f27, 0x60(r1)
    lmw r23, 0x3c(r1)
    lwz r0, 0xb4(r1)
    mtlr r0
    addi r1, r1, 0xb0
    blr
