    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stfd f31, 0x70(r1)
    psq_st f31, 0x78(r1), 0, 0
    stfd f30, 0x60(r1)
    psq_st f30, 0x68(r1), 0, 0
    stmw r26, 0x48(r1)
    fmr f30, f1
    mr r28, r3
    mr r29, r4
    mr r30, r5
    mr r31, r6
    li r27, 0x0
    b @L_80187AD4
    @L_80187A9C:
    mr r3, r27
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80187AD0
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_80187AD0
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_80187AD0
    lwz r27, 0x4(r3)
    b @L_80187B40
    @L_80187AD0:
    addi r27, r27, 0x1
    @L_80187AD4:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_80187A9C
    li r26, 0x0
    b @L_80187B30
    @L_80187AE8:
    mr r3, r26
    bl fn_8018FD88
    mr r27, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80187B2C
    lwz r0, 0x2c(r27)
    cmplw r0, r29
    bne @L_80187B2C
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r27, 0x4(r27)
    b @L_80187B40
    @L_80187B2C:
    addi r26, r26, 0x1
    @L_80187B30:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_80187AE8
    li r27, 0x0
    @L_80187B40:
    li r26, 0x0
    b @L_80187B74
    @L_80187B48:
    mr r3, r26
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80187B70
    lwz r0, 0x4(r3)
    cmplw r0, r27
    bne @L_80187B70
    mr r27, r3
    b @L_80187B84
    @L_80187B70:
    addi r26, r26, 0x1
    @L_80187B74:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_80187B48
    li r27, 0x0
    @L_80187B84:
    cmplwi r27, 0x0
    beq @L_80187D24
    mr r3, r30
    mr r4, r31
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80187D24
    bl fn_800E3D00
    mr r4, r3
    addi r3, r1, 0x14
    bl fn_800E01D0
    mr r3, r27
    bl fn_8018FCBC
    mr r5, r3
    addi r3, r1, 0x20
    addi r4, r1, 0x14
    bl fn_800E0168
    lfs f1, 0x20(r1)
    lfs f2, 0x28(r1)
    bl fn_800CE2D8
    frsp f31, f1
    li r26, 0x0
    b @L_80187C18
    @L_80187BE0:
    mr r3, r26
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80187C14
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_80187C14
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_80187C14
    lwz r26, 0x4(r3)
    b @L_80187C84
    @L_80187C14:
    addi r26, r26, 0x1
    @L_80187C18:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_80187BE0
    li r26, 0x0
    b @L_80187C74
    @L_80187C2C:
    mr r3, r26
    bl fn_8018FD88
    mr r30, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80187C70
    lwz r0, 0x2c(r30)
    cmplw r0, r29
    bne @L_80187C70
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r26, 0x4(r30)
    b @L_80187C84
    @L_80187C70:
    addi r26, r26, 0x1
    @L_80187C74:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_80187C2C
    li r26, 0x0
    @L_80187C84:
    li r27, 0x0
    b @L_80187CB8
    @L_80187C8C:
    mr r3, r27
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80187CB4
    lwz r0, 0x4(r3)
    cmplw r0, r26
    bne @L_80187CB4
    mr r26, r3
    b @L_80187CC8
    @L_80187CB4:
    addi r27, r27, 0x1
    @L_80187CB8:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_80187C8C
    li r26, 0x0
    @L_80187CC8:
    cmplwi r26, 0x0
    beq @L_80187D24
    mr r3, r26
    addi r4, r1, 0x8
    bl fn_8018FC2C
    lfs f2, lbl_8047D7C0(r2)
    lis r3, 0x4330
    lfs f0, 0xc(r1)
    li r0, 0x1
    stw r3, 0x38(r1)
    fdivs f0, f0, f2
    lfd f1, lbl_8047D7C8(r2)
    stb r0, 0x22(r26)
    fctiwz f0, f0
    stfd f0, 0x30(r1)
    lwz r0, 0x34(r1)
    xoris r0, r0, 0x8000
    stw r0, 0x3c(r1)
    lfd f0, 0x38(r1)
    fsubs f0, f0, f1
    fmadds f0, f2, f0, f31
    stfs f0, 0x40(r26)
    stfs f30, 0x44(r26)
    @L_80187D24:
    psq_l f31, 0x78(r1), 0, 0
    lfd f31, 0x70(r1)
    psq_l f30, 0x68(r1), 0, 0
    lfd f30, 0x60(r1)
    lmw r26, 0x48(r1)
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
