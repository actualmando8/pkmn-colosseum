    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r29, r4
    mr r25, r3
    bl fn_8006B09C
    mr r31, r3
    lwz r30, 0x20(r3)
    bl fn_8006B1D4
    li r26, 0x0
    clrlwi r28, r3, 16
    mr r27, r26
@L_8025D678:
    mr r3, r25
    bl fn_8006B09C
    clrlwi r4, r27, 16
    addi r3, r3, 0xb44
    bl fn_8012AC08
    bl fn_80123FBC
    clrlwi r3, r3, 24
    subic r0, r3, 0x1
    subfe r0, r0, r3
    clrlwi. r0, r0, 24
    beq @L_8025D6B0
    clrlwi r3, r26, 16
    addi r0, r3, 0x1
    clrlwi r26, r0, 16
@L_8025D6B0:
    addi r27, r27, 0x1
    cmpwi r27, 0x6
    blt @L_8025D678
    clrlwi r0, r26, 16
    lwz r3, 0x20(r31)
    cmplw r0, r28
    bge @L_8025D6D0
    mr r28, r26
@L_8025D6D0:
    clrlwi r0, r28, 16
    cmpw r3, r0
    blt @L_8025D6E4
    li r3, -0x1
    b @L_8025D730
@L_8025D6E4:
    li r3, 0x0
    mtctr r30
    cmpwi r30, 0x0
    ble @L_8025D714
@L_8025D6F4:
    addi r0, r3, 0x8
    lwzx r0, r31, r0
    cmpw r0, r29
    bne @L_8025D70C
    li r3, -0x1
    b @L_8025D730
@L_8025D70C:
    addi r3, r3, 0x4
    bdnz @L_8025D6F4
@L_8025D714:
    slwi r0, r30, 2
    mr r3, r30
    add r4, r31, r0
    stw r29, 0x8(r4)
    lwz r4, 0x20(r31)
    addi r0, r4, 0x1
    stw r0, 0x20(r31)
@L_8025D730:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
