    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD548
    li r3, 0x0
    blr
    @L_801FD548:
    lbz r3, 0x68e(r3)
    blr
