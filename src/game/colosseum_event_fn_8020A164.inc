    nofralloc
    cmplwi r3, 0x0
    bne @L_8020A174
    li r3, 0x0
    blr
@L_8020A174:
    lbz r3, 0xa9(r3)
    blr
