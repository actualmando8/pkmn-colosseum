    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD590
    li r3, 0x0
    blr
    @L_801FD590:
    lbz r3, 0x68b(r3)
    blr
