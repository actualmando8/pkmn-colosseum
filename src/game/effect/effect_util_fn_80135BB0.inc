    nofralloc
    cmplwi r3, 0x0
    beq @L_80135BC0
    lbz r3, 0x22(r3)
    blr
@L_80135BC0:
    li r3, 0x0
    blr
