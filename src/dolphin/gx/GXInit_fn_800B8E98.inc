    nofralloc
    lwz r5, -24224(r13)
    rlwinm r0, r4, 0, 24, 31
    rlwimi r0, r3, 8, 0, 23
    sth r0, 0x6(r5)
    blr
