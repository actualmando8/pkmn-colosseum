    nofralloc
    cmplwi r3, 0x0
    bne @L_801FCA98
    li r3, 0x0
    blr
    @L_801FCA98:
    clrlwi r0, r4, 24
    cmplwi r0, 0xc
    blt @L_801FCAAC
    li r3, 0x0
    blr
    @L_801FCAAC:
    add r3, r3, r0
    lbz r3, 0x8(r3)
    blr
