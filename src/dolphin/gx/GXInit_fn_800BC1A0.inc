    nofralloc
    lwz r9, -29632(r13)
    rlwinm r3, r3, 2, 0, 29
    li r0, 0x61
    add r8, r9, r3
    lwz r10, 0x130(r8)
    lis r3, 0xcc01
    rlwimi r10, r4, 12, 16, 19
    stb r0, -0x8000(r3)
    addi r4, r10, 0x0
    rlwimi r4, r5, 8, 20, 23
    rlwimi r4, r6, 4, 24, 27
    rlwimi r4, r7, 0, 28, 31
    stw r4, -0x8000(r3)
    li r0, 0x0
    stw r4, 0x130(r8)
    sth r0, 0x2(r9)
    blr
