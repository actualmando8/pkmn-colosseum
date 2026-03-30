    nofralloc
    cmplwi r3, 0x0
    bne @L_801FCC18
    li r3, 0x0
    blr
    @L_801FCC18:
    clrlwi r0, r4, 24
    cmplwi r0, 0x8
    blt @L_801FCC2C
    li r3, 0x0
    blr
    @L_801FCC2C:
    clrlslwi r0, r4, 24, 1
    add r3, r3, r0
    lhz r3, 0x14(r3)
    blr
