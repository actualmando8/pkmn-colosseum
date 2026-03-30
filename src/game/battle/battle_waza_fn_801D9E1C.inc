    nofralloc
    cmplwi r3, 0x0
    beq @L_801D9E2C
    lbz r3, 0x77(r3)
    blr
@L_801D9E2C:
    li r3, 0x0
    blr
