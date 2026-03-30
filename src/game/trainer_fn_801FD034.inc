    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD044
    li r3, 0x0
    blr
    @L_801FD044:
    lhz r3, 0x66c(r3)
    blr
