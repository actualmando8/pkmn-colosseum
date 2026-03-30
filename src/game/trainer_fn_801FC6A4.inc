    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC6B4
    li r3, 0x0
    blr
    @L_801FC6B4:
    lbz r3, 0x6(r3)
    blr
