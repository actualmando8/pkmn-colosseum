    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD0CC
    li r3, 0x0
    blr
    @L_801FD0CC:
    lhz r3, 0x8(r3)
    blr
