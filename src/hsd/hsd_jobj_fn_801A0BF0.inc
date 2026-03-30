    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r5, r4
    mr r4, r3
    stw r0, 0x14(r1)
    li r3, 0x0
    bl fn_8019C128
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
