    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD7C0
    li r3, 0x0
    blr
    @L_801FD7C0:
    lhz r3, 0x680(r3)
    blr
