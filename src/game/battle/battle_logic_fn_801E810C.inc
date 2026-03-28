    nofralloc
    mflr r0
    lis r3, -32697
    stw r0, 0x4(r1)
    stwu r1, -0x38(r1)
    stfd f31, 0x30(r1)
    stfd f30, 0x28(r1)
    stfd f29, 0x20(r1)
    stfd f28, 0x18(r1)
    stfd f27, 0x10(r1)
    stw r31, 0xc(r1)
    addi r31, r3, -11008
    li r3, 0x3
    stw r30, 0x8(r1)
    bl fn_8009B614
    lfs f27, -20968(r2)
    li r30, 0x0
    lfs f28, -20964(r2)
    lfs f29, -20960(r2)
    lfs f30, -20956(r2)
    lfs f31, -20952(r2)
    b @L_801E9AF0
@L_801E8160:
    lwz r3, -21104(r13)
    lwz r4, 0x118(r31)
    bl fn_801EB644
    lwz r3, -21104(r13)
    lwz r4, 0x11c(r31)
    bl fn_801EB644
    lwz r3, -21104(r13)
    lwz r4, 0x120(r31)
    bl fn_801EB644
    lwz r3, -21104(r13)
    lwz r4, 0x124(r31)
    bl fn_801EB644
    lwz r3, -21104(r13)
    lwz r4, 0x128(r31)
    bl fn_801EBCC0
    lwz r3, -21104(r13)
    lwz r4, 0x12c(r31)
    bl fn_801EC368
    lwz r3, 0x10c(r31)
    li r0, 0x280
    lwz r4, -21104(r13)
    subi r9, r31, 0x8
    stw r3, -21184(r13)
    clrlslwi r3, r30, 24, 4
    stw r0, -21152(r13)
    lbz r0, 0x680(r4)
    slwi r0, r0, 8
    add r0, r4, r0
    stw r0, -21120(r13)
    lwz r8, 0x118(r31)
    lwz r7, -21120(r13)
    li r4, 0x8
    mtctr r4
@L_801E81E4:
    psq_l f7, 0x0(r8), 0, 5
    psq_l f6, 0x0(r7), 0, 0
    lwz r6, 0xc(r8)
    lwz r0, 0x8(r8)
    ps_mul f7, f7, f6
    lwz r5, 0x4(r8)
    lhz r4, 0x2(r8)
    or. r6, r6, r0
@L_801E8204:
    cmpwi r6, 0x0
    bne @L_801E8350
    ps_merge00 f4, f7, f7
    cmpwi r5, 0x0
    psq_st f4, 0x8(r9), 0, 0
    bne @L_801E82B4
    psq_st f4, 0x10(r9), 0, 0
    cmpwi r4, 0x0
    psq_st f4, 0x18(r9), 0, 0
    bne @L_801E8240
    addi r7, r7, 0x20
    psq_stu f4, 0x20(r9), 0, 0
    addi r8, r8, 0x10
    bdnz @L_801E81E4
    b @L_801E8418
@L_801E8240:
    addi r8, r8, 0x10
    ps_msub f13, f7, f28, f7
    addi r7, r7, 0x20
    ps_merge00 f2, f7, f7
    lwz r5, 0x4(r8)
    ps_sub f1, f28, f29
    ps_msub f12, f7, f27, f13
    lhz r4, 0x2(r8)
    ps_merge11 f10, f7, f13
    psq_l f6, 0x0(r7), 0, 0
    ps_nmsub f11, f7, f1, f12
    ps_add f8, f2, f10
    psq_l f7, 0x0(r8), 0, 5
    ps_merge11 f9, f12, f11
    ps_sub f10, f2, f10
    lwz r6, 0xc(r8)
    ps_add f3, f2, f9
    lwz r0, 0x8(r8)
    ps_sub f9, f2, f9
    psq_stu f8, 0x8(r9), 0, 0
    ps_merge10 f9, f9, f9
    psq_stu f3, 0x8(r9), 0, 0
    ps_merge10 f10, f10, f10
    or r6, r6, r0
    psq_stu f9, 0x8(r9), 0, 0
    ps_mul f7, f7, f6
    psq_stu f10, 0x8(r9), 0, 0
    bdnz @L_801E8204
    b @L_801E8418
@L_801E82B4:
    psq_l f1, 0x4(r8), 0, 5
    psq_l f2, 0x8(r7), 0, 0
    addi r8, r8, 0x10
    ps_mul f1, f1, f2
    addi r7, r7, 0x20
    ps_sub f12, f7, f1
    ps_add f13, f7, f1
    lwz r6, 0xc(r8)
    ps_madd f11, f1, f27, f12
    ps_nmsub f10, f1, f27, f13
    ps_mul f3, f12, f28
    ps_merge00 f11, f13, f11
    lwz r0, 0x8(r8)
    ps_nmsub f9, f1, f30, f3
    ps_merge00 f10, f10, f12
    lwz r5, 0x4(r8)
    ps_sub f9, f9, f13
    ps_nmsub f8, f7, f29, f3
    lhz r4, 0x2(r8)
    ps_merge11 f13, f13, f9
    ps_msub f3, f12, f27, f9
    psq_l f7, 0x0(r8), 0, 5
    ps_add f2, f11, f13
    ps_sub f8, f8, f3
    psq_l f6, 0x0(r7), 0, 0
    ps_merge11 f12, f3, f8
    ps_sub f11, f11, f13
    psq_stu f2, 0x8(r9), 0, 0
    ps_add f4, f10, f12
    ps_sub f1, f10, f12
    or r6, r6, r0
    psq_stu f4, 0x8(r9), 0, 0
    ps_merge10 f1, f1, f1
    ps_merge10 f11, f11, f11
    psq_stu f1, 0x8(r9), 0, 0
    ps_mul f7, f7, f6
    psq_stu f11, 0x8(r9), 0, 0
    bdnz @L_801E8204
    b @L_801E8418
@L_801E8350:
    psq_l f2, 0x4(r8), 0, 5
    psq_l f10, 0x8(r7), 0, 0
    ps_mul f2, f2, f10
    psq_l f13, 0x8(r8), 0, 5
    psq_l f9, 0x10(r7), 0, 0
    ps_merge01 f4, f7, f2
    psq_l f12, 0xc(r8), 0, 5
    ps_merge01 f1, f2, f7
    psq_l f8, 0x18(r7), 0, 0
    addi r8, r8, 0x10
    ps_madd f11, f13, f9, f4
    ps_nmsub f10, f13, f9, f4
    ps_madd f9, f12, f8, f1
    ps_nmsub f8, f12, f8, f1
    addi r7, r7, 0x20
    ps_add f4, f11, f9
    ps_sub f12, f11, f9
    ps_msub f13, f8, f27, f9
    lwz r6, 0xc(r8)
    ps_sub f3, f8, f10
    ps_add f1, f10, f13
    ps_sub f13, f10, f13
    ps_mul f3, f3, f28
    lwz r0, 0x8(r8)
    ps_merge00 f1, f4, f1
    ps_nmsub f9, f10, f30, f3
    ps_msub f11, f8, f29, f3
    lwz r5, 0x4(r8)
    ps_sub f9, f9, f4
    ps_merge00 f13, f13, f12
    lhz r4, 0x2(r8)
    ps_madd f10, f12, f27, f9
    ps_merge11 f8, f4, f9
    psq_l f7, 0x0(r8), 0, 5
    ps_sub f11, f11, f10
    ps_add f12, f1, f8
    psq_l f6, 0x0(r7), 0, 0
    ps_merge11 f11, f10, f11
    ps_sub f4, f1, f8
    ps_mul f7, f7, f6
    ps_add f10, f13, f11
    ps_sub f9, f13, f11
    ps_merge10 f10, f10, f10
    psq_stu f12, 0x8(r9), 0, 0
    ps_merge10 f4, f4, f4
    psq_stu f9, 0x8(r9), 0, 0
    psq_stu f10, 0x8(r9), 0, 0
    or r6, r6, r0
    psq_stu f4, 0x8(r9), 0, 0
    bdnz @L_801E8204
@L_801E8418:
    lwz r8, -21184(r13)
    mr r9, r31
    lwz r0, -21152(r13)
    psq_l f7, 0x0(r9), 0, 0
    slwi r4, r3, 2
    psq_l f6, 0x80(r9), 0, 0
    slwi r6, r0, 2
    psq_l f5, 0x40(r9), 0, 0
    mr r7, r4
    ps_add f9, f7, f6
    psq_l f4, 0xc0(r9), 0, 0
    ps_sub f3, f7, f6
    add r6, r7, r6
    ps_add f9, f9, f31
    li r0, 0x3
    ps_add f8, f5, f4
    add r5, r8, r7
    ps_sub f2, f5, f4
    ps_add f4, f9, f8
    add r4, r8, r6
    ps_add f3, f3, f31
    mtctr r0
