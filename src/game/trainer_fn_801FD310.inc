    nofralloc
    cmplwi r3, 0x0
    beqlr
    stb r4, 0x68c(r3)
    blr
