    nofralloc
    lbz r0, 0x3(r4)
    rlwinm r8, r3, 1, 0, 30
    lbz r5, 0x0(r4)
    addi r7, r8, 0xe0
    rlwinm r9, r0, 12, 0, 19
    rlwimi r9, r5, 0, 24, 31
    lbz r10, 0x2(r4)
    lbz r6, 0x1(r4)
    rlwinm r7, r7, 24, 0, 7
    li r5, 0x61
    lwz r3, -29632(r13)
    lis r4, 0xcc01
    stb r5, -0x8000(r4)
    rlwimi r7, r9, 0, 12, 31
    addi r0, r8, 0xe1
    stw r7, -0x8000(r4)
    rlwinm r7, r6, 12, 0, 19
    rlwinm r6, r0, 24, 0, 7
    rlwimi r7, r10, 0, 24, 31
    stb r5, -0x8000(r4)
    rlwimi r6, r7, 0, 12, 31
    stw r6, -0x8000(r4)
    li r0, 0x0
    stb r5, -0x8000(r4)
    stw r6, -0x8000(r4)
    stb r5, -0x8000(r4)
    stw r6, -0x8000(r4)
    sth r0, 0x2(r3)
    blr
