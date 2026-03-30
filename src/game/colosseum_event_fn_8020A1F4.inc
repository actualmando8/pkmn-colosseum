    nofralloc
    cmplwi r3, 0x0
    bne @L_8020A204
    li r3, 0x0
    blr
@L_8020A204:
    lbz r3, 0x99(r3)
    blr
