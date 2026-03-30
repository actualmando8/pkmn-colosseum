    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC9A4
    li r3, 0x0
    blr
    @L_801FC9A4:
    lbz r3, 0x4(r3)
    blr
