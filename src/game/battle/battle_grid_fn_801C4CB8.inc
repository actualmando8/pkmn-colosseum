    nofralloc
    stwu r1, -0x120(r1)
    mflr r0
    stw r0, 0x124(r1)
    stfd f31, 0x110(r1)
    psq_st f31, 0x118(r1), 0, 0
    stfd f30, 0x100(r1)
    psq_st f30, 0x108(r1), 0, 0
    stfd f29, 0xf0(r1)
    psq_st f29, 0xf8(r1), 0, 0
    stfd f28, 0xe0(r1)
    psq_st f28, 0xe8(r1), 0, 0
    stfd f27, 0xd0(r1)
    psq_st f27, 0xd8(r1), 0, 0
    stw r31, 0xcc(r1)
    stw r30, 0xc8(r1)
    stw r29, 0xc4(r1)
    fmr f28, f1
    mr r30, r3
    fmr f27, f2
    mr r31, r4
    cmplwi r31, 0x0
    bne @L_801C4D14
    b @L_801C5378
    @L_801C4D14:
    lbz r0, lbl_8047B3B0(r13)
    cmplwi r0, 0x1
    bne @L_801C4D30
    li r0, 0x0
    mr r3, r31
    stb r0, lbl_8047B3B0(r13)
    bl fn_801C53BC
    @L_801C4D30:
    lbz r0, lbl_8047B3B0(r13)
    cmplwi r0, 0x0
    bne @L_801C4D60
    clrlwi r0, r30, 24
    cmplwi r0, 0x0
    bne @L_801C4D60
    lwz r3, lbl_8047B3B4(r13)
    cmplwi r3, 0x0
    beq @L_801C4D60
    bl fn_800D75F4
    li r0, 0x0
    stw r0, lbl_8047B3B4(r13)
    @L_801C4D60:
    fdivs f31, f28, f27
    lis r3, lbl_80467030@ha
    addi r29, r3, lbl_80467030@l
    lfs f0, lbl_8047DFE0(r2)
    lfs f2, 0x10(r29)
    lfs f1, 0x14(r29)
    fadds f1, f1, f2
    lhz r3, 0x8(r29)
    fcmpo cr0, f2, f0
    addi r0, r3, 0x1
    sth r0, 0x8(r29)
    stfs f1, 0x14(r29)
    cror eq, gt, eq
    bne @L_801C4DBC
    lfs f0, lbl_8047DFDC(r2)
    lfs f3, 0x18(r29)
    fmadds f0, f0, f31, f2
    fcmpo cr0, f1, f3
    stfs f0, 0x10(r29)
    cror eq, gt, eq
    bne @L_801C4DDC
    stfs f3, 0x14(r29)
    b @L_801C4DDC
    @L_801C4DBC:
    lfs f0, lbl_8047DFDC(r2)
    lfs f3, 0x18(r29)
    fnmsubs f0, f0, f31, f2
    fcmpo cr0, f1, f3
    stfs f0, 0x10(r29)
    cror eq, lt, eq
    bne @L_801C4DDC
    stfs f3, 0x14(r29)
    @L_801C4DDC:
    lfs f2, lbl_8047DFE0(r2)
    fmr f3, f31
    lfs f1, lbl_8047E008(r2)
    mr r3, r31
    lfs f0, lbl_8047E00C(r2)
    fmr f4, f2
    stfs f1, 0x20(r1)
    lfs f1, lbl_8047DFE4(r2)
    addi r4, r1, 0x20
    stfs f0, 0x24(r1)
    stfs f2, 0x28(r1)
    bl fn_801C63C0
    lfs f1, lbl_8047E008(r2)
    fmr f3, f31
    lfs f0, 0x14(r29)
    mr r3, r31
    lfs f2, lbl_8047DFE0(r2)
    addi r4, r1, 0x20
    fadds f4, f1, f0
    lfs f0, lbl_8047E00C(r2)
    stfs f2, 0x28(r1)
    lfs f1, lbl_8047DFE4(r2)
    stfs f4, 0x20(r1)
    lfs f4, lbl_8047DFDC(r2)
    stfs f0, 0x24(r1)
    bl fn_801C63C0
    lfs f1, lbl_8047E008(r2)
    fmr f3, f31
    lfs f0, 0x14(r29)
    mr r3, r31
    lfs f2, lbl_8047DFE0(r2)
    addi r4, r1, 0x20
    fsubs f4, f1, f0
    lfs f0, lbl_8047E00C(r2)
    stfs f2, 0x28(r1)
    lfs f1, lbl_8047DFE4(r2)
    stfs f4, 0x20(r1)
    lfs f4, lbl_8047DFDC(r2)
    stfs f0, 0x24(r1)
    bl fn_801C63C0
    lwz r0, lbl_8047B3B4(r13)
    cmplwi r0, 0x0
    beq @L_801C5354
    li r3, 0x1
    bl fn_800D9ED8
    lis r3, 0x8000
    addi r3, r3, 0x3
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    lfs f1, lbl_8047DFE0(r2)
    lfs f3, lbl_8047DFF4(r2)
    fmr f2, f1
    lfs f4, lbl_8047DFF8(r2)
    bl fn_800D9B58
    li r3, 0x1
    li r4, 0x6
    li r5, 0x7
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
    li r4, 0x7
    li r5, 0x0
    li r6, 0x1
    li r7, 0x7
    li r8, 0x0
    bl fn_800DA100
    li r3, 0x0
    bl fn_800DA028
    addi r6, r1, 0x5c
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4
    bl fn_800D848C
    addi r6, r1, 0x5c
    li r3, 0x1
    li r4, 0x0
    li r5, 0x5
    bl fn_800D848C
    li r3, 0x2
    bl fn_800DC1D4
    lis r4, lbl_80467030@ha
    li r3, 0x0
    addi r4, r4, lbl_80467030@l
    lwz r4, 0x4(r4)
    bl fn_800D85D4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_800DC224
    li r3, 0x0
    li r4, 0x8
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x1
    bl fn_800DC14C
    li r3, 0x0
    li r4, 0x9
    li r5, 0xd
    li r6, 0xc
    li r7, 0xf
    bl fn_800DC0D4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x1
    bl fn_800DC04C
    li r3, 0x0
    li r4, 0x7
    li r5, 0x4
    li r6, 0x5
    li r7, 0x7
    bl fn_800DBFD4
    mr r4, r31
    li r3, 0x1
    bl fn_800D85D4
    li r3, 0x1
    li r4, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0x0
    bl fn_800DC224
    li r3, 0x1
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_800DC14C
    li r3, 0x1
    li r4, 0xf
    li r5, 0x8
    li r6, 0x2
    li r7, 0xf
    bl fn_800DC0D4
    li r3, 0x1
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_800DC04C
    li r3, 0x1
    li r4, 0x7
    li r5, 0x7
    li r6, 0x7
    li r7, 0x1
    bl fn_800DBFD4
    lwz r3, lbl_8047B3B4(r13)
    bl fn_800D7820
    li r3, 0x4
    bl fn_800D6A00
    lhz r4, 0x8(r29)
    lis r0, 0x4330
    lfs f4, lbl_8047DFE4(r2)
    addi r3, r1, 0x2c
    stw r4, 0x94(r1)
    addi r4, r1, 0x14
    lfs f7, lbl_8047E008(r2)
    fsubs f0, f4, f31
    stw r0, 0x90(r1)
    lfd f3, lbl_8047E000(r2)
    lfd f1, 0x90(r1)
    lfs f2, lbl_8047E010(r2)
    fsubs f3, f1, f3
    lfs f1, lbl_8047DFF0(r2)
    lfs f6, lbl_8047E00C(r2)
    lfs f5, lbl_8047DFE0(r2)
    fmuls f31, f1, f0
    fdivs f0, f3, f2
    stfs f7, 0x20(r1)
    stfs f6, 0x24(r1)
    stfs f5, 0x28(r1)
    fadds f0, f4, f0
    stfs f0, 0x14(r1)
    stfs f0, 0x18(r1)
    stfs f0, 0x1c(r1)
    bl fn_800E042C
    lfs f1, lbl_8047DFE0(r2)
    addi r3, r1, 0x2c
    bl fn_800E02E8
    addi r3, r1, 0x2c
    addi r4, r1, 0x20
    bl fn_800E03B4
    lfs f1, lbl_8047E014(r2)
    addi r3, r1, 0x8
    lfs f0, lbl_8047DFE0(r2)
    addi r4, r1, 0x2c
    stfs f1, 0x14(r1)
    addi r5, r1, 0x14
    stfs f1, 0x18(r1)
    stfs f0, 0x1c(r1)
    bl fn_800DFF98
    lfs f0, 0x8(r1)
    li r3, 0x4
    lfs f3, lbl_8047E008(r2)
    lfs f1, 0xc(r1)
    fsubs f2, f0, f3
    lfs f0, lbl_8047E00C(r2)
    lfs f4, lbl_8047DFF4(r2)
    fsubs f6, f1, f0
    lfs f1, lbl_8047DFF8(r2)
    fsubs f5, f3, f2
    fadds f3, f3, f2
    stfs f2, 0x8(r1)
    fsubs f2, f0, f6
    fadds f0, f0, f6
    stfs f6, 0xc(r1)
    fdivs f30, f5, f4
    fdivs f29, f3, f4
    fdivs f27, f2, f1
    fdivs f28, f0, f1
    bl fn_800D67BC
    lfs f1, lbl_8047E018(r2)
    addi r3, r1, 0x8
    lfs f0, lbl_8047DFE0(r2)
    addi r4, r1, 0x2c
    stfs f1, 0x14(r1)
    addi r5, r1, 0x14
    stfs f1, 0x18(r1)
    stfs f0, 0x1c(r1)
    bl fn_800DFF98
    lfs f1, 0x8(r1)
    lfs f2, 0xc(r1)
    lfs f3, 0x10(r1)
    bl fn_800D6680
    fctiwz f0, f31
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    stfd f0, 0x98(r1)
    lwz r7, 0x9c(r1)
    bl fn_800D5CB8
    lfs f1, lbl_8047DFE0(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    fmr f1, f30
    li r3, 0x1
    fmr f2, f27
    bl fn_800D59B8
    lfs f2, lbl_8047E014(r2)
    addi r3, r1, 0x8
    lfs f1, lbl_8047E018(r2)
    addi r4, r1, 0x2c
    lfs f0, lbl_8047DFE0(r2)
    addi r5, r1, 0x14
    stfs f2, 0x14(r1)
    stfs f1, 0x18(r1)
    stfs f0, 0x1c(r1)
    bl fn_800DFF98
    lfs f1, 0x8(r1)
    lfs f2, 0xc(r1)
    lfs f3, 0x10(r1)
    bl fn_800D6680
    fctiwz f0, f31
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    stfd f0, 0xa0(r1)
    lwz r7, 0xa4(r1)
    bl fn_800D5CB8
    lfs f1, lbl_8047DFE4(r2)
    li r3, 0x0
    lfs f2, lbl_8047DFE0(r2)
    bl fn_800D59B8
    fmr f1, f29
    li r3, 0x1
    fmr f2, f27
    bl fn_800D59B8
    lfs f2, lbl_8047E018(r2)
    addi r3, r1, 0x8
    lfs f1, lbl_8047E014(r2)
    addi r4, r1, 0x2c
    lfs f0, lbl_8047DFE0(r2)
    addi r5, r1, 0x14
    stfs f2, 0x14(r1)
    stfs f1, 0x18(r1)
    stfs f0, 0x1c(r1)
    bl fn_800DFF98
    lfs f1, 0x8(r1)
    lfs f2, 0xc(r1)
    lfs f3, 0x10(r1)
    bl fn_800D6680
    fctiwz f0, f31
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    stfd f0, 0xa8(r1)
    lwz r7, 0xac(r1)
    bl fn_800D5CB8
    lfs f1, lbl_8047DFE0(r2)
    li r3, 0x0
    lfs f2, lbl_8047DFE4(r2)
    bl fn_800D59B8
    fmr f1, f30
    li r3, 0x1
    fmr f2, f28
    bl fn_800D59B8
    lfs f1, lbl_8047E014(r2)
    addi r3, r1, 0x8
    lfs f0, lbl_8047DFE0(r2)
    addi r4, r1, 0x2c
    stfs f1, 0x14(r1)
    addi r5, r1, 0x14
    stfs f1, 0x18(r1)
    stfs f0, 0x1c(r1)
    bl fn_800DFF98
    lfs f1, 0x8(r1)
    lfs f2, 0xc(r1)
    lfs f3, 0x10(r1)
    bl fn_800D6680
    fctiwz f0, f31
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    stfd f0, 0xb0(r1)
    lwz r7, 0xb4(r1)
    bl fn_800D5CB8
    lfs f1, lbl_8047DFE4(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    fmr f1, f29
    li r3, 0x1
    fmr f2, f28
    bl fn_800D59B8
    bl fn_800D6728
    li r3, 0x1
    bl fn_800DC1D4
    lis r3, 0x8000
    bl fn_800D888C
    li r3, 0x0
    bl fn_800D9ED8
    @L_801C5354:
    lbz r0, lbl_8047B3B0(r13)
    cmplwi r0, 0x0
    bne @L_801C5374
    clrlwi r0, r30, 24
    cmplwi r0, 0x0
    bne @L_801C5374
    mr r3, r31
    bl fn_800DC298
    @L_801C5374:
    mr r3, r30
    @L_801C5378:
    psq_l f31, 0x118(r1), 0, 0
    lfd f31, 0x110(r1)
    psq_l f30, 0x108(r1), 0, 0
    lfd f30, 0x100(r1)
    psq_l f29, 0xf8(r1), 0, 0
    lfd f29, 0xf0(r1)
    psq_l f28, 0xe8(r1), 0, 0
    lfd f28, 0xe0(r1)
    psq_l f27, 0xd8(r1), 0, 0
    lfd f27, 0xd0(r1)
    lwz r31, 0xcc(r1)
    lwz r30, 0xc8(r1)
    lwz r0, 0x124(r1)
    lwz r29, 0xc4(r1)
    mtlr r0
    addi r1, r1, 0x120
    blr
