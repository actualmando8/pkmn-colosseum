    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_8006B09C
    li r5, 0x0
    li r0, 0x6
    stw r5, 0x20(r3)
    li r4, -0x1
    mtctr r0
@L_8025D768:
    addi r0, r5, 0x8
    addi r5, r5, 0x4
    stwx r4, r3, r0
    bdnz @L_8025D768
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
