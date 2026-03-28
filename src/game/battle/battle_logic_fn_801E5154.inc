    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r4, -32697
    lis r3, -32697
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    addi r30, r4, -15984
    addi r31, r3, -21408
@L_801E5174:
    lbz r0, 0xa7(r31)
    cmplwi r0, 0x0
    beq @L_801E52E4
    b @L_801E52D8
@L_801E5184:
    bl fn_801E1B84
    mr r28, r3
    lwz r5, 0x50(r31)
    lwz r3, 0xc0(r31)
    lwz r4, 0x4(r28)
    subi r0, r5, 0x1
    add r4, r4, r3
    divwu r3, r4, r5
    mullw r3, r3, r5
    subf r3, r3, r4
    cmplw r3, r0
    bne @L_801E52BC
    lbz r0, 0xa6(r31)
    clrlwi. r0, r0, 31
    bne @L_801E52BC
    lwz r0, 0x6c(r31)
    addi r3, r30, 0x38
    lwz r7, 0x0(r28)
    addi r4, r1, 0xc
    slwi r6, r0, 2
    li r5, 0x1
    addi r26, r6, 0x8
    addi r25, r7, 0x8
    add r26, r7, r26
    bl fn_8009F2F8
    lis r3, -32697
    lwz r29, 0xc(r1)
    addi r27, r3, -21408
    li r24, 0x0
    b @L_801E5294
@L_801E51FC:
    add r3, r27, r24
    lbz r0, 0x70(r3)
    cmpwi r0, 0x0
    beq @L_801E5210
    b @L_801E5284
@L_801E5210:
    lwz r4, 0x0(r29)
    mr r3, r26
    lwz r5, 0x4(r29)
    lwz r6, 0x8(r29)
    lwz r7, 0x9c(r31)
    bl fn_801E5548
    cmpwi r3, 0x0
    stw r3, 0xac(r31)
    beq @L_801E5258
    lwz r0, -21396(r13)
    cmpwi r0, 0x0
    beq @L_801E5250
    li r3, 0x0
    bl fn_801E446C
    li r0, 0x0
    stw r0, -21396(r13)
@L_801E5250:
    addi r3, r30, 0x1058
    bl fn_800A221C
@L_801E5258:
    lwz r0, 0x4(r28)
    mr r4, r29
    addi r3, r30, 0x18
    li r5, 0x1
    stw r0, 0xc(r29)
    bl fn_8009F230
    bl OSDisableInterrupts
    lwz r4, 0xd8(r31)
    addi r0, r4, 0x1
    stw r0, 0xd8(r31)
    bl OSRestoreInterrupts
@L_801E5284:
    lwz r0, 0x0(r25)
    addi r25, r25, 0x4
    addi r24, r24, 0x1
    add r26, r26, r0
@L_801E5294:
    lwz r0, 0x6c(r31)
    cmplw r24, r0
    blt @L_801E51FC
    lwz r0, -21396(r13)
    cmpwi r0, 0x0
    beq @L_801E52BC
    li r3, 0x1
    bl fn_801E446C
    li r0, 0x0
    stw r0, -21396(r13)
@L_801E52BC:
    mr r3, r28
    bl fn_801E1BB8
    bl OSDisableInterrupts
    lwz r4, 0xd8(r31)
    addi r0, r4, 0x1
    stw r0, 0xd8(r31)
    bl OSRestoreInterrupts
@L_801E52D8:
    lwz r0, 0xd8(r31)
    cmpwi r0, 0x0
    blt @L_801E5184
@L_801E52E4:
    lbz r0, 0xa7(r31)
    cmplwi r0, 0x0
    beq @L_801E52F8
    bl fn_801E1B84
    b @L_801E52FC
@L_801E52F8:
    bl fn_801E1BE8
@L_801E52FC:
    lwz r0, 0x6c(r31)
    mr r29, r3
    lwz r6, 0x0(r3)
    addi r3, r30, 0x38
    slwi r5, r0, 2
    addi r4, r1, 0x8
    addi r24, r5, 0x8
    addi r25, r6, 0x8
    add r24, r6, r24
    li r5, 0x1
    bl fn_8009F2F8
    lwz r27, 0x8(r1)
    li r26, 0x0
    b @L_801E53CC
@L_801E5334:
    add r3, r31, r26
    lbz r0, 0x70(r3)
    cmpwi r0, 0x0
    beq @L_801E5348
    b @L_801E53BC
@L_801E5348:
    lwz r4, 0x0(r27)
    mr r3, r24
    lwz r5, 0x4(r27)
    lwz r6, 0x8(r27)
    lwz r7, 0x9c(r31)
    bl fn_801E5548
    cmpwi r3, 0x0
    stw r3, 0xac(r31)
    beq @L_801E5390
    lwz r0, -21396(r13)
    cmpwi r0, 0x0
    beq @L_801E5388
    li r3, 0x0
    bl fn_801E446C
    li r0, 0x0
    stw r0, -21396(r13)
@L_801E5388:
    addi r3, r30, 0x1058
    bl fn_800A221C
@L_801E5390:
    lwz r0, 0x4(r29)
    mr r4, r27
    addi r3, r30, 0x18
    li r5, 0x1
    stw r0, 0xc(r27)
    bl fn_8009F230
    bl OSDisableInterrupts
    lwz r4, 0xd8(r31)
    addi r0, r4, 0x1
    stw r0, 0xd8(r31)
    bl OSRestoreInterrupts
@L_801E53BC:
    lwz r0, 0x0(r25)
    addi r25, r25, 0x4
    addi r26, r26, 0x1
    add r24, r24, r0
@L_801E53CC:
    lwz r0, 0x6c(r31)
    cmplw r26, r0
    blt @L_801E5334
    lwz r0, -21396(r13)
    cmpwi r0, 0x0
    beq @L_801E53F4
    li r3, 0x1
    bl fn_801E446C
    li r0, 0x0
    stw r0, -21396(r13)
@L_801E53F4:
    mr r3, r29
    bl fn_801E1BB8
    b @L_801E5174
