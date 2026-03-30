    nofralloc
    cmplwi r3, 0x0
    bne @L_801F6730
    li r3, 0x0
    blr
    @L_801F6730:
    lhz r3, 0x2(r3)
    blr
