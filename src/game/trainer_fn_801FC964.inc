    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC974
    li r3, 0x0
    blr
    @L_801FC974:
    lbz r3, 0x3(r3)
    blr
