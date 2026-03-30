    nofralloc
    cmplwi r3, 0x0
    beqlr
    stb r4, 0x27bc(r3)
    blr
