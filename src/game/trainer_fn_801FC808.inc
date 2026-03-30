    nofralloc
    cmplwi r3, 0x0
    beqlr
    clrlwi r0, r4, 24
    cmplwi r0, 0x6
    bgelr
    add r3, r3, r0
    stb r5, 0x1c(r3)
    blr
