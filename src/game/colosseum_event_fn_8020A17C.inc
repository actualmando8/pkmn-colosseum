    nofralloc
    cmplwi r3, 0x0
    bne @L_8020A18C
    li r3, 0x0
    blr
@L_8020A18C:
    lbz r3, 0xa8(r3)
    blr
