    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD958
    li r3, 0x0
    blr
    @L_801FD958:
    lhz r3, 0x610(r3)
    blr
