    nofralloc
    mulli r0, r3, 0xc
    lis r3, 0xcc00
    addi r3, r3, 0x6400
    stwx r4, r3, r0
    blr
