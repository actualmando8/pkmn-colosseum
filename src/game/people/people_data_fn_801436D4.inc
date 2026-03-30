    nofralloc
    cmplwi r3, 0x0
    bne @L_801436E4
    li r3, 0x0
    blr
    @L_801436E4:
    lbz r0, 0x5(r3)
    extsb r3, r0
    blr
