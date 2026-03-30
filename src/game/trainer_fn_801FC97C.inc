    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC98C
    li r3, 0x0
    blr
    @L_801FC98C:
    lhz r3, 0xc(r3)
    blr
