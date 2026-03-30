    nofralloc
    cmplwi r3, 0x0
    beqlr
    stb r4, 0x14f(r3)
    blr
