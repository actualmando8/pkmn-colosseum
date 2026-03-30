    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC410
    li r3, 0x0
    blr
    @L_801FC410:
    lbz r3, 0x11(r3)
    blr
