    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD578
    li r3, 0x0
    blr
    @L_801FD578:
    lbz r3, 0x68c(r3)
    blr
