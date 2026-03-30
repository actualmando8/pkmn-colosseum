    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD380
    li r3, 0x0
    blr
    @L_801FD380:
    lhz r3, 0x6a6(r3)
    blr
