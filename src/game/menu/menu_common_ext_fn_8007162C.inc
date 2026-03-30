    nofralloc
    lis r3, -32709
    addi r3, r3, 28040
    lwz r0, 0x40(r3)
    rlwinm r0, r0, 3, 0, 28
    lwzx r3, r3, r0
    blr
