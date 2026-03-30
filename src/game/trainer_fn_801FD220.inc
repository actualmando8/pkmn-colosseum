    nofralloc
    cmplwi r3, 0x0
    beqlr
    stb r4, 0x69b(r3)
    blr
