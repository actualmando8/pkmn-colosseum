    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD198
    li r3, 0x0
    blr
    @L_801FD198:
    lbz r3, 0x6ac(r3)
    blr
