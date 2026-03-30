    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lbz r0, -22855(r13)
    cntlzw r0, r0
    rlwinm r3, r0, 27, 24, 31
    stb r3, -22855(r13)
    bl fn_800E8F80
    lwz r0, 0x14(r1)
    li r3, 0x0
    mtlr r0
    addi r1, r1, 0x10
    blr
