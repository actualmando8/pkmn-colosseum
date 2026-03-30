    nofralloc
    cmplwi r3, 0x0
    bne @L_80135AB0
    li r3, 0x0
    blr
@L_80135AB0:
    lbz r3, 0x1(r3)
    blr
