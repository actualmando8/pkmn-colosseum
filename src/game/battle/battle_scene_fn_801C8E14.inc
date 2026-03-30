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
    stfd f27, 0x70(r1)
    psq_st f27, 0x78(r1), 0, 0
    stmw r23, 0x4c(r1)
    mr r24, r3
    mr r25, r4
    mr r31, r5
    mr r26, r6
    lfs f0, -21900(r2)
    li r30, -0x1
    lwz r4, -31064(r13)
    li r27, 0x0
    stfs f0, 0x8(r1)
    lwz r0, 0x0(r4)
    cmplw r25, r0
    blt @L_801C8E84
    li r3, -0x1
    b @L_801C98D4
@L_801C8E84:
    mulli r0, r25, 0x18
    lwz r4, -31060(r13)
    add r29, r4, r0
    lwz r4, 0x14(r29)
    cmplwi r4, 0x0
    bne @L_801C8EA4
    li r3, -0x1
    b @L_801C98D4
@L_801C8EA4:
    bl fn_80113F6C
    mr r28, r3
    cmplwi r28, 0x0
    bne @L_801C8EBC
    li r3, -0x1
    b @L_801C98D4
@L_801C8EBC:
    mr r3, r24
    bl fn_80115C48
    bl fn_80115A80
    rlwinm r0, r26, 0, 24, 31
    mr r23, r3
    cmplwi r0, 0x2
    blt @L_801C9064
    cmplwi r0, 0x5
    bgt @L_801C9064
    lwz r6, 0x14(r29)
    mr r5, r23
    lbz r7, 0x8(r29)
    li r3, 0x0
    li r4, 0x64
    bl fn_801845E4
    lwz r6, 0x14(r29)
    mr r5, r23
    lbz r7, 0x8(r29)
    li r3, 0x0
    li r4, 0x65
    bl fn_801845E4
    rlwinm r0, r26, 0, 24, 31
    cmpwi r0, 0x4
    beq @L_801C8FA4
    bge @L_801C8F30
    cmpwi r0, 0x2
    beq @L_801C8F3C
    bge @L_801C8F70
    b @L_801C9008
@L_801C8F30:
    cmpwi r0, 0x6
    bge @L_801C9008
    b @L_801C8FD8
@L_801C8F3C:
    lfs f1, -21900(r2)
    li r3, 0x0
    lfs f3, -21872(r2)
    li r4, 0x64
    fmr f2, f1
    bl fn_801860F8
    lfs f1, -21900(r2)
    li r3, 0x0
    lfs f3, -21868(r2)
    li r4, 0x65
    fmr f2, f1
    bl fn_801860F8
    b @L_801C9008
@L_801C8F70:
    lfs f1, -21900(r2)
    li r3, 0x0
    lfs f3, -21868(r2)
    li r4, 0x64
    fmr f2, f1
    bl fn_801860F8
    lfs f1, -21900(r2)
    li r3, 0x0
    lfs f3, -21872(r2)
    li r4, 0x65
    fmr f2, f1
    bl fn_801860F8
    b @L_801C9008
@L_801C8FA4:
    lfs f2, -21900(r2)
    li r3, 0x0
    lfs f1, -21872(r2)
    li r4, 0x64
    fmr f3, f2
    bl fn_801860F8
    lfs f2, -21900(r2)
    li r3, 0x0
    lfs f1, -21868(r2)
    li r4, 0x65
    fmr f3, f2
    bl fn_801860F8
    b @L_801C9008
@L_801C8FD8:
    lfs f2, -21900(r2)
    li r3, 0x0
    lfs f1, -21868(r2)
    li r4, 0x64
    fmr f3, f2
    bl fn_801860F8
    lfs f2, -21900(r2)
    li r3, 0x0
    lfs f1, -21872(r2)
    li r4, 0x65
    fmr f3, f2
    bl fn_801860F8
