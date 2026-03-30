    nofralloc
    lwz r6, -29632(r2)
    clrlslwi r0, r3, 24, 19
    li r4, 0x61
    lwz r5, 0x204(r6)
    lis r3, 0xcc01
    rlwinm r5, r5, 0, 13, 11
    or r0, r5, r0
    stw r0, 0x204(r6)
    lis r0, 0xfe08
    stb r4, -0x8000(r3)
    stw r0, -0x8000(r3)
    stb r4, -0x8000(r3)
    lwz r0, 0x204(r6)
    stw r0, -0x8000(r3)
    blr
