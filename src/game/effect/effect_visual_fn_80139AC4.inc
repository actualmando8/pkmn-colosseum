    nofralloc
    stwu r1, -0xc0(r1)
    mflr r0
    stw r0, 0xc4(r1)
    stfd f31, 0xb0(r1)
    psq_st f31, 0xb8(r1), 0, 0
    stfd f30, 0xa0(r1)
    psq_st f30, 0xa8(r1), 0, 0
    stmw r23, 0x7c(r1)
    mr. r26, r3
    mr r27, r4
    beq @L_80139CE8
    lwz r0, 0x58(r26)
    lhz r31, 0x48(r26)
    cmplwi r0, 0x0
    bne @L_80139B08
    li r3, 0x0
    b @L_80139CEC
    @L_80139B08:
    lhz r0, 0x4a(r26)
    cmplw r31, r0
    bge @L_80139CE8
    lhz r29, 0x44(r26)
    li r30, 0x0
    lwz r28, 0x0(r26)
    b @L_80139CC8
    @L_80139B24:
    lhz r0, 0x12dc(r28)
    cmplw r31, r0
    blt @L_80139CB8
    lhz r0, 0x12de(r28)
    cmplw r31, r0
    bge @L_80139CB8
    addi r3, r1, 0x8
    addi r4, r26, 0x8
    bl fn_800E01D0
    lhz r25, 0x12e2(r28)
    mr r23, r28
    li r24, 0x0
    b @L_80139BA4
    @L_80139B58:
    bl fn_800E0BA0
    lfs f0, 0x10(r1)
    fmuls f30, f0, f1
    bl fn_800E0BA0
    lfs f0, 0xc(r1)
    fmuls f31, f0, f1
    bl fn_800E0BA0
    lfs f0, 0x8(r1)
    fmr f2, f31
    fmr f3, f30
    addi r3, r1, 0x14
    fmuls f1, f0, f1
    bl fn_800E01F4
    mr r3, r23
    mr r4, r23
    addi r5, r1, 0x14
    bl fn_800E019C
    addi r24, r24, 0x1
    addi r23, r23, 0x18
    @L_80139BA4:
    cmplw r24, r25
    blt @L_80139B58
    lhz r5, 0x12de(r28)
    lhz r3, 0x12dc(r28)
    lhz r6, 0x48(r26)
    add r0, r5, r3
    srawi r0, r0, 1
    cmpw r6, r0
    ble @L_80139CAC
    subf r3, r3, r5
    lis r4, 0x4330
    subf r5, r6, r5
    lbz r0, 0x5c(r26)
    srawi r3, r3, 1
    stw r4, 0x20(r1)
    xoris r5, r5, 0x8000
    lfd f3, lbl_8047D198(r2)
    xoris r3, r3, 0x8000
    stw r5, 0x24(r1)
    lfd f1, lbl_8047D1A0(r2)
    lfd f0, 0x20(r1)
    stw r3, 0x2c(r1)
    fsubs f2, f0, f3
    stw r4, 0x28(r1)
    lfd f0, 0x28(r1)
    stw r0, 0x34(r1)
    fsubs f0, f0, f3
    stw r4, 0x30(r1)
    fdivs f2, f2, f0
    lfd f0, 0x30(r1)
    stw r4, 0x40(r1)
    stw r4, 0x50(r1)
    stw r4, 0x60(r1)
    fsubs f0, f0, f1
    fmuls f0, f0, f2
    fctiwz f0, f0
    stfd f0, 0x38(r1)
    lwz r0, 0x3c(r1)
    stb r0, 0x12d8(r28)
    lbz r0, 0x5d(r26)
    stw r0, 0x44(r1)
    lfd f0, 0x40(r1)
    fsubs f0, f0, f1
    fmuls f0, f0, f2
    fctiwz f0, f0
    stfd f0, 0x48(r1)
    lwz r0, 0x4c(r1)
    stb r0, 0x12d9(r28)
    lbz r0, 0x5e(r26)
    stw r0, 0x54(r1)
    lfd f0, 0x50(r1)
    fsubs f0, f0, f1
    fmuls f0, f0, f2
    fctiwz f0, f0
    stfd f0, 0x58(r1)
    lwz r0, 0x5c(r1)
    stb r0, 0x12da(r28)
    lbz r0, 0x5f(r26)
    stw r0, 0x64(r1)
    lfd f0, 0x60(r1)
    fsubs f0, f0, f1
    fmuls f0, f0, f2
    fctiwz f0, f0
    stfd f0, 0x68(r1)
    lwz r0, 0x6c(r1)
    stb r0, 0x12db(r28)
    @L_80139CAC:
    li r0, 0x1
    sth r0, 0x12e0(r28)
    b @L_80139CC0
    @L_80139CB8:
    li r0, 0x0
    sth r0, 0x12e0(r28)
    @L_80139CC0:
    addi r30, r30, 0x1
    addi r28, r28, 0x12e4
    @L_80139CC8:
    clrlwi r0, r30, 16
    cmplw r0, r29
    blt @L_80139B24
    lhz r0, 0x48(r26)
    li r3, 0x1
    add r0, r0, r27
    sth r0, 0x48(r26)
    b @L_80139CEC
    @L_80139CE8:
    li r3, 0x0
    @L_80139CEC:
    psq_l f31, 0xb8(r1), 0, 0
    lfd f31, 0xb0(r1)
    psq_l f30, 0xa8(r1), 0, 0
    lfd f30, 0xa0(r1)
    lmw r23, 0x7c(r1)
    lwz r0, 0xc4(r1)
    mtlr r0
    addi r1, r1, 0xc0
    blr
