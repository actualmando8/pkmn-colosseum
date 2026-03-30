    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E2B8
    li r3, 0x0
    blr
@L_8020E2B8:
    lbz r3, 0x14(r3)
    blr
