    nofralloc
    cmplwi r3, 0x0
    bne @L_801FCFFC
    li r3, 0x9
    blr
    @L_801FCFFC:
    lhz r3, 0x672(r3)
    blr
