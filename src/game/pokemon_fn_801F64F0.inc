    nofralloc
    cmplwi r3, 0x0
    bne @L_801F6500
    li r3, 0x0
    blr
    @L_801F6500:
    addis r3, r3, 0x1
    lhz r3, -0x5b1c(r3)
    blr
