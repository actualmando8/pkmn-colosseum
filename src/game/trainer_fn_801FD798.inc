    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD7A8
    li r3, 0x0
    blr
    @L_801FD7A8:
    lbz r3, 0x67e(r3)
    blr
