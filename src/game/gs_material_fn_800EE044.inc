    nofralloc
    cmplwi r3, 0x0
    beqlr
    stw r3, 0x0(r4)
    blr
