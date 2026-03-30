    nofralloc
    cmplwi r3, 0x0
    bne @L_80143A1C
    li r3, 0x0
    blr
    @L_80143A1C:
    lbz r0, 0x1(r3)
    extrwi r3, r0, 4, 24
    blr
