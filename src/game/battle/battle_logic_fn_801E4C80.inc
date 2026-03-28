    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r4, -32697
    lis r3, -32697
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    addi r30, r4, -21408
    li r28, 0x0
    addi r31, r3, -20904
@L_801E4CA4:
    bl fn_801E1BE8
    lwz r0, 0x6c(r30)
    mr r29, r3
    lwz r6, 0x0(r29)
    mr r3, r31
    slwi r4, r0, 2
    li r5, 0x1
    addi r27, r4, 0x8
    addi r26, r6, 0x8
    addi r4, r1, 0x8
    add r27, r6, r27
    bl fn_8009F2F8
    lwz r0, 0x6c(r30)
    li r4, 0x0
    lwz r25, 0x8(r1)
    mtctr r0
    cmplwi r0, 0x0
    ble @L_801E4D54
@L_801E4CEC:
    add r3, r30, r4
    lbz r0, 0x70(r3)
    cmpwi r0, 0x1
    beq @L_801E4D00
    b @L_801E4D40
@L_801E4D00:
    lwz r4, 0x0(r26)
    li r5, 0x0
    lwz r0, 0xdc(r30)
    lwz r3, 0x0(r25)
    mullw r0, r4, r0
    add r4, r27, r0
    bl fn_801ECAB0
    stw r3, 0x8(r25)
    lis r3, -32697
    addi r3, r3, -20936
    mr r4, r25
    lwz r0, 0x0(r25)
    li r5, 0x1
    stw r0, 0x4(r25)
    bl fn_8009F230
    b @L_801E4D54
@L_801E4D40:
    lwz r0, 0x0(r26)
    addi r26, r26, 0x4
    addi r4, r4, 0x1
    add r27, r27, r0
    bdnz @L_801E4CEC
@L_801E4D54:
    cmpwi r28, 0x2
    bge @L_801E4D8C
    lbz r0, 0xa6(r30)
    clrlwi. r0, r0, 31
    bne @L_801E4D8C
    lwz r3, 0x50(r30)
    lwz r5, 0x4(r29)
    lwz r4, 0xc0(r30)
    subi r0, r3, 0x1
    add r3, r5, r4
    cmplw r3, r0
    bne @L_801E4D8C
    li r3, 0x1
    bl fn_801E446C
@L_801E4D8C:
    cmpwi r28, 0x2
    bne @L_801E4D9C
    li r3, 0x1
    bl fn_801E446C
@L_801E4D9C:
    mr r3, r29
    bl fn_801E1B54
    addi r28, r28, 0x1
    b @L_801E4CA4
