    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC620
    li r3, 0x0
    blr
    @L_801FC620:
    lbz r3, 0x2(r3)
    blr
