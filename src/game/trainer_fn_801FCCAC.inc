    nofralloc
    cmplwi r3, 0x0
    bne @L_801FCCBC
    li r3, 0x0
    blr
    @L_801FCCBC:
    lbz r3, 0x0(r3)
    blr
