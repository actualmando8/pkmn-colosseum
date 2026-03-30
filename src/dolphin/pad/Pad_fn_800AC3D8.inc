    nofralloc
    lis r3, 0xcc00
    lwz r0, 0x6c00(r3)
    extrwi r3, r0, 1, 30
    blr
