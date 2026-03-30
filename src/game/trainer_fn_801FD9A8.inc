    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD9B8
    li r3, 0x0
    blr
    @L_801FD9B8:
    lbz r3, 0x607(r3)
    blr
