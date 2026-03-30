    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD074
    li r3, 0x0
    blr
    @L_801FD074:
    lhz r3, 0x668(r3)
    blr
