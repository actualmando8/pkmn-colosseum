    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC2D8
    li r3, 0x0
    blr
    @L_801FC2D8:
    lbz r3, 0x17(r3)
    blr
