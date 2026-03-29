    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    lwz r30, 0x60(r28)
    bl fn_80105624
    mr r31, r3
    lhz r6, 0x6(r31)
    clrlwi r0, r6, 28
    cmpwi r0, 0x0
    beq @L_8003AC2C
    lbz r5, 0x95(r28)
    li r29, 0x1
    li r7, 0x0
    extsb r5, r5
    cmpwi r5, 0x0
    ble @L_8003A890
    cmpwi r5, 0x8
    subi r4, r5, 0x8
    ble @L_8003A878
    addi r0, r4, 0x7
    lis r3, 0x5f6
    srwi r0, r0, 3
    subi r3, r3, 0x1f00
    mtctr r0
    cmpwi r4, 0x0
    ble @L_8003A878
    @L_8003A86C:
    mullw r29, r29, r3
    addi r7, r7, 0x8
    bdnz @L_8003A86C
    @L_8003A878:
    subf r0, r7, r5
    mtctr r0
    cmpw r7, r5
    bge @L_8003A890
    @L_8003A888:
    mulli r29, r29, 0xa
    bdnz @L_8003A888
    @L_8003A890:
    clrlwi r0, r6, 31
    cmpwi r0, 0x0
    beq @L_8003AA2C
    lwz r6, 0x0(r30)
    lwz r0, 0x4(r30)
    cmpw r6, r0
    beq @L_8003AA2C
    cmpwi r29, 0xa
    bge @L_8003A8EC
    lwz r0, lbl_8047A4C8(r13)
    add r3, r0, r29
    stw r3, lbl_8047A4C8(r13)
    lwz r0, 0x0(r30)
    cmpw r3, r0
    ble @L_8003A8D4
    lwz r0, 0x4(r30)
    stw r0, lbl_8047A4C8(r13)
    @L_8003A8D4:
    li r3, 0x3c5
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    b @L_8003AA2C
    @L_8003A8EC:
    lwz r5, lbl_8047A4C8(r13)
    lis r3, 0x6666
    addi r0, r3, 0x6667
    li r7, 0x9
    divw r4, r5, r29
    mulhw r0, r0, r4
    srawi r0, r0, 2
    srwi r3, r0, 31
    add r0, r0, r3
    mulli r0, r0, 0xa
    subf r4, r0, r4
    mullw r0, r4, r29
    mulli r3, r29, 0x9
    subf r5, r0, r5
    add r0, r5, r3
    cmpw r0, r6
    ble @L_8003A9E8
    subf r3, r29, r3
    li r7, 0x8
    add r0, r5, r3
    cmpw r0, r6
    ble @L_8003A9E8
    subf r3, r29, r3
    li r7, 0x7
    add r0, r5, r3
    cmpw r0, r6
    ble @L_8003A9E8
    subf r3, r29, r3
    li r7, 0x6
    add r0, r5, r3
    cmpw r0, r6
    ble @L_8003A9E8
    subf r3, r29, r3
    li r7, 0x5
    add r0, r5, r3
    cmpw r0, r6
    ble @L_8003A9E8
    subf r3, r29, r3
    li r7, 0x4
    add r0, r5, r3
    cmpw r0, r6
    ble @L_8003A9E8
    subf r3, r29, r3
    li r7, 0x3
    add r0, r5, r3
    cmpw r0, r6
    ble @L_8003A9E8
    subf r3, r29, r3
    li r7, 0x2
    add r0, r5, r3
    cmpw r0, r6
    ble @L_8003A9E8
    subf r3, r29, r3
    li r7, 0x1
    add r0, r5, r3
    cmpw r0, r6
    ble @L_8003A9E8
    subf r3, r29, r3
    li r7, 0x0
    add r0, r5, r3
    cmpw r0, r6
    ble @L_8003A9E8
    li r7, -0x1
    @L_8003A9E8:
    addi r4, r4, 0x1
    cmpw r4, r7
    ble @L_8003A9F8
    li r4, 0x0
    @L_8003A9F8:
    mullw r0, r4, r29
    add r0, r5, r0
    stw r0, lbl_8047A4C8(r13)
    lwz r3, 0x4(r30)
    cmpw r0, r3
    bge @L_8003AA18
    stw r3, lbl_8047A4C8(r13)
    b @L_8003AA2C
    @L_8003AA18:
    li r3, 0x3c5
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    @L_8003AA2C:
    lhz r0, 0x6(r31)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_8003ABCC
    lwz r6, 0x0(r30)
    lwz r0, 0x4(r30)
    cmpw r6, r0
    beq @L_8003ABCC
    cmpwi r29, 0xa
    bge @L_8003AA8C
    lwz r0, lbl_8047A4C8(r13)
    subf r3, r29, r0
    stw r3, lbl_8047A4C8(r13)
    lwz r0, 0x4(r30)
    cmpw r3, r0
    bge @L_8003AA74
    lwz r0, 0x0(r30)
    stw r0, lbl_8047A4C8(r13)
    @L_8003AA74:
    li r3, 0x3c5
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    b @L_8003ABCC
    @L_8003AA8C:
    lwz r5, lbl_8047A4C8(r13)
    lis r3, 0x6666
    addi r0, r3, 0x6667
    li r7, 0x9
    divw r4, r5, r29
    mulhw r0, r0, r4
    srawi r0, r0, 2
    srwi r3, r0, 31
    add r0, r0, r3
    mulli r0, r0, 0xa
    subf r4, r0, r4
    mullw r0, r4, r29
    mulli r3, r29, 0x9
    subf r5, r0, r5
    add r0, r5, r3
    cmpw r0, r6
    ble @L_8003AB88
    subf r3, r29, r3
    li r7, 0x8
    add r0, r5, r3
    cmpw r0, r6
    ble @L_8003AB88
    subf r3, r29, r3
    li r7, 0x7
    add r0, r5, r3
    cmpw r0, r6
    ble @L_8003AB88
    subf r3, r29, r3
    li r7, 0x6
    add r0, r5, r3
    cmpw r0, r6
    ble @L_8003AB88
    subf r3, r29, r3
    li r7, 0x5
    add r0, r5, r3
    cmpw r0, r6
    ble @L_8003AB88
    subf r3, r29, r3
    li r7, 0x4
    add r0, r5, r3
    cmpw r0, r6
    ble @L_8003AB88
    subf r3, r29, r3
    li r7, 0x3
    add r0, r5, r3
    cmpw r0, r6
    ble @L_8003AB88
    subf r3, r29, r3
    li r7, 0x2
    add r0, r5, r3
    cmpw r0, r6
    ble @L_8003AB88
    subf r3, r29, r3
    li r7, 0x1
    add r0, r5, r3
    cmpw r0, r6
    ble @L_8003AB88
    subf r3, r29, r3
    li r7, 0x0
    add r0, r5, r3
    cmpw r0, r6
    ble @L_8003AB88
    li r7, -0x1
    @L_8003AB88:
    subi r4, r4, 0x1
    cmpwi r4, 0x0
    bge @L_8003AB98
    mr r4, r7
    @L_8003AB98:
    mullw r0, r4, r29
    add r0, r5, r0
    stw r0, lbl_8047A4C8(r13)
    lwz r3, 0x4(r30)
    cmpw r0, r3
    bge @L_8003ABB8
    stw r3, lbl_8047A4C8(r13)
    b @L_8003ABCC
    @L_8003ABB8:
    li r3, 0x3c5
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    @L_8003ABCC:
    lhz r0, 0x6(r31)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_8003ABFC
    lbz r3, 0x95(r28)
    subi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0x95(r28)
    cmpwi r0, 0x0
    bge @L_8003ABFC
    li r0, 0x0
    stb r0, 0x95(r28)
    @L_8003ABFC:
    lhz r0, 0x6(r31)
    rlwinm r0, r0, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_8003AC2C
    lbz r3, 0x95(r28)
    addi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0x95(r28)
    cmpwi r0, 0x3
    blt @L_8003AC2C
    li r0, 0x2
    stb r0, 0x95(r28)
    @L_8003AC2C:
    li r3, 0x0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
