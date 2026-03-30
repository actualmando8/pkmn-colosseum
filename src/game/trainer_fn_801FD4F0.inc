    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD500
    li r3, 0x0
    blr
    @L_801FD500:
    lbz r3, 0x691(r3)
    blr
