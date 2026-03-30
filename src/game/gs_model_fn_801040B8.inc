    nofralloc
    cmplwi r3, 0x0
    beqlr
    rlwinm r0, r4, 2, 0, 29
    add r3, r3, r0
    stw r5, 0x60(r3)
    blr
