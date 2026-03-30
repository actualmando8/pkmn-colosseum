    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    b @L_801BBEB4
    @L_801BBE7C:
    cmplwi r30, 0x0
    lwz r31, 0x8(r30)
    beq @L_801BBEB0
    lwz r4, 0x0(r30)
    mr r3, r30
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r30)
    mr r3, r30
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
    @L_801BBEB0:
    mr r30, r31
    @L_801BBEB4:
    cmplwi r30, 0x0
    bne @L_801BBE7C
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
