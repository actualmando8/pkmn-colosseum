    nofralloc
    cmplwi r3, 0x0
    bne @L_8020A1D4
    li r3, 0x0
    blr
@L_8020A1D4:
    lwz r3, 0xa0(r3)
    blr
