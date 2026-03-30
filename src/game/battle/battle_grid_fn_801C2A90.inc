    nofralloc
    cmplwi r3, 0x0
    beqlr
    lwz r0, 0x0(r3)
    rlwinm r4, r4, 0, 2, 3
    or r0, r0, r4
    stw r0, 0x0(r3)
    blr
