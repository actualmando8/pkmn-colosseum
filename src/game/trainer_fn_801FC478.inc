    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC488
    li r3, 0x0
    blr
    @L_801FC488:
    lbz r3, 0xc(r3)
    blr
