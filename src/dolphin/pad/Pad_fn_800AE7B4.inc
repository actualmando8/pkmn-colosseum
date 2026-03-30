    nofralloc
    lis r3, 0xcc00
    addi r3, r3, 0x5000
    lhz r0, 0x4(r3)
    lhz r3, 0x6(r3)
    rlwimi r3, r0, 16, 0, 15
    blr
