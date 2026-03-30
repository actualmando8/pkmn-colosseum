    nofralloc
    cmplwi r3, 0x0
    bne @L_801439C8
    li r3, 0x0
    blr
    @L_801439C8:
    lbz r0, 0x2(r3)
    clrlwi r3, r0, 28
    blr
