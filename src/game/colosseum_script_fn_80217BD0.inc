    nofralloc
    rlwinm r3, r3, 0, 16, 31
    li r0, 0x19
    subfic r3, r3, 0xff
    mulli r3, r3, 0xa
    divw r0, r3, r0
    rlwinm r3, r0, 0, 16, 31
    blr
