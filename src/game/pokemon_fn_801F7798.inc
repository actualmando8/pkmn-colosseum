    nofralloc
    cmplwi r3, 0x0
    beqlr
    clrlwi r0, r4, 24
    cmplwi r0, 0x2
    bgelr
    clrlslwi r0, r4, 24, 2
    add r3, r3, r0
    stw r5, 0xc(r3)
    blr
