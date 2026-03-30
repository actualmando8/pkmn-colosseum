    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    bl fn_80129B2C
    rlwinm r3, r3, 0, 24, 31
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
