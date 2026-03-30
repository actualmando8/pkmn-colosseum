    nofralloc
    li r6, 0x61
    lwz r3, -29632(r2)
    lis r5, 0xcc01
    lis r4, 0x5500
    stb r6, -0x8000(r5)
    addi r0, r4, 0x3ff
    stw r0, -0x8000(r5)
    lis r4, 0x5600
    addi r4, r4, 0x3ff
    stb r6, -0x8000(r5)
    li r0, 0x0
    stw r4, -0x8000(r5)
    sth r0, 0x2(r3)
    blr
