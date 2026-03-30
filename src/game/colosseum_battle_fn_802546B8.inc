    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_8023720C
    rlwinm r0, r3, 0, 24, 31
    subfic r0, r0, 0x1
    cntlzw r0, r0
    rlwinm r3, r0, 27, 24, 31
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
