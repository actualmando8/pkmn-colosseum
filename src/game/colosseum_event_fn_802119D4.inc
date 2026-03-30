    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, -21000(r13)
    rlwinm r0, r0, 0, 12, 10
    stw r0, -21000(r13)
    bl fn_80213270
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
