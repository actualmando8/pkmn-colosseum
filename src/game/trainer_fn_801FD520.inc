    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD530
    li r3, 0x0
    blr
    @L_801FD530:
    lbz r3, 0x68f(r3)
    blr
