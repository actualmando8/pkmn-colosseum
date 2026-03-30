    nofralloc
    cmplwi r3, 0x0
    bne @L_801FCFB4
    li r3, 0x0
    blr
    @L_801FCFB4:
    lhz r3, 0x678(r3)
    blr
