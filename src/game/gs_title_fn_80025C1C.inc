    nofralloc
    stwu r1, -0xa0(r1)
    mflr r0
    stw r0, 0xa4(r1)
    stfd f31, 0x90(r1)
    psq_st f31, 0x98(r1), 0, 0
    stfd f30, 0x80(r1)
    psq_st f30, 0x88(r1), 0, 0
    stw r31, 0x7c(r1)
    stw r30, 0x78(r1)
    stw r29, 0x74(r1)
    fmr f30, f1
    mr r29, r3
    fmr f31, f3
    mr r31, r5
    mr r30, r6
    clrlwi r0, r4, 24
    cmplwi r0, 0x1
    bne @L_80025C84
    lfs f0, lbl_8047B8AC(r2)
    stfs f2, 0x14(r1)
    stfs f2, 0x18(r1)
    stfs f2, 0x1c(r1)
    stfs f0, 0x8(r1)
    stfs f0, 0xc(r1)
    stfs f0, 0x10(r1)
    b @L_80025CA0
    @L_80025C84:
    lfs f0, lbl_8047B8AC(r2)
    stfs f2, 0x8(r1)
    stfs f0, 0x14(r1)
    stfs f0, 0x18(r1)
    stfs f0, 0x1c(r1)
    stfs f2, 0xc(r1)
    stfs f2, 0x10(r1)
    @L_80025CA0:
    fmr f1, f30
    bl fn_800E0CA0
    addi r3, r1, 0x20
    addi r4, r1, 0x14
    addi r5, r1, 0x8
    bl fn_800E090C
    lfs f1, lbl_8047B8DC(r2)
    clrlwi r0, r31, 24
    lfs f0, 0x20(r1)
    cmplwi r0, 0x1
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x60(r1)
    lwz r31, 0x64(r1)
    bne @L_80025CFC
    lfs f0, lbl_8047B8B0(r2)
    stfs f31, 0x8(r1)
    stfs f0, 0x14(r1)
    stfs f0, 0x18(r1)
    stfs f0, 0x1c(r1)
    stfs f31, 0xc(r1)
    stfs f0, 0x10(r1)
    b @L_80025D18
    @L_80025CFC:
    lfs f0, lbl_8047B8B0(r2)
    stfs f31, 0x14(r1)
    stfs f31, 0x18(r1)
    stfs f0, 0x1c(r1)
    stfs f0, 0x8(r1)
    stfs f0, 0xc(r1)
    stfs f0, 0x10(r1)
    @L_80025D18:
    fmr f1, f30
    bl fn_800E0CA0
    addi r3, r1, 0x20
    addi r4, r1, 0x14
    addi r5, r1, 0x8
    bl fn_800E090C
    addi r3, r1, 0x2c
    addi r4, r1, 0x20
    bl fn_800E042C
    lfs f3, lbl_8047B8F0(r2)
    fmr f1, f30
    lfs f0, lbl_8047B8F4(r2)
    lfs f2, lbl_8047B8AC(r2)
    stfs f3, 0x14(r1)
    stfs f0, 0x18(r1)
    stfs f2, 0x1c(r1)
    lfs f0, 0x0(r30)
    stfs f0, 0x8(r1)
    lfs f0, 0x4(r30)
    stfs f0, 0xc(r1)
    stfs f2, 0x10(r1)
    bl fn_800E0CA0
    addi r3, r1, 0x20
    addi r4, r1, 0x14
    addi r5, r1, 0x8
    bl fn_800E090C
    addi r3, r1, 0x2c
    addi r4, r1, 0x20
    bl fn_800E03B4
    li r3, 0x3
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    li r3, 0x4
    bl fn_800D6A00
    lis r3, lbl_80314AE8@ha
    addi r3, r3, lbl_80314AE8@l
    bl fn_800D7820
    mr r4, r29
    li r3, 0x0
    bl fn_800D85D4
    li r3, 0x4
    bl fn_800D67BC
    lfs f2, lbl_8047B90C(r2)
    addi r3, r1, 0x8
    lfs f1, lbl_8047B910(r2)
    addi r4, r1, 0x2c
    lfs f0, lbl_8047B8AC(r2)
    addi r5, r1, 0x14
    stfs f2, 0x14(r1)
    stfs f1, 0x18(r1)
    stfs f0, 0x1c(r1)
    bl fn_800DFF98
    lfs f1, 0x8(r1)
    lfs f2, 0xc(r1)
    lfs f3, 0x10(r1)
    bl fn_800D6680
    clrlwi r7, r31, 24
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047B8AC(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    lfs f2, lbl_8047B8F0(r2)
    addi r3, r1, 0x8
    lfs f1, lbl_8047B910(r2)
    addi r4, r1, 0x2c
    lfs f0, lbl_8047B8AC(r2)
    addi r5, r1, 0x14
    stfs f2, 0x14(r1)
    stfs f1, 0x18(r1)
    stfs f0, 0x1c(r1)
    bl fn_800DFF98
    lfs f1, 0x8(r1)
    lfs f2, 0xc(r1)
    lfs f3, 0x10(r1)
    bl fn_800D6680
    clrlwi r7, r31, 24
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047B8B0(r2)
    li r3, 0x0
    lfs f2, lbl_8047B8AC(r2)
    bl fn_800D59B8
    lfs f2, lbl_8047B90C(r2)
    addi r3, r1, 0x8
    lfs f1, lbl_8047B8F4(r2)
    addi r4, r1, 0x2c
    lfs f0, lbl_8047B8AC(r2)
    addi r5, r1, 0x14
    stfs f2, 0x14(r1)
    stfs f1, 0x18(r1)
    stfs f0, 0x1c(r1)
    bl fn_800DFF98
    lfs f1, 0x8(r1)
    lfs f2, 0xc(r1)
    lfs f3, 0x10(r1)
    bl fn_800D6680
    clrlwi r7, r31, 24
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047B8AC(r2)
    li r3, 0x0
    lfs f2, lbl_8047B8B0(r2)
    bl fn_800D59B8
    lfs f2, lbl_8047B8F0(r2)
    addi r3, r1, 0x8
    lfs f1, lbl_8047B8F4(r2)
    addi r4, r1, 0x2c
    lfs f0, lbl_8047B8AC(r2)
    addi r5, r1, 0x14
    stfs f2, 0x14(r1)
    stfs f1, 0x18(r1)
    stfs f0, 0x1c(r1)
    bl fn_800DFF98
    lfs f1, 0x8(r1)
    lfs f2, 0xc(r1)
    lfs f3, 0x10(r1)
    bl fn_800D6680
    clrlwi r7, r31, 24
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047B8B0(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    bl fn_800D6728
    psq_l f31, 0x98(r1), 0, 0
    lfd f31, 0x90(r1)
    psq_l f30, 0x88(r1), 0, 0
    lfd f30, 0x80(r1)
    lwz r31, 0x7c(r1)
    lwz r30, 0x78(r1)
    lwz r0, 0xa4(r1)
    lwz r29, 0x74(r1)
    mtlr r0
    addi r1, r1, 0xa0
    blr
