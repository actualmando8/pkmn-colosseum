    nofralloc
    stwu r1, -0xf0(r1)
    mflr r0
    stw r0, 0xf4(r1)
    stfd f31, 0xe0(r1)
    psq_st f31, 0xe8(r1), 0, 0
    stfd f30, 0xd0(r1)
    psq_st f30, 0xd8(r1), 0, 0
    stfd f29, 0xc0(r1)
    psq_st f29, 0xc8(r1), 0, 0
    stfd f28, 0xb0(r1)
    psq_st f28, 0xb8(r1), 0, 0
    stfd f27, 0xa0(r1)
    psq_st f27, 0xa8(r1), 0, 0
    stfd f26, 0x90(r1)
    psq_st f26, 0x98(r1), 0, 0
    stw r31, 0x8c(r1)
    stw r30, 0x88(r1)
    stw r29, 0x84(r1)
    mr r29, r4
    fmr f29, f1
    lwz r3, 0x58(r29)
    bl fn_800F92D4
    lis r4, 0xef
    mr r31, r3
    addi r3, r4, 0x1200
    bl fn_800F92D4
    cmplwi r31, 0x0
    mr r30, r3
    beq @L_8001DF58
    cmplwi r30, 0x0
    beq @L_8001DF58
    mr r3, r31
    bl fn_800EF4FC
    lha r0, 0x5c(r29)
    clrlwi r3, r3, 16
    lis r4, 0x4330
    stw r3, 0x3c(r1)
    xoris r0, r0, 0x8000
    lfd f2, lbl_8047B7D8(r2)
    stw r4, 0x38(r1)
    mr r3, r31
    lfd f1, lbl_8047B7D0(r2)
    lfd f0, 0x38(r1)
    stw r0, 0x44(r1)
    fsubs f2, f0, f2
    stw r4, 0x40(r1)
    lfd f0, 0x40(r1)
    fsubs f0, f0, f1
    fdivs f31, f0, f2
    bl fn_800EF4FC
    lha r4, 0x5c(r29)
    clrlwi r3, r3, 16
    lha r0, 0x60(r29)
    lis r5, 0x4330
    stw r3, 0x4c(r1)
    mr r3, r31
    add r0, r4, r0
    lfd f2, lbl_8047B7D8(r2)
    xoris r0, r0, 0x8000
    stw r5, 0x48(r1)
    lfd f1, lbl_8047B7D0(r2)
    lfd f0, 0x48(r1)
    stw r0, 0x54(r1)
    fsubs f2, f0, f2
    stw r5, 0x50(r1)
    lfd f0, 0x50(r1)
    fsubs f0, f0, f1
    fdivs f30, f0, f2
    bl fn_800EF4F4
    lha r0, 0x5e(r29)
    clrlwi r3, r3, 16
    lis r4, 0x4330
    stw r3, 0x5c(r1)
    xoris r0, r0, 0x8000
    lfd f2, lbl_8047B7D8(r2)
    stw r4, 0x58(r1)
    mr r3, r31
    lfd f1, lbl_8047B7D0(r2)
    lfd f0, 0x58(r1)
    stw r0, 0x64(r1)
    fsubs f2, f0, f2
    stw r4, 0x60(r1)
    lfd f0, 0x60(r1)
    fsubs f0, f0, f1
    fdivs f27, f0, f2
    bl fn_800EF4F4
    lha r4, 0x5e(r29)
    lis r5, 0x4330
    lha r0, 0x62(r29)
    clrlwi r6, r3, 16
    lis r3, 0x8000
    stw r6, 0x6c(r1)
    add r0, r4, r0
    lfd f2, lbl_8047B7D8(r2)
    xoris r0, r0, 0x8000
    stw r5, 0x68(r1)
    lfd f1, lbl_8047B7D0(r2)
    addi r3, r3, 0x3
    lfd f0, 0x68(r1)
    stw r0, 0x74(r1)
    fsubs f2, f0, f2
    stw r5, 0x70(r1)
    lfd f0, 0x70(r1)
    fsubs f0, f0, f1
    fdivs f26, f0, f2
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    mr r3, r30
    li r4, 0x1
    li r5, 0x1
    bl fn_800EF590
    addi r6, r1, 0x8
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4
    bl fn_800D848C
    addi r6, r1, 0x8
    li r3, 0x1
    li r4, 0x0
    li r5, 0x5
    bl fn_800D848C
    addi r6, r1, 0x8
    li r3, 0x2
    li r4, 0x0
    li r5, 0x6
    bl fn_800D848C
    li r3, 0x3
    bl fn_800DC1D4
    mr r4, r31
    li r3, 0x0
    bl fn_800D85D4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_800DC224
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x1
    bl fn_800DC14C
    li r3, 0x0
    li r4, 0xf
    li r5, 0x8
    li r6, 0xa
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
    mr r4, r30
    li r3, 0x1
    bl fn_800D85D4
    li r3, 0x1
    li r4, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0x0
    bl fn_800DC224
    li r3, 0x1
    li r4, 0x1
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    bl fn_800DC14C
    li r3, 0x1
    li r4, 0xf
    li r5, 0x8
    li r6, 0xd
    li r7, 0x2
    bl fn_800DC0D4
    li r3, 0x1
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    bl fn_800DC04C
    li r3, 0x1
    li r4, 0x7
    li r5, 0x7
    li r6, 0x7
    li r7, 0x1
    bl fn_800DBFD4
    mr r4, r30
    li r3, 0x2
    bl fn_800D85D4
    li r3, 0x2
    li r4, 0x0
    li r5, 0x2
    li r6, 0x2
    li r7, 0x0
    bl fn_800DC224
    li r3, 0x2
    li r4, 0x1
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    bl fn_800DC14C
    li r3, 0x2
    li r4, 0xf
    li r5, 0x8
    li r6, 0xd
    li r7, 0x2
    bl fn_800DC0D4
    li r3, 0x2
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    bl fn_800DC04C
    li r3, 0x2
    li r4, 0x7
    li r5, 0x7
    li r6, 0x7
    li r7, 0x1
    bl fn_800DBFD4
    lwz r3, lbl_8047AD00(r13)
    bl fn_800D7820
    li r3, 0x7
    bl fn_800D6A00
    li r3, 0x2
    bl fn_800D67BC
    li r3, 0x0
    li r4, 0x0
    bl fn_800D61E4
    li r3, 0x0
    li r4, 0xff
    li r5, 0x40
    li r6, 0x40
    li r7, 0xff
    bl fn_800D5CB8
    fmr f1, f31
    li r3, 0x0
    fmr f2, f27
    bl fn_800D59B8
    fadds f28, f29, f27
    li r3, 0x1
    fadds f1, f29, f31
    fmr f2, f28
    bl fn_800D59B8
    lfs f0, lbl_8047B7E0(r2)
    fmr f2, f28
    li r3, 0x2
    fsubs f27, f0, f29
    fadds f1, f27, f31
    bl fn_800D59B8
    lha r3, 0x54(r29)
    lha r4, 0x56(r29)
    bl fn_800D61E4
    li r3, 0x0
    li r4, 0xff
    li r5, 0x40
    li r6, 0x40
    li r7, 0xff
    bl fn_800D5CB8
    fmr f1, f30
    li r3, 0x0
    fmr f2, f26
    bl fn_800D59B8
    fadds f28, f29, f26
    li r3, 0x1
    fadds f1, f29, f30
    fmr f2, f28
    bl fn_800D59B8
    fmr f2, f28
    li r3, 0x2
    fadds f1, f27, f30
    bl fn_800D59B8
    bl fn_800D6728
    li r3, 0x1
    bl fn_800DC1D4
    lis r3, 0x8000
    bl fn_800D888C
    @L_8001DF58:
    li r3, 0x0
    psq_l f31, 0xe8(r1), 0, 0
    lfd f31, 0xe0(r1)
    psq_l f30, 0xd8(r1), 0, 0
    lfd f30, 0xd0(r1)
    psq_l f29, 0xc8(r1), 0, 0
    lfd f29, 0xc0(r1)
    psq_l f28, 0xb8(r1), 0, 0
    lfd f28, 0xb0(r1)
    psq_l f27, 0xa8(r1), 0, 0
    lfd f27, 0xa0(r1)
    psq_l f26, 0x98(r1), 0, 0
    lfd f26, 0x90(r1)
    lwz r31, 0x8c(r1)
    lwz r30, 0x88(r1)
    lwz r0, 0xf4(r1)
    lwz r29, 0x84(r1)
    mtlr r0
    addi r1, r1, 0xf0
    blr
