    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x1
    stw r0, 0x14(r1)
    li r5, 0x1
    li r6, 0x0
    bl fn_802274F0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
