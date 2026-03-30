    nofralloc
    cmplwi r3, 0x0
    bne @L_801FCDAC
    li r3, 0x0
    blr
    @L_801FCDAC:
    lwz r3, 0x27c0(r3)
    blr
