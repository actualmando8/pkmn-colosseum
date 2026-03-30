    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E330
    li r3, 0x0
    blr
@L_8020E330:
    lbz r3, 0xf(r3)
    blr
