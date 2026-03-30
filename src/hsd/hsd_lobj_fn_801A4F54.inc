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
    stmw r25, 0xa4(r1)
    li r6, 0x0
    li r5, 0x0
    li r4, 0x0
    li r0, 0x0
    addi r3, r3, 0x54
    stw r6, lbl_8047B2BC(r13)
    mr r31, r3
    li r30, 0x0
    stw r5, lbl_8047B2C0(r13)
    li r6, 0x0
    stw r4, lbl_8047B2C4(r13)
    stw r0, lbl_8047B2C8(r13)
    b @L_801A4FCC
    @L_801A4FB0:
    lis r3, lbl_804655E0@ha
    slwi r4, r6, 2
    addi r0, r3, lbl_804655E0@l
    li r5, 0x0
    add r3, r0, r4
    addi r6, r6, 0x1
    stw r5, 0x0(r3)
    @L_801A4FCC:
    cmpwi r6, 0x9
    blt @L_801A4FB0
    li r0, 0x0
    lwz r27, lbl_8047B2B4(r13)
    stw r0, lbl_8047B2B8(r13)
    b @L_801A57D0
    @L_801A4FE4:
    lwz r28, 0x4(r27)
    cmplwi r28, 0x0
    beq @L_801A57CC
    lhz r0, 0x8(r28)
    rlwinm. r0, r0, 0, 26, 26
    bne @L_801A57CC
    lhz r0, 0x8(r28)
    lhz r3, 0x8(r28)
    rlwinm. r29, r0, 0, 27, 29
    clrlwi r0, r3, 30
    beq @L_801A57CC
    lhz r3, 0x8(r28)
    clrlwi. r3, r3, 30
    bne @L_801A5040
    li r5, 0x8
    lis r3, lbl_804655E0@ha
    slwi r4, r5, 2
    addi r3, r3, lbl_804655E0@l
    add r3, r3, r4
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    beq @L_801A504C
    b @L_801A50D0
    @L_801A5040:
    lwz r5, lbl_8047B2B8(r13)
    addi r3, r5, 0x1
    stw r3, lbl_8047B2B8(r13)
    @L_801A504C:
    lis r3, lbl_804655E0@ha
    slwi r4, r5, 2
    addi r3, r3, lbl_804655E0@l
    cmplwi r5, 0x8
    add r3, r3, r4
    stw r28, 0x0(r3)
    bgt @L_801A50C8
    lis r4, jumptable_8036CAD0@ha
    slwi r3, r5, 2
    addi r4, r4, jumptable_8036CAD0@l
    lwzx r4, r4, r3
    mtctr r4
    bctr
    li r3, 0x1
    b @L_801A50CC
    li r3, 0x2
    b @L_801A50CC
    li r3, 0x4
    b @L_801A50CC
    li r3, 0x8
    b @L_801A50CC
    li r3, 0x10
    b @L_801A50CC
    li r3, 0x20
    b @L_801A50CC
    li r3, 0x40
    b @L_801A50CC
    li r3, 0x80
    b @L_801A50CC
    li r3, 0x100
    b @L_801A50CC
    @L_801A50C8:
    li r3, 0x0
    @L_801A50CC:
    stw r3, 0x4c(r28)
    @L_801A50D0:
    lwz r30, lbl_8047B2B8(r13)
    li r3, 0x0
    cmpwi r0, 0x2
    stw r3, 0x90(r28)
    beq @L_801A51C8
    bge @L_801A50F8
    cmpwi r0, 0x0
    beq @L_801A57CC
    bge @L_801A5104
    b @L_801A54E8
    @L_801A50F8:
    cmpwi r0, 0x4
    bge @L_801A54E8
    b @L_801A52B4
    @L_801A5104:
    cmplwi r28, 0x0
    beq @L_801A5124
    lwz r0, 0x18(r28)
    cmplwi r0, 0x0
    beq @L_801A5124
    lwz r3, 0x18(r28)
    addi r4, r1, 0x8c
    bl fn_80191688
    @L_801A5124:
    lfs f1, 0x8c(r1)
    mr r3, r31
    lfs f0, lbl_8047DBE0(r2)
    addi r4, r1, 0x8c
    lfs f3, 0x90(r1)
    addi r5, r1, 0x8c
    lfs f2, lbl_8047DBE0(r2)
    fmuls f4, f1, f0
    lfs f1, 0x94(r1)
    lfs f0, lbl_8047DBE0(r2)
    fmuls f2, f3, f2
    stfs f4, 0x8c(r1)
    fmuls f0, f1, f0
    stfs f2, 0x90(r1)
    stfs f0, 0x94(r1)
    bl fn_800A37CC
    lhz r0, 0x8(r28)
    rlwinm. r0, r0, 0, 29, 29
    beq @L_801A51A4
    lfs f1, 0x8c(r1)
    addi r3, r28, 0x50
    lfs f2, 0x90(r1)
    lfs f3, 0x94(r1)
    bl fn_800BA414
    lfs f1, lbl_8047DBE4(r2)
    addi r3, r28, 0x50
    lfs f2, lbl_8047DBE8(r2)
    lfs f3, lbl_8047DBE8(r2)
    lfs f4, lbl_8047DBE4(r2)
    lfs f5, lbl_8047DBE8(r2)
    lfs f6, lbl_8047DBE8(r2)
    bl fn_800BA198
    @L_801A51A4:
    lhz r0, 0x8(r28)
    rlwinm. r0, r0, 0, 28, 28
    beq @L_801A54E8
    lfs f1, 0x8c(r1)
    addi r3, r28, 0x94
    lfs f2, 0x90(r1)
    lfs f3, 0x94(r1)
    bl fn_800BA414
    b @L_801A54E8
    @L_801A51C8:
    lwz r0, 0x10(r28)
    addi r4, r1, 0x18
    addi r3, r28, 0x50
    stw r0, 0x18(r1)
    bl fn_800BA440
    lwz r0, 0x10(r28)
    cmplwi r28, 0x0
    stw r0, 0x14(r28)
    beq @L_801A5204
    lwz r0, 0x18(r28)
    cmplwi r0, 0x0
    beq @L_801A5204
    lwz r3, 0x18(r28)
    addi r4, r1, 0x80
    bl fn_80191688
    @L_801A5204:
    mr r3, r31
    addi r4, r1, 0x80
    addi r5, r1, 0x80
    bl fn_800A37CC
    lfs f1, 0x80(r1)
    addi r3, r28, 0x50
    lfs f2, 0x84(r1)
    lfs f3, 0x88(r1)
    bl fn_800BA414
    lfs f1, 0x80(r1)
    addi r3, r28, 0x94
    lfs f2, 0x84(r1)
    lfs f3, 0x88(r1)
    bl fn_800BA414
    lhz r0, 0x8(r28)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A526C
    lfs f1, lbl_8047DBE4(r2)
    addi r3, r28, 0x50
    lfs f2, lbl_8047DBE8(r2)
    lfs f3, lbl_8047DBE8(r2)
    lfs f4, 0x2c(r28)
    lfs f5, 0x30(r28)
    lfs f6, 0x34(r28)
    bl fn_800BA198
    b @L_801A54E8
    @L_801A526C:
    lfs f30, 0x28(r28)
    addi r3, r28, 0x50
    lfs f31, 0x2c(r28)
    lwz r26, 0x30(r28)
    fmr f2, f30
    fmr f1, f31
    mr r4, r26
    bl fn_800BA344
    lfs f1, lbl_8047DBE8(r2)
    addi r3, r28, 0x50
    li r4, 0x0
    bl fn_800BA1B4
    fmr f1, f31
    mr r4, r26
    fmr f2, f30
    addi r3, r28, 0x94
    bl fn_800BA344
    b @L_801A54E8
    @L_801A52B4:
    cmplwi r28, 0x0
    beq @L_801A52D4
    lwz r0, 0x18(r28)
    cmplwi r0, 0x0
    beq @L_801A52D4
    lwz r3, 0x18(r28)
    addi r4, r1, 0x68
    bl fn_80191688
    @L_801A52D4:
    mr r3, r31
    addi r4, r1, 0x68
    addi r5, r1, 0x68
    bl fn_800A37CC
    lis r4, lbl_80274D58@ha
    lwzu r8, lbl_80274D58@l(r4)
    lis r3, lbl_80274D64@ha
    cmplwi r28, 0x0
    addi r5, r3, lbl_80274D64@l
    lwz r7, 0x4(r4)
    lwz r6, 0x8(r4)
    lwz r4, 0x0(r5)
    lwz r3, 0x4(r5)
    lwz r0, 0x8(r5)
    stw r8, 0x50(r1)
    stw r7, 0x54(r1)
    stw r6, 0x58(r1)
    stw r4, 0x5c(r1)
    stw r3, 0x60(r1)
    stw r0, 0x64(r1)
    beq @L_801A5404
    cmplwi r28, 0x0
    beq @L_801A5348
    lwz r0, 0x18(r28)
    cmplwi r0, 0x0
    beq @L_801A5348
    lwz r3, 0x18(r28)
    addi r4, r1, 0x50
    bl fn_80191688
    @L_801A5348:
    cmplwi r28, 0x0
    beq @L_801A5368
    lwz r0, 0x1c(r28)
    cmplwi r0, 0x0
    beq @L_801A5368
    lwz r3, 0x1c(r28)
    addi r4, r1, 0x5c
    bl fn_80191688
    @L_801A5368:
    addi r3, r1, 0x5c
    addi r4, r1, 0x50
    addi r5, r1, 0x74
    bl fn_800A3A9C
    lfs f0, 0x74(r1)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A53D4
    lfs f0, 0x78(r1)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A53D4
    lfs f0, 0x7c(r1)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A53D4
    li r0, -0x1
    b @L_801A53E4
    @L_801A53D4:
    addi r3, r1, 0x74
    addi r4, r1, 0x74
    bl fn_800A3ADC
    li r0, 0x0
    @L_801A53E4:
    cmpwi r0, 0x0
    beq @L_801A5404
    lfs f2, lbl_8047DBE8(r2)
    lfs f1, lbl_8047DBE8(r2)
    lfs f0, lbl_8047DBE4(r2)
    stfs f2, 0x74(r1)
    stfs f1, 0x78(r1)
    stfs f0, 0x7c(r1)
    @L_801A5404:
    mr r3, r31
    addi r4, r1, 0x74
    addi r5, r1, 0x74
    bl fn_800A3820
    addi r3, r1, 0x74
    addi r4, r1, 0x74
    bl fn_800A3ADC
    lfs f1, 0x68(r1)
    addi r3, r28, 0x50
    lfs f2, 0x6c(r1)
    lfs f3, 0x70(r1)
    bl fn_800BA414
    lfs f1, 0x68(r1)
    addi r3, r28, 0x94
    lfs f2, 0x6c(r1)
    lfs f3, 0x70(r1)
    bl fn_800BA414
    lfs f1, 0x74(r1)
    addi r3, r28, 0x50
    lfs f2, 0x78(r1)
    lfs f3, 0x7c(r1)
    bl fn_800BA424
    lhz r0, 0x8(r28)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A548C
    lfs f1, 0x20(r28)
    addi r3, r28, 0x50
    lfs f2, 0x24(r28)
    lfs f3, 0x28(r28)
    lfs f4, 0x2c(r28)
    lfs f5, 0x30(r28)
    lfs f6, 0x34(r28)
    bl fn_800BA198
    b @L_801A54E8
    @L_801A548C:
    lfs f31, 0x2c(r28)
    lfd f0, lbl_8047DBF0(r2)
    lfs f29, 0x28(r28)
    fcmpo cr0, f31, f0
    lfs f30, 0x20(r28)
    lwz r25, 0x24(r28)
    lwz r26, 0x30(r28)
    bge @L_801A54B0
    lfs f31, lbl_8047DBEC(r2)
    @L_801A54B0:
    fmr f1, f31
    mr r4, r26
    fmr f2, f29
    addi r3, r28, 0x50
    bl fn_800BA344
    fmr f1, f31
    mr r4, r26
    fmr f2, f29
    addi r3, r28, 0x94
    bl fn_800BA344
    fmr f1, f30
    mr r4, r25
    addi r3, r28, 0x50
    bl fn_800BA1B4
    @L_801A54E8:
    andi. r0, r29, 0x14
    beq @L_801A5594
    lwz r0, 0x10(r28)
    addi r4, r1, 0x14
    addi r3, r28, 0x50
    stw r0, 0x14(r1)
    bl fn_800BA440
    lwz r0, 0x10(r28)
    stw r0, 0x14(r28)
    lhz r0, 0x8(r28)
    ori r0, r0, 0x80
    sth r0, 0x8(r28)
    lhz r0, 0x8(r28)
    clrlwi r0, r0, 30
    cmpwi r0, 0x1
    beq @L_801A5558
    blt @L_801A5548
    cmpwi r0, 0x4
    bge @L_801A5548
    lwz r3, lbl_8047B2C4(r13)
    lwz r0, 0x4c(r28)
    or r0, r3, r0
    stw r0, lbl_8047B2C4(r13)
    b @L_801A5558
    @L_801A5548:
    la r3, lbl_8047DBB8(r2)
    li r4, 0x298
    la r5, lbl_8047DBD4(r2)
    bl fn_80196E10
    @L_801A5558:
    lhz r0, 0x8(r28)
    rlwinm. r0, r0, 0, 29, 29
    beq @L_801A5574
    lwz r3, lbl_8047B2BC(r13)
    lwz r0, 0x4c(r28)
    or r0, r3, r0
    stw r0, lbl_8047B2BC(r13)
    @L_801A5574:
    lhz r0, 0x8(r28)
    rlwinm. r0, r0, 0, 27, 27
    beq @L_801A57CC
    lwz r3, lbl_8047B2C8(r13)
    lwz r0, 0x4c(r28)
    or r0, r3, r0
    stw r0, lbl_8047B2C8(r13)
    b @L_801A57CC
    @L_801A5594:
    rlwinm. r0, r29, 0, 28, 28
    beq @L_801A57CC
    lwz r29, 0x4c(r28)
    cmpwi r29, 0x0
    stw r29, 0x90(r28)
    beq @L_801A57CC
    lwz r0, 0x10(r28)
    addi r4, r1, 0x10
    addi r3, r28, 0x94
    stw r0, 0x10(r1)
    bl fn_800BA440
    lfs f0, lbl_8047DBF8(r2)
    addi r3, r28, 0x94
    lfs f1, lbl_8047DBFC(r2)
    stfs f0, 0x38(r28)
    lfs f2, lbl_8047DBFC(r2)
    lfs f4, 0x38(r28)
    lfs f3, 0x38(r28)
    lfs f0, lbl_8047DBE4(r2)
    fmuls f4, f4, f1
    lfs f1, lbl_8047DBE8(r2)
    fnmsubs f6, f3, f2, f0
    lfs f2, lbl_8047DBE8(r2)
    lfs f3, lbl_8047DBE4(r2)
    lfs f5, lbl_8047DBE8(r2)
    bl fn_800BA198
    lhz r0, 0x8(r28)
    clrlwi r0, r0, 30
    cmpwi r0, 0x1
    beq @L_801A564C
    blt @L_801A57A4
    cmpwi r0, 0x4
    bge @L_801A57A4
    cmplwi r28, 0x0
    beq @L_801A5638
    lwz r0, 0x18(r28)
    cmplwi r0, 0x0
    beq @L_801A5638
    lwz r3, 0x18(r28)
    addi r4, r28, 0x3c
    bl fn_80191688
    @L_801A5638:
    mr r3, r31
    addi r4, r28, 0x3c
    addi r5, r28, 0x3c
    bl fn_800A37CC
    b @L_801A57B4
    @L_801A564C:
    lis r4, lbl_80274D58@ha
    lwzu r8, lbl_80274D58@l(r4)
    lis r3, lbl_80274D64@ha
    cmplwi r28, 0x0
    addi r5, r3, lbl_80274D64@l
    lwz r7, 0x4(r4)
    lwz r6, 0x8(r4)
    lwz r4, 0x0(r5)
    lwz r3, 0x4(r5)
    lwz r0, 0x8(r5)
    stw r8, 0x38(r1)
    stw r7, 0x3c(r1)
    stw r6, 0x40(r1)
    stw r4, 0x44(r1)
    stw r3, 0x48(r1)
    stw r0, 0x4c(r1)
    beq @L_801A5784
    cmplwi r28, 0x0
    beq @L_801A56B0
    lwz r0, 0x18(r28)
    cmplwi r0, 0x0
    beq @L_801A56B0
    lwz r3, 0x18(r28)
    addi r4, r1, 0x38
    bl fn_80191688
    @L_801A56B0:
    cmplwi r28, 0x0
    beq @L_801A56D0
    lwz r0, 0x1c(r28)
    cmplwi r0, 0x0
    beq @L_801A56D0
    lwz r3, 0x1c(r28)
    addi r4, r1, 0x44
    bl fn_80191688
    @L_801A56D0:
    addi r3, r1, 0x44
    addi r4, r1, 0x38
    addi r5, r28, 0x3c
    bl fn_800A3A9C
    addic. r0, r28, 0x3c
    beq @L_801A56F0
    addic. r0, r28, 0x3c
    bne @L_801A56F8
    @L_801A56F0:
    li r0, -0x1
    b @L_801A5764
    @L_801A56F8:
    lfs f0, 0x3c(r28)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A5754
    lfs f0, 0x40(r28)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A5754
    lfs f0, 0x44(r28)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A5754
    li r0, -0x1
    b @L_801A5764
    @L_801A5754:
    addi r3, r28, 0x3c
    addi r4, r28, 0x3c
    bl fn_800A3ADC
    li r0, 0x0
    @L_801A5764:
    cmpwi r0, 0x0
    beq @L_801A5784
    lfs f0, lbl_8047DBE8(r2)
    lfs f1, lbl_8047DBE8(r2)
    stfs f0, 0x3c(r28)
    lfs f0, lbl_8047DBE4(r2)
    stfs f1, 0x40(r28)
    stfs f0, 0x44(r28)
    @L_801A5784:
    mr r3, r31
    addi r4, r28, 0x3c
    addi r5, r28, 0x3c
    bl fn_800A3820
    addi r3, r28, 0x3c
    addi r4, r28, 0x3c
    bl fn_800A3ADC
    b @L_801A57B4
    @L_801A57A4:
    la r3, lbl_8047DBB8(r2)
    li r4, 0x2c0
    la r5, lbl_8047DBD4(r2)
    bl fn_80196E10
    @L_801A57B4:
    lhz r0, 0x8(r28)
    ori r0, r0, 0x100
    sth r0, 0x8(r28)
    lwz r0, lbl_8047B2C0(r13)
    or r0, r0, r29
    stw r0, lbl_8047B2C0(r13)
    @L_801A57CC:
    lwz r27, 0x0(r27)
    @L_801A57D0:
    cmpwi r30, 0x8
    bge @L_801A57E0
    cmplwi r27, 0x0
    bne @L_801A4FE4
    @L_801A57E0:
    li r0, 0x8
    cmpwi r0, 0x0
    blt @L_801A580C
    cmpwi r0, 0x9
    bge @L_801A580C
    lis r3, lbl_804655E0@ha
    slwi r4, r0, 2
    addi r0, r3, lbl_804655E0@l
    add r3, r0, r4
    lwz r0, 0x0(r3)
    b @L_801A5810
    @L_801A580C:
    li r0, 0x0
    @L_801A5810:
    cmplwi r0, 0x0
    bne @L_801A591C
    b @L_801A5914
    @L_801A581C:
    lwz r3, 0x4(r27)
    cmplwi r3, 0x0
    beq @L_801A5910
    lhz r0, 0x8(r3)
    rlwinm. r0, r0, 0, 26, 26
    bne @L_801A5910
    lhz r0, 0x8(r3)
    clrlwi. r0, r0, 30
    bne @L_801A5910
    lhz r0, 0x8(r3)
    andi. r0, r0, 0x14
    beq @L_801A5910
    lhz r0, 0x8(r3)
    clrlwi. r0, r0, 30
    bne @L_801A587C
    li r6, 0x8
    lis r4, lbl_804655E0@ha
    slwi r5, r6, 2
    addi r0, r4, lbl_804655E0@l
    add r4, r0, r5
    lwz r0, 0x0(r4)
    cmplwi r0, 0x0
    beq @L_801A5888
    b @L_801A591C
    @L_801A587C:
    lwz r6, lbl_8047B2B8(r13)
    addi r0, r6, 0x1
    stw r0, lbl_8047B2B8(r13)
    @L_801A5888:
    lis r4, lbl_804655E0@ha
    slwi r5, r6, 2
    addi r0, r4, lbl_804655E0@l
    cmplwi r6, 0x8
    add r4, r0, r5
    stw r3, 0x0(r4)
    bgt @L_801A5904
    lis r4, jumptable_8036CAAC@ha
    slwi r0, r6, 2
    addi r4, r4, jumptable_8036CAAC@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    li r0, 0x1
    b @L_801A5908
    li r0, 0x2
    b @L_801A5908
    li r0, 0x4
    b @L_801A5908
    li r0, 0x8
    b @L_801A5908
    li r0, 0x10
    b @L_801A5908
    li r0, 0x20
    b @L_801A5908
    li r0, 0x40
    b @L_801A5908
    li r0, 0x80
    b @L_801A5908
    li r0, 0x100
    b @L_801A5908
    @L_801A5904:
    li r0, 0x0
    @L_801A5908:
    stw r0, 0x4c(r3)
    b @L_801A591C
    @L_801A5910:
    lwz r27, 0x0(r27)
    @L_801A5914:
    cmplwi r27, 0x0
    bne @L_801A581C
    @L_801A591C:
    lwz r27, lbl_8047B2B8(r13)
    li r28, 0x0
    b @L_801A5C14
    @L_801A5928:
    cmpwi r28, 0x0
    blt @L_801A5950
    cmpwi r28, 0x8
    bge @L_801A5950
    lis r3, lbl_804655E0@ha
    slwi r4, r28, 2
    addi r0, r3, lbl_804655E0@l
    add r3, r0, r4
    lwz r0, 0x0(r3)
    b @L_801A5954
    @L_801A5950:
    li r0, 0x0
    @L_801A5954:
    mr. r26, r0
    beq @L_801A5C10
    lhz r3, 0x8(r26)
    rlwinm. r0, r3, 0, 28, 28
    beq @L_801A5C10
    andi. r0, r3, 0x14
    beq @L_801A5C10
    mr r0, r30
    addi r30, r30, 0x1
    cmplwi r0, 0x8
    bgt @L_801A59E0
    lis r3, jumptable_8036CA88@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_8036CA88@l
    lwzx r3, r3, r0
    mtctr r3
    bctr
    li r29, 0x1
    b @L_801A59E4
    li r29, 0x2
    b @L_801A59E4
    li r29, 0x4
    b @L_801A59E4
    li r29, 0x8
    b @L_801A59E4
    li r29, 0x10
    b @L_801A59E4
    li r29, 0x20
    b @L_801A59E4
    li r29, 0x40
    b @L_801A59E4
    li r29, 0x80
    b @L_801A59E4
    li r29, 0x100
    b @L_801A59E4
    @L_801A59E0:
    li r29, 0x0
    @L_801A59E4:
    cmpwi r29, 0x0
    stw r29, 0x90(r26)
    beq @L_801A5C10
    lwz r0, 0x10(r26)
    addi r4, r1, 0xc
    addi r3, r26, 0x94
    stw r0, 0xc(r1)
    bl fn_800BA440
    lfs f0, lbl_8047DBF8(r2)
    addi r3, r26, 0x94
    lfs f1, lbl_8047DBFC(r2)
    stfs f0, 0x38(r26)
    lfs f2, lbl_8047DBFC(r2)
    lfs f4, 0x38(r26)
    lfs f3, 0x38(r26)
    lfs f0, lbl_8047DBE4(r2)
    fmuls f4, f4, f1
    lfs f1, lbl_8047DBE8(r2)
    fnmsubs f6, f3, f2, f0
    lfs f2, lbl_8047DBE8(r2)
    lfs f3, lbl_8047DBE4(r2)
    lfs f5, lbl_8047DBE8(r2)
    bl fn_800BA198
    lhz r0, 0x8(r26)
    clrlwi r0, r0, 30
    cmpwi r0, 0x1
    beq @L_801A5A90
    blt @L_801A5BE8
    cmpwi r0, 0x4
    bge @L_801A5BE8
    cmplwi r26, 0x0
    beq @L_801A5A7C
    lwz r0, 0x18(r26)
    cmplwi r0, 0x0
    beq @L_801A5A7C
    lwz r3, 0x18(r26)
    addi r4, r26, 0x3c
    bl fn_80191688
    @L_801A5A7C:
    mr r3, r31
    addi r4, r26, 0x3c
    addi r5, r26, 0x3c
    bl fn_800A37CC
    b @L_801A5BF8
    @L_801A5A90:
    lis r4, lbl_80274D58@ha
    lwzu r8, lbl_80274D58@l(r4)
    lis r3, lbl_80274D64@ha
    cmplwi r26, 0x0
    addi r5, r3, lbl_80274D64@l
    lwz r7, 0x4(r4)
    lwz r6, 0x8(r4)
    lwz r4, 0x0(r5)
    lwz r3, 0x4(r5)
    lwz r0, 0x8(r5)
    stw r8, 0x20(r1)
    stw r7, 0x24(r1)
    stw r6, 0x28(r1)
    stw r4, 0x2c(r1)
    stw r3, 0x30(r1)
    stw r0, 0x34(r1)
    beq @L_801A5BC8
    cmplwi r26, 0x0
    beq @L_801A5AF4
    lwz r0, 0x18(r26)
    cmplwi r0, 0x0
    beq @L_801A5AF4
    lwz r3, 0x18(r26)
    addi r4, r1, 0x20
    bl fn_80191688
    @L_801A5AF4:
    cmplwi r26, 0x0
    beq @L_801A5B14
    lwz r0, 0x1c(r26)
    cmplwi r0, 0x0
    beq @L_801A5B14
    lwz r3, 0x1c(r26)
    addi r4, r1, 0x2c
    bl fn_80191688
    @L_801A5B14:
    addi r3, r1, 0x2c
    addi r4, r1, 0x20
    addi r5, r26, 0x3c
    bl fn_800A3A9C
    addic. r0, r26, 0x3c
    beq @L_801A5B34
    addic. r0, r26, 0x3c
    bne @L_801A5B3C
    @L_801A5B34:
    li r0, -0x1
    b @L_801A5BA8
    @L_801A5B3C:
    lfs f0, 0x3c(r26)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A5B98
    lfs f0, 0x40(r26)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A5B98
    lfs f0, 0x44(r26)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A5B98
    li r0, -0x1
    b @L_801A5BA8
    @L_801A5B98:
    addi r3, r26, 0x3c
    addi r4, r26, 0x3c
    bl fn_800A3ADC
    li r0, 0x0
    @L_801A5BA8:
    cmpwi r0, 0x0
    beq @L_801A5BC8
    lfs f0, lbl_8047DBE8(r2)
    lfs f1, lbl_8047DBE8(r2)
    stfs f0, 0x3c(r26)
    lfs f0, lbl_8047DBE4(r2)
    stfs f1, 0x40(r26)
    stfs f0, 0x44(r26)
    @L_801A5BC8:
    mr r3, r31
    addi r4, r26, 0x3c
    addi r5, r26, 0x3c
    bl fn_800A3820
    addi r3, r26, 0x3c
    addi r4, r26, 0x3c
    bl fn_800A3ADC
    b @L_801A5BF8
    @L_801A5BE8:
    la r3, lbl_8047DBB8(r2)
    li r4, 0x2c0
    la r5, lbl_8047DBD4(r2)
    bl fn_80196E10
    @L_801A5BF8:
    lhz r0, 0x8(r26)
    ori r0, r0, 0x100
    sth r0, 0x8(r26)
    lwz r0, lbl_8047B2C0(r13)
    or r0, r0, r29
    stw r0, lbl_8047B2C0(r13)
    @L_801A5C10:
    addi r28, r28, 0x1
    @L_801A5C14:
    cmpwi r30, 0x8
    bge @L_801A5C24
    cmpw r28, r27
    blt @L_801A5928
    @L_801A5C24:
    li r28, 0x0
    b @L_801A5D98
    @L_801A5C2C:
    cmpwi r28, 0x0
    blt @L_801A5C54
    cmpwi r28, 0x8
    bge @L_801A5C54
    lis r3, lbl_804655E0@ha
    slwi r4, r28, 2
    addi r0, r3, lbl_804655E0@l
    add r3, r0, r4
    lwz r0, 0x0(r3)
    b @L_801A5C58
    @L_801A5C54:
    li r0, 0x0
    @L_801A5C58:
    mr. r26, r0
    beq @L_801A5D94
    lhz r0, 0x8(r26)
    lwz r3, 0x10(r26)
    rlwinm. r0, r0, 0, 26, 26
    lfs f29, 0x38(r26)
    stw r3, 0x1c(r1)
    bne @L_801A5D94
    lhz r0, 0x8(r26)
    clrlwi. r0, r0, 30
    beq @L_801A5D94
    lhz r0, 0x8(r26)
    andi. r0, r0, 0x14
    beq @L_801A5D1C
    lbz r3, 0x14(r26)
    lbz r0, 0x1c(r1)
    cmplw r3, r0
    bne @L_801A5CD0
    lbz r3, 0x15(r26)
    lbz r0, 0x1d(r1)
    cmplw r3, r0
    bne @L_801A5CD0
    lbz r3, 0x16(r26)
    lbz r0, 0x1e(r1)
    cmplw r3, r0
    bne @L_801A5CD0
    lbz r3, 0x17(r26)
    lbz r0, 0x1f(r1)
    cmplw r3, r0
    beq @L_801A5CF8
    @L_801A5CD0:
    lwz r0, 0x1c(r1)
    addi r4, r1, 0x8
    addi r3, r26, 0x50
    stw r0, 0x8(r1)
    bl fn_800BA440
    lwz r0, 0x1c(r1)
    stw r0, 0x14(r26)
    lhz r0, 0x8(r26)
    ori r0, r0, 0x80
    sth r0, 0x8(r26)
    @L_801A5CF8:
    lhz r0, 0x8(r26)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_801A5D1C
    lwz r4, 0x4c(r26)
    addi r3, r26, 0x50
    bl fn_800BA44C
    lhz r0, 0x8(r26)
    rlwinm r0, r0, 0, 25, 23
    sth r0, 0x8(r26)
    @L_801A5D1C:
    lwz r0, 0x90(r26)
    cmpwi r0, 0x0
    beq @L_801A5D94
    lfs f0, 0x38(r26)
    fcmpu cr0, f0, f29
    beq @L_801A5D70
    lfs f2, lbl_8047DBFC(r2)
    addi r3, r26, 0x94
    lfs f1, lbl_8047DBFC(r2)
    lfs f0, lbl_8047DBE4(r2)
    fmuls f4, f29, f2
    stfs f29, 0x38(r26)
    fnmsubs f6, f29, f1, f0
    lfs f1, lbl_8047DBE8(r2)
    lfs f2, lbl_8047DBE8(r2)
    lfs f3, lbl_8047DBE4(r2)
    lfs f5, lbl_8047DBE8(r2)
    bl fn_800BA198
    lhz r0, 0x8(r26)
    ori r0, r0, 0x100
    sth r0, 0x8(r26)
    @L_801A5D70:
    lhz r0, 0x8(r26)
    rlwinm. r0, r0, 0, 23, 23
    beq @L_801A5D94
    lwz r4, 0x90(r26)
    addi r3, r26, 0x94
    bl fn_800BA44C
    lhz r0, 0x8(r26)
    rlwinm r0, r0, 0, 24, 22
    sth r0, 0x8(r26)
    @L_801A5D94:
    addi r28, r28, 0x1
    @L_801A5D98:
    cmpw r28, r27
    blt @L_801A5C2C
    psq_l f31, 0xe8(r1), 0, 0
    lfd f31, 0xe0(r1)
    psq_l f30, 0xd8(r1), 0, 0
    lfd f30, 0xd0(r1)
    psq_l f29, 0xc8(r1), 0, 0
    lfd f29, 0xc0(r1)
    lmw r25, 0xa4(r1)
    lwz r0, 0xf4(r1)
    mtlr r0
    addi r1, r1, 0xf0
    blr
