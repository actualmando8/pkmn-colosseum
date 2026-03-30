    nofralloc
    cmplwi r3, 0x0
    beqlr
    stb r4, 0x68b(r3)
    blr
