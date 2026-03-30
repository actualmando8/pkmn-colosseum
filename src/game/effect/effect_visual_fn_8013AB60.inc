    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stmw r27, 0x6c(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    mr r31, r6
    li r3, 0x14
    bl fn_800E3534
    clrlwi. r0, r3, 16
    mr r27, r3
    beq @L_8013AD50
    bl fn_800E27B0
    addis r0, r31, 0x1
    sth r27, 0xc(r3)
    cmplwi r0, 0xffff
    bne @L_8013ACD8
    lbz r5, 0x0(r29)
    lis r4, 0x4330
    lbz r0, 0x0(r30)
    stw r5, 0xc(r1)
    lfd f3, lbl_8047D1E8(r2)
    stw r4, 0x8(r1)
    lfs f2, lbl_8047D1E0(r2)
    lfd f0, 0x8(r1)
    stw r0, 0x14(r1)
    fsubs f1, f0, f3
    stw r4, 0x10(r1)
    lfd f0, 0x10(r1)
    stw r4, 0x20(r1)
    fsubs f0, f0, f3
    stw r4, 0x28(r1)
    fsubs f0, f0, f1
    stw r4, 0x38(r1)
    stw r4, 0x40(r1)
    fmadds f0, f2, f0, f1
    stw r4, 0x50(r1)
    fctiwz f0, f0
    stw r4, 0x58(r1)
    stfd f0, 0x18(r1)
    lwz r0, 0x1c(r1)
    stb r0, 0x0(r3)
    lbz r4, 0x1(r29)
    lbz r0, 0x1(r30)
    stw r4, 0x24(r1)
    stw r0, 0x2c(r1)
    lfd f1, 0x20(r1)
    lfd f0, 0x28(r1)
    fsubs f1, f1, f3
    fsubs f0, f0, f3
    fsubs f0, f0, f1
    fmadds f0, f2, f0, f1
    fctiwz f0, f0
    stfd f0, 0x30(r1)
    lwz r0, 0x34(r1)
    stb r0, 0x1(r3)
    lbz r4, 0x2(r29)
    lbz r0, 0x2(r30)
    stw r4, 0x3c(r1)
    stw r0, 0x44(r1)
    lfd f1, 0x38(r1)
    lfd f0, 0x40(r1)
    fsubs f1, f1, f3
    fsubs f0, f0, f3
    fsubs f0, f0, f1
    fmadds f0, f2, f0, f1
    fctiwz f0, f0
    stfd f0, 0x48(r1)
    lwz r0, 0x4c(r1)
    stb r0, 0x2(r3)
    lbz r4, 0x3(r29)
    lbz r0, 0x3(r30)
    stw r4, 0x54(r1)
    stw r0, 0x5c(r1)
    lfd f1, 0x50(r1)
    lfd f0, 0x58(r1)
    fsubs f1, f1, f3
    fsubs f0, f0, f3
    fsubs f0, f0, f1
    fmadds f0, f2, f0, f1
    fctiwz f0, f0
    stfd f0, 0x60(r1)
    lwz r0, 0x64(r1)
    stb r0, 0x3(r3)
    lbz r0, 0x0(r3)
    stb r0, 0x4(r3)
    lbz r0, 0x1(r3)
    stb r0, 0x5(r3)
    lbz r0, 0x2(r3)
    stb r0, 0x6(r3)
    lbz r0, 0x3(r3)
    stb r0, 0x7(r3)
    b @L_8013AD18
    @L_8013ACD8:
    lbz r0, 0x0(r29)
    stb r0, 0x0(r3)
    lbz r0, 0x1(r29)
    stb r0, 0x1(r3)
    lbz r0, 0x2(r29)
    stb r0, 0x2(r3)
    lbz r0, 0x3(r29)
    stb r0, 0x3(r3)
    lbz r0, 0x0(r30)
    stb r0, 0x4(r3)
    lbz r0, 0x1(r30)
    stb r0, 0x5(r3)
    lbz r0, 0x2(r30)
    stb r0, 0x6(r3)
    lbz r0, 0x3(r30)
    stb r0, 0x7(r3)
    @L_8013AD18:
    stw r31, 0x8(r3)
    lwz r4, 0x50(r28)
    cmplwi r4, 0x0
    beq @L_8013AD44
    b @L_8013AD30
    @L_8013AD2C:
    mr r4, r0
    @L_8013AD30:
    lwz r0, 0x10(r4)
    cmplwi r0, 0x0
    bne @L_8013AD2C
    stw r3, 0x10(r4)
    b @L_8013AD48
    @L_8013AD44:
    stw r3, 0x50(r28)
    @L_8013AD48:
    li r0, 0x0
    stw r0, 0x10(r3)
    @L_8013AD50:
    lmw r27, 0x6c(r1)
    li r3, 0x0
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
