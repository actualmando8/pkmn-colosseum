    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stfd f31, 0x60(r1)
    psq_st f31, 0x68(r1), 0, 0
    stw r31, 0x5c(r1)
    stw r30, 0x58(r1)
    fmr f31, f2
    mr r30, r3
    mr r31, r4
    stfs f1, 0x14(r1)
    addi r3, r1, 0x20
    addi r4, r1, 0x14
    stfs f1, 0x18(r1)
    stfs f1, 0x1c(r1)
    bl fn_800E042C
    lfs f0, -22072(r2)
    addi r3, r1, 0x20
    fmuls f1, f0, f31
    bl fn_800E02E8
    mr r4, r30
    addi r3, r1, 0x20
    bl fn_800E03B4
    li r3, 0x4
    bl fn_800D67BC
    lfs f1, -22152(r2)
    addi r3, r1, 0x8
    lfs f0, -22208(r2)
    addi r4, r1, 0x20
    stfs f1, 0x14(r1)
    addi r5, r1, 0x14
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
    lfs f2, -22156(r2)
    addi r3, r1, 0x8
    lfs f1, -22152(r2)
    addi r4, r1, 0x20
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
    lfs f2, -22152(r2)
    addi r3, r1, 0x8
    lfs f1, -22156(r2)
    addi r4, r1, 0x20
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
    lfs f1, -22156(r2)
    addi r3, r1, 0x8
    lfs f0, -22208(r2)
    addi r4, r1, 0x20
    stfs f1, 0x14(r1)
    addi r5, r1, 0x14
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
    psq_l f31, 0x68(r1), 0, 0
    lwz r0, 0x74(r1)
    lfd f31, 0x60(r1)
    lwz r31, 0x5c(r1)
    lwz r30, 0x58(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
