    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r27, 0x2c(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    bl fn_8010C3FC
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_8010BDA0
    li r3, -0x2
    b @L_8010C20C
@L_8010BDA0:
    lwz r3, lbl_8047AD4C(r13)
    li r30, 0x0
    lwz r0, lbl_8047AD48(r13)
    mtctr r0
    cmpwi r0, 0x0
    ble @L_8010BDD0
@L_8010BDB8:
    lwz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8010BDD0
    addi r3, r3, 0x10
    addi r30, r30, 0x1
    bdnz @L_8010BDB8
@L_8010BDD0:
    mr r3, r27
    bl fn_8010C3FC
    lwz r7, lbl_8047AD4C(r13)
    li r4, 0x0
    lwz r6, lbl_8047AD48(r13)
    mr r5, r7
    mtctr r6
    cmpwi r6, 0x0
    ble @L_8010BE10
@L_8010BDF4:
    lwz r0, 0x0(r5)
    cmplw r3, r0
    bne @L_8010BE04
    b @L_8010BE14
@L_8010BE04:
    addi r5, r5, 0x10
    addi r4, r4, 0x1
    bdnz @L_8010BDF4
@L_8010BE10:
    li r4, -0x1
@L_8010BE14:
    cmpwi r4, 0x0
    bge @L_8010C09C
    cmpw r30, r6
    bne @L_8010C054
    lbz r0, 0x6(r7)
    lwz r4, 0x0(r7)
    lwz r5, 0x4(r7)
    cmplwi r0, 0x2
    lwz r3, 0x8(r7)
    lwz r0, 0xc(r7)
    stw r4, 0x18(r1)
    stw r5, 0x1c(r1)
    stw r3, 0x20(r1)
    stw r0, 0x24(r1)
    bne @L_8010BE58
    li r3, 0x5c0
    bl fn_800F9210
@L_8010BE58:
    subi r0, r30, 0x1
    li r4, 0x0
    cmpwi r0, 0x0
    ble @L_8010C020
    cmpwi r0, 0x8
    subi r5, r30, 0x9
    ble @L_8010BFD8
    li r3, 0x0
    addi r0, r5, 0x7
    rlwinm r0, r0, 29, 3, 31
    mtctr r0
    cmpwi r5, 0x0
    ble @L_8010BFD8
@L_8010BE8C:
    lwz r0, lbl_8047AD4C(r13)
    addi r4, r4, 0x8
    add r6, r0, r3
    lwz r5, 0x10(r6)
    lwz r0, 0x14(r6)
    stw r5, 0x0(r6)
    stw r0, 0x4(r6)
    lwz r5, 0x18(r6)
    lwz r0, 0x1c(r6)
    stw r5, 0x8(r6)
    stw r0, 0xc(r6)
    lwz r0, lbl_8047AD4C(r13)
    add r6, r0, r3
    lwz r5, 0x20(r6)
    lwz r0, 0x24(r6)
    stw r5, 0x10(r6)
    stw r0, 0x14(r6)
    lwz r5, 0x28(r6)
    lwz r0, 0x2c(r6)
    stw r5, 0x18(r6)
    stw r0, 0x1c(r6)
    lwz r0, lbl_8047AD4C(r13)
    add r6, r0, r3
    lwz r5, 0x30(r6)
    lwz r0, 0x34(r6)
    stw r5, 0x20(r6)
    stw r0, 0x24(r6)
    lwz r5, 0x38(r6)
    lwz r0, 0x3c(r6)
    stw r5, 0x28(r6)
    stw r0, 0x2c(r6)
    lwz r0, lbl_8047AD4C(r13)
    add r6, r0, r3
    lwz r5, 0x40(r6)
    lwz r0, 0x44(r6)
    stw r5, 0x30(r6)
    stw r0, 0x34(r6)
    lwz r5, 0x48(r6)
    lwz r0, 0x4c(r6)
    stw r5, 0x38(r6)
    stw r0, 0x3c(r6)
    lwz r0, lbl_8047AD4C(r13)
    add r6, r0, r3
    lwz r5, 0x50(r6)
    lwz r0, 0x54(r6)
    stw r5, 0x40(r6)
    stw r0, 0x44(r6)
    lwz r5, 0x58(r6)
    lwz r0, 0x5c(r6)
    stw r5, 0x48(r6)
    stw r0, 0x4c(r6)
    lwz r0, lbl_8047AD4C(r13)
    add r6, r0, r3
    lwz r5, 0x60(r6)
    lwz r0, 0x64(r6)
    stw r5, 0x50(r6)
    stw r0, 0x54(r6)
    lwz r5, 0x68(r6)
    lwz r0, 0x6c(r6)
    stw r5, 0x58(r6)
    stw r0, 0x5c(r6)
    lwz r0, lbl_8047AD4C(r13)
    add r6, r0, r3
    lwz r5, 0x70(r6)
    lwz r0, 0x74(r6)
    stw r5, 0x60(r6)
    stw r0, 0x64(r6)
    lwz r5, 0x78(r6)
    lwz r0, 0x7c(r6)
    stw r5, 0x68(r6)
    stw r0, 0x6c(r6)
    lwz r0, lbl_8047AD4C(r13)
    add r6, r0, r3
    addi r3, r3, 0x80
    lwz r5, 0x80(r6)
    lwz r0, 0x84(r6)
    stw r5, 0x70(r6)
    stw r0, 0x74(r6)
    lwz r5, 0x88(r6)
    lwz r0, 0x8c(r6)
    stw r5, 0x78(r6)
    stw r0, 0x7c(r6)
    bdnz @L_8010BE8C
@L_8010BFD8:
    rlwinm r5, r4, 4, 0, 27
    subi r3, r30, 0x1
    subf r0, r4, r3
    mtctr r0
    cmpw r4, r3
    bge @L_8010C020
@L_8010BFF0:
    lwz r0, lbl_8047AD4C(r13)
    add r4, r0, r5
    addi r5, r5, 0x10
    lwz r3, 0x10(r4)
    lwz r0, 0x14(r4)
    stw r3, 0x0(r4)
    stw r0, 0x4(r4)
    lwz r3, 0x18(r4)
    lwz r0, 0x1c(r4)
    stw r3, 0x8(r4)
    stw r0, 0xc(r4)
    bdnz @L_8010BFF0
@L_8010C020:
    lwz r3, lbl_8047AD48(r13)
    lwz r5, lbl_8047AD4C(r13)
    subi r30, r3, 0x1
    lwz r0, 0x18(r1)
    rlwinm r3, r30, 4, 0, 27
    lwz r4, 0x1c(r1)
    add r5, r5, r3
    lwz r3, 0x20(r1)
    stw r0, 0x0(r5)
    lwz r0, 0x24(r1)
    stw r4, 0x4(r5)
    stw r3, 0x8(r5)
    stw r0, 0xc(r5)
@L_8010C054:
    lwz r3, lbl_8047AD4C(r13)
    rlwinm r6, r30, 4, 0, 27
    li r5, 0x0
    mr r4, r30
    stwx r31, r3, r6
    lwz r0, lbl_8047AD4C(r13)
    add r3, r0, r6
    sth r27, 0x4(r3)
    lwz r0, lbl_8047AD4C(r13)
    add r3, r0, r6
    stb r5, 0x6(r3)
    lwz r0, lbl_8047AD4C(r13)
    add r3, r0, r6
    stw r28, 0x8(r3)
    lwz r0, lbl_8047AD4C(r13)
    add r3, r0, r6
    stw r29, 0xc(r3)
    b @L_8010C208
@L_8010C09C:
    rlwinm r8, r4, 4, 0, 27
    add r7, r7, r8
    lwz r6, 0x0(r7)
    lwz r5, 0x4(r7)
    lwz r3, 0x8(r7)
    lwz r0, 0xc(r7)
    stw r6, 0x8(r1)
    stw r5, 0xc(r1)
    stw r3, 0x10(r1)
    stw r0, 0x14(r1)
    subi r0, r30, 0x1
    cmpw r4, r0
    subf r5, r4, r0
    bge @L_8010C1DC
    rlwinm r3, r5, 30, 2, 31
    mr r0, r5
    cmplwi r3, 0x0
    mtctr r3
    beq @L_8010C1A4
@L_8010C0E8:
    lwz r3, lbl_8047AD4C(r13)
    add r7, r3, r8
    addi r8, r8, 0x10
    lwz r6, 0x10(r7)
    lwz r3, 0x14(r7)
    stw r6, 0x0(r7)
    stw r3, 0x4(r7)
    lwz r6, 0x18(r7)
    lwz r3, 0x1c(r7)
    stw r6, 0x8(r7)
    stw r3, 0xc(r7)
    lwz r3, lbl_8047AD4C(r13)
    add r7, r3, r8
    addi r8, r8, 0x10
    lwz r6, 0x10(r7)
    lwz r3, 0x14(r7)
    stw r6, 0x0(r7)
    stw r3, 0x4(r7)
    lwz r6, 0x18(r7)
    lwz r3, 0x1c(r7)
    stw r6, 0x8(r7)
    stw r3, 0xc(r7)
    lwz r3, lbl_8047AD4C(r13)
    add r7, r3, r8
    addi r8, r8, 0x10
    lwz r6, 0x10(r7)
    lwz r3, 0x14(r7)
    stw r6, 0x0(r7)
    stw r3, 0x4(r7)
    lwz r6, 0x18(r7)
    lwz r3, 0x1c(r7)
    stw r6, 0x8(r7)
    stw r3, 0xc(r7)
    lwz r3, lbl_8047AD4C(r13)
    add r7, r3, r8
    addi r8, r8, 0x10
    lwz r6, 0x10(r7)
    lwz r3, 0x14(r7)
    stw r6, 0x0(r7)
    stw r3, 0x4(r7)
    lwz r6, 0x18(r7)
    lwz r3, 0x1c(r7)
    stw r6, 0x8(r7)
    stw r3, 0xc(r7)
    bdnz @L_8010C0E8
    andi. r5, r5, 0x3
    beq @L_8010C1D8
@L_8010C1A4:
    mtctr r5
@L_8010C1A8:
    lwz r3, lbl_8047AD4C(r13)
    add r7, r3, r8
    addi r8, r8, 0x10
    lwz r6, 0x10(r7)
    lwz r3, 0x14(r7)
    stw r6, 0x0(r7)
    stw r3, 0x4(r7)
    lwz r6, 0x18(r7)
    lwz r3, 0x1c(r7)
    stw r6, 0x8(r7)
    stw r3, 0xc(r7)
    bdnz @L_8010C1A8
@L_8010C1D8:
    add r4, r4, r0
@L_8010C1DC:
    lwz r5, lbl_8047AD4C(r13)
    rlwinm r0, r4, 4, 0, 27
    lwz r3, 0x8(r1)
    add r5, r5, r0
    lwz r0, 0xc(r1)
    stw r3, 0x0(r5)
    lwz r3, 0x10(r1)
    stw r0, 0x4(r5)
    lwz r0, 0x14(r1)
    stw r3, 0x8(r5)
    stw r0, 0xc(r5)
@L_8010C208:
    mr r3, r4
@L_8010C20C:
    lmw r27, 0x2c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr

