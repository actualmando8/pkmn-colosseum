    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    li r3, 0x0
    li r4, 0xa
    bl fn_80129280
    cmpwi r31, 0x0
    blt @L_801D1CFC
    lwz r4, -31112(r13)
    lwz r0, 0x0(r4)
    cmplw r31, r0
    blt @L_801D1D04
@L_801D1CFC:
    li r3, 0x0
    b @L_801D1D44
@L_801D1D04:
    lhz r4, 0x400(r3)
    li r5, 0x0
    mtctr r4
    cmpwi r4, 0x0
    ble @L_801D1D30
@L_801D1D18:
    lhz r0, 0x0(r3)
    cmpw r31, r0
    beq @L_801D1D30
    addi r3, r3, 0x2
    addi r5, r5, 0x1
    bdnz @L_801D1D18
@L_801D1D30:
    cmpw r5, r4
    blt @L_801D1D40
    li r3, 0x0
    b @L_801D1D44
@L_801D1D40:
    li r3, 0x1
@L_801D1D44:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
