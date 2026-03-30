    nofralloc
    lwz r6, -29632(r13)
    rlwinm r8, r3, 0, 24, 31
    li r4, 0x10
    lwz r5, 0x204(r6)
    lis r3, 0xcc01
    li r0, 0x103f
    rlwinm r5, r5, 0, 0, 27
    or r5, r5, r8
    stw r5, 0x204(r6)
    stb r4, -0x8000(r3)
    stw r0, -0x8000(r3)
    stw r8, -0x8000(r3)
    lwz r0, 0x4f4(r6)
    ori r0, r0, 0x4
    stw r0, 0x4f4(r6)
    blr
