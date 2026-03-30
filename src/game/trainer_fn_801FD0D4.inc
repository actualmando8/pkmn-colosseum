    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD0E4
    li r3, 0x0
    blr
    @L_801FD0E4:
    lhz r3, 0x6(r3)
    blr
