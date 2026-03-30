    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC5D8
    li r3, 0x0
    blr
    @L_801FC5D8:
    lbz r3, 0x5(r3)
    blr
