    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0xd5
    li r6, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    mr r3, r4
    li r4, 0x0
    bl fn_8012640C
    subf r0, r3, r31
    lwz r31, 0xc(r1)
    cntlzw r0, r0
    rlwinm r3, r0, 27, 24, 31
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
