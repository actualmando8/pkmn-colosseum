    nofralloc
    mflr r0
    addi r5, r4, 0x0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    addi r4, r1, 0x10
    bl fn_800B29F4
    lwz r0, 0x1c(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
