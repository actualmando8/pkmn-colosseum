    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC928
    li r3, -0x1
    blr
    @L_801FC928:
    lwz r3, 0x10(r3)
    blr
