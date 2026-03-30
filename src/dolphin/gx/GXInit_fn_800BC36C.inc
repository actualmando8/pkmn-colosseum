    nofralloc
    lha r0, 0x6(r4)
    rlwinm r9, r3, 1, 0, 30
    lha r5, 0x0(r4)
    addi r8, r9, 0xe0
    clrlslwi r10, r0, 21, 12
    rlwimi r10, r5, 0, 21, 31
    lha r7, 0x4(r4)
    lha r6, 0x2(r4)
    rlwinm r8, r8, 24, 0, 7
    li r5, 0x61
    lwz r3, -29632(r13)
    lis r4, 0xcc01
    stb r5, -0x8000(r4)
    rlwimi r8, r10, 0, 9, 31
    addi r0, r9, 0xe1
    stw r8, -0x8000(r4)
    clrlslwi r8, r6, 21, 12
    rlwinm r6, r0, 24, 0, 7
    rlwimi r8, r7, 0, 21, 31
    stb r5, -0x8000(r4)
    rlwimi r6, r8, 0, 9, 31
    stw r6, -0x8000(r4)
    li r0, 0x0
    stb r5, -0x8000(r4)
    stw r6, -0x8000(r4)
    stb r5, -0x8000(r4)
    stw r6, -0x8000(r4)
    sth r0, 0x2(r3)
    blr
