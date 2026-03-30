    nofralloc
    cmplwi r3, 0x0
    bne @L_8020A1A4
    li r3, 0x0
    blr
@L_8020A1A4:
    lhz r3, 0xa6(r3)
    blr
