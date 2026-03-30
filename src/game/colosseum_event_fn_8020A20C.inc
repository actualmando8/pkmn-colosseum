    nofralloc
    cmplwi r3, 0x0
    bne @L_8020A21C
    li r3, 0x0
    blr
@L_8020A21C:
    lbz r3, 0x98(r3)
    blr
