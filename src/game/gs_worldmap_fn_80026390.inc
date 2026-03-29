    nofralloc
    li r0, 0x0
    li r5, 0x35
    stb r0, 0x64(r4)
    li r0, 0x3c
    li r3, 0x0
    stb r5, 0x65(r4)
    stb r0, 0x66(r4)
    blr