@L_801C9008:
    li r3, 0x0
    li r4, 0x65
    bl fn_80188AF4
    cmplwi r28, 0x0
    beq @L_801C905C
    extsh r4, r31
    cmpwi r4, 0x0
    blt @L_801C905C
    mr r3, r28
    bl fn_800ECCA8
    lfs f1, -21900(r2)
    mr r3, r28
    bl fn_800ECA78
    lfs f1, -21912(r2)
    mr r3, r28
    bl fn_800EC9DC
    mr r3, r28
    li r4, 0x0
    bl fn_800ECB74
    mr r3, r28
    bl fn_800EC990
@L_801C905C:
    li r3, 0x0
    b @L_801C98D4
@L_801C9064:
    rlwinm r31, r31, 0, 16, 31
    cmpwi r31, 0xc0
    beq @L_801C9668
    bge @L_801C909C
    cmpwi r31, 0x81
    bge @L_801C9090
    cmpwi r31, 0x3
    bge @L_801C96B0
    cmpwi r31, 0x1
    bge @L_801C90A8
    b @L_801C96B0
@L_801C9090:
    cmpwi r31, 0x83
    bge @L_801C96B0
    b @L_801C90A8
@L_801C909C:
    cmpwi r31, 0x100
    beq @L_801C9880
    b @L_801C96B0
@L_801C90A8:
    lbz r4, 0x0(r29)
    cmplwi r28, 0x0
    lfs f28, 0x8(r1)
    extsb r4, r4
    beq @L_801C90F8
    cmpwi r4, 0x0
    blt @L_801C90F8
    mr r3, r28
    bl fn_800ECCA8
    fmr f1, f28
    mr r3, r28
    bl fn_800ECA78
    lfs f1, -21912(r2)
    mr r3, r28
    bl fn_800EC9DC
    mr r3, r28
    li r4, 0x0
    bl fn_800ECB74
    mr r3, r28
    bl fn_800EC990
@L_801C90F8:
    rlwinm r30, r31, 0, 24, 24
    cmpwi r30, 0x0
    beq @L_801C910C
    li r3, 0x44
    bl fn_80166A28
@L_801C910C:
    cmplwi r28, 0x0
    bne @L_801C9120
    b @L_801C9134
    b @L_801C9120
@L_801C911C:
    bl fn_800F0308
@L_801C9120:
    mr r3, r28
    bl fn_800EC954
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C911C
@L_801C9134:
    lbz r4, 0x8(r29)
    mr r3, r28
    bl fn_800EE150
    addi r4, r1, 0x24
    mr r24, r3
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    mr r3, r24
    bl fn_800EE828
    rlwinm r0, r31, 0, 31, 31
    cmpwi r0, 0x0
    beq @L_801C946C
    li r3, 0x0
    li r4, 0x65
    bl fn_80188AF4
    lfs f1, 0x2c(r1)
    addi r6, r1, 0x24
    lfs f0, -21872(r2)
    li r3, 0x0
    li r4, 0x64
    li r5, 0x1
    fsubs f0, f1, f0
    stfs f0, 0x2c(r1)
    bl fn_8018AACC
    lfs f1, -21864(r2)
    lfs f0, 0x2c(r1)
    lfs f2, 0x24(r1)
    fadds f0, f1, f0
    lfs f1, 0x28(r1)
    stfs f2, 0xc(r1)
    lfs f27, -21900(r2)
    stfs f1, 0x10(r1)
    stfs f0, 0x14(r1)
    lfd f31, -21888(r2)
    lis r24, 0x4330
    lfd f29, -21880(r2)
    lfs f28, -21860(r2)
    b @L_801C9208
