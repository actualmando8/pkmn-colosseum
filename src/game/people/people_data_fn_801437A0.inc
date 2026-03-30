    nofralloc
    cmplwi r3, 0x0
    bne @L_801437B0
    li r3, 0x0
    blr
    @L_801437B0:
    lbz r3, 0xb(r3)
    blr
