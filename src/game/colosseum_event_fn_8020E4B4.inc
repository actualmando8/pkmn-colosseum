    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E4C4
    li r3, 0x0
    blr
@L_8020E4C4:
    lbz r3, 0x0(r3)
    blr
