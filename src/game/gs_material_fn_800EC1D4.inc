    nofralloc
    lwz r0, 0x0(r3)
    rlwinm r0, r0, 0, 26, 24
    stw r0, 0x0(r3)
    blr
