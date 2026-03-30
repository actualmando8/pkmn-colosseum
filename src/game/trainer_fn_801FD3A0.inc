    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD3B0
    li r3, 0x0
    blr
    @L_801FD3B0:
    lwz r3, 0x6a0(r3)
    blr
