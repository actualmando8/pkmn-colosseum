    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E270
    li r3, 0x0
    blr
@L_8020E270:
    lbz r3, 0x17(r3)
    blr
