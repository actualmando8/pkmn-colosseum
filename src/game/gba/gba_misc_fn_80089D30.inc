    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    subi r3, r31, 0x1
    bl fn_80071AE4
    rlwinm r0, r31, 1, 0, 30
    addi r4, r13, -24988
    add r4, r4, r0
    li r0, 0x0
    sth r0, -0x2(r4)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
