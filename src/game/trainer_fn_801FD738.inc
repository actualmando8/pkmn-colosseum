    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD748
    li r3, 0x0
    blr
    @L_801FD748:
    lhz r3, 0x688(r3)
    blr
