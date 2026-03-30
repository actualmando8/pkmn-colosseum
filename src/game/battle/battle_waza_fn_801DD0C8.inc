    nofralloc
    lwz r3, 0x68(r3)
    clrlwi r6, r4, 16
    clrlwi r0, r5, 16
    b @L_801DD0F4
@L_801DD0D8:
    lhz r4, 0x2c(r3)
    cmplw r4, r6
    bne @L_801DD0F0
    lhz r4, 0x2e(r3)
    cmplw r4, r0
    beqlr
@L_801DD0F0:
    lwz r3, 0x34(r3)
@L_801DD0F4:
    cmplwi r3, 0x0
    bne @L_801DD0D8
    blr
