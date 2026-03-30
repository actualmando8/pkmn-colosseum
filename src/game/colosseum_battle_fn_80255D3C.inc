    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r3, r4
    li r4, 0x0
    stw r0, 0x14(r1)
    li r5, 0xed
    li r6, 0x0
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
    subic r0, r3, 0x1
    subfe r0, r0, r3
    rlwinm r3, r0, 0, 24, 31
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
