    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD3F8
    li r3, 0x0
    blr
    @L_801FD3F8:
    lbz r3, 0x69c(r3)
    blr
