    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stfd f31, 0x80(r1)
    psq_st f31, 0x88(r1), 0, 0
    stfd f30, 0x70(r1)
    psq_st f30, 0x78(r1), 0, 0
    stfd f29, 0x60(r1)
    psq_st f29, 0x68(r1), 0, 0
    stfd f28, 0x50(r1)
    psq_st f28, 0x58(r1), 0, 0
    stfd f27, 0x40(r1)
    psq_st f27, 0x48(r1), 0, 0
    stfd f26, 0x30(r1)
    psq_st f26, 0x38(r1), 0, 0
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    stw r28, 0x20(r1)
    mr r30, r3
    mr r31, r4
    lwz r6, 0x60(r30)
    lwz r3, 0x0(r6)
    lhz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8002B8F0
    li r3, 0x0
    b @L_8002BC98
    @L_8002B8F0:
    lhz r0, 0x94(r30)
    lis r3, lbl_802E4F68@ha
    lha r5, 0x6(r31)
    addi r3, r3, lbl_802E4F68@l
    sth r0, 0x8(r1)
    li r4, 0x0
    lwz r0, 0x0(r3)
    cmpw r5, r0
    beq @L_8002B968
    addi r3, r3, 0x8
    li r4, 0x1
    lwz r0, 0x0(r3)
    cmpw r5, r0
    beq @L_8002B968
    addi r3, r3, 0x8
    li r4, 0x2
    lwz r0, 0x0(r3)
    cmpw r5, r0
    beq @L_8002B968
    addi r3, r3, 0x8
    li r4, 0x3
    lwz r0, 0x0(r3)
    cmpw r5, r0
    beq @L_8002B968
    addi r3, r3, 0x8
    li r4, 0x4
    lwz r0, 0x0(r3)
    cmpw r5, r0
    beq @L_8002B968
    li r4, 0x5
    @L_8002B968:
    cmpwi r4, 0x5
    blt @L_8002B978
    li r5, 0x0
    b @L_8002B988
    @L_8002B978:
    lis r3, lbl_802E4F68@ha
    slwi r4, r4, 3
    addi r0, r3, lbl_802E4F68@l
    add r5, r0, r4
    @L_8002B988:
    cmplwi r5, 0x0
    beq @L_8002B9F4
    lbz r0, 0x9(r1)
    lwz r3, 0x14(r6)
    extsb r4, r0
    lha r5, 0x4(r5)
    lwz r0, 0x0(r3)
    mulli r3, r4, 0x1f
    cmpwi r0, 0x0
    add r4, r5, r3
    bne @L_8002B9CC
    lwz r3, 0xc(r6)
    lfs f0, 0x0(r3)
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    add r4, r4, r0
    @L_8002B9CC:
    lwz r3, 0x0(r6)
    lhz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_8002B9E4
    li r3, 0x72
    b @L_8002B9E8
    @L_8002B9E4:
    li r3, 0xff
    @L_8002B9E8:
    extsh r0, r4
    sth r0, 0x52(r31)
    stb r3, 0x67(r31)
    @L_8002B9F4:
    lha r5, 0x84(r30)
    lha r3, 0x50(r31)
    lha r4, 0x86(r30)
    lha r0, 0x52(r31)
    add r3, r5, r3
    extsh r3, r3
    add r0, r4, r0
    extsh r4, r0
    bl fn_800FE6D0
    bl fn_800FE4D4
    lha r0, 0x56(r31)
    lis r29, 0x4330
    lha r3, 0x54(r31)
    li r28, 0x0
    xoris r0, r0, 0x8000
    stw r29, 0x18(r1)
    xoris r3, r3, 0x8000
    lfd f31, lbl_8047B998(r2)
    stw r0, 0x1c(r1)
    lfs f2, lbl_8047B98C(r2)
    lfd f0, 0x18(r1)
    stw r3, 0x14(r1)
    fsubs f1, f0, f31
    lfs f0, lbl_8047B9A0(r2)
    stw r29, 0x10(r1)
    lfs f3, lbl_8047B9A4(r2)
    fmuls f1, f2, f1
    lfd f2, 0x10(r1)
    lfs f30, lbl_8047A3F0(r13)
    fsubs f2, f2, f31
    fmuls f0, f1, f0
    fadds f0, f2, f0
    fmuls f0, f3, f0
    fdivs f29, f3, f0
    lfs f26, lbl_8047B978(r2)
    lfs f28, lbl_8047B9AC(r2)
    lfs f27, lbl_8047B97C(r2)
    @L_8002BA88:
    xoris r0, r28, 0x8000
    stw r29, 0x18(r1)
    fmr f1, f30
    mr r3, r30
    stw r0, 0x1c(r1)
    mr r4, r31
    li r5, 0xd1
    lfd f0, 0x18(r1)
    fsubs f0, f0, f31
    fdivs f0, f0, f28
    fmuls f0, f26, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r6, 0x14(r1)
    bl fn_8002B594
    fadds f30, f30, f29
    fcmpo cr0, f30, f27
    cror eq, gt, eq
    bne @L_8002BAD8
    fsubs f30, f30, f27
    @L_8002BAD8:
    addi r28, r28, 0x1
    cmpwi r28, 0x2d
    blt @L_8002BA88
    lfs f2, lbl_8047B9B0(r2)
    lfs f1, lbl_8047A3F0(r13)
    lfs f0, lbl_8047B97C(r2)
    fadds f26, f2, f1
    fcmpo cr0, f26, f0
    ble @L_8002BB00
    fsubs f26, f26, f0
    @L_8002BB00:
    li r28, 0x0
    lfs f31, lbl_8047B978(r2)
    lis r29, 0x4330
    lfd f30, lbl_8047B998(r2)
    lfs f28, lbl_8047B9AC(r2)
    lfs f27, lbl_8047B97C(r2)
    @L_8002BB18:
    xoris r0, r28, 0x8000
    stw r29, 0x18(r1)
    fmr f1, f26
    mr r3, r30
    stw r0, 0x1c(r1)
    mr r4, r31
    li r5, 0xd1
    lfd f0, 0x18(r1)
    fsubs f0, f0, f30
    fdivs f0, f0, f28
    fmuls f0, f31, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r6, 0x14(r1)
    bl fn_8002B594
    fadds f26, f26, f29
    fcmpo cr0, f26, f27
    cror eq, gt, eq
    bne @L_8002BB68
    fsubs f26, f26, f27
    @L_8002BB68:
    addi r28, r28, 0x1
    cmpwi r28, 0x2d
    blt @L_8002BB18
    lfs f2, lbl_8047B9A0(r2)
    lfs f1, lbl_8047A3F0(r13)
    lfs f0, lbl_8047B97C(r2)
    fadds f26, f2, f1
    fcmpo cr0, f26, f0
    ble @L_8002BB90
    fsubs f26, f26, f0
    @L_8002BB90:
    li r28, 0x0
    lfs f31, lbl_8047B978(r2)
    lis r29, 0x4330
    lfd f30, lbl_8047B998(r2)
    lfs f28, lbl_8047B9AC(r2)
    lfs f27, lbl_8047B97C(r2)
    @L_8002BBA8:
    xoris r0, r28, 0x8000
    stw r29, 0x18(r1)
    fmr f1, f26
    mr r3, r30
    stw r0, 0x1c(r1)
    mr r4, r31
    li r5, 0xd1
    lfd f0, 0x18(r1)
    fsubs f0, f0, f30
    fdivs f0, f0, f28
    fmuls f0, f31, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r6, 0x14(r1)
    bl fn_8002B594
    fadds f26, f26, f29
    fcmpo cr0, f26, f27
    cror eq, gt, eq
    bne @L_8002BBF8
    fsubs f26, f26, f27
    @L_8002BBF8:
    addi r28, r28, 0x1
    cmpwi r28, 0x2d
    blt @L_8002BBA8
    lfs f2, lbl_8047B9B4(r2)
    lfs f1, lbl_8047A3F0(r13)
    lfs f0, lbl_8047B97C(r2)
    fadds f26, f2, f1
    fcmpo cr0, f26, f0
    ble @L_8002BC20
    fsubs f26, f26, f0
    @L_8002BC20:
    li r28, 0x0
    lfs f27, lbl_8047B978(r2)
    lis r29, 0x4330
    lfd f28, lbl_8047B998(r2)
    lfs f31, lbl_8047B9AC(r2)
    lfs f30, lbl_8047B97C(r2)
    @L_8002BC38:
    xoris r0, r28, 0x8000
    stw r29, 0x18(r1)
    fmr f1, f26
    mr r3, r30
    stw r0, 0x1c(r1)
    mr r4, r31
    li r5, 0xd1
    lfd f0, 0x18(r1)
    fsubs f0, f0, f28
    fdivs f0, f0, f31
    fmuls f0, f27, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r6, 0x14(r1)
    bl fn_8002B594
    fadds f26, f26, f29
    fcmpo cr0, f26, f30
    cror eq, gt, eq
    bne @L_8002BC88
    fsubs f26, f26, f30
    @L_8002BC88:
    addi r28, r28, 0x1
    cmpwi r28, 0x2d
    blt @L_8002BC38
    li r3, 0x0
    @L_8002BC98:
    psq_l f31, 0x88(r1), 0, 0
    lfd f31, 0x80(r1)
    psq_l f30, 0x78(r1), 0, 0
    lfd f30, 0x70(r1)
    psq_l f29, 0x68(r1), 0, 0
    lfd f29, 0x60(r1)
    psq_l f28, 0x58(r1), 0, 0
    lfd f28, 0x50(r1)
    psq_l f27, 0x48(r1), 0, 0
    lfd f27, 0x40(r1)
    psq_l f26, 0x38(r1), 0, 0
    lfd f26, 0x30(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r0, 0x94(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
