    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC5A8
    li r3, 0x0
    blr
    @L_801FC5A8:
    lbz r3, 0x7(r3)
    blr
