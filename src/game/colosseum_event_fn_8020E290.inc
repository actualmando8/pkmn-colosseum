    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E2A0
    li r3, 0x0
    blr
@L_8020E2A0:
    lbz r3, 0x15(r3)
    blr
