    nofralloc
    cmplwi r3, 0x0
    bne @L_801FDAF4
    li r3, 0x0
    blr
    @L_801FDAF4:
    lbz r3, 0x14e(r3)
    blr
