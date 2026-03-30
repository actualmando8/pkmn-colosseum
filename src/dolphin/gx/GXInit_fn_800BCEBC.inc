    nofralloc
    lwz r6, -29632(r2)
    clrlslwi r0, r3, 24, 6
    li r3, 0x61
    lwz r5, 0x1dc(r6)
    lis r4, 0xcc01
    rlwinm r5, r5, 0, 26, 24
    or r0, r5, r0
    stw r0, 0x1dc(r6)
    li r0, 0x0
    stb r3, -0x8000(r4)
    lwz r3, 0x1dc(r6)
    stw r3, -0x8000(r4)
    sth r0, 0x2(r6)
    blr
