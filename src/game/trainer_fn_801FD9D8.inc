    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD9E8
    li r3, 0x0
    blr
    @L_801FD9E8:
    lbz r3, 0x605(r3)
    blr
