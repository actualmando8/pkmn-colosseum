    nofralloc
    lwz r6, 0x3c(r3)
    addis r0, r6, 0x102
    cmplwi r0, 0xfefe
    beqlr
    lwz r5, 0x8(r3)
    lis r4, 0xfeff
    subi r0, r4, 0x102
    stw r6, 0x10(r5)
    stw r0, 0x3c(r3)
    blr
