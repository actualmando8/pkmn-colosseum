    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stfd f31, 0x80(r1)
    psq_st f31, 0x88(r1), 0, 0
    stmw r27, 0x6c(r1)
    mr r28, r5
    mr r27, r4
    lfs f4, 0x0(r5)
    mr r31, r3
    mr r30, r7
    lfs f0, lbl_8047D844(r2)
    stfs f4, 0x38(r1)
    mr r29, r6
    fmr f31, f1
    mr r3, r30
    lfs f1, 0x4(r31)
    mr r5, r27
    addi r4, r1, 0x38
    fsubs f3, f1, f0
    fadds f0, f0, f1
    stfs f3, 0x3c(r1)
    lfs f2, 0x8(r28)
    stfs f2, 0x40(r1)
    lfs f1, 0x0(r6)
    stfs f1, 0x44(r1)
    stfs f3, 0x48(r1)
    lfs f1, 0x8(r6)
    stfs f1, 0x4c(r1)
    stfs f4, 0x50(r1)
    stfs f0, 0x54(r1)
    stfs f2, 0x58(r1)
    bl fn_8010C77C
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f1, f0
    bge @L_80187180
    li r4, 0x0
    b @L_801871F4
    @L_80187180:
    mr r4, r30
    mr r6, r27
    addi r3, r1, 0x2c
    addi r5, r1, 0x38
    bl fn_8010FA54
    mr r4, r27
    addi r3, r1, 0x2c
    bl fn_800A3BD8
    fmuls f0, f31, f31
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801871B8
    li r4, 0x0
    b @L_801871F4
    @L_801871B8:
    mr r5, r30
    addi r3, r1, 0x2c
    addi r4, r1, 0x38
    bl fn_8010F71C
    cmpwi r3, 0x0
    bne @L_801871D8
    li r4, 0x0
    b @L_801871F4
    @L_801871D8:
    lwz r3, 0x2c(r1)
    li r4, 0x1
    lwz r0, 0x30(r1)
    stw r3, 0x0(r31)
    stw r0, 0x4(r31)
    lwz r0, 0x34(r1)
    stw r0, 0x8(r31)
    @L_801871F4:
    cmpwi r4, 0x0
    beq @L_80187204
    li r3, 0x1
    b @L_801874A0
    @L_80187204:
    lfs f3, 0x0(r29)
    mr r3, r30
    lfs f0, lbl_8047D844(r2)
    mr r5, r27
    stfs f3, 0x38(r1)
    addi r4, r1, 0x38
    lfs f2, 0x4(r31)
    fsubs f1, f2, f0
    fadds f0, f0, f2
    stfs f1, 0x3c(r1)
    lfs f1, 0x8(r29)
    stfs f1, 0x40(r1)
    stfs f3, 0x44(r1)
    stfs f0, 0x48(r1)
    stfs f1, 0x4c(r1)
    bl fn_8010C77C
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f1, f0
    bge @L_80187258
    li r4, 0x0
    b @L_801872CC
    @L_80187258:
    mr r4, r30
    mr r6, r27
    addi r3, r1, 0x20
    addi r5, r1, 0x38
    bl fn_8010FA54
    mr r4, r27
    addi r3, r1, 0x20
    bl fn_800A3BD8
    fmuls f0, f31, f31
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_80187290
    li r4, 0x0
    b @L_801872CC
    @L_80187290:
    mr r5, r30
    addi r3, r1, 0x20
    addi r4, r1, 0x38
    bl fn_8010F71C
    cmpwi r3, 0x0
    bne @L_801872B0
    li r4, 0x0
    b @L_801872CC
    @L_801872B0:
    lwz r3, 0x20(r1)
    li r4, 0x1
    lwz r0, 0x24(r1)
    stw r3, 0x0(r31)
    stw r0, 0x4(r31)
    lwz r0, 0x28(r1)
    stw r0, 0x8(r31)
    @L_801872CC:
    cmpwi r4, 0x0
    beq @L_801872DC
    li r3, 0x1
    b @L_801874A0
    @L_801872DC:
    lfs f1, 0x0(r29)
    mr r3, r30
    lfs f0, lbl_8047D844(r2)
    mr r5, r27
    stfs f1, 0x38(r1)
    addi r4, r1, 0x38
    lfs f1, 0x4(r31)
    fsubs f3, f1, f0
    fadds f0, f0, f1
    stfs f3, 0x3c(r1)
    lfs f1, 0x8(r29)
    stfs f1, 0x40(r1)
    lfs f2, 0x0(r28)
    stfs f2, 0x44(r1)
    stfs f3, 0x48(r1)
    lfs f1, 0x8(r28)
    stfs f1, 0x4c(r1)
    stfs f2, 0x50(r1)
    stfs f0, 0x54(r1)
    stfs f1, 0x58(r1)
    bl fn_8010C77C
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f1, f0
    bge @L_80187344
    li r4, 0x0
    b @L_801873B8
    @L_80187344:
    mr r4, r30
    mr r6, r27
    addi r3, r1, 0x14
    addi r5, r1, 0x38
    bl fn_8010FA54
    mr r4, r27
    addi r3, r1, 0x14
    bl fn_800A3BD8
    fmuls f0, f31, f31
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_8018737C
    li r4, 0x0
    b @L_801873B8
    @L_8018737C:
    mr r5, r30
    addi r3, r1, 0x14
    addi r4, r1, 0x38
    bl fn_8010F71C
    cmpwi r3, 0x0
    bne @L_8018739C
    li r4, 0x0
    b @L_801873B8
    @L_8018739C:
    lwz r3, 0x14(r1)
    li r4, 0x1
    lwz r0, 0x18(r1)
    stw r3, 0x0(r31)
    stw r0, 0x4(r31)
    lwz r0, 0x1c(r1)
    stw r0, 0x8(r31)
    @L_801873B8:
    cmpwi r4, 0x0
    beq @L_801873C8
    li r3, 0x1
    b @L_801874A0
    @L_801873C8:
    lfs f0, 0x0(r28)
    mr r3, r30
    lfs f1, lbl_8047D844(r2)
    mr r5, r27
    stfs f0, 0x44(r1)
    addi r4, r1, 0x38
    lfs f0, 0x4(r31)
    fadds f1, f1, f0
    stfs f1, 0x48(r1)
    lfs f0, 0x8(r28)
    stfs f0, 0x4c(r1)
    lfs f0, 0x0(r29)
    stfs f0, 0x50(r1)
    stfs f1, 0x54(r1)
    lfs f0, 0x8(r29)
    stfs f0, 0x58(r1)
    bl fn_8010C77C
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f1, f0
    bge @L_80187420
    li r4, 0x0
    b @L_80187494
    @L_80187420:
    mr r4, r30
    mr r6, r27
    addi r3, r1, 0x8
    addi r5, r1, 0x38
    bl fn_8010FA54
    mr r4, r27
    addi r3, r1, 0x8
    bl fn_800A3BD8
    fmuls f0, f31, f31
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_80187458
    li r4, 0x0
    b @L_80187494
    @L_80187458:
    mr r5, r30
    addi r3, r1, 0x8
    addi r4, r1, 0x38
    bl fn_8010F71C
    cmpwi r3, 0x0
    bne @L_80187478
    li r4, 0x0
    b @L_80187494
    @L_80187478:
    lwz r3, 0x8(r1)
    li r4, 0x1
    lwz r0, 0xc(r1)
    stw r3, 0x0(r31)
    stw r0, 0x4(r31)
    lwz r0, 0x10(r1)
    stw r0, 0x8(r31)
    @L_80187494:
    neg r0, r4
    or r0, r0, r4
    srwi r3, r0, 31
    @L_801874A0:
    psq_l f31, 0x88(r1), 0, 0
    lfd f31, 0x80(r1)
    lmw r27, 0x6c(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
