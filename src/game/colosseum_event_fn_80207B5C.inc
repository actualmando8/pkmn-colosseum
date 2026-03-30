    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    rlwinm r6, r4, 0, 24, 31
    rlwinm r7, r5, 0, 16, 31
    stw r0, 0x14(r1)
    li r4, 0x0
    li r5, 0xff
    bl fn_801254B4
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
