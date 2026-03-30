    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    bl fn_8006B1D4
    li r30, 0x0
    mr r31, r27
    mr r29, r30
    clrlwi r28, r3, 16
@L_8025D41C:
    mr r3, r27
    bl fn_8006B09C
    clrlwi r4, r29, 16
    addi r3, r3, 0xb44
    bl fn_8012AC08
    bl fn_80123FBC
    clrlwi r3, r3, 24
    subic r0, r3, 0x1
    subfe r0, r0, r3
    clrlwi. r0, r0, 24
    beq @L_8025D454
    clrlwi r3, r30, 16
    addi r0, r3, 0x1
    clrlwi r30, r0, 16
@L_8025D454:
    addi r29, r29, 0x1
    cmpwi r29, 0x6
    blt @L_8025D41C
    clrlwi r0, r30, 16
    cmplw r0, r28
    bge @L_8025D470
    mr r28, r30
@L_8025D470:
    mr r3, r31
    clrlwi r30, r28, 16
    bl fn_8006B09C
    mr r28, r3
    mr r3, r31
    bl fn_8006B09C
    li r0, 0x163
    addi r5, r28, 0x28
    addi r4, r3, 0xb40
    mtctr r0
@L_8025D498:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_8025D498
    li r27, 0x0
    li r29, 0x0
    b @L_8025D544
@L_8025D4B8:
    mr r3, r31
    bl fn_8006B09C
    addi r0, r29, 0x8
    lwzx r28, r3, r0
    mr r3, r31
    bl fn_8006B09C
    clrlwi r4, r28, 16
    addi r3, r3, 0xb44
    bl fn_8012AC08
    mr r28, r3
    mr r3, r31
    bl fn_8006B09C
    clrlwi r4, r27, 16
    addi r3, r3, 0x2c
    bl fn_8012AC08
    mr r3, r31
    bl fn_8006B09C
    clrlwi r4, r27, 16
    addi r3, r3, 0x2c
    bl fn_8012AC08
    cmplwi r3, 0x0
    beq @L_8025D53C
    cmplwi r28, 0x0
    beq @L_8025D53C
    li r0, 0x27
    subi r5, r3, 0x4
    subi r4, r28, 0x4
    mtctr r0
@L_8025D528:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_8025D528
@L_8025D53C:
    addi r27, r27, 0x1
    addi r29, r29, 0x4
@L_8025D544:
    cmpw r27, r30
    blt @L_8025D4B8
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
