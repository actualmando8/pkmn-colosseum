    nofralloc
    cmplwi r3, 0x0
    beqlr
    stb r4, 0x69e(r3)
    blr
