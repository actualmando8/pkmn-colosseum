    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0x5
    li r4, 0x2
    li r5, 0x0
    bl fn_801D0748
    cmpwi r3, 0x4
    bne @L_800889D0
    li r3, 0x0
    b @L_800889D4
    @L_800889D0:
    li r3, -0x1
    @L_800889D4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
