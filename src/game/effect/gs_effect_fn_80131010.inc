    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    beq @L_80131058
    lis r4, -32714
    addi r4, r4, 13760
    lwz r0, 0x0(r4)
    cmplw r3, r0
    bgt @L_80131058
    subi r0, r3, 0x1
    lwz r3, 0xc(r4)
    mulli r0, r0, 0x34
    add r3, r3, r0
    lwz r0, 0x4(r3)
    cmpwi r0, -0x1
    beq @L_80131058
    b @L_8013105C
@L_80131058:
    li r3, 0x0
@L_8013105C:
    cmplwi r3, 0x0
    beq @L_80131098
    lwz r0, 0x4(r3)
    lwz r12, 0x14(r3)
    cmpwi r0, 0x0
    beq @L_80131098
    cmpwi r0, 0x1
    beq @L_80131098
    li r0, 0x1
    cmplwi r12, 0x0
    stw r0, 0x4(r3)
    beq @L_80131098
    lwz r3, 0x24(r3)
    mtctr r12
    bctrl
@L_80131098:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
