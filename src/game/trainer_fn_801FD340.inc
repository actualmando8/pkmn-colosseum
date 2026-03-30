    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD350
    li r3, 0x0
    blr
    @L_801FD350:
    lhz r3, 0x6aa(r3)
    blr
