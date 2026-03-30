    nofralloc
    cmplwi r3, 0x0
    beqlr
    stb r4, 0x67e(r3)
    blr
