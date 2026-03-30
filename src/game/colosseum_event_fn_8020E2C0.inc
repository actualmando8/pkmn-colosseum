    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E2D0
    li r3, 0x0
    blr
@L_8020E2D0:
    lbz r3, 0x13(r3)
    blr
