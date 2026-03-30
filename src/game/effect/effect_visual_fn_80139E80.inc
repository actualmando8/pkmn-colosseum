    nofralloc
    stwu r1, -0xb0(r1)
    mflr r0
    stw r0, 0xb4(r1)
    stfd f31, 0xa0(r1)
    psq_st f31, 0xa8(r1), 0, 0
    stmw r26, 0x88(r1)
    mr r30, r4
    lhz r7, 0x12dc(r3)
    lhz r0, 0x12de(r3)
    mr r29, r3
    lhz r4, 0x48(r4)
    mr r26, r6
    subf r0, r7, r0
    mr r31, r5
    subf r6, r7, r4
    addi r3, r1, 0x8
    addi r4, r29, 0x12cc
    addi r5, r30, 0x20
    clrlwi r27, r6, 16
    extrwi r28, r0, 15, 16
    bl fn_800E0108
    addi r3, r1, 0x8
    mr r4, r26
    mr r5, r3
    bl fn_800E0168
    cmplw r27, r28
    bge @L_80139F2C
    lis r0, 0x4330
    stw r27, 0x7c(r1)
    lfd f2, lbl_8047D1A0(r2)
    stw r0, 0x78(r1)
    lfs f3, lbl_8047D1A8(r2)
    lfd f0, 0x78(r1)
    stw r28, 0x84(r1)
    fsubs f1, f0, f2
    lfs f31, 0x38(r30)
    stw r0, 0x80(r1)
    lfd f0, 0x80(r1)
    fsubs f0, f0, f2
    fdivs f0, f1, f0
    fsubs f0, f3, f0
    fmuls f31, f31, f0
    b @L_80139F30
    @L_80139F2C:
    lfs f31, lbl_8047D190(r2)
    @L_80139F30:
    lhz r5, 0x12e2(r29)
    mr r6, r29
    li r27, 0x0
    li r4, 0x0
    b @L_80139F68
    @L_80139F44:
    lwz r3, 0xc(r6)
    lfs f0, 0x4(r3)
    fcmpo cr0, f0, f31
    ble @L_80139F60
    clrlwi r3, r27, 16
    addi r0, r3, 0x1
    clrlwi r27, r0, 16
    @L_80139F60:
    addi r4, r4, 0x1
    addi r6, r6, 0x18
    @L_80139F68:
    clrlwi r0, r4, 16
    cmplw r0, r5
    blt @L_80139F44
    addi r3, r1, 0x44
    addi r4, r1, 0x8
    bl fn_800E0560
    addi r3, r1, 0x44
    bl fn_800D7F14
    addi r3, r1, 0x44
    addi r4, r30, 0x20
    bl fn_800E042C
    addi r3, r1, 0x44
    bl fn_800D7F14
    clrlslwi r3, r27, 18, 2
    bl fn_800D67BC
    lhz r28, 0x12e2(r29)
    mr r30, r29
    li r27, 0x0
    b @L_8013A1A0
    @L_80139FB4:
    lwz r5, 0xc(r30)
    lfs f0, 0x4(r5)
    fcmpo cr0, f0, f31
    ble @L_8013A198
    mr r4, r30
    addi r3, r1, 0x20
    bl fn_800E0168
    addi r3, r1, 0x20
    mr r4, r3
    bl fn_800E0060
    lwz r5, 0xc(r30)
    mr r4, r30
    addi r3, r1, 0x14
    bl fn_800E019C
    addi r3, r1, 0x14
    lfs f1, lbl_8047D1AC(r2)
    mr r4, r3
    bl fn_800E013C
    addi r3, r1, 0x14
    mr r4, r31
    mr r5, r3
    bl fn_800E0168
    addi r3, r1, 0x14
    mr r4, r3
    bl fn_800E0060
    addi r3, r1, 0x2c
    addi r4, r1, 0x14
    addi r5, r1, 0x20
    bl fn_800DFFCC
    addi r3, r1, 0x2c
    bl fn_800E008C
    lfs f0, lbl_8047D1B0(r2)
    fcmpo cr0, f1, f0
    ble @L_8013A048
    addi r3, r1, 0x2c
    mr r4, r3
    bl fn_800E00AC
    @L_8013A048:
    lfs f1, lbl_8047D1AC(r2)
    addi r3, r1, 0x20
    lfs f0, 0x10(r30)
    addi r4, r1, 0x2c
    fmuls f1, f1, f0
    bl fn_800E013C
    lwz r4, 0xc(r30)
    addi r3, r1, 0x38
    addi r5, r1, 0x20
    bl fn_800E019C
    lfs f1, 0x38(r1)
    lfs f2, 0x3c(r1)
    lfs f3, 0x40(r1)
    bl fn_800D6680
    lbz r4, 0x12d8(r29)
    li r3, 0x0
    lbz r5, 0x12d9(r29)
    lbz r6, 0x12da(r29)
    lbz r7, 0x12db(r29)
    bl fn_800D5CB8
    lfs f1, lbl_8047D190(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    lwz r4, 0xc(r30)
    addi r3, r1, 0x38
    addi r5, r1, 0x20
    bl fn_800E0168
    lfs f1, 0x38(r1)
    lfs f2, 0x3c(r1)
    lfs f3, 0x40(r1)
    bl fn_800D6680
    lbz r4, 0x12d8(r29)
    li r3, 0x0
    lbz r5, 0x12d9(r29)
    lbz r6, 0x12da(r29)
    lbz r7, 0x12db(r29)
    bl fn_800D5CB8
    lfs f1, lbl_8047D1A8(r2)
    li r3, 0x0
    lfs f2, lbl_8047D190(r2)
    bl fn_800D59B8
    lfs f1, lbl_8047D1AC(r2)
    addi r3, r1, 0x20
    lfs f0, 0x14(r30)
    addi r4, r1, 0x2c
    fmuls f1, f1, f0
    bl fn_800E013C
    mr r4, r30
    addi r3, r1, 0x38
    addi r5, r1, 0x20
    bl fn_800E0168
    lfs f1, 0x38(r1)
    lfs f2, 0x3c(r1)
    lfs f3, 0x40(r1)
    bl fn_800D6680
    lbz r4, 0x12d8(r29)
    li r3, 0x0
    lbz r5, 0x12d9(r29)
    lbz r6, 0x12da(r29)
    lbz r7, 0x12db(r29)
    bl fn_800D5CB8
    lfs f1, lbl_8047D1A8(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    mr r4, r30
    addi r3, r1, 0x38
    addi r5, r1, 0x20
    bl fn_800E019C
    lfs f1, 0x38(r1)
    lfs f2, 0x3c(r1)
    lfs f3, 0x40(r1)
    bl fn_800D6680
    lbz r4, 0x12d8(r29)
    li r3, 0x0
    lbz r5, 0x12d9(r29)
    lbz r6, 0x12da(r29)
    lbz r7, 0x12db(r29)
    bl fn_800D5CB8
    lfs f1, lbl_8047D190(r2)
    li r3, 0x0
    lfs f2, lbl_8047D1A8(r2)
    bl fn_800D59B8
    @L_8013A198:
    addi r27, r27, 0x1
    addi r30, r30, 0x18
    @L_8013A1A0:
    clrlwi r0, r27, 16
    cmplw r0, r28
    blt @L_80139FB4
    bl fn_800D6728
    bl fn_800D7E5C
    bl fn_800D7E5C
    psq_l f31, 0xa8(r1), 0, 0
    lfd f31, 0xa0(r1)
    lmw r26, 0x88(r1)
    lwz r0, 0xb4(r1)
    mtlr r0
    addi r1, r1, 0xb0
    blr
