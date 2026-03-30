    nofralloc
    rlwinm r0, r4, 0, 24, 31
    lwz r4, -29632(r13)
    oris r0, r0, 0xf300
    rlwinm r0, r0, 0, 24, 15
    rlwimi r0, r7, 8, 16, 23
    rlwinm r7, r0, 0, 16, 12
    rlwinm r0, r3, 16, 0, 15
    or r0, r7, r0
    rlwinm r3, r0, 0, 13, 9
    rlwinm r0, r6, 19, 0, 12
    or r6, r3, r0
    li r0, 0x61
    lis r3, 0xcc01
    stb r0, -0x8000(r3)
    rlwinm r6, r6, 0, 10, 7
    rlwinm r0, r5, 22, 0, 9
    or r0, r6, r0
    stw r0, -0x8000(r3)
    li r0, 0x0
    sth r0, 0x2(r4)
    blr
