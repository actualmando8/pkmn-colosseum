    nofralloc
    cmplwi r3, 0x0
    bne @L_80135A80
    li r3, 0x0
    blr
@L_80135A80:
    lbz r3, 0x3(r3)
    blr
