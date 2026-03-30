    nofralloc
    lwz r5, -29632(r2)
    li r0, 0x61
    lis r4, 0xcc01
    lwz r6, 0x1d0(r5)
    rlwimi r6, r3, 4, 27, 27
    stb r0, -0x8000(r4)
    li r0, 0x0
    stw r6, -0x8000(r4)
    stw r6, 0x1d0(r5)
    sth r0, 0x2(r5)
    blr
