    nofralloc
    cmplwi r3, 0x0
    beqlr
    stb r4, 0x68d(r3)
    blr
