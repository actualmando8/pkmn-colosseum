    nofralloc
    rlwinm r0, r3, 0, 30, 28
    lwz r3, -24224(r13)
    ori r0, r0, 0x4
    sth r0, 0x8(r3)
    blr
