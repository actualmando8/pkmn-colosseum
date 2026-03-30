    nofralloc
    cmplwi r3, 0x0
    bne @L_80135A98
    li r3, 0x0
    blr
@L_80135A98:
    lbz r3, 0x2(r3)
    blr
