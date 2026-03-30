    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r24, r3
    mr r31, r4
    mr r27, r5
    cmplwi r24, 0x0
    lis r3, lbl_8026F1C8@ha
    addi r29, r3, lbl_8026F1C8@l
    beq @L_800832FC
    mr r3, r24
    b @L_80083308
@L_800832FC:
    li r3, 0x0
    li r4, 0xd
    bl fn_80129280
@L_80083308:
    extsb r28, r27
    mr r24, r3
    add r4, r31, r28
    addi r25, r3, 0x4000
    lbz r0, 0x61(r4)
    extsb r0, r0
    mulli r3, r0, 0x28
    addi r26, r3, 0x3ac
    add r26, r31, r26
    lhz r0, 0x0(r26)
    cmplwi r0, 0x0
    bne @L_80083348
    addi r3, r29, 0x0
    addi r5, r29, 0x10c
    li r4, 0x108
    bl fn_80196E10
@L_80083348:
    li r0, 0x1
    addi r3, r25, 0x4
    stb r0, 0x0(r25)
    mulli r0, r28, 0x5c
    lbz r4, 0x8(r31)
    add r30, r31, r0
    stb r4, 0x1(r25)
    addi r4, r30, 0x6e
    stb r27, 0x2(r25)
    bl fn_800CAA3C
    addi r3, r25, 0x60
    addi r4, r30, 0x182
    bl fn_800CAA3C
    addi r3, r25, 0xbc
    addi r4, r30, 0x296
    bl fn_800CAA3C
    mr r4, r26
    addi r3, r25, 0x118
    bl fn_800CAA3C
    lbz r5, 0xc(r26)
    add r4, r31, r28
    li r3, -0x1
    li r0, 0x0
    stb r5, 0x124(r25)
    li r27, 0x0
    lbz r4, 0x6a(r4)
    stb r4, 0x125(r25)
    lhz r4, 0x12(r26)
    sth r4, 0x126(r25)
    lhz r4, 0x14(r26)
    sth r4, 0x128(r25)
    lhz r4, 0x16(r26)
    sth r4, 0x12a(r25)
    lhz r4, 0x18(r26)
    sth r4, 0x12c(r25)
    lwz r4, 0x1c(r26)
    stw r4, 0x130(r25)
    lhz r4, 0x20(r26)
    sth r4, 0x134(r25)
    lbz r4, 0x24(r26)
    stb r4, 0x136(r25)
    stb r3, 0x1e0(r25)
    stb r0, 0x1e1(r25)
    stb r0, 0x1e2(r25)
    addi r30, r1, 0x8
@L_800833FC:
    cmplwi r24, 0x0
    beq @L_8008340C
    mr r3, r24
    b @L_80083418
@L_8008340C:
    li r3, 0x0
    li r4, 0xd
    bl fn_80129280
@L_80083418:
    cmplwi r30, 0x0
    addi r0, r3, 0x4000
    beq @L_8008342C
    li r4, 0x0
    stw r4, 0x8(r1)
@L_8008342C:
    li r4, 0x0
@L_80083430:
    addi r5, r3, 0x24
    cmplw r0, r5
    blt @L_800834C8
    lhz r5, 0x0(r3)
    cmplwi r5, 0x0
    beq @L_800834C8
    lbz r5, 0x1b(r3)
    extsb r5, r5
    cmpwi r5, 0x3
    bgt @L_80083478
    lbz r5, 0x1c(r3)
    extsb r5, r5
    cmpwi r5, 0x6
    bgt @L_80083478
    lbz r5, 0x1d(r3)
    extsb r5, r5
    cmpwi r5, 0x5
    ble @L_80083484
@L_80083478:
    li r0, 0x0
    sth r0, 0x0(r3)
    b @L_800834C8
@L_80083484:
    cmpw r4, r27
    bne @L_80083490
    stw r3, 0x8(r1)
@L_80083490:
    lbz r6, 0x1c(r3)
    addi r4, r4, 0x1
    lbz r5, 0x1d(r3)
    extsb r6, r6
    lbz r7, 0x1b(r3)
    extsb r5, r5
    mullw r5, r6, r5
    extsb r6, r7
    rlwinm r5, r5, 4, 0, 27
    addi r5, r5, 0x76
    mullw r5, r6, r5
    add r3, r5, r3
    addi r3, r3, 0x24
    b @L_80083430
