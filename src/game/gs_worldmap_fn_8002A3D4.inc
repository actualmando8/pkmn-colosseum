    nofralloc
    lwz r6, 0x60(r3)
    li r0, 0xff
    li r3, 0x0
    lbz r5, 0x10(r6)
    stb r5, 0x64(r4)
    lbz r5, 0x11(r6)
    stb r5, 0x65(r4)
    lbz r5, 0x12(r6)
    stb r5, 0x66(r4)
    stb r0, 0x67(r4)
    blr
