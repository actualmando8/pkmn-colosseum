    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    mr r6, r4
    li r5, 0x18
    stw r0, 0x44(r1)
    addi r4, r1, 0x8
    bl fn_80215008
    neg r0, r3
    andc r0, r0, r3
    rlwinm r3, r0, 1, 31, 31
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
