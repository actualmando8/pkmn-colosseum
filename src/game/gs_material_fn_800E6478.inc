    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr r30, r3
    mr r31, r4
    lhz r0, 0x152(r3)
    cmplwi r0, 0x0
    bne @L_800E6578
    li r26, 0x0
    bl fn_800EE0E8
    mr r28, r3
    li r29, 0x0
    b @L_800E64D4
    @L_800E64B0:
    mr r3, r30
    mr r4, r29
    bl fn_800EE150
    mr r27, r3
    bl fn_800EE758
    add r26, r26, r3
    mr r3, r27
    bl fn_800EE828
    addi r29, r29, 0x1
    @L_800E64D4:
    cmpw r29, r28
    blt @L_800E64B0
    cmpwi r26, 0x0
    bne @L_800E64EC
    li r26, 0x0
    b @L_800E6570
    @L_800E64EC:
    slwi r3, r26, 2
    bl fn_800E3534
    clrlwi. r0, r3, 16
    mr r29, r3
    beq @L_800E6570
    bl fn_800E27B0
    stw r3, 0x14c(r30)
    mr r25, r3
    li r24, 0x0
    sth r29, 0x154(r30)
    sth r26, 0x150(r30)
    b @L_800E6568
    @L_800E651C:
    mr r3, r30
    mr r4, r24
    bl fn_800EE150
    mr r23, r3
    bl fn_800EE758
    mr r29, r3
    li r27, 0x0
    b @L_800E6554
    @L_800E653C:
    mr r3, r23
    mr r4, r27
    bl fn_800EE6B4
    stw r3, 0x0(r25)
    addi r27, r27, 0x1
    addi r25, r25, 0x4
    @L_800E6554:
    cmpw r27, r29
    blt @L_800E653C
    mr r3, r23
    bl fn_800EE828
    addi r24, r24, 0x1
    @L_800E6568:
    cmpw r24, r28
    blt @L_800E651C
    @L_800E6570:
    mr r24, r26
    b @L_800E657C
    @L_800E6578:
    lhz r24, 0x150(r30)
    @L_800E657C:
    lhz r3, 0x152(r30)
    li r23, 0x0
    addi r0, r3, 0x1
    sth r0, 0x152(r30)
    lwz r25, 0x14c(r30)
    b @L_800E65B0
    @L_800E6594:
    lwz r3, 0x0(r25)
    cmplwi r3, 0x0
    beq @L_800E65A8
    mr r4, r31
    bl fn_800DF498
    @L_800E65A8:
    addi r23, r23, 0x1
    addi r25, r25, 0x4
    @L_800E65B0:
    cmpw r23, r24
    blt @L_800E6594
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
