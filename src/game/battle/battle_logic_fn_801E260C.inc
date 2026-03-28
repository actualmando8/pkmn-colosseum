    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    lis r5, -32697
    cmplwi r7, 0x0
    stw r0, 0x54(r1)
    addi r5, r5, -21408
    li r0, 0x3e8
    stmw r21, 0x24(r1)
    lwz r7, 0x90(r5)
    mulli r7, r7, 0x28
    divwu r28, r7, r0
    beq @L_801E2B5C
    lis r7, -32697
    li r8, 0x0
    addi r31, r7, -23488
    lwz r7, 0x34(r31)
    lwz r0, 0x30(r31)
    addc r7, r7, r4
    addze r0, r0
    addc r10, r7, r6
    addze r9, r0
    stw r10, 0x3c(r31)
    stw r9, 0x38(r31)
    b @L_801E26B4
@L_801E266C:
    slwi r0, r7, 3
    add r7, r31, r0
    lwz r0, 0x4(r7)
    lwz r7, 0x0(r7)
    subfc r0, r0, r10
    subfe r0, r7, r9
    subfe r0, r3, r3
    neg. r0, r0
    bne @L_801E26C4
    lwz r7, 0x28(r31)
    addi r0, r7, 0x1
    cmpwi r0, 0x5
    stw r0, 0x28(r31)
    blt @L_801E26A8
    stw r8, 0x28(r31)
@L_801E26A8:
    lwz r7, 0xe4(r5)
    addi r0, r7, 0x1
    stw r0, 0xe4(r5)
@L_801E26B4:
    lwz r7, 0x28(r31)
    lwz r0, 0x2c(r31)
    cmpw r7, r0
    bne @L_801E266C
@L_801E26C4:
    add r0, r4, r6
    srwi r30, r28, 1
    cmplw r0, r30
    blt @L_801E2B5C
    lwz r29, -21424(r13)
    cmplw r3, r29
    bne @L_801E2900
    lis r3, -32697
    addi r27, r3, -21408
    lwz r0, 0x8c(r27)
    cmplwi r0, 0x2
    bne @L_801E27D0
    lis r3, -32697
    lwz r25, -21420(r13)
    addi r26, r3, -23488
    mr r24, r30
    lwz r23, 0x40(r26)
    lwz r22, 0x44(r26)
@L_801E270C:
    mr r3, r29
    mr r4, r25
    mr r5, r24
    addi r6, r1, 0x14
    bl fn_801E2B74
    lwz r0, 0x14(r1)
    addc r22, r22, r3
    addze r23, r23
    cmpwi r0, 0x0
    beq @L_801E27B4
    cmpwi r0, 0x1
    bne @L_801E2788
    cmplwi r25, 0x0
    slwi r0, r3, 1
    subf r24, r3, r24
    add r29, r29, r0
    beq @L_801E2754
    add r25, r25, r0
@L_801E2754:
    lwz r0, 0x2c(r31)
    slwi r0, r0, 3
    add r3, r26, r0
    stw r22, 0x4(r3)
    stw r23, 0x0(r3)
    lwz r3, 0x2c(r31)
    addi r0, r3, 0x1
    cmpwi r0, 0x5
    stw r0, 0x2c(r31)
    blt @L_801E270C
    li r0, 0x0
    stw r0, 0x2c(r31)
    b @L_801E270C
@L_801E2788:
    slwi r21, r24, 1
    mr r3, r29
    mr r5, r21
    li r4, 0x0
    bl memset
    cmplwi r25, 0x0
    beq @L_801E27B4
    mr r3, r25
    mr r5, r21
    li r4, 0x0
    bl memset
@L_801E27B4:
    lwz r3, 0x44(r26)
    lwz r0, 0x40(r26)
    addc r3, r3, r30
    addze r0, r0
    stw r3, 0x44(r26)
    stw r0, 0x40(r26)
    b @L_801E28A8
@L_801E27D0:
    lis r3, -32697
    mr r23, r30
    addi r26, r3, -23488
    li r22, 0x0
    lwz r24, 0x40(r26)
    lwz r25, 0x44(r26)
@L_801E27E8:
    mr r3, r29
    mr r4, r22
    mr r5, r23
    addi r6, r1, 0x10
    bl fn_801E2B74
    lwz r0, 0x10(r1)
    addc r25, r25, r3
    addze r24, r24
    cmpwi r0, 0x0
    beq @L_801E2890
    cmpwi r0, 0x1
    bne @L_801E2864
    cmplwi r22, 0x0
    slwi r0, r3, 1
    subf r23, r3, r23
    add r29, r29, r0
    beq @L_801E2830
    add r22, r22, r0
@L_801E2830:
    lwz r0, 0x2c(r31)
    slwi r0, r0, 3
    add r3, r26, r0
    stw r25, 0x4(r3)
    stw r24, 0x0(r3)
    lwz r3, 0x2c(r31)
    addi r0, r3, 0x1
    cmpwi r0, 0x5
    stw r0, 0x2c(r31)
    blt @L_801E27E8
    li r0, 0x0
    stw r0, 0x2c(r31)
    b @L_801E27E8
@L_801E2864:
    slwi r21, r23, 1
    mr r3, r29
    mr r5, r21
    li r4, 0x0
    bl memset
    cmplwi r22, 0x0
    beq @L_801E2890
    mr r3, r22
    mr r5, r21
    li r4, 0x0
    bl memset
@L_801E2890:
    lwz r3, 0x44(r26)
    lwz r0, 0x40(r26)
    addc r3, r3, r30
    addze r0, r0
    stw r3, 0x44(r26)
    stw r0, 0x40(r26)