@L_801E8470:
    psq_l f11, 0x20(r9), 0, 0
    ps_msub f2, f2, f27, f8
    psq_l f10, 0x60(r9), 0, 0
    ps_sub f12, f9, f8
    ps_add f1, f3, f2
    psq_l f9, 0xa0(r9), 0, 0
    ps_sub f13, f3, f2
    psq_l f8, 0xe0(r9), 0, 0
    ps_add f3, f9, f10
    ps_sub f9, f9, f10
    addi r9, r9, 0x8
    ps_add f2, f11, f8
    ps_sub f11, f11, f8
    psq_l f7, 0x0(r9), 0, 0
    ps_add f8, f2, f3
    ps_sub f10, f2, f3
    ps_add f3, f9, f11
    psq_l f6, 0x80(r9), 0, 0
    ps_add f2, f4, f8
    ps_mul f3, f3, f28
    psq_l f5, 0x40(r9), 0, 0
    ps_sub f0, f4, f8
    ps_madd f9, f9, f30, f3
    psq_l f4, 0xc0(r9), 0, 0
    ps_sub f9, f9, f8
    addi r7, r7, 0x2
    psq_st f2, 0x0(r5), 0, 6
    ps_msub f11, f11, f29, f3
    ps_add f2, f1, f9
    ps_msub f10, f10, f27, f9
    ps_sub f1, f1, f9
    psq_st f2, 0x8(r5), 0, 6
    ps_add f3, f13, f10
    ps_add f11, f11, f10
    psq_st f3, 0x10(r5), 0, 6
    addi r6, r6, 0x2
    ps_sub f2, f12, f11
    ps_add f3, f12, f11
    psq_st f2, 0x18(r5), 0, 6
    ps_sub f2, f13, f10
    ps_add f9, f7, f6
    psq_st f3, 0x0(r4), 0, 6
    ps_sub f3, f7, f6
    ps_add f9, f9, f31
    psq_st f2, 0x8(r4), 0, 6
    ps_add f8, f5, f4
    ps_sub f2, f5, f4
    psq_st f1, 0x10(r4), 0, 6
    add r5, r8, r7
    ps_add f4, f9, f8
    psq_st f0, 0x18(r4), 0, 6
    ps_add f3, f3, f31
    add r4, r8, r6
    bdnz @L_801E8470
    psq_l f11, 0x20(r9), 0, 0
    ps_msub f2, f2, f27, f8
    psq_l f10, 0x60(r9), 0, 0
    ps_sub f12, f9, f8
    ps_add f1, f3, f2
    psq_l f9, 0xa0(r9), 0, 0
    ps_sub f13, f3, f2
    psq_l f8, 0xe0(r9), 0, 0
    ps_add f3, f9, f10
    ps_sub f9, f9, f10
    ps_add f2, f11, f8
    ps_sub f11, f11, f8
    ps_add f8, f2, f3
    ps_sub f10, f2, f3
    ps_add f3, f9, f11
    ps_add f2, f4, f8
    ps_mul f3, f3, f28
    ps_sub f0, f4, f8
    ps_madd f9, f9, f30, f3
    psq_st f2, 0x0(r5), 0, 6
    ps_sub f9, f9, f8
    ps_msub f11, f11, f29, f3
    psq_st f0, 0x18(r4), 0, 6
    ps_add f2, f1, f9
    ps_msub f10, f10, f27, f9
    ps_sub f1, f1, f9
    psq_st f2, 0x8(r5), 0, 6
    ps_add f3, f13, f10
    ps_add f11, f11, f10
    psq_st f1, 0x10(r4), 0, 6
    psq_st f3, 0x10(r5), 0, 6
    ps_sub f2, f12, f11
    ps_add f3, f12, f11
    psq_st f2, 0x18(r5), 0, 6
    ps_sub f2, f13, f10
    psq_st f3, 0x0(r4), 0, 6
    psq_st f2, 0x8(r4), 0, 6
    lwz r8, 0x11c(r31)
    lwz r7, -21120(r13)
    addi r10, r3, 0x8
    subi r9, r31, 0x8
    li r4, 0x8
    mtctr r4
@L_801E85F4:
    psq_l f7, 0x0(r8), 0, 5
    psq_l f6, 0x0(r7), 0, 0
    lwz r6, 0xc(r8)
    lwz r0, 0x8(r8)
    ps_mul f7, f7, f6
    lwz r5, 0x4(r8)
    lhz r4, 0x2(r8)
    or. r6, r6, r0
@L_801E8614:
    cmpwi r6, 0x0
    bne @L_801E8760
    ps_merge00 f4, f7, f7
    cmpwi r5, 0x0
    psq_st f4, 0x8(r9), 0, 0
    bne @L_801E86C4
    psq_st f4, 0x10(r9), 0, 0
    cmpwi r4, 0x0
    psq_st f4, 0x18(r9), 0, 0
    bne @L_801E8650
    addi r7, r7, 0x20
    psq_stu f4, 0x20(r9), 0, 0
    addi r8, r8, 0x10
    bdnz @L_801E85F4
    b @L_801E8828
@L_801E8650:
    addi r8, r8, 0x10
    ps_msub f13, f7, f28, f7
    addi r7, r7, 0x20
    ps_merge00 f2, f7, f7
    lwz r5, 0x4(r8)
    ps_sub f1, f28, f29
    ps_msub f12, f7, f27, f13
    lhz r4, 0x2(r8)
    ps_merge11 f10, f7, f13
    psq_l f6, 0x0(r7), 0, 0
    ps_nmsub f11, f7, f1, f12
    ps_add f8, f2, f10
    psq_l f7, 0x0(r8), 0, 5
    ps_merge11 f9, f12, f11
    ps_sub f10, f2, f10
    lwz r6, 0xc(r8)
    ps_add f3, f2, f9
    lwz r0, 0x8(r8)
    ps_sub f9, f2, f9
    psq_stu f8, 0x8(r9), 0, 0
    ps_merge10 f9, f9, f9
    psq_stu f3, 0x8(r9), 0, 0
    ps_merge10 f10, f10, f10
    or r6, r6, r0
    psq_stu f9, 0x8(r9), 0, 0
    ps_mul f7, f7, f6
    psq_stu f10, 0x8(r9), 0, 0
    bdnz @L_801E8614
    b @L_801E8828
@L_801E86C4:
    psq_l f1, 0x4(r8), 0, 5
    psq_l f2, 0x8(r7), 0, 0
    addi r8, r8, 0x10
    ps_mul f1, f1, f2
    addi r7, r7, 0x20
    ps_sub f12, f7, f1
    ps_add f13, f7, f1
    lwz r6, 0xc(r8)
    ps_madd f11, f1, f27, f12
    ps_nmsub f10, f1, f27, f13
    ps_mul f3, f12, f28
    ps_merge00 f11, f13, f11
    lwz r0, 0x8(r8)
    ps_nmsub f9, f1, f30, f3
    ps_merge00 f10, f10, f12
    lwz r5, 0x4(r8)
    ps_sub f9, f9, f13
    ps_nmsub f8, f7, f29, f3
    lhz r4, 0x2(r8)
    ps_merge11 f13, f13, f9
    ps_msub f3, f12, f27, f9
    psq_l f7, 0x0(r8), 0, 5
    ps_add f2, f11, f13
    ps_sub f8, f8, f3
    psq_l f6, 0x0(r7), 0, 0
    ps_merge11 f12, f3, f8
    ps_sub f11, f11, f13
    psq_stu f2, 0x8(r9), 0, 0
    ps_add f4, f10, f12
    ps_sub f1, f10, f12
    or r6, r6, r0
    psq_stu f4, 0x8(r9), 0, 0
    ps_merge10 f1, f1, f1
    ps_merge10 f11, f11, f11
    psq_stu f1, 0x8(r9), 0, 0
    ps_mul f7, f7, f6
    psq_stu f11, 0x8(r9), 0, 0
    bdnz @L_801E8614
    b @L_801E8828
