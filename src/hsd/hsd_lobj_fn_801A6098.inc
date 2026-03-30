    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    lhz r0, 0x8(r3)
    fmr f31, f1
    mr r30, r3
    mr r31, r4
    rlwinm. r0, r0, 0, 26, 26
    bne @L_801A61EC
    lhz r0, 0x8(r30)
    clrlwi. r0, r0, 30
    bne @L_801A60DC
    b @L_801A61EC
    @L_801A60DC:
    lhz r0, 0x8(r30)
    andi. r0, r0, 0x14
    beq @L_801A6174
    lbz r3, 0x14(r30)
    lbz r0, 0x0(r31)
    cmplw r3, r0
    bne @L_801A6128
    lbz r3, 0x15(r30)
    lbz r0, 0x1(r31)
    cmplw r3, r0
    bne @L_801A6128
    lbz r3, 0x16(r30)
    lbz r0, 0x2(r31)
    cmplw r3, r0
    bne @L_801A6128
    lbz r3, 0x17(r30)
    lbz r0, 0x3(r31)
    cmplw r3, r0
    beq @L_801A6150
    @L_801A6128:
    lwz r0, 0x0(r31)
    addi r4, r1, 0x8
    addi r3, r30, 0x50
    stw r0, 0x8(r1)
    bl fn_800BA440
    lwz r0, 0x0(r31)
    stw r0, 0x14(r30)
    lhz r0, 0x8(r30)
    ori r0, r0, 0x80
    sth r0, 0x8(r30)
    @L_801A6150:
    lhz r0, 0x8(r30)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_801A6174
    lwz r4, 0x4c(r30)
    addi r3, r30, 0x50
    bl fn_800BA44C
    lhz r0, 0x8(r30)
    rlwinm r0, r0, 0, 25, 23
    sth r0, 0x8(r30)
    @L_801A6174:
    lwz r0, 0x90(r30)
    cmpwi r0, 0x0
    beq @L_801A61EC
    lfs f0, 0x38(r30)
    fcmpu cr0, f0, f31
    beq @L_801A61C8
    lfs f2, lbl_8047DBFC(r2)
    addi r3, r30, 0x94
    lfs f1, lbl_8047DBFC(r2)
    lfs f0, lbl_8047DBE4(r2)
    fmuls f4, f31, f2
    stfs f31, 0x38(r30)
    fnmsubs f6, f31, f1, f0
    lfs f1, lbl_8047DBE8(r2)
    lfs f2, lbl_8047DBE8(r2)
    lfs f3, lbl_8047DBE4(r2)
    lfs f5, lbl_8047DBE8(r2)
    bl fn_800BA198
    lhz r0, 0x8(r30)
    ori r0, r0, 0x100
    sth r0, 0x8(r30)
    @L_801A61C8:
    lhz r0, 0x8(r30)
    rlwinm. r0, r0, 0, 23, 23
    beq @L_801A61EC
    lwz r4, 0x90(r30)
    addi r3, r30, 0x94
    bl fn_800BA44C
    lhz r0, 0x8(r30)
    rlwinm r0, r0, 0, 24, 22
    sth r0, 0x8(r30)
    @L_801A61EC:
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
