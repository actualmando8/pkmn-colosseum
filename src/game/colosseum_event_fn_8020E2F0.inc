    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E300
    li r3, 0x0
    blr
@L_8020E300:
    lbz r3, 0x11(r3)
    blr
