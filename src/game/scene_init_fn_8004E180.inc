    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r24, r4
    lwz r4, 0x60(r3)
    lwz r3, 0x8(r4)
    lwz r28, 0x4(r4)
    lwz r31, 0x0(r3)
    mr r3, r28
    bl fn_801D16F0
    bl fn_8017B2CC
    cmpwi r3, 0x1
    bne @L_8004E1C0
    li r0, 0x0
    b @L_8004E1C4
    @L_8004E1C0:
    li r0, 0x1
    @L_8004E1C4:
    cmpwi r0, 0x0
    bne @L_8004E1D4
    li r28, -0x1
    b @L_8004E21C
    @L_8004E1D4:
    mr r3, r28
    bl fn_801D16F0
    mr r27, r3
    bl fn_8017B448
    li r28, 0x0
    mr r29, r3
    mr r30, r28
    b @L_8004E214
    @L_8004E1F4:
    mr r3, r27
    mr r4, r30
    bl fn_8017B4BC
    bl fn_8017B5A4
    cmplwi r3, 0x9
    bne @L_8004E210
    addi r28, r28, 0x1
    @L_8004E210:
    addi r30, r30, 0x1
    @L_8004E214:
    cmpw r30, r29
    blt @L_8004E1F4
    @L_8004E21C:
    cmpwi r28, 0x0
    bgt @L_8004E22C
    li r3, 0x0
    b @L_8004E2CC
    @L_8004E22C:
    li r3, 0x36ce
    bl fn_800FA444
    srwi r26, r3, 16
    li r3, 0x36cf
    bl fn_800FA444
    srwi r25, r3, 16
    subi r0, r28, 0x1
    mullw r4, r25, r0
    lha r3, 0x54(r24)
    li r27, 0x0
    srwi r0, r3, 31
    add r0, r0, r3
    add r4, r26, r4
    srwi r3, r4, 31
    add r3, r3, r4
    srawi r3, r3, 1
    srawi r0, r0, 1
    subf r24, r3, r0
    lis r30, 0xaaab
    lis r29, 0xe6aa
    b @L_8004E2C0
    @L_8004E280:
    cmpw r27, r31
    bne @L_8004E2A4
    mr r3, r24
    addi r5, r29, 0xff
    li r4, 0x0
    li r6, 0x36ce
    bl fn_800FB680
    add r24, r24, r26
    b @L_8004E2BC
    @L_8004E2A4:
    mr r3, r24
    subi r5, r30, 0x5501
    li r4, 0x0
    li r6, 0x36cf
    bl fn_800FB680
    add r24, r24, r25
    @L_8004E2BC:
    addi r27, r27, 0x1
    @L_8004E2C0:
    cmpw r27, r28
    blt @L_8004E280
    li r3, 0x0
    @L_8004E2CC:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
