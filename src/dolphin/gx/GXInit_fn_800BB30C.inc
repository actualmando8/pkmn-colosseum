    nofralloc
    lwz r5, -29632(r13)
    rlwinm r8, r4, 2, 0, 29
    rlwinm r0, r3, 2, 0, 29
    add r3, r5, r0
    add r6, r5, r8
    lwz r4, 0x45c(r3)
    lwz r0, 0xb8(r6)
    add r7, r5, r8
    add r9, r5, r8
    rlwinm r0, r0, 0, 0, 15
    rlwimi r0, r4, 0, 22, 31
    stw r0, 0xb8(r6)
    add r10, r5, r8
    lwz r0, 0xd8(r7)
    rlwinm r0, r0, 0, 0, 15
    rlwimi r0, r4, 22, 22, 31
    stw r0, 0xd8(r7)
    li r7, 0x61
    lwz r6, 0x47c(r3)
    lwz r3, 0xb8(r9)
    rlwinm r0, r6, 30, 30, 31
    subfic r4, r0, 0x1
    rlwinm r0, r6, 0, 30, 31
    subfic r0, r0, 0x1
    cntlzw r0, r0
    rlwinm r3, r3, 0, 16, 14
    rlwinm r0, r0, 11, 8, 15
    or r0, r3, r0
    stw r0, 0xb8(r9)
    cntlzw r0, r4
    rlwinm r0, r0, 11, 8, 15
    lwz r3, 0xd8(r10)
    lis r6, 0xcc01
    add r4, r5, r8
    rlwinm r3, r3, 0, 16, 14
    or r0, r3, r0
    stw r0, 0xd8(r10)
    li r0, 0x0
    stb r7, -0x8000(r6)
    lwz r3, 0xb8(r4)
    stw r3, -0x8000(r6)
    stb r7, -0x8000(r6)
    lwz r3, 0xd8(r4)
    stw r3, -0x8000(r6)
    sth r0, 0x2(r5)
    blr
