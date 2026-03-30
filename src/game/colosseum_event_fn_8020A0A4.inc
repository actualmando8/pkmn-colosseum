    nofralloc
    cmplwi r3, 0x0
    beqlr
    stb r4, 0xa9(r3)
    blr
