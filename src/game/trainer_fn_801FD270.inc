    nofralloc
    cmplwi r3, 0x0
    beqlr
    stb r4, 0x696(r3)
    blr