@L_801E8760:
    psq_l f2, 0x4(r8), 0, 5
    psq_l f10, 0x8(r7), 0, 0
    ps_mul f2, f2, f10
    psq_l f13, 0x8(r8), 0, 5
    psq_l f9, 0x10(r7), 0, 0
    ps_merge01 f4, f7, f2
    psq_l f12, 0xc(r8), 0, 5
    ps_merge01 f1, f2, f7
    psq_l f8, 0x18(r7), 0, 0
    addi r8, r8, 0x10
    ps_madd f11, f13, f9, f4
    ps_nmsub f10, f13, f9, f4
    ps_madd f9, f12, f8, f1
    ps_nmsub f8, f12, f8, f1
    addi r7, r7, 0x20
    ps_add f4, f11, f9
    ps_sub f12, f11, f9
    ps_msub f13, f8, f27, f9
    lwz r6, 0xc(r8)
    ps_sub f3, f8, f10
    ps_add f1, f10, f13
    ps_sub f13, f10, f13
    ps_mul f3, f3, f28
    lwz r0, 0x8(r8)
    ps_merge00 f1, f4, f1
    ps_nmsub f9, f10, f30, f3
    ps_msub f11, f8, f29, f3
    lwz r5, 0x4(r8)
    ps_sub f9, f9, f4
    ps_merge00 f13, f13, f12
    lhz r4, 0x2(r8)
    ps_madd f10, f12, f27, f9
    ps_merge11 f8, f4, f9
    psq_l f7, 0x0(r8), 0, 5
    ps_sub f11, f11, f10
    ps_add f12, f1, f8
    psq_l f6, 0x0(r7), 0, 0
    ps_merge11 f11, f10, f11
    ps_sub f4, f1, f8
    ps_mul f7, f7, f6
    ps_add f10, f13, f11
    ps_sub f9, f13, f11
    ps_merge10 f10, f10, f10
    psq_stu f12, 0x8(r9), 0, 0
    ps_merge10 f4, f4, f4
    psq_stu f9, 0x8(r9), 0, 0
    psq_stu f10, 0x8(r9), 0, 0
    or r6, r6, r0
    psq_stu f4, 0x8(r9), 0, 0
    bdnz @L_801E8614
@L_801E8828:
    lwz r8, -21184(r13)
    mr r9, r31
    lwz r0, -21152(r13)
    psq_l f7, 0x0(r9), 0, 0
    slwi r10, r10, 2
    psq_l f6, 0x80(r9), 0, 0
    slwi r6, r0, 2
    psq_l f5, 0x40(r9), 0, 0
    mr r7, r10
    ps_add f9, f7, f6
    psq_l f4, 0xc0(r9), 0, 0
    ps_sub f3, f7, f6
    add r6, r7, r6
    ps_add f9, f9, f31
    li r0, 0x3
    ps_add f8, f5, f4
    add r5, r8, r7
    ps_sub f2, f5, f4
    ps_add f4, f9, f8
    add r4, r8, r6
    ps_add f3, f3, f31
    mtctr r0
@L_801E8880:
    psq_l f11, 0x20(r9), 0, 0
    ps_msub f2, f2, f27, f8
    psq_l f10, 0x60(r9), 0, 0
    ps_sub f12, f9, f8
    ps_add f1, f3, f2
    psq_l f9, 0xa0(r9), 0, 0
    ps_sub f13, f3, f2
    psq_l f8, 0xe0(r9), 0, 0
    ps_add f3, f9, f10
    ps_sub f9, f9, f10
    addi r9, r9, 0x8
    ps_add f2, f11, f8
    ps_sub f11, f11, f8
    psq_l f7, 0x0(r9), 0, 0
    ps_add f8, f2, f3
    ps_sub f10, f2, f3
    ps_add f3, f9, f11
    psq_l f6, 0x80(r9), 0, 0
    ps_add f2, f4, f8
    ps_mul f3, f3, f28
    psq_l f5, 0x40(r9), 0, 0
    ps_sub f0, f4, f8
    ps_madd f9, f9, f30, f3
    psq_l f4, 0xc0(r9), 0, 0
    ps_sub f9, f9, f8
    addi r7, r7, 0x2
    psq_st f2, 0x0(r5), 0, 6
    ps_msub f11, f11, f29, f3
    ps_add f2, f1, f9
    ps_msub f10, f10, f27, f9
    ps_sub f1, f1, f9
    psq_st f2, 0x8(r5), 0, 6
    ps_add f3, f13, f10
    ps_add f11, f11, f10
    psq_st f3, 0x10(r5), 0, 6
    addi r6, r6, 0x2
    ps_sub f2, f12, f11
    ps_add f3, f12, f11
    psq_st f2, 0x18(r5), 0, 6
    ps_sub f2, f13, f10
    ps_add f9, f7, f6
    psq_st f3, 0x0(r4), 0, 6
    ps_sub f3, f7, f6
    ps_add f9, f9, f31
    psq_st f2, 0x8(r4), 0, 6
    ps_add f8, f5, f4
    ps_sub f2, f5, f4
    psq_st f1, 0x10(r4), 0, 6
    add r5, r8, r7
    ps_add f4, f9, f8
    psq_st f0, 0x18(r4), 0, 6
    ps_add f3, f3, f31
    add r4, r8, r6
    bdnz @L_801E8880
    psq_l f11, 0x20(r9), 0, 0
    ps_msub f2, f2, f27, f8
    psq_l f10, 0x60(r9), 0, 0
    ps_sub f12, f9, f8
    ps_add f1, f3, f2
    psq_l f9, 0xa0(r9), 0, 0
    ps_sub f13, f3, f2
    psq_l f8, 0xe0(r9), 0, 0
    ps_add f3, f9, f10
    ps_sub f9, f9, f10
    ps_add f2, f11, f8
    ps_sub f11, f11, f8
    ps_add f8, f2, f3
    ps_sub f10, f2, f3
    ps_add f3, f9, f11
    ps_add f2, f4, f8
    ps_mul f3, f3, f28
    ps_sub f0, f4, f8
    ps_madd f9, f9, f30, f3
    psq_st f2, 0x0(r5), 0, 6
    ps_sub f9, f9, f8
    ps_msub f11, f11, f29, f3
    psq_st f0, 0x18(r4), 0, 6
    ps_add f2, f1, f9
    ps_msub f10, f10, f27, f9
    ps_sub f1, f1, f9
    psq_st f2, 0x8(r5), 0, 6
    ps_add f3, f13, f10
    ps_add f11, f11, f10
    psq_st f1, 0x10(r4), 0, 6
    psq_st f3, 0x10(r5), 0, 6
    ps_sub f2, f12, f11
    ps_add f3, f12, f11
    psq_st f2, 0x18(r5), 0, 6
    ps_sub f2, f13, f10
    psq_st f3, 0x0(r4), 0, 6
    psq_st f2, 0x8(r4), 0, 6
    lwz r8, 0x120(r31)
    lwz r7, -21120(r13)
    subi r9, r31, 0x8
    li r4, 0x8
    mtctr r4
@L_801E8A00:
    psq_l f7, 0x0(r8), 0, 5
    psq_l f6, 0x0(r7), 0, 0
    lwz r6, 0xc(r8)
    lwz r0, 0x8(r8)
    ps_mul f7, f7, f6
    lwz r5, 0x4(r8)
    lhz r4, 0x2(r8)
    or r6, r6, r0
@L_801E8A20:
    cmpwi r6, 0x0
    bne @L_801E8B6C
    ps_merge00 f4, f7, f7
    cmpwi r5, 0x0
    psq_st f4, 0x8(r9), 0, 0
    bne @L_801E8AD0
    psq_st f4, 0x10(r9), 0, 0
    cmpwi r4, 0x0
    psq_st f4, 0x18(r9), 0, 0
    bne @L_801E8A5C
    addi r7, r7, 0x20
    psq_stu f4, 0x20(r9), 0, 0
    addi r8, r8, 0x10
    bdnz @L_801E8A00
    b @L_801E8C34
@L_801E8A5C:
    ps_msub f13, f7, f28, f7
    addi r8, r8, 0x10
    ps_merge00 f2, f7, f7
    addi r7, r7, 0x20
    ps_sub f1, f28, f29
    lwz r5, 0x4(r8)
    ps_msub f12, f7, f27, f13
    lhz r4, 0x2(r8)
    ps_merge11 f10, f7, f13
    psq_l f6, 0x0(r7), 0, 0
    ps_nmsub f11, f7, f1, f12
    ps_add f8, f2, f10
    psq_l f7, 0x0(r8), 0, 5
    ps_merge11 f9, f12, f11
    ps_sub f10, f2, f10
    lwz r6, 0xc(r8)
    ps_add f3, f2, f9
    lwz r0, 0x8(r8)
    ps_sub f9, f2, f9
    psq_stu f8, 0x8(r9), 0, 0
    ps_merge10 f9, f9, f9
    psq_stu f3, 0x8(r9), 0, 0
    ps_merge10 f10, f10, f10
    or r6, r6, r0
    psq_stu f9, 0x8(r9), 0, 0
    ps_mul f7, f7, f6
    psq_stu f10, 0x8(r9), 0, 0
    bdnz @L_801E8A20
    b @L_801E8C34
