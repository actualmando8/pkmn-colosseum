    nofralloc
    lwz r0, 0x0(r3)
    rlwinm r0, r0, 0, 27, 25
    stw r0, 0x0(r3)
    lwz r4, 0x0(r3)
    rlwinm. r0, r4, 0, 18, 18
    beqlr
    rlwinm r0, r4, 0, 26, 24
    stw r0, 0x0(r3)
    blr
