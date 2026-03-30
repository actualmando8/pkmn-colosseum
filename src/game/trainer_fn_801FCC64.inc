    nofralloc
    cmplwi r3, 0x0
    bne @L_801FCC74
    li r3, 0x0
    blr
    @L_801FCC74:
    lhz r3, 0x2(r3)
    blr
