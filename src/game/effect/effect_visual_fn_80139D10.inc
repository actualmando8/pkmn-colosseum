    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    beq @L_80139E58
    mr r31, r3
    lwz r0, 0x58(r3)
    lhz r30, 0x44(r3)
    cmplwi r0, 0x0
    bne @L_80139D44
    li r3, 0x0
    b @L_80139E6C
    @L_80139D44:
    cmplwi r30, 0x1
    ble @L_80139D6C
    lhz r3, 0x4a(r31)
    subi r0, r30, 0x1
    divw r0, r3, r0
    clrlwi r26, r0, 16
    subf r0, r26, r3
    divw r0, r0, r30
    clrlwi r29, r0, 16
    b @L_80139D74
    @L_80139D6C:
    li r29, 0x0
    lhz r26, 0x4a(r31)
    @L_80139D74:
    mulli r28, r30, 0x12e4
    mr r3, r28
    bl fn_800E3534
    clrlwi. r0, r3, 16
    bne @L_80139DA0
    lis r3, lbl_80272D08@ha
    addi r3, r3, lbl_80272D08@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_80139E6C
    @L_80139DA0:
    sth r3, 0x4(r31)
    bl fn_800E27B0
    stw r3, 0x0(r31)
    mr r5, r28
    mr r27, r3
    li r4, 0x0
    bl memset
    clrlwi r29, r29, 16
    clrlwi r28, r26, 16
    li r26, 0x0
    b @L_80139E3C
    @L_80139DCC:
    lfs f1, lbl_8047D190(r2)
    li r0, 0x0
    sth r0, 0x12e2(r27)
    addi r3, r27, 0x12c0
    fmr f3, f1
    lfs f2, 0x38(r31)
    bl fn_800E01F4
    lfs f1, 0x3c(r31)
    mr r3, r27
    mr r4, r31
    addi r5, r27, 0x12c0
    li r6, 0x0
    bl fn_8013A1D4
    lbz r3, 0x5c(r31)
    mullw r0, r26, r29
    addi r26, r26, 0x1
    stb r3, 0x12d8(r27)
    lbz r3, 0x5d(r31)
    stb r3, 0x12d9(r27)
    lbz r3, 0x5e(r31)
    stb r3, 0x12da(r27)
    lbz r3, 0x5f(r31)
    stb r3, 0x12db(r27)
    sth r0, 0x12dc(r27)
    lhz r0, 0x12dc(r27)
    add r0, r0, r28
    sth r0, 0x12de(r27)
    addi r27, r27, 0x12e4
    @L_80139E3C:
    clrlwi r0, r26, 16
    cmplw r0, r30
    blt @L_80139DCC
    li r0, 0x0
    li r3, 0x1
    sth r0, 0x48(r31)
    b @L_80139E6C
    @L_80139E58:
    lis r3, lbl_80272D54@ha
    addi r3, r3, lbl_80272D54@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    @L_80139E6C:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
