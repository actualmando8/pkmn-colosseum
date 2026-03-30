    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E468
    li r3, 0x0
    blr
@L_8020E468:
    lbz r3, 0x2(r3)
    blr
