    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC590
    li r3, 0x0
    blr
    @L_801FC590:
    lbz r3, 0x8(r3)
    blr
