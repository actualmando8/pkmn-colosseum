    nofralloc
    stwu r1, -0xe0(r1)
    mflr r0
    stw r0, 0xe4(r1)
    stfd f31, 0xd0(r1)
    psq_st f31, 0xd8(r1), 0, 0
    stw r31, 0xcc(r1)
    stw r30, 0xc8(r1)
    stw r29, 0xc4(r1)
    stw r28, 0xc0(r1)
    mr. r28, r3
    bne @L_8019BD74
    lwz r0, lbl_8047E720(r2)
    addi r4, r1, 0xc
    lfs f1, lbl_8047DA68(r2)
    li r3, 0x0
    stw r0, 0x10(r1)
    lfs f2, lbl_8047DA68(r2)
    lwz r0, 0x10(r1)
    lfs f3, lbl_8047DA68(r2)
    stw r0, 0xc(r1)
    lfs f4, lbl_8047DA68(r2)
    bl fn_800BC8F8
    b @L_8019BFC0
    @L_8019BD74:
    bl fn_801942B8
    mr. r30, r3
    bne @L_8019BD94
    lis r4, lbl_802747DC@ha
    la r3, lbl_8047DA74(r2)
    addi r5, r4, lbl_802747DC@l
    li r4, 0x58
    bl fn_80196D78
    @L_8019BD94:
    lwz r0, 0x18(r28)
    mr r3, r30
    addi r31, r1, 0x8
    stw r0, 0x8(r1)
    bl fn_801944D0
    fmr f31, f1
    mr r3, r30
    bl fn_801944F8
    fmr f3, f1
    lwz r3, 0x8(r28)
    fmr f4, f31
    lfs f1, 0x10(r28)
    lfs f2, 0x14(r28)
    mr r4, r31
    bl fn_800BC8F8
    cmplwi r28, 0x0
    beq @L_8019BDE0
    lwz r30, 0xc(r28)
    b @L_8019BDE4
    @L_8019BDE0:
    li r30, 0x0
    @L_8019BDE4:
    cmplwi r30, 0x0
    beq @L_8019BDF4
    lwz r31, 0x8(r30)
    b @L_8019BDF8
    @L_8019BDF4:
    li r31, 0x0
    @L_8019BDF8:
    cmplwi r30, 0x0
    beq @L_8019BFB0
    clrlwi. r0, r31, 29
    beq @L_8019BFB0
    addi r3, r1, 0x30
    bl fn_800BD768
    clrlwi. r0, r31, 31
    beq @L_8019BE74
    cmplwi r30, 0x0
    lfs f4, 0x30(r1)
    lfs f3, 0x38(r1)
    beq @L_8019BE30
    lha r3, 0xc(r30)
    b @L_8019BE34
    @L_8019BE30:
    li r3, -0x1
    @L_8019BE34:
    addi r3, r3, 0x140
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    stw r0, 0xa0(r1)
    lfd f2, lbl_8047DA90(r2)
    stw r3, 0xa4(r1)
    lfs f0, lbl_8047DA84(r2)
    lfd f1, 0xa0(r1)
    fsubs f1, f1, f2
    fmuls f1, f3, f1
    fdivs f0, f1, f0
    fadds f0, f4, f0
    fctiwz f0, f0
    stfd f0, 0xa8(r1)
    lwz r29, 0xac(r1)
    b @L_8019BE90
    @L_8019BE74:
    lfs f2, 0x38(r1)
    lfs f1, lbl_8047DA88(r2)
    lfs f0, 0x30(r1)
    fmadds f0, f2, f1, f0
    fctiwz f0, f0
    stfd f0, 0xb0(r1)
    lwz r29, 0xb4(r1)
    @L_8019BE90:
    rlwinm. r0, r31, 0, 30, 30
    beq @L_8019BEB0
    cmplwi r30, 0x0
    beq @L_8019BEA8
    lhz r28, 0xe(r30)
    b @L_8019BEC0
    @L_8019BEA8:
    li r28, -0x1
    b @L_8019BEC0
    @L_8019BEB0:
    lfs f0, 0x38(r1)
    fctiwz f0, f0
    stfd f0, 0xb8(r1)
    lwz r28, 0xbc(r1)
    @L_8019BEC0:
    rlwinm. r0, r31, 0, 29, 29
    beq @L_8019BEE0
    cmplwi r30, 0x0
    beq @L_8019BED8
    addi r30, r30, 0x10
    b @L_8019BF8C
    @L_8019BED8:
    li r30, 0x0
    b @L_8019BF8C
    @L_8019BEE0:
    addi r30, r1, 0x5c
    addi r3, r1, 0x5c
    li r4, 0x0
    li r5, 0x40
    bl memset
    addi r3, r1, 0x14
    bl fn_800BD454
    lfs f0, 0x14(r1)
    fctiwz f0, f0
    stfd f0, 0xb8(r1)
    lwz r0, 0xbc(r1)
    cmpwi r0, 0x0
    beq @L_8019BF18
    b @L_8019BF54
    @L_8019BF18:
    lfs f6, 0x18(r1)
    lfs f5, 0x1c(r1)
    lfs f4, 0x20(r1)
    lfs f3, 0x24(r1)
    lfs f2, 0x28(r1)
    lfs f1, 0x2c(r1)
    lfs f0, lbl_8047DA8C(r2)
    stfs f6, 0x5c(r1)
    stfs f5, 0x64(r1)
    stfs f4, 0x70(r1)
    stfs f3, 0x74(r1)
    stfs f2, 0x84(r1)
    stfs f1, 0x88(r1)
    stfs f0, 0x94(r1)
    b @L_8019BF8C
    @L_8019BF54:
    lfs f6, 0x18(r1)
    lfs f5, 0x1c(r1)
    lfs f4, 0x20(r1)
    lfs f3, 0x24(r1)
    lfs f2, 0x28(r1)
    lfs f1, 0x2c(r1)
    lfs f0, lbl_8047DA6C(r2)
    stfs f6, 0x5c(r1)
    stfs f5, 0x68(r1)
    stfs f4, 0x70(r1)
    stfs f3, 0x78(r1)
    stfs f2, 0x84(r1)
    stfs f1, 0x88(r1)
    stfs f0, 0x98(r1)
    @L_8019BF8C:
    mr r5, r30
    addi r3, r1, 0x48
    clrlwi r4, r28, 16
    bl fn_800BCB14
    clrlwi r4, r29, 16
    addi r5, r1, 0x48
    li r3, 0x1
    bl fn_800BCCDC
    b @L_8019BFC0
    @L_8019BFB0:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    bl fn_800BCCDC
    @L_8019BFC0:
    psq_l f31, 0xd8(r1), 0, 0
    lwz r0, 0xe4(r1)
    lfd f31, 0xd0(r1)
    lwz r31, 0xcc(r1)
    lwz r30, 0xc8(r1)
    lwz r29, 0xc4(r1)
    lwz r28, 0xc0(r1)
    mtlr r0
    addi r1, r1, 0xe0
    blr
