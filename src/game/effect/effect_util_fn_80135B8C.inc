    nofralloc
    cmplwi r3, 0x0
    beqlr
    stw r6, 0x4(r3)
    stw r5, 0x0(r3)
    blr
