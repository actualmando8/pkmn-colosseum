    nofralloc
    lis r3, 0xcc00
    lhz r0, 0x5004(r3)
    extrwi r3, r0, 1, 16
    blr
