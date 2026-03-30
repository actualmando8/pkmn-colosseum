    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC2F0
    li r3, 0x0
    blr
    @L_801FC2F0:
    lbz r3, 0x16(r3)
    blr
