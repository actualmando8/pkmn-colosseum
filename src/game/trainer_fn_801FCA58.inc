    nofralloc
    cmplwi r3, 0x0
    beqlr
    clrlwi r0, r4, 24
    cmplwi r0, 0xc
    bgelr
    add r3, r3, r0
    stb r5, 0x8(r3)
    blr
