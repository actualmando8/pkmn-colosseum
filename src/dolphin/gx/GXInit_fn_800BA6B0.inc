    nofralloc
    lwz r6, -29632(r13)
    clrlslwi r0, r3, 24, 4
    rlwinm r8, r3, 0, 24, 31
    lwz r5, 0x204(r6)
    li r4, 0x10
    lis r3, 0xcc01
    rlwinm r5, r5, 0, 28, 24
    or r0, r5, r0
    stw r0, 0x204(r6)
    li r0, 0x1009
    stb r4, -0x8000(r3)
    stw r0, -0x8000(r3)
    stw r8, -0x8000(r3)
    lwz r0, 0x4f4(r6)
    ori r0, r0, 0x4
    stw r0, 0x4f4(r6)
    blr
