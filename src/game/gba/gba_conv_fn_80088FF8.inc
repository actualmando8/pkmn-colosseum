    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    cntlzw r0, r4
    rlwinm r0, r0, 27, 5, 31
    rlwinm r3, r0, 0, 24, 31
    bl fn_80089030
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
