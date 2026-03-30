    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stfd f31, 0x40(r1)
    psq_st f31, 0x48(r1), 0, 0
    stfd f30, 0x30(r1)
    psq_st f30, 0x38(r1), 0, 0
    stfd f29, 0x20(r1)
    psq_st f29, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr. r29, r3
    mr r30, r4
    beq @L_8013EA0C
    lfs f1, lbl_8047D2B8(r2)
    lfs f0, 0x24(r29)
    lbz r31, 0x19(r29)
    fdivs f29, f1, f0
    lfs f30, 0x1c(r29)
    clrlwi. r0, r31, 31
    lfs f31, 0x20(r29)
    beq @L_8013E918
    lfs f1, lbl_8047D2A8(r2)
    fadds f0, f1, f30
    stfs f1, 0x2c(r29)
    fadds f0, f0, f31
    stfs f0, 0x28(r29)
    b @L_8013E9DC
    @L_8013E918:
    rlwinm. r0, r31, 0, 30, 30
    beq @L_8013E968
    lhz r3, 0x30(r29)
    lis r0, 0x4330
    lfs f0, lbl_8047D2BC(r2)
    stw r3, 0xc(r1)
    lfd f1, lbl_8047D2D8(r2)
    fmuls f2, f0, f29
    stw r0, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f1, f0, f1
    bl fn_800CE318
    frsp f1, f1
    lfs f0, lbl_8047D2C0(r2)
    lfd f2, lbl_8047D2C8(r2)
    fmuls f0, f0, f1
    fdivs f1, f0, f29
    bl fn_800CE318
    frsp f1, f1
    b @L_8013E99C
    @L_8013E968:
    lhz r3, 0x30(r29)
    lis r0, 0x4330
    stw r0, 0x8(r1)
    fmr f2, f29
    lfd f1, lbl_8047D2D8(r2)
    stw r3, 0xc(r1)
    lfd f0, 0x8(r1)
    fsubs f1, f0, f1
    bl fn_800CE318
    frsp f1, f1
    lfs f0, lbl_8047D2D0(r2)
    fmuls f0, f0, f1
    fdivs f1, f0, f29
    @L_8013E99C:
    bl fn_800CDBE0
    rlwinm. r0, r31, 0, 28, 28
    frsp f2, f1
    beq @L_8013E9B8
    lfs f0, lbl_8047D2A8(r2)
    stfs f0, 0x2c(r29)
    b @L_8013E9CC
    @L_8013E9B8:
    lfs f0, lbl_8047D2A8(r2)
    lfs f1, lbl_8047D2D4(r2)
    fadds f0, f0, f2
    fmuls f0, f1, f0
    stfs f0, 0x2c(r29)
    @L_8013E9CC:
    fnmsubs f0, f31, f2, f30
    lfs f1, lbl_8047D2A8(r2)
    fadds f0, f1, f0
    stfs f0, 0x28(r29)
    @L_8013E9DC:
    lhz r0, 0x30(r29)
    li r3, 0x0
    add r0, r0, r30
    sth r0, 0x30(r29)
    lhz r4, 0x32(r29)
    cmpwi r4, -0x1
    beq @L_8013EA04
    lhz r0, 0x30(r29)
    cmplw r0, r4
    bge @L_8013EA10
    @L_8013EA04:
    li r3, 0x1
    b @L_8013EA10
    @L_8013EA0C:
    li r3, 0x0
    @L_8013EA10:
    psq_l f31, 0x48(r1), 0, 0
    lfd f31, 0x40(r1)
    psq_l f30, 0x38(r1), 0, 0
    lfd f30, 0x30(r1)
    psq_l f29, 0x28(r1), 0, 0
    lfd f29, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r0, 0x54(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
