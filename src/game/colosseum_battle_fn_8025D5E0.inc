    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r4
    mr r31, r5
    bl fn_8006B09C
    li r6, 0x0
    li r4, 0x0
    mtctr r30
    cmpwi r30, 0x0
    ble @L_8025D628
@L_8025D610:
    lwzx r5, r31, r4
    addi r0, r4, 0x8
    addi r6, r6, 0x1
    addi r4, r4, 0x4
    stwx r5, r3, r0
    bdnz @L_8025D610
@L_8025D628:
    stw r6, 0x20(r3)
    mr r3, r6
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
