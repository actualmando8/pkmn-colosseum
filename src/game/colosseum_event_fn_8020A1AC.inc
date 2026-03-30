    nofralloc
    cmplwi r3, 0x0
    bne @L_8020A1BC
    li r3, 0x0
    blr
@L_8020A1BC:
    lhz r3, 0xa4(r3)
    blr
