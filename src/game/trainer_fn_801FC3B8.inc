    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC3C8
    li r3, 0x0
    blr
    @L_801FC3C8:
    lbz r3, 0x14(r3)
    blr
