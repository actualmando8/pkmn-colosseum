    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x2
    li r4, 0x2
    stw r0, 0x14(r1)
    li r5, 0x0
    li r6, 0x2
    li r7, 0x0
    li r8, 0x0
    bl fn_800D37D4
    lwz r0, 0x14(r1)
    li r3, 0x0
    mtlr r0
    addi r1, r1, 0x10
    blr
