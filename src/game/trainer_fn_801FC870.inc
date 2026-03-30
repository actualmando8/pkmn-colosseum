    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC880
    li r3, -0x1
    blr
    @L_801FC880:
    lha r3, 0x8(r3)
    blr
