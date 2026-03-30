    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD4A0
    li r3, 0x0
    blr
    @L_801FD4A0:
    lbz r3, 0x695(r3)
    blr
