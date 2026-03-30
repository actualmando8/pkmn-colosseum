    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r5, 0x54
    bl fn_800C80D0
    cntlzw r0, r3
    rlwinm r0, r0, 27, 5, 31
    rlwinm r3, r0, 0, 24, 31
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
