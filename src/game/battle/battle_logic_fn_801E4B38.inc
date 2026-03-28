    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r4, -32697
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    addi r31, r4, -21408
    lis r4, -32697
    mr r29, r3
    addi r30, r4, -20960
    li r27, 0x0
    lwz r28, 0xbc(r31)
@L_801E4B64:
    lwz r0, 0x6c(r31)
    addi r25, r29, 0x8
    addi r3, r30, 0x38
    addi r4, r1, 0x8
    slwi r6, r0, 2
    li r5, 0x1
    addi r26, r6, 0x8
    add r26, r29, r26
    bl fn_8009F2F8
    lwz r0, 0x6c(r31)
    li r4, 0x0
    lwz r24, 0x8(r1)
    mtctr r0
    cmplwi r0, 0x0
    ble @L_801E4C04
@L_801E4BA0:
    add r3, r31, r4
    lbz r0, 0x70(r3)
    cmpwi r0, 0x1
    beq @L_801E4BB4
    b @L_801E4BF0
@L_801E4BB4:
    lwz r4, 0x0(r25)
    li r5, 0x0
    lwz r0, 0xdc(r31)
    lwz r3, 0x0(r24)
    mullw r0, r4, r0
    add r4, r26, r0
    bl fn_801ECAB0
    stw r3, 0x8(r24)
    mr r4, r24
    addi r3, r30, 0x18
    li r5, 0x1
    lwz r0, 0x0(r24)
    stw r0, 0x4(r24)
    bl fn_8009F230
    b @L_801E4C04
@L_801E4BF0:
    lwz r0, 0x0(r25)
    addi r25, r25, 0x4
    addi r4, r4, 0x1
    add r26, r26, r0
    bdnz @L_801E4BA0
@L_801E4C04:
    lwz r0, 0xc0(r31)
    lwz r5, 0x50(r31)
    add r4, r27, r0
    divwu r3, r4, r5
    subi r0, r5, 0x1
    mullw r3, r3, r5
    subf r3, r3, r4
    cmplw r3, r0
    bne @L_801E4C5C
    lbz r0, 0xa6(r31)
    clrlwi. r0, r0, 31
    beq @L_801E4C40
    lwz r28, 0x0(r29)
    lwz r29, 0xb4(r31)
    b @L_801E4C68
@L_801E4C40:
    cmpwi r27, 0x2
    bge @L_801E4C50
    li r3, 0x1
    bl fn_801E446C
@L_801E4C50:
    addi r3, r30, 0x1058
    bl fn_800A221C
    b @L_801E4C68
@L_801E4C5C:
    lwz r0, 0x0(r29)
    add r29, r29, r28
    mr r28, r0
@L_801E4C68:
    cmpwi r27, 0x2
    bne @L_801E4C78
    li r3, 0x1
    bl fn_801E446C
@L_801E4C78:
    addi r27, r27, 0x1
    b @L_801E4B64
