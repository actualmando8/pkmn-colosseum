    nofralloc
    lwz r6, -29632(r13)
    rlwinm r5, r4, 0, 24, 31
    li r0, 0x61
    lwz r7, 0x1d4(r6)
    lis r4, 0xcc01
    rlwimi r7, r5, 0, 24, 31
    stb r0, -0x8000(r4)
    rlwinm r0, r3, 0, 24, 31
    addi r3, r7, 0x0
    rlwimi r3, r0, 8, 23, 23
    stw r3, -0x8000(r4)
    li r0, 0x0
    stw r3, 0x1d4(r6)
    sth r0, 0x2(r6)
    blr