@L_801E28A8:
    lwz r3, -21424(r13)
    mr r4, r28
    bl DCFlushRange
    lwz r3, -31520(r13)
    mr r5, r30
    li r4, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_8014E9B4
    lwz r0, 0x8c(r27)
    cmplwi r0, 0x2
    bne @L_801E2B3C
    lwz r3, -21420(r13)
    mr r4, r28
    bl DCFlushRange
    lwz r3, -31516(r13)
    mr r5, r30
    li r4, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_8014E9B4
    b @L_801E2B3C
@L_801E2900:
    lis r3, -32697
    addi r26, r3, -21408
    lwz r0, 0x8c(r26)
    cmplwi r0, 0x2
    bne @L_801E29FC
    lis r3, -32697
    slwi r4, r30, 1
    addi r27, r3, -23488
    lwz r0, -21420(r13)
    lwz r25, 0x40(r27)
    mr r24, r30
    lwz r21, 0x44(r27)
    add r23, r29, r4
    add r22, r0, r4
@L_801E2938:
    mr r3, r23
    mr r4, r22
    mr r5, r24
    addi r6, r1, 0xc
    bl fn_801E2B74
    lwz r0, 0xc(r1)
    addc r21, r21, r3
    addze r25, r25
    cmpwi r0, 0x0
    beq @L_801E29E0
    cmpwi r0, 0x1
    bne @L_801E29B4
    cmplwi r22, 0x0
    slwi r0, r3, 1
    subf r24, r3, r24
    add r23, r23, r0
    beq @L_801E2980
    add r22, r22, r0
@L_801E2980:
    lwz r0, 0x2c(r31)
    slwi r0, r0, 3
    add r3, r27, r0
    stw r21, 0x4(r3)
    stw r25, 0x0(r3)
    lwz r3, 0x2c(r31)
    addi r0, r3, 0x1
    cmpwi r0, 0x5
    stw r0, 0x2c(r31)
    blt @L_801E2938
    li r0, 0x0
    stw r0, 0x2c(r31)
    b @L_801E2938
@L_801E29B4:
    slwi r21, r24, 1
    mr r3, r23
    mr r5, r21
    li r4, 0x0
    bl memset
    cmplwi r22, 0x0
    beq @L_801E29E0
    mr r3, r22
    mr r5, r21
    li r4, 0x0
    bl memset
@L_801E29E0:
    lwz r3, 0x44(r27)
    lwz r0, 0x40(r27)
    addc r3, r3, r30
    addze r0, r0
    stw r3, 0x44(r27)
    stw r0, 0x40(r27)
    b @L_801E2ADC
@L_801E29FC:
    lis r3, -32697
    slwi r0, r30, 1
    addi r27, r3, -23488
    mr r21, r30
    lwz r22, 0x40(r27)
    add r24, r29, r0
    lwz r25, 0x44(r27)
    li r23, 0x0
@L_801E2A1C:
    mr r3, r24
    mr r4, r23
    mr r5, r21
    addi r6, r1, 0x8
    bl fn_801E2B74
    lwz r0, 0x8(r1)
    addc r25, r25, r3
    addze r22, r22
    cmpwi r0, 0x0
    beq @L_801E2AC4
    cmpwi r0, 0x1
    bne @L_801E2A98
    cmplwi r23, 0x0
    slwi r0, r3, 1
    subf r21, r3, r21
    add r24, r24, r0
    beq @L_801E2A64
    add r23, r23, r0
@L_801E2A64:
    lwz r0, 0x2c(r31)
    slwi r0, r0, 3
    add r3, r27, r0
    stw r25, 0x4(r3)
    stw r22, 0x0(r3)
    lwz r3, 0x2c(r31)
    addi r0, r3, 0x1
    cmpwi r0, 0x5
    stw r0, 0x2c(r31)
    blt @L_801E2A1C
    li r0, 0x0
    stw r0, 0x2c(r31)
    b @L_801E2A1C
@L_801E2A98:
    slwi r21, r21, 1
    mr r3, r24
    mr r5, r21
    li r4, 0x0
    bl memset
    cmplwi r23, 0x0
    beq @L_801E2AC4
    mr r3, r23
    mr r5, r21
    li r4, 0x0
    bl memset
@L_801E2AC4:
    lwz r3, 0x44(r27)
    lwz r0, 0x40(r27)
    addc r3, r3, r30
    addze r0, r0
    stw r3, 0x44(r27)
    stw r0, 0x40(r27)
@L_801E2ADC:
    lwz r0, -21424(r13)
    slwi r21, r30, 1
    mr r4, r28
    add r3, r0, r21
    bl DCFlushRange
    lwz r3, -31520(r13)
    mr r4, r30
    mr r5, r30
    li r6, 0x0
    li r7, 0x0
    bl fn_8014E9B4
    lwz r0, 0x8c(r26)
    cmplwi r0, 0x2
    bne @L_801E2B3C
    lwz r0, -21420(r13)
    mr r4, r30
    add r3, r0, r21
    bl DCFlushRange
    lwz r3, -31516(r13)
    mr r4, r30
    mr r5, r30
    li r6, 0x0
    li r7, 0x0
    bl fn_8014E9B4
@L_801E2B3C:
    lwz r4, 0x34(r31)
    mr r3, r30
    lwz r0, 0x30(r31)
    addc r4, r4, r30
    addze r0, r0
    stw r4, 0x34(r31)
    stw r0, 0x30(r31)
    b @L_801E2B60
@L_801E2B5C:
    li r3, 0x0
@L_801E2B60:
    lmw r21, 0x24(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
