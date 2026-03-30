    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD9D0
    li r3, 0x0
    blr
    @L_801FD9D0:
    lbz r3, 0x606(r3)
    blr
