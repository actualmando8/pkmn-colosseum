    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r18, 0x8(r1)
    mr. r24, r4
    mr r23, r3
    mr r27, r5
    cmplwi r27, 0x0
    beq @L_8018053C
    lwz r30, lbl_8047B1D4(r13)
    li r28, 0x0
    b @L_801804A4
    @L_80180480:
    lwz r0, 0x20(r30)
    cmpwi r0, 0x0
    bne @L_8018049C
    li r0, 0x1
    mr r25, r30
    stw r0, 0x20(r30)
    b @L_801804B4
    @L_8018049C:
    addi r30, r30, 0x44
    addi r28, r28, 0x1
    @L_801804A4:
    lwz r0, lbl_8047B1D8(r13)
    cmplw r28, r0
    blt @L_80180480
    li r25, 0x0
    @L_801804B4:
    mr r31, r25
    bl OSDisableInterrupts
    li r0, 0x0
    mr r18, r3
    stw r0, 0x34(r31)
    li r5, 0x1
    addi r0, r27, 0x1f
    li r4, 0x0
    stw r5, 0x24(r31)
    li r5, 0x0
    clrrwi r19, r0, 5
    mr r3, r23
    stw r4, 0x38(r31)
    mr r4, r27
    stw r5, 0x3c(r31)
    stw r23, 0x28(r31)
    stw r24, 0x2c(r31)
    stw r19, 0x30(r31)
    bl DCFlushRange
    lis r3, fn_801808E4@ha
    addi r0, r27, 0x1f
    addi r10, r3, fn_801808E4@l
    mr r4, r31
    mr r3, r31
    mr r7, r23
    mr r8, r24
    clrrwi r9, r0, 5
    li r5, 0x0
    li r6, 0x0
    bl fn_800AE630
    mr r3, r18
    bl OSRestoreInterrupts
    mr r26, r31
    b @L_80180540
    @L_8018053C:
    li r26, 0x0
    @L_80180540:
    mr r21, r26
    mr r20, r21
    mr r22, r20
    @L_8018054C:
    mr r29, r22
    lwz r0, 0x24(r22)
    cmpwi r0, 0x1
    beq @L_80180564
    li r0, 0x0
    stw r0, 0x20(r29)
    @L_80180564:
    lwz r0, 0x20(r29)
    cmpwi r0, 0x0
    bne @L_8018054C
    lmw r18, 0x8(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
