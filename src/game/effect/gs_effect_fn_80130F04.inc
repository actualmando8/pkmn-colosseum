    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, -32714
    stw r0, 0x14(r1)
    addi r3, r3, 13760
    stw r31, 0xc(r1)
    lwz r31, 0x10(r3)
    b @L_80130F4C
@L_80130F24:
    lwz r12, 0x1c(r31)
    cmplwi r12, 0x0
    beq @L_80130F48
    lwz r0, 0x4(r31)
    cmpwi r0, 0x2
    bne @L_80130F48
    lwz r3, 0x24(r31)
    mtctr r12
    bctrl
@L_80130F48:
    lwz r31, 0x2c(r31)
@L_80130F4C:
    cmplwi r31, 0x0
    bne @L_80130F24
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
