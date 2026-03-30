    nofralloc
    lwz r6, -29632(r13)
    rlwinm r8, r3, 2, 0, 29
    add r7, r6, r8
    lwz r0, 0xb8(r7)
    add r9, r6, r8
    rlwinm r3, r0, 0, 14, 12
    clrlslwi r0, r4, 24, 18
    or r0, r3, r0
    stw r0, 0xb8(r7)
    clrlslwi r0, r5, 24, 19
    li r5, 0x61
    lwz r3, 0xb8(r9)
    lis r4, 0xcc01
    rlwinm r3, r3, 0, 13, 11
    or r0, r3, r0
    stw r0, 0xb8(r9)
    add r3, r6, r8
    li r0, 0x0
    stb r5, -0x8000(r4)
    lwz r3, 0xb8(r3)
    stw r3, -0x8000(r4)
    sth r0, 0x2(r6)
    blr
