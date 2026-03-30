    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC5F0
    li r3, 0x0
    blr
    @L_801FC5F0:
    lbz r3, 0x4(r3)
    blr
