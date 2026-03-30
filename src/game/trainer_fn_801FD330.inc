    nofralloc
    cmplwi r3, 0x0
    beqlr
    stb r4, 0x68a(r3)
    blr
