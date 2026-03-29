    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0x10b
    li r4, -0x1
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
