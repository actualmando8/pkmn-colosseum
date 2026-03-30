    nofralloc
    cmplwi r3, 0x0
    beqlr
    stb r4, 0x69d(r3)
    blr
