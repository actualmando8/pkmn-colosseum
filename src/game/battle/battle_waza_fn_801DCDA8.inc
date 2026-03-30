    nofralloc
    lwz r3, 0x24(r3)
    b @L_801DCDC0
@L_801DCDB0:
    lwz r0, 0x0(r3)
    cmpw r0, r4
    beqlr
    lwz r3, 0xa8(r3)
@L_801DCDC0:
    cmplwi r3, 0x0
    bne @L_801DCDB0
    blr
