    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_8006B09C
    bl fn_8006A7E8
    lwz r0, 0x14(r1)
    rlwinm r3, r3, 0, 16, 31
    mtlr r0
    addi r1, r1, 0x10
    blr
