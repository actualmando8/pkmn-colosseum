    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC560
    li r3, 0x0
    blr
    @L_801FC560:
    lbz r3, 0xa(r3)
    blr
