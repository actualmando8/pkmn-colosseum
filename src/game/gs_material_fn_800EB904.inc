    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stfd f31, 0x80(r1)
    psq_st f31, 0x88(r1), 0, 0
    stfd f30, 0x70(r1)
    psq_st f30, 0x78(r1), 0, 0
    stw r31, 0x6c(r1)
    stw r30, 0x68(r1)
    stw r29, 0x64(r1)
    stw r28, 0x60(r1)
    fmr f30, f1
    lfs f0, lbl_8047CC50(r2)
    mr. r29, r4
    mr r28, r3
    mr r30, r5
    mr r31, r6
    fsubs f31, f0, f30
    bne @L_800EB960
    la r3, lbl_8047CC40(r2)
    li r4, 0x3e4
    la r5, lbl_8047CC48(r2)
    bl fn_80196E10
    @L_800EB960:
    lwz r3, 0x38(r29)
    cmplwi r30, 0x0
    lwz r0, 0x3c(r29)
    stw r3, 0x44(r1)
    stw r0, 0x48(r1)
    lwz r0, 0x40(r29)
    stw r0, 0x4c(r1)
    bne @L_800EB990
    la r3, lbl_8047CC40(r2)
    li r4, 0x3e4
    la r5, lbl_8047CC48(r2)
    bl fn_80196E10
    @L_800EB990:
    lwz r3, 0x38(r30)
    cmplwi r31, 0x0
    lwz r0, 0x3c(r30)
    lfs f5, 0x44(r1)
    stw r3, 0x38(r1)
    lfs f3, 0x48(r1)
    stw r0, 0x3c(r1)
    lfs f1, 0x38(r1)
    lwz r0, 0x40(r30)
    lfs f0, 0x3c(r1)
    fmuls f4, f1, f30
    stw r0, 0x40(r1)
    fmuls f2, f0, f30
    lfs f1, 0x4c(r1)
    lfs f0, 0x40(r1)
    fmadds f4, f5, f31, f4
    fmuls f0, f0, f30
    fmadds f2, f3, f31, f2
    stfs f4, 0x50(r1)
    fmadds f0, f1, f31, f0
    stfs f2, 0x54(r1)
    stfs f0, 0x58(r1)
    bne @L_800EB9FC
    la r3, lbl_8047CC40(r2)
    li r4, 0x3a9
    la r5, lbl_8047CC48(r2)
    bl fn_80196E10
    @L_800EB9FC:
    lwz r0, 0x50(r1)
    lwz r3, 0x54(r1)
    stw r0, 0x38(r31)
    lwz r0, 0x58(r1)
    stw r3, 0x3c(r31)
    stw r0, 0x40(r31)
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800EBA68
    cmplwi r31, 0x0
    beq @L_800EBA68
    bne @L_800EBA3C
    la r3, lbl_8047CC40(r2)
    li r4, 0x25d
    la r5, lbl_8047CC48(r2)
    bl fn_80196E10
    @L_800EBA3C:
    lwz r4, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EBA58
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EBA58
    li r3, 0x1
    @L_800EBA58:
    cmpwi r3, 0x0
    bne @L_800EBA68
    mr r3, r31
    bl fn_8019D620
    @L_800EBA68:
    cmplwi r29, 0x0
    bne @L_800EBA80
    la r3, lbl_8047CC40(r2)
    li r4, 0x351
    la r5, lbl_8047CC48(r2)
    bl fn_80196E10
    @L_800EBA80:
    lwz r3, 0x2c(r29)
    cmplwi r30, 0x0
    lwz r0, 0x30(r29)
    stw r3, 0x44(r1)
    stw r0, 0x48(r1)
    lwz r0, 0x34(r29)
    stw r0, 0x4c(r1)
    bne @L_800EBAB0
    la r3, lbl_8047CC40(r2)
    li r4, 0x351
    la r5, lbl_8047CC48(r2)
    bl fn_80196E10
    @L_800EBAB0:
    lwz r3, 0x2c(r30)
    cmplwi r31, 0x0
    lwz r0, 0x30(r30)
    lfs f5, 0x44(r1)
    stw r3, 0x38(r1)
    lfs f3, 0x48(r1)
    stw r0, 0x3c(r1)
    lfs f1, 0x38(r1)
    lwz r0, 0x34(r30)
    lfs f0, 0x3c(r1)
    fmuls f4, f1, f30
    stw r0, 0x40(r1)
    fmuls f2, f0, f30
    lfs f1, 0x4c(r1)
    lfs f0, 0x40(r1)
    fmadds f4, f5, f31, f4
    fmuls f0, f0, f30
    fmadds f2, f3, f31, f2
    stfs f4, 0x50(r1)
    fmadds f0, f1, f31, f0
    stfs f2, 0x54(r1)
    stfs f0, 0x58(r1)
    bne @L_800EBB1C
    la r3, lbl_8047CC40(r2)
    li r4, 0x316
    la r5, lbl_8047CC48(r2)
    bl fn_80196E10
    @L_800EBB1C:
    lwz r0, 0x50(r1)
    lwz r3, 0x54(r1)
    stw r0, 0x2c(r31)
    lwz r0, 0x58(r1)
    stw r3, 0x30(r31)
    stw r0, 0x34(r31)
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800EBB88
    cmplwi r31, 0x0
    beq @L_800EBB88
    bne @L_800EBB5C
    la r3, lbl_8047CC40(r2)
    li r4, 0x25d
    la r5, lbl_8047CC48(r2)
    bl fn_80196E10
    @L_800EBB5C:
    lwz r4, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EBB78
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EBB78
    li r3, 0x1
    @L_800EBB78:
    cmpwi r3, 0x0
    bne @L_800EBB88
    mr r3, r31
    bl fn_8019D620
    @L_800EBB88:
    cmplwi r29, 0x0
    bne @L_800EBBA0
    la r3, lbl_8047CC40(r2)
    li r4, 0x2dd
    la r5, lbl_8047CC48(r2)
    bl fn_80196E10
    @L_800EBBA0:
    lwz r3, 0x1c(r29)
    cmplwi r30, 0x0
    lwz r0, 0x20(r29)
    stw r3, 0x18(r1)
    stw r0, 0x1c(r1)
    lwz r3, 0x24(r29)
    lwz r0, 0x28(r29)
    stw r3, 0x20(r1)
    stw r0, 0x24(r1)
    bne @L_800EBBD8
    la r3, lbl_8047CC40(r2)
    li r4, 0x2dd
    la r5, lbl_8047CC48(r2)
    bl fn_80196E10
    @L_800EBBD8:
    lwz r3, 0x1c(r30)
    addi r6, r28, 0xe4
    lwz r0, 0x20(r30)
    li r5, 0x4
    lfs f5, 0x18(r1)
    stw r3, 0x8(r1)
    lfs f4, 0x1c(r1)
    stw r0, 0xc(r1)
    lfs f3, 0x20(r1)
    lwz r3, 0x24(r30)
    lwz r0, 0x28(r30)
    lfs f2, 0x8(r1)
    stw r3, 0x10(r1)
    lfs f1, 0xc(r1)
    lfs f0, 0x10(r1)
    stw r0, 0x14(r1)
    lwz r4, lbl_8047ABA0(r13)
    stfs f5, 0x44(r1)
    stfs f4, 0x48(r1)
    stfs f3, 0x4c(r1)
    stfs f2, 0x38(r1)
    stfs f1, 0x3c(r1)
    stfs f0, 0x40(r1)
    b @L_800EBD4C
    @L_800EBC38:
    lwz r3, 0x0(r6)
    cmpwi r3, 0x0
    beq @L_800EBD48
    lwz r0, 0x4(r6)
    cmplw r0, r4
    bne @L_800EBD48
    cmpwi r3, 0x2
    beq @L_800EBCA0
    bge @L_800EBC68
    cmpwi r3, 0x1
    bge @L_800EBC74
    b @L_800EBD44
    @L_800EBC68:
    cmpwi r3, 0x4
    bge @L_800EBD44
    b @L_800EBCFC
    @L_800EBC74:
    lwz r3, 0x8(r6)
    lfs f2, 0x0(r3)
    stfs f2, 0x44(r1)
    lfs f1, 0x4(r3)
    stfs f1, 0x48(r1)
    lfs f0, 0x8(r3)
    stfs f0, 0x4c(r1)
    stfs f2, 0x38(r1)
    stfs f1, 0x3c(r1)
    stfs f0, 0x40(r1)
    b @L_800EBD44
    @L_800EBCA0:
    lwz r3, 0x8(r6)
    lfs f2, 0x44(r1)
    lfs f1, 0x0(r3)
    lfs f0, 0x38(r1)
    fadds f5, f2, f1
    lfs f4, 0x48(r1)
    lfs f3, 0x4c(r1)
    fadds f2, f0, f1
    lfs f1, 0x3c(r1)
    stfs f5, 0x44(r1)
    lfs f0, 0x40(r1)
    lfs f5, 0x4(r3)
    fadds f4, f4, f5
    fadds f1, f1, f5
    stfs f4, 0x48(r1)
    lfs f4, 0x8(r3)
    fadds f3, f3, f4
    stfs f2, 0x38(r1)
    fadds f0, f0, f4
    stfs f1, 0x3c(r1)
    stfs f3, 0x4c(r1)
    stfs f0, 0x40(r1)
    b @L_800EBD44
    @L_800EBCFC:
    lwz r3, 0x8(r6)
    lfs f0, 0x44(r1)
    lfs f4, 0x0(r3)
    lfs f1, 0x38(r1)
    fadds f3, f0, f4
    lfs f2, 0x4c(r1)
    lfs f0, 0x40(r1)
    fadds f1, f1, f4
    stfs f3, 0x44(r1)
    lfs f4, 0x4(r3)
    stfs f4, 0x48(r1)
    lfs f3, 0x8(r3)
    fadds f2, f2, f3
    stfs f1, 0x38(r1)
    fadds f0, f0, f3
    stfs f4, 0x3c(r1)
    stfs f2, 0x4c(r1)
    stfs f0, 0x40(r1)
    @L_800EBD44:
    li r5, 0x0
    @L_800EBD48:
    addi r6, r6, 0xc
    @L_800EBD4C:
    cmpwi r5, 0x0
    subi r5, r5, 0x1
    bne @L_800EBC38
    addi r3, r1, 0x44
    addi r4, r1, 0x18
    bl fn_801ADAAC
    addi r3, r1, 0x38
    addi r4, r1, 0x8
    bl fn_801ADAAC
    lfs f7, 0xc(r1)
    lfs f0, 0x1c(r1)
    lfs f8, 0x8(r1)
    lfs f1, 0x18(r1)
    fsubs f2, f0, f7
    fadds f0, f0, f7
    lfs f9, 0x10(r1)
    lfs f4, 0x20(r1)
    fsubs f3, f1, f8
    fadds f1, f1, f8
    fsubs f10, f4, f9
    fadds f5, f4, f9
    lfs f11, 0x14(r1)
    lfs f4, 0x24(r1)
    fmuls f3, f3, f3
    fmuls f2, f2, f2
    fsubs f12, f4, f11
    fadds f6, f4, f11
    fmuls f1, f1, f1
    fmuls f0, f0, f0
    fmuls f4, f10, f10
    fadds f3, f3, f2
    fmuls f2, f5, f5
    fadds f0, f1, f0
    fmuls f5, f12, f12
    fadds f3, f4, f3
    fmuls f1, f6, f6
    fadds f0, f2, f0
    fadds f2, f5, f3
    fadds f0, f1, f0
    fcmpo cr0, f2, f0
    ble @L_800EBE10
    fneg f3, f8
    fneg f2, f7
    fneg f1, f9
    fneg f0, f11
    stfs f3, 0x8(r1)
    stfs f2, 0xc(r1)
    stfs f1, 0x10(r1)
    stfs f0, 0x14(r1)
    @L_800EBE10:
    fmr f1, f30
    addi r3, r1, 0x18
    addi r4, r1, 0x8
    addi r5, r1, 0x28
    bl fn_801AD7CC
    cmplwi r31, 0x0
    bne @L_800EBE3C
    la r3, lbl_8047CC40(r2)
    li r4, 0x290
    la r5, lbl_8047CC48(r2)
    bl fn_80196E10
    @L_800EBE3C:
    lwz r3, 0x28(r1)
    lwz r0, 0x2c(r1)
    stw r3, 0x1c(r31)
    stw r0, 0x20(r31)
    lwz r3, 0x30(r1)
    lwz r0, 0x34(r1)
    stw r3, 0x24(r31)
    stw r0, 0x28(r31)
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800EBEB0
    cmplwi r31, 0x0
    beq @L_800EBEB0
    bne @L_800EBE84
    la r3, lbl_8047CC40(r2)
    li r4, 0x25d
    la r5, lbl_8047CC48(r2)
    bl fn_80196E10
    @L_800EBE84:
    lwz r4, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EBEA0
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EBEA0
    li r3, 0x1
    @L_800EBEA0:
    cmpwi r3, 0x0
    bne @L_800EBEB0
    mr r3, r31
    bl fn_8019D620
    @L_800EBEB0:
    lwz r3, lbl_8047ABA0(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047ABA0(r13)
    psq_l f31, 0x88(r1), 0, 0
    lfd f31, 0x80(r1)
    psq_l f30, 0x78(r1), 0, 0
    lfd f30, 0x70(r1)
    lwz r31, 0x6c(r1)
    lwz r30, 0x68(r1)
    lwz r29, 0x64(r1)
    lwz r0, 0x94(r1)
    lwz r28, 0x60(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
