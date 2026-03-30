    nofralloc
    cmplwi r3, 0x0
    beqlr
    lwz r0, 0x14(r3)
    cmplwi r0, 0x0
    bne @L_801A3254
    blr
@L_801A3254:
    addi r5, r3, 0x14
    b @L_801A3290
@L_801A325C:
    lwz r4, 0x0(r5)
    lbz r0, 0x13(r4)
    cmplwi r0, 0xc
    bne @L_801A328C
    lwz r4, 0x0(r5)
    lwz r6, 0x0(r5)
    lwz r0, 0x0(r4)
    stw r0, 0x0(r5)
    lwz r0, 0x14(r3)
    stw r0, 0x0(r6)
    stw r6, 0x14(r3)
    blr
@L_801A328C:
    lwz r5, 0x0(r5)
@L_801A3290:
    lwz r0, 0x0(r5)
    cmplwi r0, 0x0
    bne @L_801A325C
    blr
