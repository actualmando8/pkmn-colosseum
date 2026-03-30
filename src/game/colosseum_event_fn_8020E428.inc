    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E438
    li r3, 0x0
    blr
@L_8020E438:
    lbz r3, 0x4(r3)
    blr
