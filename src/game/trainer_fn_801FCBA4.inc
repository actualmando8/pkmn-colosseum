    nofralloc
    cmplwi r3, 0x0
    bne @L_801FCBB4
    li r3, 0x0
    blr
    @L_801FCBB4:
    lwz r3, 0x10(r3)
    blr
