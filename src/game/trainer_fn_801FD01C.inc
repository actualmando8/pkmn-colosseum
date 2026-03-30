    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD02C
    li r3, 0x0
    blr
    @L_801FD02C:
    lhz r3, 0x66e(r3)
    blr
