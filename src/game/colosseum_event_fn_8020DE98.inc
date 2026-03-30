    nofralloc
    cmplwi r3, 0x0
    bne @L_8020DEA8
    li r3, 0x0
    blr
@L_8020DEA8:
    lwz r3, 0x8(r3)
    blr
