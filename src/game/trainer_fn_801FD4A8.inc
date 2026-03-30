    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD4B8
    li r3, 0x0
    blr
    @L_801FD4B8:
    lbz r3, 0x694(r3)
    blr
