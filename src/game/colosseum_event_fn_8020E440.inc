    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E450
    li r3, 0x0
    blr
@L_8020E450:
    lbz r3, 0x3(r3)
    blr
