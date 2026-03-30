    nofralloc
    cmplwi r3, 0x0
    bne @L_801FDA00
    li r3, 0x0
    blr
    @L_801FDA00:
    lbz r3, 0x604(r3)
    blr
