    nofralloc
    cmplwi r3, 0x0
    beqlr
    sth r4, 0x66a(r3)
    blr
