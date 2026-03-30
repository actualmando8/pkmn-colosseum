    nofralloc
    cmplwi r3, 0x0
    beqlr
    stb r4, 0x60a(r3)
    blr
