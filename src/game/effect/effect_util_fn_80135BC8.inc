    nofralloc
    cmplwi r3, 0x0
    beq @L_80135BD8
    lbz r3, 0x21(r3)
    blr
@L_80135BD8:
    li r3, 0x0
    blr
