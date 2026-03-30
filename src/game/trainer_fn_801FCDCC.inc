    nofralloc
    cmplwi r3, 0x0
    bne @L_801FCDDC
    li r3, 0x0
    blr
    @L_801FCDDC:
    lbz r3, 0x27b5(r3)
    blr
