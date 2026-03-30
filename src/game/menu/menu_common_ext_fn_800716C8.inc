    nofralloc
    lis r6, -32709
    rlwinm r0, r3, 3, 0, 28
    addi r6, r6, 28184
    li r3, 0x0
    stwx r5, r6, r0
    add r5, r6, r0
    stw r4, 0x4(r5)
    blr
