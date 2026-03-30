    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC848
    li r3, -0x1
    blr
    @L_801FC848:
    lbz r0, 0x2(r3)
    extsb r3, r0
    blr
