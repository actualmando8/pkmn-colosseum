    nofralloc
    cmplwi r3, 0x0
    bne @L_801439E4
    li r3, 0x0
    blr
    @L_801439E4:
    lbz r0, 0x2(r3)
    extrwi r3, r0, 4, 24
    blr
