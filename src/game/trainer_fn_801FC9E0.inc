    nofralloc
    cmplwi r3, 0x0
    bne @L_801FC9F0
    li r3, -0x1
    blr
    @L_801FC9F0:
    clrlwi r0, r4, 24
    cmplwi r0, 0x6
    blt @L_801FCA04
    li r3, -0x1
    blr
    @L_801FCA04:
    add r3, r3, r0
    lbz r0, 0x1c(r3)
    extsb r3, r0
    blr
