    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD014
    li r3, 0x0
    blr
    @L_801FD014:
    lhz r3, 0x670(r3)
    blr
