    nofralloc
    lwz r7, -29632(r13)
    rlwinm r6, r4, 19, 0, 12
    li r5, 0x61
    lwz r0, 0x7c(r7)
    lis r4, 0xcc01
    rlwinm r0, r0, 0, 24, 15
    rlwimi r0, r3, 8, 16, 23
    stw r0, 0x7c(r7)
    li r0, 0x0
    lwz r3, 0x7c(r7)
    rlwinm r3, r3, 0, 13, 9
    or r3, r3, r6
    stw r3, 0x7c(r7)
    stb r5, -0x8000(r4)
    lwz r3, 0x7c(r7)
    stw r3, -0x8000(r4)
    sth r0, 0x2(r7)
    blr
