    nofralloc
    cmplwi r3, 0x0
    beqlr
    stb r4, 0x522c(r3)
    blr
