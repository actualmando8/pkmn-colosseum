    nofralloc
    lwz r3, -21580(r13)
    lwz r0, 0xc(r3)
    subfic r0, r0, 0x31
    cntlzw r0, r0
    rlwinm r3, r0, 27, 24, 31
    blr
