    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC2A8
    li r3, 0x0
    blr
    @L_801FC2A8:
    lbz r3, 0x19(r3)
    blr
