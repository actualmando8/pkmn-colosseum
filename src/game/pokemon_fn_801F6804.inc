    nofralloc
    cmplwi r3, 0x0
    beqlr
    addis r3, r3, 0x1
    stw r4, -0x5b5c(r3)
    blr
