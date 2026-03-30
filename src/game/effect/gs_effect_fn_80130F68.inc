    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r3, -32714
    stw r0, 0x24(r1)
    addi r3, r3, 13760
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    lwz r30, 0x10(r3)
    bl fn_800D3088
    li r31, 0x1
    mr r29, r3
    b @L_80130FEC
@L_80130F9C:
    lwz r12, 0x18(r30)
    cmplwi r12, 0x0
    beq @L_80130FE8
    lwz r0, 0x4(r30)
    cmpwi r0, 0x2
    bne @L_80130FE8
    mr r4, r29
    lwz r3, 0x24(r30)
    mtctr r12
    bctrl
    cmplwi r3, 0x0
    bne @L_80130FE8
    stw r31, 0x4(r30)
    lwz r12, 0x14(r30)
    cmplwi r12, 0x0
    beq @L_80130FE8
    lwz r3, 0x24(r30)
    mtctr r12
    bctrl
@L_80130FE8:
    lwz r30, 0x2c(r30)
@L_80130FEC:
    cmplwi r30, 0x0
    bne @L_80130F9C
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