@L_801E8AD0:
    psq_l f1, 0x4(r8), 0, 5
    psq_l f2, 0x8(r7), 0, 0
    addi r8, r8, 0x10
    ps_mul f1, f1, f2
    addi r7, r7, 0x20
    ps_sub f12, f7, f1
    ps_add f13, f7, f1
    lwz r6, 0xc(r8)
    ps_madd f11, f1, f27, f12
    ps_nmsub f10, f1, f27, f13
    ps_mul f3, f12, f28
    ps_merge00 f11, f13, f11
    lwz r0, 0x8(r8)
    ps_nmsub f9, f1, f30, f3
    ps_merge00 f10, f10, f12
    lwz r5, 0x4(r8)
    ps_sub f9, f9, f13
    ps_nmsub f8, f7, f29, f3
    lhz r4, 0x2(r8)
    ps_merge11 f13, f13, f9
    ps_msub f3, f12, f27, f9
    psq_l f7, 0x0(r8), 0, 5
    ps_add f2, f11, f13
    ps_sub f8, f8, f3
    psq_l f6, 0x0(r7), 0, 0
    ps_merge11 f12, f3, f8
    ps_sub f11, f11, f13
    psq_stu f2, 0x8(r9), 0, 0
    ps_add f4, f10, f12
    ps_sub f1, f10, f12
    or r6, r6, r0
    psq_stu f4, 0x8(r9), 0, 0
    ps_merge10 f1, f1, f1
    ps_merge10 f11, f11, f11
    psq_stu f1, 0x8(r9), 0, 0
    ps_mul f7, f7, f6
    psq_stu f11, 0x8(r9), 0, 0
    bdnz @L_801E8A20
    b @L_801E8C34
@L_801E8B6C:
    psq_l f2, 0x4(r8), 0, 5
    psq_l f10, 0x8(r7), 0, 0
    ps_mul f2, f2, f10
    psq_l f13, 0x8(r8), 0, 5
    psq_l f9, 0x10(r7), 0, 0
    ps_merge01 f4, f7, f2
    psq_l f12, 0xc(r8), 0, 5
    ps_merge01 f1, f2, f7
    psq_l f8, 0x18(r7), 0, 0
    addi r8, r8, 0x10
    ps_madd f11, f13, f9, f4
    ps_nmsub f10, f13, f9, f4
    ps_madd f9, f12, f8, f1
    ps_nmsub f8, f12, f8, f1
    addi r7, r7, 0x20
    ps_add f4, f11, f9
    ps_sub f12, f11, f9
    ps_msub f13, f8, f27, f9
    lwz r6, 0xc(r8)
    ps_sub f3, f8, f10
    ps_add f1, f10, f13
    ps_sub f13, f10, f13
    ps_mul f3, f3, f28
    lwz r0, 0x8(r8)
    ps_merge00 f1, f4, f1
    ps_nmsub f9, f10, f30, f3
    ps_msub f11, f8, f29, f3
    lwz r5, 0x4(r8)
    ps_sub f9, f9, f4
    ps_merge00 f13, f13, f12
    lhz r4, 0x2(r8)
    ps_madd f10, f12, f27, f9
    ps_merge11 f8, f4, f9
    psq_l f7, 0x0(r8), 0, 5
    ps_sub f11, f11, f10
    ps_add f12, f1, f8
    psq_l f6, 0x0(r7), 0, 0
    ps_merge11 f11, f10, f11
    ps_sub f4, f1, f8
    ps_mul f7, f7, f6
    ps_add f10, f13, f11
    ps_sub f9, f13, f11
    ps_merge10 f10, f10, f10
    psq_stu f12, 0x8(r9), 0, 0
    ps_merge10 f4, f4, f4
    psq_stu f9, 0x8(r9), 0, 0
    psq_stu f10, 0x8(r9), 0, 0
    or r6, r6, r0
    psq_stu f4, 0x8(r9), 0, 0
    bdnz @L_801E8A20
@L_801E8C34:
    lwz r8, -21184(r13)
    mr r9, r31
    lwz r0, -21152(r13)
    psq_l f7, 0x0(r9), 0, 0
    slwi r7, r0, 3
    psq_l f6, 0x80(r9), 0, 0
    slwi r4, r3, 2
    psq_l f5, 0x40(r9), 0, 0
    slwi r6, r0, 2
    ps_add f9, f7, f6
    add r7, r7, r4
    psq_l f4, 0xc0(r9), 0, 0
    ps_sub f3, f7, f6
    add r6, r7, r6
    ps_add f9, f9, f31
    li r0, 0x3
    ps_add f8, f5, f4
    add r5, r8, r7
    ps_sub f2, f5, f4
    ps_add f4, f9, f8
    add r4, r8, r6
    ps_add f3, f3, f31
    mtctr r0
@L_801E8C90:
    psq_l f11, 0x20(r9), 0, 0
    ps_msub f2, f2, f27, f8
    psq_l f10, 0x60(r9), 0, 0
    ps_sub f12, f9, f8
    ps_add f1, f3, f2
    psq_l f9, 0xa0(r9), 0, 0
    ps_sub f13, f3, f2
    psq_l f8, 0xe0(r9), 0, 0
    ps_add f3, f9, f10
    ps_sub f9, f9, f10
    addi r9, r9, 0x8
    ps_add f2, f11, f8
    ps_sub f11, f11, f8
    psq_l f7, 0x0(r9), 0, 0
    ps_add f8, f2, f3
    ps_sub f10, f2, f3
    ps_add f3, f9, f11
    psq_l f6, 0x80(r9), 0, 0
    ps_add f2, f4, f8
    ps_mul f3, f3, f28
    psq_l f5, 0x40(r9), 0, 0
    ps_sub f0, f4, f8
    ps_madd f9, f9, f30, f3
    psq_l f4, 0xc0(r9), 0, 0
    ps_sub f9, f9, f8
    addi r7, r7, 0x2
    psq_st f2, 0x0(r5), 0, 6
    ps_msub f11, f11, f29, f3
    ps_add f2, f1, f9
    ps_msub f10, f10, f27, f9
    ps_sub f1, f1, f9
    psq_st f2, 0x8(r5), 0, 6
    ps_add f3, f13, f10
    ps_add f11, f11, f10
    psq_st f3, 0x10(r5), 0, 6
    addi r6, r6, 0x2
    ps_sub f2, f12, f11
    ps_add f3, f12, f11
    psq_st f2, 0x18(r5), 0, 6
    ps_sub f2, f13, f10
    ps_add f9, f7, f6
    psq_st f3, 0x0(r4), 0, 6
    ps_sub f3, f7, f6
    ps_add f9, f9, f31
    psq_st f2, 0x8(r4), 0, 6
    ps_add f8, f5, f4
    ps_sub f2, f5, f4
    psq_st f1, 0x10(r4), 0, 6
    add r5, r8, r7
    ps_add f4, f9, f8
    psq_st f0, 0x18(r4), 0, 6
    ps_add f3, f3, f31
    add r4, r8, r6
    bdnz @L_801E8C90
    psq_l f11, 0x20(r9), 0, 0
    ps_msub f2, f2, f27, f8
    psq_l f10, 0x60(r9), 0, 0
    ps_sub f12, f9, f8
    ps_add f1, f3, f2
    psq_l f9, 0xa0(r9), 0, 0
    ps_sub f13, f3, f2
    psq_l f8, 0xe0(r9), 0, 0
    ps_add f3, f9, f10
    ps_sub f9, f9, f10
    ps_add f2, f11, f8
    ps_sub f11, f11, f8
    ps_add f8, f2, f3
    ps_sub f10, f2, f3
    ps_add f3, f9, f11
    ps_add f2, f4, f8
    ps_mul f3, f3, f28
    ps_sub f0, f4, f8
    ps_madd f9, f9, f30, f3
    psq_st f2, 0x0(r5), 0, 6
    ps_sub f9, f9, f8
    ps_msub f11, f11, f29, f3
    psq_st f0, 0x18(r4), 0, 6
    ps_add f2, f1, f9
    ps_msub f10, f10, f27, f9
    ps_sub f1, f1, f9
    psq_st f2, 0x8(r5), 0, 6
    ps_add f3, f13, f10
    ps_add f11, f11, f10
    psq_st f3, 0x10(r5), 0, 6
    ps_sub f2, f12, f11
    psq_st f1, 0x10(r4), 0, 6
    ps_add f3, f12, f11
    psq_st f2, 0x18(r5), 0, 6
    ps_sub f2, f13, f10
    psq_st f3, 0x0(r4), 0, 6
    psq_st f2, 0x8(r4), 0, 6
    lwz r8, 0x124(r31)
    lwz r7, -21120(r13)
    addi r9, r3, 0x8
    subi r10, r31, 0x8
    li r4, 0x8
    mtctr r4
@L_801E8E14:
    psq_l f7, 0x0(r8), 0, 5
    psq_l f6, 0x0(r7), 0, 0
    lwz r6, 0xc(r8)
    lwz r0, 0x8(r8)
    ps_mul f7, f7, f6
    lwz r5, 0x4(r8)
    lhz r4, 0x2(r8)
    or r6, r6, r0
