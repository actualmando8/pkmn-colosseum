    nofralloc
    lwz r8, -29632(r2)
    subfic r0, r3, 0x3
    cntlzw r7, r0
    subfic r0, r3, 0x2
    lwz r9, 0x1d0(r8)
    rlwimi r9, r7, 6, 20, 20
    addi r7, r9, 0x0
    rlwimi r7, r3, 0, 31, 31
    cntlzw r0, r0
    rlwimi r7, r0, 28, 30, 30
    rlwimi r7, r6, 12, 16, 19
    rlwimi r7, r4, 8, 21, 23
    li r0, 0x61
    lis r3, 0xcc01
    stb r0, -0x8000(r3)
    rlwimi r7, r5, 5, 24, 26
    li r0, 0x0
    stw r7, -0x8000(r3)
    stw r7, 0x1d0(r8)
    sth r0, 0x2(r8)
    blr
