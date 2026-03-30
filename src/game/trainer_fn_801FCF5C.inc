    nofralloc
    cmplwi r3, 0x0
    beqlr
    sth r4, 0x66c(r3)
    blr
