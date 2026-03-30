    nofralloc
    rlwinm r0, r3, 0, 24, 31
    lwz r3, -24224(r13)
    rlwinm r6, r0, 0, 31, 27
    rlwinm r0, r4, 1, 0, 30
    or r0, r6, r0
    rlwinm r4, r0, 0, 28, 26
    clrlslwi r0, r5, 24, 4
    or r0, r4, r0
    sth r0, 0x0(r3)
    blr
