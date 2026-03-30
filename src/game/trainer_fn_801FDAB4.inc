    nofralloc
    cmplwi r3, 0x0
    bne @L_801FDAC4
    li r3, 0x0
    blr
    @L_801FDAC4:
    lbz r3, 0x150(r3)
    blr
