    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD5A8
    li r3, 0x0
    blr
    @L_801FD5A8:
    lbz r3, 0x68a(r3)
    blr
