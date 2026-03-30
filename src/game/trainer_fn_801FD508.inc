    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD518
    li r3, 0x0
    blr
    @L_801FD518:
    lbz r3, 0x690(r3)
    blr
