    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD760
    li r3, 0x0
    blr
    @L_801FD760:
    lha r3, 0x686(r3)
    blr
