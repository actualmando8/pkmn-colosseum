    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD488
    li r3, 0x0
    blr
    @L_801FD488:
    lbz r3, 0x696(r3)
    blr
