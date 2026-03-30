    nofralloc
    cmplwi r3, 0x0
    bne @L_801F6904
    li r3, 0x0
    blr
    @L_801F6904:
    addis r3, r3, 0x1
    lhz r3, -0x5b44(r3)
    blr
