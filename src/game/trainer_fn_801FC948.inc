    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC958
    li r3, -0x1
    blr
    @L_801FC958:
    lbz r0, 0x0(r3)
    extsb r3, r0
    blr
