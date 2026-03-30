    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD790
    li r3, 0x0
    blr
    @L_801FD790:
    lha r3, 0x682(r3)
    blr
