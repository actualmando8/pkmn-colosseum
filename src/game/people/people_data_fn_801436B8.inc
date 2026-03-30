    nofralloc
    cmplwi r3, 0x0
    bne @L_801436C8
    li r3, 0x0
    blr
    @L_801436C8:
    lbz r0, 0x6(r3)
    extsb r3, r0
    blr
