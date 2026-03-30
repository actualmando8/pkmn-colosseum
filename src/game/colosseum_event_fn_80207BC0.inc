    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0x100
    li r6, 0x0
    stw r0, 0x14(r1)
    mr r0, r4
    li r4, 0x0
    rlwinm r7, r0, 0, 16, 31
    bl fn_801254B4
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
