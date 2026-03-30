    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, -32730
    li r6, 0x0
    stw r0, 0x14(r1)
    addi r4, r4, -13264
    addi r5, r1, 0x8
    stw r3, 0x8(r1)
    li r3, 0x0
    bl fn_801F37B0
    rlwinm r0, r3, 0, 24, 31
    subfic r3, r0, 0x1
    subic r0, r3, 0x1
    subfe r3, r0, r3
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
