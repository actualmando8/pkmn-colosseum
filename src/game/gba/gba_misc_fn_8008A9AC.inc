    nofralloc
    rlwinm r5, r3, 0, 28, 31
    rlwinm r0, r3, 28, 28, 31
    stb r5, 0x0(r4)
    rlwinm r7, r3, 24, 28, 31
    rlwinm r6, r3, 20, 28, 31
    rlwinm r5, r3, 16, 28, 31
    stb r0, 0x1(r4)
    rlwinm r0, r3, 12, 28, 31
    li r3, 0x0
    stb r7, 0x2(r4)
    stb r6, 0x3(r4)
    stb r5, 0x4(r4)
    stb r0, 0x5(r4)
    blr
