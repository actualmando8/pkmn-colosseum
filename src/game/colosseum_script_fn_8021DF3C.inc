    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x112
    stw r0, 0x14(r1)
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    lwz r0, 0x14(r1)
    li r3, 0x1
    mtlr r0
    addi r1, r1, 0x10
    blr
