    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC6CC
    li r3, 0x0
    blr
    @L_801FC6CC:
    lbz r3, 0x5(r3)
    blr
