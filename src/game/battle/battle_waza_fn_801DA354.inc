    nofralloc
    cmplwi r3, 0x0
    beq @L_801DA364
    lbz r3, 0x18(r3)
    blr
@L_801DA364:
    li r3, 0x0
    blr
