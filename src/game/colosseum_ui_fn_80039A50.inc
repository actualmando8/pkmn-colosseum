    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r5, 0x88(r3)
    li r3, 0x0
    lwz r6, -25452(r13)
    li r4, 0x0
    bl fn_800FB680
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