@L_801E8E34:
    cmpwi r6, 0x0
    bne @L_801E8F80
    ps_merge00 f4, f7, f7
    cmpwi r5, 0x0
    psq_st f4, 0x8(r10), 0, 0
    bne @L_801E8EE4
    psq_st f4, 0x10(r10), 0, 0
    cmpwi r4, 0x0
    psq_st f4, 0x18(r10), 0, 0
    bne @L_801E8E70
    addi r7, r7, 0x20
    psq_stu f4, 0x20(r10), 0, 0
    addi r8, r8, 0x10
    bdnz @L_801E8E14
    b @L_801E9048
@L_801E8E70:
    ps_msub f13, f7, f28, f7
    addi r8, r8, 0x10
    ps_merge00 f2, f7, f7
    addi r7, r7, 0x20
    ps_sub f1, f28, f29
    lwz r5, 0x4(r8)
    ps_msub f12, f7, f27, f13
    lhz r4, 0x2(r8)
    ps_merge11 f10, f7, f13
    psq_l f6, 0x0(r7), 0, 0
    ps_nmsub f11, f7, f1, f12
    ps_add f8, f2, f10
    psq_l f7, 0x0(r8), 0, 5
    ps_merge11 f9, f12, f11
    ps_sub f10, f2, f10
    lwz r6, 0xc(r8)
    ps_add f3, f2, f9
    lwz r0, 0x8(r8)
    ps_sub f9, f2, f9
    psq_stu f8, 0x8(r10), 0, 0
    ps_merge10 f9, f9, f9
    psq_stu f3, 0x8(r10), 0, 0
    ps_merge10 f10, f10, f10
    or r6, r6, r0
    psq_stu f9, 0x8(r10), 0, 0
    ps_mul f7, f7, f6
    psq_stu f10, 0x8(r10), 0, 0
    bdnz @L_801E8E34
    b @L_801E9048
@L_801E8EE4:
    psq_l f1, 0x4(r8), 0, 5
    psq_l f2, 0x8(r7), 0, 0
    addi r8, r8, 0x10
    ps_mul f1, f1, f2
    addi r7, r7, 0x20
    ps_sub f12, f7, f1
    ps_add f13, f7, f1
    lwz r6, 0xc(r8)
    ps_madd f11, f1, f27, f12
    ps_nmsub f10, f1, f27, f13
    ps_mul f3, f12, f28
    ps_merge00 f11, f13, f11
    lwz r0, 0x8(r8)
    ps_nmsub f9, f1, f30, f3
    ps_merge00 f10, f10, f12
    lwz r5, 0x4(r8)
    ps_sub f9, f9, f13
    ps_nmsub f8, f7, f29, f3
    lhz r4, 0x2(r8)
    ps_merge11 f13, f13, f9
    ps_msub f3, f12, f27, f9
    psq_l f7, 0x0(r8), 0, 5
    ps_add f2, f11, f13
    ps_sub f8, f8, f3
    psq_l f6, 0x0(r7), 0, 0
    ps_merge11 f12, f3, f8
    ps_sub f11, f11, f13
    psq_stu f2, 0x8(r10), 0, 0
    ps_add f4, f10, f12
    ps_sub f1, f10, f12
    or r6, r6, r0
    psq_stu f4, 0x8(r10), 0, 0
    ps_merge10 f1, f1, f1
    ps_merge10 f11, f11, f11
    psq_stu f1, 0x8(r10), 0, 0
    ps_mul f7, f7, f6
    psq_stu f11, 0x8(r10), 0, 0
    bdnz @L_801E8E34
    b @L_801E9048
@L_801E8F80:
    psq_l f2, 0x4(r8), 0, 5
    psq_l f10, 0x8(r7), 0, 0
    ps_mul f2, f2, f10
    psq_l f13, 0x8(r8), 0, 5
    psq_l f9, 0x10(r7), 0, 0
    ps_merge01 f4, f7, f2
    psq_l f12, 0xc(r8), 0, 5
    ps_merge01 f1, f2, f7
    psq_l f8, 0x18(r7), 0, 0
    addi r8, r8, 0x10
    ps_madd f11, f13, f9, f4
    ps_nmsub f10, f13, f9, f4
    ps_madd f9, f12, f8, f1
    ps_nmsub f8, f12, f8, f1
    addi r7, r7, 0x20
    ps_add f4, f11, f9
    ps_sub f12, f11, f9
    ps_msub f13, f8, f27, f9
    lwz r6, 0xc(r8)
    ps_sub f3, f8, f10
    ps_add f1, f10, f13
    ps_sub f13, f10, f13
    ps_mul f3, f3, f28
    lwz r0, 0x8(r8)
    ps_merge00 f1, f4, f1
    ps_nmsub f9, f10, f30, f3
    ps_msub f11, f8, f29, f3
    lwz r5, 0x4(r8)
    ps_sub f9, f9, f4
    ps_merge00 f13, f13, f12
    lhz r4, 0x2(r8)
    ps_madd f10, f12, f27, f9
    ps_merge11 f8, f4, f9
    psq_l f7, 0x0(r8), 0, 5
    ps_sub f11, f11, f10
    ps_add f12, f1, f8
    psq_l f6, 0x0(r7), 0, 0
    ps_merge11 f11, f10, f11
    ps_sub f4, f1, f8
    ps_mul f7, f7, f6
    ps_add f10, f13, f11
    ps_sub f9, f13, f11
    ps_merge10 f10, f10, f10
    psq_stu f12, 0x8(r10), 0, 0
    ps_merge10 f4, f4, f4
    psq_stu f9, 0x8(r10), 0, 0
    psq_stu f10, 0x8(r10), 0, 0
    or r6, r6, r0
    psq_stu f4, 0x8(r10), 0, 0
    bdnz @L_801E8E34
@L_801E9048:
    lwz r8, -21184(r13)
    mr r10, r31
    lwz r0, -21152(r13)
    psq_l f7, 0x0(r10), 0, 0
    slwi r7, r0, 3
    psq_l f6, 0x80(r10), 0, 0
    slwi r9, r9, 2
    psq_l f5, 0x40(r10), 0, 0
    slwi r6, r0, 2
    ps_add f9, f7, f6
    add r7, r7, r9
    psq_l f4, 0xc0(r10), 0, 0
    ps_sub f3, f7, f6
    add r6, r7, r6
    ps_add f9, f9, f31
    li r0, 0x3
    ps_add f8, f5, f4
    add r5, r8, r7
    ps_sub f2, f5, f4
    ps_add f4, f9, f8
    add r4, r8, r6
    ps_add f3, f3, f31
    mtctr r0
@L_801E90A4:
    psq_l f11, 0x20(r10), 0, 0
    ps_msub f2, f2, f27, f8
    psq_l f10, 0x60(r10), 0, 0
    ps_sub f12, f9, f8
    ps_add f1, f3, f2
    psq_l f9, 0xa0(r10), 0, 0
    ps_sub f13, f3, f2
    psq_l f8, 0xe0(r10), 0, 0
    ps_add f3, f9, f10
    ps_sub f9, f9, f10
    addi r10, r10, 0x8
    ps_add f2, f11, f8
    ps_sub f11, f11, f8
    psq_l f7, 0x0(r10), 0, 0
    ps_add f8, f2, f3
    ps_sub f10, f2, f3
    ps_add f3, f9, f11
    psq_l f6, 0x80(r10), 0, 0
    ps_add f2, f4, f8
    ps_mul f3, f3, f28
    psq_l f5, 0x40(r10), 0, 0
    ps_sub f0, f4, f8
    ps_madd f9, f9, f30, f3
    psq_l f4, 0xc0(r10), 0, 0
    ps_sub f9, f9, f8
    addi r7, r7, 0x2
    psq_st f2, 0x0(r5), 0, 6
    ps_msub f11, f11, f29, f3
    ps_add f2, f1, f9
    ps_msub f10, f10, f27, f9
    ps_sub f1, f1, f9
    psq_st f2, 0x8(r5), 0, 6
    ps_add f3, f13, f10
    ps_add f11, f11, f10
    psq_st f3, 0x10(r5), 0, 6
    addi r6, r6, 0x2
    ps_sub f2, f12, f11
    ps_add f3, f12, f11
    psq_st f2, 0x18(r5), 0, 6
    ps_sub f2, f13, f10
    ps_add f9, f7, f6
    psq_st f3, 0x0(r4), 0, 6
    ps_sub f3, f7, f6
    ps_add f9, f9, f31
    psq_st f2, 0x8(r4), 0, 6
    ps_add f8, f5, f4
    ps_sub f2, f5, f4
    psq_st f1, 0x10(r4), 0, 6
    add r5, r8, r7
    ps_add f4, f9, f8
    psq_st f0, 0x18(r4), 0, 6
    ps_add f3, f3, f31
    add r4, r8, r6
    bdnz @L_801E90A4
    psq_l f11, 0x20(r10), 0, 0
    ps_msub f2, f2, f27, f8
    psq_l f10, 0x60(r10), 0, 0
    ps_sub f12, f9, f8
    ps_add f1, f3, f2
    psq_l f9, 0xa0(r10), 0, 0
    ps_sub f13, f3, f2
    psq_l f8, 0xe0(r10), 0, 0
    ps_add f3, f9, f10
    ps_sub f9, f9, f10
    ps_add f2, f11, f8
    ps_sub f11, f11, f8
    ps_add f8, f2, f3
    ps_sub f10, f2, f3
    ps_add f3, f9, f11
    ps_add f2, f4, f8
    ps_mul f3, f3, f28
    ps_sub f0, f4, f8
    ps_madd f9, f9, f30, f3
    psq_st f2, 0x0(r5), 0, 6
    ps_sub f9, f9, f8
    ps_msub f11, f11, f29, f3
    psq_st f0, 0x18(r4), 0, 6
    ps_add f2, f1, f9
    ps_msub f10, f10, f27, f9
    ps_sub f1, f1, f9
    psq_st f2, 0x8(r5), 0, 6
    ps_add f3, f13, f10
    ps_add f11, f11, f10
    psq_st f3, 0x10(r5), 0, 6
    ps_sub f2, f12, f11
    psq_st f1, 0x10(r4), 0, 6
    ps_add f3, f12, f11
    psq_st f2, 0x18(r5), 0, 6
    ps_sub f2, f13, f10
    psq_st f3, 0x0(r4), 0, 6
    psq_st f2, 0x8(r4), 0, 6
    lwz r4, 0x110(r31)
    li r0, 0x140
    lwz r5, -21104(r13)
    stw r4, -21184(r13)
    srwi r3, r3, 1
    subi r9, r31, 0x8
    stw r0, -21152(r13)
    lbz r0, 0x686(r5)
    slwi r0, r0, 8
    add r0, r5, r0
    stw r0, -21120(r13)
    lwz r8, 0x128(r31)
    lwz r7, -21120(r13)
    li r4, 0x8
    mtctr r4
