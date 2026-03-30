    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC864
    li r3, -0x1
    blr
    @L_801FC864:
    lbz r0, 0x1(r3)
    extsb r3, r0
    blr
