    nofralloc
    rlwinm r0, r3, 1, 0, 30
    addi r3, r13, -24996
    add r3, r3, r0
    lhz r3, -0x2(r3)
    blr
