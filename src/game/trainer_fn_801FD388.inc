    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD398
    li r3, 0x0
    blr
    @L_801FD398:
    lha r3, 0x6a4(r3)
    blr
