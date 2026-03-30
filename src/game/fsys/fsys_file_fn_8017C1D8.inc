    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stmw r17, 0x34(r1)
    mr r30, r3
    mr r27, r4
    mr r22, r6
    stw r5, 0x8(r1)
    lwz r29, 0x40(r3)
    cmplwi r29, 0x0
    beq @L_8017C228
    lwz r3, 0x18(r29)
    lwz r0, 0x8(r1)
    add r19, r29, r3
    lwz r3, 0x0(r19)
    slwi r0, r0, 2
    add r20, r29, r3
    lwzx r0, r20, r0
    add r21, r29, r0
    b @L_8017C22C
    @L_8017C228:
    li r21, 0x0
    @L_8017C22C:
    lwz r0, 0x38(r22)
    mr r31, r21
    stw r0, 0x20(r1)
    lwz r3, 0x20(r1)
    bl fn_800E202C
    clrlwi. r0, r3, 16
    mr r25, r3
    beq @L_8017C25C
    mr r3, r25
    bl fn_800E24B0
    mr r3, r25
    bl fn_800E209C
    @L_8017C25C:
    li r3, 0x0
    li r0, 0x0
    stw r3, 0x38(r22)
    stw r0, 0x100(r30)
    lwz r0, 0x4(r27)
    cmplwi r0, 0x0
    bne @L_8017C284
    li r0, 0x7
    stw r0, 0x0(r27)
    b @L_8017C380
    @L_8017C284:
    li r0, 0x6
    lis r3, lbl_8036C2A0@ha
    stw r0, 0x0(r27)
    addi r28, r3, lbl_8036C2A0@l
    li r4, 0x0
    li r23, 0x0
    lwz r3, 0x20(r31)
    lwz r0, lbl_80478C48(r13)
    stw r4, 0x1c(r1)
    stw r3, 0xc(r1)
    stw r0, 0x10(r1)
    b @L_8017C2D4
    @L_8017C2B4:
    lwz r3, 0x4(r28)
    lwz r0, 0xc(r1)
    cmplw r3, r0
    bne @L_8017C2CC
    stw r28, 0x14(r1)
    b @L_8017C2E8
    @L_8017C2CC:
    addi r28, r28, 0x10
    addi r23, r23, 0x1
    @L_8017C2D4:
    lwz r0, 0x10(r1)
    cmpw r23, r0
    blt @L_8017C2B4
    li r0, 0x0
    stw r0, 0x14(r1)
    @L_8017C2E8:
    lwz r26, 0x14(r1)
    lwz r3, 0x8(r31)
    lwz r0, 0xc(r26)
    stw r3, 0x18(r1)
    cmplwi r0, 0x0
    beq @L_8017C380
    lwz r3, 0xf8(r30)
    lwz r4, 0x0(r31)
    bl fn_800F9318
    mr. r24, r3
    beq @L_8017C33C
    lwz r0, 0xc(r31)
    clrrwi. r0, r0, 31
    beq @L_8017C330
    lwz r4, 0x8(r31)
    mr r3, r24
    bl DCFlushRange
    b @L_8017C33C
    @L_8017C330:
    lwz r4, 0x14(r31)
    mr r3, r24
    bl DCFlushRange
    @L_8017C33C:
    lwz r0, 0xc(r31)
    lwz r17, 0xc(r26)
    clrrwi. r0, r0, 31
    beq @L_8017C368
    mr r12, r17
    lwz r3, 0xf8(r30)
    lwz r4, 0x0(r31)
    lwz r5, 0x8(r31)
    mtctr r12
    bctrl
    b @L_8017C380
    @L_8017C368:
    mr r12, r17
    lwz r3, 0xf8(r30)
    lwz r4, 0x0(r31)
    lwz r5, 0x14(r31)
    mtctr r12
    bctrl
    @L_8017C380:
    lmw r17, 0x34(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
