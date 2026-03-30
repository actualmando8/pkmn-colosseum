    nofralloc
    lis r5, -32709
    rlwinm r0, r3, 2, 0, 29
    addi r5, r5, 28168
    li r3, 0x0
    stwx r4, r5, r0
    blr
