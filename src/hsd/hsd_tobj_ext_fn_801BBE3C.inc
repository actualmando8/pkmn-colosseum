    nofralloc
    cmplwi r3, 0x0
    beqlr
    cmplwi r4, 0x0
    bne @L_801BBE50
    blr
    @L_801BBE50:
    lwz r0, 0x8(r3)
    stw r0, 0x8(r4)
    stw r4, 0x8(r3)
    blr
