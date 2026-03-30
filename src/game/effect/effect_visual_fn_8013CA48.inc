    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stfd f31, 0x80(r1)
    psq_st f31, 0x88(r1), 0, 0
    stfd f30, 0x70(r1)
    psq_st f30, 0x78(r1), 0, 0
    stfd f29, 0x60(r1)
    psq_st f29, 0x68(r1), 0, 0
    stfd f28, 0x50(r1)
    psq_st f28, 0x58(r1), 0, 0
    stmw r22, 0x28(r1)
    mr. r30, r3
    mr r31, r4
    beq @L_8013CBB8
    lhz r4, 0xa4(r30)
    lhz r3, 0xa6(r30)
    cmplw r4, r3
    blt @L_8013CA9C
    li r3, 0x0
    b @L_8013CBBC
    @L_8013CA9C:
    lis r0, 0x4330
    stw r4, 0xc(r1)
    lfd f2, lbl_8047D248(r2)
    stw r0, 0x8(r1)
    lwz r22, 0x80(r30)
    lfd f0, 0x8(r1)
    stw r3, 0x14(r1)
    fsubs f1, f0, f2
    lwz r23, 0x84(r30)
    stw r0, 0x10(r1)
    lwz r24, 0x88(r30)
    lfd f0, 0x10(r1)
    lhz r27, 0x18(r30)
    fsubs f0, f0, f2
    lhz r28, 0x1a(r30)
    fdivs f28, f1, f0
    bl fn_800B8DF4
    bl fn_800B856C
    lfs f1, lbl_8047D23C(r2)
    lis r3, lbl_80363CB8@ha
    lfs f2, lbl_8047D240(r2)
    addi r3, r3, lbl_80363CB8@l
    fmr f3, f1
    bl fn_800E01F4
    subi r3, r28, 0x1
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    stw r0, 0x18(r1)
    lfs f3, lbl_8047D244(r2)
    li r25, 0x0
    stw r3, 0x1c(r1)
    lfs f2, 0x68(r30)
    lfd f1, lbl_8047D250(r2)
    lfd f0, 0x18(r1)
    fmuls f3, f3, f2
    lfs f2, 0xa0(r30)
    fsubs f0, f0, f1
    lwz r29, 0x4(r30)
    fmuls f1, f3, f2
    fdivs f31, f3, f0
    fmuls f28, f28, f1
    b @L_8013CB9C
    @L_8013CB44:
    lfs f2, 0x0(r23)
    li r26, 0x0
    lfs f1, 0x0(r24)
    fdivs f0, f28, f2
    fdivs f30, f31, f2
    fadds f29, f1, f0
    b @L_8013CB84
    @L_8013CB60:
    fmr f1, f29
    bl fn_800E0CA0
    lfs f2, 0x0(r22)
    fadds f29, f29, f30
    lfs f0, 0x4c(r30)
    addi r26, r26, 0x1
    fmadds f0, f2, f1, f0
    stfs f0, 0x4(r29)
    addi r29, r29, 0xc
    @L_8013CB84:
    cmpw r26, r28
    blt @L_8013CB60
    addi r25, r25, 0x1
    addi r22, r22, 0x4
    addi r23, r23, 0x4
    addi r24, r24, 0x4
    @L_8013CB9C:
    cmpw r25, r27
    blt @L_8013CB44
    lhz r0, 0xa4(r30)
    li r3, 0x1
    add r0, r0, r31
    sth r0, 0xa4(r30)
    b @L_8013CBBC
    @L_8013CBB8:
    li r3, 0x0
    @L_8013CBBC:
    psq_l f31, 0x88(r1), 0, 0
    lfd f31, 0x80(r1)
    psq_l f30, 0x78(r1), 0, 0
    lfd f30, 0x70(r1)
    psq_l f29, 0x68(r1), 0, 0
    lfd f29, 0x60(r1)
    psq_l f28, 0x58(r1), 0, 0
    lfd f28, 0x50(r1)
    lmw r22, 0x28(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
