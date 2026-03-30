    nofralloc
    cmplwi r3, 0x0
    beqlr
    li r0, 0x0
    stb r0, 0x16(r3)
    stb r4, 0x17(r3)
    blr
