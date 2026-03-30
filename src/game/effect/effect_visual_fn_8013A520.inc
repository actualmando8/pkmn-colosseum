    nofralloc
    stwu r1, -0x170(r1)
    mflr r0
    stw r0, 0x174(r1)
    stfd f31, 0x160(r1)
    psq_st f31, 0x168(r1), 0, 0
    stfd f30, 0x150(r1)
    psq_st f30, 0x158(r1), 0, 0
    stfd f29, 0x140(r1)
    psq_st f29, 0x148(r1), 0, 0
    stfd f28, 0x130(r1)
    psq_st f28, 0x138(r1), 0, 0
    stfd f27, 0x120(r1)
    psq_st f27, 0x128(r1), 0, 0
    stfd f26, 0x110(r1)
    psq_st f26, 0x118(r1), 0, 0
    stfd f25, 0x100(r1)
    psq_st f25, 0x108(r1), 0, 0
    stfd f24, 0xf0(r1)
    psq_st f24, 0xf8(r1), 0, 0
    stfd f23, 0xe0(r1)
    psq_st f23, 0xe8(r1), 0, 0
    stfd f22, 0xd0(r1)
    psq_st f22, 0xd8(r1), 0, 0
    stmw r20, 0xa0(r1)
    mr. r22, r3
    beq @L_8013AA24
    lfs f31, 0x20(r22)
    lhz r3, 0xc(r22)
    lhz r4, 0xe(r22)
    bl fn_800F9318
    mr. r25, r3
    bne @L_8013A5A8
    li r3, 0x0
    b @L_8013AA28
    @L_8013A5A8:
    bl fn_800EE0E8
    mr r31, r3
    mr r3, r25
    addi r4, r1, 0x58
    bl fn_800E3D98
    bl fn_800D2248
    bl fn_800D2584
    addi r4, r1, 0x4c
    mr r20, r3
    bl fn_800D1F84
    addi r5, r1, 0x8
    mr r3, r20
    mr r6, r5
    addi r4, r1, 0xc
    mr r7, r5
    bl fn_800D1FDC
    addi r3, r1, 0x4c
    addi r4, r1, 0x58
    bl fn_800E0040
    fmr f29, f1
    li r3, 0x1
    li r4, 0x6
    li r5, 0x1
    bl fn_800DA4C4
    li r3, 0x1
    li r4, 0x1
    li r5, 0x0
    bl fn_800DA2BC
    li r3, 0x1
    li r4, 0x1
    li r5, 0x1
    bl fn_800DA1E8
    li r3, 0x0
    bl fn_800DA028
    lfs f0, 0xc(r1)
    lfs f3, lbl_8047D1B8(r2)
    lfs f2, 0x10(r22)
    fmuls f1, f29, f0
    lfs f0, lbl_8047D1BC(r2)
    fmuls f2, f3, f2
    fdivs f1, f2, f1
    fcmpo cr0, f1, f0
    ble @L_8013A658
    fmr f1, f0
    @L_8013A658:
    lfs f0, lbl_8047D1C0(r2)
    li r4, 0x5
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x98(r1)
    lwz r3, 0x9c(r1)
    bl fn_800B9404
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    lwz r3, 0x4(r22)
    bl fn_800D7820
    li r3, 0x2
    bl fn_800D6A00
    addi r30, r1, 0x70
    addi r29, r1, 0x7c
    addi r28, r1, 0x88
    li r27, 0x1
    b @L_8013AA14
    @L_8013A6A8:
    mr r3, r25
    mr r4, r27
    bl fn_800EE150
    mr. r24, r3
    beq @L_8013AA10
    bl fn_800EE7E0
    clrlwi. r0, r3, 24
    beq @L_8013AA08
    mr r3, r24
    addi r4, r1, 0x40
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    addi r3, r1, 0x40
    addi r4, r1, 0x58
    bl fn_800E0040
    lfs f0, lbl_8047D1C4(r2)
    fcmpo cr0, f1, f0
    ble @L_8013AA08
    addi r26, r27, 0x1
    b @L_8013AA00
    @L_8013A6FC:
    mr r3, r25
    mr r4, r26
    bl fn_800EE150
    mr. r23, r3
    beq @L_8013A9FC
    bl fn_800EE7E0
    clrlwi. r0, r3, 24
    beq @L_8013A9F4
    bl fn_800E0BE4
    fcmpo cr0, f31, f1
    ble @L_8013A9F4
    mr r3, r23
    addi r4, r1, 0x34
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    addi r3, r1, 0x40
    addi r4, r1, 0x58
    bl fn_800E0040
    lfs f0, lbl_8047D1C4(r2)
    fcmpo cr0, f1, f0
    ble @L_8013A9F4
    addi r3, r1, 0x64
    addi r4, r1, 0x40
    bl fn_800E01D0
    mr r3, r30
    addi r4, r1, 0x34
    bl fn_800E01D0
    addi r3, r1, 0x40
    addi r4, r1, 0x34
    bl fn_800E0040
    lfs f0, lbl_8047D1C8(r2)
    fmuls f30, f0, f1
    bl fn_800E0BE4
    lfs f0, lbl_8047D1CC(r2)
    fmuls f27, f0, f1
    bl fn_800E0BE4
    lfs f0, lbl_8047D1D0(r2)
    fmuls f28, f0, f1
    fmr f1, f28
    bl fn_800CE148
    frsp f24, f1
    bl fn_800E0BE4
    fmuls f29, f30, f1
    fmr f1, f27
    bl fn_800CE148
    frsp f25, f1
    fmr f1, f28
    bl fn_800CDBE0
    frsp f26, f1
    fmr f1, f27
    bl fn_800CDBE0
    frsp f1, f1
    addi r3, r1, 0x28
    fmuls f0, f29, f25
    fmuls f2, f29, f26
    fmuls f1, f29, f1
    fmuls f3, f24, f0
    fmuls f1, f24, f1
    bl fn_800E01F4
    mr r3, r29
    addi r4, r1, 0x40
    addi r5, r1, 0x28
    bl fn_800E019C
    bl fn_800E0BE4
    lfs f0, lbl_8047D1CC(r2)
    fmuls f22, f0, f1
    bl fn_800E0BE4
    lfs f0, lbl_8047D1D0(r2)
    fmuls f23, f0, f1
    fmr f1, f23
    bl fn_800CE148
    frsp f24, f1
    bl fn_800E0BE4
    fmuls f27, f30, f1
    fmr f1, f22
    bl fn_800CE148
    frsp f25, f1
    fmr f1, f23
    bl fn_800CDBE0
    frsp f26, f1
    fmr f1, f22
    bl fn_800CDBE0
    frsp f1, f1
    addi r3, r1, 0x28
    fmuls f0, f27, f25
    fmuls f2, f27, f26
    fmuls f1, f27, f1
    fmuls f3, f24, f0
    fmuls f1, f24, f1
    bl fn_800E01F4
    mr r3, r28
    addi r4, r1, 0x34
    addi r5, r1, 0x28
    bl fn_800E019C
    bl fn_800E0BA0
    lfs f3, 0x1c(r22)
    lfs f2, 0x18(r22)
    lfs f0, 0x10(r22)
    fmadds f1, f3, f1, f2
    fmuls f1, f0, f1
    bl fn_800C46B0
    mr r21, r3
    lis r4, 0x4330
    stw r21, 0x9c(r1)
    addi r0, r21, 0x1
    lfd f1, lbl_8047D1D8(r2)
    clrlwi r3, r0, 16
    stw r4, 0x98(r1)
    lfs f2, lbl_8047D1BC(r2)
    lfd f0, 0x98(r1)
    fsubs f0, f0, f1
    fdivs f28, f2, f0
    bl fn_800D67BC
    lfs f1, 0x64(r1)
    lfs f2, 0x68(r1)
    lfs f3, 0x6c(r1)
    bl fn_800D6680
    lbz r4, 0x0(r22)
    li r3, 0x0
    lbz r5, 0x1(r22)
    lbz r6, 0x2(r22)
    lbz r7, 0x3(r22)
    bl fn_800D5CB8
    fmr f27, f28
    lfs f29, lbl_8047D1CC(r2)
    lfs f30, lbl_8047D1D0(r2)
    li r20, 0x1
    b @L_8013A9C0
    @L_8013A900:
    fmr f1, f27
    addi r3, r1, 0x1c
    addi r4, r1, 0x64
    bl fn_800E07E4
    bl fn_800E0BE4
    fmuls f24, f29, f1
    bl fn_800E0BE4
    fmuls f25, f30, f1
    fmr f1, f25
    bl fn_800CE148
    frsp f23, f1
    bl fn_800E0BA0
    lfs f2, 0x14(r22)
    lfs f0, 0x10(r22)
    fmuls f0, f2, f0
    fmuls f26, f0, f1
    fmr f1, f24
    bl fn_800CE148
    frsp f22, f1
    fmr f1, f25
    bl fn_800CDBE0
    frsp f25, f1
    fmr f1, f24
    bl fn_800CDBE0
    frsp f1, f1
    addi r3, r1, 0x10
    fmuls f0, f26, f22
    fmuls f2, f26, f25
    fmuls f1, f26, f1
    fmuls f3, f23, f0
    fmuls f1, f23, f1
    bl fn_800E01F4
    addi r3, r1, 0x1c
    addi r5, r1, 0x10
    mr r4, r3
    bl fn_800E019C
    lfs f1, 0x1c(r1)
    lfs f2, 0x20(r1)
    lfs f3, 0x24(r1)
    bl fn_800D6680
    lbz r4, 0x0(r22)
    li r3, 0x0
    lbz r5, 0x1(r22)
    lbz r6, 0x2(r22)
    lbz r7, 0x3(r22)
    bl fn_800D5CB8
    fadds f27, f27, f28
    addi r20, r20, 0x1
    @L_8013A9C0:
    cmplw r20, r21
    blt @L_8013A900
    lfs f1, 0x0(r30)
    lfs f2, 0x74(r1)
    lfs f3, 0x78(r1)
    bl fn_800D6680
    lbz r4, 0x0(r22)
    li r3, 0x0
    lbz r5, 0x1(r22)
    lbz r6, 0x2(r22)
    lbz r7, 0x3(r22)
    bl fn_800D5CB8
    bl fn_800D6728
    @L_8013A9F4:
    mr r3, r23
    bl fn_800EE828
    @L_8013A9FC:
    addi r26, r26, 0x1
    @L_8013AA00:
    cmplw r26, r31
    blt @L_8013A6FC
    @L_8013AA08:
    mr r3, r24
    bl fn_800EE828
    @L_8013AA10:
    addi r27, r27, 0x1
    @L_8013AA14:
    cmplw r27, r31
    blt @L_8013A6A8
    li r3, 0x1
    b @L_8013AA28
    @L_8013AA24:
    li r3, 0x0
    @L_8013AA28:
    psq_l f31, 0x168(r1), 0, 0
    lfd f31, 0x160(r1)
    psq_l f30, 0x158(r1), 0, 0
    lfd f30, 0x150(r1)
    psq_l f29, 0x148(r1), 0, 0
    lfd f29, 0x140(r1)
    psq_l f28, 0x138(r1), 0, 0
    lfd f28, 0x130(r1)
    psq_l f27, 0x128(r1), 0, 0
    lfd f27, 0x120(r1)
    psq_l f26, 0x118(r1), 0, 0
    lfd f26, 0x110(r1)
    psq_l f25, 0x108(r1), 0, 0
    lfd f25, 0x100(r1)
    psq_l f24, 0xf8(r1), 0, 0
    lfd f24, 0xf0(r1)
    psq_l f23, 0xe8(r1), 0, 0
    lfd f23, 0xe0(r1)
    psq_l f22, 0xd8(r1), 0, 0
    lfd f22, 0xd0(r1)
    lmw r20, 0xa0(r1)
    lwz r0, 0x174(r1)
    mtlr r0
    addi r1, r1, 0x170
    blr
