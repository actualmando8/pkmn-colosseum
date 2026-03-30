    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD5E8
    li r3, 0x0
    blr
    @L_801FD5E8:
    lhz r3, 0x660(r3)
    blr
