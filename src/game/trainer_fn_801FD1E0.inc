    nofralloc
    cmplwi r3, 0x0
    beqlr
    stw r4, 0x6a0(r3)
    blr
