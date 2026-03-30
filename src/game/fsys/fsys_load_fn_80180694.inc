    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r21, 0x14(r1)
    mr. r25, r4
    mr r24, r3
    mr r28, r5
    stw r6, 0x8(r1)
    stw r7, 0xc(r1)
    cmplwi r28, 0x0
    beq @L_80180788
    lwz r30, lbl_8047B1D4(r13)
    li r29, 0x0
    b @L_801806F0
    @L_801806CC:
    lwz r0, 0x20(r30)
    cmpwi r0, 0x0
    bne @L_801806E8
    li r0, 0x1
    mr r26, r30
    stw r0, 0x20(r30)
    b @L_80180700
    @L_801806E8:
    addi r30, r30, 0x44
    addi r29, r29, 0x1
    @L_801806F0:
    lwz r0, lbl_8047B1D8(r13)
    cmplw r29, r0
    blt @L_801806CC
    li r26, 0x0
    @L_80180700:
    mr r31, r26
    bl OSDisableInterrupts
    li r0, 0x0
    mr r21, r3
    stw r0, 0x34(r31)
    li r5, 0x1
    addi r0, r28, 0x1f
    lwz r4, 0x8(r1)
    stw r5, 0x24(r31)
    clrrwi r22, r0, 5
    lwz r0, 0xc(r1)
    mr r3, r24
    stw r4, 0x38(r31)
    mr r4, r28
    stw r0, 0x3c(r31)
    stw r24, 0x28(r31)
    stw r25, 0x2c(r31)
    stw r22, 0x30(r31)
    bl DCFlushRange
    lis r3, fn_801808E4@ha
    addi r0, r28, 0x1f
    addi r10, r3, fn_801808E4@l
    mr r4, r31
    mr r3, r31
    mr r7, r24
    mr r8, r25
    clrrwi r9, r0, 5
    li r5, 0x0
    li r6, 0x0
    bl fn_800AE630
    mr r3, r21
    bl OSRestoreInterrupts
    mr r27, r31
    b @L_8018078C
    @L_80180788:
    li r27, 0x0
    @L_8018078C:
    mr r23, r27
    lwz r0, 0x44(r1)
    mr r3, r23
    lmw r21, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
