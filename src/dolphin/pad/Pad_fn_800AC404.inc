    nofralloc
    lis r3, 0xcc00
    lwz r0, 0x6c04(r3)
    clrlwi r3, r0, 24
    blr
