    nofralloc
    cmplwi r3, 0x0
    beqlr
    stw r4, 0x9c(r3)
    blr
