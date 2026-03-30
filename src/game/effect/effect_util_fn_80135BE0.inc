    nofralloc
    cmplwi r3, 0x0
    beq @L_80135BF0
    lbz r3, 0x20(r3)
    blr
@L_80135BF0:
    li r3, 0x0
    blr
