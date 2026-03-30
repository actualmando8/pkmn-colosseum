    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E318
    li r3, 0x0
    blr
@L_8020E318:
    lbz r3, 0x10(r3)
    blr
