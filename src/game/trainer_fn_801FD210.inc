    nofralloc
    cmplwi r3, 0x0
    beqlr
    stb r4, 0x69c(r3)
    blr
