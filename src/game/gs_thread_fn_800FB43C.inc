    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r27, 0x2c(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    cmplwi r30, 0x0
    lis r3, lbl_80402418@ha
    addi r31, r3, lbl_80402418@l
    bne @L_800FB470
    li r27, 0x0
    b @L_800FB500
@L_800FB470:
    lwz r3, lbl_80478B08(r13)
    srwi r4, r30, 20
    clrlwi r5, r30, 12
    lwz r9, 0x8(r3)
    b @L_800FB4F4
@L_800FB484:
    lhz r0, 0x0(r9)
    cmplw r0, r4
    bne @L_800FB4F0
    lhz r6, 0x4(r9)
    addi r8, r9, 0x10
    li r7, 0x0
    b @L_800FB4E8
@L_800FB4A0:
    add r0, r7, r6
    srwi r3, r0, 1
    slwi r10, r3, 3
    lwzx r0, r8, r10
    cmplw r0, r5
    bne @L_800FB4D8
    addi r0, r1, 0x8
    cmplwi r0, 0x0
    beq @L_800FB4C8
    stw r9, 0x8(r1)
@L_800FB4C8:
    add r3, r8, r10
    lwz r0, 0x4(r3)
    add r27, r9, r0
    b @L_800FB500
@L_800FB4D8:
    bge @L_800FB4E4
    addi r7, r3, 0x1
    b @L_800FB4E8
@L_800FB4E4:
    mr r6, r3
@L_800FB4E8:
    cmplw r7, r6
    blt @L_800FB4A0
@L_800FB4F0:
    lwz r9, 0x8(r9)
@L_800FB4F4:
    cmplwi r9, 0x0
    bne @L_800FB484
    li r27, 0x0
@L_800FB500:
    cmplwi r27, 0x0
    bne @L_800FB510
    li r3, -0x1
    b @L_800FB66C
@L_800FB510:
    mr r3, r31
    li r4, 0x0
    li r5, 0x68
    bl memset
    lis r4, 0x4330
    xoris r0, r28, 0x8000
    stw r0, 0x14(r1)
    xoris r3, r29, 0x8000
    lfs f3, lbl_8047CD08(r2)
    li r6, 0x1
    stw r4, 0x10(r1)
    li r5, -0x1
    lfd f2, lbl_8047CD10(r2)
    li r7, 0x0
    lfd f0, 0x10(r1)
    li r0, 0x3
    stw r3, 0x1c(r1)
    fsubs f1, f0, f2
    lwz r3, 0x8(r1)
    stw r4, 0x18(r1)
    lwz r8, lbl_80478B08(r13)
    lfd f0, 0x18(r1)
    stb r6, 0x0(r31)
    fsubs f0, f0, f2
    stfs f3, 0x60(r31)
    stfs f3, 0x64(r31)
    stw r5, 0x24(r31)
    stw r27, 0x28(r31)
    stw r27, 0x2c(r31)
    stw r27, 0x30(r31)
    lbz r3, 0x3(r3)
    sth r3, 0x20(r31)
    stw r30, 0x1c(r31)
    stfs f1, 0x4(r31)
    stfs f0, 0x8(r31)
    sth r7, 0x18(r31)
    sth r7, 0x1a(r31)
    stb r0, 0x44(r31)
    stw r5, 0x24(r31)
    stb r6, 0x2(r31)
    lhz r0, 0x4(r8)
    clrlwi r4, r3, 16
    mtctr r0
    cmpwi r0, 0x0
    ble @L_800FB658
@L_800FB5C4:
    lwz r0, 0x24(r8)
    add r3, r0, r7
    lhz r0, 0x0(r3)
    cmplw r0, r4
    bne @L_800FB650
    lbz r0, 0x2(r3)
    cmplwi r4, 0x0
    stb r0, 0x22(r31)
    lbz r3, 0x3(r3)
    stb r3, 0x23(r31)
    bne @L_800FB5FC
    li r0, 0xb
    stb r0, 0x42(r31)
    b @L_800FB658
@L_800FB5FC:
    cmplwi r4, 0x1
    beq @L_800FB608
    bne @L_800FB614
@L_800FB608:
    li r0, 0x6
    stb r0, 0x42(r31)
    b @L_800FB658
@L_800FB614:
    lis r0, 0x4330
    stw r3, 0x1c(r1)
    lfd f2, lbl_8047CD28(r2)
    stw r0, 0x18(r1)
    lfd f3, lbl_8047CD20(r2)
    lfd f1, 0x18(r1)
    lfd f0, lbl_8047CD18(r2)
    fsub f1, f1, f2
    fmadd f0, f3, f1, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    extsb r0, r0
    stb r0, 0x42(r31)
    b @L_800FB658
@L_800FB650:
    addi r7, r7, 0x8
    bdnz @L_800FB5C4
@L_800FB658:
    lbz r4, 0x44(r31)
    mr r3, r31
    li r5, 0x0
    li r6, 0x0
    bl fn_800FC7E0
@L_800FB66C:
    lmw r27, 0x2c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
