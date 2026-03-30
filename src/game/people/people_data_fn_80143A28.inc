    nofralloc
    cmplwi r3, 0x0
    bne @L_80143A38
    li r3, 0x0
    blr
    @L_80143A38:
    lbz r0, 0x0(r3)
    extrwi r3, r0, 4, 27
    blr
