    nofralloc
    cmplwi r3, 0x0
    bne @L_8020DFE8
    li r3, 0x0
    blr
@L_8020DFE8:
    lbz r3, 0x2(r3)
    blr
