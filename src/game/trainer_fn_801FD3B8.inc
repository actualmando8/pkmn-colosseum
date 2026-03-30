    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD3C8
    li r3, 0x0
    blr
    @L_801FD3C8:
    lbz r3, 0x69e(r3)
    blr
