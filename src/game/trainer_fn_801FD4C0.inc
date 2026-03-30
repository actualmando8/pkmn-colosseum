    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD4D0
    li r3, 0x0
    blr
    @L_801FD4D0:
    lbz r3, 0x693(r3)
    blr
