    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC2C0
    li r3, 0x0
    blr
    @L_801FC2C0:
    lbz r3, 0x18(r3)
    blr
