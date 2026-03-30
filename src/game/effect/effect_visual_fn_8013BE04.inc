    nofralloc
    stwu r1, -0xd0(r1)
    mflr r0
    stw r0, 0xd4(r1)
    stfd f31, 0xc0(r1)
    psq_st f31, 0xc8(r1), 0, 0
    stfd f30, 0xb0(r1)
    psq_st f30, 0xb8(r1), 0, 0
    stfd f29, 0xa0(r1)
    psq_st f29, 0xa8(r1), 0, 0
    stfd f28, 0x90(r1)
    psq_st f28, 0x98(r1), 0, 0
    stfd f27, 0x80(r1)
    psq_st f27, 0x88(r1), 0, 0
    stmw r20, 0x50(r1)
    fmr f27, f2
    lfs f0, lbl_8047D220(r2)
    fmr f28, f1
    lhz r29, 0x1c(r3)
    fmr f29, f3
    lhz r27, 0x1e(r3)
    fmuls f1, f0, f28
    lwz r26, 0x4(r3)
    fmuls f3, f0, f27
    lwz r25, 0x8(r3)
    lwz r24, 0xc(r3)
    mr r21, r4
    lwz r23, 0x10(r3)
    mr r22, r5
    lfs f2, lbl_8047D204(r2)
    addi r3, r1, 0x2c
    bl fn_800E01F4
    subi r3, r29, 0x1
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    lfs f2, lbl_8047D204(r2)
    stw r3, 0x3c(r1)
    addi r3, r1, 0x14
    lfd f1, lbl_8047D228(r2)
    fmr f3, f2
    stw r0, 0x38(r1)
    lfd f0, 0x38(r1)
    fsubs f0, f0, f1
    fdivs f1, f28, f0
    bl fn_800E01F4
    subi r3, r27, 0x1
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    lfs f1, lbl_8047D204(r2)
    stw r3, 0x44(r1)
    addi r3, r1, 0x8
    lfd f3, lbl_8047D228(r2)
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
    lfs f2, lbl_8047D21C(r2)
    fmuls f0, f3, f3
    fdivs f31, f2, f1
    fdivs f30, f2, f0
    bl fn_800E01D0
    lis r3, lbl_8031554C@ha
    lfd f27, lbl_8047D208(r2)
    lfs f28, lbl_8047D21C(r2)
    addi r31, r3, lbl_8031554C@l
    li r30, 0x0
    lis r20, 0x4330
    b @L_8013C020
    @L_8013BF2C:
    li r28, 0x0
    b @L_8013BFF8
    @L_8013BF34:
    mr r3, r26
    mr r4, r21
    addi r5, r1, 0x20
    bl fn_800E019C
    mr r3, r25
    mr r4, r31
    bl fn_800E01D0
    lfs f1, 0x20(r1)
    addi r3, r1, 0x20
    lfs f0, 0x2c(r1)
    mr r4, r3
    stw r20, 0x40(r1)
    addi r5, r1, 0x8
    fsubs f0, f1, f0
    fmuls f0, f29, f0
    stfs f0, 0x4(r24)
    lfs f1, 0x28(r1)
    lfs f0, 0x34(r1)
    fsubs f0, f1, f0
    fmuls f0, f29, f0
    stfs f0, 0x0(r24)
    lbz r0, 0x0(r22)
    stb r0, 0x0(r23)
    lbz r0, 0x1(r22)
    stb r0, 0x1(r23)
    lbz r0, 0x2(r22)
    stb r0, 0x2(r23)
    lbz r0, 0x3(r22)
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
    stb r0, 0x3(r23)
    bl fn_800E019C
    addi r28, r28, 0x1
    addi r26, r26, 0xc
    addi r25, r25, 0xc
    addi r24, r24, 0x8
    addi r23, r23, 0x4
    @L_8013BFF8:
    clrlwi r0, r28, 16
    cmplw r0, r27
    blt @L_8013BF34
    addi r3, r1, 0x20
    addi r5, r1, 0x14
    mr r4, r3
    bl fn_800E019C
    lfs f0, 0x34(r1)
    addi r30, r30, 0x1
    stfs f0, 0x28(r1)
    @L_8013C020:
    clrlwi r0, r30, 16
    cmplw r0, r29
    blt @L_8013BF2C
    li r3, 0x0
    li r4, 0x0
    bl fn_800E0E14
    psq_l f31, 0xc8(r1), 0, 0
    lfd f31, 0xc0(r1)
    psq_l f30, 0xb8(r1), 0, 0
    lfd f30, 0xb0(r1)
    psq_l f29, 0xa8(r1), 0, 0
    lfd f29, 0xa0(r1)
    psq_l f28, 0x98(r1), 0, 0
    lfd f28, 0x90(r1)
    psq_l f27, 0x88(r1), 0, 0
    lfd f27, 0x80(r1)
    lmw r20, 0x50(r1)
    lwz r0, 0xd4(r1)
    mtlr r0
    addi r1, r1, 0xd0
    blr
