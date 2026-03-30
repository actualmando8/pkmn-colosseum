    nofralloc
    cmplwi r3, 0x0
    bne @L_801FCEA4
    li r3, 0x0
    blr
    @L_801FCEA4:
    lhz r3, 0x0(r3)
    blr
