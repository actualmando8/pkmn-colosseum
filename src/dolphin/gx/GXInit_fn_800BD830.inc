    nofralloc
    addi r5, r3, 0x156
    lwz r3, -29632(r13)
    addi r0, r4, 0x156
    rlwinm r4, r5, 31, 1, 31
    rlwinm r4, r4, 0, 22, 11
    rlwinm r0, r0, 9, 0, 21
    or r0, r4, r0
    rlwinm r5, r0, 0, 8, 31
    li r0, 0x61
    lis r4, 0xcc01
    stb r0, -0x8000(r4)
    oris r5, r5, 0x5900
    li r0, 0x0
    stw r5, -0x8000(r4)
    sth r0, 0x2(r3)
    blr
