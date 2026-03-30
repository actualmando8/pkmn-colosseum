    nofralloc
    cmplwi r3, 0x0
    bne @L_801FCAC8
    li r3, 0x0
    blr
    @L_801FCAC8:
    lwz r3, 0x0(r3)
    blr
