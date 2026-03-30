    nofralloc
    cmplwi r3, 0x0
    beqlr
    stw r4, 0xa0(r3)
    blr
