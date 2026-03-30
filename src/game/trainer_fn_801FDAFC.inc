    nofralloc
    cmplwi r3, 0x0
    bne @L_801FDB0C
    li r3, -0x1
    blr
    @L_801FDB0C:
    lha r3, 0x14c(r3)
    blr
