    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, lbl_8036C8E0@ha
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    addi r3, r4, lbl_8036C8E0@l
    lwz r4, 0x14(r3)
    mr r3, r31
    lwz r12, 0x2c(r4)
    mtctr r12
    bctrl
    cmpwi r3, 0x0
    blt @L_8019D5FC
    li r0, 0x40
    lfs f0, lbl_8047DB30(r2)
    stw r0, 0x14(r31)
    li r3, 0x0
    lfs f1, lbl_8047DB30(r2)
    stfs f0, 0x2c(r31)
    lfs f0, lbl_8047DB30(r2)
    stfs f1, 0x30(r31)
    stfs f0, 0x34(r31)
    @L_8019D5FC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
