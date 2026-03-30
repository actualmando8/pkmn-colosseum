    nofralloc
    lwz r5, 0x4(r3)
    li r6, 0x4
    b @L_800EE2B8
    @L_800EE294:
    lwz r4, 0xe8(r5)
    lhz r0, 0x2(r3)
    cmplw r4, r0
    bne @L_800EE2B8
    li r3, 0x0
    li r0, -0x1
    stw r3, 0xe4(r5)
    stw r0, 0xe8(r5)
    blr
    @L_800EE2B8:
    cmpwi r6, 0x0
    subi r6, r6, 0x1
    bne @L_800EE294
    blr
