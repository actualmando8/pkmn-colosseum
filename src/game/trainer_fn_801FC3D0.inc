    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC3E0
    li r3, 0x0
    blr
    @L_801FC3E0:
    lbz r3, 0x13(r3)
    blr
