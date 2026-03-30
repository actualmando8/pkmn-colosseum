    nofralloc
    cmplwi r3, 0x0
    beqlr
    lhz r0, 0x8(r3)
    or r0, r0, r4
    sth r0, 0x8(r3)
    blr
