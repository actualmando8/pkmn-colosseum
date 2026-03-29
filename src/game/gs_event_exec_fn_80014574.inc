    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    lwz r30, 0x60(r27)
    bl fn_80105624
    mr r31, r3
    lhz r6, 0x6(r31)
    clrlwi r0, r6, 28
    cmpwi r0, 0x0
    beq @L_80014A30
    lis r3, lbl_80266B58@ha
    li r5, 0x0
    addi r4, r3, lbl_80266B58@l
    lwz r3, 0x4(r30)
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_80014600
    addi r4, r4, 0xc
    li r5, 0x1
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_80014600
    addi r4, r4, 0xc
    li r5, 0x2
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_80014600
    addi r4, r4, 0xc
    li r5, 0x3
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_80014600
    li r5, 0x4
    @L_80014600:
    cmpwi r5, 0x4
    blt @L_80014610
    li r3, 0x0
    b @L_80014A34
    @L_80014610:
    mulli r3, r5, 0xc
    lis r4, lbl_80266B58@ha
    lbz r0, 0x95(r27)
    li r28, 0x1
    addi r4, r4, lbl_80266B58@l
    add r3, r4, r3
    lwz r29, 0x8(r3)
    extsb r0, r0
    li r7, 0x0
    subf r3, r0, r29
    subi r5, r3, 0x1
    cmpwi r5, 0x0
    ble @L_80014690
    cmpwi r5, 0x8
    subi r4, r3, 0x9
    ble @L_80014678
    addi r0, r4, 0x7
    lis r3, 0x5f6
    srwi r0, r0, 3
    subi r3, r3, 0x1f00
    mtctr r0
    cmpwi r4, 0x0
    ble @L_80014678
    @L_8001466C:
    mullw r28, r28, r3
    addi r7, r7, 0x8
    bdnz @L_8001466C
    @L_80014678:
    subf r0, r7, r5
    mtctr r0
    cmpw r7, r5
    bge @L_80014690
    @L_80014688:
    mulli r28, r28, 0xa
    bdnz @L_80014688
    @L_80014690:
    clrlwi r0, r6, 31
    cmpwi r0, 0x0
    beq @L_8001482C
    lwz r0, 0xc(r30)
    lwz r6, 0x8(r30)
    cmpw r0, r6
    beq @L_8001482C
    cmpwi r28, 0xa
    bge @L_800146EC
    lwz r0, lbl_8047A2FC(r13)
    add r3, r0, r28
    stw r3, lbl_8047A2FC(r13)
    lwz r0, 0x8(r30)
    cmpw r3, r0
    ble @L_800146D4
    lwz r0, 0xc(r30)
    stw r0, lbl_8047A2FC(r13)
    @L_800146D4:
    li r3, 0x23
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    b @L_8001482C
    @L_800146EC:
    lwz r5, lbl_8047A2FC(r13)
    lis r3, 0x6666
    addi r0, r3, 0x6667
    li r7, 0x9
    divw r4, r5, r28
    mulhw r0, r0, r4
    srawi r0, r0, 2
    srwi r3, r0, 31
    add r0, r0, r3
    mulli r0, r0, 0xa
    subf r4, r0, r4
    mullw r0, r4, r28
    mulli r3, r28, 0x9
    subf r5, r0, r5
    add r0, r5, r3
    cmpw r0, r6
    ble @L_800147E8
    subf r3, r28, r3
    li r7, 0x8
    add r0, r5, r3
    cmpw r0, r6
    ble @L_800147E8
    subf r3, r28, r3
    li r7, 0x7
    add r0, r5, r3
    cmpw r0, r6
    ble @L_800147E8
    subf r3, r28, r3
    li r7, 0x6
    add r0, r5, r3
    cmpw r0, r6
    ble @L_800147E8
    subf r3, r28, r3
    li r7, 0x5
    add r0, r5, r3
    cmpw r0, r6
    ble @L_800147E8
    subf r3, r28, r3
    li r7, 0x4
    add r0, r5, r3
    cmpw r0, r6
    ble @L_800147E8
    subf r3, r28, r3
    li r7, 0x3
    add r0, r5, r3
    cmpw r0, r6
    ble @L_800147E8
    subf r3, r28, r3
    li r7, 0x2
    add r0, r5, r3
    cmpw r0, r6
    ble @L_800147E8
    subf r3, r28, r3
    li r7, 0x1
    add r0, r5, r3
    cmpw r0, r6
    ble @L_800147E8
    subf r3, r28, r3
    li r7, 0x0
    add r0, r5, r3
    cmpw r0, r6
    ble @L_800147E8
    li r7, -0x1
    @L_800147E8:
    addi r4, r4, 0x1
    cmpw r4, r7
    ble @L_800147F8
    li r4, 0x0
    @L_800147F8:
    mullw r0, r4, r28
    add r0, r5, r0
    stw r0, lbl_8047A2FC(r13)
    lwz r3, 0xc(r30)
    cmpw r0, r3
    bge @L_80014818
    stw r3, lbl_8047A2FC(r13)
    b @L_8001482C
    @L_80014818:
    li r3, 0x23
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    @L_8001482C:
    lhz r0, 0x6(r31)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_800149CC
    lwz r0, 0xc(r30)
    lwz r6, 0x8(r30)
    cmpw r0, r6
    beq @L_800149CC
    cmpwi r28, 0xa
    bge @L_8001488C
    lwz r0, lbl_8047A2FC(r13)
    subf r3, r28, r0
    stw r3, lbl_8047A2FC(r13)
    lwz r0, 0xc(r30)
    cmpw r3, r0
    bge @L_80014874
    lwz r0, 0x8(r30)
    stw r0, lbl_8047A2FC(r13)
    @L_80014874:
    li r3, 0x23
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    b @L_800149CC
    @L_8001488C:
    lwz r5, lbl_8047A2FC(r13)
    lis r3, 0x6666
    addi r0, r3, 0x6667
    li r7, 0x9
    divw r4, r5, r28
    mulhw r0, r0, r4
    srawi r0, r0, 2
    srwi r3, r0, 31
    add r0, r0, r3
    mulli r0, r0, 0xa
    subf r4, r0, r4
    mullw r0, r4, r28
    mulli r3, r28, 0x9
    subf r5, r0, r5
    add r0, r5, r3
    cmpw r0, r6
    ble @L_80014988
    subf r3, r28, r3
    li r7, 0x8
    add r0, r5, r3
    cmpw r0, r6
    ble @L_80014988
    subf r3, r28, r3
    li r7, 0x7
    add r0, r5, r3
    cmpw r0, r6
    ble @L_80014988
    subf r3, r28, r3
    li r7, 0x6
    add r0, r5, r3
    cmpw r0, r6
    ble @L_80014988
    subf r3, r28, r3
    li r7, 0x5
    add r0, r5, r3
    cmpw r0, r6
    ble @L_80014988
    subf r3, r28, r3
    li r7, 0x4
    add r0, r5, r3
    cmpw r0, r6
    ble @L_80014988
    subf r3, r28, r3
    li r7, 0x3
    add r0, r5, r3
    cmpw r0, r6
    ble @L_80014988
    subf r3, r28, r3
    li r7, 0x2
    add r0, r5, r3
    cmpw r0, r6
    ble @L_80014988
    subf r3, r28, r3
    li r7, 0x1
    add r0, r5, r3
    cmpw r0, r6
    ble @L_80014988
    subf r3, r28, r3
    li r7, 0x0
    add r0, r5, r3
    cmpw r0, r6
    ble @L_80014988
    li r7, -0x1
    @L_80014988:
    subi r4, r4, 0x1
    cmpwi r4, 0x0
    bge @L_80014998
    mr r4, r7
    @L_80014998:
    mullw r0, r4, r28
    add r0, r5, r0
    stw r0, lbl_8047A2FC(r13)
    lwz r3, 0xc(r30)
    cmpw r0, r3
    bge @L_800149B8
    stw r3, lbl_8047A2FC(r13)
    b @L_800149CC
    @L_800149B8:
    li r3, 0x23
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    @L_800149CC:
    lhz r0, 0x6(r31)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_80014A00
    lbz r3, 0x95(r27)
    addi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0x95(r27)
    cmpw r0, r29
    blt @L_80014A00
    subi r0, r29, 0x1
    extsb r0, r0
    stb r0, 0x95(r27)
    @L_80014A00:
    lhz r0, 0x6(r31)
    rlwinm r0, r0, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_80014A30
    lbz r3, 0x95(r27)
    subi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0x95(r27)
    cmpwi r0, 0x0
    bge @L_80014A30
    li r0, 0x0
    stb r0, 0x95(r27)
    @L_80014A30:
    li r3, 0x0
    @L_80014A34:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
