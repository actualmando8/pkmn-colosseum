    nofralloc
    cmplwi r4, 0x0
    beq @L_800E9004
    lwz r5, 0xc(r4)
    lhz r0, 0x8(r5)
    clrlwi. r0, r0, 30
    bne @L_800E9004
    li r4, 0x0
    @L_800E9004:
    stw r4, 0x160(r3)
    blr
