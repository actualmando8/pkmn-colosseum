    nofralloc
    li r0, 0x61
    lwz r4, -29632(r2)
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    li r0, 0x0
    lwz r3, 0x204(r4)
    stw r3, -0x8000(r5)
    sth r0, 0x2(r4)
    blr
