    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC458
    li r3, 0x0
    blr
    @L_801FC458:
    lbz r3, 0xe(r3)
    blr
