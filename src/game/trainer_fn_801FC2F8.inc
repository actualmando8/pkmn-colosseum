    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC308
    li r3, 0x0
    blr
    @L_801FC308:
    lbz r3, 0x15(r3)
    blr
