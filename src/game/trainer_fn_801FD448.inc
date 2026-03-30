    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD458
    li r3, 0x0
    blr
    @L_801FD458:
    lbz r3, 0x698(r3)
    blr
