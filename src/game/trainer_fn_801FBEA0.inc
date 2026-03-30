    nofralloc
    cmplwi r3, 0x0
    bne @L_801FBEB0
    li r3, 0x0
    blr
    @L_801FBEB0:
    lhz r3, 0xc(r3)
    blr
