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
    stw r31, 0x7c(r1)
    stw r30, 0x78(r1)
    fmr f28, f1
    mr r30, r4
    fmr f29, f2
    fmr f30, f3
    fmr f31, f4
    bl fn_801C680C
    lfs f2, -22204(r2)
    fmr f1, f31
    lfs f0, -22208(r2)
    addi r3, r1, 0x20
    stfs f2, 0x14(r1)
    addi r4, r1, 0x14
    addi r5, r1, 0x8
    stfs f2, 0x18(r1)
    stfs f2, 0x1c(r1)
    stfs f0, 0x8(r1)
    stfs f0, 0xc(r1)
    stfs f0, 0x10(r1)
    bl fn_800E090C
    lfs f2, -22192(r2)
    lis r0, 0x4330
    lfs f1, 0x20(r1)
    addi r3, r1, 0x2c
    lfs f0, -22204(r2)
    addi r4, r1, 0x14
    fmuls f1, f2, f1
    stw r0, 0x68(r1)
    lfd f2, -22080(r2)
    fsubs f0, f0, f30
    stfs f28, 0x14(r1)
    fctiwz f1, f1
    stfs f28, 0x18(r1)
    stfd f1, 0x60(r1)
    lwz r0, 0x64(r1)
    stfs f28, 0x1c(r1)
    xoris r0, r0, 0x8000
    stw r0, 0x6c(r1)
    lfd f1, 0x68(r1)
    fsubs f1, f1, f2
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x70(r1)
    lwz r31, 0x74(r1)
    bl fn_800E042C
    lfs f0, -22072(r2)
    addi r3, r1, 0x2c
    fmuls f1, f0, f29
    bl fn_800E02E8
    mr r4, r30
    addi r3, r1, 0x2c
    bl fn_800E03B4
    li r3, 0x4
    bl fn_800D67BC
    lfs f2, -22028(r2)
    addi r3, r1, 0x8
    lfs f1, -22024(r2)
    addi r4, r1, 0x2c
    lfs f0, -22208(r2)
    addi r5, r1, 0x14
    stfs f2, 0x14(r1)
    stfs f1, 0x18(r1)
    stfs f0, 0x1c(r1)
    bl fn_800DFF98
    lfs f1, 0x8(r1)
    lfs f2, 0xc(r1)
    lfs f3, 0x10(r1)
    bl fn_800D6680
    rlwinm r7, r31, 0, 24, 31
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, -22208(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    lfs f2, -22020(r2)
    addi r3, r1, 0x8
    lfs f1, -22024(r2)
    addi r4, r1, 0x2c
    lfs f0, -22208(r2)
    addi r5, r1, 0x14
    stfs f2, 0x14(r1)
    stfs f1, 0x18(r1)
    stfs f0, 0x1c(r1)
    bl fn_800DFF98
    lfs f1, 0x8(r1)
    lfs f2, 0xc(r1)
    lfs f3, 0x10(r1)
    bl fn_800D6680
    rlwinm r7, r31, 0, 24, 31
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, -22204(r2)
    li r3, 0x0
    lfs f2, -22208(r2)
    bl fn_800D59B8
    lfs f2, -22028(r2)
    addi r3, r1, 0x8
    lfs f1, -22016(r2)
    addi r4, r1, 0x2c
    lfs f0, -22208(r2)
    addi r5, r1, 0x14
    stfs f2, 0x14(r1)
    stfs f1, 0x18(r1)
    stfs f0, 0x1c(r1)
    bl fn_800DFF98
    lfs f1, 0x8(r1)
    lfs f2, 0xc(r1)
    lfs f3, 0x10(r1)
    bl fn_800D6680
    rlwinm r7, r31, 0, 24, 31
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, -22208(r2)
    li r3, 0x0
    lfs f2, -22204(r2)
    bl fn_800D59B8
    lfs f2, -22020(r2)
    addi r3, r1, 0x8
    lfs f1, -22016(r2)
    addi r4, r1, 0x2c
    lfs f0, -22208(r2)
    addi r5, r1, 0x14
    stfs f2, 0x14(r1)
    stfs f1, 0x18(r1)
    stfs f0, 0x1c(r1)
    bl fn_800DFF98
    lfs f1, 0x8(r1)
    lfs f2, 0xc(r1)
    lfs f3, 0x10(r1)
    bl fn_800D6680
    rlwinm r7, r31, 0, 24, 31
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, -22204(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    bl fn_800D6728
    bl fn_801C673C
    psq_l f31, 0xb8(r1), 0, 0
    lfd f31, 0xb0(r1)
    psq_l f30, 0xa8(r1), 0, 0
    lfd f30, 0xa0(r1)
    psq_l f29, 0x98(r1), 0, 0
    lfd f29, 0x90(r1)
    psq_l f28, 0x88(r1), 0, 0
    lfd f28, 0x80(r1)
    lwz r31, 0x7c(r1)
    lwz r0, 0xc4(r1)
    lwz r30, 0x78(r1)
    mtlr r0
    addi r1, r1, 0xc0
    blr
