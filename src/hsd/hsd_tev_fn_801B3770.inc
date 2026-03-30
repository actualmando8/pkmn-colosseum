    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, -21680(r13)
    rlwinm r3, r0, 0, 24, 31
    bl fn_800BC8C8
    li r0, 0x0
    stw r0, -21680(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
