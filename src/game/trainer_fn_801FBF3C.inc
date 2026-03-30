    nofralloc
    cmplwi r3, 0x0
    beqlr
    stw r4, 0xc(r3)
    blr
