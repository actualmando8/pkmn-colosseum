    nofralloc
    cmplwi r3, 0x0
    beqlr
    clrlwi r0, r4, 24
    cmplwi r0, 0x4
    bgelr
    clrlslwi r0, r4, 24, 2
    add r3, r3, r0
    stw r5, 0x24(r3)
    blr
