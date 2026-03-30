    nofralloc
    cmplwi r3, 0x0
    bne @L_80135AC8
    li r3, 0x0
    blr
@L_80135AC8:
    lbz r3, 0x0(r3)
    blr
