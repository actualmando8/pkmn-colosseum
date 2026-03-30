    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0x46a
    li r4, 0x0
    li r5, 0x7f
    bl fn_80165A20
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
