    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC578
    li r3, 0x0
    blr
    @L_801FC578:
    lbz r3, 0x9(r3)
    blr
