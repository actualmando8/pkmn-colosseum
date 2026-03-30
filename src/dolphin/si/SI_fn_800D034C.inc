    nofralloc
    lis r3, 0xcc00
    lis r0, 0x8000
    stw r0, 0x6438(r3)
    blr
