    nofralloc
    cmplwi r3, 0x0
    beqlr
    clrlwi r0, r4, 16
    cmplwi r0, 0x8
    bgelr
    addis r3, r3, 0x1
    clrlslwi r0, r4, 16, 2
    add r3, r3, r0
    stw r5, -0x5b3c(r3)
    blr
