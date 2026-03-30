    nofralloc
    cmplwi r3, 0x0
    bne @L_80143A00
    li r3, 0x0
    blr
    @L_80143A00:
    lbz r0, 0x1(r3)
    clrlwi r3, r0, 28
    blr
