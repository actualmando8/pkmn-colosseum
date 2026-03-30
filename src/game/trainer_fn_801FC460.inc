    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC470
    li r3, 0x0
    blr
    @L_801FC470:
    lbz r3, 0xd(r3)
    blr
