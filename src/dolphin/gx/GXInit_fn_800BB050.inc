    nofralloc
    li r0, 0x0
    stw r0, 0x0(r3)
    rlwinm r5, r5, 10, 0, 21
    rlwinm r0, r4, 27, 7, 31
    lwz r7, 0x0(r3)
    rlwinm r4, r7, 0, 22, 19
    or r4, r4, r5
    stw r4, 0x0(r3)
    lwz r4, 0x4(r3)
    rlwinm r4, r4, 0, 0, 10
    or r0, r4, r0
    stw r0, 0x4(r3)
    lwz r0, 0x4(r3)
    rlwinm r0, r0, 0, 8, 31
    oris r0, r0, 0x6400
    stw r0, 0x4(r3)
    sth r6, 0x8(r3)
    blr
