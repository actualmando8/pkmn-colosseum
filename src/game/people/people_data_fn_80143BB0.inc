    nofralloc
    cmplwi r3, 0x0
    beqlr
    clrlwi r0, r4, 16
    cmplwi r0, 0x3
    bgelr
    add r3, r3, r0
    stb r5, 0x24(r3)
    blr
