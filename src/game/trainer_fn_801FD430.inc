    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD440
    li r3, 0x0
    blr
    @L_801FD440:
    lbz r3, 0x699(r3)
    blr
