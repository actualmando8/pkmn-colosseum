    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD560
    li r3, 0x0
    blr
    @L_801FD560:
    lbz r3, 0x68d(r3)
    blr
