    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r25, r4
    li r30, 0x0
    li r29, 0x0
    sth r3, 0x8(r1)
    li r31, 0x0
    sth r5, 0xa(r1)
    b @L_8010C6DC
@L_8010C67C:
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x9
    beq @L_8010C6D8
    clrlslwi r0, r31, 16, 1
    lhz r3, 0x8(r1)
    lhzx r4, r25, r0
    bl fn_8010C508
    rlwinm r0, r3, 0, 16, 31
    mr r26, r3
    cmplwi r0, 0x43
    mr r28, r26
    bne @L_8010C6B4
    li r3, 0x43
    b @L_8010C738
@L_8010C6B4:
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x41
    bne @L_8010C6C8
    addi r30, r30, 0x1
    b @L_8010C6D8
@L_8010C6C8:
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x42
    bne @L_8010C6D8
    addi r29, r29, 0x1
@L_8010C6D8:
    addi r31, r31, 0x1
@L_8010C6DC:
    lhz r0, 0xa(r1)
    rlwinm r3, r31, 0, 16, 31
    cmplw r3, r0
    blt @L_8010C67C
    rlwinm. r0, r30, 0, 16, 31
    bne @L_8010C704
    rlwinm. r0, r29, 0, 16, 31
    bne @L_8010C704
    li r3, 0x3f
    b @L_8010C738
@L_8010C704:
    rlwinm r3, r30, 0, 16, 31
    rlwinm r0, r29, 0, 16, 31
    cmplw r3, r0
    bne @L_8010C71C
    li r3, 0x3f
    b @L_8010C738
@L_8010C71C:
    rlwinm r3, r30, 0, 16, 31
    rlwinm r0, r29, 0, 16, 31
    cmplw r3, r0
    ble @L_8010C734
    li r3, 0x41
    b @L_8010C738
@L_8010C734:
    li r3, 0x42
@L_8010C738:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
