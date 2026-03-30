    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x2
    stw r0, 0x14(r1)
    bl fn_801F025C
    li r4, 0x49
    bl fn_801F6E44
    rlwinm r0, r3, 0, 24, 31
    subfic r0, r0, 0x2
    cntlzw r0, r0
    rlwinm r3, r0, 27, 5, 31
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
