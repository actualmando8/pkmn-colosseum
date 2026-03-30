    nofralloc
    rlwinm r0, r4, 0, 8, 31
    lbz r4, 0x3(r3)
    lbz r5, 0x0(r3)
    li r6, 0x61
    rlwinm r7, r4, 8, 0, 23
    lwz r4, -29632(r13)
    rlwimi r7, r5, 0, 24, 31
    lis r5, 0xcc01
    rlwinm r7, r7, 0, 8, 31
    stb r6, -0x8000(r5)
    oris r7, r7, 0x4f00
    stw r7, -0x8000(r5)
    oris r7, r0, 0x5100
    li r0, 0x0
    lbz r8, 0x2(r3)
    lbz r3, 0x1(r3)
    rlwinm r3, r3, 8, 0, 23
    stb r6, -0x8000(r5)
    rlwimi r3, r8, 0, 24, 31
    rlwinm r3, r3, 0, 8, 31
    oris r3, r3, 0x5000
    stw r3, -0x8000(r5)
    stb r6, -0x8000(r5)
    stw r7, -0x8000(r5)
    sth r0, 0x2(r4)
    blr
