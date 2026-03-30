    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD694
    li r3, -0x1
    blr
    @L_801FD694:
    clrlwi r0, r4, 24
    cmplwi r0, 0xc
    blt @L_801FD6A8
    li r3, -0x1
    blr
    @L_801FD6A8:
    clrlslwi r0, r4, 24, 1
    add r3, r3, r0
    lha r3, 0x612(r3)
    blr
