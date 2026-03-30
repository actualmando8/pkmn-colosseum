    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD0FC
    li r3, 0x0
    blr
    @L_801FD0FC:
    lhz r3, 0x4(r3)
    blr
