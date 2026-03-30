    nofralloc
    cmplwi r3, 0x0
    bne @L_801F6B10
    li r3, 0x0
    blr
    @L_801F6B10:
    lhz r3, 0x10(r3)
    blr
