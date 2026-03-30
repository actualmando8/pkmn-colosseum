    nofralloc
    cmplwi r3, 0x0
    beqlr
    sth r4, 0x66e(r3)
    blr
