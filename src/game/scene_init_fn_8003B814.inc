    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r4, 0x1
    bl fn_8003B85C
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
