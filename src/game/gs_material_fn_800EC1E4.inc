    nofralloc
    lwz r4, 0x0(r3)
    rlwinm. r0, r4, 0, 28, 28
    beqlr
    ori r0, r4, 0x40
    stw r0, 0x0(r3)
    lwz r0, 0x0(r3)
    rlwinm r0, r0, 0, 17, 15
    stw r0, 0x0(r3)
    blr
