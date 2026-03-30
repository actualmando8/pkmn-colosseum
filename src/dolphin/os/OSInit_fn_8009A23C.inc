    nofralloc
    lis r3, 0xcc00
    addi r3, r3, 0x6000
    lwz r0, 0x24(r3)
    rlwinm r3, r0, 0, 24, 31
    blr
