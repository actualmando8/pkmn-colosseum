    nofralloc
    cmplwi r3, 0x0
    bne @L_801F6920
    li r3, 0x0
    blr
    @L_801F6920:
    addis r3, r3, 0x1
    lhz r3, -0x5b46(r3)
    blr
