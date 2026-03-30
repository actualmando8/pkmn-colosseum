    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, -22860(r13)
    cntlzw r0, r0
    rlwinm r3, r0, 27, 5, 31
    stw r3, -22860(r13)
    bl fn_80101B88
    lwz r0, 0x14(r1)
    li r3, 0x0
    mtlr r0
    addi r1, r1, 0x10
    blr
