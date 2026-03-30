    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD7D8
    li r3, 0x0
    blr
    @L_801FD7D8:
    lhz r3, 0x67c(r3)
    blr
