    nofralloc
    cmplwi r3, 0x0
    bne @L_801FCCA4
    li r3, 0x0
    blr
    @L_801FCCA4:
    lhz r3, 0x6(r3)
    blr
