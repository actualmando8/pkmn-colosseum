    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E3C0
    li r3, 0x0
    blr
@L_8020E3C0:
    lbz r3, 0xa(r3)
    blr