@L_801E924C:
    psq_l f7, 0x0(r8), 0, 5
    psq_l f6, 0x0(r7), 0, 0
    lwz r6, 0xc(r8)
    lwz r0, 0x8(r8)
    ps_mul f7, f7, f6
    lwz r5, 0x4(r8)
    lhz r4, 0x2(r8)
    or. r6, r6, r0
@L_801E926C:
    cmpwi r6, 0x0
    bne @L_801E93B8
    ps_merge00 f4, f7, f7
    cmpwi r5, 0x0
    psq_st f4, 0x8(r9), 0, 0
    bne @L_801E931C
    psq_st f4, 0x10(r9), 0, 0
    cmpwi r4, 0x0
    psq_st f4, 0x18(r9), 0, 0
    bne @L_801E92A8
    addi r7, r7, 0x20
    psq_stu f4, 0x20(r9), 0, 0
    addi r8, r8, 0x10
    bdnz @L_801E924C
    b @L_801E9480
@L_801E92A8:
    addi r8, r8, 0x10
    ps_msub f13, f7, f28, f7
    addi r7, r7, 0x20
    ps_merge00 f2, f7, f7
    lwz r5, 0x4(r8)
    ps_sub f1, f28, f29
    ps_msub f12, f7, f27, f13
    lhz r4, 0x2(r8)
    ps_merge11 f10, f7, f13
    psq_l f6, 0x0(r7), 0, 0
    ps_nmsub f11, f7, f1, f12
    ps_add f8, f2, f10
    psq_l f7, 0x0(r8), 0, 5
    ps_merge11 f9, f12, f11
    ps_sub f10, f2, f10
    lwz r6, 0xc(r8)
    ps_add f3, f2, f9
    lwz r0, 0x8(r8)
    ps_sub f9, f2, f9
    psq_stu f8, 0x8(r9), 0, 0
    ps_merge10 f9, f9, f9
    psq_stu f3, 0x8(r9), 0, 0
    ps_merge10 f10, f10, f10
    or r6, r6, r0
    psq_stu f9, 0x8(r9), 0, 0
    ps_mul f7, f7, f6
    psq_stu f10, 0x8(r9), 0, 0
    bdnz @L_801E926C
    b @L_801E9480
@L_801E931C:
    psq_l f1, 0x4(r8), 0, 5
    psq_l f2, 0x8(r7), 0, 0
    addi r8, r8, 0x10
    ps_mul f1, f1, f2
    addi r7, r7, 0x20
    ps_sub f12, f7, f1
    ps_add f13, f7, f1
    lwz r6, 0xc(r8)
    ps_madd f11, f1, f27, f12
    ps_nmsub f10, f1, f27, f13
    ps_mul f3, f12, f28
    ps_merge00 f11, f13, f11
    lwz r0, 0x8(r8)
    ps_nmsub f9, f1, f30, f3
    ps_merge00 f10, f10, f12
    lwz r5, 0x4(r8)
    ps_sub f9, f9, f13
    ps_nmsub f8, f7, f29, f3
    lhz r4, 0x2(r8)
    ps_merge11 f13, f13, f9
    ps_msub f3, f12, f27, f9
    psq_l f7, 0x0(r8), 0, 5
    ps_add f2, f11, f13
    ps_sub f8, f8, f3
    psq_l f6, 0x0(r7), 0, 0
    ps_merge11 f12, f3, f8
    ps_sub f11, f11, f13
    psq_stu f2, 0x8(r9), 0, 0
    ps_add f4, f10, f12
    ps_sub f1, f10, f12
    or r6, r6, r0
    psq_stu f4, 0x8(r9), 0, 0
    ps_merge10 f1, f1, f1
    ps_merge10 f11, f11, f11
    psq_stu f1, 0x8(r9), 0, 0
    ps_mul f7, f7, f6
    psq_stu f11, 0x8(r9), 0, 0
    bdnz @L_801E926C
    b @L_801E9480
@L_801E93B8:
    psq_l f2, 0x4(r8), 0, 5
    psq_l f10, 0x8(r7), 0, 0
    ps_mul f2, f2, f10
    psq_l f13, 0x8(r8), 0, 5
    psq_l f9, 0x10(r7), 0, 0
    ps_merge01 f4, f7, f2
    psq_l f12, 0xc(r8), 0, 5
    ps_merge01 f1, f2, f7
    psq_l f8, 0x18(r7), 0, 0
    addi r8, r8, 0x10
    ps_madd f11, f13, f9, f4
    ps_nmsub f10, f13, f9, f4
    ps_madd f9, f12, f8, f1
    ps_nmsub f8, f12, f8, f1
    addi r7, r7, 0x20
    ps_add f4, f11, f9
    ps_sub f12, f11, f9
    ps_msub f13, f8, f27, f9
    lwz r6, 0xc(r8)
    ps_sub f3, f8, f10
    ps_add f1, f10, f13
    ps_sub f13, f10, f13
    ps_mul f3, f3, f28
    lwz r0, 0x8(r8)
    ps_merge00 f1, f4, f1
    ps_nmsub f9, f10, f30, f3
    ps_msub f11, f8, f29, f3
    lwz r5, 0x4(r8)
    ps_sub f9, f9, f4
    ps_merge00 f13, f13, f12
    lhz r4, 0x2(r8)
    ps_madd f10, f12, f27, f9
    ps_merge11 f8, f4, f9
    psq_l f7, 0x0(r8), 0, 5
    ps_sub f11, f11, f10
    ps_add f12, f1, f8
    psq_l f6, 0x0(r7), 0, 0
    ps_merge11 f11, f10, f11
    ps_sub f4, f1, f8
    ps_mul f7, f7, f6
    ps_add f10, f13, f11
    ps_sub f9, f13, f11
    ps_merge10 f10, f10, f10
    psq_stu f12, 0x8(r9), 0, 0
    ps_merge10 f4, f4, f4
    psq_stu f9, 0x8(r9), 0, 0
    psq_stu f10, 0x8(r9), 0, 0
    or r6, r6, r0
    psq_stu f4, 0x8(r9), 0, 0
    bdnz @L_801E926C
@L_801E9480:
    lwz r8, -21184(r13)
    mr r9, r31
    lwz r0, -21152(r13)
    psq_l f7, 0x0(r9), 0, 0
    slwi r4, r3, 2
    psq_l f6, 0x80(r9), 0, 0
    slwi r6, r0, 2
    psq_l f5, 0x40(r9), 0, 0
    mr r7, r4
    ps_add f9, f7, f6
    psq_l f4, 0xc0(r9), 0, 0
    ps_sub f3, f7, f6
    add r6, r7, r6
    ps_add f9, f9, f31
    li r0, 0x3
    ps_add f8, f5, f4
    add r5, r8, r7
    ps_sub f2, f5, f4
    ps_add f4, f9, f8
    add r4, r8, r6
    ps_add f3, f3, f31
    mtctr r0
