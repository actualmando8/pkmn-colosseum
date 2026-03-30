    nofralloc
    cmplwi r3, 0x0
    bne @L_801FCC4C
    li r3, 0x0
    blr
    @L_801FCC4C:
    lhz r3, 0x4(r3)
    blr
