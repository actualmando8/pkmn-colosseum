    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    lis r4, -32697
    stw r0, 0x44(r1)
    stmw r23, 0x1c(r1)
    addi r31, r4, -21408
    lis r4, -32697
    mr r29, r3
    addi r30, r4, -15984
    li r27, 0x0
    lwz r28, 0xbc(r31)
@L_801E4F90:
    lbz r0, 0xa7(r31)
    cmplwi r0, 0x0
    beq @L_801E500C
    b @L_801E5000
@L_801E4FA0:
    bl OSDisableInterrupts
    lwz r4, 0xd8(r31)
    addi r0, r4, 0x1
    stw r0, 0xd8(r31)
    bl OSRestoreInterrupts
    lwz r0, 0xc0(r31)
    lwz r5, 0x50(r31)
    add r4, r27, r0
    divwu r3, r4, r5
    subi r0, r5, 0x1
    mullw r3, r3, r5
    subf r3, r3, r4
    cmplw r3, r0
    bne @L_801E4FF0
    lbz r0, 0xa6(r31)
    clrlwi. r0, r0, 31
    beq @L_801E500C
    lwz r28, 0x0(r29)
    lwz r29, 0xb4(r31)
    b @L_801E4FFC
@L_801E4FF0:
    lwz r0, 0x0(r29)
    add r29, r29, r28
    mr r28, r0
@L_801E4FFC:
    addi r27, r27, 0x1
@L_801E5000:
    lwz r0, 0xd8(r31)
    cmpwi r0, 0x0
    blt @L_801E4FA0
@L_801E500C:
    lwz r0, 0x6c(r31)
    addi r25, r29, 0x8
    addi r3, r30, 0x38
    addi r4, r1, 0x8
    slwi r6, r0, 2
    li r5, 0x1
    addi r26, r6, 0x8
    add r26, r29, r26
    bl fn_8009F2F8
    lwz r23, 0x8(r1)
    li r24, 0x0
    b @L_801E50D0
@L_801E503C:
    add r3, r31, r24
    lbz r0, 0x70(r3)
    cmpwi r0, 0x0
    beq @L_801E5050
    b @L_801E50C0
@L_801E5050:
    lwz r4, 0x0(r23)
    mr r3, r26
    lwz r5, 0x4(r23)
    lwz r6, 0x8(r23)
    lwz r7, 0x9c(r31)
    bl fn_801E5548
    cmpwi r3, 0x0
    stw r3, 0xac(r31)
    beq @L_801E5098
    lwz r0, -21396(r13)
    cmpwi r0, 0x0
    beq @L_801E5090
    li r3, 0x0
    bl fn_801E446C
    li r0, 0x0
    stw r0, -21396(r13)
@L_801E5090:
    addi r3, r30, 0x1058
    bl fn_800A221C
@L_801E5098:
    stw r27, 0xc(r23)
    mr r4, r23
    addi r3, r30, 0x18
    li r5, 0x1
    bl fn_8009F230
    bl OSDisableInterrupts
    lwz r4, 0xd8(r31)
    addi r0, r4, 0x1
    stw r0, 0xd8(r31)
    bl OSRestoreInterrupts
@L_801E50C0:
    lwz r0, 0x0(r25)
    addi r25, r25, 0x4
    addi r24, r24, 0x1
    add r26, r26, r0
@L_801E50D0:
    lwz r0, 0x6c(r31)
    cmplw r24, r0
    blt @L_801E503C
    lwz r0, -21396(r13)
    cmpwi r0, 0x0
    beq @L_801E50F8
    li r3, 0x1
    bl fn_801E446C
    li r0, 0x0
    stw r0, -21396(r13)
@L_801E50F8:
    lwz r0, 0xc0(r31)
    lwz r5, 0x50(r31)
    add r4, r27, r0
    divwu r3, r4, r5
    subi r0, r5, 0x1
    mullw r3, r3, r5
    subf r3, r3, r4
    cmplw r3, r0
    bne @L_801E5140
    lbz r0, 0xa6(r31)
    clrlwi. r0, r0, 31
    beq @L_801E5134
    lwz r28, 0x0(r29)
    lwz r29, 0xb4(r31)
    b @L_801E514C
@L_801E5134:
    addi r3, r30, 0x1058
    bl fn_800A221C
    b @L_801E514C
@L_801E5140:
    lwz r0, 0x0(r29)
    add r29, r29, r28
    mr r28, r0
@L_801E514C:
    addi r27, r27, 0x1
    b @L_801E4F90
