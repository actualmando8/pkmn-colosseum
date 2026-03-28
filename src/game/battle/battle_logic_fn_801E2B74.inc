    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r7, -32697
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    addi r29, r7, -21408
    mr r26, r3
    mr r27, r4
    mr r28, r5
    mr r30, r6
    lwz r0, 0xec(r29)
    cmplwi r0, 0x0
    bne @L_801E2BCC
    li r3, 0x0
    bl fn_801E4AC4
    cmplwi r3, 0x0
    stw r3, 0xec(r29)
    bne @L_801E2BCC
    li r0, 0x2
    li r3, 0x0
    stw r0, 0x0(r30)
    b @L_801E2C94
@L_801E2BCC:
    lwz r3, 0xec(r29)
    lwz r0, 0x8(r3)
    cmplwi r0, 0x0
    beq @L_801E2C90
    cmplw r0, r28
    blt @L_801E2BE8
    mr r0, r28
@L_801E2BE8:
    cmplwi r27, 0x0
    lwz r4, 0x4(r3)
    mr r31, r0
    bne @L_801E2C1C
    mtctr r0
    cmplwi r0, 0x0
    ble @L_801E2C48
@L_801E2C04:
    lha r0, 0x2(r4)
    addi r4, r4, 0x4
    sth r0, 0x0(r26)
    addi r26, r26, 0x2
    bdnz @L_801E2C04
    b @L_801E2C48
@L_801E2C1C:
    mtctr r0
    cmplwi r0, 0x0
    ble @L_801E2C48
@L_801E2C28:
    lha r0, 0x0(r4)
    sth r0, 0x0(r27)
    addi r27, r27, 0x2
    lha r0, 0x2(r4)
    addi r4, r4, 0x4
    sth r0, 0x0(r26)
    addi r26, r26, 0x2
    bdnz @L_801E2C28
@L_801E2C48:
    lwz r3, 0xec(r29)
    lwz r0, 0x8(r3)
    subf r0, r31, r0
    stw r0, 0x8(r3)
    lwz r3, 0xec(r29)
    stw r4, 0x4(r3)
    lwz r3, 0xec(r29)
    lwz r0, 0x8(r3)
    cmplwi r0, 0x0
    bne @L_801E2C88
    bl fn_801E4B08
    li r3, 0x0
    li r0, 0x1
    stw r3, 0xec(r29)
    stw r0, 0x0(r30)
    b @L_801E2C90
@L_801E2C88:
    li r0, 0x0
    stw r0, 0x0(r30)
@L_801E2C90:
    mr r3, r31
@L_801E2C94:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
