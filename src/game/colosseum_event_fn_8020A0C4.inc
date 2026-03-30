    nofralloc
    cmplwi r3, 0x0
    beqlr
    sth r4, 0xa6(r3)
    blr
