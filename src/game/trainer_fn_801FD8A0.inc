    nofralloc
    cmplwi r3, 0x0
    beqlr
    stb r4, 0x604(r3)
    blr
