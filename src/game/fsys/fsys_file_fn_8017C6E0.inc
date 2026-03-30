    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stmw r17, 0x24(r1)
    mr r30, r3
    lwz r27, 0x40(r3)
    lwz r0, 0xf4(r3)
    cmplwi r27, 0x0
    stw r0, 0x18(r1)
    beq @L_8017C72C
    lwz r3, 0x18(r27)
    lwz r0, 0x18(r1)
    add r20, r27, r3
    lwz r3, 0x0(r20)
    slwi r0, r0, 2
    add r21, r27, r3
    lwzx r0, r21, r0
    add r22, r27, r0
    b @L_8017C730
    @L_8017C72C:
    li r22, 0x0
    @L_8017C730:
    lwz r0, 0xc(r22)
    mr r28, r22
    lwz r29, 0xfc(r30)
    clrrwi. r0, r0, 31
    beq @L_8017C764
    lwz r3, 0x4(r29)
    lwz r4, 0x14(r28)
    bl DCFlushRange
    li r3, 0x67
    li r0, 0x2
    stw r3, 0x48(r30)
    stw r0, 0x0(r29)
    b @L_8017C874
    @L_8017C764:
    lwz r3, 0x4(r29)
    lwz r4, 0x14(r28)
    bl DCFlushRange
    li r0, 0x6
    li r5, 0x0
    stw r0, 0x0(r29)
    lis r3, lbl_8036C2A0@ha
    mr r31, r28
    li r23, 0x0
    lwz r4, 0x20(r28)
    addi r26, r3, lbl_8036C2A0@l
    lwz r0, lbl_80478C48(r13)
    stw r5, 0x14(r1)
    stw r4, 0x8(r1)
    stw r0, 0xc(r1)
    b @L_8017C7C4
    @L_8017C7A4:
    lwz r3, 0x4(r26)
    lwz r0, 0x8(r1)
    cmplw r3, r0
    bne @L_8017C7BC
    mr r17, r26
    b @L_8017C7D4
    @L_8017C7BC:
    addi r26, r26, 0x10
    addi r23, r23, 0x1
    @L_8017C7C4:
    lwz r0, 0xc(r1)
    cmpw r23, r0
    blt @L_8017C7A4
    li r17, 0x0
    @L_8017C7D4:
    mr r25, r17
    lwz r3, 0x8(r31)
    lwz r0, 0xc(r17)
    stw r3, 0x10(r1)
    cmplwi r0, 0x0
    beq @L_8017C86C
    lwz r3, 0xf8(r30)
    lwz r4, 0x0(r31)
    bl fn_800F9318
    mr. r24, r3
    beq @L_8017C828
    lwz r0, 0xc(r31)
    clrrwi. r0, r0, 31
    beq @L_8017C81C
    lwz r4, 0x8(r31)
    mr r3, r24
    bl DCFlushRange
    b @L_8017C828
    @L_8017C81C:
    lwz r4, 0x14(r31)
    mr r3, r24
    bl DCFlushRange
    @L_8017C828:
    lwz r0, 0xc(r31)
    lwz r18, 0xc(r25)
    clrrwi. r0, r0, 31
    beq @L_8017C854
    mr r12, r18
    lwz r3, 0xf8(r30)
    lwz r4, 0x0(r31)
    lwz r5, 0x8(r31)
    mtctr r12
    bctrl
    b @L_8017C86C
    @L_8017C854:
    mr r12, r18
    lwz r3, 0xf8(r30)
    lwz r4, 0x0(r31)
    lwz r5, 0x14(r31)
    mtctr r12
    bctrl
    @L_8017C86C:
    li r0, 0x64
    stw r0, 0x48(r30)
    @L_8017C874:
    lmw r17, 0x24(r1)
    li r3, 0x0
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
