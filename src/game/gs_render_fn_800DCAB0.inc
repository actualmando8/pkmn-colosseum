    nofralloc
    lbz r0, 0x2(r3)
    cmplwi r0, 0x0
    beqlr
    li r4, 0x1
    li r0, 0x0
    stb r4, 0x3(r3)
    stb r0, 0x70(r3)
    stb r4, 0x71(r3)
    blr