@L_800834C8:
    cmpwi r27, 0x0
    bge @L_800834D4
    stw r3, 0x8(r1)
@L_800834D4:
    lwz r28, 0x8(r1)
    cmplwi r28, 0x0
    bne @L_800834F0
    addi r3, r29, 0x0
    li r4, 0x121
    addi r5, r2, -29984
    bl fn_80196E10
@L_800834F0:
    lbz r3, 0x1a(r28)
    lbz r0, 0x1(r25)
    cmplw r3, r0
    beq @L_80083508
    addi r27, r27, 0x1
    b @L_800833FC
@L_80083508:
    cmplwi r28, 0x0
    lbz r24, 0x2(r25)
    bne @L_80083524
    addi r3, r29, 0x0
    li r4, 0x17f
    addi r5, r2, -29984
    bl fn_80196E10
@L_80083524:
    extsb r3, r24
    li r4, 0x0
    cmpwi r3, 0x0
    blt @L_80083548
    lbz r0, 0x1b(r28)
    extsb r0, r0
    cmpw r3, r0
    bge @L_80083548
    li r4, 0x1
@L_80083548:
    cmpwi r4, 0x0
    bne @L_80083560
    addi r3, r29, 0x0
    addi r5, r29, 0x10
    li r4, 0x180
    bl fn_80196E10
@L_80083560:
    lbz r3, 0x1c(r28)
    extsb r4, r24
    lbz r0, 0x1d(r28)
    mr r30, r25
    extsb r3, r3
    li r27, 0x0
    extsb r0, r0
    mullw r0, r3, r0
    rlwinm r3, r0, 4, 0, 27
    addi r0, r3, 0x76
    mullw r0, r4, r0
    add r29, r28, r0
@L_80083590:
    addi r0, r27, 0xd
    lbzx r0, r26, r0
    extsb r0, r0
    cmpwi r0, 0x0
    bge @L_800835B0
    li r0, 0x0
    sth r0, 0x138(r30)
    b @L_8008364C
@L_800835B0:
    mulli r3, r0, 0x2a
    addi r4, r3, 0x514
    add r4, r31, r4
    lwz r3, 0x0(r4)
    lwz r0, 0x4(r4)
    stw r3, 0x138(r30)
    stw r0, 0x13c(r30)
    lwz r3, 0x8(r4)
    lwz r0, 0xc(r4)
    stw r3, 0x140(r30)
    stw r0, 0x144(r30)
    lwz r3, 0x10(r4)
    lwz r0, 0x14(r4)
    stw r3, 0x148(r30)
    stw r0, 0x14c(r30)
    lwz r3, 0x18(r4)
    lwz r0, 0x1c(r4)
    stw r3, 0x150(r30)
    stw r0, 0x154(r30)
    lwz r3, 0x20(r4)
    lwz r0, 0x24(r4)
    stw r3, 0x158(r30)
    stw r0, 0x15c(r30)
    lhz r0, 0x28(r4)
    sth r0, 0x160(r30)
    lbz r0, 0x2(r4)
    cmplwi r0, 0x0
    beq @L_8008364C
    extsb r0, r27
    stb r0, 0x1e0(r25)
    lbz r0, 0x28(r4)
    stb r0, 0x1e2(r25)
    lbz r0, 0x2(r4)
    stb r0, 0x1e1(r25)
    lhz r0, 0x0(r4)
    sth r0, 0x98(r29)
    lbz r3, 0x1e1(r25)
    lhz r4, 0x0(r4)
    bl fn_801EE1E0
@L_8008364C:
    addi r30, r30, 0x2a
    addi r27, r27, 0x1
    cmpwi r27, 0x4
    blt @L_80083590
    addi r3, r29, 0x88
    addi r4, r25, 0x118
    bl fn_800CAA3C
    lbz r3, 0x125(r25)
    li r0, 0x0
    addi r4, r25, 0x118
    stb r3, 0x94(r29)
    stb r0, 0x95(r29)
    lbz r0, 0x1e1(r25)
    stb r0, 0x96(r29)
    lbz r3, 0x1e1(r25)
    bl fn_801EE2B4
    lbz r3, 0x1e1(r25)
    lbz r4, 0x125(r25)
    bl fn_801EE10C
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

