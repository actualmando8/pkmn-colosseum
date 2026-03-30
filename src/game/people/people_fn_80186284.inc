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
    stmw r26, 0x48(r1)
    fmr f31, f1
    lfs f30, lbl_8047D83C(r2)
    fmr f29, f2
    mr r30, r3
    mr r31, r4
    mr r26, r5
    mr r27, r6
    li r29, 0x0
    b @L_80186308
    @L_801862D0:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80186304
    lwz r0, 0x28(r3)
    cmplw r0, r26
    bne @L_80186304
    lwz r0, 0x2c(r3)
    cmplw r0, r27
    bne @L_80186304
    lwz r29, 0x4(r3)
    b @L_80186374
    @L_80186304:
    addi r29, r29, 0x1
    @L_80186308:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_801862D0
    li r28, 0x0
    b @L_80186364
    @L_8018631C:
    mr r3, r28
    bl fn_8018FD88
    mr r29, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80186360
    lwz r0, 0x2c(r29)
    cmplw r0, r27
    bne @L_80186360
    lis r3, lbl_80273FD8@ha
    mr r4, r26
    addi r3, r3, lbl_80273FD8@l
    mr r5, r27
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r29, 0x4(r29)
    b @L_80186374
    @L_80186360:
    addi r28, r28, 0x1
    @L_80186364:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018631C
    li r29, 0x0
    @L_80186374:
    li r28, 0x0
    b @L_801863A4
    @L_8018637C:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801863A0
    lwz r0, 0x4(r3)
    cmplw r0, r29
    bne @L_801863A0
    b @L_801863B4
    @L_801863A0:
    addi r28, r28, 0x1
    @L_801863A4:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018637C
    li r3, 0x0
    @L_801863B4:
    cmplwi r3, 0x0
    bne @L_801863C4
    li r3, 0x0
    b @L_801865F4
    @L_801863C4:
    bl fn_8018FCBC
    mr r4, r3
    addi r3, r1, 0x2c
    bl fn_800E01D0
    li r28, 0x0
    b @L_80186414
    @L_801863DC:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80186410
    lwz r0, 0x28(r3)
    cmplw r0, r30
    bne @L_80186410
    lwz r0, 0x2c(r3)
    cmplw r0, r31
    bne @L_80186410
    lwz r28, 0x4(r3)
    b @L_80186480
    @L_80186410:
    addi r28, r28, 0x1
    @L_80186414:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_801863DC
    li r28, 0x0
    b @L_80186470
    @L_80186428:
    mr r3, r28
    bl fn_8018FD88
    mr r29, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018646C
    lwz r0, 0x2c(r29)
    cmplw r0, r31
    bne @L_8018646C
    lis r3, lbl_80273FD8@ha
    mr r4, r30
    addi r3, r3, lbl_80273FD8@l
    mr r5, r31
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r29)
    b @L_80186480
    @L_8018646C:
    addi r28, r28, 0x1
    @L_80186470:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_80186428
    li r28, 0x0
    @L_80186480:
    li r29, 0x0
    b @L_801864B4
    @L_80186488:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801864B0
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_801864B0
    mr r28, r3
    b @L_801864C4
    @L_801864B0:
    addi r29, r29, 0x1
    @L_801864B4:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_80186488
    li r28, 0x0
    @L_801864C4:
    cmplwi r28, 0x0
    bne @L_801864D4
    li r3, 0x0
    b @L_801865F4
    @L_801864D4:
    mr r3, r28
    bl fn_8018FCBC
    mr r4, r3
    addi r3, r1, 0x38
    bl fn_800E01D0
    addi r3, r1, 0x14
    addi r4, r1, 0x38
    addi r5, r1, 0x2c
    bl fn_800E0168
    lfs f0, lbl_8047D800(r2)
    addi r3, r1, 0x14
    fmuls f31, f0, f31
    bl fn_800E008C
    fcmpo cr0, f1, f31
    ble @L_80186518
    li r3, 0x0
    b @L_801865F4
    @L_80186518:
    mr r3, r28
    addi r4, r1, 0x20
    bl fn_8018FC2C
    lfs f31, 0x24(r1)
    addi r3, r1, 0x8
    addi r4, r1, 0x2c
    addi r5, r1, 0x38
    bl fn_800E0168
    lfs f1, 0x8(r1)
    lfs f2, 0x10(r1)
    bl fn_800CE2D8
    frsp f0, f1
    lfd f2, lbl_8047D7F0(r2)
    fsubs f0, f0, f31
    fadd f1, f2, f0
    bl fn_800CE318
    frsp f1, f1
    lfd f0, lbl_8047D7A8(r2)
    fcmpo cr0, f1, f0
    ble @L_80186578
    lfd f0, lbl_8047D7F0(r2)
    fsub f1, f1, f0
    frsp f1, f1
    b @L_80186590
    @L_80186578:
    lfd f0, lbl_8047D820(r2)
    fcmpo cr0, f1, f0
    bge @L_80186590
    lfd f0, lbl_8047D7F0(r2)
    fadd f1, f0, f1
    frsp f1, f1
    @L_80186590:
    lfs f0, lbl_8047D814(r2)
    fabs f2, f1
    lfs f1, lbl_8047D7A4(r2)
    fmuls f0, f0, f29
    fmuls f29, f1, f0
    fcmpo cr0, f2, f29
    ble @L_801865B4
    li r3, 0x0
    b @L_801865F4
    @L_801865B4:
    lwz r3, 0x30(r28)
    bl fn_8018F6F4
    cmplwi r3, 0x0
    beq @L_801865CC
    bl fn_8018F5E4
    fmr f30, f1
    @L_801865CC:
    fmr f1, f30
    addi r3, r1, 0x38
    addi r4, r1, 0x2c
    li r5, 0x0
    bl fn_8010F188
    cmpwi r3, 0x0
    beq @L_801865F0
    li r3, 0x0
    b @L_801865F4
    @L_801865F0:
    li r3, 0x1
    @L_801865F4:
    psq_l f31, 0x88(r1), 0, 0
    lfd f31, 0x80(r1)
    psq_l f30, 0x78(r1), 0, 0
    lfd f30, 0x70(r1)
    psq_l f29, 0x68(r1), 0, 0
    lfd f29, 0x60(r1)
    lmw r26, 0x48(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
