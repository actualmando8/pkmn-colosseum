    nofralloc
    cmplwi r3, 0x0
    bne @L_801FCC00
    li r3, 0x0
    blr
    @L_801FCC00:
    lwz r3, 0xc(r3)
    blr
