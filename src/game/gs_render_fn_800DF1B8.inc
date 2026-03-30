    nofralloc
    lwz r4, 0x20(r3)
    cmplwi r4, 0x0
    stfs f1, 0x34(r3)
    beqlr
    stfs f1, 0x50(r4)
    blr
