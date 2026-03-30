    nofralloc
    cmplwi r3, 0x0
    beqlr
    stfs f1, 0x18(r3)
    blr
