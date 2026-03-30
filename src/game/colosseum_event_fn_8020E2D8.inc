    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E2E8
    li r3, 0x0
    blr
@L_8020E2E8:
    lbz r3, 0x12(r3)
    blr
