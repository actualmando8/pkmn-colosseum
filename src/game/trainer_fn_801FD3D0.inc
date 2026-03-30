    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD3E0
    li r3, 0x0
    blr
    @L_801FD3E0:
    lbz r3, 0x69d(r3)
    blr
