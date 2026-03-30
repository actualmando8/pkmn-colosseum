    nofralloc
    lbz r0, 0x3(r4)
    rlwinm r5, r3, 1, 0, 30
    lbz r6, 0x0(r4)
    addi r7, r5, 0xe0
    rlwinm r0, r0, 12, 0, 19
    rlwimi r0, r6, 0, 24, 31
    lbz r6, 0x1(r4)
    lbz r9, 0x2(r4)
    rlwinm r0, r0, 0, 12, 7
    oris r8, r0, 0x80
    lwz r3, -29632(r13)
    rlwinm r7, r7, 24, 0, 7
    li r0, 0x61
    lis r4, 0xcc01
    rlwinm r6, r6, 12, 0, 19
    stb r0, -0x8000(r4)
    rlwimi r7, r8, 0, 8, 31
    stw r7, -0x8000(r4)
    rlwimi r6, r9, 0, 24, 31
    addi r5, r5, 0xe1
    stb r0, -0x8000(r4)
    rlwinm r0, r6, 0, 12, 7
    oris r6, r0, 0x80
    rlwinm r0, r5, 24, 0, 7
    rlwimi r0, r6, 0, 8, 31
    stw r0, -0x8000(r4)
    li r0, 0x0
    sth r0, 0x2(r3)
    blr
