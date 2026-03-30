    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD368
    li r3, 0x0
    blr
    @L_801FD368:
    lha r3, 0x6a8(r3)
    blr
