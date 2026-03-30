    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stfd f31, 0x50(r1)
    psq_st f31, 0x58(r1), 0, 0
    stfd f30, 0x40(r1)
    psq_st f30, 0x48(r1), 0, 0
    stw r31, 0x3c(r1)
    stw r30, 0x38(r1)
    stw r29, 0x34(r1)
    stw r28, 0x30(r1)
    fmr f31, f1
    mr r30, r3
    mr r31, r4
    li r29, 0x0
    b @L_8018A4C4
    @L_8018A48C:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A4C0
    lwz r0, 0x28(r3)
    cmplw r0, r30
    bne @L_8018A4C0
    lwz r0, 0x2c(r3)
    cmplw r0, r31
    bne @L_8018A4C0
    lwz r29, 0x4(r3)
    b @L_8018A530
    @L_8018A4C0:
    addi r29, r29, 0x1
    @L_8018A4C4:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_8018A48C
    li r28, 0x0
    b @L_8018A520
    @L_8018A4D8:
    mr r3, r28
    bl fn_8018FD88
    mr r29, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A51C
    lwz r0, 0x2c(r29)
    cmplw r0, r31
    bne @L_8018A51C
    lis r3, lbl_80273FD8@ha
    mr r4, r30
    addi r3, r3, lbl_80273FD8@l
    mr r5, r31
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r29, 0x4(r29)
    b @L_8018A530
    @L_8018A51C:
    addi r28, r28, 0x1
    @L_8018A520:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018A4D8
    li r29, 0x0
    @L_8018A530:
    li r28, 0x0
    b @L_8018A560
    @L_8018A538:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A55C
    lwz r0, 0x4(r3)
    cmplw r0, r29
    bne @L_8018A55C
    b @L_8018A570
    @L_8018A55C:
    addi r28, r28, 0x1
    @L_8018A560:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018A538
    li r3, 0x0
    @L_8018A570:
    cmplwi r3, 0x0
    beq @L_8018A6D0
    lfs f0, lbl_8047D814(r2)
    li r28, 0x0
    lfs f30, 0x58(r3)
    fmuls f31, f0, f31
    b @L_8018A5C4
    @L_8018A58C:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A5C0
    lwz r0, 0x28(r3)
    cmplw r0, r30
    bne @L_8018A5C0
    lwz r0, 0x2c(r3)
    cmplw r0, r31
    bne @L_8018A5C0
    lwz r28, 0x4(r3)
    b @L_8018A630
    @L_8018A5C0:
    addi r28, r28, 0x1
    @L_8018A5C4:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018A58C
    li r28, 0x0
    b @L_8018A620
    @L_8018A5D8:
    mr r3, r28
    bl fn_8018FD88
    mr r29, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A61C
    lwz r0, 0x2c(r29)
    cmplw r0, r31
    bne @L_8018A61C
    lis r3, lbl_80273FD8@ha
    mr r4, r30
    addi r3, r3, lbl_80273FD8@l
    mr r5, r31
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r29)
    b @L_8018A630
    @L_8018A61C:
    addi r28, r28, 0x1
    @L_8018A620:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018A5D8
    li r28, 0x0
    @L_8018A630:
    li r29, 0x0
    b @L_8018A664
    @L_8018A638:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A660
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_8018A660
    mr r28, r3
    b @L_8018A674
    @L_8018A660:
    addi r29, r29, 0x1
    @L_8018A664:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_8018A638
    li r28, 0x0
    @L_8018A674:
    cmplwi r28, 0x0
    beq @L_8018A6D0
    mr r3, r28
    addi r4, r1, 0x8
    bl fn_8018FC2C
    lfs f2, lbl_8047D7C0(r2)
    lis r3, 0x4330
    lfs f0, 0xc(r1)
    li r0, 0x1
    stw r3, 0x20(r1)
    fdivs f0, f0, f2
    lfd f1, lbl_8047D7C8(r2)
    stb r0, 0x22(r28)
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r0, 0x1c(r1)
    xoris r0, r0, 0x8000
    stw r0, 0x24(r1)
    lfd f0, 0x20(r1)
    fsubs f0, f0, f1
    fmadds f31, f2, f0, f31
    stfs f31, 0x40(r28)
    stfs f30, 0x44(r28)
    @L_8018A6D0:
    psq_l f31, 0x58(r1), 0, 0
    lfd f31, 0x50(r1)
    psq_l f30, 0x48(r1), 0, 0
    lfd f30, 0x40(r1)
    lwz r31, 0x3c(r1)
    lwz r30, 0x38(r1)
    lwz r29, 0x34(r1)
    lwz r0, 0x64(r1)
    lwz r28, 0x30(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
