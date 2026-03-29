    nofralloc
    lwz r5, 0x60(r3)
    li r3, 0x0
    lbz r0, 0x0(r5)
    stb r0, 0x64(r4)
    lbz r0, 0x1(r5)
    stb r0, 0x65(r4)
    lbz r0, 0x2(r5)
    stb r0, 0x66(r4)
    blr
