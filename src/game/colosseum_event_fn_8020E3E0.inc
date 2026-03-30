    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E3F0
    li r3, 0x0
    blr
@L_8020E3F0:
    lbz r3, 0x7(r3)
    blr
