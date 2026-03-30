    nofralloc
    cmplwi r3, 0x0
    bne @L_8020A020
    li r3, 0x1
    blr
@L_8020A020:
    lbz r3, 0x1(r3)
    blr
