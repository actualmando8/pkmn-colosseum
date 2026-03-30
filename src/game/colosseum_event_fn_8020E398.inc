    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E3A8
    li r3, 0x0
    blr
@L_8020E3A8:
    lbz r3, 0x9(r3)
    blr
