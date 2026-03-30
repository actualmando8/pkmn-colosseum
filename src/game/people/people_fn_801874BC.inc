    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stfd f31, 0x60(r1)
    psq_st f31, 0x68(r1), 0, 0
    stfd f30, 0x50(r1)
    psq_st f30, 0x58(r1), 0, 0
    stfd f29, 0x40(r1)
    psq_st f29, 0x48(r1), 0, 0
    stfd f28, 0x30(r1)
    psq_st f28, 0x38(r1), 0, 0
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    stw r28, 0x20(r1)
    fmr f28, f1
    mr r28, r3
    fmr f29, f2
    mr r29, r4
    fmr f30, f3
    li r31, 0x0
    fmr f31, f4
    b @L_80187550
    @L_80187518:
    mr r3, r31
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018754C
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_8018754C
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_8018754C
    lwz r31, 0x4(r3)
    b @L_801875BC
    @L_8018754C:
    addi r31, r31, 0x1
    @L_80187550:
    bl fn_8018FDB4
    cmpw r31, r3
    blt @L_80187518
    li r30, 0x0
    b @L_801875AC
    @L_80187564:
    mr r3, r30
    bl fn_8018FD88
    mr r31, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801875A8
    lwz r0, 0x2c(r31)
    cmplw r0, r29
    bne @L_801875A8
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r31, 0x4(r31)
    b @L_801875BC
    @L_801875A8:
    addi r30, r30, 0x1
    @L_801875AC:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80187564
    li r31, 0x0
    @L_801875BC:
    li r30, 0x0
    b @L_801875EC
    @L_801875C4:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801875E8
    lwz r0, 0x4(r3)
    cmplw r0, r31
    bne @L_801875E8
    b @L_801875FC
    @L_801875E8:
    addi r30, r30, 0x1
    @L_801875EC:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_801875C4
    li r3, 0x0
    @L_801875FC:
    cmplwi r3, 0x0
    bne @L_8018760C
    li r3, 0x0
    b @L_801878CC
    @L_8018760C:
    bl fn_8018FCBC
    mr r4, r3
    addi r3, r1, 0x8
    bl fn_800E01D0
    li r3, 0x0
    li r4, 0x2
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80187850
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80187850
    bl fn_801779EC
    bl fn_800D258C
    bl fn_800D2248
    li r3, 0x0
    bl fn_800DA028
    lis r3, lbl_80314638@ha
    addi r3, r3, lbl_80314638@l
    bl fn_800D7820
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    li r3, 0x1
    li r4, 0x6
    li r5, 0x7
    bl fn_800DA4C4
    li r3, 0x0
    bl fn_800D9ED8
    li r3, 0x4
    bl fn_800D6A00
    li r3, 0xa
    bl fn_800D67BC
    fmr f1, f28
    lfs f2, 0xc(r1)
    fmr f3, f29
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    lfs f2, lbl_8047D844(r2)
    fmr f1, f28
    lfs f0, 0xc(r1)
    fmr f3, f29
    fadds f2, f2, f0
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    fmr f1, f28
    lfs f2, 0xc(r1)
    fmr f3, f31
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    lfs f2, lbl_8047D844(r2)
    fmr f1, f28
    lfs f0, 0xc(r1)
    fmr f3, f31
    fadds f2, f2, f0
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    fmr f1, f30
    lfs f2, 0xc(r1)
    fmr f3, f31
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    lfs f2, lbl_8047D844(r2)
    fmr f1, f30
    lfs f0, 0xc(r1)
    fmr f3, f31
    fadds f2, f2, f0
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    fmr f1, f30
    lfs f2, 0xc(r1)
    fmr f3, f29
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    lfs f2, lbl_8047D844(r2)
    fmr f1, f30
    lfs f0, 0xc(r1)
    fmr f3, f29
    fadds f2, f2, f0
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    fmr f1, f28
    lfs f2, 0xc(r1)
    fmr f3, f29
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    lfs f2, lbl_8047D844(r2)
    fmr f1, f28
    lfs f0, 0xc(r1)
    fmr f3, f29
    fadds f2, f2, f0
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    bl fn_800D6728
    @L_80187850:
    fcmpo cr0, f28, f30
    bge @L_80187864
    fmr f1, f28
    fmr f2, f30
    b @L_8018786C
    @L_80187864:
    fmr f1, f30
    fmr f2, f28
    @L_8018786C:
    fcmpo cr0, f29, f31
    bge @L_80187880
    fmr f3, f29
    fmr f4, f31
    b @L_80187888
    @L_80187880:
    fmr f3, f31
    fmr f4, f29
    @L_80187888:
    lfs f0, 0x8(r1)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801878C8
    fcmpo cr0, f0, f2
    cror eq, lt, eq
    bne @L_801878C8
    lfs f0, 0x10(r1)
    fcmpo cr0, f3, f0
    cror eq, lt, eq
    bne @L_801878C8
    fcmpo cr0, f0, f4
    cror eq, lt, eq
    bne @L_801878C8
    li r3, 0x1
    b @L_801878CC
    @L_801878C8:
    li r3, 0x0
    @L_801878CC:
    psq_l f31, 0x68(r1), 0, 0
    lfd f31, 0x60(r1)
    psq_l f30, 0x58(r1), 0, 0
    lfd f30, 0x50(r1)
    psq_l f29, 0x48(r1), 0, 0
    lfd f29, 0x40(r1)
    psq_l f28, 0x38(r1), 0, 0
    lfd f28, 0x30(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r0, 0x74(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
