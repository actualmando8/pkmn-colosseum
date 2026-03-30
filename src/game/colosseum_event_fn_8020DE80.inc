    nofralloc
    cmplwi r3, 0x0
    bne @L_8020DE90
    li r3, 0x0
    blr
@L_8020DE90:
    lbz r3, 0x0(r3)
    blr
