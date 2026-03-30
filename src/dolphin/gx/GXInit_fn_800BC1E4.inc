    nofralloc
    lwz r9, -29632(r13)
    rlwinm r3, r3, 2, 0, 29
    li r0, 0x61
    add r8, r9, r3
    lwz r10, 0x170(r8)
    lis r3, 0xcc01
    rlwimi r10, r4, 13, 16, 18
    stb r0, -0x8000(r3)
    addi r4, r10, 0x0
    rlwimi r4, r5, 10, 19, 21
    rlwimi r4, r6, 7, 22, 24
    rlwimi r4, r7, 4, 25, 27
    stw r4, -0x8000(r3)
    li r0, 0x0
    stw r4, 0x170(r8)
    sth r0, 0x2(r9)
    blr
