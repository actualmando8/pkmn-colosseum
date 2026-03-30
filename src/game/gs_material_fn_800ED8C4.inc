    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stfd f31, 0x60(r1)
    psq_st f31, 0x68(r1), 0, 0
    stfd f30, 0x50(r1)
    psq_st f30, 0x58(r1), 0, 0
    stfd f29, 0x40(r1)
    psq_st f29, 0x48(r1), 0, 0
    stfd f28, 0x30(r1)
    psq_st f28, 0x38(r1), 0, 0
    stfd f27, 0x20(r1)
    psq_st f27, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    lbz r0, lbl_80478AF8(r13)
    fmr f27, f1
    fmr f29, f2
    lfs f31, lbl_8047CC5C(r2)
    fmr f30, f3
    cmplwi r0, 0x1
    fmr f28, f4
    mr r30, r3
    mr r31, r4
    bne @L_800ED9C4
    bl fn_800D37CC
    cmpwi r3, 0x3c
    bne @L_800ED9C4
    clrlwi. r0, r31, 24
    bne @L_800ED9BC
    fctiwz f0, f27
    lis r0, 0x4330
    stw r0, 0x10(r1)
    lfd f2, lbl_8047CC80(r2)
    stfd f0, 0x8(r1)
    lfs f0, lbl_8047CC78(r2)
    lwz r0, 0xc(r1)
    xoris r0, r0, 0x8000
    stw r0, 0x14(r1)
    lfd f1, 0x10(r1)
    fsubs f1, f1, f2
    fsubs f1, f27, f1
    fcmpo cr0, f1, f0
    ble @L_800ED9B4
    bl fn_800D3088
    subi r3, r3, 0x1
    lis r0, 0x4330
    stw r3, 0x14(r1)
    lfd f2, lbl_8047CC88(r2)
    stw r0, 0x10(r1)
    lfs f0, lbl_8047CC60(r2)
    lfd f1, 0x10(r1)
    fsubs f1, f1, f2
    fmuls f1, f28, f1
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_800ED9AC
    lfs f1, lbl_8047CC5C(r2)
    @L_800ED9AC:
    fadds f1, f27, f1
    b @L_800ED9C8
    @L_800ED9B4:
    fmr f1, f27
    b @L_800ED9C8
    @L_800ED9BC:
    fmr f1, f27
    b @L_800ED9C8
    @L_800ED9C4:
    fmr f1, f27
    @L_800ED9C8:
    lfs f0, lbl_8047CC5C(r2)
    fcmpo cr0, f30, f0
    ble @L_800EDA14
    lfs f0, lbl_8047CC78(r2)
    fsubs f0, f30, f0
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_800EDA14
    cmpwi r30, 0x1
    beq @L_800EDA08
    bge @L_800EDA14
    cmpwi r30, 0x0
    bge @L_800EDA00
    b @L_800EDA14
    @L_800EDA00:
    fmr f1, f30
    b @L_800EDA14
    @L_800EDA08:
    fmr f2, f30
    bl fn_800CE318
    frsp f1, f1
    @L_800EDA14:
    fcmpu cr0, f1, f29
    beq @L_800EDA44
    fcmpo cr0, f1, f29
    bge @L_800EDA40
    cmpwi r30, 0x1
    bne @L_800EDA38
    fsubs f0, f30, f29
    fadds f31, f1, f0
    b @L_800EDA44
    @L_800EDA38:
    lfs f31, lbl_8047CC5C(r2)
    b @L_800EDA44
    @L_800EDA40:
    fsubs f31, f1, f29
    @L_800EDA44:
    lfs f0, lbl_8047CC5C(r2)
    fcmpo cr0, f31, f0
    bge @L_800EDA54
    fmr f31, f0
    @L_800EDA54:
    fmr f1, f31
    psq_l f31, 0x68(r1), 0, 0
    lfd f31, 0x60(r1)
    psq_l f30, 0x58(r1), 0, 0
    lfd f30, 0x50(r1)
    psq_l f29, 0x48(r1), 0, 0
    lfd f29, 0x40(r1)
    psq_l f28, 0x38(r1), 0, 0
    lfd f28, 0x30(r1)
    psq_l f27, 0x28(r1), 0, 0
    lfd f27, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r0, 0x74(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
