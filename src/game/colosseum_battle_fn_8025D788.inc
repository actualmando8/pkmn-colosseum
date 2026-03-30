    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    li r29, 0x0
    li r31, 0x163
@L_8025D7A0:
    mr r3, r29
    bl fn_8006B09C
    addi r30, r3, 0xb44
    mr r3, r29
    bl fn_8006B09C
    cmplwi r30, 0x0
    addi r3, r3, 0x2c
    beq @L_8025D7E8
    cmplwi r3, 0x0
    beq @L_8025D7E8
    subi r5, r3, 0x4
    subi r4, r30, 0x4
    mtctr r31
@L_8025D7D4:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_8025D7D4
@L_8025D7E8:
    addi r29, r29, 0x1
    cmpwi r29, 0x4
    blt @L_8025D7A0
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
