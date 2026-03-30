    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD7F0
    li r3, 0x0
    blr
    @L_801FD7F0:
    lbz r3, 0x67a(r3)
    blr
