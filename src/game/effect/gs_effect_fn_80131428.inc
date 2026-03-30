    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r5, -32714
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    addi r30, r5, 13760
    mr r24, r3
    mr r25, r4
    lwz r28, 0x8(r30)
    cmplwi r28, 0x0
    beq @L_80131504
    rlwinm r29, r25, 0, 16, 31
    lhz r27, 0x0(r28)
    mr r3, r29
    bl fn_800E3534
    rlwinm. r0, r3, 0, 16, 31
    mr r31, r3
    bne @L_80131478
    li r3, 0x0
    b @L_80131508
@L_80131478:
    bl fn_800E27B0
    lwz r4, 0x2c(r28)
    mr r26, r3
    cmplwi r4, 0x0
    stw r4, 0x8(r30)
    beq @L_80131498
    li r0, 0x0
    stw r0, 0x30(r4)
@L_80131498:
    mr r3, r28
    li r4, 0x0
    li r5, 0x34
    bl memset
    lis r3, -32714
    sth r27, 0x0(r28)
    addi r6, r3, 13760
    lwz r3, 0x10(r6)
    cmplwi r3, 0x0
    beq @L_801314C4
    stw r28, 0x30(r3)
@L_801314C4:
    lwz r5, 0x10(r6)
    li r0, 0x0
    mr r3, r26
    mr r4, r24
    stw r5, 0x2c(r28)
    mr r5, r29
    stw r28, 0x10(r6)
    stw r0, 0x30(r28)
    sth r31, 0x28(r28)
    stw r26, 0x24(r28)
    bl memcpy
    sth r25, 0x2(r28)
    li r0, 0x0
    stw r0, 0x4(r28)
    lhz r3, 0x0(r28)
    b @L_80131508
@L_80131504:
    li r3, 0x0
@L_80131508:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
