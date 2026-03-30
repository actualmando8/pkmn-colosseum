    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD114
    li r3, 0x0
    blr
    @L_801FD114:
    lwz r3, 0x0(r3)
    blr
