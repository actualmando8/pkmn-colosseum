    nofralloc
    rlwinm r0, r3, 0, 24, 31
    cntlzw r0, r0
    rlwinm r0, r0, 27, 5, 31
    rlwinm r0, r0, 0, 24, 31
    stb r0, -32456(r13)
    blr
