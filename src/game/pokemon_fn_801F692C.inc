    nofralloc
    cmplwi r3, 0x0
    bne @L_801F693C
    li r3, 0x0
    blr
    @L_801F693C:
    addis r3, r3, 0x1
    lhz r3, -0x5b48(r3)
    blr
