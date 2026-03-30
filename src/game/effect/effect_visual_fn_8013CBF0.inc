    nofralloc
    stwu r1, -0xc0(r1)
    mflr r0
    stw r0, 0xc4(r1)
    stfd f31, 0xb0(r1)
    psq_st f31, 0xb8(r1), 0, 0
    stfd f30, 0xa0(r1)
    psq_st f30, 0xa8(r1), 0, 0
    stfd f29, 0x90(r1)
    psq_st f29, 0x98(r1), 0, 0
    stfd f28, 0x80(r1)
    psq_st f28, 0x88(r1), 0, 0
    stfd f27, 0x70(r1)
    psq_st f27, 0x78(r1), 0, 0
    stmw r22, 0x48(r1)
    fmr f28, f1
    lhz r31, 0x18(r3)
    fmr f27, f2
    lhz r30, 0x1a(r3)
    fmr f29, f3
    lwz r29, 0x4(r3)
    lwz r28, 0x8(r3)
    mr r25, r4
    lwz r27, 0xc(r3)
    mr r26, r5
    bl fn_800B8DF4
    bl fn_800B856C
    lfs f1, lbl_8047D23C(r2)
    lis r3, lbl_80363CB8@ha
    lfs f2, lbl_8047D240(r2)
    addi r3, r3, lbl_80363CB8@l
    fmr f3, f1
    bl fn_800E01F4
    lfs f0, lbl_8047D258(r2)
    addi r3, r1, 0x2c
    lfs f2, lbl_8047D23C(r2)
    fmuls f1, f0, f28
    fmuls f3, f0, f27
    bl fn_800E01F4
    subi r3, r31, 0x1
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    lfs f2, lbl_8047D23C(r2)
    stw r3, 0x3c(r1)
    addi r3, r1, 0x14
    lfd f1, lbl_8047D250(r2)
    fmr f3, f2
    stw r0, 0x38(r1)
    lfd f0, 0x38(r1)
    fsubs f0, f0, f1
    fdivs f1, f28, f0
    bl fn_800E01F4
    subi r3, r30, 0x1
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    lfs f1, lbl_8047D23C(r2)
    stw r3, 0x44(r1)
    addi r3, r1, 0x8
    lfd f3, lbl_8047D250(r2)
    fmr f2, f1
    stw r0, 0x40(r1)
    lfd f0, 0x40(r1)
    fsubs f0, f0, f3
    fdivs f3, f27, f0
    bl fn_800E01F4
    lfs f0, 0x2c(r1)
    addi r3, r1, 0x20
    lfs f3, 0x34(r1)
    addi r4, r1, 0x2c
    fmuls f1, f0, f0
    lfs f2, lbl_8047D240(r2)
    fmuls f0, f3, f3
    fdivs f31, f2, f1
    fdivs f30, f2, f0
    bl fn_800E01D0
    lfd f27, lbl_8047D248(r2)
    li r23, 0x0
    lfs f28, lbl_8047D240(r2)
    lis r24, 0x4330
    b @L_8013CE10
    @L_8013CD2C:
    li r22, 0x0
    b @L_8013CDE8
    @L_8013CD34:
    mr r3, r29
    mr r4, r25
    addi r5, r1, 0x20
    bl fn_800E019C
    lfs f1, 0x20(r1)
    addi r3, r1, 0x20
    lfs f0, 0x2c(r1)
    mr r4, r3
    stw r24, 0x40(r1)
    addi r5, r1, 0x8
    fsubs f0, f1, f0
    fmuls f0, f29, f0
    stfs f0, 0x4(r28)
    lfs f1, 0x28(r1)
    lfs f0, 0x34(r1)
    fsubs f0, f1, f0
    fmuls f0, f29, f0
    stfs f0, 0x0(r28)
    lbz r0, 0x0(r26)
    stb r0, 0x0(r27)
    lbz r0, 0x1(r26)
    stb r0, 0x1(r27)
    lbz r0, 0x2(r26)
    stb r0, 0x2(r27)
    lbz r0, 0x3(r26)
    lfs f0, 0x20(r1)
    stw r0, 0x44(r1)
    fmuls f1, f0, f0
    lfs f0, 0x28(r1)
    lfd f2, 0x40(r1)
    fmuls f0, f0, f0
    fsubs f2, f2, f27
    fnmsubs f1, f31, f1, f28
    fnmsubs f0, f30, f0, f28
    fmuls f1, f2, f1
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x38(r1)
    lwz r0, 0x3c(r1)
    stb r0, 0x3(r27)
    bl fn_800E019C
    addi r22, r22, 0x1
    addi r29, r29, 0xc
    addi r28, r28, 0x8
    addi r27, r27, 0x4
    @L_8013CDE8:
    clrlwi r0, r22, 16
    cmplw r0, r30
    blt @L_8013CD34
    addi r3, r1, 0x20
    addi r5, r1, 0x14
    mr r4, r3
    bl fn_800E019C
    lfs f0, 0x34(r1)
    addi r23, r23, 0x1
    stfs f0, 0x28(r1)
    @L_8013CE10:
    clrlwi r0, r23, 16
    cmplw r0, r31
    blt @L_8013CD2C
    psq_l f31, 0xb8(r1), 0, 0
    lfd f31, 0xb0(r1)
    psq_l f30, 0xa8(r1), 0, 0
    lfd f30, 0xa0(r1)
    psq_l f29, 0x98(r1), 0, 0
    lfd f29, 0x90(r1)
    psq_l f28, 0x88(r1), 0, 0
    lfd f28, 0x80(r1)
    psq_l f27, 0x78(r1), 0, 0
    lfd f27, 0x70(r1)
    lmw r22, 0x48(r1)
    lwz r0, 0xc4(r1)
    mtlr r0
    addi r1, r1, 0xc0
    blr