@L_801E94D8:
    psq_l f11, 0x20(r9), 0, 0
    ps_msub f2, f2, f27, f8
    psq_l f10, 0x60(r9), 0, 0
    ps_sub f12, f9, f8
    ps_add f1, f3, f2
    psq_l f9, 0xa0(r9), 0, 0
    ps_sub f13, f3, f2
    psq_l f8, 0xe0(r9), 0, 0
    ps_add f3, f9, f10
    ps_sub f9, f9, f10
    addi r9, r9, 0x8
    ps_add f2, f11, f8
    ps_sub f11, f11, f8
    psq_l f7, 0x0(r9), 0, 0
    ps_add f8, f2, f3
    ps_sub f10, f2, f3
    ps_add f3, f9, f11
    psq_l f6, 0x80(r9), 0, 0
    ps_add f2, f4, f8
    ps_mul f3, f3, f28
    psq_l f5, 0x40(r9), 0, 0
    ps_sub f0, f4, f8
    ps_madd f9, f9, f30, f3
    psq_l f4, 0xc0(r9), 0, 0
    ps_sub f9, f9, f8
    addi r7, r7, 0x2
    psq_st f2, 0x0(r5), 0, 6
    ps_msub f11, f11, f29, f3
    ps_add f2, f1, f9
    ps_msub f10, f10, f27, f9
    ps_sub f1, f1, f9
    psq_st f2, 0x8(r5), 0, 6
    ps_add f3, f13, f10
    ps_add f11, f11, f10
    psq_st f3, 0x10(r5), 0, 6
    addi r6, r6, 0x2
    ps_sub f2, f12, f11
    ps_add f3, f12, f11
    psq_st f2, 0x18(r5), 0, 6
    ps_sub f2, f13, f10
    ps_add f9, f7, f6
    psq_st f3, 0x0(r4), 0, 6
    ps_sub f3, f7, f6
    ps_add f9, f9, f31
    psq_st f2, 0x8(r4), 0, 6
    ps_add f8, f5, f4
    ps_sub f2, f5, f4
    psq_st f1, 0x10(r4), 0, 6
    add r5, r8, r7
    ps_add f4, f9, f8
    psq_st f0, 0x18(r4), 0, 6
    ps_add f3, f3, f31
    add r4, r8, r6
    bdnz @L_801E94D8
    psq_l f11, 0x20(r9), 0, 0
    ps_msub f2, f2, f27, f8
    psq_l f10, 0x60(r9), 0, 0
    ps_sub f12, f9, f8
    ps_add f1, f3, f2
    psq_l f9, 0xa0(r9), 0, 0
    ps_sub f13, f3, f2
    psq_l f8, 0xe0(r9), 0, 0
    ps_add f3, f9, f10
    ps_sub f9, f9, f10
    ps_add f2, f11, f8
    ps_sub f11, f11, f8
    ps_add f8, f2, f3
    ps_sub f10, f2, f3
    ps_add f3, f9, f11
    ps_add f2, f4, f8
    ps_mul f3, f3, f28
    ps_sub f0, f4, f8
    ps_madd f9, f9, f30, f3
    psq_st f2, 0x0(r5), 0, 6
    ps_sub f9, f9, f8
    ps_msub f11, f11, f29, f3
    psq_st f0, 0x18(r4), 0, 6
    ps_add f2, f1, f9
    ps_msub f10, f10, f27, f9
    ps_sub f1, f1, f9
    psq_st f2, 0x8(r5), 0, 6
    ps_add f3, f13, f10
    ps_add f11, f11, f10
    psq_st f1, 0x10(r4), 0, 6
    psq_st f3, 0x10(r5), 0, 6
    ps_sub f2, f12, f11
    ps_add f3, f12, f11
    psq_st f2, 0x18(r5), 0, 6
    ps_sub f2, f13, f10
    psq_st f3, 0x0(r4), 0, 6
    psq_st f2, 0x8(r4), 0, 6
    lwz r0, 0x114(r31)
    lwz r4, -21104(r13)
    subi r8, r31, 0x8
    stw r0, -21184(r13)
    lbz r0, 0x68c(r4)
    slwi r0, r0, 8
    add r0, r4, r0
    stw r0, -21120(r13)
    lwz r9, 0x12c(r31)
    lwz r7, -21120(r13)
    li r4, 0x8
    mtctr r4
@L_801E9674:
    psq_l f7, 0x0(r9), 0, 5
    psq_l f6, 0x0(r7), 0, 0
    lwz r6, 0xc(r9)
    lwz r0, 0x8(r9)
    ps_mul f7, f7, f6
    lwz r5, 0x4(r9)
    lhz r4, 0x2(r9)
    or. r6, r6, r0
@L_801E9694:
    cmpwi r6, 0x0
    bne @L_801E97E0
    ps_merge00 f4, f7, f7
    cmpwi r5, 0x0
    psq_st f4, 0x8(r8), 0, 0
    bne @L_801E9744
    psq_st f4, 0x10(r8), 0, 0
    cmpwi r4, 0x0
    psq_st f4, 0x18(r8), 0, 0
    bne @L_801E96D0
    addi r7, r7, 0x20
    psq_stu f4, 0x20(r8), 0, 0
    addi r9, r9, 0x10
    bdnz @L_801E9674
    b @L_801E98A8
@L_801E96D0:
    addi r9, r9, 0x10
    ps_msub f13, f7, f28, f7
    addi r7, r7, 0x20
    ps_merge00 f2, f7, f7
    lwz r5, 0x4(r9)
    ps_sub f1, f28, f29
    ps_msub f12, f7, f27, f13
    lhz r4, 0x2(r9)
    ps_merge11 f10, f7, f13
    psq_l f6, 0x0(r7), 0, 0
    ps_nmsub f11, f7, f1, f12
    ps_add f8, f2, f10
    psq_l f7, 0x0(r9), 0, 5
    ps_merge11 f9, f12, f11
    ps_sub f10, f2, f10
    lwz r6, 0xc(r9)
    ps_add f3, f2, f9
    lwz r0, 0x8(r9)
    ps_sub f9, f2, f9
    psq_stu f8, 0x8(r8), 0, 0
    ps_merge10 f9, f9, f9
    psq_stu f3, 0x8(r8), 0, 0
    ps_merge10 f10, f10, f10
    or r6, r6, r0
    psq_stu f9, 0x8(r8), 0, 0
    ps_mul f7, f7, f6
    psq_stu f10, 0x8(r8), 0, 0
    bdnz @L_801E9694
    b @L_801E98A8
@L_801E9744:
    psq_l f1, 0x4(r9), 0, 5
    psq_l f2, 0x8(r7), 0, 0
    addi r9, r9, 0x10
    ps_mul f1, f1, f2
    addi r7, r7, 0x20
    ps_sub f12, f7, f1
    ps_add f13, f7, f1
    lwz r6, 0xc(r9)
    ps_madd f11, f1, f27, f12
    ps_nmsub f10, f1, f27, f13
    ps_mul f3, f12, f28
    ps_merge00 f11, f13, f11
    lwz r0, 0x8(r9)
    ps_nmsub f9, f1, f30, f3
    ps_merge00 f10, f10, f12
    lwz r5, 0x4(r9)
    ps_sub f9, f9, f13
    ps_nmsub f8, f7, f29, f3
    lhz r4, 0x2(r9)
    ps_merge11 f13, f13, f9
    ps_msub f3, f12, f27, f9
    psq_l f7, 0x0(r9), 0, 5
    ps_add f2, f11, f13
    ps_sub f8, f8, f3
    psq_l f6, 0x0(r7), 0, 0
    ps_merge11 f12, f3, f8
    ps_sub f11, f11, f13
    psq_stu f2, 0x8(r8), 0, 0
    ps_add f4, f10, f12
    ps_sub f1, f10, f12
    or r6, r6, r0
    psq_stu f4, 0x8(r8), 0, 0
    ps_merge10 f1, f1, f1
    ps_merge10 f11, f11, f11
    psq_stu f1, 0x8(r8), 0, 0
    ps_mul f7, f7, f6
    psq_stu f11, 0x8(r8), 0, 0
    bdnz @L_801E9694
    b @L_801E98A8
