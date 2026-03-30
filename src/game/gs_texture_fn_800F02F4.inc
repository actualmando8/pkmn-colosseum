    nofralloc
    lwz r3, 0x8(r1)
    mtlr r3
    lwz r3, 0xc(r1)
    addi r1, r1, 0xc
    blr
