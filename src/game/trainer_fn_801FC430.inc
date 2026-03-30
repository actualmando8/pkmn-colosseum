    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC440
    li r3, 0x0
    blr
    @L_801FC440:
    lbz r3, 0xf(r3)
    blr
