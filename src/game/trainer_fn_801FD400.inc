    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD410
    li r3, 0x0
    blr
    @L_801FD410:
    lbz r3, 0x69b(r3)
    blr
