    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E420
    li r3, 0x0
    blr
@L_8020E420:
    lbz r3, 0x5(r3)
    blr
