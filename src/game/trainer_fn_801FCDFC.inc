    nofralloc
    cmplwi r3, 0x0
    bne @L_801FCE0C
    li r3, 0x0
    blr
    @L_801FCE0C:
    lbz r3, 0x27b4(r3)
    blr
