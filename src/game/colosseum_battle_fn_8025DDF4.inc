    nofralloc
    lwz r0, 0x0(r3)
    cmplwi r0, 0x0
    beqlr
    li r0, 0x0
    stw r0, 0x0(r3)
    blr