@L_801E97E0:
    psq_l f2, 0x4(r9), 0, 5
    psq_l f10, 0x8(r7), 0, 0
    ps_mul f2, f2, f10
    psq_l f13, 0x8(r9), 0, 5
    psq_l f9, 0x10(r7), 0, 0
    ps_merge01 f4, f7, f2
    psq_l f12, 0xc(r9), 0, 5
    ps_merge01 f1, f2, f7
    psq_l f8, 0x18(r7), 0, 0
    addi r9, r9, 0x10
    ps_madd f11, f13, f9, f4
    ps_nmsub f10, f13, f9, f4
    ps_madd f9, f12, f8, f1
    ps_nmsub f8, f12, f8, f1
    addi r7, r7, 0x20
    ps_add f4, f11, f9
    ps_sub f12, f11, f9
    ps_msub f13, f8, f27, f9
    lwz r6, 0xc(r9)
    ps_sub f3, f8, f10
    ps_add f1, f10, f13
    ps_sub f13, f10, f13
    ps_mul f3, f3, f28
    lwz r0, 0x8(r9)
    ps_merge00 f1, f4, f1
    ps_nmsub f9, f10, f30, f3
    ps_msub f11, f8, f29, f3
    lwz r5, 0x4(r9)
    ps_sub f9, f9, f4
    ps_merge00 f13, f13, f12
    lhz r4, 0x2(r9)
    ps_madd f10, f12, f27, f9
    ps_merge11 f8, f4, f9
    psq_l f7, 0x0(r9), 0, 5
    ps_sub f11, f11, f10
    ps_add f12, f1, f8
    psq_l f6, 0x0(r7), 0, 0
    ps_merge11 f11, f10, f11
    ps_sub f4, f1, f8
    ps_mul f7, f7, f6
    ps_add f10, f13, f11
    ps_sub f9, f13, f11
    ps_merge10 f10, f10, f10
    psq_stu f12, 0x8(r8), 0, 0
    ps_merge10 f4, f4, f4
    psq_stu f9, 0x8(r8), 0, 0
    psq_stu f10, 0x8(r8), 0, 0
    or r6, r6, r0
    psq_stu f4, 0x8(r8), 0, 0
    bdnz @L_801E9694
@L_801E98A8:
    lwz r7, -21184(r13)
    mr r8, r31
    lwz r0, -21152(r13)
    psq_l f7, 0x0(r8), 0, 0
    slwi r3, r3, 2
    psq_l f6, 0x80(r8), 0, 0
    slwi r5, r0, 2
    psq_l f5, 0x40(r8), 0, 0
    mr r6, r3
    ps_add f9, f7, f6
    psq_l f4, 0xc0(r8), 0, 0
    ps_sub f3, f7, f6
    add r5, r6, r5
    ps_add f9, f9, f31
    li r0, 0x3
    ps_add f8, f5, f4
    add r4, r7, r6
    ps_sub f2, f5, f4
    ps_add f4, f9, f8
    add r3, r7, r5
    ps_add f3, f3, f31
    mtctr r0
@L_801E9900:
    psq_l f11, 0x20(r8), 0, 0
    ps_msub f2, f2, f27, f8
    psq_l f10, 0x60(r8), 0, 0
    ps_sub f12, f9, f8
    ps_add f1, f3, f2
    psq_l f9, 0xa0(r8), 0, 0
    ps_sub f13, f3, f2
    psq_l f8, 0xe0(r8), 0, 0
    ps_add f3, f9, f10
    ps_sub f9, f9, f10
    addi r8, r8, 0x8
    ps_add f2, f11, f8
    ps_sub f11, f11, f8
    psq_l f7, 0x0(r8), 0, 0
    ps_add f8, f2, f3
    ps_sub f10, f2, f3
    ps_add f3, f9, f11
    psq_l f6, 0x80(r8), 0, 0
    ps_add f2, f4, f8
    ps_mul f3, f3, f28
    psq_l f5, 0x40(r8), 0, 0
    ps_sub f0, f4, f8
    ps_madd f9, f9, f30, f3
    psq_l f4, 0xc0(r8), 0, 0
    ps_sub f9, f9, f8
    addi r6, r6, 0x2
    psq_st f2, 0x0(r4), 0, 6
    ps_msub f11, f11, f29, f3
    ps_add f2, f1, f9
    ps_msub f10, f10, f27, f9
    ps_sub f1, f1, f9
    psq_st f2, 0x8(r4), 0, 6
    ps_add f3, f13, f10
    ps_add f11, f11, f10
    psq_st f3, 0x10(r4), 0, 6
    addi r5, r5, 0x2
    ps_sub f2, f12, f11
    ps_add f3, f12, f11
    psq_st f2, 0x18(r4), 0, 6
    ps_sub f2, f13, f10
    ps_add f9, f7, f6
    psq_st f3, 0x0(r3), 0, 6
    ps_sub f3, f7, f6
    ps_add f9, f9, f31
    psq_st f2, 0x8(r3), 0, 6
    ps_add f8, f5, f4
    ps_sub f2, f5, f4
    psq_st f1, 0x10(r3), 0, 6
    add r4, r7, r6
    ps_add f4, f9, f8
    psq_st f0, 0x18(r3), 0, 6
    ps_add f3, f3, f31
    add r3, r7, r5
    bdnz @L_801E9900
    psq_l f11, 0x20(r8), 0, 0
    ps_msub f2, f2, f27, f8
    psq_l f10, 0x60(r8), 0, 0
    ps_sub f12, f9, f8
    ps_add f1, f3, f2
    psq_l f9, 0xa0(r8), 0, 0
    ps_sub f13, f3, f2
    psq_l f8, 0xe0(r8), 0, 0
    ps_add f3, f9, f10
    ps_sub f9, f9, f10
    ps_add f2, f11, f8
    ps_sub f11, f11, f8
    ps_add f8, f2, f3
    ps_sub f10, f2, f3
    ps_add f3, f9, f11
    ps_add f2, f4, f8
    ps_mul f3, f3, f28
    ps_sub f0, f4, f8
    ps_madd f9, f9, f30, f3
    psq_st f2, 0x0(r4), 0, 6
    ps_sub f9, f9, f8
    ps_msub f11, f11, f29, f3
    psq_st f0, 0x18(r3), 0, 6
    ps_add f2, f1, f9
    ps_msub f10, f10, f27, f9
    ps_sub f1, f1, f9
    psq_st f2, 0x8(r4), 0, 6
    ps_add f3, f13, f10
    ps_add f11, f11, f10
    psq_st f1, 0x10(r3), 0, 6
    psq_st f3, 0x10(r4), 0, 6
    ps_sub f2, f12, f11
    ps_add f3, f12, f11
    psq_st f2, 0x18(r4), 0, 6
    ps_sub f2, f13, f10
    psq_st f3, 0x0(r3), 0, 6
    psq_st f2, 0x8(r3), 0, 6
    lwz r4, -21104(r13)
    lbz r0, 0x6a9(r4)
    cmplwi r0, 0x0
    beq @L_801E9AEC
    lhz r3, 0x6ac(r4)
    subi r0, r3, 0x1
    sth r0, 0x6ac(r4)
    lwz r3, -21104(r13)
    lhz r0, 0x6ac(r3)
    cmplwi r0, 0x0
    bne @L_801E9AEC
    lhz r0, 0x6aa(r3)
    sth r0, 0x6ac(r3)
    lwz r4, -21104(r13)
    lwz r3, 0x6a4(r4)
    addi r0, r3, 0x6
    clrrwi r3, r0, 3
    addi r0, r3, 0x1
    stw r0, 0x6a4(r4)
    lwz r3, -21104(r13)
    lwz r0, 0x6a4(r3)
    cmplwi r0, 0x20
    ble @L_801E9AD0
    li r0, 0x21
    stw r0, 0x6a4(r3)
@L_801E9AD0:
    lwz r3, -21104(r13)
    li r0, 0x0
    sth r0, 0x684(r3)
    lwz r3, -21104(r13)
    sth r0, 0x68a(r3)
    lwz r3, -21104(r13)
    sth r0, 0x690(r3)
@L_801E9AEC:
    addi r30, r30, 0x1
@L_801E9AF0:
    lwz r3, -21104(r13)
    clrlwi r4, r30, 24
    lhz r0, 0x696(r3)
    cmpw r4, r0
    blt @L_801E8160
    lwz r3, 0x6b0(r3)
    li r5, 0x2800
    lwz r4, 0x10c(r31)
    bl fn_8009B55C
    lwz r3, -21104(r13)
    li r5, 0xa00
    lwz r4, 0x110(r31)
    lwz r3, 0x6b4(r3)
    bl fn_8009B55C
    lwz r3, -21104(r13)
    li r5, 0xa00
    lwz r4, 0x114(r31)
    lwz r3, 0x6b8(r3)
    bl fn_8009B55C
    lwz r4, -21104(r13)
    lwz r3, 0x6b0(r4)
    addi r0, r3, 0x2800
    stw r0, 0x6b0(r4)
    lwz r4, -21104(r13)
    lwz r3, 0x6b4(r4)
    addi r0, r3, 0xa00
    stw r0, 0x6b4(r4)
    lwz r4, -21104(r13)
    lwz r3, 0x6b8(r4)
    addi r0, r3, 0xa00
    stw r0, 0x6b8(r4)
    lwz r0, 0x3c(r1)
    lfd f31, 0x30(r1)
    lfd f30, 0x28(r1)
    lfd f29, 0x20(r1)
    lfd f28, 0x18(r1)
    lfd f27, 0x10(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    addi r1, r1, 0x38
    mtlr r0
    blr
