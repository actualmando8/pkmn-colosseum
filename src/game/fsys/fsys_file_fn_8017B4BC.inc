    nofralloc
    stwu r1, -0x50(r1)
    stmw r19, 0x1c(r1)
    li r28, 0x0
    lwz r31, lbl_8047B1B4(r13)
    lwz r31, lbl_8047B1B4(r13)
    b @L_8017B570
    @L_8017B4D4:
    lwz r29, 0x40(r31)
    cmplwi r29, 0x0
    beq @L_8017B4F4
    lwz r0, 0x18(r29)
    add r24, r29, r0
    lwz r0, 0x4(r24)
    add r25, r29, r0
    b @L_8017B4F8
    @L_8017B4F4:
    li r25, 0x0
    @L_8017B4F8:
    lwz r0, 0x48(r31)
    stw r25, 0x8(r1)
    cmpwi r0, 0x0
    beq @L_8017B568
    lwz r0, 0xf8(r31)
    cmplw r0, r3
    bne @L_8017B568
    lwz r0, 0x5c(r31)
    cmpwi r0, 0x0
    beq @L_8017B568
    lwz r30, 0x40(r31)
    lwz r0, 0x40(r31)
    cmplwi r30, 0x0
    stw r0, 0xc(r1)
    beq @L_8017B554
    lwz r5, 0x18(r30)
    slwi r0, r4, 2
    add r20, r30, r5
    lwz r5, 0x0(r20)
    add r21, r30, r5
    lwzx r0, r21, r0
    add r22, r30, r0
    b @L_8017B558
    @L_8017B554:
    li r22, 0x0
    @L_8017B558:
    lwz r0, 0x0(r22)
    li r27, 0x1
    stw r0, 0x10(r1)
    b @L_8017B584
    @L_8017B568:
    addi r31, r31, 0x140
    addi r28, r28, 0x1
    @L_8017B570:
    lis r5, lbl_80453FEC@ha
    lwz r0, lbl_80453FEC@l(r5)
    cmplw r28, r0
    blt @L_8017B4D4
    li r27, 0x0
    @L_8017B584:
    clrlwi. r0, r27, 24
    beq @L_8017B594
    lwz r3, 0x10(r1)
    b @L_8017B598
    @L_8017B594:
    li r3, 0x0
    @L_8017B598:
    lmw r19, 0x1c(r1)
    addi r1, r1, 0x50
    blr
