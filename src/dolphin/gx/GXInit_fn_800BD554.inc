    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x8(r1)
    lwz r4, -29632(r13)
    lwzu r0, 0x80(r4)
    rlwinm r0, r0, 0, 0, 25
    or r0, r0, r3
    stw r0, 0x0(r4)
    li r3, 0x0
    bl fn_800BD898
    lwz r0, 0xc(r1)
    addi r1, r1, 0x8
    mtlr r0
    blr
