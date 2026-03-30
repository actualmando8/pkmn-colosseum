    nofralloc
    cmplwi r3, 0x0
    beqlr
    clrlwi r0, r4, 24
    cmplwi r0, 0x6
    bgelr
    clrlslwi r0, r4, 24, 1
    add r3, r3, r0
    sth r5, 0x22(r3)
    blr
