    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD05C
    li r3, 0x0
    blr
    @L_801FD05C:
    lhz r3, 0x66a(r3)
    blr
