    nofralloc
    stwu r1, -0xa0(r1)
    mflr r0
    stw r0, 0xa4(r1)
    stfd f31, 0x90(r1)
    psq_st f31, 0x98(r1), 0, 0
    stfd f30, 0x80(r1)
    psq_st f30, 0x88(r1), 0, 0
    stmw r25, 0x64(r1)
    mr. r25, r3
    beq @L_80138608
    lhz r27, 0x8(r25)
    lhz r3, 0xa(r25)
    lhz r4, 0xc(r25)
    bl fn_800F9318
    cmplwi r3, 0x0
    bne @L_80138194
    li r3, 0x0
    b @L_8013860C
    @L_80138194:
    addi r4, r1, 0x1c
    bl fn_800E3D98
    bl fn_800D2248
    bl fn_800D2584
    addi r4, r1, 0x10
    mr r26, r3
    bl fn_800D1F84
    addi r5, r1, 0x8
    mr r3, r26
    mr r6, r5
    addi r4, r1, 0xc
    mr r7, r5
    bl fn_800D1FDC
    addi r3, r1, 0x10
    addi r4, r1, 0x1c
    bl fn_800E0040
    fmr f31, f1
    li r3, 0x1
    li r4, 0x6
    li r5, 0x1
    bl fn_800DA4C4
    li r3, 0x1
    li r4, 0x1
    li r5, 0x1
    bl fn_800DA2BC
    li r3, 0x1
    li r4, 0x1
    li r5, 0x1
    bl fn_800DA1E8
    li r3, 0x0
    bl fn_800DA028
    addi r3, r25, 0x28
    bl fn_800E008C
    lfs f0, lbl_8047D148(r2)
    lfs f2, 0xc(r1)
    fmuls f3, f0, f1
    lfs f0, lbl_8047D14C(r2)
    fmuls f1, f31, f2
    fdivs f1, f3, f1
    fcmpo cr0, f1, f0
    ble @L_8013823C
    fmr f1, f0
    @L_8013823C:
    lfs f0, lbl_8047D150(r2)
    li r4, 0x5
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x58(r1)
    lwz r3, 0x5c(r1)
    bl fn_800B9404
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    lwz r3, 0x14(r25)
    bl fn_800D7820
    lfs f1, 0x1c(r1)
    addi r3, r1, 0x28
    lfs f2, 0x20(r1)
    lfs f3, 0x24(r1)
    bl fn_800E05C0
    addi r3, r1, 0x28
    bl fn_800D7F14
    lfs f1, 0x28(r25)
    addi r3, r1, 0x28
    lfs f2, 0x2c(r25)
    lfs f3, 0x30(r25)
    bl fn_800E048C
    addi r3, r1, 0x28
    bl fn_800D7F14
    li r3, 0x2
    bl fn_800D6A00
    lwz r26, 0x4(r25)
    li r28, 0x0
    b @L_80138350
    @L_801382BC:
    lwz r3, 0x978(r26)
    cmplwi r3, 0x0
    beq @L_80138348
    addi r0, r3, 0x1
    clrlwi r3, r0, 16
    bl fn_800D67BC
    lfs f1, 0x960(r26)
    lfs f2, 0x964(r26)
    lfs f3, 0x968(r26)
    bl fn_800D6680
    lbz r4, 0x20(r25)
    li r3, 0x0
    lbz r5, 0x21(r25)
    lbz r6, 0x22(r25)
    lbz r7, 0x23(r25)
    bl fn_800D5CB8
    lwz r29, 0x978(r26)
    mr r30, r26
    li r31, 0x0
    b @L_8013833C
    @L_8013830C:
    lfs f1, 0x0(r30)
    lfs f2, 0x4(r30)
    lfs f3, 0x8(r30)
    bl fn_800D6680
    lbz r4, 0x20(r25)
    li r3, 0x0
    lbz r5, 0x21(r25)
    lbz r6, 0x22(r25)
    lbz r7, 0x23(r25)
    bl fn_800D5CB8
    addi r31, r31, 0x1
    addi r30, r30, 0x18
    @L_8013833C:
    cmplw r31, r29
    blt @L_8013830C
    bl fn_800D6728
    @L_80138348:
    addi r28, r28, 0x1
    addi r26, r26, 0x97c
    @L_80138350:
    clrlwi r0, r28, 16
    cmplw r0, r27
    blt @L_801382BC
    lwz r0, 0x1c(r25)
    cmplwi r0, 0x0
    beq @L_801385F8
    li r3, 0x1
    li r4, 0x2
    li r5, 0x1
    bl fn_800DA1E8
    li r3, 0x1
    li r4, 0x1
    li r5, 0x0
    bl fn_800DA2BC
    li r3, 0x3
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    lwz r3, 0x18(r25)
    bl fn_800D7820
    lwz r4, 0x1c(r25)
    li r3, 0x0
    bl fn_800D85D4
    li r3, 0x6
    bl fn_800D6A00
    clrlslwi r3, r27, 19, 3
    bl fn_800D67BC
    lwz r31, 0x4(r25)
    li r30, 0x0
    lfs f31, lbl_8047D158(r2)
    b @L_801385E8
    @L_801383CC:
    bl fn_800E0BA0
    lfs f2, 0x6c(r25)
    lfs f0, 0x68(r25)
    lfs f3, 0x960(r31)
    fmadds f30, f2, f1, f0
    lfs f0, 0x968(r31)
    lfs f2, 0x964(r31)
    fsubs f1, f3, f30
    fsubs f3, f0, f30
    bl fn_800D6680
    lbz r4, 0x24(r25)
    li r3, 0x0
    lbz r5, 0x25(r25)
    lbz r6, 0x26(r25)
    lbz r7, 0x27(r25)
    bl fn_800D5CB8
    lfs f1, lbl_8047D154(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    lfs f1, 0x960(r31)
    lfs f0, 0x968(r31)
    fadds f1, f1, f30
    lfs f2, 0x964(r31)
    fsubs f3, f0, f30
    bl fn_800D6680
    lbz r4, 0x24(r25)
    li r3, 0x0
    lbz r5, 0x25(r25)
    lbz r6, 0x26(r25)
    lbz r7, 0x27(r25)
    bl fn_800D5CB8
    lfs f1, lbl_8047D154(r2)
    li r3, 0x0
    lfs f2, lbl_8047D158(r2)
    bl fn_800D59B8
    lfs f1, 0x960(r31)
    lfs f0, 0x968(r31)
    fadds f1, f1, f30
    lfs f2, 0x964(r31)
    fadds f3, f0, f30
    bl fn_800D6680
    lbz r4, 0x24(r25)
    li r3, 0x0
    lbz r5, 0x25(r25)
    lbz r6, 0x26(r25)
    lbz r7, 0x27(r25)
    bl fn_800D5CB8
    lfs f1, lbl_8047D158(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    lfs f1, 0x960(r31)
    lfs f0, 0x968(r31)
    fsubs f1, f1, f30
    lfs f2, 0x964(r31)
    fadds f3, f0, f30
    bl fn_800D6680
    lbz r4, 0x24(r25)
    li r3, 0x0
    lbz r5, 0x25(r25)
    lbz r6, 0x26(r25)
    lbz r7, 0x27(r25)
    bl fn_800D5CB8
    lfs f1, lbl_8047D158(r2)
    li r3, 0x0
    lfs f2, lbl_8047D154(r2)
    bl fn_800D59B8
    fmuls f30, f30, f31
    lfs f1, 0x96c(r31)
    lfs f0, 0x974(r31)
    lfs f2, 0x970(r31)
    fsubs f1, f1, f30
    fsubs f3, f0, f30
    bl fn_800D6680
    lbz r4, 0x24(r25)
    li r3, 0x0
    lbz r5, 0x25(r25)
    lbz r6, 0x26(r25)
    lbz r7, 0x27(r25)
    bl fn_800D5CB8
    lfs f1, lbl_8047D154(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    lfs f1, 0x96c(r31)
    lfs f0, 0x974(r31)
    fadds f1, f1, f30
    lfs f2, 0x970(r31)
    fsubs f3, f0, f30
    bl fn_800D6680
    lbz r4, 0x24(r25)
    li r3, 0x0
    lbz r5, 0x25(r25)
    lbz r6, 0x26(r25)
    lbz r7, 0x27(r25)
    bl fn_800D5CB8
    lfs f1, lbl_8047D154(r2)
    li r3, 0x0
    lfs f2, lbl_8047D158(r2)
    bl fn_800D59B8
    lfs f1, 0x96c(r31)
    lfs f0, 0x974(r31)
    fadds f1, f1, f30
    lfs f2, 0x970(r31)
    fadds f3, f0, f30
    bl fn_800D6680
    lbz r4, 0x24(r25)
    li r3, 0x0
    lbz r5, 0x25(r25)
    lbz r6, 0x26(r25)
    lbz r7, 0x27(r25)
    bl fn_800D5CB8
    lfs f1, lbl_8047D158(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    lfs f1, 0x96c(r31)
    lfs f0, 0x974(r31)
    fsubs f1, f1, f30
    lfs f2, 0x970(r31)
    fadds f3, f0, f30
    bl fn_800D6680
    lbz r4, 0x24(r25)
    li r3, 0x0
    lbz r5, 0x25(r25)
    lbz r6, 0x26(r25)
    lbz r7, 0x27(r25)
    bl fn_800D5CB8
    lfs f1, lbl_8047D158(r2)
    li r3, 0x0
    lfs f2, lbl_8047D154(r2)
    bl fn_800D59B8
    addi r30, r30, 0x1
    addi r31, r31, 0x97c
    @L_801385E8:
    clrlwi r0, r30, 16
    cmplw r0, r27
    blt @L_801383CC
    bl fn_800D6728
    @L_801385F8:
    bl fn_800D7E5C
    bl fn_800D7E5C
    li r3, 0x1
    b @L_8013860C
    @L_80138608:
    li r3, 0x0
    @L_8013860C:
    psq_l f31, 0x98(r1), 0, 0
    lfd f31, 0x90(r1)
    psq_l f30, 0x88(r1), 0, 0
    lfd f30, 0x80(r1)
    lmw r25, 0x64(r1)
    lwz r0, 0xa4(r1)
    mtlr r0
    addi r1, r1, 0xa0
    blr
