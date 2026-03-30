    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC940
    li r3, 0x0
    blr
    @L_801FC940:
    lhz r3, 0xa(r3)
    blr
