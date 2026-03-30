    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r4, 0x0(r3)
    lwz r12, 0x4c(r4)
    mtctr r12
    bctrl
    lwz r4, 0x84(r31)
    li r3, 0x0
    li r5, 0x0
    bl fn_8019C128
    cmplw r3, r31
    bne @L_8019D004
    lwz r4, 0x84(r31)
    li r3, 0x0
    bl fn_8019C1B0
    @L_8019D004:
    lwz r0, 0x74(r31)
    cmplwi r0, 0x0
    beq @L_8019D018
    lwz r3, 0x74(r31)
    bl fn_801A8570
    @L_8019D018:
    lwz r0, 0x78(r31)
    cmplwi r0, 0x0
    beq @L_8019D02C
    lwz r3, 0x78(r31)
    bl fn_801A84F0
    @L_8019D02C:
    lis r4, lbl_8036C8E0@ha
    mr r3, r31
    addi r4, r4, lbl_8036C8E0@l
    lwz r4, 0x14(r4)
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