@L_801C91D0:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r24, 0x30(r1)
    stw r0, 0x34(r1)
    lfd f0, 0x30(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0x3c(r1)
    stw r24, 0x38(r1)
    lfd f0, 0x38(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
@L_801C9208:
    fcmpo cr0, f27, f28
    blt @L_801C91D0
    lwz r0, -21600(r13)
    cmpwi r0, 0x0
    beq @L_801C92E4
    lis r3, -32698
    li r23, 0x0
    addi r25, r3, 28816
    lfd f31, -21888(r2)
    lis r24, 0x4330
    lfd f29, -21880(r2)
    lfs f28, -21900(r2)
    b @L_801C92D8
@L_801C923C:
    mr r6, r25
    li r3, 0x0
    li r4, 0x65
    li r5, 0x1
    bl fn_8018AACC
    lfs f27, -21892(r2)
    b @L_801C92B4
@L_801C9258:
    li r3, 0x0
    li r4, 0x65
    li r5, 0x0
    bl fn_8018A280
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C927C
    li r3, 0x0
    b @L_801C92C0
@L_801C927C:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r24, 0x38(r1)
    stw r0, 0x3c(r1)
    lfd f0, 0x38(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0x34(r1)
    stw r24, 0x30(r1)
    lfd f0, 0x30(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fsubs f27, f27, f0
@L_801C92B4:
    fcmpo cr0, f27, f28
    bgt @L_801C9258
    li r3, 0x1
@L_801C92C0:
    rlwinm r0, r3, 0, 24, 31
    mr r27, r3
    cmplwi r0, 0x0
    bne @L_801C92E4
    addi r25, r25, 0xc
    addi r23, r23, 0x1
@L_801C92D8:
    lwz r0, -21600(r13)
    cmpw r23, r0
    blt @L_801C923C
@L_801C92E4:
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C9304
    addi r6, r1, 0xc
    li r3, 0x0
    li r4, 0x65
    li r5, 0x1
    bl fn_8018AACC
@L_801C9304:
    li r3, 0x0
    li r4, 0x64
    li r5, 0x1
    bl fn_8018A280
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C93A4
    lfs f27, -21892(r2)
    lfd f31, -21888(r2)
    lis r24, 0x4330
    lfd f29, -21880(r2)
    lfs f28, -21900(r2)
    b @L_801C9394
@L_801C9338:
    li r3, 0x0
    li r4, 0x65
    li r5, 0x0
    bl fn_8018A280
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C935C
    li r0, 0x0
    b @L_801C93A0
@L_801C935C:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r24, 0x38(r1)
    stw r0, 0x3c(r1)
    lfd f0, 0x38(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0x34(r1)
    stw r24, 0x30(r1)
    lfd f0, 0x30(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fsubs f27, f27, f0
@L_801C9394:
    fcmpo cr0, f27, f28
    bgt @L_801C9338
    li r0, 0x1
@L_801C93A0:
    mr r27, r0
@L_801C93A4:
    lfs f1, -21900(r2)
    li r3, 0x0
    lfs f2, -21912(r2)
    li r4, 0x64
    bl fn_8018805C
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C93D8
    lfs f1, -21900(r2)
    li r3, 0x0
    lfs f2, -21912(r2)
    li r4, 0x65
    bl fn_8018805C
@L_801C93D8:
    li r3, 0x0
    li r4, 0x64
    li r5, 0x1
    bl fn_8018A280
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C95DC
    lfs f27, -21892(r2)
    lfd f31, -21888(r2)
    lis r24, 0x4330
    lfd f29, -21880(r2)
    lfs f28, -21900(r2)
    b @L_801C9460
@L_801C940C:
    li r3, 0x0
    li r4, 0x65
    li r5, 0x0
    bl fn_8018A280
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C95DC
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r24, 0x38(r1)
    stw r0, 0x3c(r1)
    lfd f0, 0x38(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0x34(r1)
    stw r24, 0x30(r1)
    lfd f0, 0x30(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fsubs f27, f27, f0
@L_801C9460:
    fcmpo cr0, f27, f28
    bgt @L_801C940C
    b @L_801C95DC
@L_801C946C:
    li r3, 0x0
    li r4, 0x64
    bl fn_80184470
    li r3, 0x0
    li r4, 0x65
    bl fn_80184470
    li r3, 0x0
    li r4, 0x65
    bl fn_80188F78
    lbz r4, 0x8(r29)
    mr r3, r28
    bl fn_800EE150
    addi r4, r1, 0x24
    mr r24, r3
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    mr r3, r24
    bl fn_800EE828
    lfs f1, -21872(r2)
    addi r5, r1, 0x18
    lfs f0, 0x2c(r1)
    li r3, 0x0
    lfs f2, 0x24(r1)
    li r4, 0x64
    fadds f0, f1, f0
    lfs f1, 0x28(r1)
    stfs f2, 0x18(r1)
    stfs f1, 0x1c(r1)
    stfs f0, 0x20(r1)
    bl fn_8018C0A8
    lfs f1, 0x2c(r1)
    addi r5, r1, 0xc
    lfs f0, -21872(r2)
    li r3, 0x0
    lfs f2, 0x24(r1)
    li r4, 0x65
    fsubs f0, f1, f0
    lfs f1, 0x28(r1)
    stfs f2, 0xc(r1)
    stfs f1, 0x10(r1)
    stfs f0, 0x14(r1)
    bl fn_8018C0A8
    lfs f1, 0x20(r1)
    addi r6, r1, 0x18
    lfs f2, -21856(r2)
    li r3, 0x0
    lfs f0, 0x14(r1)
    li r4, 0x64
    fadds f1, f1, f2
    li r5, 0x1
    fadds f0, f0, f2
    stfs f1, 0x20(r1)
    stfs f0, 0x14(r1)
    bl fn_8018AACC
    lfs f27, -21900(r2)
    lfd f28, -21888(r2)
    lis r24, 0x4330
    lfd f30, -21880(r2)
    lfs f31, -21860(r2)
    b @L_801C9598
@L_801C9560:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r24, 0x38(r1)
    stw r0, 0x3c(r1)
    lfd f0, 0x38(r1)
    fsubs f29, f0, f28
    bl fn_800D3088
    stw r3, 0x34(r1)
    stw r24, 0x30(r1)
    lfd f0, 0x30(r1)
    fsubs f0, f0, f30
    fdivs f0, f0, f29
    fadds f27, f27, f0
@L_801C9598:
    fcmpo cr0, f27, f31
    blt @L_801C9560
    addi r6, r1, 0xc
    li r3, 0x0
    li r4, 0x65
    li r5, 0x1
    bl fn_8018AACC
    li r3, 0x0
    li r4, 0x64
    li r5, 0x1
    bl fn_8018A280
    li r3, 0x0
    li r4, 0x65
    li r5, 0x1
    bl fn_8018A280
    li r0, 0x0
    stb r0, -21596(r13)
@L_801C95DC:
    lbz r4, 0x1(r29)
    cmplwi r28, 0x0
    lfs f27, 0x8(r1)
    extsb r4, r4
    beq @L_801C962C
    cmpwi r4, 0x0
    blt @L_801C962C
    mr r3, r28
    bl fn_800ECCA8
    fmr f1, f27
    mr r3, r28
    bl fn_800ECA78
    lfs f1, -21912(r2)
    mr r3, r28
    bl fn_800EC9DC
    mr r3, r28
    li r4, 0x0
    bl fn_800ECB74
    mr r3, r28
    bl fn_800EC990
@L_801C962C:
    cmpwi r30, 0x0
    beq @L_801C963C
    li r3, 0x44
    bl fn_80166A28
@L_801C963C:
    cmplwi r28, 0x0
    bne @L_801C9650
    b @L_801C9880
    b @L_801C9650
@L_801C964C:
    bl fn_800F0308
@L_801C9650:
    mr r3, r28
    bl fn_800EC954
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C964C
    b @L_801C9880
@L_801C9668:
    cmplwi r28, 0x0
    bne @L_801C967C
    b @L_801C9690
    b @L_801C967C
@L_801C9678:
    bl fn_800F0308
@L_801C967C:
    mr r3, r28
    bl fn_800EC954
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C9678
@L_801C9690:
    rlwinm r0, r31, 0, 24, 24
    cmpwi r0, 0x0
    beq @L_801C9880
    li r3, 0x45
    bl fn_801669BC
    li r3, 0x46
    bl fn_80166A28
    b @L_801C9880
@L_801C96B0:
    rlwinm r0, r31, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_801C96C8
    lbz r30, 0x5(r29)
    extsb r30, r30
    b @L_801C96DC
@L_801C96C8:
    rlwinm r0, r31, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_801C96DC
    lbz r30, 0x6(r29)
    extsb r30, r30
@L_801C96DC:
    extsh r0, r30
    cmpwi r0, 0x0
    bge @L_801C96F0
    li r3, -0x1
    b @L_801C98D4
@L_801C96F0:
    mr r3, r28
    bl fn_800EC1BC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C970C
    li r3, -0x1
    b @L_801C98D4
@L_801C970C:
    rlwinm r0, r31, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_801C97C4
    lwz r6, 0x14(r29)
    mr r5, r23
    lbz r7, 0x8(r29)
    li r3, 0x0
    li r4, 0x64
    bl fn_801845E4
    lwz r6, 0x14(r29)
    mr r5, r23
    lbz r7, 0x8(r29)
    li r3, 0x0
    li r4, 0x65
    bl fn_801845E4
    rlwinm r0, r26, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C9788
    lfs f1, -21900(r2)
    li r3, 0x0
    lfs f3, -21872(r2)
    li r4, 0x64
    fmr f2, f1
    bl fn_801860F8
    lfs f1, -21900(r2)
    li r3, 0x0
    lfs f3, -21868(r2)
    li r4, 0x65
    fmr f2, f1
    bl fn_801860F8
    b @L_801C97B8
@L_801C9788:
    lfs f1, -21900(r2)
    li r3, 0x0
    lfs f3, -21868(r2)
    li r4, 0x64
    fmr f2, f1
    bl fn_801860F8
    lfs f1, -21900(r2)
    li r3, 0x0
    lfs f3, -21872(r2)
    li r4, 0x65
    fmr f2, f1
    bl fn_801860F8
@L_801C97B8:
    li r3, 0x0
    li r4, 0x65
    bl fn_80188AF4
@L_801C97C4:
    rlwinm r0, r31, 0, 27, 27
    cmpwi r0, 0x0
    beq @L_801C97FC
    mr r3, r28
    extsh r4, r30
    bl fn_800ECCA8
    mr r3, r28
    addi r4, r1, 0x8
    li r5, 0x0
    bl fn_800EC4D0
    lfs f1, 0x8(r1)
    lfs f0, -21920(r2)
    fsubs f0, f1, f0
    stfs f0, 0x8(r1)
@L_801C97FC:
    cmplwi r28, 0x0
    lfs f27, 0x8(r1)
    beq @L_801C9848
    extsh r4, r30
    cmpwi r4, 0x0
    blt @L_801C9848
    mr r3, r28
    bl fn_800ECCA8
    fmr f1, f27
    mr r3, r28
    bl fn_800ECA78
    lfs f1, -21912(r2)
    mr r3, r28
    bl fn_800EC9DC
    mr r3, r28
    li r4, 0x0
    bl fn_800ECB74
    mr r3, r28
    bl fn_800EC990
@L_801C9848:
    rlwinm r0, r31, 0, 25, 25
    cmpwi r0, 0x0
    beq @L_801C986C
    mr r3, r24
    mr r4, r25
    mr r6, r26
    li r5, 0xc0
    bl fn_801C8E14
    b @L_801C9880
@L_801C986C:
    rlwinm r0, r31, 0, 24, 24
    cmpwi r0, 0x0
    beq @L_801C9880
    li r3, 0x45
    bl fn_80166A28
@L_801C9880:
    mr r3, r28
    addi r4, r1, 0x8
    li r5, 0x0
    bl fn_800EC4D0
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0x3c(r1)
    lfd f3, -21888(r2)
    stw r0, 0x38(r1)
    lfs f2, -21848(r2)
    lfd f0, 0x38(r1)
    lfs f1, 0x8(r1)
    fsubs f3, f0, f3
    lfs f0, -21852(r2)
    fmuls f1, f2, f1
    fdivs f1, f1, f3
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x30(r1)
    lwz r3, 0x34(r1)
@L_801C98D4:
    psq_l f31, 0xb8(r1), 0, 0
    lfd f31, 0xb0(r1)
    psq_l f30, 0xa8(r1), 0, 0
    lfd f30, 0xa0(r1)
    psq_l f29, 0x98(r1), 0, 0
    lfd f29, 0x90(r1)
    psq_l f28, 0x88(r1), 0, 0
    lfd f28, 0x80(r1)
    psq_l f27, 0x78(r1), 0, 0
    lfd f27, 0x70(r1)
    lmw r23, 0x4c(r1)
    lwz r0, 0xc4(r1)
    mtlr r0
    addi r1, r1, 0xc0
    blr
