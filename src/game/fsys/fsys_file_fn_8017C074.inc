    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stmw r17, 0x44(r1)
    mr r27, r3
    mr r30, r4
    mr r18, r6
    stw r5, 0x8(r1)
    lwz r29, 0x40(r3)
    cmplwi r29, 0x0
    beq @L_8017C0C4
    lwz r3, 0x18(r29)
    lwz r0, 0x8(r1)
    add r21, r29, r3
    lwz r3, 0x0(r21)
    slwi r0, r0, 2
    add r22, r29, r3
    lwzx r0, r22, r0
    add r23, r29, r0
    b @L_8017C0C8
    @L_8017C0C4:
    li r23, 0x0
    @L_8017C0C8:
    lwz r4, 0x4(r30)
    lis r3, lbl_8036C2A0@ha
    li r0, 0x0
    mr r31, r23
    stw r4, 0x38(r18)
    addi r28, r3, lbl_8036C2A0@l
    li r24, 0x0
    stw r0, 0x4(r30)
    lwz r3, 0x20(r23)
    lwz r0, lbl_80478C48(r13)
    stw r3, 0x28(r1)
    stw r0, 0x2c(r1)
    b @L_8017C11C
    @L_8017C0FC:
    lwz r3, 0x4(r28)
    lwz r0, 0x28(r1)
    cmplw r3, r0
    bne @L_8017C114
    mr r19, r28
    b @L_8017C12C
    @L_8017C114:
    addi r28, r28, 0x10
    addi r24, r24, 0x1
    @L_8017C11C:
    lwz r0, 0x2c(r1)
    cmpw r24, r0
    blt @L_8017C0FC
    li r19, 0x0
    @L_8017C12C:
    mr r26, r19
    lwz r0, 0x8(r19)
    cmplwi r0, 0x0
    beq @L_8017C164
    lwz r0, 0x8(r26)
    lwz r3, 0xf8(r27)
    stw r0, 0x30(r1)
    lwz r4, 0x0(r31)
    lwz r12, 0x30(r1)
    lwz r5, 0x8(r31)
    mtctr r12
    bctrl
    mr r25, r3
    b @L_8017C1B8
    @L_8017C164:
    lwz r3, 0x8(r31)
    li r4, 0x20
    lwz r5, 0x0(r31)
    li r7, 0x0
    addi r0, r3, 0x1f
    lwz r3, 0x8(r27)
    clrrwi r0, r0, 5
    stw r5, 0x18(r1)
    stw r3, 0x1c(r1)
    lwz r6, 0x18(r1)
    stw r0, 0x20(r1)
    lwz r5, 0x1c(r1)
    stw r0, 0x14(r1)
    lwz r3, 0x20(r1)
    bl fn_800F9418
    stw r3, 0x24(r1)
    lwz r0, 0x24(r1)
    stw r0, 0x10(r1)
    lwz r0, 0x10(r1)
    stw r0, 0xc(r1)
    lwz r25, 0xc(r1)
    @L_8017C1B8:
    stw r25, 0x4(r30)
    mr r17, r25
    stw r17, 0x4(r30)
    lmw r17, 0x44(r1)
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
