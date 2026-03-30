    nofralloc
    cmplwi r3, 0x0
    bne @L_8020A078
    li r3, 0x0
    blr
@L_8020A078:
    lbz r3, 0x0(r3)
    blr
