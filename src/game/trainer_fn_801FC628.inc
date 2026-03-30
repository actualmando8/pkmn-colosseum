    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC638
    li r3, 0x0
    blr
    @L_801FC638:
    lbz r3, 0x1(r3)
    blr
