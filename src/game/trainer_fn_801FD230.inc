    nofralloc
    cmplwi r3, 0x0
    beqlr
    stb r4, 0x69a(r3)
    blr
