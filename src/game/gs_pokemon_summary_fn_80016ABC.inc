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
    lwz r0, lbl_8047A2D8(r13)
    cmpwi r0, -0x1
    beq @L_80016B24
    li r3, 0x0
    b @L_80016EC4
    @L_80016B24:
    lwz r0, lbl_8047A2E8(r13)
    cmpwi r0, 0x0
    blt @L_80016B38
    li r3, 0x0
    b @L_80016EC4
    @L_80016B38:
    lbz r0, 0x95(r30)
    lis r3, lbl_80266918@ha
    addi r3, r3, lbl_80266918@l
    extsb r0, r0
    mulli r0, r0, 0x4c
    add r3, r3, r0
    lwz r0, 0x1c(r3)
    clrlwi r3, r0, 16
    bl fn_80103E68
    lwz r4, lbl_8047A2E8(r13)
    srwi r0, r3, 16
    sth r0, 0x8(r1)
    cmpwi r4, 0x0
    blt @L_80016BCC
    lbz r3, 0x8(r1)
    lwz r0, lbl_8047A2C8(r13)
    extsb r3, r3
    subf r3, r3, r4
    cmpwi r0, 0x0
    mulli r3, r3, 0x1f
    addi r4, r3, 0x95
    beq @L_80016BA4
    lfs f0, lbl_8047A2D0(r13)
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    subf r4, r0, r4
    @L_80016BA4:
    lha r0, 0x56(r31)
    add r0, r4, r0
    cmpwi r0, 0x95
    blt @L_80016BBC
    cmpwi r4, 0x18d
    blt @L_80016BC4
    @L_80016BBC:
    li r3, 0x0
    b @L_80016C14
    @L_80016BC4:
    li r3, 0xff
    b @L_80016C14
    @L_80016BCC:
    lbz r3, 0x9(r1)
    lwz r0, lbl_8047A2C8(r13)
    extsb r3, r3
    mulli r3, r3, 0x1f
    cmpwi r0, 0x0
    addi r4, r3, 0x95
    bne @L_80016BFC
    lfs f0, lbl_8047A2D0(r13)
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    add r4, r4, r0
    @L_80016BFC:
    lwz r0, lbl_8047A2D8(r13)
    cmpwi r0, -0x1
    bne @L_80016C10
    li r3, 0x72
    b @L_80016C14
    @L_80016C10:
    li r3, 0xff
    @L_80016C14:
    extsh r0, r4
    sth r0, 0x52(r31)
    stb r3, 0x67(r31)
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
    lfd f31, lbl_8047B768(r2)
    stw r0, 0x1c(r1)
    lfs f2, lbl_8047B75C(r2)
    lfd f0, 0x18(r1)
    stw r3, 0x14(r1)
    fsubs f1, f0, f31
    lfs f0, lbl_8047B770(r2)
    stw r29, 0x10(r1)
    lfs f3, lbl_8047B774(r2)
    fmuls f1, f2, f1
    lfd f2, 0x10(r1)
    lfs f30, lbl_8047A2CC(r13)
    fsubs f2, f2, f31
    fmuls f0, f1, f0
    fadds f0, f2, f0
    fmuls f0, f3, f0
    fdivs f29, f3, f0
    lfs f26, lbl_8047B740(r2)
    lfs f28, lbl_8047B77C(r2)
    lfs f27, lbl_8047B744(r2)
    @L_80016CB4:
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
    bl fn_800167D0
    fadds f30, f30, f29
    fcmpo cr0, f30, f27
    cror eq, gt, eq
    bne @L_80016D04
    fsubs f30, f30, f27
    @L_80016D04:
    addi r28, r28, 0x1
    cmpwi r28, 0x2d
    blt @L_80016CB4
    lfs f2, lbl_8047B780(r2)
    lfs f1, lbl_8047A2CC(r13)
    lfs f0, lbl_8047B744(r2)
    fadds f26, f2, f1
    fcmpo cr0, f26, f0
    ble @L_80016D2C
    fsubs f26, f26, f0
    @L_80016D2C:
    li r28, 0x0
    lfs f31, lbl_8047B740(r2)
    lis r29, 0x4330
    lfd f30, lbl_8047B768(r2)
    lfs f28, lbl_8047B77C(r2)
    lfs f27, lbl_8047B744(r2)
    @L_80016D44:
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
    bl fn_800167D0
    fadds f26, f26, f29
    fcmpo cr0, f26, f27
    cror eq, gt, eq
    bne @L_80016D94
    fsubs f26, f26, f27
    @L_80016D94:
    addi r28, r28, 0x1
    cmpwi r28, 0x2d
    blt @L_80016D44
    lfs f2, lbl_8047B770(r2)
    lfs f1, lbl_8047A2CC(r13)
    lfs f0, lbl_8047B744(r2)
    fadds f26, f2, f1
    fcmpo cr0, f26, f0
    ble @L_80016DBC
    fsubs f26, f26, f0
    @L_80016DBC:
    li r28, 0x0
    lfs f31, lbl_8047B740(r2)
    lis r29, 0x4330
    lfd f30, lbl_8047B768(r2)
    lfs f28, lbl_8047B77C(r2)
    lfs f27, lbl_8047B744(r2)
    @L_80016DD4:
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
    bl fn_800167D0
    fadds f26, f26, f29
    fcmpo cr0, f26, f27
    cror eq, gt, eq
    bne @L_80016E24
    fsubs f26, f26, f27
    @L_80016E24:
    addi r28, r28, 0x1
    cmpwi r28, 0x2d
    blt @L_80016DD4
    lfs f2, lbl_8047B784(r2)
    lfs f1, lbl_8047A2CC(r13)
    lfs f0, lbl_8047B744(r2)
    fadds f26, f2, f1
    fcmpo cr0, f26, f0
    ble @L_80016E4C
    fsubs f26, f26, f0
    @L_80016E4C:
    li r28, 0x0
    lfs f27, lbl_8047B740(r2)
    lis r29, 0x4330
    lfd f28, lbl_8047B768(r2)
    lfs f31, lbl_8047B77C(r2)
    lfs f30, lbl_8047B744(r2)
    @L_80016E64:
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
    bl fn_800167D0
    fadds f26, f26, f29
    fcmpo cr0, f26, f30
    cror eq, gt, eq
    bne @L_80016EB4
    fsubs f26, f26, f30
    @L_80016EB4:
    addi r28, r28, 0x1
    cmpwi r28, 0x2d
    blt @L_80016E64
    li r3, 0x0
    @L_80016EC4:
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
