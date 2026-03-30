    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD988
    li r3, 0x0
    blr
    @L_801FD988:
    lbz r3, 0x609(r3)
    blr
