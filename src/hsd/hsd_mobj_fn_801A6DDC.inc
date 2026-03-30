    nofralloc
    cmplwi r3, 0x0
    beqlr
    lwz r0, 0xc(r3)
    cmplwi r0, 0x0
    bne @L_801A6DF4
    blr
    @L_801A6DF4:
    lwz r3, 0xc(r3)
    stfs f1, 0xc(r3)
    blr
