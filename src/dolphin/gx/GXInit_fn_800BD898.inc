    nofralloc
    cmpwi r3, 0x5
    bge @L_800BD8D8
    li r0, 0x8
    lwz r4, -29632(r2)
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    li r0, 0x30
    li r3, 0x10
    stb r0, -0x8000(r5)
    li r0, 0x1018
    lwz r4, 0x80(r4)
    stw r4, -0x8000(r5)
    stb r3, -0x8000(r5)
    stw r0, -0x8000(r5)
    stw r4, -0x8000(r5)
    b @L_800BD90C
@L_800BD8D8:
    li r0, 0x8
    lwz r4, -29632(r2)
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    li r0, 0x40
    li r3, 0x10
    stb r0, -0x8000(r5)
    li r0, 0x1019
    lwz r4, 0x84(r4)
    stw r4, -0x8000(r5)
    stb r3, -0x8000(r5)
    stw r0, -0x8000(r5)
    stw r4, -0x8000(r5)
@L_800BD90C:
    lwz r3, -29632(r2)
    li r0, 0x1
    sth r0, 0x2(r3)
    blr
