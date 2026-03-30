    nofralloc
    cmplwi r3, 0x0
    bne @L_801FBEC8
    li r3, 0x0
    blr
    @L_801FBEC8:
    lhz r3, 0xa(r3)
    blr
