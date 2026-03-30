    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E480
    li r3, 0x0
    blr
@L_8020E480:
    lbz r3, 0x1(r3)
    blr
