    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD778
    li r3, 0x0
    blr
    @L_801FD778:
    lhz r3, 0x684(r3)
    blr
