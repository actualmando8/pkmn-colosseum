    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    sth r3, 0x8(r1)
    sth r4, 0xa(r1)
    lhz r3, 0x8(r1)
    stw r0, 0x14(r1)
    lhz r4, 0xa(r1)
    bl fn_8010C508
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
