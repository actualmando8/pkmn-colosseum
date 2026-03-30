    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD428
    li r3, 0x0
    blr
    @L_801FD428:
    lbz r3, 0x69a(r3)
    blr
