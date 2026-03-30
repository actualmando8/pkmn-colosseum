    nofralloc
    lwz r4, -24224(r13)
    clrlslwi r0, r3, 24, 4
    lhzu r3, 0x2(r4)
    rlwinm r3, r3, 0, 28, 26
    or r0, r3, r0
    sth r0, 0x0(r4)
    blr
