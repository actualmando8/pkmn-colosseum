    nofralloc
    cmplwi r3, 0x0
    beqlr
    stb r4, 0x6ac(r3)
    blr
