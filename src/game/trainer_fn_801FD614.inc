    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD624
    li r3, 0x0
    blr
    @L_801FD624:
    clrlwi r0, r4, 24
    cmplwi r0, 0x2
    blt @L_801FD638
    li r3, 0x0
    blr
    @L_801FD638:
    clrlslwi r0, r4, 24, 1
    add r3, r3, r0
    lhz r3, 0x65c(r3)
    blr
