    nofralloc
    cmplwi r3, 0x0
    bne @L_801F68E8
    li r3, 0x0
    blr
    @L_801F68E8:
    addis r3, r3, 0x1
    lhz r3, -0x5b42(r3)
    blr
