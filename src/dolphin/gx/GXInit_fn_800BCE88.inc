    nofralloc
    lwz r6, -29632(r2)
    li r0, 0x61
    lwz r7, 0x1d8(r6)
    rlwimi r7, r3, 0, 31, 31
    lis r3, 0xcc01
    stb r0, -0x8000(r3)
    rlwimi r7, r4, 1, 28, 30
    rlwimi r7, r5, 4, 27, 27
    stw r7, -0x8000(r3)
    li r0, 0x0
    stw r7, 0x1d8(r6)
    sth r0, 0x2(r6)
    blr
