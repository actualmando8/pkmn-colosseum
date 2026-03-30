    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, -32730
    li r6, 0x0
    stw r0, 0x14(r1)
    li r0, 0x0
    addi r4, r4, -14660
    addi r5, r1, 0x8
    stw r3, 0x8(r1)
    li r3, 0x0
    stw r0, 0xc(r1)
    bl fn_801F37B0
    lwz r0, 0xc(r1)
    rlwinm r3, r0, 0, 16, 31
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
