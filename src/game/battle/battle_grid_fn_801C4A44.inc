    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stfd f31, 0x70(r1)
    psq_st f31, 0x78(r1), 0, 0
    stfd f30, 0x60(r1)
    psq_st f30, 0x68(r1), 0, 0
    stw r31, 0x5c(r1)
    stw r30, 0x58(r1)
    stw r29, 0x54(r1)
    stw r28, 0x50(r1)
    mr r28, r3
    fdivs f31, f3, f4
    lfs f0, lbl_8047DFDC(r2)
    lwz r0, lbl_8047DFD8(r2)
    stw r0, 0x8(r1)
    fdivs f30, f1, f2
    fcmpo cr0, f31, f0
    cror eq, lt, eq
    bne @L_801C4AD0
    lfs f0, lbl_8047DFE8(r2)
    lfs f2, lbl_8047DFE4(r2)
    fmuls f1, f0, f31
    lfs f0, lbl_8047DFE0(r2)
    stfs f2, 0xc(r1)
    fcmpo cr0, f1, f2
    stfs f0, 0x18(r1)
    stfs f0, 0x1c(r1)
    stfs f0, 0x20(r1)
    stfs f2, 0x10(r1)
    stfs f2, 0x14(r1)
    cror eq, gt, eq
    bne @L_801C4B0C
    fmr f1, f2
    b @L_801C4B0C
    @L_801C4AD0:
    fsubs f0, f31, f0
    lfs f3, lbl_8047DFE4(r2)
    lfs f1, lbl_8047DFEC(r2)
    lfs f2, lbl_8047DFE0(r2)
    fmuls f1, f1, f0
    stfs f3, 0x18(r1)
    stfs f3, 0x1c(r1)
    fcmpo cr0, f1, f3
    stfs f3, 0x20(r1)
    stfs f2, 0xc(r1)
    stfs f2, 0x10(r1)
    stfs f2, 0x14(r1)
    cror eq, gt, eq
    bne @L_801C4B0C
    fmr f1, f3
    @L_801C4B0C:
    addi r3, r1, 0x24
    addi r4, r1, 0x18
    addi r5, r1, 0xc
    bl fn_800E090C
    lfs f2, lbl_8047DFF0(r2)
    lis r0, 0x4330
    lfs f1, 0x24(r1)
    li r3, 0x1
    lfs f0, lbl_8047DFE4(r2)
    fmuls f1, f2, f1
    stw r0, 0x38(r1)
    lfd f2, lbl_8047E000(r2)
    fsubs f0, f0, f30
    fctiwz f1, f1
    stfd f1, 0x30(r1)
    lwz r0, 0x34(r1)
    clrlwi r0, r0, 24
    stw r0, 0x3c(r1)
    lfd f1, 0x38(r1)
    fsubs f1, f1, f2
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x40(r1)
    lwz r29, 0x44(r1)
    bl fn_800D9ED8
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    lfs f1, lbl_8047DFE0(r2)
    lfs f3, lbl_8047DFF4(r2)
    fmr f2, f1
    lfs f4, lbl_8047DFF8(r2)
    bl fn_800D9B58
    li r3, 0x1
    li r4, 0x6
    li r5, 0x1
    bl fn_800DA4C4
    li r3, 0x1
    li r4, 0x1
    li r5, 0x0
    bl fn_800DA2BC
    li r3, 0x0
    li r4, 0x1
    li r5, 0x1
    bl fn_800DA1E8
    li r3, 0x0
    bl fn_800DA028
    li r3, 0x7
    bl fn_800D6A00
    li r3, 0x0
    bl fn_800D7820
    li r3, 0x2
    bl fn_800D67BC
    lfs f1, lbl_8047DFE0(r2)
    fmr f2, f1
    fmr f3, f1
    bl fn_800D6680
    lbz r30, 0x9(r1)
    mr r7, r29
    lbz r31, 0xa(r1)
    li r3, 0x0
    lbz r4, 0x8(r1)
    mr r5, r30
    mr r6, r31
    bl fn_800D5CB8
    lfs f1, lbl_8047DFF4(r2)
    lfs f2, lbl_8047DFF8(r2)
    lfs f3, lbl_8047DFE0(r2)
    bl fn_800D6680
    lbz r4, 0x8(r1)
    mr r5, r30
    mr r6, r31
    mr r7, r29
    li r3, 0x0
    bl fn_800D5CB8
    bl fn_800D6728
    li r3, 0x0
    bl fn_800D9ED8
    lfs f0, lbl_8047DFE4(r2)
    fcmpo cr0, f31, f0
    cror eq, gt, eq
    bne @L_801C4C64
    bl fn_801C4078
    li r3, 0x0
    b @L_801C4C68
    @L_801C4C64:
    mr r3, r28
    @L_801C4C68:
    psq_l f31, 0x78(r1), 0, 0
    lfd f31, 0x70(r1)
    psq_l f30, 0x68(r1), 0, 0
    lfd f30, 0x60(r1)
    lwz r31, 0x5c(r1)
    lwz r30, 0x58(r1)
    lwz r29, 0x54(r1)
    lwz r0, 0x84(r1)
    lwz r28, 0x50(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
