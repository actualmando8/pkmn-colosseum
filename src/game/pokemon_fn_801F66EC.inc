    nofralloc
    cmplwi r3, 0x0
    bne @L_801F66FC
    li r3, 0x0
    blr
    @L_801F66FC:
    clrlwi r0, r4, 24
    cmplwi r0, 0x2
    blt @L_801F6710
    li r3, 0x0
    blr
    @L_801F6710:
    clrlslwi r0, r4, 24, 1
    add r3, r3, r0
    lhz r3, 0x14(r3)
    blr
