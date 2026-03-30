    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E288
    li r3, 0x0
    blr
@L_8020E288:
    lbz r3, 0x16(r3)
    blr
