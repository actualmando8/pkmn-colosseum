    nofralloc
    li r0, 0x10
    lwz r4, -29632(r2)
    lis r6, 0xcc01
    stb r0, -0x8000(r6)
    li r5, 0x1005
    li r0, 0x1
    stw r5, -0x8000(r6)
    stw r3, -0x8000(r6)
    sth r0, 0x2(r4)
    blr
