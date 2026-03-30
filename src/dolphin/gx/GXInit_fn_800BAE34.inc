    nofralloc
    lwz r6, 0x0(r3)
    rlwinm r0, r5, 2, 0, 29
    rlwinm r5, r6, 0, 0, 29
    or r4, r5, r4
    stw r4, 0x0(r3)
    lwz r4, 0x0(r3)
    rlwinm r4, r4, 0, 30, 27
    or r0, r4, r0
    stw r0, 0x0(r3)
    blr
