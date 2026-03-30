    nofralloc
    lis r3, 0xcc00
    lwz r0, 0x6c04(r3)
    extrwi r3, r0, 8, 16
    blr
