    nofralloc
    cmplwi r3, 0x0
    beqlr
    sth r4, 0x678(r3)
    blr
