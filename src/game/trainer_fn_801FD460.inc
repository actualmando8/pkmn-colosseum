    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD470
    li r3, 0x0
    blr
    @L_801FD470:
    lbz r3, 0x697(r3)
    blr
