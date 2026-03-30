    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC428
    li r3, 0x0
    blr
    @L_801FC428:
    lbz r3, 0x10(r3)
    blr
