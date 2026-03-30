    nofralloc
    cmplwi r3, 0x0
    beqlr
    lwz r0, 0x4(r3)
    andc r0, r0, r4
    stw r0, 0x4(r3)
    blr
