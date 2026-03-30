    nofralloc
    cmplwi r3, 0x0
    bne @L_801FDADC
    li r3, 0x0
    blr
    @L_801FDADC:
    lbz r3, 0x14f(r3)
    blr
