    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r6, r3
    li r3, 0x0
    stw r0, 0x14(r1)
    li r4, 0x1
    li r5, 0x3
    bl fn_801F2598
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
