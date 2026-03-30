    nofralloc
    lbz r0, 0x2(r3)
    cmplwi r0, 0x0
    beqlr
    stfs f1, 0x68(r3)
    blr
