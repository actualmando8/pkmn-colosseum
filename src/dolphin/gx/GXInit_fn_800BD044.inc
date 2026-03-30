    nofralloc
    rlwinm r0, r4, 0, 24, 31
    lwz r4, -29632(r13)
    rlwinm r5, r0, 0, 31, 29
    clrlslwi r0, r3, 24, 1
    or r0, r5, r0
    rlwinm r5, r0, 0, 8, 31
    li r0, 0x61
    lis r3, 0xcc01
    stb r0, -0x8000(r3)
    oris r5, r5, 0x4400
    li r0, 0x0
    stw r5, -0x8000(r3)
    sth r0, 0x2(r4)
    blr
