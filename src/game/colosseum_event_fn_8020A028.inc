    nofralloc
    cmplwi r3, 0x0
    bne @L_8020A038
    li r3, 0x1
    blr
@L_8020A038:
    lbz r3, 0x0(r3)
    blr
