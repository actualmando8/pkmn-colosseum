    nofralloc
    cmplwi r3, 0x0
    beqlr
    stb r4, 0x693(r3)
    blr
