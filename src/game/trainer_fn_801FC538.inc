    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC548
    li r3, 0x0
    blr
    @L_801FC548:
    lbz r3, 0xb(r3)
    blr
