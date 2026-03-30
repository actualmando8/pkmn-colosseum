    nofralloc
    cmplwi r3, 0x0
    beqlr
    rlwinm r0, r4, 0, 16, 31
    cmplwi r0, 0x4
    bgelr
    clrlslwi r0, r4, 16, 2
    add r3, r3, r0
    stw r5, 0x20(r3)
    blr
