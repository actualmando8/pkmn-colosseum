    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD970
    li r3, 0x0
    blr
    @L_801FD970:
    lbz r3, 0x60a(r3)
    blr
