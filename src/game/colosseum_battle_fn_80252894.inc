    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r4, r6
    stw r0, 0x14(r1)
    bl fn_802376EC
    lwz r0, 0x14(r1)
    rlwinm r3, r3, 0, 16, 31
    mtlr r0
    addi r1, r1, 0x10
    blr
