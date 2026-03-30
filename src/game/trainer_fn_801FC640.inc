    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC650
    li r3, 0x0
    blr
    @L_801FC650:
    lbz r3, 0x0(r3)
    blr
